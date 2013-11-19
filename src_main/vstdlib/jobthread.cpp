//========== Copyright © 2005, Valve Corporation, All rights reserved. ========
//
// Purpose:
//
//=============================================================================

#if defined( _WIN32 ) && !defined( _X360 )
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif
#include "tier0/dbg.h"
#include "tier0/tslist.h"
#include "tier0/icommandline.h"
#include "vstdlib/jobthread.h"
#include "vstdlib/random.h"
#include "tier1/functors.h"
#include "tier1/fmtstr.h"
#include "tier1/utlvector.h"
#include "tier1/generichash.h"

#if defined( _X360 )
#include "xbox/xbox_win32stubs.h"
#endif

#include "tier0/memdbgon.h"

#ifdef _WIN32

//-----------------------------------------------------------------------------

inline void ServiceJobAndRelease( CJob *pJob, int iThread = -1 )
{
	// TryLock() would only fail if another thread has entered
	// Execute() or Abort()
	if ( !pJob->IsFinished() && pJob->TryLock() )
	{
		// ...service the request
		pJob->SetServiceThread( iThread );
		pJob->Execute();
		pJob->Unlock();
	}
	pJob->Release();
}

//-----------------------------------------------------------------------------

#pragma pack(push)
#pragma pack(8)
class ALIGN16 CJobQueue
{
public:
	CJobQueue() :
		m_nItems( 0 ),
		m_nMaxItems( INT_MAX )
	{
	}

	int Count()
	{
		return m_nItems;
	}

	int Count( JobPriority_t priority )
	{
		return m_queues[priority].Count();
	}


	CJob *PrePush()
	{
		if ( m_nItems >= m_nMaxItems )
		{
			CJob *pOverflowJob;
			if ( Pop( &pOverflowJob ) )
			{
				return pOverflowJob;
			}
		}
		return NULL;
	}

	int Push( CJob *pJob, int iThread = -1 )
	{
		pJob->AddRef();

		CJob *pOverflowJob;
		int nOverflow = 0;
		while ( ( pOverflowJob = PrePush() ) != NULL )
		{
			ServiceJobAndRelease( pJob );
			nOverflow++;
		}

		m_queues[pJob->GetPriority()].PushItem( pJob );

		m_mutex.Lock();
		if ( ++m_nItems == 1 )
		{
			m_JobAvailableEvent.Set();
		}
		m_mutex.Unlock();

		return nOverflow;
	}

	bool Pop( CJob **ppJob )
	{
		m_mutex.Lock();
		if ( !m_nItems )
		{
			m_mutex.Unlock();
			*ppJob = NULL;
			return false;
		}
		if ( --m_nItems == 0 )
		{
			m_JobAvailableEvent.Reset();
		}
		m_mutex.Unlock();

		for ( int i = JP_HIGH; i >= 0; --i )
		{
			if ( m_queues[i].PopItem( ppJob ) )
			{
				return true;
			}
		}


		AssertMsg( 0, "Expected at least one queue item" );
		*ppJob = NULL;
		return false;
	}

	HANDLE GetEventHandle()
	{
		return m_JobAvailableEvent;
	}

	void Flush()
	{
		// Only safe to call when system is suspended
		m_mutex.Lock();
		m_nItems = 0;
		m_JobAvailableEvent.Reset();
		CJob *pJob;
		for ( int i = JP_HIGH; i >= 0; --i )
		{
			while ( m_queues[i].PopItem( &pJob ) )
			{
				pJob->Abort();
				pJob->Release();
			}
		}
		m_mutex.Unlock();
	}

private:
	CTSQueue<CJob *>	m_queues[JP_HIGH + 1];
	int					m_nItems;
	int					m_nMaxItems;
	CThreadFastMutex	m_mutex;
	CThreadManualEvent	m_JobAvailableEvent;

};

#pragma pack(pop)

//-----------------------------------------------------------------------------
//
// CThreadPool
//
//-----------------------------------------------------------------------------

class CThreadPool : public CRefCounted1<IThreadPool, CRefCountServiceMT>
{
public:
	CThreadPool();
	~CThreadPool();

	//-----------------------------------------------------
	// Thread functions
	//-----------------------------------------------------
	bool Start( const ThreadPoolStartParams_t &startParams = ThreadPoolStartParams_t() ) { return Start( startParams, NULL ); }
	bool Start( const ThreadPoolStartParams_t &startParams, const char *pszNameOverride );
	bool Stop( int timeout = TT_INFINITE );
	void Distribute( bool bDistribute = true, int *pAffinityTable = NULL );

	//-----------------------------------------------------
	// Functions for any thread
	//-----------------------------------------------------
	unsigned GetJobCount()							{ return m_nJobs; }
	int NumThreads();
	int NumIdleThreads();

	//-----------------------------------------------------
	// Pause/resume processing jobs
	//-----------------------------------------------------
	int SuspendExecution();
	int ResumeExecution();

	//-----------------------------------------------------
	// Offer the current thread to the pool
	//-----------------------------------------------------
	virtual int YieldWait( CThreadEvent *pEvents, int nEvents, bool bWaitAll = true, unsigned timeout = TT_INFINITE );
	virtual int YieldWait( CJob **, int nJobs, bool bWaitAll = true, unsigned timeout = TT_INFINITE );
	void Yield( unsigned timeout );

	//-----------------------------------------------------
	// Add a native job to the queue (master thread)
	//-----------------------------------------------------
	void AddJob( CJob * );
	void InsertJobInQueue( CJob * );

	//-----------------------------------------------------
	// Add an function object to the queue (master thread)
	//-----------------------------------------------------
	void AddFunctorInternal( CFunctor *, CJob ** = NULL, const char *pszDescription = NULL, unsigned flags = 0 );

	//-----------------------------------------------------
	// Remove a job from the queue (master thread)
	//-----------------------------------------------------
	virtual void ChangePriority( CJob *p, JobPriority_t priority );

	//-----------------------------------------------------
	// Bulk job manipulation (blocking)
	//-----------------------------------------------------
	int ExecuteToPriority( JobPriority_t toPriority, JobFilter_t pfnFilter = NULL  );
	int AbortAll();

	virtual void Reserved1() {}

	void WaitForIdle( bool bAll = true );

private:
	enum
	{
		IO_STACKSIZE = ( 64 * 1024 ),
		COMPUTATION_STACKSIZE = 0,
	};

	//-----------------------------------------------------
	//
	//-----------------------------------------------------
	CJob *PeekJob();
	CJob *GetDummyJob();

	//-----------------------------------------------------
	// Thread functions
	//-----------------------------------------------------
	int Run();

private:
	friend class CJobThread;

	CJobQueue				m_SharedQueue;
	CInterlockedInt			m_nIdleThreads;
	CUtlVector<CJobThread *> m_Threads;
	CUtlVector<HANDLE>		m_IdleEvents;

	CThreadFastMutex		m_SuspendMutex;
	int						m_nSuspend;
	CInterlockedInt			m_nJobs;
};

//-----------------------------------------------------------------------------

JOB_INTERFACE IThreadPool *CreateThreadPool()
{
	return new CThreadPool;
}

JOB_INTERFACE void DestroyThreadPool( IThreadPool *pPool )
{
	delete static_cast<CThreadPool*>( pPool );
}

//-----------------------------------------------------------------------------

class CGlobalThreadPool : public CThreadPool
{
public:
	virtual bool Start( const ThreadPoolStartParams_t &startParamsIn )
	{
		int nThreads = ( CommandLine()->ParmValue( "-threads", -1 ) - 1 );
		ThreadPoolStartParams_t startParams = startParamsIn;

		if ( nThreads >= 0 )
		{
			startParams.nThreads = nThreads;
		}
		return CThreadPool::Start( startParams );
	}

	virtual bool OnFinalRelease()
	{
		AssertMsg( 0, "Releasing global thread pool object!" );
		return false;
	}
};

//-----------------------------------------------------------------------------

class CJobThread : public CWorkerThread
{
public:
	CJobThread( CThreadPool *pOwner, int iThread ) : 
		m_SharedQueue( pOwner->m_SharedQueue ),
		m_pOwner( pOwner ),
		m_iThread( iThread )
	{
	}

	CThreadEvent &GetIdleEvent()
	{
		return m_IdleEvent;
	}

	CJobQueue &AccessDirectQueue()
	{ 
		return m_DirectQueue;
	}

private:
	unsigned Wait( int nHandles, HANDLE *pHandles )
	{
		unsigned waitResult;
#ifdef _DEBUG
		while ( ( waitResult = WaitForMultipleObjects( nHandles, pHandles, FALSE, 10 ) ) == WAIT_TIMEOUT )
		{
			waitResult = waitResult; // break here
		}
#else
		waitResult = WaitForMultipleObjects( nHandles, pHandles, FALSE, INFINITE );;
#endif
		return waitResult;
	}

	int Run()
	{
		enum Event_t
		{
			CALL_FROM_MASTER,
			SHARED_QUEUE,
			DIRECT_QUEUE,

			NUM_EVENTS
		};

		// Wait for either a call from the master thread, or an item in the queue...
		unsigned waitResult;
		bool	 bExit = false;
		HANDLE	 waitHandles[NUM_EVENTS];

		waitHandles[CALL_FROM_MASTER]	= GetCallHandle();
		waitHandles[SHARED_QUEUE]		= m_SharedQueue.GetEventHandle();
		waitHandles[DIRECT_QUEUE] 		= m_DirectQueue.GetEventHandle();

		m_pOwner->m_nIdleThreads++;
		m_IdleEvent.Set();
		while (!bExit &&
			( waitResult = Wait( ARRAYSIZE(waitHandles), waitHandles ) ) != WAIT_FAILED )
		{
			if ( PeekCall() )
			{
				switch ( GetCallParam() )
				{
				case TPM_EXIT:
					Reply( true );
					bExit = TRUE;
					break;

				case TPM_SUSPEND:
					Reply( true );
					Suspend();
					break;

				default:
					AssertMsg( 0, "Unknown call to thread" );
					Reply( false );
					break;
				}
			}
			else
			{
				CJob *pJob;
				bool bTookJob = false;
				do
				{
					if ( !m_DirectQueue.Pop( &pJob) )
					{
						if ( !m_SharedQueue.Pop( &pJob ) )
						{
							// Nothing to process, return to wait state
							break;
						}
					}
					if ( !bTookJob )
					{
						m_IdleEvent.Reset();
						m_pOwner->m_nIdleThreads--;
						bTookJob = true;
					}
					ServiceJobAndRelease( pJob, m_iThread );
					m_pOwner->m_nJobs--;
				} while ( !PeekCall() );

				if ( bTookJob )
				{
					m_pOwner->m_nIdleThreads++;
					m_IdleEvent.Set();
				}
			}
		}
		m_pOwner->m_nIdleThreads--;
		m_IdleEvent.Reset();
		return 0;
	}

	CJobQueue			m_DirectQueue;
	CJobQueue &			m_SharedQueue;
	CThreadPool *		m_pOwner;
	CThreadManualEvent	m_IdleEvent;
	int					m_iThread;
};

//-----------------------------------------------------------------------------

CGlobalThreadPool g_ThreadPool;
IThreadPool *g_pThreadPool = &g_ThreadPool;

//-----------------------------------------------------------------------------
//
// CThreadPool
//
//-----------------------------------------------------------------------------

CThreadPool::CThreadPool() :
	m_nIdleThreads( 0 ),
	m_nJobs( 0 ),
	m_nSuspend( 0 )
{
}

//---------------------------------------------------------

CThreadPool::~CThreadPool()
{
}

//---------------------------------------------------------
// 
//---------------------------------------------------------
int CThreadPool::NumThreads()
{
	return m_Threads.Count();
}

//---------------------------------------------------------
// 
//---------------------------------------------------------
int CThreadPool::NumIdleThreads()
{
	return m_nIdleThreads;
}

//---------------------------------------------------------
// Pause/resume processing jobs
//---------------------------------------------------------
int CThreadPool::SuspendExecution()
{
	AUTO_LOCK( m_SuspendMutex );

	// If not already suspended
	if ( m_nSuspend == 0 )
	{
		// Make sure state is correct
#ifdef _DEBUG
		int curCount = m_Threads[0]->Suspend();
		m_Threads[0]->Resume();
		Assert( curCount == 0 );
#endif
		int i;
		for ( i = 0; i < m_Threads.Count(); i++ )
		{
			m_Threads[i]->CallWorker( TPM_SUSPEND, 0 );
		}

		for ( i = 0; i < m_Threads.Count(); i++ )
		{
			m_Threads[i]->WaitForReply();
		}

		// Because worker must signal before suspending, we could reach
		// here with the thread not actually suspended
		for ( i = 0; i < m_Threads.Count(); i++ )
		{
			while ( m_Threads[i]->Suspend() == 0 )
			{
				m_Threads[i]->Resume();
				ThreadSleep();
			}
			m_Threads[i]->Resume();
		}

#ifdef _DEBUG
		curCount = m_Threads[0]->Suspend();
		m_Threads[0]->Resume();
		Assert( curCount > 0 );
#endif
	}

	return m_nSuspend++;
}

//---------------------------------------------------------

int CThreadPool::ResumeExecution()
{
	AUTO_LOCK( m_SuspendMutex );
	AssertMsg( m_nSuspend >= 1, "Attempted resume when not suspended");
	int result = m_nSuspend--;
	if (m_nSuspend == 0 )
	{
		for ( int i = 0; i < m_Threads.Count(); i++ )
		{
			m_Threads[i]->Resume();
		}
	}
	return result;
}

//---------------------------------------------------------

void CThreadPool::WaitForIdle( bool bAll )
{
	WaitForMultipleObjects( m_IdleEvents.Count(), m_IdleEvents.Base(), bAll, 60000 );
}

//---------------------------------------------------------

int CThreadPool::YieldWait( CThreadEvent *pEvents, int nEvents, bool bWaitAll, unsigned timeout )
{
	Assert( timeout == TT_INFINITE ); // unimplemented
	int result;
	CJob *pJob;
	while ( ( result = ThreadWaitForEvents( nEvents, pEvents, bWaitAll, 0 ) ) == WAIT_TIMEOUT )
	{
		if ( m_SharedQueue.Pop( &pJob ) )
		{
			ServiceJobAndRelease( pJob );
			m_nJobs--;
		}
		else
		{
			ThreadPause();
			ThreadSleep( 0 );
		}
	}
	return result;
}

//---------------------------------------------------------

int CThreadPool::YieldWait( CJob **ppJobs, int nJobs, bool bWaitAll, unsigned timeout )
{
	CUtlVectorFixed<HANDLE, 64> handles;
	if ( nJobs > handles.NumAllocated() - 2 )
	{
		return TW_FAILED;
	}

	for ( int i = 0; i < nJobs; i++ )
	{
		handles.AddToTail( *(ppJobs[i]->AccessEvent()) );
	}

	return YieldWait( (CThreadEvent *)handles.Base(), handles.Count(), bWaitAll, timeout);
}

//---------------------------------------------------------

void CThreadPool::Yield( unsigned timeout )
{
	// @MULTICORE (toml 10/24/2006): not implemented
	Assert( ThreadInMainThread() );
	if ( !ThreadInMainThread() )
	{
		ThreadSleep( timeout );
		return;
	}
	ThreadSleep( timeout );
}

//---------------------------------------------------------
// Add a job to the queue
//---------------------------------------------------------

void CThreadPool::AddJob( CJob *pJob )
{
	if ( !pJob )
	{
		return;
	}

	if ( pJob->m_ThreadPoolData != JOB_NO_DATA )
	{
		Warning( "Cannot add a thread job already committed to another thread pool\n" );
		return;
	}

	if ( m_Threads.Count() == 0 )
	{
		pJob->Execute();
		return;
	}

	int flags = pJob->GetFlags();

	if ( ( flags & ( JF_IO | JF_QUEUE ) ) == 0 /* @TBD && !m_queue.Count() */ )
	{
		if ( !NumIdleThreads() )
		{
			pJob->Execute();
			return;
		}
		pJob->SetPriority( JP_HIGH );
	}

	if ( !pJob->CanExecute() )
	{
		// Already handled
		ExecuteOnce( Warning( "Attempted to add job to job queue that has already been completed\n" ) );
		return;
	}

	pJob->m_pThreadPool = this;
	pJob->m_status = JOB_STATUS_PENDING;
	InsertJobInQueue( pJob );
	++m_nJobs;
}

//---------------------------------------------------------
//
//---------------------------------------------------------

void CThreadPool::InsertJobInQueue( CJob *pJob )
{
	CJobQueue *pQueue;

	if ( !( pJob->GetFlags() & JF_SERIAL ) )
	{
		int iThread = pJob->GetServiceThread();
		if ( iThread == -1 || !m_Threads.IsValidIndex( iThread ) )
		{
			pQueue = &m_SharedQueue;
		}
		else
		{
			pQueue = &(m_Threads[iThread]->AccessDirectQueue());
		}
	}
	else
	{
		pQueue = &(m_Threads[0]->AccessDirectQueue());
	}

	m_nJobs -= pQueue->Push( pJob );
}

//---------------------------------------------------------
// Add an function object to the queue (master thread)
//---------------------------------------------------------

void CThreadPool::AddFunctorInternal( CFunctor *pFunctor, CJob **ppJob, const char *pszDescription, unsigned flags )
{
	// Note: assumes caller has handled refcount
	CJob *pJob = new CFunctorJob( pFunctor, pszDescription );

	pJob->SetFlags( flags );

	AddJob( pJob );

	if ( ppJob )
	{
		*ppJob = pJob;
	}
	else
	{
		pJob->Release();
	}
}

//---------------------------------------------------------
// Remove a job from the queue
//---------------------------------------------------------

void CThreadPool::ChangePriority( CJob *pJob, JobPriority_t priority )
{
	// Right now, only support upping the priority
	if ( pJob->GetPriority() < priority )
	{
		pJob->SetPriority( priority );
		m_SharedQueue.Push( pJob );
	}
	else
	{
		ExecuteOnce( if ( pJob->GetPriority() != priority ) DevMsg( "CThreadPool::RemoveJob not implemented right now" ) );
	}

}

//---------------------------------------------------------
// Execute to a specified priority
//---------------------------------------------------------

int CThreadPool::ExecuteToPriority( JobPriority_t iToPriority, JobFilter_t pfnFilter )
{
	SuspendExecution();

	CJob *pJob;
	int nExecuted = 0;
	int i;
	int nJobsTotal = GetJobCount();
	CUtlVector<CJob *> jobsToPutBack;

	for ( int iCurPriority = JP_HIGH; iCurPriority >= iToPriority; --iCurPriority )
	{
		for ( i = 0; i < m_Threads.Count(); i++ )
		{
			CJobQueue &queue = m_Threads[i]->AccessDirectQueue();
			while ( queue.Count( (JobPriority_t)iCurPriority ) )
			{
				queue.Pop( &pJob );
				if ( pfnFilter && !(*pfnFilter)( pJob ) )
				{
					if ( pJob->CanExecute() )
					{
						jobsToPutBack.EnsureCapacity( nJobsTotal );
						jobsToPutBack.AddToTail( pJob );
					}
					else
					{
						m_nJobs--;
						pJob->Release(); // an already serviced job in queue, may as well ditch it (as in, main thread probably force executed)
					}
					continue;
				}
				ServiceJobAndRelease( pJob );
				m_nJobs--;
				nExecuted++;
			}

		}

		while ( m_SharedQueue.Count( (JobPriority_t)iCurPriority ) )
		{
			m_SharedQueue.Pop( &pJob );
			if ( pfnFilter && !(*pfnFilter)( pJob ) )
			{
				if ( pJob->CanExecute() )
				{
					jobsToPutBack.EnsureCapacity( nJobsTotal );
					jobsToPutBack.AddToTail( pJob );
				}
				else
				{
					m_nJobs--;
					pJob->Release(); // see above
				}
				continue;
			}

			ServiceJobAndRelease( pJob );
			m_nJobs--;
			nExecuted++;
		}
	}

	for ( i = 0; i < jobsToPutBack.Count(); i++ )
	{
		InsertJobInQueue( jobsToPutBack[i] );
		jobsToPutBack[i]->Release();
	}

	ResumeExecution();

	return nExecuted;
}

//---------------------------------------------------------
//
//---------------------------------------------------------

int CThreadPool::AbortAll()
{
	SuspendExecution();
	CJob *pJob;

	int iAborted = 0;
	while ( m_SharedQueue.Pop( &pJob ) )
	{
		pJob->Abort();
		pJob->Release();
		iAborted++;
	}

	for ( int i = 0; i < m_Threads.Count(); i++ )
	{
		CJobQueue &queue = m_Threads[i]->AccessDirectQueue();
		while ( queue.Pop( &pJob ) )
		{
			pJob->Abort();
			pJob->Release();
			iAborted++;
		}

	}

	m_nJobs = 0;

	ResumeExecution();

	return iAborted;
}

//---------------------------------------------------------
// CThreadPool thread functions
//---------------------------------------------------------

bool CThreadPool::Start( const ThreadPoolStartParams_t &startParams, const char *pszName )
{
	if ( IsLinux() )
		return false;

	int nThreads = startParams.nThreads;

	if ( nThreads < 0 )
	{
		const CPUInformation &ci = GetCPUInformation();
		if ( startParams.bIOThreads )
		{
			nThreads = ci.m_nLogicalProcessors;
		}
		else
		{
			nThreads = ( ci.m_nLogicalProcessors / (( ci.m_bHT ) ? 2 : 1) ) - 1; // One per
			if ( IsPC() )
			{
				if ( nThreads > 3 )
				{
					DevMsg( "Defaulting to limit of 3 worker threads, use -threads on command line if want more\n" ); // Current >4 processor configs don't really work so well, probably due to cache issues? (toml 7/12/2007)
					nThreads = 3;
				}
			}
		}
	}

	if ( nThreads <= 0 )
	{
		return true;
	}

	int nStackSize = startParams.nStackSize;

	if ( nStackSize < 0 )
	{
		if ( startParams.bIOThreads )
		{
			nStackSize = IO_STACKSIZE;
		}
		else
		{
			nStackSize = COMPUTATION_STACKSIZE;
		}
	}

	int priority = startParams.iThreadPriority;

	if ( priority == SHRT_MIN )
	{
		if ( startParams.bIOThreads )
		{
			priority = THREAD_PRIORITY_HIGHEST;
		}
		else
		{
			priority = ThreadGetPriority();
		}
	}

	bool bDistribute;
	if ( startParams.fDistribute != TRS_NONE )
	{
		bDistribute = ( startParams.fDistribute == TRS_TRUE );
	}
	else
	{
		bDistribute = !startParams.bIOThreads;
	}

	//--------------------------------------------------------

	m_Threads.EnsureCapacity( nThreads );
	m_IdleEvents.EnsureCapacity( nThreads );

	if ( !pszName )
	{
		pszName = ( startParams.bIOThreads ) ? "IOJob" : "CmpJob";
	}
	while ( nThreads-- )
	{
		int iThread = m_Threads.AddToTail();
		m_IdleEvents.AddToTail();
		m_Threads[iThread] = new CJobThread( this, iThread );
		m_IdleEvents[iThread] = m_Threads[iThread]->GetIdleEvent();
		m_Threads[iThread]->Start( nStackSize );
		m_Threads[iThread]->GetIdleEvent().Wait();
		ThreadSetDebugName( m_Threads[iThread]->GetThreadId(), CFmtStr( "%s%d", pszName, iThread ) );
		ThreadSetPriority( (ThreadHandle_t)m_Threads[iThread]->GetThreadHandle(), priority );
	}

	Distribute( bDistribute, startParams.bUseAffinityTable ? (int *)startParams.iAffinityTable : NULL );

	return true;
}

//---------------------------------------------------------

void CThreadPool::Distribute( bool bDistribute, int *pAffinityTable )
{
	if ( bDistribute )
	{
		const CPUInformation &ci = GetCPUInformation();
		int nHwThreadsPer = (( ci.m_bHT ) ? 2 : 1);
		if ( ci.m_nLogicalProcessors > 1 )
		{
			if ( !pAffinityTable )
			{
				// no affinity table, distribution is cycled across all available
				int iProc = 0;
				for ( int i = 0; i < m_Threads.Count(); i++ )
				{
					iProc += nHwThreadsPer;
					if ( iProc >= ci.m_nLogicalProcessors )
					{
						iProc %= ci.m_nLogicalProcessors;
						if ( nHwThreadsPer > 1 )
						{
							iProc = ( iProc + 1 ) % nHwThreadsPer;
						}
					}
					ThreadSetAffinity( (ThreadHandle_t)m_Threads[i]->GetThreadHandle(), 1 << iProc );
				}
			}
			else
			{
				// distribution is from affinity table
				for ( int i = 0; i < m_Threads.Count(); i++ )
				{
					ThreadSetAffinity( (ThreadHandle_t)m_Threads[i]->GetThreadHandle(), pAffinityTable[i] );
				}
			}
		}
	}
	else
	{
		DWORD dwProcessAffinity, dwSystemAffinity;
		if ( GetProcessAffinityMask( GetCurrentProcess(), &dwProcessAffinity, &dwSystemAffinity ) )
		{
			for ( int i = 0; i < m_Threads.Count(); i++ )
			{
				ThreadSetAffinity( (ThreadHandle_t)m_Threads[i]->GetThreadHandle(), dwProcessAffinity );
			}
		}
	}
}

//---------------------------------------------------------

bool CThreadPool::Stop( int timeout )
{
	for ( int i = 0; i < m_Threads.Count(); i++ )
	{
		m_Threads[i]->CallWorker( TPM_EXIT );
	}

	for ( int i = 0; i < m_Threads.Count(); ++i )
	{
		while( m_Threads[i]->GetThreadHandle() )
		{
			ThreadSleep( 0 );
		}
	}

	m_nJobs = 0;
	m_SharedQueue.Flush();
	m_nIdleThreads = 0;
	m_Threads.RemoveAll();
	m_IdleEvents.RemoveAll();

	return true;
}

//---------------------------------------------------------

CJob *CThreadPool::GetDummyJob()
{
	class CDummyJob : public CJob
	{
	public:
		CDummyJob()
		{
			Execute();
		}

		virtual JobStatus_t DoExecute() { return JOB_OK; }
	};

	static CDummyJob dummyJob;

	dummyJob.AddRef();
	return &dummyJob;
}

//-----------------------------------------------------------------------------

#elif defined( _LINUX )

IThreadPool *g_pThreadPool = NULL;

JOB_INTERFACE IThreadPool *CreateThreadPool()
{
	// No threadpool implementation on Linux yet. We -should- be able to use 99% of the Windows implementation here
	// because it mostly relies on threadtools.h. The main difference is that it requires WaitForMultipleObjects,
	// which we don't (YET) have an equivalent for in threadtools.h.
	Assert( false );
	return NULL;
}

JOB_INTERFACE void DestroyThreadPool( IThreadPool *pPool )
{
}

#else

#error( "No threadpool implementation for platform" )

#endif

#if !defined( _LINUX )
namespace ThreadPoolTest 
{
int g_iSleep;

CThreadEvent g_done;
int g_nTotalToComplete;
CThreadPool *g_pTestThreadPool;

class CCountJob : public CJob
{
public:
	virtual JobStatus_t DoExecute()
	{
		m_nCount++;
		ThreadPause();
		if ( g_iSleep >= 0)
			ThreadSleep( g_iSleep );
		if ( bDoWork )
		{
			byte pMemory[1024];
			int i;
			for ( i = 0; i < 1024; i++ )
			{
				pMemory[i] = rand();
			}
			for ( i = 0; i < 50; i++ )
			{
				sqrt( (float)HashBlock( pMemory, 1024 ) + HashBlock( pMemory, 1024 ) + 10.0 );
			}
			bDoWork = false;
		}
		if ( m_nCount == g_nTotalToComplete )
			g_done.Set();
		return 0;
	}

	static CInterlockedInt m_nCount;
	bool bDoWork;
};
CInterlockedInt CCountJob::m_nCount;
int g_nTotalAtFinish;

void Test( bool bDistribute, bool bSleep = true, bool bFinishExecute = false, bool bDoWork = false )
{
	for ( int bInterleavePushPop = 0; bInterleavePushPop < 2; bInterleavePushPop++ )
	{
		for ( g_iSleep = -10; g_iSleep <= 10; g_iSleep += 10 )
		{
			Msg( "ThreadPoolTest:         Testing! Sleep %d, interleave %d \n", g_iSleep, bInterleavePushPop );
			int nMaxThreads = ( IsX360() ) ? 6 : 8;
			int nIncrement = ( IsX360() ) ? 1 : 2;
			for ( int i = 1; i <= nMaxThreads; i += nIncrement )
			{
				CCountJob::m_nCount = 0;
				g_nTotalAtFinish = 0;
				ThreadPoolStartParams_t params;
				params.nThreads = i;
				params.fDistribute = ( bDistribute) ? TRS_TRUE : TRS_FALSE;
				g_pTestThreadPool->Start( params, "Tst" );
				if ( !bInterleavePushPop )
				{
					g_pTestThreadPool->SuspendExecution();
				}

				CCountJob jobs[4000];
				g_nTotalToComplete = ARRAYSIZE(jobs);

				CFastTimer timer, suspendTimer;

				suspendTimer.Start();
				timer.Start();
				for ( int j = 0; j < ARRAYSIZE(jobs); j++ )
				{
					jobs[j].SetFlags( JF_QUEUE );
					jobs[j].bDoWork = bDoWork;
					g_pTestThreadPool->AddJob( &jobs[j] );
					if ( bSleep && j % 16 == 0 )
					{
						ThreadSleep( 0 );
					}
				}
				if ( !bInterleavePushPop )
				{
					g_pTestThreadPool->ResumeExecution();
				}
				if ( bFinishExecute && g_iSleep <= 1 )
				{
					g_done.Wait();
				}
				g_nTotalAtFinish = CCountJob::m_nCount;
				timer.End();
				g_pTestThreadPool->SuspendExecution();
				suspendTimer.End();
				g_pTestThreadPool->ResumeExecution();
				g_pTestThreadPool->Stop();
				g_done.Reset();

				int counts[8] = { 0 };
				for ( int j = 0; j < ARRAYSIZE(jobs); j++ )
				{
					if ( jobs[j].GetServiceThread() != -1 )
					{
						counts[jobs[j].GetServiceThread()]++;
						jobs[j].ClearServiceThread();
					}
				}

				Msg( "ThreadPoolTest:         %d threads -- %d (%d) jobs processed in %fms, %fms to suspend (%f/%f) [%d, %d, %d, %d, %d, %d, %d, %d]\n", 
					i, g_nTotalAtFinish, CCountJob::m_nCount, timer.GetDuration().GetMillisecondsF(), suspendTimer.GetDuration().GetMillisecondsF() - timer.GetDuration().GetMillisecondsF(),
					timer.GetDuration().GetMillisecondsF() / (float)CCountJob::m_nCount, (suspendTimer.GetDuration().GetMillisecondsF())/(float)g_nTotalAtFinish,
					counts[0], counts[1], counts[2], counts[3], counts[4], counts[5], counts[6], counts[7] );
			}
		}
	}
}


bool g_bOutputError;
volatile int g_ReadyToExecute;
CInterlockedInt g_nReady;

class CExecuteTestJob : public CJob
{
public:
	virtual JobStatus_t DoExecute()
	{
		byte pMemory[1024];
		int i;
		for ( i = 0; i < 1024; i++ )
		{
			pMemory[i] = rand();
		}
		for ( i = 0; i < 50; i++ )
		{
			sqrt( (float)HashBlock( pMemory, 1024 ) + HashBlock( pMemory, 1024 ) + 10.0 );
		}
		if ( AccessEvent()->Check() || IsFinished() )
		{
			if ( !g_bOutputError )
			{
				Msg( "Forced execute test failed!\n" );
				DebuggerBreakIfDebugging();
			}
		}
		return 0;
	}
};

class CExecuteTestExecuteJob : public CJob
{
public:
	virtual JobStatus_t DoExecute()
	{
		bool bAbort = ( RandomInt(  1, 10 ) == 1 );
		g_nReady++;
		while ( !g_ReadyToExecute )
		{
			ThreadPause();
		}

		if ( !bAbort )
			m_pTestJob->Execute();
		else
			m_pTestJob->Abort();
		g_nReady--;
		return 0;
	}

	CExecuteTestJob *m_pTestJob;
};


void TestForcedExecute()
{
	Msg( "TestForcedExecute\n" );
	for ( int tests = 0; tests < 30; tests++ )
	{
		for ( int i = 1; i <= 5; i += 2 )
		{
			g_nReady = 0;
			ThreadPoolStartParams_t params;
			params.nThreads = i;
			params.fDistribute = TRS_TRUE;
			g_pTestThreadPool->Start( params, "Tst" );

			static CExecuteTestJob jobs[4000];
			for ( int j = 0; j < ARRAYSIZE(jobs); j++ )
			{
				g_ReadyToExecute = false;
				for ( int k = 0; k < i; k++ )
				{
					CExecuteTestExecuteJob *pJob = new CExecuteTestExecuteJob;
					pJob->SetFlags( JF_QUEUE );
					pJob->m_pTestJob = &jobs[j];
					g_pTestThreadPool->AddJob( pJob );
					pJob->Release();
				}
				while ( g_nReady < i )
				{
					ThreadPause();
				}
				g_ReadyToExecute = true;
				ThreadSleep();
				jobs[j].Execute();
				while ( g_nReady > 0 )
				{
					ThreadPause();
				}
			}
			g_pTestThreadPool->Stop();
		}
	}
	Msg( "TestForcedExecute DONE\n" );
}

} // namespace ThreadPoolTest

void RunThreadPoolTests()
{
	CThreadPool pool;
	ThreadPoolTest::g_pTestThreadPool = &pool;
	RunTSQueueTests(10000);
	RunTSListTests(10000);

	int32 mask1=-1, mask2 = -1;
#ifdef _WIN32
	GetProcessAffinityMask( GetCurrentProcess(), (DWORD *) &mask1, (DWORD *) &mask2 );
#endif
	Msg( "ThreadPoolTest: Job distribution speed\n" );
	for ( int i = 0; i < 2; i++ )
	{
		bool bToCompletion = ( i % 2 != 0 );
		if ( !IsX360() )
		{
			Msg( "ThreadPoolTest:     Non-distribute\n" );
			ThreadPoolTest::Test( false, true, bToCompletion );
		}

		Msg( "ThreadPoolTest:     Distribute\n" );
		ThreadPoolTest::Test( true, true, bToCompletion  );

		Msg( "ThreadPoolTest:     One core\n" );
		ThreadSetAffinity( 0, 1 );
		ThreadPoolTest::Test( false, true, bToCompletion  );
		ThreadSetAffinity( 0, mask1 );

		Msg( "ThreadPoolTest:     NO Sleep\n" );
		ThreadPoolTest::Test( false, false, bToCompletion  );

		Msg( "ThreadPoolTest:     Distribute\n" );
		ThreadPoolTest::Test( true, false, bToCompletion  );

		Msg( "ThreadPoolTest:     One core\n" );
		ThreadSetAffinity( 0, 1 );
		ThreadPoolTest::Test( false, false, bToCompletion  );
		ThreadSetAffinity( 0, mask1 );
	}

	Msg( "ThreadPoolTest: Jobs doing work\n" );
	for ( int i = 0; i < 2; i++ )
	{
		bool bToCompletion = true;// = ( i % 2 != 0 );
		if ( !IsX360() )
		{
			Msg( "ThreadPoolTest:     Non-distribute\n" );
			ThreadPoolTest::Test( false, true, bToCompletion, true );
		}

		Msg( "ThreadPoolTest:     Distribute\n" );
		ThreadPoolTest::Test( true, true, bToCompletion, true );

		Msg( "ThreadPoolTest:     One core\n" );
		ThreadSetAffinity( 0, 1 );
		ThreadPoolTest::Test( false, true, bToCompletion, true  );
		ThreadSetAffinity( 0, mask1 );

		Msg( "ThreadPoolTest:     NO Sleep\n" );
		ThreadPoolTest::Test( false, false, bToCompletion, true  );

		Msg( "ThreadPoolTest:     Distribute\n" );
		ThreadPoolTest::Test( true, false, bToCompletion, true  );

		Msg( "ThreadPoolTest:     One core\n" );
		ThreadSetAffinity( 0, 1 );
		ThreadPoolTest::Test( false, false, bToCompletion, true  );
		ThreadSetAffinity( 0, mask1 );
	}
#ifdef _WIN32
	GetProcessAffinityMask( GetCurrentProcess(), (DWORD *) &mask1, (DWORD *) &mask2 );
#endif

	ThreadPoolTest::TestForcedExecute();
}

#endif // _LINUX

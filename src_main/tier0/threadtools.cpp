//========== Copyright © 2005, Valve Corporation, All rights reserved. ========
//
// Purpose:
//
//=============================================================================

#include "pch_tier0.h"
#include "tier1/strtools.h"
#if defined( _WIN32 ) && !defined( _X360 )
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif
#ifdef _WIN32
#include <process.h>
#elif _LINUX
typedef int (*PTHREAD_START_ROUTINE)(
    void *lpThreadParameter
    );
typedef PTHREAD_START_ROUTINE LPTHREAD_START_ROUTINE;
#include <sched.h>
#include <exception>
#include <errno.h>
#include <signal.h>
#include <pthread.h>
#include <sys/time.h>
#define GetLastError() errno
typedef void *LPVOID;
#define max(a,b)  (((a) > (b)) ? (a) : (b))
#endif
#include <memory.h>
#include "tier0/threadtools.h"
#include "tier0/vcrmode.h"
#ifdef _X360
#include "xbox/xbox_win32stubs.h"
#endif

// Must be last header...
#include "tier0/memdbgon.h"

#define THREADS_DEBUG 1

// Need to ensure initialized before other clients call in for main thread ID
#ifdef _WIN32
#pragma warning(disable:4073)
#pragma init_seg(lib)
#endif

#ifdef _WIN32
ASSERT_INVARIANT(TT_SIZEOF_CRITICALSECTION == sizeof(CRITICAL_SECTION));
ASSERT_INVARIANT(TT_INFINITE == INFINITE);
#endif

//-----------------------------------------------------------------------------
// Simple thread functions. 
// Because _beginthreadex uses stdcall, we need to convert to cdecl
//-----------------------------------------------------------------------------
struct ThreadProcInfo_t
{
	ThreadProcInfo_t( ThreadFunc_t pfnThread, void *pParam )
	  : pfnThread( pfnThread),
		pParam( pParam )
	{
	}
	
	ThreadFunc_t pfnThread;
	void *		 pParam;
};

//---------------------------------------------------------

static unsigned __stdcall ThreadProcConvert( void *pParam )
{
	ThreadProcInfo_t info = *((ThreadProcInfo_t *)pParam);
	delete ((ThreadProcInfo_t *)pParam);
	return (*info.pfnThread)(info.pParam);
}


//---------------------------------------------------------

ThreadHandle_t CreateSimpleThread( ThreadFunc_t pfnThread, void *pParam, ThreadId_t *pID, unsigned stackSize )
{
#ifdef _WIN32
	ThreadId_t idIgnored;
	if ( !pID )
		pID = &idIgnored;
	return (ThreadHandle_t)VCRHook_CreateThread(NULL, stackSize, (LPTHREAD_START_ROUTINE)ThreadProcConvert, new ThreadProcInfo_t( pfnThread, pParam ), 0, pID);
#elif _LINUX
	pthread_t tid;
	pthread_create(&tid, NULL, ThreadProcConvert, new ThreadProcInfo_t( pfnThread, pParam ) );
	if ( pID )
		*pID = (ThreadId_t)tid;
	return tid;
#endif
}

ThreadHandle_t CreateSimpleThread( ThreadFunc_t pfnThread, void *pParam, unsigned stackSize )
{
	return CreateSimpleThread( pfnThread, pParam, NULL, stackSize );
}


bool ReleaseThreadHandle( ThreadHandle_t hThread )
{
#ifdef _WIN32
	return ( CloseHandle( hThread ) != 0 );
#else
	return true;
#endif
}

//-----------------------------------------------------------------------------
//
// Wrappers for other simple threading operations
//
//-----------------------------------------------------------------------------

void ThreadSleep(unsigned duration)
{
#ifdef _WIN32
   Sleep( duration );
#elif _LINUX
   usleep( duration * 1000 ); 
#endif
}

//-----------------------------------------------------------------------------

#ifndef ThreadGetCurrentId
uint ThreadGetCurrentId()
{
#ifdef _WIN32
	return GetCurrentThreadId();
#elif _LINUX
	return pthread_self();
#endif
}
#endif

//-----------------------------------------------------------------------------
ThreadHandle_t ThreadGetCurrentHandle()
{
#ifdef _WIN32
	return (ThreadHandle_t)GetCurrentThread();
#elif _LINUX
	return (ThreadHandle_t)pthread_self();
#endif
}

//-----------------------------------------------------------------------------

int ThreadGetPriority( ThreadHandle_t hThread )
{
#ifdef _WIN32
	if ( !hThread )
	{
		return ::GetThreadPriority( GetCurrentThread() );
	}
	return ::GetThreadPriority( (HANDLE)hThread );
#else
	return 0;
#endif
}

//-----------------------------------------------------------------------------

bool ThreadSetPriority( ThreadHandle_t hThread, int priority )
{
	if ( !hThread )
	{
		hThread = ThreadGetCurrentHandle();
	}

#ifdef _WIN32
	return ( SetThreadPriority(hThread, priority) != 0 );
#elif _LINUX
	struct sched_param thread_param; 
	thread_param.sched_priority = priority; 
	pthread_setschedparam( hThread, SCHED_RR, &thread_param );
	return true;
#endif
}

//-----------------------------------------------------------------------------

void ThreadSetAffinity( ThreadHandle_t hThread, int nAffinityMask )
{
	if ( !hThread )
	{
		hThread = ThreadGetCurrentHandle();
	}

#ifdef _WIN32
	SetThreadAffinityMask( hThread, nAffinityMask );
#elif _LINUX
// 	cpu_set_t cpuSet;
// 	CPU_ZERO( cpuSet );
// 	for( int i = 0 ; i < 32; i++ )
// 	  if ( nAffinityMask & ( 1 << i ) )
// 	    CPU_SET( cpuSet, i );
// 	sched_setaffinity( hThread, sizeof( cpuSet ), &cpuSet );
#endif

}

//-----------------------------------------------------------------------------

uint InitMainThread()
{
	ThreadSetDebugName( "MainThrd" );
#ifdef _WIN32
	return ThreadGetCurrentId();
#elif _LINUX
	return pthread_self();
#endif
}

uint g_ThreadMainThreadID = InitMainThread();

bool ThreadInMainThread()
{
	return ( ThreadGetCurrentId() == g_ThreadMainThreadID );
}

//-----------------------------------------------------------------------------
void DeclareCurrentThreadIsMainThread()
{
	g_ThreadMainThreadID = ThreadGetCurrentId();
}

bool ThreadJoin( ThreadHandle_t hThread, unsigned timeout )
{
	if ( !hThread )
	{
		return false;
	}

#ifdef _WIN32
	DWORD dwWait = VCRHook_WaitForSingleObject((HANDLE)hThread, timeout);
	if ( dwWait == WAIT_TIMEOUT)
		return false;
	if ( dwWait != WAIT_OBJECT_0 && ( dwWait != WAIT_FAILED && GetLastError() != 0 ) )
	{
		Assert( 0 );
		return false;
	}
#elif _LINUX
	if ( pthread_join( (pthread_t)hThread, NULL ) != 0 )
		return false;
	//	m_threadId = 0;
#endif
	return true;
}

//-----------------------------------------------------------------------------

void ThreadSetDebugName( ThreadId_t id, const char *pszName )
{
#ifdef _WIN32
	if ( Plat_IsInDebugSession() )
	{
#define MS_VC_EXCEPTION 0x406d1388

		typedef struct tagTHREADNAME_INFO
		{
			DWORD dwType;        // must be 0x1000
			LPCSTR szName;       // pointer to name (in same addr space)
			DWORD dwThreadID;    // thread ID (-1 caller thread)
			DWORD dwFlags;       // reserved for future use, most be zero
		} THREADNAME_INFO;

		THREADNAME_INFO info;
		info.dwType = 0x1000;
		info.szName = pszName;
		info.dwThreadID = id;
		info.dwFlags = 0;

		__try
		{
			RaiseException(MS_VC_EXCEPTION, 0, sizeof(info) / sizeof(DWORD), (DWORD *)&info);
		}
		__except (EXCEPTION_CONTINUE_EXECUTION)
		{
		}
	}
#endif
}


//-----------------------------------------------------------------------------

ASSERT_INVARIANT( TW_FAILED == WAIT_FAILED );
ASSERT_INVARIANT( TW_TIMEOUT  == WAIT_TIMEOUT );
ASSERT_INVARIANT( WAIT_OBJECT_0 == 0 );

#ifdef _WIN32
int ThreadWaitForObjects( int nEvents, const HANDLE *pHandles, bool bWaitAll, unsigned timeout )
{
	return VCRHook_WaitForMultipleObjects( nEvents, pHandles, bWaitAll, timeout );
}
#endif


//-----------------------------------------------------------------------------
// Used to thread LoadLibrary on the 360
//-----------------------------------------------------------------------------
static ThreadedLoadLibraryFunc_t s_ThreadedLoadLibraryFunc = 0;
TT_INTERFACE void SetThreadedLoadLibraryFunc( ThreadedLoadLibraryFunc_t func )
{
	s_ThreadedLoadLibraryFunc = func;
}

TT_INTERFACE ThreadedLoadLibraryFunc_t GetThreadedLoadLibraryFunc()
{
	return s_ThreadedLoadLibraryFunc;
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------

CThreadSyncObject::CThreadSyncObject()
#ifdef _WIN32
  : m_hSyncObject( NULL )
#elif _LINUX
  : m_bInitalized( false )
#endif
{
}

//---------------------------------------------------------

CThreadSyncObject::~CThreadSyncObject()
{
#ifdef _WIN32
   if (m_hSyncObject )
   {
      if ( !CloseHandle(m_hSyncObject) )
	  {
		  Assert( 0 );
	  }
   }
#elif _LINUX
   if ( m_bInitalized )
   {
	pthread_cond_destroy( &m_Condition );
        pthread_mutex_destroy( &m_Mutex );
	m_bInitalized = false;
   }
#endif
}

//---------------------------------------------------------

bool CThreadSyncObject::operator!() const
{
#ifdef _WIN32
   return !m_hSyncObject;
#elif _LINUX
   return !m_bInitalized;
#endif
}

//---------------------------------------------------------

void CThreadSyncObject::AssertUseable()
{
#ifdef THREADS_DEBUG
#ifdef _WIN32
   AssertMsg( m_hSyncObject, "Thread synchronization object is unuseable" );
#elif _LINUX
   AssertMsg( m_bInitalized, "Thread synchronization object is unuseable" );
#endif
#endif
}

//---------------------------------------------------------

bool CThreadSyncObject::Wait( uint32 dwTimeout )
{
#ifdef THREADS_DEBUG
   AssertUseable();
#endif
#ifdef _WIN32
   return ( VCRHook_WaitForSingleObject( m_hSyncObject, dwTimeout ) == WAIT_OBJECT_0 );
#elif _LINUX
    pthread_mutex_lock( &m_Mutex );
    bool bRet = false;
    if ( m_cSet > 0 )
    {
	bRet = true;
    }
    else
    {
	struct timeval tv;
	gettimeofday( &tv, NULL );
	volatile struct timespec tm;

        volatile uint64 nSec = (uint64)tv.tv_usec*1000 + (uint64)dwTimeout*1000000;
        tm.tv_sec = tv.tv_sec + nSec /1000000000;
        tm.tv_nsec = nSec % 1000000000;

	volatile int ret = 0;

	do
	{   
	    ret = pthread_cond_timedwait( &m_Condition, &m_Mutex, &tm );
	} 
	while( ret == EINTR );

	bRet = ( ret == 0 );
    }
    if ( !m_bManualReset )
		m_cSet = 0;
    pthread_mutex_unlock( &m_Mutex );
    return bRet;
#endif
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------

CThreadEvent::CThreadEvent( bool bManualReset )
{
#ifdef _WIN32
    m_hSyncObject = CreateEvent( NULL, bManualReset, FALSE, NULL );
    AssertMsg1(m_hSyncObject, "Failed to create event (error 0x%x)", GetLastError() );
#elif _LINUX
    pthread_mutexattr_t Attr;
    pthread_mutexattr_init( &Attr );
    pthread_mutex_init( &m_Mutex, &Attr );
    pthread_mutexattr_destroy( &Attr );
    pthread_cond_init( &m_Condition, NULL );
    m_bInitalized = true;
    m_cSet = 0;
    m_bManualReset = bManualReset;
#else
#error "Implement me"
#endif
}
//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------


//---------------------------------------------------------

bool CThreadEvent::Set()
{
   AssertUseable();
#ifdef _WIN32
   return ( SetEvent( m_hSyncObject ) != 0 );
#elif _LINUX
    pthread_mutex_lock( &m_Mutex );
    m_cSet = 1;
    int ret = pthread_cond_broadcast( &m_Condition );
    pthread_mutex_unlock( &m_Mutex );
    return ret == 0;
#endif
}

//---------------------------------------------------------

bool CThreadEvent::Reset()
{
#ifdef THREADS_DEBUG
   AssertUseable();
#endif
#ifdef _WIN32
   return ( ResetEvent( m_hSyncObject ) != 0 );
#elif _LINUX
   pthread_mutex_lock( &m_Mutex );
   m_cSet = 0;
   pthread_mutex_unlock( &m_Mutex );
   return true; 
#endif
}

//---------------------------------------------------------

bool CThreadEvent::Check()
{
#ifdef THREADS_DEBUG
   AssertUseable();
#endif
	return Wait( 0 );
}



bool CThreadEvent::Wait( uint32 dwTimeout )
{
	return CThreadSyncObject::Wait( dwTimeout );
}

#ifdef _WIN32
//-----------------------------------------------------------------------------
//
// CThreadSemaphore
//
// To get linux implementation, try http://www-128.ibm.com/developerworks/eserver/library/es-win32linux-sem.html
//
//-----------------------------------------------------------------------------

CThreadSemaphore::CThreadSemaphore( long initialValue, long maxValue )
{
	if ( maxValue )
	{
		AssertMsg( maxValue > 0, "Invalid max value for semaphore" );
		AssertMsg( initialValue >= 0 && initialValue <= maxValue, "Invalid initial value for semaphore" );

		m_hSyncObject = CreateSemaphore( NULL, initialValue, maxValue, NULL );

		AssertMsg1(m_hSyncObject, "Failed to create semaphore (error 0x%x)", GetLastError());
	}
	else
	{
		m_hSyncObject = NULL;
	}
}

//---------------------------------------------------------

bool CThreadSemaphore::Release( long releaseCount, long *pPreviousCount )
{
#ifdef THRDTOOL_DEBUG
   AssertUseable();
#endif
   return ( ReleaseSemaphore( m_hSyncObject, releaseCount, pPreviousCount ) != 0 );
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------

CThreadFullMutex::CThreadFullMutex( bool bEstablishInitialOwnership, const char *pszName )
{
   m_hSyncObject = CreateMutex( NULL, bEstablishInitialOwnership, pszName );

   AssertMsg1( m_hSyncObject, "Failed to create mutex (error 0x%x)", GetLastError() );
}

//---------------------------------------------------------

bool CThreadFullMutex::Release()
{
#ifdef THRDTOOL_DEBUG
   AssertUseable();
#endif
   return ( ReleaseMutex( m_hSyncObject ) != 0 );
}

#endif

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------

CThreadLocalBase::CThreadLocalBase()
{
#ifdef _WIN32
	m_index = TlsAlloc();
	AssertMsg( m_index != 0xFFFFFFFF, "Bad thread local" );
	if ( m_index == 0xFFFFFFFF )
		Error( "Out of thread local storage!\n" );
#elif _LINUX
	if ( pthread_key_create( &m_index, NULL ) != 0 )
		Error( "Out of thread local storage!\n" );
#endif
}

//---------------------------------------------------------

CThreadLocalBase::~CThreadLocalBase()
{
#ifdef _WIN32
	if ( m_index != 0xFFFFFFFF )
		TlsFree( m_index );
	m_index = 0xFFFFFFFF;
#elif _LINUX
	pthread_key_delete( m_index );
#endif
}

//---------------------------------------------------------

void * CThreadLocalBase::Get() const
{
#ifdef _WIN32
	if ( m_index != 0xFFFFFFFF )
		return TlsGetValue( m_index );
	AssertMsg( 0, "Bad thread local" );
	return NULL;
#elif _LINUX
	void *value = pthread_getspecific( m_index );
	return value;
#endif
}

//---------------------------------------------------------

void CThreadLocalBase::Set( void *value )
{
#ifdef _WIN32
	if (m_index != 0xFFFFFFFF)
		TlsSetValue(m_index, value);
	else
		AssertMsg( 0, "Bad thread local" );
#elif _LINUX
	if ( pthread_setspecific( m_index, value ) != 0 )
		AssertMsg( 0, "Bad thread local" );
#endif
}

//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------

#ifdef _WIN32
#ifdef _X360
#define TO_INTERLOCK_PARAM(p)		((long *)p)
#define TO_INTERLOCK_PTR_PARAM(p)	((void **)p)
#else
#define TO_INTERLOCK_PARAM(p)		(p)
#define TO_INTERLOCK_PTR_PARAM(p)	(p)
#endif

#ifndef USE_INTRINSIC_INTERLOCKED
long ThreadInterlockedIncrement( long volatile *pDest )
{
	Assert( (size_t)pDest % 4 == 0 );
	return InterlockedIncrement( TO_INTERLOCK_PARAM(pDest) );
}

long ThreadInterlockedDecrement( long volatile *pDest )
{
	Assert( (size_t)pDest % 4 == 0 );
	return InterlockedDecrement( TO_INTERLOCK_PARAM(pDest) );
}

long ThreadInterlockedExchange( long volatile *pDest, long value )
{
	Assert( (size_t)pDest % 4 == 0 );
	return InterlockedExchange( TO_INTERLOCK_PARAM(pDest), value );
}

long ThreadInterlockedExchangeAdd( long volatile *pDest, long value )
{
	Assert( (size_t)pDest % 4 == 0 );
	return InterlockedExchangeAdd( TO_INTERLOCK_PARAM(pDest), value );
}

long ThreadInterlockedCompareExchange( long volatile *pDest, long value, long comperand )
{
	Assert( (size_t)pDest % 4 == 0 );
	return InterlockedCompareExchange( TO_INTERLOCK_PARAM(pDest), value, comperand );
}

bool ThreadInterlockedAssignIf( long volatile *pDest, long value, long comperand )
{
	Assert( (size_t)pDest % 4 == 0 );

#if !(defined(_WIN64) || defined (_X360))
	__asm 
	{
		mov	eax,comperand
		mov	ecx,pDest
		mov edx,value
		lock cmpxchg [ecx],edx 
		mov eax,0
		setz al
	}
#else
	return ( InterlockedCompareExchange( TO_INTERLOCK_PARAM(pDest), value, comperand ) == comperand );
#endif
}

#endif

#if !defined( USE_INTRINSIC_INTERLOCKED ) || defined( _WIN64 )
void *ThreadInterlockedExchangePointer( void * volatile *pDest, void *value )
{
	Assert( (size_t)pDest % 4 == 0 );
	return InterlockedExchangePointer( TO_INTERLOCK_PARAM(pDest), value );
}

void *ThreadInterlockedCompareExchangePointer( void * volatile *pDest, void *value, void *comperand )
{
	Assert( (size_t)pDest % 4 == 0 );
	return InterlockedCompareExchangePointer( TO_INTERLOCK_PTR_PARAM(pDest), value, comperand );
}

bool ThreadInterlockedAssignPointerIf( void * volatile *pDest, void *value, void *comperand )
{
	Assert( (size_t)pDest % 4 == 0 );
#if !(defined(_WIN64) || defined (_X360))
	__asm 
	{
		mov	eax,comperand
		mov	ecx,pDest
		mov edx,value
		lock cmpxchg [ecx],edx 
		mov eax,0
		setz al
	}
#else
	return ( InterlockedCompareExchangePointer( TO_INTERLOCK_PTR_PARAM(pDest), value, comperand ) == comperand );
#endif
}
#endif

int64 ThreadInterlockedCompareExchange64( int64 volatile *pDest, int64 value, int64 comperand )
{
	Assert( (size_t)pDest % 8 == 0 );

#if defined(_WIN64) || defined (_X360)
	return InterlockedCompareExchange64( pDest, value, comperand );
#else
	__asm 
	{
		lea esi,comperand;
		lea edi,value;

		mov eax,[esi];
		mov edx,4[esi];
		mov ebx,[edi];
		mov ecx,4[edi];
		mov esi,pDest;
		lock CMPXCHG8B [esi];			
	}
#endif
}

bool ThreadInterlockedAssignIf64(volatile int64 *pDest, int64 value, int64 comperand ) 
{
	Assert( (size_t)pDest % 8 == 0 );

#if defined(_WIN32) && !defined(_X360)
	__asm
	{
		lea esi,comperand;
		lea edi,value;

		mov eax,[esi];
		mov edx,4[esi];
		mov ebx,[edi];
		mov ecx,4[edi];
		mov esi,pDest;
		lock CMPXCHG8B [esi];			
		mov eax,0;
		setz al;
	}
#else
	return ( ThreadInterlockedCompareExchange64( pDest, value, comperand ) == comperand ); 
#endif
}


int64 ThreadInterlockedIncrement64( int64 volatile *pDest )
{
	Assert( (size_t)pDest % 8 == 0 );

	int64 Old;

	do 
	{
		Old = *pDest;
	} while (ThreadInterlockedCompareExchange64(pDest, Old + 1, Old) != Old);

	return Old + 1;
}

int64 ThreadInterlockedDecrement64( int64 volatile *pDest )
{
	Assert( (size_t)pDest % 8 == 0 );
	int64 Old;

	do 
	{
		Old = *pDest;
	} while (ThreadInterlockedCompareExchange64(pDest, Old - 1, Old) != Old);

	return Old - 1;
}

int64 ThreadInterlockedExchange64( int64 volatile *pDest, int64 value )
{
	Assert( (size_t)pDest % 8 == 0 );
	int64 Old;

	do 
	{
		Old = *pDest;
	} while (ThreadInterlockedCompareExchange64(pDest, value, Old) != Old);

	return Old;
}

int64 ThreadInterlockedExchangeAdd64( int64 volatile *pDest, int64 value )
{
	Assert( (size_t)pDest % 8 == 0 );
	int64 Old;

	do 
	{
		Old = *pDest;
	} while (ThreadInterlockedCompareExchange64(pDest, Old + value, Old) != Old);

	return Old;
}

#else
// This will perform horribly, what's the Linux alternative?
// atomic_set(), atomic_add() et al should work for i386 arch
// TODO: implement these if needed
CThreadMutex g_InterlockedMutex;

long ThreadInterlockedIncrement( long volatile *pDest )
{
	AUTO_LOCK( g_InterlockedMutex );
	return ++(*pDest);
}

long ThreadInterlockedDecrement( long volatile *pDest )
{
	AUTO_LOCK( g_InterlockedMutex );
	return --(*pDest);
}

long ThreadInterlockedExchange( long volatile *pDest, long value )
{
	AUTO_LOCK( g_InterlockedMutex );
	long retVal = *pDest;
	*pDest = value;
	return retVal;
}

void *ThreadInterlockedExchangePointer( void * volatile *pDest, void *value )
{
	AUTO_LOCK( g_InterlockedMutex );
	void *retVal = *pDest;
	*pDest = value;
	return retVal;
}

long ThreadInterlockedExchangeAdd( long volatile *pDest, long value )
{
	AUTO_LOCK( g_InterlockedMutex );
	long retVal = *pDest;
	*pDest += value;
	return retVal;
}

long ThreadInterlockedCompareExchange( long volatile *pDest, long value, long comperand )
{
	AUTO_LOCK( g_InterlockedMutex );
	long retVal = *pDest;
	if ( *pDest == comperand )
		*pDest = value;
	return retVal;
}

void *ThreadInterlockedCompareExchangePointer( void * volatile *pDest, void *value, void *comperand )
{
	AUTO_LOCK( g_InterlockedMutex );
	void *retVal = *pDest;
	if ( *pDest == comperand )
		*pDest = value;
	return retVal;
}


int64 ThreadInterlockedCompareExchange64( int64 volatile *pDest, int64 value, int64 comperand )
{
	Assert( (size_t)pDest % 8 == 0 );
	AUTO_LOCK( g_InterlockedMutex );
	int64 retVal = *pDest;
	if ( *pDest == comperand )
		*pDest = value;
	return retVal;
}

int64 ThreadInterlockedExchange64( int64 volatile *pDest, int64 value )
{
	Assert( (size_t)pDest % 8 == 0 );
	int64 Old;

	do 
	{
		Old = *pDest;
	} while (ThreadInterlockedCompareExchange64(pDest, value, Old) != Old);

	return Old;
}

bool ThreadInterlockedAssignIf64(volatile int64 *pDest, int64 value, int64 comperand ) 
{
	Assert( (size_t)pDest % 8 == 0 );
	return ( ThreadInterlockedCompareExchange64( pDest, value, comperand ) == comperand ); 
}

bool ThreadInterlockedAssignIf( long volatile *pDest, long value, long comperand )
{
	Assert( (size_t)pDest % 4 == 0 );
	return ( ThreadInterlockedCompareExchange( pDest, value, comperand ) == comperand ); 
}

#endif

//-----------------------------------------------------------------------------

#if defined(_WIN32) && defined(THREAD_PROFILER)
void ThreadNotifySyncNoop(void *p) {}

#define MAP_THREAD_PROFILER_CALL( from, to ) \
	void from(void *p) \
	{ \
		static CDynamicFunction<void (*)(void *)> dynFunc( "libittnotify.dll", #to, ThreadNotifySyncNoop ); \
		(*dynFunc)(p); \
	}

MAP_THREAD_PROFILER_CALL( ThreadNotifySyncPrepare, __itt_notify_sync_prepare );
MAP_THREAD_PROFILER_CALL( ThreadNotifySyncCancel, __itt_notify_sync_cancel );
MAP_THREAD_PROFILER_CALL( ThreadNotifySyncAcquired, __itt_notify_sync_acquired );
MAP_THREAD_PROFILER_CALL( ThreadNotifySyncReleasing, __itt_notify_sync_releasing );

#endif

//-----------------------------------------------------------------------------
//
// CThreadMutex
//
//-----------------------------------------------------------------------------

#ifndef _LINUX
CThreadMutex::CThreadMutex()
{
#ifdef THREAD_MUTEX_TRACING_ENABLED
	memset( &m_CriticalSection, 0, sizeof(m_CriticalSection) );
#endif
	InitializeCriticalSectionAndSpinCount((CRITICAL_SECTION *)&m_CriticalSection, 4000);
#ifdef THREAD_MUTEX_TRACING_SUPPORTED
	// These need to be initialized unconditionally in case mixing release & debug object modules
	// Lock and unlock may be emitted as COMDATs, in which case may get spurious output
	m_currentOwnerID = m_lockCount = 0;
	m_bTrace = false;
#endif
}

CThreadMutex::~CThreadMutex()
{
	DeleteCriticalSection((CRITICAL_SECTION *)&m_CriticalSection);
}
#endif // !linux

#if defined( _WIN32 ) && !defined( _X360 )
typedef BOOL (WINAPI*TryEnterCriticalSectionFunc_t)(LPCRITICAL_SECTION);
static CDynamicFunction<TryEnterCriticalSectionFunc_t> DynTryEnterCriticalSection( "Kernel32.dll", "TryEnterCriticalSection" );
#elif defined( _X360 )
#define DynTryEnterCriticalSection TryEnterCriticalSection
#endif

bool CThreadMutex::TryLock()
{

#if defined( _WIN32 )
#ifdef THREAD_MUTEX_TRACING_ENABLED
	uint thisThreadID = ThreadGetCurrentId();
	if ( m_bTrace && m_currentOwnerID && ( m_currentOwnerID != thisThreadID ) )
		Msg( "Thread %u about to try-wait for lock %x owned by %u\n", ThreadGetCurrentId(), (CRITICAL_SECTION *)&m_CriticalSection, m_currentOwnerID );
#endif
	if ( DynTryEnterCriticalSection != NULL )
	{
		if ( (*DynTryEnterCriticalSection )( (CRITICAL_SECTION *)&m_CriticalSection ) != FALSE )
		{
#ifdef THREAD_MUTEX_TRACING_ENABLED
			if (m_lockCount == 0)
			{
				// we now own it for the first time.  Set owner information
				m_currentOwnerID = thisThreadID;
				if ( m_bTrace )
					Msg( "Thread %u now owns lock 0x%x\n", m_currentOwnerID, (CRITICAL_SECTION *)&m_CriticalSection );
			}
			m_lockCount++;
#endif
			return true;
		}
		return false;
	}
	Lock();
	return true;
#elif defined( _LINUX )
	 return pthread_mutex_trylock( &m_Mutex ) == 0;
#else
#error "Implement me!"
	return true;
#endif
}

//-----------------------------------------------------------------------------
//
// CThreadFastMutex
//
//-----------------------------------------------------------------------------

#ifndef _LINUX
void CThreadFastMutex::Lock( const uint32 threadId, unsigned nSpinSleepTime ) volatile 
{
	int i;
	if ( nSpinSleepTime != TT_INFINITE )
	{
		for ( i = 1000; i != 0; --i )
		{
			if ( TryLock( threadId ) )
			{
				return;
			}
			ThreadPause();
		}

#ifdef _WIN32
		if ( !nSpinSleepTime && GetThreadPriority( GetCurrentThread() ) > THREAD_PRIORITY_NORMAL )
		{
			nSpinSleepTime = 1;
		} 
		else
#endif

		if ( nSpinSleepTime )
		{
			for ( i = 4000; i != 0; --i )
			{
				if ( TryLock( threadId ) )
				{
					return;
				}

				ThreadPause();
				ThreadSleep( 0 );
			}

		}

		for ( ;; ) // coded as for instead of while to make easy to breakpoint success
		{
			if ( TryLock( threadId ) )
			{
				return;
			}

			ThreadPause();
			ThreadSleep( nSpinSleepTime );
		}
	}
	else
	{
		for ( ;; ) // coded as for instead of while to make easy to breakpoint success
		{
			if ( TryLock( threadId ) )
			{
				return;
			}

			ThreadPause();
		}
	}
}
#endif // !linux

//-----------------------------------------------------------------------------
//
// CThreadRWLock
//
//-----------------------------------------------------------------------------

void CThreadRWLock::WaitForRead()
{
	m_nPendingReaders++;

	do
	{
		m_mutex.Unlock();
		m_CanRead.Wait();
		m_mutex.Lock();
	}
	while (m_nWriters);

	m_nPendingReaders--;
}


void CThreadRWLock::LockForWrite()
{
	m_mutex.Lock();
	bool bWait = ( m_nWriters != 0 || m_nActiveReaders != 0 );
	m_nWriters++;
	m_CanRead.Reset();
	m_mutex.Unlock();

	if ( bWait )
	{
		m_CanWrite.Wait();
	}
}

void CThreadRWLock::UnlockWrite()
{
	m_mutex.Lock();
	m_nWriters--;
	if ( m_nWriters == 0)
	{
		if ( m_nPendingReaders )
		{
			m_CanRead.Set();
		}
	}
	else
	{
		m_CanWrite.Set();
	}
	m_mutex.Unlock();
}

//-----------------------------------------------------------------------------
//
// CThreadSpinRWLock
//
//-----------------------------------------------------------------------------

void CThreadSpinRWLock::SpinLockForWrite( const uint32 threadId )
{
	int i;

	for ( i = 1000; i != 0; --i )
	{
		if ( TryLockForWrite( threadId ) )
		{
			return;
		}
		ThreadPause();
	}

	for ( i = 20000; i != 0; --i )
	{
		if ( TryLockForWrite( threadId ) )
		{
			return;
		}

		ThreadPause();
		ThreadSleep( 0 );
	}

	for ( ;; ) // coded as for instead of while to make easy to breakpoint success
	{
		if ( TryLockForWrite( threadId ) )
		{
			return;
		}

		ThreadPause();
		ThreadSleep( 1 );
	}
}

void CThreadSpinRWLock::LockForRead()
{
	int i;

	// In order to grab a read lock, the number of readers must not change and no thread can own the write lock
	LockInfo_t oldValue;
	LockInfo_t newValue;

	oldValue.m_nReaders = m_lockInfo.m_nReaders;
	oldValue.m_writerId = 0;
	newValue.m_nReaders = oldValue.m_nReaders + 1;
	newValue.m_writerId = 0;

	if( m_nWriters == 0 && AssignIf( newValue, oldValue ) )
		return;
	ThreadPause();
	oldValue.m_nReaders = m_lockInfo.m_nReaders;
	newValue.m_nReaders = oldValue.m_nReaders + 1;

	for ( i = 1000; i != 0; --i )
	{
		if( m_nWriters == 0 && AssignIf( newValue, oldValue ) )
			return;
		ThreadPause();
		oldValue.m_nReaders = m_lockInfo.m_nReaders;
		newValue.m_nReaders = oldValue.m_nReaders + 1;
	}

	for ( i = 20000; i != 0; --i )
	{
		if( m_nWriters == 0 && AssignIf( newValue, oldValue ) )
			return;
		ThreadPause();
		ThreadSleep( 0 );
		oldValue.m_nReaders = m_lockInfo.m_nReaders;
		newValue.m_nReaders = oldValue.m_nReaders + 1;
	}

	for ( ;; ) // coded as for instead of while to make easy to breakpoint success
	{
		if( m_nWriters == 0 && AssignIf( newValue, oldValue ) )
			return;
		ThreadPause();
		ThreadSleep( 1 );
		oldValue.m_nReaders = m_lockInfo.m_nReaders;
		newValue.m_nReaders = oldValue.m_nReaders + 1;
	}
}

void CThreadSpinRWLock::UnlockRead()
{
	int i;

	Assert( m_lockInfo.m_nReaders > 0 && m_lockInfo.m_writerId == 0 );
	LockInfo_t oldValue;
	LockInfo_t newValue;

	oldValue.m_nReaders = m_lockInfo.m_nReaders;
	oldValue.m_writerId = 0;
	newValue.m_nReaders = oldValue.m_nReaders - 1;
	newValue.m_writerId = 0;

	if( AssignIf( newValue, oldValue ) )
		return;
	ThreadPause();
	oldValue.m_nReaders = m_lockInfo.m_nReaders;
	newValue.m_nReaders = oldValue.m_nReaders - 1;

	for ( i = 500; i != 0; --i )
	{
		if( AssignIf( newValue, oldValue ) )
			return;
		ThreadPause();
		oldValue.m_nReaders = m_lockInfo.m_nReaders;
		newValue.m_nReaders = oldValue.m_nReaders - 1;
	}

	for ( i = 20000; i != 0; --i )
	{
		if( AssignIf( newValue, oldValue ) )
			return;
		ThreadPause();
		ThreadSleep( 0 );
		oldValue.m_nReaders = m_lockInfo.m_nReaders;
		newValue.m_nReaders = oldValue.m_nReaders - 1;
	}

	for ( ;; ) // coded as for instead of while to make easy to breakpoint success
	{
		if( AssignIf( newValue, oldValue ) )
			return;
		ThreadPause();
		ThreadSleep( 1 );
		oldValue.m_nReaders = m_lockInfo.m_nReaders;
		newValue.m_nReaders = oldValue.m_nReaders - 1;
	}
}

void CThreadSpinRWLock::UnlockWrite()
{
	Assert( m_lockInfo.m_writerId == ThreadGetCurrentId()  && m_lockInfo.m_nReaders == 0 );
	static const LockInfo_t newValue = { 0, 0 };
#if defined(_X360)
	// X360TBD: Serious Perf implications, not yet. __sync();
#endif
	ThreadInterlockedExchange64(  (int64 *)&m_lockInfo, *((int64 *)&newValue) );
	m_nWriters--;
}



//-----------------------------------------------------------------------------
//
// CThread
//
//-----------------------------------------------------------------------------

CThreadLocalPtr<CThread> g_pCurThread;

//---------------------------------------------------------

CThread::CThread()
:	
#ifdef _WIN32
	m_hThread( NULL ),
#endif
	m_threadId( 0 ),
	m_result( 0 )
{
	m_szName[0] = 0;
}

//---------------------------------------------------------

CThread::~CThread()
{
#ifdef _WIN32
	if (m_hThread)
#elif _LINUX
	if ( m_threadId )
#endif
	{
		if ( IsAlive() )
		{
			Msg( "Illegal termination of worker thread! Threads must negotiate an end to the thread before the CThread object is destroyed.\n" ); 
#ifdef _WIN32

			DoNewAssertDialog( __FILE__, __LINE__, "Illegal termination of worker thread! Threads must negotiate an end to the thread before the CThread object is destroyed.\n" );
#endif
			if ( GetCurrentCThread() == this )
			{
				Stop(); // BUGBUG: Alfred - this doesn't make sense, this destructor fires from the hosting thread not the thread itself!!
			}
		}
	}
}


//---------------------------------------------------------

const char *CThread::GetName()
{
	AUTO_LOCK( m_Lock );
	if ( !m_szName[0] )
	{
#ifdef _WIN32
		_snprintf( m_szName, sizeof(m_szName) - 1, "Thread(%p/%p)", this, m_hThread );
#elif _LINUX
		_snprintf( m_szName, sizeof(m_szName) - 1, "Thread(%0x%x/0x%x)", this, m_threadId );
#endif
		m_szName[sizeof(m_szName) - 1] = 0;
	}
	return m_szName;
}

//---------------------------------------------------------

void CThread::SetName(const char *pszName)
{
	AUTO_LOCK( m_Lock );
	strncpy( m_szName, pszName, sizeof(m_szName) - 1 );
	m_szName[sizeof(m_szName) - 1] = 0;
}

//---------------------------------------------------------

bool CThread::Start( unsigned nBytesStack )
{
	AUTO_LOCK( m_Lock );

	if ( IsAlive() )
	{
		AssertMsg( 0, "Tried to create a thread that has already been created!" );
		return false;
	}

	bool         bInitSuccess = false;

#ifdef _WIN32
	HANDLE       hThread;
	CThreadEvent createComplete;
	ThreadInit_t init = { this, &createComplete, &bInitSuccess };
	m_hThread = hThread = (HANDLE)VCRHook_CreateThread( NULL,
														nBytesStack,
														(LPTHREAD_START_ROUTINE)GetThreadProc(),
														new ThreadInit_t(init),
														0,
														&m_threadId );
	if ( !hThread )
	{
		AssertMsg1( 0, "Failed to create thread (error 0x%x)", GetLastError() );
		return false;
	}
#elif _LINUX
	ThreadInit_t init = { this, &bInitSuccess };
	pthread_attr_t attr;
	pthread_attr_init( &attr );
	pthread_attr_setstacksize( &attr, max( nBytesStack, 1024*1024 ) );
	if ( pthread_create( &m_threadId, &attr, GetThreadProc(), new ThreadInit_t( init ) ) != 0 )
	{
		AssertMsg1( 0, "Failed to create thread (error 0x%x)", GetLastError() );
		return false;
	}
	bInitSuccess = true;
#endif


#ifdef _WIN32
	if ( !WaitForCreateComplete( &createComplete ) )
	{
		Msg( "Thread failed to initialize\n" );
		CloseHandle( m_hThread );
		m_hThread = NULL;
		return false;
	}
#endif

	if ( !bInitSuccess )
	{
		Msg( "Thread failed to initialize\n" );
#ifdef _WIN32
		CloseHandle( m_hThread );
		m_hThread = NULL;
#elif _LINUX
		m_threadId = 0;
#endif
		return false;
	}

#ifdef _WIN32
	if ( !m_hThread )
	{
		Msg( "Thread exited immediately\n" );
	}
#endif

#ifdef _WIN32
	return !!m_hThread;
#elif _LINUX
	return !!m_threadId;
#endif
}

//---------------------------------------------------------
//
// Return true if the thread exists. false otherwise
//

bool CThread::IsAlive()
{
	DWORD dwExitCode;


	return (
#ifdef _WIN32
		m_hThread 
		&& GetExitCodeThread(m_hThread, &dwExitCode) 
		&& dwExitCode == STILL_ACTIVE
#elif _LINUX
		m_threadId
#endif
		);
}

//---------------------------------------------------------

bool CThread::Join(unsigned timeout)
{
#ifdef _WIN32
	if ( m_hThread )
#elif _LINUX
	if ( m_threadId )
#endif
	{
		AssertMsg(GetCurrentCThread() != this, _T("Thread cannot be joined with self"));

#ifdef _WIN32
		return ThreadJoin( (ThreadHandle_t)m_hThread );
#elif _LINUX
		return ThreadJoin( (ThreadHandle_t)m_threadId );
#endif
	}
	return true;
}

//---------------------------------------------------------

#ifdef _WIN32

HANDLE CThread::GetThreadHandle()
{
	return m_hThread;
}

//---------------------------------------------------------

uint CThread::GetThreadId()
{
	return m_threadId;
}

#endif

//---------------------------------------------------------

int CThread::GetResult()
{
	return m_result;
}

//---------------------------------------------------------
//
// Forcibly, abnormally, but relatively cleanly stop the thread
//

void CThread::Stop(int exitCode)
{
	if ( !IsAlive() )
		return;

	if ( GetCurrentCThread() == this )
	{
		m_result = exitCode;
		if ( !( m_flags & SUPPORT_STOP_PROTOCOL ) )
		{
			OnExit();
			g_pCurThread = NULL;

#ifdef _WIN32
			CloseHandle( m_hThread );
			m_hThread = NULL;
#endif
			m_threadId = 0;
		}
		throw exitCode;
	}
	else
		AssertMsg( 0, "Only thread can stop self: Use a higher-level protocol");
}

//---------------------------------------------------------

int CThread::GetPriority() const
{
#ifdef _WIN32
	return GetThreadPriority(m_hThread);
#elif _LINUX
	struct sched_param thread_param;
	int policy;
	pthread_getschedparam( m_threadId, &policy, &thread_param );
	return thread_param.sched_priority;
#endif
}

//---------------------------------------------------------

bool CThread::SetPriority(int priority)
{
#ifdef _WIN32
	return ThreadSetPriority( (ThreadHandle_t)m_hThread, priority );
#elif _LINUX
	return ThreadSetPriority( (ThreadHandle_t)m_threadId, priority );
#endif
}

//---------------------------------------------------------

unsigned CThread::Suspend()
{
#ifdef _WIN32
	return ( SuspendThread(m_hThread) != 0 );
#elif _LINUX
	Assert ( 0 );
	return 0;
#endif
}

//---------------------------------------------------------

unsigned CThread::Resume()
{
#ifdef _WIN32
	return ( ResumeThread(m_hThread) != 0 );
#elif _LINUX
	Assert ( 0 );
	return 0;
#endif
}

//---------------------------------------------------------

bool CThread::Terminate(int exitCode)
{
#ifndef _X360
#ifdef _WIN32
	// I hope you know what you're doing!
	if (!TerminateThread(m_hThread, exitCode))
		return false;
	CloseHandle( m_hThread );
	m_hThread = NULL;
	m_threadId = 0;
#elif _LINUX
	pthread_kill( m_threadId, SIGKILL );
	m_threadId = 0;
#endif

	return true;
#else
	AssertMsg( 0, "Cannot terminate a thread on the Xbox!" );
	return false;
#endif
}

//---------------------------------------------------------
//
// Get the Thread object that represents the current thread, if any.
// Can return NULL if the current thread was not created using
// CThread
//

CThread *CThread::GetCurrentCThread()
{
	return g_pCurThread;
}

//---------------------------------------------------------
//
// Offer a context switch. Under Win32, equivalent to Sleep(0)
//

void CThread::Yield()
{
#ifdef _WIN32
	::Sleep(0);
#elif _LINUX
	pthread_yield();
#endif
}

//---------------------------------------------------------
//
// This method causes the current thread to yield and not to be
// scheduled for further execution until a certain amount of real
// time has elapsed, more or less.
//

void CThread::Sleep(unsigned duration)
{
#ifdef _WIN32
	::Sleep(duration);
#elif _LINUX
	usleep( duration * 1000 );
#endif
}

//---------------------------------------------------------

bool CThread::Init()
{
	return true;
}

//---------------------------------------------------------

void CThread::OnExit()
{
}

//---------------------------------------------------------
#ifdef _WIN32
bool CThread::WaitForCreateComplete(CThreadEvent * pEvent)
{
	// Force serialized thread creation...
	if (!pEvent->Wait(60000))
	{
		AssertMsg( 0, "Probably deadlock or failure waiting for thread to initialize." );
		return false;
	}
	return true;
}
#endif

//---------------------------------------------------------

CThread::ThreadProc_t CThread::GetThreadProc()
{
	return ThreadProc;
}

//---------------------------------------------------------

unsigned __stdcall CThread::ThreadProc(LPVOID pv)
{
#ifdef _LINUX
  ThreadInit_t *pInit = (ThreadInit_t *)pv;
#else
  std::auto_ptr<ThreadInit_t> pInit((ThreadInit_t *)pv);
#endif
  
#ifdef _X360
        // Make sure all threads are consistent w.r.t floating-point math
	SetupFPUControlWord();
#endif

	CThread *pThread = pInit->pThread;
	g_pCurThread = pThread;

	g_pCurThread->m_pStackBase = AlignValue( &pThread, 4096 );

	pInit->pThread->m_result = -1;

	try
	{
		*(pInit->pfInitSuccess) = pInit->pThread->Init();
	}

	catch (...)
	{
		*(pInit->pfInitSuccess) = false;
#ifdef _WIN32
		pInit->pInitCompleteEvent->Set();
#endif
		throw;
	}
	
	bool bInitSuccess = *(pInit->pfInitSuccess);
#ifdef _WIN32
	pInit->pInitCompleteEvent->Set();
#endif
	if (!bInitSuccess)
		return 0;

	if ( !Plat_IsInDebugSession() && (pInit->pThread->m_flags & SUPPORT_STOP_PROTOCOL) )
	{
		try
		{
			pInit->pThread->m_result = pInit->pThread->Run();
		}

		catch (...)
		{
		}
	}
	else
	{
		pInit->pThread->m_result = pInit->pThread->Run();
	}

	pInit->pThread->OnExit();
	g_pCurThread = NULL;

#ifdef _WIN32
	AUTO_LOCK( pThread->m_Lock );
	CloseHandle( pThread->m_hThread );
	pThread->m_hThread = NULL;
#endif
	pThread->m_threadId = 0;

	return pInit->pThread->m_result;
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
#ifdef _WIN32
CWorkerThread::CWorkerThread()
:	m_EventSend(true),                 // must be manual-reset for PeekCall()
	m_EventComplete(true),             // must be manual-reset to handle multiple wait with thread properly
	m_Param(0),
	m_ReturnVal(0)
{
}

//---------------------------------------------------------

int CWorkerThread::CallWorker(unsigned dw, unsigned timeout, bool fBoostWorkerPriorityToMaster)
{
	return Call(dw, timeout, fBoostWorkerPriorityToMaster);
}

//---------------------------------------------------------

int CWorkerThread::CallMaster(unsigned dw, unsigned timeout)
{
	return Call(dw, timeout, false);
}

//---------------------------------------------------------

HANDLE CWorkerThread::GetCallHandle()
{
	return m_EventSend;
}

//---------------------------------------------------------

unsigned CWorkerThread::GetCallParam() const
{
	return m_Param;
}

//---------------------------------------------------------

int CWorkerThread::BoostPriority()
{
	int iInitialPriority = GetPriority();
	const int iNewPriority = ::GetThreadPriority(GetCurrentThread());
	if (iNewPriority > iInitialPriority)
		SetPriority(iNewPriority);
	return iInitialPriority;
}

//---------------------------------------------------------

static uint32 __stdcall DefaultWaitFunc( uint32 nHandles, const HANDLE*pHandles, int bWaitAll, uint32 timeout )
{
	return VCRHook_WaitForMultipleObjects( nHandles, (const void **)pHandles, bWaitAll, timeout );
}


int CWorkerThread::Call(unsigned dwParam, unsigned timeout, bool fBoostPriority, WaitFunc_t pfnWait)
{
	AssertMsg(!m_EventSend.Check(), "Cannot perform call if there's an existing call pending" );

	AUTO_LOCK( m_Lock );

	if (!IsAlive())
		return WTCR_FAIL;

	int iInitialPriority = 0;
	if (fBoostPriority)
	{
		iInitialPriority = BoostPriority();
	}

	// set the parameter, signal the worker thread, wait for the completion to be signaled
	m_Param = dwParam;

	m_EventComplete.Reset();
	m_EventSend.Set();

	WaitForReply( timeout, pfnWait );

	if (fBoostPriority)
		SetPriority(iInitialPriority);

	return m_ReturnVal;
}

//---------------------------------------------------------
//
// Wait for a request from the client
//
//---------------------------------------------------------
int CWorkerThread::WaitForReply( unsigned timeout )
{
	return WaitForReply( timeout, NULL );
}

int CWorkerThread::WaitForReply( unsigned timeout, WaitFunc_t pfnWait )
{
	if (!pfnWait)
	{
		pfnWait = DefaultWaitFunc;
	}

	HANDLE waits[] =
	{
		GetThreadHandle(),
		m_EventComplete
	};

	unsigned result;
	bool bInDebugger = Plat_IsInDebugSession();

	do
	{
		// Make sure the thread handle hasn't been closed
		if ( !GetThreadHandle() )
		{
			result = WAIT_OBJECT_0 + 1;
			break;
		}

		result = (*pfnWait)((sizeof(waits) / sizeof(waits[0])), waits, false,
			(timeout != TT_INFINITE) ? timeout : 30000);

		AssertMsg(timeout != TT_INFINITE || result != WAIT_TIMEOUT, "Possible hung thread, call to thread timed out");

	} while ( bInDebugger && ( timeout == TT_INFINITE && result == WAIT_TIMEOUT ) );

	if ( result != WAIT_OBJECT_0 + 1 )
	{
		if (result == WAIT_TIMEOUT)
			m_ReturnVal = WTCR_TIMEOUT;
		else if (result == WAIT_OBJECT_0)
		{
			DevMsg( 2, "Thread failed to respond, probably exited\n");
			m_EventSend.Reset();
			m_ReturnVal = WTCR_TIMEOUT;
		}
		else
		{
			m_EventSend.Reset();
			m_ReturnVal = WTCR_THREAD_GONE;
		}
	}

	return m_ReturnVal;
}


//---------------------------------------------------------
//
// Wait for a request from the client
//
//---------------------------------------------------------

bool CWorkerThread::WaitForCall(unsigned * pResult)
{
	return WaitForCall(TT_INFINITE, pResult);
}

//---------------------------------------------------------

bool CWorkerThread::WaitForCall(unsigned dwTimeout, unsigned * pResult)
{
	bool returnVal = m_EventSend.Wait(dwTimeout);
	if (pResult)
		*pResult = m_Param;
	return returnVal;
}

//---------------------------------------------------------
//
// is there a request?
//

bool CWorkerThread::PeekCall(unsigned * pParam)
{
	if (!m_EventSend.Check())
	{
		return false;
	}
	else
	{
		if (pParam)
		{
			*pParam = m_Param;
		}
		return true;
	}
}

//---------------------------------------------------------
//
// Reply to the request
//

void CWorkerThread::Reply(unsigned dw)
{
	m_Param = 0;
	m_ReturnVal = dw;

	// The request is now complete so PeekCall() should fail from
	// now on
	//
	// This event should be reset BEFORE we signal the client
	m_EventSend.Reset();

	// Tell the client we're finished
	m_EventComplete.Set();
}
#endif // _WIN32

//-----------------------------------------------------------------------------

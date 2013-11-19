//-----------------------------------------------------------------------------
// NOTE! This should never be called directly from leaf code
// Just use new,delete,malloc,free etc. They will call into this eventually
//-----------------------------------------------------------------------------
#include "pch_tier0.h"

#if defined(_WIN32)
#if !defined(_X360)
#define WIN_32_LEAN_AND_MEAN
#include <windows.h>
#else
#undef Verify
#define _XBOX
#include <xtl.h>
#undef _XBOX
#include "xbox/xbox_win32stubs.h"
#endif
#endif

#include <malloc.h>
#include <algorithm>
#include "tier0/dbg.h"
#include "tier0/memalloc.h"
#include "tier0/threadtools.h"
#include "tier0/tslist.h"
#include "mem_helpers.h"

#pragma pack(4)

#ifdef _X360
#define USE_PHYSICAL_SMALL_BLOCK_HEAP 1
#endif

#define MIN_SBH_BLOCK	8
#define MIN_SBH_ALIGN	8
#define MAX_SBH_BLOCK	2048
#define MAX_POOL_REGION (4*1024*1024)
#if !defined(_X360)
#define PAGE_SIZE		(4*1024)
#define COMMIT_SIZE		(16*PAGE_SIZE)
#else
#define PAGE_SIZE		(64*1024)
#define COMMIT_SIZE		(PAGE_SIZE)
#endif
#define NUM_POOLS		42

class CSmallBlockPool
{
public:
	void Init( unsigned nBlockSize, byte *pBase, unsigned initialCommit = 0 );
	size_t GetBlockSize();
	bool IsOwner( void *p );
	void *Alloc();
	void Free( void *p );
	int CountFreeBlocks();
	int GetCommittedSize();
	int CountCommittedBlocks();
	int CountAllocatedBlocks();
	int Compact();

private:

	typedef TSLNodeBase_t FreeBlock_t;
	class CFreeList : public CTSListBase
	{
	public:
		void Push( void *p ) { CTSListBase::Push( (TSLNodeBase_t *)p );	}
	};

	CFreeList		m_FreeList;

	unsigned		m_nBlockSize;

	CInterlockedPtr<byte> m_pNextAlloc;
	byte *			m_pCommitLimit;
	byte *			m_pAllocLimit;
	byte *			m_pBase;

	CThreadFastMutex m_CommitMutex;
};


class CSmallBlockHeap
{
public:
	CSmallBlockHeap();
	bool ShouldUse( size_t nBytes );
	bool IsOwner( void * p );
	void *Alloc( size_t nBytes );
	void *Realloc( void *p, size_t nBytes );
	void Free( void *p );
	size_t GetSize( void *p );
	void DumpStats( FILE *pFile = NULL );
	int Compact();

private:
	CSmallBlockPool *FindPool( size_t nBytes );
	CSmallBlockPool *FindPool( void *p );

	CSmallBlockPool *m_PoolLookup[MAX_SBH_BLOCK >> 2];
	CSmallBlockPool m_Pools[NUM_POOLS];
	byte *m_pBase;
	byte *m_pLimit;
};

#ifdef USE_PHYSICAL_SMALL_BLOCK_HEAP
#define BYTES_X360_SBH (32*1024*1024)
#define PAGESIZE_X360_SBH (64*1024)
class CX360SmallBlockPool
{
public:
	void Init( unsigned nBlockSize );
	size_t GetBlockSize();
	bool IsOwner( void *p );
	void *Alloc();
	void Free( void *p );
	int CountFreeBlocks();
	int GetCommittedSize();
	int CountCommittedBlocks();
	int CountAllocatedBlocks();

	static CX360SmallBlockPool *FindPool( void *p )
	{
		int index = (size_t)((byte *)p - gm_pPhysicalBase) / PAGESIZE_X360_SBH;
		if ( index < 0 || index >= ARRAYSIZE(gm_AddressToPool) )
			return NULL;
		return gm_AddressToPool[ index ];
	}

private:
	friend class CX360SmallBlockHeap;

	typedef TSLNodeBase_t FreeBlock_t;
	class CFreeList : public CTSListBase
	{
	public:
		void Push( void *p ) { CTSListBase::Push( (TSLNodeBase_t *)p );	}
	};

	CFreeList		m_FreeList;

	unsigned		m_nBlockSize;
	unsigned		m_CommittedSize;

	CInterlockedPtr<byte> m_pNextAlloc;
	byte *			m_pCurBlockEnd;

	CThreadFastMutex m_CommitMutex;

	static CX360SmallBlockPool *gm_AddressToPool[BYTES_X360_SBH/PAGESIZE_X360_SBH];

	static byte *gm_pPhysicalBlock;
	static byte *gm_pPhysicalBase;
	static byte *gm_pPhysicalLimit;
};


class CX360SmallBlockHeap
{
public:
	CX360SmallBlockHeap();
	bool ShouldUse( size_t nBytes );
	bool IsOwner( void * p );
	void *Alloc( size_t nBytes );
	void *Realloc( void *p, size_t nBytes );
	void Free( void *p );
	size_t GetSize( void *p );
	void DumpStats( FILE *pFile = NULL );

	CSmallBlockHeap *GetStandardSBH();

private:
	CX360SmallBlockPool *FindPool( size_t nBytes );
	CX360SmallBlockPool *FindPool( void *p );

	CX360SmallBlockPool *m_PoolLookup[MAX_SBH_BLOCK >> 2];
	CX360SmallBlockPool m_Pools[NUM_POOLS];
};
#endif


class CStdMemAlloc : public IMemAlloc
{
public:
	CStdMemAlloc()
	  :	m_pfnFailHandler( DefaultFailHandler ),
		m_sMemoryAllocFailed( (size_t)0 )
	{
	}
	// Release versions
	virtual void *Alloc( size_t nSize );
	virtual void *Realloc( void *pMem, size_t nSize );
	virtual void  Free( void *pMem );
    virtual void *Expand_NoLongerSupported( void *pMem, size_t nSize );

	// Debug versions
    virtual void *Alloc( size_t nSize, const char *pFileName, int nLine );
    virtual void *Realloc( void *pMem, size_t nSize, const char *pFileName, int nLine );
    virtual void  Free( void *pMem, const char *pFileName, int nLine );
    virtual void *Expand_NoLongerSupported( void *pMem, size_t nSize, const char *pFileName, int nLine );

	// Returns size of a particular allocation
	virtual size_t GetSize( void *pMem );

    // Force file + line information for an allocation
    virtual void PushAllocDbgInfo( const char *pFileName, int nLine );
    virtual void PopAllocDbgInfo();

	virtual long CrtSetBreakAlloc( long lNewBreakAlloc );
	virtual	int CrtSetReportMode( int nReportType, int nReportMode );
	virtual int CrtIsValidHeapPointer( const void *pMem );
	virtual int CrtIsValidPointer( const void *pMem, unsigned int size, int access );
	virtual int CrtCheckMemory( void );
	virtual int CrtSetDbgFlag( int nNewFlag );
	virtual void CrtMemCheckpoint( _CrtMemState *pState );
	void* CrtSetReportFile( int nRptType, void* hFile );
	void* CrtSetReportHook( void* pfnNewHook );
	int CrtDbgReport( int nRptType, const char * szFile,
			int nLine, const char * szModule, const char * pMsg );
	virtual int heapchk();

	virtual void DumpStats();
	virtual void DumpStatsFileBase( char const *pchFileBase );

	virtual bool IsDebugHeap() { return false; }

	virtual void GetActualDbgInfo( const char *&pFileName, int &nLine ) {}
	virtual void RegisterAllocation( const char *pFileName, int nLine, int nLogicalSize, int nActualSize, unsigned nTime ) {}
	virtual void RegisterDeallocation( const char *pFileName, int nLine, int nLogicalSize, int nActualSize, unsigned nTime ) {}

	virtual int GetVersion() { return MEMALLOC_VERSION; }

	virtual void CompactHeap();

	virtual MemAllocFailHandler_t SetAllocFailHandler( MemAllocFailHandler_t pfnMemAllocFailHandler );
	size_t CallAllocFailHandler( size_t nBytes ) { return (*m_pfnFailHandler)( nBytes); }

	static size_t DefaultFailHandler( size_t );
	void DumpBlockStats( void *p ) {}
#ifdef _WIN32
	CSmallBlockHeap m_SmallBlockHeap;
#ifdef USE_PHYSICAL_SMALL_BLOCK_HEAP
	CX360SmallBlockHeap m_LargePageSmallBlockHeap;
#endif
#endif

#if defined( _MEMTEST )
	virtual void SetStatsExtraInfo( const char *pMapName, const char *pComment );
#endif

	virtual size_t MemoryAllocFailed();

	void		SetCRTAllocFailed( size_t nMemSize );

	MemAllocFailHandler_t m_pfnFailHandler;
	size_t				m_sMemoryAllocFailed;
};



//========= Copyright © 1996-2005, Valve Corporation, All rights reserved. ============//
//
// Purpose: Memory allocation!
//
// $NoKeywords: $
//=============================================================================//

#include "pch_tier0.h"

#if !defined(STEAM) && !defined(NO_MALLOC_OVERRIDE)

#if defined( _WIN32 ) && !defined( _X360 )
#define WIN_32_LEAN_AND_MEAN
#include <windows.h>
#define VA_COMMIT_FLAGS MEM_COMMIT
#define VA_RESERVE_FLAGS MEM_RESERVE
#elif defined( _X360 )
#undef Verify
#define VA_COMMIT_FLAGS (MEM_COMMIT|MEM_NOZERO|MEM_LARGE_PAGES)
#define VA_RESERVE_FLAGS (MEM_RESERVE|MEM_LARGE_PAGES)
#endif

#include <malloc.h>
#include <algorithm>
#include "tier0/dbg.h"
#include "tier0/memalloc.h"
#include "tier0/threadtools.h"
#include "mem_helpers.h"
#include "memstd.h"
#ifdef _X360
#include "xbox/xbox_console.h"
#endif

#ifdef TIME_ALLOC
CAverageCycleCounter g_MallocCounter;
CAverageCycleCounter g_ReallocCounter;
CAverageCycleCounter g_FreeCounter;

#define PrintOne( name ) \
	Msg("%-48s: %6.4f avg (%8.1f total, %7.3f peak, %5d iters)\n",  \
		#name, \
		g_##name##Counter.GetAverageMilliseconds(), \
		g_##name##Counter.GetTotalMilliseconds(), \
		g_##name##Counter.GetPeakMilliseconds(), \
		g_##name##Counter.GetIters() ); \
	memset( &g_##name##Counter, 0, sizeof(g_##name##Counter) )

void PrintAllocTimes()
{
	PrintOne( Malloc );
	PrintOne( Realloc );
	PrintOne( Free );
}

#define PROFILE_ALLOC(name) CAverageTimeMarker name##_ATM( &g_##name##Counter )

#else
#define PROFILE_ALLOC( name ) ((void)0)
#define PrintAllocTimes() ((void)0)
#endif

#if _MSC_VER < 1400 && !defined(_STATIC_LINKED) && (defined(_DEBUG) || defined(USE_MEM_DEBUG))
void *operator new( unsigned int nSize, int nBlockUse, const char *pFileName, int nLine )
{
	return ::operator new( nSize );
}

void *operator new[] ( unsigned int nSize, int nBlockUse, const char *pFileName, int nLine )
{
	return ::operator new[]( nSize );
}
#endif

#if (!defined(_DEBUG) && !defined(USE_MEM_DEBUG))

//-----------------------------------------------------------------------------
// Singleton...
//-----------------------------------------------------------------------------
#pragma warning( disable:4074 ) // warning C4074: initializers put in compiler reserved initialization area
#pragma init_seg( compiler )

static CStdMemAlloc s_StdMemAlloc CONSTRUCT_EARLY;

#ifndef TIER0_VALIDATE_HEAP
IMemAlloc *g_pMemAlloc = &s_StdMemAlloc;
#else
IMemAlloc *g_pActualAlloc = &s_StdMemAlloc;
#endif

#ifdef _WIN32
//-----------------------------------------------------------------------------
// Small block heap (multi-pool)
//-----------------------------------------------------------------------------

#ifndef NO_SBH
#define UsingSBH() true
#else
#define UsingSBH() false
#endif


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
template <typename T>
inline T MemAlign( T val, unsigned alignment )
{
	return (T)( ( (unsigned)val + alignment - 1 ) & ~( alignment - 1 ) );
}
//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------

void CSmallBlockPool::Init( unsigned nBlockSize, byte *pBase, unsigned initialCommit )
{
	if ( !( nBlockSize % MIN_SBH_ALIGN == 0 && nBlockSize >= MIN_SBH_BLOCK && nBlockSize >= sizeof(TSLNodeBase_t) ) )
		DebuggerBreak();

	m_nBlockSize = nBlockSize;
	m_pCommitLimit = m_pNextAlloc = m_pBase = pBase;
	m_pAllocLimit = m_pBase + MAX_POOL_REGION;

	if ( initialCommit )
	{
		initialCommit = MemAlign( initialCommit, PAGE_SIZE );
		if ( !VirtualAlloc( m_pCommitLimit, initialCommit, VA_COMMIT_FLAGS, PAGE_READWRITE ) )
		{
			Assert( 0 );
			return;
		}
		m_pCommitLimit += initialCommit;
	}
}

size_t CSmallBlockPool::GetBlockSize()
{
	return m_nBlockSize;
}

bool CSmallBlockPool::IsOwner( void *p )
{
	return ( p >= m_pBase && p < m_pAllocLimit );
}

void *CSmallBlockPool::Alloc()
{
	void *pResult = m_FreeList.Pop();
	if ( !pResult )
	{
		int nBlockSize = m_nBlockSize;
		byte *pCommitLimit;
		byte *pNextAlloc;
		for (;;)
		{
			pCommitLimit = m_pCommitLimit;
			pNextAlloc = m_pNextAlloc;
			if ( pNextAlloc + nBlockSize <= pCommitLimit )
			{
				if ( m_pNextAlloc.AssignIf( pNextAlloc, pNextAlloc + m_nBlockSize ) )
				{
					pResult = pNextAlloc;
					break;
				}
			}
			else
			{
				AUTO_LOCK( m_CommitMutex );
				if ( pCommitLimit == m_pCommitLimit )
				{
					if ( pCommitLimit + COMMIT_SIZE <= m_pAllocLimit )
					{
						if ( !VirtualAlloc( pCommitLimit, COMMIT_SIZE, VA_COMMIT_FLAGS, PAGE_READWRITE ) )
						{
							Assert( 0 );
							return NULL;
						}

						m_pCommitLimit = pCommitLimit + COMMIT_SIZE;
					}
					else
					{
						return NULL;
					}
				}
			}
		}
	}
	return pResult;
}

void CSmallBlockPool::Free( void *p )
{
	Assert( IsOwner( p ) );

	m_FreeList.Push( p );
}

// Count the free blocks.  
int CSmallBlockPool::CountFreeBlocks()
{
	return m_FreeList.Count();
}

// Size of committed memory managed by this heap:
int CSmallBlockPool::GetCommittedSize()
{
	unsigned totalSize = (unsigned)m_pCommitLimit - (unsigned)m_pBase;
	Assert( 0 != m_nBlockSize );
	Assert( 0 != ( totalSize % GetBlockSize() ) );

	return totalSize;
}

// Return the total blocks memory is committed for in the heap
int CSmallBlockPool::CountCommittedBlocks()
{		 
	return  GetCommittedSize() / GetBlockSize();
}

// Count the number of allocated blocks in the heap:
int CSmallBlockPool::CountAllocatedBlocks()
{
	return CountCommittedBlocks( ) - ( CountFreeBlocks( ) + ( m_pCommitLimit - (byte *)m_pNextAlloc ) / GetBlockSize() );
}

int CSmallBlockPool::Compact()
{
	int nBytesFreed = 0;
	if ( m_FreeList.Count() )
	{
		int i;
		int nFree = CountFreeBlocks();
		FreeBlock_t **pSortArray = (FreeBlock_t **)malloc( nFree * sizeof(FreeBlock_t *) ); // can't use new because will reenter

		if ( !pSortArray )
		{
			return 0;
		}

		i = 0;
		while ( i < nFree )
		{
			pSortArray[i++] = m_FreeList.Pop();
		}

		std::sort( pSortArray, pSortArray + nFree );

		byte *pOldNextAlloc = m_pNextAlloc;

		for ( i = nFree - 1; i >= 0; i-- )
		{
			if ( (byte *)pSortArray[i] == m_pNextAlloc - m_nBlockSize )
			{
				pSortArray[i] = NULL;
				m_pNextAlloc -= m_nBlockSize;
			}
			else
			{
				break;
			}
		}

		if ( pOldNextAlloc != m_pNextAlloc )
		{
			byte *pNewCommitLimit = MemAlign( (byte *)m_pNextAlloc, PAGE_SIZE );
			if ( pNewCommitLimit < m_pCommitLimit )
			{
				nBytesFreed = m_pCommitLimit - pNewCommitLimit;
				VirtualFree( pNewCommitLimit, nBytesFreed, MEM_DECOMMIT );
				m_pCommitLimit = pNewCommitLimit;
			}
		}

		if ( pSortArray[0] )
		{
			for ( i = 0; i < nFree ; i++ )
			{
				if ( !pSortArray[i] )
				{
					break;
				}
				m_FreeList.Push( pSortArray[i] );
			}
		}

		free( pSortArray );
	}

	return nBytesFreed;
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
#define GetInitialCommitForPool( i ) 0

CSmallBlockHeap::CSmallBlockHeap()
{
	if ( !UsingSBH() )
	{
		return;
	}

	m_pBase = (byte *)VirtualAlloc( NULL, NUM_POOLS * MAX_POOL_REGION, VA_RESERVE_FLAGS, PAGE_NOACCESS );
	m_pLimit = m_pBase + NUM_POOLS * MAX_POOL_REGION;

	// Build a lookup table used to find the correct pool based on size
	const int MAX_TABLE = MAX_SBH_BLOCK >> 2;
	int i = 0;
	int nBytesElement = 0;
	byte *pCurBase = m_pBase;
	CSmallBlockPool *pCurPool = NULL;
	int iCurPool = 0;
	
	// Blocks sized 0 - 128 are in pools in increments of 8
	for ( ; i < 32; i++ )
	{
		if ( (i + 1) % 2 == 1)
		{
			nBytesElement += 8;
			pCurPool = &m_Pools[iCurPool];
			pCurPool->Init( nBytesElement, pCurBase, GetInitialCommitForPool(iCurPool) );
			iCurPool++;
			m_PoolLookup[i] = pCurPool;
			pCurBase += MAX_POOL_REGION;
		}
		else
		{
			m_PoolLookup[i] = pCurPool;
		}
	}

	// Blocks sized 129 - 256 are in pools in increments of 16
	for ( ; i < 64; i++ )
	{
		if ( (i + 1) % 4 == 1)
		{
			nBytesElement += 16;
			pCurPool = &m_Pools[iCurPool];
			pCurPool->Init( nBytesElement, pCurBase, GetInitialCommitForPool(iCurPool) );
			iCurPool++;
			m_PoolLookup[i] = pCurPool;
			pCurBase += MAX_POOL_REGION;
		}
		else
		{
			m_PoolLookup[i] = pCurPool;
		}
	}


	// Blocks sized 257 - 512 are in pools in increments of 32
	for ( ; i < 128; i++ )
	{
		if ( (i + 1) % 8 == 1)
		{
			nBytesElement += 32;
			pCurPool = &m_Pools[iCurPool];
			pCurPool->Init( nBytesElement, pCurBase, GetInitialCommitForPool(iCurPool) );
			iCurPool++;
			m_PoolLookup[i] = pCurPool;
			pCurBase += MAX_POOL_REGION;
		}
		else
		{
			m_PoolLookup[i] = pCurPool;
		}
	}

	// Blocks sized 513 - 768 are in pools in increments of 64
	for ( ; i < 192; i++ )
	{
		if ( (i + 1) % 16 == 1)
		{
			nBytesElement += 64;
			pCurPool = &m_Pools[iCurPool];
			pCurPool->Init( nBytesElement, pCurBase, GetInitialCommitForPool(iCurPool) );
			iCurPool++;
			m_PoolLookup[i] = pCurPool;
			pCurBase += MAX_POOL_REGION;
		}
		else
		{
			m_PoolLookup[i] = pCurPool;
		}
	}

	// Blocks sized 769 - 1024 are in pools in increments of 128
	for ( ; i < 256; i++ )
	{
		if ( (i + 1) % 32 == 1)
		{
			nBytesElement += 128;
			pCurPool = &m_Pools[iCurPool];
			pCurPool->Init( nBytesElement, pCurBase, GetInitialCommitForPool(iCurPool) );
			iCurPool++;
			m_PoolLookup[i] = pCurPool;
			pCurBase += MAX_POOL_REGION;
		}
		else
		{
			m_PoolLookup[i] = pCurPool;
		}
	}

	// Blocks sized 1025 - 2048 are in pools in increments of 256
	for ( ; i < MAX_TABLE; i++ )
	{
		if ( (i + 1) % 64 == 1)
		{
			nBytesElement += 256;
			pCurPool = &m_Pools[iCurPool];
			pCurPool->Init( nBytesElement, pCurBase, GetInitialCommitForPool(iCurPool) );
			iCurPool++;
			m_PoolLookup[i] = pCurPool;
			pCurBase += MAX_POOL_REGION;
		}
		else
		{
			m_PoolLookup[i] = pCurPool;
		}
	}

	Assert( iCurPool == NUM_POOLS );
}

bool CSmallBlockHeap::ShouldUse( size_t nBytes )
{
	return ( UsingSBH() && nBytes <= MAX_SBH_BLOCK );
}

bool CSmallBlockHeap::IsOwner( void * p )
{
	return ( UsingSBH() && p >= m_pBase && p < m_pLimit );
}

void *CSmallBlockHeap::Alloc( size_t nBytes )
{
	if ( nBytes == 0)
	{
		nBytes = 1;
	}
	Assert( ShouldUse( nBytes ) );
	CSmallBlockPool *pPool = FindPool( nBytes );
	
	void *p = pPool->Alloc();
	if ( p )
	{
		return p;
	}

	if ( s_StdMemAlloc.CallAllocFailHandler( nBytes ) >= nBytes )
	{
		p = pPool->Alloc();
		if ( p )
		{
			return p;
		}
	}

	void *pRet = malloc( nBytes );
	if ( !pRet )
	{
		s_StdMemAlloc.SetCRTAllocFailed( nBytes );
	}
	return pRet;
}

void *CSmallBlockHeap::Realloc( void *p, size_t nBytes )
{
	if ( nBytes == 0)
	{
		nBytes = 1;
	}

	CSmallBlockPool *pOldPool = FindPool( p );
	CSmallBlockPool *pNewPool = ( ShouldUse( nBytes ) ) ? FindPool( nBytes ) : NULL;

	if ( pOldPool == pNewPool )
	{
		return p;
	}

	void *pNewBlock = NULL;

	if ( pNewPool )
	{
		pNewBlock = pNewPool->Alloc();

		if ( !pNewBlock )
		{
			if ( s_StdMemAlloc.CallAllocFailHandler( nBytes ) >= nBytes )
			{
				pNewBlock = pNewPool->Alloc();
			}
		}
	}

	if ( !pNewBlock )
	{
		pNewBlock = malloc( nBytes );
		if ( !pNewBlock )
		{
			s_StdMemAlloc.SetCRTAllocFailed( nBytes );
		}
	}

	if ( pNewBlock )
	{
		int nBytesCopy = min( nBytes, pOldPool->GetBlockSize() );
		memcpy( pNewBlock, p, nBytesCopy );
	}

	pOldPool->Free( p );

	return pNewBlock;
}

void CSmallBlockHeap::Free( void *p )
{
	CSmallBlockPool *pPool = FindPool( p );
	pPool->Free( p );
}

size_t CSmallBlockHeap::GetSize( void *p )
{
	CSmallBlockPool *pPool = FindPool( p );
	return pPool->GetBlockSize();
}

void CSmallBlockHeap::DumpStats( FILE *pFile )
{
	bool bSpew = true;

	if ( pFile )
	{
		for( int i = 0; i < NUM_POOLS; i++ )
		{
			// output for vxconsole parsing
			fprintf( pFile, "Pool %i: Size: %u Allocated: %i Free: %i Committed: %i CommittedSize: %i\n", 
				i, 
				m_Pools[i].GetBlockSize(), 
				m_Pools[i].CountAllocatedBlocks(), 
				m_Pools[i].CountFreeBlocks(),
				m_Pools[i].CountCommittedBlocks(), 
				m_Pools[i].GetCommittedSize() );
		}
		bSpew = false;
	}

	if ( bSpew )
	{
		unsigned bytesCommitted = 0;
		unsigned bytesAllocated = 0;

		for( int i = 0; i < NUM_POOLS; i++ )
		{
			Msg( "Pool %i: (size: %u) blocks: allocated:%i free:%i committed:%i (committed size:%u kb)\n",i, m_Pools[i].GetBlockSize(),m_Pools[i].CountAllocatedBlocks(), m_Pools[i].CountFreeBlocks(),m_Pools[i].CountCommittedBlocks(), m_Pools[i].GetCommittedSize() / 1024);

			bytesCommitted += m_Pools[i].GetCommittedSize();
			bytesAllocated += ( m_Pools[i].CountAllocatedBlocks() * m_Pools[i].GetBlockSize() );
		}

		Msg( "Totals: Committed:%u kb Allocated:%u kb\n", bytesCommitted / 1024, bytesAllocated / 1024 );
	}
}

int CSmallBlockHeap::Compact()
{
	int nBytesFreed = 0;
	for( int i = 0; i < NUM_POOLS; i++ )
	{
		nBytesFreed += m_Pools[i].Compact();
	}
	return nBytesFreed;
}

CSmallBlockPool *CSmallBlockHeap::FindPool( size_t nBytes )
{
	return m_PoolLookup[(nBytes - 1) >> 2];
}

CSmallBlockPool *CSmallBlockHeap::FindPool( void *p )
{
	size_t i = ((byte *)p - m_pBase) / MAX_POOL_REGION;
	return &m_Pools[i];
}


#endif

#if USE_PHYSICAL_SMALL_BLOCK_HEAP

CX360SmallBlockPool *CX360SmallBlockPool::gm_AddressToPool[BYTES_X360_SBH/PAGESIZE_X360_SBH];
byte *CX360SmallBlockPool::gm_pPhysicalBlock;
byte *CX360SmallBlockPool::gm_pPhysicalBase;
byte *CX360SmallBlockPool::gm_pPhysicalLimit;

void CX360SmallBlockPool::Init( unsigned nBlockSize )
{
	if ( !gm_pPhysicalBlock )
	{
		gm_pPhysicalBase = (byte *)XPhysicalAlloc( BYTES_X360_SBH, MAXULONG_PTR, 4096, PAGE_READWRITE | MEM_16MB_PAGES );
		gm_pPhysicalLimit = gm_pPhysicalBase + BYTES_X360_SBH;
		gm_pPhysicalBlock = gm_pPhysicalBase;
	}

	if ( !( nBlockSize % MIN_SBH_ALIGN == 0 && nBlockSize >= MIN_SBH_BLOCK && nBlockSize >= sizeof(TSLNodeBase_t) ) )
		DebuggerBreak();

	m_nBlockSize = nBlockSize;
	m_pCurBlockEnd = m_pNextAlloc = NULL;
	m_CommittedSize = 0;
}

size_t CX360SmallBlockPool::GetBlockSize()
{
	return m_nBlockSize;
}

bool CX360SmallBlockPool::IsOwner( void *p )
{
	return ( FindPool( p ) == this );
}

void *CX360SmallBlockPool::Alloc()
{
	void *pResult = m_FreeList.Pop();
	if ( !pResult )
	{
		if ( !m_pNextAlloc && gm_pPhysicalBlock >= gm_pPhysicalLimit )
		{
			return NULL;
		}

		int nBlockSize = m_nBlockSize;
		byte *pCurBlockEnd;
		byte *pNextAlloc;
		for (;;)
		{
			pCurBlockEnd = m_pCurBlockEnd;
			pNextAlloc = m_pNextAlloc;
			if ( pNextAlloc + nBlockSize <= pCurBlockEnd )
			{
				if ( m_pNextAlloc.AssignIf( pNextAlloc, pNextAlloc + m_nBlockSize ) )
				{
					pResult = pNextAlloc;
					break;
				}
			}
			else
			{
				AUTO_LOCK( m_CommitMutex );

				if ( pCurBlockEnd == m_pCurBlockEnd )
				{
					for (;;)
					{
						if ( gm_pPhysicalBlock >= gm_pPhysicalLimit )
						{
							m_pCurBlockEnd = m_pNextAlloc = NULL;
							return NULL;
						}
						byte *pPhysicalBlock = gm_pPhysicalBlock;
						if ( ThreadInterlockedAssignPointerIf( (void **)&gm_pPhysicalBlock, (void *)(pPhysicalBlock + PAGESIZE_X360_SBH), (void *)pPhysicalBlock ) )
						{
							int index = (size_t)((byte *)pPhysicalBlock - gm_pPhysicalBase) / PAGESIZE_X360_SBH;
							gm_AddressToPool[index] = this;
							m_pNextAlloc = pPhysicalBlock;
							m_CommittedSize += PAGESIZE_X360_SBH;
							__sync();
							m_pCurBlockEnd = pPhysicalBlock + PAGESIZE_X360_SBH;
							break;
						}
					}
				}
			}
		}
	}
	return pResult;
}

void CX360SmallBlockPool::Free( void *p )
{
	Assert( IsOwner( p ) );

	m_FreeList.Push( p );
}

// Count the free blocks.  
int CX360SmallBlockPool::CountFreeBlocks()
{
	return m_FreeList.Count();
}

// Size of committed memory managed by this heap:
int CX360SmallBlockPool::GetCommittedSize()
{
	return m_CommittedSize;
}

// Return the total blocks memory is committed for in the heap
int CX360SmallBlockPool::CountCommittedBlocks()
{		 
	return  GetCommittedSize() / GetBlockSize();
}

// Count the number of allocated blocks in the heap:
int CX360SmallBlockPool::CountAllocatedBlocks()
{
	int nBytesPossible = ( m_pNextAlloc ) ? ( m_pCurBlockEnd - (byte *)m_pNextAlloc ) : 0;
	return CountCommittedBlocks( ) - ( CountFreeBlocks( ) + nBytesPossible / GetBlockSize() );
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
#define GetInitialCommitForPool( i ) 0

CX360SmallBlockHeap::CX360SmallBlockHeap()
{
	if ( !UsingSBH() )
	{
		return;
	}

	// Build a lookup table used to find the correct pool based on size
	const int MAX_TABLE = MAX_SBH_BLOCK >> 2;
	int i = 0;
	int nBytesElement = 0;
	CX360SmallBlockPool *pCurPool = NULL;
	int iCurPool = 0;

	// Blocks sized 0 - 128 are in pools in increments of 8
	for ( ; i < 32; i++ )
	{
		if ( (i + 1) % 2 == 1)
		{
			nBytesElement += 8;
			pCurPool = &m_Pools[iCurPool];
			pCurPool->Init( nBytesElement );
			iCurPool++;
			m_PoolLookup[i] = pCurPool;
		}
		else
		{
			m_PoolLookup[i] = pCurPool;
		}
	}

	// Blocks sized 129 - 256 are in pools in increments of 16
	for ( ; i < 64; i++ )
	{
		if ( (i + 1) % 4 == 1)
		{
			nBytesElement += 16;
			pCurPool = &m_Pools[iCurPool];
			pCurPool->Init( nBytesElement );
			iCurPool++;
			m_PoolLookup[i] = pCurPool;
		}
		else
		{
			m_PoolLookup[i] = pCurPool;
		}
	}


	// Blocks sized 257 - 512 are in pools in increments of 32
	for ( ; i < 128; i++ )
	{
		if ( (i + 1) % 8 == 1)
		{
			nBytesElement += 32;
			pCurPool = &m_Pools[iCurPool];
			pCurPool->Init( nBytesElement );
			iCurPool++;
			m_PoolLookup[i] = pCurPool;
		}
		else
		{
			m_PoolLookup[i] = pCurPool;
		}
	}

	// Blocks sized 513 - 768 are in pools in increments of 64
	for ( ; i < 192; i++ )
	{
		if ( (i + 1) % 16 == 1)
		{
			nBytesElement += 64;
			pCurPool = &m_Pools[iCurPool];
			pCurPool->Init( nBytesElement );
			iCurPool++;
			m_PoolLookup[i] = pCurPool;
		}
		else
		{
			m_PoolLookup[i] = pCurPool;
		}
	}

	// Blocks sized 769 - 1024 are in pools in increments of 128
	for ( ; i < 256; i++ )
	{
		if ( (i + 1) % 32 == 1)
		{
			nBytesElement += 128;
			pCurPool = &m_Pools[iCurPool];
			pCurPool->Init( nBytesElement );
			iCurPool++;
			m_PoolLookup[i] = pCurPool;
		}
		else
		{
			m_PoolLookup[i] = pCurPool;
		}
	}

	// Blocks sized 1025 - 2048 are in pools in increments of 256
	for ( ; i < MAX_TABLE; i++ )
	{
		if ( (i + 1) % 64 == 1)
		{
			nBytesElement += 256;
			pCurPool = &m_Pools[iCurPool];
			pCurPool->Init( nBytesElement );
			iCurPool++;
			m_PoolLookup[i] = pCurPool;
		}
		else
		{
			m_PoolLookup[i] = pCurPool;
		}
	}

	Assert( iCurPool == NUM_POOLS );
}

bool CX360SmallBlockHeap::ShouldUse( size_t nBytes )
{
	return ( UsingSBH() && nBytes <= MAX_SBH_BLOCK );
}

bool CX360SmallBlockHeap::IsOwner( void * p )
{
	int index = (size_t)((byte *)p - CX360SmallBlockPool::gm_pPhysicalBase) / PAGESIZE_X360_SBH;
	return ( UsingSBH() && ( index >= 0 && index < ARRAYSIZE(CX360SmallBlockPool::gm_AddressToPool) ) );
}

void *CX360SmallBlockHeap::Alloc( size_t nBytes )
{
	if ( nBytes == 0)
	{
		nBytes = 1;
	}
	Assert( ShouldUse( nBytes ) );
	CX360SmallBlockPool *pPool = FindPool( nBytes );

	void *p = pPool->Alloc();
	if ( p )
	{
		return p;
	}

	return GetStandardSBH()->Alloc( nBytes );
}

void *CX360SmallBlockHeap::Realloc( void *p, size_t nBytes )
{
	if ( nBytes == 0)
	{
		nBytes = 1;
	}

	CX360SmallBlockPool *pOldPool = FindPool( p );
	CX360SmallBlockPool *pNewPool = ( ShouldUse( nBytes ) ) ? FindPool( nBytes ) : NULL;

	if ( pOldPool == pNewPool )
	{
		return p;
	}

	void *pNewBlock = NULL;

	if ( pNewPool )
	{
		pNewBlock = pNewPool->Alloc();

		if ( !pNewBlock )
		{
			pNewBlock = GetStandardSBH()->Alloc( nBytes );
		}
	}

	if ( !pNewBlock )
	{
		pNewBlock = malloc( nBytes );
	}

	if ( pNewBlock )
	{
		int nBytesCopy = min( nBytes, pOldPool->GetBlockSize() );
		memcpy( pNewBlock, p, nBytesCopy );
	}

	pOldPool->Free( p );

	return pNewBlock;
}

void CX360SmallBlockHeap::Free( void *p )
{
	CX360SmallBlockPool *pPool = FindPool( p );
	pPool->Free( p );
}

size_t CX360SmallBlockHeap::GetSize( void *p )
{
	CX360SmallBlockPool *pPool = FindPool( p );
	return pPool->GetBlockSize();
}

void CX360SmallBlockHeap::DumpStats( FILE *pFile )
{
	bool bSpew = true;

	if ( pFile )
	{
		for( int i = 0; i < NUM_POOLS; i++ )
		{
			// output for vxconsole parsing
			fprintf( pFile, "Pool %i: Size: %u Allocated: %i Free: %i Committed: %i CommittedSize: %i\n", 
				i, 
				m_Pools[i].GetBlockSize(), 
				m_Pools[i].CountAllocatedBlocks(), 
				m_Pools[i].CountFreeBlocks(),
				m_Pools[i].CountCommittedBlocks(), 
				m_Pools[i].GetCommittedSize() );
		}
		bSpew = false;
	}

	if ( bSpew )
	{
		unsigned bytesCommitted = 0;
		unsigned bytesAllocated = 0;

		for( int i = 0; i < NUM_POOLS; i++ )
		{
			Msg( "Pool %i: (size: %u) blocks: allocated:%i free:%i committed:%i (committed size:%u kb)\n",i, m_Pools[i].GetBlockSize(),m_Pools[i].CountAllocatedBlocks(), m_Pools[i].CountFreeBlocks(),m_Pools[i].CountCommittedBlocks(), m_Pools[i].GetCommittedSize() / 1024);

			bytesCommitted += m_Pools[i].GetCommittedSize();
			bytesAllocated += ( m_Pools[i].CountAllocatedBlocks() * m_Pools[i].GetBlockSize() );
		}

		Msg( "Totals: Committed:%u kb Allocated:%u kb\n", bytesCommitted / 1024, bytesAllocated / 1024 );
	}
}

CSmallBlockHeap *CX360SmallBlockHeap::GetStandardSBH()
{
	return &(GET_OUTER( CStdMemAlloc, m_LargePageSmallBlockHeap )->m_SmallBlockHeap);
}

CX360SmallBlockPool *CX360SmallBlockHeap::FindPool( size_t nBytes )
{
	return m_PoolLookup[(nBytes - 1) >> 2];
}

CX360SmallBlockPool *CX360SmallBlockHeap::FindPool( void *p )
{
	return CX360SmallBlockPool::FindPool( p );
}


#endif

//-----------------------------------------------------------------------------
// Release versions
//-----------------------------------------------------------------------------

void *CStdMemAlloc::Alloc( size_t nSize )
{
	PROFILE_ALLOC(Malloc);
	
	void *pMem;

#ifdef _WIN32
#ifdef USE_PHYSICAL_SMALL_BLOCK_HEAP
	if ( m_LargePageSmallBlockHeap.ShouldUse( nSize ) )
	{
		pMem = m_LargePageSmallBlockHeap.Alloc( nSize );
		ApplyMemoryInitializations( pMem, nSize );
		return pMem;
	}
#endif

	if ( m_SmallBlockHeap.ShouldUse( nSize ) )
	{
		pMem = m_SmallBlockHeap.Alloc( nSize );
		ApplyMemoryInitializations( pMem, nSize );
		return pMem;
	}

#endif

	pMem = malloc( nSize );
	ApplyMemoryInitializations( pMem, nSize );
	if ( !pMem )
	{
		SetCRTAllocFailed( nSize );
	}
	return pMem;
}

void *CStdMemAlloc::Realloc( void *pMem, size_t nSize )
{
	if ( !pMem )
	{
		return Alloc( nSize );
	}

	PROFILE_ALLOC(Realloc);

#ifdef _WIN32
#ifdef USE_PHYSICAL_SMALL_BLOCK_HEAP
	if ( m_LargePageSmallBlockHeap.IsOwner( pMem ) )
	{
		return m_LargePageSmallBlockHeap.Realloc( pMem, nSize );
	}
#endif

	if ( m_SmallBlockHeap.IsOwner( pMem ) )
	{
		return m_SmallBlockHeap.Realloc( pMem, nSize );
	}
#endif

	void *pRet = realloc( pMem, nSize );
	if ( !pRet )
	{
		SetCRTAllocFailed( nSize );
	}
	return pRet;
}

void CStdMemAlloc::Free( void *pMem )
{
	if ( !pMem )
	{
		return;
	}

	PROFILE_ALLOC(Free);

#ifdef _WIN32
#ifdef USE_PHYSICAL_SMALL_BLOCK_HEAP
	if ( m_LargePageSmallBlockHeap.IsOwner( pMem ) )
	{
		m_LargePageSmallBlockHeap.Free( pMem );
		return;
	}
#endif

	if ( m_SmallBlockHeap.IsOwner( pMem ) )
	{
		m_SmallBlockHeap.Free( pMem );
		return;
	}
#endif

	free( pMem );
}

void *CStdMemAlloc::Expand_NoLongerSupported( void *pMem, size_t nSize )
{
	return NULL;
}


//-----------------------------------------------------------------------------
// Debug versions
//-----------------------------------------------------------------------------
void *CStdMemAlloc::Alloc( size_t nSize, const char *pFileName, int nLine )
{
	return CStdMemAlloc::Alloc( nSize );
}

void *CStdMemAlloc::Realloc( void *pMem, size_t nSize, const char *pFileName, int nLine )
{
	return CStdMemAlloc::Realloc( pMem, nSize );
}

void  CStdMemAlloc::Free( void *pMem, const char *pFileName, int nLine )
{
	CStdMemAlloc::Free( pMem );
}

void *CStdMemAlloc::Expand_NoLongerSupported( void *pMem, size_t nSize, const char *pFileName, int nLine )
{
	return NULL;
}


//-----------------------------------------------------------------------------
// Returns size of a particular allocation
//-----------------------------------------------------------------------------
size_t CStdMemAlloc::GetSize( void *pMem )
{
#ifdef _WIN32
	if ( !pMem )
		return CalcHeapUsed();
	else
	{
#ifdef USE_PHYSICAL_SMALL_BLOCK_HEAP
		if ( m_LargePageSmallBlockHeap.IsOwner( pMem ) )
		{
			return m_LargePageSmallBlockHeap.GetSize( pMem );
		}
#endif
		if ( m_SmallBlockHeap.IsOwner( pMem ) )
		{
			return m_SmallBlockHeap.GetSize( pMem );
		}
		return _msize( pMem );
	}
#elif _LINUX
	Assert( "GetSize() not implemented");
#endif
}


//-----------------------------------------------------------------------------
// Force file + line information for an allocation
//-----------------------------------------------------------------------------
void CStdMemAlloc::PushAllocDbgInfo( const char *pFileName, int nLine )
{
}

void CStdMemAlloc::PopAllocDbgInfo()
{
}

//-----------------------------------------------------------------------------
// FIXME: Remove when we make our own heap! Crt stuff we're currently using
//-----------------------------------------------------------------------------
long CStdMemAlloc::CrtSetBreakAlloc( long lNewBreakAlloc )
{
	return 0;
}

int CStdMemAlloc::CrtSetReportMode( int nReportType, int nReportMode )
{
	return 0;
}

int CStdMemAlloc::CrtIsValidHeapPointer( const void *pMem )
{
	return 1;
}

int CStdMemAlloc::CrtIsValidPointer( const void *pMem, unsigned int size, int access )
{
	return 1;
}

int CStdMemAlloc::CrtCheckMemory( void )
{
	return 1;
}

int CStdMemAlloc::CrtSetDbgFlag( int nNewFlag )
{
	return 0;
}

void CStdMemAlloc::CrtMemCheckpoint( _CrtMemState *pState )
{
}

// FIXME: Remove when we have our own allocator
void* CStdMemAlloc::CrtSetReportFile( int nRptType, void* hFile )
{
	return 0;
}

void* CStdMemAlloc::CrtSetReportHook( void* pfnNewHook )
{
	return 0;
}

int CStdMemAlloc::CrtDbgReport( int nRptType, const char * szFile,
		int nLine, const char * szModule, const char * pMsg )
{
	return 0;
}

int CStdMemAlloc::heapchk()
{
#ifdef _WIN32
	return _HEAPOK;
#elif _LINUX
	return 1;
#endif
}

void CStdMemAlloc::DumpStats() 
{ 
	DumpStatsFileBase( "memstats" );
}

void CStdMemAlloc::DumpStatsFileBase( char const *pchFileBase )
{
#ifdef _WIN32
	char filename[ 512 ];
	_snprintf( filename, sizeof( filename ) - 1, ( IsX360() ) ? "D:\\%s.txt" : "%s.txt", pchFileBase );
	filename[ sizeof( filename ) - 1 ] = 0;
	FILE *pFile = fopen( filename, "wt" );
#ifdef USE_PHYSICAL_SMALL_BLOCK_HEAP
	fprintf( pFile, "X360 Large Page SBH:\n" );
	m_LargePageSmallBlockHeap.DumpStats(pFile);
#endif
	fprintf( pFile, "\nSBH:\n" );
	m_SmallBlockHeap.DumpStats(pFile);	// Dump statistics to small block heap

#if defined( _X360 ) && !defined( _RETAIL )
	XBX_rMemDump( filename );
#endif

	fclose( pFile );
#endif
}

void CStdMemAlloc::CompactHeap()
{
#if !defined( NO_SBH ) && defined( _WIN32 )
	int nBytesRecovered = m_SmallBlockHeap.Compact();
	Msg( "Compact freed %d bytes\n", nBytesRecovered );
#endif
}

MemAllocFailHandler_t CStdMemAlloc::SetAllocFailHandler( MemAllocFailHandler_t pfnMemAllocFailHandler )
{
	MemAllocFailHandler_t pfnPrevious = m_pfnFailHandler;
	m_pfnFailHandler = pfnMemAllocFailHandler;
	return pfnPrevious;
}

size_t CStdMemAlloc::DefaultFailHandler( size_t nBytes )
{
	if ( IsX360() && !IsRetail() )
	{
#ifdef _X360 
		ExecuteOnce(
		{
			char buffer[256];
			_snprintf( buffer, sizeof( buffer ), "***** Memory pool overflow, attempted allocation size: %u ****\n", nBytes );
			XBX_OutputDebugString( buffer ); 
		}
		);
#endif
	}
	return 0;
}

#if defined( _MEMTEST )
void CStdMemAlloc::void SetStatsExtraInfo( const char *pMapName, const char *pComment )
{
}
#endif

void CStdMemAlloc::SetCRTAllocFailed( size_t nSize )
{
	m_sMemoryAllocFailed = nSize;
}

size_t CStdMemAlloc::MemoryAllocFailed()
{
	return m_sMemoryAllocFailed;
}

#endif

#endif // STEAM

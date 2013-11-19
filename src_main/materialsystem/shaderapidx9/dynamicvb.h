//========= Copyright © 1996-2005, Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
// $NoKeywords: $
//
//=============================================================================//

#ifndef DYNAMICVB_H
#define DYNAMICVB_H

#ifdef _WIN32
#pragma once
#endif

#include "locald3dtypes.h"
#include "Recording.h"
#include "ShaderAPIDX8_Global.h"
#include "materialsystem/ivballoctracker.h"
#include "tier0/dbg.h"

/////////////////////////////
// D. Sim Dietrich Jr.
// sim.dietrich@nvidia.com
//////////////////////


// Helper function to unbind an vertex buffer
void Unbind( IDirect3DVertexBuffer9 *pVertexBuffer );

#define X360_VERTEX_BUFFER_SIZE_MULTIPLIER 2.0 //minimum of 1, only affects dynamic buffers
//#define X360_BLOCK_ON_VB_FLUSH //uncomment to block until all data is consumed when a flush is requested. Otherwise we only block when absolutely necessary

//#define SPEW_VERTEX_BUFFER_STALLS //uncomment to allow buffer stall spewing.


class CVertexBuffer
{
public:
	CVertexBuffer( D3DDeviceWrapper * pD3D, VertexFormat_t fmt, DWORD theFVF, int vertexSize,
					int theVertexCount, const char *pTextureBudgetName, bool bSoftwareVertexProcessing, bool dynamic = false );
	~CVertexBuffer();
	
	LPDIRECT3DVERTEXBUFFER GetInterface() const { return m_pVB; };
	
	// Use at beginning of frame to force a flush of VB contents on first draw
	void FlushAtFrameStart() { m_bFlush = true; }
	
	// lock, unlock
	unsigned char* Lock( int numVerts, int& baseVertexIndex );	
	unsigned char* Modify( bool bReadOnly, int firstVertex, int numVerts );	
	void Unlock( int numVerts );

	// Vertex size
	int VertexSize() const { return m_VertexSize; }

	// Vertex count
	int VertexCount() const { return m_VertexCount; }

	static int BufferCount()
	{
#ifdef _DEBUG
		return s_BufferCount;
#else
		return 0;
#endif
	}

	// UID
	unsigned int UID() const 
	{ 
#ifdef RECORDING
		return m_UID; 
#else
		return 0;
#endif
	}

	void HandlePerFrameTextureStats( int frame )
	{
#ifdef VPROF_ENABLED
		if ( m_Frame != frame && !m_bDynamic )
		{
			m_Frame = frame;
			m_pFrameCounter += m_nBufferSize;
		}
#endif
	}
	
	// Do we have enough room without discarding?
	bool HasEnoughRoom( int numVertices ) const;

	// Is this dynamic?
	bool IsDynamic() const { return m_bDynamic; }

	// Block until this part of the vertex buffer is free
	void BlockUntilUnused( int nBufferSize );

	// used to alter the characteristics after creation
	// allows one dynamic vb to be shared for multiple formats
	void ChangeConfiguration( int vertexSize, int totalSize ) 
	{
		Assert( m_bDynamic && !m_bLocked && vertexSize );
		m_VertexSize = vertexSize;
		m_VertexCount = m_nBufferSize / vertexSize;
	}

	// Compute the next offset for the next lock
	int NextLockOffset( ) const;

	// Returns the number of vertices we have enough room for
	int NumVerticesUntilFlush() const
	{
#if defined( _X360 )
		if( m_AllocationRing.Count() )
		{
			//Cycle through the ring buffer and see what memory is free now
			int iNode = m_AllocationRing.Head();
			while( m_AllocationRing.IsValidIndex( iNode ) )
			{
				if( Dx9Device()->IsFencePending( m_AllocationRing[iNode].m_Fence ) )
					break;

				iNode = m_AllocationRing.Next( iNode );
			}

			if( m_AllocationRing.IsValidIndex( iNode ) )
			{
				int iEndFreeOffset = m_AllocationRing[iNode].m_iEndOffset;
				if( iEndFreeOffset < m_Position )
				{
					//Wrapped. Making the arbitrary decision that the return value for this function *should* handle the singe giant allocation case which requires contiguous memory
					if( iEndFreeOffset > (m_iNextBlockingPosition - m_Position) )
						return iEndFreeOffset / m_VertexSize;
					else
						return (m_iNextBlockingPosition - m_Position) / m_VertexSize;
				}
			}
			else
			{
				//we didn't block on any fence
				return m_VertexCount;
			}
		}
		
		return m_VertexCount;
#else
		return (m_nBufferSize - NextLockOffset()) / m_VertexSize;
#endif
	}

	// Marks a fence indicating when this buffer was used
	void MarkUsedInRendering()
	{
#ifdef _X360
		if ( m_bDynamic && m_pVB )
		{
			Assert( m_AllocationRing.Count() > 0 );
			m_AllocationRing[m_AllocationRing.Tail()].m_Fence = Dx9Device()->GetCurrentFence();
		}
#endif
	}

private:
	enum LOCK_FLAGS
	{
		LOCKFLAGS_FLUSH  = D3DLOCK_NOSYSLOCK | D3DLOCK_DISCARD,
#if !defined( _X360 )
		LOCKFLAGS_APPEND = D3DLOCK_NOSYSLOCK | D3DLOCK_NOOVERWRITE
#else
		// X360BUG: forcing all locks to gpu flush, otherwise bizarre mesh corruption on decals
		// Currently iterating with microsoft 360 support to track source of gpu corruption
		LOCKFLAGS_APPEND = D3DLOCK_NOSYSLOCK
#endif
	};

	LPDIRECT3DVERTEXBUFFER m_pVB;
	
#ifdef _X360
	struct DynamicBufferAllocation_t
	{
		DWORD	m_Fence; //track whether this memory is safe to use again.
		int	m_iStartOffset;
		int	m_iEndOffset;
	};

	int						m_iNextBlockingPosition; // m_iNextBlockingPosition >= m_Position where another allocation is still in use.
	unsigned char			*m_pAllocatedMemory;
	int						m_iAllocationSize; //Total size of the ring buffer, usually more than what was asked for
	IDirect3DVertexBuffer9	m_D3DVertexBuffer; //Only need one shared D3D header for our usage patterns.
	CUtlLinkedList<DynamicBufferAllocation_t> m_AllocationRing; //tracks what chunks of our memory are potentially still in use by D3D
#endif

	int				m_nBufferSize;
	int				m_Position;
	int				m_VertexCount;
	int				m_VertexSize;

	unsigned char	m_bDynamic : 1;
	unsigned char	m_bLocked : 1;
	unsigned char	m_bFlush : 1;

#ifdef VPROF_ENABLED
	int				m_Frame;
	int				*m_pFrameCounter;
	int				*m_pGlobalCounter;
#endif

#ifdef _DEBUG
	static int		s_BufferCount;
#endif

#ifdef RECORDING
	unsigned int	m_UID;
#endif

#if !defined( _X360 )
	LockedBufferContext m_LockData;
#endif
};


#ifdef _DEBUG
int CVertexBuffer::s_BufferCount = 0;
#endif

#if defined( _X360 )
#include "UtlMap.h"
MEMALLOC_DEFINE_EXTERNAL_TRACKING( XMem_CVertexBuffer );
#endif

//-----------------------------------------------------------------------------
// constructor, destructor
//-----------------------------------------------------------------------------
CVertexBuffer::CVertexBuffer(D3DDeviceWrapper * pD3D, VertexFormat_t fmt, DWORD theFVF, 
	int vertexSize, int vertexCount, const char *pTextureBudgetName,
	bool bSoftwareVertexProcessing, bool dynamic ) :
		m_pVB(0), 
		m_Position(0),
		m_VertexSize(vertexSize), 
		m_VertexCount(vertexCount),
		m_bFlush(true),
		m_bLocked(false), 
		m_nBufferSize(vertexSize * vertexCount), 
		m_bDynamic(dynamic)
#ifdef VPROF_ENABLED
		,m_Frame( -1 )
#endif
{
	MEM_ALLOC_CREDIT_( pTextureBudgetName );

#ifdef RECORDING
	// assign a UID
	static unsigned int uid = 0;
	m_UID = uid++;
#endif

#ifdef _DEBUG
	++s_BufferCount;
#endif

#ifdef VPROF_ENABLED
	if ( !m_bDynamic )
	{
		char name[256];
		Q_strcpy( name, "TexGroup_global_" );
		Q_strcat( name, pTextureBudgetName, sizeof(name) );
		m_pGlobalCounter = g_VProfCurrentProfile.FindOrCreateCounter( name, COUNTER_GROUP_TEXTURE_GLOBAL );

		Q_strcpy( name, "TexGroup_frame_" );
		Q_strcat( name, pTextureBudgetName, sizeof(name) );
		m_pFrameCounter = g_VProfCurrentProfile.FindOrCreateCounter( name, COUNTER_GROUP_TEXTURE_PER_FRAME );
	}
	else
	{
		m_pGlobalCounter = g_VProfCurrentProfile.FindOrCreateCounter( "TexGroup_global_" TEXTURE_GROUP_DYNAMIC_VERTEX_BUFFER, COUNTER_GROUP_TEXTURE_GLOBAL );
	}
#endif


	D3DVERTEXBUFFER_DESC desc;
	memset( &desc, 0x00, sizeof( desc ) );
	desc.Format = D3DFMT_VERTEXDATA;
	desc.Size = m_nBufferSize;
	desc.Type = D3DRTYPE_VERTEXBUFFER;
	desc.Pool = m_bDynamic ? D3DPOOL_DEFAULT : D3DPOOL_MANAGED;
	desc.FVF = theFVF;

	desc.Usage = D3DUSAGE_WRITEONLY;
	if ( m_bDynamic )
	{
		desc.Usage |= D3DUSAGE_DYNAMIC;
		// Dynamic meshes should never be compressed (slows down writing to them)
		Assert( CompressionType( fmt ) == VERTEX_COMPRESSION_NONE );
	}
	if ( bSoftwareVertexProcessing )
	{
		desc.Usage |= D3DUSAGE_SOFTWAREPROCESSING;
	}
	
#if !defined( _X360 )
	RECORD_COMMAND( DX8_CREATE_VERTEX_BUFFER, 6 );
	RECORD_INT( m_UID );
	RECORD_INT( m_nBufferSize );
	RECORD_INT( desc.Usage );
	RECORD_INT( desc.FVF );
	RECORD_INT( desc.Pool );
	RECORD_INT( m_bDynamic );

	HRESULT hr = pD3D->CreateVertexBuffer( m_nBufferSize, desc.Usage, desc.FVF, desc.Pool, &m_pVB, NULL );

	if ( hr == D3DERR_OUTOFVIDEOMEMORY || hr == E_OUTOFMEMORY )
	{
		// Don't have the memory for this.  Try flushing all managed resources
		// out of vid mem and try again.
		// FIXME: need to record this
		pD3D->EvictManagedResources();
		pD3D->CreateVertexBuffer( m_nBufferSize, desc.Usage, desc.FVF, desc.Pool, &m_pVB, NULL );
	}

#ifdef _DEBUG
	if ( hr != D3D_OK )
	{
		switch ( hr )
		{
		case D3DERR_INVALIDCALL:
			Assert( !"D3DERR_INVALIDCALL" );
			break;
		case D3DERR_OUTOFVIDEOMEMORY:
			Assert( !"D3DERR_OUTOFVIDEOMEMORY" );
			break;
		case E_OUTOFMEMORY:
			Assert( !"E_OUTOFMEMORY" );
			break;
		default:
			Assert( 0 );
			break;
		}
	}
#endif

	Assert( m_pVB );

#else
	// _X360
	if ( m_bDynamic )
	{
		m_iAllocationSize = m_nBufferSize * X360_VERTEX_BUFFER_SIZE_MULTIPLIER;
		Assert( m_iAllocationSize >= m_nBufferSize );
		m_pAllocatedMemory = (unsigned char*)XPhysicalAlloc( m_iAllocationSize, MAXULONG_PTR, 0, PAGE_READWRITE | MEM_LARGE_PAGES | PAGE_WRITECOMBINE );
	}
	else
	{
		m_iAllocationSize = m_nBufferSize;
		m_pAllocatedMemory = (unsigned char*)XPhysicalAlloc( m_iAllocationSize, MAXULONG_PTR, 0, PAGE_READWRITE | PAGE_WRITECOMBINE );
	}

	if ( m_pAllocatedMemory )
	{
		MemAlloc_RegisterExternalAllocation( XMem_CVertexBuffer, m_pAllocatedMemory, XPhysicalSize( m_pAllocatedMemory ) );
	}

	m_iNextBlockingPosition = m_iAllocationSize;
#endif

#ifdef VPROF_ENABLED
	if ( IsX360() || !m_bDynamic )
	{
		Assert( m_pGlobalCounter );
		*m_pGlobalCounter += m_nBufferSize;
	}
#endif

#ifdef MEASURE_DRIVER_ALLOCATIONS
	int nMemUsed = 1024;
	VPROF_INCREMENT_GROUP_COUNTER( "vb count", COUNTER_GROUP_NO_RESET, 1 );
	VPROF_INCREMENT_GROUP_COUNTER( "vb driver mem", COUNTER_GROUP_NO_RESET, nMemUsed );
	VPROF_INCREMENT_GROUP_COUNTER( "total driver mem", COUNTER_GROUP_NO_RESET, nMemUsed );
#endif

	// Track VB allocations
#if !defined( _X360 )
	g_VBAllocTracker->CountVB( m_pVB, m_bDynamic, m_nBufferSize, m_VertexSize, fmt );
#else
	g_VBAllocTracker->CountVB( m_pAllocatedMemory, m_bDynamic, m_iAllocationSize, m_VertexSize, fmt );
#endif
}

CVertexBuffer::~CVertexBuffer()
{
	// Track VB allocations
#if !defined( _X360 )
	g_VBAllocTracker->UnCountVB( m_pVB );
#else
	g_VBAllocTracker->UnCountVB( m_pAllocatedMemory );
#endif

#ifdef MEASURE_DRIVER_ALLOCATIONS
	int nMemUsed = 1024;
	VPROF_INCREMENT_GROUP_COUNTER( "vb count", COUNTER_GROUP_NO_RESET, -1 );
	VPROF_INCREMENT_GROUP_COUNTER( "vb driver mem", COUNTER_GROUP_NO_RESET, -nMemUsed );
	VPROF_INCREMENT_GROUP_COUNTER( "total driver mem", COUNTER_GROUP_NO_RESET, -nMemUsed );
#endif

#ifdef VPROF_ENABLED
	if ( IsX360() || !m_bDynamic )
	{
		Assert( m_pGlobalCounter );
		*m_pGlobalCounter -= m_nBufferSize;
	}
#endif

#ifdef _DEBUG
	--s_BufferCount;
#endif

	Unlock( 0 );

#if !defined( _X360 )
	if ( m_pVB )
	{
		RECORD_COMMAND( DX8_DESTROY_VERTEX_BUFFER, 1 );
		RECORD_INT( m_UID );

		Dx9Device()->Release( m_pVB );
	}
#else
	if ( m_pAllocatedMemory )
	{
		MemAlloc_RegisterExternalDeallocation( XMem_CVertexBuffer, m_pAllocatedMemory, XPhysicalSize( m_pAllocatedMemory ) );
		XPhysicalFree( m_pAllocatedMemory );
	}

	m_pAllocatedMemory = NULL;
	m_pVB = NULL;
#endif
}

//-----------------------------------------------------------------------------
// Compute the next offset for the next lock
//-----------------------------------------------------------------------------
inline int CVertexBuffer::NextLockOffset( ) const
{
#if !defined( _X360 )
	int nNextOffset = ( m_Position + m_VertexSize - 1 ) / m_VertexSize;
	nNextOffset *= m_VertexSize;
	return nNextOffset;
#else
	return m_Position; //position is already aligned properly on unlocks for 360.
#endif	
}


//-----------------------------------------------------------------------------
// Do we have enough room without discarding?
//-----------------------------------------------------------------------------
inline bool CVertexBuffer::HasEnoughRoom( int numVertices ) const
{
#if defined( _X360 )
	return numVertices <= m_VertexCount; //the ring buffer will free room as needed
#else
	return (NextLockOffset() + (numVertices * m_VertexSize)) <= m_nBufferSize;
#endif
}

//-----------------------------------------------------------------------------
// Block until this part of the index buffer is free
//-----------------------------------------------------------------------------
inline void CVertexBuffer::BlockUntilUnused( int nBufferSize )
{
	Assert( nBufferSize <= m_nBufferSize );

#ifdef _X360
	int nLockOffset = NextLockOffset();
	Assert( (m_AllocationRing.Count() != 0) || ((m_Position == 0) && (m_iNextBlockingPosition == m_iAllocationSize)) );

	if ( (m_iNextBlockingPosition - nLockOffset) >= nBufferSize )
		return;

	Assert( (m_AllocationRing[m_AllocationRing.Head()].m_iStartOffset == 0) || ((m_iNextBlockingPosition == m_AllocationRing[m_AllocationRing.Head()].m_iStartOffset) && (m_Position <= m_iNextBlockingPosition)) );

	int iMinBlockPosition = nLockOffset + nBufferSize;
	if( iMinBlockPosition > m_iAllocationSize )
	{
		//Allocation requires us to wrap
		iMinBlockPosition = nBufferSize;
		m_Position = 0;

		//modify the last allocation so that it uses up the whole tail end of the buffer. Makes other code simpler
		Assert( m_AllocationRing.Count() != 0 );
		m_AllocationRing[m_AllocationRing.Tail()].m_iEndOffset = m_iAllocationSize;

		//treat all allocations between the current position and the tail end of the ring as freed since they will be before we unblock
		while( m_AllocationRing.Count() ) 
		{
			unsigned int head = m_AllocationRing.Head();
			if( m_AllocationRing[head].m_iStartOffset == 0 )
				break;

			m_AllocationRing.Remove( head );
		}
	}

	//now we go through the allocations until we find the last fence we care about. Treat everything up until that fence as freed.
	DWORD FinalFence = 0;
	while( m_AllocationRing.Count() )
	{
		unsigned int head = m_AllocationRing.Head();		

		if( m_AllocationRing[head].m_iEndOffset >= iMinBlockPosition )
		{
			//When this frees, we'll finally have enough space for the allocation
			FinalFence = m_AllocationRing[head].m_Fence;
			m_iNextBlockingPosition = m_AllocationRing[head].m_iEndOffset;
			m_AllocationRing.Remove( head );
			break;
		}
		m_AllocationRing.Remove( head );
	}
	Assert( FinalFence != 0 );

#ifdef SPEW_VERTEX_BUFFER_STALLS
	if( Dx9Device()->IsFencePending( FinalFence ) )
	{
		float st = Plat_FloatTime();
#endif

		Dx9Device()->BlockOnFence( FinalFence );

#ifdef SPEW_VERTEX_BUFFER_STALLS	
		float dt = Plat_FloatTime() - st;
		Warning( "Blocked locking dynamic vertex buffer for %f ms!\n", 1000.0 * dt );
	}
#endif

#endif
}


//-----------------------------------------------------------------------------
// lock, unlock
//-----------------------------------------------------------------------------
unsigned char* CVertexBuffer::Lock( int numVerts, int& baseVertexIndex )
{
#if defined( _X360 )
	if ( m_pVB && m_pVB->IsSet( Dx9Device() ) )
	{
		Unbind( m_pVB );
	}
#endif

	unsigned char* pLockedData = 0;
	baseVertexIndex = 0;
	int nBufferSize = numVerts * m_VertexSize;

	Assert( IsPC() || ( IsX360() && !m_bLocked ) );

	// Ensure there is enough space in the VB for this data
	if ( numVerts > m_VertexCount ) 
	{ 
		Assert( 0 );
		return 0; 
	}
	
	if ( !IsX360() && !m_pVB )
		return 0;

	DWORD dwFlags;
	if ( m_bDynamic )
	{		
		dwFlags = LOCKFLAGS_APPEND;

#if !defined( _X360 )
		// If either the user forced us to flush,
		// or there is not enough space for the vertex data,
		// then flush the buffer contents
		if ( !m_Position || m_bFlush || !HasEnoughRoom(numVerts) )
		{
			m_bFlush = false;
			m_Position = 0;
			
			dwFlags = LOCKFLAGS_FLUSH;
		}
#else
		if( m_bFlush )
		{
#			if ( defined( X360_BLOCK_ON_VB_FLUSH ) )
			{
				if( m_AllocationRing.Count() )
				{
					DWORD FinalFence = m_AllocationRing[m_AllocationRing.Tail()].m_Fence;

					m_AllocationRing.RemoveAll();
					m_Position = 0;
					m_iNextBlockingPosition = m_iAllocationSize;

#				if ( defined( SPEW_VERTEX_BUFFER_STALLS ) )
						if( Dx9Device()->IsFencePending( FinalFence ) )
						{
							float st = Plat_FloatTime();
#				endif
							Dx9Device()->BlockOnFence( FinalFence );
#				if ( defined ( SPEW_VERTEX_BUFFER_STALLS ) )
							float dt = Plat_FloatTime() - st;
							Warning( "Blocked FLUSHING dynamic vertex buffer for %f ms!\n", 1000.0 * dt );
						}
#				endif
				}
			}
#			endif
			m_bFlush = false;			
		}
#endif
	}
	else
	{
		// Since we are a static VB, always lock the beginning of the buffer.
		dwFlags = D3DLOCK_NOSYSLOCK;
		m_Position = 0;
	}

	if ( IsX360() && m_bDynamic )
	{
		// Block until we have enough room in the buffer, this affects the result of NextLockOffset() in wrap conditions.
		BlockUntilUnused( nBufferSize );
		m_pVB = NULL;
	}

	int nLockOffset = NextLockOffset( );
	RECORD_COMMAND( DX8_LOCK_VERTEX_BUFFER, 4 );
	RECORD_INT( m_UID );
	RECORD_INT( nLockOffset );
	RECORD_INT( nBufferSize );
	RECORD_INT( dwFlags );

#if !defined( _X360 )
	if ( m_bDynamic )
	{
		Dx9Device()->Lock( m_pVB, nLockOffset, 
						   nBufferSize, 
						   reinterpret_cast< void** >( &pLockedData ), 
						   dwFlags, &m_LockData );
	}
	else
	{
		Dx9Device()->Lock( m_pVB, nLockOffset, 
						   nBufferSize, 
						   reinterpret_cast< void** >( &pLockedData ), 
						   dwFlags );
	}
#else
	pLockedData = m_pAllocatedMemory + nLockOffset;
#endif

	Assert( pLockedData != 0 );
	m_bLocked = true;
	if ( !IsX360() )
	{
		baseVertexIndex = nLockOffset / m_VertexSize;
	}
	else
	{
		baseVertexIndex = 0;
	}
	return pLockedData;
}

unsigned char* CVertexBuffer::Modify( bool bReadOnly, int firstVertex, int numVerts )
{
	unsigned char* pLockedData = 0;

	// D3D still returns a pointer when you call lock with 0 verts, so just in
	// case it's actually doing something, don't even try to lock the buffer with 0 verts.
	if ( numVerts == 0 )
		return NULL;

	Assert( m_pVB && !m_bDynamic );

	if ( firstVertex + numVerts > m_VertexCount ) 
	{ 
		Assert( 0 ); 
		return NULL; 
	}

	DWORD dwFlags = D3DLOCK_NOSYSLOCK;
	if ( bReadOnly )
	{
		dwFlags |= D3DLOCK_READONLY;
	}

	RECORD_COMMAND( DX8_LOCK_VERTEX_BUFFER, 4 );
	RECORD_INT( m_UID );
	RECORD_INT( firstVertex * m_VertexSize );
	RECORD_INT( numVerts * m_VertexSize );
	RECORD_INT( dwFlags );

	// mmw: for forcing all dynamic...        LOCKFLAGS_FLUSH );
#if !defined( _X360 )
	Dx9Device()->Lock( 
		m_pVB, 
		firstVertex * m_VertexSize, 
		numVerts * m_VertexSize, 
		reinterpret_cast< void** >( &pLockedData ), 
		dwFlags );
#else
	if ( m_pVB->IsSet( Dx9Device() ) )
	{
		Unbind( m_pVB );
	}
	pLockedData = m_pAllocatedMemory + (firstVertex * m_VertexSize);
#endif
	
	m_Position = firstVertex * m_VertexSize;
	Assert( pLockedData != 0 );
	m_bLocked = true;

	return pLockedData;
}
	
void CVertexBuffer::Unlock( int numVerts )
{
	if ( !m_bLocked )
		return;
	
	if ( !IsX360() && !m_pVB )
		return;

	int nLockOffset = NextLockOffset();
	int nBufferSize = numVerts * m_VertexSize;

	RECORD_COMMAND( DX8_UNLOCK_VERTEX_BUFFER, 1 );
	RECORD_INT( m_UID );

#if !defined( _X360 )
	if ( m_bDynamic )
	{
		Dx9Device()->Unlock( m_pVB, &m_LockData, nBufferSize);
	}
	else
	{
		Dx9Device()->Unlock( m_pVB );
	}
	m_Position = nLockOffset + nBufferSize;
#else
	if ( m_bDynamic )
	{
		if ( numVerts > 0 )
		{
			DynamicBufferAllocation_t LockData;
			LockData.m_Fence = Dx9Device()->GetCurrentFence(); //This isn't the correct fence, but it's all we have access to for now and it'll provide marginal safety if something goes really wrong.
			LockData.m_iStartOffset	= nLockOffset;
			LockData.m_iEndOffset = LockData.m_iStartOffset + nBufferSize;
			
			// Round dynamic locks to 4k boundaries for GPU cache reasons
			LockData.m_iEndOffset = ALIGN_VALUE( LockData.m_iEndOffset, 4096 );
			if( LockData.m_iEndOffset > m_iAllocationSize )
				LockData.m_iEndOffset = m_iAllocationSize;
			
			m_AllocationRing.AddToTail( LockData );
			m_Position = LockData.m_iEndOffset;

			void* pLockedData = m_pAllocatedMemory + LockData.m_iStartOffset;

			//Always re-use the same vertex buffer header based on the assumption that D3D copies it off in the draw calls.
			m_pVB = &m_D3DVertexBuffer;
			XGSetVertexBufferHeader( nBufferSize, 0, D3DPOOL_DEFAULT, 0, m_pVB );
			XGOffsetResourceAddress( m_pVB, pLockedData );

			// Invalidate the GPU caches for this memory.
			Dx9Device()->InvalidateGpuCache( pLockedData, nBufferSize, 0 );
		}
	}
	else
	{
		if ( !m_pVB )
		{
			m_pVB = &m_D3DVertexBuffer;
			XGSetVertexBufferHeader( m_nBufferSize, 0, D3DPOOL_DEFAULT, 0, m_pVB );
			XGOffsetResourceAddress( m_pVB, m_pAllocatedMemory );
		}
		m_Position = nLockOffset + nBufferSize;

		// Invalidate the GPU caches for this memory.
		Dx9Device()->InvalidateGpuCache( m_pAllocatedMemory, m_nBufferSize, 0 );
	}
#endif

	m_bLocked = false;
}

#endif  // DYNAMICVB_H


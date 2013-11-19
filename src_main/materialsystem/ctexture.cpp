//========= Copyright © 1996-2005, Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
//=====================================================================================//

#ifndef _X360
	#ifdef PROTECTED_THINGS_ENABLE
		#undef PROTECTED_THINGS_ENABLE
	#endif
#endif 

#include "platform.h"

// HACK: Need ShellExecute for PSD updates
#ifdef IS_WINDOWS_PC
#include <windows.h>
#include <shellapi.h>
#pragma comment ( lib, "shell32"  )
#endif // #ifndef _X360

#include "materialsystem_global.h"
#include "shaderapi/ishaderapi.h"
#include "itextureinternal.h"
#include "utlsymbol.h"
#include "time.h"
#include <sys/types.h>
#include <sys/stat.h>
#include "bitmap/imageformat.h"
#include "bitmap/tgaloader.h"
#include "bitmap/tgawriter.h"
#ifdef _WIN32
#include "direct.h"
#endif
#include "colorspace.h"
#include "string.h"
#include <malloc.h>
#include <stdlib.h>
#include "utlmemory.h"
#include "IHardwareConfigInternal.h"
#include "filesystem.h"
#include "tier1/strtools.h"
#include "vtf/vtf.h"
#include "materialsystem/materialsystem_config.h"
#include "mempool.h"
#include "texturemanager.h"
#include "utlbuffer.h"
#include "pixelwriter.h"
#include "tier1/callqueue.h"
#include "tier1/UtlStringMap.h"
#include "filesystem/IQueuedLoader.h"
#include "tier2/fileutils.h"
#include "filesystem.h"
#include "tier2/p4helpers.h"
#include "tier2/tier2.h"
#include "p4lib/ip4.h"
#include "ctype.h"
#include "ifilelist.h"

// NOTE: This must be the last file included!!!
#include "tier0/memdbgon.h"

#if !defined( _X360 )
#define TEXTURE_FNAME_EXTENSION			".vtf"
#define TEXTURE_FNAME_EXTENSION_LEN		4
#define TEXTURE_FNAME_EXTENSION_NORMAL	"_normal.vtf"
#else
#define TEXTURE_FNAME_EXTENSION			".360.vtf"
#define TEXTURE_FNAME_EXTENSION_LEN		8
#define TEXTURE_FNAME_EXTENSION_NORMAL	"_normal.360.vtf"
#endif

//-----------------------------------------------------------------------------
// Internal texture flags
//-----------------------------------------------------------------------------
enum InternalTextureFlags
{
	TEXTUREFLAGSINTERNAL_ERROR				= 0x00000001,
	TEXTUREFLAGSINTERNAL_ALLOCATED			= 0x00000002,
	TEXTUREFLAGSINTERNAL_PRELOADED			= 0x00000004, // 360: textures that went through the preload process
	TEXTUREFLAGSINTERNAL_QUEUEDLOAD			= 0x00000008, // 360: load using the queued loader
	TEXTUREFLAGSINTERNAL_EXCLUDED			= 0x00000020, // actual exclusion state
	TEXTUREFLAGSINTERNAL_SHOULDEXCLUDE		= 0x00000040, // desired exclusion state
	TEXTUREFLAGSINTERNAL_TEMPRENDERTARGET	= 0x00000080, // 360: should only allocate texture bits upon first resolve, destroy at level end
};

//-----------------------------------------------------------------------------
// Use Warning to show texture flags.
//-----------------------------------------------------------------------------
static void PrintFlags( unsigned int flags )
{
	if ( flags & TEXTUREFLAGS_NOMIP )
	{
		Warning( "TEXTUREFLAGS_NOMIP|" );
	}
	if ( flags & TEXTUREFLAGS_NOLOD )
	{
		Warning( "TEXTUREFLAGS_NOLOD|" );
	}
	if ( flags & TEXTUREFLAGS_SRGB )
	{
		Warning( "TEXTUREFLAGS_SRGB|" );
	}
	if ( flags & TEXTUREFLAGS_POINTSAMPLE )
	{
		Warning( "TEXTUREFLAGS_POINTSAMPLE|" );
	}
	if ( flags & TEXTUREFLAGS_TRILINEAR )
	{
		Warning( "TEXTUREFLAGS_TRILINEAR|" );
	}
	if ( flags & TEXTUREFLAGS_CLAMPS )
	{
		Warning( "TEXTUREFLAGS_CLAMPS|" );
	}
	if ( flags & TEXTUREFLAGS_CLAMPT )
	{
		Warning( "TEXTUREFLAGS_CLAMPT|" );
	}
	if ( flags & TEXTUREFLAGS_HINT_DXT5 )
	{
		Warning( "TEXTUREFLAGS_HINT_DXT5|" );
	}
	if ( flags & TEXTUREFLAGS_ANISOTROPIC )
	{
		Warning( "TEXTUREFLAGS_ANISOTROPIC|" );
	}
	if ( flags & TEXTUREFLAGS_PROCEDURAL )
	{
		Warning( "TEXTUREFLAGS_PROCEDURAL|" );
	}
	if ( flags & TEXTUREFLAGS_ALL_MIPS )
	{
		Warning( "TEXTUREFLAGS_ALL_MIPS|" );
	}
	if ( flags & TEXTUREFLAGS_SINGLECOPY )
	{
		Warning( "TEXTUREFLAGS_SINGLECOPY|" );
	}
}


namespace TextureLodOverride
{
	struct OverrideInfo
	{
		OverrideInfo() : x( 0 ), y( 0 ) {}
		OverrideInfo( int8 x_, int8 y_ ) : x( x_ ), y( y_ ) {}
		int8 x, y;
	};

	// Override map
	typedef CUtlStringMap< OverrideInfo > OverrideMap_t;
	OverrideMap_t s_OverrideMap;

	// Retrieves the override info adjustments
	OverrideInfo Get( char const *szName )
	{
		UtlSymId_t idx = s_OverrideMap.Find( szName );
		if ( idx != s_OverrideMap.InvalidIndex() )
			return s_OverrideMap[ idx ];
		else
			return OverrideInfo();
	}

	// Combines the existing override info adjustments with the given one
	void Add( char const *szName, OverrideInfo oi )
	{
		OverrideInfo oiex = Get( szName );
		oiex.x += oi.x;
		oiex.y += oi.y;
		s_OverrideMap[ szName ] = oiex;
	}

}; // end namespace TextureLodOverride




//-----------------------------------------------------------------------------
// Base texture class
//-----------------------------------------------------------------------------

class CTexture : public ITextureInternal
{
public:
	CTexture();
	virtual ~CTexture();

	virtual const char *GetName( void ) const;
	const char *GetTextureGroupName( void ) const;

	// Stats about the texture itself
	virtual ImageFormat GetImageFormat() const;
	NormalDecodeMode_t GetNormalDecodeMode() const;
	virtual int GetMappingWidth() const;
	virtual int GetMappingHeight() const;
	virtual int GetActualWidth() const;
	virtual int GetActualHeight() const;
	virtual int GetNumAnimationFrames() const;
	virtual bool IsTranslucent() const;
	virtual void GetReflectivity( Vector& reflectivity );

	// Reference counting
	virtual void IncrementReferenceCount( );
	virtual void DecrementReferenceCount( );
	virtual int GetReferenceCount( );

	// Used to modify the texture bits (procedural textures only)
	virtual void SetTextureRegenerator( ITextureRegenerator *pTextureRegen );

	// Little helper polling methods
	virtual bool IsNormalMap( ) const;
	virtual bool IsCubeMap( void ) const;
	virtual bool IsRenderTarget( ) const;
	virtual bool IsTempRenderTarget( void ) const;
	virtual bool IsProcedural() const;
	virtual bool IsMipmapped() const;
	virtual bool IsError() const;

	// For volume textures
	virtual bool IsVolumeTexture() const;
	virtual int GetMappingDepth() const;
	virtual int GetActualDepth() const;

	// Various ways of initializing the texture
	void InitFileTexture( const char *pTextureFile, const char *pTextureGroupName );
	void InitProceduralTexture( const char *pTextureName, const char *pTextureGroupName, int w, int h, int d, ImageFormat fmt, int nFlags );

	// Releases the texture's hw memory
	void Release();

	virtual void OnRestore();

	// Sets the filtering modes on the texture we're modifying
	void SetFilteringAndClampingMode();
	void Download( Rect_t *pRect = NULL );

	// Loads up information about the texture 
	virtual void Precache();

	// FIXME: Bogus methods... can we please delete these?
	virtual void GetLowResColorSample( float s, float t, float *color ) const;

	// Gets texture resource data of the specified type.
	// Params:
	//		eDataType		type of resource to retrieve.
	//		pnumBytes		on return is the number of bytes available in the read-only data buffer or is undefined
	// Returns:
	//		pointer to the resource data, or NULL. Note that the data from this pointer can disappear when
	// the texture goes away - you want to copy this data!
	virtual void *GetResourceData( uint32 eDataType, size_t *pNumBytes ) const;

	virtual int GetApproximateVidMemBytes( void ) const;

	// Stretch blit the framebuffer into this texture.
	virtual void CopyFrameBufferToMe( int nRenderTargetID = 0, Rect_t *pSrcRect = NULL, Rect_t *pDstRect = NULL );

	virtual ITexture *GetEmbeddedTexture( int nIndex );

	// Get the shaderapi texture handle associated w/ a particular frame
	virtual ShaderAPITextureHandle_t GetTextureHandle( int nFrame, int nChannel = 0 );

	// Sets the texture as the render target
	virtual void Bind( Sampler_t sampler );
	virtual void Bind( Sampler_t sampler1, int nFrame, Sampler_t sampler2 = (Sampler_t) -1 );
	virtual void BindVertexTexture( VertexTextureSampler_t stage, int nFrame );

	// Set this texture as a render target	
	bool SetRenderTarget( int nRenderTargetID );

	// Set this texture as a render target (optionally set depth texture as depth buffer as well)
	bool SetRenderTarget( int nRenderTargetID, ITexture *pDepthTexture);

	virtual void MarkAsPreloaded( bool bSet );
	virtual bool IsPreloaded() const;

	virtual void MarkAsExcluded( bool bSet, int nDimensionsLimit );
	virtual bool UpdateExcludedState( void );

	// Retrieve the vtf flags mask
	virtual unsigned int GetFlags( void ) const;

	virtual void ForceLODOverride( int iNumLodsOverrideUpOrDown );

	void GetFilename( char *pOut, int maxLen ) const;
	virtual void ReloadFilesInList( IFileList *pFilesToReload );

protected:
	void ReconstructTexture();
	void ReconstructPartialTexture( const Rect_t *pRect );
	bool HasBeenAllocated() const;
	void WriteDataToShaderAPITexture( int nFrameCount, int nFaceCount, int nFirstFace, int nMipCount, IVTFTexture *pVTFTexture, ImageFormat fmt );

	// Initializes/shuts down the texture
	void Init( int w, int h, int d, ImageFormat fmt, int iFlags, int iFrameCount, bool bPaired );
	void Shutdown();

	// Sets the texture name
	void SetName( const char* pName );

	// Assigns/releases texture IDs for our animation frames
	void AllocateTextureHandles( bool bPaired = false );
	void ReleaseTextureHandles( bool bPaired = false );

	// Calculates info about whether we can make the texture smaller and by how much
	// Returns the number of skipped mip levels
	int ComputeActualSize( bool bIgnorePicmip = false, IVTFTexture *pVTFTexture = NULL );

	// Computes the actual format of the texture given a desired src format
	ImageFormat ComputeActualFormat( ImageFormat srcFormat );

	// Compute the actual mip count based on the actual size
	int ComputeActualMipCount( ) const;

	// Creates/releases the shader api texture
	void AllocateShaderAPITextures();
	void FreeShaderAPITextures();

	// Download bits
	void DownloadTexture(Rect_t *pRect);
	void ReconstructTextureBits(Rect_t *pRect);

	// Gets us modifying a particular frame of our texture
	void Modify( int iFrame, bool bPaired = false );

	// Sets the texture clamping state on the currently modified frame
	void SetWrapState( );

	// Sets the texture filtering state on the currently modified frame
	void SetFilterState();

	// Loads the texture bits from a file. Optionally provides absolute path
	IVTFTexture *LoadTextureBitsFromFile( char *pCacheFileName, char **pResolvedFilename, bool bPaired );
	IVTFTexture *HandleFileLoadFailedTexture( IVTFTexture *pVTFTexture );

	// Generates the procedural bits
	IVTFTexture *ReconstructProceduralBits( );
	IVTFTexture *ReconstructPartialProceduralBits( const Rect_t *pRect, Rect_t *pActualRect );

	// Sets up debugging texture bits, if appropriate
	bool SetupDebuggingTextures( IVTFTexture *pTexture );

	// Generate a texture that shows the various mip levels
	void GenerateShowMipLevelsTextures( IVTFTexture *pTexture );

	void Cleanup( void );

	// Converts a source image read from disk into its actual format
	bool ConvertToActualFormat( IVTFTexture *pTexture, bool bPaired = false );

	// Builds the low-res image from the texture 
	void LoadLowResTexture( IVTFTexture *pTexture );
	void CopyLowResImageToTexture( IVTFTexture *pTexture );
	
	void GetDownloadFaceCount( int &nFirstFace, int &nFaceCount );
	void ComputeMipLevelSubRect( const Rect_t* pSrcRect, int nMipLevel, Rect_t *pSubRect );

	IVTFTexture *GetScratchVTFTexture( bool bPaired = false );

	int GetOptimalReadBuffer( FileHandle_t hFile, int nFileSize, CUtlBuffer &optimalBuffer );
	void FreeOptimalReadBuffer( int nMaxSize );

	void ApplyRenderTargetSizeMode( int &width, int &height, ImageFormat fmt );

protected:
#ifdef _DEBUG
	char *m_pDebugName;
#endif

	// Reflectivity vector
	Vector m_vecReflectivity;

	CUtlSymbol m_Name;

	// What texture group this texture is in (winds up setting counters based on the group name,
	// then the budget panel views the counters).
	CUtlSymbol m_TextureGroupName;

	unsigned int m_nFlags;
	unsigned int m_nInternalFlags;

	CInterlockedInt m_nRefCount;

	// This is the *desired* image format, which may or may not represent reality
	ImageFormat m_ImageFormat;

	// mappingWidth/Height and actualWidth/Height only differ 
	// if g_config.skipMipLevels != 0, or if the card has a hard limit
	// on the maximum texture size
	// This is the iWidth/iHeight for the data that m_pImageData points to.
	unsigned short m_nMappingWidth;
	unsigned short m_nMappingHeight;
	unsigned short m_nMappingDepth;

	// This is the iWidth/iHeight for whatever is downloaded to the card.
	unsigned short m_nActualWidth;		// needed for procedural
	unsigned short m_nActualHeight;		// needed for procedural
	unsigned short m_nActualDepth;

	unsigned short m_nActualMipCount;	// Mip count when it's actually used
	unsigned short m_nFrameCount;

	unsigned short m_nOriginalRTWidth;	// The values they initially specified. We generated a different width
	unsigned short m_nOriginalRTHeight;	// and height based on screen size and the flags they specify.

	unsigned char m_LowResImageWidth;
	unsigned char m_LowResImageHeight;

	unsigned short m_nDesiredDimensionLimit;	// part of texture exclusion
	unsigned short m_nActualDimensionLimit;		// value not necessarily accurate, but mismatch denotes dirty state

	// The set of texture ids for each animation frame
	ShaderAPITextureHandle_t *m_pTextureHandles;

	// lowresimage info - used for getting color data from a texture
	// without having a huge system mem overhead.
	// FIXME: We should keep this in compressed form. .is currently decompressed at load time.
#if !defined( _X360 )
	unsigned char *m_pLowResImage;
#else
	unsigned char m_LowResImageSample[4];
#endif

	ITextureRegenerator *m_pTextureRegenerator;

	// Used to help decide whether or not to recreate the render target if AA changes.
	RenderTargetType_t m_nOriginalRenderTargetType;
	RenderTargetSizeMode_t m_RenderTargetSizeMode;

#if defined( COMPRESSED_NORMAL_FORMATS )
	// Indicates that this CTexture contains a pair of CTexture data (two surface format types...useful for normal compression with alpha)
	bool m_bPaired;
	ImageFormat m_ImageFormatPaired;
	ShaderAPITextureHandle_t *m_pTextureHandlesPaired;
#endif

	// Fixed-size allocator
//	DECLARE_FIXEDSIZE_ALLOCATOR( CTexture );
public:
	void InitRenderTarget( const char *pRTName, int w, int h, RenderTargetSizeMode_t sizeMode, 
		ImageFormat fmt, RenderTargetType_t type, unsigned int textureFlags,
		unsigned int renderTargetFlags );
	
	virtual void DeleteIfUnreferenced();

#if defined( _X360 )
	virtual bool ClearTexture( int r, int g, int b, int a );
	virtual bool CreateRenderTargetSurface( int width, int height, ImageFormat format, bool bSameAsTexture );
#endif

	void FixupTexture( const void *pData, int nSize, LoaderError_t loaderError );

	void SwapContents( ITexture *pOther );

protected:
	// private data, generally from VTF resource extensions
	struct DataChunk
	{
		void Allocate( unsigned int numBytes ) 
		{
			m_pvData = new unsigned char[ numBytes ];
			m_numBytes = numBytes;
		}
		void Deallocate() const { delete [] m_pvData; }
		
		unsigned int m_eType;
		unsigned int m_numBytes;
		unsigned char *m_pvData;
	};
	CUtlVector< DataChunk > m_arrDataChunks;
};

//////////////////////////////////////////////////////////////////////////
//
// CReferenceToHandleTexture is a special implementation of ITexture
// to be used solely for binding the texture handle when rendering.
// It is used when a D3D texture handle is available, but should be used
// at a higher level of abstraction requiring an ITexture or ITextureInternal.
//
//////////////////////////////////////////////////////////////////////////
class CReferenceToHandleTexture : public ITextureInternal
{
public:
	CReferenceToHandleTexture();
	virtual ~CReferenceToHandleTexture();

	virtual const char *GetName( void ) const { return m_Name.String(); }
	const char *GetTextureGroupName( void ) const { return m_TextureGroupName.String(); }

	// Stats about the texture itself
	virtual ImageFormat GetImageFormat() const { return IMAGE_FORMAT_UNKNOWN; }
	virtual NormalDecodeMode_t GetNormalDecodeMode() const { return NORMAL_DECODE_NONE; }
	virtual int GetMappingWidth() const { return 1; }
	virtual int GetMappingHeight() const { return 1; }
	virtual int GetActualWidth() const { return 1; }
	virtual int GetActualHeight() const { return 1; }
	virtual int GetNumAnimationFrames() const { return 1; }
	virtual bool IsTranslucent() const { return false; }
	virtual void GetReflectivity( Vector& reflectivity ) { reflectivity.Zero(); }

	// Reference counting
	virtual void IncrementReferenceCount( ) { ++ m_nRefCount; }
	virtual void DecrementReferenceCount( ) { -- m_nRefCount; }
	virtual int GetReferenceCount( ) { return m_nRefCount; }

	// Used to modify the texture bits (procedural textures only)
	virtual void SetTextureRegenerator( ITextureRegenerator *pTextureRegen ) { NULL; }

	// Little helper polling methods
	virtual bool IsNormalMap( ) const { return false; }
	virtual bool IsCubeMap( void ) const { return false; }
	virtual bool IsRenderTarget( ) const { return false; }
	virtual bool IsTempRenderTarget( void ) const { return false; }
	virtual bool IsProcedural() const { return true; }
	virtual bool IsMipmapped() const { return false; }
	virtual bool IsError() const { return false; }

	// For volume textures
	virtual bool IsVolumeTexture() const { return false; }
	virtual int GetMappingDepth() const { return 1; }
	virtual int GetActualDepth() const { return 1; }

	// Releases the texture's hw memory
	void Release() { NULL; }

	virtual void OnRestore() { NULL; }

	// Sets the filtering modes on the texture we're modifying
	void SetFilteringAndClampingMode() { NULL; }
	void Download( Rect_t *pRect = NULL ) { NULL; }

	// Loads up information about the texture 
	virtual void Precache() { NULL; }

	// FIXME: Bogus methods... can we please delete these?
	virtual void GetLowResColorSample( float s, float t, float *color ) const { NULL; }

	// Gets texture resource data of the specified type.
	// Params:
	//		eDataType		type of resource to retrieve.
	//		pnumBytes		on return is the number of bytes available in the read-only data buffer or is undefined
	// Returns:
	//		pointer to the resource data, or NULL. Note that the data from this pointer can disappear when
	// the texture goes away - you want to copy this data!
	virtual void *GetResourceData( uint32 eDataType, size_t *pNumBytes ) const { return NULL; }

	virtual int GetApproximateVidMemBytes( void ) const { return 32; }

	// Stretch blit the framebuffer into this texture.
	virtual void CopyFrameBufferToMe( int nRenderTargetID = 0, Rect_t *pSrcRect = NULL, Rect_t *pDstRect = NULL ) { NULL; }

	virtual ITexture *GetEmbeddedTexture( int nIndex ) { return ( nIndex == 0 ) ? this : NULL; }

	// Get the shaderapi texture handle associated w/ a particular frame
	virtual ShaderAPITextureHandle_t GetTextureHandle( int nFrame, int nTextureChannel = 0 ) { return m_hTexture; }

	// Bind the texture
	virtual void Bind( Sampler_t sampler );
	virtual void Bind( Sampler_t sampler1, int nFrame, Sampler_t sampler2 = (Sampler_t) -1 );
	virtual void BindVertexTexture( VertexTextureSampler_t stage, int nFrame );

	// Set this texture as a render target	
	bool SetRenderTarget( int nRenderTargetID ) { return SetRenderTarget( nRenderTargetID, NULL ); }

	// Set this texture as a render target (optionally set depth texture as depth buffer as well)
	bool SetRenderTarget( int nRenderTargetID, ITexture *pDepthTexture) { return false; }

	virtual void MarkAsPreloaded( bool bSet ) { NULL; }
	virtual bool IsPreloaded() const { return true; }

	virtual void MarkAsExcluded( bool bSet, int nDimensionsLimit ) { NULL; }
	virtual bool UpdateExcludedState( void ) { return true; }

	// Retrieve the vtf flags mask
	virtual unsigned int GetFlags( void ) const { return 0; }

	virtual void ForceLODOverride( int iNumLodsOverrideUpOrDown ) { NULL; }

	virtual void ReloadFilesInList( IFileList *pFilesToReload ) {}

protected:
#ifdef _DEBUG
	char *m_pDebugName;
#endif

	CUtlSymbol m_Name;

	// What texture group this texture is in (winds up setting counters based on the group name,
	// then the budget panel views the counters).
	CUtlSymbol m_TextureGroupName;

	// The set of texture ids for each animation frame
	ShaderAPITextureHandle_t m_hTexture;

	// Refcount
	int m_nRefCount;

public:
	virtual void DeleteIfUnreferenced();

#if defined( _X360 )
	virtual bool ClearTexture( int r, int g, int b, int a ) { return false; }
	virtual bool CreateRenderTargetSurface( int width, int height, ImageFormat format, bool bSameAsTexture ) { return false; }
#endif

	void FixupTexture( const void *pData, int nSize, LoaderError_t loaderError ) { NULL; }

	void SwapContents( ITexture *pOther ) { NULL; }

public:
	void SetName( char const *szName );
	void InitFromHandle(
		const char *pTextureName,
		const char *pTextureGroupName,
		ShaderAPITextureHandle_t hTexture );
};

CReferenceToHandleTexture::CReferenceToHandleTexture() :
	m_hTexture( INVALID_SHADERAPI_TEXTURE_HANDLE ),
#ifdef _DEBUG
	m_pDebugName( NULL ),
#endif
	m_nRefCount( 0 )
{
	NULL;
}

CReferenceToHandleTexture::~CReferenceToHandleTexture()
{
#ifdef _DEBUG
	if ( m_nRefCount != 0 )
	{
		Warning( "Reference Count(%d) != 0 in ~CReferenceToHandleTexture for texture \"%s\"\n", m_nRefCount, m_Name.String() );
	}
	if ( m_pDebugName )
	{
		delete [] m_pDebugName;
	}
#endif
}

void CReferenceToHandleTexture::SetName( char const *szName )
{
	// normalize and convert to a symbol
	char szCleanName[MAX_PATH];
	m_Name = NormalizeTextureName( szName, szCleanName, sizeof( szCleanName ) );

#ifdef _DEBUG
	if ( m_pDebugName )
	{
		delete [] m_pDebugName;
	}
	int nLen = V_strlen( szCleanName ) + 1;
	m_pDebugName = new char[nLen];
	V_memcpy( m_pDebugName, szCleanName, nLen );
#endif
}

void CReferenceToHandleTexture::InitFromHandle( const char *pTextureName, const char *pTextureGroupName, ShaderAPITextureHandle_t hTexture )
{
	SetName( pTextureName );
	m_TextureGroupName = pTextureGroupName;
	m_hTexture = hTexture;
}

void CReferenceToHandleTexture::Bind( Sampler_t sampler )
{
	if ( g_pShaderDevice->IsUsingGraphics() )
	{
		g_pShaderAPI->BindTexture( sampler, m_hTexture );
	}
}


// TODO: make paired textures work with mat_texture_list
void CReferenceToHandleTexture::Bind( Sampler_t sampler1, int nFrame, Sampler_t sampler2 /* = -1 */ )
{
	if ( g_pShaderDevice->IsUsingGraphics() )
	{
		g_pShaderAPI->BindTexture( sampler1, m_hTexture );
	}
}


void CReferenceToHandleTexture::BindVertexTexture( VertexTextureSampler_t sampler, int nFrame )
{
	if ( g_pShaderDevice->IsUsingGraphics() )
	{
		g_pShaderAPI->BindVertexTexture( sampler, m_hTexture );
	}
}

void CReferenceToHandleTexture::DeleteIfUnreferenced()
{
	if ( m_nRefCount > 0 )
		return;

	TextureManager()->RemoveTexture( this );
}


//-----------------------------------------------------------------------------
// Fixed-size allocator
//-----------------------------------------------------------------------------
//DEFINE_FIXEDSIZE_ALLOCATOR( CTexture, 1024, true );


//-----------------------------------------------------------------------------
// Static instance of VTF texture
//-----------------------------------------------------------------------------
static IVTFTexture *s_pVTFTexture = NULL;
static IVTFTexture *s_pVTFTexturePaired = NULL;

static void *s_pOptimalReadBuffer;
static int s_nOptimalReadBufferSize;

//-----------------------------------------------------------------------------
// Class factory methods
//-----------------------------------------------------------------------------
ITextureInternal *ITextureInternal::CreateFileTexture( const char *pFileName, const char *pTextureGroupName )
{
	CTexture *pTex = new CTexture;
	pTex->InitFileTexture( pFileName, pTextureGroupName );
	return pTex;
}

ITextureInternal *ITextureInternal::CreateReferenceTextureFromHandle(
	const char *pTextureName,
	const char *pTextureGroupName,
	ShaderAPITextureHandle_t hTexture )
{
	CReferenceToHandleTexture *pTex = new CReferenceToHandleTexture;
	pTex->InitFromHandle( pTextureName, pTextureGroupName, hTexture );
	return pTex;
}

ITextureInternal *ITextureInternal::CreateProceduralTexture( 
	const char			*pTextureName, 
	const char			*pTextureGroupName, 
	int					w, 
	int					h, 
	int					d,
	ImageFormat			fmt, 
	int					nFlags )
{
	CTexture *pTex = new CTexture;
	pTex->InitProceduralTexture( pTextureName, pTextureGroupName, w, h, d, fmt, nFlags );
	pTex->IncrementReferenceCount();
	return pTex;
}

// GR - named RT
ITextureInternal *ITextureInternal::CreateRenderTarget( 
	const char *pRTName, 
	int w, 
	int h, 
	RenderTargetSizeMode_t sizeMode, 
	ImageFormat fmt, 
	RenderTargetType_t type, 
	unsigned int textureFlags, 
	unsigned int renderTargetFlags )
{
	CTexture *pTex = new CTexture;
	pTex->InitRenderTarget( pRTName, w, h, sizeMode, fmt, type, textureFlags, renderTargetFlags );

	return pTex;
}

//-----------------------------------------------------------------------------
// Rebuild and exisiting render target in place.
//-----------------------------------------------------------------------------
void ITextureInternal::ChangeRenderTarget( 
	ITextureInternal *pTex,
	int w,
	int	h,
	RenderTargetSizeMode_t sizeMode, 
	ImageFormat fmt, 
	RenderTargetType_t type, 
	unsigned int textureFlags, 
	unsigned int renderTargetFlags )
{
	pTex->Release();
	dynamic_cast< CTexture * >(pTex)->InitRenderTarget( pTex->GetName(), w, h, sizeMode, fmt, type, textureFlags, renderTargetFlags );
}

void ITextureInternal::Destroy( ITextureInternal *pTex )
{
	delete pTex;
}

//-----------------------------------------------------------------------------
// Constructor, destructor
//-----------------------------------------------------------------------------
CTexture::CTexture() : m_ImageFormat( IMAGE_FORMAT_UNKNOWN )
{
	m_nActualMipCount = 0;
	m_nMappingWidth = 0;
	m_nMappingHeight = 0;
	m_nMappingDepth = 1;
	m_nActualWidth = 0;
	m_nActualHeight = 0;
	m_nActualDepth = 1;
	m_nRefCount = 0;
	m_nFlags = 0;
	m_nInternalFlags = 0;
	m_pTextureHandles = NULL;
	m_nFrameCount = 0;
	VectorClear( m_vecReflectivity );
	m_pTextureRegenerator = NULL;
	m_nOriginalRenderTargetType = NO_RENDER_TARGET;
	m_RenderTargetSizeMode = RT_SIZE_NO_CHANGE;
	m_nOriginalRTWidth = m_nOriginalRTHeight = 1;

	m_LowResImageWidth = 0;
	m_LowResImageHeight = 0;
#if !defined( _X360 )
	m_pLowResImage = NULL;
#else
	*(unsigned int *)m_LowResImageSample = 0;
#endif

	m_nDesiredDimensionLimit = 0;
	m_nActualDimensionLimit = 0;

#if defined( COMPRESSED_NORMAL_FORMATS )
	m_bPaired = false;
	m_pTextureHandlesPaired = NULL;
	m_ImageFormatPaired = IMAGE_FORMAT_UNKNOWN;
#endif

#ifdef _DEBUG
	m_pDebugName = NULL;
#endif
}

CTexture::~CTexture()
{
#ifdef _DEBUG
	if ( m_nRefCount != 0 )
	{
		Warning( "Reference Count(%d) != 0 in ~CTexture for texture \"%s\"\n", m_nRefCount, m_Name.String() );
	}
	if ( m_pDebugName )
	{
		delete [] m_pDebugName;
	}
#endif

	Shutdown();
}


//-----------------------------------------------------------------------------
// Initializes the texture
//-----------------------------------------------------------------------------
void CTexture::Init( int w, int h, int d, ImageFormat fmt, int iFlags, int iFrameCount, bool bPaired )
{
	Assert( iFrameCount > 0 );

	// Assume unpaired was initialized first, as we piggyback off of the shared values such as height, width etc
#if defined( COMPRESSED_NORMAL_FORMATS )
	if ( bPaired )
	{
		m_ImageFormatPaired = fmt;

		ReleaseTextureHandles( bPaired );
		AllocateTextureHandles( bPaired );
	}
	else
#endif
	{
		// This is necessary to prevent blowing away the allocated state,
		// which is necessary for the ReleaseTextureHandles call below to work.
		m_nInternalFlags &= ~TEXTUREFLAGSINTERNAL_ERROR;

		// free and release previous data
		// cannot change to new intialization parameters yet
		FreeShaderAPITextures();
		ReleaseTextureHandles();

		// update to new initialization parameters
		// these are the *desired* new values
		m_nMappingWidth = w;
		m_nMappingHeight = h;
		m_nMappingDepth = d;
		m_ImageFormat = fmt;
		m_nFrameCount = iFrameCount;
		// We don't know the actual width and height until we get it ready to render
		m_nActualWidth = m_nActualHeight = 0;
		m_nActualDepth = 1;
		m_nActualMipCount = 0;
		m_nFlags = iFlags;

		AllocateTextureHandles();
	}
}


//-----------------------------------------------------------------------------
// Shuts down the texture
//-----------------------------------------------------------------------------
void CTexture::Shutdown()
{
	// Clean up the low-res texture
#if !defined( _X360 )
	delete[] m_pLowResImage;
	m_pLowResImage = 0;
#endif

	// Clean up the resources data
	for ( DataChunk const *pDataChunk = m_arrDataChunks.Base(),
		  *pDataChunkEnd = pDataChunk + m_arrDataChunks.Count();
		  pDataChunk < pDataChunkEnd; ++pDataChunk )
	{
		pDataChunk->Deallocate();
	}
	m_arrDataChunks.RemoveAll();

	// Frees the texture regen class
	if ( m_pTextureRegenerator )
	{
		m_pTextureRegenerator->Release();
		m_pTextureRegenerator = NULL;
	}

	// This deletes the textures
	FreeShaderAPITextures();
	ReleaseTextureHandles();
}

void CTexture::Release()
{
	FreeShaderAPITextures();
}

IVTFTexture *CTexture::GetScratchVTFTexture( bool bPaired )
{
	if ( bPaired )
	{
		if ( !s_pVTFTexturePaired )
		{
			s_pVTFTexturePaired = CreateVTFTexture();
		}
		return s_pVTFTexturePaired;
	}
	else
	{
		if ( !s_pVTFTexture )
		{
			s_pVTFTexture = CreateVTFTexture();
		}
		return s_pVTFTexture;
	}
}

//-----------------------------------------------------------------------------
// Get an optimal read buffer, persists and avoids excessive allocations
//-----------------------------------------------------------------------------
int CTexture::GetOptimalReadBuffer( FileHandle_t hFile, int nSize, CUtlBuffer &optimalBuffer )
{
	// get an optimal read buffer, only resize if necessary
	int minSize = IsX360() ? 0 : 2 * 1024 * 1024;	// 360 has no min, PC uses 2MB min to avoid fragmentation
	nSize = max(nSize, minSize);
	int nBytesOptimalRead = g_pFullFileSystem->GetOptimalReadSize( hFile, nSize );
	if ( nBytesOptimalRead > s_nOptimalReadBufferSize )
	{
		FreeOptimalReadBuffer( 0 );

		s_nOptimalReadBufferSize = nBytesOptimalRead;
		s_pOptimalReadBuffer = g_pFullFileSystem->AllocOptimalReadBuffer( hFile, nSize );
	}

	// set external buffer and reset to empty
	optimalBuffer.SetExternalBuffer( s_pOptimalReadBuffer, s_nOptimalReadBufferSize, 0 );

	// return the optimal read size
	return nBytesOptimalRead;
}

//-----------------------------------------------------------------------------
// Free the optimal read buffer if it grows too large
//-----------------------------------------------------------------------------
void CTexture::FreeOptimalReadBuffer( int nMaxSize )
{
	if ( s_pOptimalReadBuffer && s_nOptimalReadBufferSize >= nMaxSize )
	{
		g_pFullFileSystem->FreeOptimalReadBuffer( s_pOptimalReadBuffer );
		s_pOptimalReadBuffer = NULL;
		s_nOptimalReadBufferSize = 0;
	}
}
//-----------------------------------------------------------------------------
//
// Various initialization methods
//
//-----------------------------------------------------------------------------


void CTexture::ApplyRenderTargetSizeMode( int &width, int &height, ImageFormat fmt )
{
	width = m_nOriginalRTWidth;
	height = m_nOriginalRTHeight;

	switch ( m_RenderTargetSizeMode )
	{
		case RT_SIZE_FULL_FRAME_BUFFER:
		{
			MaterialSystem()->GetBackBufferDimensions( width, height );
			if( !HardwareConfig()->SupportsNonPow2Textures() )
			{
				width = FloorPow2( width + 1 );
				height = FloorPow2( height + 1 );
			}
		}
		break;

		case RT_SIZE_FULL_FRAME_BUFFER_ROUNDED_UP:
		{
			MaterialSystem()->GetBackBufferDimensions( width, height );
			if( !HardwareConfig()->SupportsNonPow2Textures() )
			{
				width = CeilPow2( width );
				height = CeilPow2( height );
			}
		}
		break;

		case RT_SIZE_PICMIP:
		{
			int fbWidth, fbHeight;
			MaterialSystem()->GetBackBufferDimensions( fbWidth, fbHeight );
			int picmip = g_config.skipMipLevels;
			while( picmip > 0 )
			{
				width >>= 1;
				height >>= 1;
				picmip--;
			}

			while( width > fbWidth )
			{
				width >>= 1;
			}
			while( height > fbHeight )
			{
				height >>= 1;
			}
		}
		break;

		case RT_SIZE_DEFAULT:
		{
			// Assume that the input is pow2.
			Assert( ( width & ( width - 1 ) ) == 0 );
			Assert( ( height & ( height - 1 ) ) == 0 );
			int fbWidth, fbHeight;
			MaterialSystem()->GetBackBufferDimensions( fbWidth, fbHeight );
			while( width > fbWidth )
			{
				width >>= 1;
			}
			while( height > fbHeight )
			{
				height >>= 1;
			}
		}
		break;

		case RT_SIZE_HDR:
		{
			MaterialSystem()->GetBackBufferDimensions( width, height );
			width = width / 4;
			height = height / 4;
		}
		break;

		case RT_SIZE_OFFSCREEN:
		{
			int fbWidth, fbHeight;
			MaterialSystem()->GetBackBufferDimensions( fbWidth, fbHeight );

			// On 360, don't do this resizing for formats related to the shadow depth texture
#if defined( _X360 )
			if ( !( (fmt == IMAGE_FORMAT_X360_DST16) || (fmt == IMAGE_FORMAT_X360_DST24) || (fmt == IMAGE_FORMAT_X360_DST24F) || (fmt == IMAGE_FORMAT_BGR565) ) )
#endif
			{
				// Shrink the buffer if it's bigger than back buffer.  Otherwise, don't mess with it.
				while( (width > fbWidth) || (height > fbHeight) )
				{
					width >>= 1;
					height >>= 1;
				}
			}

		}
		break;

		default:
		{
			Assert( m_RenderTargetSizeMode == RT_SIZE_NO_CHANGE );
			Assert( m_nOriginalRenderTargetType == RENDER_TARGET_NO_DEPTH );	// Only can use NO_CHANGE if we don't have a depth buffer.
		}
		break;
	}
}



//-----------------------------------------------------------------------------
// Creates named render target texture
//-----------------------------------------------------------------------------
void CTexture::InitRenderTarget( 
	const char *pRTName, 
	int w, 
	int h, 
	RenderTargetSizeMode_t sizeMode, 
	ImageFormat fmt, 
	RenderTargetType_t type, 
	unsigned int textureFlags,
	unsigned int renderTargetFlags )
{
	if ( pRTName )
	{
		SetName( pRTName );
	}
	else
	{
		static int id = 0;
		char pName[128];
		Q_snprintf( pName, sizeof( pName ), "__render_target_%d", id );
		++id;
		SetName( pName );
	}

	if ( renderTargetFlags & CREATERENDERTARGETFLAGS_HDR )
	{
		if ( HardwareConfig()->GetHDRType() == HDR_TYPE_FLOAT )
		{
			// slam the format
			fmt = IMAGE_FORMAT_RGBA16161616F;
		}
	}

	int nFrameCount = 1;

	int nFlags = TEXTUREFLAGS_NOMIP | TEXTUREFLAGS_RENDERTARGET;
	nFlags |= textureFlags;

	if ( type == RENDER_TARGET_NO_DEPTH )
	{
		nFlags |= TEXTUREFLAGS_NODEPTHBUFFER;
	}
	else if ( type == RENDER_TARGET_WITH_DEPTH || type == RENDER_TARGET_ONLY_DEPTH || g_pShaderAPI->DoRenderTargetsNeedSeparateDepthBuffer() )
	{
		nFlags |= TEXTUREFLAGS_DEPTHRENDERTARGET;
		++nFrameCount;
	}

	if ( IsX360() )
	{
		// 360 RT needs its coupled surface, expected at [nFrameCount-1]
		++nFrameCount;
	}

	if ( renderTargetFlags & CREATERENDERTARGETFLAGS_TEMP )
	{
		m_nInternalFlags |= TEXTUREFLAGSINTERNAL_TEMPRENDERTARGET;
	}

	m_nOriginalRenderTargetType = type;
	m_RenderTargetSizeMode = sizeMode;
	m_nOriginalRTWidth = w;
	m_nOriginalRTHeight = h;

	ApplyRenderTargetSizeMode( w, h, fmt );

	Init( w, h, 1, fmt, nFlags, nFrameCount, false );
	m_TextureGroupName = TEXTURE_GROUP_RENDER_TARGET;
}


void CTexture::OnRestore()
{ 
	// May have to change whether or not we have a depth buffer.
	// Are we a render target?
	if ( IsPC() && ( m_nFlags & TEXTUREFLAGS_RENDERTARGET ) )
	{
		// Did they not ask for a depth buffer?
		if ( m_nOriginalRenderTargetType == RENDER_TARGET )
		{
			// But, did we force them to have one, or should we force them to have one this time around?
			bool bShouldForce = g_pShaderAPI->DoRenderTargetsNeedSeparateDepthBuffer();
			bool bDidForce = ((m_nFlags & TEXTUREFLAGS_DEPTHRENDERTARGET) != 0);
			if ( bShouldForce != bDidForce )
			{
				int nFlags = m_nFlags;
				int iFrameCount = m_nFrameCount;
				if ( bShouldForce )
				{
					Assert( !( nFlags & TEXTUREFLAGS_DEPTHRENDERTARGET ) );
					iFrameCount = 2;
					nFlags |= TEXTUREFLAGS_DEPTHRENDERTARGET;
				}
				else
				{
					Assert( ( nFlags & TEXTUREFLAGS_DEPTHRENDERTARGET ) );
					iFrameCount = 1;
					nFlags &= ~TEXTUREFLAGS_DEPTHRENDERTARGET;
				}

				Shutdown();
				
				int newWidth, newHeight;
				ApplyRenderTargetSizeMode( newWidth, newHeight, m_ImageFormat );
				
				Init( newWidth, newHeight, 1, m_ImageFormat, nFlags, iFrameCount, false );
				return;
			}
		}

		// If we didn't recreate it up above, then we may need to resize it anyway if the framebuffer
		// got smaller than we are.
		int newWidth, newHeight;
		ApplyRenderTargetSizeMode( newWidth, newHeight, m_ImageFormat );
		if ( newWidth != m_nMappingWidth || newHeight != m_nMappingHeight )
		{
			Shutdown();
			Init( newWidth, newHeight, 1, m_ImageFormat, m_nFlags, m_nFrameCount, false );
			return;
		}
	}
}


//-----------------------------------------------------------------------------
// Creates a procedural texture
//-----------------------------------------------------------------------------
void CTexture::InitProceduralTexture( const char *pTextureName, const char *pTextureGroupName, int w, int h, int d, ImageFormat fmt, int nFlags )
{
	// Compressed textures aren't allowed for procedural textures
	Assert( !ImageLoader::IsCompressed( fmt ) );

	// We shouldn't be asking for render targets here
	Assert( (nFlags & (TEXTUREFLAGS_RENDERTARGET | TEXTUREFLAGS_DEPTHRENDERTARGET)) == 0 );

	SetName( pTextureName );

	// Eliminate flags that are inappropriate...
	nFlags &= ~TEXTUREFLAGS_HINT_DXT5 | TEXTUREFLAGS_ONEBITALPHA | TEXTUREFLAGS_EIGHTBITALPHA | 
		TEXTUREFLAGS_RENDERTARGET | TEXTUREFLAGS_DEPTHRENDERTARGET;

	// Insert required flags
	nFlags |= TEXTUREFLAGS_PROCEDURAL;
	int nAlphaBits = ImageLoader::ImageFormatInfo(fmt).m_NumAlphaBits;
	if (nAlphaBits > 1)
	{
		nFlags |= TEXTUREFLAGS_EIGHTBITALPHA;
	}
	else if (nAlphaBits == 1)
	{
		nFlags |= TEXTUREFLAGS_ONEBITALPHA;
	}
	
	// Procedural textures are always one frame only
	Init( w, h, d, fmt, nFlags, 1, false );

	m_TextureGroupName = pTextureGroupName;
}


//-----------------------------------------------------------------------------
// Creates a file texture
//-----------------------------------------------------------------------------
void CTexture::InitFileTexture( const char *pTextureFile, const char *pTextureGroupName )
{
	// For files, we only really know about the file name
	// At any time, the file contents could change, and we could have
	// a different size, number of frames, etc.
	SetName( pTextureFile );
	m_TextureGroupName = pTextureGroupName;
}

//-----------------------------------------------------------------------------
// Assigns/releases texture IDs for our animation frames
//-----------------------------------------------------------------------------
void CTexture::AllocateTextureHandles( bool bPaired /* = false */ )
{
	if ( !bPaired )
	{
		Assert( !m_pTextureHandles );
		Assert( m_nFrameCount > 0 );

		m_pTextureHandles = new ShaderAPITextureHandle_t[m_nFrameCount];
		for( int i = 0; i != m_nFrameCount; ++i )
			m_pTextureHandles[i] = INVALID_SHADERAPI_TEXTURE_HANDLE;
	}
#if defined( COMPRESSED_NORMAL_FORMATS )
	else
	{
		Assert( !m_pTextureHandlesPaired );
		Assert( m_nFrameCount > 0 );

		m_pTextureHandlesPaired = new ShaderAPITextureHandle_t[m_nFrameCount];
		for( int i = 0; i != m_nFrameCount; ++i )
			m_pTextureHandlesPaired[i] = INVALID_SHADERAPI_TEXTURE_HANDLE;
		
		m_bPaired = true;
	}
#endif
}

void CTexture::ReleaseTextureHandles( bool bPaired /* = false */ )
{
	if ( !bPaired )
	{
		if ( m_pTextureHandles )
		{
			delete[] m_pTextureHandles;
			m_pTextureHandles = NULL;
		}
	}
#if defined( COMPRESSED_NORMAL_FORMATS )
	else
	{
		if ( m_pTextureHandlesPaired )
		{
			delete[] m_pTextureHandlesPaired;
			m_pTextureHandlesPaired = NULL;
		}
	}
#endif
}


//-----------------------------------------------------------------------------
// Creates the texture
//-----------------------------------------------------------------------------
void CTexture::AllocateShaderAPITextures()
{
	Assert( !HasBeenAllocated() );

	int nCount = m_nFrameCount;

	int nCreateFlags = 0;
	if ( ( m_nFlags & TEXTUREFLAGS_ENVMAP ) && HardwareConfig()->SupportsCubeMaps() )
	{
		nCreateFlags |= TEXTURE_CREATE_CUBEMAP;
	}

	if ( m_nFlags & TEXTUREFLAGS_RENDERTARGET )
	{
		nCreateFlags |= TEXTURE_CREATE_RENDERTARGET;

		// This here is simply so we can use a different call to
		// create the depth texture below	
		if ( ( m_nFlags & TEXTUREFLAGS_DEPTHRENDERTARGET ) &&
			 ( (IsPC() && (nCount == 2)) || (IsX360() && (nCount == 3)) ) ) //nCount must be 2 on pc, must be 3 on 360
		{
			--nCount;
		}
	}
	else
	{
		// If it's not a render target, use the texture manager in dx
		nCreateFlags |= TEXTURE_CREATE_MANAGED;
	}

	if ( m_nFlags & TEXTUREFLAGS_POINTSAMPLE )
	{
		nCreateFlags |= TEXTURE_CREATE_UNFILTERABLE_OK;
	}

	if ( m_nFlags & TEXTUREFLAGS_VERTEXTEXTURE )
	{
		nCreateFlags |= TEXTURE_CREATE_VERTEXTEXTURE;
	}

	int nCopies = 1;
	if ( IsProcedural() )
	{
		// This is sort of hacky... should we store the # of copies in the VTF?
		if ( !( m_nFlags & TEXTUREFLAGS_SINGLECOPY ) )
		{
			// FIXME: That 6 there is heuristically what I came up with what I
			// need to get eyes not to stall on map alyx3. We need a better way
			// of determining how many copies of the texture we should store.
			nCopies = 6;
		}
	}

	if ( IsX360() )
	{
		if ( m_nFlags & TEXTUREFLAGS_PROCEDURAL )
		{
			nCreateFlags |= TEXTURE_CREATE_CANCONVERTFORMAT;
		}
		if ( m_nFlags & TEXTUREFLAGS_RENDERTARGET )
		{
			// 360 render targets allocates one additional handle for optional EDRAM surface
			--nCount;
			m_pTextureHandles[m_nFrameCount - 1] = INVALID_SHADERAPI_TEXTURE_HANDLE; 
		}
		if ( m_nInternalFlags & TEXTUREFLAGSINTERNAL_QUEUEDLOAD )
		{
			// queued load, no d3d bits until data arrival
			nCreateFlags |= TEXTURE_CREATE_NOD3DMEMORY;

			// Artificially increment reference count (per frame) to ensure
			// a queued texture stays resident until it's wholly finalized.
			m_nRefCount += nCount;
		}
	}								   

	// For depth only render target: adjust texture width/height
	// Currently we just leave it the same size, will update with further testing
	int nShaderApiCreateTextureDepth = ( ( m_nFlags & TEXTUREFLAGS_DEPTHRENDERTARGET ) && ( m_nOriginalRenderTargetType == RENDER_TARGET_ONLY_DEPTH ) ) ? 1 : m_nActualDepth;

	// Create all animated texture frames in a single call
	g_pShaderAPI->CreateTextures(
		m_pTextureHandles, nCount,
		m_nActualWidth, m_nActualHeight, nShaderApiCreateTextureDepth, m_ImageFormat, m_nActualMipCount,
		nCopies, nCreateFlags, GetName(), GetTextureGroupName() );

#if defined( COMPRESSED_NORMAL_FORMATS )
	if ( m_bPaired )
	{
		g_pShaderAPI->CreateTextures(
			m_pTextureHandlesPaired, nCount,
			m_nActualWidth, m_nActualHeight, nShaderApiCreateTextureDepth, m_ImageFormatPaired, m_nActualMipCount,
			nCopies, nCreateFlags, GetName(), GetTextureGroupName() );
	}
#endif

	// Create the depth render target buffer
	if ( m_nFlags & TEXTUREFLAGS_DEPTHRENDERTARGET )
	{
		MEM_ALLOC_CREDIT();
		Assert( nCount == 1 );

		char debugName[128];
		sprintf( debugName, "%s_ZBuffer", GetName() );

		m_pTextureHandles[1] = g_pShaderAPI->CreateDepthTexture( 
				m_ImageFormat, 
				m_nActualWidth, 
				m_nActualHeight,
				debugName,
				( m_nOriginalRenderTargetType == RENDER_TARGET_ONLY_DEPTH ) );
	}

	m_nInternalFlags |= TEXTUREFLAGSINTERNAL_ALLOCATED;
}


//-----------------------------------------------------------------------------
// Releases the texture's hardware memory
//-----------------------------------------------------------------------------
void CTexture::FreeShaderAPITextures()
{
	if ( m_pTextureHandles && HasBeenAllocated() )
	{
		// Release the frames
		for ( int i = m_nFrameCount; --i >= 0; )
		{
			if ( g_pShaderAPI->IsTexture( m_pTextureHandles[i] ) )
			{
#ifndef _LINUX
				Assert( _heapchk() == _HEAPOK );
#endif

				g_pShaderAPI->DeleteTexture( m_pTextureHandles[i] );
				m_pTextureHandles[i] = INVALID_SHADERAPI_TEXTURE_HANDLE;
			}
		}

#if defined( COMPRESSED_NORMAL_FORMATS )
		// Do the same for the paired handles if they exist
		if ( m_bPaired && m_pTextureHandlesPaired )
		{
			for ( int i = m_nFrameCount; --i >= 0; )
			{
				if ( g_pShaderAPI->IsTexture( m_pTextureHandlesPaired[i] ) )
				{
					g_pShaderAPI->DeleteTexture( m_pTextureHandlesPaired[i] );
					m_pTextureHandlesPaired[i] = INVALID_SHADERAPI_TEXTURE_HANDLE;
				}
			}
		}
#endif
	}
	m_nInternalFlags &= ~TEXTUREFLAGSINTERNAL_ALLOCATED;
}


//-----------------------------------------------------------------------------
// Computes the actual format of the texture
//-----------------------------------------------------------------------------
ImageFormat CTexture::ComputeActualFormat( ImageFormat srcFormat )
{
	ImageFormat dstFormat;
	bool bIsCompressed = ImageLoader::IsCompressed( srcFormat );
	if ( g_config.bCompressedTextures && HardwareConfig()->SupportsCompressedTextures() && bIsCompressed )
	{
		// don't do anything since we are already in a compressed format.
		dstFormat = g_pShaderAPI->GetNearestSupportedFormat( srcFormat );
		Assert( dstFormat == srcFormat );
		return dstFormat;
	}

	if ( IsX360() && ( srcFormat == IMAGE_FORMAT_A8 ) )
	{
		// these are the right alpha formats for xbox
		return IMAGE_FORMAT_A8;
	}

	// NOTE: Below this piece of code is only called when compressed textures are
	// turned off, or if the source texture is not compressed.

	// We use the TEXTUREFLAGS_EIGHTBITALPHA and TEXTUREFLAGS_ONEBITALPHA flags
	// to decide how many bits of alpha we need; vtex checks the alpha channel
	// for all white, etc.
	if( (srcFormat == IMAGE_FORMAT_UVWQ8888) || ( srcFormat == IMAGE_FORMAT_UV88 ) || 
		( srcFormat == IMAGE_FORMAT_UVLX8888 ) || ( srcFormat == IMAGE_FORMAT_RGBA16161616 ) ||
		( srcFormat == IMAGE_FORMAT_RGBA16161616F ) )
	{
		dstFormat = g_pShaderAPI->GetNearestSupportedFormat( srcFormat );
	} 
	else if ( m_nFlags & ( TEXTUREFLAGS_EIGHTBITALPHA | TEXTUREFLAGS_ONEBITALPHA ) )
	{
		dstFormat = g_pShaderAPI->GetNearestSupportedFormat( IMAGE_FORMAT_BGRA8888 );
	}
	else if ( srcFormat == IMAGE_FORMAT_I8 )
	{
		dstFormat = g_pShaderAPI->GetNearestSupportedFormat( IMAGE_FORMAT_I8 );
	}
	else
	{
		dstFormat = g_pShaderAPI->GetNearestSupportedFormat( IMAGE_FORMAT_BGR888 );
	}
	return dstFormat;
}


//-----------------------------------------------------------------------------
// Compute the actual mip count based on the actual size
//-----------------------------------------------------------------------------
int CTexture::ComputeActualMipCount( ) const
{
	if ( m_nFlags & TEXTUREFLAGS_ENVMAP )
	{
		if ( !HardwareConfig()->SupportsMipmappedCubemaps() )
		{
			return 1;
		}
	}

	if ( m_nFlags & TEXTUREFLAGS_NOMIP )
	{
		return 1;
	}

	// Unless ALLMIPS is set, we stop mips at 32x32
	const int iMaxMipSize = 32;
	if ( !( m_nFlags & TEXTUREFLAGS_ALL_MIPS ) )
	{
		int nNumMipLevels = 1;
		int h = m_nActualWidth;
		int w = m_nActualHeight;
		while (	min( w, h ) > iMaxMipSize )
		{
			++nNumMipLevels;
			
			w >>= 1;
			h >>= 1;
		}
		return nNumMipLevels;
	}

	return ImageLoader::GetNumMipMapLevels( m_nActualWidth, m_nActualHeight, m_nActualDepth );
}

//-----------------------------------------------------------------------------
// Calculates info about whether we can make the texture smaller and by how much
//-----------------------------------------------------------------------------
int CTexture::ComputeActualSize( bool bIgnorePicmip, IVTFTexture *pVTFTexture )
{
	// Must skip mip levels if the texture is too large for our board to handle
	m_nActualWidth = m_nMappingWidth;
	m_nActualHeight = m_nMappingHeight;
	m_nActualDepth = m_nMappingDepth;

	int nClampX = m_nActualWidth;	// no clamping (clamp to texture dimensions)
	int nClampY = m_nActualHeight;
	int nClampZ = m_nActualDepth;

	//
	// PC:
	// Fetch clamping dimensions from special LOD control settings block
	// or runtime texture lod override.
	//
	if ( IsPC() )
	{
		// Fetch LOD settings from the VTF if available
		TextureLODControlSettings_t lcs;
		memset( &lcs, 0, sizeof( lcs ) );
		TextureLODControlSettings_t const *pLODInfo = NULL;
		if ( pVTFTexture )
		{
			pLODInfo = reinterpret_cast<TextureLODControlSettings_t const *> (
					pVTFTexture->GetResourceData( VTF_RSRC_TEXTURE_LOD_SETTINGS, NULL ) );
			if ( pLODInfo )
				lcs = *pLODInfo;
		}

		// Prepare the default LOD settings (that essentially result in no clamping)
		TextureLODControlSettings_t default_lod_settings;
		memset( &default_lod_settings, 0, sizeof( default_lod_settings ) );
		{
			for ( int w = m_nActualWidth; w > 1; w >>= 1 )
				  ++ default_lod_settings.m_ResolutionClampX;
			for ( int h = m_nActualHeight; h > 1; h >>= 1 )
				  ++ default_lod_settings.m_ResolutionClampY;
		}

		// Check for LOD control override
		{
			TextureLodOverride::OverrideInfo oi = TextureLodOverride::Get( GetName() );
			
			if ( oi.x && oi.y && !pLODInfo )	// If overriding texture that doesn't have lod info yet, then use default
				lcs = default_lod_settings;

			lcs.m_ResolutionClampX += oi.x;
			lcs.m_ResolutionClampY += oi.y;
			if ( int8( lcs.m_ResolutionClampX ) < 0 )
				lcs.m_ResolutionClampX = 0;
			if ( int8( lcs.m_ResolutionClampY ) < 0 )
				lcs.m_ResolutionClampY = 0;
		}

		// Compute the requested mip0 dimensions
		if ( lcs.m_ResolutionClampX && lcs.m_ResolutionClampY )
		{
			nClampX = (1 << lcs.m_ResolutionClampX );
			nClampY = (1 << lcs.m_ResolutionClampY );
		}

		// In case clamp values exceed texture dimensions, then fix up
		// the clamping values
		nClampX = min( nClampX, m_nActualWidth );
		nClampY = min( nClampY, m_nActualHeight );
	}

	//
	// Honor dimension limit restrictions
	//
	if ( m_nDesiredDimensionLimit > 0 )
	{
		while ( nClampX > m_nDesiredDimensionLimit ||
				nClampY > m_nDesiredDimensionLimit )
		{
			nClampX >>= 1;
			nClampY >>= 1;
		}
	}

	//
	// Unless ignoring picmip, reflect the global picmip level in clamp dimensions
	//
	if ( !bIgnorePicmip )
	{
		// If picmip requests texture degradation, then honor it
		// for loddable textures only
		if ( !( m_nFlags & TEXTUREFLAGS_NOLOD ) &&
			  ( g_config.skipMipLevels > 0 ) )
		{
			for ( int iDegrade = 0; iDegrade < g_config.skipMipLevels; ++ iDegrade )
			{
				// don't go lower than 4, or dxt textures won't work properly
				if ( nClampX > 4 &&
					 nClampY > 4 )
				{
					nClampX >>= 1;
					nClampY >>= 1;
				}
			}
		}

		// If picmip requests quality upgrade, then always honor it
		if ( g_config.skipMipLevels < 0 )
		{
			for ( int iUpgrade = 0; iUpgrade < - g_config.skipMipLevels; ++ iUpgrade )
			{
				if ( nClampX < m_nActualWidth &&
					 nClampY < m_nActualHeight )
				{
					nClampX <<= 1;
					nClampY <<= 1;
				}
				else
					break;
			}
		}
	}

	//
	// Now use hardware settings to clamp our "clamping dimensions"
	//
	int iHwWidth = HardwareConfig()->MaxTextureWidth();
	int iHwHeight = HardwareConfig()->MaxTextureHeight();
	int iHwDepth = HardwareConfig()->MaxTextureDepth();

	nClampX = min( nClampX, max( iHwWidth, 4 ) );
	nClampY = min( nClampY, max( iHwHeight, 4 ) );
	nClampZ = min( nClampZ, max( iHwDepth, 1 ) );

	Assert( nClampZ >= 1 );

	// In case clamp values exceed texture dimensions, then fix up
	// the clamping values.
	nClampX = min( nClampX, m_nActualWidth );
	nClampY = min( nClampY, m_nActualHeight );
	nClampZ = min( nClampZ, m_nActualDepth );
	
	//
	// Clamp to the determined dimensions
	//
	int numMipsSkipped = 0; // will compute now when clamping how many mips we drop
	while ( ( m_nActualWidth  > nClampX ) ||
		    ( m_nActualHeight > nClampY ) ||
			( m_nActualDepth  > nClampZ ) )
	{
		m_nActualWidth  >>= 1;
		m_nActualHeight >>= 1;
		m_nActualDepth  >>= 1;
		if ( m_nActualDepth < 1 )
			m_nActualDepth = 1;

		++ numMipsSkipped;
	}

	Assert( m_nActualWidth > 0 && m_nActualHeight > 0 && m_nActualDepth > 0 );

	// Now that we've got the actual size, we can figure out the mip count
	m_nActualMipCount = ComputeActualMipCount();

	// Returns the number we skipped
	return numMipsSkipped;
}


//-----------------------------------------------------------------------------
// Used to modify the texture bits (procedural textures only)
//-----------------------------------------------------------------------------
void CTexture::SetTextureRegenerator( ITextureRegenerator *pTextureRegen )
{
	// NOTE: These can only be used by procedural textures
	Assert( IsProcedural() );

	if (m_pTextureRegenerator)
	{
		m_pTextureRegenerator->Release();
	}
	m_pTextureRegenerator = pTextureRegen; 
}


//-----------------------------------------------------------------------------
// Gets us modifying a particular frame of our texture
//-----------------------------------------------------------------------------
void CTexture::Modify( int iFrame, bool bPaired /* = false */ )
{
	Assert( iFrame >= 0 && iFrame < m_nFrameCount );
#if defined( COMPRESSED_NORMAL_FORMATS )
	g_pShaderAPI->ModifyTexture( bPaired ? m_pTextureHandlesPaired[iFrame] : m_pTextureHandles[iFrame] );
#else
	g_pShaderAPI->ModifyTexture( m_pTextureHandles[iFrame] );
#endif
}


//-----------------------------------------------------------------------------
// Sets the texture clamping state on the currently modified frame
//-----------------------------------------------------------------------------
void CTexture::SetWrapState( )
{
	// Border clamp applies to all texture coordinates
	if ( m_nFlags & TEXTUREFLAGS_BORDER )
	{
		g_pShaderAPI->TexWrap( SHADER_TEXCOORD_S, SHADER_TEXWRAPMODE_BORDER );
		g_pShaderAPI->TexWrap( SHADER_TEXCOORD_T, SHADER_TEXWRAPMODE_BORDER );
		g_pShaderAPI->TexWrap( SHADER_TEXCOORD_U, SHADER_TEXWRAPMODE_BORDER );
		return;
	}

	// Clamp mode in S
	if ( m_nFlags & TEXTUREFLAGS_CLAMPS )
	{
		g_pShaderAPI->TexWrap( SHADER_TEXCOORD_S, SHADER_TEXWRAPMODE_CLAMP );
	}
	else
	{
		g_pShaderAPI->TexWrap( SHADER_TEXCOORD_S, SHADER_TEXWRAPMODE_REPEAT );
	}

	// Clamp mode in T
	if ( m_nFlags & TEXTUREFLAGS_CLAMPT )
	{
		g_pShaderAPI->TexWrap( SHADER_TEXCOORD_T, SHADER_TEXWRAPMODE_CLAMP );
	}
	else
	{
		g_pShaderAPI->TexWrap( SHADER_TEXCOORD_T, SHADER_TEXWRAPMODE_REPEAT );
	}

	// Clamp mode in U
	if ( m_nFlags & TEXTUREFLAGS_CLAMPU )
	{
		g_pShaderAPI->TexWrap( SHADER_TEXCOORD_U, SHADER_TEXWRAPMODE_CLAMP );
	}
	else
	{
		g_pShaderAPI->TexWrap( SHADER_TEXCOORD_U, SHADER_TEXWRAPMODE_REPEAT );
	}
}


//-----------------------------------------------------------------------------
// Sets the texture filtering state on the currently modified frame
//-----------------------------------------------------------------------------
void CTexture::SetFilterState()
{
	// Turns off filtering when we're point sampling
	if( m_nFlags & TEXTUREFLAGS_POINTSAMPLE )
	{
		g_pShaderAPI->TexMinFilter( SHADER_TEXFILTERMODE_NEAREST );
		g_pShaderAPI->TexMagFilter( SHADER_TEXFILTERMODE_NEAREST );
		return;
	}

	// NOTE: config.bMipMapTextures and config.bFilterTextures is handled in ShaderAPIDX8
	bool bEnableMipmapping = ( m_nFlags & TEXTUREFLAGS_NOMIP ) ? false : true;
	if( !bEnableMipmapping )
	{
		g_pShaderAPI->TexMinFilter( SHADER_TEXFILTERMODE_LINEAR );
		g_pShaderAPI->TexMagFilter( SHADER_TEXFILTERMODE_LINEAR );
		return;
	}

	// Determing the filtering mode
	bool bIsAnisotropic = false, bIsTrilinear = false;
	if ( HardwareConfig()->GetDXSupportLevel() >= 80 && (g_config.m_nForceAnisotropicLevel > 1) && 
		(HardwareConfig()->MaximumAnisotropicLevel() > 1) )
	{
		bIsAnisotropic = true;
	}
	else if ( g_config.ForceTrilinear() )
	{
		bIsAnisotropic = (( m_nFlags & TEXTUREFLAGS_ANISOTROPIC ) != 0) && (HardwareConfig()->MaximumAnisotropicLevel() > 1);
		bIsTrilinear = true;
	}
	else
	{
		bIsAnisotropic = (( m_nFlags & TEXTUREFLAGS_ANISOTROPIC ) != 0) && (HardwareConfig()->MaximumAnisotropicLevel() > 1);
		bIsTrilinear = ( m_nFlags & TEXTUREFLAGS_TRILINEAR ) != 0;
	}

	if ( bIsAnisotropic )
	{		    
		g_pShaderAPI->TexMinFilter( SHADER_TEXFILTERMODE_ANISOTROPIC );
		g_pShaderAPI->TexMagFilter( SHADER_TEXFILTERMODE_ANISOTROPIC );
	}
	else
	{
		// force trilinear if we are on a dx8 card or above
		if ( bIsTrilinear )
		{
			g_pShaderAPI->TexMinFilter( SHADER_TEXFILTERMODE_LINEAR_MIPMAP_LINEAR );
			g_pShaderAPI->TexMagFilter( SHADER_TEXFILTERMODE_LINEAR );
		}
		else
		{
			g_pShaderAPI->TexMinFilter( SHADER_TEXFILTERMODE_LINEAR_MIPMAP_NEAREST );
			g_pShaderAPI->TexMagFilter( SHADER_TEXFILTERMODE_LINEAR );
		}
	}
}


//-----------------------------------------------------------------------------
// Download bits main entry point!!
//-----------------------------------------------------------------------------
void CTexture::DownloadTexture( Rect_t *pRect )
{
	// No downloading necessary if there's no graphics
	if ( !g_pShaderDevice->IsUsingGraphics() )
		return;

	// We don't know the actual size of the texture at this stage...
	if ( !pRect )
	{
		ReconstructTexture();
	}
	else
	{
		ReconstructPartialTexture( pRect );
	}

	// Iterate over all the frames and set the appropriate wrapping + filtering state
	SetFilteringAndClampingMode();

	// texture bits have been updated, update the exclusion state
	if ( m_nInternalFlags & TEXTUREFLAGSINTERNAL_SHOULDEXCLUDE )
	{
		m_nInternalFlags |= TEXTUREFLAGSINTERNAL_EXCLUDED;
	}
	else
	{
		m_nInternalFlags &= ~TEXTUREFLAGSINTERNAL_EXCLUDED;
	}

	// texture bits have been picmipped, update the picmip state
	m_nActualDimensionLimit = m_nDesiredDimensionLimit;
}

void CTexture::Download( Rect_t *pRect )
{
	// Only download the bits if we can...
	if ( g_pShaderAPI->CanDownloadTextures() )
	{
		MaterialLock_t hLock = MaterialSystem()->Lock();
		DownloadTexture( pRect );
		MaterialSystem()->Unlock( hLock );
	}
}


void CTexture::Bind( Sampler_t sampler )
{
	Bind( sampler, 0 );
}

//-----------------------------------------------------------------------------
// Binds a particular texture (possibly paired)
//-----------------------------------------------------------------------------
void CTexture::Bind( Sampler_t sampler1, int nFrame, Sampler_t sampler2 /* = -1 */ )
{
	if ( g_pShaderDevice->IsUsingGraphics() )
	{
		if ( nFrame < 0 || nFrame >= m_nFrameCount )
		{
			// FIXME: Use the well-known 'error' id instead of frame 0
			nFrame = 0;
			//			Assert(0);
		}

		g_pShaderAPI->BindTexture( sampler1, m_pTextureHandles[nFrame] );

#if defined( COMPRESSED_NORMAL_FORMATS )
		if ( m_bPaired && ( sampler2 != -1 ) )
		{
			g_pShaderAPI->BindTexture( sampler2, m_pTextureHandlesPaired[nFrame] );
		}
#endif
	}
}



void CTexture::BindVertexTexture( VertexTextureSampler_t sampler, int nFrame )
{
	if ( g_pShaderDevice->IsUsingGraphics() )
	{
		if ( nFrame < 0 || nFrame >= m_nFrameCount )
		{
			// FIXME: Use the well-known 'error' id instead of frame 0
			nFrame = 0;
			//			Assert(0);
		}
		g_pShaderAPI->BindVertexTexture( sampler, m_pTextureHandles[nFrame] );
	}
}


//-----------------------------------------------------------------------------
// Set this texture as a render target
//-----------------------------------------------------------------------------
bool CTexture::SetRenderTarget( int nRenderTargetID )
{
	return SetRenderTarget( nRenderTargetID, NULL );
}

//-----------------------------------------------------------------------------
// Set this texture as a render target
// Optionally bind pDepthTexture as depth buffer
//-----------------------------------------------------------------------------
bool CTexture::SetRenderTarget( int nRenderTargetID, ITexture *pDepthTexture )
{
	if ( ( m_nFlags & TEXTUREFLAGS_RENDERTARGET ) == 0 )
		return false;

	ShaderAPITextureHandle_t textureHandle;
	if ( !IsX360() )
	{
		textureHandle = m_pTextureHandles[0];
	}
	else
	{
		Assert( m_nFrameCount > 1 );
		textureHandle = m_pTextureHandles[m_nFrameCount-1];
	}

	ShaderAPITextureHandle_t depthTextureHandle = (unsigned int)SHADER_RENDERTARGET_DEPTHBUFFER;

	if ( m_nFlags & TEXTUREFLAGS_DEPTHRENDERTARGET )
	{
		Assert( m_nFrameCount >= 2 );
		depthTextureHandle = m_pTextureHandles[1];
	} 
	else if ( m_nFlags & TEXTUREFLAGS_NODEPTHBUFFER )
	{
		// GR - render target without depth buffer	
		depthTextureHandle = (unsigned int)SHADER_RENDERTARGET_NONE;
	}

	if ( pDepthTexture)
	{
		depthTextureHandle = static_cast<ITextureInternal *>(pDepthTexture)->GetTextureHandle(0);
	}

	g_pShaderAPI->SetRenderTargetEx( nRenderTargetID, textureHandle, depthTextureHandle );
	return true;
}


//-----------------------------------------------------------------------------
// Reference counting
//-----------------------------------------------------------------------------
void CTexture::IncrementReferenceCount( void )
{
	++m_nRefCount;
}

void CTexture::DecrementReferenceCount( void )
{
	--m_nRefCount;

	/* FIXME: Probably have to remove this from the texture manager too..?
	if (IsProcedural() && (m_nRefCount < 0))
		delete this;
	*/
}

int CTexture::GetReferenceCount( )
{
	return m_nRefCount;
}


//-----------------------------------------------------------------------------
// Various accessor methods
//-----------------------------------------------------------------------------
const char* CTexture::GetName( ) const
{
	return m_Name.String();
}

const char* CTexture::GetTextureGroupName( ) const
{
	return m_TextureGroupName.String();
}

void CTexture::SetName( const char* pName )
{
	// normalize and convert to a symbol
	char szCleanName[MAX_PATH];
	m_Name = NormalizeTextureName( pName, szCleanName, sizeof( szCleanName ) );

#ifdef _DEBUG
	if ( m_pDebugName )
	{
		delete [] m_pDebugName;
	}
	int nLen = V_strlen( szCleanName ) + 1;
	m_pDebugName = new char[nLen];
	V_memcpy( m_pDebugName, szCleanName, nLen );
#endif
}

ImageFormat CTexture::GetImageFormat()	const
{
	return m_ImageFormat;
}

NormalDecodeMode_t CTexture::GetNormalDecodeMode() const
{
#if defined( COMPRESSED_NORMAL_FORMATS )
	if ( m_ImageFormat == IMAGE_FORMAT_ATI2N )
	{
		if ( m_bPaired && ( m_ImageFormatPaired == IMAGE_FORMAT_ATI1N ) )
		{
			return NORMAL_DECODE_ATI2N_ALPHA;
		}

		return NORMAL_DECODE_ATI2N;
	}
#endif
	return NORMAL_DECODE_NONE;
}

int CTexture::GetMappingWidth()	const
{
	return m_nMappingWidth;
}

int CTexture::GetMappingHeight() const
{
	return m_nMappingHeight;
}

int CTexture::GetMappingDepth() const
{
	return m_nMappingDepth;
}

int CTexture::GetActualWidth() const
{
	return m_nActualWidth;
}

int CTexture::GetActualHeight()	const
{
	return m_nActualHeight;
}

int CTexture::GetActualDepth()	const
{
	return m_nActualDepth;
}

int CTexture::GetNumAnimationFrames() const
{
	return m_nFrameCount;
}

void CTexture::GetReflectivity( Vector& reflectivity )
{
	Precache();
	VectorCopy( m_vecReflectivity, reflectivity );
}

//-----------------------------------------------------------------------------
// Little helper polling methods
//-----------------------------------------------------------------------------
bool CTexture::IsTranslucent() const
{
	return ( m_nFlags & (TEXTUREFLAGS_ONEBITALPHA | TEXTUREFLAGS_EIGHTBITALPHA) ) != 0;
}

bool CTexture::IsNormalMap( void ) const
{
	return ( ( m_nFlags & TEXTUREFLAGS_NORMAL ) != 0 );
}

bool CTexture::IsCubeMap( void ) const
{
	return ( ( m_nFlags & TEXTUREFLAGS_ENVMAP ) != 0 );
}

bool CTexture::IsRenderTarget( void ) const
{
	return ( ( m_nFlags & TEXTUREFLAGS_RENDERTARGET ) != 0 );
}

bool CTexture::IsTempRenderTarget( void ) const
{
	return ( ( m_nInternalFlags & TEXTUREFLAGSINTERNAL_TEMPRENDERTARGET ) != 0 );
}

bool CTexture::IsProcedural() const
{
	return ( (m_nFlags & TEXTUREFLAGS_PROCEDURAL) != 0 );
}

bool CTexture::IsMipmapped() const
{
	return ( (m_nFlags & TEXTUREFLAGS_NOMIP) == 0 );
}

unsigned int CTexture::GetFlags() const
{
	return m_nFlags;
}

void CTexture::ForceLODOverride( int iNumLodsOverrideUpOrDown )
{
	if ( IsX360() )
	{
		// not supporting
		Assert( 0 );
		return;
	}

	TextureLodOverride::OverrideInfo oi( iNumLodsOverrideUpOrDown, iNumLodsOverrideUpOrDown );
	TextureLodOverride::Add( GetName(), oi );
	Download( NULL );
}


bool CTexture::IsError() const
{
	return ( (m_nInternalFlags & TEXTUREFLAGSINTERNAL_ERROR) != 0 );
}

bool CTexture::HasBeenAllocated() const
{
	return ( (m_nInternalFlags & TEXTUREFLAGSINTERNAL_ALLOCATED) != 0 );
}

bool CTexture::IsVolumeTexture() const
{
	return (m_nMappingDepth > 1);
}

//-----------------------------------------------------------------------------
// Sets the filtering + clamping modes on the texture
//-----------------------------------------------------------------------------
void CTexture::SetFilteringAndClampingMode()
{
	if( !HasBeenAllocated() )
		return;

	int nCount = m_nFrameCount;
	if ( IsX360() && IsRenderTarget() )
	{
		// 360 render targets have a reserved surface
		nCount--;
	}

	for ( int iFrame = 0; iFrame < nCount; ++iFrame )
	{
		Modify( iFrame );			// Indicate we're changing state with respect to a particular frame
		SetWrapState();				// Send the appropriate wrap/clamping modes to the shaderapi.
		SetFilterState();			// Set the filtering mode for the texture after downloading it.
									// NOTE: Apparently, the filter state cannot be set until after download
	}

#if defined( COMPRESSED_NORMAL_FORMATS )
	if ( m_bPaired )
	{
		for ( int iFrame = 0; iFrame < nCount; ++iFrame )
		{
			Modify( iFrame, true );	// Indicate we're changing state with respect to a particular frame
			SetWrapState();			// Send the appropriate wrap/clamping modes to the shaderapi.
			SetFilterState();		// Set the filtering mode for the texture after downloading it.
									// NOTE: Apparently, the filter state cannot be set until after download
		}
	}
#endif
}

//-----------------------------------------------------------------------------
// Loads up the non-fallback information about the texture 
//-----------------------------------------------------------------------------
void CTexture::Precache()
{
	// We only have to do something in the case of a file texture
	if ( IsRenderTarget() || IsProcedural() )
		return;

	if ( HasBeenAllocated() )
		return;

	// Blow off env_cubemap too...
	if ( !Q_strnicmp( m_Name.String(), "env_cubemap", 12 ))
		return;
	
	if ( IsX360() && m_nFlags )
	{
		// 360 can be assured that precaching has already been done
		return;
	}

	IVTFTexture *pVTFTexture = GetScratchVTFTexture();

	// The texture name doubles as the relative file name
	// It's assumed to have already been set by this point	
	// Compute the cache name
	char pCacheFileName[MATERIAL_MAX_PATH];
	Q_snprintf( pCacheFileName, sizeof( pCacheFileName ), "materials/%s" TEXTURE_FNAME_EXTENSION, m_Name.String() );

#if defined( _X360 )
	// generate native texture
	pVTFTexture->UpdateOrCreate( pCacheFileName );
#endif

	int nVersion = -1;
	if ( IsPC() )
		nVersion = VTF_MAJOR_VERSION;
	else if ( IsX360() )
		nVersion = VTF_X360_MAJOR_VERSION;

	int nHeaderSize = VTFFileHeaderSize( nVersion );
	unsigned char *pMem = (unsigned char *)stackalloc( nHeaderSize );
	CUtlBuffer buf( pMem, nHeaderSize );
	if ( !g_pFullFileSystem->ReadFile( pCacheFileName, NULL, buf, nHeaderSize ) )	
	{
		goto precacheFailed;
	}

	// Unserialize the header only
#if !defined( _X360 )
	if ( !pVTFTexture->Unserialize( buf, true ) )
#else
	if ( !pVTFTexture->UnserializeFromBuffer( buf, true, true, true, 0 ) )
#endif
	{
		Warning( "Error reading material \"%s\"\n", pCacheFileName );
		goto precacheFailed;
	}

	// NOTE: Don't set the image format in case graphics are active
	VectorCopy( pVTFTexture->Reflectivity(), m_vecReflectivity );
	m_nMappingWidth = pVTFTexture->Width();
	m_nMappingHeight = pVTFTexture->Height();
	m_nMappingDepth = pVTFTexture->Depth();
	m_nFlags = pVTFTexture->Flags();
	m_nFrameCount = pVTFTexture->FrameCount();

	return;

precacheFailed:
	m_vecReflectivity.Init( 0, 0, 0 );
	m_nMappingWidth = 32;
	m_nMappingHeight = 32;
	m_nMappingDepth = 1;
	m_nFlags = TEXTUREFLAGS_NOMIP;
	m_nInternalFlags |= TEXTUREFLAGSINTERNAL_ERROR;
	m_nFrameCount = 1;
}



//-----------------------------------------------------------------------------
// Loads the low-res image from the texture 
//-----------------------------------------------------------------------------
void CTexture::LoadLowResTexture( IVTFTexture *pTexture )
{
#if !defined( _X360 )
	delete [] m_pLowResImage;
	m_pLowResImage = NULL;
#endif

	if ( pTexture->LowResWidth() == 0 || pTexture->LowResHeight() == 0 )
	{
		m_LowResImageWidth = m_LowResImageHeight = 0;
		return;
	}

	m_LowResImageWidth = pTexture->LowResWidth();
	m_LowResImageHeight = pTexture->LowResHeight();

#if !defined( _X360 )
	m_pLowResImage = new unsigned char[m_LowResImageWidth * m_LowResImageHeight * 3];
#ifdef _DEBUG
	bool retVal = 
#endif
		ImageLoader::ConvertImageFormat( pTexture->LowResImageData(), pTexture->LowResFormat(), 
			m_pLowResImage, IMAGE_FORMAT_RGB888, m_LowResImageWidth, m_LowResImageHeight );
	Assert( retVal );
#else
	*(unsigned int*)m_LowResImageSample = *(unsigned int*)pTexture->LowResImageSample();
#endif
}

void *CTexture::GetResourceData( uint32 eDataType, size_t *pnumBytes ) const
{
	for ( DataChunk const *pDataChunk = m_arrDataChunks.Base(),
		  *pDataChunkEnd = pDataChunk + m_arrDataChunks.Count();
		  pDataChunk < pDataChunkEnd; ++pDataChunk )
	{
		if ( ( pDataChunk->m_eType & ~RSRCF_MASK ) == eDataType )
		{
			if ( ( pDataChunk->m_eType & RSRCF_HAS_NO_DATA_CHUNK ) == 0 )
			{
				if ( pnumBytes)
					*pnumBytes = pDataChunk->m_numBytes;
				return pDataChunk->m_pvData;
			}
			else
			{
				if ( pnumBytes )
					*pnumBytes = sizeof( pDataChunk->m_numBytes );

				return ( void *)( &pDataChunk->m_numBytes );
			}
		}
	}
	if ( pnumBytes )
		pnumBytes = 0;
	return NULL;
}

#pragma pack(1)

struct DXTColBlock
{
	unsigned short col0;
	unsigned short col1;

	// no bit fields - use bytes
	unsigned char row[4];
};

struct DXTAlphaBlock3BitLinear
{
	unsigned char alpha0;
	unsigned char alpha1;

	unsigned char stuff[6];
};

#pragma pack()

static void FillCompressedTextureWithSingleColor( int red, int green, int blue, int alpha, unsigned char *pImageData, 
												 int width, int height, int depth, ImageFormat imageFormat )
{
	Assert( ( width < 4 ) || !( width % 4 ) );
	Assert( ( height < 4 ) || !( height % 4 ) );
	Assert( ( depth < 4 ) || !( depth % 4 ) );
	if ( width < 4 && width > 0 )
	{
		width = 4;
	}
	if ( height < 4 && height > 0 )
	{
		height = 4;
	}
	if ( depth < 4 && depth > 1 )
	{
		depth = 4;
	}
	int numBlocks = ( width * height ) >> 4;
	numBlocks *= depth;
	
	DXTColBlock colorBlock;
	memset( &colorBlock, 0, sizeof( colorBlock ) );
	( ( BGR565_t * )&( colorBlock.col0 ) )->Set( red, green, blue );
	( ( BGR565_t * )&( colorBlock.col1 ) )->Set( red, green, blue );

	switch( imageFormat )
	{
	case IMAGE_FORMAT_DXT1:
	case IMAGE_FORMAT_ATI1N:	// Invalid block data, but correct memory footprint
		{
			int i;
			for( i = 0; i < numBlocks; i++ )
			{
				memcpy( pImageData + i * 8, &colorBlock, sizeof( colorBlock ) );
			}
		}
		break;
	case IMAGE_FORMAT_DXT5:
	case IMAGE_FORMAT_ATI2N:
		{
			int i;
			for( i = 0; i < numBlocks; i++ )
			{
//				memset( pImageData + i * 16, 0, 16 );
				memcpy( pImageData + i * 16 + 8, &colorBlock, sizeof( colorBlock ) );
//				memset( pImageData + i * 16 + 8, 0xffff, 8 ); // alpha block
			}
		}
		break;
	default:
		Assert( 0 );
		break;
	}
}

//-----------------------------------------------------------------------------
// Generate a texture that shows the various mip levels
//-----------------------------------------------------------------------------
void CTexture::GenerateShowMipLevelsTextures( IVTFTexture *pTexture )
{
	if( pTexture->FaceCount() > 1 )
		return;

	switch( pTexture->Format() )
	{
	// These are formats that we don't bother with for generating mip level textures.
	case IMAGE_FORMAT_RGBA16161616F:
	case IMAGE_FORMAT_R32F:
	case IMAGE_FORMAT_RGB323232F:
	case IMAGE_FORMAT_RGBA32323232F:
	case IMAGE_FORMAT_UV88:
		break;
	default:
		for (int iFrame = 0; iFrame < pTexture->FrameCount(); ++iFrame )
		{
			for (int iFace = 0; iFace < pTexture->FaceCount(); ++iFace )
			{
				for (int iMip = 0; iMip < pTexture->MipCount(); ++iMip )
				{
					int green  =	( ( iMip + 1 ) & 1 ) ? 255 : 0;
					int red	   =	( ( iMip + 1 ) & 2 ) ? 255 : 0;
					int blue   =	( ( iMip + 1 ) & 4 ) ? 255 : 0;

					int nWidth, nHeight, nDepth;
					pTexture->ComputeMipLevelDimensions( iMip, &nWidth, &nHeight, &nDepth );
					if( pTexture->Format() == IMAGE_FORMAT_DXT1  || pTexture->Format() == IMAGE_FORMAT_DXT5 ||
					    pTexture->Format() == IMAGE_FORMAT_ATI1N || pTexture->Format() == IMAGE_FORMAT_ATI2N )
					{
						unsigned char *pImageData = pTexture->ImageData( iFrame, iFace, iMip, 0, 0, 0 );
						int alpha = 255;
						FillCompressedTextureWithSingleColor( red, green, blue, alpha, pImageData, pTexture->Width(), pTexture->Height(), nDepth, pTexture->Format() );
					}
					else
					{
						for ( int z = 0; z < nDepth; ++z )
						{
							CPixelWriter pixelWriter;
							pixelWriter.SetPixelMemory( pTexture->Format(), 
								pTexture->ImageData( iFrame, iFace, iMip, 0, 0, z ), pTexture->RowSizeInBytes( iMip ) );

							for (int y = 0; y < nHeight; ++y)
							{
								pixelWriter.Seek( 0, y );
								for (int x = 0; x < nWidth; ++x)
								{
									pixelWriter.WritePixel( red, green, blue, 255 );
								}
							}
						}
					}
				}
			}
		}
		break;
	}
}


//-----------------------------------------------------------------------------
// Generate a texture that shows the various mip levels
//-----------------------------------------------------------------------------
void CTexture::CopyLowResImageToTexture( IVTFTexture *pTexture )
{
	int nFlags = pTexture->Flags();
	nFlags |= TEXTUREFLAGS_NOMIP | TEXTUREFLAGS_POINTSAMPLE;
	nFlags &= ~(TEXTUREFLAGS_TRILINEAR | TEXTUREFLAGS_ANISOTROPIC | TEXTUREFLAGS_HINT_DXT5);
	nFlags &= ~(TEXTUREFLAGS_NORMAL | TEXTUREFLAGS_ENVMAP);
	nFlags &= ~(TEXTUREFLAGS_ONEBITALPHA | TEXTUREFLAGS_EIGHTBITALPHA);

	Assert( pTexture->FrameCount() == 1 );

	Init( pTexture->Width(), pTexture->Height(), 1, IMAGE_FORMAT_BGR888, nFlags, 1, false );
	pTexture->Init( m_LowResImageWidth, m_LowResImageHeight, 1, IMAGE_FORMAT_BGR888, nFlags, 1 );

	// Don't bother computing the actual size; it's actually equal to the low-res size
	// With only one mip level
	m_nActualWidth = m_LowResImageWidth;
	m_nActualHeight = m_LowResImageHeight;
	m_nActualDepth = 1;
	m_nActualMipCount = 1;

	// Copy the row-res image into the VTF Texture
	CPixelWriter pixelWriter;
	pixelWriter.SetPixelMemory( pTexture->Format(), 
		pTexture->ImageData( 0, 0, 0 ), pTexture->RowSizeInBytes( 0 ) );

#if !defined( _X360 )
	unsigned char *pLowResImage = m_pLowResImage;
#else
	unsigned char *pLowResImage = m_LowResImageSample;
#endif
	for ( int y = 0; y < m_LowResImageHeight; ++y )
	{
		pixelWriter.Seek( 0, y );
		for ( int x = 0; x < m_LowResImageWidth; ++x )
		{
			int red = pLowResImage[0];
			int green = pLowResImage[1];
			int blue = pLowResImage[2];
			pLowResImage += 3;

			pixelWriter.WritePixel( red, green, blue, 255 );
		}
	}
}

//-----------------------------------------------------------------------------
// Sets up debugging texture bits, if appropriate
//-----------------------------------------------------------------------------
bool CTexture::SetupDebuggingTextures( IVTFTexture *pVTFTexture )
{
	if ( IsX360() )
	{
		// not supporting
		return false;
	}

	if ( pVTFTexture->Flags() & TEXTUREFLAGS_NODEBUGOVERRIDE )
		return false;

	if ( g_config.nShowMipLevels )
	{
		// mat_showmiplevels 1 means don't do normal maps
		if ( ( g_config.nShowMipLevels == 1 ) && ( pVTFTexture->Flags() & ( TEXTUREFLAGS_NORMAL | TEXTUREFLAGS_SSBUMP ) ) )
			return false;

		// mat_showmiplevels 2 means don't do base textures
		if ( ( g_config.nShowMipLevels == 2 ) && !( pVTFTexture->Flags() & ( TEXTUREFLAGS_NORMAL | TEXTUREFLAGS_SSBUMP ) ) )
			return false;

		// This mode shows the mip levels as different colors
		GenerateShowMipLevelsTextures( pVTFTexture );
		return true;
	}
	else if ( g_config.bShowLowResImage && pVTFTexture->FrameCount() == 1 && 
		pVTFTexture->FaceCount() == 1 && ((pVTFTexture->Flags() & TEXTUREFLAGS_NORMAL) == 0) &&
		m_LowResImageWidth != 0 && m_LowResImageHeight != 0 )
	{
		// This mode just uses the low res texture
		CopyLowResImageToTexture( pVTFTexture );
		return true;
	}

	return false;
}

//-----------------------------------------------------------------------------
// Converts the texture to the actual format
// Returns true if conversion applied, false otherwise
//-----------------------------------------------------------------------------
bool CTexture::ConvertToActualFormat( IVTFTexture *pVTFTexture, bool bPaired /* = false */ )
{
	if ( !g_pShaderDevice->IsUsingGraphics() )
		return false;

	bool bConverted = false;

#if defined( COMPRESSED_NORMAL_FORMATS )
	ImageFormat fmt = bPaired ? m_ImageFormatPaired : m_ImageFormat;
#else
	ImageFormat fmt = m_ImageFormat;
#endif

	ImageFormat dstFormat = ComputeActualFormat( pVTFTexture->Format() );
	if ( fmt != dstFormat )
	{
		pVTFTexture->ConvertImageFormat( dstFormat, false );

#if defined( COMPRESSED_NORMAL_FORMATS )
		if ( bPaired )
			m_ImageFormatPaired = dstFormat;
		else
			m_ImageFormat = dstFormat;
#else
		m_ImageFormat = dstFormat;
#endif

		bConverted = true;
	}
	else if ( HardwareConfig()->GetHDRType() == HDR_TYPE_INTEGER &&
		     fmt == dstFormat && dstFormat == IMAGE_FORMAT_RGBA16161616F )
	{
		// This is to force at most the precision of int16 for fp16 texture when running the integer path.
		pVTFTexture->ConvertImageFormat( IMAGE_FORMAT_RGBA16161616, false );
		pVTFTexture->ConvertImageFormat( IMAGE_FORMAT_RGBA16161616F, false );
		bConverted = true;
	}

	return bConverted;
}

void CTexture::GetFilename( char *pOut, int maxLen ) const
{
	const char *pName = m_Name.String();
	bool bIsUNCName = ( pName[0] == '/' && pName[1] == '/' && pName[2] != '/' );

	if ( !bIsUNCName )
	{
		Q_snprintf( pOut, maxLen, 
			"materials/%s" TEXTURE_FNAME_EXTENSION, pName );
	}
	else
	{
		Q_snprintf( pOut, maxLen, "%s" TEXTURE_FNAME_EXTENSION, pName );
	}
}


void CTexture::ReloadFilesInList( IFileList *pFilesToReload )
{
	if ( IsProcedural() || IsRenderTarget() )
		return;

	char filename[MAX_PATH];
	GetFilename( filename, sizeof( filename ) );
	if ( pFilesToReload->IsFileInList( filename ) )
	{
		Download();
	}
}

//-----------------------------------------------------------------------------
// Loads the texture bits from a file.
//-----------------------------------------------------------------------------
IVTFTexture *CTexture::LoadTextureBitsFromFile( char *pCacheFileName, char **ppResolvedFilename, bool bPaired )
{
	int nHeaderSize;
	int	nFileSize;

	IVTFTexture *pVTFTexture = GetScratchVTFTexture( bPaired );

	CUtlBuffer buf;
	FileHandle_t fileHandle = FILESYSTEM_INVALID_HANDLE;
	while ( fileHandle == FILESYSTEM_INVALID_HANDLE )			// run until found a file or out of rules
	{
#if defined( _X360 )
		// generate native texture
		pVTFTexture->UpdateOrCreate( pCacheFileName );
#endif
		fileHandle = g_pFullFileSystem->OpenEx( pCacheFileName, "rb", 0, MaterialSystem()->GetForcedTextureLoadPathID(), ppResolvedFilename );
		if ( fileHandle == FILESYSTEM_INVALID_HANDLE )
		{
			// try any fallbacks.
			char *pHdrExt = Q_stristr( pCacheFileName, ".hdr" TEXTURE_FNAME_EXTENSION );
			if ( pHdrExt )
			{
				DevWarning( "A custom HDR cubemap \"%s\": cannot be found on disk.\n"
							"This really should have a HDR version, trying a fall back to a non-HDR version.\n", pCacheFileName );
				strcpy( pHdrExt, TEXTURE_FNAME_EXTENSION );
			}
			else
			{
				// no more fallbacks
				break;
			}
		}
	}
	
	if ( fileHandle == FILESYSTEM_INVALID_HANDLE )
	{
		if ( Q_strnicmp( m_Name.String(), "env_cubemap", 12 ))
		{
			DevWarning( "\"%s\": can't be found on disk\n", pCacheFileName );
		}
		return HandleFileLoadFailedTexture( pVTFTexture );
	}

	int nVersion = -1;
	if ( IsPC() )
	{
		nVersion = VTF_MAJOR_VERSION;
	}
	else if ( IsX360() )
	{
		nVersion = VTF_X360_MAJOR_VERSION;
	}

	nHeaderSize = VTFFileHeaderSize( nVersion );

	int nBytesOptimalRead;	// GCC needs this extra newline due to goto
	int nBytesRead;			// GCC needs this extra newline due to goto

	// restrict read to the header only!
	// header provides info to avoid reading the entire file
	nBytesOptimalRead = GetOptimalReadBuffer( fileHandle, nHeaderSize, buf );
	nBytesRead = g_pFullFileSystem->ReadEx( buf.Base(), nBytesOptimalRead, nHeaderSize, fileHandle );
	nBytesRead = nHeaderSize = ((VTFFileBaseHeader_t *)buf.Base())->headerSize;
	g_pFullFileSystem->Seek( fileHandle, nHeaderSize, FILESYSTEM_SEEK_HEAD );
	buf.SeekPut( CUtlBuffer::SEEK_HEAD, nBytesRead );

	// Unserialize the header only
	// need the header first to determine remainder of data
#if !defined( _X360 )
	if ( !pVTFTexture->Unserialize( buf, true ) )
#else
	if ( !pVTFTexture->UnserializeFromBuffer( buf, true, true, true, 0 ) )
#endif
	{
		Warning( "Error reading texture header \"%s\"\n", pCacheFileName );
		g_pFullFileSystem->Close( fileHandle );
		return HandleFileLoadFailedTexture( pVTFTexture );
	}

	// Seek the reading back to the front of the buffer
	buf.SeekGet( CUtlBuffer::SEEK_HEAD, 0 );

	// Initialize the texture class with vtf header data before operations
	Init( 
#if !defined( _X360 )
		pVTFTexture->Width(), 
		pVTFTexture->Height(), 
		pVTFTexture->Depth(), 
#else
		// 360 texture might be pre-picmipped, setup as it's original dimensions
		// so picmipping logic calculates correctly, and then fixup
		pVTFTexture->MappingWidth(),
		pVTFTexture->MappingHeight(),
		pVTFTexture->MappingDepth(),
#endif
		pVTFTexture->Format(), 
		pVTFTexture->Flags(), 
		pVTFTexture->FrameCount(),
		bPaired );

#if defined( _X360 )
	m_nInternalFlags |= TEXTUREFLAGSINTERNAL_QUEUEDLOAD;
	if ( !g_pQueuedLoader->IsMapLoading() || ( m_nFlags & ( TEXTUREFLAGS_PROCEDURAL|TEXTUREFLAGS_RENDERTARGET|TEXTUREFLAGS_DEPTHRENDERTARGET ) ) )
	{
		// explicitly disabled or not appropriate for texture type
		m_nInternalFlags &= ~TEXTUREFLAGSINTERNAL_QUEUEDLOAD;
	}
	else
	{
		if ( pVTFTexture->FileSize( true, 0 ) >= pVTFTexture->FileSize( false, 0 ) )
		{
			// texture is a dwarf, entirely in preload, loads normally
			m_nInternalFlags &= ~TEXTUREFLAGSINTERNAL_QUEUEDLOAD;
		}
	}
#endif

	VectorCopy( pVTFTexture->Reflectivity(), m_vecReflectivity );

	// Compute the actual texture dimensions
	int nMipSkipCount = ComputeActualSize( false, pVTFTexture );

#if defined( _X360 )
	bool bQueuedLoad = ( m_nInternalFlags & TEXTUREFLAGSINTERNAL_QUEUEDLOAD ) != 0;
	nMipSkipCount -= pVTFTexture->MipSkipCount();
	if ( nMipSkipCount < 0 || ( nMipSkipCount >= pVTFTexture->MipCount() ) )
	{
		// the 360 texture was already pre-picmipped or can't be picmipped
		// clamp to the available dimensions
		m_nActualWidth = pVTFTexture->Width();
		m_nActualHeight = pVTFTexture->Height();
		m_nActualDepth = pVTFTexture->Depth();
		m_nActualMipCount = ComputeActualMipCount();
		nMipSkipCount = 0;
	}
#endif

#if !defined( _X360 )
	// Determine how much of the file to read in
	nFileSize = pVTFTexture->FileSize( nMipSkipCount );
#else
	// A queued loading texture just gets the preload section
	// and does NOT unserialize the texture bits here
	nFileSize = pVTFTexture->FileSize( bQueuedLoad, nMipSkipCount );
#endif

	// Read only the portion of the file that we care about
	g_pFullFileSystem->Seek( fileHandle, 0, FILESYSTEM_SEEK_HEAD );
	nBytesOptimalRead = GetOptimalReadBuffer( fileHandle, nFileSize, buf );
	nBytesRead = g_pFullFileSystem->ReadEx( buf.Base(), nBytesOptimalRead, nFileSize, fileHandle );
	g_pFullFileSystem->Close( fileHandle );
	buf.SeekPut( CUtlBuffer::SEEK_HEAD, nBytesRead );

	// NOTE: Skipping mip levels here will cause the size to be changed...
#if !defined( _X360 )
	if ( !pVTFTexture->Unserialize( buf, false, nMipSkipCount ) )
#else
	if ( !pVTFTexture->UnserializeFromBuffer( buf, true, false, bQueuedLoad, nMipSkipCount ) )
#endif
	{
		Warning( "Error reading material data \"%s\"\n", pCacheFileName );
		return HandleFileLoadFailedTexture( pVTFTexture );
	}

	// Build the low-res texture
	LoadLowResTexture( pVTFTexture );

	// Load the resources
	if ( unsigned int uiRsrcCount = pVTFTexture->GetResourceTypes( NULL, 0 ) )
	{
		uint32 *arrRsrcTypes = ( uint32 * )_alloca( uiRsrcCount * sizeof( unsigned int ) );
		pVTFTexture->GetResourceTypes( arrRsrcTypes, uiRsrcCount );

		m_arrDataChunks.EnsureCapacity( uiRsrcCount );
		for ( uint32 *arrRsrcTypesEnd = arrRsrcTypes + uiRsrcCount;
			  arrRsrcTypes < arrRsrcTypesEnd; ++arrRsrcTypes )
		{
			switch ( *arrRsrcTypes )
			{
			case VTF_LEGACY_RSRC_LOW_RES_IMAGE:
			case VTF_LEGACY_RSRC_IMAGE:
				// These stock types use specific load routines
				continue;
					
			default:
				{
					DataChunk dc;
					dc.m_eType = *arrRsrcTypes;
					dc.m_eType &= ~RSRCF_MASK;

					size_t numBytes;
					if ( void *pvData = pVTFTexture->GetResourceData( dc.m_eType, &numBytes ) )
					{
						Assert( numBytes >= sizeof( uint32 ) );
						if ( numBytes == sizeof( dc.m_numBytes ) )
						{
							dc.m_eType |= RSRCF_HAS_NO_DATA_CHUNK;
							dc.m_pvData = NULL;
							memcpy( &dc.m_numBytes, pvData, numBytes );
						}
						else
						{
							dc.Allocate( numBytes );
							memcpy( dc.m_pvData, pvData, numBytes );
						}

						m_arrDataChunks.AddToTail( dc );
					}
				}
			}
		}
	}

	// Try to set up debugging textures, if we're in a debugging mode
	if ( !IsProcedural() && !IsX360() )
	{
		SetupDebuggingTextures( pVTFTexture );
	}

	if ( ConvertToActualFormat( pVTFTexture, bPaired ) )
	{
		if ( IsX360() )
		{
			// 360 vtf are baked in final formats, no format conversion can or should have occurred
			// otherwise track offender and ensure files are baked correctly
			Error( "\"%s\" not in native format\n", pCacheFileName );
		}
	}

	return pVTFTexture;
}


IVTFTexture *CTexture::HandleFileLoadFailedTexture( IVTFTexture *pVTFTexture )
{
	// create the error texture
#if defined( _X360 )
	// reset botched vtf, ensure checkerboard error texture is created now and maintains bgra8888 format
	pVTFTexture->ReleaseImageMemory();
	m_nInternalFlags &= ~TEXTUREFLAGSINTERNAL_QUEUEDLOAD;
	m_nFlags |= TEXTUREFLAGS_EIGHTBITALPHA;
#endif

	// This will make a checkerboard texture to indicate failure
	pVTFTexture->Init( 32, 32, 1, IMAGE_FORMAT_BGRA8888, m_nFlags, 1 );
	Init( pVTFTexture->Width(), pVTFTexture->Height(), pVTFTexture->Depth(), pVTFTexture->Format(), 
		  pVTFTexture->Flags(), pVTFTexture->FrameCount(), false );
	m_vecReflectivity.Init( 0.5f, 0.5f, 0.5f );

	// NOTE: For mat_picmip to work, we must use the same size (32x32)
	// Which should work since every card can handle textures of that size
	m_nActualWidth = pVTFTexture->Width();
	m_nActualHeight = pVTFTexture->Height();
	m_nActualMipCount = 1;

	// generate the checkerboard
	TextureManager()->GenerateErrorTexture( this, pVTFTexture );
	ConvertToActualFormat( pVTFTexture );

	// Deactivate procedural texture...
	m_nFlags &= ~TEXTUREFLAGS_PROCEDURAL;
	m_nInternalFlags |= TEXTUREFLAGSINTERNAL_ERROR;

	return pVTFTexture;
}

//-----------------------------------------------------------------------------
// Computes subrect for a particular miplevel
//-----------------------------------------------------------------------------
void CTexture::ComputeMipLevelSubRect( const Rect_t* pSrcRect, int nMipLevel, Rect_t *pSubRect )
{
	if (nMipLevel == 0)
	{
		*pSubRect = *pSrcRect;
		return;
	}

	float flInvShrink = 1.0f / (float)(1 << nMipLevel);
	pSubRect->x = pSrcRect->x * flInvShrink;
	pSubRect->y = pSrcRect->y * flInvShrink;
	pSubRect->width = (int)ceil( (pSrcRect->x + pSrcRect->width) * flInvShrink ) - pSubRect->x;
	pSubRect->height = (int)ceil( (pSrcRect->y + pSrcRect->height) * flInvShrink ) - pSubRect->y;
}


//-----------------------------------------------------------------------------
// Computes the face count + first face
//-----------------------------------------------------------------------------
void CTexture::GetDownloadFaceCount( int &nFirstFace, int &nFaceCount )
{
	nFaceCount = 1;
	nFirstFace = 0;
	if ( IsCubeMap() )
	{
		if ( IsX360() || HardwareConfig()->SupportsCubeMaps() )
		{
			nFaceCount = CUBEMAP_FACE_COUNT-1;
		}
		else
		{
			// This will cause us to use the spheremap instead of the cube faces
			// in the case where we don't support cubemaps
			nFirstFace = CUBEMAP_FACE_SPHEREMAP;
		}
	}
}

//-----------------------------------------------------------------------------
// Fixup a queue loaded texture with the delayed hi-res data
//-----------------------------------------------------------------------------
void CTexture::FixupTexture( const void *pData, int nSize, LoaderError_t loaderError )
{
	if ( loaderError != LOADERERROR_NONE )
	{
		// mark as invalid
		nSize = 0;
	}

	m_nInternalFlags &= ~TEXTUREFLAGSINTERNAL_QUEUEDLOAD;

#if defined( _X360 )
	// hand off the hires data down to the shaderapi to upload directly
	// Purposely bypassing downloading through material system, which is non-reentrant
	// for that operation, to avoid mutexing.

	// NOTE: Strange refcount work here to keep it threadsafe
	int nRefCount = m_nRefCount;
	int nRefCountOld = nRefCount;
	g_pShaderAPI->PostQueuedTexture( 
					pData, 
					nSize, 
					m_pTextureHandles, 
					m_nFrameCount,
					m_nActualWidth,
					m_nActualHeight,
					m_nActualDepth,
					m_nActualMipCount,
					&nRefCount );
	int nDelta = nRefCount - nRefCountOld;
	m_nRefCount += nDelta;
#endif
} 
static void QueuedLoaderCallback( void *pContext, void *pContext2, const void *pData, int nSize, LoaderError_t loaderError )
{
	reinterpret_cast< CTexture * >( pContext )->FixupTexture( pData, nSize, loaderError );
}

//-----------------------------------------------------------------------------
// Generates the procedural bits
//-----------------------------------------------------------------------------
IVTFTexture *CTexture::ReconstructPartialProceduralBits( const Rect_t *pRect, Rect_t *pActualRect )
{
	// Figure out the actual size for this texture based on the current mode
	ComputeActualSize();

	// Figure out how many mip levels we're skipping...
	int nSizeFactor = GetMappingWidth() / GetActualWidth();
	int nMipSkipCount = 0;
	while (nSizeFactor > 1)
	{
		nSizeFactor >>= 1;
		++nMipSkipCount;
	}

	// Determine a rectangle appropriate for the actual size...
	// It must bound all partially-covered pixels..
	ComputeMipLevelSubRect( pRect, nMipSkipCount, pActualRect );

	if ( IsX360() && !IsDebug() && !m_pTextureRegenerator )
	{
		// no checkerboards in 360 release
		return NULL;
	}

	// Create the texture
	IVTFTexture *pVTFTexture = GetScratchVTFTexture();

	// Initialize the texture
	pVTFTexture->Init( m_nActualWidth, m_nActualHeight, m_nActualDepth,
		ComputeActualFormat( m_ImageFormat ), m_nFlags, m_nFrameCount );

	// Generate the bits from the installed procedural regenerator
	if ( m_pTextureRegenerator )
	{
		m_pTextureRegenerator->RegenerateTextureBits( this, pVTFTexture, pActualRect );
	}
	else
	{
		// In this case, we don't have one, so just use a checkerboard...
		TextureManager()->GenerateErrorTexture( this, pVTFTexture );
	}

	return pVTFTexture;
}


//-----------------------------------------------------------------------------
// Regenerates the bits of a texture within a particular rectangle
//-----------------------------------------------------------------------------
void CTexture::ReconstructPartialTexture( const Rect_t *pRect )
{
	// FIXME: for now, only procedural textures can handle sub-rect specification.
	Assert( IsProcedural() );

	// Also, we need procedural textures that have only a single copy!!
	// Otherwise this partial upload will not occur on all copies
	Assert( m_nFlags & TEXTUREFLAGS_SINGLECOPY );

	Rect_t vtfRect;
	IVTFTexture *pVTFTexture = ReconstructPartialProceduralBits( pRect, &vtfRect );

	// FIXME: for now, depth textures do not work with this.
	Assert( pVTFTexture->Depth() == 1 );

	// Make sure we've allocated the API textures
	if ( !HasBeenAllocated() )
	{
		AllocateShaderAPITextures();
	}

	if ( IsX360() && !pVTFTexture )
	{
		// 360 inhibited procedural generation
		return;
	}

	int nFaceCount, nFirstFace;
	GetDownloadFaceCount( nFirstFace, nFaceCount );
	
	// Blit down portions of the various VTF frames into the board memory
	int nStride;
	Rect_t mipRect;
	for ( int iFrame = 0; iFrame < m_nFrameCount; ++iFrame )
	{
		Modify( iFrame );

		for ( int iFace = 0; iFace < nFaceCount; ++iFace )
		{
			for ( int iMip = 0; iMip < m_nActualMipCount;  ++iMip )
			{
				pVTFTexture->ComputeMipLevelSubRect( &vtfRect, iMip, &mipRect );
				nStride = pVTFTexture->RowSizeInBytes( iMip );
				unsigned char *pBits = pVTFTexture->ImageData( iFrame, iFace + nFirstFace, iMip, mipRect.x, mipRect.y, 0 );
				g_pShaderAPI->TexSubImage2D( 
					iMip, 
					iFace, 
					mipRect.x, 
					mipRect.y,
					0,
					mipRect.width, 
					mipRect.height, 
					pVTFTexture->Format(), 
					nStride, 
#if defined( _X360 )
					pVTFTexture->IsPreTiled(),
#else
					false,
#endif
					pBits );
			}
		}
	}

#if defined( _X360 )
	// hint the vtf system to release memory associated with its load
	pVTFTexture->ReleaseImageMemory();
#endif
}


//-----------------------------------------------------------------------------
// Generates the procedural bits
//-----------------------------------------------------------------------------
IVTFTexture *CTexture::ReconstructProceduralBits()
{
	// Figure out the actual size for this texture based on the current mode
	ComputeActualSize();

	if ( IsX360() && !IsDebug() && !m_pTextureRegenerator )
	{
		// no checkerboards in 360 release
		return NULL;
	}

	// Create the texture
	IVTFTexture *pVTFTexture = GetScratchVTFTexture();

	// Initialize the texture
	pVTFTexture->Init( m_nActualWidth, m_nActualHeight, m_nActualDepth,
		ComputeActualFormat( m_ImageFormat ), m_nFlags, m_nFrameCount );

	// Generate the bits from the installed procedural regenerator
	if ( m_pTextureRegenerator )
	{
		Rect_t rect;
		rect.x = 0; rect.y = 0;
		rect.width = m_nActualWidth; 
		rect.height = m_nActualHeight; 
		m_pTextureRegenerator->RegenerateTextureBits( this, pVTFTexture, &rect );
	}
	else
	{
		// In this case, we don't have one, so just use a checkerboard...
		TextureManager()->GenerateErrorTexture( this, pVTFTexture );
	}

	return pVTFTexture;
}

void CTexture::WriteDataToShaderAPITexture( int nFrameCount, int nFaceCount, int nFirstFace, int nMipCount, IVTFTexture *pVTFTexture, ImageFormat fmt )
{
	for ( int iFrame = 0; iFrame < m_nFrameCount; ++iFrame )
	{
#if defined( COMPRESSED_NORMAL_FORMATS )
		Modify( iFrame, fmt == m_ImageFormatPaired );
#else
		Modify( iFrame );
#endif

		for ( int iFace = 0; iFace < nFaceCount; ++iFace )
		{
			for ( int iMip = 0; iMip < nMipCount; ++iMip )
			{
				unsigned char *pBits;
				int nWidth, nHeight, nDepth;
				pVTFTexture->ComputeMipLevelDimensions( iMip, &nWidth, &nHeight, &nDepth );
				for ( int z = 0; z < nDepth; ++z )
				{
					pBits = pVTFTexture->ImageData( iFrame, iFace + nFirstFace, iMip, 0, 0, z );

					g_pShaderAPI->TexImage2D( iMip, iFace, fmt, z, nWidth, nHeight, pVTFTexture->Format(), false, pBits );
				}
			}
		}
	}
}

//-----------------------------------------------------------------------------
// Sets or updates the texture bits
//-----------------------------------------------------------------------------
void CTexture::ReconstructTexture()
{
	int oldWidth = m_nActualWidth;
	int oldHeight = m_nActualHeight;
	int oldDepth = m_nActualDepth;
	int oldMipCount = m_nActualMipCount;
	int oldFrameCount = m_nFrameCount;

	// FIXME: Should RenderTargets be a special case of Procedural?
	char *pResolvedFilename = NULL;
	IVTFTexture *pVTFTexture = NULL;
	char pCacheFileName[MATERIAL_MAX_PATH] = { 0 };

#if defined( COMPRESSED_NORMAL_FORMATS )
	// Optional ATI1N
	bool bHasATI1N = false;
	IVTFTexture *pVTFTextureA = NULL;
#endif

	if ( IsProcedural() )
	{
		// This will call the installed texture bit regeneration interface
		pVTFTexture = ReconstructProceduralBits();
	}
	else if ( IsRenderTarget() )
	{
		// Compute the actual size + format based on the current mode
		ComputeActualSize( true );
	}
	else
	{
		const char *pName;
		if ( m_nInternalFlags & TEXTUREFLAGSINTERNAL_SHOULDEXCLUDE )
		{
			pName = "dev/dev_exclude_error";
		}
		else
		{
			pName = m_Name.String();
		}
	
		bool bIsUNCName = ( pName[0] == '/' && pName[1] == '/' && pName[2] != '/' );
		if ( !bIsUNCName )
		{
			Q_snprintf( pCacheFileName, sizeof( pCacheFileName ), "materials/%s" TEXTURE_FNAME_EXTENSION, pName );
		}
		else
		{
			Q_snprintf( pCacheFileName, sizeof( pCacheFileName ), "%s" TEXTURE_FNAME_EXTENSION, pName );
		}

#if defined( COMPRESSED_NORMAL_FORMATS )
		bool bHasATI2N = false;
		char pFileNameATI2N[MATERIAL_MAX_PATH] = { 0 };
		char pFileNameATI1N[MATERIAL_MAX_PATH] = { 0 };

		// Currently 360 only
		// If we support normal compression and file ends in _normal, see if there's a compressed version on disk
		if ( IsX360() && g_pShaderAPI->SupportsNormalMapCompression() && V_strstr( pCacheFileName, TEXTURE_FNAME_EXTENSION_NORMAL ) )
		{
			V_strncpy( pFileNameATI2N, pCacheFileName, strlen(pCacheFileName) - TEXTURE_FNAME_EXTENSION_LEN + 1 );
			V_strncat( pFileNameATI2N, "_xya", MATERIAL_MAX_PATH );
			V_strncat( pFileNameATI2N, TEXTURE_FNAME_EXTENSION, MATERIAL_MAX_PATH );

			// Is there an _xya version on disk?
			if ( g_pFullFileSystem->FileExists( pFileNameATI2N, MaterialSystem()->GetForcedTextureLoadPathID() ) )	// Both _xya and _a are there
			{
				V_strncpy( pFileNameATI1N, pCacheFileName, strlen(pCacheFileName) - TEXTURE_FNAME_EXTENSION_LEN + 1 );
				V_strncat( pFileNameATI1N, "_a", MATERIAL_MAX_PATH );
				V_strncat( pFileNameATI1N, TEXTURE_FNAME_EXTENSION, MATERIAL_MAX_PATH );
				bHasATI1N = bHasATI2N = true;
			}
			else
			{
				V_strncpy( pFileNameATI2N, pCacheFileName, strlen(pCacheFileName) - TEXTURE_FNAME_EXTENSION_LEN + 1);
				V_strncat( pFileNameATI2N, "_xy", MATERIAL_MAX_PATH );
				V_strncat( pFileNameATI2N, TEXTURE_FNAME_EXTENSION, MATERIAL_MAX_PATH );
				if ( g_pFullFileSystem->FileExists( pFileNameATI2N, MaterialSystem()->GetForcedTextureLoadPathID() ) )	// Only _xy is there
				{
					bHasATI2N = true;
				}
			}
		}
		// Get the data from disk...
		// NOTE: Reloading the texture bits can cause the texture size, frames, format, pretty much *anything* can change.
		pVTFTexture = LoadTextureBitsFromFile( bHasATI2N ? pFileNameATI2N : pCacheFileName, &pResolvedFilename, false );
		if ( bHasATI1N )
		{
			pVTFTextureA = LoadTextureBitsFromFile( pFileNameATI1N, NULL, true );
			m_bPaired = true;
		}
#else
		// Get the data from disk...
		// NOTE: Reloading the texture bits can cause the texture size, frames, format, pretty much *anything* can change.
		pVTFTexture = LoadTextureBitsFromFile( pCacheFileName, &pResolvedFilename, false );
#endif
	}

	if ( !HasBeenAllocated() ||
		m_nActualWidth != oldWidth ||
		m_nActualHeight != oldHeight ||
		m_nActualDepth != oldDepth ||
		m_nActualMipCount != oldMipCount ||
		m_nFrameCount != oldFrameCount )
	{
		if ( HasBeenAllocated() )
		{
			// This is necessary for the reload case, we may discover there
			// are more frames of a texture animation, for example, which means
			// we can't rely on having the same number of texture frames.
			FreeShaderAPITextures();
		}

		// Create the shader api textures, except temp render targets on 360.
		if( !(IsX360() && IsTempRenderTarget()) )
		{
			AllocateShaderAPITextures();
		}
	}

	// Render Targets just need to be cleared, they have no upload
	if ( IsRenderTarget() )
	{
		// Clear the render target to opaque black
#if !defined( _X360 )
		CMatRenderContextPtr pRenderContext( MaterialSystem() );
		ITexture *pThisTexture = GetEmbeddedTexture( 0 );
		pRenderContext->PushRenderTargetAndViewport( pThisTexture );						// Push this texture on the stack
		g_pShaderAPI->ClearColor4ub( 0, 0, 0, 0xFF );										// Set the clear color to opaque black
		g_pShaderAPI->ClearBuffers( true, false, false, m_nActualWidth, m_nActualHeight );	// Clear the target
		pRenderContext->PopRenderTargetAndViewport();										// Pop back to previous target
#else
		// 360 may not have RT surface during init time
		// avoid complex conditionalizing, just cpu clear it, which always works
		ClearTexture( 0, 0, 0, 0xFF );
#endif
		// no upload
		return;
	}

	if ( IsX360() && IsProcedural() && !pVTFTexture )
	{
		// 360 explicitly inhibited this texture's procedural generation, so no upload needed
		return;
	}

	// Blit down the texture faces, frames, and mips into the board memory
	int nFirstFace, nFaceCount;
	GetDownloadFaceCount( nFirstFace, nFaceCount );
	
	if ( IsPC() )
	{
		WriteDataToShaderAPITexture( m_nFrameCount, nFaceCount, nFirstFace, m_nActualMipCount, pVTFTexture, m_ImageFormat );
#if defined( COMPRESSED_NORMAL_FORMATS )
		if ( bHasATI1N )
		{
			WriteDataToShaderAPITexture( m_nFrameCount, nFaceCount, nFirstFace, m_nActualMipCount, pVTFTextureA, m_ImageFormatPaired );
		}
#endif
	}

#if defined( _X360 )
	bool bDoUpload = true;
	if ( m_nInternalFlags & TEXTUREFLAGSINTERNAL_QUEUEDLOAD )
	{
		// the vtf didn't load any d3d bits, the hires bits will arrive before gameplay
		bDoUpload = false;
	}

	if ( bDoUpload )
	{
		for ( int iFrame = 0; iFrame < m_nFrameCount; ++iFrame )
		{
			Modify( iFrame );
			for ( int iFace = 0; iFace < nFaceCount; ++iFace )
			{
				for ( int iMip = 0; iMip < m_nActualMipCount; ++iMip )
				{
					unsigned char *pBits;
					int nWidth, nHeight, nDepth;
					pVTFTexture->ComputeMipLevelDimensions( iMip, &nWidth, &nHeight, &nDepth );
					pBits = pVTFTexture->ImageData( iFrame, iFace + nFirstFace, iMip, 0, 0, 0 );
					g_pShaderAPI->TexImage2D( iMip, iFace, m_ImageFormat, 0, nWidth, nHeight, 
						pVTFTexture->Format(), pVTFTexture->IsPreTiled(), pBits );
				}
			}
		}
	}

	if ( m_nInternalFlags & TEXTUREFLAGSINTERNAL_QUEUEDLOAD )
	{
		// the empty hires version was setup
		// the hires d3d bits will be delivered before gameplay (or render)
		LoaderPriority_t priority = LOADERPRIORITY_BEFOREPLAY;
		
		// add the job 
		LoaderJob_t loaderJob;
		loaderJob.m_pFilename = pResolvedFilename;
		loaderJob.m_pCallback = QueuedLoaderCallback;
		loaderJob.m_pContext = (void *)this;
		loaderJob.m_Priority =  priority;
		g_pQueuedLoader->AddJob( &loaderJob );
	}

	// hint the vtf system to release memory associated with its load
	pVTFTexture->ReleaseImageMemory();
#endif

	delete [] pResolvedFilename;

	// the 360 does not persist a large buffer
	// the pc can afford to persist a large buffer
	FreeOptimalReadBuffer( IsX360() ? 32*1024 : 4*1024*1024 );
}

// Get the shaderapi texture handle associated w/ a particular frame
ShaderAPITextureHandle_t CTexture::GetTextureHandle( int nFrame, int nTextureChannel )
{
	Assert( nFrame < m_nFrameCount );
	Assert( nTextureChannel < 2 );

#if defined( COMPRESSED_NORMAL_FORMATS )
	if ( nTextureChannel )
		return m_pTextureHandlesPaired[nFrame]; // Don't get paired handle here...callers of this function don't know about paired textures
#endif

	return m_pTextureHandles[nFrame]; // Don't get paired handle here...callers of this function don't know about paired textures
}

void CTexture::GetLowResColorSample( float s, float t, float *color ) const
{
	if ( m_LowResImageWidth <= 0 || m_LowResImageHeight <= 0 )
	{
//		Warning( "Programming error: GetLowResColorSample \"%s\": %dx%d\n", m_pName, ( int )m_LowResImageWidth, ( int )m_LowResImageHeight );
		return;
	}

#if !defined( _X360 )
	// force s and t into [0,1)
	if ( s < 0.0f )
	{
		s = ( 1.0f - ( float )( int )s ) + s;
	}
	if ( t < 0.0f )
	{
		t = ( 1.0f - ( float )( int )t ) + t;
	}
	s = s - ( float )( int )s;
	t = t - ( float )( int )t;
	
	s *= m_LowResImageWidth;
	t *= m_LowResImageHeight;
	
	int wholeS, wholeT;
	wholeS = ( int )s;
	wholeT = ( int )t;
	float fracS, fracT;
	fracS = s - ( float )( int )s;
	fracT = t - ( float )( int )t;
	
	// filter twice in the s dimension.
	float sColor[2][3];
	int wholeSPlusOne = ( wholeS + 1 ) % m_LowResImageWidth;
	int wholeTPlusOne = ( wholeT + 1 ) % m_LowResImageHeight;
	sColor[0][0] = ( 1.0f - fracS ) * ( m_pLowResImage[( wholeS + wholeT * m_LowResImageWidth ) * 3 + 0] * ( 1.0f / 255.0f ) );
	sColor[0][1] = ( 1.0f - fracS ) * ( m_pLowResImage[( wholeS + wholeT * m_LowResImageWidth ) * 3 + 1] * ( 1.0f / 255.0f ) );
	sColor[0][2] = ( 1.0f - fracS ) * ( m_pLowResImage[( wholeS + wholeT * m_LowResImageWidth ) * 3 + 2] * ( 1.0f / 255.0f ) );
	sColor[0][0] += fracS * ( m_pLowResImage[( wholeSPlusOne + wholeT * m_LowResImageWidth ) * 3 + 0] * ( 1.0f / 255.0f ) );
	sColor[0][1] += fracS * ( m_pLowResImage[( wholeSPlusOne + wholeT * m_LowResImageWidth ) * 3 + 1] * ( 1.0f / 255.0f ) );
	sColor[0][2] += fracS * ( m_pLowResImage[( wholeSPlusOne + wholeT * m_LowResImageWidth ) * 3 + 2] * ( 1.0f / 255.0f ) );

	sColor[1][0] = ( 1.0f - fracS ) * ( m_pLowResImage[( wholeS + wholeTPlusOne * m_LowResImageWidth ) * 3 + 0] * ( 1.0f / 255.0f ) );
	sColor[1][1] = ( 1.0f - fracS ) * ( m_pLowResImage[( wholeS + wholeTPlusOne * m_LowResImageWidth ) * 3 + 1] * ( 1.0f / 255.0f ) );
	sColor[1][2] = ( 1.0f - fracS ) * ( m_pLowResImage[( wholeS + wholeTPlusOne * m_LowResImageWidth ) * 3 + 2] * ( 1.0f / 255.0f ) );
	sColor[1][0] += fracS * ( m_pLowResImage[( wholeSPlusOne + wholeTPlusOne * m_LowResImageWidth ) * 3 + 0] * ( 1.0f / 255.0f ) );
	sColor[1][1] += fracS * ( m_pLowResImage[( wholeSPlusOne + wholeTPlusOne * m_LowResImageWidth ) * 3 + 1] * ( 1.0f / 255.0f ) );
	sColor[1][2] += fracS * ( m_pLowResImage[( wholeSPlusOne + wholeTPlusOne * m_LowResImageWidth ) * 3 + 2] * ( 1.0f / 255.0f ) );

	color[0] = sColor[0][0] * ( 1.0f - fracT ) + sColor[1][0] * fracT;
	color[1] = sColor[0][1] * ( 1.0f - fracT ) + sColor[1][1] * fracT;
	color[2] = sColor[0][2] * ( 1.0f - fracT ) + sColor[1][2] * fracT;
#else
	color[0] = (float)m_LowResImageSample[0] * 1.0f/255.0f;
	color[1] = (float)m_LowResImageSample[1] * 1.0f/255.0f;
	color[2] = (float)m_LowResImageSample[2] * 1.0f/255.0f;
#endif
}

int CTexture::GetApproximateVidMemBytes( void ) const
{
	ImageFormat format = GetImageFormat();
	int width = GetActualWidth();
	int height = GetActualHeight();
	int depth = GetActualDepth();
	int numFrames = GetNumAnimationFrames();
	bool isMipmapped = IsMipmapped();

	return numFrames * ImageLoader::GetMemRequired( width, height, depth, format, isMipmapped );
}

void CTexture::CopyFrameBufferToMe( int nRenderTargetID, Rect_t *pSrcRect, Rect_t *pDstRect )
{
	Assert( m_pTextureHandles && m_nFrameCount >= 1 );

	if( IsX360() &&
		(m_nInternalFlags & TEXTUREFLAGSINTERNAL_TEMPRENDERTARGET) &&
		!HasBeenAllocated() )
	{
		//need to create the texture bits now
		AllocateShaderAPITextures(); //to avoid creating the texture bits previously, we simply skipped this step
	}

	if ( m_pTextureHandles && m_nFrameCount >= 1 )
	{
		g_pShaderAPI->CopyRenderTargetToTextureEx( m_pTextureHandles[0], nRenderTargetID, pSrcRect, pDstRect );
	}
}

ITexture *CTexture::GetEmbeddedTexture( int nIndex )
{
	return ( nIndex == 0 ) ? this : NULL;
}

//-----------------------------------------------------------------------------
// Helper method to initialize texture bits in desired state.
//-----------------------------------------------------------------------------
#if defined( _X360 )
bool CTexture::ClearTexture( int r, int g, int b, int a )
{
	Assert( IsProcedural() || IsRenderTarget() );
	if( !HasBeenAllocated() )
		return false;

	if ( m_ImageFormat == IMAGE_FORMAT_X360_DST16 || m_ImageFormat == IMAGE_FORMAT_X360_DST24 || m_ImageFormat == IMAGE_FORMAT_X360_DST24F )
	{
		// not supporting non-rgba textures
		return true;
	}

	CPixelWriter writer;
	g_pShaderAPI->ModifyTexture( m_pTextureHandles[0] );
	if ( !g_pShaderAPI->TexLock( 0, 0, 0, 0, m_nActualWidth, m_nActualHeight, writer ) )
		return false;

	writer.Seek( 0, 0 );
	for ( int j = 0; j < m_nActualHeight; ++j )
	{
		for ( int k = 0; k < m_nActualWidth; ++k )
		{
			writer.WritePixel( r, g, b, a );
		}
	}
	g_pShaderAPI->TexUnlock();

	return true;
}
#endif

#if defined( _X360 )
bool CTexture::CreateRenderTargetSurface( int width, int height, ImageFormat format, bool bSameAsTexture )
{
	Assert( IsRenderTarget() && m_nFrameCount > 1 );

	if ( bSameAsTexture )
	{
		// use RT texture configuration
		width = m_nActualWidth;
		height = m_nActualHeight;
		format = m_ImageFormat;
	}

	// RT surface is expected at end of array
	m_pTextureHandles[m_nFrameCount-1] = g_pShaderAPI->CreateRenderTargetSurface( width, height, format, GetName(), TEXTURE_GROUP_RENDER_TARGET_SURFACE );

	return ( m_pTextureHandles[m_nFrameCount-1] != INVALID_SHADERAPI_TEXTURE_HANDLE );
}
#endif

void CTexture::DeleteIfUnreferenced()
{
	if ( m_nRefCount > 0 )
		return;

	TextureManager()->RemoveTexture( this );
}

//Swap everything about a texture except the name. Created to support Portal mod's need for swapping out water render targets in recursive stencil views
void CTexture::SwapContents( ITexture *pOther )
{
	if( (pOther == NULL) || (pOther == this) )
		return;

	AssertMsg( dynamic_cast<CTexture *>(pOther) != NULL, "Texture swapping broken" );

	CTexture *pOtherAsCTexture = (CTexture *)pOther;

	CTexture *pTemp = (CTexture *)stackalloc( sizeof( CTexture ) );
	
	//swap everything
	memcpy( pTemp, this, sizeof( CTexture ) );
	memcpy( this, pOtherAsCTexture, sizeof( CTexture ) );
	memcpy( pOtherAsCTexture, pTemp, sizeof( CTexture ) );

	//we have the other's name, give it back
	memcpy( &pOtherAsCTexture->m_Name, &m_Name, sizeof( m_Name ) );

	//pTemp still has our name
	memcpy( &m_Name, &pTemp->m_Name, sizeof( m_Name ) );
}

void CTexture::MarkAsPreloaded( bool bSet )
{
	if ( bSet )
	{
		m_nInternalFlags |= TEXTUREFLAGSINTERNAL_PRELOADED;
	}
	else
	{
		m_nInternalFlags &= ~TEXTUREFLAGSINTERNAL_PRELOADED;
	}
}

bool CTexture::IsPreloaded() const
{
	return ( ( m_nInternalFlags & TEXTUREFLAGSINTERNAL_PRELOADED ) != 0 );
}

void CTexture::MarkAsExcluded( bool bSet, int nDimensionsLimit )
{
	if ( bSet )
	{
		// exclusion trumps picmipping
		m_nInternalFlags |= TEXTUREFLAGSINTERNAL_SHOULDEXCLUDE;
		m_nDesiredDimensionLimit = 0;
	}
	else
	{
		// not excluding, but can optionally picmip
		m_nInternalFlags &= ~TEXTUREFLAGSINTERNAL_SHOULDEXCLUDE;
		m_nDesiredDimensionLimit = nDimensionsLimit;
	}
}

bool CTexture::UpdateExcludedState( void )
{
	bool bDesired = ( m_nInternalFlags & TEXTUREFLAGSINTERNAL_SHOULDEXCLUDE ) != 0;
	bool bActual = ( m_nInternalFlags & TEXTUREFLAGSINTERNAL_EXCLUDED ) != 0;
	if ( ( bDesired == bActual ) && ( m_nDesiredDimensionLimit == m_nActualDimensionLimit ) )
	{
 		return false;
	}

	if ( m_nInternalFlags & TEXTUREFLAGSINTERNAL_QUEUEDLOAD )
	{
		// already scheduled
		return true;
	}

	// force the texture to re-download, causes the texture bits to match its desired exclusion state
	Download();

	return true;
}


//////////////////////////////////////////////////////////////////////////
//
// Saving all the texture LOD modifications to content
//
//////////////////////////////////////////////////////////////////////////

#ifdef IS_WINDOWS_PC
static bool SetBufferValue( char *chTxtFileBuffer, char const *szLookupKey, char const *szNewValue )
{
	bool bResult = false;
	
	size_t lenTmp = strlen( szNewValue );
	size_t nTxtFileBufferLen = strlen( chTxtFileBuffer );
	
	for ( char *pch = chTxtFileBuffer;
		( NULL != ( pch = strstr( pch, szLookupKey ) ) );
		++ pch )
	{
		char *val = pch + strlen( szLookupKey );
		if ( !isspace( *val ) )
			continue;
		else
			++ val;
		char *pValStart = val;

		// Okay, here comes the value
		while ( *val && isspace( *val ) )
			++ val;
		while ( *val && !isspace( *val ) )
			++ val;

		char *pValEnd = val; // Okay, here ends the value

		memmove( pValStart + lenTmp, pValEnd, chTxtFileBuffer + nTxtFileBufferLen + 1 - pValEnd );
		memcpy( pValStart, szNewValue, lenTmp );

		nTxtFileBufferLen += ( lenTmp - ( pValEnd - pValStart ) );
		bResult = true;
	}

	if ( !bResult )
	{
		char *pchAdd = chTxtFileBuffer + nTxtFileBufferLen;
		strcpy( pchAdd + strlen( pchAdd ), "\n" );
		strcpy( pchAdd + strlen( pchAdd ), szLookupKey );
		strcpy( pchAdd + strlen( pchAdd ), " " );
		strcpy( pchAdd + strlen( pchAdd ), szNewValue );
		strcpy( pchAdd + strlen( pchAdd ), "\n" );
		bResult = true;
	}

	return bResult;
}

// Replaces the first occurrence of "szFindData" with "szNewData"
// Returns the remaining buffer past the replaced data or NULL if
// no replacement occurred.
static char * BufferReplace( char *buf, char const *szFindData, char const *szNewData )
{
	size_t len = strlen( buf ), lFind = strlen( szFindData ), lNew = strlen( szNewData );
	if ( char *pBegin = strstr( buf, szFindData ) )
	{
		memmove( pBegin + lNew, pBegin + lFind, buf + len - ( pBegin + lFind ) );
		memmove( pBegin, szNewData, lNew );
		return pBegin + lNew;
	}
	return NULL;
}


class CP4Requirement
{
public:
	CP4Requirement();
	~CP4Requirement();

protected:
	bool m_bLoadedModule;
	CSysModule *m_pP4Module;
};

CP4Requirement::CP4Requirement() :
	m_bLoadedModule( false ),
	m_pP4Module( NULL )
{
	if ( p4 )
		return;

	// load the p4 lib
	m_pP4Module = Sys_LoadModule( "p4lib" );
	m_bLoadedModule = true;
		
	if ( m_pP4Module )
	{
		CreateInterfaceFn factory = Sys_GetFactory( m_pP4Module );
		if ( factory )
		{
			p4 = ( IP4 * )factory( P4_INTERFACE_VERSION, NULL );

			if ( p4 )
			{
				extern CreateInterfaceFn g_fnMatSystemConnectCreateInterface;
				p4->Connect( g_fnMatSystemConnectCreateInterface );
				p4->Init();
			}
		}
	}

	if ( !p4 )
	{
		Warning( "Can't load p4lib.dll\n" );
	}
}

CP4Requirement::~CP4Requirement()
{
	if ( m_bLoadedModule && m_pP4Module )
	{
		if ( p4 )
		{
			p4->Shutdown();
			p4->Disconnect();
		}

		Sys_UnloadModule( m_pP4Module );
		m_pP4Module = NULL;
		p4 = NULL;
	}
}

CON_COMMAND_F( mat_texture_list_txlod_sync, "'reset' - resets all run-time changes to LOD overrides, 'save' - saves all changes to material content files", FCVAR_DONTRECORD )
{
	using namespace TextureLodOverride;

	if ( args.ArgC() != 2 )
		goto usage;

	char const *szCmd = args.Arg( 1 );
	Msg( "mat_texture_list_txlod_sync %s...\n", szCmd );

	if ( !stricmp( szCmd, "reset" ) )
	{
		for ( int k = 0; k < s_OverrideMap.GetNumStrings(); ++ k )
		{
			char const *szTx = s_OverrideMap.String( k );
			s_OverrideMap[ k ] = OverrideInfo(); // Reset the override info

			// Force the texture LOD override to get re-processed
			if ( ITexture *pTx = materials->FindTexture( szTx, "" ) )
				pTx->ForceLODOverride( 0 );
			else
				Warning( " mat_texture_list_txlod_sync reset - texture '%s' no longer found.\n", szTx );
		}

		s_OverrideMap.Purge();
		Msg("mat_texture_list_txlod_sync reset : completed.\n");
		return;
	}
	else if ( !stricmp( szCmd, "save" ) )
	{
		CP4Requirement p4req;
		if ( !p4 )
			g_p4factory->SetDummyMode( true );

		for ( int k = 0; k < s_OverrideMap.GetNumStrings(); ++ k )
		{
			char const *szTx = s_OverrideMap.String( k );
			OverrideInfo oi = s_OverrideMap[ k ];
			ITexture *pTx = materials->FindTexture( szTx, "" );
			
			if ( !oi.x || !oi.y )
				continue;

			if ( !pTx )
			{
				Warning( " mat_texture_list_txlod_sync save - texture '%s' no longer found.\n", szTx );
				continue;
			}

			int iMaxWidth = pTx->GetActualWidth(), iMaxHeight = pTx->GetActualHeight();
			
			// Save maxwidth and maxheight
			char chMaxWidth[20], chMaxHeight[20];
			sprintf( chMaxWidth, "%d", iMaxWidth ), sprintf( chMaxHeight, "%d", iMaxHeight );

			// We have the texture and path to its content
			char chResolveName[ MAX_PATH ] = {0}, chResolveNameArg[ MAX_PATH ] = {0};
			Q_snprintf( chResolveNameArg, sizeof( chResolveNameArg ) - 1, "materials/%s" TEXTURE_FNAME_EXTENSION, szTx );
			char *szTextureContentPath = const_cast< char * >( g_pFullFileSystem->RelativePathToFullPath( chResolveNameArg, "game", chResolveName, sizeof( chResolveName ) - 1 ) );
			if ( !szTextureContentPath )
			{
				Warning( " mat_texture_list_txlod_sync save - texture '%s' is not loaded from file system.\n", szTx );
				continue;
			}
			if ( !BufferReplace( szTextureContentPath, "\\game\\", "\\content\\" ) ||
				 !BufferReplace( szTextureContentPath, "\\materials\\", "\\materialsrc\\" ) )
			{
				Warning( " mat_texture_list_txlod_sync save - texture '%s' cannot be mapped to content directory.\n", szTx );
				continue;
			}

			// Figure out what kind of source content is there:
			// 1. look for TGA - if found, get the txt file (if txt file missing, create one)
			// 2. otherwise look for PSD - affecting psdinfo
			// 3. else error
			char *pExtPut = szTextureContentPath + strlen( szTextureContentPath ) - strlen( TEXTURE_FNAME_EXTENSION ); // compensating the TEXTURE_FNAME_EXTENSION(.vtf) extension
			
			// 1.tga
			sprintf( pExtPut, ".tga" );
			if ( g_pFullFileSystem->FileExists( szTextureContentPath ) )
			{
				// Have tga - pump in the txt file
				sprintf( pExtPut, ".txt" );
				
				CUtlBuffer bufTxtFileBuffer( 0, 0, CUtlBuffer::TEXT_BUFFER );
				g_pFullFileSystem->ReadFile( szTextureContentPath, 0, bufTxtFileBuffer );
				for ( int k = 0; k < 1024; ++ k ) bufTxtFileBuffer.PutChar( 0 );

				// Now fix maxwidth/maxheight settings
				SetBufferValue( ( char * ) bufTxtFileBuffer.Base(), "maxwidth", chMaxWidth );
				SetBufferValue( ( char * ) bufTxtFileBuffer.Base(), "maxheight", chMaxHeight );
				bufTxtFileBuffer.SeekPut( CUtlBuffer::SEEK_HEAD, strlen( ( char * ) bufTxtFileBuffer.Base() ) );

				// Check out or add the file
				g_p4factory->SetOpenFileChangeList( "Texture LOD Autocheckout" );
				CP4AutoEditFile autop4_edit( szTextureContentPath );

				// Save the file contents
				if ( g_pFullFileSystem->WriteFile( szTextureContentPath, 0, bufTxtFileBuffer ) )
				{
					Msg(" '%s' : saved.\n", szTextureContentPath );
					CP4AutoAddFile autop4_add( szTextureContentPath );
				}
				else
				{
					Warning( " '%s' : failed to save - set \"maxwidth %d maxheight %d\" manually.\n",
						szTextureContentPath, iMaxWidth, iMaxHeight );
				}

				continue;
			}

			// 2.psd
			sprintf( pExtPut, ".psd" );
			if ( g_pFullFileSystem->FileExists( szTextureContentPath ) )
			{
				char chCommand[MAX_PATH];
				char szTxtFileName[MAX_PATH] = {0};
				GetModSubdirectory( "tmp_lod_psdinfo.txt", szTxtFileName, sizeof( szTxtFileName ) );
				sprintf( chCommand, "/C psdinfo \"%s\" > \"%s\"", szTextureContentPath, szTxtFileName);
				ShellExecute( NULL, NULL, "cmd.exe", chCommand, NULL, SW_HIDE );
				Sleep( 200 );

				CUtlBuffer bufTxtFileBuffer( 0, 0, CUtlBuffer::TEXT_BUFFER );
				g_pFullFileSystem->ReadFile( szTxtFileName, 0, bufTxtFileBuffer );
				for ( int k = 0; k < 1024; ++ k ) bufTxtFileBuffer.PutChar( 0 );

				// Now fix maxwidth/maxheight settings
				SetBufferValue( ( char * ) bufTxtFileBuffer.Base(), "maxwidth", chMaxWidth );
				SetBufferValue( ( char * ) bufTxtFileBuffer.Base(), "maxheight", chMaxHeight );
				bufTxtFileBuffer.SeekPut( CUtlBuffer::SEEK_HEAD, strlen( ( char * ) bufTxtFileBuffer.Base() ) );

				// Check out or add the file
				// Save the file contents
				if ( g_pFullFileSystem->WriteFile( szTxtFileName, 0, bufTxtFileBuffer ) )
				{
					g_p4factory->SetOpenFileChangeList( "Texture LOD Autocheckout" );
					CP4AutoEditFile autop4_edit( szTextureContentPath );

					sprintf( chCommand, "/C psdinfo -write \"%s\" < \"%s\"", szTextureContentPath, szTxtFileName );
					Sleep( 200 );
					ShellExecute( NULL, NULL, "cmd.exe", chCommand, NULL, SW_HIDE );
					Sleep( 200 );

					Msg(" '%s' : saved.\n", szTextureContentPath );
					CP4AutoAddFile autop4_add( szTextureContentPath );
				}
				else
				{
					Warning( " '%s' : failed to save - set \"maxwidth %d maxheight %d\" manually.\n",
						szTextureContentPath, iMaxWidth, iMaxHeight );
				}

				continue;
			}

			// 3. - error
			sprintf( pExtPut, "" );
			{
				Warning( " '%s' : doesn't specify a valid TGA or PSD file!\n", szTextureContentPath );
				continue;
			}
		}

		Msg("mat_texture_list_txlod_sync save : completed.\n");
		return;
	}
	else
		goto usage;

	return;

usage:
	Warning(
		"Usage:\n"
		"  mat_texture_list_txlod_sync reset - resets all run-time changes to LOD overrides;\n"
		"  mat_texture_list_txlod_sync save  - saves all changes to material content files.\n"
		);
}
#endif

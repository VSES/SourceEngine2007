//====== Copyright © 1996-2006, Valve Corporation, All rights reserved. =======
//
// Purpose: 
//
//=============================================================================

#include "avi/ibik.h"
#if !defined( _X360 )
#	include "bink/bink.h"
#else
#	include "bink_x360/bink.h"
#endif
#include "filesystem.h"
#include "tier1/strtools.h"
#include "tier1/utllinkedlist.h"
#include "tier1/keyvalues.h"
#include "materialsystem/imaterial.h"
#include "materialsystem/imaterialsystem.h"
#include "materialsystem/materialsystemutil.h"
#include "materialsystem/itexture.h"
#include "vtf/vtf.h"
#include "pixelwriter.h"
#include "tier3/tier3.h"

#pragma warning( disable : 4201 )

#define WIN32_LEAN_AND_MEAN

#pragma warning( default : 4201 )

// We don't support the alpha channel in bink files due to dx8.  Can make it work if necessary.
//#define SUPPORT_BINK_ALPHA

class CBIKMaterial;

class CBIKMaterialYTextureRegenerator : public ITextureRegenerator
{
public:
	void SetParentMaterial( CBIKMaterial *pBIKMaterial, int nWidth, int nHeight )
	{
		m_pBIKMaterial = pBIKMaterial;
		m_nSourceWidth = nWidth;
		m_nSourceHeight = nHeight;
	}

	// Inherited from ITextureRegenerator
	virtual void RegenerateTextureBits( ITexture *pTexture, IVTFTexture *pVTFTexture, Rect_t *pRect );
	virtual void Release();

private:
	CBIKMaterial	*m_pBIKMaterial;
	int				m_nSourceWidth;
	int				m_nSourceHeight;
};

#ifdef SUPPORT_BINK_ALPHA
class CBIKMaterialATextureRegenerator : public ITextureRegenerator
{
public:
	void SetParentMaterial( CBIKMaterial *pBIKMaterial, int nWidth, int nHeight )
	{
		m_pBIKMaterial = pBIKMaterial;
		m_nSourceWidth = nWidth;
		m_nSourceHeight = nHeight;
	}

	// Inherited from ITextureRegenerator
	virtual void RegenerateTextureBits( ITexture *pTexture, IVTFTexture *pVTFTexture, Rect_t *pRect );
	virtual void Release();

private:
	CBIKMaterial	*m_pBIKMaterial;
	int				m_nSourceWidth;
	int				m_nSourceHeight;
};
#endif

class CBIKMaterialCrTextureRegenerator : public ITextureRegenerator
{
public:
	void SetParentMaterial( CBIKMaterial *pBIKMaterial, int nWidth, int nHeight )
	{
		m_pBIKMaterial = pBIKMaterial;
		m_nSourceWidth = nWidth;
		m_nSourceHeight = nHeight;
	}

	// Inherited from ITextureRegenerator
	virtual void RegenerateTextureBits( ITexture *pTexture, IVTFTexture *pVTFTexture, Rect_t *pRect );
	virtual void Release();

private:
	CBIKMaterial	 *m_pBIKMaterial;
	int				m_nSourceWidth;
	int				m_nSourceHeight;
};

class CBIKMaterialCbTextureRegenerator : public ITextureRegenerator
{
public:
	void SetParentMaterial( CBIKMaterial *pBIKMaterial, int nWidth, int nHeight )
	{
		m_pBIKMaterial = pBIKMaterial;
		m_nSourceWidth = nWidth;
		m_nSourceHeight = nHeight;
	}

	// Inherited from ITextureRegenerator
	virtual void RegenerateTextureBits( ITexture *pTexture, IVTFTexture *pVTFTexture, Rect_t *pRect );
	virtual void Release();

private:
	CBIKMaterial	 *m_pBIKMaterial;
	int				m_nSourceWidth;
	int				m_nSourceHeight;
};

//-----------------------------------------------------------------------------
//
// Class used to associated BIK files with IMaterials
//
//-----------------------------------------------------------------------------
class CBIKMaterial
{
public:
	CBIKMaterial();

	// Initializes, shuts down the material
	bool Init( const char *pMaterialName, const char *pFileName, const char *pPathID );
	void Shutdown();

	// Keeps the frames updated
	bool Update( void );

	// Returns the material
	IMaterial *GetMaterial();

	// Returns the texcoord range
	void GetTexCoordRange( float *pMaxU, float *pMaxV );

	// Returns the frame size of the BIK (stored in a subrect of the material itself)
	void GetFrameSize( int *pWidth, int *pHeight );

	// Sets the current time
	void SetTime( float flTime );

	// Returns the frame rate/count of the BIK
	int GetFrameRate( );
	int GetFrameCount( );

	// Sets the frame for an BIK material (use instead of SetTime)
	void SetFrame( float flFrame );

private:

	friend class CBIKMaterialYTextureRegenerator;
#ifdef SUPPORT_BINK_ALPHA
	friend class CBIKMaterialATextureRegenerator;
#endif
	friend class CBIKMaterialCrTextureRegenerator;
	friend class CBIKMaterialCbTextureRegenerator;

	// Initializes, shuts down the procedural texture
	void CreateProceduralTextures( const char *pTextureName );
	void DestroyProceduralTexture( CTextureReference &texture );
	void DestroyProceduralTextures();

	// Initializes, shuts down the procedural material
	void CreateProceduralMaterial( const char *pMaterialName );
	void DestroyProceduralMaterial();

	// Initializes, shuts down the video stream
	void CreateVideoStream( );
	void DestroyVideoStream( );

	CMaterialReference m_Material;
	CTextureReference m_TextureY;
#ifdef SUPPORT_BINK_ALPHA
	CTextureReference m_TextureA;
#endif
	CTextureReference m_TextureCr;
	CTextureReference m_TextureCb;

	HBINK m_pHBINK;

	BINKFRAMEBUFFERS m_buffers;

	int m_nBIKWidth;
	int m_nBIKHeight;

	int m_nFrameRate;
	int m_nFrameCount;

	int m_nCurrentFrame;

	CBIKMaterialYTextureRegenerator m_YTextureRegenerator;
#ifdef SUPPORT_BINK_ALPHA
	CBIKMaterialATextureRegenerator m_ATextureRegenerator;
#endif
	CBIKMaterialCrTextureRegenerator m_CrTextureRegenerator;
	CBIKMaterialCbTextureRegenerator m_CbTextureRegenerator;
};

//-----------------------------------------------------------------------------
// Inherited from ITextureRegenerator
//-----------------------------------------------------------------------------
void CBIKMaterialYTextureRegenerator::RegenerateTextureBits( ITexture *pTexture, IVTFTexture *pVTFTexture, Rect_t *pRect )
{
	// Error condition
	if ( (pVTFTexture->FrameCount() > 1) || (pVTFTexture->FaceCount() > 1) || (pVTFTexture->MipCount() > 1) || (pVTFTexture->Depth() > 1) )
	{
		goto BIKMaterialError;
	}

	unsigned char *pYData = (unsigned char *)m_pBIKMaterial->m_buffers.Frames[ m_pBIKMaterial->m_buffers.FrameNum ].YPlane.Buffer;

	Assert( pVTFTexture->Format() == IMAGE_FORMAT_I8 );
	Assert( pVTFTexture->RowSizeInBytes( 0 ) == pVTFTexture->Width() );
	Assert( pVTFTexture->Width() >= m_nSourceWidth );
	Assert( pVTFTexture->Height() >= m_nSourceHeight );

	// Set up the pixel writer to write into the VTF texture
	CPixelWriter pixelWriter;
	pixelWriter.SetPixelMemory( pVTFTexture->Format(), pVTFTexture->ImageData(), pVTFTexture->RowSizeInBytes( 0 ) );

	int nWidth = m_nSourceWidth;
	int nHeight = m_nSourceHeight;
	int y;
	for ( y = 0; y < nHeight; ++y )
	{
		pixelWriter.Seek( 0, y );
		memcpy( pixelWriter.GetCurrentPixel(), pYData, nWidth );
		pYData += nWidth;
	}

	return;

BIKMaterialError:
	int nBytes = pVTFTexture->ComputeTotalSize();
	memset( pVTFTexture->ImageData(), 0xFF, nBytes );
	return;
}

void CBIKMaterialYTextureRegenerator::Release()
{
}

#ifdef SUPPORT_BINK_ALPHA
//-----------------------------------------------------------------------------
// Inherited from ITextureRegenerator
//-----------------------------------------------------------------------------
void CBIKMaterialATextureRegenerator::RegenerateTextureBits( ITexture *pTexture, IVTFTexture *pVTFTexture, Rect_t *pRect )
{
	// Error condition
	if ( (pVTFTexture->FrameCount() > 1) || (pVTFTexture->FaceCount() > 1) || (pVTFTexture->MipCount() > 1) || (pVTFTexture->Depth() > 1) )
	{
		goto BIKMaterialError;
	}

	unsigned char *pAData = (unsigned char *)m_pBIKMaterial->m_buffers.Frames[ m_pBIKMaterial->m_buffers.FrameNum ].APlane.Buffer;

	Assert( pVTFTexture->Format() == IMAGE_FORMAT_I8 );
	Assert( pVTFTexture->RowSizeInBytes( 0 ) == pVTFTexture->Width() );
	Assert( pVTFTexture->Width() >= m_nSourceWidth );
	Assert( pVTFTexture->Height() >= m_nSourceHeight );

	// Set up the pixel writer to write into the VTF texture
	CPixelWriter pixelWriter;
	pixelWriter.SetPixelMemory( pVTFTexture->Format(), pVTFTexture->ImageData(), pVTFTexture->RowSizeInBytes( 0 ) );

	int nWidth = m_nSourceWidth;
	int nHeight = m_nSourceHeight;
	int y;
	if( pAData )
	{
		for ( y = 0; y < nHeight; ++y )
		{
			pixelWriter.Seek( 0, y );
			memcpy( pixelWriter.GetCurrentPixel(), pAData, nWidth );
			pAData += nWidth;
		}
	}
	else
	{
		for ( y = 0; y < nHeight; ++y )
		{
			pixelWriter.Seek( 0, y );
			memset( pixelWriter.GetCurrentPixel(), 255, nWidth );
		}
	}

	return;

BIKMaterialError:
	int nBytes = pVTFTexture->ComputeTotalSize();
	memset( pVTFTexture->ImageData(), 0xFF, nBytes );
	return;
}

void CBIKMaterialATextureRegenerator::Release()
{
}
#endif

//-----------------------------------------------------------------------------
// Inherited from ITextureRegenerator
//-----------------------------------------------------------------------------
void CBIKMaterialCrTextureRegenerator::RegenerateTextureBits( ITexture *pTexture, IVTFTexture *pVTFTexture, Rect_t *pRect )
{
	// Error condition
	if ( (pVTFTexture->FrameCount() > 1) || (pVTFTexture->FaceCount() > 1) || (pVTFTexture->MipCount() > 1) || (pVTFTexture->Depth() > 1) )
	{
		goto BIKMaterialError;
	}

	unsigned char *pCrData = (unsigned char *)m_pBIKMaterial->m_buffers.Frames[ m_pBIKMaterial->m_buffers.FrameNum ].cRPlane.Buffer;

	Assert( pVTFTexture->Format() == IMAGE_FORMAT_I8 );
	Assert( pVTFTexture->RowSizeInBytes( 0 ) == pVTFTexture->Width() );
	Assert( pVTFTexture->Width() >= ( m_nSourceWidth >> 1 ) );
	Assert( pVTFTexture->Height() >= ( m_nSourceHeight >> 1 ) );

	// Set up the pixel writer to write into the VTF texture
	CPixelWriter pixelWriter;
	pixelWriter.SetPixelMemory( pVTFTexture->Format(), pVTFTexture->ImageData(), pVTFTexture->RowSizeInBytes( 0 ) );

	int nWidth = m_nSourceWidth >> 1;
	int nHeight = m_nSourceHeight >> 1;
	int y;
	for ( y = 0; y < nHeight; ++y )
	{
		pixelWriter.Seek( 0, y );
		memcpy( pixelWriter.GetCurrentPixel(), pCrData, nWidth );
		pCrData += nWidth;
	}

	return;

BIKMaterialError:
	int nBytes = pVTFTexture->ComputeTotalSize();
	memset( pVTFTexture->ImageData(), 0xFF, nBytes );
	return;
}

void CBIKMaterialCrTextureRegenerator::Release()
{
}

//-----------------------------------------------------------------------------
// Inherited from ITextureRegenerator
//-----------------------------------------------------------------------------
void CBIKMaterialCbTextureRegenerator::RegenerateTextureBits( ITexture *pTexture, IVTFTexture *pVTFTexture, Rect_t *pRect )
{
	// Error condition
	if ( (pVTFTexture->FrameCount() > 1) || (pVTFTexture->FaceCount() > 1) || (pVTFTexture->MipCount() > 1) || (pVTFTexture->Depth() > 1) )
	{
		goto BIKMaterialError;
	}

	unsigned char *pCbData = (unsigned char *)m_pBIKMaterial->m_buffers.Frames[ m_pBIKMaterial->m_buffers.FrameNum ].cBPlane.Buffer;

	Assert( pVTFTexture->Format() == IMAGE_FORMAT_I8 );
	Assert( pVTFTexture->RowSizeInBytes( 0 ) == pVTFTexture->Width() );
	Assert( pVTFTexture->Width() >= ( m_nSourceWidth >> 1 ) );
	Assert( pVTFTexture->Height() >= ( m_nSourceHeight >> 1 ) );

	// Set up the pixel writer to write into the VTF texture
	CPixelWriter pixelWriter;
	pixelWriter.SetPixelMemory( pVTFTexture->Format(), pVTFTexture->ImageData(), pVTFTexture->RowSizeInBytes( 0 ) );

	int nWidth = m_nSourceWidth >> 1;
	int nHeight = m_nSourceHeight >> 1;
	int y;
	for ( y = 0; y < nHeight; ++y )
	{
		pixelWriter.Seek( 0, y );
		memcpy( pixelWriter.GetCurrentPixel(), pCbData, nWidth );
		pCbData += nWidth;
	}

	return;

BIKMaterialError:
	int nBytes = pVTFTexture->ComputeTotalSize();
	memset( pVTFTexture->ImageData(), 0xFF, nBytes );
	return;
}

void CBIKMaterialCbTextureRegenerator::Release()
{
}

//-----------------------------------------------------------------------------
// Constructor
//-----------------------------------------------------------------------------
CBIKMaterial::CBIKMaterial()
{
	m_pHBINK = NULL;
	Q_memset( &m_buffers, 0, sizeof( m_buffers ) );
}


//-----------------------------------------------------------------------------
// Initializes the material
//-----------------------------------------------------------------------------
bool CBIKMaterial::Init( const char *pMaterialName, const char *pFileName, const char *pPathID )
{
	// Determine the full path name of the BIK
	char pBIKFileName[ 512 ];
	char pFullBIKFileName[ 512 ];
	Q_snprintf( pBIKFileName, sizeof( pBIKFileName ), "%s", pFileName );
	Q_DefaultExtension( pBIKFileName, ".bik", sizeof( pBIKFileName ) );
	if ( !g_pFullFileSystem->RelativePathToFullPath( pBIKFileName, pPathID, pFullBIKFileName, sizeof( pFullBIKFileName ) ) )
	{
		// A file by that name was not found
		Assert( 0 );
		return false;
	}

	m_pHBINK = BinkOpen( pFullBIKFileName, BINKNOFRAMEBUFFERS | BINKSNDTRACK );
	if ( !m_pHBINK )
	{
		// The file was unable to be opened
		Assert( 0 );

		m_nBIKWidth = 64;
		m_nBIKHeight = 64;
		m_nFrameRate = 1;
		m_nFrameCount = 1;
		m_Material.Init( "debug/debugempty", TEXTURE_GROUP_OTHER );
		return false;
	}

	// Get BIK size
	m_nBIKWidth = m_pHBINK->Width;
	m_nBIKHeight = m_pHBINK->Height;
	
	// Now we can properly setup out regenerators
	m_YTextureRegenerator.SetParentMaterial( this, m_nBIKWidth, m_nBIKHeight );
#ifdef SUPPORT_BINK_ALPHA
	m_ATextureRegenerator.SetParentMaterial( this, m_nBIKWidth, m_nBIKHeight );
#endif
	m_CrTextureRegenerator.SetParentMaterial( this, m_nBIKWidth, m_nBIKHeight );
	m_CbTextureRegenerator.SetParentMaterial( this, m_nBIKWidth, m_nBIKHeight );

	m_nFrameRate = (int)( (float)m_pHBINK->FrameRate / (float)m_pHBINK->FrameRateDiv );
	m_nFrameCount = m_pHBINK->Frames;
	CreateVideoStream();
	CreateProceduralTextures( pMaterialName );
	CreateProceduralMaterial( pMaterialName );

	ConVarRef volumeConVar( "volume" );

	for( int i = 0; i != m_pHBINK->NumTracks; ++i )
	{
		BinkSetVolume( m_pHBINK, BinkGetTrackID( m_pHBINK, i ), (S32)(volumeConVar.GetFloat() * 32768.0f) );
	}

	return true;
}

void CBIKMaterial::Shutdown( void )
{
	DestroyVideoStream();
	DestroyProceduralMaterial( );
	DestroyProceduralTextures( );

	if ( m_pHBINK )
	{
		BinkClose( m_pHBINK );
		m_pHBINK = NULL;
	}
}

//-----------------------------------------------------------------------------
// Purpose: Updates our scene
// Output : Returns true on success, false on failure.
//-----------------------------------------------------------------------------
bool CBIKMaterial::Update( void )
{
	// Decompress this frame
	BinkDoFrame( m_pHBINK );

	// Regenerate our textures
	m_TextureY->Download();
#ifdef SUPPORT_BINK_ALPHA
	m_TextureA->Download();
#endif
	m_TextureCr->Download();
	m_TextureCb->Download();

	// If we're waiting, then go away
	if ( BinkWait( m_pHBINK ) )
		return true;

	// skip frames if we need to...
	while ( BinkShouldSkip( m_pHBINK ) )
	{
		BinkNextFrame( m_pHBINK );
		BinkDoFrame( m_pHBINK );
	}

	// is the video done?
	if ( m_pHBINK->FrameNum == m_pHBINK->Frames )
		return false;

	// Move on
	BinkNextFrame( m_pHBINK );
	
	return true;
}

//-----------------------------------------------------------------------------
// Returns the material
//-----------------------------------------------------------------------------
IMaterial *CBIKMaterial::GetMaterial()
{
	return m_Material;
}							   


//-----------------------------------------------------------------------------
// Returns the texcoord range
//-----------------------------------------------------------------------------
void CBIKMaterial::GetTexCoordRange( float *pMaxU, float *pMaxV )
{
	// Must have a luminosity channel
	if ( m_TextureY == NULL )
	{
		*pMaxU = *pMaxV = 1.0f;
		return;
	}

	// YA texture is always larger than the CrCb texture, so always base our size on that
	int nTextureWidth = m_TextureY->GetActualWidth();
	int nTextureHeight = m_TextureY->GetActualHeight();
	*pMaxU = (float)m_nBIKWidth / (float)nTextureWidth;
	*pMaxV = (float)m_nBIKHeight / (float)nTextureHeight;
}


//-----------------------------------------------------------------------------
// Returns the frame size of the BIK (stored in a subrect of the material itself)
//-----------------------------------------------------------------------------
void CBIKMaterial::GetFrameSize( int *pWidth, int *pHeight )
{
	*pWidth = m_nBIKWidth;
	*pHeight = m_nBIKHeight;
}

//-----------------------------------------------------------------------------
// Computes a power of two at least as big as the passed-in number
//-----------------------------------------------------------------------------
static inline int ComputeGreaterPowerOfTwo( int n )
{
	int i = 1;
	while ( i < n )
	{
		i <<= 1;
	}
	return i;
}


//-----------------------------------------------------------------------------
// Initializes, shuts down the procedural texture
//-----------------------------------------------------------------------------
void CBIKMaterial::CreateProceduralTextures( const char *pTextureName )
{
	int nWidth, nHeight;

	char textureName[MAX_PATH];
	Q_strncpy( textureName, pTextureName, MAX_PATH-1 );
	Q_StripExtension( textureName, textureName, sizeof( textureName ) );
	Q_strncat( textureName, "Y", MAX_PATH );

	unsigned int nTextureFlags = ( TEXTUREFLAGS_CLAMPS | TEXTUREFLAGS_CLAMPT | TEXTUREFLAGS_NOMIP | TEXTUREFLAGS_PROCEDURAL | TEXTUREFLAGS_SINGLECOPY | TEXTUREFLAGS_NOLOD );

	// Choose power-of-two textures which are at least as big as the BIK
	nWidth = ComputeGreaterPowerOfTwo( m_buffers.YABufferWidth ); 
	nHeight = ComputeGreaterPowerOfTwo( m_buffers.YABufferHeight ); 
	m_TextureY.InitProceduralTexture( textureName, "bik", nWidth, nHeight, IMAGE_FORMAT_I8, nTextureFlags );
	m_TextureY->SetTextureRegenerator( &m_YTextureRegenerator );

#ifdef SUPPORT_BINK_ALPHA
	Q_strncpy( textureName, pTextureName, MAX_PATH-1 );
	Q_StripExtension( textureName, textureName, sizeof( textureName ) );
	Q_strncat( textureName, "A", MAX_PATH );

	m_TextureA.InitProceduralTexture( textureName, "bik", nWidth, nHeight, IMAGE_FORMAT_I8, nTextureFlags );
	m_TextureA->SetTextureRegenerator( &m_ATextureRegenerator );
#endif

	Q_strncpy( textureName, pTextureName, MAX_PATH-1 );
	Q_StripExtension( textureName, textureName, sizeof( textureName ) );
	Q_strncat( textureName, "Cr", MAX_PATH );

	nWidth = ComputeGreaterPowerOfTwo( m_buffers.cRcBBufferWidth ); 
	nHeight = ComputeGreaterPowerOfTwo( m_buffers.cRcBBufferHeight ); 
	m_TextureCr.InitProceduralTexture( textureName, "bik", nWidth, nHeight, IMAGE_FORMAT_I8, nTextureFlags );
	m_TextureCr->SetTextureRegenerator( &m_CrTextureRegenerator );

	Q_strncpy( textureName, pTextureName, MAX_PATH-1 );
	Q_StripExtension( textureName, textureName, sizeof( textureName ) );
	Q_strncat( textureName, "Cb", MAX_PATH );

	m_TextureCb.InitProceduralTexture( textureName, "bik", nWidth, nHeight, IMAGE_FORMAT_I8, nTextureFlags );
	m_TextureCb->SetTextureRegenerator( &m_CbTextureRegenerator );
}

void CBIKMaterial::DestroyProceduralTexture( CTextureReference &texture )
{
	if( texture )
	{
		texture->SetTextureRegenerator( NULL );
		texture.Shutdown( true );
	}

}

void CBIKMaterial::DestroyProceduralTextures()
{	
	DestroyProceduralTexture( m_TextureY );
#ifdef SUPPORT_BINK_ALPHA
	DestroyProceduralTexture( m_TextureA );
#endif
	DestroyProceduralTexture( m_TextureCr );
	DestroyProceduralTexture( m_TextureCb );
}

//-----------------------------------------------------------------------------
// Initializes, shuts down the procedural material
//-----------------------------------------------------------------------------
void CBIKMaterial::CreateProceduralMaterial( const char *pMaterialName )
{
	// FIXME: gak, this is backwards.  Why doesn't the material just see that it has a funky basetexture?
	char vmtfilename[ 512 ];
	Q_strcpy( vmtfilename, pMaterialName );
	Q_SetExtension( vmtfilename, ".vmt", sizeof( vmtfilename ) );

	KeyValues *pVMTKeyValues = new KeyValues( "Bik" );
	if (!pVMTKeyValues->LoadFromFile( g_pFullFileSystem , vmtfilename, "GAME" ))
	{
		pVMTKeyValues->SetString( "$ytexture", m_TextureY->GetName() );
#ifdef SUPPORT_BINK_ALPHA
		pVMTKeyValues->SetString( "$atexture", m_TextureA->GetName() );
#endif
		pVMTKeyValues->SetString( "$crtexture", m_TextureCr->GetName() );
		pVMTKeyValues->SetString( "$cbtexture", m_TextureCb->GetName() );
		pVMTKeyValues->SetInt( "$nofog", 1 );
		pVMTKeyValues->SetInt( "$spriteorientation", 3 );
		pVMTKeyValues->SetInt( "$translucent", 1 );
		pVMTKeyValues->SetInt( "$vertexcolor", 1 );
		pVMTKeyValues->SetInt( "$vertexalpha", 1 );
		pVMTKeyValues->SetInt( "$nolod", 1 );
		pVMTKeyValues->SetInt( "$nomip", 1 );
	}

	m_Material.Init( pMaterialName, pVMTKeyValues );
	m_Material->Refresh();
}

void CBIKMaterial::DestroyProceduralMaterial()
{
	// Store the internal material pointer for later use
	IMaterial *pMaterial = m_Material;
	m_Material.Shutdown();
	materials->UncacheUnusedMaterials();

	// Now be sure to free that material because we don't want to reference it again later, we'll recreate it!
	if ( pMaterial != NULL )
	{
		pMaterial->DeleteIfUnreferenced();
	}
}


//-----------------------------------------------------------------------------
// Sets the current time
//-----------------------------------------------------------------------------
void CBIKMaterial::SetTime( float flTime )
{
	Assert( 0 );
	BinkDoFrame( m_pHBINK );
	m_TextureY->Download();
#ifdef SUPPORT_BINK_ALPHA
	m_TextureA->Download();
#endif
	m_TextureCr->Download();
	m_TextureCb->Download();
}


//-----------------------------------------------------------------------------
// Returns the frame rate of the BIK
//-----------------------------------------------------------------------------
int CBIKMaterial::GetFrameRate( )
{
	return m_nFrameRate;
}

int CBIKMaterial::GetFrameCount( )
{
	return m_nFrameCount;
}


//-----------------------------------------------------------------------------
// Sets the frame for an BIK material (use instead of SetTime)
//-----------------------------------------------------------------------------
void CBIKMaterial::SetFrame( float flFrame )
{
	U32 iFrame = (U32)flFrame + 1;

	if ( m_pHBINK->LastFrameNum != iFrame)
	{
		BinkGoto( m_pHBINK, iFrame, 0 );
		m_TextureY->Download();
#ifdef SUPPORT_BINK_ALPHA
		m_TextureA->Download();
#endif
		m_TextureCr->Download();
		m_TextureCb->Download();
	}
}

//-----------------------------------------------------------------------------
// Initializes, shuts down the video stream
//-----------------------------------------------------------------------------
void CBIKMaterial::CreateVideoStream( )
{
	// get the frame buffers info
	BinkGetFrameBuffersInfo( m_pHBINK, &m_buffers );

	// fixme: these should point to local buffers that the material system can splat
	for ( int i = 0 ; i < m_buffers.TotalFrames ; i++ )
	{  
		if ( m_buffers.Frames[ i ].YPlane.Allocate )
		{    
			// calculate a good pitch    
			m_buffers.Frames[ i ].YPlane.BufferPitch = ( m_buffers.YABufferWidth + 15 ) & ~15;
			// now allocate the pointer
			m_buffers.Frames[ i ].YPlane.Buffer = MemAlloc_AllocAligned( m_buffers.Frames[ i ].YPlane.BufferPitch * m_buffers.YABufferHeight, 16 );
		}
        if ( m_buffers.Frames[ i ].cRPlane.Allocate )
		{    
			// calculate a good pitch    
			m_buffers.Frames[ i ].cRPlane.BufferPitch = ( m_buffers.cRcBBufferWidth + 15 ) & ~15;    
			// now allocate the pointer    
			m_buffers.Frames[ i ].cRPlane.Buffer = MemAlloc_AllocAligned( m_buffers.Frames[ i ].cRPlane.BufferPitch * m_buffers.cRcBBufferHeight, 16 );  
		}

		if ( m_buffers.Frames[ i ].cBPlane.Allocate )
		{    
			// calculate a good pitch    
			m_buffers.Frames[ i ].cBPlane.BufferPitch = ( m_buffers.cRcBBufferWidth + 15 ) & ~15;    
			// now allocate the pointer    
			m_buffers.Frames[ i ].cBPlane.Buffer = MemAlloc_AllocAligned( m_buffers.Frames[ i ].cBPlane.BufferPitch * m_buffers.cRcBBufferHeight, 16 );  
		}

#ifdef SUPPORT_BINK_ALPHA
		if ( m_buffers.Frames[ i ].APlane.Allocate )  
		{    // calculate a good pitch    
			m_buffers.Frames[ i ].APlane.BufferPitch = ( m_buffers.YABufferWidth + 15 ) & ~15;    
			// now allocate the pointer   
			m_buffers.Frames[ i ].APlane.Buffer = MemAlloc_AllocAligned( m_buffers.Frames[ i ].APlane.BufferPitch * m_buffers.YABufferHeight, 16 );  
		}
#endif
	}
	// Now tell Bink to use these new planes
	BinkRegisterFrameBuffers( m_pHBINK, &m_buffers );

	// start the movie
	m_nCurrentFrame = 0;
}

void CBIKMaterial::DestroyVideoStream( )
{
	// who free's this?
	for ( int i = 0 ; i < m_buffers.TotalFrames ; i++ )
	{  
		if ( m_buffers.Frames[ i ].YPlane.Allocate && m_buffers.Frames[ i ].YPlane.Buffer )
		{    
			// now allocate the pointer
			MemAlloc_FreeAligned( m_buffers.Frames[ i ].YPlane.Buffer );
			m_buffers.Frames[ i ].YPlane.Buffer  = NULL;
		}
        if ( m_buffers.Frames[ i ].cRPlane.Allocate && m_buffers.Frames[ i ].cRPlane.Buffer )
		{    
			// now allocate the pointer    
			MemAlloc_FreeAligned( m_buffers.Frames[ i ].cRPlane.Buffer );
			m_buffers.Frames[ i ].cRPlane.Buffer = NULL;
		}

		if ( m_buffers.Frames[ i ].cBPlane.Allocate && m_buffers.Frames[ i ].cBPlane.Buffer )
		{    
			// now allocate the pointer    
			MemAlloc_FreeAligned( m_buffers.Frames[ i ].cBPlane.Buffer );
			m_buffers.Frames[ i ].cBPlane.Buffer = NULL;
		}

#ifdef SUPPORT_BINK_ALPHA
		if ( m_buffers.Frames[ i ].APlane.Allocate && m_buffers.Frames[ i ].APlane.Buffer )  
		{
			// now allocate the pointer   
			MemAlloc_FreeAligned( m_buffers.Frames[ i ].APlane.Buffer );
			m_buffers.Frames[ i ].APlane.Buffer = NULL;
		}
#endif
	}
}

//-----------------------------------------------------------------------------
//
// Implementation of IAvi
//
//-----------------------------------------------------------------------------
class CBik : public IBik
{
public:
	CBik();

	// Inherited from IAppSystem 
	virtual bool Connect( CreateInterfaceFn factory );
	virtual void Disconnect();
	virtual void *QueryInterface( const char *pInterfaceName );
	virtual InitReturnVal_t Init();
	virtual void Shutdown();

	// Inherited from IBik
	virtual BIKMaterial_t CreateMaterial( const char *pMaterialName, const char *pFileName, const char *pPathID );
	virtual void DestroyMaterial( BIKMaterial_t hMaterial );
	virtual bool Update( BIKMaterial_t hMaterial );
	virtual IMaterial* GetMaterial( BIKMaterial_t hMaterial );
	virtual void GetTexCoordRange( BIKMaterial_t hMaterial, float *pMaxU, float *pMaxV );
	virtual void GetFrameSize( BIKMaterial_t hMaterial, int *pWidth, int *pHeight );
	virtual int GetFrameRate( BIKMaterial_t hMaterial );
	virtual void SetFrame( BIKMaterial_t hMaterial, float flFrame );
	virtual int GetFrameCount( BIKMaterial_t hMaterial );
#if !defined( _X360 )
	virtual bool SetDirectSoundDevice( void *pDevice );	
#else
	virtual bool HookXAudio( void );
#endif

private:
	static void * RADLINK BinkMemAlloc( U32 bytes ) { return malloc( bytes ); };
	static void RADLINK BinkMemFree( void PTR4* ptr ) { free( ptr ); };
	// NOTE: Have to use pointers here since BIKMaterials inherit from ITextureRegenerator
	// The realloc screws up the pointers held to ITextureRegenerators in the material system.
	CUtlLinkedList< CBIKMaterial*, BIKMaterial_t > m_BIKMaterials;
};


//-----------------------------------------------------------------------------
// Singleton
//-----------------------------------------------------------------------------
static CBik g_BIK;
EXPOSE_SINGLE_INTERFACE_GLOBALVAR( CBik, IBik, BIK_INTERFACE_VERSION, g_BIK );


//-----------------------------------------------------------------------------
// Constructor/destructor
//-----------------------------------------------------------------------------
CBik::CBik()
{
}


//-----------------------------------------------------------------------------
// Connect/disconnect
//-----------------------------------------------------------------------------
bool CBik::Connect( CreateInterfaceFn factory )
{
	if ( !(  g_pFullFileSystem && materials ) )
	{
		Msg( "Bik failed to connect to a required system\n" );
	}
	return (  g_pFullFileSystem && materials );
}

//-----------------------------------------------------------------------------
// Connect/disconnect
//-----------------------------------------------------------------------------
void CBik::Disconnect( void )
{
}


//-----------------------------------------------------------------------------
// Query Interface
//-----------------------------------------------------------------------------
void *CBik::QueryInterface( const char *pInterfaceName )
{
	if (!Q_strncmp(	pInterfaceName, BIK_INTERFACE_VERSION, Q_strlen(BIK_INTERFACE_VERSION) + 1))
		return (IBik*)this;

	return NULL;
}


//-----------------------------------------------------------------------------
// Init/shutdown
//-----------------------------------------------------------------------------
InitReturnVal_t CBik::Init()
{	
	BinkSetMemory( BinkMemAlloc, BinkMemFree );

	return INIT_OK;
}

void CBik::Shutdown()
{
}


//-----------------------------------------------------------------------------
// Create/destroy an BIK material
//-----------------------------------------------------------------------------
BIKMaterial_t CBik::CreateMaterial( const char *pMaterialName, const char *pFileName, const char *pPathID )
{
	BIKMaterial_t h = m_BIKMaterials.AddToTail();
	m_BIKMaterials[h] = new CBIKMaterial;
	if ( m_BIKMaterials[h]->Init( pMaterialName, pFileName, pPathID ) == false )
	{
		delete m_BIKMaterials[h];
		m_BIKMaterials.Remove( h );
		return BIKMATERIAL_INVALID;
	}

	return h;
}

void CBik::DestroyMaterial( BIKMaterial_t h )
{
	if ( h != BIKMATERIAL_INVALID )
	{
		m_BIKMaterials[h]->Shutdown();
		delete m_BIKMaterials[h];
		m_BIKMaterials.Remove( h );
	}
}


//-----------------------------------------------------------------------------
// Purpose: 
// Input  : hMaterial - 
// Output : Returns true on success, false on failure.
//-----------------------------------------------------------------------------
bool CBik::Update( BIKMaterial_t hMaterial )
{
	if ( hMaterial == BIKMATERIAL_INVALID )
		return false;

	return m_BIKMaterials[hMaterial]->Update();
}

//-----------------------------------------------------------------------------
// Gets the IMaterial associated with an BIK material
//-----------------------------------------------------------------------------
IMaterial* CBik::GetMaterial( BIKMaterial_t h )
{
	if ( h != BIKMATERIAL_INVALID )
		return m_BIKMaterials[h]->GetMaterial();
	
	return NULL;
}


//-----------------------------------------------------------------------------
// Returns the max texture coordinate of the BIK
//-----------------------------------------------------------------------------
void CBik::GetTexCoordRange( BIKMaterial_t h, float *pMaxU, float *pMaxV )
{
	if ( h != BIKMATERIAL_INVALID )
	{
		m_BIKMaterials[h]->GetTexCoordRange( pMaxU, pMaxV );
	}
	else
	{
		*pMaxU = *pMaxV = 1.0f;
	}
}


//-----------------------------------------------------------------------------
// Returns the frame size of the BIK (is a subrect of the material itself)
//-----------------------------------------------------------------------------
void CBik::GetFrameSize( BIKMaterial_t h, int *pWidth, int *pHeight )
{
	if ( h != BIKMATERIAL_INVALID )
	{
		m_BIKMaterials[h]->GetFrameSize( pWidth, pHeight );
	}
	else
	{
		*pWidth = *pHeight = 1;
	}
}

//-----------------------------------------------------------------------------
// Returns the frame size of the BIK (is a subrect of the material itself)
//-----------------------------------------------------------------------------
int CBik::GetFrameRate( BIKMaterial_t h )
{
	if ( h == BIKMATERIAL_INVALID )
		return -1;

	return m_BIKMaterials[h]->GetFrameRate();;
}

//-----------------------------------------------------------------------------
// Returns the frame rate of the BIK
//-----------------------------------------------------------------------------
int CBik::GetFrameCount( BIKMaterial_t h )
{
	if ( h == BIKMATERIAL_INVALID )
		return -1;

	return m_BIKMaterials[h]->GetFrameCount();;
}


//-----------------------------------------------------------------------------
// Sets the frame for an BIK material (use instead of SetTime)
//-----------------------------------------------------------------------------
void CBik::SetFrame( BIKMaterial_t h, float flFrame )
{
	if ( h != BIKMATERIAL_INVALID )
	{
		m_BIKMaterials[h]->SetFrame( flFrame );
	}
}


#if !defined( _X360 )
//-----------------------------------------------------------------------------
// Purpose: 
// Input  : pDevice - 
// Output : Returns true on success, false on failure.
//-----------------------------------------------------------------------------
bool CBik::SetDirectSoundDevice( void *pDevice )
{
	return ( BinkSoundUseDirectSound( pDevice ) != 0 );
}
#else
bool CBik::HookXAudio( void )
{
	return ( BinkSoundUseXAudio() != 0 );
}
#endif

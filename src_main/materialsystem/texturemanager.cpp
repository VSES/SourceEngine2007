//===== Copyright © 1996-2005, Valve Corporation, All rights reserved. ======//
//
// Purpose: 
//
//===========================================================================//

#include <stdlib.h>
#include <malloc.h>
#include "materialsystem_global.h"
#include "string.h"
#include "shaderapi/ishaderapi.h"
#include "materialsystem/materialsystem_config.h"
#include "IHardwareConfigInternal.h"
#include "texturemanager.h"
#include "materialsystem/imaterialvar.h"
#include "materialsystem/IColorCorrection.h"
#include "tier1/strtools.h"
#include "utlvector.h"
#include "utldict.h"
#include "itextureinternal.h"
#include "vtf/vtf.h"
#include "pixelwriter.h"
#include "basetypes.h"
#include "utlbuffer.h"
#include "filesystem.h"
#include "materialsystem/imesh.h"
#include "materialsystem/ishaderapi.h"
#include "vstdlib/random.h"
#include "imorphinternal.h"
#include "tier1/utlrbtree.h"
#include "ctype.h"

// Need lightmaps access here
#define MATSYS_INTERNAL
#include "cmatlightmaps.h"
#include "cmaterialsystem.h"
#undef MATSYS_INTERNAL

#include "tier0/memdbgon.h"

#define ERROR_TEXTURE_SIZE				32
#define WHITE_TEXTURE_SIZE				1
#define BLACK_TEXTURE_SIZE				1
#define GREY_TEXTURE_SIZE				1
#define NORMALIZATION_CUBEMAP_SIZE		32

//-----------------------------------------------------------------------------
//
// Various procedural texture regeneration classes
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Creates a checkerboard texture
//-----------------------------------------------------------------------------
class CCheckerboardTexture : public ITextureRegenerator
{
public:
	CCheckerboardTexture( int nCheckerSize, color32 color1, color32 color2 ) :
		m_nCheckerSize( nCheckerSize ), m_Color1(color1), m_Color2(color2)
	{
	}

	virtual void RegenerateTextureBits( ITexture *pTexture, IVTFTexture *pVTFTexture, Rect_t *pSubRect )
	{
		for (int iFrame = 0; iFrame < pVTFTexture->FrameCount(); ++iFrame )
		{
			for (int iFace = 0; iFace < pVTFTexture->FaceCount(); ++iFace )
			{
				int nWidth = pVTFTexture->Width();
				int nHeight = pVTFTexture->Height();
				int nDepth = pVTFTexture->Depth();
				for (int z = 0; z < nDepth; ++z)
				{
					// Fill mip 0 with a checkerboard
					CPixelWriter pixelWriter;
					pixelWriter.SetPixelMemory( pVTFTexture->Format(), 
						pVTFTexture->ImageData( iFrame, iFace, 0, 0, 0, z ), pVTFTexture->RowSizeInBytes( 0 ) );
					
					for (int y = 0; y < nHeight; ++y)
					{
						pixelWriter.Seek( 0, y );
						for (int x = 0; x < nWidth; ++x)
						{
							if ( ((x & m_nCheckerSize) ^ (y & m_nCheckerSize)) ^ (z & m_nCheckerSize) )
							{
								pixelWriter.WritePixel( m_Color1.r, m_Color1.g, m_Color1.b, m_Color1.a );
							}
							else
							{
								pixelWriter.WritePixel( m_Color2.r, m_Color2.g, m_Color2.b, m_Color2.a );
							}
						}
					}
				}
			}
		}
	}

	virtual void Release()
	{
		delete this;
	}

private:
	int		m_nCheckerSize;
	color32 m_Color1;
	color32 m_Color2;
};

static void CreateCheckerboardTexture( ITextureInternal *pTexture, int nCheckerSize, color32 color1, color32 color2 )
{
	ITextureRegenerator *pRegen = new CCheckerboardTexture( nCheckerSize, color1, color2 );
	pTexture->SetTextureRegenerator( pRegen );
}


//-----------------------------------------------------------------------------
// Creates a solid texture
//-----------------------------------------------------------------------------
class CSolidTexture : public ITextureRegenerator
{
public:
	CSolidTexture( color32 color ) : m_Color(color)
	{
	}

	virtual void RegenerateTextureBits( ITexture *pTexture, IVTFTexture *pVTFTexture, Rect_t *pSubRect )
	{
		int nMipCount = pTexture->IsMipmapped() ? pVTFTexture->MipCount() : 1;
		for (int iFrame = 0; iFrame < pVTFTexture->FrameCount(); ++iFrame )
		{
			for (int iFace = 0; iFace < pVTFTexture->FaceCount(); ++iFace )
			{
				for (int iMip = 0; iMip < nMipCount; ++iMip )
				{
					int nWidth, nHeight, nDepth;
					pVTFTexture->ComputeMipLevelDimensions( iMip, &nWidth, &nHeight, &nDepth );
					for (int z = 0; z < nDepth; ++z)
					{
						CPixelWriter pixelWriter;
						pixelWriter.SetPixelMemory( pVTFTexture->Format(), 
							pVTFTexture->ImageData( iFrame, iFace, iMip, 0, 0, z ), pVTFTexture->RowSizeInBytes( iMip ) );
					
						for (int y = 0; y < nHeight; ++y)
						{
							pixelWriter.Seek( 0, y );
							for (int x = 0; x < nWidth; ++x)
							{
								pixelWriter.WritePixel( m_Color.r, m_Color.g, m_Color.b, m_Color.a );
							}
						}
					}
				}
			}
		}
	}

	virtual void Release()
	{
		delete this;
	}

private:
	color32 m_Color;
};

static void CreateSolidTexture( ITextureInternal *pTexture, color32 color )
{
	ITextureRegenerator *pRegen = new CSolidTexture( color );
	pTexture->SetTextureRegenerator( pRegen );
}

//-----------------------------------------------------------------------------
// Creates a normalization cubemap texture
//-----------------------------------------------------------------------------
class CNormalizationCubemap : public ITextureRegenerator
{
public:
	virtual void RegenerateTextureBits( ITexture *pTexture, IVTFTexture *pVTFTexture, Rect_t *pSubRect )
	{
		// Normalization cubemap doesn't make sense on low-end hardware
		// So we won't construct a spheremap out of this
		CPixelWriter pixelWriter;

		Vector direction;
		for (int iFace = 0; iFace < 6; ++iFace)
		{
			pixelWriter.SetPixelMemory( pVTFTexture->Format(), 
				pVTFTexture->ImageData( 0, iFace, 0 ), pVTFTexture->RowSizeInBytes( 0 ) );
			
			int nWidth = pVTFTexture->Width();
			int nHeight = pVTFTexture->Height();

			float flInvWidth = 2.0f / (float)(nWidth-1);
			float flInvHeight = 2.0f / (float)(nHeight-1);

			for (int y = 0; y < nHeight; ++y)
			{
				float v = y * flInvHeight - 1.0f;

				pixelWriter.Seek( 0, y );
				for (int x = 0; x < nWidth; ++x)
				{
					float u = x * flInvWidth - 1.0f;
					float oow = 1.0f / sqrt( 1.0f + u*u + v*v );

					int ix = (int)(255.0f * 0.5f * (u*oow + 1.0f) + 0.5f);
					ix = clamp( ix, 0, 255 );
					int iy = (int)(255.0f * 0.5f * (v*oow + 1.0f) + 0.5f);
					iy = clamp( iy, 0, 255 );
					int iz = (int)(255.0f * 0.5f * (oow + 1.0f) + 0.5f);
					iz = clamp( iz, 0, 255 );

					switch (iFace)
					{
					case CUBEMAP_FACE_RIGHT:
						pixelWriter.WritePixel( iz, 255 - iy, 255 - ix, 255 );
						break;
					case CUBEMAP_FACE_LEFT:
						pixelWriter.WritePixel( 255 - iz, 255 - iy, ix, 255 );
						break;
					case CUBEMAP_FACE_BACK:	
						pixelWriter.WritePixel( ix, iz, iy, 255 );
						break;
					case CUBEMAP_FACE_FRONT:
						pixelWriter.WritePixel( ix, 255 - iz, 255 - iy, 255 );
						break;
					case CUBEMAP_FACE_UP:
						pixelWriter.WritePixel( ix, 255 - iy, iz, 255 );
						break;
					case CUBEMAP_FACE_DOWN:
						pixelWriter.WritePixel( 255 - ix, 255 - iy, 255 - iz, 255 );
						break;
					default:
						break;
					}
				}
			}
		}
	}

	// NOTE: The normalization cubemap regenerator is stateless
	// so there's no need to allocate + deallocate them
	virtual void Release() {}
};

//-----------------------------------------------------------------------------
// Creates a normalization cubemap texture
//-----------------------------------------------------------------------------
class CSignedNormalizationCubemap : public ITextureRegenerator
{
public:
	virtual void RegenerateTextureBits( ITexture *pTexture, IVTFTexture *pVTFTexture, Rect_t *pSubRect )
	{
		// Normalization cubemap doesn't make sense on low-end hardware
		// So we won't construct a spheremap out of this
		CPixelWriter pixelWriter;

		Vector direction;
		for (int iFace = 0; iFace < 6; ++iFace)
		{
			pixelWriter.SetPixelMemory( pVTFTexture->Format(), 
				pVTFTexture->ImageData( 0, iFace, 0 ), pVTFTexture->RowSizeInBytes( 0 ) );
			
			int nWidth = pVTFTexture->Width();
			int nHeight = pVTFTexture->Height();

			float flInvWidth = 2.0f / (float)(nWidth-1);
			float flInvHeight = 2.0f / (float)(nHeight-1);

			for (int y = 0; y < nHeight; ++y)
			{
				float v = y * flInvHeight - 1.0f;

				pixelWriter.Seek( 0, y );
				for (int x = 0; x < nWidth; ++x)
				{
					float u = x * flInvWidth - 1.0f;
					float oow = 1.0f / sqrt( 1.0f + u*u + v*v );

					int ix = (int)(255 * 0.5 * (u*oow + 1.0f) + 0.5f);
					ix = clamp( ix, 0, 255 );
					int iy = (int)(255 * 0.5 * (v*oow + 1.0f) + 0.5f);
					iy = clamp( iy, 0, 255 );
					int iz = (int)(255 * 0.5 * (oow + 1.0f) + 0.5f);
					iz = clamp( iz, 0, 255 );

					switch (iFace)
					{
					case CUBEMAP_FACE_RIGHT:
						ix = 255 - ix;
						iy = 255 - iy;
						break;
					case CUBEMAP_FACE_LEFT:
						iy = 255 - iy;
						iz = 255 - iz;
						break;
					case CUBEMAP_FACE_BACK:	
						break;
					case CUBEMAP_FACE_FRONT:
						iy = 255 - iy;
						iz = 255 - iz;
						break;
					case CUBEMAP_FACE_UP:
						iy = 255 - iy;
						break;
					case CUBEMAP_FACE_DOWN:
						ix = 255 - ix;
						iy = 255 - iy;
						iz = 255 - iz;
						break;
					default:
						break;
					}

					ix -= 128;
					iy -= 128;
					iz -= 128;

					Assert( ix >= -128 && ix <= 127 );
					Assert( iy >= -128 && iy <= 127 );
					Assert( iz >= -128 && iz <= 127 );

					switch (iFace)
					{
					case CUBEMAP_FACE_RIGHT:
						// correct
//						pixelWriter.WritePixelSigned( -128, -128, -128, 0 );
						pixelWriter.WritePixelSigned( iz, iy, ix, 0 );
						break;
					case CUBEMAP_FACE_LEFT:
						// correct
//						pixelWriter.WritePixelSigned( -128, -128, -128, 0 );
						pixelWriter.WritePixelSigned( iz, iy, ix, 0 );
						break;
					case CUBEMAP_FACE_BACK:	
						// wrong
//						pixelWriter.WritePixelSigned( -128, -128, -128, 0 );
						pixelWriter.WritePixelSigned( ix, iz, iy, 0 );
//						pixelWriter.WritePixelSigned( -127, -127, 127, 0 );
						break;
					case CUBEMAP_FACE_FRONT:
						// wrong
//						pixelWriter.WritePixelSigned( -128, -128, -128, 0 );
						pixelWriter.WritePixelSigned( ix, iz, iy, 0 );
						break;
					case CUBEMAP_FACE_UP:
						// correct
//						pixelWriter.WritePixelSigned( -128, -128, -128, 0 );
						pixelWriter.WritePixelSigned( ix, iy, iz, 0 );
						break;
					case CUBEMAP_FACE_DOWN:
						// correct
//						pixelWriter.WritePixelSigned( -128, -128, -128, 0 );
						pixelWriter.WritePixelSigned( ix, iy, iz, 0 );
						break;
					default:
						break;
					}
				}
			}
		}
	}

	// NOTE: The normalization cubemap regenerator is stateless
	// so there's no need to allocate + deallocate them
	virtual void Release() {}
};

static void CreateNormalizationCubemap( ITextureInternal *pTexture )
{
	// NOTE: The normalization cubemap regenerator is stateless
	// so there's no need to allocate + deallocate them
	static CNormalizationCubemap s_NormalizationCubemap;
	pTexture->SetTextureRegenerator( &s_NormalizationCubemap );
}

static void CreateSignedNormalizationCubemap( ITextureInternal *pTexture )
{
	// NOTE: The normalization cubemap regenerator is stateless
	// so there's no need to allocate + deallocate them
	static CSignedNormalizationCubemap s_SignedNormalizationCubemap;
	pTexture->SetTextureRegenerator( &s_SignedNormalizationCubemap );
}

//-----------------------------------------------------------------------------
// Creates a color correction texture
//-----------------------------------------------------------------------------
class CColorCorrectionTexture : public ITextureRegenerator
{
public:
	CColorCorrectionTexture( ColorCorrectionHandle_t handle ) : m_ColorCorrectionHandle(handle)
	{
	}

	virtual void RegenerateTextureBits( ITexture *pTexture, IVTFTexture *pVTFTexture, Rect_t *pSubRect )
	{
		int nWidth = pVTFTexture->Width();
		int nHeight = pVTFTexture->Height();
		int nDepth = pVTFTexture->Depth();
		Assert( nWidth == COLOR_CORRECTION_TEXTURE_SIZE && nHeight == COLOR_CORRECTION_TEXTURE_SIZE && nDepth == COLOR_CORRECTION_TEXTURE_SIZE );

		for ( int z = 0; z < nDepth; ++z )
		{
			CPixelWriter pixelWriter;
			pixelWriter.SetPixelMemory( pVTFTexture->Format(), 
				pVTFTexture->ImageData( 0, 0, 0, 0, 0, z ), pVTFTexture->RowSizeInBytes( 0 ) );

			for ( int y = 0; y < nHeight; ++y )
			{
				pixelWriter.Seek( 0, y );
				for (int x = 0; x < nWidth; ++x)
				{
					RGBX5551_t inColor;
					inColor.r = x;
					inColor.g = y;
					inColor.b = z;

					color24 col = ColorCorrectionSystem()->GetLookup( m_ColorCorrectionHandle, inColor );
					pixelWriter.WritePixel( col.r, col.g, col.b, 255 );
				}
			}
		}
	}

	virtual void Release() 
	{
		delete this;
	}

private:
	ColorCorrectionHandle_t	m_ColorCorrectionHandle;
};


void CreateColorCorrectionTexture( ITextureInternal *pTexture, ColorCorrectionHandle_t handle )
{
	ITextureRegenerator *pRegen = new CColorCorrectionTexture( handle );
	pTexture->SetTextureRegenerator( pRegen );
}

//-----------------------------------------------------------------------------
// Implementation of the texture manager
//-----------------------------------------------------------------------------
class CTextureManager : public ITextureManager
{
public:
	CTextureManager( void );

	// Initialization + shutdown
	virtual void Init( int nFlags );
	virtual void Shutdown();

	virtual void AllocateStandardRenderTargets( );
	virtual void FreeStandardRenderTargets();

	virtual void CacheExternalStandardRenderTargets();

	virtual ITextureInternal *CreateProceduralTexture( const char *pTextureName, const char *pTextureGroupName, int w, int h, int d, ImageFormat fmt, int nFlags );
	virtual ITextureInternal *FindOrLoadTexture( const char *textureName, const char *pTextureGroupName );
 	virtual bool IsTextureLoaded( const char *pTextureName );

	virtual void AddTextureAlias( const char *pAlias, const char *pRealName );
	virtual void RemoveTextureAlias( const char *pAlias );

	virtual void SetExcludedTextures( const char *pScriptName );
	virtual void UpdateExcludedTextures();

	virtual void ResetTextureFilteringState();
	void ReloadTextures( void );

	// These are used when we lose our video memory due to a mode switch etc
	void ReleaseTextures( void );
	void RestoreNonRenderTargetTextures( void );
	void RestoreRenderTargets( void );

	// delete any texture that has a refcount <= 0
	void RemoveUnusedTextures( void );
	void DebugPrintUsedTextures( void );

	// Request a texture ID
	virtual int	RequestNextTextureID();

	// Get at a couple standard textures
	virtual ITextureInternal *ErrorTexture();
	virtual ITextureInternal *NormalizationCubemap();
	virtual ITextureInternal *SignedNormalizationCubemap();
	virtual ITextureInternal *ShadowNoise2D();
	virtual ITextureInternal *IdentityLightWarp();
	virtual ITextureInternal *ColorCorrectionTexture( int i );
	virtual ITextureInternal *FullFrameDepthTexture();

	// Generates an error texture pattern
	virtual void GenerateErrorTexture( ITexture *pTexture, IVTFTexture *pVTFTexture );

	// Updates the color correction state
	virtual void SetColorCorrectionTexture( int i, ITextureInternal *pTexture );

	virtual void ForceAllTexturesIntoHardware( void );

	virtual ITextureInternal *CreateRenderTargetTexture( 
		const char *pRTName,	// NULL for auto-generated name
		int w, 
		int h, 
		RenderTargetSizeMode_t sizeMode, 
		ImageFormat fmt, 
		RenderTargetType_t type, 
		unsigned int textureFlags, 
		unsigned int renderTargetFlags );

	virtual void RemoveTexture( ITextureInternal *pTexture );
	virtual void ReloadFilesInList( IFileList *pFilesToReload );

	// start with -1, list terminates with -1
	virtual int FindNext( int iIndex, ITextureInternal **ppTexture );

	virtual void ReleaseTempRenderTargetBits( void );

protected:
	ITextureInternal *FindTexture( const char *textureName );
	ITextureInternal *LoadTexture( const char *textureName, const char *pTextureGroupName );

	// Restores a single texture
	void RestoreTexture( ITextureInternal* pTex );

	CUtlDict< ITextureInternal *, unsigned short > m_TextureList;
	CUtlDict< const char *, unsigned short > m_TextureAliases;
	CUtlDict< int, unsigned short > m_TextureExcludes;	

	int m_iNextTexID;
	int m_nFlags;

	ITextureInternal *m_pErrorTexture;
	ITextureInternal *m_pBlackTexture;
	ITextureInternal *m_pWhiteTexture;
	ITextureInternal *m_pGreyTexture;
	ITextureInternal *m_pGreyAlphaZeroTexture;
	ITextureInternal *m_pNormalizationCubemap;
	ITextureInternal *m_pFullScreenTexture;
	ITextureInternal *m_pSignedNormalizationCubemap;
	ITextureInternal *m_pShadowNoise2D;
	ITextureInternal *m_pIdentityLightWarp;
	ITextureInternal *m_pColorCorrectionTextures[ COLOR_CORRECTION_MAX_TEXTURES ];
	ITextureInternal *m_pFullScreenDepthTexture;

	// Used to generate various error texture patterns when necessary
	CCheckerboardTexture *m_pErrorRegen;
};


//-----------------------------------------------------------------------------
// Singleton instance
//-----------------------------------------------------------------------------
static CTextureManager s_TextureManager;
ITextureManager *g_pTextureManager = &s_TextureManager;


//-----------------------------------------------------------------------------
// Texture manager
//-----------------------------------------------------------------------------
CTextureManager::CTextureManager( void ) : m_TextureList( true ), m_TextureAliases( true ), m_TextureExcludes( true )
{
	m_pErrorTexture = NULL;
	m_pBlackTexture = NULL;
	m_pWhiteTexture = NULL;
	m_pGreyTexture  = NULL;
	m_pGreyAlphaZeroTexture  = NULL;
	m_pNormalizationCubemap = NULL;
	m_pErrorRegen = NULL;
	m_pFullScreenTexture = NULL;
	m_pSignedNormalizationCubemap = NULL;
	m_pShadowNoise2D = NULL;
	m_pIdentityLightWarp = NULL;
	m_pFullScreenDepthTexture = NULL;
}


//-----------------------------------------------------------------------------
// Initialization + shutdown
//-----------------------------------------------------------------------------
void CTextureManager::Init( int nFlags )
{
	m_nFlags = nFlags;
	color32 color, color2;
	m_iNextTexID = 4096;

	// setup the checkerboard generator for failed texture loading
	color.r = color.g = color.b = 0; color.a = 128;
	color2.r = color2.b = color2.a = 255; color2.g = 0;
	m_pErrorRegen = new CCheckerboardTexture( 4, color, color2 );

	// Create an error texture
	m_pErrorTexture = CreateProceduralTexture( "error", TEXTURE_GROUP_OTHER,
		ERROR_TEXTURE_SIZE, ERROR_TEXTURE_SIZE, 1, IMAGE_FORMAT_BGRA8888, TEXTUREFLAGS_NOMIP | TEXTUREFLAGS_SINGLECOPY );
	CreateCheckerboardTexture( m_pErrorTexture, 4, color, color2 );

	// Create a white texture
	m_pWhiteTexture = CreateProceduralTexture( "white", TEXTURE_GROUP_OTHER,
		WHITE_TEXTURE_SIZE, WHITE_TEXTURE_SIZE, 1, IMAGE_FORMAT_BGRX8888, TEXTUREFLAGS_NOMIP | TEXTUREFLAGS_SINGLECOPY );
	color.r = color.g = color.b = color.a = 255;
	CreateSolidTexture( m_pWhiteTexture, color );

	// Create a black texture
	m_pBlackTexture = CreateProceduralTexture( "black", TEXTURE_GROUP_OTHER,
		BLACK_TEXTURE_SIZE, BLACK_TEXTURE_SIZE, 1, IMAGE_FORMAT_BGRX8888, TEXTUREFLAGS_NOMIP | TEXTUREFLAGS_SINGLECOPY );
	color.r = color.g = color.b = 0;
	CreateSolidTexture( m_pBlackTexture, color );

	// Create a grey texture
	m_pGreyTexture = CreateProceduralTexture( "grey", TEXTURE_GROUP_OTHER,
		GREY_TEXTURE_SIZE, GREY_TEXTURE_SIZE, 1, IMAGE_FORMAT_BGRA8888, TEXTUREFLAGS_NOMIP | TEXTUREFLAGS_SINGLECOPY );
	color.r = color.g = color.b = 128;
	color.a = 255;
	CreateSolidTexture( m_pGreyTexture, color );

	// Create a grey texture
	m_pGreyAlphaZeroTexture = CreateProceduralTexture( "greyalphazero", TEXTURE_GROUP_OTHER,
		GREY_TEXTURE_SIZE, GREY_TEXTURE_SIZE, 1, IMAGE_FORMAT_BGRA8888, TEXTUREFLAGS_NOMIP | TEXTUREFLAGS_SINGLECOPY );
	color.r = color.g = color.b = 128;
	color.a = 0;
	CreateSolidTexture( m_pGreyAlphaZeroTexture, color );

	if ( HardwareConfig()->GetMaxDXSupportLevel() >= 80 )
	{
		// Create a normalization cubemap
		m_pNormalizationCubemap = CreateProceduralTexture( "normalize", TEXTURE_GROUP_CUBE_MAP,
			NORMALIZATION_CUBEMAP_SIZE, NORMALIZATION_CUBEMAP_SIZE, 1, IMAGE_FORMAT_BGRX8888,
			TEXTUREFLAGS_ENVMAP | TEXTUREFLAGS_NOMIP | TEXTUREFLAGS_SINGLECOPY |
			TEXTUREFLAGS_CLAMPS | TEXTUREFLAGS_CLAMPT );
		CreateNormalizationCubemap( m_pNormalizationCubemap );
	}

	if ( HardwareConfig()->GetMaxDXSupportLevel() >= 90 )
	{
		// Create a normalization cubemap
		m_pSignedNormalizationCubemap = CreateProceduralTexture( "normalizesigned", TEXTURE_GROUP_CUBE_MAP,
			NORMALIZATION_CUBEMAP_SIZE, NORMALIZATION_CUBEMAP_SIZE, 1, IMAGE_FORMAT_UVWQ8888,
			TEXTUREFLAGS_ENVMAP | TEXTUREFLAGS_NOMIP | TEXTUREFLAGS_NOLOD | TEXTUREFLAGS_SINGLECOPY |
			TEXTUREFLAGS_CLAMPS | TEXTUREFLAGS_CLAMPT );
		CreateSignedNormalizationCubemap( m_pSignedNormalizationCubemap );
		m_pIdentityLightWarp = FindOrLoadTexture( "dev/IdentityLightWarp", TEXTURE_GROUP_OTHER );
		m_pIdentityLightWarp->IncrementReferenceCount();
	}

	// High end hardware needs this texture for shadow mapping
	if ( HardwareConfig()->ActuallySupportsPixelShaders_2_b() )
	{
		m_pShadowNoise2D = FindOrLoadTexture( "engine/NormalizedRandomDirections2D", TEXTURE_GROUP_OTHER );
		m_pShadowNoise2D->IncrementReferenceCount();
	}
}

void CTextureManager::Shutdown()
{
	FreeStandardRenderTargets();

	// These checks added because it's possible for shutdown to be called before the material system is 
	// fully initialized.
	if ( m_pWhiteTexture )
	{
		m_pWhiteTexture->DecrementReferenceCount();
		m_pWhiteTexture = NULL;
	}

	if ( m_pBlackTexture )
	{
		m_pBlackTexture->DecrementReferenceCount();
		m_pBlackTexture = NULL;
	}

	if ( m_pGreyTexture )
	{
		m_pGreyTexture->DecrementReferenceCount();
		m_pGreyTexture = NULL;
	}

	if ( m_pGreyAlphaZeroTexture )
	{
		m_pGreyAlphaZeroTexture->DecrementReferenceCount();
		m_pGreyAlphaZeroTexture = NULL;
	}

	if ( m_pNormalizationCubemap )
	{
		m_pNormalizationCubemap->DecrementReferenceCount();
		m_pNormalizationCubemap = NULL;
	}

	if ( m_pSignedNormalizationCubemap )
	{
		m_pSignedNormalizationCubemap->DecrementReferenceCount();
		m_pSignedNormalizationCubemap = NULL;
	}

	if ( m_pShadowNoise2D )
	{
		m_pShadowNoise2D->DecrementReferenceCount();
		m_pShadowNoise2D = NULL;
	}

	if ( m_pIdentityLightWarp )
	{
		m_pIdentityLightWarp->DecrementReferenceCount();
		m_pIdentityLightWarp = NULL;
	}

	if ( m_pErrorTexture )
	{
		m_pErrorTexture->DecrementReferenceCount();
		m_pErrorTexture = NULL;
	}

	ReleaseTextures();

	if ( m_pErrorRegen )
	{
		m_pErrorRegen->Release();
		m_pErrorRegen = NULL;
	}

	for ( int i = m_TextureList.First(); i != m_TextureList.InvalidIndex(); i = m_TextureList.Next( i ) )
	{
		ITextureInternal::Destroy( m_TextureList[i] );
	}
	m_TextureList.RemoveAll();

	for( int i = m_TextureAliases.First(); i != m_TextureAliases.InvalidIndex(); i = m_TextureAliases.Next( i ) )
	{
		delete []m_TextureAliases[i];
	}
	m_TextureAliases.RemoveAll();

	m_TextureExcludes.RemoveAll();
}


//-----------------------------------------------------------------------------
// Allocate, free standard render target textures
//-----------------------------------------------------------------------------
void CTextureManager::AllocateStandardRenderTargets( )
{
	bool bAllocateFullscreenTexture = ( m_nFlags & MATERIAL_INIT_ALLOCATE_FULLSCREEN_TEXTURE ) != 0;
	bool bAllocateMorphAccumTexture = g_pMorphMgr->ShouldAllocateScratchTextures();

	if ( IsPC() && ( bAllocateFullscreenTexture || bAllocateMorphAccumTexture ) )
	{
		MaterialSystem()->BeginRenderTargetAllocation();

		// A offscreen render target which is the size + format of the back buffer (*not* HDR format!)
		if ( bAllocateFullscreenTexture )
		{
			m_pFullScreenTexture = CreateRenderTargetTexture( "_rt_FullScreen", 1, 1, RT_SIZE_FULL_FRAME_BUFFER_ROUNDED_UP, 
				MaterialSystem()->GetBackBufferFormat(), RENDER_TARGET, TEXTUREFLAGS_CLAMPS | TEXTUREFLAGS_CLAMPT, 0 );
			m_pFullScreenTexture->IncrementReferenceCount();
		}

		// This texture is the one we accumulate morph deltas into
		if ( bAllocateMorphAccumTexture )
		{
			g_pMorphMgr->AllocateScratchTextures();
			g_pMorphMgr->AllocateMaterials();
		}

		MaterialSystem()->EndRenderTargetAllocation();
	}
}


void CTextureManager::FreeStandardRenderTargets()
{
	if ( m_pFullScreenTexture )
	{
		m_pFullScreenTexture->DecrementReferenceCount();
		m_pFullScreenTexture = NULL;
	}

	g_pMorphMgr->FreeMaterials();
	g_pMorphMgr->FreeScratchTextures();
}


void CTextureManager::CacheExternalStandardRenderTargets()
{
	m_pFullScreenDepthTexture = FindTexture( "_rt_FullFrameDepth" ); //created/destroyed in engine/matsys_interface.cpp to properly track hdr changes
}


//-----------------------------------------------------------------------------
// Generates an error texture pattern
//-----------------------------------------------------------------------------
void CTextureManager::GenerateErrorTexture( ITexture *pTexture, IVTFTexture *pVTFTexture )
{
	m_pErrorRegen->RegenerateTextureBits( pTexture, pVTFTexture, NULL );
}

//-----------------------------------------------------------------------------
// Updates the color correction state
//-----------------------------------------------------------------------------
ITextureInternal *CTextureManager::ColorCorrectionTexture( int i )
{
	Assert( i<COLOR_CORRECTION_MAX_TEXTURES );
	return m_pColorCorrectionTextures[ i ];
}

void CTextureManager::SetColorCorrectionTexture( int i, ITextureInternal *pTexture )
{
	Assert( i<COLOR_CORRECTION_MAX_TEXTURES );

	if( m_pColorCorrectionTextures[i] )
	{
		m_pColorCorrectionTextures[i]->DecrementReferenceCount();
	}

	m_pColorCorrectionTextures[i] = pTexture;
	if( pTexture )
		pTexture->IncrementReferenceCount();
}


//-----------------------------------------------------------------------------
// Releases all textures (cause we've lost video memory)
//-----------------------------------------------------------------------------
void CTextureManager::ReleaseTextures( void )
{
	g_pShaderAPI->SetFullScreenTextureHandle( INVALID_SHADERAPI_TEXTURE_HANDLE );

	for ( int i = m_TextureList.First(); i != m_TextureList.InvalidIndex(); i = m_TextureList.Next( i ) )
	{
		// Release the texture...
		m_TextureList[i]->Release();
	}
}


//-----------------------------------------------------------------------------
// Request a texture ID
//-----------------------------------------------------------------------------
int CTextureManager::RequestNextTextureID()
{
	// FIXME: Deal better with texture ids
	// The range between 19000 and 21000 are used for standard textures + lightmaps
	if (m_iNextTexID == 19000)
	{
		m_iNextTexID = 21000;
	}

	return m_iNextTexID++;
}


//-----------------------------------------------------------------------------
// Restores a single texture
//-----------------------------------------------------------------------------
void CTextureManager::RestoreTexture( ITextureInternal* pTexture )
{
	// Put the texture back onto the board
	pTexture->OnRestore();	// Give render targets a chance to reinitialize themselves if necessary (due to AA changes).
	pTexture->Download();
}


//-----------------------------------------------------------------------------
// Restore all textures (cause we've got video memory again)
//-----------------------------------------------------------------------------
void CTextureManager::RestoreNonRenderTargetTextures( )
{
	// 360 should not have gotten here
	Assert( !IsX360() );

	for ( int i = m_TextureList.First(); i != m_TextureList.InvalidIndex(); i = m_TextureList.Next( i ) )
	{
		if ( !m_TextureList[i]->IsRenderTarget() )
		{
			RestoreTexture( m_TextureList[i] );
		}
	}
}

//-----------------------------------------------------------------------------
// Restore just the render targets (cause we've got video memory again)
//-----------------------------------------------------------------------------
void CTextureManager::RestoreRenderTargets()
{
	// 360 should not have gotten here
	Assert( !IsX360() );

	for ( int i = m_TextureList.First(); i != m_TextureList.InvalidIndex(); i = m_TextureList.Next( i ) )
	{
		if ( m_TextureList[i]->IsRenderTarget() )
		{
			RestoreTexture( m_TextureList[i] );
		}
	}

	if ( m_pFullScreenTexture )
	{
		g_pShaderAPI->SetFullScreenTextureHandle( m_pFullScreenTexture->GetTextureHandle( 0 ) );
	}

	CacheExternalStandardRenderTargets();
}


//-----------------------------------------------------------------------------
// Reloads all textures
//-----------------------------------------------------------------------------
void CTextureManager::ReloadTextures()
{
	for ( int i = m_TextureList.First(); i != m_TextureList.InvalidIndex(); i = m_TextureList.Next( i ) )
	{
		// Put the texture back onto the board
		m_TextureList[i]->Download();
	}
}

static void ForceTextureIntoHardware( ITexture *pTexture, IMaterial *pMaterial, IMaterialVar *pBaseTextureVar )
{
	if ( IsX360() )
		return;

	pBaseTextureVar->SetTextureValue( pTexture );

	CMatRenderContextPtr pRenderContext( MaterialSystem()->GetRenderContext() );
	pRenderContext->Bind( pMaterial );
	IMesh* pMesh = pRenderContext->GetDynamicMesh( true );

	CMeshBuilder meshBuilder;
	meshBuilder.Begin( pMesh, MATERIAL_TRIANGLES, 1 );

	meshBuilder.Position3f( 0.0f, 0.0f, 0.0f );
	meshBuilder.TangentS3f( 0.0f, 1.0f, 0.0f );
	meshBuilder.TangentT3f( 1.0f, 0.0f, 0.0f );
	meshBuilder.Normal3f( 0.0f, 0.0f, 1.0f );
	meshBuilder.TexCoord2f( 0, 0.0f, 0.0f );
	meshBuilder.AdvanceVertex();

	meshBuilder.Position3f( 0.0f, 0.0f, 0.0f );
	meshBuilder.TangentS3f( 0.0f, 1.0f, 0.0f );
	meshBuilder.TangentT3f( 1.0f, 0.0f, 0.0f );
	meshBuilder.Normal3f( 0.0f, 0.0f, 1.0f );
	meshBuilder.TexCoord2f( 0, 0.0f, 0.0f );
	meshBuilder.AdvanceVertex();

	meshBuilder.Position3f( 0.0f, 0.0f, 0.0f );
	meshBuilder.TangentS3f( 0.0f, 1.0f, 0.0f );
	meshBuilder.TangentT3f( 1.0f, 0.0f, 0.0f );
	meshBuilder.Normal3f( 0.0f, 0.0f, 1.0f );
	meshBuilder.TexCoord2f( 0, 0.0f, 0.0f );
	meshBuilder.AdvanceVertex();

	meshBuilder.End();
	pMesh->Draw();
}

//-----------------------------------------------------------------------------
// Reloads all textures
//-----------------------------------------------------------------------------
void CTextureManager::ForceAllTexturesIntoHardware( void )
{
	if ( IsX360() )
		return;

	IMaterial *pMaterial = MaterialSystem()->FindMaterial( "engine/preloadtexture", "texture preload" );
	pMaterial = ((IMaterialInternal *)pMaterial)->GetRealTimeVersion(); //always work with the realtime material internally
	bool bFound;
	IMaterialVar *pBaseTextureVar = pMaterial->FindVar( "$basetexture", &bFound );
	if( !bFound )
	{
		return;
	}

	for ( int i = m_TextureList.First(); i != m_TextureList.InvalidIndex(); i = m_TextureList.Next( i ) )
	{
		// Put the texture back onto the board
		ForceTextureIntoHardware( m_TextureList[i], pMaterial, pBaseTextureVar );
	}
}

//-----------------------------------------------------------------------------
// Get at a couple standard textures
//-----------------------------------------------------------------------------
ITextureInternal *CTextureManager::ErrorTexture()
{
	return m_pErrorTexture;
}

ITextureInternal *CTextureManager::NormalizationCubemap()
{
	return m_pNormalizationCubemap; 
}

ITextureInternal *CTextureManager::SignedNormalizationCubemap()
{
	return m_pSignedNormalizationCubemap; 
}

ITextureInternal *CTextureManager::ShadowNoise2D()
{
	return m_pShadowNoise2D; 
}

ITextureInternal *CTextureManager::IdentityLightWarp()
{
	return m_pIdentityLightWarp; 
}

ITextureInternal *CTextureManager::FullFrameDepthTexture()
{
	return m_pFullScreenDepthTexture;
}


//-----------------------------------------------------------------------------
// Creates a procedural texture
//-----------------------------------------------------------------------------
ITextureInternal *CTextureManager::CreateProceduralTexture( 
	const char			*pTextureName, 
	const char			*pTextureGroupName, 
	int					w, 
	int					h, 
	int					d, 
	ImageFormat			fmt, 
	int					nFlags )
{
	ITextureInternal *pNewTexture = ITextureInternal::CreateProceduralTexture( pTextureName, pTextureGroupName, w, h, d, fmt, nFlags );
	if ( !pNewTexture )
		return NULL;

	// Add it to the list of textures so it can be restored, etc.
	m_TextureList.Insert( pNewTexture->GetName(), pNewTexture );

	// NOTE: This will download the texture only if the shader api is ready
	pNewTexture->Download();

	return pNewTexture;
}

//-----------------------------------------------------------------------------
// FIXME: Need some better understanding of when textures should be added to
// the texture dictionary here. Is it only for files, for example?
// Texture dictionary...
//-----------------------------------------------------------------------------
ITextureInternal *CTextureManager::LoadTexture( const char *pTextureName, const char *pTextureGroupName )
{
	ITextureInternal *pNewTexture = ITextureInternal::CreateFileTexture( pTextureName, pTextureGroupName );
	if ( pNewTexture )
	{
		int iIndex = m_TextureExcludes.Find( pNewTexture->GetName() );
		if ( m_TextureExcludes.IsValidIndex( iIndex ) )
		{
			// mark the new texture as excluded
			int nDimensionsLimit = m_TextureExcludes[iIndex];
			pNewTexture->MarkAsExcluded( ( nDimensionsLimit == 0 ), nDimensionsLimit );
		}

		// Stick the texture onto the board
		pNewTexture->Download();

		// FIXME: If there's been an error loading, we don't also want this error...
	}

	return pNewTexture;
}

ITextureInternal *CTextureManager::FindTexture( const char *pTextureName )
{
	if ( !pTextureName || pTextureName[0] == 0 )
		return NULL;
	
	char szCleanName[MAX_PATH];
	NormalizeTextureName( pTextureName, szCleanName, sizeof( szCleanName ) );

	int i = m_TextureList.Find( szCleanName );
	if ( i != m_TextureList.InvalidIndex() )
	{
		return m_TextureList[i];
	}

	i = m_TextureAliases.Find( szCleanName );
	if ( i != m_TextureAliases.InvalidIndex() )
	{
		return FindTexture( m_TextureAliases[i] );
	}

	// Special handling: lightmaps
	if ( char const *szLightMapNum = StringAfterPrefix( szCleanName, "[lightmap" ) )
	{
		int iLightMapNum = atoi( szLightMapNum );
		extern CMaterialSystem g_MaterialSystem;
		CMatLightmaps *plm = g_MaterialSystem.GetLightmaps();
		if ( iLightMapNum >= 0 &&
			 iLightMapNum < plm->GetNumLightmapPages() )
		{
			ShaderAPITextureHandle_t hTex = plm->GetLightmapPageTextureHandle( iLightMapNum );
			if ( hTex != INVALID_SHADERAPI_TEXTURE_HANDLE )
			{
				// Establish the lookup linking in the dictionary
				ITextureInternal *pTxInt = ITextureInternal::CreateReferenceTextureFromHandle( pTextureName, TEXTURE_GROUP_LIGHTMAP, hTex );
				m_TextureList.Insert( pTextureName, pTxInt );
				return pTxInt;
			}
		}
	}

	return NULL;
}

void CTextureManager::AddTextureAlias( const char *pAlias, const char *pRealName )
{
	if	( (pAlias == NULL) || (pRealName == NULL) )
		return; //invalid alias

	char szCleanName[MAX_PATH];
	int index = m_TextureAliases.Find( NormalizeTextureName( pAlias, szCleanName, sizeof( szCleanName ) ) );

	if	( index != m_TextureAliases.InvalidIndex() )
	{
		AssertMsg( Q_stricmp( pRealName, m_TextureAliases[index] ) == 0, "Trying to use one name to alias two different textures." );
		RemoveTextureAlias( pAlias ); //remove the old alias to make room for the new one.
	}

	size_t iRealNameLength = strlen( pRealName ) + 1;
	char *pRealNameCopy = new char [iRealNameLength];
	memcpy( pRealNameCopy, pRealName, iRealNameLength );

	m_TextureAliases.Insert( szCleanName, pRealNameCopy );
}

void CTextureManager::RemoveTextureAlias( const char *pAlias )
{
	if ( pAlias == NULL )
		return;

	char szCleanName[MAX_PATH];
	int index = m_TextureAliases.Find( NormalizeTextureName( pAlias, szCleanName, sizeof( szCleanName ) ) );
	if ( index == m_TextureAliases.InvalidIndex() )
		return; //not found

	delete []m_TextureAliases[index];
	m_TextureAliases.RemoveAt( index );
}

void CTextureManager::SetExcludedTextures( const char *pScriptName )
{
	// clear all exisiting texture's exclusion
	for ( int i = m_TextureExcludes.First(); i != m_TextureExcludes.InvalidIndex(); i = m_TextureExcludes.Next( i ) )
	{
		ITextureInternal *pTexture = FindTexture( m_TextureExcludes.GetElementName( i ) );
		if ( pTexture )
		{
			pTexture->MarkAsExcluded( false, 0 );
		}
	}
	m_TextureExcludes.RemoveAll();

	MEM_ALLOC_CREDIT();

	// get optional script
	CUtlBuffer excludeBuffer( 0, 0, CUtlBuffer::TEXT_BUFFER );
	if ( g_pFullFileSystem->ReadFile( pScriptName, NULL, excludeBuffer ) )
	{
		char szToken[MAX_PATH];
		while ( 1 )
		{
			// must support spaces in names without quotes
			// have to brute force parse up to a valid line
			while ( 1 )
			{
				excludeBuffer.EatWhiteSpace();
				if ( !excludeBuffer.EatCPPComment() )
				{
					// not a comment
					break;
				}
			}
			excludeBuffer.GetLine( szToken, sizeof( szToken ) );
			int tokenLength = strlen( szToken );
			if ( !tokenLength )
			{
				// end of list
				break;
			}

			// remove all trailing whitespace
			while ( tokenLength > 0 )
			{
				tokenLength--;
				if ( isgraph( szToken[tokenLength] ) )
				{
					break;
				}
				szToken[tokenLength] = '\0';
			}

			// first optional token may be a dimension limit hint
			int nDimensionsLimit = 0;
			char *pTextureName = szToken;
			if ( pTextureName[0] != 0 && isdigit( pTextureName[0] ) )
			{
				nDimensionsLimit = atoi( pTextureName );
				
				// skip forward to name
				for ( ;; )
				{
					char ch = *pTextureName;
					if ( !ch || ( !isdigit( ch ) && !isspace( ch ) ) )
					{
						break;
					}
					pTextureName++;
				}
			}

			char szCleanName[MAX_PATH];
			NormalizeTextureName( pTextureName, szCleanName, sizeof( szCleanName ) );

			if ( m_TextureExcludes.Find( szCleanName ) != m_TextureExcludes.InvalidIndex() )
			{
				// avoid duplicates
				continue;
			}

			m_TextureExcludes.Insert( szCleanName, nDimensionsLimit );

			// set any existing texture's exclusion
			// textures that don't exist yet will get caught during their creation path
			ITextureInternal *pTexture = FindTexture( szCleanName );
			if ( pTexture )
			{
				pTexture->MarkAsExcluded( ( nDimensionsLimit == 0 ), nDimensionsLimit );
			}
		}
	}
}

void CTextureManager::UpdateExcludedTextures( void )
{
	for ( int i = m_TextureList.First(); i != m_TextureList.InvalidIndex(); i = m_TextureList.Next( i ) )
	{
		m_TextureList[i]->UpdateExcludedState();
	}
}

ITextureInternal *CTextureManager::FindOrLoadTexture( const char *pTextureName, const char *pTextureGroupName )
{
	ITextureInternal *pTexture = FindTexture( pTextureName );
	if ( !pTexture )
	{
		pTexture = LoadTexture( pTextureName, pTextureGroupName );
		if ( pTexture )
		{
			// insert into the dictionary using the processed texture name
			m_TextureList.Insert( pTexture->GetName(), pTexture );
		}
	}

	return pTexture;
}

bool CTextureManager::IsTextureLoaded( const char *pTextureName )
{
	ITextureInternal *pTexture = FindTexture( pTextureName );
	return ( pTexture != NULL );
}


//-----------------------------------------------------------------------------
// Creates a texture that's a render target
//-----------------------------------------------------------------------------
ITextureInternal *CTextureManager::CreateRenderTargetTexture( 
	const char *pRTName,	// NULL for auto-generated name
	int w, 
	int h, 
	RenderTargetSizeMode_t sizeMode, 
	ImageFormat fmt, 
	RenderTargetType_t type, 
	unsigned int textureFlags, 
	unsigned int renderTargetFlags )
{
	MEM_ALLOC_CREDIT_( __FILE__ ": Render target" );

	ITextureInternal *pTexture;
	if ( pRTName )
	{
		// caller is re-initing or changing
		pTexture = FindTexture( pRTName );
		if ( pTexture )
		{
			// Changing the underlying render target, but leaving the pointer and refcount
			// alone fixes callers that have exisiting references to this object.
			ITextureInternal::ChangeRenderTarget( pTexture, w, h, sizeMode, fmt, type, 
					textureFlags, renderTargetFlags );

			// download if ready
			pTexture->Download();
			return pTexture;
		}
	}
 
	pTexture = ITextureInternal::CreateRenderTarget( pRTName, w, h, sizeMode, fmt, type, 
											  textureFlags, renderTargetFlags );
	if ( !pTexture )
		return NULL;

	// Add the render target to the list of textures
	// that way it'll get cleaned up correctly in case of a task switch
	m_TextureList.Insert( pTexture->GetName(), pTexture );

	// NOTE: This will download the texture only if the shader api is ready
	pTexture->Download();

	return pTexture;
}

void CTextureManager::ResetTextureFilteringState( )
{
	for ( int i = m_TextureList.First(); i != m_TextureList.InvalidIndex(); i = m_TextureList.Next( i ) )
	{
		m_TextureList[i]->SetFilteringAndClampingMode();
	}
}


void CTextureManager::RemoveUnusedTextures( void )
{
	int iNext;
	for ( int i = m_TextureList.First(); i != m_TextureList.InvalidIndex(); i = iNext )
	{
		iNext = m_TextureList.Next( i );

#ifdef _DEBUG
		if ( m_TextureList[i]->GetReferenceCount() < 0 )
		{
			Warning( "RemoveUnusedTextures: pTexture->m_referenceCount < 0 for %s\n", m_TextureList[i]->GetName() );
		}
#endif
		if ( m_TextureList[i]->GetReferenceCount() <= 0 )
		{
			ITextureInternal::Destroy( m_TextureList[i] );
			m_TextureList.RemoveAt( i );
		}
	}
}

void CTextureManager::RemoveTexture( ITextureInternal *pTexture )
{
	Assert( pTexture->GetReferenceCount() <= 0 );

	for ( int i = m_TextureList.First(); i != m_TextureList.InvalidIndex(); i = m_TextureList.Next( i ) )
	{
		// search by object
		if ( m_TextureList[i] == pTexture )
		{
			ITextureInternal::Destroy( m_TextureList[i] );
			m_TextureList.RemoveAt( i );
			break;
		}
	}
}

void CTextureManager::ReloadFilesInList( IFileList *pFilesToReload )
{
	if ( !IsPC() )
		return;

	for ( int i=m_TextureList.First(); i != m_TextureList.InvalidIndex(); i=m_TextureList.Next( i ) )
	{
		ITextureInternal *pTex = m_TextureList[i];

		pTex->ReloadFilesInList( pFilesToReload );
	}
}

void CTextureManager::ReleaseTempRenderTargetBits( void )
{
	if( IsX360() ) //only sane on 360
	{
		int iNext;
		for ( int i = m_TextureList.First(); i != m_TextureList.InvalidIndex(); i = iNext )
		{
			iNext = m_TextureList.Next( i );

			if ( m_TextureList[i]->IsTempRenderTarget() )
			{
				m_TextureList[i]->Release();
			}
		}
	}
}

void CTextureManager::DebugPrintUsedTextures( void )
{
	for ( int i = m_TextureList.First(); i != m_TextureList.InvalidIndex(); i = m_TextureList.Next( i ) )
	{
		ITextureInternal *pTexture = m_TextureList[i];
		Msg( "Texture: '%s' RefCount: %d\n", pTexture->GetName(), pTexture->GetReferenceCount() );
	}

	if ( m_TextureExcludes.Count() )
	{
		Msg( "\nExcluded Textures: (%d)\n", m_TextureExcludes.Count() );
		for ( int i = m_TextureExcludes.First(); i != m_TextureExcludes.InvalidIndex(); i = m_TextureExcludes.Next( i ) )
		{
			char buff[256];
			const char *pName = m_TextureExcludes.GetElementName( i );
			V_snprintf( buff, sizeof( buff ), "Excluded: %d '%s' \n", m_TextureExcludes[i], pName );
	
			// an excluded texture is valid, but forced tiny
			if ( IsTextureLoaded( pName ) )
			{
				Msg( buff );
			}
			else
			{
				// warn as unknown, could be a spelling error
				Warning( buff );
			}
		}
	}
}

int CTextureManager::FindNext( int iIndex, ITextureInternal **pTexInternal )
{
	if ( iIndex == -1 && m_TextureList.Count() )
	{
		iIndex = m_TextureList.First();
	}
	else if ( !m_TextureList.Count() || !m_TextureList.IsValidIndex( iIndex ) )
	{
		*pTexInternal = NULL;
		return -1;
	}

	*pTexInternal = m_TextureList[iIndex];

	iIndex = m_TextureList.Next( iIndex );
	if ( iIndex == m_TextureList.InvalidIndex() )
	{
		// end of list
		iIndex = -1;
	}

	return iIndex;
}

//========== Copyright © 2005, Valve Corporation, All rights reserved. ========
//
// Purpose:
//
//=============================================================================

#ifndef CMATERIALSYSTEM_H
#define CMATERIALSYSTEM_H

#include "tier1/delegates.h"

#include "materialsystem_global.h"
#include "materialsystem/imaterialsystem.h"
#include "materialsystem/ishaderapi.h"
#include "imaterialinternal.h"
#include "imaterialsysteminternal.h"
#include "shaderapi/ishaderutil.h"
#include "materialsystem/deformations.h"

#include "cmaterialdict.h"
#include "cmatlightmaps.h"
#include "cmatrendercontext.h"
#include "cmatqueuedrendercontext.h"
#include "materialsystem_global.h"

#ifndef MATSYS_INTERNAL
#error "This file is private to the implementation of IMaterialSystem/IMaterialSystemInternal"
#endif

#if defined( _WIN32 )
#pragma once
#endif

//-----------------------------------------------------------------------------

class CJob;
struct DeferredUpdateLightmapInfo_t;

// typedefs to allow use of delegation macros
typedef int LightmapOffset_t[2];

//-----------------------------------------------------------------------------

extern CThreadFastMutex g_MatSysMutex;

//-----------------------------------------------------------------------------
// The material system implementation
//-----------------------------------------------------------------------------


class CMaterialSystem : public CTier2AppSystem< IMaterialSystemInternal >, public IShaderUtil
{
	typedef CTier2AppSystem< IMaterialSystemInternal > BaseClass;
public:

	CMaterialSystem();
	~CMaterialSystem();

	//---------------------------------------------------------
	// Initialization and shutdown
	//---------------------------------------------------------

	//
	// IAppSystem
	//
	virtual bool							Connect( CreateInterfaceFn factory );
	virtual void							Disconnect();
	virtual void *							QueryInterface( const char *pInterfaceName );
	virtual InitReturnVal_t					Init();
	virtual void							Shutdown();

	CreateInterfaceFn						Init( const char* pShaderDLL,
													IMaterialProxyFactory* pMaterialProxyFactory,
													CreateInterfaceFn fileSystemFactory,
													CreateInterfaceFn cvarFactory );

	// Call this to set an explicit shader version to use 
	// Must be called before Init().
	void									SetShaderAPI( const char *pShaderAPIDLL );

	// Must be called before Init(), if you're going to call it at all...
	void									SetAdapter( int nAdapter, int nFlags );

	void									ModInit();
	void									ModShutdown();

private:
	// Used to dynamically load and unload the shader api
	CreateInterfaceFn						CreateShaderAPI( const char* pShaderDLL );
	void									DestroyShaderAPI();
	
	// Method to get at interfaces supported by the SHADDERAPI
	void *									QueryShaderAPI( const char *pInterfaceName );

	// Initializes the color correction terms
	void									InitColorCorrection();


public:
	virtual void							SetThreadMode( MaterialThreadMode_t nextThreadMode, int nServiceThread );
	virtual MaterialThreadMode_t			GetThreadMode();
	virtual void							ExecuteQueued();

	//---------------------------------------------------------
	// Component accessors
	//---------------------------------------------------------
	const CMatLightmaps *					GetLightmaps() const		{ return &m_Lightmaps; }
	CMatLightmaps *							GetLightmaps()				{ return &m_Lightmaps; }

	IMatRenderContext *						GetRenderContext();
	IMatRenderContext *						CreateRenderContext( MaterialContextType_t type );
	IMatRenderContext *						SetRenderContext( IMatRenderContext * );

	const IMatRenderContextInternal *		GetRenderContextInternal() const	{ IMatRenderContextInternal *pRenderContext = m_pRenderContext.Get(); return ( pRenderContext ) ? pRenderContext : &m_HardwareRenderContext; }
	IMatRenderContextInternal *				GetRenderContextInternal()			{ IMatRenderContextInternal *pRenderContext = m_pRenderContext.Get(); return ( pRenderContext ) ? pRenderContext : &m_HardwareRenderContext; }

	const CMaterialDict *					GetMaterialDict() const		{ return &m_MaterialDict; }
	CMaterialDict *							GetMaterialDict()			{ return &m_MaterialDict; }

	virtual void							ReloadFilesInList( IFileList *pFilesToReload );

public:
	//---------------------------------------------------------
	// Config management
	//---------------------------------------------------------

	// IMaterialSystem
	virtual IMaterialSystemHardwareConfig *	GetHardwareConfig( const char *pVersion, int *returnCode );

	// Get the current config for this video card (as last set by control panel or the default if not)
	virtual bool							UpdateConfig( bool forceUpdate );
	virtual bool							OverrideConfig( const MaterialSystem_Config_t &config, bool bForceUpdate );
	const MaterialSystem_Config_t &			GetCurrentConfigForVideoCard() const;

	// Gets *recommended* configuration information associated with the display card, 
	virtual bool							GetRecommendedConfigurationInfo( int nDXLevel, KeyValues * pKeyValues );

	// IShaderUtil
	MaterialSystem_Config_t &				GetConfig();

private:
	//---------------------------------

	// This is called when the config changes
	void									GenerateConfigFromConfigKeyValues( MaterialSystem_Config_t *pConfig, bool bOverwriteCommandLineValues );

	// Read/write config into convars
	void									ReadConfigFromConVars( MaterialSystem_Config_t *pConfig );
	void									WriteConfigIntoConVars( const MaterialSystem_Config_t &config );

	// Write dxsupport info to configvars 
	void									WriteConfigurationInfoToConVars( bool bOverwriteCommandLineValues = true );


public:
	// -----------------------------------------------------------
	// Device methods
	// -----------------------------------------------------------
	int										GetDisplayAdapterCount() const;
	int										GetCurrentAdapter() const;
	void									GetDisplayAdapterInfo( int adapter, MaterialAdapterInfo_t& info ) const;
	int										GetModeCount( int adapter ) const;
	void									GetModeInfo( int adapter, int mode, MaterialVideoMode_t& info ) const;
	void									AddModeChangeCallBack( ModeChangeCallbackFunc_t func );
	void									RemoveModeChangeCallBack( ModeChangeCallbackFunc_t func );

	// Returns the mode info for the current display device
	void									GetDisplayMode( MaterialVideoMode_t& info ) const;
	bool									SetMode( void* hwnd, const MaterialSystem_Config_t &config );

	// Reports support for a given MSAA mode
	bool									SupportsMSAAMode( int nMSAAMode );

	// Reports support for a given CSAA mode
	bool									SupportsCSAAMode( int nNumSamples, int nQualityLevel );

	bool				                    SupportsHDRMode( HDRType_t nHDRModede );

	bool									UsesSRGBCorrectBlending() const;

	// Reports support for shadow depth texturing
	bool									SupportsShadowDepthTextures( void );

	// Reports support for fetch4 texture access (useful for shadow map PCF in shader)
	bool									SupportsFetch4( void );

	// Vendor-dependent shadow depth texture format
	ImageFormat								GetShadowDepthTextureFormat( void );

	// Vendor-dependent slim texture format
	ImageFormat								GetNullTextureFormat( void );

	// Shadow depth bias factors
	void									SetShadowDepthBiasFactors( float fShadowSlopeScaleDepthBias, float fShadowDepthBias );

	const MaterialSystemHardwareIdentifier_t &GetVideoCardIdentifier() const;

	// Use this to spew information about the 3D layer 
	void									SpewDriverInfo() const;

	DELEGATE_TO_OBJECT_2V(					GetDXLevelDefaults, uint &, uint &, g_pShaderAPI );

	DELEGATE_TO_OBJECT_2VC(					GetBackBufferDimensions, int &, int &, g_pShaderDevice );
	DELEGATE_TO_OBJECT_0C( ImageFormat,		GetBackBufferFormat, g_pShaderDevice );

	DELEGATE_TO_OBJECT_1V(                  PushDeformation, DeformationBase_t const *, g_pShaderAPI );

	DELEGATE_TO_OBJECT_0V(                  PopDeformation, g_pShaderAPI );

	DELEGATE_TO_OBJECT_0C(int,				GetNumActiveDeformations, g_pShaderAPI );

	// -----------------------------------------------------------
	// Window methods
	// -----------------------------------------------------------
	// Creates/ destroys a child window
	bool									AddView( void* hwnd );
	void									RemoveView( void* hwnd );

	// Sets the view
	void									SetView( void* hwnd );

	// -----------------------------------------------------------
	// Control flow
	// -----------------------------------------------------------
	void									BeginFrame( float frameTime );
	void									EndFrame();
	virtual bool							IsInFrame( ) const;
	void									Flush( bool flushHardware = false );
	void									SwapBuffers();

	// Flushes managed textures from the texture cacher
	void									EvictManagedResources();

	void									ReleaseResources();
	void									ReacquireResources();

	// Recomputes a state snapshot
	void									RecomputeAllStateSnapshots();

	void									NoteAnisotropicLevel( int currentLevel );

	// -----------------------------------------------------------
	// Device loss/restore
	// -----------------------------------------------------------
	// Installs a function to be called when we need to release vertex buffers
	void									AddReleaseFunc( MaterialBufferReleaseFunc_t func );
	void									RemoveReleaseFunc( MaterialBufferReleaseFunc_t func );

	// Installs a function to be called when we need to restore vertex buffers
	void									AddRestoreFunc( MaterialBufferRestoreFunc_t func );
	void									RemoveRestoreFunc( MaterialBufferRestoreFunc_t func );

	// Called by the shader API when it's just about to lose video memory
	void									ReleaseShaderObjects();
	void									RestoreShaderObjects( CreateInterfaceFn shaderFactory, int nChangeFlags = 0 );

	// Release temporary HW memory...
	void									ResetTempHWMemory( bool bExitingLevel = false );

	// For dealing with device lost in cases where SwapBuffers isn't called all the time (Hammer)
	void									HandleDeviceLost();


	// -----------------------------------------------------------
	// Shaders
	// -----------------------------------------------------------
	int										ShaderCount() const;
	int										GetShaders( int nFirstShader, int nCount, IShader **ppShaderList ) const;
	int										ShaderFlagCount() const;
	const char *							ShaderFlagName( int nIndex ) const;
	void									GetShaderFallback( const char *pShaderName, char *pFallbackShader, int nFallbackLength );


	// -----------------------------------------------------------
	// Material proxies
	// -----------------------------------------------------------
	IMaterialProxyFactory*					GetMaterialProxyFactory();
	void									SetMaterialProxyFactory( IMaterialProxyFactory* pFactory );


	// -----------------------------------------------------------
	// Editor mode
	// -----------------------------------------------------------
	bool									InEditorMode() const;

	// Used to enable editor materials. Must be called before Init.
	void									EnableEditorMaterials();

	// Can we use editor materials?
	bool									CanUseEditorMaterials() const;


	// -----------------------------------------------------------
	// Stub mode mode
	// -----------------------------------------------------------
	void									SetInStubMode( bool bInStubMode );
	bool									IsInStubMode();


	//---------------------------------------------------------
	// Image formats
	//---------------------------------------------------------
	ImageFormatInfo_t const&				ImageFormatInfo( ImageFormat fmt) const;
	
	int										GetMemRequired( int width, int height, int depth, ImageFormat format, bool mipmap );

	bool									ConvertImageFormat( unsigned char *src, enum ImageFormat srcImageFormat,
																unsigned char *dst, enum ImageFormat dstImageFormat, 
																int width, int height, int srcStride = 0, int dstStride = 0 );


	//---------------------------------------------------------
	// Debug support
	//---------------------------------------------------------
	void									CreateDebugMaterials();
	void									CleanUpDebugMaterials();

	void									DebugPrintUsedMaterials( const char *pSearchSubString, bool bVerbose );
	void									DebugPrintUsedTextures();

	void									ToggleSuppressMaterial( const char* pMaterialName );
	void									ToggleDebugMaterial( const char* pMaterialName );


	//---------------------------------------------------------
	// Misc features
	//---------------------------------------------------------

	//returns whether fast clipping is being used or not - needed to be exposed for better per-object clip behavior
	bool									UsingFastClipping();

	int										StencilBufferBits();


	//---------------------------------------------------------
	// Standard material and textures
	//---------------------------------------------------------
	void									AllocateStandardTextures();
	void									ReleaseStandardTextures();

	IMaterialInternal *						GetDrawFlatMaterial()									{ return m_pDrawFlatMaterial; }
	IMaterialInternal *						GetBufferClearObeyStencil( int i )						{ return m_pBufferClearObeyStencil[i]; }
	IMaterialInternal *						GetRenderTargetBlitMaterial()							{ return m_pRenderTargetBlitMaterial; }

	ShaderAPITextureHandle_t				GetFullbrightLightmapTextureHandle() const				{ return m_FullbrightLightmapTextureHandle; }
	ShaderAPITextureHandle_t				GetFullbrightBumpedLightmapTextureHandle() const		{ return m_FullbrightBumpedLightmapTextureHandle; }
	ShaderAPITextureHandle_t				GetBlackTextureHandle() const							{ return m_BlackTextureHandle; }
	ShaderAPITextureHandle_t				GetFlatNormalTextureHandle() const						{ return m_FlatNormalTextureHandle; }
	ShaderAPITextureHandle_t				GetGreyTextureHandle() const							{ return m_GreyTextureHandle; }
	ShaderAPITextureHandle_t				GetGreyAlphaZeroTextureHandle() const					{ return m_GreyAlphaZeroTextureHandle; }
	ShaderAPITextureHandle_t				GetWhiteTextureHandle() const							{ return m_WhiteTextureHandle; }
	ShaderAPITextureHandle_t				GetLinearToGammaTableTextureHandle() const				{ return m_LinearToGammaTableTextureHandle; }
	ShaderAPITextureHandle_t				GetLinearToGammaIdentityTableTextureHandle() const		{ return m_LinearToGammaIdentityTableTextureHandle; }
	ShaderAPITextureHandle_t				GetMaxDepthTextureHandle() const						{ return m_MaxDepthTextureHandle; }

	//---------------------------------------------------------
	// Material and texture management
	//---------------------------------------------------------
	void									UncacheAllMaterials();
	void									UncacheUnusedMaterials( bool bRecomputeStateSnapshots );
	void									CacheUsedMaterials();
	void									ReloadTextures();
	void									ReloadMaterials( const char *pSubString = NULL );

	// Create new materials	(currently only used by the editor!)
	IMaterial *								CreateMaterial( const char *pMaterialName, KeyValues *pVMTKeyValues );
	IMaterial *								FindMaterial( const char *materialName, const char *pTextureGroupName, bool complain = true, const char *pComplainPrefix = NULL );
	virtual IMaterial *						FindProceduralMaterial( const char *pMaterialName, const char *pTextureGroupName, KeyValues *pVMTKeyValues );
	const char *							GetForcedTextureLoadPathID() { return m_pForcedTextureLoadPathID; }

	//---------------------------------

	DELEGATE_TO_OBJECT_0C( MaterialHandle_t, FirstMaterial, &m_MaterialDict );
	DELEGATE_TO_OBJECT_1C( MaterialHandle_t, NextMaterial, MaterialHandle_t, &m_MaterialDict );
	DELEGATE_TO_OBJECT_0C( MaterialHandle_t, InvalidMaterial, &m_MaterialDict );
	DELEGATE_TO_OBJECT_1C( IMaterial *,		GetMaterial, MaterialHandle_t, &m_MaterialDict );
	DELEGATE_TO_OBJECT_1C( IMaterialInternal *, GetMaterialInternal, MaterialHandle_t, &m_MaterialDict );
	DELEGATE_TO_OBJECT_0C( int,				GetNumMaterials, &m_MaterialDict );
	DELEGATE_TO_OBJECT_1V(					AddMaterialToMaterialList, IMaterialInternal *, &m_MaterialDict );
	DELEGATE_TO_OBJECT_1V(					RemoveMaterial, IMaterialInternal *, &m_MaterialDict );
	DELEGATE_TO_OBJECT_1V(					RemoveMaterialSubRect, IMaterialInternal *, &m_MaterialDict );

	//---------------------------------

	ITexture *								FindTexture( const char* pTextureName, const char *pTextureGroupName, bool complain = true );
	bool									IsTextureLoaded( const char* pTextureName ) const;

	void									AddTextureAlias( const char *pAlias, const char *pRealName );
	void									RemoveTextureAlias( const char *pAlias );

	void									SetExcludedTextures( const char *pScriptName );
	void									UpdateExcludedTextures( void );

	// Creates a procedural texture
	ITexture *								CreateProceduralTexture( const char	*pTextureName, 
																		const char			*pTextureGroupName, 
																		int					w, 
																		int					h, 
																		ImageFormat			fmt, 
																		int					nFlags );

	//
	// Render targets
	//
	void									BeginRenderTargetAllocation();
	void									EndRenderTargetAllocation();	// Simulate an Alt-Tab in here, which causes a release/restore of all resources

	// Creates a texture for use as a render target
	ITexture *								CreateRenderTargetTexture( int w, 
																		int h, 
																		RenderTargetSizeMode_t sizeMode,	// Controls how size is generated (and regenerated on video mode change).
																		ImageFormat	format,
																		MaterialRenderTargetDepth_t depth = MATERIAL_RT_DEPTH_SHARED );

	ITexture *								CreateNamedRenderTargetTextureEx(  const char *pRTName,				// Pass in NULL here for an unnamed render target.
																				int w, 
																				int h, 
																				RenderTargetSizeMode_t sizeMode,	// Controls how size is generated (and regenerated on video mode change).
																				ImageFormat format, 
																				MaterialRenderTargetDepth_t depth = MATERIAL_RT_DEPTH_SHARED, 
																				unsigned int textureFlags = TEXTUREFLAGS_CLAMPS | TEXTUREFLAGS_CLAMPT,
																				unsigned int renderTargetFlags = 0 );

	ITexture *								CreateNamedRenderTargetTexture( const char *pRTName, 
																			int w, 
																			int h, 
																			RenderTargetSizeMode_t sizeMode,	// Controls how size is generated (and regenerated on video mode change).
																			ImageFormat format, 
																			MaterialRenderTargetDepth_t depth = MATERIAL_RT_DEPTH_SHARED, 
																			bool bClampTexCoords = true, 
																			bool bAutoMipMap = false );

	ITexture *								CreateNamedRenderTargetTextureEx2( const char *pRTName,				// Pass in NULL here for an unnamed render target.
																				int w, 
																				int h, 
																				RenderTargetSizeMode_t sizeMode,	// Controls how size is generated (and regenerated on video mode change).
																				ImageFormat format, 
																				MaterialRenderTargetDepth_t depth = MATERIAL_RT_DEPTH_SHARED, 
																				unsigned int textureFlags = TEXTUREFLAGS_CLAMPS | TEXTUREFLAGS_CLAMPT,
																				unsigned int renderTargetFlags = 0 );


	// -----------------------------------------------------------
	
	bool									OnDrawMesh( IMesh *pMesh, int firstIndex, int numIndices );
	bool									OnDrawMesh( IMesh *pMesh, CPrimList *pLists, int nLists );
	DELEGATE_TO_OBJECT_3( bool,				OnSetFlexMesh, IMesh *, IMesh *, int, GetRenderContextInternal() );
	DELEGATE_TO_OBJECT_3( bool,				OnSetColorMesh, IMesh *, IMesh *, int, GetRenderContextInternal() );
	DELEGATE_TO_OBJECT_2( bool,				OnSetPrimitiveType, IMesh *, MaterialPrimitiveType_t, GetRenderContextInternal() );
	DELEGATE_TO_OBJECT_0V(					SyncMatrices, GetRenderContextInternal() );
	DELEGATE_TO_OBJECT_1V(					SyncMatrix, MaterialMatrixMode_t, GetRenderContextInternal() );
	DELEGATE_TO_OBJECT_0( bool,				OnFlushBufferedPrimitives, GetRenderContextInternal() );


	// -----------------------------------------------------------
	// Lightmaps delegates
	// -----------------------------------------------------------
	DELEGATE_TO_OBJECT_0V(					BeginLightmapAllocation, &m_Lightmaps );

	void EndLightmapAllocation()
	{
		GetLightmaps()->EndLightmapAllocation();
		AllocateStandardTextures();
	}

	DELEGATE_TO_OBJECT_4( int,				AllocateLightmap, int, int, LightmapOffset_t, IMaterial *, &m_Lightmaps );
	DELEGATE_TO_OBJECT_1( int,				AllocateWhiteLightmap, IMaterial *, &m_Lightmaps );
	DELEGATE_TO_OBJECT_3( int,				AllocateDynamicLightmap, LightmapOffset_t, int *, int, &m_Lightmaps );
	void									UpdateLightmap( int, LightmapOffset_t, LightmapOffset_t, float *, float *, float *, float * );
	DELEGATE_TO_OBJECT_0( int,				GetNumSortIDs, &m_Lightmaps );
	DELEGATE_TO_OBJECT_1V(					GetSortInfo, MaterialSystem_SortInfo_t *, &m_Lightmaps );
	DELEGATE_TO_OBJECT_3VC(					GetLightmapPageSize, int, int *, int *, &m_Lightmaps );
	DELEGATE_TO_OBJECT_0V(					ResetMaterialLightmapPageInfo, &m_Lightmaps );
	DELEGATE_TO_OBJECT_1C( int,				GetLightmapWidth, int, &m_Lightmaps );
	DELEGATE_TO_OBJECT_1C( int,				GetLightmapHeight, int, &m_Lightmaps );
	DELEGATE_TO_OBJECT_0V(					BeginUpdateLightmaps, &m_Lightmaps );
	DELEGATE_TO_OBJECT_0V(					EndUpdateLightmaps, &m_Lightmaps );

	// -----------------------------------------------------------
	// Render context delegates
	// -----------------------------------------------------------

	// IMaterialSystem
	DELEGATE_TO_OBJECT_3V(					ClearBuffers, bool, bool, bool, GetRenderContextInternal() );

	// IMaterialSystemInternal
	DELEGATE_TO_OBJECT_0( IMaterial *,		GetCurrentMaterial, GetRenderContextInternal() );
	DELEGATE_TO_OBJECT_0( int,				GetLightmapPage, GetRenderContextInternal() );
	DELEGATE_TO_OBJECT_0( ITexture *,		GetLocalCubemap, GetRenderContextInternal() );
	DELEGATE_TO_OBJECT_1V(					ForceDepthFuncEquals, bool, GetRenderContextInternal() );
	DELEGATE_TO_OBJECT_0( MaterialHeightClipMode_t, GetHeightClipMode, GetRenderContextInternal() );
	DELEGATE_TO_OBJECT_0C( bool,			InFlashlightMode, GetRenderContextInternal() );

	// IShaderUtil
	DELEGATE_TO_OBJECT_2V(					BindStandardTexture, Sampler_t, StandardTextureId_t, &m_HardwareRenderContext );
	DELEGATE_TO_OBJECT_2V(					BindStandardVertexTexture, VertexTextureSampler_t, StandardTextureId_t, &m_HardwareRenderContext );
	DELEGATE_TO_OBJECT_2V(					GetLightmapDimensions, int *, int *, &m_HardwareRenderContext );
	DELEGATE_TO_OBJECT_3V(					GetStandardTextureDimensions, int *, int *, StandardTextureId_t, &m_HardwareRenderContext );
	DELEGATE_TO_OBJECT_0( MorphFormat_t,	GetBoundMorphFormat, &m_HardwareRenderContext );
	DELEGATE_TO_OBJECT_1( ITexture *,		GetRenderTargetEx, int, &m_HardwareRenderContext );
	DELEGATE_TO_OBJECT_6V(					DrawClearBufferQuad, unsigned char, unsigned char, unsigned char, unsigned char, bool, bool, &m_HardwareRenderContext );
	DELEGATE_TO_OBJECT_0C( int,				MaxHWMorphBatchCount, g_pMorphMgr );
	DELEGATE_TO_OBJECT_1V(					GetCurrentColorCorrection, ShaderColorCorrectionInfo_t*, g_pColorCorrectionSystem );

#if defined( _X360 )
	void									ListUsedMaterials();
	HXUIFONT								OpenTrueTypeFont( const char *pFontname, int tall, int style );
	void									CloseTrueTypeFont( HXUIFONT hFont );
	bool									GetTrueTypeFontMetrics( HXUIFONT hFont, XUIFontMetrics *pFontMetrics, XUICharMetrics charMetrics[256] );
	// Render a sequence of characters and extract the data into a buffer
	// For each character, provide the width+height of the font texture subrect,
	// an offset to apply when rendering the glyph, and an offset into a buffer to receive the RGBA data
	bool									GetTrueTypeGlyphs( HXUIFONT hFont, int numChars, wchar_t *pWch, int *pOffsetX, int *pOffsetY, int *pWidth, int *pHeight, unsigned char *pRGBA, int *pOffset );
	void									ReadBackBuffer( Rect_t *pSrcRect, Rect_t *pDstRect, unsigned char *pData, ImageFormat dstFormat, int nDstStride );
	void									PersistDisplay();
	void									*GetD3DDevice();
	bool									OwnGPUResources( bool bEnable );
#endif

	MaterialLock_t							Lock();
	void									Unlock( MaterialLock_t );
	CMatCallQueue *							GetRenderCallQueue();
	uint									GetRenderThreadId() const { return m_nRenderThreadID; }
	void									UnbindMaterial( IMaterial *pMaterial );

	virtual void							CompactMemory();

	// -----------------------------------------------------------
private:
	CON_COMMAND_MEMBER_F( CMaterialSystem, "mat_showmaterials", DebugPrintUsedMaterials, "Show materials.", 0 );
	CON_COMMAND_MEMBER_F( CMaterialSystem, "mat_showmaterialsverbose", DebugPrintUsedMaterialsVerbose, "Show materials (verbose version).", 0 );
	CON_COMMAND_MEMBER_F( CMaterialSystem, "mat_showtextures", DebugPrintUsedTextures, "Show used textures.", 0 );
	CON_COMMAND_MEMBER_F( CMaterialSystem, "mat_reloadallmaterials", ReloadAllMaterials, "Reloads all materials", FCVAR_CHEAT );
	CON_COMMAND_MEMBER_F( CMaterialSystem, "mat_reloadmaterial", ReloadMaterials, "Reloads a single material", FCVAR_CHEAT );
	CON_COMMAND_MEMBER_F( CMaterialSystem, "mat_reloadtextures", ReloadTextures, "Reloads all textures", FCVAR_CHEAT );

#if defined( _X360 )
	CON_COMMAND_MEMBER_F( CMaterialSystem, "mat_material_list", ListUsedMaterials, "Show used textures.", 0 );
#endif

	friend void* InstantiateMaterialSystemV76Interface();
	friend CMaterialSystem *CMatLightmaps::GetMaterialSystem() const;

	void ThreadExecuteQueuedContext( CMatQueuedRenderContext *pContext );

	CMaterialDict							m_MaterialDict;
	CMatLightmaps							m_Lightmaps;
	CThreadLocal<IMatRenderContextInternal *> m_pRenderContext;
	CMatRenderContext						m_HardwareRenderContext;

	CMatQueuedRenderContext					m_QueuedRenderContexts[2];
	int										m_iCurQueuedContext;

	MaterialThreadMode_t					m_ThreadMode;
	MaterialThreadMode_t					m_IdealThreadMode;
	int										m_nServiceThread;

	//---------------------------------

	char *									m_pShaderDLL;
	CSysModule *							m_ShaderHInst; // Used to dynamically load the shader DLL
	CreateInterfaceFn						m_ShaderAPIFactory;

	int										m_nAdapter;
	int										m_nAdapterFlags;

	//---------------------------------

	IMaterialProxyFactory *					m_pMaterialProxyFactory;
	
	//---------------------------------
	// Callback methods for releasing + restoring video memory
	CUtlVector< MaterialBufferReleaseFunc_t > m_ReleaseFunc;
	CUtlVector< MaterialBufferRestoreFunc_t > m_RestoreFunc;

	//---------------------------------

	bool									m_bRequestedEditorMaterials;
	bool									m_bCanUseEditorMaterials;

	//---------------------------------

	// Store texids for various things
	ShaderAPITextureHandle_t				m_FullbrightLightmapTextureHandle;
	ShaderAPITextureHandle_t				m_FullbrightBumpedLightmapTextureHandle;
	ShaderAPITextureHandle_t 				m_BlackTextureHandle;
	ShaderAPITextureHandle_t				m_FlatNormalTextureHandle;
	ShaderAPITextureHandle_t				m_GreyTextureHandle;
	ShaderAPITextureHandle_t				m_GreyAlphaZeroTextureHandle;
	ShaderAPITextureHandle_t				m_WhiteTextureHandle;
	ShaderAPITextureHandle_t				m_LinearToGammaTableTextureHandle; //linear to gamma srgb conversion lookup for the current hardware
	ShaderAPITextureHandle_t				m_LinearToGammaIdentityTableTextureHandle; //An identity lookup for when srgb writes are off
	ShaderAPITextureHandle_t				m_MaxDepthTextureHandle; //a 1x1 texture with maximum depth values.

	// Have we allocated the standard lightmaps?
	bool									m_StandardTexturesAllocated;

	//---------------------------------
	// Shared materials used for debugging....
	//---------------------------------

	enum BufferClearType_t
	{
		BUFFER_CLEAR_NONE,
		BUFFER_CLEAR_COLOR,
		BUFFER_CLEAR_DEPTH,
		BUFFER_CLEAR_COLOR_AND_DEPTH,

		BUFFER_CLEAR_TYPE_COUNT
	};

    IMaterialInternal *						m_pBufferClearObeyStencil[BUFFER_CLEAR_TYPE_COUNT];
	IMaterialInternal *						m_pDrawFlatMaterial;
	IMaterialInternal *						m_pRenderTargetBlitMaterial;

	//---------------------------------

	const char *							m_pForcedTextureLoadPathID;
	uint									m_nRenderThreadID;
	bool									m_bAllocatingRenderTargets;
	bool									m_bInStubMode;
	bool									m_bGeneratedConfig;
	bool									m_bInFrame;

	//---------------------------------

	CJob *									m_pActiveAsyncJob;
};

//-----------------------------------------------------------------------------

inline CMaterialSystem *CMatLightmaps::GetMaterialSystem() const
{
	return GET_OUTER( CMaterialSystem, m_Lightmaps );
}

//-----------------------------------------------------------------------------

#endif // CMATERIALSYSTEM_H

//===== Copyright © 1996-2005, Valve Corporation, All rights reserved. ======//
//
// Purpose: 
//
//===========================================================================//
	
#include "pch_materialsystem.h"

#define MATSYS_INTERNAL

#include "cmaterialsystem.h"

#include "colorspace.h"
#include "materialsystem/materialsystem_config.h"
#include "IHardwareConfigInternal.h"
#include "shadersystem.h"
#include "texturemanager.h"
#include "shaderlib/ShaderDLL.h"
#include "tier1/callqueue.h"
#include "vstdlib/jobthread.h"
#include "cmatnullrendercontext.h"
#include "filesystem/IQueuedLoader.h"
#if defined( _X360 )
#include "xbox/xbox_console.h"
#include "xbox/xbox_win32stubs.h"
#endif

// NOTE: This must be the last file included!!!
#include "tier0/memdbgon.h"

#ifdef _LINUX
#define _finite finite
#endif

// this is hooked into the engines convar
ConVar mat_debugalttab( "mat_debugalttab", "0", FCVAR_CHEAT );

ConVar mat_forcemanagedtextureintohardware( "mat_forcemanagedtextureintohardware", "1" );

ConVar mat_supportflashlight( "mat_supportflashlight", "-1", 0, "0 - do not support flashlight (don't load flashlight shader combos), 1 - flashlight is supported" );

IMaterialInternal *g_pErrorMaterial = NULL;

CreateInterfaceFn g_fnMatSystemConnectCreateInterface = NULL;  


//#define PERF_TESTING 1

//-----------------------------------------------------------------------------
// Implementational structures
//-----------------------------------------------------------------------------

#define MATERIAL_MAX_TREE_DEPTH 256


//-----------------------------------------------------------------------------
// Singleton instance exposed to the engine
//-----------------------------------------------------------------------------

CMaterialSystem g_MaterialSystem;
EXPOSE_SINGLE_INTERFACE_GLOBALVAR( CMaterialSystem, IMaterialSystem, 
						MATERIAL_SYSTEM_INTERFACE_VERSION, g_MaterialSystem );

// Expose this to the external shader DLLs
MaterialSystem_Config_t g_config;
EXPOSE_SINGLE_INTERFACE_GLOBALVAR( MaterialSystem_Config_t, MaterialSystem_Config_t, MATERIALSYSTEM_CONFIG_VERSION, g_config );

//-----------------------------------------------------------------------------

CThreadFastMutex g_MatSysMutex;

//-----------------------------------------------------------------------------
// Purpose: additional materialsystem information, internal use only
//-----------------------------------------------------------------------------
#ifndef _X360
struct MaterialSystem_Config_Internal_t
{
	int r_waterforceexpensive;
};
MaterialSystem_Config_Internal_t g_config_internal;
#endif

//-----------------------------------------------------------------------------
// Necessary to allow the shader DLLs to get ahold of IMaterialSystemHardwareConfig
//-----------------------------------------------------------------------------
IHardwareConfigInternal* g_pHWConfig = 0;
static void *GetHardwareConfig()
{
	if ( g_pHWConfig )
		return (IMaterialSystemHardwareConfig*)g_pHWConfig;

	// can't call QueryShaderAPI here because it calls a factory function
	// and we end up in an infinite recursion
	return NULL;
}
EXPOSE_INTERFACE_FN( GetHardwareConfig, IMaterialSystemHardwareConfig, MATERIALSYSTEM_HARDWARECONFIG_INTERFACE_VERSION );


//-----------------------------------------------------------------------------
// Necessary to allow the shader DLLs to get ahold of ICvar
//-----------------------------------------------------------------------------
static void *GetICVar()
{
	return g_pCVar;
}
EXPOSE_INTERFACE_FN( GetICVar, ICVar, CVAR_INTERFACE_VERSION );

//-----------------------------------------------------------------------------
// Accessor to get at the material system
//-----------------------------------------------------------------------------
IMaterialSystemInternal *g_pInternalMaterialSystem = &g_MaterialSystem;
IShaderUtil *g_pShaderUtil = &g_MaterialSystem;

//-----------------------------------------------------------------------------
// Factory used to get at internal interfaces (used by shaderapi + shader dlls)
//-----------------------------------------------------------------------------
void *ShaderFactory( const char *pName, int *pReturnCode )
{
	if (pReturnCode)
	{
		*pReturnCode = IFACE_OK;
	}
	
	if ( !Q_stricmp( pName, FILESYSTEM_INTERFACE_VERSION ))
		return g_pFullFileSystem;

	if ( !Q_stricmp( pName, QUEUEDLOADER_INTERFACE_VERSION ))
		return g_pQueuedLoader;

	if ( !Q_stricmp( pName, SHADER_UTIL_INTERFACE_VERSION ))
		return g_pShaderUtil;

	void * pInterface = g_MaterialSystem.QueryInterface( pName );
	if ( pInterface )
		return pInterface;

	if ( pReturnCode )
	{
		*pReturnCode = IFACE_FAILED;
	}
	return NULL;	
}

//-----------------------------------------------------------------------------
// Resource preloading for materials.
//-----------------------------------------------------------------------------
class CResourcePreloadMaterial : public CResourcePreload
{
	virtual bool CreateResource( const char *pName )
	{
		IMaterial *pMaterial = g_MaterialSystem.FindMaterial( pName, TEXTURE_GROUP_WORLD, false );
		IMaterialInternal *pMatInternal = static_cast< IMaterialInternal * >( pMaterial );
		if ( pMatInternal )
		{
			// always work with the realtime material internally
			pMatInternal = pMatInternal->GetRealTimeVersion();

			// tag these for later identification (prevents an unwanted purge)
			pMatInternal->MarkAsPreloaded( true );
			if ( !pMatInternal->IsErrorMaterial() )
			{
				// force material's textures to create now
				pMatInternal->Precache();
				return true;
			}
		}

		return false;
	}

	//-----------------------------------------------------------------------------
	// Called before queued loader i/o jobs are actually performed. Must free up memory
	// to ensure i/o requests have enough memory to succeed.  The materials that were
	// touched by the CreateResource() are inhibited from purging (as is their textures,
	// by virtue of ref counts), all others are candidates.  The preloaded materials
	// are by definition zero ref'd until owned by the normal loading process. Any material
	// that stays zero ref'd is a candidate for the post load purge.
	//-----------------------------------------------------------------------------
	virtual void PurgeUnreferencedResources()
	{
		bool bSpew = ( g_pQueuedLoader->GetSpewDetail() & LOADER_DETAIL_PURGES ) != 0;

		bool bDidUncacheMaterial = false;
		MaterialHandle_t hNext;
		for ( MaterialHandle_t hMaterial = g_MaterialSystem.FirstMaterial(); hMaterial != g_MaterialSystem.InvalidMaterial(); hMaterial = hNext )
		{
			hNext = g_MaterialSystem.NextMaterial( hMaterial );

			IMaterialInternal *pMatInternal = g_MaterialSystem.GetMaterialInternal( hMaterial );
			Assert( pMatInternal->GetReferenceCount() >= 0 );

			// preloaded materials are safe from this pre-purge
			if ( !pMatInternal->IsPreloaded() )
			{
				// undo any possible artifical ref count
				pMatInternal->ArtificialRelease();
				if ( pMatInternal->GetReferenceCount() <= 0 )
				{
					if ( bSpew )
					{
						Msg( "CResourcePreloadMaterial: Purging: %s (%d)\n", pMatInternal->GetName(), pMatInternal->GetReferenceCount() );
					}
					bDidUncacheMaterial = true;
					pMatInternal->Uncache();
					pMatInternal->DeleteIfUnreferenced();
				}
			}
			else
			{
				// clear the bit
				pMatInternal->MarkAsPreloaded( false );
			}
		}

		// purged materials unreference their textures
		// purge any zero ref'd textures
		TextureManager()->RemoveUnusedTextures();

		// fixup any excluded textures, may cause some new batch requests
		MaterialSystem()->UpdateExcludedTextures();
	}

	virtual void PurgeAll()
	{
		bool bSpew = ( g_pQueuedLoader->GetSpewDetail() & LOADER_DETAIL_PURGES ) != 0;

		bool bDidUncacheMaterial = false;
		MaterialHandle_t hNext;
		for ( MaterialHandle_t hMaterial = g_MaterialSystem.FirstMaterial(); hMaterial != g_MaterialSystem.InvalidMaterial(); hMaterial = hNext )
		{
			hNext = g_MaterialSystem.NextMaterial( hMaterial );

			IMaterialInternal *pMatInternal = g_MaterialSystem.GetMaterialInternal( hMaterial );
			Assert( pMatInternal->GetReferenceCount() >= 0 );

			pMatInternal->MarkAsPreloaded( false );
			// undo any possible artifical ref count
			pMatInternal->ArtificialRelease();
			if ( pMatInternal->GetReferenceCount() <= 0 )
			{
				if ( bSpew )
				{
					Msg( "CResourcePreloadMaterial: Purging: %s (%d)\n", pMatInternal->GetName(), pMatInternal->GetReferenceCount() );
				}
				bDidUncacheMaterial = true;
				pMatInternal->Uncache();
				pMatInternal->DeleteIfUnreferenced();
			}
		}

		// purged materials unreference their textures
		// purge any zero ref'd textures
		TextureManager()->RemoveUnusedTextures();
	}
};

static CResourcePreloadMaterial s_ResourcePreloadMaterial;

//-----------------------------------------------------------------------------
// Resource preloading for cubemaps.
//-----------------------------------------------------------------------------
class CResourcePreloadCubemap : public CResourcePreload
{
	virtual bool CreateResource( const char *pName )
	{
		ITexture *pTexture = g_MaterialSystem.FindTexture( pName, TEXTURE_GROUP_CUBE_MAP, true );
		ITextureInternal *pTexInternal = static_cast< ITextureInternal * >( pTexture );
		if ( pTexInternal )
		{
			// There can be cubemaps that are unbound by materials. To prevent an unwanted purge,
			// mark and increase the ref count. Otherwise the pre-purge discards these zero
			// ref'd textures, and then the normal loading process hitches on the miss.
			// The zombie cubemaps DO get discarded after the normal loading process completes
			// if no material references them.
			pTexInternal->MarkAsPreloaded( true );
			pTexInternal->IncrementReferenceCount();
			if ( !IsErrorTexture( pTexInternal ) )
			{
				return true;
			}
		}
		return false;
	}

	//-----------------------------------------------------------------------------
	// All valid cubemaps should have been owned by their materials.  Undo the preloaded
	// cubemap locks. Any zero ref'd cubemaps will be purged by the normal loading path conclusion.
	//-----------------------------------------------------------------------------
	virtual void OnEndMapLoading( bool bAbort )
	{
		int iIndex = -1;
		for ( ;; )
		{
			ITextureInternal *pTexInternal;
			iIndex = TextureManager()->FindNext( iIndex, &pTexInternal );
			if ( iIndex == -1 || !pTexInternal )
			{
				// end of list
				break;
			}

			if ( pTexInternal->IsPreloaded() )
			{
				// undo the artificial increase
				pTexInternal->MarkAsPreloaded( false );
				pTexInternal->DecrementReferenceCount();
			}
		}	
	}
};
static CResourcePreloadCubemap s_ResourcePreloadCubemap;

//-----------------------------------------------------------------------------
// Creates the debugging materials
//-----------------------------------------------------------------------------
void CMaterialSystem::CreateDebugMaterials()
{
	if ( !m_pDrawFlatMaterial )
	{
		KeyValues *pVMTKeyValues = new KeyValues( "UnlitGeneric" );
		pVMTKeyValues->SetInt( "$model", 1 );
		pVMTKeyValues->SetFloat( "$decalscale", 0.05f );
		pVMTKeyValues->SetString( "$basetexture", "error" );	// This is the "error texture"
		g_pErrorMaterial = static_cast<IMaterialInternal*>(CreateMaterial( "___error.vmt", pVMTKeyValues ))->GetRealTimeVersion();

		pVMTKeyValues = new KeyValues( "UnlitGeneric" );
		pVMTKeyValues->SetInt( "$flat", 1 );
		pVMTKeyValues->SetInt( "$vertexcolor", 1 );
		m_pDrawFlatMaterial = static_cast<IMaterialInternal*>(CreateMaterial( "___flat.vmt", pVMTKeyValues ))->GetRealTimeVersion();

		pVMTKeyValues = new KeyValues( "BufferClearObeyStencil" );
		pVMTKeyValues->SetInt( "$nocull", 1 );
		m_pBufferClearObeyStencil[BUFFER_CLEAR_NONE] = static_cast<IMaterialInternal*>(CreateMaterial( "___buffer_clear_obey_stencil0.vmt", pVMTKeyValues ))->GetRealTimeVersion();

		pVMTKeyValues = new KeyValues( "BufferClearObeyStencil" );
		pVMTKeyValues->SetInt( "$nocull", 1 );
		pVMTKeyValues->SetInt( "$clearcolor", 1 );
		pVMTKeyValues->SetInt( "$vertexcolor", 1 );
		m_pBufferClearObeyStencil[BUFFER_CLEAR_COLOR] = static_cast<IMaterialInternal*>(CreateMaterial( "___buffer_clear_obey_stencil1.vmt", pVMTKeyValues ))->GetRealTimeVersion();

		pVMTKeyValues = new KeyValues( "BufferClearObeyStencil" );
		pVMTKeyValues->SetInt( "$nocull", 1 );
		pVMTKeyValues->SetInt( "$cleardepth", 1 );
		m_pBufferClearObeyStencil[BUFFER_CLEAR_DEPTH] = static_cast<IMaterialInternal*>(CreateMaterial( "___buffer_clear_obey_stencil2.vmt", pVMTKeyValues ))->GetRealTimeVersion();

		pVMTKeyValues = new KeyValues( "BufferClearObeyStencil" );
		pVMTKeyValues->SetInt( "$nocull", 1 );
		pVMTKeyValues->SetInt( "$cleardepth", 1 );
		pVMTKeyValues->SetInt( "$clearcolor", 1 );
		pVMTKeyValues->SetInt( "$vertexcolor", 1 );
		m_pBufferClearObeyStencil[BUFFER_CLEAR_COLOR_AND_DEPTH] = static_cast<IMaterialInternal*>(CreateMaterial( "___buffer_clear_obey_stencil3.vmt", pVMTKeyValues ))->GetRealTimeVersion();

		if ( IsX360() )
		{
			pVMTKeyValues = new KeyValues( "RenderTargetBlit_X360" );
			m_pRenderTargetBlitMaterial = static_cast<IMaterialInternal*>(CreateMaterial( "___renderTargetBlit.vmt", pVMTKeyValues ))->GetRealTimeVersion();
		}

		ShaderSystem()->CreateDebugMaterials();
	}
}


//-----------------------------------------------------------------------------
// Creates the debugging materials
//-----------------------------------------------------------------------------
void CMaterialSystem::CleanUpDebugMaterials()
{
	if ( m_pDrawFlatMaterial )
	{
		g_pErrorMaterial->DecrementReferenceCount();
		m_pDrawFlatMaterial->DecrementReferenceCount();

		RemoveMaterial( g_pErrorMaterial );
		RemoveMaterial( m_pDrawFlatMaterial );

		g_pErrorMaterial = NULL;
		m_pDrawFlatMaterial = NULL;

		for ( int i = BUFFER_CLEAR_NONE; i < BUFFER_CLEAR_TYPE_COUNT; ++i )
		{
			m_pBufferClearObeyStencil[i]->DecrementReferenceCount();
			RemoveMaterial( m_pBufferClearObeyStencil[i] );
			m_pBufferClearObeyStencil[i] = NULL;
		}

		if ( IsX360() )
		{
			m_pRenderTargetBlitMaterial->DecrementReferenceCount();
			RemoveMaterial( m_pRenderTargetBlitMaterial );
			m_pRenderTargetBlitMaterial = NULL;
		}

		ShaderSystem()->CleanUpDebugMaterials();
	}
}


//-----------------------------------------------------------------------------
// Constructor
//-----------------------------------------------------------------------------
CMaterialSystem::CMaterialSystem()
{
	m_nRenderThreadID = 0xFFFFFFFF;
	m_ShaderHInst = 0;
	m_pMaterialProxyFactory = NULL;
	m_nAdapter = 0;
	m_nAdapterFlags = 0;
	m_bRequestedEditorMaterials = false;
	m_bCanUseEditorMaterials = false;
	m_StandardTexturesAllocated = false;
	m_bInFrame = false;
	m_pShaderDLL = NULL;
	m_FullbrightLightmapTextureHandle = INVALID_SHADERAPI_TEXTURE_HANDLE;
	m_FullbrightBumpedLightmapTextureHandle = INVALID_SHADERAPI_TEXTURE_HANDLE;
	m_BlackTextureHandle = INVALID_SHADERAPI_TEXTURE_HANDLE;
	m_FlatNormalTextureHandle = INVALID_SHADERAPI_TEXTURE_HANDLE;
	m_GreyTextureHandle = INVALID_SHADERAPI_TEXTURE_HANDLE;
	m_GreyAlphaZeroTextureHandle = INVALID_SHADERAPI_TEXTURE_HANDLE;
	m_WhiteTextureHandle = INVALID_SHADERAPI_TEXTURE_HANDLE;
	m_LinearToGammaTableTextureHandle = INVALID_SHADERAPI_TEXTURE_HANDLE;
	m_LinearToGammaIdentityTableTextureHandle = INVALID_SHADERAPI_TEXTURE_HANDLE;
	m_MaxDepthTextureHandle = INVALID_SHADERAPI_TEXTURE_HANDLE;

	m_bInStubMode = false;
	m_pForcedTextureLoadPathID = NULL;
	m_bAllocatingRenderTargets = false;
	m_pRenderContext.Set( &m_HardwareRenderContext );
	m_iCurQueuedContext = 0;
	m_bGeneratedConfig = false;
	m_pActiveAsyncJob = NULL;
	m_IdealThreadMode = m_ThreadMode = MATERIAL_SINGLE_THREADED;
	m_nServiceThread = 0;
}

CMaterialSystem::~CMaterialSystem()
{
	if (m_pShaderDLL)
	{
		delete[] m_pShaderDLL;
	}
}


//-----------------------------------------------------------------------------
// Creates/destroys the shader implementation for the selected API
//-----------------------------------------------------------------------------
CreateInterfaceFn CMaterialSystem::CreateShaderAPI( char const* pShaderDLL )
{
	if ( !pShaderDLL )
		return 0;

	// Clean up the old shader
	DestroyShaderAPI();

	// Load the new shader
	m_ShaderHInst = Sys_LoadModule( pShaderDLL );

	// Error loading the shader
	if ( !m_ShaderHInst )
		return 0;

	// Get our class factory methods...
	return Sys_GetFactory( m_ShaderHInst );
}

void CMaterialSystem::DestroyShaderAPI()
{
	if (m_ShaderHInst)
	{
		// NOTE: By unloading the library, this will destroy m_pShaderAPI
		Sys_UnloadModule( m_ShaderHInst );
		g_pShaderAPI = 0;
		g_pHWConfig = 0;
		g_pShaderShadow = 0;
		m_ShaderHInst = 0;
	}
}


//-----------------------------------------------------------------------------
// Sets which shader we should be using. Has to be done before connect!
//-----------------------------------------------------------------------------
void CMaterialSystem::SetShaderAPI( char const *pShaderAPIDLL )
{
	if ( m_ShaderAPIFactory )
	{
		Error( "Cannot set the shader API twice!\n" );
	}

	if ( !pShaderAPIDLL )
	{
		pShaderAPIDLL = "shaderapidx9";
	}

	// m_pShaderDLL is needed to spew driver info
	Assert( pShaderAPIDLL );
	int len = Q_strlen( pShaderAPIDLL ) + 1;
	m_pShaderDLL = new char[len];
	memcpy( m_pShaderDLL, pShaderAPIDLL, len );

	m_ShaderAPIFactory = CreateShaderAPI( pShaderAPIDLL );
	if ( !m_ShaderAPIFactory )
	{
		DestroyShaderAPI();
	}
}
	

//-----------------------------------------------------------------------------
// Connect/disconnect
//-----------------------------------------------------------------------------
bool CMaterialSystem::Connect( CreateInterfaceFn factory )
{
	if ( !factory )
		return false;

	if ( !BaseClass::Connect( factory ) )
		return false;

	if ( !g_pFullFileSystem )
	{
		Warning( "The material system requires the filesystem to run!\n" );
		return false;
	}

	// Get at the interfaces exported by the shader DLL
	g_pShaderDeviceMgr = (IShaderDeviceMgr*)m_ShaderAPIFactory( SHADER_DEVICE_MGR_INTERFACE_VERSION, 0 );
	if ( !g_pShaderDeviceMgr )
		return false;
	g_pHWConfig = (IHardwareConfigInternal*)m_ShaderAPIFactory( MATERIALSYSTEM_HARDWARECONFIG_INTERFACE_VERSION, 0 );
	if ( !g_pHWConfig )
		return false;

	// FIXME: ShaderAPI, ShaderDevice, and ShaderShadow should only come in after setting mode
	g_pShaderAPI = (IShaderAPI*)m_ShaderAPIFactory( SHADERAPI_INTERFACE_VERSION, 0 );
	if ( !g_pShaderAPI )
		return false;
	g_pShaderDevice = (IShaderDevice*)m_ShaderAPIFactory( SHADER_DEVICE_INTERFACE_VERSION, 0 );
	if ( !g_pShaderDevice )
		return false;
	g_pShaderShadow = (IShaderShadow*)m_ShaderAPIFactory( SHADERSHADOW_INTERFACE_VERSION, 0 );
	if ( !g_pShaderShadow )
		return false;

	// Remember the factory for connect
	g_fnMatSystemConnectCreateInterface = factory;

	return g_pShaderDeviceMgr->Connect( ShaderFactory );	
}

void CMaterialSystem::Disconnect()
{
	// Forget the factory for connect
	g_fnMatSystemConnectCreateInterface = NULL;

	if ( g_pShaderDeviceMgr )
	{
		g_pShaderDeviceMgr->Disconnect();
		g_pShaderDeviceMgr = NULL;

		// Unload the DLL
		DestroyShaderAPI();
	}
	g_pShaderAPI = NULL;
	g_pHWConfig = NULL;
	g_pShaderShadow = NULL;
	g_pShaderDevice = NULL;
	BaseClass::Disconnect();
}


//-----------------------------------------------------------------------------
// Used to enable editor materials. Must be called before Init.
//-----------------------------------------------------------------------------
void CMaterialSystem::EnableEditorMaterials()
{
	m_bRequestedEditorMaterials = true;
}


//-----------------------------------------------------------------------------
// Method to get at interfaces supported by the SHADDERAPI
//-----------------------------------------------------------------------------
void *CMaterialSystem::QueryShaderAPI( const char *pInterfaceName )
{
	// Returns various interfaces supported by the shader API dll
	void *pInterface = NULL;
	if (m_ShaderAPIFactory)
	{
		pInterface = m_ShaderAPIFactory( pInterfaceName, NULL );
	}
	return pInterface;
}


//-----------------------------------------------------------------------------
// Method to get at different interfaces supported by the material system
//-----------------------------------------------------------------------------
void *CMaterialSystem::QueryInterface( const char *pInterfaceName )
{
	// Returns various interfaces supported by the shader API dll
	void *pInterface = QueryShaderAPI( pInterfaceName );
	if ( pInterface )
		return pInterface;

	CreateInterfaceFn factory = Sys_GetFactoryThis();	// This silly construction is necessary
	return factory( pInterfaceName, NULL );				// to prevent the LTCG compiler from crashing.
}


//-----------------------------------------------------------------------------
// Must be called before Init(), if you're going to call it at all...
//-----------------------------------------------------------------------------
void CMaterialSystem::SetAdapter( int nAdapter, int nAdapterFlags )
{
	m_nAdapter = nAdapter;
	m_nAdapterFlags = nAdapterFlags;
}

	
//-----------------------------------------------------------------------------
// Initializes the color correction terms
//-----------------------------------------------------------------------------
void CMaterialSystem::InitColorCorrection( )
{
	if ( ColorCorrectionSystem() )
	{
		ColorCorrectionSystem()->Init();
	}
}

//-----------------------------------------------------------------------------
// Initialization + shutdown of the material system
//-----------------------------------------------------------------------------
InitReturnVal_t CMaterialSystem::Init()
{
	InitReturnVal_t nRetVal = BaseClass::Init();
	if ( nRetVal != INIT_OK )
		return nRetVal;

	// NOTE! : Overbright is 1.0 so that Hammer will work properly with the white bumped and unbumped lightmaps.
	MathLib_Init( 2.2f, 2.2f, 0.0f, 2.0f );

	g_pShaderDeviceMgr->SetAdapter( m_nAdapter, m_nAdapterFlags );
	if ( g_pShaderDeviceMgr->Init( ) != INIT_OK )
	{
		DestroyShaderAPI();
		return INIT_FAILED;
	}

	// Texture manager...
	TextureManager()->Init( m_nAdapterFlags );

	// Shader system!
	ShaderSystem()->Init();

#if defined( WIN32 ) && !defined( _X360 )
	// HACKHACK: <sigh> This horrible hack is possibly the only way to reliably detect an old
	// version of hammer initializing the material system. We need to know this so that we set
	// up the editor materials properly. If we don't do this, we never allocate the white lightmap,
	// for example. We can remove this when we update the SDK!!
	char szExeName[_MAX_PATH];
    if ( ::GetModuleFileName( ( HINSTANCE )GetModuleHandle( NULL ), szExeName, sizeof( szExeName ) ) )
    {
		char szRight[20];
		Q_StrRight( szExeName, 11, szRight, sizeof( szRight ) );
		if ( !Q_stricmp( szRight, "\\hammer.exe" ) )
		{
			m_bRequestedEditorMaterials = true;
		}
	}
#endif // WIN32

	m_bCanUseEditorMaterials = m_bRequestedEditorMaterials;

	InitColorCorrection();

	// Set up debug materials...
	CreateDebugMaterials();	

	if ( IsX360() )
	{
		g_pQueuedLoader->InstallLoader( RESOURCEPRELOAD_MATERIAL, &s_ResourcePreloadMaterial );
		g_pQueuedLoader->InstallLoader( RESOURCEPRELOAD_CUBEMAP, &s_ResourcePreloadCubemap );
	}

	// Set up a default material system config
//	GenerateConfigFromConfigKeyValues( &g_config, false );
//	UpdateConfig( false );

	return m_HardwareRenderContext.Init( this );
}

//-----------------------------------------------------------------------------
// For backwards compatability
//-----------------------------------------------------------------------------
static CreateInterfaceFn s_TempCVarFactory;
static CreateInterfaceFn s_TempFileSystemFactory;

void* TempCreateInterface( const char *pName, int *pReturnCode )
{
	void *pRetVal = NULL;

	if ( s_TempCVarFactory )
	{
		pRetVal = s_TempCVarFactory( pName, pReturnCode );
		if (pRetVal)
			return pRetVal;
	}

	pRetVal = s_TempFileSystemFactory( pName, pReturnCode );
	if (pRetVal)
		return pRetVal;

	return NULL;
}


//-----------------------------------------------------------------------------
// Initializes and shuts down the shader API
//-----------------------------------------------------------------------------
CreateInterfaceFn CMaterialSystem::Init( char const* pShaderAPIDLL,
										IMaterialProxyFactory *pMaterialProxyFactory,
										CreateInterfaceFn fileSystemFactory,
										CreateInterfaceFn cvarFactory )
{
	SetShaderAPI( pShaderAPIDLL );

	s_TempCVarFactory = cvarFactory;
	s_TempFileSystemFactory = fileSystemFactory;
	if ( !Connect( TempCreateInterface ) )
		return 0;

	if (Init() != INIT_OK)
		return NULL;

	// save the proxy factory
	m_pMaterialProxyFactory = pMaterialProxyFactory;

	return m_ShaderAPIFactory;
}

void CMaterialSystem::Shutdown( )
{
	m_HardwareRenderContext.Shutdown();

	// Clean up standard textures
	ReleaseStandardTextures();

	// Clean up the debug materials
	CleanUpDebugMaterials();

	g_pMorphMgr->FreeMaterials();
	g_pOcclusionQueryMgr->FreeOcclusionQueryObjects();

	GetLightmaps()->Shutdown();
	m_MaterialDict.Shutdown();

	// Shader system!
	ShaderSystem()->Shutdown();

	// Texture manager...
	TextureManager()->Shutdown();

	if (g_pShaderDeviceMgr)
	{
		g_pShaderDeviceMgr->Shutdown();
	}

	BaseClass::Shutdown();
}

void CMaterialSystem::ModInit()
{
	// Set up a default material system config
	GenerateConfigFromConfigKeyValues( &g_config, false );
	UpdateConfig( false );

	// Shader system!
	ShaderSystem()->ModInit();
}

void CMaterialSystem::ModShutdown()
{
	// Shader system!
	ShaderSystem()->ModShutdown();
}

//-----------------------------------------------------------------------------
// Returns the current adapter in use
//-----------------------------------------------------------------------------
IMaterialSystemHardwareConfig *CMaterialSystem::GetHardwareConfig( const char *pVersion, int *returnCode )
{
	return ( IMaterialSystemHardwareConfig * )m_ShaderAPIFactory( pVersion, returnCode );
}


//-----------------------------------------------------------------------------
// Returns the current adapter in use
//-----------------------------------------------------------------------------
int CMaterialSystem::GetCurrentAdapter() const
{
	return g_pShaderDevice->GetCurrentAdapter();
}


//-----------------------------------------------------------------------------
// 
//-----------------------------------------------------------------------------
void CMaterialSystem::SetThreadMode( MaterialThreadMode_t nextThreadMode, int nServiceThread ) 
{
	m_IdealThreadMode = nextThreadMode;
	m_nServiceThread = nServiceThread;
}

MaterialThreadMode_t CMaterialSystem::GetThreadMode()
{ 
	return m_ThreadMode; 
}

void CMaterialSystem::ExecuteQueued() 
{
}

//-----------------------------------------------------------------------------
// 
//-----------------------------------------------------------------------------
IMatRenderContext *CMaterialSystem::GetRenderContext()
{ 
	IMatRenderContext *pResult = m_pRenderContext.Get();
	if ( !pResult )
	{
		pResult = &m_HardwareRenderContext;
		m_pRenderContext.Set( &m_HardwareRenderContext );
	}
	return RetAddRef( pResult ); 
}

//-----------------------------------------------------------------------------
// 
//-----------------------------------------------------------------------------
IMatRenderContext *CMaterialSystem::CreateRenderContext( MaterialContextType_t type )
{
	switch ( type )
	{
	case MATERIAL_HARDWARE_CONTEXT:		
		return NULL;

	case MATERIAL_QUEUED_CONTEXT:		
		{
			CMatQueuedRenderContext *pQueuedContext = new CMatQueuedRenderContext;
			pQueuedContext->Init( this, &m_HardwareRenderContext );
			pQueuedContext->BeginQueue( &m_HardwareRenderContext );
			return pQueuedContext;

		}

	case MATERIAL_NULL_CONTEXT:
		{
			CMatRenderContextBase *pNullContext = CreateNullRenderContext();
			pNullContext->InitializeFrom( &m_HardwareRenderContext );
			return pNullContext;
		}
	}
	Assert(0);
	return NULL;
}

//-----------------------------------------------------------------------------
// 
//-----------------------------------------------------------------------------
IMatRenderContext *CMaterialSystem::SetRenderContext( IMatRenderContext *pNewContext )
{
	IMatRenderContext *pOldContext = m_pRenderContext.Get();
	if ( pNewContext )
	{
		pNewContext->AddRef();
		m_pRenderContext.Set( assert_cast<IMatRenderContextInternal *>(pNewContext) );
	}
	else
	{
		m_pRenderContext.Set( NULL );
	}
	return pOldContext;
}


//-----------------------------------------------------------------------------
// Get/Set Material proxy factory
//-----------------------------------------------------------------------------
IMaterialProxyFactory* CMaterialSystem::GetMaterialProxyFactory()
{ 
	return m_pMaterialProxyFactory; 
}

void CMaterialSystem::SetMaterialProxyFactory( IMaterialProxyFactory* pFactory )
{
	// Changing the factory requires an uncaching of all materials
	// since the factory may contain different proxies
	UncacheAllMaterials();

	m_pMaterialProxyFactory = pFactory; 
}


//-----------------------------------------------------------------------------
// Can we use editor materials?
//-----------------------------------------------------------------------------
bool CMaterialSystem::CanUseEditorMaterials() const
{
	return m_bCanUseEditorMaterials;
}


//-----------------------------------------------------------------------------
// Methods related to mode setting...
//-----------------------------------------------------------------------------

// Gets the number of adapters...
int	 CMaterialSystem::GetDisplayAdapterCount() const
{
	return g_pShaderDeviceMgr->GetAdapterCount( );
}

// Returns info about each adapter
void CMaterialSystem::GetDisplayAdapterInfo( int adapter, MaterialAdapterInfo_t& info ) const
{
	g_pShaderDeviceMgr->GetAdapterInfo( adapter, info );
}

// Returns the number of modes
int	 CMaterialSystem::GetModeCount( int adapter ) const
{
	return g_pShaderDeviceMgr->GetModeCount( adapter );
}


//-----------------------------------------------------------------------------
// Compatability function, should go away eventually
//-----------------------------------------------------------------------------
static void ConvertModeStruct( ShaderDeviceInfo_t *pMode, const MaterialSystem_Config_t &config ) 
{
	pMode->m_DisplayMode.m_nWidth = config.m_VideoMode.m_Width;					
	pMode->m_DisplayMode.m_nHeight = config.m_VideoMode.m_Height;
	pMode->m_DisplayMode.m_Format = config.m_VideoMode.m_Format;			
	pMode->m_DisplayMode.m_nRefreshRateNumerator = config.m_VideoMode.m_RefreshRate;	
	pMode->m_DisplayMode.m_nRefreshRateDenominator = config.m_VideoMode.m_RefreshRate ? 1 : 0;	
	pMode->m_nBackBufferCount = 1;			
	pMode->m_nAASamples = config.m_nAASamples;
	pMode->m_nAAQuality = config.m_nAAQuality;
	pMode->m_nDXLevel = config.dxSupportLevel;					
	pMode->m_nWindowedSizeLimitWidth = (int)config.m_WindowedSizeLimitWidth;	
	pMode->m_nWindowedSizeLimitHeight = (int)config.m_WindowedSizeLimitHeight;

	pMode->m_bWindowed = config.Windowed();
	pMode->m_bResizing = config.Resizing();			
	pMode->m_bUseStencil = config.Stencil();
	pMode->m_bLimitWindowedSize = config.LimitWindowedSize();	
	pMode->m_bWaitForVSync = config.WaitForVSync();	
	pMode->m_bScaleToOutputResolution = config.ScaleToOutputResolution();
	pMode->m_bUsingMultipleWindows = config.UsingMultipleWindows();
}

static void ConvertModeStruct( MaterialVideoMode_t *pMode, const ShaderDisplayMode_t &info ) 
{
	pMode->m_Width = info.m_nWidth;					
	pMode->m_Height = info.m_nHeight;
	pMode->m_Format = info.m_Format;			
	pMode->m_RefreshRate = info.m_nRefreshRateNumerator / info.m_nRefreshRateDenominator;
}


//-----------------------------------------------------------------------------
// Returns mode information..
//-----------------------------------------------------------------------------
void CMaterialSystem::GetModeInfo( int nAdapter, int nMode, MaterialVideoMode_t& info ) const
{
	ShaderDisplayMode_t shaderInfo;
	g_pShaderDeviceMgr->GetModeInfo( &shaderInfo, nAdapter, nMode );
	ConvertModeStruct( &info, shaderInfo );
}


//-----------------------------------------------------------------------------
// Returns the mode info for the current display device
//-----------------------------------------------------------------------------
void CMaterialSystem::GetDisplayMode( MaterialVideoMode_t& info ) const
{
	ShaderDisplayMode_t shaderInfo;
	g_pShaderDeviceMgr->GetCurrentModeInfo( &shaderInfo, m_nAdapter );
	ConvertModeStruct( &info, shaderInfo );
}


//-----------------------------------------------------------------------------
// Sets the mode...
//-----------------------------------------------------------------------------
bool CMaterialSystem::SetMode( void* hwnd, const MaterialSystem_Config_t &config )
{
	Assert( m_bGeneratedConfig );

	ShaderDeviceInfo_t info;
	ConvertModeStruct( &info, config );

	bool bPreviouslyUsingGraphics = g_pShaderDevice->IsUsingGraphics();
	bool bOk = g_pShaderAPI->SetMode( hwnd, m_nAdapter, info );
	if ( !bOk )
		return false;

	TextureManager()->FreeStandardRenderTargets();
	TextureManager()->AllocateStandardRenderTargets();

	// FIXME: There's gotta be a better way of doing this?
	// After the first mode set, make sure to download any textures created
	// before the first mode set. After the first mode set, all textures
	// will be reloaded via the reaquireresources call. Same goes for procedural materials
	if ( !bPreviouslyUsingGraphics )
	{
		if ( IsPC() )
		{
			TextureManager()->RestoreRenderTargets();
			TextureManager()->RestoreNonRenderTargetTextures();
			if ( MaterialSystem()->CanUseEditorMaterials() )
			{
				// We are in Hammer.  Allocate these here since we aren't going to allocate
				// lightmaps.
				// HACK!
				// NOTE! : Overbright is 1.0 so that Hammer will work properly with the white bumped and unbumped lightmaps.
				MathLib_Init( 2.2f, 2.2f, 0.0f, OVERBRIGHT );
				AllocateStandardTextures();
			}
		}

		if ( IsX360() )
		{
			// shaderapi was not viable at init time, it is now
			TextureManager()->ReloadTextures();
			AllocateStandardTextures();
		}
	}

	g_pShaderDevice->SetHardwareGammaRamp( config.m_fMonitorGamma, config.m_fGammaTVRangeMin, config.m_fGammaTVRangeMax, 
		config.m_fGammaTVExponent, config.m_bGammaTVEnabled );

	// Copy over that state which isn't stored currently in convars
	g_config.m_VideoMode = config.m_VideoMode;
	g_config.SetFlag( MATSYS_VIDCFG_FLAGS_WINDOWED, config.Windowed() );
	g_config.SetFlag( MATSYS_VIDCFG_FLAGS_STENCIL, config.Stencil() );
	WriteConfigIntoConVars( config );

	extern void SetupDirtyDiskReportFunc(); 
	SetupDirtyDiskReportFunc();

	return true;
}

// Creates/ destroys a child window
bool CMaterialSystem::AddView( void* hwnd )
{
	return g_pShaderDevice->AddView( hwnd );
}

void CMaterialSystem::RemoveView( void* hwnd )
{
	g_pShaderDevice->RemoveView( hwnd );
}

// Activates a view
void CMaterialSystem::SetView( void* hwnd )
{
	g_pShaderDevice->SetView( hwnd );
}


//-----------------------------------------------------------------------------
// Installs a function to be called when we need to release vertex buffers
//-----------------------------------------------------------------------------
void CMaterialSystem::AddReleaseFunc( MaterialBufferReleaseFunc_t func )
{
	// Shouldn't have two copies in our list
	Assert( m_ReleaseFunc.Find( func ) == -1 );
	m_ReleaseFunc.AddToTail( func );
}

void CMaterialSystem::RemoveReleaseFunc( MaterialBufferReleaseFunc_t func )
{
	int i = m_ReleaseFunc.Find( func );
	if( i != -1 )
		m_ReleaseFunc.Remove(i);
}


//-----------------------------------------------------------------------------
// Installs a function to be called when we need to restore vertex buffers
//-----------------------------------------------------------------------------
void CMaterialSystem::AddRestoreFunc( MaterialBufferRestoreFunc_t func )
{
	// Shouldn't have two copies in our list
	Assert( m_RestoreFunc.Find( func ) == -1 );
	m_RestoreFunc.AddToTail( func );
}

void CMaterialSystem::RemoveRestoreFunc( MaterialBufferRestoreFunc_t func )
{
	int i = m_RestoreFunc.Find( func );
	Assert( i != -1 );
	m_RestoreFunc.Remove(i);
}


//-----------------------------------------------------------------------------
// Called by the shader API when it's just about to lose video memory
//-----------------------------------------------------------------------------
void CMaterialSystem::ReleaseShaderObjects()
{
	if( mat_debugalttab.GetBool() )
	{
		Warning( "mat_debugalttab: CMaterialSystem::ReleaseShaderObjects\n" );
	}

	m_HardwareRenderContext.OnReleaseShaderObjects();

	g_pOcclusionQueryMgr->FreeOcclusionQueryObjects();
	TextureManager()->ReleaseTextures();
	ReleaseStandardTextures();
	GetLightmaps()->ReleaseLightmapPages();
	for (int i = 0; i < m_ReleaseFunc.Count(); ++i)
	{
		m_ReleaseFunc[i]();
	}
}

void CMaterialSystem::RestoreShaderObjects( CreateInterfaceFn shaderFactory, int nChangeFlags )
{
	if ( shaderFactory )
	{
		g_pShaderAPI = (IShaderAPI*)shaderFactory( SHADERAPI_INTERFACE_VERSION, NULL );
		g_pShaderDevice = (IShaderDevice*)shaderFactory( SHADER_DEVICE_INTERFACE_VERSION, NULL );
		g_pShaderShadow = (IShaderShadow*)shaderFactory( SHADERSHADOW_INTERFACE_VERSION, NULL );
	}

	for( MaterialHandle_t i = m_MaterialDict.FirstMaterial(); i != m_MaterialDict.InvalidMaterial(); i = m_MaterialDict.NextMaterial( i ) )
	{
		IMaterialInternal *pMat = m_MaterialDict.GetMaterialInternal( i );
		if ( pMat )
			pMat->ReportVarChanged( NULL );
	}


	if ( mat_debugalttab.GetBool() )
	{
		Warning( "mat_debugalttab: CMaterialSystem::RestoreShaderObjects\n" );
	}
	// Shader API sets this to the max value the card supports when it resets
	// the state, so restore this value.
	g_pShaderAPI->SetAnisotropicLevel( GetCurrentConfigForVideoCard().m_nForceAnisotropicLevel );

	// NOTE: render targets must be restored first, then vb/ibs, then managed textures
	// FIXME: Gotta restore lightmap pages + standard textures before restore funcs are called
	// because they use them both.
	TextureManager()->RestoreRenderTargets();
	AllocateStandardTextures();
	GetLightmaps()->RestoreLightmapPages();
	g_pOcclusionQueryMgr->AllocOcclusionQueryObjects();
	for (int i = 0; i < m_RestoreFunc.Count(); ++i)
	{
		m_RestoreFunc[i]( nChangeFlags );
	}
	TextureManager()->RestoreNonRenderTargetTextures( );
}


//-----------------------------------------------------------------------------
// Use this to spew information about the 3D layer 
//-----------------------------------------------------------------------------
void CMaterialSystem::SpewDriverInfo() const
{
	Warning( "ShaderAPI: %s\n", m_pShaderDLL );
	g_pShaderDevice->SpewDriverInfo();
}


//-----------------------------------------------------------------------------
// Color converting blitter
//-----------------------------------------------------------------------------

bool CMaterialSystem::ConvertImageFormat( unsigned char *src, enum ImageFormat srcImageFormat,
						unsigned char *dst, enum ImageFormat dstImageFormat, 
						int width, int height, int srcStride, int dstStride )
{
	return ImageLoader::ConvertImageFormat( src, srcImageFormat, 
		dst, dstImageFormat, width, height, srcStride, dstStride );
}

//-----------------------------------------------------------------------------
// Figures out the amount of memory needed by a bitmap
//-----------------------------------------------------------------------------
int CMaterialSystem::GetMemRequired( int width, int height, int depth, ImageFormat format, bool mipmap )
{
	return ImageLoader::GetMemRequired( width, height, depth, format, mipmap );
}


//-----------------------------------------------------------------------------
// Method to allow clients access to the MaterialSystem_Config
//-----------------------------------------------------------------------------
MaterialSystem_Config_t& CMaterialSystem::GetConfig()
{
	Assert( m_bGeneratedConfig );
	return g_config;
}


//-----------------------------------------------------------------------------
// Gets image format info
//-----------------------------------------------------------------------------
ImageFormatInfo_t const& CMaterialSystem::ImageFormatInfo( ImageFormat fmt) const
{
	return ImageLoader::ImageFormatInfo(fmt);
}


//-----------------------------------------------------------------------------
// Reads keyvalues for information
//-----------------------------------------------------------------------------
static void ReadInt( KeyValues *pGroup, const char *pName, int nDefaultVal, int nUndefinedVal, int *pDest )
{
	*pDest = pGroup->GetInt( pName, nDefaultVal );
//	Warning( "\t%s = %d\n", pName, *pDest );
	Assert( *pDest != nUndefinedVal );
}

static void ReadFlt( KeyValues *pGroup, const char *pName, float flDefaultVal, float flUndefinedVal, float *pDest )
{
	*pDest = pGroup->GetFloat( pName, flDefaultVal );
//	Warning( "\t%s = %f\n", pName, *pDest );
	Assert( *pDest != flUndefinedVal );
}

static void LoadFlags( KeyValues *pGroup, const char *pName, bool bDefaultValue, unsigned int nFlag, unsigned int *pFlags )
{
	int nValue = pGroup->GetInt( pName, bDefaultValue ? 1 : 0 );
//	Warning( "\t%s = %s\n", pName, nValue ? "true" : "false" );
	if ( nValue )
	{
		*pFlags |= nFlag;
	}
}


//-----------------------------------------------------------------------------
// This is called when the config changes
//-----------------------------------------------------------------------------
void CMaterialSystem::GenerateConfigFromConfigKeyValues( MaterialSystem_Config_t *pConfig, bool bOverwriteCommandLineValues )
{
	// Look for the default recommended dx support level
	MaterialAdapterInfo_t adapterInfo;
	g_pShaderDeviceMgr->GetAdapterInfo( m_nAdapter, adapterInfo );

	pConfig->dxSupportLevel = adapterInfo.m_nDXSupportLevel;
	KeyValues *pKeyValues = new KeyValues( "config" );
	if ( !GetRecommendedConfigurationInfo( pConfig->dxSupportLevel, pKeyValues ) )
	{
		pKeyValues->deleteThis();
		return;
	}

	pConfig->m_Flags = 0;

	ReadInt( pKeyValues, "DefaultRes", 640, -1, &pConfig->m_VideoMode.m_Width );
	pConfig->m_VideoMode.m_Height = ( pConfig->m_VideoMode.m_Width * 3 ) / 4;

#if defined( _X360 )
	pConfig->m_VideoMode.m_Width = GetSystemMetrics( SM_CXSCREEN );
	pConfig->m_VideoMode.m_Height = GetSystemMetrics( SM_CYSCREEN );
#endif

	pKeyValues->deleteThis();

	WriteConfigurationInfoToConVars( bOverwriteCommandLineValues );
	m_bGeneratedConfig = true;
}


//-----------------------------------------------------------------------------
// If mat_proxy goes to 0, we need to reload all materials, because their shader
// params might have been messed with.
//-----------------------------------------------------------------------------
static void MatProxyCallback( IConVar *pConVar, const char *old, float flOldValue )
{
	ConVarRef var( pConVar );
	int oldVal = (int)flOldValue;
	if ( var.GetInt() == 0 && oldVal != 0 )
	{
		g_MaterialSystem.ReloadMaterials();
	}
}


//-----------------------------------------------------------------------------
// Convars that control the config record
//-----------------------------------------------------------------------------
static ConVar mat_vsync(			"mat_vsync", "0", 0, "Force sync to vertical retrace", true, 0.0, true, 1.0 );
static ConVar mat_forcehardwaresync( "mat_forcehardwaresync", IsPC() ? "1" : "0" );

// Texture-related
static ConVar mat_trilinear(		"mat_trilinear", "0" );
static ConVar mat_forceaniso(		"mat_forceaniso", "1" );
static ConVar mat_filterlightmaps(	"mat_filterlightmaps", "1" );
static ConVar mat_filtertextures(	"mat_filtertextures", "1" );
static ConVar mat_mipmaptextures(	"mat_mipmaptextures", "1" );

static void mat_showmiplevels_Callback_f( IConVar *var, const char *pOldValue, float flOldValue )
{
	// turn off texture filtering if we are showing mip levels.
	mat_filtertextures.SetValue( ( ( ConVar * )var )->GetInt() == 0 );
}
// Debugging textures
static ConVar mat_showmiplevels(	"mat_showmiplevels", "0", 0, "color-code miplevels 2: normalmaps, 1: everything else", mat_showmiplevels_Callback_f );

static ConVar mat_specular(			"mat_specular", "1", 0, "Enable/Disable specularity for perf testing.  Will cause a material reload upon change." );
static ConVar mat_bumpmap(			"mat_bumpmap", "1" );
static ConVar mat_parallaxmap(		"mat_parallaxmap", "1" );
static ConVar mat_reducefillrate(	"mat_reducefillrate", "0" );
static ConVar mat_dxlevel(			"mat_dxlevel", "0" );
static ConVar mat_picmip(			"mat_picmip", "0", FCVAR_NONE, "", true, -10, true, 4 );
static ConVar mat_slopescaledepthbias_normal( "mat_slopescaledepthbias_normal", "0.0f", FCVAR_CHEAT );
static ConVar mat_depthbias_normal( "mat_depthbias_normal", "0.0f", FCVAR_CHEAT );
static ConVar mat_slopescaledepthbias_decal( "mat_slopescaledepthbias_decal", "-0.5", FCVAR_CHEAT );		// Reciprocals of these biases sent to API
static ConVar mat_depthbias_decal(	"mat_depthbias_decal", "-262144", FCVAR_CHEAT  );						//

static ConVar mat_slopescaledepthbias_shadowmap( "mat_slopescaledepthbias_shadowmap", "16", FCVAR_CHEAT );
static ConVar mat_depthbias_shadowmap(	"mat_depthbias_shadowmap", "0.0005", FCVAR_CHEAT  );

static ConVar mat_monitorgamma(		"mat_monitorgamma", "2.2", 0, "monitor gamma (typically 2.2 for CRT and 1.7 for LCD)", true, 1.6f, true, 2.6f  );
static ConVar mat_monitorgamma_tv_range_min( "mat_monitorgamma_tv_range_min", "16" );
static ConVar mat_monitorgamma_tv_range_max( "mat_monitorgamma_tv_range_max", "255" );
// TV's generally have a 2.5 gamma, so we need to convert our 2.2 frame buffer into a 2.5 frame buffer for display on a TV
static ConVar mat_monitorgamma_tv_exp( "mat_monitorgamma_tv_exp", "2.5", 0, "", true, 1.0f, true, 4.0f );
#ifdef _X360
static ConVar mat_monitorgamma_tv_enabled( "mat_monitorgamma_tv_enabled", "1", FCVAR_ARCHIVE, "" );
#else
static ConVar mat_monitorgamma_tv_enabled( "mat_monitorgamma_tv_enabled", "0", FCVAR_ARCHIVE, "" );
#endif

static ConVar mat_antialias(		"mat_antialias", "0" );
static ConVar mat_aaquality(		"mat_aaquality", "0" );
static ConVar mat_diffuse(			"mat_diffuse", "1" );
static ConVar mat_showlowresimage(	"mat_showlowresimage", "0" );
static ConVar mat_fullbright(		"mat_fullbright","0", FCVAR_CHEAT );
static ConVar mat_normalmaps(		"mat_normalmaps", "0", FCVAR_CHEAT );
static ConVar mat_measurefillrate(	"mat_measurefillrate", "0", FCVAR_CHEAT );
static ConVar mat_fillrate(			"mat_fillrate", "0", FCVAR_CHEAT );
static ConVar mat_reversedepth(		"mat_reversedepth", "0", FCVAR_CHEAT );
static ConVar mat_bufferprimitives( "mat_bufferprimitives", "1" );
static ConVar mat_drawflat(			"mat_drawflat","0", FCVAR_CHEAT );
static ConVar mat_softwarelighting( "mat_softwarelighting", "0" );
static ConVar mat_proxy(			"mat_proxy", "0", FCVAR_CHEAT, "", MatProxyCallback );
static ConVar mat_norendering(		"mat_norendering", "0", FCVAR_CHEAT );
static ConVar mat_compressedtextures(  "mat_compressedtextures", "1" );
static ConVar mat_fastspecular(		"mat_fastspecular", "1", 0, "Enable/Disable specularity for visual testing.  Will not reload materials and will not affect perf." );
static ConVar mat_fastnobump(		"mat_fastnobump", "0", FCVAR_CHEAT ); // Binds 1-texel normal map for quick internal testing

// These are not controlled by the material system, but are limited by settings in the material system
static ConVar r_shadowrendertotexture(		"r_shadowrendertotexture", "0" );
static ConVar r_flashlightdepthtexture(		"r_flashlightdepthtexture", "1" );
#ifndef _X360
static ConVar r_waterforceexpensive(		"r_waterforceexpensive", "0" );
#endif
static ConVar r_waterforcereflectentities(	"r_waterforcereflectentities", "0" );
static ConVar mat_motion_blur_enabled( "mat_motion_blur_enabled", "1" );


uint32 g_nDebugVarsSignature = 0;


//-----------------------------------------------------------------------------
// This is called when the config changes
//-----------------------------------------------------------------------------
void CMaterialSystem::ReadConfigFromConVars( MaterialSystem_Config_t *pConfig )
{
	if ( !g_pCVar )
		return;

	// video panel config items
	pConfig->SetFlag( MATSYS_VIDCFG_FLAGS_NO_WAIT_FOR_VSYNC, !mat_vsync.GetBool() );
	pConfig->SetFlag( MATSYS_VIDCFG_FLAGS_FORCE_TRILINEAR, mat_trilinear.GetBool() );
	pConfig->SetFlag( MATSYS_VIDCFG_FLAGS_DISABLE_SPECULAR, !mat_specular.GetBool() );
	pConfig->SetFlag( MATSYS_VIDCFG_FLAGS_DISABLE_BUMPMAP, !mat_bumpmap.GetBool() );
	pConfig->SetFlag( MATSYS_VIDCFG_FLAGS_ENABLE_PARALLAX_MAPPING, mat_parallaxmap.GetBool() );
	pConfig->SetFlag( MATSYS_VIDCFG_FLAGS_REDUCE_FILLRATE, mat_reducefillrate.GetBool() );
	pConfig->m_nForceAnisotropicLevel = max( mat_forceaniso.GetInt(), 1 );
	pConfig->dxSupportLevel = mat_dxlevel.GetInt();
	pConfig->skipMipLevels = mat_picmip.GetInt();
	pConfig->SetFlag( MATSYS_VIDCFG_FLAGS_FORCE_HWSYNC, mat_forcehardwaresync.GetBool() );
	pConfig->m_SlopeScaleDepthBias_Decal = mat_slopescaledepthbias_decal.GetFloat();
	pConfig->m_DepthBias_Decal = mat_depthbias_decal.GetFloat();
	pConfig->m_SlopeScaleDepthBias_Normal = mat_slopescaledepthbias_normal.GetFloat();
	pConfig->m_DepthBias_Normal = mat_depthbias_normal.GetFloat();
	pConfig->m_SlopeScaleDepthBias_ShadowMap = mat_slopescaledepthbias_shadowmap.GetFloat();
	pConfig->m_DepthBias_ShadowMap = mat_depthbias_shadowmap.GetFloat();

	pConfig->m_fMonitorGamma = mat_monitorgamma.GetFloat();
	pConfig->m_fGammaTVRangeMin = mat_monitorgamma_tv_range_min.GetFloat();
	pConfig->m_fGammaTVRangeMax = mat_monitorgamma_tv_range_max.GetFloat();
	pConfig->m_fGammaTVExponent = mat_monitorgamma_tv_exp.GetFloat();
	pConfig->m_bGammaTVEnabled = mat_monitorgamma_tv_enabled.GetBool();

	pConfig->m_nAASamples = mat_antialias.GetInt();
	pConfig->m_nAAQuality = mat_aaquality.GetInt();
	pConfig->bShowDiffuse = mat_diffuse.GetInt() ? true : false;	
//	pConfig->bAllowCheats = false; // hack
	pConfig->bShowNormalMap = mat_normalmaps.GetInt() ? true : false;
	pConfig->bShowLowResImage = mat_showlowresimage.GetInt() ? true : false;
	pConfig->bMeasureFillRate = mat_measurefillrate.GetInt() ? true : false;
	pConfig->bVisualizeFillRate = mat_fillrate.GetInt() ? true : false;
	pConfig->bFilterLightmaps = mat_filterlightmaps.GetInt() ? true : false;
	pConfig->bFilterTextures = mat_filtertextures.GetInt() ? true : false;
	pConfig->bMipMapTextures = mat_mipmaptextures.GetInt() ? true : false;
	pConfig->nShowMipLevels = mat_showmiplevels.GetInt();
	pConfig->bReverseDepth = mat_reversedepth.GetInt() ? true : false;
	pConfig->bBufferPrimitives = mat_bufferprimitives.GetInt() ? true : false;
	pConfig->bDrawFlat = mat_drawflat.GetInt() ? true : false;
	pConfig->bSoftwareLighting = mat_softwarelighting.GetInt() ? true : false;
	pConfig->proxiesTestMode = mat_proxy.GetInt();
	pConfig->m_bSuppressRendering = mat_norendering.GetInt() != 0;
	pConfig->bCompressedTextures = mat_compressedtextures.GetBool();
	pConfig->bShowSpecular = mat_fastspecular.GetInt() ? true : false;
	pConfig->nFullbright = mat_fullbright.GetInt();
	pConfig->m_bFastNoBump = mat_fastnobump.GetInt() != 0;
	pConfig->m_bMotionBlur = mat_motion_blur_enabled.GetBool();
	pConfig->m_bSupportFlashlight = mat_supportflashlight.GetInt() != 0;
	pConfig->m_bShadowDepthTexture = r_flashlightdepthtexture.GetBool();

	pConfig->SetFlag( MATSYS_VIDCFG_FLAGS_ENABLE_HDR, HardwareConfig() && HardwareConfig()->GetHDREnabled() );

	// Render-to-texture shadows are disabled for dxlevel 70 because of material issues
	if ( pConfig->dxSupportLevel < 80 )
	{
		r_shadowrendertotexture.SetValue( 0 );
#ifndef _X360
		r_waterforceexpensive.SetValue( 0 );
#endif
		r_waterforcereflectentities.SetValue( 0 );
	}
	if ( pConfig->dxSupportLevel < 90 )
	{
		r_flashlightdepthtexture.SetValue( 0 );
		mat_motion_blur_enabled.SetValue( 0 );
		pConfig->m_bShadowDepthTexture = false;
		pConfig->m_bMotionBlur = false;
		pConfig->SetFlag( MATSYS_VIDCFG_FLAGS_ENABLE_HDR, false );
	}
}


//-----------------------------------------------------------------------------
// Was the convar specified on the command-line? 
//-----------------------------------------------------------------------------
static bool WasConVarSpecifiedOnCommandLine( const char *pConfigName )
{
	// mat_dxlevel cannot be used on the command-line. Use -dxlevel instead.
	if ( !Q_stricmp( pConfigName, "mat_dxlevel" ) )
		return false;

	return ( g_pCVar->GetCommandLineValue( pConfigName ) != NULL);
}


static const char *pConvarsAllowedInDXSupport[]={
	"cl_detaildist",
	"cl_detailfade",
	"cl_ejectbrass",
	"dsp_off",
	"dsp_slow_cpu",
	"mat_antialias",
	"mat_aaquality",
	"mat_bumpmap",
	"mat_colorcorrection",
	"mat_depthbias_decal",
	"mat_depthbias_normal",
	"mat_disable_ps_patch",
	"mat_forceaniso",
	"mat_forcehardwaresync",
	"mat_forcemanagedtextureintohardware",
	"mat_hdr_level",
	"mat_parallaxmap",
	"mat_picmip",
	"mat_reducefillrate",
	"mat_slopescaledepthbias_decal",
	"mat_slopescaledepthbias_normal",
	"mat_softwarelighting",
	"mat_specular",
	"mat_trilinear",
	"mat_vsync",
	"props_break_max_pieces",
	"r_VehicleViewDampen",
	"r_decal_cullsize",
	"r_dopixelvisibility",
	"r_drawdetailprops",
	"r_drawflecks",
	"r_drawmodeldecals",
	"r_dynamic",
	"r_lightcache_zbuffercache",
	"r_fastzreject",
	"r_overlayfademax",
	"r_overlayfademin",
	"r_rootlod",
	"r_screenfademaxsize",
	"r_screenfademinsize",
	"r_shadowrendertotexture",
	"r_shadows",
	"r_waterforceexpensive",
	"r_waterforcereflectentities",
	"sv_alternateticks",
	"mat_dxlevel",
	"mat_fallbackEyeRefract20",
	"r_shader_srgb",
	"mat_motion_blur_enabled",
	"r_flashlightdepthtexture",
	"mat_disablehwmorph",
	"r_portal_stencil_depth",
	"cl_blobbyshadows",
	"r_flex",
	"r_drawropes",
	"props_break_max_pieces",
	"cl_ragdoll_fade_time",
	"cl_ragdoll_forcefade",
	"tf_impactwatertimeenable",
	"fx_drawimpactdebris",
	"fx_drawimpactdust",
	"fx_drawmetalspark",
	"mem_min_heapsize",
	"mem_max_heapsize",
	"mem_max_heapsize_dedicated",
	"snd_spatialize_roundrobin",
	"snd_cull_duplicates",
	"cl_particle_retire_cost"
};

//-----------------------------------------------------------------------------
// Write dxsupport info to configvars 
//-----------------------------------------------------------------------------
void CMaterialSystem::WriteConfigurationInfoToConVars( bool bOverwriteCommandLineValues )
{
	if ( !g_pCVar )
		return;

	KeyValues *pKeyValues = new KeyValues( "config" );
	if ( !GetRecommendedConfigurationInfo( g_config.dxSupportLevel, pKeyValues ) )
	{
		pKeyValues->deleteThis();
		return;
	}

	for( KeyValues *pKey = pKeyValues->GetFirstSubKey(); pKey; pKey = pKey->GetNextKey() )
	{
		const char *pConfigName = pKey->GetName();
		if ( Q_strnicmp( pConfigName, "convar.", 7 ))
			continue;

		pConfigName += 7;
		// check if legal
		bool bLegalVar = false;
		for(int i=0; i< NELEMS( pConvarsAllowedInDXSupport ) ; i++)
		{
			if (! stricmp( pConvarsAllowedInDXSupport[i], pConfigName ) )
			{
				bLegalVar = true;
				break;
			}
		}
		if (! bLegalVar )
		{
			Msg(" Bad convar found in dxsupport - %s\n", pConfigName );
			continue;
		}

		if ( bOverwriteCommandLineValues || !WasConVarSpecifiedOnCommandLine( pConfigName ) )
		{
			ConVar *pConVar = g_pCVar->FindVar( pConfigName );
			if ( !pConVar )
			{
				// NOTE: This is essential for dealing with convars that
				// are not specified in either the app that uses the materialsystem
				// or the materialsystem itself
				
				// Yes, this causes a memory leak. Too bad!
				int nLen = Q_strlen( pConfigName ) + 1;
				char *pString = new char[nLen];
				Q_strncpy( pString, pConfigName, nLen );

				// Actually, we need two memory leaks, or we lose the default string.
				int nDefaultLen = Q_strlen( pKey->GetString() ) + 1;
				char *pDefaultString = new char[nDefaultLen];
				Q_strncpy( pDefaultString, pKey->GetString(), nDefaultLen );

				pConVar = new ConVar( pString, pDefaultString );
			}
			pConVar->SetValue( pKey->GetString() );
		}
	}

	pKeyValues->deleteThis();
}


//-----------------------------------------------------------------------------
// This is called when the config changes
//-----------------------------------------------------------------------------
void CMaterialSystem::WriteConfigIntoConVars( const MaterialSystem_Config_t &config )
{
	if ( !g_pCVar )
		return;

	mat_vsync.SetValue( config.WaitForVSync() );
	mat_trilinear.SetValue( config.ForceTrilinear() );
	mat_specular.SetValue( config.UseSpecular() );
	mat_bumpmap.SetValue( config.UseBumpmapping() );
	mat_parallaxmap.SetValue( config.UseParallaxMapping() );
	mat_reducefillrate.SetValue( config.ReduceFillrate() );
	mat_forceaniso.SetValue( config.m_nForceAnisotropicLevel );
	mat_dxlevel.SetValue( config.dxSupportLevel );
	mat_picmip.SetValue( config.skipMipLevels );
	mat_forcehardwaresync.SetValue( config.ForceHWSync() );
	mat_slopescaledepthbias_normal.SetValue( config.m_SlopeScaleDepthBias_Normal );
	mat_depthbias_normal.SetValue( config.m_DepthBias_Normal );
	mat_slopescaledepthbias_decal.SetValue( config.m_SlopeScaleDepthBias_Decal );
	mat_depthbias_decal.SetValue( config.m_DepthBias_Decal );
	mat_slopescaledepthbias_shadowmap.SetValue( config.m_SlopeScaleDepthBias_ShadowMap );
	mat_depthbias_shadowmap.SetValue( config.m_DepthBias_ShadowMap );

	mat_monitorgamma.SetValue( config.m_fMonitorGamma );
	mat_monitorgamma_tv_range_min.SetValue( config.m_fGammaTVRangeMin );
	mat_monitorgamma_tv_range_max.SetValue( config.m_fGammaTVRangeMax );
	mat_monitorgamma_tv_exp.SetValue( config.m_fGammaTVExponent );
	mat_monitorgamma_tv_enabled.SetValue( config.m_bGammaTVEnabled );

	mat_antialias.SetValue( config.m_nAASamples );
	mat_aaquality.SetValue( config.m_nAAQuality );
	mat_diffuse.SetValue( config.bShowDiffuse ? 1 : 0 );	
//	config.bAllowCheats = false; // hack
	mat_normalmaps.SetValue( config.bShowNormalMap ? 1 : 0 );
	mat_showlowresimage.SetValue( config.bShowLowResImage ? 1 : 0 );
	mat_measurefillrate.SetValue( config.bMeasureFillRate ? 1 : 0 );
	mat_fillrate.SetValue( config.bVisualizeFillRate ? 1 : 0 );
	mat_filterlightmaps.SetValue( config.bFilterLightmaps ? 1 : 0 );
	mat_filtertextures.SetValue( config.bFilterTextures ? 1 : 0 );
	mat_mipmaptextures.SetValue( config.bMipMapTextures ? 1 : 0 );
	mat_showmiplevels.SetValue( config.nShowMipLevels );
	mat_reversedepth.SetValue( config.bReverseDepth ? 1 : 0 );
	mat_bufferprimitives.SetValue( config.bBufferPrimitives ? 1 : 0 );
	mat_drawflat.SetValue( config.bDrawFlat ? 1 : 0 );
	mat_softwarelighting.SetValue( config.bSoftwareLighting ? 1 : 0 );
	mat_proxy.SetValue( config.proxiesTestMode );
	mat_norendering.SetValue( config.m_bSuppressRendering ? 1 : 0 );
	mat_compressedtextures.SetValue( config.bCompressedTextures ? 1 : 0 );
	mat_fastspecular.SetValue( config.bShowSpecular ? 1 : 0 );
	mat_fullbright.SetValue( config.nFullbright );
	mat_fastnobump.SetValue( config.m_bFastNoBump ? 1 : 0 );
	HardwareConfig()->SetHDREnabled( config.HDREnabled() );
	r_flashlightdepthtexture.SetValue( config.m_bShadowDepthTexture ? 1 : 0 );
	mat_motion_blur_enabled.SetValue( config.m_bMotionBlur ? 1 : 0 );
	mat_supportflashlight.SetValue( config.m_bSupportFlashlight ? 1 : 0 );
}


//-----------------------------------------------------------------------------
// This is called constantly to catch for config changes
//-----------------------------------------------------------------------------
bool CMaterialSystem::OverrideConfig( const MaterialSystem_Config_t &_config, bool forceUpdate )
{
	Assert( m_bGeneratedConfig );
	if ( memcmp( &_config, &g_config, sizeof(_config) ) == 0 )
	{
		return false;
	}

	MaterialLock_t hLock = Lock();
	MaterialSystem_Config_t config = _config;

	bool bRedownloadLightmaps = false;
	bool bRedownloadTextures = false;
    bool recomputeSnapshots = false;
	bool dxSupportLevelChanged = false;
	bool bReloadMaterials = false;
	bool bResetAnisotropy = false;
	bool bSetStandardVertexShaderConstants = false;
	bool bMonitorGammaChanged = false;
	bool bVideoModeChange = false;
	bool bResetTextureFilter = false;
	bool bForceAltTab = false;

	// internal config settings
#ifndef _X360
	MaterialSystem_Config_Internal_t config_internal;
	config_internal.r_waterforceexpensive = r_waterforceexpensive.GetInt();
#endif

	if ( !g_pShaderDevice->IsUsingGraphics() )
	{
		g_config = config;
#ifndef _X360
		g_config_internal = config_internal;
#endif

		// Shouldn't call this more than once.
		ColorSpace::SetGamma( 2.2f, 2.2f, OVERBRIGHT, g_config.bAllowCheats, false );
		Unlock( hLock );
		return bRedownloadLightmaps;
	}

	// set the default state since we might be changing the number of
	// texture units, etc. (i.e. we don't want to leave unit 2 in overbright mode
	// if it isn't going to be reset upon each SetDefaultState because there is
	// effectively only one texture unit.)
	g_pShaderAPI->SetDefaultState();
	
	// toggle dx emulation level
	if ( config.dxSupportLevel != g_config.dxSupportLevel )
	{
		if ( mat_debugalttab.GetBool() )
		{
			Warning( "mat_debugalttab: Setting dxSupportLevelChanged, bResetAnisotropy, and bReloadMaterials because new dxlevel = %d and old dxlevel = %d\n",
				( int )config.dxSupportLevel, g_config.dxSupportLevel );
		}
		dxSupportLevelChanged = true;
		bResetAnisotropy = true;

		// Necessary for DXSupportLevelChanged to work 
		g_config.dxSupportLevel = config.dxSupportLevel;

		// This will reset config to match whatever the dxlevel wants
		// and slam to convars to match
		g_pShaderAPI->DXSupportLevelChanged( );
		WriteConfigurationInfoToConVars();
		ReadConfigFromConVars( &config );
		bReloadMaterials = true;
	}

	if ( config.HDREnabled() != g_config.HDREnabled() )
	{
		if ( mat_debugalttab.GetBool() )
		{
			Warning( "mat_debugalttab: Setting forceUpdate, bReloadMaterials, and bForceAltTab because new hdr level = %d and old hdr level = %d\n",
				( int )config.HDREnabled(), g_config.HDREnabled() );
		}

		forceUpdate = true;
		bReloadMaterials = true;
		bForceAltTab = true;
	}

	if ( config.ShadowDepthTexture() != g_config.ShadowDepthTexture() )
	{
		if ( mat_debugalttab.GetBool() )
		{
			Warning( "mat_debugalttab: Setting forceUpdate, bReloadMaterials and recomputeSnapshots (ShadowDepthTexture changed: %d -> %d)\n",
				g_config.ShadowDepthTexture() ? 1 : 0, config.ShadowDepthTexture() ? 1 : 0 );
		}

		forceUpdate = true;
		bReloadMaterials = true;
		recomputeSnapshots = true;
	}

	// Don't use compressed textures for the moment if we don't support them
	if ( HardwareConfig() && !HardwareConfig()->SupportsCompressedTextures() )
	{
		config.bCompressedTextures = false;
	}

	if ( forceUpdate )
	{
		if ( mat_debugalttab.GetBool() )
		{
			Warning( "mat_debugalttab: forceUpdate is true, therefore setting recomputeSnapshots, bRedownloadLightmaps, bRedownloadTextures, bResetAnisotropy, and bSetStandardVertexShaderConstants\n" );
		}
		GetLightmaps()->EnableLightmapFiltering( config.bFilterLightmaps );
		recomputeSnapshots = true;
		bRedownloadLightmaps = true;
		bRedownloadTextures = true;
		bResetAnisotropy = true;
		bSetStandardVertexShaderConstants = true;
	}

	// toggle bump mapping
	if ( config.UseBumpmapping() != g_config.UseBumpmapping() )
	{
		if( mat_debugalttab.GetBool() )
		{
			Warning( "mat_debugalttab: forceUpdate is true, therefore setting recomputeSnapshots, bRedownloadLightmaps, bRedownloadTextures, bResetAnisotropy, and bSetStandardVertexShaderConstants\n" );
		}
		recomputeSnapshots = true;
		bReloadMaterials = true;
		bResetAnisotropy = true;
	}
	
	// toggle specularity
	if ( config.UseSpecular() != g_config.UseSpecular() )
	{
		if( mat_debugalttab.GetBool() )
		{
			Warning( "mat_debugalttab: new usespecular=%d, old usespecular=%d, setting recomputeSnapshots, bReloadMaterials, and bResetAnisotropy\n", 
				( int )config.UseSpecular(), ( int )g_config.UseSpecular() );
		}
		recomputeSnapshots = true;
		bReloadMaterials = true;
		bResetAnisotropy = true;
	}
	
	// toggle parallax mapping
	if ( config.UseParallaxMapping() != g_config.UseParallaxMapping() )
	{
		if ( mat_debugalttab.GetBool() )
		{
			Warning( "mat_debugalttab: new UseParallaxMapping=%d, old UseParallaxMapping=%d, setting bReloadMaterials\n",
				( int )config.UseParallaxMapping(), ( int )g_config.UseParallaxMapping() );
		}
		bReloadMaterials = true;
	}
	
	// Reload materials if we want reduced fillrate
	if ( config.ReduceFillrate() != g_config.ReduceFillrate() )
	{
		if ( mat_debugalttab.GetBool() )
		{
			Warning( "mat_debugalttab: new ReduceFillrate=%d, old ReduceFillrate=%d, setting bReloadMaterials\n",
				( int )config.ReduceFillrate(), ( int )g_config.ReduceFillrate() );
		}
		bReloadMaterials = true;
	}

	// toggle reverse depth
	if ( config.bReverseDepth != g_config.bReverseDepth )
	{
		if( mat_debugalttab.GetBool() )
		{
			Warning( "mat_debugalttab: new ReduceFillrate=%d, old ReduceFillrate=%d, setting bReloadMaterials\n",
				( int )config.ReduceFillrate(), ( int )g_config.ReduceFillrate() );
		}
		recomputeSnapshots = true;
		bResetAnisotropy = true;
	}

	// toggle no transparency
	if ( config.bNoTransparency != g_config.bNoTransparency )
	{
		if ( mat_debugalttab.GetBool() )
		{
			Warning( "mat_debugalttab: new bNoTransparency=%d, old bNoTransparency=%d, setting recomputeSnapshots and bResetAnisotropy\n",
				( int )config.bNoTransparency, ( int )g_config.bNoTransparency );
		}
		recomputeSnapshots = true;
		bResetAnisotropy = true;
	}

	// toggle lightmap filtering
	if ( config.bFilterLightmaps != g_config.bFilterLightmaps )
	{
		if ( mat_debugalttab.GetBool() )
		{
			Warning( "mat_debugalttab: new bFilterLightmaps=%d, old bFilterLightmaps=%d, setting EnableLightmapFiltering\n",
				( int )config.bFilterLightmaps, ( int )g_config.bFilterLightmaps );
		}
		GetLightmaps()->EnableLightmapFiltering( config.bFilterLightmaps );
	}
	
	// toggle software lighting
	if ( config.bSoftwareLighting != g_config.bSoftwareLighting )
	{
		if( mat_debugalttab.GetBool() )
		{
			Warning( "mat_debugalttab: new bSoftwareLighting=%d, old bSoftwareLighting=%d, setting bReloadMaterials\n",
				( int )config.bFilterLightmaps, ( int )g_config.bFilterLightmaps );
		}
		bReloadMaterials = true;
	}

#ifndef _X360
	if ( config_internal.r_waterforceexpensive != g_config_internal.r_waterforceexpensive )
	{
		if ( mat_debugalttab.GetBool() )
		{
			Warning( "mat_debugalttab: new r_waterforceexpensive=%d, old r_waterforceexpensive=%d, setting bReloadMaterials\n",
				( int )config_internal.r_waterforceexpensive, ( int )g_config_internal.r_waterforceexpensive );
		}
		bReloadMaterials = true;
	}
#endif

	// generic things that cause us to redownload lightmaps
	if ( config.bAllowCheats != g_config.bAllowCheats )
	{
		if ( mat_debugalttab.GetBool() )
		{
			Warning( "mat_debugalttab: new bAllowCheats=%d, old bAllowCheats=%d, setting bRedownloadLightmaps\n",
				( int )config.bAllowCheats, ( int )g_config.bAllowCheats );
		}
		bRedownloadLightmaps = true;
	}

	// generic things that cause us to redownload textures
	if ( config.bAllowCheats != g_config.bAllowCheats ||
		config.skipMipLevels != g_config.skipMipLevels  ||
		config.nShowMipLevels != g_config.nShowMipLevels  ||
		((config.bCompressedTextures != g_config.bCompressedTextures) && HardwareConfig()->SupportsCompressedTextures())||
		config.bShowLowResImage != g_config.bShowLowResImage 
		)
	{
		if ( mat_debugalttab.GetBool() )
		{
			Warning( "mat_debugalttab: setting bRedownloadTextures, recomputeSnapshots, and bResetAnisotropy\n" );
		}
		bRedownloadTextures = true;
		recomputeSnapshots = true;
		bResetAnisotropy = true;
	}		

	if ( config.ForceTrilinear() != g_config.ForceTrilinear() )
	{
		if ( mat_debugalttab.GetBool() )
		{
			Warning( "mat_debugalttab: new forcetrilinear: %d, old forcetrilinear: %d, setting bResetTextureFilter\n",
				( int )config.ForceTrilinear(), ( int )g_config.ForceTrilinear() );
		}
		bResetTextureFilter = true;
	}

	if ( config.m_nForceAnisotropicLevel != g_config.m_nForceAnisotropicLevel )
	{
		if( mat_debugalttab.GetBool() )
		{
			Warning( "mat_debugalttab: new m_nForceAnisotropicLevel: %d, old m_nForceAnisotropicLevel: %d, setting bResetAnisotropy and bResetTextureFilter\n",
				( int )config.ForceTrilinear(), ( int )g_config.ForceTrilinear() );
		}
		bResetAnisotropy = true;
		bResetTextureFilter = true;
	}

	if ( config.m_fMonitorGamma != g_config.m_fMonitorGamma || config.m_fGammaTVRangeMin != g_config.m_fGammaTVRangeMin ||
		config.m_fGammaTVRangeMax != g_config.m_fGammaTVRangeMax ||	config.m_fGammaTVExponent != g_config.m_fGammaTVExponent ||
		config.m_bGammaTVEnabled != g_config.m_bGammaTVEnabled )
	{
		if( mat_debugalttab.GetBool() )
		{
			Warning( "mat_debugalttab: new monitorgamma: %f, old monitorgamma: %f, setting bMonitorGammaChanged\n",
				config.m_fMonitorGamma, g_config.m_fMonitorGamma );
		}
		bMonitorGammaChanged = true;
	}

	if ( config.m_VideoMode.m_Width != g_config.m_VideoMode.m_Width ||
		config.m_VideoMode.m_Height != g_config.m_VideoMode.m_Height ||
		config.m_VideoMode.m_RefreshRate != g_config.m_VideoMode.m_RefreshRate ||
		config.m_nAASamples != g_config.m_nAASamples ||
		config.m_nAAQuality != g_config.m_nAAQuality ||
		config.Windowed() != g_config.Windowed() ||
		config.Stencil() != g_config.Stencil() )
	{
		if( mat_debugalttab.GetBool() )
		{
			Warning( "mat_debugalttab: video mode changed for one of various reasons\n" );
		}
		bVideoModeChange = true;
	}

	// toggle wait for vsync
	if ( (IsX360() || !config.Windowed()) && (config.WaitForVSync() != g_config.WaitForVSync()) )
	{
#		if ( !defined( _X360 ) )
		{
			if ( mat_debugalttab.GetBool() )
			{
				Warning( "mat_debugalttab: video mode changed due to toggle of wait for vsync\n" );
			}
			bVideoModeChange = true;
		}
#		else
		{
			g_pShaderAPI->EnableVSync_360( config.WaitForVSync() );
		}
#		endif
	}


	g_config = config;
#ifndef _X360
	g_config_internal = config_internal;
#endif

	if ( dxSupportLevelChanged )
	{
		if ( mat_debugalttab.GetBool() )
		{
			Warning( "mat_debugalttab: dx support level changed, clearing snapshots\n" );
		}
		// All snapshots have basically become invalid;
		g_pShaderAPI->ClearSnapshots();
	}

	if ( bRedownloadTextures || bRedownloadLightmaps )
	{
		// Get rid of this?
		ColorSpace::SetGamma( 2.2f, 2.2f, OVERBRIGHT, g_config.bAllowCheats, false );
	}

	// 360 does not support various configuration changes and cannot reload materials
	if ( bReloadMaterials && !IsX360() )
	{
		if ( mat_debugalttab.GetBool() )
		{
			Warning( "mat_debugalttab: ReloadMaterials\n" );
		}
		ReloadMaterials();
	}

	// 360 does not support various configuration changes and cannot reload textures
	// 360 has no reason to reload textures, it's unnecessary and massively expensive
	// 360 does not use this path as an init affect to get its textures into memory
	if ( bRedownloadTextures && !IsX360() )
	{
		if ( mat_debugalttab.GetBool() )
		{
			Warning( "mat_debugalttab: redownloading textures\n" );
		}
		TextureManager()->RestoreRenderTargets();
		TextureManager()->RestoreNonRenderTargetTextures();
	}
	else if ( bResetTextureFilter )
	{
		if( mat_debugalttab.GetBool() )
		{
			Warning( "mat_debugalttab: ResetTextureFilteringState\n" );
		}
		TextureManager()->ResetTextureFilteringState();
	}

	// Recompute all state snapshots
	if ( recomputeSnapshots )
	{
		if( mat_debugalttab.GetBool() )
		{
			Warning( "mat_debugalttab: RecomputeAllStateSnapshots\n" );
		}
		RecomputeAllStateSnapshots();
	}

	if ( bResetAnisotropy )
	{
		if( mat_debugalttab.GetBool() )
		{
			Warning( "mat_debugalttab: SetAnisotropicLevel\n" );
		}
		g_pShaderAPI->SetAnisotropicLevel( config.m_nForceAnisotropicLevel );
	}

	if ( bSetStandardVertexShaderConstants )
	{
		if ( mat_debugalttab.GetBool() )
		{
			Warning( "mat_debugalttab: SetStandardVertexShaderConstants\n" );
		}
		g_pShaderAPI->SetStandardVertexShaderConstants( OVERBRIGHT );
	}

	if ( bMonitorGammaChanged )
	{
		if( mat_debugalttab.GetBool() )
		{
			Warning( "mat_debugalttab: SetHardwareGammaRamp\n" );
		}
		g_pShaderDevice->SetHardwareGammaRamp( config.m_fMonitorGamma, config.m_fGammaTVRangeMin, config.m_fGammaTVRangeMax, 
			config.m_fGammaTVExponent, config.m_bGammaTVEnabled );
	}

	if ( bVideoModeChange )
	{
		if ( mat_debugalttab.GetBool() )
		{
			Warning( "mat_debugalttab: ChangeVideoMode\n" );
		}
		ShaderDeviceInfo_t info;
		ConvertModeStruct( &info, config );
		g_pShaderAPI->ChangeVideoMode( info );
	}

	if ( bForceAltTab )
	{
		// Simulate an Alt-Tab
//		g_pShaderAPI->ReleaseResources();
//		g_pShaderAPI->ReacquireResources();
	}

	Unlock( hLock );
	return bRedownloadLightmaps;
}


//-----------------------------------------------------------------------------
// This is called when the config changes
//-----------------------------------------------------------------------------
bool CMaterialSystem::UpdateConfig( bool forceUpdate )
{
	MaterialSystem_Config_t config = g_config;
	ReadConfigFromConVars( &config );
	return OverrideConfig( config, forceUpdate );
}



void CMaterialSystem::ReleaseResources()
{
	if( mat_debugalttab.GetBool() )
	{
		Warning( "mat_debugalttab: CMaterialSystem::ReleaseResources\n" );
	}
	g_pShaderAPI->FlushBufferedPrimitives();
	g_pShaderDevice->ReleaseResources();
}

void CMaterialSystem::ReacquireResources()
{
	if( mat_debugalttab.GetBool() )
	{
		Warning( "mat_debugalttab: CMaterialSystem::ReacquireResources\n" );
	}
	g_pShaderDevice->ReacquireResources();
}


//-----------------------------------------------------------------------------
// 
//-----------------------------------------------------------------------------
bool CMaterialSystem::OnDrawMesh( IMesh *pMesh, int firstIndex, int numIndices )
{
	if ( IsInStubMode() )
	{
		return false;
	}

	return GetRenderContextInternal()->OnDrawMesh( pMesh, firstIndex, numIndices );
}

bool CMaterialSystem::OnDrawMesh( IMesh *pMesh, CPrimList *pLists, int nLists )
{
	if ( IsInStubMode() )
	{
		return false;
	}

	return GetRenderContextInternal()->OnDrawMesh( pMesh, pLists, nLists );
}

//-----------------------------------------------------------------------------
// Creates a procedural texture
//-----------------------------------------------------------------------------
ITexture *CMaterialSystem::CreateProceduralTexture( 
	const char			*pTextureName, 
	const char			*pTextureGroupName, 
	int					w, 
	int					h, 
	ImageFormat			fmt, 
	int					nFlags )
{
	ITextureInternal* pTex = TextureManager()->CreateProceduralTexture( pTextureName, pTextureGroupName, w, h, 1, fmt, nFlags );
	return pTex;
}

	
//-----------------------------------------------------------------------------
// Create new materials	(currently only used by the editor!)
//-----------------------------------------------------------------------------
IMaterial *CMaterialSystem::CreateMaterial( const char *pMaterialName, KeyValues *pVMTKeyValues )
{
	// For not, just create a material with no default settings
	IMaterialInternal* pMaterial = IMaterialInternal::CreateMaterial( pMaterialName, TEXTURE_GROUP_OTHER, pVMTKeyValues );
	pMaterial->IncrementReferenceCount();
	AddMaterialToMaterialList( pMaterial );
	return pMaterial->GetQueueFriendlyVersion();
}


//-----------------------------------------------------------------------------
// Finds or creates a procedural material
//-----------------------------------------------------------------------------
IMaterial *CMaterialSystem::FindProceduralMaterial( const char *pMaterialName, const char *pTextureGroupName, KeyValues *pVMTKeyValues )
{
	// We need lower-case symbols for this to work
	int nLen = Q_strlen( pMaterialName ) + 1;
	char *pTemp = (char*)stackalloc( nLen );
	Q_strncpy( pTemp, pMaterialName, nLen );
	Q_strlower( pTemp );
	Q_FixSlashes( pTemp, '/' );

	// 'true' causes the search to find procedural materials
	IMaterialInternal *pMaterial = m_MaterialDict.FindMaterial( pTemp, true );
	if ( pMaterial )
	{
		pVMTKeyValues->deleteThis();
	}
	else
	{
		pMaterial = IMaterialInternal::CreateMaterial( pMaterialName, pTextureGroupName, pVMTKeyValues );
		pMaterial->IncrementReferenceCount();
		AddMaterialToMaterialList( static_cast<IMaterialInternal*>( pMaterial ) );
	}

	return pMaterial->GetQueueFriendlyVersion();
}


//-----------------------------------------------------------------------------
// Search by name
//-----------------------------------------------------------------------------
IMaterial* CMaterialSystem::FindMaterial( char const *pMaterialName, const char *pTextureGroupName, bool bComplain, const char *pComplainPrefix )
{
	// Hammer incorrectly puts a slash in front of the material name if it's in the materials\ folder.
	// If we don't get rid of it, we'll get a filename like materials\\blah.vmt and it can cause
	// problems in other systems.
	if ( (pMaterialName[0] == '/' || pMaterialName[0] == '\\') &&
		!(pMaterialName[1] == '/' || pMaterialName[1] == '\\') )
	{
		pMaterialName++;			
	}

	// We need lower-case symbols for this to work
	int nLen = Q_strlen( pMaterialName ) + 1;
	char *pTemp = (char*)stackalloc( nLen );
	Q_StripExtension( pMaterialName, pTemp, nLen );
	Q_strlower( pTemp );
	Q_FixSlashes( pTemp, '/' );
	Assert( nLen >= Q_strlen( pTemp ) + 1 );

	IMaterialInternal *pExistingMaterial = m_MaterialDict.FindMaterial( pTemp, false );	// 'false' causes the search to find only file-created materials

	if ( pExistingMaterial )
		return pExistingMaterial->GetQueueFriendlyVersion();

	// It hasn't been seen yet, so let's check to see if it's in the filesystem.
	nLen = Q_strlen( "materials/" ) + Q_strlen( pTemp ) + Q_strlen( ".vmt" ) + 1;
	char *vmtName = (char *)stackalloc( nLen );

	// Check to see if this is a UNC-specified material name
	bool bIsUNC = pTemp[0] == '/' && pTemp[1] == '/' && pTemp[2] != '/';
	if ( !bIsUNC )
	{
		Q_strncpy( vmtName, "materials/", nLen );
		Q_strncat( vmtName, pTemp, nLen, COPY_ALL_CHARACTERS );
	}
	else
	{
		Q_strncpy( vmtName, pTemp, nLen );
	}

	//Q_strncat( vmtName, ".vmt", nLen, COPY_ALL_CHARACTERS );
	Assert( nLen >= (int)Q_strlen( vmtName ) + 1 );

	CUtlVector<FileNameHandle_t> includes;
	KeyValues *pKeyValues = new KeyValues("vmt");
	KeyValues *pPatchKeyValues = new KeyValues( "vmt_patches" );
	if ( !LoadVMTFile( *pKeyValues, *pPatchKeyValues, vmtName, true, &includes ) )
	{
		pKeyValues->deleteThis();
		pKeyValues = NULL;
		pPatchKeyValues->deleteThis();
		pPatchKeyValues = NULL;
	}
	else
	{
		char *matNameWithExtension;
		nLen = Q_strlen( pTemp ) + Q_strlen( ".vmt" ) + 1;
		matNameWithExtension = (char *)stackalloc( nLen );
		Q_strncpy( matNameWithExtension, pTemp, nLen );
		Q_strncat( matNameWithExtension, ".vmt", nLen, COPY_ALL_CHARACTERS );

		IMaterialInternal *pMat = NULL;
		if ( !Q_stricmp( pKeyValues->GetName(), "subrect" ) )
		{
			pMat = m_MaterialDict.AddMaterialSubRect( matNameWithExtension, pTextureGroupName, pKeyValues, pPatchKeyValues );
		}
		else
		{
			pMat = m_MaterialDict.AddMaterial( matNameWithExtension, pTextureGroupName );
			if ( g_pShaderDevice->IsUsingGraphics() )
			{
				if ( !bIsUNC )
				{
					m_pForcedTextureLoadPathID = "GAME";
				}
				pMat->PrecacheVars( pKeyValues, pPatchKeyValues, &includes );
				m_pForcedTextureLoadPathID = NULL;
			}
		}
		pKeyValues->deleteThis();
		pPatchKeyValues->deleteThis();
		return pMat->GetQueueFriendlyVersion();
	}

	if ( bComplain )
	{
		Assert( pTemp );

		// convert to lowercase
		nLen = Q_strlen(pTemp) + 1 ;
		char *name = (char*)stackalloc( nLen );
		Q_strncpy( name, pTemp, nLen );
		Q_strlower( name );

		if ( m_MaterialDict.NoteMissing( name ) )
		{
			if ( pComplainPrefix )
			{
				DevWarning( "%s", pComplainPrefix );
			}
			DevWarning( "material \"%s\" not found.\n", name );
		}
	}

	return g_pErrorMaterial->GetRealTimeVersion();
}

static char const *TextureAliases[] = 
{
	// this table is only here for backwards compatibility where a render target change was made,
	// and we wish to redirect an existing old client.dll for hl2 to reference this texture. It's
	// not meant as a general texture aliasing system.
	"_rt_FullFrameFB1", "_rt_FullScreen"
};

ITexture *CMaterialSystem::FindTexture( char const *pTextureName, const char *pTextureGroupName, bool bComplain )
{
 	ITextureInternal *pTexture = TextureManager()->FindOrLoadTexture( pTextureName, pTextureGroupName );
	Assert( pTexture );
	if ( pTexture->IsError() )
	{
		if ( IsPC() )
		{
			for ( int i=0; i<NELEMS( TextureAliases ); i+=2 )
			{
				if ( !Q_stricmp( pTextureName, TextureAliases[i] ) )
				{
					return FindTexture( TextureAliases[i+1], pTextureGroupName, bComplain );
				}
			}
		}
		if ( bComplain )
		{
			DevWarning( "Texture '%s' not found.\n", pTextureName );
		}
	}

	return pTexture;
}

bool CMaterialSystem::IsTextureLoaded( char const* pTextureName ) const
{
	return TextureManager()->IsTextureLoaded( pTextureName );
}

void CMaterialSystem::AddTextureAlias( const char *pAlias, const char *pRealName )
{
	TextureManager()->AddTextureAlias( pAlias, pRealName );
}

void CMaterialSystem::RemoveTextureAlias( const char *pAlias )
{
	TextureManager()->RemoveTextureAlias( pAlias );
}

void CMaterialSystem::SetExcludedTextures( const char *pScriptName )
{
	TextureManager()->SetExcludedTextures( pScriptName );
}

void CMaterialSystem::UpdateExcludedTextures( void )
{
	TextureManager()->UpdateExcludedTextures();
	// Have to re-setup the representative textures since they may have been removed out from under us by the queued loader.
	for (MaterialHandle_t i = FirstMaterial(); i != InvalidMaterial(); i = NextMaterial(i) )
	{
		GetMaterialInternal(i)->FindRepresentativeTexture();
		GetMaterialInternal(i)->PrecacheMappingDimensions();
	}
}

//-----------------------------------------------------------------------------
// Recomputes state snapshots for all materials
//-----------------------------------------------------------------------------
void CMaterialSystem::RecomputeAllStateSnapshots()
{
	g_pShaderAPI->ClearSnapshots();
	for (MaterialHandle_t i = FirstMaterial(); i != InvalidMaterial(); i = NextMaterial(i) )
	{
		GetMaterialInternal(i)->RecomputeStateSnapshots();
	}
	g_pShaderAPI->ResetRenderState();
}


//-----------------------------------------------------------------------------
// Uncache all materials
//-----------------------------------------------------------------------------
void CMaterialSystem::UncacheAllMaterials()
{
	MaterialLock_t hLock = Lock();
	Flush( true );

	for ( MaterialHandle_t i = FirstMaterial(); i != InvalidMaterial(); i = NextMaterial( i ) )
	{
		Assert( GetMaterialInternal(i)->GetReferenceCount() >= 0 );
		GetMaterialInternal(i)->Uncache();
	}
	TextureManager()->RemoveUnusedTextures();
	Unlock( hLock );
}


//-----------------------------------------------------------------------------
// Uncache unused materials
//-----------------------------------------------------------------------------
void CMaterialSystem::UncacheUnusedMaterials( bool bRecomputeStateSnapshots )
{
	MaterialLock_t hLock = Lock();
	Flush( true );

	// We need two loops to make sure we don't reset the snapshots if nothing got removed,
	// otherwise the snapshot recomputation is expensive and avoided at load time
	bool bDidUncacheMaterial = false;
	for ( MaterialHandle_t i = FirstMaterial(); i != InvalidMaterial(); i = NextMaterial(i) )
	{
		IMaterialInternal *pMatInternal = GetMaterialInternal( i );
		Assert( pMatInternal->GetReferenceCount() >= 0 );
		if ( pMatInternal->GetReferenceCount() <= 0 )
		{
			bDidUncacheMaterial = true;
			pMatInternal->Uncache();
		}
	}

	if ( IsX360() && bRecomputeStateSnapshots )
	{
		// Always recompute snapshots because the queued loading process skips it during pre-purge,
		// allowing it to happen just once, here.
		bDidUncacheMaterial = true;
	}

	if ( bDidUncacheMaterial && bRecomputeStateSnapshots )
	{
		// Clear the state snapshots since we are going to rebuild all of them.
		g_pShaderAPI->ClearSnapshots();
		g_pShaderAPI->ClearVertexAndPixelShaderRefCounts();

		for ( MaterialHandle_t i = FirstMaterial(); i != InvalidMaterial(); i = NextMaterial(i) )
		{
			IMaterialInternal *pMatInternal = GetMaterialInternal(i);
			if ( pMatInternal->GetReferenceCount() > 0 )
			{
				// Recompute the state snapshots for the materials that we are keeping
				// since we blew all of them away above.
				pMatInternal->RecomputeStateSnapshots();
			}
		}
		g_pShaderAPI->PurgeUnusedVertexAndPixelShaders();
	}

	if ( bRecomputeStateSnapshots )
	{
		// kick out all per material context datas
		for( MaterialHandle_t i = m_MaterialDict.FirstMaterial(); i != m_MaterialDict.InvalidMaterial(); i = m_MaterialDict.NextMaterial( i ) )
		{
			GetMaterialInternal(i)->ClearContextData();
		}
	}

	TextureManager()->RemoveUnusedTextures();
	Unlock( hLock );
}


//-----------------------------------------------------------------------------
// Release temporary HW memory...
//-----------------------------------------------------------------------------
void CMaterialSystem::ResetTempHWMemory( bool bExitingLevel )
{
	g_pShaderAPI->DestroyVertexBuffers( bExitingLevel );
	TextureManager()->ReleaseTempRenderTargetBits();
}


//-----------------------------------------------------------------------------
// Cache used materials
//-----------------------------------------------------------------------------
void CMaterialSystem::CacheUsedMaterials( )
{
	g_pShaderAPI->EvictManagedResources();
	for (MaterialHandle_t i = FirstMaterial(); i != InvalidMaterial(); i = NextMaterial(i) )
	{
		Assert( GetMaterialInternal(i)->GetReferenceCount() >= 0 );
		if( GetMaterialInternal(i)->GetReferenceCount() > 0 )
		{
			GetMaterialInternal(i)->Precache();
		}
	}
	if( mat_forcemanagedtextureintohardware.GetBool() )
	{
		TextureManager()->ForceAllTexturesIntoHardware();
	}
}

//-----------------------------------------------------------------------------
// Reloads textures + materials
//-----------------------------------------------------------------------------
void CMaterialSystem::ReloadTextures( void )
{
	// 360 should not have gotten here
	Assert( !IsX360() );

	TextureManager()->RestoreRenderTargets();
	TextureManager()->RestoreNonRenderTargetTextures();
}

void CMaterialSystem::ReloadMaterials( const char *pSubString )
{
	bool bVertexFormatChanged = false;
	if( pSubString == NULL )
	{
		bVertexFormatChanged = true;
		UncacheAllMaterials();
		CacheUsedMaterials();
	}
	else
	{
		char const chMultiDelim = '*';
		CUtlVector< char > arrSearchSubString;
		CUtlVector< char const * > arrSearchItems;

		if ( strchr( pSubString, chMultiDelim ) )
		{
			arrSearchSubString.SetCount( strlen( pSubString ) + 1 );
			strcpy( arrSearchSubString.Base(), pSubString );
			for ( char * pch = arrSearchSubString.Base(); pch; )
			{
				char *pchEnd = strchr( pch, chMultiDelim );
				pchEnd ? *( pchEnd ++ ) = 0 : 0;
				arrSearchItems.AddToTail( pch );
				pch = pchEnd;
			}
		}

		for (MaterialHandle_t i = FirstMaterial(); i != InvalidMaterial(); i = NextMaterial(i) )
		{
			if( GetMaterialInternal(i)->GetReferenceCount() <= 0 )
				continue;

			char const *szMatName = GetMaterialInternal(i)->GetName();
			
			if ( arrSearchItems.Count() > 1 )
			{
				bool bMatched = false;
				
				for ( int k = 0; !bMatched && ( k < arrSearchItems.Count() ); ++ k )
					if( Q_stristr( szMatName, arrSearchItems[k] ) )
						bMatched = true;
				
				if ( !bMatched )
					continue;
			}
			else
			{
				if( !Q_stristr( szMatName, pSubString ) )
					continue;
			}

			if ( !GetMaterialInternal(i)->IsPrecached() )
			{
				if ( GetMaterialInternal(i)->IsPrecachedVars() )
				{
					GetMaterialInternal(i)->Uncache( );
				}
			}
			else
			{
				VertexFormat_t oldVertexFormat = GetMaterialInternal(i)->GetVertexFormat();
				GetMaterialInternal(i)->Uncache();
				GetMaterialInternal(i)->Precache();
				GetMaterialInternal(i)->ReloadTextures();
				if( GetMaterialInternal(i)->GetVertexFormat() != oldVertexFormat )
				{
					bVertexFormatChanged = true;
				}
			}
		}
	}

	if( bVertexFormatChanged )
	{
		// Reloading materials could cause a vertex format change, so
		// we need to release and restore
		ReleaseShaderObjects();
		RestoreShaderObjects( NULL, MATERIAL_RESTORE_VERTEX_FORMAT_CHANGED );
	}
}

//-----------------------------------------------------------------------------
// Allocates the standard textures used by the material system
//-----------------------------------------------------------------------------
void CMaterialSystem::AllocateStandardTextures()
{
	if ( m_StandardTexturesAllocated )
		return;

	m_StandardTexturesAllocated = true;

	float nominal_lightmap_value = 1.0;
	if ( HardwareConfig()->GetHDRType() == HDR_TYPE_INTEGER )
		nominal_lightmap_value = 1.0/16.0;

	unsigned char texel[4];
	texel[3] = 255;

	int tcFlags = TEXTURE_CREATE_MANAGED;
	if ( IsX360() )
	{
		tcFlags |= TEXTURE_CREATE_CANCONVERTFORMAT;
	}

	// allocate a white, single texel texture for the fullbright lightmap
	// note: make sure and redo this when changing gamma, etc.
	// don't mipmap lightmaps
	m_FullbrightLightmapTextureHandle = g_pShaderAPI->CreateTexture( 1, 1, 1, IMAGE_FORMAT_BGRX8888, 1, 1, tcFlags, "[FULLBRIGHT_LIGHTMAP_TEXID]", TEXTURE_GROUP_LIGHTMAP );
	g_pShaderAPI->ModifyTexture( m_FullbrightLightmapTextureHandle );
	g_pShaderAPI->TexMinFilter( SHADER_TEXFILTERMODE_LINEAR );
	g_pShaderAPI->TexMagFilter( SHADER_TEXFILTERMODE_LINEAR );
	float tmpVect[3] = { nominal_lightmap_value, nominal_lightmap_value, nominal_lightmap_value };
	ColorSpace::LinearToLightmap( texel, tmpVect );
	g_pShaderAPI->TexImage2D( 0, 0, IMAGE_FORMAT_BGRX8888, 0, 1, 1, IMAGE_FORMAT_BGRX8888, false, texel );

	// allocate a black single texel texture
#if !defined( _X360 )
	m_BlackTextureHandle = g_pShaderAPI->CreateTexture( 1, 1, 1, IMAGE_FORMAT_BGRX8888, 1, 1, tcFlags, "[BLACK_TEXID]", TEXTURE_GROUP_OTHER );
	g_pShaderAPI->ModifyTexture( m_BlackTextureHandle );
	g_pShaderAPI->TexMinFilter( SHADER_TEXFILTERMODE_LINEAR );
	g_pShaderAPI->TexMagFilter( SHADER_TEXFILTERMODE_LINEAR );
	texel[0] = texel[1] = texel[2] = 0;
	g_pShaderAPI->TexImage2D( 0, 0, IMAGE_FORMAT_BGRX8888, 0, 1, 1, IMAGE_FORMAT_BGRX8888, false, texel );
#else
	m_BlackTextureHandle = ((ITextureInternal*)FindTexture( "black", TEXTURE_GROUP_OTHER, true ))->GetTextureHandle( 0 );
#endif
	g_pShaderAPI->SetStandardTextureHandle( TEXTURE_BLACK, m_BlackTextureHandle );

	// allocate a fully white single texel texture
#if !defined( _X360 )
	m_WhiteTextureHandle = g_pShaderAPI->CreateTexture( 1, 1, 1, IMAGE_FORMAT_BGRX8888, 1, 1, tcFlags, "[WHITE_TEXID]", TEXTURE_GROUP_OTHER );
	g_pShaderAPI->ModifyTexture( m_WhiteTextureHandle );
	g_pShaderAPI->TexMinFilter( SHADER_TEXFILTERMODE_LINEAR );
	g_pShaderAPI->TexMagFilter( SHADER_TEXFILTERMODE_LINEAR );
	texel[0] = texel[1] = texel[2] = 255;
	g_pShaderAPI->TexImage2D( 0, 0, IMAGE_FORMAT_BGRX8888, 0, 1, 1, IMAGE_FORMAT_BGRX8888, false, texel );
#else
	m_WhiteTextureHandle = ((ITextureInternal*)FindTexture( "white", TEXTURE_GROUP_OTHER, true ))->GetTextureHandle( 0 );
#endif
	g_pShaderAPI->SetStandardTextureHandle( TEXTURE_WHITE, m_WhiteTextureHandle );

	// allocate a grey single texel texture with an alpha of zero (for mat_fullbright 2)
#if !defined( _X360 )
	m_GreyTextureHandle = g_pShaderAPI->CreateTexture( 1, 1, 1, IMAGE_FORMAT_BGRX8888, 1, 1, tcFlags, "[GREY_TEXID]", TEXTURE_GROUP_OTHER );
	g_pShaderAPI->ModifyTexture( m_GreyTextureHandle );
	g_pShaderAPI->TexMinFilter( SHADER_TEXFILTERMODE_LINEAR );
	g_pShaderAPI->TexMagFilter( SHADER_TEXFILTERMODE_LINEAR );
	texel[0] = texel[1] = texel[2] = 128;
	texel[3] = 255; // needs to be 255 so that mat_fullbright 2 stuff isn't translucent.
	g_pShaderAPI->TexImage2D( 0, 0, IMAGE_FORMAT_BGRX8888, 0, 1, 1, IMAGE_FORMAT_BGRX8888, false, texel );
#else
	m_GreyTextureHandle = ((ITextureInternal*)FindTexture( "grey", TEXTURE_GROUP_OTHER, true ))->GetTextureHandle( 0 );
#endif
	g_pShaderAPI->SetStandardTextureHandle( TEXTURE_GREY, m_GreyTextureHandle );

	// allocate a grey single texel texture with an alpha of zero (for mat_fullbright 2)
#if !defined( _X360 )
	m_GreyAlphaZeroTextureHandle = g_pShaderAPI->CreateTexture( 1, 1, 1, IMAGE_FORMAT_RGBA8888, 1, 1, tcFlags, "[GREY_TEXID]", TEXTURE_GROUP_OTHER );
	g_pShaderAPI->ModifyTexture( m_GreyAlphaZeroTextureHandle );
	g_pShaderAPI->TexMinFilter( SHADER_TEXFILTERMODE_LINEAR );
	g_pShaderAPI->TexMagFilter( SHADER_TEXFILTERMODE_LINEAR );
	texel[0] = texel[1] = texel[2] = 128;
	texel[3] = 0; // needs to be 0 so that self-illum doens't affect mat_fullbright 2
	g_pShaderAPI->TexImage2D( 0, 0, IMAGE_FORMAT_RGBA8888, 0, 1, 1, IMAGE_FORMAT_RGBA8888, false, texel );
	texel[3] = 255; // set back to default value so we don't affect the rest of this code.'
#else
	m_GreyAlphaZeroTextureHandle = ((ITextureInternal*)FindTexture( "greyalphazero", TEXTURE_GROUP_OTHER, true ))->GetTextureHandle( 0 );
#endif
	g_pShaderAPI->SetStandardTextureHandle( TEXTURE_GREY_ALPHA_ZERO, m_GreyAlphaZeroTextureHandle );

	// allocate a single texel flat normal texture lightmap
	m_FlatNormalTextureHandle = g_pShaderAPI->CreateTexture( 1, 1, 1, IMAGE_FORMAT_BGRX8888, 1, 1, tcFlags, "[FLAT_NORMAL_TEXTURE]", TEXTURE_GROUP_OTHER );
	g_pShaderAPI->ModifyTexture( m_FlatNormalTextureHandle );
	g_pShaderAPI->TexMinFilter( SHADER_TEXFILTERMODE_LINEAR );
	g_pShaderAPI->TexMagFilter( SHADER_TEXFILTERMODE_LINEAR );
	texel[0] = 127;
	texel[1] = 127;
	texel[2] = 255;
	g_pShaderAPI->TexImage2D( 0, 0, IMAGE_FORMAT_BGRX8888, 0, 1, 1, IMAGE_FORMAT_BGRX8888, false, texel );
	g_pShaderAPI->SetStandardTextureHandle( TEXTURE_NORMALMAP_FLAT, m_FlatNormalTextureHandle );

	// allocate a single texel fullbright 1 lightmap for use with bump textures
	m_FullbrightBumpedLightmapTextureHandle = g_pShaderAPI->CreateTexture( 1, 1, 1, IMAGE_FORMAT_BGRX8888, 1, 1, tcFlags, "[FULLBRIGHT_BUMPED_LIGHTMAP_TEXID]", TEXTURE_GROUP_LIGHTMAP );
	g_pShaderAPI->ModifyTexture( m_FullbrightBumpedLightmapTextureHandle );
	g_pShaderAPI->TexMinFilter( SHADER_TEXFILTERMODE_LINEAR );
	g_pShaderAPI->TexMagFilter( SHADER_TEXFILTERMODE_LINEAR );
	float linearColor[3] = { nominal_lightmap_value, nominal_lightmap_value, nominal_lightmap_value };
	unsigned char dummy[3];
	ColorSpace::LinearToBumpedLightmap( linearColor, linearColor, linearColor, linearColor,
				dummy, texel, dummy, dummy );
	g_pShaderAPI->TexImage2D( 0, 0, IMAGE_FORMAT_BGRX8888, 0, 1, 1, IMAGE_FORMAT_BGRX8888, false, texel );
	g_pShaderAPI->SetStandardTextureHandle( TEXTURE_LIGHTMAP_BUMPED_FULLBRIGHT, m_FullbrightBumpedLightmapTextureHandle );
	
	{
		int iGammaLookupFlags = tcFlags;
		ImageFormat gammalookupfmt;
		gammalookupfmt = IMAGE_FORMAT_I8;

		// generate the linear->gamma conversion table texture.
		{
			const int LINEAR_TO_GAMMA_TABLE_WIDTH = 512;
			m_LinearToGammaTableTextureHandle = g_pShaderAPI->CreateTexture( LINEAR_TO_GAMMA_TABLE_WIDTH, 1, 1, gammalookupfmt, 1, 1, iGammaLookupFlags, "[LINEAR_TO_GAMMA_LOOKUP_SRGBON_TEXID]", TEXTURE_GROUP_PIXEL_SHADERS );
			g_pShaderAPI->ModifyTexture( m_LinearToGammaTableTextureHandle );
			g_pShaderAPI->TexMinFilter( SHADER_TEXFILTERMODE_LINEAR );
			g_pShaderAPI->TexMagFilter( SHADER_TEXFILTERMODE_LINEAR );
			g_pShaderAPI->TexWrap( SHADER_TEXCOORD_S, SHADER_TEXWRAPMODE_CLAMP );
			g_pShaderAPI->TexWrap( SHADER_TEXCOORD_T, SHADER_TEXWRAPMODE_CLAMP );
			g_pShaderAPI->TexWrap( SHADER_TEXCOORD_U, SHADER_TEXWRAPMODE_CLAMP );

			float pixelData[LINEAR_TO_GAMMA_TABLE_WIDTH]; //sometimes used as float, sometimes as uint8, sizeof(float) > sizeof(uint8)
			for( int i = 0; i != LINEAR_TO_GAMMA_TABLE_WIDTH; ++i )
			{
				float fLookupResult = ((float)i) / ((float)(LINEAR_TO_GAMMA_TABLE_WIDTH - 1));
				fLookupResult = g_pShaderAPI->LinearToGamma_HardwareSpecific( fLookupResult );
				
				//do an extra srgb conversion because we'll be converting back on texture read
				fLookupResult = g_pShaderAPI->LinearToGamma_HardwareSpecific( fLookupResult ); //that's right, linear->gamma->gamma2x so that that gamma->linear srgb read still ends up in gamma
				
				int iColor = RoundFloatToInt( fLookupResult * 255.0f );
				if( iColor > 255 )
					iColor = 255;

				((uint8 *)pixelData)[i] = (uint8)iColor;
			}

			g_pShaderAPI->TexImage2D( 0, 0, gammalookupfmt, 0, LINEAR_TO_GAMMA_TABLE_WIDTH, 1, gammalookupfmt, false, (void *)pixelData );
		}

		// generate the identity conversion table texture.
		{
			const int LINEAR_TO_GAMMA_IDENTITY_TABLE_WIDTH = 256;
			m_LinearToGammaIdentityTableTextureHandle = g_pShaderAPI->CreateTexture( LINEAR_TO_GAMMA_IDENTITY_TABLE_WIDTH, 1, 1, gammalookupfmt, 1, 1, tcFlags, "[LINEAR_TO_GAMMA_LOOKUP_SRGBOFF_TEXID]", TEXTURE_GROUP_PIXEL_SHADERS );
			g_pShaderAPI->ModifyTexture( m_LinearToGammaIdentityTableTextureHandle );
			g_pShaderAPI->TexMinFilter( SHADER_TEXFILTERMODE_LINEAR );
			g_pShaderAPI->TexMagFilter( SHADER_TEXFILTERMODE_LINEAR );
			g_pShaderAPI->TexWrap( SHADER_TEXCOORD_S, SHADER_TEXWRAPMODE_CLAMP );
			g_pShaderAPI->TexWrap( SHADER_TEXCOORD_T, SHADER_TEXWRAPMODE_CLAMP );
			g_pShaderAPI->TexWrap( SHADER_TEXCOORD_U, SHADER_TEXWRAPMODE_CLAMP );

			float pixelData[LINEAR_TO_GAMMA_IDENTITY_TABLE_WIDTH]; //sometimes used as float, sometimes as uint8, sizeof(float) > sizeof(uint8)
			for( int i = 0; i != LINEAR_TO_GAMMA_IDENTITY_TABLE_WIDTH; ++i )
			{
				float fLookupResult = ((float)i) / ((float)(LINEAR_TO_GAMMA_IDENTITY_TABLE_WIDTH - 1));
				
				//do an extra srgb conversion because we'll be converting back on texture read
				fLookupResult = g_pShaderAPI->LinearToGamma_HardwareSpecific( fLookupResult );

				int iColor = RoundFloatToInt( fLookupResult * 255.0f );
				if ( iColor > 255 )
					iColor = 255;

				((uint8 *)pixelData)[i] = (uint8)iColor;
			}

			g_pShaderAPI->TexImage2D( 0, 0, gammalookupfmt, 0, LINEAR_TO_GAMMA_IDENTITY_TABLE_WIDTH, 1, gammalookupfmt, false, (void *)pixelData );
		}
	}

	//create the maximum depth texture
	{
		m_MaxDepthTextureHandle = g_pShaderAPI->CreateTexture( 1, 1, 1, IMAGE_FORMAT_RGBA8888, 1, 1, tcFlags, "[MAXDEPTH_TEXID]", TEXTURE_GROUP_OTHER );
		g_pShaderAPI->ModifyTexture( m_MaxDepthTextureHandle );
		g_pShaderAPI->TexMinFilter( SHADER_TEXFILTERMODE_LINEAR );
		g_pShaderAPI->TexMagFilter( SHADER_TEXFILTERMODE_LINEAR );
		
		//360 gets depth out of the red channel (which doubles as depth in D24S8) and may be 0/1 depending on REVERSE_DEPTH_ON_X360
		//PC gets depth out of the alpha channel
		texel[0] = texel[1] = texel[2] = ReverseDepthOnX360() ? 0 : 255;
		texel[3] = 255;

		g_pShaderAPI->TexImage2D( 0, 0, IMAGE_FORMAT_RGBA8888, 0, 1, 1, IMAGE_FORMAT_RGBA8888, false, texel );
	}

	//only the shaderapi can handle switching between textures correctly, so pass off the textures to it.
	g_pShaderAPI->SetLinearToGammaConversionTextures( m_LinearToGammaTableTextureHandle, m_LinearToGammaIdentityTableTextureHandle );
}

void CMaterialSystem::ReleaseStandardTextures()
{
	if ( m_StandardTexturesAllocated )
	{
		if ( IsPC() )
		{
			g_pShaderAPI->DeleteTexture( m_BlackTextureHandle );
			g_pShaderAPI->DeleteTexture( m_WhiteTextureHandle );
			g_pShaderAPI->DeleteTexture( m_GreyTextureHandle );
			g_pShaderAPI->DeleteTexture( m_GreyAlphaZeroTextureHandle );
		}
		g_pShaderAPI->DeleteTexture( m_FullbrightLightmapTextureHandle );
		g_pShaderAPI->DeleteTexture( m_FlatNormalTextureHandle );
		g_pShaderAPI->DeleteTexture( m_FullbrightBumpedLightmapTextureHandle );

		g_pShaderAPI->DeleteTexture( m_LinearToGammaTableTextureHandle );
		g_pShaderAPI->DeleteTexture( m_LinearToGammaIdentityTableTextureHandle );
		g_pShaderAPI->SetLinearToGammaConversionTextures( INVALID_SHADERAPI_TEXTURE_HANDLE, INVALID_SHADERAPI_TEXTURE_HANDLE );

		g_pShaderAPI->DeleteTexture( m_MaxDepthTextureHandle );

		m_StandardTexturesAllocated = false;
	}
}


//-----------------------------------------------------------------------------
// 
//-----------------------------------------------------------------------------
void CMaterialSystem::BeginFrame( float frameTime )
{
	// Safety measure (calls should only come from the main thread, also check correct pairing)
	if ( !ThreadInMainThread() || IsInFrame() )
		return;

	// check debug vars. we will use these to setup g_nDebugVarsSignature so that materials will
	// rebuild their draw lists when debug modes changed.
	g_nDebugVarsSignature = ( 
		(mat_specular.GetInt() != 0 ) + ( mat_normalmaps.GetInt() << 1 ) +
		( mat_fullbright.GetInt() << 2 ) + (mat_fastnobump.GetInt() << 4 ) ) << 24;
	
	
	Assert( m_bGeneratedConfig );

	VPROF_BUDGET( "CMaterialSystem::BeginFrame", VPROF_BUDGETGROUP_SWAP_BUFFERS );

	IMatRenderContextInternal *pRenderContext = GetRenderContextInternal();
	if ( g_config.ForceHWSync() && m_ThreadMode != MATERIAL_QUEUED_THREADED )
	{
		pRenderContext->ForceHardwareSync();
	}

	pRenderContext->BeginFrame();
	pRenderContext->SetFrameTime( frameTime );
	pRenderContext->SetToneMappingScaleLinear( Vector( 1,1,1) );

	Assert( !m_bInFrame );
	m_bInFrame = true;
}

bool CMaterialSystem::IsInFrame( ) const
{
	return m_bInFrame;
}


ConVar mat_queue_mode( "mat_queue_mode", "-1", 0, "The queue/thread mode the material system should use: -1=default, 0=synchronous single thread, 1=queued single thread, 2=queued multithreaded" );

void CMaterialSystem::ThreadExecuteQueuedContext( CMatQueuedRenderContext *pContext )
{
	m_nRenderThreadID = ThreadGetCurrentId(); 
	m_pRenderContext.Set( &m_HardwareRenderContext );
	pContext->EndQueue( true );
	m_pRenderContext.Set( NULL );
	m_nRenderThreadID = 0xFFFFFFFF; 
}

void CMaterialSystem::EndFrame( void )
{
	// Safety measure (calls should only come from the main thread, also check correct pairing)
	if ( !ThreadInMainThread() || !IsInFrame() )
		return;

	Assert( m_bGeneratedConfig );
	VPROF_BUDGET( "CMaterialSystem::EndFrame", VPROF_BUDGETGROUP_SWAP_BUFFERS );

	GetRenderContextInternal()->EndFrame();
	    
	//-------------------------------------------------------------

	int iConVarThreadMode = mat_queue_mode.GetInt();
	MaterialThreadMode_t nextThreadMode = ( iConVarThreadMode >= 0 ) ? (MaterialThreadMode_t)iConVarThreadMode : m_IdealThreadMode;

	switch ( m_ThreadMode )
	{
	case MATERIAL_SINGLE_THREADED:
		break;

	case MATERIAL_QUEUED_THREADED:
		{
			VPROF_BUDGET( "Mat_ThreadedEndframe", "Mat_ThreadedEndframe" );
			if ( m_pActiveAsyncJob && !m_pActiveAsyncJob->IsFinished() )
			{
				m_pActiveAsyncJob->WaitForFinish();
				if ( g_config.ForceHWSync() )
				{
					g_pShaderAPI->ForceHardwareSync();
				}
			}
			SafeRelease( m_pActiveAsyncJob );

			CMatQueuedRenderContext *pPrevContext = &m_QueuedRenderContexts[m_iCurQueuedContext];

			m_QueuedRenderContexts[m_iCurQueuedContext].GetCallQueueInternal()->QueueCall( g_pShaderAPI, &IShaderAPI::ReleaseThreadOwnership );
			m_iCurQueuedContext = ( ( m_iCurQueuedContext + 1 ) % ARRAYSIZE( m_QueuedRenderContexts) );
			m_QueuedRenderContexts[m_iCurQueuedContext].BeginQueue( pPrevContext );
			m_QueuedRenderContexts[m_iCurQueuedContext].GetCallQueueInternal()->QueueCall( g_pShaderAPI, &IShaderAPI::AcquireThreadOwnership );
			m_pRenderContext.Set( &m_QueuedRenderContexts[m_iCurQueuedContext] );

			m_pActiveAsyncJob = new CFunctorJob( CreateFunctor( this, &CMaterialSystem::ThreadExecuteQueuedContext, pPrevContext ) );
			if ( IsX360() )
			{
				if ( m_nServiceThread >= 0 )
				{
					m_pActiveAsyncJob->SetServiceThread( m_nServiceThread );
				}
				m_pActiveAsyncJob->SetFlags( m_pActiveAsyncJob->GetFlags() | JF_QUEUE );
			}

			g_pThreadPool->AddJob( m_pActiveAsyncJob );
			break;
		}

	case MATERIAL_QUEUED_SINGLE_THREADED:
		{
			VPROF_BUDGET( "Mat_ThreadedEndframe", "Mat_QueuedEndframe" );

			g_pShaderAPI->SetDisallowAccess( false );
			m_pRenderContext.Set( &m_HardwareRenderContext );
			m_QueuedRenderContexts[m_iCurQueuedContext].CallQueued();
			m_pRenderContext.Set( &m_QueuedRenderContexts[m_iCurQueuedContext] );
			g_pShaderAPI->SetDisallowAccess( true );
			break;
		}
	}

	if ( m_ThreadMode != nextThreadMode )
	{
		// Shut down the current mode & set new mode
		switch ( m_ThreadMode )
		{
		case MATERIAL_SINGLE_THREADED:
			break;

		case MATERIAL_QUEUED_THREADED:
			{
				if ( m_pActiveAsyncJob )
				{
					m_pActiveAsyncJob->WaitForFinish();
					SafeRelease( m_pActiveAsyncJob );
				}
				m_QueuedRenderContexts[m_iCurQueuedContext].EndQueue( true );
				g_pShaderAPI->AcquireThreadOwnership();
			}
			break;

		case MATERIAL_QUEUED_SINGLE_THREADED:
			{
				g_pShaderAPI->SetDisallowAccess( false );
				m_QueuedRenderContexts[m_iCurQueuedContext].EndQueue( true );
				break;
			}
		}

		m_ThreadMode = nextThreadMode;
#ifndef _LINUX
		Assert( g_MatSysMutex.GetOwnerId() == 0 );
#endif

		g_pShaderAPI->EnableShaderShaderMutex( m_ThreadMode != MATERIAL_SINGLE_THREADED ); // use mutex even for queued to allow "disalow access" to function properly
		g_pShaderAPI->EnableBuffer2FramesAhead( true );

		switch ( m_ThreadMode )
		{
		case MATERIAL_SINGLE_THREADED:
			m_pRenderContext.Set( &m_HardwareRenderContext );
			for ( int i = 0; i < ARRAYSIZE( m_QueuedRenderContexts ); i++ )
			{
				Assert( m_QueuedRenderContexts[i].IsInitialized() );
				m_QueuedRenderContexts[i].Term();
			}
			break;

		case MATERIAL_QUEUED_SINGLE_THREADED:
		case MATERIAL_QUEUED_THREADED:
			for ( int i = 0; i < ARRAYSIZE( m_QueuedRenderContexts ); i++ )
			{
				if ( !m_QueuedRenderContexts[i].IsInitialized() )
				{
					m_QueuedRenderContexts[i].Init( this, &m_HardwareRenderContext );
				}
			}
			m_iCurQueuedContext = 0;
			m_QueuedRenderContexts[m_iCurQueuedContext].BeginQueue( &m_HardwareRenderContext );
			m_pRenderContext.Set( &m_QueuedRenderContexts[m_iCurQueuedContext] );
			if ( m_ThreadMode == MATERIAL_QUEUED_SINGLE_THREADED )
			{
				g_pShaderAPI->SetDisallowAccess( true );
			}
			else
			{
				g_pShaderAPI->ReleaseThreadOwnership();
				m_QueuedRenderContexts[m_iCurQueuedContext].GetCallQueueInternal()->QueueCall( g_pShaderAPI, &IShaderAPI::AcquireThreadOwnership );
			}
			break;
		}
	}

	Assert( m_bInFrame );
	m_bInFrame = false;
}

void CMaterialSystem::SetInStubMode( bool bInStubMode )
{
	m_bInStubMode = bInStubMode;
}

bool CMaterialSystem::IsInStubMode()
{
	return m_bInStubMode;
}

void CMaterialSystem::Flush( bool flushHardware )
{
	GetRenderContextInternal()->Flush( flushHardware );
}


//-----------------------------------------------------------------------------
// Flushes managed textures from the texture cacher
//-----------------------------------------------------------------------------
void CMaterialSystem::EvictManagedResources()
{
	g_pShaderAPI->EvictManagedResources();
}

int __cdecl MaterialNameCompareFunc( const void *elem1, const void *elem2 )
{
	IMaterialInternal *pMaterialA = g_MaterialSystem.GetMaterialInternal( *(MaterialHandle_t *)elem1 );
	IMaterialInternal *pMaterialB = g_MaterialSystem.GetMaterialInternal( *(MaterialHandle_t *)elem2 );

	// case insensitive to group similar named materials
	return stricmp( pMaterialA->GetName(), pMaterialB->GetName() );
}

void CMaterialSystem::DebugPrintUsedMaterials( const char *pSearchSubString, bool bVerbose )
{
	MaterialHandle_t	h;
	int					i;
	int					nNumCached;
	int					nRefCount;
	int					nSortedMaterials;
	int					nNumErrors;
	
	// build a mapping to sort the material names
	MaterialHandle_t *pSorted = (MaterialHandle_t*)stackalloc( GetNumMaterials() * sizeof(MaterialHandle_t) );
	nSortedMaterials = 0;
	for (h = FirstMaterial(); h != InvalidMaterial(); h = NextMaterial(h) )
	{
		pSorted[nSortedMaterials++] = h;
	}
	qsort( pSorted, nSortedMaterials, sizeof(MaterialHandle_t), MaterialNameCompareFunc );

	nNumCached = 0;
	nNumErrors = 0;
	for (i = 0; i < nSortedMaterials; i++)
	{
		// iterate using sort mapping
		IMaterialInternal *pMaterial = GetMaterialInternal(pSorted[i]);

		nRefCount = pMaterial->GetReferenceCount();
		
		if ( nRefCount < 0 )
		{
			nNumErrors++;
		}
		else if (!nRefCount)
		{
			if (pMaterial->IsPrecached() || pMaterial->IsPrecachedVars())
			{
				nNumErrors++;
			}
		}
		else
		{
			// nonzero reference count
			// tally the valid ones
			nNumCached++;

			if( pSearchSubString )
			{
				if( !Q_stristr( pMaterial->GetName(), pSearchSubString ) &&
					(!pMaterial->GetShader() || !Q_stristr( pMaterial->GetShader()->GetName(), pSearchSubString )) )
				{
					continue;
				}
			}

			DevMsg( "%s (shader: %s) refCount: %d.\n", pMaterial->GetName(), 
				pMaterial->GetShader() ? pMaterial->GetShader()->GetName() : "unknown\n", nRefCount );

			if( !bVerbose )
			{
				continue;
			}

			if( pMaterial->IsPrecached() )
			{
				int j;
				if( pMaterial->GetShader() )
				{
					for( j = 0; j < pMaterial->GetShader()->GetNumParams(); j++ )
					{
						IMaterialVar *var;
						var = pMaterial->GetShaderParams()[j];
						
						if( var )
						{
							switch( var->GetType() )
							{
							case MATERIAL_VAR_TYPE_TEXTURE:
								{
									ITextureInternal *texture = static_cast<ITextureInternal *>( var->GetTextureValue() );
									if( !texture )
									{
										DevWarning( "Programming error: CMaterialSystem::DebugPrintUsedMaterialsCallback: NULL texture\n" );
										continue;
									}
									
									if( IsTextureInternalEnvCubemap( texture ) )
									{
										DevMsg( "    \"%s\" \"env_cubemap\"\n", var->GetName() );
									}
									else
									{
										DevMsg( "    \"%s\" \"%s\"\n", 
											var->GetName(),
											texture->GetName() );
										DevMsg( "        %dx%d refCount: %d numframes: %d\n", texture->GetActualWidth(), texture->GetActualHeight(), 
											texture->GetReferenceCount(), texture->GetNumAnimationFrames() );
									}
								}
								break;
							case MATERIAL_VAR_TYPE_UNDEFINED:
								break;
							default:
								DevMsg( "    \"%s\" \"%s\"\n", var->GetName(), var->GetStringValue() );
								break;
							}
						}
					}
				}
			}
		}
	}

	// list the critical errors after, otherwise the console log scrolls them away
	if (nNumErrors)
	{
		for (i = 0; i < nSortedMaterials; i++)
		{
			// iterate using sort mapping
			IMaterialInternal *pMaterial = GetMaterialInternal(pSorted[i]);

			nRefCount = pMaterial->GetReferenceCount();

			if ( nRefCount < 0 )
			{
				// reference counts should not be negative
				DevWarning( "DebugPrintUsedMaterials: refCount (%d) < 0 for material: \"%s\"\n",
					nRefCount, pMaterial->GetName() );
			}
			else if (!nRefCount)
			{
				// ensure that it stayed uncached after the post loading uncache
				// this is effectively a coding bug thats needs to be fixed
				// a material is being precached without incrementing its reference
				if (pMaterial->IsPrecached() || pMaterial->IsPrecachedVars())
				{
					DevWarning( "DebugPrintUsedMaterials: material: \"%s\" didn't unache\n",
						pMaterial->GetName() );
				}
			}
		}
		DevWarning( "%d Errors\n", nNumErrors );
	}

	if (!pSearchSubString)
	{
		DevMsg( "%d Cached, %d Total Materials\n", nNumCached, GetNumMaterials() );
	}
}

void CMaterialSystem::DebugPrintUsedTextures( void )
{
	TextureManager()->DebugPrintUsedTextures();
}

#if defined( _X360 )
void CMaterialSystem::ListUsedMaterials( void )
{	
	int numMaterials = GetNumMaterials();
	xMaterialList_t* pMaterialList = (xMaterialList_t *)stackalloc( numMaterials * sizeof( xMaterialList_t ) );

	numMaterials = 0;
	for ( MaterialHandle_t hMaterial = FirstMaterial(); hMaterial != InvalidMaterial(); hMaterial = NextMaterial( hMaterial ) )
	{
		IMaterialInternal *pMaterial = GetMaterialInternal( hMaterial );
		pMaterialList[numMaterials].pName = pMaterial->GetName();
		pMaterialList[numMaterials].pShaderName = pMaterial->GetShader() ? pMaterial->GetShader()->GetName() : "???";
		pMaterialList[numMaterials].refCount = pMaterial->GetReferenceCount();
		numMaterials++;
	}

	XBX_rMaterialList( numMaterials, pMaterialList );
}
#endif

void CMaterialSystem::ToggleSuppressMaterial( char const* pMaterialName )
{
	/*
	// This version suppresses all but the material
	IMaterial *pMaterial = GetFirstMaterial();
	while (pMaterial)
	{
		if (stricmp(pMaterial->GetName(), pMaterialName))
		{
			IMaterialInternal* pMatInt = static_cast<IMaterialInternal*>(pMaterial);
			pMatInt->ToggleSuppression();
		}
		pMaterial = GetNextMaterial();
	}
	*/

	// Note: if we use this function a lot, we'll want to do something else, like have them
	// pass in a texture group or reuse whatever texture group the material already had.
	// As it is, this is rarely used, so if it's not in TEXTURE_GROUP_OTHER, it'll go in 
	// TEXTURE_GROUP_SHARED.
	IMaterial* pMaterial = FindMaterial( pMaterialName, TEXTURE_GROUP_OTHER, true, NULL );
	if ( !IsErrorMaterial( pMaterial ) )
	{
		IMaterialInternal* pMatInt = static_cast<IMaterialInternal*>(pMaterial);
		pMatInt = pMatInt->GetRealTimeVersion(); //always work with the realtime material internally
		pMatInt->ToggleSuppression();
	}
}

void CMaterialSystem::ToggleDebugMaterial( char const* pMaterialName )
{
	// Note: if we use this function a lot, we'll want to do something else, like have them
	// pass in a texture group or reuse whatever texture group the material already had.
	// As it is, this is rarely used, so if it's not in TEXTURE_GROUP_OTHER, it'll go in 
	// TEXTURE_GROUP_SHARED.
	IMaterial* pMaterial = FindMaterial( pMaterialName, TEXTURE_GROUP_OTHER, false, NULL );
	if ( !IsErrorMaterial( pMaterial ) )
	{
		IMaterialInternal* pMatInt = static_cast<IMaterialInternal*>(pMaterial);
		pMatInt = pMatInt->GetRealTimeVersion(); //always work with the realtime material internally
		pMatInt->ToggleDebugTrace();
	}
	else
	{
		Warning("Unknown material %s\n", pMaterialName );
	}
}


//-----------------------------------------------------------------------------
// Used to iterate over all shaders for editing purposes
//-----------------------------------------------------------------------------
int CMaterialSystem::ShaderCount() const
{
	return ShaderSystem()->ShaderCount();
}

int CMaterialSystem::GetShaders( int nFirstShader, int nMaxCount, IShader **ppShaderList ) const
{
	return ShaderSystem()->GetShaders( nFirstShader, nMaxCount, ppShaderList );
}


//-----------------------------------------------------------------------------
// FIXME: Is there a better way of doing this?
// Returns shader flag names for editors to be able to edit them
//-----------------------------------------------------------------------------
int CMaterialSystem::ShaderFlagCount() const
{
	return ShaderSystem()->ShaderStateCount( );
}

const char *CMaterialSystem::ShaderFlagName( int nIndex ) const
{
	return ShaderSystem()->ShaderStateString( nIndex );
}


//-----------------------------------------------------------------------------
// Returns the currently active shader fallback for a particular shader
//-----------------------------------------------------------------------------
void CMaterialSystem::GetShaderFallback( const char *pShaderName, char *pFallbackShader, int nFallbackLength )
{
	// FIXME: This is pretty much a hack. We need a better way for the
	// editor to get ahold of shader fallbacks
	int nCount = ShaderCount();
	IShader** ppShaderList = (IShader**)_alloca( nCount * sizeof(IShader) );
	GetShaders( 0, nCount, ppShaderList );

	do
	{
		int i;
		for ( i = 0; i < nCount; ++i )
		{
			if ( !Q_stricmp( pShaderName, ppShaderList[i]->GetName() ) )
				break;
		}

		// Didn't find a match!
		if ( i == nCount )
		{
			Q_strncpy( pFallbackShader, "wireframe", nFallbackLength );
			return;
		}

		// Found a match
		// FIXME: Theoretically, getting fallbacks should require a param list
		// In practice, it looks rare or maybe even neved done
		const char *pFallback = ppShaderList[i]->GetFallbackShader( NULL );
		if ( !pFallback )
		{
			Q_strncpy( pFallbackShader, pShaderName, nFallbackLength );
			return;
		}
		else
		{
			pShaderName = pFallback;
		}
	} while (true);
}

	
void CMaterialSystem::SwapBuffers( void )
{
	VPROF_BUDGET( "CMaterialSystem::SwapBuffers", VPROF_BUDGETGROUP_SWAP_BUFFERS );
	GetRenderContextInternal()->SwapBuffers();
	g_FrameNum++;
}

bool CMaterialSystem::InEditorMode() const
{
	Assert( m_bGeneratedConfig );
	return g_config.bEditMode && CanUseEditorMaterials();
}

void CMaterialSystem::NoteAnisotropicLevel( int currentLevel )
{
	Assert( m_bGeneratedConfig );
	g_config.m_nForceAnisotropicLevel = currentLevel;
}

// Get the current config for this video card (as last set by control panel or the default if not)
const MaterialSystem_Config_t &CMaterialSystem::GetCurrentConfigForVideoCard() const
{
	Assert( m_bGeneratedConfig );
	return g_config;
}

// Does the device support the given MSAA level?
bool CMaterialSystem::SupportsMSAAMode( int nNumSamples )
{
	return g_pShaderAPI->SupportsMSAAMode( nNumSamples );
}

void CMaterialSystem::ReloadFilesInList( IFileList *pFilesToReload )
{
	if ( !IsPC() )
		return;

	// We have to flush the materials in 2 steps because they have recursive dependencies. The problem case
	// is if you have two materials, A and B, that depend on C. You tell A to reload and it also reloads C. Then
	// the filesystem thinks C doesn't need to be reloaded anymore. So when you get to B, it decides not to reload 
	// either since C doesn't need to be reloaded. To fix this, we ask all materials if they want to reload in
	// one stage, then in the next stage we actually reload the appropriate ones.
	MaterialHandle_t hNext;
	for ( MaterialHandle_t h=m_MaterialDict.FirstMaterial(); h != m_MaterialDict.InvalidMaterial(); h=hNext )
	{
		hNext = m_MaterialDict.NextMaterial( h );
		IMaterialInternal *pMat = m_MaterialDict.GetMaterialInternal( h );

		pMat->DecideShouldReloadFromWhitelist( pFilesToReload );
	}

	// Now reload the materials that wanted to be reloaded.
	for ( MaterialHandle_t h=m_MaterialDict.FirstMaterial(); h != m_MaterialDict.InvalidMaterial(); h=hNext )
	{
		hNext = m_MaterialDict.NextMaterial( h );
		IMaterialInternal *pMat = m_MaterialDict.GetMaterialInternal( h );

		pMat->ReloadFromWhitelistIfMarked();
	}

	// Flush out necessary textures.
	TextureManager()->ReloadFilesInList( pFilesToReload );
}

// Does the device support the given CSAA level?
bool CMaterialSystem::SupportsCSAAMode( int nNumSamples, int nQualityLevel )
{
	return g_pShaderAPI->SupportsCSAAMode( nNumSamples, nQualityLevel );
}

// Does the device support shadow depth texturing?
bool CMaterialSystem::SupportsShadowDepthTextures( void )
{
	return g_pShaderAPI->SupportsShadowDepthTextures();
}

// Does the device support Fetch4
bool CMaterialSystem::SupportsFetch4( void )
{
	return g_pShaderAPI->SupportsFetch4();
}

// Vendor-dependent shadow depth texture format
ImageFormat CMaterialSystem::GetShadowDepthTextureFormat( void )
{
	return g_pShaderAPI->GetShadowDepthTextureFormat();
}

// Vendor-dependent slim texture format
ImageFormat CMaterialSystem::GetNullTextureFormat( void )
{
	return g_pShaderAPI->GetNullTextureFormat();
}

void CMaterialSystem::SetShadowDepthBiasFactors( float fShadowSlopeScaleDepthBias, float fShadowDepthBias ) 
{
	g_pShaderAPI->SetShadowDepthBiasFactors( fShadowSlopeScaleDepthBias, fShadowDepthBias );
}

bool CMaterialSystem::SupportsHDRMode( HDRType_t nHDRMode )
{
	return HardwareConfig()->SupportsHDRMode( nHDRMode );
}

bool CMaterialSystem::UsesSRGBCorrectBlending( void ) const
{
	return HardwareConfig()->UsesSRGBCorrectBlending();
}

// Get video card identitier
const MaterialSystemHardwareIdentifier_t &CMaterialSystem::GetVideoCardIdentifier( void ) const
{
	static MaterialSystemHardwareIdentifier_t foo;
	Assert( 0 );
	return foo;
}

void CMaterialSystem::AddModeChangeCallBack( ModeChangeCallbackFunc_t func )
{
	g_pShaderDeviceMgr->AddModeChangeCallback( func );
}

void CMaterialSystem::RemoveModeChangeCallBack( ModeChangeCallbackFunc_t func )
{
	g_pShaderDeviceMgr->RemoveModeChangeCallback( func );
}


//-----------------------------------------------------------------------------
// Gets configuration information associated with the display card, and optionally for a particular DX level.
// It will return a list of ConVars and values to set.
//-----------------------------------------------------------------------------
bool CMaterialSystem::GetRecommendedConfigurationInfo( int nDXLevel, KeyValues *pKeyValues )
{
	MaterialLock_t hLock = Lock();
	bool bResult = g_pShaderDeviceMgr->GetRecommendedConfigurationInfo( m_nAdapter, nDXLevel, pKeyValues );
	Unlock( hLock );
	return bResult;
}


//-----------------------------------------------------------------------------
// For dealing with device lost in cases where SwapBuffers isn't called all the time (Hammer)
//-----------------------------------------------------------------------------
void CMaterialSystem::HandleDeviceLost()
{
	if ( IsX360() )
		return;

	g_pShaderAPI->HandleDeviceLost();
}
	
bool CMaterialSystem::UsingFastClipping( void )
{
	return (HardwareConfig()->UseFastClipping() || (HardwareConfig()->MaxUserClipPlanes() < 1));
};

int CMaterialSystem::StencilBufferBits( void )
{
	return HardwareConfig()->StencilBufferBits();
}

ITexture* CMaterialSystem::CreateRenderTargetTexture( 
	int w, 
	int h, 
	RenderTargetSizeMode_t sizeMode,	// Controls how size is generated (and regenerated on video mode change).
	ImageFormat format, 
	MaterialRenderTargetDepth_t depth )
{
	return CreateNamedRenderTargetTextureEx( NULL, w, h, sizeMode, format, depth, TEXTUREFLAGS_CLAMPS|TEXTUREFLAGS_CLAMPT, 0 );
}

ITexture* CMaterialSystem::CreateNamedRenderTargetTexture( 
	const char *pRTName, 
	int w, 
	int h, 
	RenderTargetSizeMode_t sizeMode,	// Controls how size is generated (and regenerated on video mode change).
	ImageFormat format, 
	MaterialRenderTargetDepth_t depth,
	bool bClampTexCoords, 
	bool bAutoMipMap )
{
	unsigned int textureFlags = 0;
	if ( bClampTexCoords )
	{
		textureFlags |= TEXTUREFLAGS_CLAMPS | TEXTUREFLAGS_CLAMPT;
	}

	unsigned int renderTargetFlags = 0;
	if ( bAutoMipMap )
	{
		renderTargetFlags |= CREATERENDERTARGETFLAGS_AUTOMIPMAP;
	}

	return CreateNamedRenderTargetTextureEx( pRTName, w, h, sizeMode, format, depth, textureFlags, renderTargetFlags );
}

ITexture* CMaterialSystem::CreateNamedRenderTargetTextureEx( 
	const char *pRTName, 
	int w, 
	int h, 
	RenderTargetSizeMode_t sizeMode,	// Controls how size is generated (and regenerated on video mode change).
	ImageFormat format, 
	MaterialRenderTargetDepth_t depth,
	unsigned int textureFlags, 
	unsigned int renderTargetFlags )
{
	RenderTargetType_t rtType;

	// GR - determine RT type based on depth buffer requirements
	switch ( depth )
	{
	case MATERIAL_RT_DEPTH_SEPARATE:
		// using own depth buffer
		rtType = RENDER_TARGET_WITH_DEPTH;
		break;
	case MATERIAL_RT_DEPTH_NONE:
		// no depth buffer
		rtType = RENDER_TARGET_NO_DEPTH;
		break;
	case MATERIAL_RT_DEPTH_ONLY:
		// only depth buffer
		rtType = RENDER_TARGET_ONLY_DEPTH;
		break;
	case MATERIAL_RT_DEPTH_SHARED:
	default:
		// using shared depth buffer
		rtType = RENDER_TARGET;
		break;
	}
	
	ITextureInternal* pTex = TextureManager()->CreateRenderTargetTexture( pRTName, w, h, sizeMode, format, rtType, textureFlags, renderTargetFlags );
	pTex->IncrementReferenceCount();

#if defined( _X360 )
	if ( !( renderTargetFlags & CREATERENDERTARGETFLAGS_NOEDRAM ) )
	{
		// create the EDRAM surface that is bound to the RT Texture
		pTex->CreateRenderTargetSurface( 0, 0, IMAGE_FORMAT_UNKNOWN, true );
	}
#endif

	// If we're not in a BeginRenderTargetAllocation-EndRenderTargetAllocation block
	// because we're being called by a legacy path (i.e. a mod), force an Alt-Tab after every
	// RT allocation to ensure that all RTs get priority during allocation
	if ( !m_bAllocatingRenderTargets )
	{
		EndRenderTargetAllocation();
	}

	return pTex;
}

//-----------------------------------------------------------------------------------------------------
// New version which must be called inside BeginRenderTargetAllocation-EndRenderTargetAllocation block
//-----------------------------------------------------------------------------------------------------
ITexture *CMaterialSystem::CreateNamedRenderTargetTextureEx2(
	const char *pRTName, 
	int w, 
	int h, 
	RenderTargetSizeMode_t sizeMode,	// Controls how size is generated (and regenerated on video mode change).
	ImageFormat format, 
	MaterialRenderTargetDepth_t depth,
	unsigned int textureFlags, 
	unsigned int renderTargetFlags )
{
	// Only proceed if we are between BeginRenderTargetAllocation and EndRenderTargetAllocation
	if ( !m_bAllocatingRenderTargets )
	{
		Warning( "Tried to create render target outside of CMaterialSystem::BeginRenderTargetAllocation/EndRenderTargetAllocation block\n" );
		return NULL;
	}

	ITexture* pTexture = CreateNamedRenderTargetTextureEx( pRTName, w, h, sizeMode, format, depth, textureFlags, renderTargetFlags );

	pTexture->DecrementReferenceCount(); // Follow the same convention as CTextureManager::LoadTexture (return refcount of 0).
	return pTexture;
}

void CMaterialSystem::BeginRenderTargetAllocation( void )
{
	g_pShaderAPI->FlushBufferedPrimitives();
	m_bAllocatingRenderTargets = true;
}

void CMaterialSystem::EndRenderTargetAllocation( void )
{
	g_pShaderAPI->FlushBufferedPrimitives();
	m_bAllocatingRenderTargets = false;

	if ( IsPC() )
	{
		// Simulate an Alt-Tab...will cause RTs to be allocated first
		g_pShaderDevice->ReleaseResources();
		g_pShaderDevice->ReacquireResources();
	}

	TextureManager()->CacheExternalStandardRenderTargets();
}

//-----------------------------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------------------------
void CMaterialSystem::UpdateLightmap( int lightmapPageID, int lightmapSize[2],
										int offsetIntoLightmapPage[2], 
										float *pFloatImage, float *pFloatImageBump1,
										float *pFloatImageBump2, float *pFloatImageBump3 )
{
	CMatCallQueue *pCallQueue = GetRenderCallQueue();
	if ( !pCallQueue )
	{
		m_Lightmaps.UpdateLightmap( lightmapPageID, lightmapSize, offsetIntoLightmapPage, pFloatImage, pFloatImageBump1, pFloatImageBump2, pFloatImageBump3 );
	}
	else
	{
		ExecuteOnce( DebuggerBreakIfDebugging() );
	}
}

//-----------------------------------------------------------------------------------------------------
// 360 TTF Font Support
//-----------------------------------------------------------------------------------------------------
#if defined( _X360 )
HXUIFONT CMaterialSystem::OpenTrueTypeFont( const char *pFontname, int tall, int style )
{
	MaterialLock_t hLock = Lock();
	HXUIFONT result = g_pShaderAPI->OpenTrueTypeFont( pFontname, tall, style );
	Unlock( hLock );
	return result;
}
void CMaterialSystem::CloseTrueTypeFont( HXUIFONT hFont )
{
	MaterialLock_t hLock = Lock();
	g_pShaderAPI->CloseTrueTypeFont( hFont );
	Unlock( hLock );
}
bool CMaterialSystem::GetTrueTypeFontMetrics( HXUIFONT hFont, XUIFontMetrics *pFontMetrics, XUICharMetrics charMetrics[256] )
{
	MaterialLock_t hLock = Lock();
	bool result = g_pShaderAPI->GetTrueTypeFontMetrics( hFont, pFontMetrics, charMetrics );
	Unlock( hLock );
	return result;
}
bool CMaterialSystem::GetTrueTypeGlyphs( HXUIFONT hFont, int numChars, wchar_t *pWch, int *pOffsetX, int *pOffsetY, int *pWidth, int *pHeight, unsigned char *pRGBA, int *pRGBAOffset )
{
	MaterialLock_t hLock = Lock();
	bool result = g_pShaderAPI->GetTrueTypeGlyphs( hFont, numChars, pWch, pOffsetX, pOffsetY, pWidth, pHeight, pRGBA, pRGBAOffset );
	Unlock( hLock );
	return result;
}
#endif

//-----------------------------------------------------------------------------------------------------
// 360 Back Buffer access. Due to hardware, RT data must be blitted from EDRAM
// and converted.
//-----------------------------------------------------------------------------------------------------
#if defined( _X360 )
void CMaterialSystem::ReadBackBuffer( Rect_t *pSrcRect, Rect_t *pDstRect, unsigned char *pDstData, ImageFormat dstFormat, int dstStride ) 
{
	Assert( pSrcRect && pDstRect && pDstData );

	int fbWidth, fbHeight;
	g_pShaderAPI->GetBackBufferDimensions( fbWidth, fbHeight ); 

	if ( pDstRect->width > fbWidth || pDstRect->height > fbHeight )
	{
		Assert( 0 );
		return;
	}

	// intermediate results will be placed at (0,0)
	Rect_t	rect;
	rect.x = 0;
	rect.y = 0;
	rect.width = pDstRect->width;
	rect.height = pDstRect->height;

	ITexture *pTempRT;
	bool bStretch = ( pSrcRect->width != pDstRect->width || pSrcRect->height != pDstRect->height );
	if ( !bStretch )
	{
		// hijack an unused RT (no surface required) for 1:1 resolve work, fastest path
		pTempRT = FindTexture( "_rt_FullFrameFB", TEXTURE_GROUP_RENDER_TARGET );
	}
	else
	{
		// hijack an unused RT (with surface abilities) for stretch work, slower path
		pTempRT = FindTexture( "_rt_WaterReflection", TEXTURE_GROUP_RENDER_TARGET );
	}

	Assert( !pTempRT->IsError() && pDstRect->width <= pTempRT->GetActualWidth() && pDstRect->height <= pTempRT->GetActualHeight() );
	GetRenderContextInternal()->CopyRenderTargetToTextureEx( pTempRT, 0, pSrcRect, &rect );

	// access the RT bits
	CPixelWriter writer;
	g_pShaderAPI->ModifyTexture( ((ITextureInternal*)pTempRT)->GetTextureHandle( 0 ) );
	if ( !g_pShaderAPI->TexLock( 0, 0, 0, 0, pTempRT->GetActualWidth(), pTempRT->GetActualHeight(), writer ) )
		return;

	// this will be adequate for non-block formats
	int srcStride = pTempRT->GetActualWidth() * ImageLoader::SizeInBytes( pTempRT->GetImageFormat() );

	// untile intermediate RT in place to achieve linear access
	XGUntileTextureLevel(
		pTempRT->GetActualWidth(),
		pTempRT->GetActualHeight(),
		0,
		XGGetGpuFormat( ImageLoader::ImageFormatToD3DFormat( pTempRT->GetImageFormat() ) ),
		0,
		(char*)writer.GetPixelMemory(),
		srcStride,
		NULL,
		writer.GetPixelMemory(),
		NULL );

	// swap back to x86 order as expected by image conversion
	ImageLoader::ByteSwapImageData( (unsigned char*)writer.GetPixelMemory(), srcStride*pTempRT->GetActualHeight(), pTempRT->GetImageFormat() );

	// convert to callers format
	Assert( dstFormat == IMAGE_FORMAT_RGB888 );
	ImageLoader::ConvertImageFormat( (unsigned char*)writer.GetPixelMemory(), pTempRT->GetImageFormat(), pDstData, dstFormat, pDstRect->width, pDstRect->height, srcStride, dstStride );

	g_pShaderAPI->TexUnlock();
}
#endif

#if defined( _X360 )
void CMaterialSystem::PersistDisplay() 
{
	g_pShaderAPI->PersistDisplay();
}
#endif

#if defined( _X360 )
void *CMaterialSystem::GetD3DDevice() 
{
	return g_pShaderAPI->GetD3DDevice();
}
#endif

#if defined( _X360 )
bool CMaterialSystem::OwnGPUResources( bool bEnable ) 
{
	return g_pShaderAPI->OwnGPUResources( bEnable );
}
#endif

//-----------------------------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------------------------
MaterialLock_t CMaterialSystem::Lock()
{
	IMatRenderContextInternal *pCurContext = GetRenderContextInternal();
	if ( pCurContext != &m_HardwareRenderContext && m_pActiveAsyncJob )
	{
		m_pActiveAsyncJob->WaitForFinishAndRelease();
		m_pActiveAsyncJob = NULL;
	}

	g_MatSysMutex.Lock();
	g_pShaderAPI->ShaderLock();

	MaterialLock_t hMaterialLock = (MaterialLock_t)pCurContext;
	m_pRenderContext.Set( &m_HardwareRenderContext );

	if ( m_ThreadMode != MATERIAL_SINGLE_THREADED )
	{
		g_pShaderAPI->SetDisallowAccess( false );
		if ( pCurContext->GetCallQueueInternal() )
		{
			g_pShaderAPI->AcquireThreadOwnership();

		}
	}

	return hMaterialLock;
}

//-----------------------------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------------------------
void CMaterialSystem::Unlock( MaterialLock_t hMaterialLock )
{
	IMatRenderContextInternal *pRenderContext = (IMatRenderContextInternal *)hMaterialLock;
	m_pRenderContext.Set( pRenderContext );
	g_pShaderAPI->ShaderUnlock();

	if ( m_ThreadMode == MATERIAL_QUEUED_SINGLE_THREADED )
	{
		g_pShaderAPI->SetDisallowAccess( true );
	}
	else if ( m_ThreadMode == MATERIAL_QUEUED_THREADED )
	{
		if ( pRenderContext->GetCallQueueInternal() )
		{
			g_pShaderAPI->ReleaseThreadOwnership();
		}
	}
	g_MatSysMutex.Unlock();
}

//-----------------------------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------------------------
CMatCallQueue *CMaterialSystem::GetRenderCallQueue()
{
	IMatRenderContextInternal *pRenderContext = m_pRenderContext.Get(); 
	return pRenderContext ? pRenderContext->GetCallQueueInternal() : NULL;
}

void CMaterialSystem::UnbindMaterial( IMaterial *pMaterial )
{
	Assert( (pMaterial == NULL) || ((IMaterialInternal *)pMaterial)->IsRealTimeVersion() );
	if ( m_HardwareRenderContext.GetCurrentMaterial() == pMaterial )
	{
		m_HardwareRenderContext.Bind( g_pErrorMaterial, NULL );
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void CMaterialSystem::CompactMemory()
{
	for ( int i = 0; i < ARRAYSIZE(m_QueuedRenderContexts); i++)
	{
		m_QueuedRenderContexts[i].CompactMemory();
	}
}

//-----------------------------------------------------------------------------
// Material + texture related commands
//-----------------------------------------------------------------------------
void CMaterialSystem::DebugPrintUsedMaterials( const CCommand &args )
{
	if( args.ArgC() == 1 )
	{
		DebugPrintUsedMaterials( NULL, false );
	}
	else
	{
		DebugPrintUsedMaterials( args[ 1 ], false );
	}
}

void CMaterialSystem::DebugPrintUsedMaterialsVerbose( const CCommand &args )
{
	if( args.ArgC() == 1 )
	{
		DebugPrintUsedMaterials( NULL, true );
	}
	else
	{
		DebugPrintUsedMaterials( args[ 1 ], true );
	}
}

void CMaterialSystem::DebugPrintUsedTextures( const CCommand &args )
{
	DebugPrintUsedTextures();
}

#if defined( _X360 )
void CMaterialSystem::ListUsedMaterials( const CCommand &args )
{
	ListUsedMaterials();
}
#endif // !_X360

void CMaterialSystem::ReloadAllMaterials( const CCommand &args )
{
	ReloadMaterials( NULL );
}

void CMaterialSystem::ReloadMaterials( const CCommand &args )
{
	if( args.ArgC() != 2 )
	{
		ConWarning( "Usage: mat_reloadmaterial material_name_substring\n"
					"   or  mat_reloadmaterial substring1*substring2*...*substringN\n" );
		return;
	}
	ReloadMaterials( args[ 1 ] );
}

void CMaterialSystem::ReloadTextures( const CCommand &args )
{
	ReloadTextures();
}

CON_COMMAND( mat_hdr_enabled, "Report if HDR is enabled for debugging" )
{
	if( HardwareConfig() && HardwareConfig()->GetHDREnabled() )
	{
		Warning( "HDR Enabled\n" );
	}
	else
	{
		Warning( "HDR Disabled\n" );
	}
}

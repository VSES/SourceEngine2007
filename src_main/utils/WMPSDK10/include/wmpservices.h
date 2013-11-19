

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 6.00.0365 */
/* Compiler settings for wmpservices.idl:
    Oicf, W1, Zp8, env=Win32 (32b run)
    protocol : dce , ms_ext, c_ext, robust
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
//@@MIDL_FILE_HEADING(  )

#pragma warning( disable: 4049 )  /* more than 64k source lines */


/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 475
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __RPCNDR_H_VERSION__
#error this stub requires an updated version of <rpcndr.h>
#endif // __RPCNDR_H_VERSION__

#ifndef COM_NO_WINDOWS_H
#include "windows.h"
#include "ole2.h"
#endif /*COM_NO_WINDOWS_H*/

#ifndef __wmpservices_h__
#define __wmpservices_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __IWMPServices_FWD_DEFINED__
#define __IWMPServices_FWD_DEFINED__
typedef interface IWMPServices IWMPServices;
#endif 	/* __IWMPServices_FWD_DEFINED__ */


#ifndef __IWMPMediaPluginRegistrar_FWD_DEFINED__
#define __IWMPMediaPluginRegistrar_FWD_DEFINED__
typedef interface IWMPMediaPluginRegistrar IWMPMediaPluginRegistrar;
#endif 	/* __IWMPMediaPluginRegistrar_FWD_DEFINED__ */


#ifndef __IWMPPlugin_FWD_DEFINED__
#define __IWMPPlugin_FWD_DEFINED__
typedef interface IWMPPlugin IWMPPlugin;
#endif 	/* __IWMPPlugin_FWD_DEFINED__ */


#ifndef __IWMPPluginEnable_FWD_DEFINED__
#define __IWMPPluginEnable_FWD_DEFINED__
typedef interface IWMPPluginEnable IWMPPluginEnable;
#endif 	/* __IWMPPluginEnable_FWD_DEFINED__ */


#ifndef __IWMPGraphCreation_FWD_DEFINED__
#define __IWMPGraphCreation_FWD_DEFINED__
typedef interface IWMPGraphCreation IWMPGraphCreation;
#endif 	/* __IWMPGraphCreation_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"

#ifdef __cplusplus
extern "C"{
#endif 

void * __RPC_USER MIDL_user_allocate(size_t);
void __RPC_USER MIDL_user_free( void * ); 

/* interface __MIDL_itf_wmpservices_0000 */
/* [local] */ 

//=========================================================================
//
// Microsoft Windows Media Technologies
// Copyright (C) Microsoft Corporation. All rights reserved.
//
//=========================================================================
typedef /* [public][helpstring] */ 
enum WMPServices_StreamState
    {	WMPServices_StreamState_Stop	= 0,
	WMPServices_StreamState_Pause	= WMPServices_StreamState_Stop + 1,
	WMPServices_StreamState_Play	= WMPServices_StreamState_Pause + 1
    } 	WMPServices_StreamState;



extern RPC_IF_HANDLE __MIDL_itf_wmpservices_0000_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_wmpservices_0000_v0_0_s_ifspec;

#ifndef __IWMPServices_INTERFACE_DEFINED__
#define __IWMPServices_INTERFACE_DEFINED__

/* interface IWMPServices */
/* [unique][helpstring][uuid][object] */ 


EXTERN_C const IID IID_IWMPServices;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("AFB6B76B-1E20-4198-83B3-191DB6E0B149")
    IWMPServices : public IUnknown
    {
    public:
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE GetStreamTime( 
            /* [retval][out] */ LONGLONG *prt) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE GetStreamState( 
            /* [retval][out] */ WMPServices_StreamState *pState) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IWMPServicesVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IWMPServices * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IWMPServices * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IWMPServices * This);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *GetStreamTime )( 
            IWMPServices * This,
            /* [retval][out] */ LONGLONG *prt);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *GetStreamState )( 
            IWMPServices * This,
            /* [retval][out] */ WMPServices_StreamState *pState);
        
        END_INTERFACE
    } IWMPServicesVtbl;

    interface IWMPServices
    {
        CONST_VTBL struct IWMPServicesVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IWMPServices_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IWMPServices_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IWMPServices_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IWMPServices_GetStreamTime(This,prt)	\
    (This)->lpVtbl -> GetStreamTime(This,prt)

#define IWMPServices_GetStreamState(This,pState)	\
    (This)->lpVtbl -> GetStreamState(This,pState)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring] */ HRESULT STDMETHODCALLTYPE IWMPServices_GetStreamTime_Proxy( 
    IWMPServices * This,
    /* [retval][out] */ LONGLONG *prt);


void __RPC_STUB IWMPServices_GetStreamTime_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IWMPServices_GetStreamState_Proxy( 
    IWMPServices * This,
    /* [retval][out] */ WMPServices_StreamState *pState);


void __RPC_STUB IWMPServices_GetStreamState_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IWMPServices_INTERFACE_DEFINED__ */


/* interface __MIDL_itf_wmpservices_0262 */
/* [local] */ 

EXTERN_GUID( CLSID_WMPMediaPluginRegistrar, 0x5569e7f5,0x424b,0x4b93, 0x89, 0xca, 0x79, 0xd1, 0x79, 0x24, 0x68, 0x9a );
EXTERN_GUID( WMP_PLUGINTYPE_DSP, 0x6434baea, 0x4954, 0x498d, 0xab, 0xd5, 0x2b, 0x7, 0x12, 0x3e, 0x1f, 0x4 );
EXTERN_GUID( WMP_PLUGINTYPE_RENDERING, 0xa8554541, 0x115d, 0x406a, 0xa4, 0xc7, 0x51, 0x11, 0x1c, 0x33, 0x1, 0x83 );


extern RPC_IF_HANDLE __MIDL_itf_wmpservices_0262_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_wmpservices_0262_v0_0_s_ifspec;

#ifndef __IWMPMediaPluginRegistrar_INTERFACE_DEFINED__
#define __IWMPMediaPluginRegistrar_INTERFACE_DEFINED__

/* interface IWMPMediaPluginRegistrar */
/* [unique][helpstring][uuid][object][local] */ 


EXTERN_C const IID IID_IWMPMediaPluginRegistrar;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("68E27045-05BD-40b2-9720-23088C78E390")
    IWMPMediaPluginRegistrar : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE WMPRegisterPlayerPlugin( 
            LPWSTR pwszFriendlyName,
            LPWSTR pwszDescription,
            LPWSTR pwszUninstallString,
            DWORD dwPriority,
            GUID guidPluginType,
            CLSID clsid,
            UINT cMediaTypes,
            LPVOID pMediaTypes) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE WMPUnRegisterPlayerPlugin( 
            GUID guidPluginType,
            CLSID clsid) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IWMPMediaPluginRegistrarVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IWMPMediaPluginRegistrar * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IWMPMediaPluginRegistrar * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IWMPMediaPluginRegistrar * This);
        
        HRESULT ( STDMETHODCALLTYPE *WMPRegisterPlayerPlugin )( 
            IWMPMediaPluginRegistrar * This,
            LPWSTR pwszFriendlyName,
            LPWSTR pwszDescription,
            LPWSTR pwszUninstallString,
            DWORD dwPriority,
            GUID guidPluginType,
            CLSID clsid,
            UINT cMediaTypes,
            LPVOID pMediaTypes);
        
        HRESULT ( STDMETHODCALLTYPE *WMPUnRegisterPlayerPlugin )( 
            IWMPMediaPluginRegistrar * This,
            GUID guidPluginType,
            CLSID clsid);
        
        END_INTERFACE
    } IWMPMediaPluginRegistrarVtbl;

    interface IWMPMediaPluginRegistrar
    {
        CONST_VTBL struct IWMPMediaPluginRegistrarVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IWMPMediaPluginRegistrar_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IWMPMediaPluginRegistrar_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IWMPMediaPluginRegistrar_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IWMPMediaPluginRegistrar_WMPRegisterPlayerPlugin(This,pwszFriendlyName,pwszDescription,pwszUninstallString,dwPriority,guidPluginType,clsid,cMediaTypes,pMediaTypes)	\
    (This)->lpVtbl -> WMPRegisterPlayerPlugin(This,pwszFriendlyName,pwszDescription,pwszUninstallString,dwPriority,guidPluginType,clsid,cMediaTypes,pMediaTypes)

#define IWMPMediaPluginRegistrar_WMPUnRegisterPlayerPlugin(This,guidPluginType,clsid)	\
    (This)->lpVtbl -> WMPUnRegisterPlayerPlugin(This,guidPluginType,clsid)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE IWMPMediaPluginRegistrar_WMPRegisterPlayerPlugin_Proxy( 
    IWMPMediaPluginRegistrar * This,
    LPWSTR pwszFriendlyName,
    LPWSTR pwszDescription,
    LPWSTR pwszUninstallString,
    DWORD dwPriority,
    GUID guidPluginType,
    CLSID clsid,
    UINT cMediaTypes,
    LPVOID pMediaTypes);


void __RPC_STUB IWMPMediaPluginRegistrar_WMPRegisterPlayerPlugin_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IWMPMediaPluginRegistrar_WMPUnRegisterPlayerPlugin_Proxy( 
    IWMPMediaPluginRegistrar * This,
    GUID guidPluginType,
    CLSID clsid);


void __RPC_STUB IWMPMediaPluginRegistrar_WMPUnRegisterPlayerPlugin_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IWMPMediaPluginRegistrar_INTERFACE_DEFINED__ */


/* interface __MIDL_itf_wmpservices_0263 */
/* [local] */ 

typedef /* [public][helpstring] */ 
enum WMPPlugin_Caps
    {	WMPPlugin_Caps_CannotConvertFormats	= 1
    } 	WMPPlugin_Caps;



extern RPC_IF_HANDLE __MIDL_itf_wmpservices_0263_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_wmpservices_0263_v0_0_s_ifspec;

#ifndef __IWMPPlugin_INTERFACE_DEFINED__
#define __IWMPPlugin_INTERFACE_DEFINED__

/* interface IWMPPlugin */
/* [unique][helpstring][uuid][object] */ 


EXTERN_C const IID IID_IWMPPlugin;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("f1392a70-024c-42bb-a998-73dfdfe7d5a7")
    IWMPPlugin : public IUnknown
    {
    public:
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE Init( 
            DWORD dwPlaybackContext) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE Shutdown( void) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE GetID( 
            /* [retval][out] */ GUID *pGUID) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE GetCaps( 
            /* [retval][out] */ DWORD *pdwFlags) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE AdviseWMPServices( 
            /* [in] */ IWMPServices *pWMPServices) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE UnAdviseWMPServices( void) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IWMPPluginVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IWMPPlugin * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IWMPPlugin * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IWMPPlugin * This);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *Init )( 
            IWMPPlugin * This,
            DWORD dwPlaybackContext);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *Shutdown )( 
            IWMPPlugin * This);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *GetID )( 
            IWMPPlugin * This,
            /* [retval][out] */ GUID *pGUID);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *GetCaps )( 
            IWMPPlugin * This,
            /* [retval][out] */ DWORD *pdwFlags);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *AdviseWMPServices )( 
            IWMPPlugin * This,
            /* [in] */ IWMPServices *pWMPServices);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *UnAdviseWMPServices )( 
            IWMPPlugin * This);
        
        END_INTERFACE
    } IWMPPluginVtbl;

    interface IWMPPlugin
    {
        CONST_VTBL struct IWMPPluginVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IWMPPlugin_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IWMPPlugin_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IWMPPlugin_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IWMPPlugin_Init(This,dwPlaybackContext)	\
    (This)->lpVtbl -> Init(This,dwPlaybackContext)

#define IWMPPlugin_Shutdown(This)	\
    (This)->lpVtbl -> Shutdown(This)

#define IWMPPlugin_GetID(This,pGUID)	\
    (This)->lpVtbl -> GetID(This,pGUID)

#define IWMPPlugin_GetCaps(This,pdwFlags)	\
    (This)->lpVtbl -> GetCaps(This,pdwFlags)

#define IWMPPlugin_AdviseWMPServices(This,pWMPServices)	\
    (This)->lpVtbl -> AdviseWMPServices(This,pWMPServices)

#define IWMPPlugin_UnAdviseWMPServices(This)	\
    (This)->lpVtbl -> UnAdviseWMPServices(This)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring] */ HRESULT STDMETHODCALLTYPE IWMPPlugin_Init_Proxy( 
    IWMPPlugin * This,
    DWORD dwPlaybackContext);


void __RPC_STUB IWMPPlugin_Init_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IWMPPlugin_Shutdown_Proxy( 
    IWMPPlugin * This);


void __RPC_STUB IWMPPlugin_Shutdown_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IWMPPlugin_GetID_Proxy( 
    IWMPPlugin * This,
    /* [retval][out] */ GUID *pGUID);


void __RPC_STUB IWMPPlugin_GetID_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IWMPPlugin_GetCaps_Proxy( 
    IWMPPlugin * This,
    /* [retval][out] */ DWORD *pdwFlags);


void __RPC_STUB IWMPPlugin_GetCaps_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IWMPPlugin_AdviseWMPServices_Proxy( 
    IWMPPlugin * This,
    /* [in] */ IWMPServices *pWMPServices);


void __RPC_STUB IWMPPlugin_AdviseWMPServices_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IWMPPlugin_UnAdviseWMPServices_Proxy( 
    IWMPPlugin * This);


void __RPC_STUB IWMPPlugin_UnAdviseWMPServices_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IWMPPlugin_INTERFACE_DEFINED__ */


#ifndef __IWMPPluginEnable_INTERFACE_DEFINED__
#define __IWMPPluginEnable_INTERFACE_DEFINED__

/* interface IWMPPluginEnable */
/* [unique][helpstring][uuid][object] */ 


EXTERN_C const IID IID_IWMPPluginEnable;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("5FCA444C-7AD1-479d-A4EF-40566A5309D6")
    IWMPPluginEnable : public IUnknown
    {
    public:
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE SetEnable( 
            /* [in] */ BOOL fEnable) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE GetEnable( 
            /* [out] */ BOOL *pfEnable) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IWMPPluginEnableVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IWMPPluginEnable * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IWMPPluginEnable * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IWMPPluginEnable * This);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *SetEnable )( 
            IWMPPluginEnable * This,
            /* [in] */ BOOL fEnable);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *GetEnable )( 
            IWMPPluginEnable * This,
            /* [out] */ BOOL *pfEnable);
        
        END_INTERFACE
    } IWMPPluginEnableVtbl;

    interface IWMPPluginEnable
    {
        CONST_VTBL struct IWMPPluginEnableVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IWMPPluginEnable_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IWMPPluginEnable_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IWMPPluginEnable_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IWMPPluginEnable_SetEnable(This,fEnable)	\
    (This)->lpVtbl -> SetEnable(This,fEnable)

#define IWMPPluginEnable_GetEnable(This,pfEnable)	\
    (This)->lpVtbl -> GetEnable(This,pfEnable)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring] */ HRESULT STDMETHODCALLTYPE IWMPPluginEnable_SetEnable_Proxy( 
    IWMPPluginEnable * This,
    /* [in] */ BOOL fEnable);


void __RPC_STUB IWMPPluginEnable_SetEnable_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IWMPPluginEnable_GetEnable_Proxy( 
    IWMPPluginEnable * This,
    /* [out] */ BOOL *pfEnable);


void __RPC_STUB IWMPPluginEnable_GetEnable_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IWMPPluginEnable_INTERFACE_DEFINED__ */


#ifndef __IWMPGraphCreation_INTERFACE_DEFINED__
#define __IWMPGraphCreation_INTERFACE_DEFINED__

/* interface IWMPGraphCreation */
/* [unique][helpstring][uuid][object] */ 


EXTERN_C const IID IID_IWMPGraphCreation;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("bfb377e5-c594-4369-a970-de896d5ece74")
    IWMPGraphCreation : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE GraphCreationPreRender( 
            /* [in] */ IUnknown *pFilterGraph,
            /* [in] */ IUnknown *pReserved) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GraphCreationPostRender( 
            /* [in] */ IUnknown *pFilterGraph) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetGraphCreationFlags( 
            /* [out] */ DWORD *pdwFlags) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IWMPGraphCreationVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IWMPGraphCreation * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IWMPGraphCreation * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IWMPGraphCreation * This);
        
        HRESULT ( STDMETHODCALLTYPE *GraphCreationPreRender )( 
            IWMPGraphCreation * This,
            /* [in] */ IUnknown *pFilterGraph,
            /* [in] */ IUnknown *pReserved);
        
        HRESULT ( STDMETHODCALLTYPE *GraphCreationPostRender )( 
            IWMPGraphCreation * This,
            /* [in] */ IUnknown *pFilterGraph);
        
        HRESULT ( STDMETHODCALLTYPE *GetGraphCreationFlags )( 
            IWMPGraphCreation * This,
            /* [out] */ DWORD *pdwFlags);
        
        END_INTERFACE
    } IWMPGraphCreationVtbl;

    interface IWMPGraphCreation
    {
        CONST_VTBL struct IWMPGraphCreationVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IWMPGraphCreation_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IWMPGraphCreation_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IWMPGraphCreation_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IWMPGraphCreation_GraphCreationPreRender(This,pFilterGraph,pReserved)	\
    (This)->lpVtbl -> GraphCreationPreRender(This,pFilterGraph,pReserved)

#define IWMPGraphCreation_GraphCreationPostRender(This,pFilterGraph)	\
    (This)->lpVtbl -> GraphCreationPostRender(This,pFilterGraph)

#define IWMPGraphCreation_GetGraphCreationFlags(This,pdwFlags)	\
    (This)->lpVtbl -> GetGraphCreationFlags(This,pdwFlags)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE IWMPGraphCreation_GraphCreationPreRender_Proxy( 
    IWMPGraphCreation * This,
    /* [in] */ IUnknown *pFilterGraph,
    /* [in] */ IUnknown *pReserved);


void __RPC_STUB IWMPGraphCreation_GraphCreationPreRender_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IWMPGraphCreation_GraphCreationPostRender_Proxy( 
    IWMPGraphCreation * This,
    /* [in] */ IUnknown *pFilterGraph);


void __RPC_STUB IWMPGraphCreation_GraphCreationPostRender_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IWMPGraphCreation_GetGraphCreationFlags_Proxy( 
    IWMPGraphCreation * This,
    /* [out] */ DWORD *pdwFlags);


void __RPC_STUB IWMPGraphCreation_GetGraphCreationFlags_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IWMPGraphCreation_INTERFACE_DEFINED__ */


/* interface __MIDL_itf_wmpservices_0266 */
/* [local] */ 

#define WMPGC_FLAGS_ALLOW_PREROLL    0x00000001
#define WMPGC_FLAGS_SUPPRESS_DIALOGS 0x00000002
#define WMPGC_FLAGS_IGNORE_AV_SYNC   0x00000004
#define WMPGC_FLAGS_DISABLE_PLUGINS  0x00000008


extern RPC_IF_HANDLE __MIDL_itf_wmpservices_0266_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_wmpservices_0266_v0_0_s_ifspec;

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif



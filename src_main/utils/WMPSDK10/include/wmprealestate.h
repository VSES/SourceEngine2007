

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 6.00.0365 */
/* Compiler settings for wmprealestate.idl:
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

#ifndef __wmprealestate_h__
#define __wmprealestate_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __IWMPNodeRealEstate_FWD_DEFINED__
#define __IWMPNodeRealEstate_FWD_DEFINED__
typedef interface IWMPNodeRealEstate IWMPNodeRealEstate;
#endif 	/* __IWMPNodeRealEstate_FWD_DEFINED__ */


#ifndef __IWMPNodeRealEstateHost_FWD_DEFINED__
#define __IWMPNodeRealEstateHost_FWD_DEFINED__
typedef interface IWMPNodeRealEstateHost IWMPNodeRealEstateHost;
#endif 	/* __IWMPNodeRealEstateHost_FWD_DEFINED__ */


#ifndef __IWMPNodeWindowed_FWD_DEFINED__
#define __IWMPNodeWindowed_FWD_DEFINED__
typedef interface IWMPNodeWindowed IWMPNodeWindowed;
#endif 	/* __IWMPNodeWindowed_FWD_DEFINED__ */


#ifndef __IWMPNodeWindowedHost_FWD_DEFINED__
#define __IWMPNodeWindowedHost_FWD_DEFINED__
typedef interface IWMPNodeWindowedHost IWMPNodeWindowedHost;
#endif 	/* __IWMPNodeWindowedHost_FWD_DEFINED__ */


#ifndef __IWMPWindowMessageSink_FWD_DEFINED__
#define __IWMPWindowMessageSink_FWD_DEFINED__
typedef interface IWMPWindowMessageSink IWMPWindowMessageSink;
#endif 	/* __IWMPWindowMessageSink_FWD_DEFINED__ */


#ifndef __IWMPNodeWindowless_FWD_DEFINED__
#define __IWMPNodeWindowless_FWD_DEFINED__
typedef interface IWMPNodeWindowless IWMPNodeWindowless;
#endif 	/* __IWMPNodeWindowless_FWD_DEFINED__ */


#ifndef __IWMPNodeWindowlessHost_FWD_DEFINED__
#define __IWMPNodeWindowlessHost_FWD_DEFINED__
typedef interface IWMPNodeWindowlessHost IWMPNodeWindowlessHost;
#endif 	/* __IWMPNodeWindowlessHost_FWD_DEFINED__ */


/* header files for imported files */
#include "unknwn.h"
#include "ocidl.h"

#ifdef __cplusplus
extern "C"{
#endif 

void * __RPC_USER MIDL_user_allocate(size_t);
void __RPC_USER MIDL_user_free( void * ); 

/* interface __MIDL_itf_wmprealestate_0000 */
/* [local] */ 

//=========================================================================
//
// Microsoft Windows Media Technologies
// Copyright (C) Microsoft Corporation. All rights reserved.
//
//=========================================================================


extern RPC_IF_HANDLE __MIDL_itf_wmprealestate_0000_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_wmprealestate_0000_v0_0_s_ifspec;

#ifndef __IWMPNodeRealEstate_INTERFACE_DEFINED__
#define __IWMPNodeRealEstate_INTERFACE_DEFINED__

/* interface IWMPNodeRealEstate */
/* [oleautomation][helpstring][uuid][object] */ 


EXTERN_C const IID IID_IWMPNodeRealEstate;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("42751198-5A50-4460-BCB4-709F8BDC8E59")
    IWMPNodeRealEstate : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE GetDesiredSize( 
            /* [out] */ LPSIZE pSize) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SetRects( 
            /* [in] */ const RECT *pSrc,
            /* [in] */ const RECT *pDest,
            /* [in] */ const RECT *pClip) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetRects( 
            /* [out] */ RECT *pSrc,
            /* [out] */ RECT *pDest,
            /* [out] */ RECT *pClip) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SetWindowless( 
            /* [in] */ BOOL fWindowless) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetWindowless( 
            /* [out] */ BOOL *pfWindowless) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SetFullScreen( 
            /* [in] */ BOOL fFullScreen) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetFullScreen( 
            /* [out] */ BOOL *pfFullScreen) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IWMPNodeRealEstateVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IWMPNodeRealEstate * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IWMPNodeRealEstate * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IWMPNodeRealEstate * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetDesiredSize )( 
            IWMPNodeRealEstate * This,
            /* [out] */ LPSIZE pSize);
        
        HRESULT ( STDMETHODCALLTYPE *SetRects )( 
            IWMPNodeRealEstate * This,
            /* [in] */ const RECT *pSrc,
            /* [in] */ const RECT *pDest,
            /* [in] */ const RECT *pClip);
        
        HRESULT ( STDMETHODCALLTYPE *GetRects )( 
            IWMPNodeRealEstate * This,
            /* [out] */ RECT *pSrc,
            /* [out] */ RECT *pDest,
            /* [out] */ RECT *pClip);
        
        HRESULT ( STDMETHODCALLTYPE *SetWindowless )( 
            IWMPNodeRealEstate * This,
            /* [in] */ BOOL fWindowless);
        
        HRESULT ( STDMETHODCALLTYPE *GetWindowless )( 
            IWMPNodeRealEstate * This,
            /* [out] */ BOOL *pfWindowless);
        
        HRESULT ( STDMETHODCALLTYPE *SetFullScreen )( 
            IWMPNodeRealEstate * This,
            /* [in] */ BOOL fFullScreen);
        
        HRESULT ( STDMETHODCALLTYPE *GetFullScreen )( 
            IWMPNodeRealEstate * This,
            /* [out] */ BOOL *pfFullScreen);
        
        END_INTERFACE
    } IWMPNodeRealEstateVtbl;

    interface IWMPNodeRealEstate
    {
        CONST_VTBL struct IWMPNodeRealEstateVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IWMPNodeRealEstate_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IWMPNodeRealEstate_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IWMPNodeRealEstate_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IWMPNodeRealEstate_GetDesiredSize(This,pSize)	\
    (This)->lpVtbl -> GetDesiredSize(This,pSize)

#define IWMPNodeRealEstate_SetRects(This,pSrc,pDest,pClip)	\
    (This)->lpVtbl -> SetRects(This,pSrc,pDest,pClip)

#define IWMPNodeRealEstate_GetRects(This,pSrc,pDest,pClip)	\
    (This)->lpVtbl -> GetRects(This,pSrc,pDest,pClip)

#define IWMPNodeRealEstate_SetWindowless(This,fWindowless)	\
    (This)->lpVtbl -> SetWindowless(This,fWindowless)

#define IWMPNodeRealEstate_GetWindowless(This,pfWindowless)	\
    (This)->lpVtbl -> GetWindowless(This,pfWindowless)

#define IWMPNodeRealEstate_SetFullScreen(This,fFullScreen)	\
    (This)->lpVtbl -> SetFullScreen(This,fFullScreen)

#define IWMPNodeRealEstate_GetFullScreen(This,pfFullScreen)	\
    (This)->lpVtbl -> GetFullScreen(This,pfFullScreen)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE IWMPNodeRealEstate_GetDesiredSize_Proxy( 
    IWMPNodeRealEstate * This,
    /* [out] */ LPSIZE pSize);


void __RPC_STUB IWMPNodeRealEstate_GetDesiredSize_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IWMPNodeRealEstate_SetRects_Proxy( 
    IWMPNodeRealEstate * This,
    /* [in] */ const RECT *pSrc,
    /* [in] */ const RECT *pDest,
    /* [in] */ const RECT *pClip);


void __RPC_STUB IWMPNodeRealEstate_SetRects_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IWMPNodeRealEstate_GetRects_Proxy( 
    IWMPNodeRealEstate * This,
    /* [out] */ RECT *pSrc,
    /* [out] */ RECT *pDest,
    /* [out] */ RECT *pClip);


void __RPC_STUB IWMPNodeRealEstate_GetRects_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IWMPNodeRealEstate_SetWindowless_Proxy( 
    IWMPNodeRealEstate * This,
    /* [in] */ BOOL fWindowless);


void __RPC_STUB IWMPNodeRealEstate_SetWindowless_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IWMPNodeRealEstate_GetWindowless_Proxy( 
    IWMPNodeRealEstate * This,
    /* [out] */ BOOL *pfWindowless);


void __RPC_STUB IWMPNodeRealEstate_GetWindowless_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IWMPNodeRealEstate_SetFullScreen_Proxy( 
    IWMPNodeRealEstate * This,
    /* [in] */ BOOL fFullScreen);


void __RPC_STUB IWMPNodeRealEstate_SetFullScreen_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IWMPNodeRealEstate_GetFullScreen_Proxy( 
    IWMPNodeRealEstate * This,
    /* [out] */ BOOL *pfFullScreen);


void __RPC_STUB IWMPNodeRealEstate_GetFullScreen_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IWMPNodeRealEstate_INTERFACE_DEFINED__ */


#ifndef __IWMPNodeRealEstateHost_INTERFACE_DEFINED__
#define __IWMPNodeRealEstateHost_INTERFACE_DEFINED__

/* interface IWMPNodeRealEstateHost */
/* [oleautomation][helpstring][uuid][object] */ 


EXTERN_C const IID IID_IWMPNodeRealEstateHost;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("1491087D-2C6B-44c8-B019-B3C929D2ADA9")
    IWMPNodeRealEstateHost : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE OnDesiredSizeChange( 
            /* [in] */ LPSIZE pSize) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE OnFullScreenTransition( 
            /* [in] */ BOOL fFullScreen) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IWMPNodeRealEstateHostVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IWMPNodeRealEstateHost * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IWMPNodeRealEstateHost * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IWMPNodeRealEstateHost * This);
        
        HRESULT ( STDMETHODCALLTYPE *OnDesiredSizeChange )( 
            IWMPNodeRealEstateHost * This,
            /* [in] */ LPSIZE pSize);
        
        HRESULT ( STDMETHODCALLTYPE *OnFullScreenTransition )( 
            IWMPNodeRealEstateHost * This,
            /* [in] */ BOOL fFullScreen);
        
        END_INTERFACE
    } IWMPNodeRealEstateHostVtbl;

    interface IWMPNodeRealEstateHost
    {
        CONST_VTBL struct IWMPNodeRealEstateHostVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IWMPNodeRealEstateHost_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IWMPNodeRealEstateHost_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IWMPNodeRealEstateHost_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IWMPNodeRealEstateHost_OnDesiredSizeChange(This,pSize)	\
    (This)->lpVtbl -> OnDesiredSizeChange(This,pSize)

#define IWMPNodeRealEstateHost_OnFullScreenTransition(This,fFullScreen)	\
    (This)->lpVtbl -> OnFullScreenTransition(This,fFullScreen)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE IWMPNodeRealEstateHost_OnDesiredSizeChange_Proxy( 
    IWMPNodeRealEstateHost * This,
    /* [in] */ LPSIZE pSize);


void __RPC_STUB IWMPNodeRealEstateHost_OnDesiredSizeChange_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IWMPNodeRealEstateHost_OnFullScreenTransition_Proxy( 
    IWMPNodeRealEstateHost * This,
    /* [in] */ BOOL fFullScreen);


void __RPC_STUB IWMPNodeRealEstateHost_OnFullScreenTransition_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IWMPNodeRealEstateHost_INTERFACE_DEFINED__ */


/* interface __MIDL_itf_wmprealestate_0263 */
/* [local] */ 

typedef OLE_HANDLE OLE_HWND;



extern RPC_IF_HANDLE __MIDL_itf_wmprealestate_0263_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_wmprealestate_0263_v0_0_s_ifspec;

#ifndef __IWMPNodeWindowed_INTERFACE_DEFINED__
#define __IWMPNodeWindowed_INTERFACE_DEFINED__

/* interface IWMPNodeWindowed */
/* [oleautomation][helpstring][uuid][object] */ 


EXTERN_C const IID IID_IWMPNodeWindowed;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("96740BFA-C56A-45d1-A3A4-762914D4ADE9")
    IWMPNodeWindowed : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE SetOwnerWindow( 
            /* [in] */ OLE_HWND hwnd) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetOwnerWindow( 
            /* [out] */ OLE_HWND *phwnd) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IWMPNodeWindowedVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IWMPNodeWindowed * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IWMPNodeWindowed * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IWMPNodeWindowed * This);
        
        HRESULT ( STDMETHODCALLTYPE *SetOwnerWindow )( 
            IWMPNodeWindowed * This,
            /* [in] */ OLE_HWND hwnd);
        
        HRESULT ( STDMETHODCALLTYPE *GetOwnerWindow )( 
            IWMPNodeWindowed * This,
            /* [out] */ OLE_HWND *phwnd);
        
        END_INTERFACE
    } IWMPNodeWindowedVtbl;

    interface IWMPNodeWindowed
    {
        CONST_VTBL struct IWMPNodeWindowedVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IWMPNodeWindowed_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IWMPNodeWindowed_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IWMPNodeWindowed_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IWMPNodeWindowed_SetOwnerWindow(This,hwnd)	\
    (This)->lpVtbl -> SetOwnerWindow(This,hwnd)

#define IWMPNodeWindowed_GetOwnerWindow(This,phwnd)	\
    (This)->lpVtbl -> GetOwnerWindow(This,phwnd)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE IWMPNodeWindowed_SetOwnerWindow_Proxy( 
    IWMPNodeWindowed * This,
    /* [in] */ OLE_HWND hwnd);


void __RPC_STUB IWMPNodeWindowed_SetOwnerWindow_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IWMPNodeWindowed_GetOwnerWindow_Proxy( 
    IWMPNodeWindowed * This,
    /* [out] */ OLE_HWND *phwnd);


void __RPC_STUB IWMPNodeWindowed_GetOwnerWindow_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IWMPNodeWindowed_INTERFACE_DEFINED__ */


#ifndef __IWMPNodeWindowedHost_INTERFACE_DEFINED__
#define __IWMPNodeWindowedHost_INTERFACE_DEFINED__

/* interface IWMPNodeWindowedHost */
/* [oleautomation][helpstring][uuid][object] */ 


EXTERN_C const IID IID_IWMPNodeWindowedHost;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("A300415A-54AA-4081-ADBF-3B13610D8958")
    IWMPNodeWindowedHost : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE OnWindowMessageFromRenderer( 
            /* [in] */ UINT uMsg,
            /* [in] */ WPARAM wparam,
            /* [in] */ LPARAM lparam,
            /* [out] */ LRESULT *plRet,
            /* [out] */ BOOL *pfHandled) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IWMPNodeWindowedHostVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IWMPNodeWindowedHost * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IWMPNodeWindowedHost * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IWMPNodeWindowedHost * This);
        
        HRESULT ( STDMETHODCALLTYPE *OnWindowMessageFromRenderer )( 
            IWMPNodeWindowedHost * This,
            /* [in] */ UINT uMsg,
            /* [in] */ WPARAM wparam,
            /* [in] */ LPARAM lparam,
            /* [out] */ LRESULT *plRet,
            /* [out] */ BOOL *pfHandled);
        
        END_INTERFACE
    } IWMPNodeWindowedHostVtbl;

    interface IWMPNodeWindowedHost
    {
        CONST_VTBL struct IWMPNodeWindowedHostVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IWMPNodeWindowedHost_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IWMPNodeWindowedHost_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IWMPNodeWindowedHost_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IWMPNodeWindowedHost_OnWindowMessageFromRenderer(This,uMsg,wparam,lparam,plRet,pfHandled)	\
    (This)->lpVtbl -> OnWindowMessageFromRenderer(This,uMsg,wparam,lparam,plRet,pfHandled)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE IWMPNodeWindowedHost_OnWindowMessageFromRenderer_Proxy( 
    IWMPNodeWindowedHost * This,
    /* [in] */ UINT uMsg,
    /* [in] */ WPARAM wparam,
    /* [in] */ LPARAM lparam,
    /* [out] */ LRESULT *plRet,
    /* [out] */ BOOL *pfHandled);


void __RPC_STUB IWMPNodeWindowedHost_OnWindowMessageFromRenderer_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IWMPNodeWindowedHost_INTERFACE_DEFINED__ */


#ifndef __IWMPWindowMessageSink_INTERFACE_DEFINED__
#define __IWMPWindowMessageSink_INTERFACE_DEFINED__

/* interface IWMPWindowMessageSink */
/* [oleautomation][helpstring][uuid][object] */ 


EXTERN_C const IID IID_IWMPWindowMessageSink;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("3A0DAA30-908D-4789-BA87-AED879B5C49B")
    IWMPWindowMessageSink : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE OnWindowMessage( 
            UINT uMsg,
            WPARAM wparam,
            LPARAM lparam,
            LRESULT *plRet,
            BOOL *pfHandled) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IWMPWindowMessageSinkVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IWMPWindowMessageSink * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IWMPWindowMessageSink * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IWMPWindowMessageSink * This);
        
        HRESULT ( STDMETHODCALLTYPE *OnWindowMessage )( 
            IWMPWindowMessageSink * This,
            UINT uMsg,
            WPARAM wparam,
            LPARAM lparam,
            LRESULT *plRet,
            BOOL *pfHandled);
        
        END_INTERFACE
    } IWMPWindowMessageSinkVtbl;

    interface IWMPWindowMessageSink
    {
        CONST_VTBL struct IWMPWindowMessageSinkVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IWMPWindowMessageSink_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IWMPWindowMessageSink_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IWMPWindowMessageSink_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IWMPWindowMessageSink_OnWindowMessage(This,uMsg,wparam,lparam,plRet,pfHandled)	\
    (This)->lpVtbl -> OnWindowMessage(This,uMsg,wparam,lparam,plRet,pfHandled)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE IWMPWindowMessageSink_OnWindowMessage_Proxy( 
    IWMPWindowMessageSink * This,
    UINT uMsg,
    WPARAM wparam,
    LPARAM lparam,
    LRESULT *plRet,
    BOOL *pfHandled);


void __RPC_STUB IWMPWindowMessageSink_OnWindowMessage_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IWMPWindowMessageSink_INTERFACE_DEFINED__ */


/* interface __MIDL_itf_wmprealestate_0266 */
/* [local] */ 

typedef OLE_HANDLE OLE_HDC;



extern RPC_IF_HANDLE __MIDL_itf_wmprealestate_0266_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_wmprealestate_0266_v0_0_s_ifspec;

#ifndef __IWMPNodeWindowless_INTERFACE_DEFINED__
#define __IWMPNodeWindowless_INTERFACE_DEFINED__

/* interface IWMPNodeWindowless */
/* [oleautomation][helpstring][uuid][object] */ 


EXTERN_C const IID IID_IWMPNodeWindowless;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("9B9199AD-780C-4eda-B816-261EBA5D1575")
    IWMPNodeWindowless : public IWMPWindowMessageSink
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE OnDraw( 
            /* [in] */ OLE_HDC hdc,
            /* [in] */ const RECT *prcDraw) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IWMPNodeWindowlessVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IWMPNodeWindowless * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IWMPNodeWindowless * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IWMPNodeWindowless * This);
        
        HRESULT ( STDMETHODCALLTYPE *OnWindowMessage )( 
            IWMPNodeWindowless * This,
            UINT uMsg,
            WPARAM wparam,
            LPARAM lparam,
            LRESULT *plRet,
            BOOL *pfHandled);
        
        HRESULT ( STDMETHODCALLTYPE *OnDraw )( 
            IWMPNodeWindowless * This,
            /* [in] */ OLE_HDC hdc,
            /* [in] */ const RECT *prcDraw);
        
        END_INTERFACE
    } IWMPNodeWindowlessVtbl;

    interface IWMPNodeWindowless
    {
        CONST_VTBL struct IWMPNodeWindowlessVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IWMPNodeWindowless_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IWMPNodeWindowless_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IWMPNodeWindowless_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IWMPNodeWindowless_OnWindowMessage(This,uMsg,wparam,lparam,plRet,pfHandled)	\
    (This)->lpVtbl -> OnWindowMessage(This,uMsg,wparam,lparam,plRet,pfHandled)


#define IWMPNodeWindowless_OnDraw(This,hdc,prcDraw)	\
    (This)->lpVtbl -> OnDraw(This,hdc,prcDraw)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE IWMPNodeWindowless_OnDraw_Proxy( 
    IWMPNodeWindowless * This,
    /* [in] */ OLE_HDC hdc,
    /* [in] */ const RECT *prcDraw);


void __RPC_STUB IWMPNodeWindowless_OnDraw_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IWMPNodeWindowless_INTERFACE_DEFINED__ */


#ifndef __IWMPNodeWindowlessHost_INTERFACE_DEFINED__
#define __IWMPNodeWindowlessHost_INTERFACE_DEFINED__

/* interface IWMPNodeWindowlessHost */
/* [oleautomation][helpstring][uuid][object] */ 


EXTERN_C const IID IID_IWMPNodeWindowlessHost;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("BE7017C6-CE34-4901-8106-770381AA6E3E")
    IWMPNodeWindowlessHost : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE InvalidateRect( 
            /* [in] */ const RECT *prc,
            /* [in] */ BOOL fErase) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IWMPNodeWindowlessHostVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IWMPNodeWindowlessHost * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IWMPNodeWindowlessHost * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IWMPNodeWindowlessHost * This);
        
        HRESULT ( STDMETHODCALLTYPE *InvalidateRect )( 
            IWMPNodeWindowlessHost * This,
            /* [in] */ const RECT *prc,
            /* [in] */ BOOL fErase);
        
        END_INTERFACE
    } IWMPNodeWindowlessHostVtbl;

    interface IWMPNodeWindowlessHost
    {
        CONST_VTBL struct IWMPNodeWindowlessHostVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IWMPNodeWindowlessHost_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IWMPNodeWindowlessHost_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IWMPNodeWindowlessHost_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IWMPNodeWindowlessHost_InvalidateRect(This,prc,fErase)	\
    (This)->lpVtbl -> InvalidateRect(This,prc,fErase)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE IWMPNodeWindowlessHost_InvalidateRect_Proxy( 
    IWMPNodeWindowlessHost * This,
    /* [in] */ const RECT *prc,
    /* [in] */ BOOL fErase);


void __RPC_STUB IWMPNodeWindowlessHost_InvalidateRect_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IWMPNodeWindowlessHost_INTERFACE_DEFINED__ */


/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif



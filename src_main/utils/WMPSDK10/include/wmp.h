

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 6.00.0365 */
/* Compiler settings for wmp.idl:
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

#ifndef __wmp_h__
#define __wmp_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __IWMPErrorItem_FWD_DEFINED__
#define __IWMPErrorItem_FWD_DEFINED__
typedef interface IWMPErrorItem IWMPErrorItem;
#endif 	/* __IWMPErrorItem_FWD_DEFINED__ */


#ifndef __IWMPError_FWD_DEFINED__
#define __IWMPError_FWD_DEFINED__
typedef interface IWMPError IWMPError;
#endif 	/* __IWMPError_FWD_DEFINED__ */


#ifndef __IWMPMedia_FWD_DEFINED__
#define __IWMPMedia_FWD_DEFINED__
typedef interface IWMPMedia IWMPMedia;
#endif 	/* __IWMPMedia_FWD_DEFINED__ */


#ifndef __IWMPControls_FWD_DEFINED__
#define __IWMPControls_FWD_DEFINED__
typedef interface IWMPControls IWMPControls;
#endif 	/* __IWMPControls_FWD_DEFINED__ */


#ifndef __IWMPSettings_FWD_DEFINED__
#define __IWMPSettings_FWD_DEFINED__
typedef interface IWMPSettings IWMPSettings;
#endif 	/* __IWMPSettings_FWD_DEFINED__ */


#ifndef __IWMPClosedCaption_FWD_DEFINED__
#define __IWMPClosedCaption_FWD_DEFINED__
typedef interface IWMPClosedCaption IWMPClosedCaption;
#endif 	/* __IWMPClosedCaption_FWD_DEFINED__ */


#ifndef __IWMPPlaylist_FWD_DEFINED__
#define __IWMPPlaylist_FWD_DEFINED__
typedef interface IWMPPlaylist IWMPPlaylist;
#endif 	/* __IWMPPlaylist_FWD_DEFINED__ */


#ifndef __IWMPCdrom_FWD_DEFINED__
#define __IWMPCdrom_FWD_DEFINED__
typedef interface IWMPCdrom IWMPCdrom;
#endif 	/* __IWMPCdrom_FWD_DEFINED__ */


#ifndef __IWMPCdromCollection_FWD_DEFINED__
#define __IWMPCdromCollection_FWD_DEFINED__
typedef interface IWMPCdromCollection IWMPCdromCollection;
#endif 	/* __IWMPCdromCollection_FWD_DEFINED__ */


#ifndef __IWMPStringCollection_FWD_DEFINED__
#define __IWMPStringCollection_FWD_DEFINED__
typedef interface IWMPStringCollection IWMPStringCollection;
#endif 	/* __IWMPStringCollection_FWD_DEFINED__ */


#ifndef __IWMPMediaCollection_FWD_DEFINED__
#define __IWMPMediaCollection_FWD_DEFINED__
typedef interface IWMPMediaCollection IWMPMediaCollection;
#endif 	/* __IWMPMediaCollection_FWD_DEFINED__ */


#ifndef __IWMPPlaylistArray_FWD_DEFINED__
#define __IWMPPlaylistArray_FWD_DEFINED__
typedef interface IWMPPlaylistArray IWMPPlaylistArray;
#endif 	/* __IWMPPlaylistArray_FWD_DEFINED__ */


#ifndef __IWMPPlaylistCollection_FWD_DEFINED__
#define __IWMPPlaylistCollection_FWD_DEFINED__
typedef interface IWMPPlaylistCollection IWMPPlaylistCollection;
#endif 	/* __IWMPPlaylistCollection_FWD_DEFINED__ */


#ifndef __IWMPNetwork_FWD_DEFINED__
#define __IWMPNetwork_FWD_DEFINED__
typedef interface IWMPNetwork IWMPNetwork;
#endif 	/* __IWMPNetwork_FWD_DEFINED__ */


#ifndef __IWMPCore_FWD_DEFINED__
#define __IWMPCore_FWD_DEFINED__
typedef interface IWMPCore IWMPCore;
#endif 	/* __IWMPCore_FWD_DEFINED__ */


#ifndef __IWMPPlayer_FWD_DEFINED__
#define __IWMPPlayer_FWD_DEFINED__
typedef interface IWMPPlayer IWMPPlayer;
#endif 	/* __IWMPPlayer_FWD_DEFINED__ */


#ifndef __IWMPPlayer2_FWD_DEFINED__
#define __IWMPPlayer2_FWD_DEFINED__
typedef interface IWMPPlayer2 IWMPPlayer2;
#endif 	/* __IWMPPlayer2_FWD_DEFINED__ */


#ifndef __IWMPMedia2_FWD_DEFINED__
#define __IWMPMedia2_FWD_DEFINED__
typedef interface IWMPMedia2 IWMPMedia2;
#endif 	/* __IWMPMedia2_FWD_DEFINED__ */


#ifndef __IWMPControls2_FWD_DEFINED__
#define __IWMPControls2_FWD_DEFINED__
typedef interface IWMPControls2 IWMPControls2;
#endif 	/* __IWMPControls2_FWD_DEFINED__ */


#ifndef __IWMPDVD_FWD_DEFINED__
#define __IWMPDVD_FWD_DEFINED__
typedef interface IWMPDVD IWMPDVD;
#endif 	/* __IWMPDVD_FWD_DEFINED__ */


#ifndef __IWMPCore2_FWD_DEFINED__
#define __IWMPCore2_FWD_DEFINED__
typedef interface IWMPCore2 IWMPCore2;
#endif 	/* __IWMPCore2_FWD_DEFINED__ */


#ifndef __IWMPPlayer3_FWD_DEFINED__
#define __IWMPPlayer3_FWD_DEFINED__
typedef interface IWMPPlayer3 IWMPPlayer3;
#endif 	/* __IWMPPlayer3_FWD_DEFINED__ */


#ifndef __IWMPErrorItem2_FWD_DEFINED__
#define __IWMPErrorItem2_FWD_DEFINED__
typedef interface IWMPErrorItem2 IWMPErrorItem2;
#endif 	/* __IWMPErrorItem2_FWD_DEFINED__ */


#ifndef __IWMPRemoteMediaServices_FWD_DEFINED__
#define __IWMPRemoteMediaServices_FWD_DEFINED__
typedef interface IWMPRemoteMediaServices IWMPRemoteMediaServices;
#endif 	/* __IWMPRemoteMediaServices_FWD_DEFINED__ */


#ifndef __IWMPSkinManager_FWD_DEFINED__
#define __IWMPSkinManager_FWD_DEFINED__
typedef interface IWMPSkinManager IWMPSkinManager;
#endif 	/* __IWMPSkinManager_FWD_DEFINED__ */


#ifndef __IWMPMetadataPicture_FWD_DEFINED__
#define __IWMPMetadataPicture_FWD_DEFINED__
typedef interface IWMPMetadataPicture IWMPMetadataPicture;
#endif 	/* __IWMPMetadataPicture_FWD_DEFINED__ */


#ifndef __IWMPMetadataText_FWD_DEFINED__
#define __IWMPMetadataText_FWD_DEFINED__
typedef interface IWMPMetadataText IWMPMetadataText;
#endif 	/* __IWMPMetadataText_FWD_DEFINED__ */


#ifndef __IWMPMedia3_FWD_DEFINED__
#define __IWMPMedia3_FWD_DEFINED__
typedef interface IWMPMedia3 IWMPMedia3;
#endif 	/* __IWMPMedia3_FWD_DEFINED__ */


#ifndef __IWMPSettings2_FWD_DEFINED__
#define __IWMPSettings2_FWD_DEFINED__
typedef interface IWMPSettings2 IWMPSettings2;
#endif 	/* __IWMPSettings2_FWD_DEFINED__ */


#ifndef __IWMPControls3_FWD_DEFINED__
#define __IWMPControls3_FWD_DEFINED__
typedef interface IWMPControls3 IWMPControls3;
#endif 	/* __IWMPControls3_FWD_DEFINED__ */


#ifndef __IWMPClosedCaption2_FWD_DEFINED__
#define __IWMPClosedCaption2_FWD_DEFINED__
typedef interface IWMPClosedCaption2 IWMPClosedCaption2;
#endif 	/* __IWMPClosedCaption2_FWD_DEFINED__ */


#ifndef __IWMPPlayerApplication_FWD_DEFINED__
#define __IWMPPlayerApplication_FWD_DEFINED__
typedef interface IWMPPlayerApplication IWMPPlayerApplication;
#endif 	/* __IWMPPlayerApplication_FWD_DEFINED__ */


#ifndef __IWMPCore3_FWD_DEFINED__
#define __IWMPCore3_FWD_DEFINED__
typedef interface IWMPCore3 IWMPCore3;
#endif 	/* __IWMPCore3_FWD_DEFINED__ */


#ifndef __IWMPPlayer4_FWD_DEFINED__
#define __IWMPPlayer4_FWD_DEFINED__
typedef interface IWMPPlayer4 IWMPPlayer4;
#endif 	/* __IWMPPlayer4_FWD_DEFINED__ */


#ifndef __IWMPPlayerServices_FWD_DEFINED__
#define __IWMPPlayerServices_FWD_DEFINED__
typedef interface IWMPPlayerServices IWMPPlayerServices;
#endif 	/* __IWMPPlayerServices_FWD_DEFINED__ */


#ifndef __IWMPSyncDevice_FWD_DEFINED__
#define __IWMPSyncDevice_FWD_DEFINED__
typedef interface IWMPSyncDevice IWMPSyncDevice;
#endif 	/* __IWMPSyncDevice_FWD_DEFINED__ */


#ifndef __IWMPSyncServices_FWD_DEFINED__
#define __IWMPSyncServices_FWD_DEFINED__
typedef interface IWMPSyncServices IWMPSyncServices;
#endif 	/* __IWMPSyncServices_FWD_DEFINED__ */


#ifndef __IWMPPlayerServices2_FWD_DEFINED__
#define __IWMPPlayerServices2_FWD_DEFINED__
typedef interface IWMPPlayerServices2 IWMPPlayerServices2;
#endif 	/* __IWMPPlayerServices2_FWD_DEFINED__ */


#ifndef __IWMPEvents_FWD_DEFINED__
#define __IWMPEvents_FWD_DEFINED__
typedef interface IWMPEvents IWMPEvents;
#endif 	/* __IWMPEvents_FWD_DEFINED__ */


#ifndef __IWMPEvents2_FWD_DEFINED__
#define __IWMPEvents2_FWD_DEFINED__
typedef interface IWMPEvents2 IWMPEvents2;
#endif 	/* __IWMPEvents2_FWD_DEFINED__ */


#ifndef ___WMPOCXEvents_FWD_DEFINED__
#define ___WMPOCXEvents_FWD_DEFINED__
typedef interface _WMPOCXEvents _WMPOCXEvents;
#endif 	/* ___WMPOCXEvents_FWD_DEFINED__ */


#ifndef __WindowsMediaPlayer_FWD_DEFINED__
#define __WindowsMediaPlayer_FWD_DEFINED__

#ifdef __cplusplus
typedef class WindowsMediaPlayer WindowsMediaPlayer;
#else
typedef struct WindowsMediaPlayer WindowsMediaPlayer;
#endif /* __cplusplus */

#endif 	/* __WindowsMediaPlayer_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"

#ifdef __cplusplus
extern "C"{
#endif 

void * __RPC_USER MIDL_user_allocate(size_t);
void __RPC_USER MIDL_user_free( void * ); 

/* interface __MIDL_itf_wmp_0000 */
/* [local] */ 

//=========================================================================
//
// Microsoft Windows Media Technologies
// Copyright (C) Microsoft Corporation. All rights reserved.
//
//=========================================================================
typedef /* [public][helpstring] */ 
enum WMPOpenState
    {	wmposUndefined	= 0,
	wmposPlaylistChanging	= wmposUndefined + 1,
	wmposPlaylistLocating	= wmposPlaylistChanging + 1,
	wmposPlaylistConnecting	= wmposPlaylistLocating + 1,
	wmposPlaylistLoading	= wmposPlaylistConnecting + 1,
	wmposPlaylistOpening	= wmposPlaylistLoading + 1,
	wmposPlaylistOpenNoMedia	= wmposPlaylistOpening + 1,
	wmposPlaylistChanged	= wmposPlaylistOpenNoMedia + 1,
	wmposMediaChanging	= wmposPlaylistChanged + 1,
	wmposMediaLocating	= wmposMediaChanging + 1,
	wmposMediaConnecting	= wmposMediaLocating + 1,
	wmposMediaLoading	= wmposMediaConnecting + 1,
	wmposMediaOpening	= wmposMediaLoading + 1,
	wmposMediaOpen	= wmposMediaOpening + 1,
	wmposBeginCodecAcquisition	= wmposMediaOpen + 1,
	wmposEndCodecAcquisition	= wmposBeginCodecAcquisition + 1,
	wmposBeginLicenseAcquisition	= wmposEndCodecAcquisition + 1,
	wmposEndLicenseAcquisition	= wmposBeginLicenseAcquisition + 1,
	wmposBeginIndividualization	= wmposEndLicenseAcquisition + 1,
	wmposEndIndividualization	= wmposBeginIndividualization + 1,
	wmposMediaWaiting	= wmposEndIndividualization + 1,
	wmposOpeningUnknownURL	= wmposMediaWaiting + 1
    } 	WMPOpenState;

typedef /* [public][helpstring] */ 
enum WMPPlayState
    {	wmppsUndefined	= 0,
	wmppsStopped	= wmppsUndefined + 1,
	wmppsPaused	= wmppsStopped + 1,
	wmppsPlaying	= wmppsPaused + 1,
	wmppsScanForward	= wmppsPlaying + 1,
	wmppsScanReverse	= wmppsScanForward + 1,
	wmppsBuffering	= wmppsScanReverse + 1,
	wmppsWaiting	= wmppsBuffering + 1,
	wmppsMediaEnded	= wmppsWaiting + 1,
	wmppsTransitioning	= wmppsMediaEnded + 1,
	wmppsReady	= wmppsTransitioning + 1,
	wmppsReconnecting	= wmppsReady + 1,
	wmppsLast	= wmppsReconnecting + 1
    } 	WMPPlayState;

typedef /* [public][helpstring] */ 
enum WMPPlaylistChangeEventType
    {	wmplcUnknown	= 0,
	wmplcClear	= wmplcUnknown + 1,
	wmplcInfoChange	= wmplcClear + 1,
	wmplcMove	= wmplcInfoChange + 1,
	wmplcDelete	= wmplcMove + 1,
	wmplcInsert	= wmplcDelete + 1,
	wmplcAppend	= wmplcInsert + 1,
	wmplcPrivate	= wmplcAppend + 1,
	wmplcNameChange	= wmplcPrivate + 1,
	wmplcMorph	= wmplcNameChange + 1,
	wmplcSort	= wmplcMorph + 1,
	wmplcLast	= wmplcSort + 1
    } 	WMPPlaylistChangeEventType;




extern RPC_IF_HANDLE __MIDL_itf_wmp_0000_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_wmp_0000_v0_0_s_ifspec;

#ifndef __IWMPErrorItem_INTERFACE_DEFINED__
#define __IWMPErrorItem_INTERFACE_DEFINED__

/* interface IWMPErrorItem */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_IWMPErrorItem;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("3614C646-3B3B-4de7-A81E-930E3F2127B3")
    IWMPErrorItem : public IDispatch
    {
    public:
        virtual /* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE get_errorCode( 
            /* [retval][out] */ long *phr) = 0;
        
        virtual /* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE get_errorDescription( 
            /* [retval][out] */ BSTR *pbstrDescription) = 0;
        
        virtual /* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE get_errorContext( 
            /* [retval][out] */ VARIANT *pvarContext) = 0;
        
        virtual /* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE get_remedy( 
            /* [retval][out] */ long *plRemedy) = 0;
        
        virtual /* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE get_customUrl( 
            /* [retval][out] */ BSTR *pbstrCustomUrl) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IWMPErrorItemVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IWMPErrorItem * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IWMPErrorItem * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IWMPErrorItem * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IWMPErrorItem * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IWMPErrorItem * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IWMPErrorItem * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IWMPErrorItem * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_errorCode )( 
            IWMPErrorItem * This,
            /* [retval][out] */ long *phr);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_errorDescription )( 
            IWMPErrorItem * This,
            /* [retval][out] */ BSTR *pbstrDescription);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_errorContext )( 
            IWMPErrorItem * This,
            /* [retval][out] */ VARIANT *pvarContext);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_remedy )( 
            IWMPErrorItem * This,
            /* [retval][out] */ long *plRemedy);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_customUrl )( 
            IWMPErrorItem * This,
            /* [retval][out] */ BSTR *pbstrCustomUrl);
        
        END_INTERFACE
    } IWMPErrorItemVtbl;

    interface IWMPErrorItem
    {
        CONST_VTBL struct IWMPErrorItemVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IWMPErrorItem_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IWMPErrorItem_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IWMPErrorItem_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IWMPErrorItem_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IWMPErrorItem_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IWMPErrorItem_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IWMPErrorItem_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IWMPErrorItem_get_errorCode(This,phr)	\
    (This)->lpVtbl -> get_errorCode(This,phr)

#define IWMPErrorItem_get_errorDescription(This,pbstrDescription)	\
    (This)->lpVtbl -> get_errorDescription(This,pbstrDescription)

#define IWMPErrorItem_get_errorContext(This,pvarContext)	\
    (This)->lpVtbl -> get_errorContext(This,pvarContext)

#define IWMPErrorItem_get_remedy(This,plRemedy)	\
    (This)->lpVtbl -> get_remedy(This,plRemedy)

#define IWMPErrorItem_get_customUrl(This,pbstrCustomUrl)	\
    (This)->lpVtbl -> get_customUrl(This,pbstrCustomUrl)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE IWMPErrorItem_get_errorCode_Proxy( 
    IWMPErrorItem * This,
    /* [retval][out] */ long *phr);


void __RPC_STUB IWMPErrorItem_get_errorCode_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE IWMPErrorItem_get_errorDescription_Proxy( 
    IWMPErrorItem * This,
    /* [retval][out] */ BSTR *pbstrDescription);


void __RPC_STUB IWMPErrorItem_get_errorDescription_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE IWMPErrorItem_get_errorContext_Proxy( 
    IWMPErrorItem * This,
    /* [retval][out] */ VARIANT *pvarContext);


void __RPC_STUB IWMPErrorItem_get_errorContext_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE IWMPErrorItem_get_remedy_Proxy( 
    IWMPErrorItem * This,
    /* [retval][out] */ long *plRemedy);


void __RPC_STUB IWMPErrorItem_get_remedy_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE IWMPErrorItem_get_customUrl_Proxy( 
    IWMPErrorItem * This,
    /* [retval][out] */ BSTR *pbstrCustomUrl);


void __RPC_STUB IWMPErrorItem_get_customUrl_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IWMPErrorItem_INTERFACE_DEFINED__ */


#ifndef __IWMPError_INTERFACE_DEFINED__
#define __IWMPError_INTERFACE_DEFINED__

/* interface IWMPError */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_IWMPError;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("A12DCF7D-14AB-4c1b-A8CD-63909F06025B")
    IWMPError : public IDispatch
    {
    public:
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE clearErrorQueue( void) = 0;
        
        virtual /* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE get_errorCount( 
            /* [retval][out] */ long *plNumErrors) = 0;
        
        virtual /* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE get_item( 
            /* [in] */ long dwIndex,
            /* [retval][out] */ IWMPErrorItem **ppErrorItem) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE webHelp( void) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IWMPErrorVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IWMPError * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IWMPError * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IWMPError * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IWMPError * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IWMPError * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IWMPError * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IWMPError * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *clearErrorQueue )( 
            IWMPError * This);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_errorCount )( 
            IWMPError * This,
            /* [retval][out] */ long *plNumErrors);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_item )( 
            IWMPError * This,
            /* [in] */ long dwIndex,
            /* [retval][out] */ IWMPErrorItem **ppErrorItem);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *webHelp )( 
            IWMPError * This);
        
        END_INTERFACE
    } IWMPErrorVtbl;

    interface IWMPError
    {
        CONST_VTBL struct IWMPErrorVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IWMPError_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IWMPError_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IWMPError_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IWMPError_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IWMPError_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IWMPError_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IWMPError_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IWMPError_clearErrorQueue(This)	\
    (This)->lpVtbl -> clearErrorQueue(This)

#define IWMPError_get_errorCount(This,plNumErrors)	\
    (This)->lpVtbl -> get_errorCount(This,plNumErrors)

#define IWMPError_get_item(This,dwIndex,ppErrorItem)	\
    (This)->lpVtbl -> get_item(This,dwIndex,ppErrorItem)

#define IWMPError_webHelp(This)	\
    (This)->lpVtbl -> webHelp(This)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IWMPError_clearErrorQueue_Proxy( 
    IWMPError * This);


void __RPC_STUB IWMPError_clearErrorQueue_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE IWMPError_get_errorCount_Proxy( 
    IWMPError * This,
    /* [retval][out] */ long *plNumErrors);


void __RPC_STUB IWMPError_get_errorCount_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE IWMPError_get_item_Proxy( 
    IWMPError * This,
    /* [in] */ long dwIndex,
    /* [retval][out] */ IWMPErrorItem **ppErrorItem);


void __RPC_STUB IWMPError_get_item_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IWMPError_webHelp_Proxy( 
    IWMPError * This);


void __RPC_STUB IWMPError_webHelp_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IWMPError_INTERFACE_DEFINED__ */


#ifndef __IWMPMedia_INTERFACE_DEFINED__
#define __IWMPMedia_INTERFACE_DEFINED__

/* interface IWMPMedia */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_IWMPMedia;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("94D55E95-3FAC-11d3-B155-00C04F79FAA6")
    IWMPMedia : public IDispatch
    {
    public:
        virtual /* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE get_isIdentical( 
            /* [in] */ IWMPMedia *pIWMPMedia,
            /* [retval][out] */ VARIANT_BOOL *pvbool) = 0;
        
        virtual /* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE get_sourceURL( 
            /* [retval][out] */ BSTR *pbstrSourceURL) = 0;
        
        virtual /* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE get_name( 
            /* [retval][out] */ BSTR *pbstrName) = 0;
        
        virtual /* [helpstring][propput][id] */ HRESULT STDMETHODCALLTYPE put_name( 
            /* [in] */ BSTR bstrName) = 0;
        
        virtual /* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE get_imageSourceWidth( 
            /* [retval][out] */ long *pWidth) = 0;
        
        virtual /* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE get_imageSourceHeight( 
            /* [retval][out] */ long *pHeight) = 0;
        
        virtual /* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE get_markerCount( 
            /* [retval][out] */ long *pMarkerCount) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE getMarkerTime( 
            /* [in] */ long MarkerNum,
            /* [retval][out] */ double *pMarkerTime) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE getMarkerName( 
            /* [in] */ long MarkerNum,
            /* [retval][out] */ BSTR *pbstrMarkerName) = 0;
        
        virtual /* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE get_duration( 
            /* [retval][out] */ double *pDuration) = 0;
        
        virtual /* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE get_durationString( 
            /* [retval][out] */ BSTR *pbstrDuration) = 0;
        
        virtual /* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE get_attributeCount( 
            /* [retval][out] */ long *plCount) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE getAttributeName( 
            /* [in] */ long lIndex,
            /* [retval][out] */ BSTR *pbstrItemName) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE getItemInfo( 
            /* [in] */ BSTR bstrItemName,
            /* [retval][out] */ BSTR *pbstrVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE setItemInfo( 
            /* [in] */ BSTR bstrItemName,
            /* [in] */ BSTR bstrVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE getItemInfoByAtom( 
            /* [in] */ long lAtom,
            /* [retval][out] */ BSTR *pbstrVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE isMemberOf( 
            /* [in] */ IWMPPlaylist *pPlaylist,
            /* [retval][out] */ VARIANT_BOOL *pvarfIsMemberOf) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE isReadOnlyItem( 
            /* [in] */ BSTR bstrItemName,
            /* [retval][out] */ VARIANT_BOOL *pvarfIsReadOnly) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IWMPMediaVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IWMPMedia * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IWMPMedia * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IWMPMedia * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IWMPMedia * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IWMPMedia * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IWMPMedia * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IWMPMedia * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_isIdentical )( 
            IWMPMedia * This,
            /* [in] */ IWMPMedia *pIWMPMedia,
            /* [retval][out] */ VARIANT_BOOL *pvbool);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_sourceURL )( 
            IWMPMedia * This,
            /* [retval][out] */ BSTR *pbstrSourceURL);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_name )( 
            IWMPMedia * This,
            /* [retval][out] */ BSTR *pbstrName);
        
        /* [helpstring][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_name )( 
            IWMPMedia * This,
            /* [in] */ BSTR bstrName);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_imageSourceWidth )( 
            IWMPMedia * This,
            /* [retval][out] */ long *pWidth);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_imageSourceHeight )( 
            IWMPMedia * This,
            /* [retval][out] */ long *pHeight);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_markerCount )( 
            IWMPMedia * This,
            /* [retval][out] */ long *pMarkerCount);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *getMarkerTime )( 
            IWMPMedia * This,
            /* [in] */ long MarkerNum,
            /* [retval][out] */ double *pMarkerTime);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *getMarkerName )( 
            IWMPMedia * This,
            /* [in] */ long MarkerNum,
            /* [retval][out] */ BSTR *pbstrMarkerName);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_duration )( 
            IWMPMedia * This,
            /* [retval][out] */ double *pDuration);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_durationString )( 
            IWMPMedia * This,
            /* [retval][out] */ BSTR *pbstrDuration);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_attributeCount )( 
            IWMPMedia * This,
            /* [retval][out] */ long *plCount);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *getAttributeName )( 
            IWMPMedia * This,
            /* [in] */ long lIndex,
            /* [retval][out] */ BSTR *pbstrItemName);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *getItemInfo )( 
            IWMPMedia * This,
            /* [in] */ BSTR bstrItemName,
            /* [retval][out] */ BSTR *pbstrVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *setItemInfo )( 
            IWMPMedia * This,
            /* [in] */ BSTR bstrItemName,
            /* [in] */ BSTR bstrVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *getItemInfoByAtom )( 
            IWMPMedia * This,
            /* [in] */ long lAtom,
            /* [retval][out] */ BSTR *pbstrVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *isMemberOf )( 
            IWMPMedia * This,
            /* [in] */ IWMPPlaylist *pPlaylist,
            /* [retval][out] */ VARIANT_BOOL *pvarfIsMemberOf);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *isReadOnlyItem )( 
            IWMPMedia * This,
            /* [in] */ BSTR bstrItemName,
            /* [retval][out] */ VARIANT_BOOL *pvarfIsReadOnly);
        
        END_INTERFACE
    } IWMPMediaVtbl;

    interface IWMPMedia
    {
        CONST_VTBL struct IWMPMediaVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IWMPMedia_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IWMPMedia_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IWMPMedia_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IWMPMedia_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IWMPMedia_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IWMPMedia_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IWMPMedia_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IWMPMedia_get_isIdentical(This,pIWMPMedia,pvbool)	\
    (This)->lpVtbl -> get_isIdentical(This,pIWMPMedia,pvbool)

#define IWMPMedia_get_sourceURL(This,pbstrSourceURL)	\
    (This)->lpVtbl -> get_sourceURL(This,pbstrSourceURL)

#define IWMPMedia_get_name(This,pbstrName)	\
    (This)->lpVtbl -> get_name(This,pbstrName)

#define IWMPMedia_put_name(This,bstrName)	\
    (This)->lpVtbl -> put_name(This,bstrName)

#define IWMPMedia_get_imageSourceWidth(This,pWidth)	\
    (This)->lpVtbl -> get_imageSourceWidth(This,pWidth)

#define IWMPMedia_get_imageSourceHeight(This,pHeight)	\
    (This)->lpVtbl -> get_imageSourceHeight(This,pHeight)

#define IWMPMedia_get_markerCount(This,pMarkerCount)	\
    (This)->lpVtbl -> get_markerCount(This,pMarkerCount)

#define IWMPMedia_getMarkerTime(This,MarkerNum,pMarkerTime)	\
    (This)->lpVtbl -> getMarkerTime(This,MarkerNum,pMarkerTime)

#define IWMPMedia_getMarkerName(This,MarkerNum,pbstrMarkerName)	\
    (This)->lpVtbl -> getMarkerName(This,MarkerNum,pbstrMarkerName)

#define IWMPMedia_get_duration(This,pDuration)	\
    (This)->lpVtbl -> get_duration(This,pDuration)

#define IWMPMedia_get_durationString(This,pbstrDuration)	\
    (This)->lpVtbl -> get_durationString(This,pbstrDuration)

#define IWMPMedia_get_attributeCount(This,plCount)	\
    (This)->lpVtbl -> get_attributeCount(This,plCount)

#define IWMPMedia_getAttributeName(This,lIndex,pbstrItemName)	\
    (This)->lpVtbl -> getAttributeName(This,lIndex,pbstrItemName)

#define IWMPMedia_getItemInfo(This,bstrItemName,pbstrVal)	\
    (This)->lpVtbl -> getItemInfo(This,bstrItemName,pbstrVal)

#define IWMPMedia_setItemInfo(This,bstrItemName,bstrVal)	\
    (This)->lpVtbl -> setItemInfo(This,bstrItemName,bstrVal)

#define IWMPMedia_getItemInfoByAtom(This,lAtom,pbstrVal)	\
    (This)->lpVtbl -> getItemInfoByAtom(This,lAtom,pbstrVal)

#define IWMPMedia_isMemberOf(This,pPlaylist,pvarfIsMemberOf)	\
    (This)->lpVtbl -> isMemberOf(This,pPlaylist,pvarfIsMemberOf)

#define IWMPMedia_isReadOnlyItem(This,bstrItemName,pvarfIsReadOnly)	\
    (This)->lpVtbl -> isReadOnlyItem(This,bstrItemName,pvarfIsReadOnly)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE IWMPMedia_get_isIdentical_Proxy( 
    IWMPMedia * This,
    /* [in] */ IWMPMedia *pIWMPMedia,
    /* [retval][out] */ VARIANT_BOOL *pvbool);


void __RPC_STUB IWMPMedia_get_isIdentical_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE IWMPMedia_get_sourceURL_Proxy( 
    IWMPMedia * This,
    /* [retval][out] */ BSTR *pbstrSourceURL);


void __RPC_STUB IWMPMedia_get_sourceURL_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE IWMPMedia_get_name_Proxy( 
    IWMPMedia * This,
    /* [retval][out] */ BSTR *pbstrName);


void __RPC_STUB IWMPMedia_get_name_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propput][id] */ HRESULT STDMETHODCALLTYPE IWMPMedia_put_name_Proxy( 
    IWMPMedia * This,
    /* [in] */ BSTR bstrName);


void __RPC_STUB IWMPMedia_put_name_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE IWMPMedia_get_imageSourceWidth_Proxy( 
    IWMPMedia * This,
    /* [retval][out] */ long *pWidth);


void __RPC_STUB IWMPMedia_get_imageSourceWidth_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE IWMPMedia_get_imageSourceHeight_Proxy( 
    IWMPMedia * This,
    /* [retval][out] */ long *pHeight);


void __RPC_STUB IWMPMedia_get_imageSourceHeight_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE IWMPMedia_get_markerCount_Proxy( 
    IWMPMedia * This,
    /* [retval][out] */ long *pMarkerCount);


void __RPC_STUB IWMPMedia_get_markerCount_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IWMPMedia_getMarkerTime_Proxy( 
    IWMPMedia * This,
    /* [in] */ long MarkerNum,
    /* [retval][out] */ double *pMarkerTime);


void __RPC_STUB IWMPMedia_getMarkerTime_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IWMPMedia_getMarkerName_Proxy( 
    IWMPMedia * This,
    /* [in] */ long MarkerNum,
    /* [retval][out] */ BSTR *pbstrMarkerName);


void __RPC_STUB IWMPMedia_getMarkerName_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE IWMPMedia_get_duration_Proxy( 
    IWMPMedia * This,
    /* [retval][out] */ double *pDuration);


void __RPC_STUB IWMPMedia_get_duration_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE IWMPMedia_get_durationString_Proxy( 
    IWMPMedia * This,
    /* [retval][out] */ BSTR *pbstrDuration);


void __RPC_STUB IWMPMedia_get_durationString_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE IWMPMedia_get_attributeCount_Proxy( 
    IWMPMedia * This,
    /* [retval][out] */ long *plCount);


void __RPC_STUB IWMPMedia_get_attributeCount_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IWMPMedia_getAttributeName_Proxy( 
    IWMPMedia * This,
    /* [in] */ long lIndex,
    /* [retval][out] */ BSTR *pbstrItemName);


void __RPC_STUB IWMPMedia_getAttributeName_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IWMPMedia_getItemInfo_Proxy( 
    IWMPMedia * This,
    /* [in] */ BSTR bstrItemName,
    /* [retval][out] */ BSTR *pbstrVal);


void __RPC_STUB IWMPMedia_getItemInfo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IWMPMedia_setItemInfo_Proxy( 
    IWMPMedia * This,
    /* [in] */ BSTR bstrItemName,
    /* [in] */ BSTR bstrVal);


void __RPC_STUB IWMPMedia_setItemInfo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IWMPMedia_getItemInfoByAtom_Proxy( 
    IWMPMedia * This,
    /* [in] */ long lAtom,
    /* [retval][out] */ BSTR *pbstrVal);


void __RPC_STUB IWMPMedia_getItemInfoByAtom_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IWMPMedia_isMemberOf_Proxy( 
    IWMPMedia * This,
    /* [in] */ IWMPPlaylist *pPlaylist,
    /* [retval][out] */ VARIANT_BOOL *pvarfIsMemberOf);


void __RPC_STUB IWMPMedia_isMemberOf_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IWMPMedia_isReadOnlyItem_Proxy( 
    IWMPMedia * This,
    /* [in] */ BSTR bstrItemName,
    /* [retval][out] */ VARIANT_BOOL *pvarfIsReadOnly);


void __RPC_STUB IWMPMedia_isReadOnlyItem_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IWMPMedia_INTERFACE_DEFINED__ */


#ifndef __IWMPControls_INTERFACE_DEFINED__
#define __IWMPControls_INTERFACE_DEFINED__

/* interface IWMPControls */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_IWMPControls;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("74C09E02-F828-11d2-A74B-00A0C905F36E")
    IWMPControls : public IDispatch
    {
    public:
        virtual /* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE get_isAvailable( 
            /* [in] */ BSTR bstrItem,
            /* [retval][out] */ VARIANT_BOOL *pIsAvailable) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE play( void) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE stop( void) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE pause( void) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE fastForward( void) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE fastReverse( void) = 0;
        
        virtual /* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE get_currentPosition( 
            /* [retval][out] */ double *pdCurrentPosition) = 0;
        
        virtual /* [helpstring][propput][id] */ HRESULT STDMETHODCALLTYPE put_currentPosition( 
            /* [in] */ double dCurrentPosition) = 0;
        
        virtual /* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE get_currentPositionString( 
            /* [retval][out] */ BSTR *pbstrCurrentPosition) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE next( void) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE previous( void) = 0;
        
        virtual /* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE get_currentItem( 
            /* [retval][out] */ IWMPMedia **ppIWMPMedia) = 0;
        
        virtual /* [propput][id] */ HRESULT STDMETHODCALLTYPE put_currentItem( 
            /* [in] */ IWMPMedia *pIWMPMedia) = 0;
        
        virtual /* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE get_currentMarker( 
            /* [retval][out] */ long *plMarker) = 0;
        
        virtual /* [helpstring][propput][id] */ HRESULT STDMETHODCALLTYPE put_currentMarker( 
            /* [in] */ long lMarker) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE playItem( 
            /* [in] */ IWMPMedia *pIWMPMedia) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IWMPControlsVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IWMPControls * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IWMPControls * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IWMPControls * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IWMPControls * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IWMPControls * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IWMPControls * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IWMPControls * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_isAvailable )( 
            IWMPControls * This,
            /* [in] */ BSTR bstrItem,
            /* [retval][out] */ VARIANT_BOOL *pIsAvailable);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *play )( 
            IWMPControls * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *stop )( 
            IWMPControls * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *pause )( 
            IWMPControls * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *fastForward )( 
            IWMPControls * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *fastReverse )( 
            IWMPControls * This);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_currentPosition )( 
            IWMPControls * This,
            /* [retval][out] */ double *pdCurrentPosition);
        
        /* [helpstring][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_currentPosition )( 
            IWMPControls * This,
            /* [in] */ double dCurrentPosition);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_currentPositionString )( 
            IWMPControls * This,
            /* [retval][out] */ BSTR *pbstrCurrentPosition);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *next )( 
            IWMPControls * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *previous )( 
            IWMPControls * This);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_currentItem )( 
            IWMPControls * This,
            /* [retval][out] */ IWMPMedia **ppIWMPMedia);
        
        /* [propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_currentItem )( 
            IWMPControls * This,
            /* [in] */ IWMPMedia *pIWMPMedia);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_currentMarker )( 
            IWMPControls * This,
            /* [retval][out] */ long *plMarker);
        
        /* [helpstring][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_currentMarker )( 
            IWMPControls * This,
            /* [in] */ long lMarker);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *playItem )( 
            IWMPControls * This,
            /* [in] */ IWMPMedia *pIWMPMedia);
        
        END_INTERFACE
    } IWMPControlsVtbl;

    interface IWMPControls
    {
        CONST_VTBL struct IWMPControlsVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IWMPControls_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IWMPControls_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IWMPControls_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IWMPControls_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IWMPControls_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IWMPControls_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IWMPControls_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IWMPControls_get_isAvailable(This,bstrItem,pIsAvailable)	\
    (This)->lpVtbl -> get_isAvailable(This,bstrItem,pIsAvailable)

#define IWMPControls_play(This)	\
    (This)->lpVtbl -> play(This)

#define IWMPControls_stop(This)	\
    (This)->lpVtbl -> stop(This)

#define IWMPControls_pause(This)	\
    (This)->lpVtbl -> pause(This)

#define IWMPControls_fastForward(This)	\
    (This)->lpVtbl -> fastForward(This)

#define IWMPControls_fastReverse(This)	\
    (This)->lpVtbl -> fastReverse(This)

#define IWMPControls_get_currentPosition(This,pdCurrentPosition)	\
    (This)->lpVtbl -> get_currentPosition(This,pdCurrentPosition)

#define IWMPControls_put_currentPosition(This,dCurrentPosition)	\
    (This)->lpVtbl -> put_currentPosition(This,dCurrentPosition)

#define IWMPControls_get_currentPositionString(This,pbstrCurrentPosition)	\
    (This)->lpVtbl -> get_currentPositionString(This,pbstrCurrentPosition)

#define IWMPControls_next(This)	\
    (This)->lpVtbl -> next(This)

#define IWMPControls_previous(This)	\
    (This)->lpVtbl -> previous(This)

#define IWMPControls_get_currentItem(This,ppIWMPMedia)	\
    (This)->lpVtbl -> get_currentItem(This,ppIWMPMedia)

#define IWMPControls_put_currentItem(This,pIWMPMedia)	\
    (This)->lpVtbl -> put_currentItem(This,pIWMPMedia)

#define IWMPControls_get_currentMarker(This,plMarker)	\
    (This)->lpVtbl -> get_currentMarker(This,plMarker)

#define IWMPControls_put_currentMarker(This,lMarker)	\
    (This)->lpVtbl -> put_currentMarker(This,lMarker)

#define IWMPControls_playItem(This,pIWMPMedia)	\
    (This)->lpVtbl -> playItem(This,pIWMPMedia)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE IWMPControls_get_isAvailable_Proxy( 
    IWMPControls * This,
    /* [in] */ BSTR bstrItem,
    /* [retval][out] */ VARIANT_BOOL *pIsAvailable);


void __RPC_STUB IWMPControls_get_isAvailable_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IWMPControls_play_Proxy( 
    IWMPControls * This);


void __RPC_STUB IWMPControls_play_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IWMPControls_stop_Proxy( 
    IWMPControls * This);


void __RPC_STUB IWMPControls_stop_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IWMPControls_pause_Proxy( 
    IWMPControls * This);


void __RPC_STUB IWMPControls_pause_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IWMPControls_fastForward_Proxy( 
    IWMPControls * This);


void __RPC_STUB IWMPControls_fastForward_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IWMPControls_fastReverse_Proxy( 
    IWMPControls * This);


void __RPC_STUB IWMPControls_fastReverse_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE IWMPControls_get_currentPosition_Proxy( 
    IWMPControls * This,
    /* [retval][out] */ double *pdCurrentPosition);


void __RPC_STUB IWMPControls_get_currentPosition_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propput][id] */ HRESULT STDMETHODCALLTYPE IWMPControls_put_currentPosition_Proxy( 
    IWMPControls * This,
    /* [in] */ double dCurrentPosition);


void __RPC_STUB IWMPControls_put_currentPosition_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE IWMPControls_get_currentPositionString_Proxy( 
    IWMPControls * This,
    /* [retval][out] */ BSTR *pbstrCurrentPosition);


void __RPC_STUB IWMPControls_get_currentPositionString_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IWMPControls_next_Proxy( 
    IWMPControls * This);


void __RPC_STUB IWMPControls_next_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IWMPControls_previous_Proxy( 
    IWMPControls * This);


void __RPC_STUB IWMPControls_previous_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE IWMPControls_get_currentItem_Proxy( 
    IWMPControls * This,
    /* [retval][out] */ IWMPMedia **ppIWMPMedia);


void __RPC_STUB IWMPControls_get_currentItem_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [propput][id] */ HRESULT STDMETHODCALLTYPE IWMPControls_put_currentItem_Proxy( 
    IWMPControls * This,
    /* [in] */ IWMPMedia *pIWMPMedia);


void __RPC_STUB IWMPControls_put_currentItem_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE IWMPControls_get_currentMarker_Proxy( 
    IWMPControls * This,
    /* [retval][out] */ long *plMarker);


void __RPC_STUB IWMPControls_get_currentMarker_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propput][id] */ HRESULT STDMETHODCALLTYPE IWMPControls_put_currentMarker_Proxy( 
    IWMPControls * This,
    /* [in] */ long lMarker);


void __RPC_STUB IWMPControls_put_currentMarker_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IWMPControls_playItem_Proxy( 
    IWMPControls * This,
    /* [in] */ IWMPMedia *pIWMPMedia);


void __RPC_STUB IWMPControls_playItem_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IWMPControls_INTERFACE_DEFINED__ */


#ifndef __IWMPSettings_INTERFACE_DEFINED__
#define __IWMPSettings_INTERFACE_DEFINED__

/* interface IWMPSettings */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_IWMPSettings;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("9104D1AB-80C9-4fed-ABF0-2E6417A6DF14")
    IWMPSettings : public IDispatch
    {
    public:
        virtual /* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE get_isAvailable( 
            /* [in] */ BSTR bstrItem,
            /* [retval][out] */ VARIANT_BOOL *pIsAvailable) = 0;
        
        virtual /* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE get_autoStart( 
            /* [retval][out] */ VARIANT_BOOL *pfAutoStart) = 0;
        
        virtual /* [helpstring][propput][id] */ HRESULT STDMETHODCALLTYPE put_autoStart( 
            /* [in] */ VARIANT_BOOL fAutoStart) = 0;
        
        virtual /* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE get_baseURL( 
            /* [retval][out] */ BSTR *pbstrBaseURL) = 0;
        
        virtual /* [helpstring][propput][id] */ HRESULT STDMETHODCALLTYPE put_baseURL( 
            /* [in] */ BSTR bstrBaseURL) = 0;
        
        virtual /* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE get_defaultFrame( 
            /* [retval][out] */ BSTR *pbstrDefaultFrame) = 0;
        
        virtual /* [helpstring][propput][id] */ HRESULT STDMETHODCALLTYPE put_defaultFrame( 
            /* [in] */ BSTR bstrDefaultFrame) = 0;
        
        virtual /* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE get_invokeURLs( 
            /* [retval][out] */ VARIANT_BOOL *pfInvokeURLs) = 0;
        
        virtual /* [helpstring][propput][id] */ HRESULT STDMETHODCALLTYPE put_invokeURLs( 
            /* [in] */ VARIANT_BOOL fInvokeURLs) = 0;
        
        virtual /* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE get_mute( 
            /* [retval][out] */ VARIANT_BOOL *pfMute) = 0;
        
        virtual /* [helpstring][propput][id] */ HRESULT STDMETHODCALLTYPE put_mute( 
            /* [in] */ VARIANT_BOOL fMute) = 0;
        
        virtual /* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE get_playCount( 
            /* [retval][out] */ long *plCount) = 0;
        
        virtual /* [helpstring][propput][id] */ HRESULT STDMETHODCALLTYPE put_playCount( 
            /* [in] */ long lCount) = 0;
        
        virtual /* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE get_rate( 
            /* [retval][out] */ double *pdRate) = 0;
        
        virtual /* [helpstring][propput][id] */ HRESULT STDMETHODCALLTYPE put_rate( 
            /* [in] */ double dRate) = 0;
        
        virtual /* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE get_balance( 
            /* [retval][out] */ long *plBalance) = 0;
        
        virtual /* [helpstring][propput][id] */ HRESULT STDMETHODCALLTYPE put_balance( 
            /* [in] */ long lBalance) = 0;
        
        virtual /* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE get_volume( 
            /* [retval][out] */ long *plVolume) = 0;
        
        virtual /* [helpstring][propput][id] */ HRESULT STDMETHODCALLTYPE put_volume( 
            /* [in] */ long lVolume) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE getMode( 
            /* [in] */ BSTR bstrMode,
            /* [retval][out] */ VARIANT_BOOL *pvarfMode) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE setMode( 
            /* [in] */ BSTR bstrMode,
            /* [in] */ VARIANT_BOOL varfMode) = 0;
        
        virtual /* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE get_enableErrorDialogs( 
            /* [retval][out] */ VARIANT_BOOL *pfEnableErrorDialogs) = 0;
        
        virtual /* [helpstring][propput][id] */ HRESULT STDMETHODCALLTYPE put_enableErrorDialogs( 
            /* [in] */ VARIANT_BOOL fEnableErrorDialogs) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IWMPSettingsVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IWMPSettings * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IWMPSettings * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IWMPSettings * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IWMPSettings * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IWMPSettings * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IWMPSettings * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IWMPSettings * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_isAvailable )( 
            IWMPSettings * This,
            /* [in] */ BSTR bstrItem,
            /* [retval][out] */ VARIANT_BOOL *pIsAvailable);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_autoStart )( 
            IWMPSettings * This,
            /* [retval][out] */ VARIANT_BOOL *pfAutoStart);
        
        /* [helpstring][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_autoStart )( 
            IWMPSettings * This,
            /* [in] */ VARIANT_BOOL fAutoStart);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_baseURL )( 
            IWMPSettings * This,
            /* [retval][out] */ BSTR *pbstrBaseURL);
        
        /* [helpstring][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_baseURL )( 
            IWMPSettings * This,
            /* [in] */ BSTR bstrBaseURL);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_defaultFrame )( 
            IWMPSettings * This,
            /* [retval][out] */ BSTR *pbstrDefaultFrame);
        
        /* [helpstring][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_defaultFrame )( 
            IWMPSettings * This,
            /* [in] */ BSTR bstrDefaultFrame);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_invokeURLs )( 
            IWMPSettings * This,
            /* [retval][out] */ VARIANT_BOOL *pfInvokeURLs);
        
        /* [helpstring][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_invokeURLs )( 
            IWMPSettings * This,
            /* [in] */ VARIANT_BOOL fInvokeURLs);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_mute )( 
            IWMPSettings * This,
            /* [retval][out] */ VARIANT_BOOL *pfMute);
        
        /* [helpstring][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_mute )( 
            IWMPSettings * This,
            /* [in] */ VARIANT_BOOL fMute);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_playCount )( 
            IWMPSettings * This,
            /* [retval][out] */ long *plCount);
        
        /* [helpstring][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_playCount )( 
            IWMPSettings * This,
            /* [in] */ long lCount);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_rate )( 
            IWMPSettings * This,
            /* [retval][out] */ double *pdRate);
        
        /* [helpstring][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_rate )( 
            IWMPSettings * This,
            /* [in] */ double dRate);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_balance )( 
            IWMPSettings * This,
            /* [retval][out] */ long *plBalance);
        
        /* [helpstring][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_balance )( 
            IWMPSettings * This,
            /* [in] */ long lBalance);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_volume )( 
            IWMPSettings * This,
            /* [retval][out] */ long *plVolume);
        
        /* [helpstring][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_volume )( 
            IWMPSettings * This,
            /* [in] */ long lVolume);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *getMode )( 
            IWMPSettings * This,
            /* [in] */ BSTR bstrMode,
            /* [retval][out] */ VARIANT_BOOL *pvarfMode);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *setMode )( 
            IWMPSettings * This,
            /* [in] */ BSTR bstrMode,
            /* [in] */ VARIANT_BOOL varfMode);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_enableErrorDialogs )( 
            IWMPSettings * This,
            /* [retval][out] */ VARIANT_BOOL *pfEnableErrorDialogs);
        
        /* [helpstring][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_enableErrorDialogs )( 
            IWMPSettings * This,
            /* [in] */ VARIANT_BOOL fEnableErrorDialogs);
        
        END_INTERFACE
    } IWMPSettingsVtbl;

    interface IWMPSettings
    {
        CONST_VTBL struct IWMPSettingsVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IWMPSettings_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IWMPSettings_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IWMPSettings_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IWMPSettings_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IWMPSettings_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IWMPSettings_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IWMPSettings_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IWMPSettings_get_isAvailable(This,bstrItem,pIsAvailable)	\
    (This)->lpVtbl -> get_isAvailable(This,bstrItem,pIsAvailable)

#define IWMPSettings_get_autoStart(This,pfAutoStart)	\
    (This)->lpVtbl -> get_autoStart(This,pfAutoStart)

#define IWMPSettings_put_autoStart(This,fAutoStart)	\
    (This)->lpVtbl -> put_autoStart(This,fAutoStart)

#define IWMPSettings_get_baseURL(This,pbstrBaseURL)	\
    (This)->lpVtbl -> get_baseURL(This,pbstrBaseURL)

#define IWMPSettings_put_baseURL(This,bstrBaseURL)	\
    (This)->lpVtbl -> put_baseURL(This,bstrBaseURL)

#define IWMPSettings_get_defaultFrame(This,pbstrDefaultFrame)	\
    (This)->lpVtbl -> get_defaultFrame(This,pbstrDefaultFrame)

#define IWMPSettings_put_defaultFrame(This,bstrDefaultFrame)	\
    (This)->lpVtbl -> put_defaultFrame(This,bstrDefaultFrame)

#define IWMPSettings_get_invokeURLs(This,pfInvokeURLs)	\
    (This)->lpVtbl -> get_invokeURLs(This,pfInvokeURLs)

#define IWMPSettings_put_invokeURLs(This,fInvokeURLs)	\
    (This)->lpVtbl -> put_invokeURLs(This,fInvokeURLs)

#define IWMPSettings_get_mute(This,pfMute)	\
    (This)->lpVtbl -> get_mute(This,pfMute)

#define IWMPSettings_put_mute(This,fMute)	\
    (This)->lpVtbl -> put_mute(This,fMute)

#define IWMPSettings_get_playCount(This,plCount)	\
    (This)->lpVtbl -> get_playCount(This,plCount)

#define IWMPSettings_put_playCount(This,lCount)	\
    (This)->lpVtbl -> put_playCount(This,lCount)

#define IWMPSettings_get_rate(This,pdRate)	\
    (This)->lpVtbl -> get_rate(This,pdRate)

#define IWMPSettings_put_rate(This,dRate)	\
    (This)->lpVtbl -> put_rate(This,dRate)

#define IWMPSettings_get_balance(This,plBalance)	\
    (This)->lpVtbl -> get_balance(This,plBalance)

#define IWMPSettings_put_balance(This,lBalance)	\
    (This)->lpVtbl -> put_balance(This,lBalance)

#define IWMPSettings_get_volume(This,plVolume)	\
    (This)->lpVtbl -> get_volume(This,plVolume)

#define IWMPSettings_put_volume(This,lVolume)	\
    (This)->lpVtbl -> put_volume(This,lVolume)

#define IWMPSettings_getMode(This,bstrMode,pvarfMode)	\
    (This)->lpVtbl -> getMode(This,bstrMode,pvarfMode)

#define IWMPSettings_setMode(This,bstrMode,varfMode)	\
    (This)->lpVtbl -> setMode(This,bstrMode,varfMode)

#define IWMPSettings_get_enableErrorDialogs(This,pfEnableErrorDialogs)	\
    (This)->lpVtbl -> get_enableErrorDialogs(This,pfEnableErrorDialogs)

#define IWMPSettings_put_enableErrorDialogs(This,fEnableErrorDialogs)	\
    (This)->lpVtbl -> put_enableErrorDialogs(This,fEnableErrorDialogs)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE IWMPSettings_get_isAvailable_Proxy( 
    IWMPSettings * This,
    /* [in] */ BSTR bstrItem,
    /* [retval][out] */ VARIANT_BOOL *pIsAvailable);


void __RPC_STUB IWMPSettings_get_isAvailable_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE IWMPSettings_get_autoStart_Proxy( 
    IWMPSettings * This,
    /* [retval][out] */ VARIANT_BOOL *pfAutoStart);


void __RPC_STUB IWMPSettings_get_autoStart_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propput][id] */ HRESULT STDMETHODCALLTYPE IWMPSettings_put_autoStart_Proxy( 
    IWMPSettings * This,
    /* [in] */ VARIANT_BOOL fAutoStart);


void __RPC_STUB IWMPSettings_put_autoStart_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE IWMPSettings_get_baseURL_Proxy( 
    IWMPSettings * This,
    /* [retval][out] */ BSTR *pbstrBaseURL);


void __RPC_STUB IWMPSettings_get_baseURL_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propput][id] */ HRESULT STDMETHODCALLTYPE IWMPSettings_put_baseURL_Proxy( 
    IWMPSettings * This,
    /* [in] */ BSTR bstrBaseURL);


void __RPC_STUB IWMPSettings_put_baseURL_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE IWMPSettings_get_defaultFrame_Proxy( 
    IWMPSettings * This,
    /* [retval][out] */ BSTR *pbstrDefaultFrame);


void __RPC_STUB IWMPSettings_get_defaultFrame_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propput][id] */ HRESULT STDMETHODCALLTYPE IWMPSettings_put_defaultFrame_Proxy( 
    IWMPSettings * This,
    /* [in] */ BSTR bstrDefaultFrame);


void __RPC_STUB IWMPSettings_put_defaultFrame_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE IWMPSettings_get_invokeURLs_Proxy( 
    IWMPSettings * This,
    /* [retval][out] */ VARIANT_BOOL *pfInvokeURLs);


void __RPC_STUB IWMPSettings_get_invokeURLs_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propput][id] */ HRESULT STDMETHODCALLTYPE IWMPSettings_put_invokeURLs_Proxy( 
    IWMPSettings * This,
    /* [in] */ VARIANT_BOOL fInvokeURLs);


void __RPC_STUB IWMPSettings_put_invokeURLs_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE IWMPSettings_get_mute_Proxy( 
    IWMPSettings * This,
    /* [retval][out] */ VARIANT_BOOL *pfMute);


void __RPC_STUB IWMPSettings_get_mute_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propput][id] */ HRESULT STDMETHODCALLTYPE IWMPSettings_put_mute_Proxy( 
    IWMPSettings * This,
    /* [in] */ VARIANT_BOOL fMute);


void __RPC_STUB IWMPSettings_put_mute_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE IWMPSettings_get_playCount_Proxy( 
    IWMPSettings * This,
    /* [retval][out] */ long *plCount);


void __RPC_STUB IWMPSettings_get_playCount_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propput][id] */ HRESULT STDMETHODCALLTYPE IWMPSettings_put_playCount_Proxy( 
    IWMPSettings * This,
    /* [in] */ long lCount);


void __RPC_STUB IWMPSettings_put_playCount_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE IWMPSettings_get_rate_Proxy( 
    IWMPSettings * This,
    /* [retval][out] */ double *pdRate);


void __RPC_STUB IWMPSettings_get_rate_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propput][id] */ HRESULT STDMETHODCALLTYPE IWMPSettings_put_rate_Proxy( 
    IWMPSettings * This,
    /* [in] */ double dRate);


void __RPC_STUB IWMPSettings_put_rate_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE IWMPSettings_get_balance_Proxy( 
    IWMPSettings * This,
    /* [retval][out] */ long *plBalance);


void __RPC_STUB IWMPSettings_get_balance_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propput][id] */ HRESULT STDMETHODCALLTYPE IWMPSettings_put_balance_Proxy( 
    IWMPSettings * This,
    /* [in] */ long lBalance);


void __RPC_STUB IWMPSettings_put_balance_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE IWMPSettings_get_volume_Proxy( 
    IWMPSettings * This,
    /* [retval][out] */ long *plVolume);


void __RPC_STUB IWMPSettings_get_volume_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propput][id] */ HRESULT STDMETHODCALLTYPE IWMPSettings_put_volume_Proxy( 
    IWMPSettings * This,
    /* [in] */ long lVolume);


void __RPC_STUB IWMPSettings_put_volume_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IWMPSettings_getMode_Proxy( 
    IWMPSettings * This,
    /* [in] */ BSTR bstrMode,
    /* [retval][out] */ VARIANT_BOOL *pvarfMode);


void __RPC_STUB IWMPSettings_getMode_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IWMPSettings_setMode_Proxy( 
    IWMPSettings * This,
    /* [in] */ BSTR bstrMode,
    /* [in] */ VARIANT_BOOL varfMode);


void __RPC_STUB IWMPSettings_setMode_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE IWMPSettings_get_enableErrorDialogs_Proxy( 
    IWMPSettings * This,
    /* [retval][out] */ VARIANT_BOOL *pfEnableErrorDialogs);


void __RPC_STUB IWMPSettings_get_enableErrorDialogs_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propput][id] */ HRESULT STDMETHODCALLTYPE IWMPSettings_put_enableErrorDialogs_Proxy( 
    IWMPSettings * This,
    /* [in] */ VARIANT_BOOL fEnableErrorDialogs);


void __RPC_STUB IWMPSettings_put_enableErrorDialogs_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IWMPSettings_INTERFACE_DEFINED__ */


#ifndef __IWMPClosedCaption_INTERFACE_DEFINED__
#define __IWMPClosedCaption_INTERFACE_DEFINED__

/* interface IWMPClosedCaption */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_IWMPClosedCaption;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("4F2DF574-C588-11d3-9ED0-00C04FB6E937")
    IWMPClosedCaption : public IDispatch
    {
    public:
        virtual /* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE get_SAMIStyle( 
            /* [retval][out] */ BSTR *pbstrSAMIStyle) = 0;
        
        virtual /* [helpstring][propput][id] */ HRESULT STDMETHODCALLTYPE put_SAMIStyle( 
            /* [in] */ BSTR bstrSAMIStyle) = 0;
        
        virtual /* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE get_SAMILang( 
            /* [retval][out] */ BSTR *pbstrSAMILang) = 0;
        
        virtual /* [helpstring][propput][id] */ HRESULT STDMETHODCALLTYPE put_SAMILang( 
            /* [in] */ BSTR bstrSAMILang) = 0;
        
        virtual /* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE get_SAMIFileName( 
            /* [retval][out] */ BSTR *pbstrSAMIFileName) = 0;
        
        virtual /* [helpstring][propput][id] */ HRESULT STDMETHODCALLTYPE put_SAMIFileName( 
            /* [in] */ BSTR bstrSAMIFileName) = 0;
        
        virtual /* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE get_captioningId( 
            /* [retval][out] */ BSTR *pbstrCaptioningID) = 0;
        
        virtual /* [helpstring][propput][id] */ HRESULT STDMETHODCALLTYPE put_captioningId( 
            /* [in] */ BSTR bstrCaptioningID) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IWMPClosedCaptionVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IWMPClosedCaption * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IWMPClosedCaption * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IWMPClosedCaption * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IWMPClosedCaption * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IWMPClosedCaption * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IWMPClosedCaption * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IWMPClosedCaption * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_SAMIStyle )( 
            IWMPClosedCaption * This,
            /* [retval][out] */ BSTR *pbstrSAMIStyle);
        
        /* [helpstring][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_SAMIStyle )( 
            IWMPClosedCaption * This,
            /* [in] */ BSTR bstrSAMIStyle);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_SAMILang )( 
            IWMPClosedCaption * This,
            /* [retval][out] */ BSTR *pbstrSAMILang);
        
        /* [helpstring][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_SAMILang )( 
            IWMPClosedCaption * This,
            /* [in] */ BSTR bstrSAMILang);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_SAMIFileName )( 
            IWMPClosedCaption * This,
            /* [retval][out] */ BSTR *pbstrSAMIFileName);
        
        /* [helpstring][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_SAMIFileName )( 
            IWMPClosedCaption * This,
            /* [in] */ BSTR bstrSAMIFileName);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_captioningId )( 
            IWMPClosedCaption * This,
            /* [retval][out] */ BSTR *pbstrCaptioningID);
        
        /* [helpstring][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_captioningId )( 
            IWMPClosedCaption * This,
            /* [in] */ BSTR bstrCaptioningID);
        
        END_INTERFACE
    } IWMPClosedCaptionVtbl;

    interface IWMPClosedCaption
    {
        CONST_VTBL struct IWMPClosedCaptionVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IWMPClosedCaption_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IWMPClosedCaption_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IWMPClosedCaption_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IWMPClosedCaption_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IWMPClosedCaption_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IWMPClosedCaption_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IWMPClosedCaption_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IWMPClosedCaption_get_SAMIStyle(This,pbstrSAMIStyle)	\
    (This)->lpVtbl -> get_SAMIStyle(This,pbstrSAMIStyle)

#define IWMPClosedCaption_put_SAMIStyle(This,bstrSAMIStyle)	\
    (This)->lpVtbl -> put_SAMIStyle(This,bstrSAMIStyle)

#define IWMPClosedCaption_get_SAMILang(This,pbstrSAMILang)	\
    (This)->lpVtbl -> get_SAMILang(This,pbstrSAMILang)

#define IWMPClosedCaption_put_SAMILang(This,bstrSAMILang)	\
    (This)->lpVtbl -> put_SAMILang(This,bstrSAMILang)

#define IWMPClosedCaption_get_SAMIFileName(This,pbstrSAMIFileName)	\
    (This)->lpVtbl -> get_SAMIFileName(This,pbstrSAMIFileName)

#define IWMPClosedCaption_put_SAMIFileName(This,bstrSAMIFileName)	\
    (This)->lpVtbl -> put_SAMIFileName(This,bstrSAMIFileName)

#define IWMPClosedCaption_get_captioningId(This,pbstrCaptioningID)	\
    (This)->lpVtbl -> get_captioningId(This,pbstrCaptioningID)

#define IWMPClosedCaption_put_captioningId(This,bstrCaptioningID)	\
    (This)->lpVtbl -> put_captioningId(This,bstrCaptioningID)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE IWMPClosedCaption_get_SAMIStyle_Proxy( 
    IWMPClosedCaption * This,
    /* [retval][out] */ BSTR *pbstrSAMIStyle);


void __RPC_STUB IWMPClosedCaption_get_SAMIStyle_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propput][id] */ HRESULT STDMETHODCALLTYPE IWMPClosedCaption_put_SAMIStyle_Proxy( 
    IWMPClosedCaption * This,
    /* [in] */ BSTR bstrSAMIStyle);


void __RPC_STUB IWMPClosedCaption_put_SAMIStyle_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE IWMPClosedCaption_get_SAMILang_Proxy( 
    IWMPClosedCaption * This,
    /* [retval][out] */ BSTR *pbstrSAMILang);


void __RPC_STUB IWMPClosedCaption_get_SAMILang_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propput][id] */ HRESULT STDMETHODCALLTYPE IWMPClosedCaption_put_SAMILang_Proxy( 
    IWMPClosedCaption * This,
    /* [in] */ BSTR bstrSAMILang);


void __RPC_STUB IWMPClosedCaption_put_SAMILang_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE IWMPClosedCaption_get_SAMIFileName_Proxy( 
    IWMPClosedCaption * This,
    /* [retval][out] */ BSTR *pbstrSAMIFileName);


void __RPC_STUB IWMPClosedCaption_get_SAMIFileName_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propput][id] */ HRESULT STDMETHODCALLTYPE IWMPClosedCaption_put_SAMIFileName_Proxy( 
    IWMPClosedCaption * This,
    /* [in] */ BSTR bstrSAMIFileName);


void __RPC_STUB IWMPClosedCaption_put_SAMIFileName_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE IWMPClosedCaption_get_captioningId_Proxy( 
    IWMPClosedCaption * This,
    /* [retval][out] */ BSTR *pbstrCaptioningID);


void __RPC_STUB IWMPClosedCaption_get_captioningId_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propput][id] */ HRESULT STDMETHODCALLTYPE IWMPClosedCaption_put_captioningId_Proxy( 
    IWMPClosedCaption * This,
    /* [in] */ BSTR bstrCaptioningID);


void __RPC_STUB IWMPClosedCaption_put_captioningId_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IWMPClosedCaption_INTERFACE_DEFINED__ */


#ifndef __IWMPPlaylist_INTERFACE_DEFINED__
#define __IWMPPlaylist_INTERFACE_DEFINED__

/* interface IWMPPlaylist */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_IWMPPlaylist;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("D5F0F4F1-130C-11d3-B14E-00C04F79FAA6")
    IWMPPlaylist : public IDispatch
    {
    public:
        virtual /* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE get_count( 
            /* [retval][out] */ long *plCount) = 0;
        
        virtual /* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE get_name( 
            /* [retval][out] */ BSTR *pbstrName) = 0;
        
        virtual /* [helpstring][propput][id] */ HRESULT STDMETHODCALLTYPE put_name( 
            /* [in] */ BSTR bstrName) = 0;
        
        virtual /* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE get_attributeCount( 
            /* [retval][out] */ long *plCount) = 0;
        
        virtual /* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE get_attributeName( 
            /* [in] */ long lIndex,
            /* [retval][out] */ BSTR *pbstrAttributeName) = 0;
        
        virtual /* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE get_item( 
            long lIndex,
            /* [retval][out] */ IWMPMedia **ppIWMPMedia) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE getItemInfo( 
            /* [in] */ BSTR bstrName,
            /* [retval][out] */ BSTR *pbstrVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE setItemInfo( 
            /* [in] */ BSTR bstrName,
            /* [in] */ BSTR bstrValue) = 0;
        
        virtual /* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE get_isIdentical( 
            /* [in] */ IWMPPlaylist *pIWMPPlaylist,
            /* [retval][out] */ VARIANT_BOOL *pvbool) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE clear( void) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE insertItem( 
            /* [in] */ long lIndex,
            /* [in] */ IWMPMedia *pIWMPMedia) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE appendItem( 
            /* [in] */ IWMPMedia *pIWMPMedia) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE removeItem( 
            /* [in] */ IWMPMedia *pIWMPMedia) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE moveItem( 
            long lIndexOld,
            long lIndexNew) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IWMPPlaylistVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IWMPPlaylist * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IWMPPlaylist * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IWMPPlaylist * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IWMPPlaylist * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IWMPPlaylist * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IWMPPlaylist * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IWMPPlaylist * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_count )( 
            IWMPPlaylist * This,
            /* [retval][out] */ long *plCount);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_name )( 
            IWMPPlaylist * This,
            /* [retval][out] */ BSTR *pbstrName);
        
        /* [helpstring][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_name )( 
            IWMPPlaylist * This,
            /* [in] */ BSTR bstrName);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_attributeCount )( 
            IWMPPlaylist * This,
            /* [retval][out] */ long *plCount);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_attributeName )( 
            IWMPPlaylist * This,
            /* [in] */ long lIndex,
            /* [retval][out] */ BSTR *pbstrAttributeName);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_item )( 
            IWMPPlaylist * This,
            long lIndex,
            /* [retval][out] */ IWMPMedia **ppIWMPMedia);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *getItemInfo )( 
            IWMPPlaylist * This,
            /* [in] */ BSTR bstrName,
            /* [retval][out] */ BSTR *pbstrVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *setItemInfo )( 
            IWMPPlaylist * This,
            /* [in] */ BSTR bstrName,
            /* [in] */ BSTR bstrValue);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_isIdentical )( 
            IWMPPlaylist * This,
            /* [in] */ IWMPPlaylist *pIWMPPlaylist,
            /* [retval][out] */ VARIANT_BOOL *pvbool);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *clear )( 
            IWMPPlaylist * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *insertItem )( 
            IWMPPlaylist * This,
            /* [in] */ long lIndex,
            /* [in] */ IWMPMedia *pIWMPMedia);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *appendItem )( 
            IWMPPlaylist * This,
            /* [in] */ IWMPMedia *pIWMPMedia);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *removeItem )( 
            IWMPPlaylist * This,
            /* [in] */ IWMPMedia *pIWMPMedia);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *moveItem )( 
            IWMPPlaylist * This,
            long lIndexOld,
            long lIndexNew);
        
        END_INTERFACE
    } IWMPPlaylistVtbl;

    interface IWMPPlaylist
    {
        CONST_VTBL struct IWMPPlaylistVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IWMPPlaylist_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IWMPPlaylist_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IWMPPlaylist_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IWMPPlaylist_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IWMPPlaylist_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IWMPPlaylist_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IWMPPlaylist_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IWMPPlaylist_get_count(This,plCount)	\
    (This)->lpVtbl -> get_count(This,plCount)

#define IWMPPlaylist_get_name(This,pbstrName)	\
    (This)->lpVtbl -> get_name(This,pbstrName)

#define IWMPPlaylist_put_name(This,bstrName)	\
    (This)->lpVtbl -> put_name(This,bstrName)

#define IWMPPlaylist_get_attributeCount(This,plCount)	\
    (This)->lpVtbl -> get_attributeCount(This,plCount)

#define IWMPPlaylist_get_attributeName(This,lIndex,pbstrAttributeName)	\
    (This)->lpVtbl -> get_attributeName(This,lIndex,pbstrAttributeName)

#define IWMPPlaylist_get_item(This,lIndex,ppIWMPMedia)	\
    (This)->lpVtbl -> get_item(This,lIndex,ppIWMPMedia)

#define IWMPPlaylist_getItemInfo(This,bstrName,pbstrVal)	\
    (This)->lpVtbl -> getItemInfo(This,bstrName,pbstrVal)

#define IWMPPlaylist_setItemInfo(This,bstrName,bstrValue)	\
    (This)->lpVtbl -> setItemInfo(This,bstrName,bstrValue)

#define IWMPPlaylist_get_isIdentical(This,pIWMPPlaylist,pvbool)	\
    (This)->lpVtbl -> get_isIdentical(This,pIWMPPlaylist,pvbool)

#define IWMPPlaylist_clear(This)	\
    (This)->lpVtbl -> clear(This)

#define IWMPPlaylist_insertItem(This,lIndex,pIWMPMedia)	\
    (This)->lpVtbl -> insertItem(This,lIndex,pIWMPMedia)

#define IWMPPlaylist_appendItem(This,pIWMPMedia)	\
    (This)->lpVtbl -> appendItem(This,pIWMPMedia)

#define IWMPPlaylist_removeItem(This,pIWMPMedia)	\
    (This)->lpVtbl -> removeItem(This,pIWMPMedia)

#define IWMPPlaylist_moveItem(This,lIndexOld,lIndexNew)	\
    (This)->lpVtbl -> moveItem(This,lIndexOld,lIndexNew)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE IWMPPlaylist_get_count_Proxy( 
    IWMPPlaylist * This,
    /* [retval][out] */ long *plCount);


void __RPC_STUB IWMPPlaylist_get_count_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE IWMPPlaylist_get_name_Proxy( 
    IWMPPlaylist * This,
    /* [retval][out] */ BSTR *pbstrName);


void __RPC_STUB IWMPPlaylist_get_name_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propput][id] */ HRESULT STDMETHODCALLTYPE IWMPPlaylist_put_name_Proxy( 
    IWMPPlaylist * This,
    /* [in] */ BSTR bstrName);


void __RPC_STUB IWMPPlaylist_put_name_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE IWMPPlaylist_get_attributeCount_Proxy( 
    IWMPPlaylist * This,
    /* [retval][out] */ long *plCount);


void __RPC_STUB IWMPPlaylist_get_attributeCount_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE IWMPPlaylist_get_attributeName_Proxy( 
    IWMPPlaylist * This,
    /* [in] */ long lIndex,
    /* [retval][out] */ BSTR *pbstrAttributeName);


void __RPC_STUB IWMPPlaylist_get_attributeName_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE IWMPPlaylist_get_item_Proxy( 
    IWMPPlaylist * This,
    long lIndex,
    /* [retval][out] */ IWMPMedia **ppIWMPMedia);


void __RPC_STUB IWMPPlaylist_get_item_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IWMPPlaylist_getItemInfo_Proxy( 
    IWMPPlaylist * This,
    /* [in] */ BSTR bstrName,
    /* [retval][out] */ BSTR *pbstrVal);


void __RPC_STUB IWMPPlaylist_getItemInfo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IWMPPlaylist_setItemInfo_Proxy( 
    IWMPPlaylist * This,
    /* [in] */ BSTR bstrName,
    /* [in] */ BSTR bstrValue);


void __RPC_STUB IWMPPlaylist_setItemInfo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE IWMPPlaylist_get_isIdentical_Proxy( 
    IWMPPlaylist * This,
    /* [in] */ IWMPPlaylist *pIWMPPlaylist,
    /* [retval][out] */ VARIANT_BOOL *pvbool);


void __RPC_STUB IWMPPlaylist_get_isIdentical_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IWMPPlaylist_clear_Proxy( 
    IWMPPlaylist * This);


void __RPC_STUB IWMPPlaylist_clear_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IWMPPlaylist_insertItem_Proxy( 
    IWMPPlaylist * This,
    /* [in] */ long lIndex,
    /* [in] */ IWMPMedia *pIWMPMedia);


void __RPC_STUB IWMPPlaylist_insertItem_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IWMPPlaylist_appendItem_Proxy( 
    IWMPPlaylist * This,
    /* [in] */ IWMPMedia *pIWMPMedia);


void __RPC_STUB IWMPPlaylist_appendItem_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IWMPPlaylist_removeItem_Proxy( 
    IWMPPlaylist * This,
    /* [in] */ IWMPMedia *pIWMPMedia);


void __RPC_STUB IWMPPlaylist_removeItem_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IWMPPlaylist_moveItem_Proxy( 
    IWMPPlaylist * This,
    long lIndexOld,
    long lIndexNew);


void __RPC_STUB IWMPPlaylist_moveItem_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IWMPPlaylist_INTERFACE_DEFINED__ */


#ifndef __IWMPCdrom_INTERFACE_DEFINED__
#define __IWMPCdrom_INTERFACE_DEFINED__

/* interface IWMPCdrom */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_IWMPCdrom;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("cfab6e98-8730-11d3-b388-00c04f68574b")
    IWMPCdrom : public IDispatch
    {
    public:
        virtual /* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE get_driveSpecifier( 
            /* [retval][out] */ BSTR *pbstrDrive) = 0;
        
        virtual /* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE get_playlist( 
            /* [retval][out] */ IWMPPlaylist **ppPlaylist) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE eject( void) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IWMPCdromVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IWMPCdrom * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IWMPCdrom * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IWMPCdrom * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IWMPCdrom * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IWMPCdrom * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IWMPCdrom * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IWMPCdrom * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_driveSpecifier )( 
            IWMPCdrom * This,
            /* [retval][out] */ BSTR *pbstrDrive);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_playlist )( 
            IWMPCdrom * This,
            /* [retval][out] */ IWMPPlaylist **ppPlaylist);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *eject )( 
            IWMPCdrom * This);
        
        END_INTERFACE
    } IWMPCdromVtbl;

    interface IWMPCdrom
    {
        CONST_VTBL struct IWMPCdromVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IWMPCdrom_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IWMPCdrom_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IWMPCdrom_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IWMPCdrom_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IWMPCdrom_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IWMPCdrom_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IWMPCdrom_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IWMPCdrom_get_driveSpecifier(This,pbstrDrive)	\
    (This)->lpVtbl -> get_driveSpecifier(This,pbstrDrive)

#define IWMPCdrom_get_playlist(This,ppPlaylist)	\
    (This)->lpVtbl -> get_playlist(This,ppPlaylist)

#define IWMPCdrom_eject(This)	\
    (This)->lpVtbl -> eject(This)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE IWMPCdrom_get_driveSpecifier_Proxy( 
    IWMPCdrom * This,
    /* [retval][out] */ BSTR *pbstrDrive);


void __RPC_STUB IWMPCdrom_get_driveSpecifier_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE IWMPCdrom_get_playlist_Proxy( 
    IWMPCdrom * This,
    /* [retval][out] */ IWMPPlaylist **ppPlaylist);


void __RPC_STUB IWMPCdrom_get_playlist_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IWMPCdrom_eject_Proxy( 
    IWMPCdrom * This);


void __RPC_STUB IWMPCdrom_eject_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IWMPCdrom_INTERFACE_DEFINED__ */


#ifndef __IWMPCdromCollection_INTERFACE_DEFINED__
#define __IWMPCdromCollection_INTERFACE_DEFINED__

/* interface IWMPCdromCollection */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_IWMPCdromCollection;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("EE4C8FE2-34B2-11d3-A3BF-006097C9B344")
    IWMPCdromCollection : public IDispatch
    {
    public:
        virtual /* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE get_count( 
            /* [retval][out] */ long *plCount) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE item( 
            /* [in] */ long lIndex,
            /* [retval][out] */ IWMPCdrom **ppItem) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE getByDriveSpecifier( 
            /* [in] */ BSTR bstrDriveSpecifier,
            /* [retval][out] */ IWMPCdrom **ppCdrom) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IWMPCdromCollectionVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IWMPCdromCollection * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IWMPCdromCollection * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IWMPCdromCollection * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IWMPCdromCollection * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IWMPCdromCollection * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IWMPCdromCollection * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IWMPCdromCollection * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_count )( 
            IWMPCdromCollection * This,
            /* [retval][out] */ long *plCount);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *item )( 
            IWMPCdromCollection * This,
            /* [in] */ long lIndex,
            /* [retval][out] */ IWMPCdrom **ppItem);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *getByDriveSpecifier )( 
            IWMPCdromCollection * This,
            /* [in] */ BSTR bstrDriveSpecifier,
            /* [retval][out] */ IWMPCdrom **ppCdrom);
        
        END_INTERFACE
    } IWMPCdromCollectionVtbl;

    interface IWMPCdromCollection
    {
        CONST_VTBL struct IWMPCdromCollectionVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IWMPCdromCollection_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IWMPCdromCollection_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IWMPCdromCollection_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IWMPCdromCollection_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IWMPCdromCollection_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IWMPCdromCollection_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IWMPCdromCollection_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IWMPCdromCollection_get_count(This,plCount)	\
    (This)->lpVtbl -> get_count(This,plCount)

#define IWMPCdromCollection_item(This,lIndex,ppItem)	\
    (This)->lpVtbl -> item(This,lIndex,ppItem)

#define IWMPCdromCollection_getByDriveSpecifier(This,bstrDriveSpecifier,ppCdrom)	\
    (This)->lpVtbl -> getByDriveSpecifier(This,bstrDriveSpecifier,ppCdrom)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE IWMPCdromCollection_get_count_Proxy( 
    IWMPCdromCollection * This,
    /* [retval][out] */ long *plCount);


void __RPC_STUB IWMPCdromCollection_get_count_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IWMPCdromCollection_item_Proxy( 
    IWMPCdromCollection * This,
    /* [in] */ long lIndex,
    /* [retval][out] */ IWMPCdrom **ppItem);


void __RPC_STUB IWMPCdromCollection_item_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IWMPCdromCollection_getByDriveSpecifier_Proxy( 
    IWMPCdromCollection * This,
    /* [in] */ BSTR bstrDriveSpecifier,
    /* [retval][out] */ IWMPCdrom **ppCdrom);


void __RPC_STUB IWMPCdromCollection_getByDriveSpecifier_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IWMPCdromCollection_INTERFACE_DEFINED__ */


#ifndef __IWMPStringCollection_INTERFACE_DEFINED__
#define __IWMPStringCollection_INTERFACE_DEFINED__

/* interface IWMPStringCollection */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_IWMPStringCollection;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("4a976298-8c0d-11d3-b389-00c04f68574b")
    IWMPStringCollection : public IDispatch
    {
    public:
        virtual /* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE get_count( 
            /* [retval][out] */ long *plCount) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE item( 
            /* [in] */ long lIndex,
            /* [retval][out] */ BSTR *pbstrString) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IWMPStringCollectionVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IWMPStringCollection * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IWMPStringCollection * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IWMPStringCollection * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IWMPStringCollection * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IWMPStringCollection * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IWMPStringCollection * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IWMPStringCollection * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_count )( 
            IWMPStringCollection * This,
            /* [retval][out] */ long *plCount);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *item )( 
            IWMPStringCollection * This,
            /* [in] */ long lIndex,
            /* [retval][out] */ BSTR *pbstrString);
        
        END_INTERFACE
    } IWMPStringCollectionVtbl;

    interface IWMPStringCollection
    {
        CONST_VTBL struct IWMPStringCollectionVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IWMPStringCollection_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IWMPStringCollection_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IWMPStringCollection_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IWMPStringCollection_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IWMPStringCollection_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IWMPStringCollection_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IWMPStringCollection_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IWMPStringCollection_get_count(This,plCount)	\
    (This)->lpVtbl -> get_count(This,plCount)

#define IWMPStringCollection_item(This,lIndex,pbstrString)	\
    (This)->lpVtbl -> item(This,lIndex,pbstrString)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE IWMPStringCollection_get_count_Proxy( 
    IWMPStringCollection * This,
    /* [retval][out] */ long *plCount);


void __RPC_STUB IWMPStringCollection_get_count_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IWMPStringCollection_item_Proxy( 
    IWMPStringCollection * This,
    /* [in] */ long lIndex,
    /* [retval][out] */ BSTR *pbstrString);


void __RPC_STUB IWMPStringCollection_item_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IWMPStringCollection_INTERFACE_DEFINED__ */


#ifndef __IWMPMediaCollection_INTERFACE_DEFINED__
#define __IWMPMediaCollection_INTERFACE_DEFINED__

/* interface IWMPMediaCollection */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_IWMPMediaCollection;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("8363BC22-B4B4-4b19-989D-1CD765749DD1")
    IWMPMediaCollection : public IDispatch
    {
    public:
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE add( 
            /* [in] */ BSTR bstrURL,
            /* [retval][out] */ IWMPMedia **ppItem) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE getAll( 
            /* [retval][out] */ IWMPPlaylist **ppMediaItems) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE getByName( 
            /* [in] */ BSTR bstrName,
            /* [retval][out] */ IWMPPlaylist **ppMediaItems) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE getByGenre( 
            /* [in] */ BSTR bstrGenre,
            /* [retval][out] */ IWMPPlaylist **ppMediaItems) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE getByAuthor( 
            /* [in] */ BSTR bstrAuthor,
            /* [retval][out] */ IWMPPlaylist **ppMediaItems) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE getByAlbum( 
            /* [in] */ BSTR bstrAlbum,
            /* [retval][out] */ IWMPPlaylist **ppMediaItems) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE getByAttribute( 
            /* [in] */ BSTR bstrAttribute,
            /* [in] */ BSTR bstrValue,
            /* [retval][out] */ IWMPPlaylist **ppMediaItems) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE remove( 
            /* [in] */ IWMPMedia *pItem,
            /* [in] */ VARIANT_BOOL varfDeleteFile) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE getAttributeStringCollection( 
            /* [in] */ BSTR bstrAttribute,
            /* [in] */ BSTR bstrMediaType,
            /* [retval][out] */ IWMPStringCollection **ppStringCollection) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE getMediaAtom( 
            /* [in] */ BSTR bstrItemName,
            /* [retval][out] */ long *plAtom) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE setDeleted( 
            /* [in] */ IWMPMedia *pItem,
            /* [in] */ VARIANT_BOOL varfIsDeleted) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE isDeleted( 
            /* [in] */ IWMPMedia *pItem,
            /* [retval][out] */ VARIANT_BOOL *pvarfIsDeleted) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IWMPMediaCollectionVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IWMPMediaCollection * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IWMPMediaCollection * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IWMPMediaCollection * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IWMPMediaCollection * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IWMPMediaCollection * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IWMPMediaCollection * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IWMPMediaCollection * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *add )( 
            IWMPMediaCollection * This,
            /* [in] */ BSTR bstrURL,
            /* [retval][out] */ IWMPMedia **ppItem);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *getAll )( 
            IWMPMediaCollection * This,
            /* [retval][out] */ IWMPPlaylist **ppMediaItems);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *getByName )( 
            IWMPMediaCollection * This,
            /* [in] */ BSTR bstrName,
            /* [retval][out] */ IWMPPlaylist **ppMediaItems);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *getByGenre )( 
            IWMPMediaCollection * This,
            /* [in] */ BSTR bstrGenre,
            /* [retval][out] */ IWMPPlaylist **ppMediaItems);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *getByAuthor )( 
            IWMPMediaCollection * This,
            /* [in] */ BSTR bstrAuthor,
            /* [retval][out] */ IWMPPlaylist **ppMediaItems);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *getByAlbum )( 
            IWMPMediaCollection * This,
            /* [in] */ BSTR bstrAlbum,
            /* [retval][out] */ IWMPPlaylist **ppMediaItems);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *getByAttribute )( 
            IWMPMediaCollection * This,
            /* [in] */ BSTR bstrAttribute,
            /* [in] */ BSTR bstrValue,
            /* [retval][out] */ IWMPPlaylist **ppMediaItems);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *remove )( 
            IWMPMediaCollection * This,
            /* [in] */ IWMPMedia *pItem,
            /* [in] */ VARIANT_BOOL varfDeleteFile);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *getAttributeStringCollection )( 
            IWMPMediaCollection * This,
            /* [in] */ BSTR bstrAttribute,
            /* [in] */ BSTR bstrMediaType,
            /* [retval][out] */ IWMPStringCollection **ppStringCollection);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *getMediaAtom )( 
            IWMPMediaCollection * This,
            /* [in] */ BSTR bstrItemName,
            /* [retval][out] */ long *plAtom);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *setDeleted )( 
            IWMPMediaCollection * This,
            /* [in] */ IWMPMedia *pItem,
            /* [in] */ VARIANT_BOOL varfIsDeleted);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *isDeleted )( 
            IWMPMediaCollection * This,
            /* [in] */ IWMPMedia *pItem,
            /* [retval][out] */ VARIANT_BOOL *pvarfIsDeleted);
        
        END_INTERFACE
    } IWMPMediaCollectionVtbl;

    interface IWMPMediaCollection
    {
        CONST_VTBL struct IWMPMediaCollectionVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IWMPMediaCollection_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IWMPMediaCollection_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IWMPMediaCollection_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IWMPMediaCollection_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IWMPMediaCollection_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IWMPMediaCollection_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IWMPMediaCollection_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IWMPMediaCollection_add(This,bstrURL,ppItem)	\
    (This)->lpVtbl -> add(This,bstrURL,ppItem)

#define IWMPMediaCollection_getAll(This,ppMediaItems)	\
    (This)->lpVtbl -> getAll(This,ppMediaItems)

#define IWMPMediaCollection_getByName(This,bstrName,ppMediaItems)	\
    (This)->lpVtbl -> getByName(This,bstrName,ppMediaItems)

#define IWMPMediaCollection_getByGenre(This,bstrGenre,ppMediaItems)	\
    (This)->lpVtbl -> getByGenre(This,bstrGenre,ppMediaItems)

#define IWMPMediaCollection_getByAuthor(This,bstrAuthor,ppMediaItems)	\
    (This)->lpVtbl -> getByAuthor(This,bstrAuthor,ppMediaItems)

#define IWMPMediaCollection_getByAlbum(This,bstrAlbum,ppMediaItems)	\
    (This)->lpVtbl -> getByAlbum(This,bstrAlbum,ppMediaItems)

#define IWMPMediaCollection_getByAttribute(This,bstrAttribute,bstrValue,ppMediaItems)	\
    (This)->lpVtbl -> getByAttribute(This,bstrAttribute,bstrValue,ppMediaItems)

#define IWMPMediaCollection_remove(This,pItem,varfDeleteFile)	\
    (This)->lpVtbl -> remove(This,pItem,varfDeleteFile)

#define IWMPMediaCollection_getAttributeStringCollection(This,bstrAttribute,bstrMediaType,ppStringCollection)	\
    (This)->lpVtbl -> getAttributeStringCollection(This,bstrAttribute,bstrMediaType,ppStringCollection)

#define IWMPMediaCollection_getMediaAtom(This,bstrItemName,plAtom)	\
    (This)->lpVtbl -> getMediaAtom(This,bstrItemName,plAtom)

#define IWMPMediaCollection_setDeleted(This,pItem,varfIsDeleted)	\
    (This)->lpVtbl -> setDeleted(This,pItem,varfIsDeleted)

#define IWMPMediaCollection_isDeleted(This,pItem,pvarfIsDeleted)	\
    (This)->lpVtbl -> isDeleted(This,pItem,pvarfIsDeleted)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IWMPMediaCollection_add_Proxy( 
    IWMPMediaCollection * This,
    /* [in] */ BSTR bstrURL,
    /* [retval][out] */ IWMPMedia **ppItem);


void __RPC_STUB IWMPMediaCollection_add_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IWMPMediaCollection_getAll_Proxy( 
    IWMPMediaCollection * This,
    /* [retval][out] */ IWMPPlaylist **ppMediaItems);


void __RPC_STUB IWMPMediaCollection_getAll_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IWMPMediaCollection_getByName_Proxy( 
    IWMPMediaCollection * This,
    /* [in] */ BSTR bstrName,
    /* [retval][out] */ IWMPPlaylist **ppMediaItems);


void __RPC_STUB IWMPMediaCollection_getByName_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IWMPMediaCollection_getByGenre_Proxy( 
    IWMPMediaCollection * This,
    /* [in] */ BSTR bstrGenre,
    /* [retval][out] */ IWMPPlaylist **ppMediaItems);


void __RPC_STUB IWMPMediaCollection_getByGenre_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IWMPMediaCollection_getByAuthor_Proxy( 
    IWMPMediaCollection * This,
    /* [in] */ BSTR bstrAuthor,
    /* [retval][out] */ IWMPPlaylist **ppMediaItems);


void __RPC_STUB IWMPMediaCollection_getByAuthor_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IWMPMediaCollection_getByAlbum_Proxy( 
    IWMPMediaCollection * This,
    /* [in] */ BSTR bstrAlbum,
    /* [retval][out] */ IWMPPlaylist **ppMediaItems);


void __RPC_STUB IWMPMediaCollection_getByAlbum_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IWMPMediaCollection_getByAttribute_Proxy( 
    IWMPMediaCollection * This,
    /* [in] */ BSTR bstrAttribute,
    /* [in] */ BSTR bstrValue,
    /* [retval][out] */ IWMPPlaylist **ppMediaItems);


void __RPC_STUB IWMPMediaCollection_getByAttribute_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IWMPMediaCollection_remove_Proxy( 
    IWMPMediaCollection * This,
    /* [in] */ IWMPMedia *pItem,
    /* [in] */ VARIANT_BOOL varfDeleteFile);


void __RPC_STUB IWMPMediaCollection_remove_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IWMPMediaCollection_getAttributeStringCollection_Proxy( 
    IWMPMediaCollection * This,
    /* [in] */ BSTR bstrAttribute,
    /* [in] */ BSTR bstrMediaType,
    /* [retval][out] */ IWMPStringCollection **ppStringCollection);


void __RPC_STUB IWMPMediaCollection_getAttributeStringCollection_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IWMPMediaCollection_getMediaAtom_Proxy( 
    IWMPMediaCollection * This,
    /* [in] */ BSTR bstrItemName,
    /* [retval][out] */ long *plAtom);


void __RPC_STUB IWMPMediaCollection_getMediaAtom_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IWMPMediaCollection_setDeleted_Proxy( 
    IWMPMediaCollection * This,
    /* [in] */ IWMPMedia *pItem,
    /* [in] */ VARIANT_BOOL varfIsDeleted);


void __RPC_STUB IWMPMediaCollection_setDeleted_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IWMPMediaCollection_isDeleted_Proxy( 
    IWMPMediaCollection * This,
    /* [in] */ IWMPMedia *pItem,
    /* [retval][out] */ VARIANT_BOOL *pvarfIsDeleted);


void __RPC_STUB IWMPMediaCollection_isDeleted_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IWMPMediaCollection_INTERFACE_DEFINED__ */


#ifndef __IWMPPlaylistArray_INTERFACE_DEFINED__
#define __IWMPPlaylistArray_INTERFACE_DEFINED__

/* interface IWMPPlaylistArray */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_IWMPPlaylistArray;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("679409c0-99f7-11d3-9fb7-00105aa620bb")
    IWMPPlaylistArray : public IDispatch
    {
    public:
        virtual /* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE get_count( 
            /* [retval][out] */ long *plCount) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE item( 
            /* [in] */ long lIndex,
            /* [retval][out] */ IWMPPlaylist **ppItem) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IWMPPlaylistArrayVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IWMPPlaylistArray * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IWMPPlaylistArray * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IWMPPlaylistArray * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IWMPPlaylistArray * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IWMPPlaylistArray * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IWMPPlaylistArray * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IWMPPlaylistArray * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_count )( 
            IWMPPlaylistArray * This,
            /* [retval][out] */ long *plCount);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *item )( 
            IWMPPlaylistArray * This,
            /* [in] */ long lIndex,
            /* [retval][out] */ IWMPPlaylist **ppItem);
        
        END_INTERFACE
    } IWMPPlaylistArrayVtbl;

    interface IWMPPlaylistArray
    {
        CONST_VTBL struct IWMPPlaylistArrayVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IWMPPlaylistArray_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IWMPPlaylistArray_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IWMPPlaylistArray_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IWMPPlaylistArray_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IWMPPlaylistArray_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IWMPPlaylistArray_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IWMPPlaylistArray_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IWMPPlaylistArray_get_count(This,plCount)	\
    (This)->lpVtbl -> get_count(This,plCount)

#define IWMPPlaylistArray_item(This,lIndex,ppItem)	\
    (This)->lpVtbl -> item(This,lIndex,ppItem)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE IWMPPlaylistArray_get_count_Proxy( 
    IWMPPlaylistArray * This,
    /* [retval][out] */ long *plCount);


void __RPC_STUB IWMPPlaylistArray_get_count_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IWMPPlaylistArray_item_Proxy( 
    IWMPPlaylistArray * This,
    /* [in] */ long lIndex,
    /* [retval][out] */ IWMPPlaylist **ppItem);


void __RPC_STUB IWMPPlaylistArray_item_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IWMPPlaylistArray_INTERFACE_DEFINED__ */


#ifndef __IWMPPlaylistCollection_INTERFACE_DEFINED__
#define __IWMPPlaylistCollection_INTERFACE_DEFINED__

/* interface IWMPPlaylistCollection */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_IWMPPlaylistCollection;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("10A13217-23A7-439b-B1C0-D847C79B7774")
    IWMPPlaylistCollection : public IDispatch
    {
    public:
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE newPlaylist( 
            /* [in] */ BSTR bstrName,
            /* [retval][out] */ IWMPPlaylist **ppItem) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE getAll( 
            /* [retval][out] */ IWMPPlaylistArray **ppPlaylistArray) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE getByName( 
            /* [in] */ BSTR bstrName,
            /* [retval][out] */ IWMPPlaylistArray **ppPlaylistArray) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE remove( 
            /* [in] */ IWMPPlaylist *pItem) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE setDeleted( 
            /* [in] */ IWMPPlaylist *pItem,
            /* [in] */ VARIANT_BOOL varfIsDeleted) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE isDeleted( 
            /* [in] */ IWMPPlaylist *pItem,
            /* [retval][out] */ VARIANT_BOOL *pvarfIsDeleted) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE importPlaylist( 
            /* [in] */ IWMPPlaylist *pItem,
            /* [retval][out] */ IWMPPlaylist **ppImportedItem) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IWMPPlaylistCollectionVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IWMPPlaylistCollection * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IWMPPlaylistCollection * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IWMPPlaylistCollection * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IWMPPlaylistCollection * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IWMPPlaylistCollection * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IWMPPlaylistCollection * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IWMPPlaylistCollection * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *newPlaylist )( 
            IWMPPlaylistCollection * This,
            /* [in] */ BSTR bstrName,
            /* [retval][out] */ IWMPPlaylist **ppItem);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *getAll )( 
            IWMPPlaylistCollection * This,
            /* [retval][out] */ IWMPPlaylistArray **ppPlaylistArray);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *getByName )( 
            IWMPPlaylistCollection * This,
            /* [in] */ BSTR bstrName,
            /* [retval][out] */ IWMPPlaylistArray **ppPlaylistArray);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *remove )( 
            IWMPPlaylistCollection * This,
            /* [in] */ IWMPPlaylist *pItem);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *setDeleted )( 
            IWMPPlaylistCollection * This,
            /* [in] */ IWMPPlaylist *pItem,
            /* [in] */ VARIANT_BOOL varfIsDeleted);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *isDeleted )( 
            IWMPPlaylistCollection * This,
            /* [in] */ IWMPPlaylist *pItem,
            /* [retval][out] */ VARIANT_BOOL *pvarfIsDeleted);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *importPlaylist )( 
            IWMPPlaylistCollection * This,
            /* [in] */ IWMPPlaylist *pItem,
            /* [retval][out] */ IWMPPlaylist **ppImportedItem);
        
        END_INTERFACE
    } IWMPPlaylistCollectionVtbl;

    interface IWMPPlaylistCollection
    {
        CONST_VTBL struct IWMPPlaylistCollectionVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IWMPPlaylistCollection_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IWMPPlaylistCollection_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IWMPPlaylistCollection_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IWMPPlaylistCollection_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IWMPPlaylistCollection_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IWMPPlaylistCollection_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IWMPPlaylistCollection_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IWMPPlaylistCollection_newPlaylist(This,bstrName,ppItem)	\
    (This)->lpVtbl -> newPlaylist(This,bstrName,ppItem)

#define IWMPPlaylistCollection_getAll(This,ppPlaylistArray)	\
    (This)->lpVtbl -> getAll(This,ppPlaylistArray)

#define IWMPPlaylistCollection_getByName(This,bstrName,ppPlaylistArray)	\
    (This)->lpVtbl -> getByName(This,bstrName,ppPlaylistArray)

#define IWMPPlaylistCollection_remove(This,pItem)	\
    (This)->lpVtbl -> remove(This,pItem)

#define IWMPPlaylistCollection_setDeleted(This,pItem,varfIsDeleted)	\
    (This)->lpVtbl -> setDeleted(This,pItem,varfIsDeleted)

#define IWMPPlaylistCollection_isDeleted(This,pItem,pvarfIsDeleted)	\
    (This)->lpVtbl -> isDeleted(This,pItem,pvarfIsDeleted)

#define IWMPPlaylistCollection_importPlaylist(This,pItem,ppImportedItem)	\
    (This)->lpVtbl -> importPlaylist(This,pItem,ppImportedItem)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IWMPPlaylistCollection_newPlaylist_Proxy( 
    IWMPPlaylistCollection * This,
    /* [in] */ BSTR bstrName,
    /* [retval][out] */ IWMPPlaylist **ppItem);


void __RPC_STUB IWMPPlaylistCollection_newPlaylist_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IWMPPlaylistCollection_getAll_Proxy( 
    IWMPPlaylistCollection * This,
    /* [retval][out] */ IWMPPlaylistArray **ppPlaylistArray);


void __RPC_STUB IWMPPlaylistCollection_getAll_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IWMPPlaylistCollection_getByName_Proxy( 
    IWMPPlaylistCollection * This,
    /* [in] */ BSTR bstrName,
    /* [retval][out] */ IWMPPlaylistArray **ppPlaylistArray);


void __RPC_STUB IWMPPlaylistCollection_getByName_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IWMPPlaylistCollection_remove_Proxy( 
    IWMPPlaylistCollection * This,
    /* [in] */ IWMPPlaylist *pItem);


void __RPC_STUB IWMPPlaylistCollection_remove_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IWMPPlaylistCollection_setDeleted_Proxy( 
    IWMPPlaylistCollection * This,
    /* [in] */ IWMPPlaylist *pItem,
    /* [in] */ VARIANT_BOOL varfIsDeleted);


void __RPC_STUB IWMPPlaylistCollection_setDeleted_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IWMPPlaylistCollection_isDeleted_Proxy( 
    IWMPPlaylistCollection * This,
    /* [in] */ IWMPPlaylist *pItem,
    /* [retval][out] */ VARIANT_BOOL *pvarfIsDeleted);


void __RPC_STUB IWMPPlaylistCollection_isDeleted_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IWMPPlaylistCollection_importPlaylist_Proxy( 
    IWMPPlaylistCollection * This,
    /* [in] */ IWMPPlaylist *pItem,
    /* [retval][out] */ IWMPPlaylist **ppImportedItem);


void __RPC_STUB IWMPPlaylistCollection_importPlaylist_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IWMPPlaylistCollection_INTERFACE_DEFINED__ */


#ifndef __IWMPNetwork_INTERFACE_DEFINED__
#define __IWMPNetwork_INTERFACE_DEFINED__

/* interface IWMPNetwork */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_IWMPNetwork;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("EC21B779-EDEF-462d-BBA4-AD9DDE2B29A7")
    IWMPNetwork : public IDispatch
    {
    public:
        virtual /* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE get_bandWidth( 
            /* [retval][out] */ long *plBandwidth) = 0;
        
        virtual /* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE get_recoveredPackets( 
            /* [retval][out] */ long *plRecoveredPackets) = 0;
        
        virtual /* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE get_sourceProtocol( 
            /* [retval][out] */ BSTR *pbstrSourceProtocol) = 0;
        
        virtual /* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE get_receivedPackets( 
            /* [retval][out] */ long *plReceivedPackets) = 0;
        
        virtual /* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE get_lostPackets( 
            /* [retval][out] */ long *plLostPackets) = 0;
        
        virtual /* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE get_receptionQuality( 
            /* [retval][out] */ long *plReceptionQuality) = 0;
        
        virtual /* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE get_bufferingCount( 
            /* [retval][out] */ long *plBufferingCount) = 0;
        
        virtual /* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE get_bufferingProgress( 
            /* [retval][out] */ long *plBufferingProgress) = 0;
        
        virtual /* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE get_bufferingTime( 
            /* [retval][out] */ long *plBufferingTime) = 0;
        
        virtual /* [helpstring][propput][id] */ HRESULT STDMETHODCALLTYPE put_bufferingTime( 
            /* [in] */ long lBufferingTime) = 0;
        
        virtual /* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE get_frameRate( 
            /* [retval][out] */ long *plFrameRate) = 0;
        
        virtual /* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE get_maxBitRate( 
            /* [retval][out] */ long *plBitRate) = 0;
        
        virtual /* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE get_bitRate( 
            /* [retval][out] */ long *plBitRate) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE getProxySettings( 
            /* [in] */ BSTR bstrProtocol,
            /* [retval][out] */ long *plProxySetting) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE setProxySettings( 
            /* [in] */ BSTR bstrProtocol,
            /* [in] */ long lProxySetting) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE getProxyName( 
            /* [in] */ BSTR bstrProtocol,
            /* [retval][out] */ BSTR *pbstrProxyName) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE setProxyName( 
            /* [in] */ BSTR bstrProtocol,
            /* [in] */ BSTR bstrProxyName) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE getProxyPort( 
            /* [in] */ BSTR bstrProtocol,
            /* [retval][out] */ long *lProxyPort) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE setProxyPort( 
            /* [in] */ BSTR bstrProtocol,
            /* [in] */ long lProxyPort) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE getProxyExceptionList( 
            /* [in] */ BSTR bstrProtocol,
            /* [retval][out] */ BSTR *pbstrExceptionList) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE setProxyExceptionList( 
            /* [in] */ BSTR bstrProtocol,
            /* [in] */ BSTR pbstrExceptionList) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE getProxyBypassForLocal( 
            /* [in] */ BSTR bstrProtocol,
            /* [retval][out] */ VARIANT_BOOL *pfBypassForLocal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE setProxyBypassForLocal( 
            /* [in] */ BSTR bstrProtocol,
            /* [in] */ VARIANT_BOOL fBypassForLocal) = 0;
        
        virtual /* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE get_maxBandwidth( 
            /* [retval][out] */ long *lMaxBandwidth) = 0;
        
        virtual /* [propput][id] */ HRESULT STDMETHODCALLTYPE put_maxBandwidth( 
            /* [in] */ long lMaxBandwidth) = 0;
        
        virtual /* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE get_downloadProgress( 
            /* [retval][out] */ long *plDownloadProgress) = 0;
        
        virtual /* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE get_encodedFrameRate( 
            /* [retval][out] */ long *plFrameRate) = 0;
        
        virtual /* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE get_framesSkipped( 
            /* [retval][out] */ long *plFrames) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IWMPNetworkVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IWMPNetwork * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IWMPNetwork * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IWMPNetwork * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IWMPNetwork * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IWMPNetwork * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IWMPNetwork * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IWMPNetwork * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_bandWidth )( 
            IWMPNetwork * This,
            /* [retval][out] */ long *plBandwidth);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_recoveredPackets )( 
            IWMPNetwork * This,
            /* [retval][out] */ long *plRecoveredPackets);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_sourceProtocol )( 
            IWMPNetwork * This,
            /* [retval][out] */ BSTR *pbstrSourceProtocol);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_receivedPackets )( 
            IWMPNetwork * This,
            /* [retval][out] */ long *plReceivedPackets);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_lostPackets )( 
            IWMPNetwork * This,
            /* [retval][out] */ long *plLostPackets);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_receptionQuality )( 
            IWMPNetwork * This,
            /* [retval][out] */ long *plReceptionQuality);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_bufferingCount )( 
            IWMPNetwork * This,
            /* [retval][out] */ long *plBufferingCount);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_bufferingProgress )( 
            IWMPNetwork * This,
            /* [retval][out] */ long *plBufferingProgress);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_bufferingTime )( 
            IWMPNetwork * This,
            /* [retval][out] */ long *plBufferingTime);
        
        /* [helpstring][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_bufferingTime )( 
            IWMPNetwork * This,
            /* [in] */ long lBufferingTime);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_frameRate )( 
            IWMPNetwork * This,
            /* [retval][out] */ long *plFrameRate);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_maxBitRate )( 
            IWMPNetwork * This,
            /* [retval][out] */ long *plBitRate);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_bitRate )( 
            IWMPNetwork * This,
            /* [retval][out] */ long *plBitRate);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *getProxySettings )( 
            IWMPNetwork * This,
            /* [in] */ BSTR bstrProtocol,
            /* [retval][out] */ long *plProxySetting);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *setProxySettings )( 
            IWMPNetwork * This,
            /* [in] */ BSTR bstrProtocol,
            /* [in] */ long lProxySetting);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *getProxyName )( 
            IWMPNetwork * This,
            /* [in] */ BSTR bstrProtocol,
            /* [retval][out] */ BSTR *pbstrProxyName);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *setProxyName )( 
            IWMPNetwork * This,
            /* [in] */ BSTR bstrProtocol,
            /* [in] */ BSTR bstrProxyName);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *getProxyPort )( 
            IWMPNetwork * This,
            /* [in] */ BSTR bstrProtocol,
            /* [retval][out] */ long *lProxyPort);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *setProxyPort )( 
            IWMPNetwork * This,
            /* [in] */ BSTR bstrProtocol,
            /* [in] */ long lProxyPort);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *getProxyExceptionList )( 
            IWMPNetwork * This,
            /* [in] */ BSTR bstrProtocol,
            /* [retval][out] */ BSTR *pbstrExceptionList);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *setProxyExceptionList )( 
            IWMPNetwork * This,
            /* [in] */ BSTR bstrProtocol,
            /* [in] */ BSTR pbstrExceptionList);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *getProxyBypassForLocal )( 
            IWMPNetwork * This,
            /* [in] */ BSTR bstrProtocol,
            /* [retval][out] */ VARIANT_BOOL *pfBypassForLocal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *setProxyBypassForLocal )( 
            IWMPNetwork * This,
            /* [in] */ BSTR bstrProtocol,
            /* [in] */ VARIANT_BOOL fBypassForLocal);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_maxBandwidth )( 
            IWMPNetwork * This,
            /* [retval][out] */ long *lMaxBandwidth);
        
        /* [propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_maxBandwidth )( 
            IWMPNetwork * This,
            /* [in] */ long lMaxBandwidth);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_downloadProgress )( 
            IWMPNetwork * This,
            /* [retval][out] */ long *plDownloadProgress);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_encodedFrameRate )( 
            IWMPNetwork * This,
            /* [retval][out] */ long *plFrameRate);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_framesSkipped )( 
            IWMPNetwork * This,
            /* [retval][out] */ long *plFrames);
        
        END_INTERFACE
    } IWMPNetworkVtbl;

    interface IWMPNetwork
    {
        CONST_VTBL struct IWMPNetworkVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IWMPNetwork_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IWMPNetwork_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IWMPNetwork_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IWMPNetwork_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IWMPNetwork_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IWMPNetwork_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IWMPNetwork_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IWMPNetwork_get_bandWidth(This,plBandwidth)	\
    (This)->lpVtbl -> get_bandWidth(This,plBandwidth)

#define IWMPNetwork_get_recoveredPackets(This,plRecoveredPackets)	\
    (This)->lpVtbl -> get_recoveredPackets(This,plRecoveredPackets)

#define IWMPNetwork_get_sourceProtocol(This,pbstrSourceProtocol)	\
    (This)->lpVtbl -> get_sourceProtocol(This,pbstrSourceProtocol)

#define IWMPNetwork_get_receivedPackets(This,plReceivedPackets)	\
    (This)->lpVtbl -> get_receivedPackets(This,plReceivedPackets)

#define IWMPNetwork_get_lostPackets(This,plLostPackets)	\
    (This)->lpVtbl -> get_lostPackets(This,plLostPackets)

#define IWMPNetwork_get_receptionQuality(This,plReceptionQuality)	\
    (This)->lpVtbl -> get_receptionQuality(This,plReceptionQuality)

#define IWMPNetwork_get_bufferingCount(This,plBufferingCount)	\
    (This)->lpVtbl -> get_bufferingCount(This,plBufferingCount)

#define IWMPNetwork_get_bufferingProgress(This,plBufferingProgress)	\
    (This)->lpVtbl -> get_bufferingProgress(This,plBufferingProgress)

#define IWMPNetwork_get_bufferingTime(This,plBufferingTime)	\
    (This)->lpVtbl -> get_bufferingTime(This,plBufferingTime)

#define IWMPNetwork_put_bufferingTime(This,lBufferingTime)	\
    (This)->lpVtbl -> put_bufferingTime(This,lBufferingTime)

#define IWMPNetwork_get_frameRate(This,plFrameRate)	\
    (This)->lpVtbl -> get_frameRate(This,plFrameRate)

#define IWMPNetwork_get_maxBitRate(This,plBitRate)	\
    (This)->lpVtbl -> get_maxBitRate(This,plBitRate)

#define IWMPNetwork_get_bitRate(This,plBitRate)	\
    (This)->lpVtbl -> get_bitRate(This,plBitRate)

#define IWMPNetwork_getProxySettings(This,bstrProtocol,plProxySetting)	\
    (This)->lpVtbl -> getProxySettings(This,bstrProtocol,plProxySetting)

#define IWMPNetwork_setProxySettings(This,bstrProtocol,lProxySetting)	\
    (This)->lpVtbl -> setProxySettings(This,bstrProtocol,lProxySetting)

#define IWMPNetwork_getProxyName(This,bstrProtocol,pbstrProxyName)	\
    (This)->lpVtbl -> getProxyName(This,bstrProtocol,pbstrProxyName)

#define IWMPNetwork_setProxyName(This,bstrProtocol,bstrProxyName)	\
    (This)->lpVtbl -> setProxyName(This,bstrProtocol,bstrProxyName)

#define IWMPNetwork_getProxyPort(This,bstrProtocol,lProxyPort)	\
    (This)->lpVtbl -> getProxyPort(This,bstrProtocol,lProxyPort)

#define IWMPNetwork_setProxyPort(This,bstrProtocol,lProxyPort)	\
    (This)->lpVtbl -> setProxyPort(This,bstrProtocol,lProxyPort)

#define IWMPNetwork_getProxyExceptionList(This,bstrProtocol,pbstrExceptionList)	\
    (This)->lpVtbl -> getProxyExceptionList(This,bstrProtocol,pbstrExceptionList)

#define IWMPNetwork_setProxyExceptionList(This,bstrProtocol,pbstrExceptionList)	\
    (This)->lpVtbl -> setProxyExceptionList(This,bstrProtocol,pbstrExceptionList)

#define IWMPNetwork_getProxyBypassForLocal(This,bstrProtocol,pfBypassForLocal)	\
    (This)->lpVtbl -> getProxyBypassForLocal(This,bstrProtocol,pfBypassForLocal)

#define IWMPNetwork_setProxyBypassForLocal(This,bstrProtocol,fBypassForLocal)	\
    (This)->lpVtbl -> setProxyBypassForLocal(This,bstrProtocol,fBypassForLocal)

#define IWMPNetwork_get_maxBandwidth(This,lMaxBandwidth)	\
    (This)->lpVtbl -> get_maxBandwidth(This,lMaxBandwidth)

#define IWMPNetwork_put_maxBandwidth(This,lMaxBandwidth)	\
    (This)->lpVtbl -> put_maxBandwidth(This,lMaxBandwidth)

#define IWMPNetwork_get_downloadProgress(This,plDownloadProgress)	\
    (This)->lpVtbl -> get_downloadProgress(This,plDownloadProgress)

#define IWMPNetwork_get_encodedFrameRate(This,plFrameRate)	\
    (This)->lpVtbl -> get_encodedFrameRate(This,plFrameRate)

#define IWMPNetwork_get_framesSkipped(This,plFrames)	\
    (This)->lpVtbl -> get_framesSkipped(This,plFrames)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE IWMPNetwork_get_bandWidth_Proxy( 
    IWMPNetwork * This,
    /* [retval][out] */ long *plBandwidth);


void __RPC_STUB IWMPNetwork_get_bandWidth_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE IWMPNetwork_get_recoveredPackets_Proxy( 
    IWMPNetwork * This,
    /* [retval][out] */ long *plRecoveredPackets);


void __RPC_STUB IWMPNetwork_get_recoveredPackets_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE IWMPNetwork_get_sourceProtocol_Proxy( 
    IWMPNetwork * This,
    /* [retval][out] */ BSTR *pbstrSourceProtocol);


void __RPC_STUB IWMPNetwork_get_sourceProtocol_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE IWMPNetwork_get_receivedPackets_Proxy( 
    IWMPNetwork * This,
    /* [retval][out] */ long *plReceivedPackets);


void __RPC_STUB IWMPNetwork_get_receivedPackets_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE IWMPNetwork_get_lostPackets_Proxy( 
    IWMPNetwork * This,
    /* [retval][out] */ long *plLostPackets);


void __RPC_STUB IWMPNetwork_get_lostPackets_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE IWMPNetwork_get_receptionQuality_Proxy( 
    IWMPNetwork * This,
    /* [retval][out] */ long *plReceptionQuality);


void __RPC_STUB IWMPNetwork_get_receptionQuality_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE IWMPNetwork_get_bufferingCount_Proxy( 
    IWMPNetwork * This,
    /* [retval][out] */ long *plBufferingCount);


void __RPC_STUB IWMPNetwork_get_bufferingCount_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE IWMPNetwork_get_bufferingProgress_Proxy( 
    IWMPNetwork * This,
    /* [retval][out] */ long *plBufferingProgress);


void __RPC_STUB IWMPNetwork_get_bufferingProgress_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE IWMPNetwork_get_bufferingTime_Proxy( 
    IWMPNetwork * This,
    /* [retval][out] */ long *plBufferingTime);


void __RPC_STUB IWMPNetwork_get_bufferingTime_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propput][id] */ HRESULT STDMETHODCALLTYPE IWMPNetwork_put_bufferingTime_Proxy( 
    IWMPNetwork * This,
    /* [in] */ long lBufferingTime);


void __RPC_STUB IWMPNetwork_put_bufferingTime_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE IWMPNetwork_get_frameRate_Proxy( 
    IWMPNetwork * This,
    /* [retval][out] */ long *plFrameRate);


void __RPC_STUB IWMPNetwork_get_frameRate_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE IWMPNetwork_get_maxBitRate_Proxy( 
    IWMPNetwork * This,
    /* [retval][out] */ long *plBitRate);


void __RPC_STUB IWMPNetwork_get_maxBitRate_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE IWMPNetwork_get_bitRate_Proxy( 
    IWMPNetwork * This,
    /* [retval][out] */ long *plBitRate);


void __RPC_STUB IWMPNetwork_get_bitRate_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IWMPNetwork_getProxySettings_Proxy( 
    IWMPNetwork * This,
    /* [in] */ BSTR bstrProtocol,
    /* [retval][out] */ long *plProxySetting);


void __RPC_STUB IWMPNetwork_getProxySettings_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IWMPNetwork_setProxySettings_Proxy( 
    IWMPNetwork * This,
    /* [in] */ BSTR bstrProtocol,
    /* [in] */ long lProxySetting);


void __RPC_STUB IWMPNetwork_setProxySettings_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IWMPNetwork_getProxyName_Proxy( 
    IWMPNetwork * This,
    /* [in] */ BSTR bstrProtocol,
    /* [retval][out] */ BSTR *pbstrProxyName);


void __RPC_STUB IWMPNetwork_getProxyName_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IWMPNetwork_setProxyName_Proxy( 
    IWMPNetwork * This,
    /* [in] */ BSTR bstrProtocol,
    /* [in] */ BSTR bstrProxyName);


void __RPC_STUB IWMPNetwork_setProxyName_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IWMPNetwork_getProxyPort_Proxy( 
    IWMPNetwork * This,
    /* [in] */ BSTR bstrProtocol,
    /* [retval][out] */ long *lProxyPort);


void __RPC_STUB IWMPNetwork_getProxyPort_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IWMPNetwork_setProxyPort_Proxy( 
    IWMPNetwork * This,
    /* [in] */ BSTR bstrProtocol,
    /* [in] */ long lProxyPort);


void __RPC_STUB IWMPNetwork_setProxyPort_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IWMPNetwork_getProxyExceptionList_Proxy( 
    IWMPNetwork * This,
    /* [in] */ BSTR bstrProtocol,
    /* [retval][out] */ BSTR *pbstrExceptionList);


void __RPC_STUB IWMPNetwork_getProxyExceptionList_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IWMPNetwork_setProxyExceptionList_Proxy( 
    IWMPNetwork * This,
    /* [in] */ BSTR bstrProtocol,
    /* [in] */ BSTR pbstrExceptionList);


void __RPC_STUB IWMPNetwork_setProxyExceptionList_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IWMPNetwork_getProxyBypassForLocal_Proxy( 
    IWMPNetwork * This,
    /* [in] */ BSTR bstrProtocol,
    /* [retval][out] */ VARIANT_BOOL *pfBypassForLocal);


void __RPC_STUB IWMPNetwork_getProxyBypassForLocal_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IWMPNetwork_setProxyBypassForLocal_Proxy( 
    IWMPNetwork * This,
    /* [in] */ BSTR bstrProtocol,
    /* [in] */ VARIANT_BOOL fBypassForLocal);


void __RPC_STUB IWMPNetwork_setProxyBypassForLocal_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE IWMPNetwork_get_maxBandwidth_Proxy( 
    IWMPNetwork * This,
    /* [retval][out] */ long *lMaxBandwidth);


void __RPC_STUB IWMPNetwork_get_maxBandwidth_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [propput][id] */ HRESULT STDMETHODCALLTYPE IWMPNetwork_put_maxBandwidth_Proxy( 
    IWMPNetwork * This,
    /* [in] */ long lMaxBandwidth);


void __RPC_STUB IWMPNetwork_put_maxBandwidth_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE IWMPNetwork_get_downloadProgress_Proxy( 
    IWMPNetwork * This,
    /* [retval][out] */ long *plDownloadProgress);


void __RPC_STUB IWMPNetwork_get_downloadProgress_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE IWMPNetwork_get_encodedFrameRate_Proxy( 
    IWMPNetwork * This,
    /* [retval][out] */ long *plFrameRate);


void __RPC_STUB IWMPNetwork_get_encodedFrameRate_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE IWMPNetwork_get_framesSkipped_Proxy( 
    IWMPNetwork * This,
    /* [retval][out] */ long *plFrames);


void __RPC_STUB IWMPNetwork_get_framesSkipped_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IWMPNetwork_INTERFACE_DEFINED__ */


#ifndef __IWMPCore_INTERFACE_DEFINED__
#define __IWMPCore_INTERFACE_DEFINED__

/* interface IWMPCore */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_IWMPCore;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("D84CCA99-CCE2-11d2-9ECC-0000F8085981")
    IWMPCore : public IDispatch
    {
    public:
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE close( void) = 0;
        
        virtual /* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE get_URL( 
            /* [retval][out] */ BSTR *pbstrURL) = 0;
        
        virtual /* [propput][id] */ HRESULT STDMETHODCALLTYPE put_URL( 
            /* [in] */ BSTR bstrURL) = 0;
        
        virtual /* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE get_openState( 
            /* [retval][out] */ WMPOpenState *pwmpos) = 0;
        
        virtual /* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE get_playState( 
            /* [retval][out] */ WMPPlayState *pwmpps) = 0;
        
        virtual /* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE get_controls( 
            /* [retval][out] */ IWMPControls **ppControl) = 0;
        
        virtual /* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE get_settings( 
            /* [retval][out] */ IWMPSettings **ppSettings) = 0;
        
        virtual /* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE get_currentMedia( 
            /* [retval][out] */ IWMPMedia **ppMedia) = 0;
        
        virtual /* [propput][id] */ HRESULT STDMETHODCALLTYPE put_currentMedia( 
            /* [in] */ IWMPMedia *pMedia) = 0;
        
        virtual /* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE get_mediaCollection( 
            /* [retval][out] */ IWMPMediaCollection **ppMediaCollection) = 0;
        
        virtual /* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE get_playlistCollection( 
            /* [retval][out] */ IWMPPlaylistCollection **ppPlaylistCollection) = 0;
        
        virtual /* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE get_versionInfo( 
            /* [retval][out] */ BSTR *pbstrVersionInfo) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE launchURL( 
            /* [in] */ BSTR bstrURL) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_network( 
            /* [retval][out] */ IWMPNetwork **ppQNI) = 0;
        
        virtual /* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE get_currentPlaylist( 
            /* [retval][out] */ IWMPPlaylist **ppPL) = 0;
        
        virtual /* [helpstring][propput][id] */ HRESULT STDMETHODCALLTYPE put_currentPlaylist( 
            /* [in] */ IWMPPlaylist *pPL) = 0;
        
        virtual /* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE get_cdromCollection( 
            /* [retval][out] */ IWMPCdromCollection **ppCdromCollection) = 0;
        
        virtual /* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE get_closedCaption( 
            /* [retval][out] */ IWMPClosedCaption **ppClosedCaption) = 0;
        
        virtual /* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE get_isOnline( 
            /* [retval][out] */ VARIANT_BOOL *pfOnline) = 0;
        
        virtual /* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE get_error( 
            /* [retval][out] */ IWMPError **ppError) = 0;
        
        virtual /* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE get_status( 
            /* [retval][out] */ BSTR *pbstrStatus) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IWMPCoreVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IWMPCore * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IWMPCore * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IWMPCore * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IWMPCore * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IWMPCore * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IWMPCore * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IWMPCore * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *close )( 
            IWMPCore * This);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_URL )( 
            IWMPCore * This,
            /* [retval][out] */ BSTR *pbstrURL);
        
        /* [propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_URL )( 
            IWMPCore * This,
            /* [in] */ BSTR bstrURL);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_openState )( 
            IWMPCore * This,
            /* [retval][out] */ WMPOpenState *pwmpos);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_playState )( 
            IWMPCore * This,
            /* [retval][out] */ WMPPlayState *pwmpps);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_controls )( 
            IWMPCore * This,
            /* [retval][out] */ IWMPControls **ppControl);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_settings )( 
            IWMPCore * This,
            /* [retval][out] */ IWMPSettings **ppSettings);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_currentMedia )( 
            IWMPCore * This,
            /* [retval][out] */ IWMPMedia **ppMedia);
        
        /* [propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_currentMedia )( 
            IWMPCore * This,
            /* [in] */ IWMPMedia *pMedia);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_mediaCollection )( 
            IWMPCore * This,
            /* [retval][out] */ IWMPMediaCollection **ppMediaCollection);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_playlistCollection )( 
            IWMPCore * This,
            /* [retval][out] */ IWMPPlaylistCollection **ppPlaylistCollection);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_versionInfo )( 
            IWMPCore * This,
            /* [retval][out] */ BSTR *pbstrVersionInfo);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *launchURL )( 
            IWMPCore * This,
            /* [in] */ BSTR bstrURL);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_network )( 
            IWMPCore * This,
            /* [retval][out] */ IWMPNetwork **ppQNI);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_currentPlaylist )( 
            IWMPCore * This,
            /* [retval][out] */ IWMPPlaylist **ppPL);
        
        /* [helpstring][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_currentPlaylist )( 
            IWMPCore * This,
            /* [in] */ IWMPPlaylist *pPL);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_cdromCollection )( 
            IWMPCore * This,
            /* [retval][out] */ IWMPCdromCollection **ppCdromCollection);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_closedCaption )( 
            IWMPCore * This,
            /* [retval][out] */ IWMPClosedCaption **ppClosedCaption);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_isOnline )( 
            IWMPCore * This,
            /* [retval][out] */ VARIANT_BOOL *pfOnline);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_error )( 
            IWMPCore * This,
            /* [retval][out] */ IWMPError **ppError);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_status )( 
            IWMPCore * This,
            /* [retval][out] */ BSTR *pbstrStatus);
        
        END_INTERFACE
    } IWMPCoreVtbl;

    interface IWMPCore
    {
        CONST_VTBL struct IWMPCoreVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IWMPCore_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IWMPCore_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IWMPCore_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IWMPCore_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IWMPCore_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IWMPCore_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IWMPCore_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IWMPCore_close(This)	\
    (This)->lpVtbl -> close(This)

#define IWMPCore_get_URL(This,pbstrURL)	\
    (This)->lpVtbl -> get_URL(This,pbstrURL)

#define IWMPCore_put_URL(This,bstrURL)	\
    (This)->lpVtbl -> put_URL(This,bstrURL)

#define IWMPCore_get_openState(This,pwmpos)	\
    (This)->lpVtbl -> get_openState(This,pwmpos)

#define IWMPCore_get_playState(This,pwmpps)	\
    (This)->lpVtbl -> get_playState(This,pwmpps)

#define IWMPCore_get_controls(This,ppControl)	\
    (This)->lpVtbl -> get_controls(This,ppControl)

#define IWMPCore_get_settings(This,ppSettings)	\
    (This)->lpVtbl -> get_settings(This,ppSettings)

#define IWMPCore_get_currentMedia(This,ppMedia)	\
    (This)->lpVtbl -> get_currentMedia(This,ppMedia)

#define IWMPCore_put_currentMedia(This,pMedia)	\
    (This)->lpVtbl -> put_currentMedia(This,pMedia)

#define IWMPCore_get_mediaCollection(This,ppMediaCollection)	\
    (This)->lpVtbl -> get_mediaCollection(This,ppMediaCollection)

#define IWMPCore_get_playlistCollection(This,ppPlaylistCollection)	\
    (This)->lpVtbl -> get_playlistCollection(This,ppPlaylistCollection)

#define IWMPCore_get_versionInfo(This,pbstrVersionInfo)	\
    (This)->lpVtbl -> get_versionInfo(This,pbstrVersionInfo)

#define IWMPCore_launchURL(This,bstrURL)	\
    (This)->lpVtbl -> launchURL(This,bstrURL)

#define IWMPCore_get_network(This,ppQNI)	\
    (This)->lpVtbl -> get_network(This,ppQNI)

#define IWMPCore_get_currentPlaylist(This,ppPL)	\
    (This)->lpVtbl -> get_currentPlaylist(This,ppPL)

#define IWMPCore_put_currentPlaylist(This,pPL)	\
    (This)->lpVtbl -> put_currentPlaylist(This,pPL)

#define IWMPCore_get_cdromCollection(This,ppCdromCollection)	\
    (This)->lpVtbl -> get_cdromCollection(This,ppCdromCollection)

#define IWMPCore_get_closedCaption(This,ppClosedCaption)	\
    (This)->lpVtbl -> get_closedCaption(This,ppClosedCaption)

#define IWMPCore_get_isOnline(This,pfOnline)	\
    (This)->lpVtbl -> get_isOnline(This,pfOnline)

#define IWMPCore_get_error(This,ppError)	\
    (This)->lpVtbl -> get_error(This,ppError)

#define IWMPCore_get_status(This,pbstrStatus)	\
    (This)->lpVtbl -> get_status(This,pbstrStatus)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IWMPCore_close_Proxy( 
    IWMPCore * This);


void __RPC_STUB IWMPCore_close_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE IWMPCore_get_URL_Proxy( 
    IWMPCore * This,
    /* [retval][out] */ BSTR *pbstrURL);


void __RPC_STUB IWMPCore_get_URL_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [propput][id] */ HRESULT STDMETHODCALLTYPE IWMPCore_put_URL_Proxy( 
    IWMPCore * This,
    /* [in] */ BSTR bstrURL);


void __RPC_STUB IWMPCore_put_URL_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE IWMPCore_get_openState_Proxy( 
    IWMPCore * This,
    /* [retval][out] */ WMPOpenState *pwmpos);


void __RPC_STUB IWMPCore_get_openState_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE IWMPCore_get_playState_Proxy( 
    IWMPCore * This,
    /* [retval][out] */ WMPPlayState *pwmpps);


void __RPC_STUB IWMPCore_get_playState_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE IWMPCore_get_controls_Proxy( 
    IWMPCore * This,
    /* [retval][out] */ IWMPControls **ppControl);


void __RPC_STUB IWMPCore_get_controls_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE IWMPCore_get_settings_Proxy( 
    IWMPCore * This,
    /* [retval][out] */ IWMPSettings **ppSettings);


void __RPC_STUB IWMPCore_get_settings_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE IWMPCore_get_currentMedia_Proxy( 
    IWMPCore * This,
    /* [retval][out] */ IWMPMedia **ppMedia);


void __RPC_STUB IWMPCore_get_currentMedia_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [propput][id] */ HRESULT STDMETHODCALLTYPE IWMPCore_put_currentMedia_Proxy( 
    IWMPCore * This,
    /* [in] */ IWMPMedia *pMedia);


void __RPC_STUB IWMPCore_put_currentMedia_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE IWMPCore_get_mediaCollection_Proxy( 
    IWMPCore * This,
    /* [retval][out] */ IWMPMediaCollection **ppMediaCollection);


void __RPC_STUB IWMPCore_get_mediaCollection_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE IWMPCore_get_playlistCollection_Proxy( 
    IWMPCore * This,
    /* [retval][out] */ IWMPPlaylistCollection **ppPlaylistCollection);


void __RPC_STUB IWMPCore_get_playlistCollection_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE IWMPCore_get_versionInfo_Proxy( 
    IWMPCore * This,
    /* [retval][out] */ BSTR *pbstrVersionInfo);


void __RPC_STUB IWMPCore_get_versionInfo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id] */ HRESULT STDMETHODCALLTYPE IWMPCore_launchURL_Proxy( 
    IWMPCore * This,
    /* [in] */ BSTR bstrURL);


void __RPC_STUB IWMPCore_launchURL_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IWMPCore_get_network_Proxy( 
    IWMPCore * This,
    /* [retval][out] */ IWMPNetwork **ppQNI);


void __RPC_STUB IWMPCore_get_network_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE IWMPCore_get_currentPlaylist_Proxy( 
    IWMPCore * This,
    /* [retval][out] */ IWMPPlaylist **ppPL);


void __RPC_STUB IWMPCore_get_currentPlaylist_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propput][id] */ HRESULT STDMETHODCALLTYPE IWMPCore_put_currentPlaylist_Proxy( 
    IWMPCore * This,
    /* [in] */ IWMPPlaylist *pPL);


void __RPC_STUB IWMPCore_put_currentPlaylist_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE IWMPCore_get_cdromCollection_Proxy( 
    IWMPCore * This,
    /* [retval][out] */ IWMPCdromCollection **ppCdromCollection);


void __RPC_STUB IWMPCore_get_cdromCollection_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE IWMPCore_get_closedCaption_Proxy( 
    IWMPCore * This,
    /* [retval][out] */ IWMPClosedCaption **ppClosedCaption);


void __RPC_STUB IWMPCore_get_closedCaption_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE IWMPCore_get_isOnline_Proxy( 
    IWMPCore * This,
    /* [retval][out] */ VARIANT_BOOL *pfOnline);


void __RPC_STUB IWMPCore_get_isOnline_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE IWMPCore_get_error_Proxy( 
    IWMPCore * This,
    /* [retval][out] */ IWMPError **ppError);


void __RPC_STUB IWMPCore_get_error_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE IWMPCore_get_status_Proxy( 
    IWMPCore * This,
    /* [retval][out] */ BSTR *pbstrStatus);


void __RPC_STUB IWMPCore_get_status_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IWMPCore_INTERFACE_DEFINED__ */


#ifndef __IWMPPlayer_INTERFACE_DEFINED__
#define __IWMPPlayer_INTERFACE_DEFINED__

/* interface IWMPPlayer */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_IWMPPlayer;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("6BF52A4F-394A-11d3-B153-00C04F79FAA6")
    IWMPPlayer : public IWMPCore
    {
    public:
        virtual /* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE get_enabled( 
            /* [retval][out] */ VARIANT_BOOL *pbEnabled) = 0;
        
        virtual /* [helpstring][propput][id] */ HRESULT STDMETHODCALLTYPE put_enabled( 
            /* [in] */ VARIANT_BOOL bEnabled) = 0;
        
        virtual /* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE get_fullScreen( 
            /* [retval][out] */ VARIANT_BOOL *pbFullScreen) = 0;
        
        virtual /* [helpstring][propput][id] */ HRESULT STDMETHODCALLTYPE put_fullScreen( 
            VARIANT_BOOL bFullScreen) = 0;
        
        virtual /* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE get_enableContextMenu( 
            /* [retval][out] */ VARIANT_BOOL *pbEnableContextMenu) = 0;
        
        virtual /* [helpstring][propput][id] */ HRESULT STDMETHODCALLTYPE put_enableContextMenu( 
            VARIANT_BOOL bEnableContextMenu) = 0;
        
        virtual /* [helpstring][propput][id] */ HRESULT STDMETHODCALLTYPE put_uiMode( 
            /* [in] */ BSTR bstrMode) = 0;
        
        virtual /* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE get_uiMode( 
            /* [retval][out] */ BSTR *pbstrMode) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IWMPPlayerVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IWMPPlayer * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IWMPPlayer * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IWMPPlayer * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IWMPPlayer * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IWMPPlayer * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IWMPPlayer * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IWMPPlayer * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *close )( 
            IWMPPlayer * This);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_URL )( 
            IWMPPlayer * This,
            /* [retval][out] */ BSTR *pbstrURL);
        
        /* [propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_URL )( 
            IWMPPlayer * This,
            /* [in] */ BSTR bstrURL);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_openState )( 
            IWMPPlayer * This,
            /* [retval][out] */ WMPOpenState *pwmpos);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_playState )( 
            IWMPPlayer * This,
            /* [retval][out] */ WMPPlayState *pwmpps);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_controls )( 
            IWMPPlayer * This,
            /* [retval][out] */ IWMPControls **ppControl);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_settings )( 
            IWMPPlayer * This,
            /* [retval][out] */ IWMPSettings **ppSettings);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_currentMedia )( 
            IWMPPlayer * This,
            /* [retval][out] */ IWMPMedia **ppMedia);
        
        /* [propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_currentMedia )( 
            IWMPPlayer * This,
            /* [in] */ IWMPMedia *pMedia);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_mediaCollection )( 
            IWMPPlayer * This,
            /* [retval][out] */ IWMPMediaCollection **ppMediaCollection);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_playlistCollection )( 
            IWMPPlayer * This,
            /* [retval][out] */ IWMPPlaylistCollection **ppPlaylistCollection);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_versionInfo )( 
            IWMPPlayer * This,
            /* [retval][out] */ BSTR *pbstrVersionInfo);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *launchURL )( 
            IWMPPlayer * This,
            /* [in] */ BSTR bstrURL);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_network )( 
            IWMPPlayer * This,
            /* [retval][out] */ IWMPNetwork **ppQNI);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_currentPlaylist )( 
            IWMPPlayer * This,
            /* [retval][out] */ IWMPPlaylist **ppPL);
        
        /* [helpstring][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_currentPlaylist )( 
            IWMPPlayer * This,
            /* [in] */ IWMPPlaylist *pPL);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_cdromCollection )( 
            IWMPPlayer * This,
            /* [retval][out] */ IWMPCdromCollection **ppCdromCollection);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_closedCaption )( 
            IWMPPlayer * This,
            /* [retval][out] */ IWMPClosedCaption **ppClosedCaption);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_isOnline )( 
            IWMPPlayer * This,
            /* [retval][out] */ VARIANT_BOOL *pfOnline);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_error )( 
            IWMPPlayer * This,
            /* [retval][out] */ IWMPError **ppError);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_status )( 
            IWMPPlayer * This,
            /* [retval][out] */ BSTR *pbstrStatus);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_enabled )( 
            IWMPPlayer * This,
            /* [retval][out] */ VARIANT_BOOL *pbEnabled);
        
        /* [helpstring][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_enabled )( 
            IWMPPlayer * This,
            /* [in] */ VARIANT_BOOL bEnabled);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_fullScreen )( 
            IWMPPlayer * This,
            /* [retval][out] */ VARIANT_BOOL *pbFullScreen);
        
        /* [helpstring][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_fullScreen )( 
            IWMPPlayer * This,
            VARIANT_BOOL bFullScreen);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_enableContextMenu )( 
            IWMPPlayer * This,
            /* [retval][out] */ VARIANT_BOOL *pbEnableContextMenu);
        
        /* [helpstring][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_enableContextMenu )( 
            IWMPPlayer * This,
            VARIANT_BOOL bEnableContextMenu);
        
        /* [helpstring][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_uiMode )( 
            IWMPPlayer * This,
            /* [in] */ BSTR bstrMode);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_uiMode )( 
            IWMPPlayer * This,
            /* [retval][out] */ BSTR *pbstrMode);
        
        END_INTERFACE
    } IWMPPlayerVtbl;

    interface IWMPPlayer
    {
        CONST_VTBL struct IWMPPlayerVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IWMPPlayer_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IWMPPlayer_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IWMPPlayer_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IWMPPlayer_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IWMPPlayer_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IWMPPlayer_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IWMPPlayer_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IWMPPlayer_close(This)	\
    (This)->lpVtbl -> close(This)

#define IWMPPlayer_get_URL(This,pbstrURL)	\
    (This)->lpVtbl -> get_URL(This,pbstrURL)

#define IWMPPlayer_put_URL(This,bstrURL)	\
    (This)->lpVtbl -> put_URL(This,bstrURL)

#define IWMPPlayer_get_openState(This,pwmpos)	\
    (This)->lpVtbl -> get_openState(This,pwmpos)

#define IWMPPlayer_get_playState(This,pwmpps)	\
    (This)->lpVtbl -> get_playState(This,pwmpps)

#define IWMPPlayer_get_controls(This,ppControl)	\
    (This)->lpVtbl -> get_controls(This,ppControl)

#define IWMPPlayer_get_settings(This,ppSettings)	\
    (This)->lpVtbl -> get_settings(This,ppSettings)

#define IWMPPlayer_get_currentMedia(This,ppMedia)	\
    (This)->lpVtbl -> get_currentMedia(This,ppMedia)

#define IWMPPlayer_put_currentMedia(This,pMedia)	\
    (This)->lpVtbl -> put_currentMedia(This,pMedia)

#define IWMPPlayer_get_mediaCollection(This,ppMediaCollection)	\
    (This)->lpVtbl -> get_mediaCollection(This,ppMediaCollection)

#define IWMPPlayer_get_playlistCollection(This,ppPlaylistCollection)	\
    (This)->lpVtbl -> get_playlistCollection(This,ppPlaylistCollection)

#define IWMPPlayer_get_versionInfo(This,pbstrVersionInfo)	\
    (This)->lpVtbl -> get_versionInfo(This,pbstrVersionInfo)

#define IWMPPlayer_launchURL(This,bstrURL)	\
    (This)->lpVtbl -> launchURL(This,bstrURL)

#define IWMPPlayer_get_network(This,ppQNI)	\
    (This)->lpVtbl -> get_network(This,ppQNI)

#define IWMPPlayer_get_currentPlaylist(This,ppPL)	\
    (This)->lpVtbl -> get_currentPlaylist(This,ppPL)

#define IWMPPlayer_put_currentPlaylist(This,pPL)	\
    (This)->lpVtbl -> put_currentPlaylist(This,pPL)

#define IWMPPlayer_get_cdromCollection(This,ppCdromCollection)	\
    (This)->lpVtbl -> get_cdromCollection(This,ppCdromCollection)

#define IWMPPlayer_get_closedCaption(This,ppClosedCaption)	\
    (This)->lpVtbl -> get_closedCaption(This,ppClosedCaption)

#define IWMPPlayer_get_isOnline(This,pfOnline)	\
    (This)->lpVtbl -> get_isOnline(This,pfOnline)

#define IWMPPlayer_get_error(This,ppError)	\
    (This)->lpVtbl -> get_error(This,ppError)

#define IWMPPlayer_get_status(This,pbstrStatus)	\
    (This)->lpVtbl -> get_status(This,pbstrStatus)


#define IWMPPlayer_get_enabled(This,pbEnabled)	\
    (This)->lpVtbl -> get_enabled(This,pbEnabled)

#define IWMPPlayer_put_enabled(This,bEnabled)	\
    (This)->lpVtbl -> put_enabled(This,bEnabled)

#define IWMPPlayer_get_fullScreen(This,pbFullScreen)	\
    (This)->lpVtbl -> get_fullScreen(This,pbFullScreen)

#define IWMPPlayer_put_fullScreen(This,bFullScreen)	\
    (This)->lpVtbl -> put_fullScreen(This,bFullScreen)

#define IWMPPlayer_get_enableContextMenu(This,pbEnableContextMenu)	\
    (This)->lpVtbl -> get_enableContextMenu(This,pbEnableContextMenu)

#define IWMPPlayer_put_enableContextMenu(This,bEnableContextMenu)	\
    (This)->lpVtbl -> put_enableContextMenu(This,bEnableContextMenu)

#define IWMPPlayer_put_uiMode(This,bstrMode)	\
    (This)->lpVtbl -> put_uiMode(This,bstrMode)

#define IWMPPlayer_get_uiMode(This,pbstrMode)	\
    (This)->lpVtbl -> get_uiMode(This,pbstrMode)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE IWMPPlayer_get_enabled_Proxy( 
    IWMPPlayer * This,
    /* [retval][out] */ VARIANT_BOOL *pbEnabled);


void __RPC_STUB IWMPPlayer_get_enabled_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propput][id] */ HRESULT STDMETHODCALLTYPE IWMPPlayer_put_enabled_Proxy( 
    IWMPPlayer * This,
    /* [in] */ VARIANT_BOOL bEnabled);


void __RPC_STUB IWMPPlayer_put_enabled_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE IWMPPlayer_get_fullScreen_Proxy( 
    IWMPPlayer * This,
    /* [retval][out] */ VARIANT_BOOL *pbFullScreen);


void __RPC_STUB IWMPPlayer_get_fullScreen_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propput][id] */ HRESULT STDMETHODCALLTYPE IWMPPlayer_put_fullScreen_Proxy( 
    IWMPPlayer * This,
    VARIANT_BOOL bFullScreen);


void __RPC_STUB IWMPPlayer_put_fullScreen_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE IWMPPlayer_get_enableContextMenu_Proxy( 
    IWMPPlayer * This,
    /* [retval][out] */ VARIANT_BOOL *pbEnableContextMenu);


void __RPC_STUB IWMPPlayer_get_enableContextMenu_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propput][id] */ HRESULT STDMETHODCALLTYPE IWMPPlayer_put_enableContextMenu_Proxy( 
    IWMPPlayer * This,
    VARIANT_BOOL bEnableContextMenu);


void __RPC_STUB IWMPPlayer_put_enableContextMenu_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propput][id] */ HRESULT STDMETHODCALLTYPE IWMPPlayer_put_uiMode_Proxy( 
    IWMPPlayer * This,
    /* [in] */ BSTR bstrMode);


void __RPC_STUB IWMPPlayer_put_uiMode_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE IWMPPlayer_get_uiMode_Proxy( 
    IWMPPlayer * This,
    /* [retval][out] */ BSTR *pbstrMode);


void __RPC_STUB IWMPPlayer_get_uiMode_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IWMPPlayer_INTERFACE_DEFINED__ */


#ifndef __IWMPPlayer2_INTERFACE_DEFINED__
#define __IWMPPlayer2_INTERFACE_DEFINED__

/* interface IWMPPlayer2 */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_IWMPPlayer2;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("0E6B01D1-D407-4c85-BF5F-1C01F6150280")
    IWMPPlayer2 : public IWMPCore
    {
    public:
        virtual /* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE get_enabled( 
            /* [retval][out] */ VARIANT_BOOL *pbEnabled) = 0;
        
        virtual /* [helpstring][propput][id] */ HRESULT STDMETHODCALLTYPE put_enabled( 
            /* [in] */ VARIANT_BOOL bEnabled) = 0;
        
        virtual /* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE get_fullScreen( 
            /* [retval][out] */ VARIANT_BOOL *pbFullScreen) = 0;
        
        virtual /* [helpstring][propput][id] */ HRESULT STDMETHODCALLTYPE put_fullScreen( 
            VARIANT_BOOL bFullScreen) = 0;
        
        virtual /* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE get_enableContextMenu( 
            /* [retval][out] */ VARIANT_BOOL *pbEnableContextMenu) = 0;
        
        virtual /* [helpstring][propput][id] */ HRESULT STDMETHODCALLTYPE put_enableContextMenu( 
            VARIANT_BOOL bEnableContextMenu) = 0;
        
        virtual /* [helpstring][propput][id] */ HRESULT STDMETHODCALLTYPE put_uiMode( 
            /* [in] */ BSTR bstrMode) = 0;
        
        virtual /* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE get_uiMode( 
            /* [retval][out] */ BSTR *pbstrMode) = 0;
        
        virtual /* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE get_stretchToFit( 
            /* [retval][out] */ VARIANT_BOOL *pbEnabled) = 0;
        
        virtual /* [helpstring][propput][id] */ HRESULT STDMETHODCALLTYPE put_stretchToFit( 
            /* [in] */ VARIANT_BOOL bEnabled) = 0;
        
        virtual /* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE get_windowlessVideo( 
            /* [retval][out] */ VARIANT_BOOL *pbEnabled) = 0;
        
        virtual /* [helpstring][propput][id] */ HRESULT STDMETHODCALLTYPE put_windowlessVideo( 
            /* [in] */ VARIANT_BOOL bEnabled) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IWMPPlayer2Vtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IWMPPlayer2 * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IWMPPlayer2 * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IWMPPlayer2 * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IWMPPlayer2 * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IWMPPlayer2 * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IWMPPlayer2 * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IWMPPlayer2 * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *close )( 
            IWMPPlayer2 * This);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_URL )( 
            IWMPPlayer2 * This,
            /* [retval][out] */ BSTR *pbstrURL);
        
        /* [propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_URL )( 
            IWMPPlayer2 * This,
            /* [in] */ BSTR bstrURL);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_openState )( 
            IWMPPlayer2 * This,
            /* [retval][out] */ WMPOpenState *pwmpos);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_playState )( 
            IWMPPlayer2 * This,
            /* [retval][out] */ WMPPlayState *pwmpps);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_controls )( 
            IWMPPlayer2 * This,
            /* [retval][out] */ IWMPControls **ppControl);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_settings )( 
            IWMPPlayer2 * This,
            /* [retval][out] */ IWMPSettings **ppSettings);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_currentMedia )( 
            IWMPPlayer2 * This,
            /* [retval][out] */ IWMPMedia **ppMedia);
        
        /* [propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_currentMedia )( 
            IWMPPlayer2 * This,
            /* [in] */ IWMPMedia *pMedia);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_mediaCollection )( 
            IWMPPlayer2 * This,
            /* [retval][out] */ IWMPMediaCollection **ppMediaCollection);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_playlistCollection )( 
            IWMPPlayer2 * This,
            /* [retval][out] */ IWMPPlaylistCollection **ppPlaylistCollection);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_versionInfo )( 
            IWMPPlayer2 * This,
            /* [retval][out] */ BSTR *pbstrVersionInfo);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *launchURL )( 
            IWMPPlayer2 * This,
            /* [in] */ BSTR bstrURL);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_network )( 
            IWMPPlayer2 * This,
            /* [retval][out] */ IWMPNetwork **ppQNI);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_currentPlaylist )( 
            IWMPPlayer2 * This,
            /* [retval][out] */ IWMPPlaylist **ppPL);
        
        /* [helpstring][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_currentPlaylist )( 
            IWMPPlayer2 * This,
            /* [in] */ IWMPPlaylist *pPL);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_cdromCollection )( 
            IWMPPlayer2 * This,
            /* [retval][out] */ IWMPCdromCollection **ppCdromCollection);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_closedCaption )( 
            IWMPPlayer2 * This,
            /* [retval][out] */ IWMPClosedCaption **ppClosedCaption);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_isOnline )( 
            IWMPPlayer2 * This,
            /* [retval][out] */ VARIANT_BOOL *pfOnline);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_error )( 
            IWMPPlayer2 * This,
            /* [retval][out] */ IWMPError **ppError);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_status )( 
            IWMPPlayer2 * This,
            /* [retval][out] */ BSTR *pbstrStatus);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_enabled )( 
            IWMPPlayer2 * This,
            /* [retval][out] */ VARIANT_BOOL *pbEnabled);
        
        /* [helpstring][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_enabled )( 
            IWMPPlayer2 * This,
            /* [in] */ VARIANT_BOOL bEnabled);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_fullScreen )( 
            IWMPPlayer2 * This,
            /* [retval][out] */ VARIANT_BOOL *pbFullScreen);
        
        /* [helpstring][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_fullScreen )( 
            IWMPPlayer2 * This,
            VARIANT_BOOL bFullScreen);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_enableContextMenu )( 
            IWMPPlayer2 * This,
            /* [retval][out] */ VARIANT_BOOL *pbEnableContextMenu);
        
        /* [helpstring][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_enableContextMenu )( 
            IWMPPlayer2 * This,
            VARIANT_BOOL bEnableContextMenu);
        
        /* [helpstring][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_uiMode )( 
            IWMPPlayer2 * This,
            /* [in] */ BSTR bstrMode);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_uiMode )( 
            IWMPPlayer2 * This,
            /* [retval][out] */ BSTR *pbstrMode);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_stretchToFit )( 
            IWMPPlayer2 * This,
            /* [retval][out] */ VARIANT_BOOL *pbEnabled);
        
        /* [helpstring][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_stretchToFit )( 
            IWMPPlayer2 * This,
            /* [in] */ VARIANT_BOOL bEnabled);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_windowlessVideo )( 
            IWMPPlayer2 * This,
            /* [retval][out] */ VARIANT_BOOL *pbEnabled);
        
        /* [helpstring][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_windowlessVideo )( 
            IWMPPlayer2 * This,
            /* [in] */ VARIANT_BOOL bEnabled);
        
        END_INTERFACE
    } IWMPPlayer2Vtbl;

    interface IWMPPlayer2
    {
        CONST_VTBL struct IWMPPlayer2Vtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IWMPPlayer2_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IWMPPlayer2_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IWMPPlayer2_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IWMPPlayer2_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IWMPPlayer2_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IWMPPlayer2_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IWMPPlayer2_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IWMPPlayer2_close(This)	\
    (This)->lpVtbl -> close(This)

#define IWMPPlayer2_get_URL(This,pbstrURL)	\
    (This)->lpVtbl -> get_URL(This,pbstrURL)

#define IWMPPlayer2_put_URL(This,bstrURL)	\
    (This)->lpVtbl -> put_URL(This,bstrURL)

#define IWMPPlayer2_get_openState(This,pwmpos)	\
    (This)->lpVtbl -> get_openState(This,pwmpos)

#define IWMPPlayer2_get_playState(This,pwmpps)	\
    (This)->lpVtbl -> get_playState(This,pwmpps)

#define IWMPPlayer2_get_controls(This,ppControl)	\
    (This)->lpVtbl -> get_controls(This,ppControl)

#define IWMPPlayer2_get_settings(This,ppSettings)	\
    (This)->lpVtbl -> get_settings(This,ppSettings)

#define IWMPPlayer2_get_currentMedia(This,ppMedia)	\
    (This)->lpVtbl -> get_currentMedia(This,ppMedia)

#define IWMPPlayer2_put_currentMedia(This,pMedia)	\
    (This)->lpVtbl -> put_currentMedia(This,pMedia)

#define IWMPPlayer2_get_mediaCollection(This,ppMediaCollection)	\
    (This)->lpVtbl -> get_mediaCollection(This,ppMediaCollection)

#define IWMPPlayer2_get_playlistCollection(This,ppPlaylistCollection)	\
    (This)->lpVtbl -> get_playlistCollection(This,ppPlaylistCollection)

#define IWMPPlayer2_get_versionInfo(This,pbstrVersionInfo)	\
    (This)->lpVtbl -> get_versionInfo(This,pbstrVersionInfo)

#define IWMPPlayer2_launchURL(This,bstrURL)	\
    (This)->lpVtbl -> launchURL(This,bstrURL)

#define IWMPPlayer2_get_network(This,ppQNI)	\
    (This)->lpVtbl -> get_network(This,ppQNI)

#define IWMPPlayer2_get_currentPlaylist(This,ppPL)	\
    (This)->lpVtbl -> get_currentPlaylist(This,ppPL)

#define IWMPPlayer2_put_currentPlaylist(This,pPL)	\
    (This)->lpVtbl -> put_currentPlaylist(This,pPL)

#define IWMPPlayer2_get_cdromCollection(This,ppCdromCollection)	\
    (This)->lpVtbl -> get_cdromCollection(This,ppCdromCollection)

#define IWMPPlayer2_get_closedCaption(This,ppClosedCaption)	\
    (This)->lpVtbl -> get_closedCaption(This,ppClosedCaption)

#define IWMPPlayer2_get_isOnline(This,pfOnline)	\
    (This)->lpVtbl -> get_isOnline(This,pfOnline)

#define IWMPPlayer2_get_error(This,ppError)	\
    (This)->lpVtbl -> get_error(This,ppError)

#define IWMPPlayer2_get_status(This,pbstrStatus)	\
    (This)->lpVtbl -> get_status(This,pbstrStatus)


#define IWMPPlayer2_get_enabled(This,pbEnabled)	\
    (This)->lpVtbl -> get_enabled(This,pbEnabled)

#define IWMPPlayer2_put_enabled(This,bEnabled)	\
    (This)->lpVtbl -> put_enabled(This,bEnabled)

#define IWMPPlayer2_get_fullScreen(This,pbFullScreen)	\
    (This)->lpVtbl -> get_fullScreen(This,pbFullScreen)

#define IWMPPlayer2_put_fullScreen(This,bFullScreen)	\
    (This)->lpVtbl -> put_fullScreen(This,bFullScreen)

#define IWMPPlayer2_get_enableContextMenu(This,pbEnableContextMenu)	\
    (This)->lpVtbl -> get_enableContextMenu(This,pbEnableContextMenu)

#define IWMPPlayer2_put_enableContextMenu(This,bEnableContextMenu)	\
    (This)->lpVtbl -> put_enableContextMenu(This,bEnableContextMenu)

#define IWMPPlayer2_put_uiMode(This,bstrMode)	\
    (This)->lpVtbl -> put_uiMode(This,bstrMode)

#define IWMPPlayer2_get_uiMode(This,pbstrMode)	\
    (This)->lpVtbl -> get_uiMode(This,pbstrMode)

#define IWMPPlayer2_get_stretchToFit(This,pbEnabled)	\
    (This)->lpVtbl -> get_stretchToFit(This,pbEnabled)

#define IWMPPlayer2_put_stretchToFit(This,bEnabled)	\
    (This)->lpVtbl -> put_stretchToFit(This,bEnabled)

#define IWMPPlayer2_get_windowlessVideo(This,pbEnabled)	\
    (This)->lpVtbl -> get_windowlessVideo(This,pbEnabled)

#define IWMPPlayer2_put_windowlessVideo(This,bEnabled)	\
    (This)->lpVtbl -> put_windowlessVideo(This,bEnabled)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE IWMPPlayer2_get_enabled_Proxy( 
    IWMPPlayer2 * This,
    /* [retval][out] */ VARIANT_BOOL *pbEnabled);


void __RPC_STUB IWMPPlayer2_get_enabled_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propput][id] */ HRESULT STDMETHODCALLTYPE IWMPPlayer2_put_enabled_Proxy( 
    IWMPPlayer2 * This,
    /* [in] */ VARIANT_BOOL bEnabled);


void __RPC_STUB IWMPPlayer2_put_enabled_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE IWMPPlayer2_get_fullScreen_Proxy( 
    IWMPPlayer2 * This,
    /* [retval][out] */ VARIANT_BOOL *pbFullScreen);


void __RPC_STUB IWMPPlayer2_get_fullScreen_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propput][id] */ HRESULT STDMETHODCALLTYPE IWMPPlayer2_put_fullScreen_Proxy( 
    IWMPPlayer2 * This,
    VARIANT_BOOL bFullScreen);


void __RPC_STUB IWMPPlayer2_put_fullScreen_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE IWMPPlayer2_get_enableContextMenu_Proxy( 
    IWMPPlayer2 * This,
    /* [retval][out] */ VARIANT_BOOL *pbEnableContextMenu);


void __RPC_STUB IWMPPlayer2_get_enableContextMenu_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propput][id] */ HRESULT STDMETHODCALLTYPE IWMPPlayer2_put_enableContextMenu_Proxy( 
    IWMPPlayer2 * This,
    VARIANT_BOOL bEnableContextMenu);


void __RPC_STUB IWMPPlayer2_put_enableContextMenu_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propput][id] */ HRESULT STDMETHODCALLTYPE IWMPPlayer2_put_uiMode_Proxy( 
    IWMPPlayer2 * This,
    /* [in] */ BSTR bstrMode);


void __RPC_STUB IWMPPlayer2_put_uiMode_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE IWMPPlayer2_get_uiMode_Proxy( 
    IWMPPlayer2 * This,
    /* [retval][out] */ BSTR *pbstrMode);


void __RPC_STUB IWMPPlayer2_get_uiMode_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE IWMPPlayer2_get_stretchToFit_Proxy( 
    IWMPPlayer2 * This,
    /* [retval][out] */ VARIANT_BOOL *pbEnabled);


void __RPC_STUB IWMPPlayer2_get_stretchToFit_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propput][id] */ HRESULT STDMETHODCALLTYPE IWMPPlayer2_put_stretchToFit_Proxy( 
    IWMPPlayer2 * This,
    /* [in] */ VARIANT_BOOL bEnabled);


void __RPC_STUB IWMPPlayer2_put_stretchToFit_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE IWMPPlayer2_get_windowlessVideo_Proxy( 
    IWMPPlayer2 * This,
    /* [retval][out] */ VARIANT_BOOL *pbEnabled);


void __RPC_STUB IWMPPlayer2_get_windowlessVideo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propput][id] */ HRESULT STDMETHODCALLTYPE IWMPPlayer2_put_windowlessVideo_Proxy( 
    IWMPPlayer2 * This,
    /* [in] */ VARIANT_BOOL bEnabled);


void __RPC_STUB IWMPPlayer2_put_windowlessVideo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IWMPPlayer2_INTERFACE_DEFINED__ */


#ifndef __IWMPMedia2_INTERFACE_DEFINED__
#define __IWMPMedia2_INTERFACE_DEFINED__

/* interface IWMPMedia2 */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_IWMPMedia2;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("AB7C88BB-143E-4ea4-ACC3-E4350B2106C3")
    IWMPMedia2 : public IWMPMedia
    {
    public:
        virtual /* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE get_error( 
            /* [retval][out] */ IWMPErrorItem **ppIWMPErrorItem) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IWMPMedia2Vtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IWMPMedia2 * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IWMPMedia2 * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IWMPMedia2 * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IWMPMedia2 * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IWMPMedia2 * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IWMPMedia2 * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IWMPMedia2 * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_isIdentical )( 
            IWMPMedia2 * This,
            /* [in] */ IWMPMedia *pIWMPMedia,
            /* [retval][out] */ VARIANT_BOOL *pvbool);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_sourceURL )( 
            IWMPMedia2 * This,
            /* [retval][out] */ BSTR *pbstrSourceURL);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_name )( 
            IWMPMedia2 * This,
            /* [retval][out] */ BSTR *pbstrName);
        
        /* [helpstring][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_name )( 
            IWMPMedia2 * This,
            /* [in] */ BSTR bstrName);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_imageSourceWidth )( 
            IWMPMedia2 * This,
            /* [retval][out] */ long *pWidth);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_imageSourceHeight )( 
            IWMPMedia2 * This,
            /* [retval][out] */ long *pHeight);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_markerCount )( 
            IWMPMedia2 * This,
            /* [retval][out] */ long *pMarkerCount);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *getMarkerTime )( 
            IWMPMedia2 * This,
            /* [in] */ long MarkerNum,
            /* [retval][out] */ double *pMarkerTime);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *getMarkerName )( 
            IWMPMedia2 * This,
            /* [in] */ long MarkerNum,
            /* [retval][out] */ BSTR *pbstrMarkerName);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_duration )( 
            IWMPMedia2 * This,
            /* [retval][out] */ double *pDuration);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_durationString )( 
            IWMPMedia2 * This,
            /* [retval][out] */ BSTR *pbstrDuration);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_attributeCount )( 
            IWMPMedia2 * This,
            /* [retval][out] */ long *plCount);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *getAttributeName )( 
            IWMPMedia2 * This,
            /* [in] */ long lIndex,
            /* [retval][out] */ BSTR *pbstrItemName);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *getItemInfo )( 
            IWMPMedia2 * This,
            /* [in] */ BSTR bstrItemName,
            /* [retval][out] */ BSTR *pbstrVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *setItemInfo )( 
            IWMPMedia2 * This,
            /* [in] */ BSTR bstrItemName,
            /* [in] */ BSTR bstrVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *getItemInfoByAtom )( 
            IWMPMedia2 * This,
            /* [in] */ long lAtom,
            /* [retval][out] */ BSTR *pbstrVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *isMemberOf )( 
            IWMPMedia2 * This,
            /* [in] */ IWMPPlaylist *pPlaylist,
            /* [retval][out] */ VARIANT_BOOL *pvarfIsMemberOf);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *isReadOnlyItem )( 
            IWMPMedia2 * This,
            /* [in] */ BSTR bstrItemName,
            /* [retval][out] */ VARIANT_BOOL *pvarfIsReadOnly);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_error )( 
            IWMPMedia2 * This,
            /* [retval][out] */ IWMPErrorItem **ppIWMPErrorItem);
        
        END_INTERFACE
    } IWMPMedia2Vtbl;

    interface IWMPMedia2
    {
        CONST_VTBL struct IWMPMedia2Vtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IWMPMedia2_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IWMPMedia2_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IWMPMedia2_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IWMPMedia2_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IWMPMedia2_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IWMPMedia2_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IWMPMedia2_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IWMPMedia2_get_isIdentical(This,pIWMPMedia,pvbool)	\
    (This)->lpVtbl -> get_isIdentical(This,pIWMPMedia,pvbool)

#define IWMPMedia2_get_sourceURL(This,pbstrSourceURL)	\
    (This)->lpVtbl -> get_sourceURL(This,pbstrSourceURL)

#define IWMPMedia2_get_name(This,pbstrName)	\
    (This)->lpVtbl -> get_name(This,pbstrName)

#define IWMPMedia2_put_name(This,bstrName)	\
    (This)->lpVtbl -> put_name(This,bstrName)

#define IWMPMedia2_get_imageSourceWidth(This,pWidth)	\
    (This)->lpVtbl -> get_imageSourceWidth(This,pWidth)

#define IWMPMedia2_get_imageSourceHeight(This,pHeight)	\
    (This)->lpVtbl -> get_imageSourceHeight(This,pHeight)

#define IWMPMedia2_get_markerCount(This,pMarkerCount)	\
    (This)->lpVtbl -> get_markerCount(This,pMarkerCount)

#define IWMPMedia2_getMarkerTime(This,MarkerNum,pMarkerTime)	\
    (This)->lpVtbl -> getMarkerTime(This,MarkerNum,pMarkerTime)

#define IWMPMedia2_getMarkerName(This,MarkerNum,pbstrMarkerName)	\
    (This)->lpVtbl -> getMarkerName(This,MarkerNum,pbstrMarkerName)

#define IWMPMedia2_get_duration(This,pDuration)	\
    (This)->lpVtbl -> get_duration(This,pDuration)

#define IWMPMedia2_get_durationString(This,pbstrDuration)	\
    (This)->lpVtbl -> get_durationString(This,pbstrDuration)

#define IWMPMedia2_get_attributeCount(This,plCount)	\
    (This)->lpVtbl -> get_attributeCount(This,plCount)

#define IWMPMedia2_getAttributeName(This,lIndex,pbstrItemName)	\
    (This)->lpVtbl -> getAttributeName(This,lIndex,pbstrItemName)

#define IWMPMedia2_getItemInfo(This,bstrItemName,pbstrVal)	\
    (This)->lpVtbl -> getItemInfo(This,bstrItemName,pbstrVal)

#define IWMPMedia2_setItemInfo(This,bstrItemName,bstrVal)	\
    (This)->lpVtbl -> setItemInfo(This,bstrItemName,bstrVal)

#define IWMPMedia2_getItemInfoByAtom(This,lAtom,pbstrVal)	\
    (This)->lpVtbl -> getItemInfoByAtom(This,lAtom,pbstrVal)

#define IWMPMedia2_isMemberOf(This,pPlaylist,pvarfIsMemberOf)	\
    (This)->lpVtbl -> isMemberOf(This,pPlaylist,pvarfIsMemberOf)

#define IWMPMedia2_isReadOnlyItem(This,bstrItemName,pvarfIsReadOnly)	\
    (This)->lpVtbl -> isReadOnlyItem(This,bstrItemName,pvarfIsReadOnly)


#define IWMPMedia2_get_error(This,ppIWMPErrorItem)	\
    (This)->lpVtbl -> get_error(This,ppIWMPErrorItem)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE IWMPMedia2_get_error_Proxy( 
    IWMPMedia2 * This,
    /* [retval][out] */ IWMPErrorItem **ppIWMPErrorItem);


void __RPC_STUB IWMPMedia2_get_error_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IWMPMedia2_INTERFACE_DEFINED__ */


#ifndef __IWMPControls2_INTERFACE_DEFINED__
#define __IWMPControls2_INTERFACE_DEFINED__

/* interface IWMPControls2 */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_IWMPControls2;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("6F030D25-0890-480f-9775-1F7E40AB5B8E")
    IWMPControls2 : public IWMPControls
    {
    public:
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE step( 
            /* [in] */ long lStep) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IWMPControls2Vtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IWMPControls2 * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IWMPControls2 * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IWMPControls2 * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IWMPControls2 * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IWMPControls2 * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IWMPControls2 * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IWMPControls2 * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_isAvailable )( 
            IWMPControls2 * This,
            /* [in] */ BSTR bstrItem,
            /* [retval][out] */ VARIANT_BOOL *pIsAvailable);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *play )( 
            IWMPControls2 * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *stop )( 
            IWMPControls2 * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *pause )( 
            IWMPControls2 * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *fastForward )( 
            IWMPControls2 * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *fastReverse )( 
            IWMPControls2 * This);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_currentPosition )( 
            IWMPControls2 * This,
            /* [retval][out] */ double *pdCurrentPosition);
        
        /* [helpstring][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_currentPosition )( 
            IWMPControls2 * This,
            /* [in] */ double dCurrentPosition);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_currentPositionString )( 
            IWMPControls2 * This,
            /* [retval][out] */ BSTR *pbstrCurrentPosition);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *next )( 
            IWMPControls2 * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *previous )( 
            IWMPControls2 * This);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_currentItem )( 
            IWMPControls2 * This,
            /* [retval][out] */ IWMPMedia **ppIWMPMedia);
        
        /* [propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_currentItem )( 
            IWMPControls2 * This,
            /* [in] */ IWMPMedia *pIWMPMedia);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_currentMarker )( 
            IWMPControls2 * This,
            /* [retval][out] */ long *plMarker);
        
        /* [helpstring][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_currentMarker )( 
            IWMPControls2 * This,
            /* [in] */ long lMarker);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *playItem )( 
            IWMPControls2 * This,
            /* [in] */ IWMPMedia *pIWMPMedia);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *step )( 
            IWMPControls2 * This,
            /* [in] */ long lStep);
        
        END_INTERFACE
    } IWMPControls2Vtbl;

    interface IWMPControls2
    {
        CONST_VTBL struct IWMPControls2Vtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IWMPControls2_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IWMPControls2_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IWMPControls2_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IWMPControls2_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IWMPControls2_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IWMPControls2_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IWMPControls2_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IWMPControls2_get_isAvailable(This,bstrItem,pIsAvailable)	\
    (This)->lpVtbl -> get_isAvailable(This,bstrItem,pIsAvailable)

#define IWMPControls2_play(This)	\
    (This)->lpVtbl -> play(This)

#define IWMPControls2_stop(This)	\
    (This)->lpVtbl -> stop(This)

#define IWMPControls2_pause(This)	\
    (This)->lpVtbl -> pause(This)

#define IWMPControls2_fastForward(This)	\
    (This)->lpVtbl -> fastForward(This)

#define IWMPControls2_fastReverse(This)	\
    (This)->lpVtbl -> fastReverse(This)

#define IWMPControls2_get_currentPosition(This,pdCurrentPosition)	\
    (This)->lpVtbl -> get_currentPosition(This,pdCurrentPosition)

#define IWMPControls2_put_currentPosition(This,dCurrentPosition)	\
    (This)->lpVtbl -> put_currentPosition(This,dCurrentPosition)

#define IWMPControls2_get_currentPositionString(This,pbstrCurrentPosition)	\
    (This)->lpVtbl -> get_currentPositionString(This,pbstrCurrentPosition)

#define IWMPControls2_next(This)	\
    (This)->lpVtbl -> next(This)

#define IWMPControls2_previous(This)	\
    (This)->lpVtbl -> previous(This)

#define IWMPControls2_get_currentItem(This,ppIWMPMedia)	\
    (This)->lpVtbl -> get_currentItem(This,ppIWMPMedia)

#define IWMPControls2_put_currentItem(This,pIWMPMedia)	\
    (This)->lpVtbl -> put_currentItem(This,pIWMPMedia)

#define IWMPControls2_get_currentMarker(This,plMarker)	\
    (This)->lpVtbl -> get_currentMarker(This,plMarker)

#define IWMPControls2_put_currentMarker(This,lMarker)	\
    (This)->lpVtbl -> put_currentMarker(This,lMarker)

#define IWMPControls2_playItem(This,pIWMPMedia)	\
    (This)->lpVtbl -> playItem(This,pIWMPMedia)


#define IWMPControls2_step(This,lStep)	\
    (This)->lpVtbl -> step(This,lStep)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IWMPControls2_step_Proxy( 
    IWMPControls2 * This,
    /* [in] */ long lStep);


void __RPC_STUB IWMPControls2_step_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IWMPControls2_INTERFACE_DEFINED__ */


#ifndef __IWMPDVD_INTERFACE_DEFINED__
#define __IWMPDVD_INTERFACE_DEFINED__

/* interface IWMPDVD */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_IWMPDVD;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("8DA61686-4668-4a5c-AE5D-803193293DBE")
    IWMPDVD : public IDispatch
    {
    public:
        virtual /* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE get_isAvailable( 
            /* [in] */ BSTR bstrItem,
            /* [retval][out] */ VARIANT_BOOL *pIsAvailable) = 0;
        
        virtual /* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE get_domain( 
            /* [retval][out] */ BSTR *strDomain) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE topMenu( void) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE titleMenu( void) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE back( void) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE resume( void) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IWMPDVDVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IWMPDVD * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IWMPDVD * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IWMPDVD * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IWMPDVD * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IWMPDVD * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IWMPDVD * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IWMPDVD * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_isAvailable )( 
            IWMPDVD * This,
            /* [in] */ BSTR bstrItem,
            /* [retval][out] */ VARIANT_BOOL *pIsAvailable);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_domain )( 
            IWMPDVD * This,
            /* [retval][out] */ BSTR *strDomain);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *topMenu )( 
            IWMPDVD * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *titleMenu )( 
            IWMPDVD * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *back )( 
            IWMPDVD * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *resume )( 
            IWMPDVD * This);
        
        END_INTERFACE
    } IWMPDVDVtbl;

    interface IWMPDVD
    {
        CONST_VTBL struct IWMPDVDVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IWMPDVD_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IWMPDVD_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IWMPDVD_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IWMPDVD_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IWMPDVD_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IWMPDVD_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IWMPDVD_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IWMPDVD_get_isAvailable(This,bstrItem,pIsAvailable)	\
    (This)->lpVtbl -> get_isAvailable(This,bstrItem,pIsAvailable)

#define IWMPDVD_get_domain(This,strDomain)	\
    (This)->lpVtbl -> get_domain(This,strDomain)

#define IWMPDVD_topMenu(This)	\
    (This)->lpVtbl -> topMenu(This)

#define IWMPDVD_titleMenu(This)	\
    (This)->lpVtbl -> titleMenu(This)

#define IWMPDVD_back(This)	\
    (This)->lpVtbl -> back(This)

#define IWMPDVD_resume(This)	\
    (This)->lpVtbl -> resume(This)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE IWMPDVD_get_isAvailable_Proxy( 
    IWMPDVD * This,
    /* [in] */ BSTR bstrItem,
    /* [retval][out] */ VARIANT_BOOL *pIsAvailable);


void __RPC_STUB IWMPDVD_get_isAvailable_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE IWMPDVD_get_domain_Proxy( 
    IWMPDVD * This,
    /* [retval][out] */ BSTR *strDomain);


void __RPC_STUB IWMPDVD_get_domain_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IWMPDVD_topMenu_Proxy( 
    IWMPDVD * This);


void __RPC_STUB IWMPDVD_topMenu_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IWMPDVD_titleMenu_Proxy( 
    IWMPDVD * This);


void __RPC_STUB IWMPDVD_titleMenu_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IWMPDVD_back_Proxy( 
    IWMPDVD * This);


void __RPC_STUB IWMPDVD_back_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IWMPDVD_resume_Proxy( 
    IWMPDVD * This);


void __RPC_STUB IWMPDVD_resume_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IWMPDVD_INTERFACE_DEFINED__ */


#ifndef __IWMPCore2_INTERFACE_DEFINED__
#define __IWMPCore2_INTERFACE_DEFINED__

/* interface IWMPCore2 */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_IWMPCore2;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("BC17E5B7-7561-4c18-BB90-17D485775659")
    IWMPCore2 : public IWMPCore
    {
    public:
        virtual /* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE get_dvd( 
            /* [retval][out] */ IWMPDVD **ppDVD) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IWMPCore2Vtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IWMPCore2 * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IWMPCore2 * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IWMPCore2 * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IWMPCore2 * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IWMPCore2 * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IWMPCore2 * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IWMPCore2 * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *close )( 
            IWMPCore2 * This);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_URL )( 
            IWMPCore2 * This,
            /* [retval][out] */ BSTR *pbstrURL);
        
        /* [propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_URL )( 
            IWMPCore2 * This,
            /* [in] */ BSTR bstrURL);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_openState )( 
            IWMPCore2 * This,
            /* [retval][out] */ WMPOpenState *pwmpos);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_playState )( 
            IWMPCore2 * This,
            /* [retval][out] */ WMPPlayState *pwmpps);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_controls )( 
            IWMPCore2 * This,
            /* [retval][out] */ IWMPControls **ppControl);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_settings )( 
            IWMPCore2 * This,
            /* [retval][out] */ IWMPSettings **ppSettings);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_currentMedia )( 
            IWMPCore2 * This,
            /* [retval][out] */ IWMPMedia **ppMedia);
        
        /* [propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_currentMedia )( 
            IWMPCore2 * This,
            /* [in] */ IWMPMedia *pMedia);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_mediaCollection )( 
            IWMPCore2 * This,
            /* [retval][out] */ IWMPMediaCollection **ppMediaCollection);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_playlistCollection )( 
            IWMPCore2 * This,
            /* [retval][out] */ IWMPPlaylistCollection **ppPlaylistCollection);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_versionInfo )( 
            IWMPCore2 * This,
            /* [retval][out] */ BSTR *pbstrVersionInfo);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *launchURL )( 
            IWMPCore2 * This,
            /* [in] */ BSTR bstrURL);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_network )( 
            IWMPCore2 * This,
            /* [retval][out] */ IWMPNetwork **ppQNI);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_currentPlaylist )( 
            IWMPCore2 * This,
            /* [retval][out] */ IWMPPlaylist **ppPL);
        
        /* [helpstring][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_currentPlaylist )( 
            IWMPCore2 * This,
            /* [in] */ IWMPPlaylist *pPL);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_cdromCollection )( 
            IWMPCore2 * This,
            /* [retval][out] */ IWMPCdromCollection **ppCdromCollection);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_closedCaption )( 
            IWMPCore2 * This,
            /* [retval][out] */ IWMPClosedCaption **ppClosedCaption);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_isOnline )( 
            IWMPCore2 * This,
            /* [retval][out] */ VARIANT_BOOL *pfOnline);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_error )( 
            IWMPCore2 * This,
            /* [retval][out] */ IWMPError **ppError);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_status )( 
            IWMPCore2 * This,
            /* [retval][out] */ BSTR *pbstrStatus);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_dvd )( 
            IWMPCore2 * This,
            /* [retval][out] */ IWMPDVD **ppDVD);
        
        END_INTERFACE
    } IWMPCore2Vtbl;

    interface IWMPCore2
    {
        CONST_VTBL struct IWMPCore2Vtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IWMPCore2_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IWMPCore2_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IWMPCore2_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IWMPCore2_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IWMPCore2_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IWMPCore2_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IWMPCore2_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IWMPCore2_close(This)	\
    (This)->lpVtbl -> close(This)

#define IWMPCore2_get_URL(This,pbstrURL)	\
    (This)->lpVtbl -> get_URL(This,pbstrURL)

#define IWMPCore2_put_URL(This,bstrURL)	\
    (This)->lpVtbl -> put_URL(This,bstrURL)

#define IWMPCore2_get_openState(This,pwmpos)	\
    (This)->lpVtbl -> get_openState(This,pwmpos)

#define IWMPCore2_get_playState(This,pwmpps)	\
    (This)->lpVtbl -> get_playState(This,pwmpps)

#define IWMPCore2_get_controls(This,ppControl)	\
    (This)->lpVtbl -> get_controls(This,ppControl)

#define IWMPCore2_get_settings(This,ppSettings)	\
    (This)->lpVtbl -> get_settings(This,ppSettings)

#define IWMPCore2_get_currentMedia(This,ppMedia)	\
    (This)->lpVtbl -> get_currentMedia(This,ppMedia)

#define IWMPCore2_put_currentMedia(This,pMedia)	\
    (This)->lpVtbl -> put_currentMedia(This,pMedia)

#define IWMPCore2_get_mediaCollection(This,ppMediaCollection)	\
    (This)->lpVtbl -> get_mediaCollection(This,ppMediaCollection)

#define IWMPCore2_get_playlistCollection(This,ppPlaylistCollection)	\
    (This)->lpVtbl -> get_playlistCollection(This,ppPlaylistCollection)

#define IWMPCore2_get_versionInfo(This,pbstrVersionInfo)	\
    (This)->lpVtbl -> get_versionInfo(This,pbstrVersionInfo)

#define IWMPCore2_launchURL(This,bstrURL)	\
    (This)->lpVtbl -> launchURL(This,bstrURL)

#define IWMPCore2_get_network(This,ppQNI)	\
    (This)->lpVtbl -> get_network(This,ppQNI)

#define IWMPCore2_get_currentPlaylist(This,ppPL)	\
    (This)->lpVtbl -> get_currentPlaylist(This,ppPL)

#define IWMPCore2_put_currentPlaylist(This,pPL)	\
    (This)->lpVtbl -> put_currentPlaylist(This,pPL)

#define IWMPCore2_get_cdromCollection(This,ppCdromCollection)	\
    (This)->lpVtbl -> get_cdromCollection(This,ppCdromCollection)

#define IWMPCore2_get_closedCaption(This,ppClosedCaption)	\
    (This)->lpVtbl -> get_closedCaption(This,ppClosedCaption)

#define IWMPCore2_get_isOnline(This,pfOnline)	\
    (This)->lpVtbl -> get_isOnline(This,pfOnline)

#define IWMPCore2_get_error(This,ppError)	\
    (This)->lpVtbl -> get_error(This,ppError)

#define IWMPCore2_get_status(This,pbstrStatus)	\
    (This)->lpVtbl -> get_status(This,pbstrStatus)


#define IWMPCore2_get_dvd(This,ppDVD)	\
    (This)->lpVtbl -> get_dvd(This,ppDVD)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE IWMPCore2_get_dvd_Proxy( 
    IWMPCore2 * This,
    /* [retval][out] */ IWMPDVD **ppDVD);


void __RPC_STUB IWMPCore2_get_dvd_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IWMPCore2_INTERFACE_DEFINED__ */


#ifndef __IWMPPlayer3_INTERFACE_DEFINED__
#define __IWMPPlayer3_INTERFACE_DEFINED__

/* interface IWMPPlayer3 */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_IWMPPlayer3;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("54062B68-052A-4c25-A39F-8B63346511D4")
    IWMPPlayer3 : public IWMPCore2
    {
    public:
        virtual /* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE get_enabled( 
            /* [retval][out] */ VARIANT_BOOL *pbEnabled) = 0;
        
        virtual /* [helpstring][propput][id] */ HRESULT STDMETHODCALLTYPE put_enabled( 
            /* [in] */ VARIANT_BOOL bEnabled) = 0;
        
        virtual /* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE get_fullScreen( 
            /* [retval][out] */ VARIANT_BOOL *pbFullScreen) = 0;
        
        virtual /* [helpstring][propput][id] */ HRESULT STDMETHODCALLTYPE put_fullScreen( 
            VARIANT_BOOL bFullScreen) = 0;
        
        virtual /* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE get_enableContextMenu( 
            /* [retval][out] */ VARIANT_BOOL *pbEnableContextMenu) = 0;
        
        virtual /* [helpstring][propput][id] */ HRESULT STDMETHODCALLTYPE put_enableContextMenu( 
            VARIANT_BOOL bEnableContextMenu) = 0;
        
        virtual /* [helpstring][propput][id] */ HRESULT STDMETHODCALLTYPE put_uiMode( 
            /* [in] */ BSTR bstrMode) = 0;
        
        virtual /* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE get_uiMode( 
            /* [retval][out] */ BSTR *pbstrMode) = 0;
        
        virtual /* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE get_stretchToFit( 
            /* [retval][out] */ VARIANT_BOOL *pbEnabled) = 0;
        
        virtual /* [helpstring][propput][id] */ HRESULT STDMETHODCALLTYPE put_stretchToFit( 
            /* [in] */ VARIANT_BOOL bEnabled) = 0;
        
        virtual /* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE get_windowlessVideo( 
            /* [retval][out] */ VARIANT_BOOL *pbEnabled) = 0;
        
        virtual /* [helpstring][propput][id] */ HRESULT STDMETHODCALLTYPE put_windowlessVideo( 
            /* [in] */ VARIANT_BOOL bEnabled) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IWMPPlayer3Vtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IWMPPlayer3 * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IWMPPlayer3 * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IWMPPlayer3 * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IWMPPlayer3 * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IWMPPlayer3 * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IWMPPlayer3 * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IWMPPlayer3 * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *close )( 
            IWMPPlayer3 * This);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_URL )( 
            IWMPPlayer3 * This,
            /* [retval][out] */ BSTR *pbstrURL);
        
        /* [propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_URL )( 
            IWMPPlayer3 * This,
            /* [in] */ BSTR bstrURL);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_openState )( 
            IWMPPlayer3 * This,
            /* [retval][out] */ WMPOpenState *pwmpos);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_playState )( 
            IWMPPlayer3 * This,
            /* [retval][out] */ WMPPlayState *pwmpps);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_controls )( 
            IWMPPlayer3 * This,
            /* [retval][out] */ IWMPControls **ppControl);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_settings )( 
            IWMPPlayer3 * This,
            /* [retval][out] */ IWMPSettings **ppSettings);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_currentMedia )( 
            IWMPPlayer3 * This,
            /* [retval][out] */ IWMPMedia **ppMedia);
        
        /* [propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_currentMedia )( 
            IWMPPlayer3 * This,
            /* [in] */ IWMPMedia *pMedia);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_mediaCollection )( 
            IWMPPlayer3 * This,
            /* [retval][out] */ IWMPMediaCollection **ppMediaCollection);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_playlistCollection )( 
            IWMPPlayer3 * This,
            /* [retval][out] */ IWMPPlaylistCollection **ppPlaylistCollection);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_versionInfo )( 
            IWMPPlayer3 * This,
            /* [retval][out] */ BSTR *pbstrVersionInfo);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *launchURL )( 
            IWMPPlayer3 * This,
            /* [in] */ BSTR bstrURL);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_network )( 
            IWMPPlayer3 * This,
            /* [retval][out] */ IWMPNetwork **ppQNI);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_currentPlaylist )( 
            IWMPPlayer3 * This,
            /* [retval][out] */ IWMPPlaylist **ppPL);
        
        /* [helpstring][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_currentPlaylist )( 
            IWMPPlayer3 * This,
            /* [in] */ IWMPPlaylist *pPL);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_cdromCollection )( 
            IWMPPlayer3 * This,
            /* [retval][out] */ IWMPCdromCollection **ppCdromCollection);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_closedCaption )( 
            IWMPPlayer3 * This,
            /* [retval][out] */ IWMPClosedCaption **ppClosedCaption);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_isOnline )( 
            IWMPPlayer3 * This,
            /* [retval][out] */ VARIANT_BOOL *pfOnline);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_error )( 
            IWMPPlayer3 * This,
            /* [retval][out] */ IWMPError **ppError);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_status )( 
            IWMPPlayer3 * This,
            /* [retval][out] */ BSTR *pbstrStatus);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_dvd )( 
            IWMPPlayer3 * This,
            /* [retval][out] */ IWMPDVD **ppDVD);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_enabled )( 
            IWMPPlayer3 * This,
            /* [retval][out] */ VARIANT_BOOL *pbEnabled);
        
        /* [helpstring][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_enabled )( 
            IWMPPlayer3 * This,
            /* [in] */ VARIANT_BOOL bEnabled);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_fullScreen )( 
            IWMPPlayer3 * This,
            /* [retval][out] */ VARIANT_BOOL *pbFullScreen);
        
        /* [helpstring][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_fullScreen )( 
            IWMPPlayer3 * This,
            VARIANT_BOOL bFullScreen);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_enableContextMenu )( 
            IWMPPlayer3 * This,
            /* [retval][out] */ VARIANT_BOOL *pbEnableContextMenu);
        
        /* [helpstring][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_enableContextMenu )( 
            IWMPPlayer3 * This,
            VARIANT_BOOL bEnableContextMenu);
        
        /* [helpstring][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_uiMode )( 
            IWMPPlayer3 * This,
            /* [in] */ BSTR bstrMode);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_uiMode )( 
            IWMPPlayer3 * This,
            /* [retval][out] */ BSTR *pbstrMode);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_stretchToFit )( 
            IWMPPlayer3 * This,
            /* [retval][out] */ VARIANT_BOOL *pbEnabled);
        
        /* [helpstring][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_stretchToFit )( 
            IWMPPlayer3 * This,
            /* [in] */ VARIANT_BOOL bEnabled);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_windowlessVideo )( 
            IWMPPlayer3 * This,
            /* [retval][out] */ VARIANT_BOOL *pbEnabled);
        
        /* [helpstring][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_windowlessVideo )( 
            IWMPPlayer3 * This,
            /* [in] */ VARIANT_BOOL bEnabled);
        
        END_INTERFACE
    } IWMPPlayer3Vtbl;

    interface IWMPPlayer3
    {
        CONST_VTBL struct IWMPPlayer3Vtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IWMPPlayer3_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IWMPPlayer3_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IWMPPlayer3_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IWMPPlayer3_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IWMPPlayer3_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IWMPPlayer3_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IWMPPlayer3_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IWMPPlayer3_close(This)	\
    (This)->lpVtbl -> close(This)

#define IWMPPlayer3_get_URL(This,pbstrURL)	\
    (This)->lpVtbl -> get_URL(This,pbstrURL)

#define IWMPPlayer3_put_URL(This,bstrURL)	\
    (This)->lpVtbl -> put_URL(This,bstrURL)

#define IWMPPlayer3_get_openState(This,pwmpos)	\
    (This)->lpVtbl -> get_openState(This,pwmpos)

#define IWMPPlayer3_get_playState(This,pwmpps)	\
    (This)->lpVtbl -> get_playState(This,pwmpps)

#define IWMPPlayer3_get_controls(This,ppControl)	\
    (This)->lpVtbl -> get_controls(This,ppControl)

#define IWMPPlayer3_get_settings(This,ppSettings)	\
    (This)->lpVtbl -> get_settings(This,ppSettings)

#define IWMPPlayer3_get_currentMedia(This,ppMedia)	\
    (This)->lpVtbl -> get_currentMedia(This,ppMedia)

#define IWMPPlayer3_put_currentMedia(This,pMedia)	\
    (This)->lpVtbl -> put_currentMedia(This,pMedia)

#define IWMPPlayer3_get_mediaCollection(This,ppMediaCollection)	\
    (This)->lpVtbl -> get_mediaCollection(This,ppMediaCollection)

#define IWMPPlayer3_get_playlistCollection(This,ppPlaylistCollection)	\
    (This)->lpVtbl -> get_playlistCollection(This,ppPlaylistCollection)

#define IWMPPlayer3_get_versionInfo(This,pbstrVersionInfo)	\
    (This)->lpVtbl -> get_versionInfo(This,pbstrVersionInfo)

#define IWMPPlayer3_launchURL(This,bstrURL)	\
    (This)->lpVtbl -> launchURL(This,bstrURL)

#define IWMPPlayer3_get_network(This,ppQNI)	\
    (This)->lpVtbl -> get_network(This,ppQNI)

#define IWMPPlayer3_get_currentPlaylist(This,ppPL)	\
    (This)->lpVtbl -> get_currentPlaylist(This,ppPL)

#define IWMPPlayer3_put_currentPlaylist(This,pPL)	\
    (This)->lpVtbl -> put_currentPlaylist(This,pPL)

#define IWMPPlayer3_get_cdromCollection(This,ppCdromCollection)	\
    (This)->lpVtbl -> get_cdromCollection(This,ppCdromCollection)

#define IWMPPlayer3_get_closedCaption(This,ppClosedCaption)	\
    (This)->lpVtbl -> get_closedCaption(This,ppClosedCaption)

#define IWMPPlayer3_get_isOnline(This,pfOnline)	\
    (This)->lpVtbl -> get_isOnline(This,pfOnline)

#define IWMPPlayer3_get_error(This,ppError)	\
    (This)->lpVtbl -> get_error(This,ppError)

#define IWMPPlayer3_get_status(This,pbstrStatus)	\
    (This)->lpVtbl -> get_status(This,pbstrStatus)


#define IWMPPlayer3_get_dvd(This,ppDVD)	\
    (This)->lpVtbl -> get_dvd(This,ppDVD)


#define IWMPPlayer3_get_enabled(This,pbEnabled)	\
    (This)->lpVtbl -> get_enabled(This,pbEnabled)

#define IWMPPlayer3_put_enabled(This,bEnabled)	\
    (This)->lpVtbl -> put_enabled(This,bEnabled)

#define IWMPPlayer3_get_fullScreen(This,pbFullScreen)	\
    (This)->lpVtbl -> get_fullScreen(This,pbFullScreen)

#define IWMPPlayer3_put_fullScreen(This,bFullScreen)	\
    (This)->lpVtbl -> put_fullScreen(This,bFullScreen)

#define IWMPPlayer3_get_enableContextMenu(This,pbEnableContextMenu)	\
    (This)->lpVtbl -> get_enableContextMenu(This,pbEnableContextMenu)

#define IWMPPlayer3_put_enableContextMenu(This,bEnableContextMenu)	\
    (This)->lpVtbl -> put_enableContextMenu(This,bEnableContextMenu)

#define IWMPPlayer3_put_uiMode(This,bstrMode)	\
    (This)->lpVtbl -> put_uiMode(This,bstrMode)

#define IWMPPlayer3_get_uiMode(This,pbstrMode)	\
    (This)->lpVtbl -> get_uiMode(This,pbstrMode)

#define IWMPPlayer3_get_stretchToFit(This,pbEnabled)	\
    (This)->lpVtbl -> get_stretchToFit(This,pbEnabled)

#define IWMPPlayer3_put_stretchToFit(This,bEnabled)	\
    (This)->lpVtbl -> put_stretchToFit(This,bEnabled)

#define IWMPPlayer3_get_windowlessVideo(This,pbEnabled)	\
    (This)->lpVtbl -> get_windowlessVideo(This,pbEnabled)

#define IWMPPlayer3_put_windowlessVideo(This,bEnabled)	\
    (This)->lpVtbl -> put_windowlessVideo(This,bEnabled)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE IWMPPlayer3_get_enabled_Proxy( 
    IWMPPlayer3 * This,
    /* [retval][out] */ VARIANT_BOOL *pbEnabled);


void __RPC_STUB IWMPPlayer3_get_enabled_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propput][id] */ HRESULT STDMETHODCALLTYPE IWMPPlayer3_put_enabled_Proxy( 
    IWMPPlayer3 * This,
    /* [in] */ VARIANT_BOOL bEnabled);


void __RPC_STUB IWMPPlayer3_put_enabled_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE IWMPPlayer3_get_fullScreen_Proxy( 
    IWMPPlayer3 * This,
    /* [retval][out] */ VARIANT_BOOL *pbFullScreen);


void __RPC_STUB IWMPPlayer3_get_fullScreen_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propput][id] */ HRESULT STDMETHODCALLTYPE IWMPPlayer3_put_fullScreen_Proxy( 
    IWMPPlayer3 * This,
    VARIANT_BOOL bFullScreen);


void __RPC_STUB IWMPPlayer3_put_fullScreen_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE IWMPPlayer3_get_enableContextMenu_Proxy( 
    IWMPPlayer3 * This,
    /* [retval][out] */ VARIANT_BOOL *pbEnableContextMenu);


void __RPC_STUB IWMPPlayer3_get_enableContextMenu_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propput][id] */ HRESULT STDMETHODCALLTYPE IWMPPlayer3_put_enableContextMenu_Proxy( 
    IWMPPlayer3 * This,
    VARIANT_BOOL bEnableContextMenu);


void __RPC_STUB IWMPPlayer3_put_enableContextMenu_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propput][id] */ HRESULT STDMETHODCALLTYPE IWMPPlayer3_put_uiMode_Proxy( 
    IWMPPlayer3 * This,
    /* [in] */ BSTR bstrMode);


void __RPC_STUB IWMPPlayer3_put_uiMode_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE IWMPPlayer3_get_uiMode_Proxy( 
    IWMPPlayer3 * This,
    /* [retval][out] */ BSTR *pbstrMode);


void __RPC_STUB IWMPPlayer3_get_uiMode_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE IWMPPlayer3_get_stretchToFit_Proxy( 
    IWMPPlayer3 * This,
    /* [retval][out] */ VARIANT_BOOL *pbEnabled);


void __RPC_STUB IWMPPlayer3_get_stretchToFit_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propput][id] */ HRESULT STDMETHODCALLTYPE IWMPPlayer3_put_stretchToFit_Proxy( 
    IWMPPlayer3 * This,
    /* [in] */ VARIANT_BOOL bEnabled);


void __RPC_STUB IWMPPlayer3_put_stretchToFit_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE IWMPPlayer3_get_windowlessVideo_Proxy( 
    IWMPPlayer3 * This,
    /* [retval][out] */ VARIANT_BOOL *pbEnabled);


void __RPC_STUB IWMPPlayer3_get_windowlessVideo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propput][id] */ HRESULT STDMETHODCALLTYPE IWMPPlayer3_put_windowlessVideo_Proxy( 
    IWMPPlayer3 * This,
    /* [in] */ VARIANT_BOOL bEnabled);


void __RPC_STUB IWMPPlayer3_put_windowlessVideo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IWMPPlayer3_INTERFACE_DEFINED__ */


#ifndef __IWMPErrorItem2_INTERFACE_DEFINED__
#define __IWMPErrorItem2_INTERFACE_DEFINED__

/* interface IWMPErrorItem2 */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_IWMPErrorItem2;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("F75CCEC0-C67C-475c-931E-8719870BEE7D")
    IWMPErrorItem2 : public IWMPErrorItem
    {
    public:
        virtual /* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE get_condition( 
            /* [retval][out] */ long *plCondition) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IWMPErrorItem2Vtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IWMPErrorItem2 * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IWMPErrorItem2 * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IWMPErrorItem2 * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IWMPErrorItem2 * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IWMPErrorItem2 * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IWMPErrorItem2 * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IWMPErrorItem2 * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_errorCode )( 
            IWMPErrorItem2 * This,
            /* [retval][out] */ long *phr);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_errorDescription )( 
            IWMPErrorItem2 * This,
            /* [retval][out] */ BSTR *pbstrDescription);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_errorContext )( 
            IWMPErrorItem2 * This,
            /* [retval][out] */ VARIANT *pvarContext);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_remedy )( 
            IWMPErrorItem2 * This,
            /* [retval][out] */ long *plRemedy);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_customUrl )( 
            IWMPErrorItem2 * This,
            /* [retval][out] */ BSTR *pbstrCustomUrl);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_condition )( 
            IWMPErrorItem2 * This,
            /* [retval][out] */ long *plCondition);
        
        END_INTERFACE
    } IWMPErrorItem2Vtbl;

    interface IWMPErrorItem2
    {
        CONST_VTBL struct IWMPErrorItem2Vtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IWMPErrorItem2_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IWMPErrorItem2_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IWMPErrorItem2_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IWMPErrorItem2_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IWMPErrorItem2_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IWMPErrorItem2_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IWMPErrorItem2_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IWMPErrorItem2_get_errorCode(This,phr)	\
    (This)->lpVtbl -> get_errorCode(This,phr)

#define IWMPErrorItem2_get_errorDescription(This,pbstrDescription)	\
    (This)->lpVtbl -> get_errorDescription(This,pbstrDescription)

#define IWMPErrorItem2_get_errorContext(This,pvarContext)	\
    (This)->lpVtbl -> get_errorContext(This,pvarContext)

#define IWMPErrorItem2_get_remedy(This,plRemedy)	\
    (This)->lpVtbl -> get_remedy(This,plRemedy)

#define IWMPErrorItem2_get_customUrl(This,pbstrCustomUrl)	\
    (This)->lpVtbl -> get_customUrl(This,pbstrCustomUrl)


#define IWMPErrorItem2_get_condition(This,plCondition)	\
    (This)->lpVtbl -> get_condition(This,plCondition)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE IWMPErrorItem2_get_condition_Proxy( 
    IWMPErrorItem2 * This,
    /* [retval][out] */ long *plCondition);


void __RPC_STUB IWMPErrorItem2_get_condition_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IWMPErrorItem2_INTERFACE_DEFINED__ */


#ifndef __IWMPRemoteMediaServices_INTERFACE_DEFINED__
#define __IWMPRemoteMediaServices_INTERFACE_DEFINED__

/* interface IWMPRemoteMediaServices */
/* [unique][helpstring][uuid][object] */ 


EXTERN_C const IID IID_IWMPRemoteMediaServices;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("CBB92747-741F-44fe-AB5B-F1A48F3B2A59")
    IWMPRemoteMediaServices : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE GetServiceType( 
            /* [out] */ BSTR *pbstrType) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetApplicationName( 
            /* [out] */ BSTR *pbstrName) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetScriptableObject( 
            /* [out] */ BSTR *pbstrName,
            /* [out] */ IDispatch **ppDispatch) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetCustomUIMode( 
            /* [out] */ BSTR *pbstrFile) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IWMPRemoteMediaServicesVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IWMPRemoteMediaServices * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IWMPRemoteMediaServices * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IWMPRemoteMediaServices * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetServiceType )( 
            IWMPRemoteMediaServices * This,
            /* [out] */ BSTR *pbstrType);
        
        HRESULT ( STDMETHODCALLTYPE *GetApplicationName )( 
            IWMPRemoteMediaServices * This,
            /* [out] */ BSTR *pbstrName);
        
        HRESULT ( STDMETHODCALLTYPE *GetScriptableObject )( 
            IWMPRemoteMediaServices * This,
            /* [out] */ BSTR *pbstrName,
            /* [out] */ IDispatch **ppDispatch);
        
        HRESULT ( STDMETHODCALLTYPE *GetCustomUIMode )( 
            IWMPRemoteMediaServices * This,
            /* [out] */ BSTR *pbstrFile);
        
        END_INTERFACE
    } IWMPRemoteMediaServicesVtbl;

    interface IWMPRemoteMediaServices
    {
        CONST_VTBL struct IWMPRemoteMediaServicesVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IWMPRemoteMediaServices_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IWMPRemoteMediaServices_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IWMPRemoteMediaServices_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IWMPRemoteMediaServices_GetServiceType(This,pbstrType)	\
    (This)->lpVtbl -> GetServiceType(This,pbstrType)

#define IWMPRemoteMediaServices_GetApplicationName(This,pbstrName)	\
    (This)->lpVtbl -> GetApplicationName(This,pbstrName)

#define IWMPRemoteMediaServices_GetScriptableObject(This,pbstrName,ppDispatch)	\
    (This)->lpVtbl -> GetScriptableObject(This,pbstrName,ppDispatch)

#define IWMPRemoteMediaServices_GetCustomUIMode(This,pbstrFile)	\
    (This)->lpVtbl -> GetCustomUIMode(This,pbstrFile)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE IWMPRemoteMediaServices_GetServiceType_Proxy( 
    IWMPRemoteMediaServices * This,
    /* [out] */ BSTR *pbstrType);


void __RPC_STUB IWMPRemoteMediaServices_GetServiceType_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IWMPRemoteMediaServices_GetApplicationName_Proxy( 
    IWMPRemoteMediaServices * This,
    /* [out] */ BSTR *pbstrName);


void __RPC_STUB IWMPRemoteMediaServices_GetApplicationName_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IWMPRemoteMediaServices_GetScriptableObject_Proxy( 
    IWMPRemoteMediaServices * This,
    /* [out] */ BSTR *pbstrName,
    /* [out] */ IDispatch **ppDispatch);


void __RPC_STUB IWMPRemoteMediaServices_GetScriptableObject_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IWMPRemoteMediaServices_GetCustomUIMode_Proxy( 
    IWMPRemoteMediaServices * This,
    /* [out] */ BSTR *pbstrFile);


void __RPC_STUB IWMPRemoteMediaServices_GetCustomUIMode_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IWMPRemoteMediaServices_INTERFACE_DEFINED__ */


/* interface __MIDL_itf_wmp_0285 */
/* [local] */ 

EXTERN_GUID( CLSID_WMPSkinManager, 0xB2A7FD52,0x301F,0x4348, 0xB9, 0x3A, 0x63, 0x8C, 0x6D, 0xE4, 0x92, 0x29 );


extern RPC_IF_HANDLE __MIDL_itf_wmp_0285_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_wmp_0285_v0_0_s_ifspec;

#ifndef __IWMPSkinManager_INTERFACE_DEFINED__
#define __IWMPSkinManager_INTERFACE_DEFINED__

/* interface IWMPSkinManager */
/* [unique][helpstring][uuid][object] */ 


EXTERN_C const IID IID_IWMPSkinManager;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("076F2FA6-ED30-448B-8CC5-3F3EF3529C7A")
    IWMPSkinManager : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE SetVisualStyle( 
            /* [in] */ BSTR bstrPath) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IWMPSkinManagerVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IWMPSkinManager * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IWMPSkinManager * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IWMPSkinManager * This);
        
        HRESULT ( STDMETHODCALLTYPE *SetVisualStyle )( 
            IWMPSkinManager * This,
            /* [in] */ BSTR bstrPath);
        
        END_INTERFACE
    } IWMPSkinManagerVtbl;

    interface IWMPSkinManager
    {
        CONST_VTBL struct IWMPSkinManagerVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IWMPSkinManager_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IWMPSkinManager_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IWMPSkinManager_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IWMPSkinManager_SetVisualStyle(This,bstrPath)	\
    (This)->lpVtbl -> SetVisualStyle(This,bstrPath)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE IWMPSkinManager_SetVisualStyle_Proxy( 
    IWMPSkinManager * This,
    /* [in] */ BSTR bstrPath);


void __RPC_STUB IWMPSkinManager_SetVisualStyle_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IWMPSkinManager_INTERFACE_DEFINED__ */


#ifndef __IWMPMetadataPicture_INTERFACE_DEFINED__
#define __IWMPMetadataPicture_INTERFACE_DEFINED__

/* interface IWMPMetadataPicture */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_IWMPMetadataPicture;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("5C29BBE0-F87D-4c45-AA28-A70F0230FFA9")
    IWMPMetadataPicture : public IDispatch
    {
    public:
        virtual /* [propget][id] */ HRESULT STDMETHODCALLTYPE get_mimeType( 
            /* [retval][out] */ BSTR *pbstrMimeType) = 0;
        
        virtual /* [propget][id] */ HRESULT STDMETHODCALLTYPE get_pictureType( 
            /* [retval][out] */ BSTR *pbstrPictureType) = 0;
        
        virtual /* [propget][id] */ HRESULT STDMETHODCALLTYPE get_description( 
            /* [retval][out] */ BSTR *pbstrDescription) = 0;
        
        virtual /* [propget][id] */ HRESULT STDMETHODCALLTYPE get_URL( 
            /* [retval][out] */ BSTR *pbstrURL) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IWMPMetadataPictureVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IWMPMetadataPicture * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IWMPMetadataPicture * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IWMPMetadataPicture * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IWMPMetadataPicture * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IWMPMetadataPicture * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IWMPMetadataPicture * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IWMPMetadataPicture * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_mimeType )( 
            IWMPMetadataPicture * This,
            /* [retval][out] */ BSTR *pbstrMimeType);
        
        /* [propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_pictureType )( 
            IWMPMetadataPicture * This,
            /* [retval][out] */ BSTR *pbstrPictureType);
        
        /* [propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_description )( 
            IWMPMetadataPicture * This,
            /* [retval][out] */ BSTR *pbstrDescription);
        
        /* [propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_URL )( 
            IWMPMetadataPicture * This,
            /* [retval][out] */ BSTR *pbstrURL);
        
        END_INTERFACE
    } IWMPMetadataPictureVtbl;

    interface IWMPMetadataPicture
    {
        CONST_VTBL struct IWMPMetadataPictureVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IWMPMetadataPicture_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IWMPMetadataPicture_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IWMPMetadataPicture_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IWMPMetadataPicture_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IWMPMetadataPicture_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IWMPMetadataPicture_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IWMPMetadataPicture_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IWMPMetadataPicture_get_mimeType(This,pbstrMimeType)	\
    (This)->lpVtbl -> get_mimeType(This,pbstrMimeType)

#define IWMPMetadataPicture_get_pictureType(This,pbstrPictureType)	\
    (This)->lpVtbl -> get_pictureType(This,pbstrPictureType)

#define IWMPMetadataPicture_get_description(This,pbstrDescription)	\
    (This)->lpVtbl -> get_description(This,pbstrDescription)

#define IWMPMetadataPicture_get_URL(This,pbstrURL)	\
    (This)->lpVtbl -> get_URL(This,pbstrURL)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [propget][id] */ HRESULT STDMETHODCALLTYPE IWMPMetadataPicture_get_mimeType_Proxy( 
    IWMPMetadataPicture * This,
    /* [retval][out] */ BSTR *pbstrMimeType);


void __RPC_STUB IWMPMetadataPicture_get_mimeType_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [propget][id] */ HRESULT STDMETHODCALLTYPE IWMPMetadataPicture_get_pictureType_Proxy( 
    IWMPMetadataPicture * This,
    /* [retval][out] */ BSTR *pbstrPictureType);


void __RPC_STUB IWMPMetadataPicture_get_pictureType_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [propget][id] */ HRESULT STDMETHODCALLTYPE IWMPMetadataPicture_get_description_Proxy( 
    IWMPMetadataPicture * This,
    /* [retval][out] */ BSTR *pbstrDescription);


void __RPC_STUB IWMPMetadataPicture_get_description_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [propget][id] */ HRESULT STDMETHODCALLTYPE IWMPMetadataPicture_get_URL_Proxy( 
    IWMPMetadataPicture * This,
    /* [retval][out] */ BSTR *pbstrURL);


void __RPC_STUB IWMPMetadataPicture_get_URL_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IWMPMetadataPicture_INTERFACE_DEFINED__ */


#ifndef __IWMPMetadataText_INTERFACE_DEFINED__
#define __IWMPMetadataText_INTERFACE_DEFINED__

/* interface IWMPMetadataText */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_IWMPMetadataText;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("769A72DB-13D2-45e2-9C48-53CA9D5B7450")
    IWMPMetadataText : public IDispatch
    {
    public:
        virtual /* [propget][id] */ HRESULT STDMETHODCALLTYPE get_description( 
            /* [retval][out] */ BSTR *pbstrDescription) = 0;
        
        virtual /* [propget][id] */ HRESULT STDMETHODCALLTYPE get_text( 
            /* [retval][out] */ BSTR *pbstrText) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IWMPMetadataTextVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IWMPMetadataText * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IWMPMetadataText * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IWMPMetadataText * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IWMPMetadataText * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IWMPMetadataText * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IWMPMetadataText * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IWMPMetadataText * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_description )( 
            IWMPMetadataText * This,
            /* [retval][out] */ BSTR *pbstrDescription);
        
        /* [propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_text )( 
            IWMPMetadataText * This,
            /* [retval][out] */ BSTR *pbstrText);
        
        END_INTERFACE
    } IWMPMetadataTextVtbl;

    interface IWMPMetadataText
    {
        CONST_VTBL struct IWMPMetadataTextVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IWMPMetadataText_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IWMPMetadataText_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IWMPMetadataText_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IWMPMetadataText_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IWMPMetadataText_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IWMPMetadataText_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IWMPMetadataText_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IWMPMetadataText_get_description(This,pbstrDescription)	\
    (This)->lpVtbl -> get_description(This,pbstrDescription)

#define IWMPMetadataText_get_text(This,pbstrText)	\
    (This)->lpVtbl -> get_text(This,pbstrText)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [propget][id] */ HRESULT STDMETHODCALLTYPE IWMPMetadataText_get_description_Proxy( 
    IWMPMetadataText * This,
    /* [retval][out] */ BSTR *pbstrDescription);


void __RPC_STUB IWMPMetadataText_get_description_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [propget][id] */ HRESULT STDMETHODCALLTYPE IWMPMetadataText_get_text_Proxy( 
    IWMPMetadataText * This,
    /* [retval][out] */ BSTR *pbstrText);


void __RPC_STUB IWMPMetadataText_get_text_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IWMPMetadataText_INTERFACE_DEFINED__ */


#ifndef __IWMPMedia3_INTERFACE_DEFINED__
#define __IWMPMedia3_INTERFACE_DEFINED__

/* interface IWMPMedia3 */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_IWMPMedia3;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("F118EFC7-F03A-4fb4-99C9-1C02A5C1065B")
    IWMPMedia3 : public IWMPMedia2
    {
    public:
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE getAttributeCountByType( 
            /* [in] */ BSTR bstrType,
            /* [in] */ BSTR bstrLanguage,
            /* [retval][out] */ long *plCount) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE getItemInfoByType( 
            /* [in] */ BSTR bstrType,
            /* [in] */ BSTR bstrLanguage,
            /* [in] */ long lIndex,
            /* [retval][out] */ VARIANT *pvarValue) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IWMPMedia3Vtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IWMPMedia3 * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IWMPMedia3 * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IWMPMedia3 * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IWMPMedia3 * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IWMPMedia3 * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IWMPMedia3 * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IWMPMedia3 * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_isIdentical )( 
            IWMPMedia3 * This,
            /* [in] */ IWMPMedia *pIWMPMedia,
            /* [retval][out] */ VARIANT_BOOL *pvbool);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_sourceURL )( 
            IWMPMedia3 * This,
            /* [retval][out] */ BSTR *pbstrSourceURL);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_name )( 
            IWMPMedia3 * This,
            /* [retval][out] */ BSTR *pbstrName);
        
        /* [helpstring][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_name )( 
            IWMPMedia3 * This,
            /* [in] */ BSTR bstrName);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_imageSourceWidth )( 
            IWMPMedia3 * This,
            /* [retval][out] */ long *pWidth);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_imageSourceHeight )( 
            IWMPMedia3 * This,
            /* [retval][out] */ long *pHeight);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_markerCount )( 
            IWMPMedia3 * This,
            /* [retval][out] */ long *pMarkerCount);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *getMarkerTime )( 
            IWMPMedia3 * This,
            /* [in] */ long MarkerNum,
            /* [retval][out] */ double *pMarkerTime);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *getMarkerName )( 
            IWMPMedia3 * This,
            /* [in] */ long MarkerNum,
            /* [retval][out] */ BSTR *pbstrMarkerName);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_duration )( 
            IWMPMedia3 * This,
            /* [retval][out] */ double *pDuration);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_durationString )( 
            IWMPMedia3 * This,
            /* [retval][out] */ BSTR *pbstrDuration);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_attributeCount )( 
            IWMPMedia3 * This,
            /* [retval][out] */ long *plCount);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *getAttributeName )( 
            IWMPMedia3 * This,
            /* [in] */ long lIndex,
            /* [retval][out] */ BSTR *pbstrItemName);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *getItemInfo )( 
            IWMPMedia3 * This,
            /* [in] */ BSTR bstrItemName,
            /* [retval][out] */ BSTR *pbstrVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *setItemInfo )( 
            IWMPMedia3 * This,
            /* [in] */ BSTR bstrItemName,
            /* [in] */ BSTR bstrVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *getItemInfoByAtom )( 
            IWMPMedia3 * This,
            /* [in] */ long lAtom,
            /* [retval][out] */ BSTR *pbstrVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *isMemberOf )( 
            IWMPMedia3 * This,
            /* [in] */ IWMPPlaylist *pPlaylist,
            /* [retval][out] */ VARIANT_BOOL *pvarfIsMemberOf);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *isReadOnlyItem )( 
            IWMPMedia3 * This,
            /* [in] */ BSTR bstrItemName,
            /* [retval][out] */ VARIANT_BOOL *pvarfIsReadOnly);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_error )( 
            IWMPMedia3 * This,
            /* [retval][out] */ IWMPErrorItem **ppIWMPErrorItem);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *getAttributeCountByType )( 
            IWMPMedia3 * This,
            /* [in] */ BSTR bstrType,
            /* [in] */ BSTR bstrLanguage,
            /* [retval][out] */ long *plCount);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *getItemInfoByType )( 
            IWMPMedia3 * This,
            /* [in] */ BSTR bstrType,
            /* [in] */ BSTR bstrLanguage,
            /* [in] */ long lIndex,
            /* [retval][out] */ VARIANT *pvarValue);
        
        END_INTERFACE
    } IWMPMedia3Vtbl;

    interface IWMPMedia3
    {
        CONST_VTBL struct IWMPMedia3Vtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IWMPMedia3_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IWMPMedia3_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IWMPMedia3_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IWMPMedia3_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IWMPMedia3_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IWMPMedia3_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IWMPMedia3_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IWMPMedia3_get_isIdentical(This,pIWMPMedia,pvbool)	\
    (This)->lpVtbl -> get_isIdentical(This,pIWMPMedia,pvbool)

#define IWMPMedia3_get_sourceURL(This,pbstrSourceURL)	\
    (This)->lpVtbl -> get_sourceURL(This,pbstrSourceURL)

#define IWMPMedia3_get_name(This,pbstrName)	\
    (This)->lpVtbl -> get_name(This,pbstrName)

#define IWMPMedia3_put_name(This,bstrName)	\
    (This)->lpVtbl -> put_name(This,bstrName)

#define IWMPMedia3_get_imageSourceWidth(This,pWidth)	\
    (This)->lpVtbl -> get_imageSourceWidth(This,pWidth)

#define IWMPMedia3_get_imageSourceHeight(This,pHeight)	\
    (This)->lpVtbl -> get_imageSourceHeight(This,pHeight)

#define IWMPMedia3_get_markerCount(This,pMarkerCount)	\
    (This)->lpVtbl -> get_markerCount(This,pMarkerCount)

#define IWMPMedia3_getMarkerTime(This,MarkerNum,pMarkerTime)	\
    (This)->lpVtbl -> getMarkerTime(This,MarkerNum,pMarkerTime)

#define IWMPMedia3_getMarkerName(This,MarkerNum,pbstrMarkerName)	\
    (This)->lpVtbl -> getMarkerName(This,MarkerNum,pbstrMarkerName)

#define IWMPMedia3_get_duration(This,pDuration)	\
    (This)->lpVtbl -> get_duration(This,pDuration)

#define IWMPMedia3_get_durationString(This,pbstrDuration)	\
    (This)->lpVtbl -> get_durationString(This,pbstrDuration)

#define IWMPMedia3_get_attributeCount(This,plCount)	\
    (This)->lpVtbl -> get_attributeCount(This,plCount)

#define IWMPMedia3_getAttributeName(This,lIndex,pbstrItemName)	\
    (This)->lpVtbl -> getAttributeName(This,lIndex,pbstrItemName)

#define IWMPMedia3_getItemInfo(This,bstrItemName,pbstrVal)	\
    (This)->lpVtbl -> getItemInfo(This,bstrItemName,pbstrVal)

#define IWMPMedia3_setItemInfo(This,bstrItemName,bstrVal)	\
    (This)->lpVtbl -> setItemInfo(This,bstrItemName,bstrVal)

#define IWMPMedia3_getItemInfoByAtom(This,lAtom,pbstrVal)	\
    (This)->lpVtbl -> getItemInfoByAtom(This,lAtom,pbstrVal)

#define IWMPMedia3_isMemberOf(This,pPlaylist,pvarfIsMemberOf)	\
    (This)->lpVtbl -> isMemberOf(This,pPlaylist,pvarfIsMemberOf)

#define IWMPMedia3_isReadOnlyItem(This,bstrItemName,pvarfIsReadOnly)	\
    (This)->lpVtbl -> isReadOnlyItem(This,bstrItemName,pvarfIsReadOnly)


#define IWMPMedia3_get_error(This,ppIWMPErrorItem)	\
    (This)->lpVtbl -> get_error(This,ppIWMPErrorItem)


#define IWMPMedia3_getAttributeCountByType(This,bstrType,bstrLanguage,plCount)	\
    (This)->lpVtbl -> getAttributeCountByType(This,bstrType,bstrLanguage,plCount)

#define IWMPMedia3_getItemInfoByType(This,bstrType,bstrLanguage,lIndex,pvarValue)	\
    (This)->lpVtbl -> getItemInfoByType(This,bstrType,bstrLanguage,lIndex,pvarValue)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [id] */ HRESULT STDMETHODCALLTYPE IWMPMedia3_getAttributeCountByType_Proxy( 
    IWMPMedia3 * This,
    /* [in] */ BSTR bstrType,
    /* [in] */ BSTR bstrLanguage,
    /* [retval][out] */ long *plCount);


void __RPC_STUB IWMPMedia3_getAttributeCountByType_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id] */ HRESULT STDMETHODCALLTYPE IWMPMedia3_getItemInfoByType_Proxy( 
    IWMPMedia3 * This,
    /* [in] */ BSTR bstrType,
    /* [in] */ BSTR bstrLanguage,
    /* [in] */ long lIndex,
    /* [retval][out] */ VARIANT *pvarValue);


void __RPC_STUB IWMPMedia3_getItemInfoByType_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IWMPMedia3_INTERFACE_DEFINED__ */


#ifndef __IWMPSettings2_INTERFACE_DEFINED__
#define __IWMPSettings2_INTERFACE_DEFINED__

/* interface IWMPSettings2 */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_IWMPSettings2;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("FDA937A4-EECE-4da5-A0B6-39BF89ADE2C2")
    IWMPSettings2 : public IWMPSettings
    {
    public:
        virtual /* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE get_defaultAudioLanguage( 
            /* [retval][out] */ long *plLangID) = 0;
        
        virtual /* [propget][id] */ HRESULT STDMETHODCALLTYPE get_mediaAccessRights( 
            /* [retval][out] */ BSTR *pbstrRights) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE requestMediaAccessRights( 
            /* [in] */ BSTR bstrDesiredAccess,
            /* [retval][out] */ VARIANT_BOOL *pvbAccepted) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IWMPSettings2Vtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IWMPSettings2 * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IWMPSettings2 * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IWMPSettings2 * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IWMPSettings2 * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IWMPSettings2 * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IWMPSettings2 * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IWMPSettings2 * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_isAvailable )( 
            IWMPSettings2 * This,
            /* [in] */ BSTR bstrItem,
            /* [retval][out] */ VARIANT_BOOL *pIsAvailable);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_autoStart )( 
            IWMPSettings2 * This,
            /* [retval][out] */ VARIANT_BOOL *pfAutoStart);
        
        /* [helpstring][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_autoStart )( 
            IWMPSettings2 * This,
            /* [in] */ VARIANT_BOOL fAutoStart);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_baseURL )( 
            IWMPSettings2 * This,
            /* [retval][out] */ BSTR *pbstrBaseURL);
        
        /* [helpstring][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_baseURL )( 
            IWMPSettings2 * This,
            /* [in] */ BSTR bstrBaseURL);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_defaultFrame )( 
            IWMPSettings2 * This,
            /* [retval][out] */ BSTR *pbstrDefaultFrame);
        
        /* [helpstring][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_defaultFrame )( 
            IWMPSettings2 * This,
            /* [in] */ BSTR bstrDefaultFrame);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_invokeURLs )( 
            IWMPSettings2 * This,
            /* [retval][out] */ VARIANT_BOOL *pfInvokeURLs);
        
        /* [helpstring][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_invokeURLs )( 
            IWMPSettings2 * This,
            /* [in] */ VARIANT_BOOL fInvokeURLs);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_mute )( 
            IWMPSettings2 * This,
            /* [retval][out] */ VARIANT_BOOL *pfMute);
        
        /* [helpstring][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_mute )( 
            IWMPSettings2 * This,
            /* [in] */ VARIANT_BOOL fMute);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_playCount )( 
            IWMPSettings2 * This,
            /* [retval][out] */ long *plCount);
        
        /* [helpstring][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_playCount )( 
            IWMPSettings2 * This,
            /* [in] */ long lCount);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_rate )( 
            IWMPSettings2 * This,
            /* [retval][out] */ double *pdRate);
        
        /* [helpstring][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_rate )( 
            IWMPSettings2 * This,
            /* [in] */ double dRate);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_balance )( 
            IWMPSettings2 * This,
            /* [retval][out] */ long *plBalance);
        
        /* [helpstring][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_balance )( 
            IWMPSettings2 * This,
            /* [in] */ long lBalance);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_volume )( 
            IWMPSettings2 * This,
            /* [retval][out] */ long *plVolume);
        
        /* [helpstring][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_volume )( 
            IWMPSettings2 * This,
            /* [in] */ long lVolume);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *getMode )( 
            IWMPSettings2 * This,
            /* [in] */ BSTR bstrMode,
            /* [retval][out] */ VARIANT_BOOL *pvarfMode);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *setMode )( 
            IWMPSettings2 * This,
            /* [in] */ BSTR bstrMode,
            /* [in] */ VARIANT_BOOL varfMode);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_enableErrorDialogs )( 
            IWMPSettings2 * This,
            /* [retval][out] */ VARIANT_BOOL *pfEnableErrorDialogs);
        
        /* [helpstring][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_enableErrorDialogs )( 
            IWMPSettings2 * This,
            /* [in] */ VARIANT_BOOL fEnableErrorDialogs);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_defaultAudioLanguage )( 
            IWMPSettings2 * This,
            /* [retval][out] */ long *plLangID);
        
        /* [propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_mediaAccessRights )( 
            IWMPSettings2 * This,
            /* [retval][out] */ BSTR *pbstrRights);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *requestMediaAccessRights )( 
            IWMPSettings2 * This,
            /* [in] */ BSTR bstrDesiredAccess,
            /* [retval][out] */ VARIANT_BOOL *pvbAccepted);
        
        END_INTERFACE
    } IWMPSettings2Vtbl;

    interface IWMPSettings2
    {
        CONST_VTBL struct IWMPSettings2Vtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IWMPSettings2_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IWMPSettings2_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IWMPSettings2_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IWMPSettings2_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IWMPSettings2_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IWMPSettings2_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IWMPSettings2_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IWMPSettings2_get_isAvailable(This,bstrItem,pIsAvailable)	\
    (This)->lpVtbl -> get_isAvailable(This,bstrItem,pIsAvailable)

#define IWMPSettings2_get_autoStart(This,pfAutoStart)	\
    (This)->lpVtbl -> get_autoStart(This,pfAutoStart)

#define IWMPSettings2_put_autoStart(This,fAutoStart)	\
    (This)->lpVtbl -> put_autoStart(This,fAutoStart)

#define IWMPSettings2_get_baseURL(This,pbstrBaseURL)	\
    (This)->lpVtbl -> get_baseURL(This,pbstrBaseURL)

#define IWMPSettings2_put_baseURL(This,bstrBaseURL)	\
    (This)->lpVtbl -> put_baseURL(This,bstrBaseURL)

#define IWMPSettings2_get_defaultFrame(This,pbstrDefaultFrame)	\
    (This)->lpVtbl -> get_defaultFrame(This,pbstrDefaultFrame)

#define IWMPSettings2_put_defaultFrame(This,bstrDefaultFrame)	\
    (This)->lpVtbl -> put_defaultFrame(This,bstrDefaultFrame)

#define IWMPSettings2_get_invokeURLs(This,pfInvokeURLs)	\
    (This)->lpVtbl -> get_invokeURLs(This,pfInvokeURLs)

#define IWMPSettings2_put_invokeURLs(This,fInvokeURLs)	\
    (This)->lpVtbl -> put_invokeURLs(This,fInvokeURLs)

#define IWMPSettings2_get_mute(This,pfMute)	\
    (This)->lpVtbl -> get_mute(This,pfMute)

#define IWMPSettings2_put_mute(This,fMute)	\
    (This)->lpVtbl -> put_mute(This,fMute)

#define IWMPSettings2_get_playCount(This,plCount)	\
    (This)->lpVtbl -> get_playCount(This,plCount)

#define IWMPSettings2_put_playCount(This,lCount)	\
    (This)->lpVtbl -> put_playCount(This,lCount)

#define IWMPSettings2_get_rate(This,pdRate)	\
    (This)->lpVtbl -> get_rate(This,pdRate)

#define IWMPSettings2_put_rate(This,dRate)	\
    (This)->lpVtbl -> put_rate(This,dRate)

#define IWMPSettings2_get_balance(This,plBalance)	\
    (This)->lpVtbl -> get_balance(This,plBalance)

#define IWMPSettings2_put_balance(This,lBalance)	\
    (This)->lpVtbl -> put_balance(This,lBalance)

#define IWMPSettings2_get_volume(This,plVolume)	\
    (This)->lpVtbl -> get_volume(This,plVolume)

#define IWMPSettings2_put_volume(This,lVolume)	\
    (This)->lpVtbl -> put_volume(This,lVolume)

#define IWMPSettings2_getMode(This,bstrMode,pvarfMode)	\
    (This)->lpVtbl -> getMode(This,bstrMode,pvarfMode)

#define IWMPSettings2_setMode(This,bstrMode,varfMode)	\
    (This)->lpVtbl -> setMode(This,bstrMode,varfMode)

#define IWMPSettings2_get_enableErrorDialogs(This,pfEnableErrorDialogs)	\
    (This)->lpVtbl -> get_enableErrorDialogs(This,pfEnableErrorDialogs)

#define IWMPSettings2_put_enableErrorDialogs(This,fEnableErrorDialogs)	\
    (This)->lpVtbl -> put_enableErrorDialogs(This,fEnableErrorDialogs)


#define IWMPSettings2_get_defaultAudioLanguage(This,plLangID)	\
    (This)->lpVtbl -> get_defaultAudioLanguage(This,plLangID)

#define IWMPSettings2_get_mediaAccessRights(This,pbstrRights)	\
    (This)->lpVtbl -> get_mediaAccessRights(This,pbstrRights)

#define IWMPSettings2_requestMediaAccessRights(This,bstrDesiredAccess,pvbAccepted)	\
    (This)->lpVtbl -> requestMediaAccessRights(This,bstrDesiredAccess,pvbAccepted)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE IWMPSettings2_get_defaultAudioLanguage_Proxy( 
    IWMPSettings2 * This,
    /* [retval][out] */ long *plLangID);


void __RPC_STUB IWMPSettings2_get_defaultAudioLanguage_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [propget][id] */ HRESULT STDMETHODCALLTYPE IWMPSettings2_get_mediaAccessRights_Proxy( 
    IWMPSettings2 * This,
    /* [retval][out] */ BSTR *pbstrRights);


void __RPC_STUB IWMPSettings2_get_mediaAccessRights_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id] */ HRESULT STDMETHODCALLTYPE IWMPSettings2_requestMediaAccessRights_Proxy( 
    IWMPSettings2 * This,
    /* [in] */ BSTR bstrDesiredAccess,
    /* [retval][out] */ VARIANT_BOOL *pvbAccepted);


void __RPC_STUB IWMPSettings2_requestMediaAccessRights_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IWMPSettings2_INTERFACE_DEFINED__ */


#ifndef __IWMPControls3_INTERFACE_DEFINED__
#define __IWMPControls3_INTERFACE_DEFINED__

/* interface IWMPControls3 */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_IWMPControls3;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("A1D1110E-D545-476a-9A78-AC3E4CB1E6BD")
    IWMPControls3 : public IWMPControls2
    {
    public:
        virtual /* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE get_audioLanguageCount( 
            /* [retval][out] */ long *plCount) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE getAudioLanguageID( 
            /* [in] */ long lIndex,
            /* [retval][out] */ long *plLangID) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE getAudioLanguageDescription( 
            /* [in] */ long lIndex,
            /* [retval][out] */ BSTR *pbstrLangDesc) = 0;
        
        virtual /* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE get_currentAudioLanguage( 
            /* [retval][out] */ long *plLangID) = 0;
        
        virtual /* [helpstring][propput][id] */ HRESULT STDMETHODCALLTYPE put_currentAudioLanguage( 
            /* [in] */ long lLangID) = 0;
        
        virtual /* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE get_currentAudioLanguageIndex( 
            /* [retval][out] */ long *plIndex) = 0;
        
        virtual /* [helpstring][propput][id] */ HRESULT STDMETHODCALLTYPE put_currentAudioLanguageIndex( 
            /* [in] */ long lIndex) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE getLanguageName( 
            /* [in] */ long lLangID,
            /* [retval][out] */ BSTR *pbstrLangName) = 0;
        
        virtual /* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE get_currentPositionTimecode( 
            /* [retval][out] */ BSTR *bstrTimecode) = 0;
        
        virtual /* [helpstring][propput][id] */ HRESULT STDMETHODCALLTYPE put_currentPositionTimecode( 
            /* [in] */ BSTR bstrTimecode) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IWMPControls3Vtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IWMPControls3 * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IWMPControls3 * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IWMPControls3 * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IWMPControls3 * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IWMPControls3 * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IWMPControls3 * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IWMPControls3 * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_isAvailable )( 
            IWMPControls3 * This,
            /* [in] */ BSTR bstrItem,
            /* [retval][out] */ VARIANT_BOOL *pIsAvailable);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *play )( 
            IWMPControls3 * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *stop )( 
            IWMPControls3 * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *pause )( 
            IWMPControls3 * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *fastForward )( 
            IWMPControls3 * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *fastReverse )( 
            IWMPControls3 * This);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_currentPosition )( 
            IWMPControls3 * This,
            /* [retval][out] */ double *pdCurrentPosition);
        
        /* [helpstring][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_currentPosition )( 
            IWMPControls3 * This,
            /* [in] */ double dCurrentPosition);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_currentPositionString )( 
            IWMPControls3 * This,
            /* [retval][out] */ BSTR *pbstrCurrentPosition);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *next )( 
            IWMPControls3 * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *previous )( 
            IWMPControls3 * This);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_currentItem )( 
            IWMPControls3 * This,
            /* [retval][out] */ IWMPMedia **ppIWMPMedia);
        
        /* [propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_currentItem )( 
            IWMPControls3 * This,
            /* [in] */ IWMPMedia *pIWMPMedia);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_currentMarker )( 
            IWMPControls3 * This,
            /* [retval][out] */ long *plMarker);
        
        /* [helpstring][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_currentMarker )( 
            IWMPControls3 * This,
            /* [in] */ long lMarker);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *playItem )( 
            IWMPControls3 * This,
            /* [in] */ IWMPMedia *pIWMPMedia);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *step )( 
            IWMPControls3 * This,
            /* [in] */ long lStep);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_audioLanguageCount )( 
            IWMPControls3 * This,
            /* [retval][out] */ long *plCount);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *getAudioLanguageID )( 
            IWMPControls3 * This,
            /* [in] */ long lIndex,
            /* [retval][out] */ long *plLangID);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *getAudioLanguageDescription )( 
            IWMPControls3 * This,
            /* [in] */ long lIndex,
            /* [retval][out] */ BSTR *pbstrLangDesc);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_currentAudioLanguage )( 
            IWMPControls3 * This,
            /* [retval][out] */ long *plLangID);
        
        /* [helpstring][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_currentAudioLanguage )( 
            IWMPControls3 * This,
            /* [in] */ long lLangID);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_currentAudioLanguageIndex )( 
            IWMPControls3 * This,
            /* [retval][out] */ long *plIndex);
        
        /* [helpstring][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_currentAudioLanguageIndex )( 
            IWMPControls3 * This,
            /* [in] */ long lIndex);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *getLanguageName )( 
            IWMPControls3 * This,
            /* [in] */ long lLangID,
            /* [retval][out] */ BSTR *pbstrLangName);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_currentPositionTimecode )( 
            IWMPControls3 * This,
            /* [retval][out] */ BSTR *bstrTimecode);
        
        /* [helpstring][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_currentPositionTimecode )( 
            IWMPControls3 * This,
            /* [in] */ BSTR bstrTimecode);
        
        END_INTERFACE
    } IWMPControls3Vtbl;

    interface IWMPControls3
    {
        CONST_VTBL struct IWMPControls3Vtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IWMPControls3_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IWMPControls3_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IWMPControls3_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IWMPControls3_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IWMPControls3_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IWMPControls3_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IWMPControls3_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IWMPControls3_get_isAvailable(This,bstrItem,pIsAvailable)	\
    (This)->lpVtbl -> get_isAvailable(This,bstrItem,pIsAvailable)

#define IWMPControls3_play(This)	\
    (This)->lpVtbl -> play(This)

#define IWMPControls3_stop(This)	\
    (This)->lpVtbl -> stop(This)

#define IWMPControls3_pause(This)	\
    (This)->lpVtbl -> pause(This)

#define IWMPControls3_fastForward(This)	\
    (This)->lpVtbl -> fastForward(This)

#define IWMPControls3_fastReverse(This)	\
    (This)->lpVtbl -> fastReverse(This)

#define IWMPControls3_get_currentPosition(This,pdCurrentPosition)	\
    (This)->lpVtbl -> get_currentPosition(This,pdCurrentPosition)

#define IWMPControls3_put_currentPosition(This,dCurrentPosition)	\
    (This)->lpVtbl -> put_currentPosition(This,dCurrentPosition)

#define IWMPControls3_get_currentPositionString(This,pbstrCurrentPosition)	\
    (This)->lpVtbl -> get_currentPositionString(This,pbstrCurrentPosition)

#define IWMPControls3_next(This)	\
    (This)->lpVtbl -> next(This)

#define IWMPControls3_previous(This)	\
    (This)->lpVtbl -> previous(This)

#define IWMPControls3_get_currentItem(This,ppIWMPMedia)	\
    (This)->lpVtbl -> get_currentItem(This,ppIWMPMedia)

#define IWMPControls3_put_currentItem(This,pIWMPMedia)	\
    (This)->lpVtbl -> put_currentItem(This,pIWMPMedia)

#define IWMPControls3_get_currentMarker(This,plMarker)	\
    (This)->lpVtbl -> get_currentMarker(This,plMarker)

#define IWMPControls3_put_currentMarker(This,lMarker)	\
    (This)->lpVtbl -> put_currentMarker(This,lMarker)

#define IWMPControls3_playItem(This,pIWMPMedia)	\
    (This)->lpVtbl -> playItem(This,pIWMPMedia)


#define IWMPControls3_step(This,lStep)	\
    (This)->lpVtbl -> step(This,lStep)


#define IWMPControls3_get_audioLanguageCount(This,plCount)	\
    (This)->lpVtbl -> get_audioLanguageCount(This,plCount)

#define IWMPControls3_getAudioLanguageID(This,lIndex,plLangID)	\
    (This)->lpVtbl -> getAudioLanguageID(This,lIndex,plLangID)

#define IWMPControls3_getAudioLanguageDescription(This,lIndex,pbstrLangDesc)	\
    (This)->lpVtbl -> getAudioLanguageDescription(This,lIndex,pbstrLangDesc)

#define IWMPControls3_get_currentAudioLanguage(This,plLangID)	\
    (This)->lpVtbl -> get_currentAudioLanguage(This,plLangID)

#define IWMPControls3_put_currentAudioLanguage(This,lLangID)	\
    (This)->lpVtbl -> put_currentAudioLanguage(This,lLangID)

#define IWMPControls3_get_currentAudioLanguageIndex(This,plIndex)	\
    (This)->lpVtbl -> get_currentAudioLanguageIndex(This,plIndex)

#define IWMPControls3_put_currentAudioLanguageIndex(This,lIndex)	\
    (This)->lpVtbl -> put_currentAudioLanguageIndex(This,lIndex)

#define IWMPControls3_getLanguageName(This,lLangID,pbstrLangName)	\
    (This)->lpVtbl -> getLanguageName(This,lLangID,pbstrLangName)

#define IWMPControls3_get_currentPositionTimecode(This,bstrTimecode)	\
    (This)->lpVtbl -> get_currentPositionTimecode(This,bstrTimecode)

#define IWMPControls3_put_currentPositionTimecode(This,bstrTimecode)	\
    (This)->lpVtbl -> put_currentPositionTimecode(This,bstrTimecode)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE IWMPControls3_get_audioLanguageCount_Proxy( 
    IWMPControls3 * This,
    /* [retval][out] */ long *plCount);


void __RPC_STUB IWMPControls3_get_audioLanguageCount_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IWMPControls3_getAudioLanguageID_Proxy( 
    IWMPControls3 * This,
    /* [in] */ long lIndex,
    /* [retval][out] */ long *plLangID);


void __RPC_STUB IWMPControls3_getAudioLanguageID_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IWMPControls3_getAudioLanguageDescription_Proxy( 
    IWMPControls3 * This,
    /* [in] */ long lIndex,
    /* [retval][out] */ BSTR *pbstrLangDesc);


void __RPC_STUB IWMPControls3_getAudioLanguageDescription_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE IWMPControls3_get_currentAudioLanguage_Proxy( 
    IWMPControls3 * This,
    /* [retval][out] */ long *plLangID);


void __RPC_STUB IWMPControls3_get_currentAudioLanguage_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propput][id] */ HRESULT STDMETHODCALLTYPE IWMPControls3_put_currentAudioLanguage_Proxy( 
    IWMPControls3 * This,
    /* [in] */ long lLangID);


void __RPC_STUB IWMPControls3_put_currentAudioLanguage_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE IWMPControls3_get_currentAudioLanguageIndex_Proxy( 
    IWMPControls3 * This,
    /* [retval][out] */ long *plIndex);


void __RPC_STUB IWMPControls3_get_currentAudioLanguageIndex_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propput][id] */ HRESULT STDMETHODCALLTYPE IWMPControls3_put_currentAudioLanguageIndex_Proxy( 
    IWMPControls3 * This,
    /* [in] */ long lIndex);


void __RPC_STUB IWMPControls3_put_currentAudioLanguageIndex_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IWMPControls3_getLanguageName_Proxy( 
    IWMPControls3 * This,
    /* [in] */ long lLangID,
    /* [retval][out] */ BSTR *pbstrLangName);


void __RPC_STUB IWMPControls3_getLanguageName_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE IWMPControls3_get_currentPositionTimecode_Proxy( 
    IWMPControls3 * This,
    /* [retval][out] */ BSTR *bstrTimecode);


void __RPC_STUB IWMPControls3_get_currentPositionTimecode_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propput][id] */ HRESULT STDMETHODCALLTYPE IWMPControls3_put_currentPositionTimecode_Proxy( 
    IWMPControls3 * This,
    /* [in] */ BSTR bstrTimecode);


void __RPC_STUB IWMPControls3_put_currentPositionTimecode_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IWMPControls3_INTERFACE_DEFINED__ */


#ifndef __IWMPClosedCaption2_INTERFACE_DEFINED__
#define __IWMPClosedCaption2_INTERFACE_DEFINED__

/* interface IWMPClosedCaption2 */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_IWMPClosedCaption2;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("350BA78B-6BC8-4113-A5F5-312056934EB6")
    IWMPClosedCaption2 : public IWMPClosedCaption
    {
    public:
        virtual /* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE get_SAMILangCount( 
            /* [retval][out] */ long *plCount) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE getSAMILangName( 
            /* [in] */ long nIndex,
            /* [retval][out] */ BSTR *pbstrName) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE getSAMILangID( 
            /* [in] */ long nIndex,
            /* [retval][out] */ long *plLangID) = 0;
        
        virtual /* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE get_SAMIStyleCount( 
            /* [retval][out] */ long *plCount) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE getSAMIStyleName( 
            /* [in] */ long nIndex,
            /* [retval][out] */ BSTR *pbstrName) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IWMPClosedCaption2Vtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IWMPClosedCaption2 * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IWMPClosedCaption2 * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IWMPClosedCaption2 * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IWMPClosedCaption2 * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IWMPClosedCaption2 * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IWMPClosedCaption2 * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IWMPClosedCaption2 * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_SAMIStyle )( 
            IWMPClosedCaption2 * This,
            /* [retval][out] */ BSTR *pbstrSAMIStyle);
        
        /* [helpstring][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_SAMIStyle )( 
            IWMPClosedCaption2 * This,
            /* [in] */ BSTR bstrSAMIStyle);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_SAMILang )( 
            IWMPClosedCaption2 * This,
            /* [retval][out] */ BSTR *pbstrSAMILang);
        
        /* [helpstring][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_SAMILang )( 
            IWMPClosedCaption2 * This,
            /* [in] */ BSTR bstrSAMILang);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_SAMIFileName )( 
            IWMPClosedCaption2 * This,
            /* [retval][out] */ BSTR *pbstrSAMIFileName);
        
        /* [helpstring][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_SAMIFileName )( 
            IWMPClosedCaption2 * This,
            /* [in] */ BSTR bstrSAMIFileName);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_captioningId )( 
            IWMPClosedCaption2 * This,
            /* [retval][out] */ BSTR *pbstrCaptioningID);
        
        /* [helpstring][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_captioningId )( 
            IWMPClosedCaption2 * This,
            /* [in] */ BSTR bstrCaptioningID);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_SAMILangCount )( 
            IWMPClosedCaption2 * This,
            /* [retval][out] */ long *plCount);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *getSAMILangName )( 
            IWMPClosedCaption2 * This,
            /* [in] */ long nIndex,
            /* [retval][out] */ BSTR *pbstrName);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *getSAMILangID )( 
            IWMPClosedCaption2 * This,
            /* [in] */ long nIndex,
            /* [retval][out] */ long *plLangID);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_SAMIStyleCount )( 
            IWMPClosedCaption2 * This,
            /* [retval][out] */ long *plCount);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *getSAMIStyleName )( 
            IWMPClosedCaption2 * This,
            /* [in] */ long nIndex,
            /* [retval][out] */ BSTR *pbstrName);
        
        END_INTERFACE
    } IWMPClosedCaption2Vtbl;

    interface IWMPClosedCaption2
    {
        CONST_VTBL struct IWMPClosedCaption2Vtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IWMPClosedCaption2_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IWMPClosedCaption2_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IWMPClosedCaption2_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IWMPClosedCaption2_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IWMPClosedCaption2_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IWMPClosedCaption2_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IWMPClosedCaption2_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IWMPClosedCaption2_get_SAMIStyle(This,pbstrSAMIStyle)	\
    (This)->lpVtbl -> get_SAMIStyle(This,pbstrSAMIStyle)

#define IWMPClosedCaption2_put_SAMIStyle(This,bstrSAMIStyle)	\
    (This)->lpVtbl -> put_SAMIStyle(This,bstrSAMIStyle)

#define IWMPClosedCaption2_get_SAMILang(This,pbstrSAMILang)	\
    (This)->lpVtbl -> get_SAMILang(This,pbstrSAMILang)

#define IWMPClosedCaption2_put_SAMILang(This,bstrSAMILang)	\
    (This)->lpVtbl -> put_SAMILang(This,bstrSAMILang)

#define IWMPClosedCaption2_get_SAMIFileName(This,pbstrSAMIFileName)	\
    (This)->lpVtbl -> get_SAMIFileName(This,pbstrSAMIFileName)

#define IWMPClosedCaption2_put_SAMIFileName(This,bstrSAMIFileName)	\
    (This)->lpVtbl -> put_SAMIFileName(This,bstrSAMIFileName)

#define IWMPClosedCaption2_get_captioningId(This,pbstrCaptioningID)	\
    (This)->lpVtbl -> get_captioningId(This,pbstrCaptioningID)

#define IWMPClosedCaption2_put_captioningId(This,bstrCaptioningID)	\
    (This)->lpVtbl -> put_captioningId(This,bstrCaptioningID)


#define IWMPClosedCaption2_get_SAMILangCount(This,plCount)	\
    (This)->lpVtbl -> get_SAMILangCount(This,plCount)

#define IWMPClosedCaption2_getSAMILangName(This,nIndex,pbstrName)	\
    (This)->lpVtbl -> getSAMILangName(This,nIndex,pbstrName)

#define IWMPClosedCaption2_getSAMILangID(This,nIndex,plLangID)	\
    (This)->lpVtbl -> getSAMILangID(This,nIndex,plLangID)

#define IWMPClosedCaption2_get_SAMIStyleCount(This,plCount)	\
    (This)->lpVtbl -> get_SAMIStyleCount(This,plCount)

#define IWMPClosedCaption2_getSAMIStyleName(This,nIndex,pbstrName)	\
    (This)->lpVtbl -> getSAMIStyleName(This,nIndex,pbstrName)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE IWMPClosedCaption2_get_SAMILangCount_Proxy( 
    IWMPClosedCaption2 * This,
    /* [retval][out] */ long *plCount);


void __RPC_STUB IWMPClosedCaption2_get_SAMILangCount_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IWMPClosedCaption2_getSAMILangName_Proxy( 
    IWMPClosedCaption2 * This,
    /* [in] */ long nIndex,
    /* [retval][out] */ BSTR *pbstrName);


void __RPC_STUB IWMPClosedCaption2_getSAMILangName_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IWMPClosedCaption2_getSAMILangID_Proxy( 
    IWMPClosedCaption2 * This,
    /* [in] */ long nIndex,
    /* [retval][out] */ long *plLangID);


void __RPC_STUB IWMPClosedCaption2_getSAMILangID_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE IWMPClosedCaption2_get_SAMIStyleCount_Proxy( 
    IWMPClosedCaption2 * This,
    /* [retval][out] */ long *plCount);


void __RPC_STUB IWMPClosedCaption2_get_SAMIStyleCount_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IWMPClosedCaption2_getSAMIStyleName_Proxy( 
    IWMPClosedCaption2 * This,
    /* [in] */ long nIndex,
    /* [retval][out] */ BSTR *pbstrName);


void __RPC_STUB IWMPClosedCaption2_getSAMIStyleName_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IWMPClosedCaption2_INTERFACE_DEFINED__ */


#ifndef __IWMPPlayerApplication_INTERFACE_DEFINED__
#define __IWMPPlayerApplication_INTERFACE_DEFINED__

/* interface IWMPPlayerApplication */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_IWMPPlayerApplication;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("40897764-CEAB-47be-AD4A-8E28537F9BBF")
    IWMPPlayerApplication : public IDispatch
    {
    public:
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE switchToPlayerApplication( void) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE switchToControl( void) = 0;
        
        virtual /* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE get_playerDocked( 
            /* [retval][out] */ VARIANT_BOOL *pbPlayerDocked) = 0;
        
        virtual /* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE get_hasDisplay( 
            /* [retval][out] */ VARIANT_BOOL *pbHasDisplay) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IWMPPlayerApplicationVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IWMPPlayerApplication * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IWMPPlayerApplication * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IWMPPlayerApplication * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IWMPPlayerApplication * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IWMPPlayerApplication * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IWMPPlayerApplication * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IWMPPlayerApplication * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *switchToPlayerApplication )( 
            IWMPPlayerApplication * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *switchToControl )( 
            IWMPPlayerApplication * This);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_playerDocked )( 
            IWMPPlayerApplication * This,
            /* [retval][out] */ VARIANT_BOOL *pbPlayerDocked);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_hasDisplay )( 
            IWMPPlayerApplication * This,
            /* [retval][out] */ VARIANT_BOOL *pbHasDisplay);
        
        END_INTERFACE
    } IWMPPlayerApplicationVtbl;

    interface IWMPPlayerApplication
    {
        CONST_VTBL struct IWMPPlayerApplicationVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IWMPPlayerApplication_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IWMPPlayerApplication_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IWMPPlayerApplication_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IWMPPlayerApplication_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IWMPPlayerApplication_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IWMPPlayerApplication_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IWMPPlayerApplication_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IWMPPlayerApplication_switchToPlayerApplication(This)	\
    (This)->lpVtbl -> switchToPlayerApplication(This)

#define IWMPPlayerApplication_switchToControl(This)	\
    (This)->lpVtbl -> switchToControl(This)

#define IWMPPlayerApplication_get_playerDocked(This,pbPlayerDocked)	\
    (This)->lpVtbl -> get_playerDocked(This,pbPlayerDocked)

#define IWMPPlayerApplication_get_hasDisplay(This,pbHasDisplay)	\
    (This)->lpVtbl -> get_hasDisplay(This,pbHasDisplay)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IWMPPlayerApplication_switchToPlayerApplication_Proxy( 
    IWMPPlayerApplication * This);


void __RPC_STUB IWMPPlayerApplication_switchToPlayerApplication_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IWMPPlayerApplication_switchToControl_Proxy( 
    IWMPPlayerApplication * This);


void __RPC_STUB IWMPPlayerApplication_switchToControl_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE IWMPPlayerApplication_get_playerDocked_Proxy( 
    IWMPPlayerApplication * This,
    /* [retval][out] */ VARIANT_BOOL *pbPlayerDocked);


void __RPC_STUB IWMPPlayerApplication_get_playerDocked_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE IWMPPlayerApplication_get_hasDisplay_Proxy( 
    IWMPPlayerApplication * This,
    /* [retval][out] */ VARIANT_BOOL *pbHasDisplay);


void __RPC_STUB IWMPPlayerApplication_get_hasDisplay_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IWMPPlayerApplication_INTERFACE_DEFINED__ */


#ifndef __IWMPCore3_INTERFACE_DEFINED__
#define __IWMPCore3_INTERFACE_DEFINED__

/* interface IWMPCore3 */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_IWMPCore3;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("7587C667-628F-499f-88E7-6A6F4E888464")
    IWMPCore3 : public IWMPCore2
    {
    public:
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE newPlaylist( 
            /* [in] */ BSTR bstrName,
            /* [in] */ BSTR bstrURL,
            /* [retval][out] */ IWMPPlaylist **ppPlaylist) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE newMedia( 
            /* [in] */ BSTR bstrURL,
            /* [retval][out] */ IWMPMedia **ppMedia) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IWMPCore3Vtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IWMPCore3 * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IWMPCore3 * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IWMPCore3 * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IWMPCore3 * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IWMPCore3 * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IWMPCore3 * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IWMPCore3 * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *close )( 
            IWMPCore3 * This);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_URL )( 
            IWMPCore3 * This,
            /* [retval][out] */ BSTR *pbstrURL);
        
        /* [propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_URL )( 
            IWMPCore3 * This,
            /* [in] */ BSTR bstrURL);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_openState )( 
            IWMPCore3 * This,
            /* [retval][out] */ WMPOpenState *pwmpos);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_playState )( 
            IWMPCore3 * This,
            /* [retval][out] */ WMPPlayState *pwmpps);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_controls )( 
            IWMPCore3 * This,
            /* [retval][out] */ IWMPControls **ppControl);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_settings )( 
            IWMPCore3 * This,
            /* [retval][out] */ IWMPSettings **ppSettings);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_currentMedia )( 
            IWMPCore3 * This,
            /* [retval][out] */ IWMPMedia **ppMedia);
        
        /* [propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_currentMedia )( 
            IWMPCore3 * This,
            /* [in] */ IWMPMedia *pMedia);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_mediaCollection )( 
            IWMPCore3 * This,
            /* [retval][out] */ IWMPMediaCollection **ppMediaCollection);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_playlistCollection )( 
            IWMPCore3 * This,
            /* [retval][out] */ IWMPPlaylistCollection **ppPlaylistCollection);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_versionInfo )( 
            IWMPCore3 * This,
            /* [retval][out] */ BSTR *pbstrVersionInfo);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *launchURL )( 
            IWMPCore3 * This,
            /* [in] */ BSTR bstrURL);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_network )( 
            IWMPCore3 * This,
            /* [retval][out] */ IWMPNetwork **ppQNI);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_currentPlaylist )( 
            IWMPCore3 * This,
            /* [retval][out] */ IWMPPlaylist **ppPL);
        
        /* [helpstring][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_currentPlaylist )( 
            IWMPCore3 * This,
            /* [in] */ IWMPPlaylist *pPL);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_cdromCollection )( 
            IWMPCore3 * This,
            /* [retval][out] */ IWMPCdromCollection **ppCdromCollection);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_closedCaption )( 
            IWMPCore3 * This,
            /* [retval][out] */ IWMPClosedCaption **ppClosedCaption);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_isOnline )( 
            IWMPCore3 * This,
            /* [retval][out] */ VARIANT_BOOL *pfOnline);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_error )( 
            IWMPCore3 * This,
            /* [retval][out] */ IWMPError **ppError);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_status )( 
            IWMPCore3 * This,
            /* [retval][out] */ BSTR *pbstrStatus);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_dvd )( 
            IWMPCore3 * This,
            /* [retval][out] */ IWMPDVD **ppDVD);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *newPlaylist )( 
            IWMPCore3 * This,
            /* [in] */ BSTR bstrName,
            /* [in] */ BSTR bstrURL,
            /* [retval][out] */ IWMPPlaylist **ppPlaylist);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *newMedia )( 
            IWMPCore3 * This,
            /* [in] */ BSTR bstrURL,
            /* [retval][out] */ IWMPMedia **ppMedia);
        
        END_INTERFACE
    } IWMPCore3Vtbl;

    interface IWMPCore3
    {
        CONST_VTBL struct IWMPCore3Vtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IWMPCore3_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IWMPCore3_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IWMPCore3_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IWMPCore3_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IWMPCore3_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IWMPCore3_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IWMPCore3_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IWMPCore3_close(This)	\
    (This)->lpVtbl -> close(This)

#define IWMPCore3_get_URL(This,pbstrURL)	\
    (This)->lpVtbl -> get_URL(This,pbstrURL)

#define IWMPCore3_put_URL(This,bstrURL)	\
    (This)->lpVtbl -> put_URL(This,bstrURL)

#define IWMPCore3_get_openState(This,pwmpos)	\
    (This)->lpVtbl -> get_openState(This,pwmpos)

#define IWMPCore3_get_playState(This,pwmpps)	\
    (This)->lpVtbl -> get_playState(This,pwmpps)

#define IWMPCore3_get_controls(This,ppControl)	\
    (This)->lpVtbl -> get_controls(This,ppControl)

#define IWMPCore3_get_settings(This,ppSettings)	\
    (This)->lpVtbl -> get_settings(This,ppSettings)

#define IWMPCore3_get_currentMedia(This,ppMedia)	\
    (This)->lpVtbl -> get_currentMedia(This,ppMedia)

#define IWMPCore3_put_currentMedia(This,pMedia)	\
    (This)->lpVtbl -> put_currentMedia(This,pMedia)

#define IWMPCore3_get_mediaCollection(This,ppMediaCollection)	\
    (This)->lpVtbl -> get_mediaCollection(This,ppMediaCollection)

#define IWMPCore3_get_playlistCollection(This,ppPlaylistCollection)	\
    (This)->lpVtbl -> get_playlistCollection(This,ppPlaylistCollection)

#define IWMPCore3_get_versionInfo(This,pbstrVersionInfo)	\
    (This)->lpVtbl -> get_versionInfo(This,pbstrVersionInfo)

#define IWMPCore3_launchURL(This,bstrURL)	\
    (This)->lpVtbl -> launchURL(This,bstrURL)

#define IWMPCore3_get_network(This,ppQNI)	\
    (This)->lpVtbl -> get_network(This,ppQNI)

#define IWMPCore3_get_currentPlaylist(This,ppPL)	\
    (This)->lpVtbl -> get_currentPlaylist(This,ppPL)

#define IWMPCore3_put_currentPlaylist(This,pPL)	\
    (This)->lpVtbl -> put_currentPlaylist(This,pPL)

#define IWMPCore3_get_cdromCollection(This,ppCdromCollection)	\
    (This)->lpVtbl -> get_cdromCollection(This,ppCdromCollection)

#define IWMPCore3_get_closedCaption(This,ppClosedCaption)	\
    (This)->lpVtbl -> get_closedCaption(This,ppClosedCaption)

#define IWMPCore3_get_isOnline(This,pfOnline)	\
    (This)->lpVtbl -> get_isOnline(This,pfOnline)

#define IWMPCore3_get_error(This,ppError)	\
    (This)->lpVtbl -> get_error(This,ppError)

#define IWMPCore3_get_status(This,pbstrStatus)	\
    (This)->lpVtbl -> get_status(This,pbstrStatus)


#define IWMPCore3_get_dvd(This,ppDVD)	\
    (This)->lpVtbl -> get_dvd(This,ppDVD)


#define IWMPCore3_newPlaylist(This,bstrName,bstrURL,ppPlaylist)	\
    (This)->lpVtbl -> newPlaylist(This,bstrName,bstrURL,ppPlaylist)

#define IWMPCore3_newMedia(This,bstrURL,ppMedia)	\
    (This)->lpVtbl -> newMedia(This,bstrURL,ppMedia)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IWMPCore3_newPlaylist_Proxy( 
    IWMPCore3 * This,
    /* [in] */ BSTR bstrName,
    /* [in] */ BSTR bstrURL,
    /* [retval][out] */ IWMPPlaylist **ppPlaylist);


void __RPC_STUB IWMPCore3_newPlaylist_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IWMPCore3_newMedia_Proxy( 
    IWMPCore3 * This,
    /* [in] */ BSTR bstrURL,
    /* [retval][out] */ IWMPMedia **ppMedia);


void __RPC_STUB IWMPCore3_newMedia_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IWMPCore3_INTERFACE_DEFINED__ */


#ifndef __IWMPPlayer4_INTERFACE_DEFINED__
#define __IWMPPlayer4_INTERFACE_DEFINED__

/* interface IWMPPlayer4 */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_IWMPPlayer4;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("6C497D62-8919-413c-82DB-E935FB3EC584")
    IWMPPlayer4 : public IWMPCore3
    {
    public:
        virtual /* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE get_enabled( 
            /* [retval][out] */ VARIANT_BOOL *pbEnabled) = 0;
        
        virtual /* [helpstring][propput][id] */ HRESULT STDMETHODCALLTYPE put_enabled( 
            /* [in] */ VARIANT_BOOL bEnabled) = 0;
        
        virtual /* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE get_fullScreen( 
            /* [retval][out] */ VARIANT_BOOL *pbFullScreen) = 0;
        
        virtual /* [helpstring][propput][id] */ HRESULT STDMETHODCALLTYPE put_fullScreen( 
            VARIANT_BOOL bFullScreen) = 0;
        
        virtual /* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE get_enableContextMenu( 
            /* [retval][out] */ VARIANT_BOOL *pbEnableContextMenu) = 0;
        
        virtual /* [helpstring][propput][id] */ HRESULT STDMETHODCALLTYPE put_enableContextMenu( 
            VARIANT_BOOL bEnableContextMenu) = 0;
        
        virtual /* [helpstring][propput][id] */ HRESULT STDMETHODCALLTYPE put_uiMode( 
            /* [in] */ BSTR bstrMode) = 0;
        
        virtual /* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE get_uiMode( 
            /* [retval][out] */ BSTR *pbstrMode) = 0;
        
        virtual /* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE get_stretchToFit( 
            /* [retval][out] */ VARIANT_BOOL *pbEnabled) = 0;
        
        virtual /* [helpstring][propput][id] */ HRESULT STDMETHODCALLTYPE put_stretchToFit( 
            /* [in] */ VARIANT_BOOL bEnabled) = 0;
        
        virtual /* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE get_windowlessVideo( 
            /* [retval][out] */ VARIANT_BOOL *pbEnabled) = 0;
        
        virtual /* [helpstring][propput][id] */ HRESULT STDMETHODCALLTYPE put_windowlessVideo( 
            /* [in] */ VARIANT_BOOL bEnabled) = 0;
        
        virtual /* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE get_isRemote( 
            /* [retval][out] */ VARIANT_BOOL *pvarfIsRemote) = 0;
        
        virtual /* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE get_playerApplication( 
            /* [retval][out] */ IWMPPlayerApplication **ppIWMPPlayerApplication) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE openPlayer( 
            /* [in] */ BSTR bstrURL) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IWMPPlayer4Vtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IWMPPlayer4 * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IWMPPlayer4 * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IWMPPlayer4 * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IWMPPlayer4 * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IWMPPlayer4 * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IWMPPlayer4 * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IWMPPlayer4 * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *close )( 
            IWMPPlayer4 * This);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_URL )( 
            IWMPPlayer4 * This,
            /* [retval][out] */ BSTR *pbstrURL);
        
        /* [propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_URL )( 
            IWMPPlayer4 * This,
            /* [in] */ BSTR bstrURL);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_openState )( 
            IWMPPlayer4 * This,
            /* [retval][out] */ WMPOpenState *pwmpos);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_playState )( 
            IWMPPlayer4 * This,
            /* [retval][out] */ WMPPlayState *pwmpps);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_controls )( 
            IWMPPlayer4 * This,
            /* [retval][out] */ IWMPControls **ppControl);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_settings )( 
            IWMPPlayer4 * This,
            /* [retval][out] */ IWMPSettings **ppSettings);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_currentMedia )( 
            IWMPPlayer4 * This,
            /* [retval][out] */ IWMPMedia **ppMedia);
        
        /* [propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_currentMedia )( 
            IWMPPlayer4 * This,
            /* [in] */ IWMPMedia *pMedia);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_mediaCollection )( 
            IWMPPlayer4 * This,
            /* [retval][out] */ IWMPMediaCollection **ppMediaCollection);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_playlistCollection )( 
            IWMPPlayer4 * This,
            /* [retval][out] */ IWMPPlaylistCollection **ppPlaylistCollection);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_versionInfo )( 
            IWMPPlayer4 * This,
            /* [retval][out] */ BSTR *pbstrVersionInfo);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *launchURL )( 
            IWMPPlayer4 * This,
            /* [in] */ BSTR bstrURL);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_network )( 
            IWMPPlayer4 * This,
            /* [retval][out] */ IWMPNetwork **ppQNI);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_currentPlaylist )( 
            IWMPPlayer4 * This,
            /* [retval][out] */ IWMPPlaylist **ppPL);
        
        /* [helpstring][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_currentPlaylist )( 
            IWMPPlayer4 * This,
            /* [in] */ IWMPPlaylist *pPL);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_cdromCollection )( 
            IWMPPlayer4 * This,
            /* [retval][out] */ IWMPCdromCollection **ppCdromCollection);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_closedCaption )( 
            IWMPPlayer4 * This,
            /* [retval][out] */ IWMPClosedCaption **ppClosedCaption);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_isOnline )( 
            IWMPPlayer4 * This,
            /* [retval][out] */ VARIANT_BOOL *pfOnline);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_error )( 
            IWMPPlayer4 * This,
            /* [retval][out] */ IWMPError **ppError);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_status )( 
            IWMPPlayer4 * This,
            /* [retval][out] */ BSTR *pbstrStatus);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_dvd )( 
            IWMPPlayer4 * This,
            /* [retval][out] */ IWMPDVD **ppDVD);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *newPlaylist )( 
            IWMPPlayer4 * This,
            /* [in] */ BSTR bstrName,
            /* [in] */ BSTR bstrURL,
            /* [retval][out] */ IWMPPlaylist **ppPlaylist);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *newMedia )( 
            IWMPPlayer4 * This,
            /* [in] */ BSTR bstrURL,
            /* [retval][out] */ IWMPMedia **ppMedia);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_enabled )( 
            IWMPPlayer4 * This,
            /* [retval][out] */ VARIANT_BOOL *pbEnabled);
        
        /* [helpstring][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_enabled )( 
            IWMPPlayer4 * This,
            /* [in] */ VARIANT_BOOL bEnabled);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_fullScreen )( 
            IWMPPlayer4 * This,
            /* [retval][out] */ VARIANT_BOOL *pbFullScreen);
        
        /* [helpstring][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_fullScreen )( 
            IWMPPlayer4 * This,
            VARIANT_BOOL bFullScreen);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_enableContextMenu )( 
            IWMPPlayer4 * This,
            /* [retval][out] */ VARIANT_BOOL *pbEnableContextMenu);
        
        /* [helpstring][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_enableContextMenu )( 
            IWMPPlayer4 * This,
            VARIANT_BOOL bEnableContextMenu);
        
        /* [helpstring][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_uiMode )( 
            IWMPPlayer4 * This,
            /* [in] */ BSTR bstrMode);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_uiMode )( 
            IWMPPlayer4 * This,
            /* [retval][out] */ BSTR *pbstrMode);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_stretchToFit )( 
            IWMPPlayer4 * This,
            /* [retval][out] */ VARIANT_BOOL *pbEnabled);
        
        /* [helpstring][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_stretchToFit )( 
            IWMPPlayer4 * This,
            /* [in] */ VARIANT_BOOL bEnabled);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_windowlessVideo )( 
            IWMPPlayer4 * This,
            /* [retval][out] */ VARIANT_BOOL *pbEnabled);
        
        /* [helpstring][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_windowlessVideo )( 
            IWMPPlayer4 * This,
            /* [in] */ VARIANT_BOOL bEnabled);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_isRemote )( 
            IWMPPlayer4 * This,
            /* [retval][out] */ VARIANT_BOOL *pvarfIsRemote);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_playerApplication )( 
            IWMPPlayer4 * This,
            /* [retval][out] */ IWMPPlayerApplication **ppIWMPPlayerApplication);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *openPlayer )( 
            IWMPPlayer4 * This,
            /* [in] */ BSTR bstrURL);
        
        END_INTERFACE
    } IWMPPlayer4Vtbl;

    interface IWMPPlayer4
    {
        CONST_VTBL struct IWMPPlayer4Vtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IWMPPlayer4_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IWMPPlayer4_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IWMPPlayer4_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IWMPPlayer4_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IWMPPlayer4_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IWMPPlayer4_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IWMPPlayer4_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IWMPPlayer4_close(This)	\
    (This)->lpVtbl -> close(This)

#define IWMPPlayer4_get_URL(This,pbstrURL)	\
    (This)->lpVtbl -> get_URL(This,pbstrURL)

#define IWMPPlayer4_put_URL(This,bstrURL)	\
    (This)->lpVtbl -> put_URL(This,bstrURL)

#define IWMPPlayer4_get_openState(This,pwmpos)	\
    (This)->lpVtbl -> get_openState(This,pwmpos)

#define IWMPPlayer4_get_playState(This,pwmpps)	\
    (This)->lpVtbl -> get_playState(This,pwmpps)

#define IWMPPlayer4_get_controls(This,ppControl)	\
    (This)->lpVtbl -> get_controls(This,ppControl)

#define IWMPPlayer4_get_settings(This,ppSettings)	\
    (This)->lpVtbl -> get_settings(This,ppSettings)

#define IWMPPlayer4_get_currentMedia(This,ppMedia)	\
    (This)->lpVtbl -> get_currentMedia(This,ppMedia)

#define IWMPPlayer4_put_currentMedia(This,pMedia)	\
    (This)->lpVtbl -> put_currentMedia(This,pMedia)

#define IWMPPlayer4_get_mediaCollection(This,ppMediaCollection)	\
    (This)->lpVtbl -> get_mediaCollection(This,ppMediaCollection)

#define IWMPPlayer4_get_playlistCollection(This,ppPlaylistCollection)	\
    (This)->lpVtbl -> get_playlistCollection(This,ppPlaylistCollection)

#define IWMPPlayer4_get_versionInfo(This,pbstrVersionInfo)	\
    (This)->lpVtbl -> get_versionInfo(This,pbstrVersionInfo)

#define IWMPPlayer4_launchURL(This,bstrURL)	\
    (This)->lpVtbl -> launchURL(This,bstrURL)

#define IWMPPlayer4_get_network(This,ppQNI)	\
    (This)->lpVtbl -> get_network(This,ppQNI)

#define IWMPPlayer4_get_currentPlaylist(This,ppPL)	\
    (This)->lpVtbl -> get_currentPlaylist(This,ppPL)

#define IWMPPlayer4_put_currentPlaylist(This,pPL)	\
    (This)->lpVtbl -> put_currentPlaylist(This,pPL)

#define IWMPPlayer4_get_cdromCollection(This,ppCdromCollection)	\
    (This)->lpVtbl -> get_cdromCollection(This,ppCdromCollection)

#define IWMPPlayer4_get_closedCaption(This,ppClosedCaption)	\
    (This)->lpVtbl -> get_closedCaption(This,ppClosedCaption)

#define IWMPPlayer4_get_isOnline(This,pfOnline)	\
    (This)->lpVtbl -> get_isOnline(This,pfOnline)

#define IWMPPlayer4_get_error(This,ppError)	\
    (This)->lpVtbl -> get_error(This,ppError)

#define IWMPPlayer4_get_status(This,pbstrStatus)	\
    (This)->lpVtbl -> get_status(This,pbstrStatus)


#define IWMPPlayer4_get_dvd(This,ppDVD)	\
    (This)->lpVtbl -> get_dvd(This,ppDVD)


#define IWMPPlayer4_newPlaylist(This,bstrName,bstrURL,ppPlaylist)	\
    (This)->lpVtbl -> newPlaylist(This,bstrName,bstrURL,ppPlaylist)

#define IWMPPlayer4_newMedia(This,bstrURL,ppMedia)	\
    (This)->lpVtbl -> newMedia(This,bstrURL,ppMedia)


#define IWMPPlayer4_get_enabled(This,pbEnabled)	\
    (This)->lpVtbl -> get_enabled(This,pbEnabled)

#define IWMPPlayer4_put_enabled(This,bEnabled)	\
    (This)->lpVtbl -> put_enabled(This,bEnabled)

#define IWMPPlayer4_get_fullScreen(This,pbFullScreen)	\
    (This)->lpVtbl -> get_fullScreen(This,pbFullScreen)

#define IWMPPlayer4_put_fullScreen(This,bFullScreen)	\
    (This)->lpVtbl -> put_fullScreen(This,bFullScreen)

#define IWMPPlayer4_get_enableContextMenu(This,pbEnableContextMenu)	\
    (This)->lpVtbl -> get_enableContextMenu(This,pbEnableContextMenu)

#define IWMPPlayer4_put_enableContextMenu(This,bEnableContextMenu)	\
    (This)->lpVtbl -> put_enableContextMenu(This,bEnableContextMenu)

#define IWMPPlayer4_put_uiMode(This,bstrMode)	\
    (This)->lpVtbl -> put_uiMode(This,bstrMode)

#define IWMPPlayer4_get_uiMode(This,pbstrMode)	\
    (This)->lpVtbl -> get_uiMode(This,pbstrMode)

#define IWMPPlayer4_get_stretchToFit(This,pbEnabled)	\
    (This)->lpVtbl -> get_stretchToFit(This,pbEnabled)

#define IWMPPlayer4_put_stretchToFit(This,bEnabled)	\
    (This)->lpVtbl -> put_stretchToFit(This,bEnabled)

#define IWMPPlayer4_get_windowlessVideo(This,pbEnabled)	\
    (This)->lpVtbl -> get_windowlessVideo(This,pbEnabled)

#define IWMPPlayer4_put_windowlessVideo(This,bEnabled)	\
    (This)->lpVtbl -> put_windowlessVideo(This,bEnabled)

#define IWMPPlayer4_get_isRemote(This,pvarfIsRemote)	\
    (This)->lpVtbl -> get_isRemote(This,pvarfIsRemote)

#define IWMPPlayer4_get_playerApplication(This,ppIWMPPlayerApplication)	\
    (This)->lpVtbl -> get_playerApplication(This,ppIWMPPlayerApplication)

#define IWMPPlayer4_openPlayer(This,bstrURL)	\
    (This)->lpVtbl -> openPlayer(This,bstrURL)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE IWMPPlayer4_get_enabled_Proxy( 
    IWMPPlayer4 * This,
    /* [retval][out] */ VARIANT_BOOL *pbEnabled);


void __RPC_STUB IWMPPlayer4_get_enabled_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propput][id] */ HRESULT STDMETHODCALLTYPE IWMPPlayer4_put_enabled_Proxy( 
    IWMPPlayer4 * This,
    /* [in] */ VARIANT_BOOL bEnabled);


void __RPC_STUB IWMPPlayer4_put_enabled_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE IWMPPlayer4_get_fullScreen_Proxy( 
    IWMPPlayer4 * This,
    /* [retval][out] */ VARIANT_BOOL *pbFullScreen);


void __RPC_STUB IWMPPlayer4_get_fullScreen_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propput][id] */ HRESULT STDMETHODCALLTYPE IWMPPlayer4_put_fullScreen_Proxy( 
    IWMPPlayer4 * This,
    VARIANT_BOOL bFullScreen);


void __RPC_STUB IWMPPlayer4_put_fullScreen_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE IWMPPlayer4_get_enableContextMenu_Proxy( 
    IWMPPlayer4 * This,
    /* [retval][out] */ VARIANT_BOOL *pbEnableContextMenu);


void __RPC_STUB IWMPPlayer4_get_enableContextMenu_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propput][id] */ HRESULT STDMETHODCALLTYPE IWMPPlayer4_put_enableContextMenu_Proxy( 
    IWMPPlayer4 * This,
    VARIANT_BOOL bEnableContextMenu);


void __RPC_STUB IWMPPlayer4_put_enableContextMenu_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propput][id] */ HRESULT STDMETHODCALLTYPE IWMPPlayer4_put_uiMode_Proxy( 
    IWMPPlayer4 * This,
    /* [in] */ BSTR bstrMode);


void __RPC_STUB IWMPPlayer4_put_uiMode_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE IWMPPlayer4_get_uiMode_Proxy( 
    IWMPPlayer4 * This,
    /* [retval][out] */ BSTR *pbstrMode);


void __RPC_STUB IWMPPlayer4_get_uiMode_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE IWMPPlayer4_get_stretchToFit_Proxy( 
    IWMPPlayer4 * This,
    /* [retval][out] */ VARIANT_BOOL *pbEnabled);


void __RPC_STUB IWMPPlayer4_get_stretchToFit_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propput][id] */ HRESULT STDMETHODCALLTYPE IWMPPlayer4_put_stretchToFit_Proxy( 
    IWMPPlayer4 * This,
    /* [in] */ VARIANT_BOOL bEnabled);


void __RPC_STUB IWMPPlayer4_put_stretchToFit_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE IWMPPlayer4_get_windowlessVideo_Proxy( 
    IWMPPlayer4 * This,
    /* [retval][out] */ VARIANT_BOOL *pbEnabled);


void __RPC_STUB IWMPPlayer4_get_windowlessVideo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propput][id] */ HRESULT STDMETHODCALLTYPE IWMPPlayer4_put_windowlessVideo_Proxy( 
    IWMPPlayer4 * This,
    /* [in] */ VARIANT_BOOL bEnabled);


void __RPC_STUB IWMPPlayer4_put_windowlessVideo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE IWMPPlayer4_get_isRemote_Proxy( 
    IWMPPlayer4 * This,
    /* [retval][out] */ VARIANT_BOOL *pvarfIsRemote);


void __RPC_STUB IWMPPlayer4_get_isRemote_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE IWMPPlayer4_get_playerApplication_Proxy( 
    IWMPPlayer4 * This,
    /* [retval][out] */ IWMPPlayerApplication **ppIWMPPlayerApplication);


void __RPC_STUB IWMPPlayer4_get_playerApplication_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IWMPPlayer4_openPlayer_Proxy( 
    IWMPPlayer4 * This,
    /* [in] */ BSTR bstrURL);


void __RPC_STUB IWMPPlayer4_openPlayer_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IWMPPlayer4_INTERFACE_DEFINED__ */


#ifndef __IWMPPlayerServices_INTERFACE_DEFINED__
#define __IWMPPlayerServices_INTERFACE_DEFINED__

/* interface IWMPPlayerServices */
/* [unique][helpstring][uuid][object] */ 


EXTERN_C const IID IID_IWMPPlayerServices;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("1D01FBDB-ADE2-4c8d-9842-C190B95C3306")
    IWMPPlayerServices : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE activateUIPlugin( 
            /* [in] */ BSTR bstrPlugin) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE setTaskPane( 
            /* [in] */ BSTR bstrTaskPane) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE setTaskPaneURL( 
            /* [in] */ BSTR bstrTaskPane,
            /* [in] */ BSTR bstrURL,
            /* [in] */ BSTR bstrFriendlyName) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IWMPPlayerServicesVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IWMPPlayerServices * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IWMPPlayerServices * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IWMPPlayerServices * This);
        
        HRESULT ( STDMETHODCALLTYPE *activateUIPlugin )( 
            IWMPPlayerServices * This,
            /* [in] */ BSTR bstrPlugin);
        
        HRESULT ( STDMETHODCALLTYPE *setTaskPane )( 
            IWMPPlayerServices * This,
            /* [in] */ BSTR bstrTaskPane);
        
        HRESULT ( STDMETHODCALLTYPE *setTaskPaneURL )( 
            IWMPPlayerServices * This,
            /* [in] */ BSTR bstrTaskPane,
            /* [in] */ BSTR bstrURL,
            /* [in] */ BSTR bstrFriendlyName);
        
        END_INTERFACE
    } IWMPPlayerServicesVtbl;

    interface IWMPPlayerServices
    {
        CONST_VTBL struct IWMPPlayerServicesVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IWMPPlayerServices_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IWMPPlayerServices_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IWMPPlayerServices_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IWMPPlayerServices_activateUIPlugin(This,bstrPlugin)	\
    (This)->lpVtbl -> activateUIPlugin(This,bstrPlugin)

#define IWMPPlayerServices_setTaskPane(This,bstrTaskPane)	\
    (This)->lpVtbl -> setTaskPane(This,bstrTaskPane)

#define IWMPPlayerServices_setTaskPaneURL(This,bstrTaskPane,bstrURL,bstrFriendlyName)	\
    (This)->lpVtbl -> setTaskPaneURL(This,bstrTaskPane,bstrURL,bstrFriendlyName)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE IWMPPlayerServices_activateUIPlugin_Proxy( 
    IWMPPlayerServices * This,
    /* [in] */ BSTR bstrPlugin);


void __RPC_STUB IWMPPlayerServices_activateUIPlugin_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IWMPPlayerServices_setTaskPane_Proxy( 
    IWMPPlayerServices * This,
    /* [in] */ BSTR bstrTaskPane);


void __RPC_STUB IWMPPlayerServices_setTaskPane_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IWMPPlayerServices_setTaskPaneURL_Proxy( 
    IWMPPlayerServices * This,
    /* [in] */ BSTR bstrTaskPane,
    /* [in] */ BSTR bstrURL,
    /* [in] */ BSTR bstrFriendlyName);


void __RPC_STUB IWMPPlayerServices_setTaskPaneURL_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IWMPPlayerServices_INTERFACE_DEFINED__ */


/* interface __MIDL_itf_wmp_0296 */
/* [local] */ 

typedef /* [public][helpstring] */ 
enum WMPSyncState
    {	wmpssUnknown	= 0,
	wmpssSynchronizing	= wmpssUnknown + 1,
	wmpssStopped	= wmpssSynchronizing + 1,
	wmpssLast	= wmpssStopped + 1
    } 	WMPSyncState;

typedef /* [public][helpstring] */ 
enum WMPDeviceStatus
    {	wmpdsUnknown	= 0,
	wmpdsPartnershipExists	= wmpdsUnknown + 1,
	wmpdsPartnershipDeclined	= wmpdsPartnershipExists + 1,
	wmpdsPartnershipAnother	= wmpdsPartnershipDeclined + 1,
	wmpdsManualDevice	= wmpdsPartnershipAnother + 1,
	wmpdsNewDevice	= wmpdsManualDevice + 1,
	wmpdsLast	= wmpdsNewDevice + 1
    } 	WMPDeviceStatus;



extern RPC_IF_HANDLE __MIDL_itf_wmp_0296_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_wmp_0296_v0_0_s_ifspec;

#ifndef __IWMPSyncDevice_INTERFACE_DEFINED__
#define __IWMPSyncDevice_INTERFACE_DEFINED__

/* interface IWMPSyncDevice */
/* [oleautomation][unique][helpstring][uuid][object] */ 


EXTERN_C const IID IID_IWMPSyncDevice;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("82A2986C-0293-4fd0-B279-B21B86C058BE")
    IWMPSyncDevice : public IUnknown
    {
    public:
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_friendlyName( 
            /* [retval][out] */ BSTR *pbstrName) = 0;
        
        virtual /* [propput] */ HRESULT STDMETHODCALLTYPE put_friendlyName( 
            /* [in] */ BSTR bstrName) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_deviceName( 
            /* [retval][out] */ BSTR *pbstrName) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_deviceId( 
            /* [retval][out] */ BSTR *pbstrDeviceId) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_partnershipIndex( 
            /* [retval][out] */ long *plIndex) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_connected( 
            /* [retval][out] */ VARIANT_BOOL *pvbConnected) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_status( 
            /* [retval][out] */ WMPDeviceStatus *pwmpds) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_syncState( 
            /* [retval][out] */ WMPSyncState *pwmpss) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_progress( 
            /* [retval][out] */ long *plProgress) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE getItemInfo( 
            /* [in] */ BSTR bstrItemName,
            /* [retval][out] */ BSTR *pbstrVal) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE createPartnership( 
            /* [in] */ VARIANT_BOOL vbShowUI) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE deletePartnership( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE start( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE stop( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE showSettings( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE isIdentical( 
            /* [in] */ IWMPSyncDevice *pDevice,
            /* [retval][out] */ VARIANT_BOOL *pvbool) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IWMPSyncDeviceVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IWMPSyncDevice * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IWMPSyncDevice * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IWMPSyncDevice * This);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_friendlyName )( 
            IWMPSyncDevice * This,
            /* [retval][out] */ BSTR *pbstrName);
        
        /* [propput] */ HRESULT ( STDMETHODCALLTYPE *put_friendlyName )( 
            IWMPSyncDevice * This,
            /* [in] */ BSTR bstrName);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_deviceName )( 
            IWMPSyncDevice * This,
            /* [retval][out] */ BSTR *pbstrName);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_deviceId )( 
            IWMPSyncDevice * This,
            /* [retval][out] */ BSTR *pbstrDeviceId);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_partnershipIndex )( 
            IWMPSyncDevice * This,
            /* [retval][out] */ long *plIndex);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_connected )( 
            IWMPSyncDevice * This,
            /* [retval][out] */ VARIANT_BOOL *pvbConnected);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_status )( 
            IWMPSyncDevice * This,
            /* [retval][out] */ WMPDeviceStatus *pwmpds);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_syncState )( 
            IWMPSyncDevice * This,
            /* [retval][out] */ WMPSyncState *pwmpss);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_progress )( 
            IWMPSyncDevice * This,
            /* [retval][out] */ long *plProgress);
        
        HRESULT ( STDMETHODCALLTYPE *getItemInfo )( 
            IWMPSyncDevice * This,
            /* [in] */ BSTR bstrItemName,
            /* [retval][out] */ BSTR *pbstrVal);
        
        HRESULT ( STDMETHODCALLTYPE *createPartnership )( 
            IWMPSyncDevice * This,
            /* [in] */ VARIANT_BOOL vbShowUI);
        
        HRESULT ( STDMETHODCALLTYPE *deletePartnership )( 
            IWMPSyncDevice * This);
        
        HRESULT ( STDMETHODCALLTYPE *start )( 
            IWMPSyncDevice * This);
        
        HRESULT ( STDMETHODCALLTYPE *stop )( 
            IWMPSyncDevice * This);
        
        HRESULT ( STDMETHODCALLTYPE *showSettings )( 
            IWMPSyncDevice * This);
        
        HRESULT ( STDMETHODCALLTYPE *isIdentical )( 
            IWMPSyncDevice * This,
            /* [in] */ IWMPSyncDevice *pDevice,
            /* [retval][out] */ VARIANT_BOOL *pvbool);
        
        END_INTERFACE
    } IWMPSyncDeviceVtbl;

    interface IWMPSyncDevice
    {
        CONST_VTBL struct IWMPSyncDeviceVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IWMPSyncDevice_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IWMPSyncDevice_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IWMPSyncDevice_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IWMPSyncDevice_get_friendlyName(This,pbstrName)	\
    (This)->lpVtbl -> get_friendlyName(This,pbstrName)

#define IWMPSyncDevice_put_friendlyName(This,bstrName)	\
    (This)->lpVtbl -> put_friendlyName(This,bstrName)

#define IWMPSyncDevice_get_deviceName(This,pbstrName)	\
    (This)->lpVtbl -> get_deviceName(This,pbstrName)

#define IWMPSyncDevice_get_deviceId(This,pbstrDeviceId)	\
    (This)->lpVtbl -> get_deviceId(This,pbstrDeviceId)

#define IWMPSyncDevice_get_partnershipIndex(This,plIndex)	\
    (This)->lpVtbl -> get_partnershipIndex(This,plIndex)

#define IWMPSyncDevice_get_connected(This,pvbConnected)	\
    (This)->lpVtbl -> get_connected(This,pvbConnected)

#define IWMPSyncDevice_get_status(This,pwmpds)	\
    (This)->lpVtbl -> get_status(This,pwmpds)

#define IWMPSyncDevice_get_syncState(This,pwmpss)	\
    (This)->lpVtbl -> get_syncState(This,pwmpss)

#define IWMPSyncDevice_get_progress(This,plProgress)	\
    (This)->lpVtbl -> get_progress(This,plProgress)

#define IWMPSyncDevice_getItemInfo(This,bstrItemName,pbstrVal)	\
    (This)->lpVtbl -> getItemInfo(This,bstrItemName,pbstrVal)

#define IWMPSyncDevice_createPartnership(This,vbShowUI)	\
    (This)->lpVtbl -> createPartnership(This,vbShowUI)

#define IWMPSyncDevice_deletePartnership(This)	\
    (This)->lpVtbl -> deletePartnership(This)

#define IWMPSyncDevice_start(This)	\
    (This)->lpVtbl -> start(This)

#define IWMPSyncDevice_stop(This)	\
    (This)->lpVtbl -> stop(This)

#define IWMPSyncDevice_showSettings(This)	\
    (This)->lpVtbl -> showSettings(This)

#define IWMPSyncDevice_isIdentical(This,pDevice,pvbool)	\
    (This)->lpVtbl -> isIdentical(This,pDevice,pvbool)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [propget] */ HRESULT STDMETHODCALLTYPE IWMPSyncDevice_get_friendlyName_Proxy( 
    IWMPSyncDevice * This,
    /* [retval][out] */ BSTR *pbstrName);


void __RPC_STUB IWMPSyncDevice_get_friendlyName_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [propput] */ HRESULT STDMETHODCALLTYPE IWMPSyncDevice_put_friendlyName_Proxy( 
    IWMPSyncDevice * This,
    /* [in] */ BSTR bstrName);


void __RPC_STUB IWMPSyncDevice_put_friendlyName_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [propget] */ HRESULT STDMETHODCALLTYPE IWMPSyncDevice_get_deviceName_Proxy( 
    IWMPSyncDevice * This,
    /* [retval][out] */ BSTR *pbstrName);


void __RPC_STUB IWMPSyncDevice_get_deviceName_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [propget] */ HRESULT STDMETHODCALLTYPE IWMPSyncDevice_get_deviceId_Proxy( 
    IWMPSyncDevice * This,
    /* [retval][out] */ BSTR *pbstrDeviceId);


void __RPC_STUB IWMPSyncDevice_get_deviceId_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [propget] */ HRESULT STDMETHODCALLTYPE IWMPSyncDevice_get_partnershipIndex_Proxy( 
    IWMPSyncDevice * This,
    /* [retval][out] */ long *plIndex);


void __RPC_STUB IWMPSyncDevice_get_partnershipIndex_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [propget] */ HRESULT STDMETHODCALLTYPE IWMPSyncDevice_get_connected_Proxy( 
    IWMPSyncDevice * This,
    /* [retval][out] */ VARIANT_BOOL *pvbConnected);


void __RPC_STUB IWMPSyncDevice_get_connected_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [propget] */ HRESULT STDMETHODCALLTYPE IWMPSyncDevice_get_status_Proxy( 
    IWMPSyncDevice * This,
    /* [retval][out] */ WMPDeviceStatus *pwmpds);


void __RPC_STUB IWMPSyncDevice_get_status_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [propget] */ HRESULT STDMETHODCALLTYPE IWMPSyncDevice_get_syncState_Proxy( 
    IWMPSyncDevice * This,
    /* [retval][out] */ WMPSyncState *pwmpss);


void __RPC_STUB IWMPSyncDevice_get_syncState_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [propget] */ HRESULT STDMETHODCALLTYPE IWMPSyncDevice_get_progress_Proxy( 
    IWMPSyncDevice * This,
    /* [retval][out] */ long *plProgress);


void __RPC_STUB IWMPSyncDevice_get_progress_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IWMPSyncDevice_getItemInfo_Proxy( 
    IWMPSyncDevice * This,
    /* [in] */ BSTR bstrItemName,
    /* [retval][out] */ BSTR *pbstrVal);


void __RPC_STUB IWMPSyncDevice_getItemInfo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IWMPSyncDevice_createPartnership_Proxy( 
    IWMPSyncDevice * This,
    /* [in] */ VARIANT_BOOL vbShowUI);


void __RPC_STUB IWMPSyncDevice_createPartnership_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IWMPSyncDevice_deletePartnership_Proxy( 
    IWMPSyncDevice * This);


void __RPC_STUB IWMPSyncDevice_deletePartnership_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IWMPSyncDevice_start_Proxy( 
    IWMPSyncDevice * This);


void __RPC_STUB IWMPSyncDevice_start_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IWMPSyncDevice_stop_Proxy( 
    IWMPSyncDevice * This);


void __RPC_STUB IWMPSyncDevice_stop_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IWMPSyncDevice_showSettings_Proxy( 
    IWMPSyncDevice * This);


void __RPC_STUB IWMPSyncDevice_showSettings_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IWMPSyncDevice_isIdentical_Proxy( 
    IWMPSyncDevice * This,
    /* [in] */ IWMPSyncDevice *pDevice,
    /* [retval][out] */ VARIANT_BOOL *pvbool);


void __RPC_STUB IWMPSyncDevice_isIdentical_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IWMPSyncDevice_INTERFACE_DEFINED__ */


#ifndef __IWMPSyncServices_INTERFACE_DEFINED__
#define __IWMPSyncServices_INTERFACE_DEFINED__

/* interface IWMPSyncServices */
/* [oleautomation][unique][helpstring][uuid][object] */ 


EXTERN_C const IID IID_IWMPSyncServices;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("8B5050FF-E0A4-4808-B3A8-893A9E1ED894")
    IWMPSyncServices : public IUnknown
    {
    public:
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_deviceCount( 
            /* [retval][out] */ long *plCount) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE getDevice( 
            /* [in] */ long lIndex,
            /* [retval][out] */ IWMPSyncDevice **ppDevice) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IWMPSyncServicesVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IWMPSyncServices * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IWMPSyncServices * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IWMPSyncServices * This);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_deviceCount )( 
            IWMPSyncServices * This,
            /* [retval][out] */ long *plCount);
        
        HRESULT ( STDMETHODCALLTYPE *getDevice )( 
            IWMPSyncServices * This,
            /* [in] */ long lIndex,
            /* [retval][out] */ IWMPSyncDevice **ppDevice);
        
        END_INTERFACE
    } IWMPSyncServicesVtbl;

    interface IWMPSyncServices
    {
        CONST_VTBL struct IWMPSyncServicesVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IWMPSyncServices_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IWMPSyncServices_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IWMPSyncServices_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IWMPSyncServices_get_deviceCount(This,plCount)	\
    (This)->lpVtbl -> get_deviceCount(This,plCount)

#define IWMPSyncServices_getDevice(This,lIndex,ppDevice)	\
    (This)->lpVtbl -> getDevice(This,lIndex,ppDevice)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [propget] */ HRESULT STDMETHODCALLTYPE IWMPSyncServices_get_deviceCount_Proxy( 
    IWMPSyncServices * This,
    /* [retval][out] */ long *plCount);


void __RPC_STUB IWMPSyncServices_get_deviceCount_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IWMPSyncServices_getDevice_Proxy( 
    IWMPSyncServices * This,
    /* [in] */ long lIndex,
    /* [retval][out] */ IWMPSyncDevice **ppDevice);


void __RPC_STUB IWMPSyncServices_getDevice_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IWMPSyncServices_INTERFACE_DEFINED__ */


#ifndef __IWMPPlayerServices2_INTERFACE_DEFINED__
#define __IWMPPlayerServices2_INTERFACE_DEFINED__

/* interface IWMPPlayerServices2 */
/* [unique][helpstring][uuid][object] */ 


EXTERN_C const IID IID_IWMPPlayerServices2;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("1BB1592F-F040-418a-9F71-17C7512B4D70")
    IWMPPlayerServices2 : public IWMPPlayerServices
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE setBackgroundProcessingPriority( 
            /* [in] */ BSTR bstrPriority) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IWMPPlayerServices2Vtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IWMPPlayerServices2 * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IWMPPlayerServices2 * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IWMPPlayerServices2 * This);
        
        HRESULT ( STDMETHODCALLTYPE *activateUIPlugin )( 
            IWMPPlayerServices2 * This,
            /* [in] */ BSTR bstrPlugin);
        
        HRESULT ( STDMETHODCALLTYPE *setTaskPane )( 
            IWMPPlayerServices2 * This,
            /* [in] */ BSTR bstrTaskPane);
        
        HRESULT ( STDMETHODCALLTYPE *setTaskPaneURL )( 
            IWMPPlayerServices2 * This,
            /* [in] */ BSTR bstrTaskPane,
            /* [in] */ BSTR bstrURL,
            /* [in] */ BSTR bstrFriendlyName);
        
        HRESULT ( STDMETHODCALLTYPE *setBackgroundProcessingPriority )( 
            IWMPPlayerServices2 * This,
            /* [in] */ BSTR bstrPriority);
        
        END_INTERFACE
    } IWMPPlayerServices2Vtbl;

    interface IWMPPlayerServices2
    {
        CONST_VTBL struct IWMPPlayerServices2Vtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IWMPPlayerServices2_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IWMPPlayerServices2_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IWMPPlayerServices2_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IWMPPlayerServices2_activateUIPlugin(This,bstrPlugin)	\
    (This)->lpVtbl -> activateUIPlugin(This,bstrPlugin)

#define IWMPPlayerServices2_setTaskPane(This,bstrTaskPane)	\
    (This)->lpVtbl -> setTaskPane(This,bstrTaskPane)

#define IWMPPlayerServices2_setTaskPaneURL(This,bstrTaskPane,bstrURL,bstrFriendlyName)	\
    (This)->lpVtbl -> setTaskPaneURL(This,bstrTaskPane,bstrURL,bstrFriendlyName)


#define IWMPPlayerServices2_setBackgroundProcessingPriority(This,bstrPriority)	\
    (This)->lpVtbl -> setBackgroundProcessingPriority(This,bstrPriority)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE IWMPPlayerServices2_setBackgroundProcessingPriority_Proxy( 
    IWMPPlayerServices2 * This,
    /* [in] */ BSTR bstrPriority);


void __RPC_STUB IWMPPlayerServices2_setBackgroundProcessingPriority_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IWMPPlayerServices2_INTERFACE_DEFINED__ */


/* interface __MIDL_itf_wmp_0299 */
/* [local] */ 

struct DECLSPEC_UUID("6BF52A50-394A-11d3-B153-00C04F79FAA6") WMPLib;
struct DECLSPEC_UUID("DF333473-2CF7-4be2-907F-9AAD5661364F") WMPRemoteMediaServices;


extern RPC_IF_HANDLE __MIDL_itf_wmp_0299_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_wmp_0299_v0_0_s_ifspec;


#ifndef __WMPLib_LIBRARY_DEFINED__
#define __WMPLib_LIBRARY_DEFINED__

/* library WMPLib */
/* [helpstring][version][uuid] */ 


EXTERN_C const IID LIBID_WMPLib;

#ifndef __IWMPEvents_INTERFACE_DEFINED__
#define __IWMPEvents_INTERFACE_DEFINED__

/* interface IWMPEvents */
/* [unique][helpstring][uuid][object] */ 


EXTERN_C const IID IID_IWMPEvents;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("19A6627B-DA9E-47c1-BB23-00B5E668236A")
    IWMPEvents : public IUnknown
    {
    public:
        virtual /* [helpstring][id] */ void STDMETHODCALLTYPE OpenStateChange( 
            /* [in] */ long NewState) = 0;
        
        virtual /* [helpstring][id] */ void STDMETHODCALLTYPE PlayStateChange( 
            /* [in] */ long NewState) = 0;
        
        virtual /* [helpstring][id] */ void STDMETHODCALLTYPE AudioLanguageChange( 
            /* [in] */ long LangID) = 0;
        
        virtual /* [helpstring][id] */ void STDMETHODCALLTYPE StatusChange( void) = 0;
        
        virtual /* [helpstring][id] */ void STDMETHODCALLTYPE ScriptCommand( 
            /* [in] */ BSTR scType,
            /* [in] */ BSTR Param) = 0;
        
        virtual /* [helpstring][id] */ void STDMETHODCALLTYPE NewStream( void) = 0;
        
        virtual /* [helpstring][id] */ void STDMETHODCALLTYPE Disconnect( 
            /* [in] */ long Result) = 0;
        
        virtual /* [helpstring][id] */ void STDMETHODCALLTYPE Buffering( 
            /* [in] */ VARIANT_BOOL Start) = 0;
        
        virtual /* [helpstring][id] */ void STDMETHODCALLTYPE Error( void) = 0;
        
        virtual /* [helpstring][id] */ void STDMETHODCALLTYPE Warning( 
            /* [in] */ long WarningType,
            /* [in] */ long Param,
            /* [in] */ BSTR Description) = 0;
        
        virtual /* [helpstring][id] */ void STDMETHODCALLTYPE EndOfStream( 
            /* [in] */ long Result) = 0;
        
        virtual /* [helpstring][id] */ void STDMETHODCALLTYPE PositionChange( 
            /* [in] */ double oldPosition,
            /* [in] */ double newPosition) = 0;
        
        virtual /* [helpstring][id] */ void STDMETHODCALLTYPE MarkerHit( 
            /* [in] */ long MarkerNum) = 0;
        
        virtual /* [helpstring][id] */ void STDMETHODCALLTYPE DurationUnitChange( 
            /* [in] */ long NewDurationUnit) = 0;
        
        virtual /* [helpstring][id] */ void STDMETHODCALLTYPE CdromMediaChange( 
            /* [in] */ long CdromNum) = 0;
        
        virtual /* [helpstring][id] */ void STDMETHODCALLTYPE PlaylistChange( 
            /* [in] */ IDispatch *Playlist,
            /* [in] */ WMPPlaylistChangeEventType change) = 0;
        
        virtual /* [helpstring][id] */ void STDMETHODCALLTYPE CurrentPlaylistChange( 
            /* [in] */ WMPPlaylistChangeEventType change) = 0;
        
        virtual /* [helpstring][id] */ void STDMETHODCALLTYPE CurrentPlaylistItemAvailable( 
            /* [in] */ BSTR bstrItemName) = 0;
        
        virtual /* [helpstring][id] */ void STDMETHODCALLTYPE MediaChange( 
            /* [in] */ IDispatch *Item) = 0;
        
        virtual /* [helpstring][id] */ void STDMETHODCALLTYPE CurrentMediaItemAvailable( 
            /* [in] */ BSTR bstrItemName) = 0;
        
        virtual /* [helpstring][id] */ void STDMETHODCALLTYPE CurrentItemChange( 
            /* [in] */ IDispatch *pdispMedia) = 0;
        
        virtual /* [helpstring][id] */ void STDMETHODCALLTYPE MediaCollectionChange( void) = 0;
        
        virtual /* [helpstring][id] */ void STDMETHODCALLTYPE MediaCollectionAttributeStringAdded( 
            /* [in] */ BSTR bstrAttribName,
            /* [in] */ BSTR bstrAttribVal) = 0;
        
        virtual /* [helpstring][id] */ void STDMETHODCALLTYPE MediaCollectionAttributeStringRemoved( 
            /* [in] */ BSTR bstrAttribName,
            /* [in] */ BSTR bstrAttribVal) = 0;
        
        virtual /* [helpstring][id] */ void STDMETHODCALLTYPE MediaCollectionAttributeStringChanged( 
            /* [in] */ BSTR bstrAttribName,
            /* [in] */ BSTR bstrOldAttribVal,
            /* [in] */ BSTR bstrNewAttribVal) = 0;
        
        virtual /* [helpstring][id] */ void STDMETHODCALLTYPE PlaylistCollectionChange( void) = 0;
        
        virtual /* [helpstring][id] */ void STDMETHODCALLTYPE PlaylistCollectionPlaylistAdded( 
            /* [in] */ BSTR bstrPlaylistName) = 0;
        
        virtual /* [helpstring][id] */ void STDMETHODCALLTYPE PlaylistCollectionPlaylistRemoved( 
            /* [in] */ BSTR bstrPlaylistName) = 0;
        
        virtual /* [helpstring][id] */ void STDMETHODCALLTYPE PlaylistCollectionPlaylistSetAsDeleted( 
            /* [in] */ BSTR bstrPlaylistName,
            /* [in] */ VARIANT_BOOL varfIsDeleted) = 0;
        
        virtual /* [helpstring][id] */ void STDMETHODCALLTYPE ModeChange( 
            /* [in] */ BSTR ModeName,
            /* [in] */ VARIANT_BOOL NewValue) = 0;
        
        virtual /* [helpstring][id] */ void STDMETHODCALLTYPE MediaError( 
            /* [in] */ IDispatch *pMediaObject) = 0;
        
        virtual /* [helpstring][id] */ void STDMETHODCALLTYPE OpenPlaylistSwitch( 
            /* [in] */ IDispatch *pItem) = 0;
        
        virtual /* [helpstring][id] */ void STDMETHODCALLTYPE DomainChange( 
            /* [in] */ BSTR strDomain) = 0;
        
        virtual /* [helpstring][id] */ void STDMETHODCALLTYPE SwitchedToPlayerApplication( void) = 0;
        
        virtual /* [helpstring][id] */ void STDMETHODCALLTYPE SwitchedToControl( void) = 0;
        
        virtual /* [helpstring][id] */ void STDMETHODCALLTYPE PlayerDockedStateChange( void) = 0;
        
        virtual /* [helpstring][id] */ void STDMETHODCALLTYPE PlayerReconnect( void) = 0;
        
        virtual /* [helpstring][id] */ void STDMETHODCALLTYPE Click( 
            /* [in] */ short nButton,
            /* [in] */ short nShiftState,
            /* [in] */ long fX,
            /* [in] */ long fY) = 0;
        
        virtual /* [helpstring][id] */ void STDMETHODCALLTYPE DoubleClick( 
            /* [in] */ short nButton,
            /* [in] */ short nShiftState,
            /* [in] */ long fX,
            /* [in] */ long fY) = 0;
        
        virtual /* [helpstring][id] */ void STDMETHODCALLTYPE KeyDown( 
            /* [in] */ short nKeyCode,
            /* [in] */ short nShiftState) = 0;
        
        virtual /* [helpstring][id] */ void STDMETHODCALLTYPE KeyPress( 
            /* [in] */ short nKeyAscii) = 0;
        
        virtual /* [helpstring][id] */ void STDMETHODCALLTYPE KeyUp( 
            /* [in] */ short nKeyCode,
            /* [in] */ short nShiftState) = 0;
        
        virtual /* [helpstring][id] */ void STDMETHODCALLTYPE MouseDown( 
            /* [in] */ short nButton,
            /* [in] */ short nShiftState,
            /* [in] */ long fX,
            /* [in] */ long fY) = 0;
        
        virtual /* [helpstring][id] */ void STDMETHODCALLTYPE MouseMove( 
            /* [in] */ short nButton,
            /* [in] */ short nShiftState,
            /* [in] */ long fX,
            /* [in] */ long fY) = 0;
        
        virtual /* [helpstring][id] */ void STDMETHODCALLTYPE MouseUp( 
            /* [in] */ short nButton,
            /* [in] */ short nShiftState,
            /* [in] */ long fX,
            /* [in] */ long fY) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IWMPEventsVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IWMPEvents * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IWMPEvents * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IWMPEvents * This);
        
        /* [helpstring][id] */ void ( STDMETHODCALLTYPE *OpenStateChange )( 
            IWMPEvents * This,
            /* [in] */ long NewState);
        
        /* [helpstring][id] */ void ( STDMETHODCALLTYPE *PlayStateChange )( 
            IWMPEvents * This,
            /* [in] */ long NewState);
        
        /* [helpstring][id] */ void ( STDMETHODCALLTYPE *AudioLanguageChange )( 
            IWMPEvents * This,
            /* [in] */ long LangID);
        
        /* [helpstring][id] */ void ( STDMETHODCALLTYPE *StatusChange )( 
            IWMPEvents * This);
        
        /* [helpstring][id] */ void ( STDMETHODCALLTYPE *ScriptCommand )( 
            IWMPEvents * This,
            /* [in] */ BSTR scType,
            /* [in] */ BSTR Param);
        
        /* [helpstring][id] */ void ( STDMETHODCALLTYPE *NewStream )( 
            IWMPEvents * This);
        
        /* [helpstring][id] */ void ( STDMETHODCALLTYPE *Disconnect )( 
            IWMPEvents * This,
            /* [in] */ long Result);
        
        /* [helpstring][id] */ void ( STDMETHODCALLTYPE *Buffering )( 
            IWMPEvents * This,
            /* [in] */ VARIANT_BOOL Start);
        
        /* [helpstring][id] */ void ( STDMETHODCALLTYPE *Error )( 
            IWMPEvents * This);
        
        /* [helpstring][id] */ void ( STDMETHODCALLTYPE *Warning )( 
            IWMPEvents * This,
            /* [in] */ long WarningType,
            /* [in] */ long Param,
            /* [in] */ BSTR Description);
        
        /* [helpstring][id] */ void ( STDMETHODCALLTYPE *EndOfStream )( 
            IWMPEvents * This,
            /* [in] */ long Result);
        
        /* [helpstring][id] */ void ( STDMETHODCALLTYPE *PositionChange )( 
            IWMPEvents * This,
            /* [in] */ double oldPosition,
            /* [in] */ double newPosition);
        
        /* [helpstring][id] */ void ( STDMETHODCALLTYPE *MarkerHit )( 
            IWMPEvents * This,
            /* [in] */ long MarkerNum);
        
        /* [helpstring][id] */ void ( STDMETHODCALLTYPE *DurationUnitChange )( 
            IWMPEvents * This,
            /* [in] */ long NewDurationUnit);
        
        /* [helpstring][id] */ void ( STDMETHODCALLTYPE *CdromMediaChange )( 
            IWMPEvents * This,
            /* [in] */ long CdromNum);
        
        /* [helpstring][id] */ void ( STDMETHODCALLTYPE *PlaylistChange )( 
            IWMPEvents * This,
            /* [in] */ IDispatch *Playlist,
            /* [in] */ WMPPlaylistChangeEventType change);
        
        /* [helpstring][id] */ void ( STDMETHODCALLTYPE *CurrentPlaylistChange )( 
            IWMPEvents * This,
            /* [in] */ WMPPlaylistChangeEventType change);
        
        /* [helpstring][id] */ void ( STDMETHODCALLTYPE *CurrentPlaylistItemAvailable )( 
            IWMPEvents * This,
            /* [in] */ BSTR bstrItemName);
        
        /* [helpstring][id] */ void ( STDMETHODCALLTYPE *MediaChange )( 
            IWMPEvents * This,
            /* [in] */ IDispatch *Item);
        
        /* [helpstring][id] */ void ( STDMETHODCALLTYPE *CurrentMediaItemAvailable )( 
            IWMPEvents * This,
            /* [in] */ BSTR bstrItemName);
        
        /* [helpstring][id] */ void ( STDMETHODCALLTYPE *CurrentItemChange )( 
            IWMPEvents * This,
            /* [in] */ IDispatch *pdispMedia);
        
        /* [helpstring][id] */ void ( STDMETHODCALLTYPE *MediaCollectionChange )( 
            IWMPEvents * This);
        
        /* [helpstring][id] */ void ( STDMETHODCALLTYPE *MediaCollectionAttributeStringAdded )( 
            IWMPEvents * This,
            /* [in] */ BSTR bstrAttribName,
            /* [in] */ BSTR bstrAttribVal);
        
        /* [helpstring][id] */ void ( STDMETHODCALLTYPE *MediaCollectionAttributeStringRemoved )( 
            IWMPEvents * This,
            /* [in] */ BSTR bstrAttribName,
            /* [in] */ BSTR bstrAttribVal);
        
        /* [helpstring][id] */ void ( STDMETHODCALLTYPE *MediaCollectionAttributeStringChanged )( 
            IWMPEvents * This,
            /* [in] */ BSTR bstrAttribName,
            /* [in] */ BSTR bstrOldAttribVal,
            /* [in] */ BSTR bstrNewAttribVal);
        
        /* [helpstring][id] */ void ( STDMETHODCALLTYPE *PlaylistCollectionChange )( 
            IWMPEvents * This);
        
        /* [helpstring][id] */ void ( STDMETHODCALLTYPE *PlaylistCollectionPlaylistAdded )( 
            IWMPEvents * This,
            /* [in] */ BSTR bstrPlaylistName);
        
        /* [helpstring][id] */ void ( STDMETHODCALLTYPE *PlaylistCollectionPlaylistRemoved )( 
            IWMPEvents * This,
            /* [in] */ BSTR bstrPlaylistName);
        
        /* [helpstring][id] */ void ( STDMETHODCALLTYPE *PlaylistCollectionPlaylistSetAsDeleted )( 
            IWMPEvents * This,
            /* [in] */ BSTR bstrPlaylistName,
            /* [in] */ VARIANT_BOOL varfIsDeleted);
        
        /* [helpstring][id] */ void ( STDMETHODCALLTYPE *ModeChange )( 
            IWMPEvents * This,
            /* [in] */ BSTR ModeName,
            /* [in] */ VARIANT_BOOL NewValue);
        
        /* [helpstring][id] */ void ( STDMETHODCALLTYPE *MediaError )( 
            IWMPEvents * This,
            /* [in] */ IDispatch *pMediaObject);
        
        /* [helpstring][id] */ void ( STDMETHODCALLTYPE *OpenPlaylistSwitch )( 
            IWMPEvents * This,
            /* [in] */ IDispatch *pItem);
        
        /* [helpstring][id] */ void ( STDMETHODCALLTYPE *DomainChange )( 
            IWMPEvents * This,
            /* [in] */ BSTR strDomain);
        
        /* [helpstring][id] */ void ( STDMETHODCALLTYPE *SwitchedToPlayerApplication )( 
            IWMPEvents * This);
        
        /* [helpstring][id] */ void ( STDMETHODCALLTYPE *SwitchedToControl )( 
            IWMPEvents * This);
        
        /* [helpstring][id] */ void ( STDMETHODCALLTYPE *PlayerDockedStateChange )( 
            IWMPEvents * This);
        
        /* [helpstring][id] */ void ( STDMETHODCALLTYPE *PlayerReconnect )( 
            IWMPEvents * This);
        
        /* [helpstring][id] */ void ( STDMETHODCALLTYPE *Click )( 
            IWMPEvents * This,
            /* [in] */ short nButton,
            /* [in] */ short nShiftState,
            /* [in] */ long fX,
            /* [in] */ long fY);
        
        /* [helpstring][id] */ void ( STDMETHODCALLTYPE *DoubleClick )( 
            IWMPEvents * This,
            /* [in] */ short nButton,
            /* [in] */ short nShiftState,
            /* [in] */ long fX,
            /* [in] */ long fY);
        
        /* [helpstring][id] */ void ( STDMETHODCALLTYPE *KeyDown )( 
            IWMPEvents * This,
            /* [in] */ short nKeyCode,
            /* [in] */ short nShiftState);
        
        /* [helpstring][id] */ void ( STDMETHODCALLTYPE *KeyPress )( 
            IWMPEvents * This,
            /* [in] */ short nKeyAscii);
        
        /* [helpstring][id] */ void ( STDMETHODCALLTYPE *KeyUp )( 
            IWMPEvents * This,
            /* [in] */ short nKeyCode,
            /* [in] */ short nShiftState);
        
        /* [helpstring][id] */ void ( STDMETHODCALLTYPE *MouseDown )( 
            IWMPEvents * This,
            /* [in] */ short nButton,
            /* [in] */ short nShiftState,
            /* [in] */ long fX,
            /* [in] */ long fY);
        
        /* [helpstring][id] */ void ( STDMETHODCALLTYPE *MouseMove )( 
            IWMPEvents * This,
            /* [in] */ short nButton,
            /* [in] */ short nShiftState,
            /* [in] */ long fX,
            /* [in] */ long fY);
        
        /* [helpstring][id] */ void ( STDMETHODCALLTYPE *MouseUp )( 
            IWMPEvents * This,
            /* [in] */ short nButton,
            /* [in] */ short nShiftState,
            /* [in] */ long fX,
            /* [in] */ long fY);
        
        END_INTERFACE
    } IWMPEventsVtbl;

    interface IWMPEvents
    {
        CONST_VTBL struct IWMPEventsVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IWMPEvents_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IWMPEvents_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IWMPEvents_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IWMPEvents_OpenStateChange(This,NewState)	\
    (This)->lpVtbl -> OpenStateChange(This,NewState)

#define IWMPEvents_PlayStateChange(This,NewState)	\
    (This)->lpVtbl -> PlayStateChange(This,NewState)

#define IWMPEvents_AudioLanguageChange(This,LangID)	\
    (This)->lpVtbl -> AudioLanguageChange(This,LangID)

#define IWMPEvents_StatusChange(This)	\
    (This)->lpVtbl -> StatusChange(This)

#define IWMPEvents_ScriptCommand(This,scType,Param)	\
    (This)->lpVtbl -> ScriptCommand(This,scType,Param)

#define IWMPEvents_NewStream(This)	\
    (This)->lpVtbl -> NewStream(This)

#define IWMPEvents_Disconnect(This,Result)	\
    (This)->lpVtbl -> Disconnect(This,Result)

#define IWMPEvents_Buffering(This,Start)	\
    (This)->lpVtbl -> Buffering(This,Start)

#define IWMPEvents_Error(This)	\
    (This)->lpVtbl -> Error(This)

#define IWMPEvents_Warning(This,WarningType,Param,Description)	\
    (This)->lpVtbl -> Warning(This,WarningType,Param,Description)

#define IWMPEvents_EndOfStream(This,Result)	\
    (This)->lpVtbl -> EndOfStream(This,Result)

#define IWMPEvents_PositionChange(This,oldPosition,newPosition)	\
    (This)->lpVtbl -> PositionChange(This,oldPosition,newPosition)

#define IWMPEvents_MarkerHit(This,MarkerNum)	\
    (This)->lpVtbl -> MarkerHit(This,MarkerNum)

#define IWMPEvents_DurationUnitChange(This,NewDurationUnit)	\
    (This)->lpVtbl -> DurationUnitChange(This,NewDurationUnit)

#define IWMPEvents_CdromMediaChange(This,CdromNum)	\
    (This)->lpVtbl -> CdromMediaChange(This,CdromNum)

#define IWMPEvents_PlaylistChange(This,Playlist,change)	\
    (This)->lpVtbl -> PlaylistChange(This,Playlist,change)

#define IWMPEvents_CurrentPlaylistChange(This,change)	\
    (This)->lpVtbl -> CurrentPlaylistChange(This,change)

#define IWMPEvents_CurrentPlaylistItemAvailable(This,bstrItemName)	\
    (This)->lpVtbl -> CurrentPlaylistItemAvailable(This,bstrItemName)

#define IWMPEvents_MediaChange(This,Item)	\
    (This)->lpVtbl -> MediaChange(This,Item)

#define IWMPEvents_CurrentMediaItemAvailable(This,bstrItemName)	\
    (This)->lpVtbl -> CurrentMediaItemAvailable(This,bstrItemName)

#define IWMPEvents_CurrentItemChange(This,pdispMedia)	\
    (This)->lpVtbl -> CurrentItemChange(This,pdispMedia)

#define IWMPEvents_MediaCollectionChange(This)	\
    (This)->lpVtbl -> MediaCollectionChange(This)

#define IWMPEvents_MediaCollectionAttributeStringAdded(This,bstrAttribName,bstrAttribVal)	\
    (This)->lpVtbl -> MediaCollectionAttributeStringAdded(This,bstrAttribName,bstrAttribVal)

#define IWMPEvents_MediaCollectionAttributeStringRemoved(This,bstrAttribName,bstrAttribVal)	\
    (This)->lpVtbl -> MediaCollectionAttributeStringRemoved(This,bstrAttribName,bstrAttribVal)

#define IWMPEvents_MediaCollectionAttributeStringChanged(This,bstrAttribName,bstrOldAttribVal,bstrNewAttribVal)	\
    (This)->lpVtbl -> MediaCollectionAttributeStringChanged(This,bstrAttribName,bstrOldAttribVal,bstrNewAttribVal)

#define IWMPEvents_PlaylistCollectionChange(This)	\
    (This)->lpVtbl -> PlaylistCollectionChange(This)

#define IWMPEvents_PlaylistCollectionPlaylistAdded(This,bstrPlaylistName)	\
    (This)->lpVtbl -> PlaylistCollectionPlaylistAdded(This,bstrPlaylistName)

#define IWMPEvents_PlaylistCollectionPlaylistRemoved(This,bstrPlaylistName)	\
    (This)->lpVtbl -> PlaylistCollectionPlaylistRemoved(This,bstrPlaylistName)

#define IWMPEvents_PlaylistCollectionPlaylistSetAsDeleted(This,bstrPlaylistName,varfIsDeleted)	\
    (This)->lpVtbl -> PlaylistCollectionPlaylistSetAsDeleted(This,bstrPlaylistName,varfIsDeleted)

#define IWMPEvents_ModeChange(This,ModeName,NewValue)	\
    (This)->lpVtbl -> ModeChange(This,ModeName,NewValue)

#define IWMPEvents_MediaError(This,pMediaObject)	\
    (This)->lpVtbl -> MediaError(This,pMediaObject)

#define IWMPEvents_OpenPlaylistSwitch(This,pItem)	\
    (This)->lpVtbl -> OpenPlaylistSwitch(This,pItem)

#define IWMPEvents_DomainChange(This,strDomain)	\
    (This)->lpVtbl -> DomainChange(This,strDomain)

#define IWMPEvents_SwitchedToPlayerApplication(This)	\
    (This)->lpVtbl -> SwitchedToPlayerApplication(This)

#define IWMPEvents_SwitchedToControl(This)	\
    (This)->lpVtbl -> SwitchedToControl(This)

#define IWMPEvents_PlayerDockedStateChange(This)	\
    (This)->lpVtbl -> PlayerDockedStateChange(This)

#define IWMPEvents_PlayerReconnect(This)	\
    (This)->lpVtbl -> PlayerReconnect(This)

#define IWMPEvents_Click(This,nButton,nShiftState,fX,fY)	\
    (This)->lpVtbl -> Click(This,nButton,nShiftState,fX,fY)

#define IWMPEvents_DoubleClick(This,nButton,nShiftState,fX,fY)	\
    (This)->lpVtbl -> DoubleClick(This,nButton,nShiftState,fX,fY)

#define IWMPEvents_KeyDown(This,nKeyCode,nShiftState)	\
    (This)->lpVtbl -> KeyDown(This,nKeyCode,nShiftState)

#define IWMPEvents_KeyPress(This,nKeyAscii)	\
    (This)->lpVtbl -> KeyPress(This,nKeyAscii)

#define IWMPEvents_KeyUp(This,nKeyCode,nShiftState)	\
    (This)->lpVtbl -> KeyUp(This,nKeyCode,nShiftState)

#define IWMPEvents_MouseDown(This,nButton,nShiftState,fX,fY)	\
    (This)->lpVtbl -> MouseDown(This,nButton,nShiftState,fX,fY)

#define IWMPEvents_MouseMove(This,nButton,nShiftState,fX,fY)	\
    (This)->lpVtbl -> MouseMove(This,nButton,nShiftState,fX,fY)

#define IWMPEvents_MouseUp(This,nButton,nShiftState,fX,fY)	\
    (This)->lpVtbl -> MouseUp(This,nButton,nShiftState,fX,fY)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][id] */ void STDMETHODCALLTYPE IWMPEvents_OpenStateChange_Proxy( 
    IWMPEvents * This,
    /* [in] */ long NewState);


void __RPC_STUB IWMPEvents_OpenStateChange_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ void STDMETHODCALLTYPE IWMPEvents_PlayStateChange_Proxy( 
    IWMPEvents * This,
    /* [in] */ long NewState);


void __RPC_STUB IWMPEvents_PlayStateChange_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ void STDMETHODCALLTYPE IWMPEvents_AudioLanguageChange_Proxy( 
    IWMPEvents * This,
    /* [in] */ long LangID);


void __RPC_STUB IWMPEvents_AudioLanguageChange_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ void STDMETHODCALLTYPE IWMPEvents_StatusChange_Proxy( 
    IWMPEvents * This);


void __RPC_STUB IWMPEvents_StatusChange_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ void STDMETHODCALLTYPE IWMPEvents_ScriptCommand_Proxy( 
    IWMPEvents * This,
    /* [in] */ BSTR scType,
    /* [in] */ BSTR Param);


void __RPC_STUB IWMPEvents_ScriptCommand_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ void STDMETHODCALLTYPE IWMPEvents_NewStream_Proxy( 
    IWMPEvents * This);


void __RPC_STUB IWMPEvents_NewStream_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ void STDMETHODCALLTYPE IWMPEvents_Disconnect_Proxy( 
    IWMPEvents * This,
    /* [in] */ long Result);


void __RPC_STUB IWMPEvents_Disconnect_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ void STDMETHODCALLTYPE IWMPEvents_Buffering_Proxy( 
    IWMPEvents * This,
    /* [in] */ VARIANT_BOOL Start);


void __RPC_STUB IWMPEvents_Buffering_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ void STDMETHODCALLTYPE IWMPEvents_Error_Proxy( 
    IWMPEvents * This);


void __RPC_STUB IWMPEvents_Error_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ void STDMETHODCALLTYPE IWMPEvents_Warning_Proxy( 
    IWMPEvents * This,
    /* [in] */ long WarningType,
    /* [in] */ long Param,
    /* [in] */ BSTR Description);


void __RPC_STUB IWMPEvents_Warning_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ void STDMETHODCALLTYPE IWMPEvents_EndOfStream_Proxy( 
    IWMPEvents * This,
    /* [in] */ long Result);


void __RPC_STUB IWMPEvents_EndOfStream_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ void STDMETHODCALLTYPE IWMPEvents_PositionChange_Proxy( 
    IWMPEvents * This,
    /* [in] */ double oldPosition,
    /* [in] */ double newPosition);


void __RPC_STUB IWMPEvents_PositionChange_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ void STDMETHODCALLTYPE IWMPEvents_MarkerHit_Proxy( 
    IWMPEvents * This,
    /* [in] */ long MarkerNum);


void __RPC_STUB IWMPEvents_MarkerHit_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ void STDMETHODCALLTYPE IWMPEvents_DurationUnitChange_Proxy( 
    IWMPEvents * This,
    /* [in] */ long NewDurationUnit);


void __RPC_STUB IWMPEvents_DurationUnitChange_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ void STDMETHODCALLTYPE IWMPEvents_CdromMediaChange_Proxy( 
    IWMPEvents * This,
    /* [in] */ long CdromNum);


void __RPC_STUB IWMPEvents_CdromMediaChange_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ void STDMETHODCALLTYPE IWMPEvents_PlaylistChange_Proxy( 
    IWMPEvents * This,
    /* [in] */ IDispatch *Playlist,
    /* [in] */ WMPPlaylistChangeEventType change);


void __RPC_STUB IWMPEvents_PlaylistChange_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ void STDMETHODCALLTYPE IWMPEvents_CurrentPlaylistChange_Proxy( 
    IWMPEvents * This,
    /* [in] */ WMPPlaylistChangeEventType change);


void __RPC_STUB IWMPEvents_CurrentPlaylistChange_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ void STDMETHODCALLTYPE IWMPEvents_CurrentPlaylistItemAvailable_Proxy( 
    IWMPEvents * This,
    /* [in] */ BSTR bstrItemName);


void __RPC_STUB IWMPEvents_CurrentPlaylistItemAvailable_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ void STDMETHODCALLTYPE IWMPEvents_MediaChange_Proxy( 
    IWMPEvents * This,
    /* [in] */ IDispatch *Item);


void __RPC_STUB IWMPEvents_MediaChange_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ void STDMETHODCALLTYPE IWMPEvents_CurrentMediaItemAvailable_Proxy( 
    IWMPEvents * This,
    /* [in] */ BSTR bstrItemName);


void __RPC_STUB IWMPEvents_CurrentMediaItemAvailable_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ void STDMETHODCALLTYPE IWMPEvents_CurrentItemChange_Proxy( 
    IWMPEvents * This,
    /* [in] */ IDispatch *pdispMedia);


void __RPC_STUB IWMPEvents_CurrentItemChange_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ void STDMETHODCALLTYPE IWMPEvents_MediaCollectionChange_Proxy( 
    IWMPEvents * This);


void __RPC_STUB IWMPEvents_MediaCollectionChange_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ void STDMETHODCALLTYPE IWMPEvents_MediaCollectionAttributeStringAdded_Proxy( 
    IWMPEvents * This,
    /* [in] */ BSTR bstrAttribName,
    /* [in] */ BSTR bstrAttribVal);


void __RPC_STUB IWMPEvents_MediaCollectionAttributeStringAdded_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ void STDMETHODCALLTYPE IWMPEvents_MediaCollectionAttributeStringRemoved_Proxy( 
    IWMPEvents * This,
    /* [in] */ BSTR bstrAttribName,
    /* [in] */ BSTR bstrAttribVal);


void __RPC_STUB IWMPEvents_MediaCollectionAttributeStringRemoved_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ void STDMETHODCALLTYPE IWMPEvents_MediaCollectionAttributeStringChanged_Proxy( 
    IWMPEvents * This,
    /* [in] */ BSTR bstrAttribName,
    /* [in] */ BSTR bstrOldAttribVal,
    /* [in] */ BSTR bstrNewAttribVal);


void __RPC_STUB IWMPEvents_MediaCollectionAttributeStringChanged_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ void STDMETHODCALLTYPE IWMPEvents_PlaylistCollectionChange_Proxy( 
    IWMPEvents * This);


void __RPC_STUB IWMPEvents_PlaylistCollectionChange_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ void STDMETHODCALLTYPE IWMPEvents_PlaylistCollectionPlaylistAdded_Proxy( 
    IWMPEvents * This,
    /* [in] */ BSTR bstrPlaylistName);


void __RPC_STUB IWMPEvents_PlaylistCollectionPlaylistAdded_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ void STDMETHODCALLTYPE IWMPEvents_PlaylistCollectionPlaylistRemoved_Proxy( 
    IWMPEvents * This,
    /* [in] */ BSTR bstrPlaylistName);


void __RPC_STUB IWMPEvents_PlaylistCollectionPlaylistRemoved_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ void STDMETHODCALLTYPE IWMPEvents_PlaylistCollectionPlaylistSetAsDeleted_Proxy( 
    IWMPEvents * This,
    /* [in] */ BSTR bstrPlaylistName,
    /* [in] */ VARIANT_BOOL varfIsDeleted);


void __RPC_STUB IWMPEvents_PlaylistCollectionPlaylistSetAsDeleted_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ void STDMETHODCALLTYPE IWMPEvents_ModeChange_Proxy( 
    IWMPEvents * This,
    /* [in] */ BSTR ModeName,
    /* [in] */ VARIANT_BOOL NewValue);


void __RPC_STUB IWMPEvents_ModeChange_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ void STDMETHODCALLTYPE IWMPEvents_MediaError_Proxy( 
    IWMPEvents * This,
    /* [in] */ IDispatch *pMediaObject);


void __RPC_STUB IWMPEvents_MediaError_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ void STDMETHODCALLTYPE IWMPEvents_OpenPlaylistSwitch_Proxy( 
    IWMPEvents * This,
    /* [in] */ IDispatch *pItem);


void __RPC_STUB IWMPEvents_OpenPlaylistSwitch_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ void STDMETHODCALLTYPE IWMPEvents_DomainChange_Proxy( 
    IWMPEvents * This,
    /* [in] */ BSTR strDomain);


void __RPC_STUB IWMPEvents_DomainChange_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ void STDMETHODCALLTYPE IWMPEvents_SwitchedToPlayerApplication_Proxy( 
    IWMPEvents * This);


void __RPC_STUB IWMPEvents_SwitchedToPlayerApplication_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ void STDMETHODCALLTYPE IWMPEvents_SwitchedToControl_Proxy( 
    IWMPEvents * This);


void __RPC_STUB IWMPEvents_SwitchedToControl_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ void STDMETHODCALLTYPE IWMPEvents_PlayerDockedStateChange_Proxy( 
    IWMPEvents * This);


void __RPC_STUB IWMPEvents_PlayerDockedStateChange_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ void STDMETHODCALLTYPE IWMPEvents_PlayerReconnect_Proxy( 
    IWMPEvents * This);


void __RPC_STUB IWMPEvents_PlayerReconnect_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ void STDMETHODCALLTYPE IWMPEvents_Click_Proxy( 
    IWMPEvents * This,
    /* [in] */ short nButton,
    /* [in] */ short nShiftState,
    /* [in] */ long fX,
    /* [in] */ long fY);


void __RPC_STUB IWMPEvents_Click_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ void STDMETHODCALLTYPE IWMPEvents_DoubleClick_Proxy( 
    IWMPEvents * This,
    /* [in] */ short nButton,
    /* [in] */ short nShiftState,
    /* [in] */ long fX,
    /* [in] */ long fY);


void __RPC_STUB IWMPEvents_DoubleClick_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ void STDMETHODCALLTYPE IWMPEvents_KeyDown_Proxy( 
    IWMPEvents * This,
    /* [in] */ short nKeyCode,
    /* [in] */ short nShiftState);


void __RPC_STUB IWMPEvents_KeyDown_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ void STDMETHODCALLTYPE IWMPEvents_KeyPress_Proxy( 
    IWMPEvents * This,
    /* [in] */ short nKeyAscii);


void __RPC_STUB IWMPEvents_KeyPress_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ void STDMETHODCALLTYPE IWMPEvents_KeyUp_Proxy( 
    IWMPEvents * This,
    /* [in] */ short nKeyCode,
    /* [in] */ short nShiftState);


void __RPC_STUB IWMPEvents_KeyUp_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ void STDMETHODCALLTYPE IWMPEvents_MouseDown_Proxy( 
    IWMPEvents * This,
    /* [in] */ short nButton,
    /* [in] */ short nShiftState,
    /* [in] */ long fX,
    /* [in] */ long fY);


void __RPC_STUB IWMPEvents_MouseDown_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ void STDMETHODCALLTYPE IWMPEvents_MouseMove_Proxy( 
    IWMPEvents * This,
    /* [in] */ short nButton,
    /* [in] */ short nShiftState,
    /* [in] */ long fX,
    /* [in] */ long fY);


void __RPC_STUB IWMPEvents_MouseMove_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ void STDMETHODCALLTYPE IWMPEvents_MouseUp_Proxy( 
    IWMPEvents * This,
    /* [in] */ short nButton,
    /* [in] */ short nShiftState,
    /* [in] */ long fX,
    /* [in] */ long fY);


void __RPC_STUB IWMPEvents_MouseUp_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IWMPEvents_INTERFACE_DEFINED__ */


#ifndef __IWMPEvents2_INTERFACE_DEFINED__
#define __IWMPEvents2_INTERFACE_DEFINED__

/* interface IWMPEvents2 */
/* [unique][helpstring][uuid][object] */ 


EXTERN_C const IID IID_IWMPEvents2;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("1E7601FA-47EA-4107-9EA9-9004ED9684FF")
    IWMPEvents2 : public IWMPEvents
    {
    public:
        virtual /* [helpstring][id] */ void STDMETHODCALLTYPE DeviceConnect( 
            /* [in] */ IWMPSyncDevice *pDevice) = 0;
        
        virtual /* [helpstring][id] */ void STDMETHODCALLTYPE DeviceDisconnect( 
            /* [in] */ IWMPSyncDevice *pDevice) = 0;
        
        virtual /* [helpstring][id] */ void STDMETHODCALLTYPE DeviceStatusChange( 
            /* [in] */ IWMPSyncDevice *pDevice,
            /* [in] */ WMPDeviceStatus NewStatus) = 0;
        
        virtual /* [helpstring][id] */ void STDMETHODCALLTYPE DeviceSyncStateChange( 
            /* [in] */ IWMPSyncDevice *pDevice,
            /* [in] */ WMPSyncState NewState) = 0;
        
        virtual /* [helpstring][id] */ void STDMETHODCALLTYPE DeviceSyncError( 
            /* [in] */ IWMPSyncDevice *pDevice,
            /* [in] */ IDispatch *pMedia) = 0;
        
        virtual /* [helpstring][id] */ void STDMETHODCALLTYPE CreatePartnershipComplete( 
            /* [in] */ IWMPSyncDevice *pDevice,
            /* [in] */ HRESULT hrResult) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IWMPEvents2Vtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IWMPEvents2 * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IWMPEvents2 * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IWMPEvents2 * This);
        
        /* [helpstring][id] */ void ( STDMETHODCALLTYPE *OpenStateChange )( 
            IWMPEvents2 * This,
            /* [in] */ long NewState);
        
        /* [helpstring][id] */ void ( STDMETHODCALLTYPE *PlayStateChange )( 
            IWMPEvents2 * This,
            /* [in] */ long NewState);
        
        /* [helpstring][id] */ void ( STDMETHODCALLTYPE *AudioLanguageChange )( 
            IWMPEvents2 * This,
            /* [in] */ long LangID);
        
        /* [helpstring][id] */ void ( STDMETHODCALLTYPE *StatusChange )( 
            IWMPEvents2 * This);
        
        /* [helpstring][id] */ void ( STDMETHODCALLTYPE *ScriptCommand )( 
            IWMPEvents2 * This,
            /* [in] */ BSTR scType,
            /* [in] */ BSTR Param);
        
        /* [helpstring][id] */ void ( STDMETHODCALLTYPE *NewStream )( 
            IWMPEvents2 * This);
        
        /* [helpstring][id] */ void ( STDMETHODCALLTYPE *Disconnect )( 
            IWMPEvents2 * This,
            /* [in] */ long Result);
        
        /* [helpstring][id] */ void ( STDMETHODCALLTYPE *Buffering )( 
            IWMPEvents2 * This,
            /* [in] */ VARIANT_BOOL Start);
        
        /* [helpstring][id] */ void ( STDMETHODCALLTYPE *Error )( 
            IWMPEvents2 * This);
        
        /* [helpstring][id] */ void ( STDMETHODCALLTYPE *Warning )( 
            IWMPEvents2 * This,
            /* [in] */ long WarningType,
            /* [in] */ long Param,
            /* [in] */ BSTR Description);
        
        /* [helpstring][id] */ void ( STDMETHODCALLTYPE *EndOfStream )( 
            IWMPEvents2 * This,
            /* [in] */ long Result);
        
        /* [helpstring][id] */ void ( STDMETHODCALLTYPE *PositionChange )( 
            IWMPEvents2 * This,
            /* [in] */ double oldPosition,
            /* [in] */ double newPosition);
        
        /* [helpstring][id] */ void ( STDMETHODCALLTYPE *MarkerHit )( 
            IWMPEvents2 * This,
            /* [in] */ long MarkerNum);
        
        /* [helpstring][id] */ void ( STDMETHODCALLTYPE *DurationUnitChange )( 
            IWMPEvents2 * This,
            /* [in] */ long NewDurationUnit);
        
        /* [helpstring][id] */ void ( STDMETHODCALLTYPE *CdromMediaChange )( 
            IWMPEvents2 * This,
            /* [in] */ long CdromNum);
        
        /* [helpstring][id] */ void ( STDMETHODCALLTYPE *PlaylistChange )( 
            IWMPEvents2 * This,
            /* [in] */ IDispatch *Playlist,
            /* [in] */ WMPPlaylistChangeEventType change);
        
        /* [helpstring][id] */ void ( STDMETHODCALLTYPE *CurrentPlaylistChange )( 
            IWMPEvents2 * This,
            /* [in] */ WMPPlaylistChangeEventType change);
        
        /* [helpstring][id] */ void ( STDMETHODCALLTYPE *CurrentPlaylistItemAvailable )( 
            IWMPEvents2 * This,
            /* [in] */ BSTR bstrItemName);
        
        /* [helpstring][id] */ void ( STDMETHODCALLTYPE *MediaChange )( 
            IWMPEvents2 * This,
            /* [in] */ IDispatch *Item);
        
        /* [helpstring][id] */ void ( STDMETHODCALLTYPE *CurrentMediaItemAvailable )( 
            IWMPEvents2 * This,
            /* [in] */ BSTR bstrItemName);
        
        /* [helpstring][id] */ void ( STDMETHODCALLTYPE *CurrentItemChange )( 
            IWMPEvents2 * This,
            /* [in] */ IDispatch *pdispMedia);
        
        /* [helpstring][id] */ void ( STDMETHODCALLTYPE *MediaCollectionChange )( 
            IWMPEvents2 * This);
        
        /* [helpstring][id] */ void ( STDMETHODCALLTYPE *MediaCollectionAttributeStringAdded )( 
            IWMPEvents2 * This,
            /* [in] */ BSTR bstrAttribName,
            /* [in] */ BSTR bstrAttribVal);
        
        /* [helpstring][id] */ void ( STDMETHODCALLTYPE *MediaCollectionAttributeStringRemoved )( 
            IWMPEvents2 * This,
            /* [in] */ BSTR bstrAttribName,
            /* [in] */ BSTR bstrAttribVal);
        
        /* [helpstring][id] */ void ( STDMETHODCALLTYPE *MediaCollectionAttributeStringChanged )( 
            IWMPEvents2 * This,
            /* [in] */ BSTR bstrAttribName,
            /* [in] */ BSTR bstrOldAttribVal,
            /* [in] */ BSTR bstrNewAttribVal);
        
        /* [helpstring][id] */ void ( STDMETHODCALLTYPE *PlaylistCollectionChange )( 
            IWMPEvents2 * This);
        
        /* [helpstring][id] */ void ( STDMETHODCALLTYPE *PlaylistCollectionPlaylistAdded )( 
            IWMPEvents2 * This,
            /* [in] */ BSTR bstrPlaylistName);
        
        /* [helpstring][id] */ void ( STDMETHODCALLTYPE *PlaylistCollectionPlaylistRemoved )( 
            IWMPEvents2 * This,
            /* [in] */ BSTR bstrPlaylistName);
        
        /* [helpstring][id] */ void ( STDMETHODCALLTYPE *PlaylistCollectionPlaylistSetAsDeleted )( 
            IWMPEvents2 * This,
            /* [in] */ BSTR bstrPlaylistName,
            /* [in] */ VARIANT_BOOL varfIsDeleted);
        
        /* [helpstring][id] */ void ( STDMETHODCALLTYPE *ModeChange )( 
            IWMPEvents2 * This,
            /* [in] */ BSTR ModeName,
            /* [in] */ VARIANT_BOOL NewValue);
        
        /* [helpstring][id] */ void ( STDMETHODCALLTYPE *MediaError )( 
            IWMPEvents2 * This,
            /* [in] */ IDispatch *pMediaObject);
        
        /* [helpstring][id] */ void ( STDMETHODCALLTYPE *OpenPlaylistSwitch )( 
            IWMPEvents2 * This,
            /* [in] */ IDispatch *pItem);
        
        /* [helpstring][id] */ void ( STDMETHODCALLTYPE *DomainChange )( 
            IWMPEvents2 * This,
            /* [in] */ BSTR strDomain);
        
        /* [helpstring][id] */ void ( STDMETHODCALLTYPE *SwitchedToPlayerApplication )( 
            IWMPEvents2 * This);
        
        /* [helpstring][id] */ void ( STDMETHODCALLTYPE *SwitchedToControl )( 
            IWMPEvents2 * This);
        
        /* [helpstring][id] */ void ( STDMETHODCALLTYPE *PlayerDockedStateChange )( 
            IWMPEvents2 * This);
        
        /* [helpstring][id] */ void ( STDMETHODCALLTYPE *PlayerReconnect )( 
            IWMPEvents2 * This);
        
        /* [helpstring][id] */ void ( STDMETHODCALLTYPE *Click )( 
            IWMPEvents2 * This,
            /* [in] */ short nButton,
            /* [in] */ short nShiftState,
            /* [in] */ long fX,
            /* [in] */ long fY);
        
        /* [helpstring][id] */ void ( STDMETHODCALLTYPE *DoubleClick )( 
            IWMPEvents2 * This,
            /* [in] */ short nButton,
            /* [in] */ short nShiftState,
            /* [in] */ long fX,
            /* [in] */ long fY);
        
        /* [helpstring][id] */ void ( STDMETHODCALLTYPE *KeyDown )( 
            IWMPEvents2 * This,
            /* [in] */ short nKeyCode,
            /* [in] */ short nShiftState);
        
        /* [helpstring][id] */ void ( STDMETHODCALLTYPE *KeyPress )( 
            IWMPEvents2 * This,
            /* [in] */ short nKeyAscii);
        
        /* [helpstring][id] */ void ( STDMETHODCALLTYPE *KeyUp )( 
            IWMPEvents2 * This,
            /* [in] */ short nKeyCode,
            /* [in] */ short nShiftState);
        
        /* [helpstring][id] */ void ( STDMETHODCALLTYPE *MouseDown )( 
            IWMPEvents2 * This,
            /* [in] */ short nButton,
            /* [in] */ short nShiftState,
            /* [in] */ long fX,
            /* [in] */ long fY);
        
        /* [helpstring][id] */ void ( STDMETHODCALLTYPE *MouseMove )( 
            IWMPEvents2 * This,
            /* [in] */ short nButton,
            /* [in] */ short nShiftState,
            /* [in] */ long fX,
            /* [in] */ long fY);
        
        /* [helpstring][id] */ void ( STDMETHODCALLTYPE *MouseUp )( 
            IWMPEvents2 * This,
            /* [in] */ short nButton,
            /* [in] */ short nShiftState,
            /* [in] */ long fX,
            /* [in] */ long fY);
        
        /* [helpstring][id] */ void ( STDMETHODCALLTYPE *DeviceConnect )( 
            IWMPEvents2 * This,
            /* [in] */ IWMPSyncDevice *pDevice);
        
        /* [helpstring][id] */ void ( STDMETHODCALLTYPE *DeviceDisconnect )( 
            IWMPEvents2 * This,
            /* [in] */ IWMPSyncDevice *pDevice);
        
        /* [helpstring][id] */ void ( STDMETHODCALLTYPE *DeviceStatusChange )( 
            IWMPEvents2 * This,
            /* [in] */ IWMPSyncDevice *pDevice,
            /* [in] */ WMPDeviceStatus NewStatus);
        
        /* [helpstring][id] */ void ( STDMETHODCALLTYPE *DeviceSyncStateChange )( 
            IWMPEvents2 * This,
            /* [in] */ IWMPSyncDevice *pDevice,
            /* [in] */ WMPSyncState NewState);
        
        /* [helpstring][id] */ void ( STDMETHODCALLTYPE *DeviceSyncError )( 
            IWMPEvents2 * This,
            /* [in] */ IWMPSyncDevice *pDevice,
            /* [in] */ IDispatch *pMedia);
        
        /* [helpstring][id] */ void ( STDMETHODCALLTYPE *CreatePartnershipComplete )( 
            IWMPEvents2 * This,
            /* [in] */ IWMPSyncDevice *pDevice,
            /* [in] */ HRESULT hrResult);
        
        END_INTERFACE
    } IWMPEvents2Vtbl;

    interface IWMPEvents2
    {
        CONST_VTBL struct IWMPEvents2Vtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IWMPEvents2_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IWMPEvents2_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IWMPEvents2_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IWMPEvents2_OpenStateChange(This,NewState)	\
    (This)->lpVtbl -> OpenStateChange(This,NewState)

#define IWMPEvents2_PlayStateChange(This,NewState)	\
    (This)->lpVtbl -> PlayStateChange(This,NewState)

#define IWMPEvents2_AudioLanguageChange(This,LangID)	\
    (This)->lpVtbl -> AudioLanguageChange(This,LangID)

#define IWMPEvents2_StatusChange(This)	\
    (This)->lpVtbl -> StatusChange(This)

#define IWMPEvents2_ScriptCommand(This,scType,Param)	\
    (This)->lpVtbl -> ScriptCommand(This,scType,Param)

#define IWMPEvents2_NewStream(This)	\
    (This)->lpVtbl -> NewStream(This)

#define IWMPEvents2_Disconnect(This,Result)	\
    (This)->lpVtbl -> Disconnect(This,Result)

#define IWMPEvents2_Buffering(This,Start)	\
    (This)->lpVtbl -> Buffering(This,Start)

#define IWMPEvents2_Error(This)	\
    (This)->lpVtbl -> Error(This)

#define IWMPEvents2_Warning(This,WarningType,Param,Description)	\
    (This)->lpVtbl -> Warning(This,WarningType,Param,Description)

#define IWMPEvents2_EndOfStream(This,Result)	\
    (This)->lpVtbl -> EndOfStream(This,Result)

#define IWMPEvents2_PositionChange(This,oldPosition,newPosition)	\
    (This)->lpVtbl -> PositionChange(This,oldPosition,newPosition)

#define IWMPEvents2_MarkerHit(This,MarkerNum)	\
    (This)->lpVtbl -> MarkerHit(This,MarkerNum)

#define IWMPEvents2_DurationUnitChange(This,NewDurationUnit)	\
    (This)->lpVtbl -> DurationUnitChange(This,NewDurationUnit)

#define IWMPEvents2_CdromMediaChange(This,CdromNum)	\
    (This)->lpVtbl -> CdromMediaChange(This,CdromNum)

#define IWMPEvents2_PlaylistChange(This,Playlist,change)	\
    (This)->lpVtbl -> PlaylistChange(This,Playlist,change)

#define IWMPEvents2_CurrentPlaylistChange(This,change)	\
    (This)->lpVtbl -> CurrentPlaylistChange(This,change)

#define IWMPEvents2_CurrentPlaylistItemAvailable(This,bstrItemName)	\
    (This)->lpVtbl -> CurrentPlaylistItemAvailable(This,bstrItemName)

#define IWMPEvents2_MediaChange(This,Item)	\
    (This)->lpVtbl -> MediaChange(This,Item)

#define IWMPEvents2_CurrentMediaItemAvailable(This,bstrItemName)	\
    (This)->lpVtbl -> CurrentMediaItemAvailable(This,bstrItemName)

#define IWMPEvents2_CurrentItemChange(This,pdispMedia)	\
    (This)->lpVtbl -> CurrentItemChange(This,pdispMedia)

#define IWMPEvents2_MediaCollectionChange(This)	\
    (This)->lpVtbl -> MediaCollectionChange(This)

#define IWMPEvents2_MediaCollectionAttributeStringAdded(This,bstrAttribName,bstrAttribVal)	\
    (This)->lpVtbl -> MediaCollectionAttributeStringAdded(This,bstrAttribName,bstrAttribVal)

#define IWMPEvents2_MediaCollectionAttributeStringRemoved(This,bstrAttribName,bstrAttribVal)	\
    (This)->lpVtbl -> MediaCollectionAttributeStringRemoved(This,bstrAttribName,bstrAttribVal)

#define IWMPEvents2_MediaCollectionAttributeStringChanged(This,bstrAttribName,bstrOldAttribVal,bstrNewAttribVal)	\
    (This)->lpVtbl -> MediaCollectionAttributeStringChanged(This,bstrAttribName,bstrOldAttribVal,bstrNewAttribVal)

#define IWMPEvents2_PlaylistCollectionChange(This)	\
    (This)->lpVtbl -> PlaylistCollectionChange(This)

#define IWMPEvents2_PlaylistCollectionPlaylistAdded(This,bstrPlaylistName)	\
    (This)->lpVtbl -> PlaylistCollectionPlaylistAdded(This,bstrPlaylistName)

#define IWMPEvents2_PlaylistCollectionPlaylistRemoved(This,bstrPlaylistName)	\
    (This)->lpVtbl -> PlaylistCollectionPlaylistRemoved(This,bstrPlaylistName)

#define IWMPEvents2_PlaylistCollectionPlaylistSetAsDeleted(This,bstrPlaylistName,varfIsDeleted)	\
    (This)->lpVtbl -> PlaylistCollectionPlaylistSetAsDeleted(This,bstrPlaylistName,varfIsDeleted)

#define IWMPEvents2_ModeChange(This,ModeName,NewValue)	\
    (This)->lpVtbl -> ModeChange(This,ModeName,NewValue)

#define IWMPEvents2_MediaError(This,pMediaObject)	\
    (This)->lpVtbl -> MediaError(This,pMediaObject)

#define IWMPEvents2_OpenPlaylistSwitch(This,pItem)	\
    (This)->lpVtbl -> OpenPlaylistSwitch(This,pItem)

#define IWMPEvents2_DomainChange(This,strDomain)	\
    (This)->lpVtbl -> DomainChange(This,strDomain)

#define IWMPEvents2_SwitchedToPlayerApplication(This)	\
    (This)->lpVtbl -> SwitchedToPlayerApplication(This)

#define IWMPEvents2_SwitchedToControl(This)	\
    (This)->lpVtbl -> SwitchedToControl(This)

#define IWMPEvents2_PlayerDockedStateChange(This)	\
    (This)->lpVtbl -> PlayerDockedStateChange(This)

#define IWMPEvents2_PlayerReconnect(This)	\
    (This)->lpVtbl -> PlayerReconnect(This)

#define IWMPEvents2_Click(This,nButton,nShiftState,fX,fY)	\
    (This)->lpVtbl -> Click(This,nButton,nShiftState,fX,fY)

#define IWMPEvents2_DoubleClick(This,nButton,nShiftState,fX,fY)	\
    (This)->lpVtbl -> DoubleClick(This,nButton,nShiftState,fX,fY)

#define IWMPEvents2_KeyDown(This,nKeyCode,nShiftState)	\
    (This)->lpVtbl -> KeyDown(This,nKeyCode,nShiftState)

#define IWMPEvents2_KeyPress(This,nKeyAscii)	\
    (This)->lpVtbl -> KeyPress(This,nKeyAscii)

#define IWMPEvents2_KeyUp(This,nKeyCode,nShiftState)	\
    (This)->lpVtbl -> KeyUp(This,nKeyCode,nShiftState)

#define IWMPEvents2_MouseDown(This,nButton,nShiftState,fX,fY)	\
    (This)->lpVtbl -> MouseDown(This,nButton,nShiftState,fX,fY)

#define IWMPEvents2_MouseMove(This,nButton,nShiftState,fX,fY)	\
    (This)->lpVtbl -> MouseMove(This,nButton,nShiftState,fX,fY)

#define IWMPEvents2_MouseUp(This,nButton,nShiftState,fX,fY)	\
    (This)->lpVtbl -> MouseUp(This,nButton,nShiftState,fX,fY)


#define IWMPEvents2_DeviceConnect(This,pDevice)	\
    (This)->lpVtbl -> DeviceConnect(This,pDevice)

#define IWMPEvents2_DeviceDisconnect(This,pDevice)	\
    (This)->lpVtbl -> DeviceDisconnect(This,pDevice)

#define IWMPEvents2_DeviceStatusChange(This,pDevice,NewStatus)	\
    (This)->lpVtbl -> DeviceStatusChange(This,pDevice,NewStatus)

#define IWMPEvents2_DeviceSyncStateChange(This,pDevice,NewState)	\
    (This)->lpVtbl -> DeviceSyncStateChange(This,pDevice,NewState)

#define IWMPEvents2_DeviceSyncError(This,pDevice,pMedia)	\
    (This)->lpVtbl -> DeviceSyncError(This,pDevice,pMedia)

#define IWMPEvents2_CreatePartnershipComplete(This,pDevice,hrResult)	\
    (This)->lpVtbl -> CreatePartnershipComplete(This,pDevice,hrResult)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][id] */ void STDMETHODCALLTYPE IWMPEvents2_DeviceConnect_Proxy( 
    IWMPEvents2 * This,
    /* [in] */ IWMPSyncDevice *pDevice);


void __RPC_STUB IWMPEvents2_DeviceConnect_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ void STDMETHODCALLTYPE IWMPEvents2_DeviceDisconnect_Proxy( 
    IWMPEvents2 * This,
    /* [in] */ IWMPSyncDevice *pDevice);


void __RPC_STUB IWMPEvents2_DeviceDisconnect_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ void STDMETHODCALLTYPE IWMPEvents2_DeviceStatusChange_Proxy( 
    IWMPEvents2 * This,
    /* [in] */ IWMPSyncDevice *pDevice,
    /* [in] */ WMPDeviceStatus NewStatus);


void __RPC_STUB IWMPEvents2_DeviceStatusChange_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ void STDMETHODCALLTYPE IWMPEvents2_DeviceSyncStateChange_Proxy( 
    IWMPEvents2 * This,
    /* [in] */ IWMPSyncDevice *pDevice,
    /* [in] */ WMPSyncState NewState);


void __RPC_STUB IWMPEvents2_DeviceSyncStateChange_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ void STDMETHODCALLTYPE IWMPEvents2_DeviceSyncError_Proxy( 
    IWMPEvents2 * This,
    /* [in] */ IWMPSyncDevice *pDevice,
    /* [in] */ IDispatch *pMedia);


void __RPC_STUB IWMPEvents2_DeviceSyncError_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ void STDMETHODCALLTYPE IWMPEvents2_CreatePartnershipComplete_Proxy( 
    IWMPEvents2 * This,
    /* [in] */ IWMPSyncDevice *pDevice,
    /* [in] */ HRESULT hrResult);


void __RPC_STUB IWMPEvents2_CreatePartnershipComplete_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IWMPEvents2_INTERFACE_DEFINED__ */


#ifndef ___WMPOCXEvents_DISPINTERFACE_DEFINED__
#define ___WMPOCXEvents_DISPINTERFACE_DEFINED__

/* dispinterface _WMPOCXEvents */
/* [hidden][helpstring][uuid] */ 


EXTERN_C const IID DIID__WMPOCXEvents;

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("6BF52A51-394A-11d3-B153-00C04F79FAA6")
    _WMPOCXEvents : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct _WMPOCXEventsVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            _WMPOCXEvents * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            _WMPOCXEvents * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            _WMPOCXEvents * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            _WMPOCXEvents * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            _WMPOCXEvents * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            _WMPOCXEvents * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            _WMPOCXEvents * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        END_INTERFACE
    } _WMPOCXEventsVtbl;

    interface _WMPOCXEvents
    {
        CONST_VTBL struct _WMPOCXEventsVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define _WMPOCXEvents_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define _WMPOCXEvents_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define _WMPOCXEvents_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define _WMPOCXEvents_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define _WMPOCXEvents_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define _WMPOCXEvents_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define _WMPOCXEvents_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* ___WMPOCXEvents_DISPINTERFACE_DEFINED__ */


EXTERN_C const CLSID CLSID_WindowsMediaPlayer;

#ifdef __cplusplus

class DECLSPEC_UUID("6BF52A52-394A-11d3-B153-00C04F79FAA6")
WindowsMediaPlayer;
#endif
#endif /* __WMPLib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

unsigned long             __RPC_USER  BSTR_UserSize(     unsigned long *, unsigned long            , BSTR * ); 
unsigned char * __RPC_USER  BSTR_UserMarshal(  unsigned long *, unsigned char *, BSTR * ); 
unsigned char * __RPC_USER  BSTR_UserUnmarshal(unsigned long *, unsigned char *, BSTR * ); 
void                      __RPC_USER  BSTR_UserFree(     unsigned long *, BSTR * ); 

unsigned long             __RPC_USER  VARIANT_UserSize(     unsigned long *, unsigned long            , VARIANT * ); 
unsigned char * __RPC_USER  VARIANT_UserMarshal(  unsigned long *, unsigned char *, VARIANT * ); 
unsigned char * __RPC_USER  VARIANT_UserUnmarshal(unsigned long *, unsigned char *, VARIANT * ); 
void                      __RPC_USER  VARIANT_UserFree(     unsigned long *, VARIANT * ); 

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif



/* this ALWAYS GENERATED file contains the definitions for the interfaces */


/* File created by MIDL compiler version 5.01.0164 */
/* at Mon Jul 14 21:40:07 2003
 */
/* Compiler settings for dotXSI.idl:
    Oicf (OptLev=i2), W1, Zp8, env=Win32, ms_ext, c_ext
    error checks: allocation ref bounds_check enum stub_data 
*/
//@@MIDL_FILE_HEADING(  )


/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 440
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

#ifndef __dotXSI_h__
#define __dotXSI_h__

#ifdef __cplusplus
extern "C"{
#endif 

/* Forward Declarations */ 

#ifndef __IParam_FWD_DEFINED__
#define __IParam_FWD_DEFINED__
typedef interface IParam IParam;
#endif 	/* __IParam_FWD_DEFINED__ */


#ifndef __IParams_FWD_DEFINED__
#define __IParams_FWD_DEFINED__
typedef interface IParams IParams;
#endif 	/* __IParams_FWD_DEFINED__ */


#ifndef __ITemplate_FWD_DEFINED__
#define __ITemplate_FWD_DEFINED__
typedef interface ITemplate ITemplate;
#endif 	/* __ITemplate_FWD_DEFINED__ */


#ifndef __ITemplates_FWD_DEFINED__
#define __ITemplates_FWD_DEFINED__
typedef interface ITemplates ITemplates;
#endif 	/* __ITemplates_FWD_DEFINED__ */


#ifndef __IdotXSIFileHandler_FWD_DEFINED__
#define __IdotXSIFileHandler_FWD_DEFINED__
typedef interface IdotXSIFileHandler IdotXSIFileHandler;
#endif 	/* __IdotXSIFileHandler_FWD_DEFINED__ */


#ifndef __Param_FWD_DEFINED__
#define __Param_FWD_DEFINED__

#ifdef __cplusplus
typedef class Param Param;
#else
typedef struct Param Param;
#endif /* __cplusplus */

#endif 	/* __Param_FWD_DEFINED__ */


#ifndef __Params_FWD_DEFINED__
#define __Params_FWD_DEFINED__

#ifdef __cplusplus
typedef class Params Params;
#else
typedef struct Params Params;
#endif /* __cplusplus */

#endif 	/* __Params_FWD_DEFINED__ */


#ifndef __Template_FWD_DEFINED__
#define __Template_FWD_DEFINED__

#ifdef __cplusplus
typedef class Template Template;
#else
typedef struct Template Template;
#endif /* __cplusplus */

#endif 	/* __Template_FWD_DEFINED__ */


#ifndef __Templates_FWD_DEFINED__
#define __Templates_FWD_DEFINED__

#ifdef __cplusplus
typedef class Templates Templates;
#else
typedef struct Templates Templates;
#endif /* __cplusplus */

#endif 	/* __Templates_FWD_DEFINED__ */


#ifndef __dotXSIFileHandler_FWD_DEFINED__
#define __dotXSIFileHandler_FWD_DEFINED__

#ifdef __cplusplus
typedef class dotXSIFileHandler dotXSIFileHandler;
#else
typedef struct dotXSIFileHandler dotXSIFileHandler;
#endif /* __cplusplus */

#endif 	/* __dotXSIFileHandler_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"

void __RPC_FAR * __RPC_USER MIDL_user_allocate(size_t);
void __RPC_USER MIDL_user_free( void __RPC_FAR * ); 

/* interface __MIDL_itf_dotXSI_0000 */
/* [local] */ 






typedef /* [public][public] */ 
enum __MIDL___MIDL_itf_dotXSI_0000_0001
    {	DOTXSI_FORMAT_TEXT	= 0,
	DOTXSI_FORMAT_BINARY	= 1,
	DOTXSI_FORMAT_COMPRESS	= 2,
	DOTXSI_FORMAT_ENCRYPTED	= 4
    }	eDotXSIFileFormat;



extern RPC_IF_HANDLE __MIDL_itf_dotXSI_0000_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_dotXSI_0000_v0_0_s_ifspec;

#ifndef __IParam_INTERFACE_DEFINED__
#define __IParam_INTERFACE_DEFINED__

/* interface IParam */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_IParam;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("5E611818-3B0B-11D4-B77F-00A0C9EC5EA3")
    IParam : public IDispatch
    {
    public:
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SetName( 
            /* [in] */ BSTR in_bstrName) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetName( 
            /* [out] */ BSTR __RPC_FAR *out_pbstrName) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SetValue( 
            /* [in] */ VARIANT in_vValue) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetValue( 
            /* [out] */ VARIANT __RPC_FAR *out_pvValue) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE AllocateArray( 
            /* [out] */ VARIANT __RPC_FAR *out_pvValue,
            /* [in] */ int in_lSize) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE FreeStringVariant( 
            /* [in] */ VARIANT in_vValue) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IParamVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IParam __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IParam __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IParam __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            IParam __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            IParam __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            IParam __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            IParam __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetName )( 
            IParam __RPC_FAR * This,
            /* [in] */ BSTR in_bstrName);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetName )( 
            IParam __RPC_FAR * This,
            /* [out] */ BSTR __RPC_FAR *out_pbstrName);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetValue )( 
            IParam __RPC_FAR * This,
            /* [in] */ VARIANT in_vValue);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetValue )( 
            IParam __RPC_FAR * This,
            /* [out] */ VARIANT __RPC_FAR *out_pvValue);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *AllocateArray )( 
            IParam __RPC_FAR * This,
            /* [out] */ VARIANT __RPC_FAR *out_pvValue,
            /* [in] */ int in_lSize);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *FreeStringVariant )( 
            IParam __RPC_FAR * This,
            /* [in] */ VARIANT in_vValue);
        
        END_INTERFACE
    } IParamVtbl;

    interface IParam
    {
        CONST_VTBL struct IParamVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IParam_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IParam_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IParam_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IParam_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IParam_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IParam_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IParam_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IParam_SetName(This,in_bstrName)	\
    (This)->lpVtbl -> SetName(This,in_bstrName)

#define IParam_GetName(This,out_pbstrName)	\
    (This)->lpVtbl -> GetName(This,out_pbstrName)

#define IParam_SetValue(This,in_vValue)	\
    (This)->lpVtbl -> SetValue(This,in_vValue)

#define IParam_GetValue(This,out_pvValue)	\
    (This)->lpVtbl -> GetValue(This,out_pvValue)

#define IParam_AllocateArray(This,out_pvValue,in_lSize)	\
    (This)->lpVtbl -> AllocateArray(This,out_pvValue,in_lSize)

#define IParam_FreeStringVariant(This,in_vValue)	\
    (This)->lpVtbl -> FreeStringVariant(This,in_vValue)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IParam_SetName_Proxy( 
    IParam __RPC_FAR * This,
    /* [in] */ BSTR in_bstrName);


void __RPC_STUB IParam_SetName_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IParam_GetName_Proxy( 
    IParam __RPC_FAR * This,
    /* [out] */ BSTR __RPC_FAR *out_pbstrName);


void __RPC_STUB IParam_GetName_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IParam_SetValue_Proxy( 
    IParam __RPC_FAR * This,
    /* [in] */ VARIANT in_vValue);


void __RPC_STUB IParam_SetValue_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IParam_GetValue_Proxy( 
    IParam __RPC_FAR * This,
    /* [out] */ VARIANT __RPC_FAR *out_pvValue);


void __RPC_STUB IParam_GetValue_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IParam_AllocateArray_Proxy( 
    IParam __RPC_FAR * This,
    /* [out] */ VARIANT __RPC_FAR *out_pvValue,
    /* [in] */ int in_lSize);


void __RPC_STUB IParam_AllocateArray_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IParam_FreeStringVariant_Proxy( 
    IParam __RPC_FAR * This,
    /* [in] */ VARIANT in_vValue);


void __RPC_STUB IParam_FreeStringVariant_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IParam_INTERFACE_DEFINED__ */


#ifndef __IParams_INTERFACE_DEFINED__
#define __IParams_INTERFACE_DEFINED__

/* interface IParams */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_IParams;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("5E611819-3B0B-11D4-B77F-00A0C9EC5EA3")
    IParams : public IDispatch
    {
    public:
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetCount( 
            /* [out] */ int __RPC_FAR *out_plCount) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Item( 
            /* [in] */ int in_nIndex,
            /* [out] */ IParam __RPC_FAR *__RPC_FAR *out_ppParam) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Add( 
            /* [in] */ IParam __RPC_FAR *in_pParam,
            /* [in] */ int in_nPos) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Remove( 
            /* [in] */ int in_nIndex) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE CreateParam( 
            /* [out] */ IParam __RPC_FAR *__RPC_FAR *out_ppParam) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IParamsVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IParams __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IParams __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IParams __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            IParams __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            IParams __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            IParams __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            IParams __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetCount )( 
            IParams __RPC_FAR * This,
            /* [out] */ int __RPC_FAR *out_plCount);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Item )( 
            IParams __RPC_FAR * This,
            /* [in] */ int in_nIndex,
            /* [out] */ IParam __RPC_FAR *__RPC_FAR *out_ppParam);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Add )( 
            IParams __RPC_FAR * This,
            /* [in] */ IParam __RPC_FAR *in_pParam,
            /* [in] */ int in_nPos);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Remove )( 
            IParams __RPC_FAR * This,
            /* [in] */ int in_nIndex);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *CreateParam )( 
            IParams __RPC_FAR * This,
            /* [out] */ IParam __RPC_FAR *__RPC_FAR *out_ppParam);
        
        END_INTERFACE
    } IParamsVtbl;

    interface IParams
    {
        CONST_VTBL struct IParamsVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IParams_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IParams_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IParams_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IParams_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IParams_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IParams_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IParams_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IParams_GetCount(This,out_plCount)	\
    (This)->lpVtbl -> GetCount(This,out_plCount)

#define IParams_Item(This,in_nIndex,out_ppParam)	\
    (This)->lpVtbl -> Item(This,in_nIndex,out_ppParam)

#define IParams_Add(This,in_pParam,in_nPos)	\
    (This)->lpVtbl -> Add(This,in_pParam,in_nPos)

#define IParams_Remove(This,in_nIndex)	\
    (This)->lpVtbl -> Remove(This,in_nIndex)

#define IParams_CreateParam(This,out_ppParam)	\
    (This)->lpVtbl -> CreateParam(This,out_ppParam)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IParams_GetCount_Proxy( 
    IParams __RPC_FAR * This,
    /* [out] */ int __RPC_FAR *out_plCount);


void __RPC_STUB IParams_GetCount_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IParams_Item_Proxy( 
    IParams __RPC_FAR * This,
    /* [in] */ int in_nIndex,
    /* [out] */ IParam __RPC_FAR *__RPC_FAR *out_ppParam);


void __RPC_STUB IParams_Item_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IParams_Add_Proxy( 
    IParams __RPC_FAR * This,
    /* [in] */ IParam __RPC_FAR *in_pParam,
    /* [in] */ int in_nPos);


void __RPC_STUB IParams_Add_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IParams_Remove_Proxy( 
    IParams __RPC_FAR * This,
    /* [in] */ int in_nIndex);


void __RPC_STUB IParams_Remove_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IParams_CreateParam_Proxy( 
    IParams __RPC_FAR * This,
    /* [out] */ IParam __RPC_FAR *__RPC_FAR *out_ppParam);


void __RPC_STUB IParams_CreateParam_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IParams_INTERFACE_DEFINED__ */


#ifndef __ITemplate_INTERFACE_DEFINED__
#define __ITemplate_INTERFACE_DEFINED__

/* interface ITemplate */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_ITemplate;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("5E61181A-3B0B-11D4-B77F-00A0C9EC5EA3")
    ITemplate : public IDispatch
    {
    public:
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Children( 
            /* [out] */ ITemplates __RPC_FAR *__RPC_FAR *out_ppTemplates) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SetInstanceName( 
            /* [in] */ BSTR in_bstrName) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetInstanceName( 
            /* [out] */ BSTR __RPC_FAR *out_pbstrName) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SetUserDataType( 
            /* [in] */ BSTR in_DataType) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetUserDataType( 
            /* [out] */ BSTR __RPC_FAR *out_pbstrDataType) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SetParent( 
            /* [in] */ ITemplate __RPC_FAR *in_pParent) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetParent( 
            /* [out] */ ITemplate __RPC_FAR *__RPC_FAR *out_ppParent) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SetSystemFlags( 
            /* [in] */ int in_nSysFlags) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetSystemFlags( 
            /* [out] */ int __RPC_FAR *out_pnSysFlags) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SetUserFlags( 
            /* [in] */ int in_nUserFlags) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetUserFlags( 
            /* [out] */ int __RPC_FAR *out_pnUserFlags) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Params( 
            /* [out] */ IParams __RPC_FAR *__RPC_FAR *out_ppParams) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SetTemplateName( 
            /* [in] */ BSTR in_bstrTemplateName) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetTemplateName( 
            /* [out] */ BSTR __RPC_FAR *out_pbstrTemplateName) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE CreateTemplates( 
            /* [out] */ ITemplates __RPC_FAR *__RPC_FAR *out_ppTemplates) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct ITemplateVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            ITemplate __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            ITemplate __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            ITemplate __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            ITemplate __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            ITemplate __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            ITemplate __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            ITemplate __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Children )( 
            ITemplate __RPC_FAR * This,
            /* [out] */ ITemplates __RPC_FAR *__RPC_FAR *out_ppTemplates);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetInstanceName )( 
            ITemplate __RPC_FAR * This,
            /* [in] */ BSTR in_bstrName);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetInstanceName )( 
            ITemplate __RPC_FAR * This,
            /* [out] */ BSTR __RPC_FAR *out_pbstrName);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetUserDataType )( 
            ITemplate __RPC_FAR * This,
            /* [in] */ BSTR in_DataType);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetUserDataType )( 
            ITemplate __RPC_FAR * This,
            /* [out] */ BSTR __RPC_FAR *out_pbstrDataType);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetParent )( 
            ITemplate __RPC_FAR * This,
            /* [in] */ ITemplate __RPC_FAR *in_pParent);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetParent )( 
            ITemplate __RPC_FAR * This,
            /* [out] */ ITemplate __RPC_FAR *__RPC_FAR *out_ppParent);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetSystemFlags )( 
            ITemplate __RPC_FAR * This,
            /* [in] */ int in_nSysFlags);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetSystemFlags )( 
            ITemplate __RPC_FAR * This,
            /* [out] */ int __RPC_FAR *out_pnSysFlags);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetUserFlags )( 
            ITemplate __RPC_FAR * This,
            /* [in] */ int in_nUserFlags);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetUserFlags )( 
            ITemplate __RPC_FAR * This,
            /* [out] */ int __RPC_FAR *out_pnUserFlags);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Params )( 
            ITemplate __RPC_FAR * This,
            /* [out] */ IParams __RPC_FAR *__RPC_FAR *out_ppParams);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetTemplateName )( 
            ITemplate __RPC_FAR * This,
            /* [in] */ BSTR in_bstrTemplateName);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTemplateName )( 
            ITemplate __RPC_FAR * This,
            /* [out] */ BSTR __RPC_FAR *out_pbstrTemplateName);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *CreateTemplates )( 
            ITemplate __RPC_FAR * This,
            /* [out] */ ITemplates __RPC_FAR *__RPC_FAR *out_ppTemplates);
        
        END_INTERFACE
    } ITemplateVtbl;

    interface ITemplate
    {
        CONST_VTBL struct ITemplateVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ITemplate_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define ITemplate_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define ITemplate_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define ITemplate_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define ITemplate_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define ITemplate_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define ITemplate_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define ITemplate_Children(This,out_ppTemplates)	\
    (This)->lpVtbl -> Children(This,out_ppTemplates)

#define ITemplate_SetInstanceName(This,in_bstrName)	\
    (This)->lpVtbl -> SetInstanceName(This,in_bstrName)

#define ITemplate_GetInstanceName(This,out_pbstrName)	\
    (This)->lpVtbl -> GetInstanceName(This,out_pbstrName)

#define ITemplate_SetUserDataType(This,in_DataType)	\
    (This)->lpVtbl -> SetUserDataType(This,in_DataType)

#define ITemplate_GetUserDataType(This,out_pbstrDataType)	\
    (This)->lpVtbl -> GetUserDataType(This,out_pbstrDataType)

#define ITemplate_SetParent(This,in_pParent)	\
    (This)->lpVtbl -> SetParent(This,in_pParent)

#define ITemplate_GetParent(This,out_ppParent)	\
    (This)->lpVtbl -> GetParent(This,out_ppParent)

#define ITemplate_SetSystemFlags(This,in_nSysFlags)	\
    (This)->lpVtbl -> SetSystemFlags(This,in_nSysFlags)

#define ITemplate_GetSystemFlags(This,out_pnSysFlags)	\
    (This)->lpVtbl -> GetSystemFlags(This,out_pnSysFlags)

#define ITemplate_SetUserFlags(This,in_nUserFlags)	\
    (This)->lpVtbl -> SetUserFlags(This,in_nUserFlags)

#define ITemplate_GetUserFlags(This,out_pnUserFlags)	\
    (This)->lpVtbl -> GetUserFlags(This,out_pnUserFlags)

#define ITemplate_Params(This,out_ppParams)	\
    (This)->lpVtbl -> Params(This,out_ppParams)

#define ITemplate_SetTemplateName(This,in_bstrTemplateName)	\
    (This)->lpVtbl -> SetTemplateName(This,in_bstrTemplateName)

#define ITemplate_GetTemplateName(This,out_pbstrTemplateName)	\
    (This)->lpVtbl -> GetTemplateName(This,out_pbstrTemplateName)

#define ITemplate_CreateTemplates(This,out_ppTemplates)	\
    (This)->lpVtbl -> CreateTemplates(This,out_ppTemplates)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ITemplate_Children_Proxy( 
    ITemplate __RPC_FAR * This,
    /* [out] */ ITemplates __RPC_FAR *__RPC_FAR *out_ppTemplates);


void __RPC_STUB ITemplate_Children_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ITemplate_SetInstanceName_Proxy( 
    ITemplate __RPC_FAR * This,
    /* [in] */ BSTR in_bstrName);


void __RPC_STUB ITemplate_SetInstanceName_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ITemplate_GetInstanceName_Proxy( 
    ITemplate __RPC_FAR * This,
    /* [out] */ BSTR __RPC_FAR *out_pbstrName);


void __RPC_STUB ITemplate_GetInstanceName_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ITemplate_SetUserDataType_Proxy( 
    ITemplate __RPC_FAR * This,
    /* [in] */ BSTR in_DataType);


void __RPC_STUB ITemplate_SetUserDataType_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ITemplate_GetUserDataType_Proxy( 
    ITemplate __RPC_FAR * This,
    /* [out] */ BSTR __RPC_FAR *out_pbstrDataType);


void __RPC_STUB ITemplate_GetUserDataType_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ITemplate_SetParent_Proxy( 
    ITemplate __RPC_FAR * This,
    /* [in] */ ITemplate __RPC_FAR *in_pParent);


void __RPC_STUB ITemplate_SetParent_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ITemplate_GetParent_Proxy( 
    ITemplate __RPC_FAR * This,
    /* [out] */ ITemplate __RPC_FAR *__RPC_FAR *out_ppParent);


void __RPC_STUB ITemplate_GetParent_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ITemplate_SetSystemFlags_Proxy( 
    ITemplate __RPC_FAR * This,
    /* [in] */ int in_nSysFlags);


void __RPC_STUB ITemplate_SetSystemFlags_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ITemplate_GetSystemFlags_Proxy( 
    ITemplate __RPC_FAR * This,
    /* [out] */ int __RPC_FAR *out_pnSysFlags);


void __RPC_STUB ITemplate_GetSystemFlags_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ITemplate_SetUserFlags_Proxy( 
    ITemplate __RPC_FAR * This,
    /* [in] */ int in_nUserFlags);


void __RPC_STUB ITemplate_SetUserFlags_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ITemplate_GetUserFlags_Proxy( 
    ITemplate __RPC_FAR * This,
    /* [out] */ int __RPC_FAR *out_pnUserFlags);


void __RPC_STUB ITemplate_GetUserFlags_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ITemplate_Params_Proxy( 
    ITemplate __RPC_FAR * This,
    /* [out] */ IParams __RPC_FAR *__RPC_FAR *out_ppParams);


void __RPC_STUB ITemplate_Params_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ITemplate_SetTemplateName_Proxy( 
    ITemplate __RPC_FAR * This,
    /* [in] */ BSTR in_bstrTemplateName);


void __RPC_STUB ITemplate_SetTemplateName_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ITemplate_GetTemplateName_Proxy( 
    ITemplate __RPC_FAR * This,
    /* [out] */ BSTR __RPC_FAR *out_pbstrTemplateName);


void __RPC_STUB ITemplate_GetTemplateName_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ITemplate_CreateTemplates_Proxy( 
    ITemplate __RPC_FAR * This,
    /* [out] */ ITemplates __RPC_FAR *__RPC_FAR *out_ppTemplates);


void __RPC_STUB ITemplate_CreateTemplates_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __ITemplate_INTERFACE_DEFINED__ */


#ifndef __ITemplates_INTERFACE_DEFINED__
#define __ITemplates_INTERFACE_DEFINED__

/* interface ITemplates */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_ITemplates;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("5E61181B-3B0B-11D4-B77F-00A0C9EC5EA3")
    ITemplates : public IDispatch
    {
    public:
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetCount( 
            /* [out] */ int __RPC_FAR *out_plCount) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Item( 
            /* [in] */ int in_nIndex,
            /* [out] */ ITemplate __RPC_FAR *__RPC_FAR *out_ppTemplate) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Add( 
            /* [in] */ ITemplate __RPC_FAR *in_pTemplate,
            /* [in] */ int in_nPos) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Remove( 
            /* [in] */ int in_nIndex) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE CreateTemplate( 
            /* [out] */ ITemplate __RPC_FAR *__RPC_FAR *out_ppTemplate) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct ITemplatesVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            ITemplates __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            ITemplates __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            ITemplates __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            ITemplates __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            ITemplates __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            ITemplates __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            ITemplates __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetCount )( 
            ITemplates __RPC_FAR * This,
            /* [out] */ int __RPC_FAR *out_plCount);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Item )( 
            ITemplates __RPC_FAR * This,
            /* [in] */ int in_nIndex,
            /* [out] */ ITemplate __RPC_FAR *__RPC_FAR *out_ppTemplate);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Add )( 
            ITemplates __RPC_FAR * This,
            /* [in] */ ITemplate __RPC_FAR *in_pTemplate,
            /* [in] */ int in_nPos);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Remove )( 
            ITemplates __RPC_FAR * This,
            /* [in] */ int in_nIndex);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *CreateTemplate )( 
            ITemplates __RPC_FAR * This,
            /* [out] */ ITemplate __RPC_FAR *__RPC_FAR *out_ppTemplate);
        
        END_INTERFACE
    } ITemplatesVtbl;

    interface ITemplates
    {
        CONST_VTBL struct ITemplatesVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ITemplates_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define ITemplates_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define ITemplates_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define ITemplates_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define ITemplates_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define ITemplates_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define ITemplates_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define ITemplates_GetCount(This,out_plCount)	\
    (This)->lpVtbl -> GetCount(This,out_plCount)

#define ITemplates_Item(This,in_nIndex,out_ppTemplate)	\
    (This)->lpVtbl -> Item(This,in_nIndex,out_ppTemplate)

#define ITemplates_Add(This,in_pTemplate,in_nPos)	\
    (This)->lpVtbl -> Add(This,in_pTemplate,in_nPos)

#define ITemplates_Remove(This,in_nIndex)	\
    (This)->lpVtbl -> Remove(This,in_nIndex)

#define ITemplates_CreateTemplate(This,out_ppTemplate)	\
    (This)->lpVtbl -> CreateTemplate(This,out_ppTemplate)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ITemplates_GetCount_Proxy( 
    ITemplates __RPC_FAR * This,
    /* [out] */ int __RPC_FAR *out_plCount);


void __RPC_STUB ITemplates_GetCount_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ITemplates_Item_Proxy( 
    ITemplates __RPC_FAR * This,
    /* [in] */ int in_nIndex,
    /* [out] */ ITemplate __RPC_FAR *__RPC_FAR *out_ppTemplate);


void __RPC_STUB ITemplates_Item_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ITemplates_Add_Proxy( 
    ITemplates __RPC_FAR * This,
    /* [in] */ ITemplate __RPC_FAR *in_pTemplate,
    /* [in] */ int in_nPos);


void __RPC_STUB ITemplates_Add_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ITemplates_Remove_Proxy( 
    ITemplates __RPC_FAR * This,
    /* [in] */ int in_nIndex);


void __RPC_STUB ITemplates_Remove_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ITemplates_CreateTemplate_Proxy( 
    ITemplates __RPC_FAR * This,
    /* [out] */ ITemplate __RPC_FAR *__RPC_FAR *out_ppTemplate);


void __RPC_STUB ITemplates_CreateTemplate_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __ITemplates_INTERFACE_DEFINED__ */


#ifndef __IdotXSIFileHandler_INTERFACE_DEFINED__
#define __IdotXSIFileHandler_INTERFACE_DEFINED__

/* interface IdotXSIFileHandler */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_IdotXSIFileHandler;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("5E61181E-3B0B-11D4-B77F-00A0C9EC5EA3")
    IdotXSIFileHandler : public IDispatch
    {
    public:
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Open( 
            /* [in] */ BSTR in_bstrFilename) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Create( 
            /* [in] */ BSTR in_bstrFilename,
            /* [in] */ eDotXSIFileFormat in_bOpenMode) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Close( void) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Read( void) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Write( void) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ReadString( void) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE WriteString( void) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE RootTemplates( 
            /* [out] */ ITemplates __RPC_FAR *__RPC_FAR *out_ppTemplates) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetFileVersionMajor( 
            /* [out] */ int __RPC_FAR *out_plVersionMajor) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetFileVersionMinor( 
            /* [out] */ int __RPC_FAR *out_plVersionMinor) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SetFileVersion( 
            /* [in] */ int in_lVersionMajor,
            /* [in] */ int in_lVersionMinor) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetNumVersion( 
            /* [out] */ int __RPC_FAR *out_plNumVersion) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetVersionInfo( 
            /* [in] */ int in_lIndex,
            /* [out] */ int __RPC_FAR *out_plMajor,
            /* [out] */ int __RPC_FAR *out_plMinor) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SetParser( 
            /* [in] */ int in_parser) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IdotXSIFileHandlerVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IdotXSIFileHandler __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IdotXSIFileHandler __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IdotXSIFileHandler __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            IdotXSIFileHandler __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            IdotXSIFileHandler __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            IdotXSIFileHandler __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            IdotXSIFileHandler __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Open )( 
            IdotXSIFileHandler __RPC_FAR * This,
            /* [in] */ BSTR in_bstrFilename);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Create )( 
            IdotXSIFileHandler __RPC_FAR * This,
            /* [in] */ BSTR in_bstrFilename,
            /* [in] */ eDotXSIFileFormat in_bOpenMode);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Close )( 
            IdotXSIFileHandler __RPC_FAR * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Read )( 
            IdotXSIFileHandler __RPC_FAR * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Write )( 
            IdotXSIFileHandler __RPC_FAR * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *ReadString )( 
            IdotXSIFileHandler __RPC_FAR * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *WriteString )( 
            IdotXSIFileHandler __RPC_FAR * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *RootTemplates )( 
            IdotXSIFileHandler __RPC_FAR * This,
            /* [out] */ ITemplates __RPC_FAR *__RPC_FAR *out_ppTemplates);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetFileVersionMajor )( 
            IdotXSIFileHandler __RPC_FAR * This,
            /* [out] */ int __RPC_FAR *out_plVersionMajor);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetFileVersionMinor )( 
            IdotXSIFileHandler __RPC_FAR * This,
            /* [out] */ int __RPC_FAR *out_plVersionMinor);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetFileVersion )( 
            IdotXSIFileHandler __RPC_FAR * This,
            /* [in] */ int in_lVersionMajor,
            /* [in] */ int in_lVersionMinor);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetNumVersion )( 
            IdotXSIFileHandler __RPC_FAR * This,
            /* [out] */ int __RPC_FAR *out_plNumVersion);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetVersionInfo )( 
            IdotXSIFileHandler __RPC_FAR * This,
            /* [in] */ int in_lIndex,
            /* [out] */ int __RPC_FAR *out_plMajor,
            /* [out] */ int __RPC_FAR *out_plMinor);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetParser )( 
            IdotXSIFileHandler __RPC_FAR * This,
            /* [in] */ int in_parser);
        
        END_INTERFACE
    } IdotXSIFileHandlerVtbl;

    interface IdotXSIFileHandler
    {
        CONST_VTBL struct IdotXSIFileHandlerVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IdotXSIFileHandler_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IdotXSIFileHandler_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IdotXSIFileHandler_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IdotXSIFileHandler_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IdotXSIFileHandler_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IdotXSIFileHandler_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IdotXSIFileHandler_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IdotXSIFileHandler_Open(This,in_bstrFilename)	\
    (This)->lpVtbl -> Open(This,in_bstrFilename)

#define IdotXSIFileHandler_Create(This,in_bstrFilename,in_bOpenMode)	\
    (This)->lpVtbl -> Create(This,in_bstrFilename,in_bOpenMode)

#define IdotXSIFileHandler_Close(This)	\
    (This)->lpVtbl -> Close(This)

#define IdotXSIFileHandler_Read(This)	\
    (This)->lpVtbl -> Read(This)

#define IdotXSIFileHandler_Write(This)	\
    (This)->lpVtbl -> Write(This)

#define IdotXSIFileHandler_ReadString(This)	\
    (This)->lpVtbl -> ReadString(This)

#define IdotXSIFileHandler_WriteString(This)	\
    (This)->lpVtbl -> WriteString(This)

#define IdotXSIFileHandler_RootTemplates(This,out_ppTemplates)	\
    (This)->lpVtbl -> RootTemplates(This,out_ppTemplates)

#define IdotXSIFileHandler_GetFileVersionMajor(This,out_plVersionMajor)	\
    (This)->lpVtbl -> GetFileVersionMajor(This,out_plVersionMajor)

#define IdotXSIFileHandler_GetFileVersionMinor(This,out_plVersionMinor)	\
    (This)->lpVtbl -> GetFileVersionMinor(This,out_plVersionMinor)

#define IdotXSIFileHandler_SetFileVersion(This,in_lVersionMajor,in_lVersionMinor)	\
    (This)->lpVtbl -> SetFileVersion(This,in_lVersionMajor,in_lVersionMinor)

#define IdotXSIFileHandler_GetNumVersion(This,out_plNumVersion)	\
    (This)->lpVtbl -> GetNumVersion(This,out_plNumVersion)

#define IdotXSIFileHandler_GetVersionInfo(This,in_lIndex,out_plMajor,out_plMinor)	\
    (This)->lpVtbl -> GetVersionInfo(This,in_lIndex,out_plMajor,out_plMinor)

#define IdotXSIFileHandler_SetParser(This,in_parser)	\
    (This)->lpVtbl -> SetParser(This,in_parser)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IdotXSIFileHandler_Open_Proxy( 
    IdotXSIFileHandler __RPC_FAR * This,
    /* [in] */ BSTR in_bstrFilename);


void __RPC_STUB IdotXSIFileHandler_Open_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IdotXSIFileHandler_Create_Proxy( 
    IdotXSIFileHandler __RPC_FAR * This,
    /* [in] */ BSTR in_bstrFilename,
    /* [in] */ eDotXSIFileFormat in_bOpenMode);


void __RPC_STUB IdotXSIFileHandler_Create_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IdotXSIFileHandler_Close_Proxy( 
    IdotXSIFileHandler __RPC_FAR * This);


void __RPC_STUB IdotXSIFileHandler_Close_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IdotXSIFileHandler_Read_Proxy( 
    IdotXSIFileHandler __RPC_FAR * This);


void __RPC_STUB IdotXSIFileHandler_Read_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IdotXSIFileHandler_Write_Proxy( 
    IdotXSIFileHandler __RPC_FAR * This);


void __RPC_STUB IdotXSIFileHandler_Write_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IdotXSIFileHandler_ReadString_Proxy( 
    IdotXSIFileHandler __RPC_FAR * This);


void __RPC_STUB IdotXSIFileHandler_ReadString_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IdotXSIFileHandler_WriteString_Proxy( 
    IdotXSIFileHandler __RPC_FAR * This);


void __RPC_STUB IdotXSIFileHandler_WriteString_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IdotXSIFileHandler_RootTemplates_Proxy( 
    IdotXSIFileHandler __RPC_FAR * This,
    /* [out] */ ITemplates __RPC_FAR *__RPC_FAR *out_ppTemplates);


void __RPC_STUB IdotXSIFileHandler_RootTemplates_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IdotXSIFileHandler_GetFileVersionMajor_Proxy( 
    IdotXSIFileHandler __RPC_FAR * This,
    /* [out] */ int __RPC_FAR *out_plVersionMajor);


void __RPC_STUB IdotXSIFileHandler_GetFileVersionMajor_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IdotXSIFileHandler_GetFileVersionMinor_Proxy( 
    IdotXSIFileHandler __RPC_FAR * This,
    /* [out] */ int __RPC_FAR *out_plVersionMinor);


void __RPC_STUB IdotXSIFileHandler_GetFileVersionMinor_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IdotXSIFileHandler_SetFileVersion_Proxy( 
    IdotXSIFileHandler __RPC_FAR * This,
    /* [in] */ int in_lVersionMajor,
    /* [in] */ int in_lVersionMinor);


void __RPC_STUB IdotXSIFileHandler_SetFileVersion_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IdotXSIFileHandler_GetNumVersion_Proxy( 
    IdotXSIFileHandler __RPC_FAR * This,
    /* [out] */ int __RPC_FAR *out_plNumVersion);


void __RPC_STUB IdotXSIFileHandler_GetNumVersion_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IdotXSIFileHandler_GetVersionInfo_Proxy( 
    IdotXSIFileHandler __RPC_FAR * This,
    /* [in] */ int in_lIndex,
    /* [out] */ int __RPC_FAR *out_plMajor,
    /* [out] */ int __RPC_FAR *out_plMinor);


void __RPC_STUB IdotXSIFileHandler_GetVersionInfo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IdotXSIFileHandler_SetParser_Proxy( 
    IdotXSIFileHandler __RPC_FAR * This,
    /* [in] */ int in_parser);


void __RPC_STUB IdotXSIFileHandler_SetParser_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IdotXSIFileHandler_INTERFACE_DEFINED__ */



#ifndef __DOTXSILib_LIBRARY_DEFINED__
#define __DOTXSILib_LIBRARY_DEFINED__

/* library DOTXSILib */
/* [helpstring][version][uuid] */ 


EXTERN_C const IID LIBID_DOTXSILib;

EXTERN_C const CLSID CLSID_Param;

#ifdef __cplusplus

class DECLSPEC_UUID("0F0AC51C-3AF9-11D4-B77F-00A0C9EC5EA3")
Param;
#endif

EXTERN_C const CLSID CLSID_Params;

#ifdef __cplusplus

class DECLSPEC_UUID("0F0AC51E-3AF9-11D4-B77F-00A0C9EC5EA3")
Params;
#endif

EXTERN_C const CLSID CLSID_Template;

#ifdef __cplusplus

class DECLSPEC_UUID("0F0AC520-3AF9-11D4-B77F-00A0C9EC5EA3")
Template;
#endif

EXTERN_C const CLSID CLSID_Templates;

#ifdef __cplusplus

class DECLSPEC_UUID("5E61181C-3B0B-11D4-B77F-00A0C9EC5EA3")
Templates;
#endif

EXTERN_C const CLSID CLSID_dotXSIFileHandler;

#ifdef __cplusplus

class DECLSPEC_UUID("5ADB71B8-5F53-11d5-8983-00D0B71DA41F")
dotXSIFileHandler;
#endif
#endif /* __DOTXSILib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

ULONG             __RPC_USER  BSTR_UserSize(     ULONG __RPC_FAR *, ULONG            , BSTR __RPC_FAR * ); 
unsigned char __RPC_FAR * __RPC_USER  BSTR_UserMarshal(  ULONG __RPC_FAR *, unsigned char __RPC_FAR *, BSTR __RPC_FAR * ); 
unsigned char __RPC_FAR * __RPC_USER  BSTR_UserUnmarshal(ULONG __RPC_FAR *, unsigned char __RPC_FAR *, BSTR __RPC_FAR * ); 
void                      __RPC_USER  BSTR_UserFree(     ULONG __RPC_FAR *, BSTR __RPC_FAR * ); 

ULONG             __RPC_USER  VARIANT_UserSize(     ULONG __RPC_FAR *, ULONG            , VARIANT __RPC_FAR * ); 
unsigned char __RPC_FAR * __RPC_USER  VARIANT_UserMarshal(  ULONG __RPC_FAR *, unsigned char __RPC_FAR *, VARIANT __RPC_FAR * ); 
unsigned char __RPC_FAR * __RPC_USER  VARIANT_UserUnmarshal(ULONG __RPC_FAR *, unsigned char __RPC_FAR *, VARIANT __RPC_FAR * ); 
void                      __RPC_USER  VARIANT_UserFree(     ULONG __RPC_FAR *, VARIANT __RPC_FAR * ); 

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif

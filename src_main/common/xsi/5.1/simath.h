

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 6.00.0361 */
/* at Wed Mar 22 20:59:17 2006
 */
/* Compiler settings for SIMath.idl:
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

#ifndef __simath_h__
#define __simath_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __XSIMath_FWD_DEFINED__
#define __XSIMath_FWD_DEFINED__
typedef interface XSIMath XSIMath;
#endif 	/* __XSIMath_FWD_DEFINED__ */


#ifndef __ISIBoundingVolume_FWD_DEFINED__
#define __ISIBoundingVolume_FWD_DEFINED__
typedef interface ISIBoundingVolume ISIBoundingVolume;
#endif 	/* __ISIBoundingVolume_FWD_DEFINED__ */


#ifndef __SIVector3_FWD_DEFINED__
#define __SIVector3_FWD_DEFINED__

#ifdef __cplusplus
typedef class SIVector3 SIVector3;
#else
typedef struct SIVector3 SIVector3;
#endif /* __cplusplus */

#endif 	/* __SIVector3_FWD_DEFINED__ */


#ifndef __SIMatrix3_FWD_DEFINED__
#define __SIMatrix3_FWD_DEFINED__

#ifdef __cplusplus
typedef class SIMatrix3 SIMatrix3;
#else
typedef struct SIMatrix3 SIMatrix3;
#endif /* __cplusplus */

#endif 	/* __SIMatrix3_FWD_DEFINED__ */


#ifndef __SIMatrix4_FWD_DEFINED__
#define __SIMatrix4_FWD_DEFINED__

#ifdef __cplusplus
typedef class SIMatrix4 SIMatrix4;
#else
typedef struct SIMatrix4 SIMatrix4;
#endif /* __cplusplus */

#endif 	/* __SIMatrix4_FWD_DEFINED__ */


#ifndef __SIRotation_FWD_DEFINED__
#define __SIRotation_FWD_DEFINED__

#ifdef __cplusplus
typedef class SIRotation SIRotation;
#else
typedef struct SIRotation SIRotation;
#endif /* __cplusplus */

#endif 	/* __SIRotation_FWD_DEFINED__ */


#ifndef __SIQuaternion_FWD_DEFINED__
#define __SIQuaternion_FWD_DEFINED__

#ifdef __cplusplus
typedef class SIQuaternion SIQuaternion;
#else
typedef struct SIQuaternion SIQuaternion;
#endif /* __cplusplus */

#endif 	/* __SIQuaternion_FWD_DEFINED__ */


#ifndef __SITransformation_FWD_DEFINED__
#define __SITransformation_FWD_DEFINED__

#ifdef __cplusplus
typedef class SITransformation SITransformation;
#else
typedef struct SITransformation SITransformation;
#endif /* __cplusplus */

#endif 	/* __SITransformation_FWD_DEFINED__ */


#ifndef __CXSIMath_FWD_DEFINED__
#define __CXSIMath_FWD_DEFINED__

#ifdef __cplusplus
typedef class CXSIMath CXSIMath;
#else
typedef struct CXSIMath CXSIMath;
#endif /* __cplusplus */

#endif 	/* __CXSIMath_FWD_DEFINED__ */


#ifndef __ISIVector3_FWD_DEFINED__
#define __ISIVector3_FWD_DEFINED__
typedef interface ISIVector3 ISIVector3;
#endif 	/* __ISIVector3_FWD_DEFINED__ */


#ifndef __ISIMatrix3_FWD_DEFINED__
#define __ISIMatrix3_FWD_DEFINED__
typedef interface ISIMatrix3 ISIMatrix3;
#endif 	/* __ISIMatrix3_FWD_DEFINED__ */


#ifndef __ISIMatrix4_FWD_DEFINED__
#define __ISIMatrix4_FWD_DEFINED__
typedef interface ISIMatrix4 ISIMatrix4;
#endif 	/* __ISIMatrix4_FWD_DEFINED__ */


#ifndef __ISIQuaternion_FWD_DEFINED__
#define __ISIQuaternion_FWD_DEFINED__
typedef interface ISIQuaternion ISIQuaternion;
#endif 	/* __ISIQuaternion_FWD_DEFINED__ */


#ifndef __ISIRotation_FWD_DEFINED__
#define __ISIRotation_FWD_DEFINED__
typedef interface ISIRotation ISIRotation;
#endif 	/* __ISIRotation_FWD_DEFINED__ */


#ifndef __ISITransformation_FWD_DEFINED__
#define __ISITransformation_FWD_DEFINED__
typedef interface ISITransformation ISITransformation;
#endif 	/* __ISITransformation_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"

#ifdef __cplusplus
extern "C"{
#endif 

void * __RPC_USER MIDL_user_allocate(size_t);
void __RPC_USER MIDL_user_free( void * ); 

/* interface __MIDL_itf_SIMath_0000 */
/* [local] */ 











extern RPC_IF_HANDLE __MIDL_itf_SIMath_0000_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_SIMath_0000_v0_0_s_ifspec;

#ifndef __XSIMath_INTERFACE_DEFINED__
#define __XSIMath_INTERFACE_DEFINED__

/* interface XSIMath */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_XSIMath;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("927A181C-A441-419e-9A9A-86FD93DF288B")
    XSIMath : public IDispatch
    {
    public:
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_PI( 
            /* [retval][out] */ double *pVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE CreateQuaternion( 
            /* [optional][in] */ VARIANT in_dW,
            /* [optional][in] */ VARIANT in_dX,
            /* [optional][in] */ VARIANT in_dY,
            /* [optional][in] */ VARIANT in_dZ,
            /* [retval][out] */ ISIQuaternion **out_ppQuaternion) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE CreateMatrix4( 
            /* [optional][in] */ VARIANT in_d0,
            /* [optional][in] */ VARIANT in_d1,
            /* [optional][in] */ VARIANT in_d2,
            /* [optional][in] */ VARIANT in_d3,
            /* [optional][in] */ VARIANT in_d4,
            /* [optional][in] */ VARIANT in_d5,
            /* [optional][in] */ VARIANT in_d6,
            /* [optional][in] */ VARIANT in_d7,
            /* [optional][in] */ VARIANT in_d8,
            /* [optional][in] */ VARIANT in_d9,
            /* [optional][in] */ VARIANT in_d10,
            /* [optional][in] */ VARIANT in_d11,
            /* [optional][in] */ VARIANT in_d12,
            /* [optional][in] */ VARIANT in_d13,
            /* [optional][in] */ VARIANT in_d14,
            /* [optional][in] */ VARIANT in_d15,
            /* [retval][out] */ ISIMatrix4 **out_ppMatrix4) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE CreateMatrix3( 
            /* [optional][in] */ VARIANT in_d0,
            /* [optional][in] */ VARIANT in_d1,
            /* [optional][in] */ VARIANT in_d2,
            /* [optional][in] */ VARIANT in_d3,
            /* [optional][in] */ VARIANT in_d4,
            /* [optional][in] */ VARIANT in_d5,
            /* [optional][in] */ VARIANT in_d6,
            /* [optional][in] */ VARIANT in_d7,
            /* [optional][in] */ VARIANT in_d8,
            /* [retval][out] */ ISIMatrix3 **out_ppMatrix3) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE CreateVector3( 
            /* [optional][in] */ VARIANT in_dX,
            /* [optional][in] */ VARIANT in_dY,
            /* [optional][in] */ VARIANT in_dZ,
            /* [retval][out] */ ISIVector3 **out_ppVector3) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE CreateTransform( 
            /* [retval][out] */ ISITransformation **out_ppTransformation) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE CreateBoundingVolume( 
            /* [retval][out] */ ISIBoundingVolume **out_ppBoundingVolume) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE CreateRotation( 
            /* [optional][in] */ VARIANT in_dX,
            /* [optional][in] */ VARIANT in_dY,
            /* [optional][in] */ VARIANT in_dZ,
            /* [retval][out] */ ISIRotation **out_ppRotation) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE DegreesToRadians( 
            /* [in] */ double in_deg,
            /* [retval][out] */ double *out_pRad) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE RadiansToDegrees( 
            /* [in] */ double in_rad,
            /* [retval][out] */ double *out_pRad) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE MapObjectPoseToWorldSpace( 
            /* [in] */ ISITransformation *in_pObjectSpace,
            /* [in] */ ISITransformation *in_pPose,
            /* [retval][out] */ ISITransformation **out_ppRetVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE MapObjectOrientationToWorldSpace( 
            /* [in] */ ISITransformation *in_pObjectSpace,
            /* [in] */ ISIVector3 *in_pOrientation,
            /* [retval][out] */ ISIVector3 **out_ppRetVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE MapObjectPositionToWorldSpace( 
            /* [in] */ ISITransformation *in_pObjectSpace,
            /* [in] */ ISIVector3 *in_pPosition,
            /* [retval][out] */ ISIVector3 **out_ppRetVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE MapWorldPoseToObjectSpace( 
            /* [in] */ ISITransformation *in_pObjectSpace,
            /* [in] */ ISITransformation *in_pPose,
            /* [retval][out] */ ISITransformation **out_ppRetVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE MapWorldOrientationToObjectSpace( 
            /* [in] */ ISITransformation *in_pObjectSapce,
            /* [in] */ ISIVector3 *in_pOrientation,
            /* [retval][out] */ ISIVector3 **out_ppRetVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE MapWorldPositionToObjectSpace( 
            /* [in] */ ISITransformation *in_pObjectSpace,
            /* [in] */ ISIVector3 *in_pPosition,
            /* [retval][out] */ ISIVector3 **out_ppRetVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE MapObjectPoseToObjectSpace( 
            /* [in] */ ISITransformation *in_pObjectSpace,
            /* [in] */ ISITransformation *in_pSpace,
            /* [in] */ ISITransformation *in_pPose,
            /* [retval][out] */ ISITransformation **out_ppRetVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE MapObjectOrientationToObjectSpace( 
            /* [in] */ ISITransformation *in_pObjectSpace,
            /* [in] */ ISITransformation *in_pSpace,
            /* [in] */ ISIVector3 *in_pOrientation,
            /* [retval][out] */ ISIVector3 **out_ppRetVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE MapObjectPositionToObjectSpace( 
            /* [in] */ ISITransformation *in_pObjectSpace,
            /* [in] */ ISITransformation *in_pSpace,
            /* [in] */ ISIVector3 *in_pPosition,
            /* [retval][out] */ ISIVector3 **out_ppRetVal) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct XSIMathVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            XSIMath * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            XSIMath * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            XSIMath * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            XSIMath * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            XSIMath * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            XSIMath * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            XSIMath * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_PI )( 
            XSIMath * This,
            /* [retval][out] */ double *pVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *CreateQuaternion )( 
            XSIMath * This,
            /* [optional][in] */ VARIANT in_dW,
            /* [optional][in] */ VARIANT in_dX,
            /* [optional][in] */ VARIANT in_dY,
            /* [optional][in] */ VARIANT in_dZ,
            /* [retval][out] */ ISIQuaternion **out_ppQuaternion);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *CreateMatrix4 )( 
            XSIMath * This,
            /* [optional][in] */ VARIANT in_d0,
            /* [optional][in] */ VARIANT in_d1,
            /* [optional][in] */ VARIANT in_d2,
            /* [optional][in] */ VARIANT in_d3,
            /* [optional][in] */ VARIANT in_d4,
            /* [optional][in] */ VARIANT in_d5,
            /* [optional][in] */ VARIANT in_d6,
            /* [optional][in] */ VARIANT in_d7,
            /* [optional][in] */ VARIANT in_d8,
            /* [optional][in] */ VARIANT in_d9,
            /* [optional][in] */ VARIANT in_d10,
            /* [optional][in] */ VARIANT in_d11,
            /* [optional][in] */ VARIANT in_d12,
            /* [optional][in] */ VARIANT in_d13,
            /* [optional][in] */ VARIANT in_d14,
            /* [optional][in] */ VARIANT in_d15,
            /* [retval][out] */ ISIMatrix4 **out_ppMatrix4);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *CreateMatrix3 )( 
            XSIMath * This,
            /* [optional][in] */ VARIANT in_d0,
            /* [optional][in] */ VARIANT in_d1,
            /* [optional][in] */ VARIANT in_d2,
            /* [optional][in] */ VARIANT in_d3,
            /* [optional][in] */ VARIANT in_d4,
            /* [optional][in] */ VARIANT in_d5,
            /* [optional][in] */ VARIANT in_d6,
            /* [optional][in] */ VARIANT in_d7,
            /* [optional][in] */ VARIANT in_d8,
            /* [retval][out] */ ISIMatrix3 **out_ppMatrix3);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *CreateVector3 )( 
            XSIMath * This,
            /* [optional][in] */ VARIANT in_dX,
            /* [optional][in] */ VARIANT in_dY,
            /* [optional][in] */ VARIANT in_dZ,
            /* [retval][out] */ ISIVector3 **out_ppVector3);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *CreateTransform )( 
            XSIMath * This,
            /* [retval][out] */ ISITransformation **out_ppTransformation);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *CreateBoundingVolume )( 
            XSIMath * This,
            /* [retval][out] */ ISIBoundingVolume **out_ppBoundingVolume);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *CreateRotation )( 
            XSIMath * This,
            /* [optional][in] */ VARIANT in_dX,
            /* [optional][in] */ VARIANT in_dY,
            /* [optional][in] */ VARIANT in_dZ,
            /* [retval][out] */ ISIRotation **out_ppRotation);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *DegreesToRadians )( 
            XSIMath * This,
            /* [in] */ double in_deg,
            /* [retval][out] */ double *out_pRad);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *RadiansToDegrees )( 
            XSIMath * This,
            /* [in] */ double in_rad,
            /* [retval][out] */ double *out_pRad);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *MapObjectPoseToWorldSpace )( 
            XSIMath * This,
            /* [in] */ ISITransformation *in_pObjectSpace,
            /* [in] */ ISITransformation *in_pPose,
            /* [retval][out] */ ISITransformation **out_ppRetVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *MapObjectOrientationToWorldSpace )( 
            XSIMath * This,
            /* [in] */ ISITransformation *in_pObjectSpace,
            /* [in] */ ISIVector3 *in_pOrientation,
            /* [retval][out] */ ISIVector3 **out_ppRetVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *MapObjectPositionToWorldSpace )( 
            XSIMath * This,
            /* [in] */ ISITransformation *in_pObjectSpace,
            /* [in] */ ISIVector3 *in_pPosition,
            /* [retval][out] */ ISIVector3 **out_ppRetVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *MapWorldPoseToObjectSpace )( 
            XSIMath * This,
            /* [in] */ ISITransformation *in_pObjectSpace,
            /* [in] */ ISITransformation *in_pPose,
            /* [retval][out] */ ISITransformation **out_ppRetVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *MapWorldOrientationToObjectSpace )( 
            XSIMath * This,
            /* [in] */ ISITransformation *in_pObjectSapce,
            /* [in] */ ISIVector3 *in_pOrientation,
            /* [retval][out] */ ISIVector3 **out_ppRetVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *MapWorldPositionToObjectSpace )( 
            XSIMath * This,
            /* [in] */ ISITransformation *in_pObjectSpace,
            /* [in] */ ISIVector3 *in_pPosition,
            /* [retval][out] */ ISIVector3 **out_ppRetVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *MapObjectPoseToObjectSpace )( 
            XSIMath * This,
            /* [in] */ ISITransformation *in_pObjectSpace,
            /* [in] */ ISITransformation *in_pSpace,
            /* [in] */ ISITransformation *in_pPose,
            /* [retval][out] */ ISITransformation **out_ppRetVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *MapObjectOrientationToObjectSpace )( 
            XSIMath * This,
            /* [in] */ ISITransformation *in_pObjectSpace,
            /* [in] */ ISITransformation *in_pSpace,
            /* [in] */ ISIVector3 *in_pOrientation,
            /* [retval][out] */ ISIVector3 **out_ppRetVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *MapObjectPositionToObjectSpace )( 
            XSIMath * This,
            /* [in] */ ISITransformation *in_pObjectSpace,
            /* [in] */ ISITransformation *in_pSpace,
            /* [in] */ ISIVector3 *in_pPosition,
            /* [retval][out] */ ISIVector3 **out_ppRetVal);
        
        END_INTERFACE
    } XSIMathVtbl;

    interface XSIMath
    {
        CONST_VTBL struct XSIMathVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define XSIMath_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define XSIMath_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define XSIMath_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define XSIMath_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define XSIMath_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define XSIMath_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define XSIMath_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define XSIMath_get_PI(This,pVal)	\
    (This)->lpVtbl -> get_PI(This,pVal)

#define XSIMath_CreateQuaternion(This,in_dW,in_dX,in_dY,in_dZ,out_ppQuaternion)	\
    (This)->lpVtbl -> CreateQuaternion(This,in_dW,in_dX,in_dY,in_dZ,out_ppQuaternion)

#define XSIMath_CreateMatrix4(This,in_d0,in_d1,in_d2,in_d3,in_d4,in_d5,in_d6,in_d7,in_d8,in_d9,in_d10,in_d11,in_d12,in_d13,in_d14,in_d15,out_ppMatrix4)	\
    (This)->lpVtbl -> CreateMatrix4(This,in_d0,in_d1,in_d2,in_d3,in_d4,in_d5,in_d6,in_d7,in_d8,in_d9,in_d10,in_d11,in_d12,in_d13,in_d14,in_d15,out_ppMatrix4)

#define XSIMath_CreateMatrix3(This,in_d0,in_d1,in_d2,in_d3,in_d4,in_d5,in_d6,in_d7,in_d8,out_ppMatrix3)	\
    (This)->lpVtbl -> CreateMatrix3(This,in_d0,in_d1,in_d2,in_d3,in_d4,in_d5,in_d6,in_d7,in_d8,out_ppMatrix3)

#define XSIMath_CreateVector3(This,in_dX,in_dY,in_dZ,out_ppVector3)	\
    (This)->lpVtbl -> CreateVector3(This,in_dX,in_dY,in_dZ,out_ppVector3)

#define XSIMath_CreateTransform(This,out_ppTransformation)	\
    (This)->lpVtbl -> CreateTransform(This,out_ppTransformation)

#define XSIMath_CreateBoundingVolume(This,out_ppBoundingVolume)	\
    (This)->lpVtbl -> CreateBoundingVolume(This,out_ppBoundingVolume)

#define XSIMath_CreateRotation(This,in_dX,in_dY,in_dZ,out_ppRotation)	\
    (This)->lpVtbl -> CreateRotation(This,in_dX,in_dY,in_dZ,out_ppRotation)

#define XSIMath_DegreesToRadians(This,in_deg,out_pRad)	\
    (This)->lpVtbl -> DegreesToRadians(This,in_deg,out_pRad)

#define XSIMath_RadiansToDegrees(This,in_rad,out_pRad)	\
    (This)->lpVtbl -> RadiansToDegrees(This,in_rad,out_pRad)

#define XSIMath_MapObjectPoseToWorldSpace(This,in_pObjectSpace,in_pPose,out_ppRetVal)	\
    (This)->lpVtbl -> MapObjectPoseToWorldSpace(This,in_pObjectSpace,in_pPose,out_ppRetVal)

#define XSIMath_MapObjectOrientationToWorldSpace(This,in_pObjectSpace,in_pOrientation,out_ppRetVal)	\
    (This)->lpVtbl -> MapObjectOrientationToWorldSpace(This,in_pObjectSpace,in_pOrientation,out_ppRetVal)

#define XSIMath_MapObjectPositionToWorldSpace(This,in_pObjectSpace,in_pPosition,out_ppRetVal)	\
    (This)->lpVtbl -> MapObjectPositionToWorldSpace(This,in_pObjectSpace,in_pPosition,out_ppRetVal)

#define XSIMath_MapWorldPoseToObjectSpace(This,in_pObjectSpace,in_pPose,out_ppRetVal)	\
    (This)->lpVtbl -> MapWorldPoseToObjectSpace(This,in_pObjectSpace,in_pPose,out_ppRetVal)

#define XSIMath_MapWorldOrientationToObjectSpace(This,in_pObjectSapce,in_pOrientation,out_ppRetVal)	\
    (This)->lpVtbl -> MapWorldOrientationToObjectSpace(This,in_pObjectSapce,in_pOrientation,out_ppRetVal)

#define XSIMath_MapWorldPositionToObjectSpace(This,in_pObjectSpace,in_pPosition,out_ppRetVal)	\
    (This)->lpVtbl -> MapWorldPositionToObjectSpace(This,in_pObjectSpace,in_pPosition,out_ppRetVal)

#define XSIMath_MapObjectPoseToObjectSpace(This,in_pObjectSpace,in_pSpace,in_pPose,out_ppRetVal)	\
    (This)->lpVtbl -> MapObjectPoseToObjectSpace(This,in_pObjectSpace,in_pSpace,in_pPose,out_ppRetVal)

#define XSIMath_MapObjectOrientationToObjectSpace(This,in_pObjectSpace,in_pSpace,in_pOrientation,out_ppRetVal)	\
    (This)->lpVtbl -> MapObjectOrientationToObjectSpace(This,in_pObjectSpace,in_pSpace,in_pOrientation,out_ppRetVal)

#define XSIMath_MapObjectPositionToObjectSpace(This,in_pObjectSpace,in_pSpace,in_pPosition,out_ppRetVal)	\
    (This)->lpVtbl -> MapObjectPositionToObjectSpace(This,in_pObjectSpace,in_pSpace,in_pPosition,out_ppRetVal)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE XSIMath_get_PI_Proxy( 
    XSIMath * This,
    /* [retval][out] */ double *pVal);


void __RPC_STUB XSIMath_get_PI_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE XSIMath_CreateQuaternion_Proxy( 
    XSIMath * This,
    /* [optional][in] */ VARIANT in_dW,
    /* [optional][in] */ VARIANT in_dX,
    /* [optional][in] */ VARIANT in_dY,
    /* [optional][in] */ VARIANT in_dZ,
    /* [retval][out] */ ISIQuaternion **out_ppQuaternion);


void __RPC_STUB XSIMath_CreateQuaternion_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE XSIMath_CreateMatrix4_Proxy( 
    XSIMath * This,
    /* [optional][in] */ VARIANT in_d0,
    /* [optional][in] */ VARIANT in_d1,
    /* [optional][in] */ VARIANT in_d2,
    /* [optional][in] */ VARIANT in_d3,
    /* [optional][in] */ VARIANT in_d4,
    /* [optional][in] */ VARIANT in_d5,
    /* [optional][in] */ VARIANT in_d6,
    /* [optional][in] */ VARIANT in_d7,
    /* [optional][in] */ VARIANT in_d8,
    /* [optional][in] */ VARIANT in_d9,
    /* [optional][in] */ VARIANT in_d10,
    /* [optional][in] */ VARIANT in_d11,
    /* [optional][in] */ VARIANT in_d12,
    /* [optional][in] */ VARIANT in_d13,
    /* [optional][in] */ VARIANT in_d14,
    /* [optional][in] */ VARIANT in_d15,
    /* [retval][out] */ ISIMatrix4 **out_ppMatrix4);


void __RPC_STUB XSIMath_CreateMatrix4_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE XSIMath_CreateMatrix3_Proxy( 
    XSIMath * This,
    /* [optional][in] */ VARIANT in_d0,
    /* [optional][in] */ VARIANT in_d1,
    /* [optional][in] */ VARIANT in_d2,
    /* [optional][in] */ VARIANT in_d3,
    /* [optional][in] */ VARIANT in_d4,
    /* [optional][in] */ VARIANT in_d5,
    /* [optional][in] */ VARIANT in_d6,
    /* [optional][in] */ VARIANT in_d7,
    /* [optional][in] */ VARIANT in_d8,
    /* [retval][out] */ ISIMatrix3 **out_ppMatrix3);


void __RPC_STUB XSIMath_CreateMatrix3_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE XSIMath_CreateVector3_Proxy( 
    XSIMath * This,
    /* [optional][in] */ VARIANT in_dX,
    /* [optional][in] */ VARIANT in_dY,
    /* [optional][in] */ VARIANT in_dZ,
    /* [retval][out] */ ISIVector3 **out_ppVector3);


void __RPC_STUB XSIMath_CreateVector3_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE XSIMath_CreateTransform_Proxy( 
    XSIMath * This,
    /* [retval][out] */ ISITransformation **out_ppTransformation);


void __RPC_STUB XSIMath_CreateTransform_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE XSIMath_CreateBoundingVolume_Proxy( 
    XSIMath * This,
    /* [retval][out] */ ISIBoundingVolume **out_ppBoundingVolume);


void __RPC_STUB XSIMath_CreateBoundingVolume_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE XSIMath_CreateRotation_Proxy( 
    XSIMath * This,
    /* [optional][in] */ VARIANT in_dX,
    /* [optional][in] */ VARIANT in_dY,
    /* [optional][in] */ VARIANT in_dZ,
    /* [retval][out] */ ISIRotation **out_ppRotation);


void __RPC_STUB XSIMath_CreateRotation_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE XSIMath_DegreesToRadians_Proxy( 
    XSIMath * This,
    /* [in] */ double in_deg,
    /* [retval][out] */ double *out_pRad);


void __RPC_STUB XSIMath_DegreesToRadians_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE XSIMath_RadiansToDegrees_Proxy( 
    XSIMath * This,
    /* [in] */ double in_rad,
    /* [retval][out] */ double *out_pRad);


void __RPC_STUB XSIMath_RadiansToDegrees_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE XSIMath_MapObjectPoseToWorldSpace_Proxy( 
    XSIMath * This,
    /* [in] */ ISITransformation *in_pObjectSpace,
    /* [in] */ ISITransformation *in_pPose,
    /* [retval][out] */ ISITransformation **out_ppRetVal);


void __RPC_STUB XSIMath_MapObjectPoseToWorldSpace_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE XSIMath_MapObjectOrientationToWorldSpace_Proxy( 
    XSIMath * This,
    /* [in] */ ISITransformation *in_pObjectSpace,
    /* [in] */ ISIVector3 *in_pOrientation,
    /* [retval][out] */ ISIVector3 **out_ppRetVal);


void __RPC_STUB XSIMath_MapObjectOrientationToWorldSpace_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE XSIMath_MapObjectPositionToWorldSpace_Proxy( 
    XSIMath * This,
    /* [in] */ ISITransformation *in_pObjectSpace,
    /* [in] */ ISIVector3 *in_pPosition,
    /* [retval][out] */ ISIVector3 **out_ppRetVal);


void __RPC_STUB XSIMath_MapObjectPositionToWorldSpace_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE XSIMath_MapWorldPoseToObjectSpace_Proxy( 
    XSIMath * This,
    /* [in] */ ISITransformation *in_pObjectSpace,
    /* [in] */ ISITransformation *in_pPose,
    /* [retval][out] */ ISITransformation **out_ppRetVal);


void __RPC_STUB XSIMath_MapWorldPoseToObjectSpace_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE XSIMath_MapWorldOrientationToObjectSpace_Proxy( 
    XSIMath * This,
    /* [in] */ ISITransformation *in_pObjectSapce,
    /* [in] */ ISIVector3 *in_pOrientation,
    /* [retval][out] */ ISIVector3 **out_ppRetVal);


void __RPC_STUB XSIMath_MapWorldOrientationToObjectSpace_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE XSIMath_MapWorldPositionToObjectSpace_Proxy( 
    XSIMath * This,
    /* [in] */ ISITransformation *in_pObjectSpace,
    /* [in] */ ISIVector3 *in_pPosition,
    /* [retval][out] */ ISIVector3 **out_ppRetVal);


void __RPC_STUB XSIMath_MapWorldPositionToObjectSpace_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE XSIMath_MapObjectPoseToObjectSpace_Proxy( 
    XSIMath * This,
    /* [in] */ ISITransformation *in_pObjectSpace,
    /* [in] */ ISITransformation *in_pSpace,
    /* [in] */ ISITransformation *in_pPose,
    /* [retval][out] */ ISITransformation **out_ppRetVal);


void __RPC_STUB XSIMath_MapObjectPoseToObjectSpace_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE XSIMath_MapObjectOrientationToObjectSpace_Proxy( 
    XSIMath * This,
    /* [in] */ ISITransformation *in_pObjectSpace,
    /* [in] */ ISITransformation *in_pSpace,
    /* [in] */ ISIVector3 *in_pOrientation,
    /* [retval][out] */ ISIVector3 **out_ppRetVal);


void __RPC_STUB XSIMath_MapObjectOrientationToObjectSpace_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE XSIMath_MapObjectPositionToObjectSpace_Proxy( 
    XSIMath * This,
    /* [in] */ ISITransformation *in_pObjectSpace,
    /* [in] */ ISITransformation *in_pSpace,
    /* [in] */ ISIVector3 *in_pPosition,
    /* [retval][out] */ ISIVector3 **out_ppRetVal);


void __RPC_STUB XSIMath_MapObjectPositionToObjectSpace_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __XSIMath_INTERFACE_DEFINED__ */


#ifndef __ISIBoundingVolume_INTERFACE_DEFINED__
#define __ISIBoundingVolume_INTERFACE_DEFINED__

/* interface ISIBoundingVolume */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_ISIBoundingVolume;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("21B2A66D-266E-4b43-9919-655E9508D8E6")
    ISIBoundingVolume : public IDispatch
    {
    public:
    };
    
#else 	/* C style interface */

    typedef struct ISIBoundingVolumeVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            ISIBoundingVolume * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            ISIBoundingVolume * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            ISIBoundingVolume * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            ISIBoundingVolume * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            ISIBoundingVolume * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            ISIBoundingVolume * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            ISIBoundingVolume * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        END_INTERFACE
    } ISIBoundingVolumeVtbl;

    interface ISIBoundingVolume
    {
        CONST_VTBL struct ISIBoundingVolumeVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ISIBoundingVolume_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define ISIBoundingVolume_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define ISIBoundingVolume_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define ISIBoundingVolume_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define ISIBoundingVolume_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define ISIBoundingVolume_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define ISIBoundingVolume_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __ISIBoundingVolume_INTERFACE_DEFINED__ */



#ifndef __SIMATHLib_LIBRARY_DEFINED__
#define __SIMATHLib_LIBRARY_DEFINED__

/* library SIMATHLib */
/* [helpstring][version][uuid] */ 

typedef /* [helpstring] */ 
enum siPleaseLoadMeType
    {	siPleaseLoadMe	= -1
    } 	siPleaseLoadMeType;


EXTERN_C const IID LIBID_SIMATHLib;

EXTERN_C const CLSID CLSID_SIVector3;

#ifdef __cplusplus

class DECLSPEC_UUID("0222732F-232E-11D3-BC3A-00A0C9612B97")
SIVector3;
#endif

EXTERN_C const CLSID CLSID_SIMatrix3;

#ifdef __cplusplus

class DECLSPEC_UUID("BFB3B914-24CC-11D3-BC3A-00A0C9612B97")
SIMatrix3;
#endif

EXTERN_C const CLSID CLSID_SIMatrix4;

#ifdef __cplusplus

class DECLSPEC_UUID("BFB3B916-24CC-11D3-BC3A-00A0C9612B97")
SIMatrix4;
#endif

EXTERN_C const CLSID CLSID_SIRotation;

#ifdef __cplusplus

class DECLSPEC_UUID("BFB3B918-24CC-11D3-BC3A-00A0C9612B97")
SIRotation;
#endif

EXTERN_C const CLSID CLSID_SIQuaternion;

#ifdef __cplusplus

class DECLSPEC_UUID("BFB3B91A-24CC-11D3-BC3A-00A0C9612B97")
SIQuaternion;
#endif

EXTERN_C const CLSID CLSID_SITransformation;

#ifdef __cplusplus

class DECLSPEC_UUID("BFB3B91C-24CC-11D3-BC3A-00A0C9612B97")
SITransformation;
#endif

EXTERN_C const CLSID CLSID_CXSIMath;

#ifdef __cplusplus

class DECLSPEC_UUID("8EB8041B-11F5-4f56-9FEB-6461811BA1B3")
CXSIMath;
#endif
#endif /* __SIMATHLib_LIBRARY_DEFINED__ */

#ifndef __ISIVector3_INTERFACE_DEFINED__
#define __ISIVector3_INTERFACE_DEFINED__

/* interface ISIVector3 */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_ISIVector3;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("0222732E-232E-11D3-BC3A-00A0C9612B97")
    ISIVector3 : public IDispatch
    {
    public:
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Value( 
            /* [in] */ short in_nIndex,
            /* [retval][out] */ double *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_Value( 
            /* [in] */ short in_nIndex,
            /* [in] */ double newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_X( 
            /* [retval][out] */ double *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_X( 
            /* [in] */ double newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Y( 
            /* [retval][out] */ double *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_Y( 
            /* [in] */ double newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Z( 
            /* [retval][out] */ double *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_Z( 
            /* [in] */ double newVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Copy( 
            /* [in] */ ISIVector3 *in_pVector) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Set( 
            /* [in] */ double in_dXValue,
            /* [in] */ double in_dYValue,
            /* [in] */ double in_dZValue) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Add( 
            /* [in] */ ISIVector3 *in_pVector1,
            /* [in] */ ISIVector3 *in_pVector2) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE AddInPlace( 
            /* [in] */ ISIVector3 *in_pVector) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Sub( 
            /* [in] */ ISIVector3 *in_pVector1,
            /* [in] */ ISIVector3 *in_pVector2) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SubInPlace( 
            /* [in] */ ISIVector3 *in_pVector) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Negate( 
            /* [in] */ ISIVector3 *in_pVector) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE NegateInPlace( void) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Scale( 
            /* [in] */ double in_dScalar,
            /* [in] */ ISIVector3 *in_pVector) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ScaleInPlace( 
            /* [in] */ double in_dScalar) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ScaleAdd( 
            /* [in] */ double in_dScalar,
            /* [in] */ ISIVector3 *in_pVector1,
            /* [in] */ ISIVector3 *in_pVector2) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ScaleAddInPlace( 
            /* [in] */ double in_dScalar,
            /* [in] */ ISIVector3 *in_pVector) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Equals( 
            /* [in] */ ISIVector3 *in_pVector,
            /* [retval][out] */ BOOL *out_pbEquality) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE EpsilonEquals( 
            /* [in] */ ISIVector3 *in_pVector,
            /* [in] */ double in_dEpsilon,
            /* [retval][out] */ BOOL *out_pbEquality) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Clamp( 
            /* [in] */ double in_dMin,
            /* [in] */ double in_dMax,
            /* [in] */ ISIVector3 *in_pVector) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ClampInPlace( 
            /* [in] */ double in_dMin,
            /* [in] */ double in_dMax) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Absolute( 
            /* [in] */ ISIVector3 *in_pVector) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE AbsoluteInPlace( void) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE LinearlyInterpolate( 
            /* [in] */ ISIVector3 *in_pVector1,
            /* [in] */ ISIVector3 *in_pVector2,
            /* [in] */ double in_dAlpha) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Angle( 
            /* [in] */ ISIVector3 *in_pVector,
            /* [retval][out] */ double *out_pdAngle) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Cross( 
            /* [in] */ ISIVector3 *in_pVector1,
            /* [in] */ ISIVector3 *in_pVector2) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Dot( 
            /* [in] */ ISIVector3 *in_pVector,
            /* [retval][out] */ double *out_pdDot) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Normalize( 
            /* [in] */ ISIVector3 *in_pVector,
            /* [retval][out] */ BOOL *out_pbNormalized) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE NormalizeInPlace( 
            /* [retval][out] */ BOOL *out_pbNormalized) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE LengthSquared( 
            /* [retval][out] */ double *out_pdLengthSquared) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Length( 
            /* [retval][out] */ double *out_pdLength) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE MinComponent( 
            /* [retval][out] */ double *out_pdMinComponent) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE MinComponentIndex( 
            /* [retval][out] */ short *out_pnMinComponentIndex) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE MaxComponent( 
            /* [retval][out] */ double *out_pdMaxComponent) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE MaxComponentIndex( 
            /* [retval][out] */ short *out_pnMaxComponentIndex) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SetNull( void) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE MulByMatrix3( 
            /* [in] */ ISIVector3 *in_pVector,
            /* [in] */ ISIMatrix3 *in_pMatrix) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE MulByMatrix3InPlace( 
            /* [in] */ ISIMatrix3 *in_pMatrix) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE MulByMatrix4( 
            ISIVector3 *in_pVector,
            /* [in] */ ISIMatrix4 *in_pMatrix) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE MulByMatrix4InPlace( 
            /* [in] */ ISIMatrix4 *in_pMatrix) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE MulByRotation( 
            ISIVector3 *in_pVector,
            /* [in] */ ISIRotation *in_pRotation) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE MulByRotationInPlace( 
            /* [in] */ ISIRotation *in_pRotation) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE MulByTransformation( 
            /* [in] */ ISIVector3 *in_pVector,
            /* [in] */ ISITransformation *in_pTransformation) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE MulByTransformationInPlace( 
            /* [in] */ ISITransformation *in_pTransformation) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Get( 
            /* [out] */ VARIANT *out_pvarX,
            /* [out] */ VARIANT *out_pvarY,
            /* [out] */ VARIANT *out_pvarZ) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Get2( 
            /* [retval][out] */ VARIANT *out_pVal) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct ISIVector3Vtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            ISIVector3 * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            ISIVector3 * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            ISIVector3 * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            ISIVector3 * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            ISIVector3 * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            ISIVector3 * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            ISIVector3 * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Value )( 
            ISIVector3 * This,
            /* [in] */ short in_nIndex,
            /* [retval][out] */ double *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_Value )( 
            ISIVector3 * This,
            /* [in] */ short in_nIndex,
            /* [in] */ double newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_X )( 
            ISIVector3 * This,
            /* [retval][out] */ double *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_X )( 
            ISIVector3 * This,
            /* [in] */ double newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Y )( 
            ISIVector3 * This,
            /* [retval][out] */ double *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_Y )( 
            ISIVector3 * This,
            /* [in] */ double newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Z )( 
            ISIVector3 * This,
            /* [retval][out] */ double *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_Z )( 
            ISIVector3 * This,
            /* [in] */ double newVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Copy )( 
            ISIVector3 * This,
            /* [in] */ ISIVector3 *in_pVector);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Set )( 
            ISIVector3 * This,
            /* [in] */ double in_dXValue,
            /* [in] */ double in_dYValue,
            /* [in] */ double in_dZValue);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Add )( 
            ISIVector3 * This,
            /* [in] */ ISIVector3 *in_pVector1,
            /* [in] */ ISIVector3 *in_pVector2);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *AddInPlace )( 
            ISIVector3 * This,
            /* [in] */ ISIVector3 *in_pVector);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Sub )( 
            ISIVector3 * This,
            /* [in] */ ISIVector3 *in_pVector1,
            /* [in] */ ISIVector3 *in_pVector2);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SubInPlace )( 
            ISIVector3 * This,
            /* [in] */ ISIVector3 *in_pVector);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Negate )( 
            ISIVector3 * This,
            /* [in] */ ISIVector3 *in_pVector);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *NegateInPlace )( 
            ISIVector3 * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Scale )( 
            ISIVector3 * This,
            /* [in] */ double in_dScalar,
            /* [in] */ ISIVector3 *in_pVector);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *ScaleInPlace )( 
            ISIVector3 * This,
            /* [in] */ double in_dScalar);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *ScaleAdd )( 
            ISIVector3 * This,
            /* [in] */ double in_dScalar,
            /* [in] */ ISIVector3 *in_pVector1,
            /* [in] */ ISIVector3 *in_pVector2);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *ScaleAddInPlace )( 
            ISIVector3 * This,
            /* [in] */ double in_dScalar,
            /* [in] */ ISIVector3 *in_pVector);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Equals )( 
            ISIVector3 * This,
            /* [in] */ ISIVector3 *in_pVector,
            /* [retval][out] */ BOOL *out_pbEquality);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *EpsilonEquals )( 
            ISIVector3 * This,
            /* [in] */ ISIVector3 *in_pVector,
            /* [in] */ double in_dEpsilon,
            /* [retval][out] */ BOOL *out_pbEquality);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Clamp )( 
            ISIVector3 * This,
            /* [in] */ double in_dMin,
            /* [in] */ double in_dMax,
            /* [in] */ ISIVector3 *in_pVector);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *ClampInPlace )( 
            ISIVector3 * This,
            /* [in] */ double in_dMin,
            /* [in] */ double in_dMax);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Absolute )( 
            ISIVector3 * This,
            /* [in] */ ISIVector3 *in_pVector);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *AbsoluteInPlace )( 
            ISIVector3 * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *LinearlyInterpolate )( 
            ISIVector3 * This,
            /* [in] */ ISIVector3 *in_pVector1,
            /* [in] */ ISIVector3 *in_pVector2,
            /* [in] */ double in_dAlpha);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Angle )( 
            ISIVector3 * This,
            /* [in] */ ISIVector3 *in_pVector,
            /* [retval][out] */ double *out_pdAngle);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Cross )( 
            ISIVector3 * This,
            /* [in] */ ISIVector3 *in_pVector1,
            /* [in] */ ISIVector3 *in_pVector2);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Dot )( 
            ISIVector3 * This,
            /* [in] */ ISIVector3 *in_pVector,
            /* [retval][out] */ double *out_pdDot);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Normalize )( 
            ISIVector3 * This,
            /* [in] */ ISIVector3 *in_pVector,
            /* [retval][out] */ BOOL *out_pbNormalized);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *NormalizeInPlace )( 
            ISIVector3 * This,
            /* [retval][out] */ BOOL *out_pbNormalized);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *LengthSquared )( 
            ISIVector3 * This,
            /* [retval][out] */ double *out_pdLengthSquared);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Length )( 
            ISIVector3 * This,
            /* [retval][out] */ double *out_pdLength);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *MinComponent )( 
            ISIVector3 * This,
            /* [retval][out] */ double *out_pdMinComponent);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *MinComponentIndex )( 
            ISIVector3 * This,
            /* [retval][out] */ short *out_pnMinComponentIndex);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *MaxComponent )( 
            ISIVector3 * This,
            /* [retval][out] */ double *out_pdMaxComponent);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *MaxComponentIndex )( 
            ISIVector3 * This,
            /* [retval][out] */ short *out_pnMaxComponentIndex);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SetNull )( 
            ISIVector3 * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *MulByMatrix3 )( 
            ISIVector3 * This,
            /* [in] */ ISIVector3 *in_pVector,
            /* [in] */ ISIMatrix3 *in_pMatrix);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *MulByMatrix3InPlace )( 
            ISIVector3 * This,
            /* [in] */ ISIMatrix3 *in_pMatrix);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *MulByMatrix4 )( 
            ISIVector3 * This,
            ISIVector3 *in_pVector,
            /* [in] */ ISIMatrix4 *in_pMatrix);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *MulByMatrix4InPlace )( 
            ISIVector3 * This,
            /* [in] */ ISIMatrix4 *in_pMatrix);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *MulByRotation )( 
            ISIVector3 * This,
            ISIVector3 *in_pVector,
            /* [in] */ ISIRotation *in_pRotation);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *MulByRotationInPlace )( 
            ISIVector3 * This,
            /* [in] */ ISIRotation *in_pRotation);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *MulByTransformation )( 
            ISIVector3 * This,
            /* [in] */ ISIVector3 *in_pVector,
            /* [in] */ ISITransformation *in_pTransformation);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *MulByTransformationInPlace )( 
            ISIVector3 * This,
            /* [in] */ ISITransformation *in_pTransformation);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Get )( 
            ISIVector3 * This,
            /* [out] */ VARIANT *out_pvarX,
            /* [out] */ VARIANT *out_pvarY,
            /* [out] */ VARIANT *out_pvarZ);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Get2 )( 
            ISIVector3 * This,
            /* [retval][out] */ VARIANT *out_pVal);
        
        END_INTERFACE
    } ISIVector3Vtbl;

    interface ISIVector3
    {
        CONST_VTBL struct ISIVector3Vtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ISIVector3_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define ISIVector3_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define ISIVector3_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define ISIVector3_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define ISIVector3_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define ISIVector3_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define ISIVector3_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define ISIVector3_get_Value(This,in_nIndex,pVal)	\
    (This)->lpVtbl -> get_Value(This,in_nIndex,pVal)

#define ISIVector3_put_Value(This,in_nIndex,newVal)	\
    (This)->lpVtbl -> put_Value(This,in_nIndex,newVal)

#define ISIVector3_get_X(This,pVal)	\
    (This)->lpVtbl -> get_X(This,pVal)

#define ISIVector3_put_X(This,newVal)	\
    (This)->lpVtbl -> put_X(This,newVal)

#define ISIVector3_get_Y(This,pVal)	\
    (This)->lpVtbl -> get_Y(This,pVal)

#define ISIVector3_put_Y(This,newVal)	\
    (This)->lpVtbl -> put_Y(This,newVal)

#define ISIVector3_get_Z(This,pVal)	\
    (This)->lpVtbl -> get_Z(This,pVal)

#define ISIVector3_put_Z(This,newVal)	\
    (This)->lpVtbl -> put_Z(This,newVal)

#define ISIVector3_Copy(This,in_pVector)	\
    (This)->lpVtbl -> Copy(This,in_pVector)

#define ISIVector3_Set(This,in_dXValue,in_dYValue,in_dZValue)	\
    (This)->lpVtbl -> Set(This,in_dXValue,in_dYValue,in_dZValue)

#define ISIVector3_Add(This,in_pVector1,in_pVector2)	\
    (This)->lpVtbl -> Add(This,in_pVector1,in_pVector2)

#define ISIVector3_AddInPlace(This,in_pVector)	\
    (This)->lpVtbl -> AddInPlace(This,in_pVector)

#define ISIVector3_Sub(This,in_pVector1,in_pVector2)	\
    (This)->lpVtbl -> Sub(This,in_pVector1,in_pVector2)

#define ISIVector3_SubInPlace(This,in_pVector)	\
    (This)->lpVtbl -> SubInPlace(This,in_pVector)

#define ISIVector3_Negate(This,in_pVector)	\
    (This)->lpVtbl -> Negate(This,in_pVector)

#define ISIVector3_NegateInPlace(This)	\
    (This)->lpVtbl -> NegateInPlace(This)

#define ISIVector3_Scale(This,in_dScalar,in_pVector)	\
    (This)->lpVtbl -> Scale(This,in_dScalar,in_pVector)

#define ISIVector3_ScaleInPlace(This,in_dScalar)	\
    (This)->lpVtbl -> ScaleInPlace(This,in_dScalar)

#define ISIVector3_ScaleAdd(This,in_dScalar,in_pVector1,in_pVector2)	\
    (This)->lpVtbl -> ScaleAdd(This,in_dScalar,in_pVector1,in_pVector2)

#define ISIVector3_ScaleAddInPlace(This,in_dScalar,in_pVector)	\
    (This)->lpVtbl -> ScaleAddInPlace(This,in_dScalar,in_pVector)

#define ISIVector3_Equals(This,in_pVector,out_pbEquality)	\
    (This)->lpVtbl -> Equals(This,in_pVector,out_pbEquality)

#define ISIVector3_EpsilonEquals(This,in_pVector,in_dEpsilon,out_pbEquality)	\
    (This)->lpVtbl -> EpsilonEquals(This,in_pVector,in_dEpsilon,out_pbEquality)

#define ISIVector3_Clamp(This,in_dMin,in_dMax,in_pVector)	\
    (This)->lpVtbl -> Clamp(This,in_dMin,in_dMax,in_pVector)

#define ISIVector3_ClampInPlace(This,in_dMin,in_dMax)	\
    (This)->lpVtbl -> ClampInPlace(This,in_dMin,in_dMax)

#define ISIVector3_Absolute(This,in_pVector)	\
    (This)->lpVtbl -> Absolute(This,in_pVector)

#define ISIVector3_AbsoluteInPlace(This)	\
    (This)->lpVtbl -> AbsoluteInPlace(This)

#define ISIVector3_LinearlyInterpolate(This,in_pVector1,in_pVector2,in_dAlpha)	\
    (This)->lpVtbl -> LinearlyInterpolate(This,in_pVector1,in_pVector2,in_dAlpha)

#define ISIVector3_Angle(This,in_pVector,out_pdAngle)	\
    (This)->lpVtbl -> Angle(This,in_pVector,out_pdAngle)

#define ISIVector3_Cross(This,in_pVector1,in_pVector2)	\
    (This)->lpVtbl -> Cross(This,in_pVector1,in_pVector2)

#define ISIVector3_Dot(This,in_pVector,out_pdDot)	\
    (This)->lpVtbl -> Dot(This,in_pVector,out_pdDot)

#define ISIVector3_Normalize(This,in_pVector,out_pbNormalized)	\
    (This)->lpVtbl -> Normalize(This,in_pVector,out_pbNormalized)

#define ISIVector3_NormalizeInPlace(This,out_pbNormalized)	\
    (This)->lpVtbl -> NormalizeInPlace(This,out_pbNormalized)

#define ISIVector3_LengthSquared(This,out_pdLengthSquared)	\
    (This)->lpVtbl -> LengthSquared(This,out_pdLengthSquared)

#define ISIVector3_Length(This,out_pdLength)	\
    (This)->lpVtbl -> Length(This,out_pdLength)

#define ISIVector3_MinComponent(This,out_pdMinComponent)	\
    (This)->lpVtbl -> MinComponent(This,out_pdMinComponent)

#define ISIVector3_MinComponentIndex(This,out_pnMinComponentIndex)	\
    (This)->lpVtbl -> MinComponentIndex(This,out_pnMinComponentIndex)

#define ISIVector3_MaxComponent(This,out_pdMaxComponent)	\
    (This)->lpVtbl -> MaxComponent(This,out_pdMaxComponent)

#define ISIVector3_MaxComponentIndex(This,out_pnMaxComponentIndex)	\
    (This)->lpVtbl -> MaxComponentIndex(This,out_pnMaxComponentIndex)

#define ISIVector3_SetNull(This)	\
    (This)->lpVtbl -> SetNull(This)

#define ISIVector3_MulByMatrix3(This,in_pVector,in_pMatrix)	\
    (This)->lpVtbl -> MulByMatrix3(This,in_pVector,in_pMatrix)

#define ISIVector3_MulByMatrix3InPlace(This,in_pMatrix)	\
    (This)->lpVtbl -> MulByMatrix3InPlace(This,in_pMatrix)

#define ISIVector3_MulByMatrix4(This,in_pVector,in_pMatrix)	\
    (This)->lpVtbl -> MulByMatrix4(This,in_pVector,in_pMatrix)

#define ISIVector3_MulByMatrix4InPlace(This,in_pMatrix)	\
    (This)->lpVtbl -> MulByMatrix4InPlace(This,in_pMatrix)

#define ISIVector3_MulByRotation(This,in_pVector,in_pRotation)	\
    (This)->lpVtbl -> MulByRotation(This,in_pVector,in_pRotation)

#define ISIVector3_MulByRotationInPlace(This,in_pRotation)	\
    (This)->lpVtbl -> MulByRotationInPlace(This,in_pRotation)

#define ISIVector3_MulByTransformation(This,in_pVector,in_pTransformation)	\
    (This)->lpVtbl -> MulByTransformation(This,in_pVector,in_pTransformation)

#define ISIVector3_MulByTransformationInPlace(This,in_pTransformation)	\
    (This)->lpVtbl -> MulByTransformationInPlace(This,in_pTransformation)

#define ISIVector3_Get(This,out_pvarX,out_pvarY,out_pvarZ)	\
    (This)->lpVtbl -> Get(This,out_pvarX,out_pvarY,out_pvarZ)

#define ISIVector3_Get2(This,out_pVal)	\
    (This)->lpVtbl -> Get2(This,out_pVal)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE ISIVector3_get_Value_Proxy( 
    ISIVector3 * This,
    /* [in] */ short in_nIndex,
    /* [retval][out] */ double *pVal);


void __RPC_STUB ISIVector3_get_Value_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE ISIVector3_put_Value_Proxy( 
    ISIVector3 * This,
    /* [in] */ short in_nIndex,
    /* [in] */ double newVal);


void __RPC_STUB ISIVector3_put_Value_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE ISIVector3_get_X_Proxy( 
    ISIVector3 * This,
    /* [retval][out] */ double *pVal);


void __RPC_STUB ISIVector3_get_X_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE ISIVector3_put_X_Proxy( 
    ISIVector3 * This,
    /* [in] */ double newVal);


void __RPC_STUB ISIVector3_put_X_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE ISIVector3_get_Y_Proxy( 
    ISIVector3 * This,
    /* [retval][out] */ double *pVal);


void __RPC_STUB ISIVector3_get_Y_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE ISIVector3_put_Y_Proxy( 
    ISIVector3 * This,
    /* [in] */ double newVal);


void __RPC_STUB ISIVector3_put_Y_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE ISIVector3_get_Z_Proxy( 
    ISIVector3 * This,
    /* [retval][out] */ double *pVal);


void __RPC_STUB ISIVector3_get_Z_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE ISIVector3_put_Z_Proxy( 
    ISIVector3 * This,
    /* [in] */ double newVal);


void __RPC_STUB ISIVector3_put_Z_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ISIVector3_Copy_Proxy( 
    ISIVector3 * This,
    /* [in] */ ISIVector3 *in_pVector);


void __RPC_STUB ISIVector3_Copy_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ISIVector3_Set_Proxy( 
    ISIVector3 * This,
    /* [in] */ double in_dXValue,
    /* [in] */ double in_dYValue,
    /* [in] */ double in_dZValue);


void __RPC_STUB ISIVector3_Set_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ISIVector3_Add_Proxy( 
    ISIVector3 * This,
    /* [in] */ ISIVector3 *in_pVector1,
    /* [in] */ ISIVector3 *in_pVector2);


void __RPC_STUB ISIVector3_Add_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ISIVector3_AddInPlace_Proxy( 
    ISIVector3 * This,
    /* [in] */ ISIVector3 *in_pVector);


void __RPC_STUB ISIVector3_AddInPlace_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ISIVector3_Sub_Proxy( 
    ISIVector3 * This,
    /* [in] */ ISIVector3 *in_pVector1,
    /* [in] */ ISIVector3 *in_pVector2);


void __RPC_STUB ISIVector3_Sub_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ISIVector3_SubInPlace_Proxy( 
    ISIVector3 * This,
    /* [in] */ ISIVector3 *in_pVector);


void __RPC_STUB ISIVector3_SubInPlace_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ISIVector3_Negate_Proxy( 
    ISIVector3 * This,
    /* [in] */ ISIVector3 *in_pVector);


void __RPC_STUB ISIVector3_Negate_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ISIVector3_NegateInPlace_Proxy( 
    ISIVector3 * This);


void __RPC_STUB ISIVector3_NegateInPlace_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ISIVector3_Scale_Proxy( 
    ISIVector3 * This,
    /* [in] */ double in_dScalar,
    /* [in] */ ISIVector3 *in_pVector);


void __RPC_STUB ISIVector3_Scale_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ISIVector3_ScaleInPlace_Proxy( 
    ISIVector3 * This,
    /* [in] */ double in_dScalar);


void __RPC_STUB ISIVector3_ScaleInPlace_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ISIVector3_ScaleAdd_Proxy( 
    ISIVector3 * This,
    /* [in] */ double in_dScalar,
    /* [in] */ ISIVector3 *in_pVector1,
    /* [in] */ ISIVector3 *in_pVector2);


void __RPC_STUB ISIVector3_ScaleAdd_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ISIVector3_ScaleAddInPlace_Proxy( 
    ISIVector3 * This,
    /* [in] */ double in_dScalar,
    /* [in] */ ISIVector3 *in_pVector);


void __RPC_STUB ISIVector3_ScaleAddInPlace_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ISIVector3_Equals_Proxy( 
    ISIVector3 * This,
    /* [in] */ ISIVector3 *in_pVector,
    /* [retval][out] */ BOOL *out_pbEquality);


void __RPC_STUB ISIVector3_Equals_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ISIVector3_EpsilonEquals_Proxy( 
    ISIVector3 * This,
    /* [in] */ ISIVector3 *in_pVector,
    /* [in] */ double in_dEpsilon,
    /* [retval][out] */ BOOL *out_pbEquality);


void __RPC_STUB ISIVector3_EpsilonEquals_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ISIVector3_Clamp_Proxy( 
    ISIVector3 * This,
    /* [in] */ double in_dMin,
    /* [in] */ double in_dMax,
    /* [in] */ ISIVector3 *in_pVector);


void __RPC_STUB ISIVector3_Clamp_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ISIVector3_ClampInPlace_Proxy( 
    ISIVector3 * This,
    /* [in] */ double in_dMin,
    /* [in] */ double in_dMax);


void __RPC_STUB ISIVector3_ClampInPlace_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ISIVector3_Absolute_Proxy( 
    ISIVector3 * This,
    /* [in] */ ISIVector3 *in_pVector);


void __RPC_STUB ISIVector3_Absolute_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ISIVector3_AbsoluteInPlace_Proxy( 
    ISIVector3 * This);


void __RPC_STUB ISIVector3_AbsoluteInPlace_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ISIVector3_LinearlyInterpolate_Proxy( 
    ISIVector3 * This,
    /* [in] */ ISIVector3 *in_pVector1,
    /* [in] */ ISIVector3 *in_pVector2,
    /* [in] */ double in_dAlpha);


void __RPC_STUB ISIVector3_LinearlyInterpolate_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ISIVector3_Angle_Proxy( 
    ISIVector3 * This,
    /* [in] */ ISIVector3 *in_pVector,
    /* [retval][out] */ double *out_pdAngle);


void __RPC_STUB ISIVector3_Angle_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ISIVector3_Cross_Proxy( 
    ISIVector3 * This,
    /* [in] */ ISIVector3 *in_pVector1,
    /* [in] */ ISIVector3 *in_pVector2);


void __RPC_STUB ISIVector3_Cross_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ISIVector3_Dot_Proxy( 
    ISIVector3 * This,
    /* [in] */ ISIVector3 *in_pVector,
    /* [retval][out] */ double *out_pdDot);


void __RPC_STUB ISIVector3_Dot_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ISIVector3_Normalize_Proxy( 
    ISIVector3 * This,
    /* [in] */ ISIVector3 *in_pVector,
    /* [retval][out] */ BOOL *out_pbNormalized);


void __RPC_STUB ISIVector3_Normalize_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ISIVector3_NormalizeInPlace_Proxy( 
    ISIVector3 * This,
    /* [retval][out] */ BOOL *out_pbNormalized);


void __RPC_STUB ISIVector3_NormalizeInPlace_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ISIVector3_LengthSquared_Proxy( 
    ISIVector3 * This,
    /* [retval][out] */ double *out_pdLengthSquared);


void __RPC_STUB ISIVector3_LengthSquared_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ISIVector3_Length_Proxy( 
    ISIVector3 * This,
    /* [retval][out] */ double *out_pdLength);


void __RPC_STUB ISIVector3_Length_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ISIVector3_MinComponent_Proxy( 
    ISIVector3 * This,
    /* [retval][out] */ double *out_pdMinComponent);


void __RPC_STUB ISIVector3_MinComponent_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ISIVector3_MinComponentIndex_Proxy( 
    ISIVector3 * This,
    /* [retval][out] */ short *out_pnMinComponentIndex);


void __RPC_STUB ISIVector3_MinComponentIndex_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ISIVector3_MaxComponent_Proxy( 
    ISIVector3 * This,
    /* [retval][out] */ double *out_pdMaxComponent);


void __RPC_STUB ISIVector3_MaxComponent_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ISIVector3_MaxComponentIndex_Proxy( 
    ISIVector3 * This,
    /* [retval][out] */ short *out_pnMaxComponentIndex);


void __RPC_STUB ISIVector3_MaxComponentIndex_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ISIVector3_SetNull_Proxy( 
    ISIVector3 * This);


void __RPC_STUB ISIVector3_SetNull_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ISIVector3_MulByMatrix3_Proxy( 
    ISIVector3 * This,
    /* [in] */ ISIVector3 *in_pVector,
    /* [in] */ ISIMatrix3 *in_pMatrix);


void __RPC_STUB ISIVector3_MulByMatrix3_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ISIVector3_MulByMatrix3InPlace_Proxy( 
    ISIVector3 * This,
    /* [in] */ ISIMatrix3 *in_pMatrix);


void __RPC_STUB ISIVector3_MulByMatrix3InPlace_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ISIVector3_MulByMatrix4_Proxy( 
    ISIVector3 * This,
    ISIVector3 *in_pVector,
    /* [in] */ ISIMatrix4 *in_pMatrix);


void __RPC_STUB ISIVector3_MulByMatrix4_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ISIVector3_MulByMatrix4InPlace_Proxy( 
    ISIVector3 * This,
    /* [in] */ ISIMatrix4 *in_pMatrix);


void __RPC_STUB ISIVector3_MulByMatrix4InPlace_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ISIVector3_MulByRotation_Proxy( 
    ISIVector3 * This,
    ISIVector3 *in_pVector,
    /* [in] */ ISIRotation *in_pRotation);


void __RPC_STUB ISIVector3_MulByRotation_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ISIVector3_MulByRotationInPlace_Proxy( 
    ISIVector3 * This,
    /* [in] */ ISIRotation *in_pRotation);


void __RPC_STUB ISIVector3_MulByRotationInPlace_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ISIVector3_MulByTransformation_Proxy( 
    ISIVector3 * This,
    /* [in] */ ISIVector3 *in_pVector,
    /* [in] */ ISITransformation *in_pTransformation);


void __RPC_STUB ISIVector3_MulByTransformation_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ISIVector3_MulByTransformationInPlace_Proxy( 
    ISIVector3 * This,
    /* [in] */ ISITransformation *in_pTransformation);


void __RPC_STUB ISIVector3_MulByTransformationInPlace_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ISIVector3_Get_Proxy( 
    ISIVector3 * This,
    /* [out] */ VARIANT *out_pvarX,
    /* [out] */ VARIANT *out_pvarY,
    /* [out] */ VARIANT *out_pvarZ);


void __RPC_STUB ISIVector3_Get_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ISIVector3_Get2_Proxy( 
    ISIVector3 * This,
    /* [retval][out] */ VARIANT *out_pVal);


void __RPC_STUB ISIVector3_Get2_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __ISIVector3_INTERFACE_DEFINED__ */


#ifndef __ISIMatrix3_INTERFACE_DEFINED__
#define __ISIMatrix3_INTERFACE_DEFINED__

/* interface ISIMatrix3 */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_ISIMatrix3;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("BFB3B913-24CC-11D3-BC3A-00A0C9612B97")
    ISIMatrix3 : public IDispatch
    {
    public:
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Value( 
            /* [in] */ short in_nRow,
            /* [in] */ short in_nCol,
            /* [retval][out] */ double *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_Value( 
            /* [in] */ short in_nRow,
            /* [in] */ short in_nCol,
            /* [in] */ double newVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Copy( 
            /* [in] */ ISIMatrix3 *in_pMatrix) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Set( 
            /* [in] */ double in_dM00,
            /* [in] */ double in_dM01,
            /* [in] */ double in_dM02,
            /* [in] */ double in_dM10,
            /* [in] */ double in_dM11,
            /* [in] */ double in_dM12,
            /* [in] */ double in_dM020,
            /* [in] */ double in_dM21,
            /* [in] */ double in_dM22) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Mul( 
            /* [in] */ ISIMatrix3 *in_pMatrix1,
            /* [in] */ ISIMatrix3 *in_pMatrix2) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE MulInPlace( 
            /* [in] */ ISIMatrix3 *in_pMatrix) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Transpose( 
            /* [in] */ ISIMatrix3 *in_pMatrix) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE TransposeInPlace( void) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE TransposeInverse( 
            /* [in] */ ISIMatrix3 *in_pMatrix,
            /* [retval][out] */ BOOL *out_pbInverted) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE TransposeInverseInPlace( 
            /* [retval][out] */ BOOL *out_pbInverted) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Invert( 
            /* [in] */ ISIMatrix3 *in_pMatrix,
            /* [retval][out] */ BOOL *out_pbInverted) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE InvertInPlace( 
            /* [retval][out] */ BOOL *out_pbInverted) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SetIdentity( void) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Get( 
            /* [out] */ VARIANT *out_pvarM00,
            /* [out] */ VARIANT *out_pvarM01,
            /* [out] */ VARIANT *out_pvarM02,
            /* [out] */ VARIANT *out_pvarM10,
            /* [out] */ VARIANT *out_pvarM11,
            /* [out] */ VARIANT *out_pvarM12,
            /* [out] */ VARIANT *out_pvarM020,
            /* [out] */ VARIANT *out_pvarM21,
            /* [out] */ VARIANT *out_pvarM22) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Get2( 
            /* [retval][out] */ VARIANT *out_pVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetQuaternion( 
            /* [in] */ ISIQuaternion *out_pVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SetFromQuaternion( 
            /* [in] */ ISIQuaternion *out_pVal) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct ISIMatrix3Vtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            ISIMatrix3 * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            ISIMatrix3 * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            ISIMatrix3 * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            ISIMatrix3 * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            ISIMatrix3 * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            ISIMatrix3 * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            ISIMatrix3 * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Value )( 
            ISIMatrix3 * This,
            /* [in] */ short in_nRow,
            /* [in] */ short in_nCol,
            /* [retval][out] */ double *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_Value )( 
            ISIMatrix3 * This,
            /* [in] */ short in_nRow,
            /* [in] */ short in_nCol,
            /* [in] */ double newVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Copy )( 
            ISIMatrix3 * This,
            /* [in] */ ISIMatrix3 *in_pMatrix);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Set )( 
            ISIMatrix3 * This,
            /* [in] */ double in_dM00,
            /* [in] */ double in_dM01,
            /* [in] */ double in_dM02,
            /* [in] */ double in_dM10,
            /* [in] */ double in_dM11,
            /* [in] */ double in_dM12,
            /* [in] */ double in_dM020,
            /* [in] */ double in_dM21,
            /* [in] */ double in_dM22);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Mul )( 
            ISIMatrix3 * This,
            /* [in] */ ISIMatrix3 *in_pMatrix1,
            /* [in] */ ISIMatrix3 *in_pMatrix2);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *MulInPlace )( 
            ISIMatrix3 * This,
            /* [in] */ ISIMatrix3 *in_pMatrix);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Transpose )( 
            ISIMatrix3 * This,
            /* [in] */ ISIMatrix3 *in_pMatrix);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *TransposeInPlace )( 
            ISIMatrix3 * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *TransposeInverse )( 
            ISIMatrix3 * This,
            /* [in] */ ISIMatrix3 *in_pMatrix,
            /* [retval][out] */ BOOL *out_pbInverted);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *TransposeInverseInPlace )( 
            ISIMatrix3 * This,
            /* [retval][out] */ BOOL *out_pbInverted);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Invert )( 
            ISIMatrix3 * This,
            /* [in] */ ISIMatrix3 *in_pMatrix,
            /* [retval][out] */ BOOL *out_pbInverted);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *InvertInPlace )( 
            ISIMatrix3 * This,
            /* [retval][out] */ BOOL *out_pbInverted);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SetIdentity )( 
            ISIMatrix3 * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Get )( 
            ISIMatrix3 * This,
            /* [out] */ VARIANT *out_pvarM00,
            /* [out] */ VARIANT *out_pvarM01,
            /* [out] */ VARIANT *out_pvarM02,
            /* [out] */ VARIANT *out_pvarM10,
            /* [out] */ VARIANT *out_pvarM11,
            /* [out] */ VARIANT *out_pvarM12,
            /* [out] */ VARIANT *out_pvarM020,
            /* [out] */ VARIANT *out_pvarM21,
            /* [out] */ VARIANT *out_pvarM22);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Get2 )( 
            ISIMatrix3 * This,
            /* [retval][out] */ VARIANT *out_pVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetQuaternion )( 
            ISIMatrix3 * This,
            /* [in] */ ISIQuaternion *out_pVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SetFromQuaternion )( 
            ISIMatrix3 * This,
            /* [in] */ ISIQuaternion *out_pVal);
        
        END_INTERFACE
    } ISIMatrix3Vtbl;

    interface ISIMatrix3
    {
        CONST_VTBL struct ISIMatrix3Vtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ISIMatrix3_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define ISIMatrix3_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define ISIMatrix3_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define ISIMatrix3_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define ISIMatrix3_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define ISIMatrix3_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define ISIMatrix3_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define ISIMatrix3_get_Value(This,in_nRow,in_nCol,pVal)	\
    (This)->lpVtbl -> get_Value(This,in_nRow,in_nCol,pVal)

#define ISIMatrix3_put_Value(This,in_nRow,in_nCol,newVal)	\
    (This)->lpVtbl -> put_Value(This,in_nRow,in_nCol,newVal)

#define ISIMatrix3_Copy(This,in_pMatrix)	\
    (This)->lpVtbl -> Copy(This,in_pMatrix)

#define ISIMatrix3_Set(This,in_dM00,in_dM01,in_dM02,in_dM10,in_dM11,in_dM12,in_dM020,in_dM21,in_dM22)	\
    (This)->lpVtbl -> Set(This,in_dM00,in_dM01,in_dM02,in_dM10,in_dM11,in_dM12,in_dM020,in_dM21,in_dM22)

#define ISIMatrix3_Mul(This,in_pMatrix1,in_pMatrix2)	\
    (This)->lpVtbl -> Mul(This,in_pMatrix1,in_pMatrix2)

#define ISIMatrix3_MulInPlace(This,in_pMatrix)	\
    (This)->lpVtbl -> MulInPlace(This,in_pMatrix)

#define ISIMatrix3_Transpose(This,in_pMatrix)	\
    (This)->lpVtbl -> Transpose(This,in_pMatrix)

#define ISIMatrix3_TransposeInPlace(This)	\
    (This)->lpVtbl -> TransposeInPlace(This)

#define ISIMatrix3_TransposeInverse(This,in_pMatrix,out_pbInverted)	\
    (This)->lpVtbl -> TransposeInverse(This,in_pMatrix,out_pbInverted)

#define ISIMatrix3_TransposeInverseInPlace(This,out_pbInverted)	\
    (This)->lpVtbl -> TransposeInverseInPlace(This,out_pbInverted)

#define ISIMatrix3_Invert(This,in_pMatrix,out_pbInverted)	\
    (This)->lpVtbl -> Invert(This,in_pMatrix,out_pbInverted)

#define ISIMatrix3_InvertInPlace(This,out_pbInverted)	\
    (This)->lpVtbl -> InvertInPlace(This,out_pbInverted)

#define ISIMatrix3_SetIdentity(This)	\
    (This)->lpVtbl -> SetIdentity(This)

#define ISIMatrix3_Get(This,out_pvarM00,out_pvarM01,out_pvarM02,out_pvarM10,out_pvarM11,out_pvarM12,out_pvarM020,out_pvarM21,out_pvarM22)	\
    (This)->lpVtbl -> Get(This,out_pvarM00,out_pvarM01,out_pvarM02,out_pvarM10,out_pvarM11,out_pvarM12,out_pvarM020,out_pvarM21,out_pvarM22)

#define ISIMatrix3_Get2(This,out_pVal)	\
    (This)->lpVtbl -> Get2(This,out_pVal)

#define ISIMatrix3_GetQuaternion(This,out_pVal)	\
    (This)->lpVtbl -> GetQuaternion(This,out_pVal)

#define ISIMatrix3_SetFromQuaternion(This,out_pVal)	\
    (This)->lpVtbl -> SetFromQuaternion(This,out_pVal)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE ISIMatrix3_get_Value_Proxy( 
    ISIMatrix3 * This,
    /* [in] */ short in_nRow,
    /* [in] */ short in_nCol,
    /* [retval][out] */ double *pVal);


void __RPC_STUB ISIMatrix3_get_Value_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE ISIMatrix3_put_Value_Proxy( 
    ISIMatrix3 * This,
    /* [in] */ short in_nRow,
    /* [in] */ short in_nCol,
    /* [in] */ double newVal);


void __RPC_STUB ISIMatrix3_put_Value_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ISIMatrix3_Copy_Proxy( 
    ISIMatrix3 * This,
    /* [in] */ ISIMatrix3 *in_pMatrix);


void __RPC_STUB ISIMatrix3_Copy_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ISIMatrix3_Set_Proxy( 
    ISIMatrix3 * This,
    /* [in] */ double in_dM00,
    /* [in] */ double in_dM01,
    /* [in] */ double in_dM02,
    /* [in] */ double in_dM10,
    /* [in] */ double in_dM11,
    /* [in] */ double in_dM12,
    /* [in] */ double in_dM020,
    /* [in] */ double in_dM21,
    /* [in] */ double in_dM22);


void __RPC_STUB ISIMatrix3_Set_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ISIMatrix3_Mul_Proxy( 
    ISIMatrix3 * This,
    /* [in] */ ISIMatrix3 *in_pMatrix1,
    /* [in] */ ISIMatrix3 *in_pMatrix2);


void __RPC_STUB ISIMatrix3_Mul_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ISIMatrix3_MulInPlace_Proxy( 
    ISIMatrix3 * This,
    /* [in] */ ISIMatrix3 *in_pMatrix);


void __RPC_STUB ISIMatrix3_MulInPlace_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ISIMatrix3_Transpose_Proxy( 
    ISIMatrix3 * This,
    /* [in] */ ISIMatrix3 *in_pMatrix);


void __RPC_STUB ISIMatrix3_Transpose_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ISIMatrix3_TransposeInPlace_Proxy( 
    ISIMatrix3 * This);


void __RPC_STUB ISIMatrix3_TransposeInPlace_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ISIMatrix3_TransposeInverse_Proxy( 
    ISIMatrix3 * This,
    /* [in] */ ISIMatrix3 *in_pMatrix,
    /* [retval][out] */ BOOL *out_pbInverted);


void __RPC_STUB ISIMatrix3_TransposeInverse_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ISIMatrix3_TransposeInverseInPlace_Proxy( 
    ISIMatrix3 * This,
    /* [retval][out] */ BOOL *out_pbInverted);


void __RPC_STUB ISIMatrix3_TransposeInverseInPlace_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ISIMatrix3_Invert_Proxy( 
    ISIMatrix3 * This,
    /* [in] */ ISIMatrix3 *in_pMatrix,
    /* [retval][out] */ BOOL *out_pbInverted);


void __RPC_STUB ISIMatrix3_Invert_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ISIMatrix3_InvertInPlace_Proxy( 
    ISIMatrix3 * This,
    /* [retval][out] */ BOOL *out_pbInverted);


void __RPC_STUB ISIMatrix3_InvertInPlace_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ISIMatrix3_SetIdentity_Proxy( 
    ISIMatrix3 * This);


void __RPC_STUB ISIMatrix3_SetIdentity_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ISIMatrix3_Get_Proxy( 
    ISIMatrix3 * This,
    /* [out] */ VARIANT *out_pvarM00,
    /* [out] */ VARIANT *out_pvarM01,
    /* [out] */ VARIANT *out_pvarM02,
    /* [out] */ VARIANT *out_pvarM10,
    /* [out] */ VARIANT *out_pvarM11,
    /* [out] */ VARIANT *out_pvarM12,
    /* [out] */ VARIANT *out_pvarM020,
    /* [out] */ VARIANT *out_pvarM21,
    /* [out] */ VARIANT *out_pvarM22);


void __RPC_STUB ISIMatrix3_Get_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ISIMatrix3_Get2_Proxy( 
    ISIMatrix3 * This,
    /* [retval][out] */ VARIANT *out_pVal);


void __RPC_STUB ISIMatrix3_Get2_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ISIMatrix3_GetQuaternion_Proxy( 
    ISIMatrix3 * This,
    /* [in] */ ISIQuaternion *out_pVal);


void __RPC_STUB ISIMatrix3_GetQuaternion_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ISIMatrix3_SetFromQuaternion_Proxy( 
    ISIMatrix3 * This,
    /* [in] */ ISIQuaternion *out_pVal);


void __RPC_STUB ISIMatrix3_SetFromQuaternion_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __ISIMatrix3_INTERFACE_DEFINED__ */


#ifndef __ISIMatrix4_INTERFACE_DEFINED__
#define __ISIMatrix4_INTERFACE_DEFINED__

/* interface ISIMatrix4 */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_ISIMatrix4;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("BFB3B915-24CC-11D3-BC3A-00A0C9612B97")
    ISIMatrix4 : public IDispatch
    {
    public:
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Value( 
            /* [in] */ short in_nRow,
            /* [in] */ short in_nCol,
            /* [retval][out] */ double *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_Value( 
            /* [in] */ short in_nRow,
            /* [in] */ short in_nCol,
            /* [in] */ double newVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Copy( 
            /* [in] */ ISIMatrix4 *in_pMatrix) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Set( 
            /* [in] */ double in_dM00,
            /* [in] */ double in_dM01,
            /* [in] */ double in_dM02,
            /* [in] */ double in_dM03,
            /* [in] */ double in_dM10,
            /* [in] */ double in_dM11,
            /* [in] */ double in_dM12,
            /* [in] */ double in_dM13,
            /* [in] */ double in_dM20,
            /* [in] */ double in_dM21,
            /* [in] */ double in_dM22,
            /* [in] */ double in_dM23,
            /* [in] */ double in_dM30,
            /* [in] */ double in_dM31,
            /* [in] */ double in_dM32,
            /* [in] */ double in_dM33) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Get( 
            /* [out] */ VARIANT *out_pvarM00,
            /* [out] */ VARIANT *out_pvarM01,
            /* [out] */ VARIANT *out_pvarM02,
            /* [out] */ VARIANT *out_pvarM03,
            /* [out] */ VARIANT *out_pvarM10,
            /* [out] */ VARIANT *out_pvarM11,
            /* [out] */ VARIANT *out_pvarM12,
            /* [out] */ VARIANT *out_pvarM13,
            /* [out] */ VARIANT *out_pvarM20,
            /* [out] */ VARIANT *out_pvarM21,
            /* [out] */ VARIANT *out_pvarM22,
            /* [out] */ VARIANT *out_pvarM23,
            /* [out] */ VARIANT *out_pvarM30,
            /* [out] */ VARIANT *out_pvarM31,
            /* [out] */ VARIANT *out_pvarM32,
            /* [out] */ VARIANT *out_pvarM33) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Mul( 
            /* [in] */ ISIMatrix4 *in_pMatrix1,
            /* [in] */ ISIMatrix4 *in_pMatrix2) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE MulInPlace( 
            /* [in] */ ISIMatrix4 *in_pMatrix) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Transpose( 
            /* [in] */ ISIMatrix4 *in_pMatrix) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE TransposeInPlace( void) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE TransposeInverse( 
            /* [in] */ ISIMatrix4 *in_pMatrix,
            /* [retval][out] */ BOOL *out_pbInverted) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE TransposeInverseInPlace( 
            /* [retval][out] */ BOOL *out_pbInverted) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Invert( 
            /* [in] */ ISIMatrix4 *in_pMatrix,
            /* [retval][out] */ BOOL *out_pbInverted) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE InvertInPlace( 
            /* [retval][out] */ BOOL *out_pbInverted) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SetIdentity( void) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Get2( 
            /* [retval][out] */ VARIANT *out_pVal) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct ISIMatrix4Vtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            ISIMatrix4 * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            ISIMatrix4 * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            ISIMatrix4 * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            ISIMatrix4 * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            ISIMatrix4 * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            ISIMatrix4 * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            ISIMatrix4 * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Value )( 
            ISIMatrix4 * This,
            /* [in] */ short in_nRow,
            /* [in] */ short in_nCol,
            /* [retval][out] */ double *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_Value )( 
            ISIMatrix4 * This,
            /* [in] */ short in_nRow,
            /* [in] */ short in_nCol,
            /* [in] */ double newVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Copy )( 
            ISIMatrix4 * This,
            /* [in] */ ISIMatrix4 *in_pMatrix);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Set )( 
            ISIMatrix4 * This,
            /* [in] */ double in_dM00,
            /* [in] */ double in_dM01,
            /* [in] */ double in_dM02,
            /* [in] */ double in_dM03,
            /* [in] */ double in_dM10,
            /* [in] */ double in_dM11,
            /* [in] */ double in_dM12,
            /* [in] */ double in_dM13,
            /* [in] */ double in_dM20,
            /* [in] */ double in_dM21,
            /* [in] */ double in_dM22,
            /* [in] */ double in_dM23,
            /* [in] */ double in_dM30,
            /* [in] */ double in_dM31,
            /* [in] */ double in_dM32,
            /* [in] */ double in_dM33);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Get )( 
            ISIMatrix4 * This,
            /* [out] */ VARIANT *out_pvarM00,
            /* [out] */ VARIANT *out_pvarM01,
            /* [out] */ VARIANT *out_pvarM02,
            /* [out] */ VARIANT *out_pvarM03,
            /* [out] */ VARIANT *out_pvarM10,
            /* [out] */ VARIANT *out_pvarM11,
            /* [out] */ VARIANT *out_pvarM12,
            /* [out] */ VARIANT *out_pvarM13,
            /* [out] */ VARIANT *out_pvarM20,
            /* [out] */ VARIANT *out_pvarM21,
            /* [out] */ VARIANT *out_pvarM22,
            /* [out] */ VARIANT *out_pvarM23,
            /* [out] */ VARIANT *out_pvarM30,
            /* [out] */ VARIANT *out_pvarM31,
            /* [out] */ VARIANT *out_pvarM32,
            /* [out] */ VARIANT *out_pvarM33);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Mul )( 
            ISIMatrix4 * This,
            /* [in] */ ISIMatrix4 *in_pMatrix1,
            /* [in] */ ISIMatrix4 *in_pMatrix2);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *MulInPlace )( 
            ISIMatrix4 * This,
            /* [in] */ ISIMatrix4 *in_pMatrix);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Transpose )( 
            ISIMatrix4 * This,
            /* [in] */ ISIMatrix4 *in_pMatrix);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *TransposeInPlace )( 
            ISIMatrix4 * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *TransposeInverse )( 
            ISIMatrix4 * This,
            /* [in] */ ISIMatrix4 *in_pMatrix,
            /* [retval][out] */ BOOL *out_pbInverted);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *TransposeInverseInPlace )( 
            ISIMatrix4 * This,
            /* [retval][out] */ BOOL *out_pbInverted);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Invert )( 
            ISIMatrix4 * This,
            /* [in] */ ISIMatrix4 *in_pMatrix,
            /* [retval][out] */ BOOL *out_pbInverted);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *InvertInPlace )( 
            ISIMatrix4 * This,
            /* [retval][out] */ BOOL *out_pbInverted);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SetIdentity )( 
            ISIMatrix4 * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Get2 )( 
            ISIMatrix4 * This,
            /* [retval][out] */ VARIANT *out_pVal);
        
        END_INTERFACE
    } ISIMatrix4Vtbl;

    interface ISIMatrix4
    {
        CONST_VTBL struct ISIMatrix4Vtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ISIMatrix4_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define ISIMatrix4_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define ISIMatrix4_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define ISIMatrix4_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define ISIMatrix4_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define ISIMatrix4_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define ISIMatrix4_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define ISIMatrix4_get_Value(This,in_nRow,in_nCol,pVal)	\
    (This)->lpVtbl -> get_Value(This,in_nRow,in_nCol,pVal)

#define ISIMatrix4_put_Value(This,in_nRow,in_nCol,newVal)	\
    (This)->lpVtbl -> put_Value(This,in_nRow,in_nCol,newVal)

#define ISIMatrix4_Copy(This,in_pMatrix)	\
    (This)->lpVtbl -> Copy(This,in_pMatrix)

#define ISIMatrix4_Set(This,in_dM00,in_dM01,in_dM02,in_dM03,in_dM10,in_dM11,in_dM12,in_dM13,in_dM20,in_dM21,in_dM22,in_dM23,in_dM30,in_dM31,in_dM32,in_dM33)	\
    (This)->lpVtbl -> Set(This,in_dM00,in_dM01,in_dM02,in_dM03,in_dM10,in_dM11,in_dM12,in_dM13,in_dM20,in_dM21,in_dM22,in_dM23,in_dM30,in_dM31,in_dM32,in_dM33)

#define ISIMatrix4_Get(This,out_pvarM00,out_pvarM01,out_pvarM02,out_pvarM03,out_pvarM10,out_pvarM11,out_pvarM12,out_pvarM13,out_pvarM20,out_pvarM21,out_pvarM22,out_pvarM23,out_pvarM30,out_pvarM31,out_pvarM32,out_pvarM33)	\
    (This)->lpVtbl -> Get(This,out_pvarM00,out_pvarM01,out_pvarM02,out_pvarM03,out_pvarM10,out_pvarM11,out_pvarM12,out_pvarM13,out_pvarM20,out_pvarM21,out_pvarM22,out_pvarM23,out_pvarM30,out_pvarM31,out_pvarM32,out_pvarM33)

#define ISIMatrix4_Mul(This,in_pMatrix1,in_pMatrix2)	\
    (This)->lpVtbl -> Mul(This,in_pMatrix1,in_pMatrix2)

#define ISIMatrix4_MulInPlace(This,in_pMatrix)	\
    (This)->lpVtbl -> MulInPlace(This,in_pMatrix)

#define ISIMatrix4_Transpose(This,in_pMatrix)	\
    (This)->lpVtbl -> Transpose(This,in_pMatrix)

#define ISIMatrix4_TransposeInPlace(This)	\
    (This)->lpVtbl -> TransposeInPlace(This)

#define ISIMatrix4_TransposeInverse(This,in_pMatrix,out_pbInverted)	\
    (This)->lpVtbl -> TransposeInverse(This,in_pMatrix,out_pbInverted)

#define ISIMatrix4_TransposeInverseInPlace(This,out_pbInverted)	\
    (This)->lpVtbl -> TransposeInverseInPlace(This,out_pbInverted)

#define ISIMatrix4_Invert(This,in_pMatrix,out_pbInverted)	\
    (This)->lpVtbl -> Invert(This,in_pMatrix,out_pbInverted)

#define ISIMatrix4_InvertInPlace(This,out_pbInverted)	\
    (This)->lpVtbl -> InvertInPlace(This,out_pbInverted)

#define ISIMatrix4_SetIdentity(This)	\
    (This)->lpVtbl -> SetIdentity(This)

#define ISIMatrix4_Get2(This,out_pVal)	\
    (This)->lpVtbl -> Get2(This,out_pVal)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE ISIMatrix4_get_Value_Proxy( 
    ISIMatrix4 * This,
    /* [in] */ short in_nRow,
    /* [in] */ short in_nCol,
    /* [retval][out] */ double *pVal);


void __RPC_STUB ISIMatrix4_get_Value_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE ISIMatrix4_put_Value_Proxy( 
    ISIMatrix4 * This,
    /* [in] */ short in_nRow,
    /* [in] */ short in_nCol,
    /* [in] */ double newVal);


void __RPC_STUB ISIMatrix4_put_Value_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ISIMatrix4_Copy_Proxy( 
    ISIMatrix4 * This,
    /* [in] */ ISIMatrix4 *in_pMatrix);


void __RPC_STUB ISIMatrix4_Copy_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ISIMatrix4_Set_Proxy( 
    ISIMatrix4 * This,
    /* [in] */ double in_dM00,
    /* [in] */ double in_dM01,
    /* [in] */ double in_dM02,
    /* [in] */ double in_dM03,
    /* [in] */ double in_dM10,
    /* [in] */ double in_dM11,
    /* [in] */ double in_dM12,
    /* [in] */ double in_dM13,
    /* [in] */ double in_dM20,
    /* [in] */ double in_dM21,
    /* [in] */ double in_dM22,
    /* [in] */ double in_dM23,
    /* [in] */ double in_dM30,
    /* [in] */ double in_dM31,
    /* [in] */ double in_dM32,
    /* [in] */ double in_dM33);


void __RPC_STUB ISIMatrix4_Set_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ISIMatrix4_Get_Proxy( 
    ISIMatrix4 * This,
    /* [out] */ VARIANT *out_pvarM00,
    /* [out] */ VARIANT *out_pvarM01,
    /* [out] */ VARIANT *out_pvarM02,
    /* [out] */ VARIANT *out_pvarM03,
    /* [out] */ VARIANT *out_pvarM10,
    /* [out] */ VARIANT *out_pvarM11,
    /* [out] */ VARIANT *out_pvarM12,
    /* [out] */ VARIANT *out_pvarM13,
    /* [out] */ VARIANT *out_pvarM20,
    /* [out] */ VARIANT *out_pvarM21,
    /* [out] */ VARIANT *out_pvarM22,
    /* [out] */ VARIANT *out_pvarM23,
    /* [out] */ VARIANT *out_pvarM30,
    /* [out] */ VARIANT *out_pvarM31,
    /* [out] */ VARIANT *out_pvarM32,
    /* [out] */ VARIANT *out_pvarM33);


void __RPC_STUB ISIMatrix4_Get_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ISIMatrix4_Mul_Proxy( 
    ISIMatrix4 * This,
    /* [in] */ ISIMatrix4 *in_pMatrix1,
    /* [in] */ ISIMatrix4 *in_pMatrix2);


void __RPC_STUB ISIMatrix4_Mul_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ISIMatrix4_MulInPlace_Proxy( 
    ISIMatrix4 * This,
    /* [in] */ ISIMatrix4 *in_pMatrix);


void __RPC_STUB ISIMatrix4_MulInPlace_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ISIMatrix4_Transpose_Proxy( 
    ISIMatrix4 * This,
    /* [in] */ ISIMatrix4 *in_pMatrix);


void __RPC_STUB ISIMatrix4_Transpose_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ISIMatrix4_TransposeInPlace_Proxy( 
    ISIMatrix4 * This);


void __RPC_STUB ISIMatrix4_TransposeInPlace_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ISIMatrix4_TransposeInverse_Proxy( 
    ISIMatrix4 * This,
    /* [in] */ ISIMatrix4 *in_pMatrix,
    /* [retval][out] */ BOOL *out_pbInverted);


void __RPC_STUB ISIMatrix4_TransposeInverse_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ISIMatrix4_TransposeInverseInPlace_Proxy( 
    ISIMatrix4 * This,
    /* [retval][out] */ BOOL *out_pbInverted);


void __RPC_STUB ISIMatrix4_TransposeInverseInPlace_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ISIMatrix4_Invert_Proxy( 
    ISIMatrix4 * This,
    /* [in] */ ISIMatrix4 *in_pMatrix,
    /* [retval][out] */ BOOL *out_pbInverted);


void __RPC_STUB ISIMatrix4_Invert_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ISIMatrix4_InvertInPlace_Proxy( 
    ISIMatrix4 * This,
    /* [retval][out] */ BOOL *out_pbInverted);


void __RPC_STUB ISIMatrix4_InvertInPlace_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ISIMatrix4_SetIdentity_Proxy( 
    ISIMatrix4 * This);


void __RPC_STUB ISIMatrix4_SetIdentity_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ISIMatrix4_Get2_Proxy( 
    ISIMatrix4 * This,
    /* [retval][out] */ VARIANT *out_pVal);


void __RPC_STUB ISIMatrix4_Get2_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __ISIMatrix4_INTERFACE_DEFINED__ */


#ifndef __ISIQuaternion_INTERFACE_DEFINED__
#define __ISIQuaternion_INTERFACE_DEFINED__

/* interface ISIQuaternion */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_ISIQuaternion;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("BFB3B919-24CC-11D3-BC3A-00A0C9612B97")
    ISIQuaternion : public IDispatch
    {
    public:
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Value( 
            /* [in] */ short in_nIndex,
            /* [retval][out] */ double *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_Value( 
            /* [in] */ short in_nIndex,
            /* [in] */ double newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_X( 
            /* [retval][out] */ double *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_X( 
            /* [in] */ double newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Y( 
            /* [retval][out] */ double *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_Y( 
            /* [in] */ double newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Z( 
            /* [retval][out] */ double *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_Z( 
            /* [in] */ double newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_W( 
            /* [retval][out] */ double *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_W( 
            /* [in] */ double newVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Set( 
            /* [in] */ double in_dW,
            /* [in] */ double in_dX,
            /* [in] */ double in_dY,
            /* [in] */ double in_dZ) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Get( 
            /* [out] */ VARIANT *out_pvarW,
            /* [out] */ VARIANT *out_pvarX,
            /* [out] */ VARIANT *out_pvarY,
            /* [out] */ VARIANT *out_pvarZ) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Copy( 
            /* [in] */ ISIQuaternion *in_pSrcQuat) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SetIdentity( void) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Invert( 
            /* [in] */ ISIQuaternion *in_pQuat) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE InvertInPlace( void) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Conjugate( 
            /* [in] */ ISIQuaternion *in_pQuat) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ConjugateInPlace( void) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Add( 
            /* [in] */ ISIQuaternion *in_pQuat1,
            /* [in] */ ISIQuaternion *in_pQuat2) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE AddInPlace( 
            /* [in] */ ISIQuaternion *in_pQuat) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Length( 
            /* [retval][out] */ double *out_pdLength) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE LengthSquared( 
            /* [retval][out] */ double *out_pdLength) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Negate( 
            /* [in] */ ISIQuaternion *in_pQuat) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE NegateInPlace( void) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Sub( 
            /* [in] */ ISIQuaternion *in_pQuat1,
            /* [in] */ ISIQuaternion *in_pQuat2) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SubInPlace( 
            /* [in] */ ISIQuaternion *in_pQuat) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Mul( 
            /* [in] */ ISIQuaternion *in_pQuat1,
            /* [in] */ ISIQuaternion *in_pQuat2) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE MulInPlace( 
            /* [in] */ ISIQuaternion *in_pQuat) = 0;
        
        virtual /* [helpstring][id][hidden] */ HRESULT STDMETHODCALLTYPE _Obsolete( 
            /* [in] */ ISIQuaternion *in_pQuat1,
            /* [in] */ ISIQuaternion *in_pQuat2,
            /* [in] */ double in_dU) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Equals( 
            /* [in] */ ISIQuaternion *in_pQuat,
            /* [retval][out] */ BOOL *out_pbEquality) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE EpsilonEquals( 
            /* [in] */ ISIQuaternion *in_pQuat,
            /* [in] */ double in_dEpsilon,
            /* [retval][out] */ BOOL *out_pbEquality) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Normalize( void) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Get2( 
            /* [retval][out] */ VARIANT *out_pVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SetFromXYZAngleValues( 
            /* [in] */ double in_dX,
            /* [in] */ double in_dY,
            /* [in] */ double in_dZ) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetXYZAngleValues( 
            /* [out] */ VARIANT *in_vX,
            /* [out] */ VARIANT *in_vY,
            /* [out] */ VARIANT *in_vZ) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetXYZAngleValues2( 
            /* [retval][out] */ VARIANT *out_pVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Slerp( 
            /* [in] */ ISIQuaternion *in_pQuat1,
            /* [in] */ ISIQuaternion *in_pQuat2,
            /* [in] */ double in_dU) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct ISIQuaternionVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            ISIQuaternion * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            ISIQuaternion * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            ISIQuaternion * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            ISIQuaternion * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            ISIQuaternion * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            ISIQuaternion * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            ISIQuaternion * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Value )( 
            ISIQuaternion * This,
            /* [in] */ short in_nIndex,
            /* [retval][out] */ double *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_Value )( 
            ISIQuaternion * This,
            /* [in] */ short in_nIndex,
            /* [in] */ double newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_X )( 
            ISIQuaternion * This,
            /* [retval][out] */ double *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_X )( 
            ISIQuaternion * This,
            /* [in] */ double newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Y )( 
            ISIQuaternion * This,
            /* [retval][out] */ double *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_Y )( 
            ISIQuaternion * This,
            /* [in] */ double newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Z )( 
            ISIQuaternion * This,
            /* [retval][out] */ double *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_Z )( 
            ISIQuaternion * This,
            /* [in] */ double newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_W )( 
            ISIQuaternion * This,
            /* [retval][out] */ double *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_W )( 
            ISIQuaternion * This,
            /* [in] */ double newVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Set )( 
            ISIQuaternion * This,
            /* [in] */ double in_dW,
            /* [in] */ double in_dX,
            /* [in] */ double in_dY,
            /* [in] */ double in_dZ);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Get )( 
            ISIQuaternion * This,
            /* [out] */ VARIANT *out_pvarW,
            /* [out] */ VARIANT *out_pvarX,
            /* [out] */ VARIANT *out_pvarY,
            /* [out] */ VARIANT *out_pvarZ);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Copy )( 
            ISIQuaternion * This,
            /* [in] */ ISIQuaternion *in_pSrcQuat);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SetIdentity )( 
            ISIQuaternion * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Invert )( 
            ISIQuaternion * This,
            /* [in] */ ISIQuaternion *in_pQuat);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *InvertInPlace )( 
            ISIQuaternion * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Conjugate )( 
            ISIQuaternion * This,
            /* [in] */ ISIQuaternion *in_pQuat);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *ConjugateInPlace )( 
            ISIQuaternion * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Add )( 
            ISIQuaternion * This,
            /* [in] */ ISIQuaternion *in_pQuat1,
            /* [in] */ ISIQuaternion *in_pQuat2);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *AddInPlace )( 
            ISIQuaternion * This,
            /* [in] */ ISIQuaternion *in_pQuat);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Length )( 
            ISIQuaternion * This,
            /* [retval][out] */ double *out_pdLength);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *LengthSquared )( 
            ISIQuaternion * This,
            /* [retval][out] */ double *out_pdLength);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Negate )( 
            ISIQuaternion * This,
            /* [in] */ ISIQuaternion *in_pQuat);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *NegateInPlace )( 
            ISIQuaternion * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Sub )( 
            ISIQuaternion * This,
            /* [in] */ ISIQuaternion *in_pQuat1,
            /* [in] */ ISIQuaternion *in_pQuat2);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SubInPlace )( 
            ISIQuaternion * This,
            /* [in] */ ISIQuaternion *in_pQuat);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Mul )( 
            ISIQuaternion * This,
            /* [in] */ ISIQuaternion *in_pQuat1,
            /* [in] */ ISIQuaternion *in_pQuat2);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *MulInPlace )( 
            ISIQuaternion * This,
            /* [in] */ ISIQuaternion *in_pQuat);
        
        /* [helpstring][id][hidden] */ HRESULT ( STDMETHODCALLTYPE *_Obsolete )( 
            ISIQuaternion * This,
            /* [in] */ ISIQuaternion *in_pQuat1,
            /* [in] */ ISIQuaternion *in_pQuat2,
            /* [in] */ double in_dU);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Equals )( 
            ISIQuaternion * This,
            /* [in] */ ISIQuaternion *in_pQuat,
            /* [retval][out] */ BOOL *out_pbEquality);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *EpsilonEquals )( 
            ISIQuaternion * This,
            /* [in] */ ISIQuaternion *in_pQuat,
            /* [in] */ double in_dEpsilon,
            /* [retval][out] */ BOOL *out_pbEquality);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Normalize )( 
            ISIQuaternion * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Get2 )( 
            ISIQuaternion * This,
            /* [retval][out] */ VARIANT *out_pVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SetFromXYZAngleValues )( 
            ISIQuaternion * This,
            /* [in] */ double in_dX,
            /* [in] */ double in_dY,
            /* [in] */ double in_dZ);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetXYZAngleValues )( 
            ISIQuaternion * This,
            /* [out] */ VARIANT *in_vX,
            /* [out] */ VARIANT *in_vY,
            /* [out] */ VARIANT *in_vZ);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetXYZAngleValues2 )( 
            ISIQuaternion * This,
            /* [retval][out] */ VARIANT *out_pVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Slerp )( 
            ISIQuaternion * This,
            /* [in] */ ISIQuaternion *in_pQuat1,
            /* [in] */ ISIQuaternion *in_pQuat2,
            /* [in] */ double in_dU);
        
        END_INTERFACE
    } ISIQuaternionVtbl;

    interface ISIQuaternion
    {
        CONST_VTBL struct ISIQuaternionVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ISIQuaternion_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define ISIQuaternion_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define ISIQuaternion_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define ISIQuaternion_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define ISIQuaternion_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define ISIQuaternion_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define ISIQuaternion_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define ISIQuaternion_get_Value(This,in_nIndex,pVal)	\
    (This)->lpVtbl -> get_Value(This,in_nIndex,pVal)

#define ISIQuaternion_put_Value(This,in_nIndex,newVal)	\
    (This)->lpVtbl -> put_Value(This,in_nIndex,newVal)

#define ISIQuaternion_get_X(This,pVal)	\
    (This)->lpVtbl -> get_X(This,pVal)

#define ISIQuaternion_put_X(This,newVal)	\
    (This)->lpVtbl -> put_X(This,newVal)

#define ISIQuaternion_get_Y(This,pVal)	\
    (This)->lpVtbl -> get_Y(This,pVal)

#define ISIQuaternion_put_Y(This,newVal)	\
    (This)->lpVtbl -> put_Y(This,newVal)

#define ISIQuaternion_get_Z(This,pVal)	\
    (This)->lpVtbl -> get_Z(This,pVal)

#define ISIQuaternion_put_Z(This,newVal)	\
    (This)->lpVtbl -> put_Z(This,newVal)

#define ISIQuaternion_get_W(This,pVal)	\
    (This)->lpVtbl -> get_W(This,pVal)

#define ISIQuaternion_put_W(This,newVal)	\
    (This)->lpVtbl -> put_W(This,newVal)

#define ISIQuaternion_Set(This,in_dW,in_dX,in_dY,in_dZ)	\
    (This)->lpVtbl -> Set(This,in_dW,in_dX,in_dY,in_dZ)

#define ISIQuaternion_Get(This,out_pvarW,out_pvarX,out_pvarY,out_pvarZ)	\
    (This)->lpVtbl -> Get(This,out_pvarW,out_pvarX,out_pvarY,out_pvarZ)

#define ISIQuaternion_Copy(This,in_pSrcQuat)	\
    (This)->lpVtbl -> Copy(This,in_pSrcQuat)

#define ISIQuaternion_SetIdentity(This)	\
    (This)->lpVtbl -> SetIdentity(This)

#define ISIQuaternion_Invert(This,in_pQuat)	\
    (This)->lpVtbl -> Invert(This,in_pQuat)

#define ISIQuaternion_InvertInPlace(This)	\
    (This)->lpVtbl -> InvertInPlace(This)

#define ISIQuaternion_Conjugate(This,in_pQuat)	\
    (This)->lpVtbl -> Conjugate(This,in_pQuat)

#define ISIQuaternion_ConjugateInPlace(This)	\
    (This)->lpVtbl -> ConjugateInPlace(This)

#define ISIQuaternion_Add(This,in_pQuat1,in_pQuat2)	\
    (This)->lpVtbl -> Add(This,in_pQuat1,in_pQuat2)

#define ISIQuaternion_AddInPlace(This,in_pQuat)	\
    (This)->lpVtbl -> AddInPlace(This,in_pQuat)

#define ISIQuaternion_Length(This,out_pdLength)	\
    (This)->lpVtbl -> Length(This,out_pdLength)

#define ISIQuaternion_LengthSquared(This,out_pdLength)	\
    (This)->lpVtbl -> LengthSquared(This,out_pdLength)

#define ISIQuaternion_Negate(This,in_pQuat)	\
    (This)->lpVtbl -> Negate(This,in_pQuat)

#define ISIQuaternion_NegateInPlace(This)	\
    (This)->lpVtbl -> NegateInPlace(This)

#define ISIQuaternion_Sub(This,in_pQuat1,in_pQuat2)	\
    (This)->lpVtbl -> Sub(This,in_pQuat1,in_pQuat2)

#define ISIQuaternion_SubInPlace(This,in_pQuat)	\
    (This)->lpVtbl -> SubInPlace(This,in_pQuat)

#define ISIQuaternion_Mul(This,in_pQuat1,in_pQuat2)	\
    (This)->lpVtbl -> Mul(This,in_pQuat1,in_pQuat2)

#define ISIQuaternion_MulInPlace(This,in_pQuat)	\
    (This)->lpVtbl -> MulInPlace(This,in_pQuat)

#define ISIQuaternion__Obsolete(This,in_pQuat1,in_pQuat2,in_dU)	\
    (This)->lpVtbl -> _Obsolete(This,in_pQuat1,in_pQuat2,in_dU)

#define ISIQuaternion_Equals(This,in_pQuat,out_pbEquality)	\
    (This)->lpVtbl -> Equals(This,in_pQuat,out_pbEquality)

#define ISIQuaternion_EpsilonEquals(This,in_pQuat,in_dEpsilon,out_pbEquality)	\
    (This)->lpVtbl -> EpsilonEquals(This,in_pQuat,in_dEpsilon,out_pbEquality)

#define ISIQuaternion_Normalize(This)	\
    (This)->lpVtbl -> Normalize(This)

#define ISIQuaternion_Get2(This,out_pVal)	\
    (This)->lpVtbl -> Get2(This,out_pVal)

#define ISIQuaternion_SetFromXYZAngleValues(This,in_dX,in_dY,in_dZ)	\
    (This)->lpVtbl -> SetFromXYZAngleValues(This,in_dX,in_dY,in_dZ)

#define ISIQuaternion_GetXYZAngleValues(This,in_vX,in_vY,in_vZ)	\
    (This)->lpVtbl -> GetXYZAngleValues(This,in_vX,in_vY,in_vZ)

#define ISIQuaternion_GetXYZAngleValues2(This,out_pVal)	\
    (This)->lpVtbl -> GetXYZAngleValues2(This,out_pVal)

#define ISIQuaternion_Slerp(This,in_pQuat1,in_pQuat2,in_dU)	\
    (This)->lpVtbl -> Slerp(This,in_pQuat1,in_pQuat2,in_dU)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE ISIQuaternion_get_Value_Proxy( 
    ISIQuaternion * This,
    /* [in] */ short in_nIndex,
    /* [retval][out] */ double *pVal);


void __RPC_STUB ISIQuaternion_get_Value_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE ISIQuaternion_put_Value_Proxy( 
    ISIQuaternion * This,
    /* [in] */ short in_nIndex,
    /* [in] */ double newVal);


void __RPC_STUB ISIQuaternion_put_Value_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE ISIQuaternion_get_X_Proxy( 
    ISIQuaternion * This,
    /* [retval][out] */ double *pVal);


void __RPC_STUB ISIQuaternion_get_X_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE ISIQuaternion_put_X_Proxy( 
    ISIQuaternion * This,
    /* [in] */ double newVal);


void __RPC_STUB ISIQuaternion_put_X_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE ISIQuaternion_get_Y_Proxy( 
    ISIQuaternion * This,
    /* [retval][out] */ double *pVal);


void __RPC_STUB ISIQuaternion_get_Y_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE ISIQuaternion_put_Y_Proxy( 
    ISIQuaternion * This,
    /* [in] */ double newVal);


void __RPC_STUB ISIQuaternion_put_Y_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE ISIQuaternion_get_Z_Proxy( 
    ISIQuaternion * This,
    /* [retval][out] */ double *pVal);


void __RPC_STUB ISIQuaternion_get_Z_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE ISIQuaternion_put_Z_Proxy( 
    ISIQuaternion * This,
    /* [in] */ double newVal);


void __RPC_STUB ISIQuaternion_put_Z_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE ISIQuaternion_get_W_Proxy( 
    ISIQuaternion * This,
    /* [retval][out] */ double *pVal);


void __RPC_STUB ISIQuaternion_get_W_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE ISIQuaternion_put_W_Proxy( 
    ISIQuaternion * This,
    /* [in] */ double newVal);


void __RPC_STUB ISIQuaternion_put_W_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ISIQuaternion_Set_Proxy( 
    ISIQuaternion * This,
    /* [in] */ double in_dW,
    /* [in] */ double in_dX,
    /* [in] */ double in_dY,
    /* [in] */ double in_dZ);


void __RPC_STUB ISIQuaternion_Set_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ISIQuaternion_Get_Proxy( 
    ISIQuaternion * This,
    /* [out] */ VARIANT *out_pvarW,
    /* [out] */ VARIANT *out_pvarX,
    /* [out] */ VARIANT *out_pvarY,
    /* [out] */ VARIANT *out_pvarZ);


void __RPC_STUB ISIQuaternion_Get_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ISIQuaternion_Copy_Proxy( 
    ISIQuaternion * This,
    /* [in] */ ISIQuaternion *in_pSrcQuat);


void __RPC_STUB ISIQuaternion_Copy_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ISIQuaternion_SetIdentity_Proxy( 
    ISIQuaternion * This);


void __RPC_STUB ISIQuaternion_SetIdentity_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ISIQuaternion_Invert_Proxy( 
    ISIQuaternion * This,
    /* [in] */ ISIQuaternion *in_pQuat);


void __RPC_STUB ISIQuaternion_Invert_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ISIQuaternion_InvertInPlace_Proxy( 
    ISIQuaternion * This);


void __RPC_STUB ISIQuaternion_InvertInPlace_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ISIQuaternion_Conjugate_Proxy( 
    ISIQuaternion * This,
    /* [in] */ ISIQuaternion *in_pQuat);


void __RPC_STUB ISIQuaternion_Conjugate_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ISIQuaternion_ConjugateInPlace_Proxy( 
    ISIQuaternion * This);


void __RPC_STUB ISIQuaternion_ConjugateInPlace_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ISIQuaternion_Add_Proxy( 
    ISIQuaternion * This,
    /* [in] */ ISIQuaternion *in_pQuat1,
    /* [in] */ ISIQuaternion *in_pQuat2);


void __RPC_STUB ISIQuaternion_Add_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ISIQuaternion_AddInPlace_Proxy( 
    ISIQuaternion * This,
    /* [in] */ ISIQuaternion *in_pQuat);


void __RPC_STUB ISIQuaternion_AddInPlace_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ISIQuaternion_Length_Proxy( 
    ISIQuaternion * This,
    /* [retval][out] */ double *out_pdLength);


void __RPC_STUB ISIQuaternion_Length_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ISIQuaternion_LengthSquared_Proxy( 
    ISIQuaternion * This,
    /* [retval][out] */ double *out_pdLength);


void __RPC_STUB ISIQuaternion_LengthSquared_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ISIQuaternion_Negate_Proxy( 
    ISIQuaternion * This,
    /* [in] */ ISIQuaternion *in_pQuat);


void __RPC_STUB ISIQuaternion_Negate_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ISIQuaternion_NegateInPlace_Proxy( 
    ISIQuaternion * This);


void __RPC_STUB ISIQuaternion_NegateInPlace_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ISIQuaternion_Sub_Proxy( 
    ISIQuaternion * This,
    /* [in] */ ISIQuaternion *in_pQuat1,
    /* [in] */ ISIQuaternion *in_pQuat2);


void __RPC_STUB ISIQuaternion_Sub_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ISIQuaternion_SubInPlace_Proxy( 
    ISIQuaternion * This,
    /* [in] */ ISIQuaternion *in_pQuat);


void __RPC_STUB ISIQuaternion_SubInPlace_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ISIQuaternion_Mul_Proxy( 
    ISIQuaternion * This,
    /* [in] */ ISIQuaternion *in_pQuat1,
    /* [in] */ ISIQuaternion *in_pQuat2);


void __RPC_STUB ISIQuaternion_Mul_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ISIQuaternion_MulInPlace_Proxy( 
    ISIQuaternion * This,
    /* [in] */ ISIQuaternion *in_pQuat);


void __RPC_STUB ISIQuaternion_MulInPlace_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][hidden] */ HRESULT STDMETHODCALLTYPE ISIQuaternion__Obsolete_Proxy( 
    ISIQuaternion * This,
    /* [in] */ ISIQuaternion *in_pQuat1,
    /* [in] */ ISIQuaternion *in_pQuat2,
    /* [in] */ double in_dU);


void __RPC_STUB ISIQuaternion__Obsolete_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ISIQuaternion_Equals_Proxy( 
    ISIQuaternion * This,
    /* [in] */ ISIQuaternion *in_pQuat,
    /* [retval][out] */ BOOL *out_pbEquality);


void __RPC_STUB ISIQuaternion_Equals_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ISIQuaternion_EpsilonEquals_Proxy( 
    ISIQuaternion * This,
    /* [in] */ ISIQuaternion *in_pQuat,
    /* [in] */ double in_dEpsilon,
    /* [retval][out] */ BOOL *out_pbEquality);


void __RPC_STUB ISIQuaternion_EpsilonEquals_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ISIQuaternion_Normalize_Proxy( 
    ISIQuaternion * This);


void __RPC_STUB ISIQuaternion_Normalize_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ISIQuaternion_Get2_Proxy( 
    ISIQuaternion * This,
    /* [retval][out] */ VARIANT *out_pVal);


void __RPC_STUB ISIQuaternion_Get2_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ISIQuaternion_SetFromXYZAngleValues_Proxy( 
    ISIQuaternion * This,
    /* [in] */ double in_dX,
    /* [in] */ double in_dY,
    /* [in] */ double in_dZ);


void __RPC_STUB ISIQuaternion_SetFromXYZAngleValues_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ISIQuaternion_GetXYZAngleValues_Proxy( 
    ISIQuaternion * This,
    /* [out] */ VARIANT *in_vX,
    /* [out] */ VARIANT *in_vY,
    /* [out] */ VARIANT *in_vZ);


void __RPC_STUB ISIQuaternion_GetXYZAngleValues_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ISIQuaternion_GetXYZAngleValues2_Proxy( 
    ISIQuaternion * This,
    /* [retval][out] */ VARIANT *out_pVal);


void __RPC_STUB ISIQuaternion_GetXYZAngleValues2_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ISIQuaternion_Slerp_Proxy( 
    ISIQuaternion * This,
    /* [in] */ ISIQuaternion *in_pQuat1,
    /* [in] */ ISIQuaternion *in_pQuat2,
    /* [in] */ double in_dU);


void __RPC_STUB ISIQuaternion_Slerp_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __ISIQuaternion_INTERFACE_DEFINED__ */


#ifndef __ISIRotation_INTERFACE_DEFINED__
#define __ISIRotation_INTERFACE_DEFINED__

/* interface ISIRotation */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_ISIRotation;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("BFB3B917-24CC-11D3-BC3A-00A0C9612B97")
    ISIRotation : public IDispatch
    {
    public:
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SetIdentity( void) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetMatrix3( 
            /* [in] */ ISIMatrix3 *io_pMatrix) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SetFromXYZAngles( 
            /* [in] */ ISIVector3 *in_pXZYAngles) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetXYZAngles( 
            /* [in] */ ISIVector3 *io_pXZYAngles) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SetFromAxisAngle( 
            /* [in] */ ISIVector3 *in_pAxis,
            /* [in] */ double in_dAngle) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetAxisAngle( 
            /* [in] */ ISIVector3 *io_pAxis,
            /* [out] */ VARIANT *io_pvarAngle) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SetFromQuaternion( 
            /* [in] */ ISIQuaternion *in_pQuat) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetQuaternion( 
            /* [in] */ ISIQuaternion *io_pQuat) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Invert( 
            /* [in] */ ISIRotation *in_pRot) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE InvertInPlace( void) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Mul( 
            /* [in] */ ISIRotation *in_pRot1,
            /* [in] */ ISIRotation *in_pRot2) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE MulInPlace( 
            /* [in] */ ISIRotation *in_pRot) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Copy( 
            /* [in] */ ISIRotation *in_pSrcRot) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SetFromXYZAxes( 
            /* [in] */ ISIVector3 *in_pXAxis,
            /* [in] */ ISIVector3 *in_pYAxis,
            /* [in] */ ISIVector3 *in_pZAxis) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetXYZAnglesValues( 
            /* [out] */ VARIANT *out_pvarX,
            /* [out] */ VARIANT *out_pvarY,
            /* [out] */ VARIANT *out_pvarZ) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SetFromXYZAnglesValues( 
            /* [in] */ double in_dX,
            /* [in] */ double in_dY,
            /* [in] */ double in_dZ) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetAxisAngle2( 
            /* [in] */ ISIVector3 *io_pAxis,
            /* [retval][out] */ VARIANT *io_pvarAngle) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetXYZAnglesValues2( 
            /* [retval][out] */ VARIANT *out_pVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SetFromMatrix3( 
            /* [in] */ ISIMatrix3 *io_pMatrix) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_RotX( 
            /* [in] */ double in_RotX) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_RotX( 
            /* [retval][out] */ double *out_RotX) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_RotY( 
            /* [in] */ double in_RotY) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_RotY( 
            /* [retval][out] */ double *out_RotY) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_RotZ( 
            /* [in] */ double in_RotZ) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_RotZ( 
            /* [retval][out] */ double *out_RotZ) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_Quaternion( 
            /* [in] */ ISIQuaternion *in_pQuat) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Quaternion( 
            /* [retval][out] */ ISIQuaternion **out_pQuat) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_XYZAngles( 
            /* [in] */ ISIVector3 *in_pXYZAngles) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_XYZAngles( 
            /* [retval][out] */ ISIVector3 **out_pXYZAngles) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct ISIRotationVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            ISIRotation * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            ISIRotation * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            ISIRotation * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            ISIRotation * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            ISIRotation * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            ISIRotation * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            ISIRotation * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SetIdentity )( 
            ISIRotation * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetMatrix3 )( 
            ISIRotation * This,
            /* [in] */ ISIMatrix3 *io_pMatrix);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SetFromXYZAngles )( 
            ISIRotation * This,
            /* [in] */ ISIVector3 *in_pXZYAngles);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetXYZAngles )( 
            ISIRotation * This,
            /* [in] */ ISIVector3 *io_pXZYAngles);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SetFromAxisAngle )( 
            ISIRotation * This,
            /* [in] */ ISIVector3 *in_pAxis,
            /* [in] */ double in_dAngle);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetAxisAngle )( 
            ISIRotation * This,
            /* [in] */ ISIVector3 *io_pAxis,
            /* [out] */ VARIANT *io_pvarAngle);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SetFromQuaternion )( 
            ISIRotation * This,
            /* [in] */ ISIQuaternion *in_pQuat);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetQuaternion )( 
            ISIRotation * This,
            /* [in] */ ISIQuaternion *io_pQuat);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Invert )( 
            ISIRotation * This,
            /* [in] */ ISIRotation *in_pRot);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *InvertInPlace )( 
            ISIRotation * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Mul )( 
            ISIRotation * This,
            /* [in] */ ISIRotation *in_pRot1,
            /* [in] */ ISIRotation *in_pRot2);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *MulInPlace )( 
            ISIRotation * This,
            /* [in] */ ISIRotation *in_pRot);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Copy )( 
            ISIRotation * This,
            /* [in] */ ISIRotation *in_pSrcRot);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SetFromXYZAxes )( 
            ISIRotation * This,
            /* [in] */ ISIVector3 *in_pXAxis,
            /* [in] */ ISIVector3 *in_pYAxis,
            /* [in] */ ISIVector3 *in_pZAxis);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetXYZAnglesValues )( 
            ISIRotation * This,
            /* [out] */ VARIANT *out_pvarX,
            /* [out] */ VARIANT *out_pvarY,
            /* [out] */ VARIANT *out_pvarZ);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SetFromXYZAnglesValues )( 
            ISIRotation * This,
            /* [in] */ double in_dX,
            /* [in] */ double in_dY,
            /* [in] */ double in_dZ);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetAxisAngle2 )( 
            ISIRotation * This,
            /* [in] */ ISIVector3 *io_pAxis,
            /* [retval][out] */ VARIANT *io_pvarAngle);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetXYZAnglesValues2 )( 
            ISIRotation * This,
            /* [retval][out] */ VARIANT *out_pVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SetFromMatrix3 )( 
            ISIRotation * This,
            /* [in] */ ISIMatrix3 *io_pMatrix);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_RotX )( 
            ISIRotation * This,
            /* [in] */ double in_RotX);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_RotX )( 
            ISIRotation * This,
            /* [retval][out] */ double *out_RotX);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_RotY )( 
            ISIRotation * This,
            /* [in] */ double in_RotY);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_RotY )( 
            ISIRotation * This,
            /* [retval][out] */ double *out_RotY);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_RotZ )( 
            ISIRotation * This,
            /* [in] */ double in_RotZ);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_RotZ )( 
            ISIRotation * This,
            /* [retval][out] */ double *out_RotZ);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_Quaternion )( 
            ISIRotation * This,
            /* [in] */ ISIQuaternion *in_pQuat);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Quaternion )( 
            ISIRotation * This,
            /* [retval][out] */ ISIQuaternion **out_pQuat);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_XYZAngles )( 
            ISIRotation * This,
            /* [in] */ ISIVector3 *in_pXYZAngles);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_XYZAngles )( 
            ISIRotation * This,
            /* [retval][out] */ ISIVector3 **out_pXYZAngles);
        
        END_INTERFACE
    } ISIRotationVtbl;

    interface ISIRotation
    {
        CONST_VTBL struct ISIRotationVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ISIRotation_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define ISIRotation_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define ISIRotation_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define ISIRotation_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define ISIRotation_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define ISIRotation_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define ISIRotation_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define ISIRotation_SetIdentity(This)	\
    (This)->lpVtbl -> SetIdentity(This)

#define ISIRotation_GetMatrix3(This,io_pMatrix)	\
    (This)->lpVtbl -> GetMatrix3(This,io_pMatrix)

#define ISIRotation_SetFromXYZAngles(This,in_pXZYAngles)	\
    (This)->lpVtbl -> SetFromXYZAngles(This,in_pXZYAngles)

#define ISIRotation_GetXYZAngles(This,io_pXZYAngles)	\
    (This)->lpVtbl -> GetXYZAngles(This,io_pXZYAngles)

#define ISIRotation_SetFromAxisAngle(This,in_pAxis,in_dAngle)	\
    (This)->lpVtbl -> SetFromAxisAngle(This,in_pAxis,in_dAngle)

#define ISIRotation_GetAxisAngle(This,io_pAxis,io_pvarAngle)	\
    (This)->lpVtbl -> GetAxisAngle(This,io_pAxis,io_pvarAngle)

#define ISIRotation_SetFromQuaternion(This,in_pQuat)	\
    (This)->lpVtbl -> SetFromQuaternion(This,in_pQuat)

#define ISIRotation_GetQuaternion(This,io_pQuat)	\
    (This)->lpVtbl -> GetQuaternion(This,io_pQuat)

#define ISIRotation_Invert(This,in_pRot)	\
    (This)->lpVtbl -> Invert(This,in_pRot)

#define ISIRotation_InvertInPlace(This)	\
    (This)->lpVtbl -> InvertInPlace(This)

#define ISIRotation_Mul(This,in_pRot1,in_pRot2)	\
    (This)->lpVtbl -> Mul(This,in_pRot1,in_pRot2)

#define ISIRotation_MulInPlace(This,in_pRot)	\
    (This)->lpVtbl -> MulInPlace(This,in_pRot)

#define ISIRotation_Copy(This,in_pSrcRot)	\
    (This)->lpVtbl -> Copy(This,in_pSrcRot)

#define ISIRotation_SetFromXYZAxes(This,in_pXAxis,in_pYAxis,in_pZAxis)	\
    (This)->lpVtbl -> SetFromXYZAxes(This,in_pXAxis,in_pYAxis,in_pZAxis)

#define ISIRotation_GetXYZAnglesValues(This,out_pvarX,out_pvarY,out_pvarZ)	\
    (This)->lpVtbl -> GetXYZAnglesValues(This,out_pvarX,out_pvarY,out_pvarZ)

#define ISIRotation_SetFromXYZAnglesValues(This,in_dX,in_dY,in_dZ)	\
    (This)->lpVtbl -> SetFromXYZAnglesValues(This,in_dX,in_dY,in_dZ)

#define ISIRotation_GetAxisAngle2(This,io_pAxis,io_pvarAngle)	\
    (This)->lpVtbl -> GetAxisAngle2(This,io_pAxis,io_pvarAngle)

#define ISIRotation_GetXYZAnglesValues2(This,out_pVal)	\
    (This)->lpVtbl -> GetXYZAnglesValues2(This,out_pVal)

#define ISIRotation_SetFromMatrix3(This,io_pMatrix)	\
    (This)->lpVtbl -> SetFromMatrix3(This,io_pMatrix)

#define ISIRotation_put_RotX(This,in_RotX)	\
    (This)->lpVtbl -> put_RotX(This,in_RotX)

#define ISIRotation_get_RotX(This,out_RotX)	\
    (This)->lpVtbl -> get_RotX(This,out_RotX)

#define ISIRotation_put_RotY(This,in_RotY)	\
    (This)->lpVtbl -> put_RotY(This,in_RotY)

#define ISIRotation_get_RotY(This,out_RotY)	\
    (This)->lpVtbl -> get_RotY(This,out_RotY)

#define ISIRotation_put_RotZ(This,in_RotZ)	\
    (This)->lpVtbl -> put_RotZ(This,in_RotZ)

#define ISIRotation_get_RotZ(This,out_RotZ)	\
    (This)->lpVtbl -> get_RotZ(This,out_RotZ)

#define ISIRotation_put_Quaternion(This,in_pQuat)	\
    (This)->lpVtbl -> put_Quaternion(This,in_pQuat)

#define ISIRotation_get_Quaternion(This,out_pQuat)	\
    (This)->lpVtbl -> get_Quaternion(This,out_pQuat)

#define ISIRotation_put_XYZAngles(This,in_pXYZAngles)	\
    (This)->lpVtbl -> put_XYZAngles(This,in_pXYZAngles)

#define ISIRotation_get_XYZAngles(This,out_pXYZAngles)	\
    (This)->lpVtbl -> get_XYZAngles(This,out_pXYZAngles)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ISIRotation_SetIdentity_Proxy( 
    ISIRotation * This);


void __RPC_STUB ISIRotation_SetIdentity_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ISIRotation_GetMatrix3_Proxy( 
    ISIRotation * This,
    /* [in] */ ISIMatrix3 *io_pMatrix);


void __RPC_STUB ISIRotation_GetMatrix3_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ISIRotation_SetFromXYZAngles_Proxy( 
    ISIRotation * This,
    /* [in] */ ISIVector3 *in_pXZYAngles);


void __RPC_STUB ISIRotation_SetFromXYZAngles_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ISIRotation_GetXYZAngles_Proxy( 
    ISIRotation * This,
    /* [in] */ ISIVector3 *io_pXZYAngles);


void __RPC_STUB ISIRotation_GetXYZAngles_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ISIRotation_SetFromAxisAngle_Proxy( 
    ISIRotation * This,
    /* [in] */ ISIVector3 *in_pAxis,
    /* [in] */ double in_dAngle);


void __RPC_STUB ISIRotation_SetFromAxisAngle_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ISIRotation_GetAxisAngle_Proxy( 
    ISIRotation * This,
    /* [in] */ ISIVector3 *io_pAxis,
    /* [out] */ VARIANT *io_pvarAngle);


void __RPC_STUB ISIRotation_GetAxisAngle_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ISIRotation_SetFromQuaternion_Proxy( 
    ISIRotation * This,
    /* [in] */ ISIQuaternion *in_pQuat);


void __RPC_STUB ISIRotation_SetFromQuaternion_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ISIRotation_GetQuaternion_Proxy( 
    ISIRotation * This,
    /* [in] */ ISIQuaternion *io_pQuat);


void __RPC_STUB ISIRotation_GetQuaternion_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ISIRotation_Invert_Proxy( 
    ISIRotation * This,
    /* [in] */ ISIRotation *in_pRot);


void __RPC_STUB ISIRotation_Invert_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ISIRotation_InvertInPlace_Proxy( 
    ISIRotation * This);


void __RPC_STUB ISIRotation_InvertInPlace_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ISIRotation_Mul_Proxy( 
    ISIRotation * This,
    /* [in] */ ISIRotation *in_pRot1,
    /* [in] */ ISIRotation *in_pRot2);


void __RPC_STUB ISIRotation_Mul_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ISIRotation_MulInPlace_Proxy( 
    ISIRotation * This,
    /* [in] */ ISIRotation *in_pRot);


void __RPC_STUB ISIRotation_MulInPlace_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ISIRotation_Copy_Proxy( 
    ISIRotation * This,
    /* [in] */ ISIRotation *in_pSrcRot);


void __RPC_STUB ISIRotation_Copy_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ISIRotation_SetFromXYZAxes_Proxy( 
    ISIRotation * This,
    /* [in] */ ISIVector3 *in_pXAxis,
    /* [in] */ ISIVector3 *in_pYAxis,
    /* [in] */ ISIVector3 *in_pZAxis);


void __RPC_STUB ISIRotation_SetFromXYZAxes_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ISIRotation_GetXYZAnglesValues_Proxy( 
    ISIRotation * This,
    /* [out] */ VARIANT *out_pvarX,
    /* [out] */ VARIANT *out_pvarY,
    /* [out] */ VARIANT *out_pvarZ);


void __RPC_STUB ISIRotation_GetXYZAnglesValues_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ISIRotation_SetFromXYZAnglesValues_Proxy( 
    ISIRotation * This,
    /* [in] */ double in_dX,
    /* [in] */ double in_dY,
    /* [in] */ double in_dZ);


void __RPC_STUB ISIRotation_SetFromXYZAnglesValues_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ISIRotation_GetAxisAngle2_Proxy( 
    ISIRotation * This,
    /* [in] */ ISIVector3 *io_pAxis,
    /* [retval][out] */ VARIANT *io_pvarAngle);


void __RPC_STUB ISIRotation_GetAxisAngle2_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ISIRotation_GetXYZAnglesValues2_Proxy( 
    ISIRotation * This,
    /* [retval][out] */ VARIANT *out_pVal);


void __RPC_STUB ISIRotation_GetXYZAnglesValues2_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ISIRotation_SetFromMatrix3_Proxy( 
    ISIRotation * This,
    /* [in] */ ISIMatrix3 *io_pMatrix);


void __RPC_STUB ISIRotation_SetFromMatrix3_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE ISIRotation_put_RotX_Proxy( 
    ISIRotation * This,
    /* [in] */ double in_RotX);


void __RPC_STUB ISIRotation_put_RotX_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE ISIRotation_get_RotX_Proxy( 
    ISIRotation * This,
    /* [retval][out] */ double *out_RotX);


void __RPC_STUB ISIRotation_get_RotX_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE ISIRotation_put_RotY_Proxy( 
    ISIRotation * This,
    /* [in] */ double in_RotY);


void __RPC_STUB ISIRotation_put_RotY_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE ISIRotation_get_RotY_Proxy( 
    ISIRotation * This,
    /* [retval][out] */ double *out_RotY);


void __RPC_STUB ISIRotation_get_RotY_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE ISIRotation_put_RotZ_Proxy( 
    ISIRotation * This,
    /* [in] */ double in_RotZ);


void __RPC_STUB ISIRotation_put_RotZ_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE ISIRotation_get_RotZ_Proxy( 
    ISIRotation * This,
    /* [retval][out] */ double *out_RotZ);


void __RPC_STUB ISIRotation_get_RotZ_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE ISIRotation_put_Quaternion_Proxy( 
    ISIRotation * This,
    /* [in] */ ISIQuaternion *in_pQuat);


void __RPC_STUB ISIRotation_put_Quaternion_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE ISIRotation_get_Quaternion_Proxy( 
    ISIRotation * This,
    /* [retval][out] */ ISIQuaternion **out_pQuat);


void __RPC_STUB ISIRotation_get_Quaternion_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE ISIRotation_put_XYZAngles_Proxy( 
    ISIRotation * This,
    /* [in] */ ISIVector3 *in_pXYZAngles);


void __RPC_STUB ISIRotation_put_XYZAngles_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE ISIRotation_get_XYZAngles_Proxy( 
    ISIRotation * This,
    /* [retval][out] */ ISIVector3 **out_pXYZAngles);


void __RPC_STUB ISIRotation_get_XYZAngles_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __ISIRotation_INTERFACE_DEFINED__ */


#ifndef __ISITransformation_INTERFACE_DEFINED__
#define __ISITransformation_INTERFACE_DEFINED__

/* interface ISITransformation */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_ISITransformation;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("BFB3B91B-24CC-11D3-BC3A-00A0C9612B97")
    ISITransformation : public IDispatch
    {
    public:
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetScaling( 
            /* [in] */ ISIVector3 *io_pScale) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SetScaling( 
            /* [in] */ ISIVector3 *in_pScale) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetRotation( 
            /* [in] */ ISIRotation *io_pRotation) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetRotationMatrix3( 
            /* [in] */ ISIMatrix3 *io_pRotMat) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetRotationXYZAngles( 
            /* [in] */ ISIVector3 *io_pXYZAngles) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetRotationAxisAngle( 
            /* [in] */ ISIVector3 *io_pAxis,
            /* [out] */ VARIANT *io_pvarAngle) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetRotationQuaternion( 
            /* [in] */ ISIQuaternion *io_pQuat) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SetRotation( 
            /* [in] */ ISIRotation *in_pRotation) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SetRotationFromXYZAngles( 
            /* [in] */ ISIVector3 *in_pXYZAngles) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SetRotationFromAxisAngle( 
            /* [in] */ ISIVector3 *in_pAxis,
            /* [in] */ double in_dAngle) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SetRotationFromQuaternion( 
            /* [in] */ ISIQuaternion *in_pQuat) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetTranslation( 
            /* [in] */ ISIVector3 *io_pTransVect) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SetTranslation( 
            /* [in] */ ISIVector3 *in_pTransVect) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SetIdentity( void) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Copy( 
            /* [in] */ ISITransformation *in_pSrcTransfo) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetScalingValues( 
            /* [out] */ VARIANT *out_pvarX,
            /* [out] */ VARIANT *out_pvarY,
            /* [out] */ VARIANT *out_pvarZ) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SetScalingFromValues( 
            /* [in] */ double in_dX,
            /* [in] */ double in_dY,
            /* [in] */ double in_dZ) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetTranslationValues( 
            /* [out] */ VARIANT *out_pvarX,
            /* [out] */ VARIANT *out_pvarY,
            /* [out] */ VARIANT *out_pvarZ) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SetTranslationFromValues( 
            /* [in] */ double in_dX,
            /* [in] */ double in_dY,
            /* [in] */ double in_dZ) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SetRotationFromXYZAxes( 
            /* [in] */ ISIVector3 *in_pXAxis,
            /* [in] */ ISIVector3 *in_pYAxis,
            /* [in] */ ISIVector3 *in_pZAxis) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE AddLocalScaling( 
            /* [in] */ ISIVector3 *in_pScaling) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE AddParentScaling( 
            /* [in] */ ISIVector3 *in_pScaling) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE AddLocalRotation( 
            /* [in] */ ISIRotation *in_pRotation,
            /* [defaultvalue][optional][in] */ BOOL in_bIncrementEulerAngles = FALSE) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE AddParentRotation( 
            /* [in] */ ISIRotation *in_pRotation,
            /* [defaultvalue][optional][in] */ BOOL in_bIncrementEulerAngles = FALSE) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE AddLocalTranslation( 
            /* [in] */ ISIVector3 *in_pTranslation) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE AddParentTranslation( 
            /* [in] */ ISIVector3 *in_pTranslation) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Mul( 
            /* [in] */ ISITransformation *in_pTransfo1,
            /* [in] */ ISITransformation *in_pTransfo2) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE MulInPlace( 
            /* [in] */ ISITransformation *in_pTransfo) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetRotationXYZAnglesValues( 
            /* [out] */ VARIANT *out_pvarX,
            /* [out] */ VARIANT *out_pvarY,
            /* [out] */ VARIANT *out_pvarZ) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SetRotationFromXYZAnglesValues( 
            /* [in] */ double in_dX,
            /* [in] */ double in_dY,
            /* [in] */ double in_dZ) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetMatrix4( 
            /* [in] */ ISIMatrix4 *io_pMatrix) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SetMatrix4( 
            /* [in] */ ISIMatrix4 *in_pMatrix) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetScalingOrientationXYZAngles( 
            /* [out] */ VARIANT *out_pvarX,
            /* [out] */ VARIANT *out_pvarY,
            /* [out] */ VARIANT *out_pvarZ) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE HasScalingOrientation( 
            /* [retval][out] */ VARIANT_BOOL *out_pbSclOri) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SetScalingOrientationFromXYZAngles( 
            /* [in] */ double in_dX,
            /* [in] */ double in_dY,
            /* [in] */ double in_dZ) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetRotationAxisAngle2( 
            /* [in] */ ISIVector3 *io_pAxis,
            /* [retval][out] */ VARIANT *out_pRetValues) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetScalingValues2( 
            /* [retval][out] */ VARIANT *out_pRetValues) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetTranslationValues2( 
            /* [retval][out] */ VARIANT *out_pRetValues) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetRotationXYZAnglesValues2( 
            /* [retval][out] */ VARIANT *out_pRetValues) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetScalingOrientationXYZAngles2( 
            /* [retval][out] */ VARIANT *out_pRetValues) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SetRotationFromMatrix3( 
            /* [in] */ ISIMatrix3 *in_pRotMat) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_PosX( 
            /* [in] */ double in_PosX) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_PosX( 
            /* [retval][out] */ double *out_PosX) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_PosY( 
            /* [in] */ double in_PosY) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_PosY( 
            /* [retval][out] */ double *out_PosY) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_PosZ( 
            /* [in] */ double in_PosZ) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_PosZ( 
            /* [retval][out] */ double *out_PosZ) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_RotX( 
            /* [in] */ double in_RotX) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_RotX( 
            /* [retval][out] */ double *out_RotX) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_RotY( 
            /* [in] */ double in_RotY) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_RotY( 
            /* [retval][out] */ double *out_RotY) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_RotZ( 
            /* [in] */ double in_RotZ) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_RotZ( 
            /* [retval][out] */ double *out_RotZ) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_SclX( 
            /* [in] */ double in_SclX) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_SclX( 
            /* [retval][out] */ double *out_SclX) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_SclY( 
            /* [in] */ double in_SclY) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_SclY( 
            /* [retval][out] */ double *out_SclY) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_SclZ( 
            /* [in] */ double in_SclZ) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_SclZ( 
            /* [retval][out] */ double *out_SclZ) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_Translation( 
            /* [in] */ ISIVector3 *in_pVector3) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Translation( 
            /* [retval][out] */ ISIVector3 **out_pVector3) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_Rotation( 
            /* [in] */ ISIRotation *in_pRotation) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Rotation( 
            /* [retval][out] */ ISIRotation **out_pRotation) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_Matrix4( 
            /* [in] */ ISIMatrix4 *in_pMatrix4) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Matrix4( 
            /* [retval][out] */ ISIMatrix4 **out_pMatrix4) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_Scaling( 
            /* [in] */ ISIVector3 *in_pSIVector3) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Scaling( 
            /* [retval][out] */ ISIVector3 **out_pSIVector3) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct ISITransformationVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            ISITransformation * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            ISITransformation * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            ISITransformation * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            ISITransformation * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            ISITransformation * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            ISITransformation * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            ISITransformation * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetScaling )( 
            ISITransformation * This,
            /* [in] */ ISIVector3 *io_pScale);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SetScaling )( 
            ISITransformation * This,
            /* [in] */ ISIVector3 *in_pScale);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetRotation )( 
            ISITransformation * This,
            /* [in] */ ISIRotation *io_pRotation);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetRotationMatrix3 )( 
            ISITransformation * This,
            /* [in] */ ISIMatrix3 *io_pRotMat);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetRotationXYZAngles )( 
            ISITransformation * This,
            /* [in] */ ISIVector3 *io_pXYZAngles);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetRotationAxisAngle )( 
            ISITransformation * This,
            /* [in] */ ISIVector3 *io_pAxis,
            /* [out] */ VARIANT *io_pvarAngle);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetRotationQuaternion )( 
            ISITransformation * This,
            /* [in] */ ISIQuaternion *io_pQuat);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SetRotation )( 
            ISITransformation * This,
            /* [in] */ ISIRotation *in_pRotation);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SetRotationFromXYZAngles )( 
            ISITransformation * This,
            /* [in] */ ISIVector3 *in_pXYZAngles);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SetRotationFromAxisAngle )( 
            ISITransformation * This,
            /* [in] */ ISIVector3 *in_pAxis,
            /* [in] */ double in_dAngle);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SetRotationFromQuaternion )( 
            ISITransformation * This,
            /* [in] */ ISIQuaternion *in_pQuat);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetTranslation )( 
            ISITransformation * This,
            /* [in] */ ISIVector3 *io_pTransVect);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SetTranslation )( 
            ISITransformation * This,
            /* [in] */ ISIVector3 *in_pTransVect);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SetIdentity )( 
            ISITransformation * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Copy )( 
            ISITransformation * This,
            /* [in] */ ISITransformation *in_pSrcTransfo);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetScalingValues )( 
            ISITransformation * This,
            /* [out] */ VARIANT *out_pvarX,
            /* [out] */ VARIANT *out_pvarY,
            /* [out] */ VARIANT *out_pvarZ);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SetScalingFromValues )( 
            ISITransformation * This,
            /* [in] */ double in_dX,
            /* [in] */ double in_dY,
            /* [in] */ double in_dZ);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetTranslationValues )( 
            ISITransformation * This,
            /* [out] */ VARIANT *out_pvarX,
            /* [out] */ VARIANT *out_pvarY,
            /* [out] */ VARIANT *out_pvarZ);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SetTranslationFromValues )( 
            ISITransformation * This,
            /* [in] */ double in_dX,
            /* [in] */ double in_dY,
            /* [in] */ double in_dZ);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SetRotationFromXYZAxes )( 
            ISITransformation * This,
            /* [in] */ ISIVector3 *in_pXAxis,
            /* [in] */ ISIVector3 *in_pYAxis,
            /* [in] */ ISIVector3 *in_pZAxis);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *AddLocalScaling )( 
            ISITransformation * This,
            /* [in] */ ISIVector3 *in_pScaling);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *AddParentScaling )( 
            ISITransformation * This,
            /* [in] */ ISIVector3 *in_pScaling);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *AddLocalRotation )( 
            ISITransformation * This,
            /* [in] */ ISIRotation *in_pRotation,
            /* [defaultvalue][optional][in] */ BOOL in_bIncrementEulerAngles);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *AddParentRotation )( 
            ISITransformation * This,
            /* [in] */ ISIRotation *in_pRotation,
            /* [defaultvalue][optional][in] */ BOOL in_bIncrementEulerAngles);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *AddLocalTranslation )( 
            ISITransformation * This,
            /* [in] */ ISIVector3 *in_pTranslation);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *AddParentTranslation )( 
            ISITransformation * This,
            /* [in] */ ISIVector3 *in_pTranslation);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Mul )( 
            ISITransformation * This,
            /* [in] */ ISITransformation *in_pTransfo1,
            /* [in] */ ISITransformation *in_pTransfo2);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *MulInPlace )( 
            ISITransformation * This,
            /* [in] */ ISITransformation *in_pTransfo);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetRotationXYZAnglesValues )( 
            ISITransformation * This,
            /* [out] */ VARIANT *out_pvarX,
            /* [out] */ VARIANT *out_pvarY,
            /* [out] */ VARIANT *out_pvarZ);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SetRotationFromXYZAnglesValues )( 
            ISITransformation * This,
            /* [in] */ double in_dX,
            /* [in] */ double in_dY,
            /* [in] */ double in_dZ);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetMatrix4 )( 
            ISITransformation * This,
            /* [in] */ ISIMatrix4 *io_pMatrix);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SetMatrix4 )( 
            ISITransformation * This,
            /* [in] */ ISIMatrix4 *in_pMatrix);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetScalingOrientationXYZAngles )( 
            ISITransformation * This,
            /* [out] */ VARIANT *out_pvarX,
            /* [out] */ VARIANT *out_pvarY,
            /* [out] */ VARIANT *out_pvarZ);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *HasScalingOrientation )( 
            ISITransformation * This,
            /* [retval][out] */ VARIANT_BOOL *out_pbSclOri);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SetScalingOrientationFromXYZAngles )( 
            ISITransformation * This,
            /* [in] */ double in_dX,
            /* [in] */ double in_dY,
            /* [in] */ double in_dZ);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetRotationAxisAngle2 )( 
            ISITransformation * This,
            /* [in] */ ISIVector3 *io_pAxis,
            /* [retval][out] */ VARIANT *out_pRetValues);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetScalingValues2 )( 
            ISITransformation * This,
            /* [retval][out] */ VARIANT *out_pRetValues);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetTranslationValues2 )( 
            ISITransformation * This,
            /* [retval][out] */ VARIANT *out_pRetValues);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetRotationXYZAnglesValues2 )( 
            ISITransformation * This,
            /* [retval][out] */ VARIANT *out_pRetValues);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetScalingOrientationXYZAngles2 )( 
            ISITransformation * This,
            /* [retval][out] */ VARIANT *out_pRetValues);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SetRotationFromMatrix3 )( 
            ISITransformation * This,
            /* [in] */ ISIMatrix3 *in_pRotMat);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_PosX )( 
            ISITransformation * This,
            /* [in] */ double in_PosX);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_PosX )( 
            ISITransformation * This,
            /* [retval][out] */ double *out_PosX);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_PosY )( 
            ISITransformation * This,
            /* [in] */ double in_PosY);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_PosY )( 
            ISITransformation * This,
            /* [retval][out] */ double *out_PosY);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_PosZ )( 
            ISITransformation * This,
            /* [in] */ double in_PosZ);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_PosZ )( 
            ISITransformation * This,
            /* [retval][out] */ double *out_PosZ);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_RotX )( 
            ISITransformation * This,
            /* [in] */ double in_RotX);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_RotX )( 
            ISITransformation * This,
            /* [retval][out] */ double *out_RotX);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_RotY )( 
            ISITransformation * This,
            /* [in] */ double in_RotY);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_RotY )( 
            ISITransformation * This,
            /* [retval][out] */ double *out_RotY);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_RotZ )( 
            ISITransformation * This,
            /* [in] */ double in_RotZ);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_RotZ )( 
            ISITransformation * This,
            /* [retval][out] */ double *out_RotZ);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_SclX )( 
            ISITransformation * This,
            /* [in] */ double in_SclX);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_SclX )( 
            ISITransformation * This,
            /* [retval][out] */ double *out_SclX);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_SclY )( 
            ISITransformation * This,
            /* [in] */ double in_SclY);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_SclY )( 
            ISITransformation * This,
            /* [retval][out] */ double *out_SclY);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_SclZ )( 
            ISITransformation * This,
            /* [in] */ double in_SclZ);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_SclZ )( 
            ISITransformation * This,
            /* [retval][out] */ double *out_SclZ);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_Translation )( 
            ISITransformation * This,
            /* [in] */ ISIVector3 *in_pVector3);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Translation )( 
            ISITransformation * This,
            /* [retval][out] */ ISIVector3 **out_pVector3);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_Rotation )( 
            ISITransformation * This,
            /* [in] */ ISIRotation *in_pRotation);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Rotation )( 
            ISITransformation * This,
            /* [retval][out] */ ISIRotation **out_pRotation);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_Matrix4 )( 
            ISITransformation * This,
            /* [in] */ ISIMatrix4 *in_pMatrix4);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Matrix4 )( 
            ISITransformation * This,
            /* [retval][out] */ ISIMatrix4 **out_pMatrix4);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_Scaling )( 
            ISITransformation * This,
            /* [in] */ ISIVector3 *in_pSIVector3);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Scaling )( 
            ISITransformation * This,
            /* [retval][out] */ ISIVector3 **out_pSIVector3);
        
        END_INTERFACE
    } ISITransformationVtbl;

    interface ISITransformation
    {
        CONST_VTBL struct ISITransformationVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ISITransformation_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define ISITransformation_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define ISITransformation_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define ISITransformation_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define ISITransformation_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define ISITransformation_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define ISITransformation_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define ISITransformation_GetScaling(This,io_pScale)	\
    (This)->lpVtbl -> GetScaling(This,io_pScale)

#define ISITransformation_SetScaling(This,in_pScale)	\
    (This)->lpVtbl -> SetScaling(This,in_pScale)

#define ISITransformation_GetRotation(This,io_pRotation)	\
    (This)->lpVtbl -> GetRotation(This,io_pRotation)

#define ISITransformation_GetRotationMatrix3(This,io_pRotMat)	\
    (This)->lpVtbl -> GetRotationMatrix3(This,io_pRotMat)

#define ISITransformation_GetRotationXYZAngles(This,io_pXYZAngles)	\
    (This)->lpVtbl -> GetRotationXYZAngles(This,io_pXYZAngles)

#define ISITransformation_GetRotationAxisAngle(This,io_pAxis,io_pvarAngle)	\
    (This)->lpVtbl -> GetRotationAxisAngle(This,io_pAxis,io_pvarAngle)

#define ISITransformation_GetRotationQuaternion(This,io_pQuat)	\
    (This)->lpVtbl -> GetRotationQuaternion(This,io_pQuat)

#define ISITransformation_SetRotation(This,in_pRotation)	\
    (This)->lpVtbl -> SetRotation(This,in_pRotation)

#define ISITransformation_SetRotationFromXYZAngles(This,in_pXYZAngles)	\
    (This)->lpVtbl -> SetRotationFromXYZAngles(This,in_pXYZAngles)

#define ISITransformation_SetRotationFromAxisAngle(This,in_pAxis,in_dAngle)	\
    (This)->lpVtbl -> SetRotationFromAxisAngle(This,in_pAxis,in_dAngle)

#define ISITransformation_SetRotationFromQuaternion(This,in_pQuat)	\
    (This)->lpVtbl -> SetRotationFromQuaternion(This,in_pQuat)

#define ISITransformation_GetTranslation(This,io_pTransVect)	\
    (This)->lpVtbl -> GetTranslation(This,io_pTransVect)

#define ISITransformation_SetTranslation(This,in_pTransVect)	\
    (This)->lpVtbl -> SetTranslation(This,in_pTransVect)

#define ISITransformation_SetIdentity(This)	\
    (This)->lpVtbl -> SetIdentity(This)

#define ISITransformation_Copy(This,in_pSrcTransfo)	\
    (This)->lpVtbl -> Copy(This,in_pSrcTransfo)

#define ISITransformation_GetScalingValues(This,out_pvarX,out_pvarY,out_pvarZ)	\
    (This)->lpVtbl -> GetScalingValues(This,out_pvarX,out_pvarY,out_pvarZ)

#define ISITransformation_SetScalingFromValues(This,in_dX,in_dY,in_dZ)	\
    (This)->lpVtbl -> SetScalingFromValues(This,in_dX,in_dY,in_dZ)

#define ISITransformation_GetTranslationValues(This,out_pvarX,out_pvarY,out_pvarZ)	\
    (This)->lpVtbl -> GetTranslationValues(This,out_pvarX,out_pvarY,out_pvarZ)

#define ISITransformation_SetTranslationFromValues(This,in_dX,in_dY,in_dZ)	\
    (This)->lpVtbl -> SetTranslationFromValues(This,in_dX,in_dY,in_dZ)

#define ISITransformation_SetRotationFromXYZAxes(This,in_pXAxis,in_pYAxis,in_pZAxis)	\
    (This)->lpVtbl -> SetRotationFromXYZAxes(This,in_pXAxis,in_pYAxis,in_pZAxis)

#define ISITransformation_AddLocalScaling(This,in_pScaling)	\
    (This)->lpVtbl -> AddLocalScaling(This,in_pScaling)

#define ISITransformation_AddParentScaling(This,in_pScaling)	\
    (This)->lpVtbl -> AddParentScaling(This,in_pScaling)

#define ISITransformation_AddLocalRotation(This,in_pRotation,in_bIncrementEulerAngles)	\
    (This)->lpVtbl -> AddLocalRotation(This,in_pRotation,in_bIncrementEulerAngles)

#define ISITransformation_AddParentRotation(This,in_pRotation,in_bIncrementEulerAngles)	\
    (This)->lpVtbl -> AddParentRotation(This,in_pRotation,in_bIncrementEulerAngles)

#define ISITransformation_AddLocalTranslation(This,in_pTranslation)	\
    (This)->lpVtbl -> AddLocalTranslation(This,in_pTranslation)

#define ISITransformation_AddParentTranslation(This,in_pTranslation)	\
    (This)->lpVtbl -> AddParentTranslation(This,in_pTranslation)

#define ISITransformation_Mul(This,in_pTransfo1,in_pTransfo2)	\
    (This)->lpVtbl -> Mul(This,in_pTransfo1,in_pTransfo2)

#define ISITransformation_MulInPlace(This,in_pTransfo)	\
    (This)->lpVtbl -> MulInPlace(This,in_pTransfo)

#define ISITransformation_GetRotationXYZAnglesValues(This,out_pvarX,out_pvarY,out_pvarZ)	\
    (This)->lpVtbl -> GetRotationXYZAnglesValues(This,out_pvarX,out_pvarY,out_pvarZ)

#define ISITransformation_SetRotationFromXYZAnglesValues(This,in_dX,in_dY,in_dZ)	\
    (This)->lpVtbl -> SetRotationFromXYZAnglesValues(This,in_dX,in_dY,in_dZ)

#define ISITransformation_GetMatrix4(This,io_pMatrix)	\
    (This)->lpVtbl -> GetMatrix4(This,io_pMatrix)

#define ISITransformation_SetMatrix4(This,in_pMatrix)	\
    (This)->lpVtbl -> SetMatrix4(This,in_pMatrix)

#define ISITransformation_GetScalingOrientationXYZAngles(This,out_pvarX,out_pvarY,out_pvarZ)	\
    (This)->lpVtbl -> GetScalingOrientationXYZAngles(This,out_pvarX,out_pvarY,out_pvarZ)

#define ISITransformation_HasScalingOrientation(This,out_pbSclOri)	\
    (This)->lpVtbl -> HasScalingOrientation(This,out_pbSclOri)

#define ISITransformation_SetScalingOrientationFromXYZAngles(This,in_dX,in_dY,in_dZ)	\
    (This)->lpVtbl -> SetScalingOrientationFromXYZAngles(This,in_dX,in_dY,in_dZ)

#define ISITransformation_GetRotationAxisAngle2(This,io_pAxis,out_pRetValues)	\
    (This)->lpVtbl -> GetRotationAxisAngle2(This,io_pAxis,out_pRetValues)

#define ISITransformation_GetScalingValues2(This,out_pRetValues)	\
    (This)->lpVtbl -> GetScalingValues2(This,out_pRetValues)

#define ISITransformation_GetTranslationValues2(This,out_pRetValues)	\
    (This)->lpVtbl -> GetTranslationValues2(This,out_pRetValues)

#define ISITransformation_GetRotationXYZAnglesValues2(This,out_pRetValues)	\
    (This)->lpVtbl -> GetRotationXYZAnglesValues2(This,out_pRetValues)

#define ISITransformation_GetScalingOrientationXYZAngles2(This,out_pRetValues)	\
    (This)->lpVtbl -> GetScalingOrientationXYZAngles2(This,out_pRetValues)

#define ISITransformation_SetRotationFromMatrix3(This,in_pRotMat)	\
    (This)->lpVtbl -> SetRotationFromMatrix3(This,in_pRotMat)

#define ISITransformation_put_PosX(This,in_PosX)	\
    (This)->lpVtbl -> put_PosX(This,in_PosX)

#define ISITransformation_get_PosX(This,out_PosX)	\
    (This)->lpVtbl -> get_PosX(This,out_PosX)

#define ISITransformation_put_PosY(This,in_PosY)	\
    (This)->lpVtbl -> put_PosY(This,in_PosY)

#define ISITransformation_get_PosY(This,out_PosY)	\
    (This)->lpVtbl -> get_PosY(This,out_PosY)

#define ISITransformation_put_PosZ(This,in_PosZ)	\
    (This)->lpVtbl -> put_PosZ(This,in_PosZ)

#define ISITransformation_get_PosZ(This,out_PosZ)	\
    (This)->lpVtbl -> get_PosZ(This,out_PosZ)

#define ISITransformation_put_RotX(This,in_RotX)	\
    (This)->lpVtbl -> put_RotX(This,in_RotX)

#define ISITransformation_get_RotX(This,out_RotX)	\
    (This)->lpVtbl -> get_RotX(This,out_RotX)

#define ISITransformation_put_RotY(This,in_RotY)	\
    (This)->lpVtbl -> put_RotY(This,in_RotY)

#define ISITransformation_get_RotY(This,out_RotY)	\
    (This)->lpVtbl -> get_RotY(This,out_RotY)

#define ISITransformation_put_RotZ(This,in_RotZ)	\
    (This)->lpVtbl -> put_RotZ(This,in_RotZ)

#define ISITransformation_get_RotZ(This,out_RotZ)	\
    (This)->lpVtbl -> get_RotZ(This,out_RotZ)

#define ISITransformation_put_SclX(This,in_SclX)	\
    (This)->lpVtbl -> put_SclX(This,in_SclX)

#define ISITransformation_get_SclX(This,out_SclX)	\
    (This)->lpVtbl -> get_SclX(This,out_SclX)

#define ISITransformation_put_SclY(This,in_SclY)	\
    (This)->lpVtbl -> put_SclY(This,in_SclY)

#define ISITransformation_get_SclY(This,out_SclY)	\
    (This)->lpVtbl -> get_SclY(This,out_SclY)

#define ISITransformation_put_SclZ(This,in_SclZ)	\
    (This)->lpVtbl -> put_SclZ(This,in_SclZ)

#define ISITransformation_get_SclZ(This,out_SclZ)	\
    (This)->lpVtbl -> get_SclZ(This,out_SclZ)

#define ISITransformation_put_Translation(This,in_pVector3)	\
    (This)->lpVtbl -> put_Translation(This,in_pVector3)

#define ISITransformation_get_Translation(This,out_pVector3)	\
    (This)->lpVtbl -> get_Translation(This,out_pVector3)

#define ISITransformation_put_Rotation(This,in_pRotation)	\
    (This)->lpVtbl -> put_Rotation(This,in_pRotation)

#define ISITransformation_get_Rotation(This,out_pRotation)	\
    (This)->lpVtbl -> get_Rotation(This,out_pRotation)

#define ISITransformation_put_Matrix4(This,in_pMatrix4)	\
    (This)->lpVtbl -> put_Matrix4(This,in_pMatrix4)

#define ISITransformation_get_Matrix4(This,out_pMatrix4)	\
    (This)->lpVtbl -> get_Matrix4(This,out_pMatrix4)

#define ISITransformation_put_Scaling(This,in_pSIVector3)	\
    (This)->lpVtbl -> put_Scaling(This,in_pSIVector3)

#define ISITransformation_get_Scaling(This,out_pSIVector3)	\
    (This)->lpVtbl -> get_Scaling(This,out_pSIVector3)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ISITransformation_GetScaling_Proxy( 
    ISITransformation * This,
    /* [in] */ ISIVector3 *io_pScale);


void __RPC_STUB ISITransformation_GetScaling_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ISITransformation_SetScaling_Proxy( 
    ISITransformation * This,
    /* [in] */ ISIVector3 *in_pScale);


void __RPC_STUB ISITransformation_SetScaling_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ISITransformation_GetRotation_Proxy( 
    ISITransformation * This,
    /* [in] */ ISIRotation *io_pRotation);


void __RPC_STUB ISITransformation_GetRotation_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ISITransformation_GetRotationMatrix3_Proxy( 
    ISITransformation * This,
    /* [in] */ ISIMatrix3 *io_pRotMat);


void __RPC_STUB ISITransformation_GetRotationMatrix3_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ISITransformation_GetRotationXYZAngles_Proxy( 
    ISITransformation * This,
    /* [in] */ ISIVector3 *io_pXYZAngles);


void __RPC_STUB ISITransformation_GetRotationXYZAngles_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ISITransformation_GetRotationAxisAngle_Proxy( 
    ISITransformation * This,
    /* [in] */ ISIVector3 *io_pAxis,
    /* [out] */ VARIANT *io_pvarAngle);


void __RPC_STUB ISITransformation_GetRotationAxisAngle_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ISITransformation_GetRotationQuaternion_Proxy( 
    ISITransformation * This,
    /* [in] */ ISIQuaternion *io_pQuat);


void __RPC_STUB ISITransformation_GetRotationQuaternion_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ISITransformation_SetRotation_Proxy( 
    ISITransformation * This,
    /* [in] */ ISIRotation *in_pRotation);


void __RPC_STUB ISITransformation_SetRotation_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ISITransformation_SetRotationFromXYZAngles_Proxy( 
    ISITransformation * This,
    /* [in] */ ISIVector3 *in_pXYZAngles);


void __RPC_STUB ISITransformation_SetRotationFromXYZAngles_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ISITransformation_SetRotationFromAxisAngle_Proxy( 
    ISITransformation * This,
    /* [in] */ ISIVector3 *in_pAxis,
    /* [in] */ double in_dAngle);


void __RPC_STUB ISITransformation_SetRotationFromAxisAngle_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ISITransformation_SetRotationFromQuaternion_Proxy( 
    ISITransformation * This,
    /* [in] */ ISIQuaternion *in_pQuat);


void __RPC_STUB ISITransformation_SetRotationFromQuaternion_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ISITransformation_GetTranslation_Proxy( 
    ISITransformation * This,
    /* [in] */ ISIVector3 *io_pTransVect);


void __RPC_STUB ISITransformation_GetTranslation_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ISITransformation_SetTranslation_Proxy( 
    ISITransformation * This,
    /* [in] */ ISIVector3 *in_pTransVect);


void __RPC_STUB ISITransformation_SetTranslation_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ISITransformation_SetIdentity_Proxy( 
    ISITransformation * This);


void __RPC_STUB ISITransformation_SetIdentity_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ISITransformation_Copy_Proxy( 
    ISITransformation * This,
    /* [in] */ ISITransformation *in_pSrcTransfo);


void __RPC_STUB ISITransformation_Copy_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ISITransformation_GetScalingValues_Proxy( 
    ISITransformation * This,
    /* [out] */ VARIANT *out_pvarX,
    /* [out] */ VARIANT *out_pvarY,
    /* [out] */ VARIANT *out_pvarZ);


void __RPC_STUB ISITransformation_GetScalingValues_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ISITransformation_SetScalingFromValues_Proxy( 
    ISITransformation * This,
    /* [in] */ double in_dX,
    /* [in] */ double in_dY,
    /* [in] */ double in_dZ);


void __RPC_STUB ISITransformation_SetScalingFromValues_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ISITransformation_GetTranslationValues_Proxy( 
    ISITransformation * This,
    /* [out] */ VARIANT *out_pvarX,
    /* [out] */ VARIANT *out_pvarY,
    /* [out] */ VARIANT *out_pvarZ);


void __RPC_STUB ISITransformation_GetTranslationValues_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ISITransformation_SetTranslationFromValues_Proxy( 
    ISITransformation * This,
    /* [in] */ double in_dX,
    /* [in] */ double in_dY,
    /* [in] */ double in_dZ);


void __RPC_STUB ISITransformation_SetTranslationFromValues_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ISITransformation_SetRotationFromXYZAxes_Proxy( 
    ISITransformation * This,
    /* [in] */ ISIVector3 *in_pXAxis,
    /* [in] */ ISIVector3 *in_pYAxis,
    /* [in] */ ISIVector3 *in_pZAxis);


void __RPC_STUB ISITransformation_SetRotationFromXYZAxes_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ISITransformation_AddLocalScaling_Proxy( 
    ISITransformation * This,
    /* [in] */ ISIVector3 *in_pScaling);


void __RPC_STUB ISITransformation_AddLocalScaling_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ISITransformation_AddParentScaling_Proxy( 
    ISITransformation * This,
    /* [in] */ ISIVector3 *in_pScaling);


void __RPC_STUB ISITransformation_AddParentScaling_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ISITransformation_AddLocalRotation_Proxy( 
    ISITransformation * This,
    /* [in] */ ISIRotation *in_pRotation,
    /* [defaultvalue][optional][in] */ BOOL in_bIncrementEulerAngles);


void __RPC_STUB ISITransformation_AddLocalRotation_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ISITransformation_AddParentRotation_Proxy( 
    ISITransformation * This,
    /* [in] */ ISIRotation *in_pRotation,
    /* [defaultvalue][optional][in] */ BOOL in_bIncrementEulerAngles);


void __RPC_STUB ISITransformation_AddParentRotation_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ISITransformation_AddLocalTranslation_Proxy( 
    ISITransformation * This,
    /* [in] */ ISIVector3 *in_pTranslation);


void __RPC_STUB ISITransformation_AddLocalTranslation_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ISITransformation_AddParentTranslation_Proxy( 
    ISITransformation * This,
    /* [in] */ ISIVector3 *in_pTranslation);


void __RPC_STUB ISITransformation_AddParentTranslation_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ISITransformation_Mul_Proxy( 
    ISITransformation * This,
    /* [in] */ ISITransformation *in_pTransfo1,
    /* [in] */ ISITransformation *in_pTransfo2);


void __RPC_STUB ISITransformation_Mul_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ISITransformation_MulInPlace_Proxy( 
    ISITransformation * This,
    /* [in] */ ISITransformation *in_pTransfo);


void __RPC_STUB ISITransformation_MulInPlace_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ISITransformation_GetRotationXYZAnglesValues_Proxy( 
    ISITransformation * This,
    /* [out] */ VARIANT *out_pvarX,
    /* [out] */ VARIANT *out_pvarY,
    /* [out] */ VARIANT *out_pvarZ);


void __RPC_STUB ISITransformation_GetRotationXYZAnglesValues_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ISITransformation_SetRotationFromXYZAnglesValues_Proxy( 
    ISITransformation * This,
    /* [in] */ double in_dX,
    /* [in] */ double in_dY,
    /* [in] */ double in_dZ);


void __RPC_STUB ISITransformation_SetRotationFromXYZAnglesValues_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ISITransformation_GetMatrix4_Proxy( 
    ISITransformation * This,
    /* [in] */ ISIMatrix4 *io_pMatrix);


void __RPC_STUB ISITransformation_GetMatrix4_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ISITransformation_SetMatrix4_Proxy( 
    ISITransformation * This,
    /* [in] */ ISIMatrix4 *in_pMatrix);


void __RPC_STUB ISITransformation_SetMatrix4_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ISITransformation_GetScalingOrientationXYZAngles_Proxy( 
    ISITransformation * This,
    /* [out] */ VARIANT *out_pvarX,
    /* [out] */ VARIANT *out_pvarY,
    /* [out] */ VARIANT *out_pvarZ);


void __RPC_STUB ISITransformation_GetScalingOrientationXYZAngles_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ISITransformation_HasScalingOrientation_Proxy( 
    ISITransformation * This,
    /* [retval][out] */ VARIANT_BOOL *out_pbSclOri);


void __RPC_STUB ISITransformation_HasScalingOrientation_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ISITransformation_SetScalingOrientationFromXYZAngles_Proxy( 
    ISITransformation * This,
    /* [in] */ double in_dX,
    /* [in] */ double in_dY,
    /* [in] */ double in_dZ);


void __RPC_STUB ISITransformation_SetScalingOrientationFromXYZAngles_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ISITransformation_GetRotationAxisAngle2_Proxy( 
    ISITransformation * This,
    /* [in] */ ISIVector3 *io_pAxis,
    /* [retval][out] */ VARIANT *out_pRetValues);


void __RPC_STUB ISITransformation_GetRotationAxisAngle2_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ISITransformation_GetScalingValues2_Proxy( 
    ISITransformation * This,
    /* [retval][out] */ VARIANT *out_pRetValues);


void __RPC_STUB ISITransformation_GetScalingValues2_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ISITransformation_GetTranslationValues2_Proxy( 
    ISITransformation * This,
    /* [retval][out] */ VARIANT *out_pRetValues);


void __RPC_STUB ISITransformation_GetTranslationValues2_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ISITransformation_GetRotationXYZAnglesValues2_Proxy( 
    ISITransformation * This,
    /* [retval][out] */ VARIANT *out_pRetValues);


void __RPC_STUB ISITransformation_GetRotationXYZAnglesValues2_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ISITransformation_GetScalingOrientationXYZAngles2_Proxy( 
    ISITransformation * This,
    /* [retval][out] */ VARIANT *out_pRetValues);


void __RPC_STUB ISITransformation_GetScalingOrientationXYZAngles2_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ISITransformation_SetRotationFromMatrix3_Proxy( 
    ISITransformation * This,
    /* [in] */ ISIMatrix3 *in_pRotMat);


void __RPC_STUB ISITransformation_SetRotationFromMatrix3_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE ISITransformation_put_PosX_Proxy( 
    ISITransformation * This,
    /* [in] */ double in_PosX);


void __RPC_STUB ISITransformation_put_PosX_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE ISITransformation_get_PosX_Proxy( 
    ISITransformation * This,
    /* [retval][out] */ double *out_PosX);


void __RPC_STUB ISITransformation_get_PosX_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE ISITransformation_put_PosY_Proxy( 
    ISITransformation * This,
    /* [in] */ double in_PosY);


void __RPC_STUB ISITransformation_put_PosY_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE ISITransformation_get_PosY_Proxy( 
    ISITransformation * This,
    /* [retval][out] */ double *out_PosY);


void __RPC_STUB ISITransformation_get_PosY_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE ISITransformation_put_PosZ_Proxy( 
    ISITransformation * This,
    /* [in] */ double in_PosZ);


void __RPC_STUB ISITransformation_put_PosZ_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE ISITransformation_get_PosZ_Proxy( 
    ISITransformation * This,
    /* [retval][out] */ double *out_PosZ);


void __RPC_STUB ISITransformation_get_PosZ_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE ISITransformation_put_RotX_Proxy( 
    ISITransformation * This,
    /* [in] */ double in_RotX);


void __RPC_STUB ISITransformation_put_RotX_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE ISITransformation_get_RotX_Proxy( 
    ISITransformation * This,
    /* [retval][out] */ double *out_RotX);


void __RPC_STUB ISITransformation_get_RotX_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE ISITransformation_put_RotY_Proxy( 
    ISITransformation * This,
    /* [in] */ double in_RotY);


void __RPC_STUB ISITransformation_put_RotY_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE ISITransformation_get_RotY_Proxy( 
    ISITransformation * This,
    /* [retval][out] */ double *out_RotY);


void __RPC_STUB ISITransformation_get_RotY_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE ISITransformation_put_RotZ_Proxy( 
    ISITransformation * This,
    /* [in] */ double in_RotZ);


void __RPC_STUB ISITransformation_put_RotZ_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE ISITransformation_get_RotZ_Proxy( 
    ISITransformation * This,
    /* [retval][out] */ double *out_RotZ);


void __RPC_STUB ISITransformation_get_RotZ_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE ISITransformation_put_SclX_Proxy( 
    ISITransformation * This,
    /* [in] */ double in_SclX);


void __RPC_STUB ISITransformation_put_SclX_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE ISITransformation_get_SclX_Proxy( 
    ISITransformation * This,
    /* [retval][out] */ double *out_SclX);


void __RPC_STUB ISITransformation_get_SclX_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE ISITransformation_put_SclY_Proxy( 
    ISITransformation * This,
    /* [in] */ double in_SclY);


void __RPC_STUB ISITransformation_put_SclY_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE ISITransformation_get_SclY_Proxy( 
    ISITransformation * This,
    /* [retval][out] */ double *out_SclY);


void __RPC_STUB ISITransformation_get_SclY_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE ISITransformation_put_SclZ_Proxy( 
    ISITransformation * This,
    /* [in] */ double in_SclZ);


void __RPC_STUB ISITransformation_put_SclZ_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE ISITransformation_get_SclZ_Proxy( 
    ISITransformation * This,
    /* [retval][out] */ double *out_SclZ);


void __RPC_STUB ISITransformation_get_SclZ_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE ISITransformation_put_Translation_Proxy( 
    ISITransformation * This,
    /* [in] */ ISIVector3 *in_pVector3);


void __RPC_STUB ISITransformation_put_Translation_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE ISITransformation_get_Translation_Proxy( 
    ISITransformation * This,
    /* [retval][out] */ ISIVector3 **out_pVector3);


void __RPC_STUB ISITransformation_get_Translation_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE ISITransformation_put_Rotation_Proxy( 
    ISITransformation * This,
    /* [in] */ ISIRotation *in_pRotation);


void __RPC_STUB ISITransformation_put_Rotation_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE ISITransformation_get_Rotation_Proxy( 
    ISITransformation * This,
    /* [retval][out] */ ISIRotation **out_pRotation);


void __RPC_STUB ISITransformation_get_Rotation_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE ISITransformation_put_Matrix4_Proxy( 
    ISITransformation * This,
    /* [in] */ ISIMatrix4 *in_pMatrix4);


void __RPC_STUB ISITransformation_put_Matrix4_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE ISITransformation_get_Matrix4_Proxy( 
    ISITransformation * This,
    /* [retval][out] */ ISIMatrix4 **out_pMatrix4);


void __RPC_STUB ISITransformation_get_Matrix4_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE ISITransformation_put_Scaling_Proxy( 
    ISITransformation * This,
    /* [in] */ ISIVector3 *in_pSIVector3);


void __RPC_STUB ISITransformation_put_Scaling_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE ISITransformation_get_Scaling_Proxy( 
    ISITransformation * This,
    /* [retval][out] */ ISIVector3 **out_pSIVector3);


void __RPC_STUB ISITransformation_get_Scaling_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __ISITransformation_INTERFACE_DEFINED__ */


/* Additional Prototypes for ALL interfaces */

unsigned long             __RPC_USER  VARIANT_UserSize(     unsigned long *, unsigned long            , VARIANT * ); 
unsigned char * __RPC_USER  VARIANT_UserMarshal(  unsigned long *, unsigned char *, VARIANT * ); 
unsigned char * __RPC_USER  VARIANT_UserUnmarshal(unsigned long *, unsigned char *, VARIANT * ); 
void                      __RPC_USER  VARIANT_UserFree(     unsigned long *, VARIANT * ); 

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif



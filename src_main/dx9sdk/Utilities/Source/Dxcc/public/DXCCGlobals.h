#ifndef DXCCGLOBALS_H
#define DXCCGLOBALS_H

#define DEFSTDMETHOD(method)							HRESULT STDMETHODCALLTYPE method
#define DEFSTDMETHOD_(type,method)						type STDMETHODCALLTYPE method

//#if defined(DEBUG) | defined(_DEBUG)
#include "DXCCDebugPrintF.h"
//#endif


#define DXCC_RELEASE(pIUK)								DXCCInterfaceRelease((LPUNKNOWN*)&(pIUK))
#define DXCC_STATUS_EXIT(_hr,_Value,_e_Exit,_Break)		do{ (_hr)=(_Value); DXCC_GOTO_EXIT(_e_Exit, _Break); }while(0)
#define DXCC_GOTO_EXIT(_e_Exit, _Break)					do{ DXCC_ASSERT(!(_Break)); goto _e_Exit; }while(0)


//#if defined(DEBUG) | defined(_DEBUG)
	#define DXCC_ASSERT(_exp_)					        do{ if(!(_exp_)){ DXCC_DPF_HEADER(TEXT("ASSERT")); DXCC_DPF_BODY(TEXT( #_exp_ )); DXCCBreakPoint(); } }while(0)
	#define DXCC_ASSERT_RANGE(min, exp, max)	        DXCC_ASSERT(((min) >= (exp))&&((exp) <= (max)))
	#define DXCC_SUCCEEDED(_hr_)				        SUCCEEDED(_hr_)
	#define DXCC_FAILED(_hr_)					        ( FAILED(_hr_) ? DXCC_DPF_STATUS(_hr_), TRUE : FALSE )
	#define DXCC_CHECK_HRESULT( _hr_ )					do{ DXCC_ASSERT( SUCCEEDED( _hr_ ) ); }while(0)
/*
#else 
    #define DXCC_ASSERT(_exp_)	
	#define DXCC_ASSERT_RANGE(min, exp, max)
	#define DXCC_SUCCEEDED(_hr_)				        SUCCEEDED(_hr_)
	#define DXCC_FAILED(_hr_)					        FAILED(_hr_)
	#define DXCC_CHECK_HRESULT( _hr_ )					do{ _hr_; }while(0)
#endif
*/


typedef VOID (CALLBACK* DXCCBreakPointCallback)();




#define DXCC_DEFAULT_STRING_LENGTH 256

#ifdef DXCC_EXPORTS
	#define EXPORTDEF __declspec(dllexport)
#else
	#define EXPORTDEF
#endif

enum DXCCIteratorType
{
	DXCCITERATOR_DEPTHFIRST,
	DXCCITERATOR_BREADTH,
};

typedef UINT		DXCCHANDLE;

#ifdef __cplusplus
extern "C" {
#endif //__cplusplus

HINSTANCE DXCCGetHINSTANCE();

HMODULE DXCCGetHMODULE();

HRESULT WINAPI
	DXCCWindowlessSetup(
	HINSTANCE*			phOut_Instance,
	HWND*				phOut_Wnd,
	LPDIRECT3D9*		ppOut_D3D9,
	LPDIRECT3DDEVICE9*	ppOut_D3DDevice9);

ULONG DXCCInterfaceRelease(LPUNKNOWN* ppUnknown);

VOID cdecl DXCCSetBreakPointCallback( DXCCBreakPointCallback callback );
VOID CALLBACK DXCCBreakPointDefaultCallback();
//use macro DXCC_ASSERT instead of this;
VOID cdecl DXCCBreakPoint();

#ifdef __cplusplus
}
#endif //__cplusplus

#endif
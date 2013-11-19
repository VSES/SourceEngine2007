#ifndef DXCCDEBUGPRINTF_H
#define DXCCDEBUGPRINTF_H

//all basic windows sub-includes
#include "windows.h"


#if defined(_UNICODE) | defined(UNICODE)
	#define DXCC_DPF_REPORT		    DXCC_DPFW_REPORT
	#define DXCC_DPF_ERROR		    DXCC_DPFW_ERROR
	#define DXCC_DPF			    DXCC_DPFW
	#define DXCC_DPF_HEADER			DXCC_DPFW_HEADER
	#define DXCC_DPF_BODY           DXCC_DPFW_BODY        
#else
	#define DXCC_DPF_REPORT		    DXCC_DPFA_REPORT
	#define DXCC_DPF_ERROR		    DXCC_DPFA_ERROR
	#define DXCC_DPF			    DXCC_DPFA
	#define DXCC_DPF_HEADER			DXCC_DPFA_HEADER
	#define DXCC_DPF_BODY           DXCC_DPFA_BODY        
#endif

#define DXCC_DPF_STATUS( _status_ )			DXCC_DPFA_HEADER("HRESULT"), DXCCDebugPrintfHRESULT(_status_)
#define DXCC_DPFA_REPORT					DXCC_DPFA_HEADER("INFO"), DXCC_DPFA_BODY
#define DXCC_DPFW_REPORT					DXCC_DPFA_HEADER("INFO"), DXCC_DPFW_BODY
#define DXCC_DPFA_ERROR						DXCC_DPFA_HEADER("ERROR"), DXCC_DPFA_BODY
#define DXCC_DPFW_ERROR						DXCC_DPFA_HEADER("ERROR"), DXCC_DPFW_BODY
#define DXCC_DPFA_HEADER(_type_)			DXCCDebugPrintfHeaderA( _type_,  __FILE__  , __LINE__,  __FUNCTION__  )
#define DXCC_DPFW_HEADER(_type_)			DXCCDebugPrintfHeaderW( _type_,  __FILE__  , __LINE__,  __FUNCTION__  )
#define DXCC_DPFA_BODY                      DXCCDebugPrintfMessageA        
#define DXCC_DPFW_BODY                      DXCCDebugPrintfMessageW        
#define DXCC_DPFA							DXCCDebugPrintfA
#define DXCC_DPFW							DXCCDebugPrintfW


//#ifdef defined(DEBUG) | defined(_DEBUG)
//#else 
//	#define DXCC_DPF_STATUS(_status_)			
//	#define DXCC_DPFA_REPORT						
//	#define DXCC_DPFW_REPORT						
//	#define DXCC_DPFA_ERROR						
//	#define DXCC_DPFW_ERROR						
//	#define DXCC_DPFA_HEADER(_type_)	
//	#define DXCC_DPFW_HEADER(_type_)	
//	#define DXCC_DPFA_BODY                          
//	#define DXCC_DPFW_BODY                          
//	#define DXCC_DPFA	
//	#define DXCC_DPFW	
//#endif

#ifdef __cplusplus
extern "C" {
#endif //__cplusplus


typedef VOID (CALLBACK* DXCCDebugPrintfACallback)(LPCSTR szFormat);
typedef VOID (CALLBACK* DXCCDebugPrintfWCallback)(LPCWSTR szFormat);


VOID cdecl DXCCSetDebugPrintfACallback(DXCCDebugPrintfACallback callback);
VOID CALLBACK DXCCDebugPrintfADefaultCallback(LPCSTR szFormat);
VOID cdecl DXCCSetDebugPrintfWCallback(DXCCDebugPrintfWCallback callback);
VOID CALLBACK DXCCDebugPrintfWDefaultCallback(LPCWSTR szFormat);

VOID cdecl DXCCDebugPrintfHRESULT(HRESULT hr);
VOID cdecl DXCCDebugPrintfHeaderA(LPCSTR strReportType, LPCSTR strFile, UINT nLine, LPCSTR strFunction);
VOID cdecl DXCCDebugPrintfHeaderW(LPCWSTR strReportType, LPCSTR strFile, UINT nLine, LPCSTR strFunction);
VOID cdecl DXCCDebugPrintfMessageA(LPCSTR szFormat, ...);
VOID cdecl DXCCDebugPrintfMessageW(LPCWSTR szFormat, ...);
VOID cdecl DXCCDebugPrintfA(LPCSTR szFormat, ...);
VOID cdecl DXCCDebugPrintfW(LPCWSTR szFormat, ...);

#ifdef __cplusplus
}
#endif //__cplusplus

#endif
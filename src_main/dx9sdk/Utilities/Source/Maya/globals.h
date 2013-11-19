#pragma once
#ifndef GLOBALS_H
#define GLOBALS_H

//#if defined(_DEBUG) | defined(DEBUG)
	#define DXMFAIL( _stat )					(MFAIL( _stat ) ? (DXCC_DPFA_ERROR( #_stat ), DXCCBreakPoint(), TRUE ) :  FALSE )
	#define DXMSUCCESS( _stat )					( MStatus::kSuccess == ( _stat ) )
	#define DXCHECK_MSTATUS( _stat )			do{ MStatus _maya_status = ( _stat ); if(MFAIL( _maya_status )){ MString errStr= _maya_status.errorString(); DXCC_DPFA_ERROR( "%s Caused by (" #_stat ")", errStr.asChar() ); DXCCBreakPoint();} }while(0)
/*
#else

	#define DXMFAIL( _stat )					MFAIL(_stat)
	#define DXMSUCCESS( _stat )					(!(MFAIL(_stat)))
	#define DXCHECK_MSTATUS(_stat )				_stat
#endif
*/

void AttributeMakePublic( MFnAttribute& attribute);
void AttributeMakePrivate( MFnAttribute& attribute);

bool VersionMatch(LPCSTR Type1, INT Ver1, LPCSTR Type2, INT Ver2, char* strError);

MString MakeNameExportable(MString &in);


void ATGTransformDirection( D3DXVECTOR3* pDestDirection, const D3DXVECTOR3* pSrcDirection );
void ATGTransformMatrix( D3DXMATRIX* pDestMatrix, const D3DXMATRIX* pSrcMatrix );
void ATGTransformPosition( D3DXVECTOR3* pDestPosition, const D3DXVECTOR3* pSrcPosition );

bool	DAGIsVisible(MDagPath& dagPath, MStatus* status);
bool	NodeIsVisible(MObject& node, MStatus* _status);

UINT	MayaGetFPS();

void ConvertLocalMatrix(D3DXMATRIX& ToD3D, const MMatrix& FromMaya);
void ConvertLocalMatrix(D3DXMATRIX& ToD3D, const MFloatMatrix& FromMaya);
void ConvertWorldMatrix(D3DXMATRIX& ToD3D, const MMatrix& FromMaya);
void ConvertWorldMatrix(D3DXMATRIX& ToD3D, const MFloatMatrix& FromMaya);




VOID CALLBACK MayaDebugPrintfACallback(LPCSTR szFormat);
VOID CALLBACK MayaDebugPrintfWCallback(LPCWSTR szFormat);


#endif
#ifndef DXCCFLOAT4_H
#define DXCCFLOAT4_H

//all basic windows sub-includes
//#include "windows.h"
//defines INITGUID and includes guiddef.h
#include "initguid.h"
#include "d3d9.h"
#include "d3dx9.h"

#include "DXCCglobals.h"
//Interchanger for Vertex Buffers and shader registers
//privides conversion fucntions to and from standard types.
//converts them into an array of up to 4 values
typedef struct EXPORTDEF DXCCFLOAT4
{
public:
	FLOAT		value[4];
	WORD		flags;
	BYTE		swizzle;
	BYTE		count;

	//encode value[4] from the function param
	DXCCFLOAT4* Encode( signed char* pValue,		 UINT nCount);	
	DXCCFLOAT4* Encode( unsigned char* pValue,		 UINT nCount);	
	DXCCFLOAT4* Encode( signed short* pValue,		 UINT nCount);	
	DXCCFLOAT4* Encode( unsigned short* pValue,		 UINT nCount);	
	DXCCFLOAT4* Encode( signed int* pValue,			 UINT nCount);		
	DXCCFLOAT4* Encode( unsigned int* pValue,		 UINT nCount);	
	DXCCFLOAT4* Encode( signed long* pValue,		 UINT nCount);	
	DXCCFLOAT4* Encode( unsigned long* pValue,		 UINT nCount);	
	DXCCFLOAT4* Encode( float* pValue,				 UINT nCount);			
	DXCCFLOAT4* Encode( double* pValue,				 UINT nCount);			
	DXCCFLOAT4* Encode( D3DXFLOAT16* pValue,		 UINT nCount);	
	DXCCFLOAT4* Encode( D3DCOLOR* pValue);						
	DXCCFLOAT4* Encode( D3DXCOLOR* pValue);					
	DXCCFLOAT4* Encode( D3DCOLORVALUE* pValue);				
	DXCCFLOAT4* Encode( D3DVECTOR* pValue);					
	DXCCFLOAT4* Encode( D3DXVECTOR2* pValue);					
	DXCCFLOAT4* Encode( D3DXVECTOR3* pValue);					
	DXCCFLOAT4* Encode( D3DXVECTOR4* pValue);					
	DXCCFLOAT4* Encode( D3DXPLANE* pValue);					
	DXCCFLOAT4* Encode( D3DXVECTOR2_16F* pValue);				
	DXCCFLOAT4* Encode( D3DXVECTOR3_16F* pValue);				
	DXCCFLOAT4* Encode( D3DXVECTOR4_16F* pValue);				

	//decode value[4] into the function param
	DXCCFLOAT4* Decode(  signed char* pValue,		 UINT nCount);	
	DXCCFLOAT4* Decode(  unsigned char* pValue,	 UINT nCount);	
	DXCCFLOAT4* Decode(  signed short* pValue,		 UINT nCount);	
	DXCCFLOAT4* Decode(  unsigned short* pValue,	 UINT nCount);	
	DXCCFLOAT4* Decode(  signed int* pValue,		 UINT nCount);		
	DXCCFLOAT4* Decode(  unsigned int* pValue,		 UINT nCount);	
	DXCCFLOAT4* Decode(  signed long* pValue,		 UINT nCount);	
	DXCCFLOAT4* Decode(  unsigned long* pValue,	 UINT nCount);	
	DXCCFLOAT4* Decode(  float* pValue,			 UINT nCount);			
	DXCCFLOAT4* Decode(  double* pValue,			 UINT nCount);			
	DXCCFLOAT4* Decode(  D3DXFLOAT16* pValue,		 UINT nCount);	
	DXCCFLOAT4* Decode(  D3DCOLOR* pValue);					
	DXCCFLOAT4* Decode(  D3DXCOLOR* _pValue);					
	DXCCFLOAT4* Decode(  D3DCOLORVALUE* pValue);				
	DXCCFLOAT4* Decode(  D3DVECTOR*_pValue);					
	DXCCFLOAT4* Decode(  D3DXVECTOR2* pValue);					
	DXCCFLOAT4* Decode(  D3DXVECTOR3* pValue);					
	DXCCFLOAT4* Decode(  D3DXVECTOR4* pValue);					
	DXCCFLOAT4* Decode(  D3DXPLANE* pValue);					
	DXCCFLOAT4* Decode(  D3DXVECTOR2_16F* pValue);				
	DXCCFLOAT4* Decode(  D3DXVECTOR3_16F* pValue);				
	DXCCFLOAT4* Decode(  D3DXVECTOR4_16F* pValue);

} *LPDXCCFLOAT4;



#endif
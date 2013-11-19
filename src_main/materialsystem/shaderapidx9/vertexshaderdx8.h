//===== Copyright © 1996-2005, Valve Corporation, All rights reserved. ======//
//
// Purpose: 
//
// $NoKeywords: $
//
//===========================================================================//

#ifndef VERTEXSHADERDX8_H
#define VERTEXSHADERDX8_H

#ifdef _WIN32
#pragma once
#endif

#include "shaderapi/ishaderapi.h"
#include "locald3dtypes.h"

enum VertexShaderLightTypes_t
{
	LIGHT_NONE = -1,
	LIGHT_SPOT = 0,
	LIGHT_POINT = 1,
	LIGHT_DIRECTIONAL = 2,
	LIGHT_STATIC = 3,
	LIGHT_AMBIENTCUBE = 4,
};

//-----------------------------------------------------------------------------
// Vertex + pixel shader manager
//-----------------------------------------------------------------------------
abstract_class IShaderManager
{
protected:

	// The current vertex and pixel shader index
	int m_nVertexShaderIndex;
	int m_nPixelShaderIndex;

public:
	// Initialize, shutdown
	virtual void Init() = 0;
	virtual void Shutdown() = 0;

	// Compiles vertex shaders
	virtual IShaderBuffer *CompileShader( const char *pProgram, size_t nBufLen, const char *pShaderVersion ) = 0;

	// New version of these methods	[dx10 port]
	virtual VertexShaderHandle_t CreateVertexShader( IShaderBuffer* pShaderBuffer ) = 0;
	virtual void DestroyVertexShader( VertexShaderHandle_t hShader ) = 0;
	virtual PixelShaderHandle_t CreatePixelShader( IShaderBuffer* pShaderBuffer ) = 0;
	virtual void DestroyPixelShader( PixelShaderHandle_t hShader ) = 0;

	// Creates vertex, pixel shaders
	virtual VertexShader_t CreateVertexShader( const char *pVertexShaderFile, int nStaticVshIndex = 0 ) = 0;
	virtual PixelShader_t CreatePixelShader( const char *pPixelShaderFile, int nStaticPshIndex = 0 ) = 0;

	// Sets which dynamic version of the vertex + pixel shader to use
	FORCEINLINE void SetVertexShaderIndex( int vshIndex );
	FORCEINLINE void SetPixelShaderIndex( int pshIndex );

	// Sets the vertex + pixel shader render state
	virtual void SetVertexShader( VertexShader_t shader ) = 0;
	virtual void SetPixelShader( PixelShader_t shader ) = 0;

	// Resets the vertex + pixel shader state
	virtual void ResetShaderState() = 0;

	// Returns the current vertex + pixel shaders
	virtual void *GetCurrentVertexShader() = 0;
	virtual void *GetCurrentPixelShader() = 0;

	virtual void ClearVertexAndPixelShaderRefCounts() = 0;
	virtual void PurgeUnusedVertexAndPixelShaders() = 0;

	// The low-level dx call to set the vertex shader state
	virtual void BindVertexShader( VertexShaderHandle_t shader ) = 0;
	virtual void BindPixelShader( PixelShaderHandle_t shader ) = 0;

#if defined( _X360 )
	virtual const char *GetActiveVertexShaderName() = 0;
	virtual const char *GetActivePixelShaderName() = 0;
#endif
};

//-----------------------------------------------------------------------------
//
// Methods related to setting vertex + pixel shader state
//
//-----------------------------------------------------------------------------
FORCEINLINE void IShaderManager::SetVertexShaderIndex( int vshIndex )
{
	m_nVertexShaderIndex = vshIndex;
}

FORCEINLINE void IShaderManager::SetPixelShaderIndex( int pshIndex )
{
	m_nPixelShaderIndex = pshIndex;
}

#endif // VERTEXSHADERDX8_H

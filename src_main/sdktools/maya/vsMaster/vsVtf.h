//======= Copyright © 1996-2006, Valve Corporation, All rights reserved. ======
//
// Purpose:
//
//=============================================================================

#ifndef VSTTEXTURE_H
#define VSTTEXTURE_H
#if defined( _WIN32 )
#pragma once
#endif


// Maya
#include <maya/MImage.h>
#include <maya/MFloatVector.h>


// OpenGL
#include <GL/gl.h>
#include <GL/glu.h>


// Valve includes
#include <vtf/vtf.h>


//=============================================================================
//
//=============================================================================
class CGLTexObj
{
public:
	CGLTexObj( GLenum target = GL_TEXTURE_2D )
	: m_bGenTextureAllocated( false )
	{
		assert( glGetError() == GL_NO_ERROR );

		m_fTarget = target;

		// Set up default values for the texture parameters.
		// They are the same as the OpenGL default.
		m_fMinFilterParam = GL_NEAREST;
		m_fMagFilterParam = GL_LINEAR;
		m_fWrapSParam = GL_REPEAT;
		m_fWrapTParam = GL_REPEAT;
	}

	~CGLTexObj()
	{
		if ( m_bGenTextureAllocated )
		{
			glDeleteTextures( 1, &m_fTextureNum );
		}
	}

	void bind()
	{
		if ( !m_bGenTextureAllocated )
		{
			// Get a texture identifier.
			glGenTextures( 1, &m_fTextureNum );
			Assert( glGetError() == GL_NO_ERROR );
			m_bGenTextureAllocated = true;
		}

		assert(glGetError() == GL_NO_ERROR);

		// Bind the texture.
		glBindTexture( m_fTarget, m_fTextureNum );
		assert(glGetError() == GL_NO_ERROR);

		// Set up the texture parameters.
		glTexParameteri( m_fTarget, GL_TEXTURE_MIN_FILTER, m_fMinFilterParam );
		assert(glGetError() == GL_NO_ERROR);
		glTexParameteri(m_fTarget, GL_TEXTURE_MAG_FILTER, m_fMagFilterParam);
		assert(glGetError() == GL_NO_ERROR);
		glTexParameteri(m_fTarget, GL_TEXTURE_WRAP_S, m_fWrapSParam);
		assert(glGetError() == GL_NO_ERROR);
		glTexParameteri( m_fTarget, GL_TEXTURE_WRAP_T, m_fWrapTParam );
		assert(glGetError() == GL_NO_ERROR);
	}

	void parameter( GLenum pname, GLint param )
	{
		switch (pname)
		{
		case GL_TEXTURE_MIN_FILTER:
			m_fMinFilterParam = param;
			break;
		case GL_TEXTURE_MAG_FILTER:
			m_fMagFilterParam = param;
			break;
		case GL_TEXTURE_WRAP_S:
			m_fWrapSParam = param;
			break;
		case GL_TEXTURE_WRAP_T:
			m_fWrapTParam = param;
			break;
		}
	}

private:
	GLenum m_fTarget;

	GLint m_fMinFilterParam;
	GLint m_fMagFilterParam;
	GLint m_fWrapSParam;
	GLint m_fWrapTParam;

	GLuint m_fTextureNum;
	bool m_bGenTextureAllocated;
};


//=============================================================================
//
//=============================================================================
class CVsVtf
{
public:
	CVsVtf();

	~CVsVtf();

	void Clear();

	MStatus Load( const char *pFilename );

	void Bind();

	void Specify( bool bAlpha );

	MStatus CopyToImage( MImage &mImage );

	inline int Width() const { return m_pVtf ? m_pVtf->Width() : 0; }

	inline int Height() const { return m_pVtf ? m_pVtf->Height() : 0; }

	inline bool HasAlpha() const { return m_bHasAlpha; }

	void GetColor( const float2 &uv, MFloatVector &c );

protected:
	static bool HasAlpha( IVTFTexture *pVtf );

	IVTFTexture *m_pVtf;	// Valve VTF Texture
	CGLTexObj m_glTexObj;	// OpenGL Texture Object Manager
	bool m_bHasAlpha;		// Sadly, in memory it will always be RGBA, flag if the original image had an alpha channel
};

#endif // VSTTEXTURE_H
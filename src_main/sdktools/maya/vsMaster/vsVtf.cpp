//======= Copyright © 1996-2007, Valve Corporation, All rights reserved. ======
//
//=============================================================================


// Valve includes
#include "filesystem.h"
#include "valveMaya.h"
#include "tier1/utlbuffer.h"


// Local includes
#include "vsVtf.h"


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
CVsVtf::CVsVtf()
: m_pVtf( NULL )
, m_bHasAlpha( false )
{
	Clear();
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
CVsVtf::~CVsVtf()
{
	Clear();
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void CVsVtf::Clear()
{
	if ( m_pVtf )
	{
		DestroyVTFTexture( m_pVtf );
		m_pVtf = NULL;
	}

	m_bHasAlpha = false;
}


//-----------------------------------------------------------------------------
// Don't know a better way to do this
//-----------------------------------------------------------------------------
bool CVsVtf::HasAlpha( IVTFTexture *pVtf )
{
	return ( pVtf->Flags() & ( TEXTUREFLAGS_ONEBITALPHA | TEXTUREFLAGS_EIGHTBITALPHA ) ) != 0;
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
MStatus CVsVtf::Load( const char *pFileName )
{
	if ( !pFileName )
	{
		merr << "CVsVtf: No filename specified" << std::endl;
		return MS::kFailure;
	}

	CUtlBuffer buf;
	if ( !g_pFullFileSystem->ReadFile( pFileName, NULL, buf ) )
	{
		merr << "CVsVtf: Couldn't read \"" << pFileName << "\"" << std::endl;
		return MS::kFailure;
	}

	{
		IVTFTexture *pVtf = CreateVTFTexture();

		if ( !pVtf )
		{
			merr << "CVsVtf: Couldn't create VTF texture for \"" << pFileName << "\"" << std::endl;
			return MS::kFailure;
		}

		Clear();
		m_pVtf = pVtf;
	}

	// Load
	m_pVtf->Unserialize( buf );

	if ( HasAlpha( m_pVtf ) )
	{
		m_bHasAlpha = true;
	}
	else
	{
		m_bHasAlpha = false;
	}

	m_pVtf->ConvertImageFormat( IMAGE_FORMAT_RGBA8888, false );

	// TODO: Handle this in the texture transform
	ImageLoader::FlipImageVertically( m_pVtf->ImageData(), m_pVtf->ImageData(), Width(), Height(), m_pVtf->Format() );

	return MS::kSuccess;
}


//-----------------------------------------------------------------------------
// Purpose: Upload texture to OpenGL
//-----------------------------------------------------------------------------
void CVsVtf::Specify( bool bAlpha )
{
	if ( Width() <= 0 || Height() <= 0 )
		return;

	m_glTexObj.bind();

	if ( bAlpha && HasAlpha() )
	{
		glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA8, Width(), Height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, m_pVtf->ImageData() );
	}
	else
	{
		glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB8, Width(), Height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, m_pVtf->ImageData() );
	}

	m_glTexObj.parameter( GL_TEXTURE_MIN_FILTER, GL_LINEAR );
	m_glTexObj.parameter( GL_TEXTURE_MAG_FILTER, GL_LINEAR );

	if ( m_pVtf->Flags() & TEXTUREFLAGS_CLAMPS )
	{
		m_glTexObj.parameter( GL_TEXTURE_WRAP_S, GL_CLAMP );
	}
	else
	{
		m_glTexObj.parameter( GL_TEXTURE_WRAP_S, GL_REPEAT );
	}

	if ( m_pVtf->Flags() & TEXTUREFLAGS_CLAMPT )
	{
		m_glTexObj.parameter( GL_TEXTURE_WRAP_T, GL_CLAMP );
	}
	else
	{
		m_glTexObj.parameter( GL_TEXTURE_WRAP_T, GL_REPEAT );
	}
}


//-----------------------------------------------------------------------------
// Purpose: Bind in OpenGL
//-----------------------------------------------------------------------------
void CVsVtf::Bind()
{
	m_glTexObj.bind();
}


//-----------------------------------------------------------------------------
// Purpose: Copies the data to a Maya MImage
//-----------------------------------------------------------------------------
MStatus CVsVtf::CopyToImage( MImage &mImage )
{
	if ( Width() <= 0 || Height() <= 0 )
		return MS::kFailure;

	MImage tmpImage;

	// Copy the image data into a Maya MImage
	tmpImage.create( Width(), Height() );
	tmpImage.setPixels( m_pVtf->ImageData(), Width(), Height() );

	// Resize the MImage to match the requested size
	unsigned int nNewWidth = Width();
	unsigned int nNewHeight = Height();
	mImage.getSize( nNewWidth, nNewHeight );
	tmpImage.resize( nNewWidth, nNewHeight, false );

	// Copy the image data to the requested image
	mImage.setPixels( tmpImage.pixels(), nNewWidth, nNewHeight );

	// Set the alpha channel to all white
	const unsigned char *pEndAlpha = mImage.pixels() + 3 + nNewWidth * nNewHeight * 4;
	for ( unsigned char *pAlpha = mImage.pixels() + 3; pAlpha < pEndAlpha; pAlpha += 4 )
	{
		*pAlpha = 255U;
	}

	return MS::kSuccess;
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void CVsVtf::GetColor( const float2 &uv, MFloatVector &c )
{
	if ( Width() <= 0 || Height() <= 0 || !m_pVtf )
		return;

	const int nIndex = static_cast< int >( uv[ 0 ] - floorf( uv[ 0 ] ) ) + static_cast< int >( uv[ 1 ] - floorf( uv[ 1 ] ) ) * Width();
	if ( nIndex < Width() * Height() )
	{
		const unsigned char *pImagePixel = m_pVtf->ImageData() + 4 * nIndex;
		c.x = *pImagePixel;
		c.y = *( pImagePixel + 1 );
		c.z = *( pImagePixel + 2 );
	}
}

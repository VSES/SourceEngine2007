//===== Copyright © 1996-2005, Valve Corporation, All rights reserved. ======//
//
// Purpose: 
//
// $Header: $
// $NoKeywords: $
//===========================================================================//

#ifndef BITMAP_H
#define BITMAP_H

#ifdef _WIN32
#pragma once
#endif


#include "bitmap/imageformat.h"


//-----------------------------------------------------------------------------
// A Bitmap
//-----------------------------------------------------------------------------
struct Bitmap_t
{
	Bitmap_t();
	~Bitmap_t();
	void Init( int nWidth, int nHeight, ImageFormat imageFormat );
	unsigned char *GetPixel( int x, int y );

	int m_nWidth;
	int m_nHeight;
	ImageFormat m_ImageFormat;
	unsigned char *m_pBits;
};

inline Bitmap_t::Bitmap_t()
{
	m_nWidth = 0;
	m_nHeight = 0;
	m_ImageFormat = IMAGE_FORMAT_UNKNOWN;
	m_pBits = NULL;
}

inline Bitmap_t::~Bitmap_t()
{
	if ( m_pBits )
	{
		delete[] m_pBits;
		m_pBits = NULL;
	}
}

inline void Bitmap_t::Init( int nWidth, int nHeight, ImageFormat imageFormat )
{
	if ( m_pBits )
	{
		delete[] m_pBits;
		m_pBits = NULL;
	}

	m_nWidth = nWidth;
	m_nHeight = nHeight;
	m_ImageFormat = imageFormat;
	m_pBits = new unsigned char[ nWidth * nHeight * ImageLoader::SizeInBytes( m_ImageFormat ) ];
}

inline unsigned char *Bitmap_t::GetPixel( int x, int y )
{
	if ( !m_pBits )
		return NULL;

	int nPixelSize = ImageLoader::SizeInBytes( m_ImageFormat );
	return &m_pBits[ ( m_nWidth * y + x ) * nPixelSize ];
}


#endif // BITMAP_H

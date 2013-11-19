//========= Copyright © 1996-2005, Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
// $NoKeywords: $
//=============================================================================//

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <vgui/ISurface.h>
#include "bitmap.h"
#include "vgui_internal.h"
#include "filesystem.h"
#include "UtlBuffer.h"
#include <tier0/dbg.h>

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

using namespace vgui;

//-----------------------------------------------------------------------------
// Purpose: Constructor
// Input  : *filename - image file to load 
//-----------------------------------------------------------------------------
Bitmap::Bitmap(const char *filename, bool hardwareFiltered)
{
	_filtered = hardwareFiltered;

	int size = strlen(filename) + 1;
	_filename = (char *)malloc( size );
	Assert( _filename );

	Q_snprintf( _filename, size, "%s", filename );

	_bProcedural = false;

	if ( Q_stristr( filename, ".pic" ) )
	{
		_bProcedural = true;
	}

	_id = 0;
	_uploaded = false;
	_color = Color(255, 255, 255, 255);
	_pos[0] = _pos[1] = 0;
	_valid = true;
	_wide = 0;
	_tall = 0;
	ForceUpload();
}

//-----------------------------------------------------------------------------
// Purpose: Destructor
//-----------------------------------------------------------------------------
Bitmap::~Bitmap()
{
	if ( _filename )
	{
		free( _filename );
	}
}

//-----------------------------------------------------------------------------
// Purpose: data accessor
//-----------------------------------------------------------------------------
void Bitmap::GetSize(int &wide, int &tall)
{
	wide = 0;
	tall = 0;

	if (!_valid)
		return;

	// if a size has not been set, get it from the texture
	if ( 0 == _wide && 0 ==_tall )
	{
		g_pSurface->DrawGetTextureSize(_id, _wide, _tall);

	}
	wide = _wide;
	tall = _tall;
}

//-----------------------------------------------------------------------------
// Purpose: size of the bitmap
//-----------------------------------------------------------------------------
void Bitmap::GetContentSize(int &wide, int &tall)
{
	GetSize(wide, tall);
}

//-----------------------------------------------------------------------------
// Purpose: ignored
//-----------------------------------------------------------------------------
void Bitmap::SetSize(int x, int y)
{
//	AssertMsg( _filtered, "Bitmap::SetSize called on non-hardware filtered texture.  Bitmap can't be scaled; you don't want to be calling this." );
	_wide = x;
	_tall = y;
}

//-----------------------------------------------------------------------------
// Purpose: data accessor
//-----------------------------------------------------------------------------
void Bitmap::SetPos(int x, int y)
{
	_pos[0] = x;
	_pos[1] = y;
}

//-----------------------------------------------------------------------------
// Purpose: data accessor
//-----------------------------------------------------------------------------
void Bitmap::SetColor(Color col)
{
	_color = col;
}

//-----------------------------------------------------------------------------
// Purpose: returns the file name of the bitmap
//-----------------------------------------------------------------------------
const char *Bitmap::GetName()
{
	return _filename;
}

//-----------------------------------------------------------------------------
// Purpose: Renders the loaded image, uploading it if necessary
//			Assumes a valid image is always returned from uploading
//-----------------------------------------------------------------------------
void Bitmap::Paint()
{
	if ( !_valid )
		return;

	// if we don't have an _id then lets make one
	if ( !_id )
	{
		_id = g_pSurface->CreateNewTextureID();
	}
	
	// if we have not uploaded yet, lets go ahead and do so
	if ( !_uploaded )
	{
		ForceUpload();
	}
	
	// set the texture current, set the color, and draw the biatch
	g_pSurface->DrawSetColor( _color[0], _color[1], _color[2], _color[3] );
	g_pSurface->DrawSetTexture( _id );

	if ( _wide == 0 )
	{
		GetSize( _wide, _tall);
	}
	g_pSurface->DrawTexturedRect(_pos[0], _pos[1], _pos[0] + _wide, _pos[1] + _tall);
}

//-----------------------------------------------------------------------------
// Purpose: ensures the bitmap has been uploaded
//-----------------------------------------------------------------------------
void Bitmap::ForceUpload()
{
	if ( !_valid || _uploaded )
		return;

	if ( !_id )
	{
		_id = g_pSurface->CreateNewTextureID( _bProcedural );
	}

	if ( !_bProcedural )
	{
		g_pSurface->DrawSetTextureFile( _id, _filename, _filtered, false );
	}

	_uploaded = true;

	_valid = g_pSurface->IsTextureIDValid( _id );
}

//-----------------------------------------------------------------------------
// Purpose: data accessor
//-----------------------------------------------------------------------------
HTexture Bitmap::GetID()
{
	return _id;
}








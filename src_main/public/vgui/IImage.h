//========= Copyright © 1996-2005, Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
// $NoKeywords: $
//=============================================================================//

#ifndef IIMAGE_H
#define IIMAGE_H

#ifdef _WIN32
#pragma once
#endif

#include <vgui/VGUI.h>

class Color;

namespace vgui
{

//-----------------------------------------------------------------------------
// Purpose: Interface to drawing an image
//-----------------------------------------------------------------------------
class IImage
{
public:
	// Call to Paint the image
	// Image will draw within the current panel context at the specified position
	virtual void Paint() = 0;

	// Set the position of the image
	virtual void SetPos(int x, int y) = 0;

	// Gets the size of the content
	virtual void GetContentSize(int &wide, int &tall) = 0;

	// Get the size the image will actually draw in (usually defaults to the content size)
	virtual void GetSize(int &wide, int &tall) = 0;

	// Sets the size of the image
	virtual void SetSize(int wide, int tall) = 0;

	// Set the draw color 
	virtual void SetColor(Color col) = 0;

	// virtual destructor
	virtual ~IImage() {}
};

} // namespace vgui


#endif // IIMAGE_H

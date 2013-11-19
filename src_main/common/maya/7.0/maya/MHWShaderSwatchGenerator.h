#ifndef _MHWShaderSwatchGenerator
#define _MHWShaderSwatchGenerator

//
//-
// ==========================================================================
// Copyright (C) Alias Systems Corp., and/or its licensors ("Alias").  
// All rights reserved.  These coded instructions, statements, computer  
// programs, and/or related material (collectively, the "Material")
// contain unpublished information proprietary to Alias, which is
// protected by Canadian and US federal copyright law and by international
// treaties. This Material may not be disclosed to third parties, or be copied
// or duplicated, in whole or in part, without the prior written consent of
// Alias.  ALIAS HEREBY DISCLAIMS ALL WARRANTIES RELATING TO THE MATERIAL,
// INCLUDING, WITHOUT LIMITATION, ANY AND ALL EXPRESS OR IMPLIED WARRANTIES OF
// NON-INFRINGEMENT, MERCHANTABILITY AND/OR FITNESS FOR A PARTICULAR PURPOSE.
// IN NO EVENT SHALL ALIAS BE LIABLE FOR ANY DAMAGES WHATSOEVER, WHETHER DIRECT,
// INDIRECT, SPECIAL, OR PUNITIVE, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE
// OR OTHER TORTIOUS ACTION, OR IN EQUITY, ARISING OUT OF OR RELATED TO THE
// ACCESS TO, USE OF, OR RELIANCE UPON THE MATERIAL.
// ==========================================================================
//+
//
// CLASS:    MHWShaderSwatchGenerator
//
// *****************************************************************************
//
// CLASS DESCRIPTION (MHWShaderSwatchGenerator)
//
// This MHWShaderSwatchGenerator is derived from MSwatchRenderBase
// which provides an interface for a plugin shader to provide a custom rendered
// image which can be used as a swatch which is displayed in hypershade and
// the Attribute Editor.
//
// *****************************************************************************

#if defined __cplusplus

#ifdef _WIN32
#pragma warning(disable: 4522)
#endif // _WIN32

// INCLUDED HEADER FILES


#include <maya/MSwatchRenderBase.h>
#include <maya/MSwatchRenderRegister.h>
#include <maya/MSwatchRenderBase.h>

// CLASS DECLARATION (MHWShaderSwatchGenerator)

/// Hardware shader swatch generator utility class
/**

Hardware shader swatch generator utility class

*/

class OPENMAYAUI_EXPORT MHWShaderSwatchGenerator : public MSwatchRenderBase
{
public:
	///
	MHWShaderSwatchGenerator( MObject obj, MObject renderObj, int res ) : 
		MSwatchRenderBase( obj, renderObj, res )
	{
	}

	///
	static MSwatchRenderBase* createObj(MObject obj, MObject renderObj, int res);

	///
	static const MString& initialize();

	///
	static void getSwatchBackgroundColor(float& r, float& g, float& b, float& a);

	///
	virtual bool doIteration( void );

protected:
// No protected members

private:
// No private members

};


#ifdef _WIN32
#pragma warning(default: 4522)
#endif // _WIN32

// *****************************************************************************

#endif /* __cplusplus */
#endif /* _MHWShaderSwatchGenerator */



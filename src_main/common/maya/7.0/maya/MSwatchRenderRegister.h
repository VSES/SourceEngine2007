
#ifndef _MSwatchRenderRegister
#define _MSwatchRenderRegister

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
// CLASS:    MSwatchRenderRegister
//
// ****************************************************************************
//
// CLASS DESCRIPTION (MSwatchRenderRegister)
//
//
// ****************************************************************************

#if defined __cplusplus

// ****************************************************************************

// INCLUDED HEADER FILES


#include <maya/MStatus.h>
#include <maya/MTypes.h>



// ****************************************************************************

// DECLARATIONS

class MObject;
class MString ;
class MSwatchRenderBase ;

///
typedef MSwatchRenderBase* (*MSwatchRenderCreatorFnPtr) (MObject dependNode, MObject renderNode, int imageResolution);

/// Manages swatch generators 
/**
Provides an interface for plugins to register/unregister swatch
generator classes (derived from MSwatchRenderBase) with Maya.
Whenever a swatch needs to be generated for a node, it checks the
classfication string of the node for the preferred swatch generator.
If a match is found, it creates and manages the swatch generator
object (that is it deletes the swatch generator object once the image
has been generated). The doIteration function is called for generating
the swatch.  The doIteration function is called repeatedly (during
idle events) till it returns true. This allows for generation of the
swatch in stages.
*/
class OPENMAYARENDER_EXPORT MSwatchRenderRegister {
public:
	/// registers a new swatch generator creation function by name. 
	static MStatus	registerSwatchRender(MString swatchGenName, MSwatchRenderCreatorFnPtr fnPtr);

	/// removes the previously registered swatch generator
	static MStatus	unregisterSwatchRender(MString swatchGenName);

protected:
// No protected members

private:
// No private members
};


// ****************************************************************************
#endif /* __cplusplus */
#endif /* _MSwatchRenderRegister */


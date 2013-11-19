
#ifndef _MPxHwShaderNode
#define _MPxHwShaderNode
//
//-
// ==========================================================================
// Copyright (C) 1995 - 2005 Alias Systems Corp. and/or its licensors.  All 
// rights reserved.
// 
// The coded instructions, statements, computer programs, and/or related 
// material (collectively the "Data") in these files contain unpublished 
// information proprietary to Alias Systems Corp. ("Alias") and/or its 
// licensors, which is protected by Canadian and US federal copyright law and 
// by international treaties.
// 
// The Data may not be disclosed or distributed to third parties or be copied 
// or duplicated, in whole or in part, without the prior written consent of 
// Alias.
// 
// THE DATA IS PROVIDED "AS IS". ALIAS HEREBY DISCLAIMS ALL WARRANTIES RELATING 
// TO THE DATA, INCLUDING, WITHOUT LIMITATION, ANY AND ALL EXPRESS OR IMPLIED 
// WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY AND/OR FITNESS FOR A 
// PARTICULAR PURPOSE. IN NO EVENT SHALL ALIAS BE LIABLE FOR ANY DAMAGES 
// WHATSOEVER, WHETHER DIRECT, INDIRECT, SPECIAL, OR PUNITIVE, WHETHER IN AN 
// ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, OR IN EQUITY, 
// ARISING OUT OF ACCESS TO, USE OF, OR RELIANCE UPON THE DATA.
// ==========================================================================
//+
//
// CLASS:    MPxHwShaderNode
//
// *****************************************************************************
//
// CLASS DESCRIPTION (MPxHwShaderNode)
//
// MPxHwShaderNode allows the creation of user-defined hwShaders.  A hwShader
// is a node which takes any number of input geometries, deforms them and
// places the output into the output geometry attribute.
//
// For more information, please see the built-in documentation
// in MPxHwShaderNode.cpp.
//
// *****************************************************************************

#if defined __cplusplus

// *****************************************************************************

// INCLUDED HEADER FILES



#include <maya/MStatus.h>
#include <maya/MTypes.h>
#include <maya/MObject.h>
#include <maya/MPxNode.h>
#include <maya/MDrawRequest.h>
#include <maya/M3dView.h>
#include <maya/MDagPath.h>

// *****************************************************************************

// DECLARATIONS

// *****************************************************************************

// CLASS DECLARATION (MPxHwShaderNode)

/// Base class for user defined hardware shaders (OpenMayaUI)
/**
  Create user defined hardware shaders.
*/
#ifdef _WIN32
#pragma warning(disable: 4522)
#endif // _WIN32

class OPENMAYAUI_EXPORT MPxHwShaderNode : public MPxNode
{
public:
	///
	enum Writeable {
		///
		kWriteNone				= 0x0000,
		///
		kWriteVertexArray		= 0x0001,
		///
		kWriteNormalArray		= 0x0002,
		///
		kWriteColorArrays		= 0x0004,
		///
		kWriteTexCoordArrays	= 0x0008,
		///
		kWriteAll				= 0x000f
	};

	///
	enum DirtyMask {
		///
		kDirtyNone				= 0x0000,
		///
		kDirtyVertexArray		= 0x0001,
		///
		kDirtyNormalArray		= 0x0002,
		///
		kDirtyColorArrays		= 0x0004,
		///
		kDirtyTexCoordArrays	= 0x0008,
		///
		kDirtyAll				= 0x000f
	};

    ///
	MPxHwShaderNode();

    ///
	virtual ~MPxHwShaderNode();

	///
	virtual MPxNode::Type type() const;

	// Methods to overload


	// Override this method to set up the OpenGL state
	//
	///
	virtual MStatus		bind( const MDrawRequest& request,
							  M3dView& view );

	// Override this method to return OpenGL to a sane state
	//
	///
	virtual MStatus		unbind( const MDrawRequest& request,
								M3dView& view );

	// Override this method to actually draw primitives on the display
	//
	///
	virtual MStatus		geometry( const MDrawRequest& request,
								  M3dView& view,
								  int prim,
								  unsigned int writable,
								  int indexCount,
								  const unsigned int * indexArray,
								  int vertexCount,
								  const int * vertexIDs,
								  const float * vertexArray,
								  int normalCount,
								  const float ** normalArrays,
								  int colorCount,
								  const float ** colorArrays,
								  int texCoordCount,
								  const float ** texCoordArrays);

	// These methods should only be overridden if support
	// for the hardware renderer is required.
    // By default the interface above will invoke the methods below.
	//
    ///
	virtual MStatus     glBind  ( const MDagPath& shapePath );
    ///
	virtual MStatus	    glUnbind( const MDagPath& shapePath );
    ///
	virtual MStatus	    glGeometry( const MDagPath& shapePath,
                                  int glPrim,
								  unsigned int writeMask,
								  int indexCount,
								  const unsigned int* indexArray,
								  int vertexCount,
								  const int * vertexIDs,
								  const float * vertexArray,
								  int normalCount,
								  const float ** normalArrays,
								  int colorCount,
								  const float ** colorArrays,
								  int texCoordCount,
								  const float ** texCoordArrays);

	// Method which returns the path for the current object
	// being drawn using the shader. 
	//
	///
	const MDagPath & currentPath() const;

	// Method which returns which particular geometry items
	// have changed for the current object being drawn,
	// from the last time that geometry was
	// requested to draw (either via geometry() or glGeometry()).
	//
	// It is only guaranteed to contain valid information 
	// within either of geometry calls.
	//
	///
	unsigned int	dirtyMask() const;

	// Override this method to specify how many "normals" per vertex
	// the hardware shader would like.  Maya can provide from 0 to 3
	// normals per vertex.  The second and third "normal" will be
	// tangents.  If you do not override this method, Maya will
	// provide 1 normal per vertex.
	//
	///
	virtual	int		normalsPerVertex();

	// Override this method to specify how many colors per vertex the
	// hardware shader would like Maya to provide.  Maya may not provide
	// this many if they are not available.  If you do not override
	// this method, Maya will provide 0 colors per vertex.
	//
	///
	virtual int		colorsPerVertex();

	// Override this method to specify an array of names of color per
	// vertex sets that should be provided.  When Maya calls the 
	// geometry method, the color values from the nth name in the list 
	// will be passed in the nth colorArray.
	//
	///
	virtual int		getColorSetNames(MStringArray& names);

	// Override this method to specify how many texture coordinates
	// per vertex the hardware shader would like Maya to provide.
	// Maya may not provide this many if they are not available.  If
	// you do not override this method, Maya will provide 0 texture
	// coordinates per vertex.
	//
	///
	virtual int		texCoordsPerVertex();

	// Override this method to specify an array of names of uvSets
	// that should be provided.  When Maya calls the geometry method,
	// the uv values from the nth name in the list will be passed in
	// the nth texCoordArray.
	//
	///
	virtual int		getTexCoordSetNames(MStringArray& names);

	// Specifies whether or not the hw shader uses transparency.  If
	// so, the objects that use this shader must be drawn after all
	// the opaque objects.
	//
	///
	virtual bool	hasTransparency();

	// Specifies whether or not the hw shader wants a map of the
	// vertex IDs in the vertexArray provided to the geomery method.
	//
	///
	virtual bool	provideVertexIDs();

	// Override this method to specify the list of images
	// that are associated with the given uv set in this shader. 
	// This method is used to determine which texture images 
	// are available in the UV texture editor.
	//
	///
	virtual MStatus	getAvailableImages( const MString& uvSetName, 
								        MStringArray& imageNames);

	// Override this method to draw an image of this material.
	// This method allows a shader to override the rendering of
	// the background image used in the UV texture editor.
	// Maya will only use this method if getAvailableImages 
	// returns at least one image name. The imageWidth and 
	// imageHeight parameters should be populated with the 
	// native resolution of the input image to allow pixel 
	// snapping or other resolution dependent operations.
	//
	///
	virtual MStatus	renderImage( const MString& imageName, 
								 const float region[2][2],
								 int& imageWidth,
								 int& imageHeight);

	// Override this method to draw a image for swatch rendering.
	///
	virtual MStatus renderSwatchImage( MImage & image );
	///
	static  MPxHwShaderNode* getHwShaderNodePtr( MObject& object );


	// Attributes inherited from surfaceShader
	/// output color value
	static MObject outColor;
	/// output color red
	static MObject outColorR;
	/// output color green
	static MObject outColorG;
	/// output color blue
	static MObject outColorB;

	/// output transparency value
	static MObject outTransparency;
	/// output transparency red
	static MObject outTransparencyR;
	/// output transparency green
	static MObject outTransparencyG;
	/// output transparency blue
	static MObject outTransparencyB;

	/// output matte opacity value
	static MObject outMatteOpacity;
	/// output matte opacity red
	static MObject outMatteOpacityR;
	/// output matte opacity green
	static MObject outMatteOpacityG;
	/// output matte opacity blue
	static MObject outMatteOpacityB;

	/// output glow color value
	static MObject outGlowColor;
	/// output glow color red
	static MObject outGlowColorR;
	/// output glow color green
	static MObject outGlowColorG;
	/// output glow color blue
	static MObject outGlowColorB;


protected:
	// Current path.
	void				setCurrentPath( const void *pathPtr );
	MDagPath			fCurrentPath;
	
	// Dirty flags for geometry for current path.
	void				setDirtyMask( unsigned int mask );
	unsigned int		fDirtyMask;

private:
	static void				initialSetup();
	static const char*	    className();
};

#ifdef _WIN32
#pragma warning(default: 4522)
#endif // _WIN32

// *****************************************************************************
#endif /* __cplusplus */
#endif /* _MPxNode */

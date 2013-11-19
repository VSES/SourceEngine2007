#ifndef _M3dView
#define _M3dView
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
// CLASS:    M3dView
//
// ****************************************************************************
//
// CLASS DESCRIPTION (M3dView)
//
//  M3dView provides methods for working with 3D model views.  3D views are
//  based on OpenGL drawing areas.
//
//  Maya can operate in two different color modes, RGBA and color index.  Color
//  index mode is used to increase performance when shading is not required.
//  Drawing in color index mode is more complicated, but this class provides 
//  methods to simplify color selection.  
//
//  Maya has four color tables that can be used in RGBA, and that must be used
//  in color index mode.  These four color tables represent four sets of bit
//  planes that are independent of each other.  So, for example, it is possible 
//  to clear all active objects from the display and redraw them without 
//  redrawing the dormant and templated objects.  
//
//  The extra performance of color index mode comes at the cost of a
//  limited number of colors.  If this restriction causes difficulty, then
//  it is possible for the user to force all displays into RGBA mode 
//  where any color may be used.
//
//  When an object is affected by another in the scene, it is drawn in a
//  magenta colour by default.  This is denoted in the DisplayStatus enum by
//  kActiveAffected.  These objects are drawn in the active planes even though
//  they are dormant for performance reasons.
//
// ****************************************************************************

#if defined __cplusplus

// ****************************************************************************
// INCLUDED HEADER FILES


#if ( defined (OSMac_) && !defined (MAC_PLUGIN) )
#include "HOS.h"
#endif

#include <maya/MStatus.h>
#include <maya/MObject.h>

#if defined(OSMac_)
#	if defined(OSMac_MachO_)
#		include <AGL/agl.h>
#	else
#		include <agl.h>
#	endif
typedef WindowRef MWindow;

#elif defined (SGI) || defined (LINUX)

#include <GL/glx.h>
#include <X11/Intrinsic.h>
typedef  Widget MWindow;

#elif defined (_WIN32)

#include "windows.h"
#include <gl/Gl.h>
typedef  HWND MWindow;
#else
#error Unknown OS 
#endif //OSMac_

// *****************************************************************************

// DECLARATIONS

class MString;
class MDagPath;
class MPoint;
class MVector;
class MMatrix;
class MColor;
class MPxGlBuffer;
class MImage;

// *****************************************************************************

// CLASS DECLARATION (M3dView)

#if (defined (SGI) || defined (LINUX) || defined (OSMac_))
	#if (defined (SGI) || defined(LINUX))
		typedef Window M3dWindow;
	#elif defined(OSMac_)
		typedef WindowPtr		M3dWindow;
	#endif // defined (SGI)  || defined(LINUX)
#elif defined(_WIN32)
typedef HWND M3dWindow;
#else
#error Unknown OS 
#endif // OSUnix_

#if defined (_WIN32)
#pragma warning(disable: 4522)
#endif // _WIN32

/// A 3-D view (OpenMayaUI)
/**

3-D view class.

*/
class OPENMAYAUI_EXPORT M3dView {

public:

    ///
    enum DisplayStyle {
        /// bounding box display
        kBoundingBox,
        /// flat shaded display
        kFlatShaded,
        /// gouraud shaded display
        kGouraudShaded,
        /// wire frame display
        kWireFrame,
        /// points only display
        kPoints
    }; 


    ///
    enum DisplayStatus {
        /// object is active (selected)
        kActive,
        /// object is live (construction surface)
        kLive,
        /// object is domant
        kDormant,
        /// object is invisible (not drawn)
        kInvisible,
        /// object is hilited (has selectable components)
        kHilite,
        /// object is templated (Not renderable)
        kTemplate,
        /// object is active and templated
        kActiveTemplate, 
        /// object has active components
        kActiveComponent,
		/// Last selected object
		kLead,
		/// Construction object (not drawn)
		kIntermediateObject,
		/// Affected by active object(s)
		kActiveAffected,
        /// object does not have a valid display status
        kNoStatus
    }; 

    ///
    enum ColorTable {
        /// colors for active objects
        kActiveColors = kActive, 
        /// colors for dormant objects
        kDormantColors = kDormant, 
        /// colors for templated objects
        kTemplateColor = kTemplate,
        /// colors for background color
        kBackgroundColor
    };  

    ///
    enum TextPosition {
        /// draw text to the left of the point
        kLeft, 
        /// draw text centered around the point
        kCenter, 
        /// draw text to the right of the point
        kRight
    };


	///
	enum DisplayObjects {
		///	show everything
		kDisplayEverything	= 0,
		///	show nurbs curves
		kDisplayNurbsCurves,
		///	show nurbs surfaces
		kDisplayNurbsSurfaces,
		///	show meshes
		kDisplayMeshes,
		///	show planes
		kDisplayPlanes,
		///	show lights
		kDisplayLights,
		///	show camera
		kDisplayCameras,
		///	show joints
		kDisplayJoints,
		///	show IK handles
		kDisplayIkHandles,
		///	show deformers
		kDisplayDeformers,
		///	show dynamics
		kDisplayDynamics,
		///	show locators
		kDisplayLocators,
		///	show dimensions
		kDisplayDimensions,
		///	show selection handles
		kDisplaySelectHandles,
		///	show pivots
		kDisplayPivots,
		///	show textures
		kDisplayTextures,
		///	show the grid
		kDisplayGrid,
		///	show NURBS CVs
		kDisplayCVs,
		///	show NURBS hulls
		kDisplayHulls,
		///	show strokes
		kDisplayStrokes,
		///	show subdivision surfaces
		kDisplaySubdivSurfaces
	};

	enum LightingMode {
		kLightAll,		// All lights ON mode
		kLightSelected, // Selected lights ON mode
		kLightActive,	// Active lights ON mode
		kLightDefault	// Default light ON mode
	};

	enum RendererName {
		// Equivalent to when the renderer name is "base_OpenGL_Renderer" when queried from the "modelEditor" command
		kDefaultQualityRenderer, 
		// Equivalent to when the renderer name is "hwRender_OpenGL_Renderer" when queried from the "modelEditor" command
		kHighQualityRenderer
	};

	// Possible depth buffer formats to read into : 8 bit, or floating point.
	enum DepthBufferFormat 
	{
		kDepth_8 = 0,
		kDepth_Float
	};
        
    ///
    M3dView();
    ///
    virtual ~M3dView();

	// global methods
    ///
    static M3dView		active3dView( MStatus * ReturnStatus = NULL );
    ///
	static unsigned     numberOf3dViews();
	///
	static MStatus      get3dView( const unsigned index, 
							                M3dView & view );

    ///
#if defined(OSMac_)
	AGLContext			display ( MStatus * ReturnStatus = NULL );
	// deviceContext() method not needed on the Mac
#elif defined (SGI) || defined (LINUX)
    Display *			display( MStatus * ReturnStatus = NULL );
	///
	GLXContext	        glxContext( MStatus * ReturnStatus = NULL );
#elif defined (_WIN32)
	HGLRC  				display( MStatus * ReturnStatus = NULL );
	///
	HDC     			deviceContext( MStatus * ReturnStatus = NULL );
#else
#error Unknown OS 
#endif

	///
	static MWindow		applicationShell( MStatus * ReturnStatus = NULL );

    ///
	M3dWindow  	window( MStatus * ReturnStatus = NULL );
    ///
    int         portWidth( MStatus * ReturnStatus = NULL );
    ///
    int         portHeight( MStatus * ReturnStatus = NULL );


	// OpenGL wrapper methods
    ///
    MStatus     beginGL();
    ///
    MStatus     endGL();

    //  OpenGL Selection
    //
	///
    void            beginSelect (GLuint *buffer = NULL, GLsizei size = 0);
	///
    GLint           endSelect   ();
	///
    bool            selectMode  () const;
	///
	bool 			textureMode () const;
	///
    void            loadName    (GLuint name);
	///
    void            pushName    (GLuint name);
	///
    void            popName     ();
	///
    void            initNames   ();


    // Overlay plane methods

    ///
    MStatus     beginOverlayDrawing();  
    ///
    MStatus     endOverlayDrawing();
    ///
    MStatus     clearOverlayPlane();    

    // Color methods

    ///
    MStatus     setDrawColor( unsigned index,
							  ColorTable table = kActiveColors );
    ///
    MStatus     setDrawColor( const MColor & color );

    ///
    bool        isColorIndexMode( MStatus * ReturnStatus = NULL ); 
    ///
    unsigned    numDormantColors( MStatus * ReturnStatus = NULL );
    ///
    unsigned    numActiveColors( MStatus * ReturnStatus = NULL );
    ///
    unsigned    numUserDefinedColors( MStatus * ReturnStatus = NULL );
    
    ///
    MStatus     setUserDefinedColor( unsigned index, const MColor & color );
    ///
    unsigned    userDefinedColorIndex( unsigned index, 
                                       MStatus * ReturnStatus = NULL );
  
    ///
    MColor      templateColor( MStatus * ReturnStatus = NULL ); 
    /// 
    MColor      backgroundColor( MStatus * ReturnStatus = NULL );

    /// 
    MColor      colorAtIndex( unsigned index, ColorTable table = kActiveColors,
                              MStatus * ReturnStatus = NULL ); 
	///
	MStatus		getColorIndexAndTable( unsigned glindex, unsigned &index, 
									   ColorTable &table ) const;

    // Text drawing methods

    ///
    MStatus     drawText( const MString & text, const MPoint position,
                          TextPosition textPosition = kLeft );

    // Camera methods

    ///
    MStatus     getCamera( MDagPath & camera );
    ///
    MStatus     setCamera( MDagPath & camera );

    ///
    MStatus     refresh( bool all = false, bool force = false );
    MStatus     refresh( bool all, bool force, bool offscreen );
	///
    MStatus     refresh( MPxGlBuffer &buffer );
    MStatus     refresh( MPxGlBuffer &buffer, bool offscreen );
    MStatus     refresh( MPxGlBuffer &buffer, bool offscreen, const MMatrix& projectionMatrix );

	// Light methods
	MStatus		getLightCount( unsigned int &count, bool visible = TRUE );
	MStatus		getLightingMode(LightingMode &mode );
	MStatus		getLightPath( unsigned int lightNumber, MDagPath &light );
	MStatus		isLightVisible( unsigned int lightNumber, bool &visible );
	MStatus		getLightIndex( unsigned int lightNumber, unsigned int &lightIndex );

    // Transformation methods

    ///
    MStatus     viewToWorld( short x_pos, short y_pos,
                             MPoint & worldPt, MVector & worldVector ) const;
    ///
    MStatus     viewToWorld( short x_pos, short y_pos,
                             MPoint & nearClipPt, MPoint & farClipPt ) const;
    ///
    MStatus     viewToObjectSpace( short x_pos, short y_pos,
                                   const MMatrix & localMatrixInverse,
                                   MPoint & oPt, MVector & oVector ) const;
    ///
    bool        worldToView( const MPoint& worldPt, 
                             short& x_pos, short& y_pos,
                             MStatus * ReturnStatus = NULL ) const;

	MStatus     projectionMatrix( MMatrix& projectionMatrix ) const;

	//	View selected prefix
	//
	///
	MString			viewSelectedPrefix(MStatus *ReturnStatus) const;
	///
	MStatus			setViewSelectedPrefix(MString &prefix);

	//	isolateSelect behavior 
	//
	//
	bool			showViewSelectedChildren(MStatus *ReturnStatus) const;
	///
	MStatus			setShowViewSelectedChildren(bool);

	//	Method to get a M3dView from a modelPanel
	//
	///
	static MStatus 	getM3dViewFromModelPanel(const MString &modelPaneName,
											 M3dView &view);

	//	Method to get a M3dView from a modelEditor
	//
	///
	static MStatus 	getM3dViewFromModelEditor(const MString &modelPaneName,
											  M3dView &view);

    // Display style methods 
	//
    ///
    DisplayStyle    displayStyle( MStatus * ReturnStatus = NULL ) const;
    ///
    bool        isShadeActiveOnly( MStatus * ReturnStatus = NULL ) const;
    ///
    MStatus     setDisplayStyle ( DisplayStyle style, bool activeOnly = false);


	// Renderer methods
	RendererName	getRendererName(MStatus *ReturnStatus /* = NULL */) const;
	bool			wireframeOnlyInShadedMode(MStatus *ReturnStatus /* = NULL */) const;

	// Viewport buffer methods
	MStatus			readColorBuffer( MImage &image );
	MStatus			writeColorBuffer( const MImage &image, signed short x = 0, signed short y = 0) const;
	MStatus			readDepthMap(unsigned short x, unsigned short y,
								unsigned int width, unsigned int height, 
								unsigned char* bufferPtr,
								DepthBufferFormat depthMapPrecision);
	MStatus			readBufferTo2dTexture(unsigned short x, unsigned short y,
								unsigned int width, unsigned int height);

protected:
// No protected members

private:
    static const char* className();
    M3dView( const void * );
    const void * fPtr;
};


#ifdef _WIN32
#pragma warning(default: 4522)
#endif // _WIN32

#endif /* __cplusplus */
#endif /* _M3dView */

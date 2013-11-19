//======= Copyright © 1996-2006, Valve Corporation, All rights reserved. ======
//
// Purpose: An example Maya locator node (definition)
//
//=============================================================================

// Maya includes

#include <maya/MFnAttribute.h>
#include <maya/MFnEnumAttribute.h>
#include <maya/MPlug.h>
#include <maya/MPxLocatorNode.h>

// Valve includes

#include "VsMayaMPxFactory.h"

//-----------------------------------------------------------------------------
// Example locator class
//-----------------------------------------------------------------------------
class CVstExampleLocator : public CVsMayaMPxLocatorNode
{
public:
	CVstExampleLocator();

	virtual ~CVstExampleLocator();

	static MStatus Initialize();

	virtual void postConstructor();

	virtual void draw(
		M3dView &i_m3dView,
		const MDagPath &i_mDagPath,
		M3dView::DisplayStyle i_displayStyle,
		M3dView::DisplayStatus i_displayStatus);

	virtual bool isBounded() const;

	virtual MBoundingBox boundingBox() const;

public:
	static const MTypeId s_id;
	static const char *const s_name;

	static MObject s_iaDisplayStyle;

protected:
	void DrawCube() const;
};


INSTALL_MAYA_MPXLOCATORNODE(
	CVstExampleLocator,
	vstExampleLocator,
	0x0010e4c1,
	CVstExampleLocator::Initialize,
	"An example locator that draws a cube written in the Valve way" );


//-----------------------------------------------------------------------------
// These are the attributes on the node
//-----------------------------------------------------------------------------
MObject CVstExampleLocator::s_iaDisplayStyle;


//-----------------------------------------------------------------------------
// Constructor
//-----------------------------------------------------------------------------
CVstExampleLocator::CVstExampleLocator()
{
}


//-----------------------------------------------------------------------------
// Destructor
//-----------------------------------------------------------------------------
CVstExampleLocator::~CVstExampleLocator()
{
}


//-----------------------------------------------------------------------------
// Purpose: Initialize the node, add static attributes, etc...
// Output : MStatus::kSuccess if everything was ok
//-----------------------------------------------------------------------------
MStatus CVstExampleLocator::Initialize()
{
	// Add a little enum attribute to control how it's drawn

	MFnEnumAttribute eFn;
	s_iaDisplayStyle = eFn.create( "displayStyle", "ds", 0 );
	eFn.setKeyable ( true );
	eFn.addField( "maya",		0 );
	eFn.addField( "shaded",		1 );
	eFn.addField( "wireframe",	2 );
	eFn.addField( "points",		3 );

	addAttribute(s_iaDisplayStyle);

	return MS::kSuccess;
}


//-----------------------------------------------------------------------------
// Purpose: Do things after the node is creator and the constructor is called
//			and the node is associated with it's internal Maya MObject
//			These are things that cannot be done in Factory(), Initialize()
//			or the Constructor because the node hasn't been inserted into
//			Maya's graph until now
//-----------------------------------------------------------------------------

 void CVstExampleLocator::postConstructor()
{
}


//-----------------------------------------------------------------------------
// Purpose:
// Input  :	i_m3dView	The Maya 3D view the node will be drawn in
//			i_mDagPath		The Maya dagPath to the node that will be drawn
//			i_displayStyle	The style in which to draw the object, can be
//							completely ignored but one of the following
//							will be passed:
//
//							M3dView::kBoundingBox	Just draw a bounding box
//							M3dView::kFlatShaded	Draw object flat shaded
//							M3dView::kGouraudShaded	Draw object smooth shaded
//							M3dView::kWireFrame		Draw object wireframe
//							M3dView::kPoints		Draw object as points only
//			i_displayStatus	The status of the object being drawn, status mainly
//							refers to the selection status of the object
//							and for some things Maya uses different colors
//							to reflect that status
//							See M3dView::DisplayStatus for more info
//-----------------------------------------------------------------------------
void CVstExampleLocator::draw(
	M3dView &i_m3dView,
	const MDagPath & /* i_mDagPath */,
	M3dView::DisplayStyle i_displayStyle,
	M3dView::DisplayStatus /* i_displayStatus */ )
{
	// Necessary step before doing any OpenGL calls
	// NOTE: This call does not push or save any OpenGL state info
	//       doing so is up to the implementor of draw()

	i_m3dView.beginGL();

	// Drawing happens here

	// Save the parts of the OpenGL state that will be changed

	glPushAttrib( GL_POLYGON_BIT );

	// Set up drawing state as requested by Maya or the user

	M3dView::DisplayStyle displayStyle( i_displayStyle );

	// Get the MObject for this node
	const MObject thisObj( thisMObject() );
	
	// Create an MPlug for the s_iaDisplayStyle attribute
	const MPlug iaDisplayStyleP( thisObj, s_iaDisplayStyle );

	// Get the value of that attribute
	short iaDisplayStyleV( 0 );
	iaDisplayStyleP.getValue( iaDisplayStyleV );

	switch ( iaDisplayStyleV )
	{
	case 0:		// Use Maya
	default:
		break;
	case 1:		// Force shaded
		displayStyle = M3dView::kFlatShaded;
		break;
	case 2:		// Force wireframe
		displayStyle = M3dView::kWireFrame;
		break;
	case 3:		// Force points
		displayStyle = M3dView::kFlatShaded;
		break;
	}

	switch ( displayStyle )
	{
	case M3dView::kFlatShaded:
	case M3dView::kGouraudShaded:
		// In this case, flat and smooth shaded are the same thing
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		break;
	case M3dView::kPoints:
		glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
		break;
	case M3dView::kBoundingBox:
	case M3dView::kWireFrame:
	default:
		// In this case, wireframe and bounding box are the same thing
		// and we'll default to wireframe
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		break;
	}

	// Set up the drawing color as requested

	// For now, do nothing... let Maya set the color

	DrawCube();

	// Restore the OpenGL state to what it was before we mucked with it
	// Not necessary to restore current draw position, etc...

	glPopAttrib();

	// Necessary step after all OpenGL drawing has finished

	i_m3dView.endGL();
}


//-----------------------------------------------------------------------------
// Purpose: Tells maya whether this particular locator node is bounded or not
//          If it isn't then screen selection doesn't work and draw is called
//          all the time.  It's better is a node is bounded but you can manage
//			some funky tricks if it's not (i.e. if you want to draw some OpenGL
//			info on the screen always and need to adjust for the camera or
//			draw in an Ortho view, etc...)
// Output : true is the locator has a known bounding box
//-----------------------------------------------------------------------------
bool CVstExampleLocator::isBounded() const
{
	// This is only here instead of inline in the header for sake of
	// completeness and all the comments in one place

	return true;
}


//-----------------------------------------------------------------------------
// Purpose: Figure out the bounding box of whatever is being drawn
// Output : The bounding box of whatever is being drawn
//-----------------------------------------------------------------------------
MBoundingBox CVstExampleLocator::boundingBox() const
{
	// In this example, a 1x1x1 cube is being drawn centered around the origin
	// so the bounding box is always constant
	// Never take into account the matrix coming into the locator node
	// Maya handles that... If 'localPosition' and/or 'localScale' are
	// desirable attributes to support (generally they are ignored) then
	// they must be taken into account
	//
	// < -0.5, -0.5, -0.5 > - <  0.5,  0.5,  0.5 >

	return MBoundingBox(
		MPoint( -0.5, -0.5, -0.5 ),
		MPoint(  0.5,  0.5,  0.5 )
	);
}


//-----------------------------------------------------------------------------
// Purpose: Draws an OpenGL cube
//-----------------------------------------------------------------------------
void CVstExampleLocator::DrawCube() const
{
	static const float cube[][3] = {
		{ -0.5, -0.5,  0.5 },
		{ -0.5,  0.5,  0.5 },
		{  0.5,  0.5,  0.5 },
		{  0.5, -0.5,  0.5 },
		{  0.5, -0.5, -0.5 },
		{  0.5,  0.5, -0.5 },
		{ -0.5,  0.5, -0.5 },
		{ -0.5, -0.5, -0.5 }
	};

	// Front Face

	glNormal3f(  0.0,  0.0,  1.0 );

	glBegin( GL_POLYGON );
		glVertex3fv( cube[0] );
		glVertex3fv( cube[1] );
		glVertex3fv( cube[2] );
		glVertex3fv( cube[3] );
	glEnd();

	// Back Face

	glNormal3f(  0.0,  0.0, -1.0 );

	glBegin( GL_POLYGON );
		glVertex3fv( cube[4] );
		glVertex3fv( cube[5] );
		glVertex3fv( cube[6] );
		glVertex3fv( cube[7] );
	glEnd();

	// Left Face

	glNormal3f(  1.0,  0.0,  0.0 );

	glBegin( GL_POLYGON );
		glVertex3fv( cube[7] );
		glVertex3fv( cube[6] );
		glVertex3fv( cube[1] );
		glVertex3fv( cube[0] );
	glEnd();

	// Right Face

	glNormal3f( -1.0,  0.0,  0.0 );

	glBegin( GL_POLYGON );
		glVertex3fv( cube[3] );
		glVertex3fv( cube[2] );
		glVertex3fv( cube[5] );
		glVertex3fv( cube[4] );
	glEnd();

	// Top Face

	glNormal3f(  0.0,  1.0,  0.0 );

	glBegin( GL_POLYGON );
		glVertex3fv( cube[1] );
		glVertex3fv( cube[6] );
		glVertex3fv( cube[5] );
		glVertex3fv( cube[2] );
	glEnd();

	// Bottom Face

	glNormal3f(  0.0, -1.0,  0.0 );

	glBegin( GL_POLYGON );
		glVertex3fv( cube[7] );
		glVertex3fv( cube[0] );
		glVertex3fv( cube[3] );
		glVertex3fv( cube[4] );
	glEnd();
}
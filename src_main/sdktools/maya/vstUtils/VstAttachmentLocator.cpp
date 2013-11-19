//======= Copyright © 1996-2006, Valve Corporation, All rights reserved. ======
//
// A maya locator for Valve attachment nodes
//
//=============================================================================

// Maya includes
#include <maya/MColor.h>
#include <maya/MFnEnumAttribute.h>
#include <maya/MFnNumericAttribute.h>

// Valve includes
#include "valveMaya.h"
#include "VsMayaMPxFactory.h"


class CVstAttachmentLocatorNode : public CVsMayaMPxLocatorNode
{
public:
	CVstAttachmentLocatorNode();

	virtual ~CVstAttachmentLocatorNode();

	static MStatus Initialize();

	virtual bool getInternalValueInContext(
		const MPlug &mPlug,
		MDataHandle &mDataHandle,
		MDGContext &mDGContext);

	virtual bool setInternalValueInContext(
		const MPlug &mPlug,
		const MDataHandle &mDataHandle,
		MDGContext &mDGContext );

	virtual bool isBounded() const { return true; }

	virtual MBoundingBox boundingBox() const;

	virtual void draw(
		M3dView &m3dView,
		const MDagPath &mDagPath,
		M3dView::DisplayStyle displayStyle,
		M3dView::DisplayStatus displayStatus );

	virtual MStatus shouldSave(
		const MPlug &mPlug,
		bool &result );

	static MObject m_iaRigid;	// Only for backwards compatibility
	static MObject m_iaType;
	static MObject m_iaWorldAligned;

	static MObject m_iaEndType;

protected:
	static void DrawArrowhead();

	int GetDrawColor(
		const MDagPath &mDagPath,
		M3dView::DisplayStatus displayStatus );
};


//-----------------------------------------------------------------------------
// Statics
//-----------------------------------------------------------------------------
MObject CVstAttachmentLocatorNode::m_iaRigid;
MObject CVstAttachmentLocatorNode::m_iaType;
MObject CVstAttachmentLocatorNode::m_iaWorldAligned;

MObject CVstAttachmentLocatorNode::m_iaEndType;


//-----------------------------------------------------------------------------
// Register the node with Maya
//-----------------------------------------------------------------------------
INSTALL_MAYA_MPXLOCATORNODE(
	CVstAttachmentLocatorNode,
	vstAttachment,
	0x0010e4c5,
	CVstAttachmentLocatorNode::Initialize,
	"Animatable Valve Attachments"
);


//-----------------------------------------------------------------------------
// Constructor
//-----------------------------------------------------------------------------
CVstAttachmentLocatorNode::CVstAttachmentLocatorNode()
{
}


//-----------------------------------------------------------------------------
// Destructor
//-----------------------------------------------------------------------------
CVstAttachmentLocatorNode::~CVstAttachmentLocatorNode()
{
}


//-----------------------------------------------------------------------------
// Purpose: Initialize the node, add static attributes, etc...
//-----------------------------------------------------------------------------
MStatus CVstAttachmentLocatorNode::Initialize()
{
	MFnEnumAttribute eFn;
	MFnNumericAttribute nFn;

	// Behavioral Attributes
	m_iaRigid = nFn.create( "rigid", "rg", MFnNumericData::kBoolean );
	nFn.setInternal( true );
	nFn.setStorable( false );
	nFn.setHidden( true );

	m_iaType = eFn.create( "type", "typ", 1 );
	eFn.addField( "Bone + Attachment", 0 );
	eFn.addField( "Just Attachment", 1 );
	eFn.addField( "Just Bone", 2 );
	eFn.setKeyable( true );

	m_iaWorldAligned = nFn.create( "worldAligned", "wa", MFnNumericData::kBoolean, false );
	nFn.setKeyable( true );

	addAttribute( m_iaType );

	addAttribute( m_iaRigid );

	addAttribute( m_iaWorldAligned );

	// UI Attributes
	m_iaEndType = eFn.create( "endType", "et", 1 );
	eFn.addField( "None", 0 );
	eFn.addField( "Arrows", 1 );
	eFn.addField( "Blah", 2 );
	eFn.setKeyable( true );

	addAttribute( m_iaEndType );

	return MS::kSuccess;
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
bool CVstAttachmentLocatorNode::getInternalValueInContext(
	const MPlug &mPlug,
	MDataHandle &mDataHandle,
	MDGContext &mDGContext )
{
	if ( mPlug == m_iaRigid )
	{
		MDataBlock mDataBlock = forceCache( mDGContext );
		mDataHandle.set( mDataBlock.inputValue( m_iaType ).asShort() == 0 ? false : true );

		return true;
	}

	return false;
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
bool CVstAttachmentLocatorNode::setInternalValueInContext(
	const MPlug &mPlug,
	const MDataHandle &mDataHandle,
	MDGContext &mDGContext )
{
	if ( mPlug == m_iaRigid )
	{
		MDataBlock mDataBlock = forceCache( mDGContext );
		mDataBlock.outputValue( m_iaType ).set( mDataHandle.asBool() ? short( 1 ) : short( 0 ) );

		return true;
	}

	return false;
}


//-----------------------------------------------------------------------------
// Purpose: Return the bounding box for the node, Maya uses this to determine
//			if the node needs to be drawn and for selection
//-----------------------------------------------------------------------------
MBoundingBox CVstAttachmentLocatorNode::boundingBox() const
{
	static const MPoint ll( -0.5, -0.5, -0.5 );
	static const MPoint ur(  0.5,  0.5,  0.5 );

	MBoundingBox bbox( ll, ur );

	MObject nodeObj( thisMObject() );
	MPlug waP( nodeObj, m_iaWorldAligned );
	bool waV( false );
	waP.getValue( waV );

	if ( waV )
	{
		MDagPath mDagPath;
		MDagPath::getAPathTo( nodeObj, mDagPath );
		MTransformationMatrix tm( mDagPath.inclusiveMatrixInverse() );
		tm.addTranslation( MVector( mDagPath.exclusiveMatrix()[ 3 ] ), MSpace::kWorld );
		bbox.transformUsing( tm.asMatrix() );
	}
	else
	{
		MPlug lpP( nodeObj, localPosition );
		MVector lp;
		lpP.child( 0 ).getValue( lp.x );
		lpP.child( 1 ).getValue( lp.y );
		lpP.child( 2 ).getValue( lp.z );
		bbox = MBoundingBox( ll + lp, ur + lp );
	}

	return bbox;
}


//-----------------------------------------------------------------------------
// Purpose: Draw the locator...
//-----------------------------------------------------------------------------
void CVstAttachmentLocatorNode::draw(
	M3dView &m3dView,
	const MDagPath &mDagPath,
	M3dView::DisplayStyle displayStyle,
	M3dView::DisplayStatus displayStatus )
{
	const uint active( displayStatus == M3dView::kActive || displayStatus == M3dView::kLead ? 0 : 1 );

	// Table of colors to draw the little positive axis identifiers
	static const int colorTable[][ 3 ] =
	{
		{ 12, 13,  5 },	// Active  X, Y, Z
		{ 11,  6,  4 }	// Dormant X, Y, Z
	};

	m3dView.beginGL();

	glPushAttrib( GL_CURRENT_BIT | GL_TRANSFORM_BIT );

	glMatrixMode( GL_MODELVIEW );
	glPushMatrix();

	MObject nodeObj( thisMObject() );
	MPlug waP( nodeObj, m_iaWorldAligned );
	bool waV( false );
	waP.getValue( waV );

	if ( waV )
	{
		glMultMatrixd( mDagPath.inclusiveMatrixInverse()[ 0 ] );
		const MMatrix wm( mDagPath.exclusiveMatrix() );
		glTranslated( wm[ 3 ][ 0 ], wm[ 3 ][ 1 ], wm[ 3 ][ 2 ] );
	}
	else
	{
		MPlug lpP( nodeObj, localPosition );
		MVector lp;
		lpP.child( 0 ).getValue( lp.x );
		lpP.child( 1 ).getValue( lp.y );
		lpP.child( 2 ).getValue( lp.z );
		glTranslated( lp.x, lp.y, lp.z );
	}

	m3dView.setDrawColor( GetDrawColor( mDagPath, displayStatus ) );

	glBegin( GL_LINES );

	glVertex3f( -1.0f,  0.0f,  0.0f );
	glVertex3f(  1.0f,  0.0f,  0.0f );

	glVertex3f(  0.0f, -1.0f,  0.0f );
	glVertex3f(  0.0f,  1.0f,  0.0f );

	glVertex3f(  0.0f,  0.0f, -1.0f );
	glVertex3f(  0.0f,  0.0f,  1.0f );

	glEnd();

	MPlug etP( nodeObj, m_iaEndType );
	bool etV( false );
	etP.getValue( etV );

	if ( etV )
	{
		glPushMatrix();
		m3dView.setDrawColor( colorTable[ active ][ 0 ] );
		glRotatef(  90.0f,  0.0f,  0.0f, -1.0f );
		DrawArrowhead();
		glPopMatrix();

		glPushMatrix();
		m3dView.setDrawColor( colorTable[ active ][ 1 ] );
		DrawArrowhead();
		glPopMatrix();

		glPushMatrix();
		m3dView.setDrawColor( colorTable[ active ][ 2 ] );
		glRotatef(  90.0f,  1.0f,  0.0f,  0.0f );
		DrawArrowhead();
		glPopMatrix();
	}

	glPopMatrix();

	glPopAttrib();

	m3dView.endGL();
}


//-----------------------------------------------------------------------------
// Overrides Maya's behaviour of not saving default values.
// Allows the default value of 'rigid' to change without breaking
// saved scenes.
//-----------------------------------------------------------------------------
MStatus CVstAttachmentLocatorNode::shouldSave(
	const MPlug &mPlug,
	bool &result )
{
	if ( mPlug == m_iaRigid )
	{
		result = true;
		return MS::kSuccess;
	}

	return MS::kUnknownParameter;
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void CVstAttachmentLocatorNode::DrawArrowhead()
{
	static const float r2( sqrtf( 2.0f ) / 2.0f );

	glTranslatef( 0.0f, 1.0f, 0.0f );
	glScalef(  0.05f, 0.25f, 0.05f );

	glBegin( GL_TRIANGLE_FAN );

	// Tip of arrowhead
	glNormal3f(  0.0f,  0.0f,  0.0f );
	glVertex3f(  0.0f,  0.0f,  0.0f );

	// Base of arrowhead (unit circle points)
	glNormal3f(  1.0f, -1.0f,  0.0f );
	glVertex3f(  1.0f, -1.0f,  0.0f );

	glNormal3f(  r2,   -1.0f,  r2 );
	glVertex3f(  r2,   -1.0f,  r2 );

	glNormal3f(  0.0f, -1.0f,  1.0f );
	glVertex3f(  0.0f, -1.0f,  1.0f );

	glNormal3f(  -r2,  -1.0f,  r2 );
	glVertex3f(  -r2,  -1.0f,  r2 );

	glNormal3f( -1.0f, -1.0f,  0.0f );
	glVertex3f( -1.0f, -1.0f,  0.0f );

	glNormal3f( -r2,   -1.0f, -r2 );
	glVertex3f( -r2,   -1.0f, -r2 );

	glNormal3f(  0.0f, -1.0f, -1.0f );
	glVertex3f(  0.0f, -1.0f, -1.0f );

	glNormal3f(  r2,   -1.0f, -r2 );
	glVertex3f(  r2,   -1.0f, -r2 );

	glNormal3f(  1.0f, -1.0f,  0.0f );
	glVertex3f(  1.0f, -1.0f,  0.0f );

	glEnd();
}


int CVstAttachmentLocatorNode::GetDrawColor(
	const MDagPath &mDagPath,
	M3dView::DisplayStatus displayStatus )
{
	// This seems to be a bug in Maya...
	// Lead objects get the dormant color returned instead of the active color
	// 13 seems to be the active color
	int drawColor( displayStatus == M3dView::kLead ? 13 : color( displayStatus ) );

	MDagPath cDagPath( mDagPath );
	cDagPath.push( thisMObject() );
	MFnDagNode dagFn( cDagPath );
	MPlug oveP( dagFn.findPlug( "ove" ) );	// Draw override
	bool oveV;
	oveP.getValue( oveV );
	if ( oveV )
	{
		MPlug ovcP( dagFn.findPlug( "ovc" ) );
		char ovcV;
		ovcP.getValue( ovcV );
		if ( ovcV > 0 )
		{
			drawColor = ovcV - 1;
		}
	}
	else {
		MDagPath mDagPath( mDagPath );
		MFnDagNode dagFn( mDagPath );
		MPlug oveP( dagFn.findPlug( "ove" ) );	// Draw override
		bool oveV;
		oveP.getValue( oveV );
		if ( oveV )
		{
			MPlug ovcP( dagFn.findPlug( "ovc" ) );
			char ovcV;
			ovcP.getValue( ovcV );
			if ( ovcV > 0 )
			{
				drawColor = ovcV - 1;
			}
		}
	}

	MPlug useObjectColorP( mDagPath.node(), useObjectColor );
	bool useObjectColorV;
	useObjectColorP.getValue( useObjectColorV );
	if ( useObjectColorV )
	{
		MPlug objectColorP( mDagPath.node(), objectColor );
		short objectColorV;
		objectColorP.getValue( objectColorV );
		drawColor = objectColorV;
	}

	return drawColor;
}
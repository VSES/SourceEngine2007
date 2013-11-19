//======= Copyright © 1996-2006, Valve Corporation, All rights reserved. ======
//
// Purpose: An example Maya locator node (definition)
//
//=============================================================================

// Standard includes
#include <math.h>


// Maya includes
#include <maya/MAngle.h>
#include <maya/MAttributeSpec.h>
#include <maya/MAttributeSpecArray.h>
#include <maya/MDistance.h>
#include <maya/MDrawData.h>
#include <maya/MFnNumericAttribute.h>
#include <maya/MFnUnitAttribute.h>
#include <maya/MMaterial.h>


// OpenGL
#include <gl/GL.h>


// Valve includes
#include "valveMaya.h"
#include "mathlib/mathlib.h"


// Local Includes
#include "vstEyeballNode.h"


// Get rid of min() & max() macros as they interfere with
// MBoundingBox::min() & MBoundingBox::max() 
#if defined min
#undef min
#endif // defined min

#if defined max
#undef max
#endif // defined max


// Define template versions, which they should have been all along...
template < class T_t >
inline T_t min( const T_t &a, const T_t &b )
{
	return a < b ? a : b;
}


template < class T_t >
inline T_t max( const T_t &a, const T_t &b )
{
	return a > b ? a : b;
}


INSTALL_MAYA_MPXSHAPENODE(
	CVstEyeballNode,
	vstEyeball,
	0x0010e4c9,
	CVstEyeballNode::Initialize,
	CVstEyeballUI,
	"Valve Model Eyeball Node"
);


//-----------------------------------------------------------------------------
// Statics
//-----------------------------------------------------------------------------
MObject CVstEyeballNode::m_iaDiameter;
MObject CVstEyeballNode::m_iaAngle;
MObject CVstEyeballNode::m_iaIrisScale;
MObject CVstEyeballNode::m_iaPupilScale;

MObject CVstEyeballNode::m_iaSlices;
MObject CVstEyeballNode::m_iaStacks;

MObject CVstEyeballNode::m_iaDrawEyeball;	// Draw the eyeball
MObject CVstEyeballNode::m_iaDrawIris;		// Draw the iris
MObject CVstEyeballNode::m_iaDrawPupil;		// Draw the pupil

MObject CVstEyeballNode::m_caSyncAttrs;


//-----------------------------------------------------------------------------
// Constructor
//-----------------------------------------------------------------------------
CVstEyeballNode::CVstEyeballNode()
{
}


//-----------------------------------------------------------------------------
// Destructor
//-----------------------------------------------------------------------------
CVstEyeballNode::~CVstEyeballNode()
{
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void CVstEyeballNode::postConstructor()
{
	// Allow a shading group to be connected
	setRenderable( true );
}


//-----------------------------------------------------------------------------
// Initialize the node, add static attributes, etc...
//-----------------------------------------------------------------------------
MStatus CVstEyeballNode::Initialize()
{
	MFnNumericAttribute nFn;
	MFnUnitAttribute uFn;

	m_iaDiameter = uFn.create( "eyeballDiameter", "eb", MDistance( 2.0 ) );
	uFn.setSoftMin( MDistance(  0.0 ) );
	uFn.setSoftMax( MDistance( 10.0 ) );
	uFn.setKeyable( true );

	m_iaAngle = uFn.create( "yawAngle", "ya", MAngle( 2.0, MAngle::kDegrees ) );
	uFn.setSoftMin( MAngle( -45.0, MAngle::kDegrees ) );
	uFn.setSoftMax( MAngle(  45.0, MAngle::kDegrees ) );
	uFn.setKeyable( true );

	m_iaIrisScale = nFn.create( "irisScale", "is", MFnNumericData::kDouble, 0.5 );
	nFn.setSoftMin( 0.0 );
	nFn.setSoftMax( 1.0 );
	nFn.setKeyable( true );

	m_iaPupilScale = nFn.create( "pupilScale", "ps", MFnNumericData::kDouble, 0.5 );
	nFn.setSoftMin( 0.0 );
	nFn.setSoftMax( 1.0 );
	nFn.setKeyable( true );

	m_iaStacks = nFn.create( "stacks", "st", MFnNumericData::kInt, 20 );
	nFn.setMin( 5 );
	nFn.setSoftMax( 50 );
	nFn.setKeyable( true );

	m_iaSlices = nFn.create( "slices", "sl", MFnNumericData::kInt, 20 );
	nFn.setMin( 5 );
	nFn.setSoftMax( 50 );
	nFn.setKeyable( true );

	m_iaDrawEyeball = nFn.create( "drawEyeball", "de", MFnNumericData::kBoolean, true );
	nFn.setKeyable( true );

	m_iaDrawIris = nFn.create( "drawIris", "di", MFnNumericData::kBoolean, false );
	nFn.setKeyable( true );

	m_iaDrawPupil = nFn.create( "drawPupil", "dp", MFnNumericData::kBoolean, false );
	nFn.setKeyable( true );

	m_caSyncAttrs = nFn.create( "ca_SyncAttrs", "casa", MFnNumericData::kBoolean );
	nFn.setHidden( true );
	nFn.setStorable( false );
	nFn.setReadable( false );
	nFn.setWritable( false );
	nFn.setConnectable( false );

	addAttribute( m_iaDiameter );

	addAttribute( m_iaAngle );

	addAttribute( m_iaIrisScale );

	addAttribute( m_iaPupilScale );

	addAttribute( m_iaStacks );

	addAttribute( m_iaSlices );

	addAttribute( m_iaDrawEyeball );
	addAttribute( m_iaDrawIris );
	addAttribute( m_iaDrawPupil );

	addAttribute( m_caSyncAttrs );

	attributeAffects( m_iaDiameter, m_caSyncAttrs );
	attributeAffects( m_iaAngle, m_caSyncAttrs );
	attributeAffects( m_iaIrisScale, m_caSyncAttrs );
	attributeAffects( m_iaPupilScale, m_caSyncAttrs );
	attributeAffects( m_iaStacks, m_caSyncAttrs );
	attributeAffects( m_iaSlices, m_caSyncAttrs );

	return MS::kSuccess;
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
MStatus CVstEyeballNode::compute(
	const MPlug &mPlug,
	MDataBlock &mDataBlock )
{
	if ( mPlug == m_caSyncAttrs )
	{
		const double dIrisScale = mDataBlock.inputValue( m_iaIrisScale ).asDouble();
		const double dPupilScale = dIrisScale * mDataBlock.inputValue( m_iaPupilScale ).asDouble();

		const double dEyeballBegin = 1.0 - FastSqrt( 1 - dIrisScale * dIrisScale );
		const double dIrisBegin = 1.0 - FastSqrt( 1 - dPupilScale * dPupilScale );
		const double dIrisEnd = 1.0 - FastSqrt( 1 - dIrisScale * dIrisScale );
		const double dPupilEnd = 1.0 - FastSqrt( 1 - dPupilScale * dPupilScale );

		minfo << " * irisScale:  " << dIrisScale << std::endl;
		minfo << " * pupilScale: " << dPupilScale << std::endl;
		minfo << " * Pupil:      " << 0.0 << " - " << dPupilEnd << std::endl;
		minfo << " * Iris:       " << dIrisBegin << " - " << dIrisEnd << std::endl;
		minfo << " * Eyeball:    " << dEyeballBegin << " - " << 1.0 << std::endl;

		m_eyeball.SetStacks( max( mDataBlock.inputValue( m_iaStacks ).asInt(), 1 ) );
		m_eyeball.SetSlices( max( mDataBlock.inputValue( m_iaSlices ).asInt(), 1 ) );
		m_eyeball.SetRadius( mDataBlock.inputValue( m_iaDiameter ).asDistance().as( MDistance::internalUnit() ) / 2.0 );
		m_eyeball.SetLatBegin( dEyeballBegin );

		m_iris.SetStacks( max( mDataBlock.inputValue( m_iaStacks ).asInt(), 1 ) );
		m_iris.SetSlices( max( mDataBlock.inputValue( m_iaSlices ).asInt(), 1 ) );
		m_iris.SetRadius( mDataBlock.inputValue( m_iaDiameter ).asDistance().as( MDistance::internalUnit() ) / 2.0 );
		m_iris.SetLatBegin( dIrisBegin );
		m_iris.SetLatEnd( dIrisEnd );

		m_pupil.SetStacks( max( mDataBlock.inputValue( m_iaStacks ).asInt(), 1 ) );
		m_pupil.SetSlices( max( mDataBlock.inputValue( m_iaSlices ).asInt(), 1 ) );
		m_pupil.SetRadius( mDataBlock.inputValue( m_iaDiameter ).asDistance().as( MDistance::internalUnit() ) / 2.0 );
		m_pupil.SetLatEnd( dPupilEnd );

		mDataBlock.setClean( m_caSyncAttrs );
	}

	return MS::kUnknownParameter;
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
MBoundingBox CVstEyeballNode::boundingBox() const
{
	SyncAttrs();

	const double dScale = m_eyeball.GetRadius();

	return MBoundingBox( MPoint( -dScale, -dScale, -dScale ), MPoint( dScale, dScale, dScale ) );
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
bool CVstEyeballNode::match(
	const MSelectionMask &mask,
	const MObjectArray &cList ) const
{
	Assert( 0 );
	minfo << " * Match" << std::endl;

	// Don't know what to do for the first thing
	if ( cList.length() == 0 )
		return mask.intersects( MSelectionMask::kSelectMeshes );

	for ( uint i( 0 ); i != cList.length(); ++i )
	{
		if ( cList[ i ].apiType() == MFn::kMeshVertComponent && mask.intersects( MSelectionMask::kSelectMeshVerts ) )
		{
			minfo << "   + " << true << std::endl;
			return true;
		}
	}

	return false;
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
MPxSurfaceShape::MatchResult CVstEyeballNode::matchComponent(
	const MSelectionList &iList,
	const MAttributeSpecArray &specArray,
	MSelectionList &oList )
{
	const MAttributeSpec volumeSpec( specArray[ 0 ] );

//	if ( specArray.length() != 1 || volumeSpec.dimensions() <= 0 ||
//		!( !Q_stricmp( volumeSpec.name().asChar(), "eyeball" ) ) )
		return MPxSurfaceShape::matchComponent( iList, specArray, oList );

	return MPxSurfaceShape::kMatchNone;
}


#if 0
//-----------------------------------------------------------------------------
// Purpose: Draw the locator...
//-----------------------------------------------------------------------------
void CVstEyeballNode::DrawBoundingBox() const
{
	glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );

	MBoundingBox bbox( MPoint( -1.0, -1.0, -1.0 ), MPoint( 1.0, 1.0, 1.0 ) );

	MPoint n( bbox.min() );
	MPoint x( bbox.max() );
	glBegin( GL_QUAD_STRIP );
	{
		glVertex3d( x.x, n.y, n.z );
		glVertex3d( n.x, n.y, n.z );
		glVertex3d( x.x, x.y, n.z );
		glVertex3d( n.x, x.y, n.z );
		glVertex3d( x.x, x.y, x.z );
		glVertex3d( n.x, x.y, x.z );
		glVertex3d( x.x, n.y, x.z );
		glVertex3d( n.x, n.y, x.z );
		glVertex3d( x.x, n.y, n.z );
		glVertex3d( n.x, n.y, n.z );
	}
	glEnd();
}

#endif // 0


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void CVstEyeballNode::SyncAttrs() const
{
	bool bJunk;

	MPlug( thisMObject(), m_caSyncAttrs ).getValue( bJunk );
}


//=============================================================================
//
// CVstEyeballUI
//
//=============================================================================



//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
CUnitSphere::CUnitSphere()
: m_bRebuildSphere( true )
, m_dRadius( 1.0 )
, m_nSlices( 15 )
, m_nStacks( 10 )
, m_hdc( NULL )
, m_hglrc( NULL )
, m_displayList( 0U )
, m_dLatBegin( 0.0 )
, m_dLatEnd( 1.0 )
{
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
CUnitSphere::~CUnitSphere()
{
	if ( m_hdc && m_hglrc )
	{
		HDC hdc = wglGetCurrentDC();
		HGLRC hglrc = wglGetCurrentContext();

		if ( wglMakeCurrent( m_hdc, m_hglrc ) && glIsList( m_displayList ) )
		{
			glDeleteLists( m_displayList, 1 );
		}

		if ( hdc && hglrc )
		{
			wglMakeCurrent( hdc, hglrc );
		}
	}
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void CUnitSphere::Draw( M3dView &m3dView )
{
	if ( !m_hdc )
	{
		m_hdc = m3dView.deviceContext();
		m_hglrc = m3dView.display();
		m_displayList = glGenLists( 1 );
	}

	if ( m_bRebuildSphere )
	{
		glNewList( m_displayList, GL_COMPILE );
		{
			const int nLatBegin = static_cast< int >( floor( m_dLatBegin * m_nStacks ) );
			const int nLatEnd = static_cast< int >( floor( m_dLatEnd * m_nStacks ) );

			const int nLatCount = nLatEnd - nLatBegin + 1;
			const double dStacks = static_cast< double >( m_nStacks );

			double *pLatVals = reinterpret_cast< double * >( alloca( nLatCount * sizeof( double ) ) );
			pLatVals[ 0 ] = m_dLatBegin * M_PI;
			pLatVals[ nLatCount - 1 ] = m_dLatEnd * M_PI;

			for ( int i = 1; i < nLatCount - 1; ++i )
			{
				pLatVals[ i ] = ( i + nLatBegin ) / dStacks * M_PI;
			}

			const int nLongCount = m_nSlices;
			const double dLongCount = static_cast< double >( nLongCount );

			Vector *pSemiCircle = reinterpret_cast< Vector * >( alloca( nLatCount * sizeof( Vector ) ) );
			memset( pSemiCircle, 0, nLatCount * sizeof( Vector ) );

			Vector *pBuf0 = reinterpret_cast< Vector * >( alloca( nLatCount * sizeof( Vector ) ) );
			Vector *pBuf1 = reinterpret_cast< Vector * >( alloca( nLatCount * sizeof( Vector ) ) );
			Vector *pLhs = pSemiCircle;
			Vector *pRhs = pBuf1;
			Vector *pTmp;

			for ( int i = 0; i < nLatCount; ++i )
			{
				SinCos( pLatVals[ i ], &pSemiCircle[ i ].y, &pSemiCircle[ i ].z );
				*( pSemiCircle + i ) *= m_dRadius;
			}

			matrix3x4_t mRot;

			for ( int i = 0; i <= nLongCount; ++i )
			{
				AngleMatrix( RadianEuler( 0.0f, 0.0f, i / dLongCount * 2.0 * M_PI ), mRot );

				if ( i == nLongCount )
				{
					pRhs = pSemiCircle;
				}
				else
				{
					for ( int j = 0; j < nLatCount; ++j )
					{
						VectorRotate( pSemiCircle[ j ], mRot, pRhs[ j ] );
					}
				}

				Vector n;
				glBegin( GL_QUADS );
				for ( int j = 0; j < nLatCount - 1; ++j )
				{
					n = pLhs[ j ] + pRhs[ j ];
					n.NormalizeInPlace();
					glNormal3fv( &n.x );

					glVertex3fv( reinterpret_cast< float * >( pLhs + j ) );
					//						glNormal3fv( reinterpret_cast< float * >( pLhs + j ) );

					glVertex3fv( reinterpret_cast< float * >( pRhs + j ) );
					//						glNormal3fv( reinterpret_cast< float * >( pRhs + j ) );

					glVertex3fv( reinterpret_cast< float * >( pRhs + j + 1 ) );
					//						glNormal3fv( reinterpret_cast< float * >( pRhs + j + 1 ) );

					glVertex3fv( reinterpret_cast< float * >( pLhs + j + 1 ) );
					//						glNormal3fv( reinterpret_cast< float * >( pLhs + j + 1 ) );
				}
				glEnd();

				if ( i == 0 )
				{
					pLhs = pRhs;
					pRhs = pBuf0;
				}
				else
				{
					pTmp = pLhs;
					pLhs = pRhs;
					pRhs = pTmp;
				}
			}
		}
		glEndList();

		m_bRebuildSphere = false;
	}

	glCallList( m_displayList );
}


//=============================================================================
//
//=============================================================================
class CVstEyeballUI : public MPxSurfaceShapeUI
{
public:
	CVstEyeballUI();

	virtual ~CVstEyeballUI();

	virtual void getDrawRequests(
		const MDrawInfo &mDrawInfo,
		bool objectAndActiveOnly,
		MDrawRequestQueue &mDrawRequestQueue );

	virtual void draw(
		const MDrawRequest &mDrawRequest,
		M3dView &m3dView ) const;

	virtual bool select(
		MSelectInfo &MSelectInfo,
		MSelectionList &mSelectionList,
		MPointArray &worldSpaceSelectPts ) const;

protected:
	bool SelectComponents(
		MSelectInfo &mSelectInfo,
		MSelectionList &mSelectionList,
		MPointArray &worldSpaceSelectPts ) const;

	void DoDraw(
		const MDagPath &mDagPath,
		M3dView &m3dView,
		short drawToken,
		const MDrawRequest *pDrawRequest = NULL ) const;

	enum {
		kDrawBoundingBox,
		kDrawVertices,
		kDrawWireframe,
		kDrawWireframeOnShaded,
		kDrawSmoothShaded,
		kDrawFlatShaded,
		kLastToken
	};
};


//-----------------------------------------------------------------------------
// Constructor
//-----------------------------------------------------------------------------
CVstEyeballUI::CVstEyeballUI()
{
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
CVstEyeballUI::~CVstEyeballUI()
{
}


//-----------------------------------------------------------------------------
// Purpose: Draw the locator...
//-----------------------------------------------------------------------------
void CVstEyeballUI::getDrawRequests(
	const MDrawInfo &mDrawInfo,
	bool objectAndActiveOnly,
	MDrawRequestQueue &mDrawRequestQueue )
{
	// Object and component color defines
	//
#define LEAD_COLOR				18	// green
#define ACTIVE_COLOR			15	// white
#define ACTIVE_AFFECTED_COLOR	8	// purple
#define DORMANT_COLOR			4	// blue
#define HILITE_COLOR			17	// pale blue
#define DORMANT_VERTEX_COLOR	8	// purple
#define ACTIVE_VERTEX_COLOR		16	// yellow

	MDrawData mDrawData;
	CVstEyeballNode *pEyeball( reinterpret_cast< CVstEyeballNode * >( surfaceShape() ) );
	MDrawRequest mDrawRequest( mDrawInfo.getPrototype( *this ) );
	getDrawData( pEyeball, mDrawData );
	mDrawRequest.setDrawData( mDrawData );

	const M3dView::DisplayStyle  displayStyle( mDrawInfo.displayStyle() );
	const M3dView::DisplayStatus displayStatus( mDrawInfo.displayStatus() );

	switch ( displayStyle )
	{
	case M3dView::kBoundingBox:
		mDrawRequest.setToken( kDrawBoundingBox );
		mDrawRequestQueue.add( mDrawRequest );
		break;
	case M3dView::kWireFrame:
		{
			mDrawRequest.setToken( kDrawWireframe );

			const M3dView::ColorTable activeColorTable( M3dView::kActiveColors );
			const M3dView::ColorTable dormantColorTable( M3dView::kDormantColors );

			switch ( displayStatus )
			{
			case M3dView::kLead:
				mDrawRequest.setColor( LEAD_COLOR, activeColorTable );
				break;
			case M3dView::kActive:
				mDrawRequest.setColor( ACTIVE_COLOR, activeColorTable );
				break;
			case M3dView::kActiveAffected:
				mDrawRequest.setColor( ACTIVE_AFFECTED_COLOR, activeColorTable );
				break;
			case M3dView::kDormant:
				mDrawRequest.setColor( DORMANT_COLOR, dormantColorTable );
				break;
			case M3dView::kHilite:
				mDrawRequest.setColor( HILITE_COLOR, activeColorTable );
				break;
			default:
				break;
			}

			mDrawRequestQueue.add( mDrawRequest );
		}
		break;
	case M3dView::kGouraudShaded:
	case M3dView::kFlatShaded:
		{
			// Create the smooth shaded draw request
			//
			mDrawRequest.setToken( kDrawSmoothShaded );

			// Need to get the material info
			//
			MDagPath path = mDrawInfo.multiPath();	// path to your dag object 
			M3dView view = mDrawInfo.view();; 		// view to draw to
			MMaterial material = MPxSurfaceShapeUI::material( path );

			// Evaluate the material and if necessary, the texture.
			//
			if ( ! material.evaluateMaterial( view, path ) )
			{
				merr << "Couldn't evaluate material" << std::endl;
			}

			if ( material.materialIsTextured() )
			{
				material.evaluateTexture( mDrawData );
			}

			mDrawRequest.setMaterial( material );

			// request.setDisplayStyle( appearance );

			bool materialTransparent = false;
			
			if ( material.getHasTransparency( materialTransparent ) )
			{
				mDrawRequest.setIsTransparent( true );
			}

			mDrawRequestQueue.add( mDrawRequest );

			// create a draw request for wireframe on shaded if
			// necessary.
			//
			if ( displayStatus == M3dView::kActive ||
				displayStatus == M3dView::kLead ||
				displayStatus == M3dView::kHilite )
			{
				MDrawRequest wireRequest = mDrawInfo.getPrototype( *this );
				wireRequest.setDrawData( mDrawData );
				wireRequest.setToken( kDrawWireframeOnShaded );
				wireRequest.setDisplayStyle( M3dView::kWireFrame );

				M3dView::ColorTable activeColorTable = M3dView::kActiveColors;

				switch ( displayStatus )
				{
				case M3dView::kLead :
					wireRequest.setColor( LEAD_COLOR, activeColorTable );
					break;
				case M3dView::kActive :
					wireRequest.setColor( ACTIVE_COLOR, activeColorTable );
					break;
				case M3dView::kHilite :
					wireRequest.setColor( HILITE_COLOR, activeColorTable );
					break;
				default :	
					break;
				}

				mDrawRequestQueue.add( wireRequest );
			}
			break;
		}
	default:
		mDrawRequestQueue.add( mDrawRequest );
		break;
	}

	if ( !objectAndActiveOnly )
	{
		// Inactive Components
		if ( displayStyle == M3dView::kPoints || displayStatus == M3dView::kHilite )
		{
			MDrawRequest vertexRequest( mDrawInfo.getPrototype( *this ) );
			vertexRequest.setDrawData( mDrawData );
			vertexRequest.setToken( kDrawVertices );
			vertexRequest.setColor( DORMANT_VERTEX_COLOR, M3dView::kActiveColors );

			mDrawRequestQueue.add( vertexRequest );
		}

		// Active components
		if ( pEyeball->hasActiveComponents() )
		{
			MDrawRequest vertexRequest( mDrawInfo.getPrototype( *this ) );
			vertexRequest.setDrawData( mDrawData );
			vertexRequest.setToken( kDrawVertices );
			vertexRequest.setColor( ACTIVE_VERTEX_COLOR, M3dView::kActiveColors );

			MObjectArray cList( pEyeball->activeComponents() );
			MObject cObj( cList[ 0 ] );	// TODO: Filter List?
			vertexRequest.setComponent( cObj );

			mDrawRequestQueue.add( vertexRequest );
		}
	}
}


//-----------------------------------------------------------------------------
// Purpose: Draw the locator...
//-----------------------------------------------------------------------------
void CVstEyeballUI::draw(
	const MDrawRequest &mDrawRequest,
	M3dView &m3dView ) const
{
	m3dView.beginGL();

	DoDraw( mDrawRequest.multiPath(), m3dView, mDrawRequest.token(), &mDrawRequest );

	m3dView.endGL();
}


//-----------------------------------------------------------------------------
// Purpose: Draw the locator...
//-----------------------------------------------------------------------------
void CVstEyeballUI::DoDraw(
	const MDagPath &mDagPath,
	M3dView &m3dView,
	short drawToken,
	const MDrawRequest *pDrawRequest ) const
{
	CVstEyeballNode *pEyeball = reinterpret_cast< CVstEyeballNode * >( surfaceShape() );
	if ( !pEyeball )
		return;

	glPushAttrib( GL_POLYGON_BIT | GL_CURRENT_BIT );
	glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );

	glColor3f( 1.0f, 1.0f, 1.0f );
	pEyeball->m_eyeball.Draw( m3dView );

	glColor3f( 0.0f, 0.0f, 1.0f );
	pEyeball->m_iris.Draw( m3dView );

	glColor3f( 0.0f, 0.0f, 0.0f );
	pEyeball->m_pupil.Draw( m3dView );

	glPopAttrib();

#if 0
	const MMatrix inclusiveMatrixInverse( mDagPath.inclusiveMatrixInverse() );

	GLint matrixMode( GL_MODELVIEW );
	glGetIntegerv( GL_MATRIX_MODE, &matrixMode );
	glMatrixMode( GL_MODELVIEW );

	glPushMatrix();
	{
		glMultMatrixd( &inclusiveMatrixInverse.matrix[ 0 ][ 0 ] );

		bool turnOnLighting( false );

		if ( glIsEnabled( GL_LIGHTING ) )
		{
			turnOnLighting = true;
			glDisable( GL_LIGHTING );
		}

		if ( drawToken == kDrawBoundingBox )
		{
			CVstEyeballNode *pSkinner( reinterpret_cast< CVstEyeballNode * >( surfaceShape() ) );
			pSkinner->DrawBoundingBox();
		}
		else if ( drawToken == kDrawVertices )
		{
			DrawVolumes( m3dView, pDrawRequest );
		}
		else
		{
			DrawVolumes( m3dView );
		}

		if ( turnOnLighting )
		{
			glEnable( GL_LIGHTING );
		}
	}
	glPopMatrix();

	glMatrixMode( matrixMode );
#endif // 0
}


//-----------------------------------------------------------------------------
// Do user screen selection of the shape
//-----------------------------------------------------------------------------
bool CVstEyeballUI::select(
	MSelectInfo &mSelectInfo,
	MSelectionList &mSelectionList,
	MPointArray &worldSpaceSelectPts ) const
{
	return false;

#if 0
	bool retVal( false );

	// Make sure all of the data is computated
	CVstEyeballNode *pSkinner( reinterpret_cast< CVstEyeballNode * >( surfaceShape() ) );
	pSkinner->SyncGeometryWeights();

	M3dView m3dView( mSelectInfo.view() );

	if ( mSelectInfo.displayStatus() == M3dView::kHilite )
	{
		return SelectComponents( mSelectInfo, mSelectionList, worldSpaceSelectPts );
	}

	short drawToken( 0 );
	switch ( m3dView.displayStyle() ) {
	case M3dView::kBoundingBox:
		drawToken = kDrawBoundingBox;
		break;
	case M3dView::kWireFrame:
		drawToken = kDrawWireframe;
		break;
	case M3dView::kPoints:
		drawToken = kDrawBoundingBox;
		break;
	case M3dView::kGouraudShaded:
		drawToken = kDrawSmoothShaded;
		break;
	case M3dView::kFlatShaded:
		drawToken = kDrawFlatShaded;
		break;
	default:
		break;
	}

	uint selectBuf[ 4 ];

	m3dView.beginGL();

	m3dView.beginSelect( selectBuf, 4 );
	m3dView.initNames();
	m3dView.pushName( 0 );

	DoDraw( mSelectInfo.selectPath(), m3dView, drawToken );

	if ( m3dView.endSelect() > 0  )
	{
		retVal = true;

		MPoint wPoint(0.0, 0.0, 0.0, 1.0);

		if ( mSelectInfo.singleSelection() )
		{
			// Determine the depth of the selection:
			const double depth( static_cast< double >( selectBuf[ 1 ] ) / ( 65536.0 * 65536.0 - 1.0 ) );

			// Get the current GL Model View matrix:
			MMatrix mvmat;
			glGetDoublev(GL_MODELVIEW_MATRIX, &mvmat.matrix[ 0 ][ 0 ] );

			// Get the current GL Projection matrix:
			MMatrix pmat;
			glGetDoublev(GL_PROJECTION_MATRIX, &pmat.matrix[ 0 ][ 0 ] );

			// Determine where the NDC (Normalized Device Coordinates, Screen Space [ 0, 1 ]
			// point (0,0,depth) is in world space:
			const MMatrix ipmvmat( ( mvmat * pmat ).inverse() );
			wPoint = MPoint( ipmvmat[ 2 ] ) * ( depth + depth - 1.0 ) + MPoint( ipmvmat[ 3 ] );
		}

		MSelectionList mSelectionItem;
		mSelectionItem.add( mSelectInfo.selectPath() );
		mSelectInfo.addSelection( mSelectionItem, wPoint, mSelectionList, worldSpaceSelectPts, MSelectionMask::kSelectNurbsSurfaces, false );
	}

	m3dView.endGL();

	return retVal;
#endif // 0
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
bool CVstEyeballUI::SelectComponents(
	MSelectInfo &mSelectInfo,
	MSelectionList &mSelectionList,
	MPointArray &worldSpaceSelectPts ) const
{
	return false;

#if 0
	bool retVal( false );

	M3dView m3dView( mSelectInfo.view() );
	m3dView.beginGL();

	GLint matrixMode( GL_MODELVIEW );
	glGetIntegerv( GL_MATRIX_MODE, &matrixMode );
	glMatrixMode( GL_MODELVIEW );

	CVstEyeballNode *pSkinner( reinterpret_cast< CVstEyeballNode * >( surfaceShape() ) );
	MObject skinnerObj( pSkinner->thisMObject() );
	MPlug vAP( skinnerObj, CVstEyeballNode::m_iaVolume );
	MPlug wP;
	MPlugArray pA;

	MFnSingleIndexedComponent cFn;
	MObject cObj( cFn.create( MFn::kMeshVertComponent ) );

	MSelectionList sList;

	glPushMatrix();
	{
		const MMatrix inclusiveMatrixInverse( mSelectInfo.selectPath().inclusiveMatrixInverse() );
		glMultMatrixd( &inclusiveMatrixInverse.matrix[ 0 ][ 0 ] );

		const CUtlVector< CVstEyeballNode::VolumeData_s > &volumeData( pSkinner->m_volumeData );
		const int nVolume( volumeData.Count() );

		for ( int i = 0; i < nVolume; ++i )
		{
			const CVstEyeballNode::VolumeData_s &volumeData( volumeData[ i ] );

			m3dView.beginSelect();
			{
				glPushMatrix();
				glMultMatrixd( &volumeData.m_worldMatrix.matrix[ 0 ][ 0 ] );
				DrawUnitSphere();
				glPopMatrix();
			}

			if ( m3dView.endSelect() )
			{
				retVal = true;
				cFn.addElement( volumeData.m_volumeLogicalIndex );
				wP = vAP.elementByLogicalIndex( volumeData.m_volumeLogicalIndex ).child( CVstEyeballNode::m_iaVolumeWorldMatrix );
				if ( wP.connectedTo( pA, true, false ) && pA.length() )
				{
					MDagPath lDagPath;
					MDagPath::getAPathTo( pA[ 0 ].node(), lDagPath );
					if ( lDagPath.length() > 1U )
					{
						lDagPath.pop();
						sList.add( lDagPath, MObject::kNullObj, true );
					}
				}
			}
		}
	}
	glPopMatrix();

	glMatrixMode( matrixMode );

	m3dView.endGL();

	if ( retVal )
	{
		sList.add( mSelectInfo.selectPath(), cObj );
		mSelectInfo.addSelection( sList, MPoint::origin, mSelectionList, worldSpaceSelectPts, MSelectionMask( MSelectionMask::kSelectComponentsMask ), true );
	}

	return retVal;
#endif // 0
}
//======= Copyright © 1996-2006, Valve Corporation, All rights reserved. ======
//
// Purpose: Implementation of CVmfImport
//
//=============================================================================

// Standard includes
#include <io.h>
#include <vector>


// Maya includes
#include <maya/MAngle.h>
#include <maya/MCommandResult.h>
#include <maya/MEulerRotation.h>
#include <maya/MFnMatrixData.h>
#include <maya/MFnMesh.h>
#include <maya/MFnNumericAttribute.h>
#include <maya/MFnPartition.h>
#include <maya/MFnSet.h>
#include <maya/MFnSingleIndexedComponent.h>
#include <maya/MFnTransform.h>
#include <maya/MFnUnitAttribute.h>
#include <maya/MGlobal.h>
#include <maya/MItDependencyNodes.h>
#include <maya/MItMeshPolygon.h>
#include <maya/MPlug.h>
#include <maya/MPlugArray.h>
#include <maya/MSelectionList.h>
#include <maya/MMatrix.h>
#include <maya/MTransformationMatrix.h>
#include <maya/MQuaternion.h>
#include <maya/MVector.h>


// Valve includs
#include <filesystem.h>
#include <KeyValues.h>
#include <utlbuffer.h>


// Local includes
#include "VmfImport.h"


// Statics
CVmfImport *CVmfImport::m_pVmfImport = NULL;


//-----------------------------------------------------------------------------
// Constructor
//-----------------------------------------------------------------------------
CVmfImport::CVmfImport()
: m_pDagModifier( NULL )
, m_pUndoObjs( NULL )
{
	Clear();
}


//-----------------------------------------------------------------------------
// Destructor
//-----------------------------------------------------------------------------
CVmfImport::~CVmfImport()
{
	Clear();
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void
CVmfImport::Clear()
{
	m_optBakeUVs =			true;
	m_optImportTools =		false;
	m_optImportDev =		false;
	m_optImportBrushes =	true;
	m_optImportLights  =	true;
	m_optImportModels  =	true;
	m_optTextureArchive.clear();

	m_pObj = MObject::kNullObj;
	m_lightObj = MObject::kNullObj;
	m_brushObj = MObject::kNullObj;

	m_pDagModifier = NULL;
	m_pUndoObjs = NULL;

	m_pVmfImport = NULL;
}


//-----------------------------------------------------------------------------
// Main Entry Point
//-----------------------------------------------------------------------------
MStatus CVmfImport::DoIt(
	const MString &filename,
	MDagModifier *pDagModifier,
	MObjectArray *pUndoObjs )
{
	if ( m_pVmfImport )
	{
		merr << "Static VmfImport instance is not NULL, another import running?" <<
			std::endl;
		return MS::kFailure;
	}

	m_mComputation.beginComputation();
	m_mProgressWindow.reserve();
	m_mProgressWindow.setTitle( "Importing VMF: " + filename );
	m_mProgressWindow.setProgressStatus( "Importing VMF: " + filename );
	m_mProgressWindow.setProgressRange( 0, 100 );
	m_mProgressWindow.setInterruptable( true );
	m_mProgressWindow.setProgress( 0 );
	m_mProgressWindow.startProgress();

	m_pVmfImport = this;

	MDagModifier mDagModifier;
	m_pDagModifier = pDagModifier ? pDagModifier : &mDagModifier;
	MObjectArray undoObjs;
	m_pUndoObjs = pUndoObjs ? pUndoObjs : &undoObjs;

	MString nodeName;
	MGlobal::executeCommand( MString( "match \"[^\\\\/]*$\" \"" ) + filename + "\";", nodeName );
	if ( nodeName.length() == 0 )
	{
		nodeName = "vmfImport";
	}

	ValveMaya::CreateDagNode( "transform", nodeName.asChar(), MObject::kNullObj, &m_pObj, NULL, m_pDagModifier );
	MDagPath mDagPath;
	MDagPath::getAPathTo( m_pObj, mDagPath );

	ValveMaya::CreateDagNode( "transform", "lights", m_pObj, &m_lightObj, NULL, m_pDagModifier );

	ValveMaya::CreateDagNode( "transform", "brushes", m_pObj, &m_brushObj, NULL, m_pDagModifier );

	ValveMaya::CreateDagNode( "transform", "mdls", m_pObj, &m_mdlObj, NULL, m_pDagModifier );

	LoadMapFile( filename.asChar() );

	// Debug
	MFnTransform tFn( m_pObj );
	tFn.setRotation(
		MEulerRotation( MVector( -90.0, -90.0, 0.0 ) / 180.0 * M_PI ).asQuaternion(),
		MSpace::kTransform );

	m_pVmfImport = NULL;

	MGlobal::select( mDagPath, MObject::kNullObj, MGlobal::kReplaceList );

	m_mComputation.endComputation();
	m_mProgressWindow.setProgress( 100 );
	m_mProgressWindow.endProgress();

	MDagPath::getAPathTo( m_lightObj, mDagPath );
	if ( mDagPath.childCount() == 0 )
	{
		m_pDagModifier->deleteNode( m_lightObj );
		m_pDagModifier->doIt();
	}

	MDagPath::getAPathTo( m_brushObj, mDagPath );
	if ( mDagPath.childCount() == 0 )
	{
		m_pDagModifier->deleteNode( m_brushObj );
		m_pDagModifier->doIt();
	}

	MDagPath::getAPathTo( m_mdlObj, mDagPath );
	if ( mDagPath.childCount() == 0 )
	{
		m_pDagModifier->deleteNode( m_mdlObj );
		m_pDagModifier->doIt();
	}

	Clear();

	return MS::kSuccess;
}


//-----------------------------------------------------------------------------
// Callback from LoadMapFile every time a Side is handled
//-----------------------------------------------------------------------------
void CVmfImport::AddSideCallback(
	mapbrush_t *pMapBrush,
	side_t *pSide,
	const brush_texture_t &brushTexture,
	const Vector &pp0,
	const Vector &pp1,
	const Vector &pp2 )
{
	if ( !m_optImportBrushes )
		return;

	if ( m_mComputation.isInterruptRequested() )
		return;

	if ( m_mProgressWindow.isCancelled() )
		return;

	if ( !( m_solids.size() && m_solids.top().m_pMapBrush == pMapBrush ) )
	{
		m_solids.push( Solid_s( pMapBrush ) );
	}

	Assert( m_solids.size() );

	Solid_s &solid = m_solids.top();

	SideInfo_s &sideInfo = solid.m_sideMap[ pSide ];
	sideInfo.m_p0 = MFloatPoint( pp0.x, pp0.y, pp0.z );
	sideInfo.m_p1 = MFloatPoint( pp1.x, pp1.y, pp1.z );
	sideInfo.m_p2 = MFloatPoint( pp2.x, pp2.y, pp2.z );

	brush_texture_t &bt = sideInfo.m_brushTexture;
	bt = brushTexture;
	if ( Q_strlen( bt.name ) == 0 )
	{
		Q_strcpy( bt.name, "DEBUG/DEBUGEMPTY" );
	}
}


//-----------------------------------------------------------------------------
// Callback from LoadMapFile every time a Solid/Map Brush has been loaded
//-----------------------------------------------------------------------------
void CVmfImport::MapBrushToMayaCallback(
	mapbrush_t *pMapBrush )
{
	if ( !m_optImportBrushes )
		return;

	if ( m_mComputation.isInterruptRequested() )
		return;

	if ( m_mProgressWindow.isCancelled() )
		return;

	m_mProgressWindow.setProgressStatus( MString( "Converting Brush " ) + pMapBrush->id );

	Assert( m_solids.size() );
	if ( m_solids.empty() || m_solids.top().m_pMapBrush != pMapBrush )
	{
		merr << "Unseen brush processing requested, brush id: " << pMapBrush->id << std::endl;
		return;
	}

	SideMap_t &sideMap = m_solids.top().m_sideMap;
	ConvertBrush( pMapBrush, sideMap );

	m_solids.pop();
	return;
}


//-----------------------------------------------------------------------------
// Callback from LoadMapFile every time a model entity has been loaded
//-----------------------------------------------------------------------------
void CVmfImport::ImportModelCallback(
	const char *pModel,
	const char *pId,
	const char *pAngles,
	const char *pOrigin )
{
	if ( !m_optImportModels )
		return;

	if ( m_mComputation.isInterruptRequested() )
		return;

	if ( m_mProgressWindow.isCancelled() )
		return;

	m_mProgressWindow.setProgressStatus( MString( "Importing Model " ) + pModel );

	const std::map< std::string, MDagPath >::const_iterator mi( m_mdlCache.find( pModel ) );
	MString melCmd;

	if ( mi != m_mdlCache.end() )
	{
		melCmd += "duplicate -ilf \"";
		melCmd += mi->second.fullPathName();
		melCmd += "\"";
	}
	else
	{
		melCmd += "vstSmdIO -i -f \"";
		melCmd += pModel;
		melCmd += "\"";
		if ( m_optTextureArchive.length() )
		{
			melCmd += " -t \"";
			melCmd += m_optTextureArchive;
			melCmd += "\"";
		}
		melCmd += " -vmf";

		MVector o;
		sscanf( pOrigin, "%lf %lf %lf", &o.x, &o.y, &o.z );
		MVector a;
		sscanf( pAngles, "%lf %lf %lf", &a.x, &a.y, &a.z );

		melCmd += " -o ";
		melCmd += o.x;
		melCmd += " ";
		melCmd += o.y;
		melCmd += " ";
		melCmd += o.z;

		melCmd += " -a ";
		melCmd += a.x;
		melCmd += " ";
		melCmd += a.y;
		melCmd += " ";
		melCmd += a.z;
	}

	MCommandResult result;
	MGlobal::executeCommand( melCmd, result, true, true );

	MSelectionList resultList;
	if ( result.resultType() == MCommandResult::kString )
	{
		MString resultStr;
		result.getResult( resultStr );
		resultList.add( resultStr );
	}
	else if ( result.resultType() == MCommandResult::kStringArray )
	{
		MStringArray resultStrArray;
		result.getResult( resultStrArray );
		if ( resultStrArray.length() )
		{
			resultList.add( resultStrArray[ 0 ] );
		}
	}

	if ( resultList.length() )
	{
		MDagPath mDagPath;
		if ( resultList.getDagPath( 0, mDagPath ) )
		{
			m_pDagModifier->reparentNode( mDagPath.node(), m_mdlObj );
			m_pDagModifier->doIt();

			MFnTransform tFn( mDagPath );

			if ( mi == m_mdlCache.end() )
			{
				m_mdlCache[ pModel ] = mDagPath;
			}

			{
				tFn.setRotation( MEulerRotation( 0.0, 0.0, 0.0 ) );
				tFn.setTranslation( MVector::zero, MSpace::kObject );

				uint modelUp( 2U );
				MDagPath sDagPath( mDagPath );
				sDagPath.extendToShapeDirectlyBelow( 0 );
				MFnDagNode sFn( sDagPath );
				MPlug yUpP( MFnDagNode( sDagPath ).findPlug( "yUp" ) );
				if ( !yUpP.isNull() )
				{
					bool yUp( false );
					yUpP.getValue( yUp );
					if ( yUp )
					{
						modelUp = 1U;
					}
				}

				MVector o;
				if ( pOrigin && sscanf( pOrigin, "%lf %lf %lf", &o.x, &o.y, &o.z ) == 3 )
				{
					tFn.setTranslation( o, MSpace::kObject );
				}

				MVector a;
				if ( pAngles && sscanf( pAngles, "%lf %lf %lf", &a.y, &a.z, &a.x ) == 3 )
				{
					const MEulerRotation e( a.x / 180.0 * M_PI, a.y / 180.0 * M_PI, a.z / 180.0 * M_PI, MEulerRotation::kXYZ );
					tFn.setRotation( e );
				}

				if ( modelUp == 1U )
				{
					tFn.rotateBy( MEulerRotation( 90.0 / 180.0 * M_PI, 0.0, 90.0 / 180.0 * M_PI ).asQuaternion(), MSpace::kObject );
				}
				else
				{
					tFn.rotateBy( MEulerRotation( 0.0, 0.0, 90.0 / 180.0 * M_PI ).asQuaternion(), MSpace::kObject );
				}
			}

			m_pDagModifier->reparentNode( mDagPath.node(), m_mdlObj );
			m_pDagModifier->doIt();
		}
	}
}


//-----------------------------------------------------------------------------
// Callback from LoadMapFile every time a light entity has been loaded
//-----------------------------------------------------------------------------
void CVmfImport::ImportLightCallback(
	const char *pId,
	const char *pOrigin,
	const char *pLight,
	const char *pLightHDR,
	const char *pLightScaleHDR,
	const char *pQuadraticAttn )
{
	if ( !m_optImportLights )
		return;

	if ( m_mComputation.isInterruptRequested() )
		return;

	if ( m_mProgressWindow.isCancelled() )
		return;

	m_mProgressWindow.setProgressStatus( MString( "Processing Light " ) + pId );

	MObject tObj;
	MObject sObj;
	ValveMaya::CreateDagNode( "pointLight", ( MString( "light_" ) + pId ).asChar(), MObject::kNullObj, &tObj, &sObj, m_pDagModifier );
	m_pDagModifier->reparentNode( tObj, m_lightObj );
	m_pDagModifier->doIt();

	MDagPath tDagPath;
	MDagPath::getAPathTo( tObj, tDagPath );

	MDagPath sDagPath;
	MDagPath::getAPathTo( sObj, sDagPath );

	// origin
	{
		MFloatVector origin;
		if ( pOrigin && sscanf( pOrigin, "%f %f %f", &origin.x, &origin.y, &origin.z ) == 3 )
		{
			MFnTransform tFn( tObj );
			tFn.setTranslation( origin, MSpace::kTransform );
		}
	}

	// Set Shape Attributes

	MFnDependencyNode sFn( sObj );
	MFnNumericAttribute nFn;

	// _light
	{
		MColor _light;
		if ( pLight && sscanf( pLight, "%f %f %f %f", &_light.r, &_light.g, &_light.b, &_light.a ) == 4 )
		{

			sFn.findPlug( "colorR" ).setValue( _light.r / 255.0f );
			sFn.findPlug( "colorG" ).setValue( _light.g / 255.0f );
			sFn.findPlug( "colorB" ).setValue( _light.b / 255.0f );

			// _lightAlpha
			{
				MObject aObj = nFn.create( "_lightAlpha", "_lightAlpha", MFnNumericData::kFloat, 1.0 );
				nFn.setKeyable( true );
				m_pDagModifier->addAttribute( sObj, aObj );
				m_pDagModifier->doIt();

				MPlug( sObj, aObj ).setValue( _light.a / 255.0f );
			}
		}
	}

	// _lightHDR
	{
		MColor _lightHDR;
		if ( pLightHDR && sscanf( pLightHDR, "%f %f %f %f", &_lightHDR.r, &_lightHDR.g, &_lightHDR.b, &_lightHDR.a ) == 4 )
		{
			MObject aObj = nFn.createColor( "_lightHDR", "_lightHDR" );
			nFn.setKeyable( true );
			MFnNumericAttribute( nFn.child( 0 ) ).setKeyable( true );
			MFnNumericAttribute( nFn.child( 1 ) ).setKeyable( true );
			MFnNumericAttribute( nFn.child( 2 ) ).setKeyable( true );
			m_pDagModifier->addAttribute( sObj, aObj );
			m_pDagModifier->doIt();

			MPlug( aObj, nFn.child( 0 ) ).setValue( _lightHDR.r );
			MPlug( aObj, nFn.child( 1 ) ).setValue( _lightHDR.g );
			MPlug( aObj, nFn.child( 2 ) ).setValue( _lightHDR.b );

			// lightHDRAlpha
			{
				MObject aObj = nFn.create( "_lightHDRAlpha", "_lightHDRAlpha", MFnNumericData::kFloat, 1.0 );
				nFn.setKeyable( true );
				m_pDagModifier->addAttribute( sObj, aObj );
				m_pDagModifier->doIt();

				MPlug( sObj, aObj ).setValue( _lightHDR.a );
			}
		}
	}

	// _lightScaleHDR
	{
		float _lightScaleHDR( 1.0f );
		if ( pLightScaleHDR && sscanf( pLightScaleHDR, "%f", &_lightScaleHDR ) == 1 )
		{

			MObject aObj = nFn.create( "_lightscaleHDR", "_lightscaleHDR", MFnNumericData::kFloat, 1.0 );
			nFn.setKeyable( true );
			m_pDagModifier->addAttribute( sObj, aObj );
			m_pDagModifier->doIt();

			MPlug( sObj, aObj ).setValue( _lightScaleHDR );
		}
	}

	// _quadricAttn
	{
		float _quadraticAttn( 1.0f );
		if ( pQuadraticAttn && sscanf( pQuadraticAttn, "%f", &_quadraticAttn ) == 1 )
		{
			MObject aObj = nFn.create( "_quadraticAttn", "_quadraticAttn", MFnNumericData::kFloat, 1.0 );
			nFn.setKeyable( true );
			m_pDagModifier->addAttribute( sObj, aObj );
			m_pDagModifier->doIt();

			MPlug( sObj, aObj ).setValue( _quadraticAttn );
		}
	}
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void CVmfImport::ImportLightSpotCallback(
	const char *pId,
	const char *pOrigin,
	const char *pAngles,
	const char *pPitch,
	const char *pLight,
	const char *pLightHDR,
	const char *pLightScaleHDR,
	const char *pQuadraticAttn,
	const char *pInnerCone,
	const char *pCone,
	const char *pExponent )
{
	if ( !m_optImportLights )
		return;

	if ( m_mComputation.isInterruptRequested() )
		return;

	if ( m_mProgressWindow.isCancelled() )
		return;

	m_mProgressWindow.setProgressStatus( MString( "Processing Spot Light " ) + pId );

	MObject tObj;
	MObject sObj;
	ValveMaya::CreateDagNode( "spotLight", ( MString( "lightSpot_" ) + pId ).asChar(), MObject::kNullObj, &tObj, &sObj, m_pDagModifier );
	m_pDagModifier->reparentNode( tObj, m_lightObj );
	m_pDagModifier->doIt();

	MDagPath tDagPath;
	MDagPath::getAPathTo( tObj, tDagPath );

	MDagPath sDagPath;
	MDagPath::getAPathTo( sObj, sDagPath );

	// origin
	{
		MFloatVector origin;
		if ( pOrigin && sscanf( pOrigin, "%f %f %f", &origin.x, &origin.y, &origin.z ) == 3 )
		{
			MFnTransform tFn( tObj );
			tFn.setTranslation( origin, MSpace::kTransform );
		}
	}

	// angles
	// Come in a Yaw Pitch & Roll ( Y Z X )
	// Except that "pitch" in angles is ignored and it actually comes from pitch instead!  Hooray!
	{
		double junk, x, y, z;
		if ( pOrigin && sscanf( pAngles, "%lf %lf %lf", &junk, &z, &x ) == 3 && pPitch && sscanf( pPitch, "%lf", &y ) == 1 )
		{
			const MEulerRotation e( x / 180.0 * M_PI, -y / 180.0 * M_PI, z / 180.0 * M_PI, MEulerRotation::kYZX );
			MFnTransform tFn( tObj );
			tFn.setRotateOrientation( MQuaternion( -90.0 / 180.0 * M_PI, MVector( 0.0, 1.0, 0.0 ) ), MSpace::kTransform, true );
			tFn.setRotationOrder( MTransformationMatrix::kYZX, false );
			tFn.setRotation( e );
		}
	}

	// Set Shape Attributes

	MFnDependencyNode sFn( sObj );
	MFnNumericAttribute nFn;

	// _light
	{
		MColor _light;
		if ( pLight && sscanf( pLight, "%f %f %f %f", &_light.r, &_light.g, &_light.b, &_light.a ) == 4 )
		{

			sFn.findPlug( "colorR" ).setValue( _light.r / 255.0f );
			sFn.findPlug( "colorG" ).setValue( _light.g / 255.0f );
			sFn.findPlug( "colorB" ).setValue( _light.b / 255.0f );

			// _lightAlpha
			{
				MObject aObj = nFn.create( "_lightAlpha", "_lightAlpha", MFnNumericData::kFloat, 1.0 );
				nFn.setKeyable( true );
				m_pDagModifier->addAttribute( sObj, aObj );
				m_pDagModifier->doIt();

				MPlug( sObj, aObj ).setValue( _light.a / 255.0f );
			}
		}
	}

	// _lightHDR
	{
		MColor _lightHDR;
		if ( pLightHDR && sscanf( pLightHDR, "%f %f %f %f", &_lightHDR.r, &_lightHDR.g, &_lightHDR.b, &_lightHDR.a ) == 4 )
		{
			MObject aObj = nFn.createColor( "_lightHDR", "_lightHDR" );
			nFn.setKeyable( true );
			MFnNumericAttribute( nFn.child( 0 ) ).setKeyable( true );
			MFnNumericAttribute( nFn.child( 1 ) ).setKeyable( true );
			MFnNumericAttribute( nFn.child( 2 ) ).setKeyable( true );
			m_pDagModifier->addAttribute( sObj, aObj );
			m_pDagModifier->doIt();

			MPlug( aObj, nFn.child( 0 ) ).setValue( _lightHDR.r );
			MPlug( aObj, nFn.child( 1 ) ).setValue( _lightHDR.g );
			MPlug( aObj, nFn.child( 2 ) ).setValue( _lightHDR.b );

			// lightHDRAlpha
			{
				MObject aObj = nFn.create( "_lightHDRAlpha", "_lightHDRAlpha", MFnNumericData::kFloat, 1.0 );
				nFn.setKeyable( true );
				m_pDagModifier->addAttribute( sObj, aObj );
				m_pDagModifier->doIt();

				MPlug( sObj, aObj ).setValue( _lightHDR.a );
			}
		}
	}

	// _lightScaleHDR
	{
		float _lightScaleHDR( 1.0f );
		if ( pLightScaleHDR && sscanf( pLightScaleHDR, "%f", &_lightScaleHDR ) == 1 )
		{

			MObject aObj = nFn.create( "_lightscaleHDR", "_lightscaleHDR", MFnNumericData::kFloat, 1.0 );
			nFn.setKeyable( true );
			m_pDagModifier->addAttribute( sObj, aObj );
			m_pDagModifier->doIt();

			MPlug( sObj, aObj ).setValue( _lightScaleHDR );
		}
	}

	// _quadricAttn
	{
		float _quadraticAttn( 1.0f );
		if ( pQuadraticAttn && sscanf( pQuadraticAttn, "%f", &_quadraticAttn ) == 1 )
		{
			MObject aObj = nFn.create( "_quadraticAttn", "_quadraticAttn", MFnNumericData::kFloat, 1.0 );
			nFn.setKeyable( true );
			m_pDagModifier->addAttribute( sObj, aObj );
			m_pDagModifier->doIt();

			MPlug( sObj, aObj ).setValue( _quadraticAttn );
		}
	}

	// _cone
	{
		float _cone( 0.0f );
		if ( pCone && sscanf( pCone, "%f", &_cone ) == 1 )
		{
			sFn.findPlug( "coneAngle" ).setValue( _cone / 180.0 * M_PI );
		}
	}

	// _inner_cone
	{
		float _inner_cone( 0.0f );
		if ( pInnerCone && sscanf( pInnerCone, "%f", &_inner_cone ) == 1 )
		{
			sFn.findPlug( "penumbraAngle" ).setValue( _inner_cone / 180.0 * M_PI );
		}
	}

	// _exponent
	{
		float _exponent( 0.0f );
		if ( pExponent && sscanf( pExponent, "%f", &_exponent ) == 1 )
		{
			MObject aObj = nFn.create( "_exponent", "_exponent", MFnNumericData::kFloat, 1.0 );
			nFn.setKeyable( true );
			m_pDagModifier->addAttribute( sObj, aObj );
			m_pDagModifier->doIt();

			MPlug( sObj, aObj ).setValue( _exponent );
		}
	}
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void CVmfImport::ImportLightDynamicCallback(
	const char *pId,
	const char *pOrigin,
	const char *pAngles,
	const char *pPitch,
	const char *pLight,
	const char *pQuadraticAttn,
	const char *pInnerCone,
	const char *pCone,
	const char *pBrightness,
	const char *pDistance,
	const char *pSpotlightRadius )
{
	if ( !m_optImportLights )
		return;

	if ( m_mComputation.isInterruptRequested() )
		return;

	if ( m_mProgressWindow.isCancelled() )
		return;

	m_mProgressWindow.setProgressStatus( MString( "Processing Dynamic Light " ) + pId );

	MObject tObj;
	MObject sObj;
	ValveMaya::CreateDagNode( "spotLight", ( MString( "lightDynamic_" ) + pId ).asChar(), MObject::kNullObj, &tObj, &sObj, m_pDagModifier );
	m_pDagModifier->reparentNode( tObj, m_lightObj );
	m_pDagModifier->doIt();

	MDagPath tDagPath;
	MDagPath::getAPathTo( tObj, tDagPath );

	MDagPath sDagPath;
	MDagPath::getAPathTo( sObj, sDagPath );

	// origin
	{
		MFloatVector origin;
		if ( pOrigin && sscanf( pOrigin, "%f %f %f", &origin.x, &origin.y, &origin.z ) == 3 )
		{
			MFnTransform tFn( tObj );
			tFn.setTranslation( origin, MSpace::kTransform );
		}
	}

	// angles
	// Come in a Yaw Pitch & Roll ( Y Z X )
	// Except that "pitch" in angles is ignored and it actually comes from pitch instead!  Hooray!
	{
		double junk, x, y, z;
		if ( pOrigin && sscanf( pAngles, "%lf %lf %lf", &junk, &z, &x ) == 3 && pPitch && sscanf( pPitch, "%lf", &y ) == 1 )
		{
			const MEulerRotation e( x / 180.0 * M_PI, -y / 180.0 * M_PI, z / 180.0 * M_PI, MEulerRotation::kYZX );
			MFnTransform tFn( tObj );
			tFn.setRotateOrientation( MQuaternion( -90.0 / 180.0 * M_PI, MVector( 0.0, 1.0, 0.0 ) ), MSpace::kTransform, true );
			tFn.setRotationOrder( MTransformationMatrix::kYZX, false );
			tFn.setRotation( e );
		}
	}

	// Set Shape Attributes

	MFnDependencyNode sFn( sObj );
	MFnNumericAttribute nFn;

	// _light
	{
		MColor _light;
		if ( pLight && sscanf( pLight, "%f %f %f %f", &_light.r, &_light.g, &_light.b, &_light.a ) == 4 )
		{

			sFn.findPlug( "colorR" ).setValue( _light.r / 255.0f );
			sFn.findPlug( "colorG" ).setValue( _light.g / 255.0f );
			sFn.findPlug( "colorB" ).setValue( _light.b / 255.0f );

			// _lightAlpha
			{
				MObject aObj = nFn.create( "_lightAlpha", "_lightAlpha", MFnNumericData::kFloat, 1.0 );
				nFn.setKeyable( true );
				m_pDagModifier->addAttribute( sObj, aObj );
				m_pDagModifier->doIt();

				MPlug( sObj, aObj ).setValue( _light.a / 255.0f );
			}
		}
	}

	// _quadricAttn
	{
		float _quadraticAttn( 1.0f );
		if ( pQuadraticAttn && sscanf( pQuadraticAttn, "%f", &_quadraticAttn ) == 1 )
		{
			MObject aObj = nFn.create( "_quadraticAttn", "_quadraticAttn", MFnNumericData::kFloat, 1.0 );
			nFn.setKeyable( true );
			m_pDagModifier->addAttribute( sObj, aObj );
			m_pDagModifier->doIt();

			MPlug( sObj, aObj ).setValue( _quadraticAttn );
		}
	}

	// _cone
	{
		float _cone( 0.0f );
		if ( pCone && sscanf( pCone, "%f", &_cone ) == 1 )
		{
			sFn.findPlug( "coneAngle" ).setValue( _cone / 180.0 * M_PI );
		}
	}

	// _inner_cone
	{
		float _inner_cone( 0.0f );
		if ( pInnerCone && sscanf( pInnerCone, "%f", &_inner_cone ) == 1 )
		{
			sFn.findPlug( "penumbraAngle" ).setValue( _inner_cone / 180.0 * M_PI );
		}
	}

	// brightness
	{
		float brightness( 0.0f );
		if ( pBrightness && sscanf( pBrightness, "%f", &brightness ) == 1 )
		{
			MObject aObj = nFn.create( "brightness", "brightness", MFnNumericData::kFloat, 1.0 );
			nFn.setKeyable( true );
			m_pDagModifier->addAttribute( sObj, aObj );
			m_pDagModifier->doIt();

			MPlug( sObj, aObj ).setValue( brightness );
		}
	}

	// distance
	{
		float distance( 0.0f );
		if ( pDistance && sscanf( pDistance, "%f", &distance ) == 1 )
		{
			MObject aObj = nFn.create( "distance", "distance", MFnNumericData::kFloat, 1.0 );
			nFn.setKeyable( true );
			m_pDagModifier->addAttribute( sObj, aObj );
			m_pDagModifier->doIt();

			MPlug( sObj, aObj ).setValue( distance );
		}
	}

	// spotlightRadius
	{
		float spotlightRadius( 0.0f );
		if ( pSpotlightRadius && sscanf( pSpotlightRadius, "%f", &spotlightRadius ) == 1 )
		{
			MObject aObj = nFn.create( "spotlightRadius", "spotlightRadius", MFnNumericData::kFloat, 1.0 );
			nFn.setKeyable( true );
			m_pDagModifier->addAttribute( sObj, aObj );
			m_pDagModifier->doIt();

			MPlug( sObj, aObj ).setValue( spotlightRadius );
		}
	}
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void CVmfImport::ImportLightEnvironmentCallback(
	const char *pId,
	const char *pOrigin,
	const char *pAngles,
	const char *pPitch,
	const char *pLight,
	const char *pLightHDR,
	const char *pLightScaleHDR,
	const char *pAmbient,
	const char *pAmbientHDR,
	const char *pAmbientScaleHDR,
	const char *pSunSpreadAngle )
{
	if ( !m_optImportLights )
		return;

	if ( m_mComputation.isInterruptRequested() )
		return;

	if ( m_mProgressWindow.isCancelled() )
		return;

	m_mProgressWindow.setProgressStatus( MString( "Processing Environment Light " ) + pId );

	MObject tObj;
	MObject sObj;
	ValveMaya::CreateDagNode( "volumeLight", ( MString( "lightEnvironment_" ) + pId ).asChar(), MObject::kNullObj, &tObj, &sObj, m_pDagModifier );
	m_pDagModifier->reparentNode( tObj, m_lightObj );
	m_pDagModifier->doIt();

	MDagPath tDagPath;
	MDagPath::getAPathTo( tObj, tDagPath );

	MDagPath sDagPath;
	MDagPath::getAPathTo( sObj, sDagPath );

	// origin
	{
		MFloatVector origin;
		if ( pOrigin && sscanf( pOrigin, "%f %f %f", &origin.x, &origin.y, &origin.z ) == 3 )
		{
			MFnTransform tFn( tObj );
			tFn.setTranslation( origin, MSpace::kTransform );
		}
	}

	// angles
	// Come in a Yaw Pitch & Roll ( Y Z X )
	// Except that "pitch" in angles is ignored and it actually comes from pitch instead!  Hooray!
	{
		double junk, x, y, z;
		if ( pOrigin && sscanf( pAngles, "%lf %lf %lf", &junk, &z, &x ) == 3 && pPitch && sscanf( pPitch, "%lf", &y ) == 1 )
		{
			const MEulerRotation e( x / 180.0 * M_PI, -y / 180.0 * M_PI, z / 180.0 * M_PI, MEulerRotation::kYZX );
			MFnTransform tFn( tObj );
			tFn.setRotateOrientation( MQuaternion( -90.0 / 180.0 * M_PI, MVector( 0.0, 1.0, 0.0 ) ), MSpace::kTransform, true );
			tFn.setRotationOrder( MTransformationMatrix::kYZX, false );
			tFn.setRotation( e );
		}
	}

	// Set Shape Attributes

	MFnDependencyNode sFn( sObj );
	MFnNumericAttribute nFn;

	// _light
	{
		MColor _light;
		if ( pLight && sscanf( pLight, "%f %f %f %f", &_light.r, &_light.g, &_light.b, &_light.a ) == 4 )
		{

			sFn.findPlug( "colorR" ).setValue( _light.r / 255.0f );
			sFn.findPlug( "colorG" ).setValue( _light.g / 255.0f );
			sFn.findPlug( "colorB" ).setValue( _light.b / 255.0f );

			// _lightAlpha
			{
				MObject aObj = nFn.create( "_lightAlpha", "_lightAlpha", MFnNumericData::kFloat, 1.0 );
				nFn.setKeyable( true );
				m_pDagModifier->addAttribute( sObj, aObj );
				m_pDagModifier->doIt();

				MPlug( sObj, aObj ).setValue( _light.a / 255.0f );
			}
		}
	}

	// _lightHDR
	{
		MColor _lightHDR;
		if ( pLightHDR && sscanf( pLightHDR, "%f %f %f %f", &_lightHDR.r, &_lightHDR.g, &_lightHDR.b, &_lightHDR.a ) == 4 )
		{
			MObject aObj = nFn.createColor( "_lightHDR", "_lightHDR" );
			nFn.setKeyable( true );
			MFnNumericAttribute( nFn.child( 0 ) ).setKeyable( true );
			MFnNumericAttribute( nFn.child( 1 ) ).setKeyable( true );
			MFnNumericAttribute( nFn.child( 2 ) ).setKeyable( true );
			m_pDagModifier->addAttribute( sObj, aObj );
			m_pDagModifier->doIt();

			MPlug( aObj, nFn.child( 0 ) ).setValue( _lightHDR.r );
			MPlug( aObj, nFn.child( 1 ) ).setValue( _lightHDR.g );
			MPlug( aObj, nFn.child( 2 ) ).setValue( _lightHDR.b );

			// lightHDRAlpha
			{
				MObject aObj = nFn.create( "_lightHDRAlpha", "_lightHDRAlpha", MFnNumericData::kFloat, 1.0 );
				nFn.setKeyable( true );
				m_pDagModifier->addAttribute( sObj, aObj );
				m_pDagModifier->doIt();

				MPlug( sObj, aObj ).setValue( _lightHDR.a );
			}
		}
	}

	// _lightScaleHDR
	{
		float _lightScaleHDR( 1.0f );
		if ( pLightScaleHDR && sscanf( pLightScaleHDR, "%f", &_lightScaleHDR ) == 1 )
		{

			MObject aObj = nFn.create( "_lightscaleHDR", "_lightscaleHDR", MFnNumericData::kFloat, 1.0 );
			nFn.setKeyable( true );
			m_pDagModifier->addAttribute( sObj, aObj );
			m_pDagModifier->doIt();

			MPlug( sObj, aObj ).setValue( _lightScaleHDR );
		}
	}

	// _ambient
	{
		MColor _ambient;
		if ( pAmbient && sscanf( pAmbient, "%f %f %f %f", &_ambient.r, &_ambient.g, &_ambient.b, &_ambient.a ) == 4 )
		{
			MObject aObj = nFn.createColor( "_ambient", "_ambient" );
			nFn.setKeyable( true );
			MFnNumericAttribute( nFn.child( 0 ) ).setKeyable( true );
			MFnNumericAttribute( nFn.child( 1 ) ).setKeyable( true );
			MFnNumericAttribute( nFn.child( 2 ) ).setKeyable( true );
			m_pDagModifier->addAttribute( sObj, aObj );
			m_pDagModifier->doIt();

			MPlug( aObj, nFn.child( 0 ) ).setValue( _ambient.r );
			MPlug( aObj, nFn.child( 1 ) ).setValue( _ambient.g );
			MPlug( aObj, nFn.child( 2 ) ).setValue( _ambient.b );

			// _ambientAlpha
			{
				MObject aObj = nFn.create( "_ambientAlpha", "_ambientAlpha", MFnNumericData::kFloat, 1.0 );
				nFn.setKeyable( true );
				m_pDagModifier->addAttribute( sObj, aObj );
				m_pDagModifier->doIt();

				MPlug( sObj, aObj ).setValue( _ambient.a / 255.0f );
			}
		}
	}

	// _ambientHDR
	{
		MColor _ambientHDR;
		if ( pAmbientHDR && sscanf( pAmbientHDR, "%f %f %f %f", &_ambientHDR.r, &_ambientHDR.g, &_ambientHDR.b, &_ambientHDR.a ) == 4 )
		{
			MObject aObj = nFn.createColor( "_ambientHDR", "_ambientHDR" );
			nFn.setKeyable( true );
			MFnNumericAttribute( nFn.child( 0 ) ).setKeyable( true );
			MFnNumericAttribute( nFn.child( 1 ) ).setKeyable( true );
			MFnNumericAttribute( nFn.child( 2 ) ).setKeyable( true );
			m_pDagModifier->addAttribute( sObj, aObj );
			m_pDagModifier->doIt();

			MPlug( aObj, nFn.child( 0 ) ).setValue( _ambientHDR.r );
			MPlug( aObj, nFn.child( 1 ) ).setValue( _ambientHDR.g );
			MPlug( aObj, nFn.child( 2 ) ).setValue( _ambientHDR.b );

			// ambientHDRAlpha
			{
				MObject aObj = nFn.create( "_ambientHDRAlpha", "_ambientHDRAlpha", MFnNumericData::kFloat, 1.0 );
				nFn.setKeyable( true );
				m_pDagModifier->addAttribute( sObj, aObj );
				m_pDagModifier->doIt();

				MPlug( sObj, aObj ).setValue( _ambientHDR.a );
			}
		}
	}

	// _ambientScaleHDR
	{
		float _ambientScaleHDR( 1.0f );
		if ( pAmbientScaleHDR && sscanf( pAmbientScaleHDR, "%f", &_ambientScaleHDR ) == 1 )
		{

			MObject aObj = nFn.create( "_ambientscaleHDR", "_ambientscaleHDR", MFnNumericData::kFloat, 1.0 );
			nFn.setKeyable( true );
			m_pDagModifier->addAttribute( sObj, aObj );
			m_pDagModifier->doIt();

			MPlug( sObj, aObj ).setValue( _ambientScaleHDR );
		}
	}

	// SunSpreadAngle
	{
		float sunSpreadAngle( 0.0f );
		if ( pSunSpreadAngle && sscanf( pSunSpreadAngle, "%f", &sunSpreadAngle ) == 1 )
		{
			MFnUnitAttribute uFn;
			MObject aObj = uFn.create( "sunSpreadAngle", "sunSpreadAngle", MAngle( 0.0 ) );
			nFn.setKeyable( true );
			m_pDagModifier->addAttribute( sObj, aObj );
			m_pDagModifier->doIt();

			MPlug( sObj, aObj ).setValue( sunSpreadAngle / 180.0 * M_PI );
		}
	}
}


//-----------------------------------------------------------------------------
// Gets the counts for number of polygons, vertices & faceVertices in a brush
//-----------------------------------------------------------------------------
bool
CVmfImport::GetBrushCounts(
	const mapbrush_t *pMapBrush,
	SideMap_t &sideMap,
	uint &nPolygons,
	uint &nVertices,
	uint &nFaceVertices ) const
{
	bool hasDispInfo( false );

	for ( int i = 0; i < pMapBrush->numsides; ++i )
	{
		const side_t *const pSide = pMapBrush->original_sides + i;
		if ( !pSide )
			continue;

		if ( pSide->pMapDisp )
		{
			hasDispInfo = true;
			break;
		}
	}

	nPolygons = 0;
	nVertices = 0;
	nFaceVertices = 0;

	for ( int i = 0; i < pMapBrush->numsides; ++i )
	{
		const side_t *const pSide = pMapBrush->original_sides + i;
		if ( !pSide )
			continue;

		if ( sideMap.find( pSide ) == sideMap.end() )
			continue;

		SideInfo_s &sideInfo = sideMap[ pSide ];

		const winding_t *const pWinding = pSide->winding;
		if ( !pWinding )
			continue;

		uint tmpNPolygons( 0U );
		uint tmpNVertices( 0U );
		uint tmpNFaceVertices( 0U );

		GetSideCounts( hasDispInfo, pSide, tmpNPolygons, tmpNVertices, tmpNFaceVertices );

		if ( SkipSide( sideInfo ) )
			continue;

		nPolygons += tmpNPolygons;
		nVertices += tmpNVertices;
		nFaceVertices += tmpNFaceVertices;
	}

	return hasDispInfo;
}


//-----------------------------------------------------------------------------
// Gets the counts for number of polygons, vertices & faceVertices in a side
//-----------------------------------------------------------------------------
void
CVmfImport::GetSideCounts(
	bool dispOnly,
	const side_t *pSide,
	uint &nPolygons,
	uint &nVertices,
	uint &nFaceVertices )
{
	nPolygons = 0;
	nVertices = 0;
	nFaceVertices = 0;

	if ( !pSide )
		return;

	if ( pSide->pMapDisp )
	{
		// power = 2 ^ power + 1;
		const uint power = ( 1 << pSide->pMapDisp->power ) + 1;
		assert( power == pow( 2.f, static_cast< int >( pSide->pMapDisp->power ) ) + 1 );
		if ( power )
		{
			nPolygons = ( ( power - 1 ) * ( power - 1 ) );
			nVertices = power * power;
			nFaceVertices = nPolygons * 4;

			return;
		}
	}

	if ( dispOnly )
		return;

	const winding_t *const pWinding = pSide->winding;
	if ( !pWinding )
		return;

	nPolygons = 1;
	nVertices = pWinding->numpoints;
	nFaceVertices = pWinding->numpoints;
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void CVmfImport::GetMeshDispData(
	const SideInfo_s &sideInfo,
	const winding_t *pWinding,
	const mapdispinfo_t *pDispInfo,
	MFloatPointArray &vertexArray,
	MIntArray &polygonCounts,
	MIntArray &polygonConnects,
	MFloatArray &uArray,
	MFloatArray &vArray )
{
	int vertex_winding_indices[4] = { 0, 1, 2, 3 };
	{
		for ( int ff = 0; ff < pWinding->numpoints; ++ff )
		{
			float diff1 = fabs( pWinding->p[ff][0] - pDispInfo->startPosition[0] );
			float diff2 = fabs( pWinding->p[ff][1] - pDispInfo->startPosition[1] );
			float diff3 = fabs( pWinding->p[ff][2] - pDispInfo->startPosition[2] );

			if ( ( diff1 < 0.001f ) && ( diff2 < 0.001f ) && ( diff3 < 0.001f ) )
			{
				vertex_winding_indices[0] = ff;
				vertex_winding_indices[1] = ff + 1;
				vertex_winding_indices[2] = ff + 2;
				vertex_winding_indices[3] = ff + 3;
				break;
			}
		}

		for ( int vv = 1; vv < 4; vv++ )
		{
			if ( vertex_winding_indices[vv] > 3 )
			{
				vertex_winding_indices[vv] -= 4;
			}
		}
	}

	const uint power = ( 1 << pDispInfo->power ) + 1;
	const float step = 1.0f / static_cast< float >( power - 1 );

	MFloatPointArray l_pWestVectors( power );
	{
		const MFloatVector tmpAxis( (
			MFloatPoint(
			pWinding->p[ vertex_winding_indices[1] ][0],
			pWinding->p[ vertex_winding_indices[1] ][1],
			pWinding->p[ vertex_winding_indices[1] ][2] ) -
			MFloatPoint(
			pWinding->p[ vertex_winding_indices[0] ][0],
			pWinding->p[ vertex_winding_indices[0] ][1],
			pWinding->p[ vertex_winding_indices[0] ][2] ) ) * step );

		for ( uint i = 0; i != power; ++i )
		{
			l_pWestVectors[ i ] = MFloatPoint(
				pWinding->p[ vertex_winding_indices[0] ][0],
				pWinding->p[ vertex_winding_indices[0] ][1],
				pWinding->p[ vertex_winding_indices[0] ][2] ) +
				( tmpAxis * static_cast< float >( i ) );
		}
	}

	MFloatPointArray l_pEastVectors( power );
	{
		const MFloatVector tmpAxis( (
			MFloatPoint(
			pWinding->p[vertex_winding_indices[2]][0],
			pWinding->p[vertex_winding_indices[2]][1],
			pWinding->p[vertex_winding_indices[2]][2] ) -
			MFloatPoint(
			pWinding->p[vertex_winding_indices[3]][0],
			pWinding->p[vertex_winding_indices[3]][1],
			pWinding->p[vertex_winding_indices[3]][2] ) ) * step );

		for ( uint i = 0; i != power; ++i )
		{
			l_pEastVectors[ i ] = MFloatPoint(
				pWinding->p[vertex_winding_indices[3]][0],
				pWinding->p[vertex_winding_indices[3]][1],
				pWinding->p[vertex_winding_indices[3]][2] ) +
				( tmpAxis * static_cast< float >( i ) );
		}
	}

	const uint nVertices( power * power );
	vertexArray.setLength( nVertices );
	uArray.setLength( nVertices );
	vArray.setLength( nVertices );

	{
		MFloatVector tmpAxis;
		uint vIndex( 0U );
		for ( uint i = 0; i != power; ++i )
		{
			tmpAxis = ( l_pEastVectors[ i ] - l_pWestVectors[ i ] ) * step;
			for ( uint j = 0; j != power; ++j, ++vIndex )
			{
				vertexArray[ vIndex ] = l_pWestVectors[ i ] + ( tmpAxis * static_cast< float >( j ) );
			}
		}
	}

	const MFloatPoint &p0 = sideInfo.m_p0;
	const MFloatPoint &p1 = sideInfo.m_p1;
	const MFloatPoint &p2 = sideInfo.m_p2;

	const MFloatVector uVector( p2 - p1 );
	const double uVectorLength( uVector.length() );
	const MFloatVector uUnitVector( uVector.normal() );

	const MFloatVector vVector( p1 - p0 );
	const double vVectorLength( vVector.length() );
	const MFloatVector vUnitVector( vVector.normal() );

	const MFloatVector planeNormal = ( uVector ^ vVector ).normal();

	for ( int v = 0; v < nVertices; ++v )
	{
		const float distance = pDispInfo->dispDists[v];

		const MFloatVector normal(
			pDispInfo->vectorDisps[ v ][ 0 ],
			pDispInfo->vectorDisps[ v ][ 1 ],
			pDispInfo->vectorDisps[ v ][ 2 ] );

		const MFloatVector offset(
			pDispInfo->vectorOffsets[ v ][ 0 ],
			pDispInfo->vectorOffsets[ v ][ 1 ],
			pDispInfo->vectorOffsets[ v ][ 2 ] );

		vertexArray[v] = vertexArray[v] + planeNormal * pDispInfo->m_elevation + offset + normal * distance;

		uArray[ v ] = ( uUnitVector * ( vertexArray[ v ] - p1 ) ) / uVectorLength;
		vArray[ v ] = ( vUnitVector * ( vertexArray[ v ] - p0 ) ) / vVectorLength;
	}

	const int nPolygons( ( power - 1 ) * ( power - 1 ) );
	polygonCounts.setLength( nPolygons );
	polygonConnects.setLength( nPolygons * 4 );

	{
		int polystart = 0;
		int numpoly = 0;

		int vIndex = 0;
		for ( int pIndex = 0; pIndex < nPolygons; ++pIndex, vIndex += 4 )
		{
			polygonCounts[ pIndex ] = 4;

			polygonConnects[ vIndex ] = polystart;
			polygonConnects[ vIndex + 1 ] = polystart + 1;
			polygonConnects[ vIndex + 2 ] = polystart + power + 1;
			polygonConnects[ vIndex + 3 ] = polystart + power;

			++numpoly;
			if ( numpoly == ( power - 1 ) )
			{
				polystart += 2;
				numpoly = 0;
			}
			else
			{
				++polystart;
			}
		}
	}
}


//-----------------------------------------------------------------------------
// Get the mesh vertices, topology and normalized UVs
//-----------------------------------------------------------------------------
void CVmfImport::GetMeshWindingData(
	const SideInfo_s &sideInfo,
	const winding_t *pWinding,
	MFloatPointArray &vertexArray,
	MIntArray &polygonCounts,
	MIntArray &polygonConnects,
	MFloatArray &uArray,
	MFloatArray &vArray )
{
	const MFloatPoint &p0 = sideInfo.m_p0;
	const MFloatPoint &p1 = sideInfo.m_p1;
	const MFloatPoint &p2 = sideInfo.m_p2;

	const MFloatVector uVector( p2 - p1 );
	const double uVectorLength( uVector.length() );
	const MFloatVector uUnitVector( uVector.normal() );

	const MFloatVector vVector( p1 - p0 );
	const double vVectorLength( vVector.length() );
	const MFloatVector vUnitVector( vVector.normal() );

	const uint nVertices = static_cast< uint >( max( 0, pWinding->numpoints ) );
	vertexArray.setLength( nVertices );
	polygonCounts.setLength( 1 );
	polygonCounts[ 0 ] = nVertices;
	polygonConnects.setLength( nVertices );
	uArray.setLength( nVertices );
	vArray.setLength( nVertices );

	uint j;
	for ( uint i = 0; i != nVertices; ++i )
	{
		j = nVertices - i - 1U;
		const MFloatPoint p( pWinding->p[ i ][ 0 ], pWinding->p[ i ][ 1 ], pWinding->p[ i ][ 2 ] );
		vertexArray[ j ] = p;
		polygonConnects[ j ] = j;
		uArray[ j ] = ( uUnitVector * ( p - p1 ) ) / uVectorLength;
		vArray[ j ] = ( vUnitVector * ( p - p0 ) ) / vVectorLength;
	}
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void CVmfImport::ConvertBrush(
	const mapbrush_t *pMapBrush,
	SideMap_t &sideMap )
{
	uint nPolygons( 0U );
	uint nVertices( 0U );
	uint nFaceVertices( 0U );

	const bool hasDispInfo = GetBrushCounts( pMapBrush, sideMap, nPolygons, nVertices, nFaceVertices );

	MFloatPointArray vertexArray( nVertices );
	MIntArray polygonCounts( nPolygons );
	MIntArray polygonConnects( nFaceVertices );
	MFloatArray uArray( nVertices );
	MFloatArray vArray( nVertices );
	MFloatArray luArray( nVertices );
	MFloatArray lvArray( nVertices );

	uint vIndex( 0U );
	uint pIndex( 0U );
	uint fvIndex( 0U );

	const int nSides = pMapBrush->numsides;

	std::vector< MIntArray > faceSets( nSides );

	for ( int i = 0; i < nSides; ++i )
	{
		side_t *pSide = pMapBrush->original_sides + i;
		if ( !pSide )
			continue;

		winding_t *pWinding = pSide->winding;
		if ( !pWinding )
			continue;

		const SideInfo_s &sideInfo( sideMap[ pSide ] );

		if ( SkipSide( sideInfo ) )
			continue;

		MFloatPointArray tmpVertexArray;
		MIntArray tmpPolygonCounts;
		MIntArray tmpPolygonConnects;
		MFloatArray tmpUArray;
		MFloatArray tmpVArray;

		MIntArray &cIndices( faceSets[ i ] );

		if ( pSide->pMapDisp )
		{
			GetMeshDispData( sideInfo, pWinding, pSide->pMapDisp, tmpVertexArray, tmpPolygonCounts, tmpPolygonConnects, tmpUArray, tmpVArray );
		}
		else if ( hasDispInfo )
		{
			continue;
		}
		else
		{
			GetMeshWindingData( sideInfo, pWinding, tmpVertexArray, tmpPolygonCounts, tmpPolygonConnects, tmpUArray, tmpVArray );
		}

		Assert( vertexArray.length() >= vIndex + tmpVertexArray.length() );
		Assert( tmpUArray.length() == tmpVArray.length() );
		Assert( tmpVertexArray.length() == tmpUArray.length() );
		Assert( uArray.length() >= vIndex + tmpUArray.length() );
		Assert( vArray.length() >= vIndex + tmpVArray.length() );
		const uint vOffset( vIndex );
		for ( int j = 0; j < tmpVertexArray.length(); ++j, ++vIndex )
		{
			vertexArray[ vIndex ] = tmpVertexArray[ j ];
			uArray[ vIndex ] = tmpUArray[ j ];
			vArray[ vIndex ] = tmpVArray[ j ];
		}

		Assert( polygonCounts.length() >= pIndex + tmpPolygonCounts.length() );
		for ( int j = 0; j < tmpPolygonCounts.length(); ++j, ++pIndex )
		{
			polygonCounts[ pIndex ] = tmpPolygonCounts[ j ];
			cIndices.append( pIndex );
		}

		Assert( polygonConnects.length() >= fvIndex + tmpPolygonConnects.length() );
		for ( int j = 0; j < tmpPolygonConnects.length(); ++j, ++fvIndex )
		{
			polygonConnects[ fvIndex ] = vOffset + tmpPolygonConnects[ j ];
		}
	}

	// Create the mesh in Maya 
	MFnMesh meshFn;
	MObject mObj = meshFn.create(
		vertexArray.length(), polygonCounts.length(),
		vertexArray, polygonCounts, polygonConnects );

	if ( !mObj.isNull() )
	{
		m_pUndoObjs->append( mObj );
	}

	meshFn.clearUVs();
	meshFn.setUVs( uArray, vArray );
	meshFn.assignUVs( polygonCounts, polygonConnects );

	MDagPath mDagPath;
	MDagPath::getAPathTo( mObj, mDagPath );

	MString name = MString( "brush_" ) + pMapBrush->id;
	m_pDagModifier->renameNode( mObj, name );
	m_pDagModifier->doIt();

	m_pDagModifier->reparentNode( mObj, m_brushObj );
	m_pDagModifier->doIt();

	Assert( faceSets.size() == nSides );
	for ( int i = 0; i < nSides; ++i )
	{
		side_t *pSide = pMapBrush->original_sides + i;
		if ( !pSide )
			continue;

		if ( hasDispInfo && !pSide->pMapDisp )
			continue;

		winding_t *pWinding = pSide->winding;
		if ( !pWinding )
			continue;

		const SideInfo_s &sideInfo( sideMap[ pSide ] );

		if ( SkipSide( sideInfo ) )
			continue;

		AssignMaterial( mDagPath, faceSets[ i ], pMapBrush, m_solids.top().m_sideMap[ pSide ].m_brushTexture, uArray, vArray, luArray, lvArray );
	}

	if ( m_optBakeUVs )
	{
		meshFn.setUVs( uArray, vArray );

		MString lightMapUvSet( "lightMap" );
		meshFn.createUVSet( lightMapUvSet );
		meshFn.clearUVs( &lightMapUvSet );
		meshFn.setUVs( luArray, lvArray, &lightMapUvSet );
		meshFn.assignUVs( polygonCounts, polygonConnects, &lightMapUvSet );
	}
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void CVmfImport::GetTexturePathsFromMaterialPath(
	const MString &materialPath,
	MString &baseTexturePath,
	MString &bumpMapPath )
{

	baseTexturePath.clear();
	bumpMapPath.clear();

	MString mat = materialPath;
	mat = "materials/" + mat;
	mat += ".vmt";
	char localPath[ MAX_PATH ];
	g_pFullFileSystem->RelativePathToFullPath(
		mat.asChar(), "game", localPath, sizeof( localPath ) );

	CUtlBuffer utlBuf( 0, 0, CUtlBuffer::TEXT_BUFFER );
	if ( g_pFullFileSystem->ReadFile( mat.asChar(), "game", utlBuf ) )
	{
		KeyValues *pKeyValues( new KeyValues( "" ) );
		if ( pKeyValues->LoadFromBuffer( mat.asChar(), utlBuf ) )
		{
			const char *pTexturePath( pKeyValues->GetString( "$basetexture" ) );

			if ( pTexturePath && *pTexturePath != '\0')
			{
				baseTexturePath = pTexturePath;
			}

			const char *pBumpPath( pKeyValues->GetString( "$bumpmap" ) );
			if ( pBumpPath )
			{
				bumpMapPath = pBumpPath;
			}
		}
		else
		{
			mwarn << "Couldn't load VMT file: " << mat << " Based On Material Path: " << materialPath << std::endl;
		}

		pKeyValues->deleteThis();
	}
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
MString CVmfImport::GetTgaFromTexturePath(
	const MString &texturePath ) const
{
	// If the Texture Path is empty, return an empty TGA path
	if ( texturePath.length() == 0 )
		return texturePath;

	MString vtf = MString( "materials/" ) + texturePath + ".vtf";

	char localPath[ MAX_PATH ];
	g_pFullFileSystem->RelativePathToFullPath(
		vtf.asChar(), "game", localPath, sizeof( localPath ) );
	Q_FixSlashes( localPath, '/' );

	MString tga = localPath;
	MGlobal::executeCommand(
		MString( "substitute \"/materials/\" \"" ) +
		localPath +
		"\" \"/materialsrc/\";",
		tga );
	MGlobal::executeCommand(
		MString( "substitute \"/game/\" \"" ) +
		tga +
		"\" \"/content/\";",
		tga );
	MGlobal::executeCommand(
		MString( "substitute \"\\.vtf$\" \"" ) +
		tga +
		"\" \".tga\";",
		tga );

	if ( _access( tga.asChar(), 04 ) == 0 )
		return TextureArchive( tga );

	// Couldn't find the texture by the default methods... revert to path searching

	char searchPath[ MAX_PATH ];
	g_pFullFileSystem->GetSearchPath( "GAME", false, searchPath, sizeof( searchPath ) );
	const char *pEnd( searchPath + MAX_PATH );
	for ( char *pC( searchPath ); *pC && pC != pEnd; ++pC )
	{
		if ( *pC == '\\' )
		{
			*pC = '/';
		}
	}

	MStringArray searchPaths;
	MString( searchPath ).split( ';', searchPaths );

	for ( int i = 0; i < searchPaths.length(); ++i )
	{
		tga = searchPaths[ i ] + "materialsrc/" + texturePath + ".tga";
		MStringArray paths;
		tga.split( '/', paths );
		if ( paths.length() )
		{
			tga = paths[ 0 ];
			for ( int j = 1; j < paths.length(); ++j )
			{
				tga += "/";

				if ( paths[ j ] == "game" )
				{
					tga += "content";
				}
				else
				{
					tga += paths[ j ];

				}
			}
		}

		if ( _access( tga.asChar(), 04 ) == 0 )
			return TextureArchive( tga );
	}

	merr << "Couldn't find a texture for texture path " << texturePath.asChar() << std::endl;

	// Return an empty string to indicate no texture file could be found
	return texturePath;
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void CVmfImport::CreateShadingGroup(
	const MDagPath &mDagPath,
	MIntArray &cIndices,
	const MString &shaderType,
	const MString &shaderName,	
	MObject &sgObj,
	MObject &ssObj )
{
	MStatus mStatus;

	// Create the shading group set
	MFnSingleIndexedComponent cFn;
	MObject cObj = cFn.create( MFn::kMeshPolygonComponent );
	cFn.addElements( cIndices );

	MFnSet sgFn;
	MSelectionList mSelectionList;
	mSelectionList.add( mDagPath, cObj );
	sgObj = sgFn.create( mSelectionList, MFnSet::kRenderableOnly, &mStatus );

	m_pUndoObjs->append( sgObj );

	// Create the surfaceShader node
	ssObj = m_pDagModifier->MDGModifier::createNode( shaderType );
	m_pDagModifier->doIt();

	if ( ssObj.isNull() )
		return;

	m_pDagModifier->renameNode( sgObj, shaderName + "SG" );
	m_pDagModifier->renameNode( ssObj, shaderName );
	m_pDagModifier->doIt();

	const MFnDependencyNode ssFn( ssObj );

	// Connecting shading group to surface shader
	MPlug srcP( ssFn.findPlug( "outColor", &mStatus ) );
	if ( mStatus && !srcP.isNull() )
	{
		MPlug dstP( sgFn.findPlug( "surfaceShader", &mStatus ) );
		if ( mStatus && !dstP.isNull() )
		{
			MPlugArray mPlugArray;
			if ( dstP.connectedTo( mPlugArray, true, false ) )
			{
				for ( uint i( 0 ); i != mPlugArray.length(); ++i )
				{
					m_pDagModifier->disconnect( mPlugArray[ i ], dstP );
				}
			}
			m_pDagModifier->connect( srcP, dstP );
		}
	}

	m_pDagModifier->doIt();
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
MPlug CVmfImport::GetNextDefaultRenderUtilitiesPlug()
{
	MItDependencyNodes dIt( MFn::kRenderUtilityList );
	if ( dIt.isDone() )
		return MPlug();

	MPlug uAP( MFnDependencyNode( dIt.thisNode() ).findPlug( "utilities" ) );
	if ( uAP.isNull() )
		return MPlug();

	// This should return the -nextAvailable index
	// Assuming logical elements are sorted...
	return uAP.elementByLogicalIndex( uAP.elementByPhysicalIndex( uAP.evaluateNumElements() - 1 ).logicalIndex() + 1 );
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
MObject CVmfImport::CreateFileNode(
	const MString &textureName )
{
	MObject resultObj = m_pDagModifier->MDGModifier::createNode( "file" );
	m_pDagModifier->doIt();

	if ( resultObj.isNull() )
		return resultObj;

	MFnDependencyNode resultFn( resultObj );
	resultFn.findPlug( "fileTextureName" ).setValue( textureName.asChar() );

	const MPlug srcP( resultFn.findPlug( "message" ) );
	const MPlug dstP( GetNextDefaultRenderUtilitiesPlug() );

	if ( !srcP.isNull() && !dstP.isNull() )
	{
		m_pDagModifier->connect( srcP, dstP );
		m_pDagModifier->doIt();
	}

	return resultObj;
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
MObject CVmfImport::CreateBump2DNode()
{
	MObject resultObj = m_pDagModifier->MDGModifier::createNode( "bump2d" );
	m_pDagModifier->doIt();

	const MFnDependencyNode resultFn( resultObj );

	const MPlug srcP( resultFn.findPlug( "message" ) );
	const MPlug dstP( GetNextDefaultRenderUtilitiesPlug() );

	if ( !srcP.isNull() && !dstP.isNull() )
	{
		m_pDagModifier->connect( srcP, dstP );
		m_pDagModifier->doIt();
	}

	return resultObj;
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
MObject CVmfImport::CreateProjectionNode(
	const MDagPath &mDagPath,
	const brush_texture_t &brushTexture,
	int width,
	int height )
{
	MObject resultObj = m_pDagModifier->MDGModifier::createNode( "projection" );
	m_pDagModifier->doIt();

	if ( resultObj.isNull() )
		return resultObj;

	MObject plObj = m_pDagModifier->createNode( "place3dTexture" );
	m_pDagModifier->doIt();

	if ( plObj.isNull() )
		return resultObj;

	MFnTransform plFn( plObj );
	const MFnDependencyNode resultFn( resultObj );

	MPlug srcP( resultFn.findPlug( "message" ) );
	MPlug dstP( GetNextDefaultRenderUtilitiesPlug() );

	if ( !srcP.isNull() && !dstP.isNull() )
	{
		m_pDagModifier->connect( srcP, dstP );
	}

	srcP = plFn.findPlug( "worldInverseMatrix" ).elementByLogicalIndex( 0 );
	dstP = resultFn.findPlug( "placementMatrix" );

	if ( !srcP.isNull() && !dstP.isNull() )
	{
		m_pDagModifier->connect( srcP, dstP );
	}
	m_pDagModifier->reparentNode( plObj, mDagPath.transform() );
	m_pDagModifier->doIt();

	const double invWidth = width == 0 ? 1.0 : static_cast< double >( width );
	const double invHeight = height == 0 ? 1.0 : static_cast< double >( height );

	const double uScale = brushTexture.textureWorldUnitsPerTexel[ 0 ];
	const double vScale = -brushTexture.textureWorldUnitsPerTexel[ 1 ];

	const MVector u( brushTexture.UAxis.x, brushTexture.UAxis.y, brushTexture.UAxis.z );
	const MVector v( brushTexture.VAxis.x, brushTexture.VAxis.y, brushTexture.VAxis.z );

	// Maya projects are in the middle, VMF bottom left

	const double uShift =  ( ( -brushTexture.shift[ 0 ] - ( width / 2.0f ) ) * uScale );
	const double vShift = -( ( -brushTexture.shift[ 1 ] - ( height / 2.0f ) ) * vScale );

	// Avoid shear in Matrix
	const MVector z = ( u ^ v ).normal();
	const float fm[4][4] =
	{
		{	u.x,	u.y,	u.z,	0.0f	},
		{	v.x,	v.y,	v.z,	0.0f	},
		{	z.x,	z.y,	z.z,	0.0f	},
		{	uShift * u.x + vShift * v.x, uShift * u.y + vShift * v.y, uShift * u.z + vShift * v.z, 1.0f }
	};

	const MMatrix mm( fm );
	MTransformationMatrix tm( mm );

	plFn.set( tm );

	double s[ 3 ];
	s[ 0 ] = uScale * invWidth * 0.5;
	s[ 1 ] = vScale * invHeight * 0.5;
	s[ 2 ] = 1.0;

	plFn.setScale( s );

	return resultObj;

	// Create a projection node

	{
		MString shaderCmd;

		shaderCmd += "{\n";

		shaderCmd += "string $pr = `shadingNode -asUtility projection`;\n";

		shaderCmd += "string $pl = `shadingNode -asUtility place3dTexture`;\n";

		shaderCmd += "setAttr ( $pl + \".visibility\" ) false;\n";

		shaderCmd += "connectAttr ( $pl + \".worldInverseMatrix[0]\" ) ( $pr + \".placementMatrix\" );\n";

		shaderCmd += "select -r -ne $pr $pl;\n";	// Probably unnecessary

		shaderCmd += "}\n";

		MGlobal::executeCommand( shaderCmd );
	}

	// Get the Maya object created and then selected

	MSelectionList tSelectionList;
	MGlobal::getActiveSelectionList( tSelectionList );
	if ( tSelectionList.length() )
	{
		tSelectionList.getDependNode( 0, resultObj );

		MObject plObj;
		tSelectionList.getDependNode( 1, plObj );
		m_pDagModifier->reparentNode( plObj, mDagPath.transform() );
		m_pDagModifier->doIt();

		const double invWidth = width == 0 ? 1.0 : static_cast< double >( width );
		const double invHeight = height == 0 ? 1.0 : static_cast< double >( height );

		const double uScale = brushTexture.textureWorldUnitsPerTexel[ 0 ];
		const double vScale = -brushTexture.textureWorldUnitsPerTexel[ 1 ];

		const MVector u( brushTexture.UAxis.x, brushTexture.UAxis.y, brushTexture.UAxis.z );
		const MVector v( brushTexture.VAxis.x, brushTexture.VAxis.y, brushTexture.VAxis.z );

		// Maya projects are in the middle, VMF bottom left

		const double uShift =  ( ( -brushTexture.shift[ 0 ] - ( width / 2.0f ) ) * uScale );
		const double vShift = -( ( -brushTexture.shift[ 1 ] - ( height / 2.0f ) ) * vScale );

		// Avoid shear in Matrix
		const MVector z = ( u ^ v ).normal();
		const float fm[4][4] =
		{
			{	u.x,	u.y,	u.z,	0.0f	},
			{	v.x,	v.y,	v.z,	0.0f	},
			{	z.x,	z.y,	z.z,	0.0f	},
			{	uShift * u.x + vShift * v.x,
			uShift * u.y + vShift * v.y,
			uShift * u.z + vShift * v.z,
			1.0f }
		};

		const MMatrix mm( fm );
		MTransformationMatrix tm( mm );

		MFnTransform plFn( plObj );
		plFn.set( tm );

		double s[ 3 ];
		s[ 0 ] = uScale * invWidth * 0.5;
		s[ 1 ] = vScale * invHeight * 0.5;
		s[ 2 ] = 1.0;

		plFn.setScale( s );
	}

	return resultObj;
}



//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
MObject CVmfImport::GetShadingEngine(
	const MDagPath &mDagPath,
	MIntArray &cIndices,
	const brush_texture_t &brushTexture,
	int &width,
	int &height )
{
	MObject sgObj;	// Shading group node
	MObject btObj;	// Base Texture File node
	MObject b2Obj;	// Bump2D Shader Node

	// See if it's already been cached
	const std::map< std::string, SeInfo_s >::const_iterator ti = m_texMap.find( brushTexture.name );

	MFnDependencyNode btFn;
	MFnDependencyNode b2Fn;

	const MString materialPath( brushTexture.name );								// Relative path to .VMT file

	if ( ti != m_texMap.end() )
	{
		sgObj = ti->second.m_sgObj;
		btObj = ti->second.m_btObj;
		b2Obj = ti->second.m_b2Obj;
		width = ti->second.m_width;
		height = ti->second.m_height;

		// If UVs are baked, everything using this texture can share the same shading group

		if ( m_optBakeUVs )
			return sgObj;

		btFn.setObject( btObj );
		b2Fn.setObject( b2Obj );
	}
	else
	{
		// Always need to create a file node if it hasn't been cached

		MString baseTexturePath;														// $basetexture from .VMT file
		MString bumpMapPath;															// $bumpmap from .VMT file

		GetTexturePathsFromMaterialPath( materialPath, baseTexturePath, bumpMapPath );

		// TODO: Load VTF's directly!!!
		const MString baseTextureTga( GetTgaFromTexturePath( baseTexturePath ) );

		// TODO: Check for empty tgaPath and create a silly shader instead

		btObj = CreateFileNode( baseTextureTga );
		btFn.setObject( btObj );

		// Get the texture map size

		btFn.findPlug( "outSizeX" ).getValue( width );
		btFn.findPlug( "outSizeY" ).getValue( height );

		const MString bumpMapTga( GetTgaFromTexturePath( bumpMapPath ) );
		if ( bumpMapTga.length() )
		{
			MObject bmObj = CreateFileNode( bumpMapTga );
			const MFnDependencyNode bmFn( bmObj );
			b2Obj = CreateBump2DNode();
			b2Fn.setObject( b2Obj );

			m_pDagModifier->connect( bmFn.findPlug( "outAlpha" ), b2Fn.findPlug( "bumpValue" ) );
			m_pDagModifier->doIt();
		}
	}

	// Create the shading group

	MObject ssObj;	// Surface shader node
	MString shaderName;
	MGlobal::executeCommand( "match(\"[^\\\\/]*$\", \"" + materialPath + "\");", shaderName );

	MDagPath tDagPath;
	MDagPath::getAPathTo( mDagPath.transform(), tDagPath );
	CreateShadingGroup( mDagPath, cIndices, "lambert", shaderName, sgObj, ssObj );

	const MFnDependencyNode ssFn( ssObj );

	if ( !m_optBakeUVs )
	{
		const MObject pObj( CreateProjectionNode( mDagPath, brushTexture, width, height ) );
		const MFnDependencyNode pFn( pObj );

		// Connect everything up!

		MPlug foP( btFn.findPlug( "outColor" ) );
		MPlug piP( pFn.findPlug( "image" ) );
		MPlug poP( pFn.findPlug( "outColor" ) );
		MPlug ssiP( ssFn.findPlug( "color" ) );

		m_pDagModifier->connect( foP, piP );
		m_pDagModifier->connect( poP, ssiP );
	}
	else
	{
		if ( !b2Obj.isNull() )
		{
			m_pDagModifier->connect( b2Fn.findPlug( "outNormal" ), ssFn.findPlug( "normalCamera" ) );
		}

		m_pDagModifier->connect( btFn.findPlug( "outColor" ), ssFn.findPlug( "color" ) );
	}

	m_pDagModifier->doIt();

	if ( ti == m_texMap.end() )
	{
		// Cache the result

		SeInfo_s &seInfo = m_texMap[ brushTexture.name ];
		seInfo.m_sgObj = sgObj;
		seInfo.m_btObj = btObj;
		seInfo.m_b2Obj = b2Obj;
		seInfo.m_width = width;
		seInfo.m_height = height;
	}

	return sgObj;
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void CVmfImport::AssignMaterial(
	const MDagPath &mDagPath,
	MIntArray &cIndices,
	const mapbrush_t *pMapBrush,
	const brush_texture_t &brushTexture,
	MFloatArray &uArray,
	MFloatArray &vArray,
	MFloatArray &luArray,
	MFloatArray &lvArray )
{
	int width = 0;
	int height = 0;

	MObject sgObj = GetShadingEngine( mDagPath, cIndices, brushTexture, width, height );
	if ( sgObj == MObject::kNullObj )
	{
		merr << "No shadingEngine node created for texture " << brushTexture.name << std::endl;
		return;
	}

	MFnSingleIndexedComponent cFn;
	MObject cObj = cFn.create( MFn::kMeshPolygonComponent );
	cFn.addElements( cIndices );

	MSelectionList mSelectionList;
	mSelectionList.add( mDagPath, cObj );

	MFnSet sgFn( sgObj );
	sgFn.addMembers( mSelectionList );

	if ( m_optBakeUVs )
	{
		// TODO: Assign lightMap UVs
		AssignProjectedUVs( mDagPath, cObj, brushTexture, width, height, uArray, vArray, luArray, lvArray );
	}
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void CVmfImport::AssignProjectedUVs(
	const MDagPath &mDagPath,
	MObject &cObj,
	const brush_texture_t &brushTexture,
	const int width,
	const int height,
	MFloatArray &uArray,
	MFloatArray &vArray,
	MFloatArray &luArray,
	MFloatArray &lvArray )
{
	const MVector uAxis(
		brushTexture.UAxis.x, brushTexture.UAxis.y, brushTexture.UAxis.z );
	const MVector vAxis(
		brushTexture.VAxis.x, brushTexture.VAxis.y, brushTexture.VAxis.z );

	const double uScale = brushTexture.textureWorldUnitsPerTexel[ 0 ];
	const double vScale = -brushTexture.textureWorldUnitsPerTexel[ 1 ];

	const double luScale = brushTexture.lightmapWorldUnitsPerLuxel;
	const double lvScale = -brushTexture.lightmapWorldUnitsPerLuxel;

	const double uShift = brushTexture.shift[ 0 ];
	const double vShift = -brushTexture.shift[ 1 ];

	const double invWidth = width == 0 ? 1.0 : 1.0 / static_cast< double >( width );
	const double invHeight = height == 0 ? 1.0 : 1.0 / static_cast< double >( height );

	int uvIndex = 0;
	MVector pos;
	for ( MItMeshPolygon pIt( mDagPath, cObj ); !pIt.isDone(); pIt.next() )
	{
		const uint nVertices = pIt.polygonVertexCount();
		for ( uint i = 0; i < nVertices; ++i )
		{
			pIt.getUVIndex( i, uvIndex );
			pos = pIt.point( i );
			uArray[ uvIndex ] = ( ( uAxis * pos ) / uScale + uShift ) * invWidth;
			vArray[ uvIndex ] = ( ( vAxis * pos ) / vScale + vShift ) * invHeight;
			luArray[ uvIndex ] = ( ( uAxis * pos ) / luScale + uShift ) * invWidth;
			lvArray[ uvIndex ] = ( ( vAxis * pos ) / lvScale + vShift ) * invHeight;
		}
	}
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
bool CVmfImport::SkipSide(
	const SideInfo_s &sideInfo ) const
{
	if ( !m_optImportTools )
	{
		if ( !_strnicmp( sideInfo.m_brushTexture.name, "TOOLS/", 6 ) ||
			!_strnicmp( sideInfo.m_brushTexture.name, "TOOLS\\", 6 ) )
			return true;
	}

	if ( !m_optImportDev )
	{
		if ( !_strnicmp( sideInfo.m_brushTexture.name, "DEV/", 4 ) ||
			!_strnicmp( sideInfo.m_brushTexture.name, "DEV\\", 4 ) )
			return true;
	}

	return false;
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
MString CVmfImport::TextureArchive(
	const MString &filename ) const
{
	if ( m_optTextureArchive.length() == 0 )
		return filename;

	char buf0[ MAX_PATH ];
	char buf1[ MAX_PATH ];

	Q_strcpy( buf0, filename.asChar() );
	Q_FixSlashes( buf0 );
	const char *file = Q_UnqualifiedFileName( buf0 );
	Q_ComposeFileName( m_optTextureArchive.asChar(), file, buf1, sizeof( buf1 ) );
	Q_FixSlashes( buf0, '/' );
	Q_FixSlashes( buf1, '/' );

	MString melCmd;

	melCmd += "{\n";

	melCmd += MString( "  string $c = \"copy \\\"" ) + buf0 + "\\\" \\\"" + buf1 + "\\\"\";\n";
	melCmd += "  $c = `substituteAllString $c \"/\" \"\\\\\"`;\n";
	melCmd += "  system $c;\n";

	melCmd += "}\n";

	MGlobal::executeCommand( melCmd );

	return buf1;
}
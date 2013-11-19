//======= Copyright © 1996-2006, Valve Corporation, All rights reserved. ======
//
// Purpose:
//
//=============================================================================


// Standard includes
#include <fstream>
#include <string>


// Maya includes
#include <maya/MEulerRotation.h>
#include <maya/MFileIO.h>
#include <maya/MFnDagNode.h>
#include <maya/MGlobal.h>
#include <maya/MItDag.h>
#include <maya/MItSelectionList.h>
#include <maya/MPlug.h>
#include <maya/MQuaternion.h>
#include <maya/MMatrix.h>
#include <maya/MTransformationMatrix.h>
#include <maya/MVector.h>


// Valve includes
#include "valveMaya.h"


// Local includes
#include "QciExport.h"


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
MStatus CQciExport::DoIt(
	const MString &filename,
	const MSelectionList &exportList,
	MStringArray &result )
{
	if ( m_os.is_open() )
	{
		merr << "Stream already open for writing, aboring .qci export" << std::endl;
		return MS::kFailure;
	}

	m_filename = filename;

	MDagPath mDagPath;
	MDagPath sDagPath;

	MSelectionList attachList;

	if ( exportList.length() )
	{
		for ( MItSelectionList sIt( exportList ); !sIt.isDone(); sIt.next() )
		{
			if ( !sIt.getDagPath( mDagPath ) )
				continue;

			sDagPath = mDagPath;
			sDagPath.extendToShapeDirectlyBelow( 0 );

			const MFnDagNode dagFn( sDagPath );
			if ( dagFn.typeName() == "vstAttachment" )
			{
				attachList.add( sDagPath, MObject::kNullObj, true );
			}
		}
	}
	else
	{
		for ( MItDag dIt; !dIt.isDone(); dIt.next() )
		{
			if ( !dIt.getPath( mDagPath ) )
				continue;

			sDagPath = mDagPath;
			sDagPath.extendToShapeDirectlyBelow( 0 );

			const MFnDagNode dagFn( sDagPath );
			if ( dagFn.typeName() == "vstAttachment" )
			{
				attachList.add( sDagPath, MObject::kNullObj, true );
			}
		}
	}

	for ( MItSelectionList sIt( attachList ); !sIt.isDone(); sIt.next() )
	{
		if ( sIt.getDagPath( mDagPath ) )
		{
			ExportAttachment( mDagPath );
		}
	}

	if ( m_os )
	{
		result.append( "Wrote qci " + m_filename );
		m_os.close();
	}

	return MS::kSuccess;
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void CQciExport::ExportAttachment( const MDagPath &sDagPath )
{
	OpenStream();

	const MFnDagNode sDagFn( sDagPath );

	MDagPath tDagPath( sDagPath );
	tDagPath.pop();
	const MFnDagNode tDagFn( tDagPath );

	MPlug tP( sDagFn.findPlug( "type" ) );
	short tV( 1 );
	if ( !tP.isNull() )
	{
		tP.getValue( tV );
	}

	// Type is one of: 0 - Bone + Attachment
	//                 1 - Just Attachment
	//                 2 - Just Bone

	if ( tV == 2 )
		return;		// Bone exported in smd file, nothing to do here

	m_os << "// vstAttachment: " << sDagPath.fullPathName().asChar() << " - Type: " << tV << " ";
	switch ( tV )
	{
	case 0:
		m_os << " Bone+Attachment";
		break;
	case 1:
		m_os << " Just Attachment";
		break;
	case 2:
		m_os << " Just Bone";
		break;
	default:
		m_os << " Unknown";
		break;
	}
	m_os << "\n";

	m_os << "$attachment";

	if ( tV == 0 && tDagPath.length() > 1 )
	{
		m_os << " " << sDagFn.name().asChar();	// name
		m_os << " " << tDagFn.name().asChar();	// bone name
	}
	else
	{
		m_os << " " << tDagFn.name().asChar();	// name
		MDagPath pDagPath ( tDagPath );
		pDagPath.pop();
		m_os << " " << MFnDagNode( pDagPath ).name().asChar();
	}

	MVector lp;
	MTransformationMatrix localMatrix;

	MPlug lpP( sDagFn.findPlug( "localPosition" ) );
	lpP.child( 0 ).getValue( lp.x );
	lpP.child( 1 ).getValue( lp.y );
	lpP.child( 2 ).getValue( lp.z );

	if ( tV == 1 )
	{
		// Just attachment
		MMatrix inclusiveMatrix( tDagPath.inclusiveMatrix() );

		if ( !lp.isEquivalent( MVector::zero ) )
		{
			MMatrix lpMat;
			lpMat[ 3 ][ 0 ] = lp.x;
			lpMat[ 3 ][ 1 ] = lp.y;
			lpMat[ 3 ][ 2 ] = lp.z;

			inclusiveMatrix = lpMat * inclusiveMatrix;
		}

		localMatrix = inclusiveMatrix * tDagPath.exclusiveMatrixInverse();

		// Translate
		const MVector t( localMatrix.getTranslation( MSpace::kObject ) );
		PrettyPrint( t );
	}
	else
	{
		// Bone + Attachment

		if ( lp.isEquivalent( MVector::zero ) )
		{
			m_os << " 0 0 0";
		}
		else
		{
			PrettyPrint( lp	);
		}
	}

	MPlug waP( sDagFn.findPlug( "worldAligned" ) );
	if ( !waP.isNull() )
	{
		bool waV( false );
		waP.getValue( waV );
		if ( waV )
			m_os << " absolute";
	}

	if ( tV != 0 )
		m_os << " rigid";

	if ( tV == 1 )
	{
		// Bone + Attachment

		// Rotate, if applicable
		const MQuaternion q( localMatrix.rotation() );
		if ( !q.isEquivalent( MQuaternion::identity ) )
		{

			const MVector r( q.asEulerRotation().asVector() * 180.0 / M_PI );
			m_os << " rotate ";
			PrettyPrint( r );
		}
	}

	m_os << "\n" << std::endl;
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void CQciExport::OpenStream()
{
	if ( !m_os.is_open() )
	{
		m_os.open( m_filename.asChar() );

		m_os.precision( 6 );
		m_os.setf( std::ios_base::fixed );

		m_os << "// QCI Export\n//\n";

		MString melResult;
		MStringArray melArrayResult;

		MGlobal::executeCommand( "about -a", melResult );
		m_os << "// EDITOR=" << melResult.asChar() << " " << MGlobal::mayaVersion().asChar() << "\n";

		m_os << "// SCENE=" << MFileIO::currentFile().asChar() << "\n";

		MGlobal::executeCommand( "about -cd", melResult );
		m_os << "// DATE=" << melResult.asChar();

		MGlobal::executeCommand( "about -ct", melResult );
		m_os << " " << melResult.asChar() << "\n";

		char pTmpBuf[256];
		*pTmpBuf ='\0';
		DWORD dwSize( sizeof( pTmpBuf ) );

		GetComputerName( pTmpBuf, &dwSize);
		m_os << "// MACHINE=" << pTmpBuf << "\n";

		MGlobal::executeCommand( "strip( about( \"-osv\" ) )", melResult );
		m_os << "// OS=" << melResult.asChar() << "\n";

		GetUserName( pTmpBuf, &dwSize);
		m_os << "// USERNAME=" << pTmpBuf << "\n";

		const char *pVProject = getenv( "VPROJECT" );
		if ( pVProject )
		{
			m_os << "// VPROJECT=" << pVProject << "\n";
		}

		m_os << "//\n\n";
	}
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void CQciExport::PrettyPrint(
	const MVector &v )
{
	m_os <<
		" " << ( fabs( v.x ) < 1.0e-8 ? 0 : v.x ) <<
		" " << ( fabs( v.y ) < 1.0e-8 ? 0 : v.y ) <<
		" " << ( fabs( v.z ) < 1.0e-8 ? 0 : v.z );
}
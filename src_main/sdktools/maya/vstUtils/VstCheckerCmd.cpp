//======= Copyright © 1996-2006, Valve Corporation, All rights reserved. ======
//
// Purpose: Implementation of CVstCheckerCmd
//
//=============================================================================

// Maya includes
#include <maya/MDistance.h>
#include <maya/MFloatArray.h>
#include <maya/MFnSingleIndexedComponent.h>
#include <maya/MFnDoubleIndexedComponent.h>
#include <maya/MItDag.h>
#include <maya/MItMeshFaceVertex.h>
#include <maya/MItMeshPolygon.h>
#include <maya/MItSelectionList.h>
#include <maya/MPointArray.h>
#include <maya/MString.h>
#include <maya/MStringArray.h>


// Valve includes
#include "valveMaya.h"
#include "valveMaya/Undo.h"
#include "VsMayaMPxFactory.h"
#include "tier1/utlvector.h"


class CVstCheckerCmd : public CVsMayaMPxCommand
{
public:
	CVstCheckerCmd();

	// Inherited from MPxCommand
	virtual MStatus doIt( const MArgList &mArgList );

	virtual MStatus undoIt();

	virtual MStatus redoIt();

	virtual bool isUndoable() const { return m_undo.IsUndoable(); }

	virtual bool hasSyntax() const { return true; }

	// Inherited from CVsMayaCommand
	void SpecifySyntax( MSyntax &mSyntax, ValveMaya::CMSyntaxHelp &mSyntaxHelp );

protected:
	enum ErrorToken
	{
		kNonDefaultScale,
		kZeroLengthEdge,
		kZeroLengthTextureEdge,
		kZeroAreaFace,
		kZeroUVArea,
		kZeroLengthNormal,
		kNonNormalizedNormal,
		kHoleInPolygonFace
	};

	ValveMaya::CUndo m_undo;

	void CheckIt();

	void CheckDagNode( const MDagPath &mDagPath, MSelectionList &errorList );

	void CheckTransform( const MDagPath &mDagPath, MSelectionList &errorList );
	
	void CheckMesh( const MDagPath &mDagPath, MSelectionList &errorList );

	static MObject EdgeComponent( MItMeshPolygon &pIt, uint nPolygonEdgeIndex );

	static MObject VertexComponent( MItMeshPolygon &pIt, uint nPolygonVertexIndex );

	static MObject FaceVertexComponent( MItMeshPolygon &pIt, uint nPolygonVertexIndex );

	static MObject FaceComponent( MItMeshPolygon &pIt );

	static MObject FaceMapComponent( MItMeshPolygon &pIt );

	static MSelectionList FaceMapSelection( const MDagPath &mDagPath, MItMeshPolygon &pIt );

	static MObject EdgeMapComponent( MItMeshPolygon &pIt, uint nPolygonEdgeIndex );

	static MSelectionList EdgeMapSelection( const MDagPath &mDagPath, MItMeshPolygon &pIt, uint nPolygonEdgeIndex );

	static MObject VertexMapComponent( MItMeshPolygon &pIt, uint nPolygonVertexIndex );

	bool AddError( ErrorToken errorToken, const MSelectionList &errorList );

	bool AddError( ErrorToken errorToken, const MDagPath &mDagPath, const MObject &cObject = MObject::kNullObj );

	inline bool Abort() const { return m_optAbortOnError && m_errors.Count(); }

	static const char *s_errorMessages[];

	class CErrorData
	{
	public:
		CErrorData( ErrorToken errorToken, const MSelectionList &errorList )
		: m_errorToken( errorToken )
		, m_errorList( errorList )
		{
		}

		const char *ErrorString() const
		{
			return s_errorMessages[ m_errorToken ];
		}

		MString SelectionString() const
		{
			MString selectionString;

			MStringArray mStringArray;
			m_errorList.getSelectionStrings( mStringArray );

			for ( uint i = 0; i != mStringArray.length(); ++i )
			{
				if ( i > 0 )
				{
					selectionString += " ";
				}
				selectionString += mStringArray[ i ];
			}

			return selectionString;
		}

		operator MString() const
		{
			MString errString( ErrorString() );
			errString += ": select -r ";
			errString += SelectionString();
			errString += ";";

			return errString;
		}

		const ErrorToken m_errorToken;	// The type of error
		MSelectionList m_errorList;		// The stuff which has the invalid thingies
	};

	double m_mEpsilon;		// Model epsilon (internal maya units)
	double m_tEpsilon;		// Texture epsilon (internal maya units)

	double m_mSqEpsilon;	// Model epsilon squared
	double m_tSqEpsilon;	// Texture epsilon squared

	bool m_optAbortOnError;
	uint m_nErrorCount;

	MSelectionList m_errorList;
	CUtlVector< CErrorData > m_errors;

	static MSelectionList m_lastSelection;
};


//-----------------------------------------------------------------------------
// Statics
//-----------------------------------------------------------------------------
MSelectionList CVstCheckerCmd::m_lastSelection;


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
const char *CVstCheckerCmd::s_errorMessages[] =
{
	"Non Default Scale",
	"Zero Length Edge In Model Space",
	"Zero Length Edge In Texture Space",
	"Zero Area Face In Model Space",
	"Zero Area Face In Texture Space",
	"Zero Length Normal",
	"Non-Normalized Normal",
	"Hole In Face"
};

//-----------------------------------------------------------------------------
// Hook the command so we can see it in Maya
//-----------------------------------------------------------------------------
INSTALL_MAYA_MPXCOMMAND(
	CVstCheckerCmd,
	vstChecker,
	"Checks Maya scenes for potential problems when converting those scenes to smd or dmx" );


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
CVstCheckerCmd::CVstCheckerCmd()
: m_mEpsilon( FLT_EPSILON * 10.0f )
, m_mSqEpsilon( m_mEpsilon * m_mEpsilon )
, m_tEpsilon( FLT_EPSILON * 10.0f )
, m_tSqEpsilon( m_tEpsilon * m_tEpsilon )
, m_optAbortOnError( false )
{
}


//-----------------------------------------------------------------------------
//
// Purpose: MSyntax factory for the command
// Input  :
// Output : A Maya MSyntax class describing command's command line syntax
//
//-----------------------------------------------------------------------------
static const char *const kOptHelp( "h" );
static const char *const kOptSelection( "sl" );
static const char *const kOptReturn( "r" );
static const char *const kOptThreshold( "t" );
static const char *const kOptTextureThreshold( "tt" );
static const char *const kOptModelThreshold( "mt" );
static const char *const kOptUseLastSelection( "ul" );

void CVstCheckerCmd::SpecifySyntax( MSyntax &mSyntax, ValveMaya::CMSyntaxHelp &mSyntaxHelp )
{
	mSyntaxHelp.Clear();

	mSyntaxHelp.AddFlag( mSyntax, kOptHelp, "help", "general", "Prints this information" );
	mSyntaxHelp.AddFlag( mSyntax, kOptSelection, "selection", "general", "Specifies that the command should only be run on the specified or selected Maya nodes" );
	mSyntaxHelp.AddFlag( mSyntax, kOptReturn, "return", "general", "Return an array consisting of the error message string and the Maya object for each object found in error" );

	mSyntaxHelp.AddFlag( mSyntax, kOptThreshold, "threshold", "general", "Specifies the threshold for determining if an edge is 0 length or not in model & texture space", MSyntax::kDistance );

	mSyntaxHelp.AddFlag( mSyntax, kOptModelThreshold, "modelThreshold", "general", "Specifies the threshold for determining if an edge is 0 length or not in model space only", MSyntax::kDistance );

	mSyntaxHelp.AddFlag( mSyntax, kOptTextureThreshold, "textureThreshold", "general", "Specifies the threshold for determining if an edge is 0 length or not in texture space only", MSyntax::kDistance );

	mSyntaxHelp.AddFlag( mSyntax, kOptUseLastSelection, "useLastSelection", "general", "Uses the same selection list from the last invocation of the command.  If there was no previous invocation, checks whole scene" );

	mSyntax.setObjectType( MSyntax::kSelectionList );
	mSyntax.useSelectionAsDefault( true );
	mSyntax.enableEdit( false );
}


//-----------------------------------------------------------------------------
//
// Purpose: When this command is executed by Maya for the first time,
//          a new instance of the command class is created and this function
//          is called with the specified command line arguments
// Input  : The command line arguments specified by the user
// Output : MStatus indicating success or failure of the command
//          In order for the command to be kept in the undo stack
//          the return value must be MStatus::kSuccess
//          Also the virtual isUndoable() has to return true for the
//          command to be kept in the undo stack for later undoing
//
//-----------------------------------------------------------------------------
MStatus CVstCheckerCmd::doIt( const MArgList &mArgList )
{
	if ( m_undo.SetArgList( syntax(), mArgList ) )
		return redoIt();

	return MS::kFailure;
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
MStatus CVstCheckerCmd::undoIt()
{
	m_undo.Undo();

	return MS::kSuccess;
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
MStatus CVstCheckerCmd::redoIt()
{
	if ( m_undo.ArgDatabase().isFlagSet( kOptHelp ) )
	{
		GetSyntaxHelp()->PrintHelp( GetName(), GetDesc() );
		return MS::kSuccess;
	}

	MDistance t;

	if ( m_undo.ArgDatabase().isFlagSet( kOptThreshold ) )
	{
		if ( m_undo.ArgDatabase().getFlagArgument( kOptThreshold, 0, t ) )
		{
			m_mEpsilon = t.value();
			m_mSqEpsilon = m_mEpsilon * m_mEpsilon;
			m_tEpsilon = t.value();
			m_tSqEpsilon = m_tEpsilon * m_tEpsilon;
		}
	}

	if ( m_undo.ArgDatabase().isFlagSet( kOptModelThreshold ) )
	{
		if ( m_undo.ArgDatabase().getFlagArgument( kOptModelThreshold, 0, t ) )
		{
			m_mEpsilon = t.value();
			m_mSqEpsilon = m_mEpsilon * m_mEpsilon;
		}
	}

	if ( m_undo.ArgDatabase().isFlagSet( kOptTextureThreshold ) )
	{
		if ( m_undo.ArgDatabase().getFlagArgument( kOptTextureThreshold, 0, t ) )
		{
			m_tEpsilon = t.value();
			m_tSqEpsilon = m_tEpsilon * m_tEpsilon;
		}
	}

	CheckIt();

	if ( m_undo.ArgDatabase().isFlagSet( kOptReturn ) )
	{
		MStringArray result;

		const int nErrorCount = m_errors.Count();
		result.setLength( nErrorCount * 2 );

		for ( int i = 0; i < nErrorCount; ++i )
		{
			result[ i * 2 ] = m_errors[ i ].ErrorString();
			result[ i * 2 + 1 ] = m_errors[ i ].SelectionString();
		}

		setResult( result );
	}

	return MS::kSuccess;
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void CVstCheckerCmd::CheckIt()
{
	m_errorList.clear();
	m_errors.RemoveAll();

	MDagPath mDagPath;

	m_undo.SaveCurrentSelection();

	MSelectionList errorList;

	MSelectionList checkList;

	if ( m_undo.ArgDatabase().isFlagSet( kOptUseLastSelection ) )
	{
		if ( m_lastSelection.length() )
		{
			checkList = m_lastSelection;
		}
	}
	else if ( m_undo.ArgDatabase().isFlagSet( kOptSelection ) )
	{
		m_undo.ArgDatabase().getObjects( checkList );
		m_lastSelection = checkList;
	}
	else
	{
		m_lastSelection.clear();
	}

	if ( checkList.length() )
	{
		for ( MItSelectionList sIt( checkList ); !Abort() && !sIt.isDone(); sIt.next() )
		{
			sIt.getDagPath( mDagPath );

			MItDag dIt;
			for ( dIt.reset( mDagPath ); !Abort() && !dIt.isDone(); dIt.next() )
			{
				dIt.getPath( mDagPath );
				CheckDagNode( mDagPath, errorList );
			}
		}
	}
	else
	{
		if ( m_undo.ArgDatabase().isFlagSet( kOptSelection ) )
		{
			mwarn << "-selection specified but no objects selected or passed on the command line, checking entire scene" << std::endl;
		}

		for ( MItDag dIt; !Abort() && !dIt.isDone(); dIt.next() )
		{
			dIt.getPath( mDagPath );
			CheckDagNode( mDagPath, errorList );
		}
	}
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void CVstCheckerCmd::CheckDagNode( const MDagPath &mDagPath, MSelectionList &errorList )
{
	if ( mDagPath.hasFn( MFn::kGroundPlane ) )
		return;

	const MFnDagNode dagFn( mDagPath );
	if ( dagFn.isIntermediateObject() )
		return;

	for ( uint i = 0; i < mDagPath.childCount(); ++i )
	{
		MDagPath childPath( mDagPath );
		childPath.push( mDagPath.child( i ) );
		if ( childPath.hasFn( MFn::kGroundPlane ) )
			return;
	}

	switch ( mDagPath.apiType() )
	{
	case MFn::kMesh:
		CheckMesh( mDagPath, errorList );
		return;
	default:
		break;
	}

	if ( mDagPath.hasFn( MFn::kTransform ) )
	{
		CheckTransform( mDagPath, errorList );
		return;
	}
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void CVstCheckerCmd::CheckTransform( const MDagPath &mDagPath, MSelectionList &errorList )
{
	const MFnTransform tFn( mDagPath );

	{
		// Check For Scale != < 1.0, 1.0, 1.0 >
		double scale[ 3 ];
		tFn.getScale( scale );

		if ( abs( scale[ 0 ] - 1.0 ) > m_mEpsilon || abs( scale[ 1 ] - 1.0 ) > m_mEpsilon || abs( scale[ 2 ] - 1.0 ) > m_mEpsilon )
		{
			AddError( kNonDefaultScale, mDagPath );
		}
	}
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void CVstCheckerCmd::CheckMesh( const MDagPath &mDagPath, MSelectionList &errorList )
{
	MVector n;
	double area;

	MIntArray edges;
	MPointArray points;
	MFloatArray us;
	MFloatArray vs;

	for ( MItMeshPolygon pIt( mDagPath ); !Abort() && !pIt.isDone(); pIt.next() )
	{
		if ( pIt.getEdges( edges ) )
		{
			// Zero length edges
			pIt.getPoints( points );
			const uint nPointCount = points.length();
			for ( uint i = 0; !Abort() && i < nPointCount; ++i )
			{
				if ( ( points[ i ] - points[ ( i + 1 ) % nPointCount ] ).isEquivalent( MPoint::origin, m_mEpsilon ) )
				{
					AddError( kZeroLengthEdge, mDagPath, EdgeComponent( pIt, i ) );
				}
			}

			// Zero UV length edge
			pIt.getUVs( us, vs );
			for ( uint i = 0; !Abort() && i < nPointCount; ++i )
			{
				if ( abs( us[ i ] - us[ ( i + 1 ) % nPointCount ] ) < m_tEpsilon && abs( vs[ i ] - vs[ ( i + 1 ) % nPointCount ] ) < m_tEpsilon )
				{
					AddError( kZeroLengthTextureEdge, EdgeMapSelection( mDagPath, pIt, i ) );
				}
			}
		}

		// Zero Area Face
		if ( pIt.getArea( area ) && area < m_mSqEpsilon )
		{
			AddError( kZeroAreaFace, mDagPath, pIt.polygon() );
		}

		// Zero Area Texture Face
		if ( pIt.getUVArea( area ) && area < m_tSqEpsilon )
		{
			AddError( kZeroUVArea, FaceMapSelection( mDagPath, pIt ) );
		}

		const uint nPolygonVertexCount = pIt.polygonVertexCount();
		for ( uint i = 0; !Abort() && i < nPolygonVertexCount; ++i )
		{
			pIt.getNormal( i, n );

			// Zero length vertex normal
			if ( abs( n.length() ) < m_mEpsilon )
			{
				MSelectionList errorList;
				errorList.add( mDagPath, VertexComponent( pIt, i ) );
				errorList.add( mDagPath, FaceComponent( pIt ) );
				AddError( kZeroLengthNormal, errorList );
			}

			// Non-normalized vertex normal
			if ( abs( n.length() - 1.0 ) > m_mEpsilon )
			{
				MSelectionList errorList;
				errorList.add( mDagPath, VertexComponent( pIt, i ) );
				errorList.add( mDagPath, FaceComponent( pIt ) );
				AddError( kNonNormalizedNormal, errorList );
			}
		}

		if ( pIt.isHoled() )
		{
			AddError( kHoleInPolygonFace, mDagPath, pIt.polygon() );
		}
	}
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
MObject CVstCheckerCmd::EdgeComponent( MItMeshPolygon &pIt, uint nPolygonEdgeIndex )
{
	MIntArray edgeIndices;
	pIt.getEdges( edgeIndices );

	Assert( nPolygonEdgeIndex < edgeIndices.length() );

	MFnSingleIndexedComponent sFn;
	MObject cObject = sFn.create( MFn::kMeshEdgeComponent );

	sFn.addElement( edgeIndices[ nPolygonEdgeIndex ] );

	return cObject;
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
MObject CVstCheckerCmd::VertexComponent( MItMeshPolygon &pIt, uint nPolygonVertexIndex )
{
	MIntArray vertexIndices;
	pIt.getVertices( vertexIndices );

	Assert( nPolygonVertexIndex < vertexIndices.length() );

	MFnSingleIndexedComponent sFn;
	MObject cObject = sFn.create( MFn::kMeshVertComponent );

	sFn.addElement( vertexIndices[ nPolygonVertexIndex ] );

	return cObject;
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
MObject CVstCheckerCmd::FaceVertexComponent( MItMeshPolygon &pIt, uint nPolygonVertexIndex )
{
	MFnDoubleIndexedComponent dFn;
	MObject cObject = dFn.create( MFn::kMeshVtxFaceComponent );

	dFn.addElement( pIt.index(), nPolygonVertexIndex );

	return cObject;
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
MObject CVstCheckerCmd::FaceComponent( MItMeshPolygon &pIt )
{
	MFnSingleIndexedComponent sFn;
	MObject cObject = sFn.create( MFn::kMeshPolygonComponent );

	sFn.addElement( pIt.index() );

	return cObject;
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
MObject CVstCheckerCmd::FaceMapComponent( MItMeshPolygon &pIt )
{
	int uvIndex;

	MFnSingleIndexedComponent sFn;
	MObject cObject = sFn.create( MFn::kMeshMapComponent );

	const uint nPolygonVertexCount = pIt.polygonVertexCount();

	for ( uint i = 0; i < nPolygonVertexCount; ++i )
	{
		pIt.getUVIndex( i, uvIndex );
		sFn.addElement( uvIndex );
	}

	return cObject;
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
MSelectionList CVstCheckerCmd::FaceMapSelection( const MDagPath &mDagPath, MItMeshPolygon &pIt )
{
	MSelectionList errorList;

	errorList.add( mDagPath, FaceMapComponent( pIt ), true );
	errorList.add( mDagPath, pIt.polygon(), true );

	return errorList;
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
MObject CVstCheckerCmd::EdgeMapComponent( MItMeshPolygon &pIt, uint nPolygonEdgeIndex )
{
	MFnSingleIndexedComponent sFn;
	MObject cObject = sFn.create( MFn::kMeshMapComponent );

	const uint nPolygonVertexCount = pIt.polygonVertexCount();

	{
		int uvIndex;

		pIt.getUVIndex( nPolygonEdgeIndex, uvIndex );
		sFn.addElement( uvIndex );

		pIt.getUVIndex( ( nPolygonEdgeIndex + 1 ) % nPolygonVertexCount, uvIndex );
		sFn.addElement( uvIndex );
	}

	return cObject;
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
MSelectionList CVstCheckerCmd::EdgeMapSelection( const MDagPath &mDagPath, MItMeshPolygon &pIt, uint nPolygonEdgeIndex )
{
	MSelectionList errorList;

	errorList.add( mDagPath, EdgeMapComponent( pIt, nPolygonEdgeIndex ), true );
	errorList.add( mDagPath, EdgeComponent( pIt, nPolygonEdgeIndex ), true );

	return errorList;
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
MObject CVstCheckerCmd::VertexMapComponent( MItMeshPolygon &pIt, uint nPolygonVertexIndex )
{
	MFnSingleIndexedComponent sFn;
	MObject cObject = sFn.create( MFn::kMeshMapComponent );

	{
		int uvIndex;

		pIt.getUVIndex( nPolygonVertexIndex, uvIndex );
		sFn.addElement( uvIndex );
	}

	return cObject;
}


//-----------------------------------------------------------------------------
// false means the error was not added as that component is already in error
// for a different reason
// true means the error was added
//-----------------------------------------------------------------------------
bool CVstCheckerCmd::AddError( ErrorToken errorToken, const MSelectionList &errorList )
{
	MDagPath mDagPath;
	MObject cObject;
	const uint nErrorListCount = errorList.length();

	bool bHasEverything = true;

	for ( uint i = 0; i < nErrorListCount; ++i )
	{
		errorList.getDagPath( i, mDagPath, cObject );
		if ( !m_errorList.hasItem( mDagPath, cObject ) )
		{
			bHasEverything = false;
			break;
		}
	}

	if ( bHasEverything )
		return false;

	for ( uint i = 0; i < nErrorListCount; ++i )
	{
		errorList.getDagPath( i, mDagPath, cObject );
		m_errorList.add( mDagPath, cObject );
	}

	const CErrorData &errorData = m_errors[ m_errors.AddToTail( CErrorData( errorToken, errorList ) ) ];

	merr << errorData << std::endl;

	return true;
}


//-----------------------------------------------------------------------------
// false means the error was not added as that component is already in error
// for a different reason
// true means the error was added
//-----------------------------------------------------------------------------
bool CVstCheckerCmd::AddError( ErrorToken errorToken, const MDagPath &mDagPath, const MObject &cObject )
{
	MSelectionList errorList;
	errorList.add( mDagPath, cObject );

	return AddError( errorToken, errorList );
}
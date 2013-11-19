//======= Copyright © 1996-2006, Valve Corporation, All rights reserved. ======
//
// Purpose:
//
//=============================================================================

#include <maya/MFnDagNode.h>
#include <maya/MPlug.h>

#include "valveMaya.h"

//-----------------------------------------------------------------------------
//
// Purpose: Create a Maya Dag node named properly
//
//-----------------------------------------------------------------------------
MStatus ValveMaya::CreateDagNode(
	const char *const i_nodeType,
	const char *const i_transformName,
	const MObject &i_parentObj,
	MObject *o_pTransformObj,
	MObject *o_pShapeObj,
	MDagModifier *i_mDagModifier)
{
	MStatus retVal;

	// If the user didn't supply an MDagModifier make a temporary one
	MDagModifier tmpDagModifier;
	MDagModifier &mDagModifier( i_mDagModifier ? *i_mDagModifier : tmpDagModifier );

	// Try and create the node
	const MObject tObj( mDagModifier.createNode( i_nodeType, i_parentObj, &retVal ) );
	if ( !retVal )
		return retVal;

	mDagModifier.doIt();

	// Failed... undo the Dag modifier and return failure
	if ( tObj.isNull() )
	{
		mDagModifier.undoIt();
        return MS::kFailure;
	}

	// If the caller wants a copy of the created Transform MObject save it
	if ( o_pTransformObj )
		*o_pTransformObj = tObj;

	// The name the shape will be called
	MString sName;

	// Set the name of the transform, it's either the name of the node type or the called specified name
	if ( i_transformName )
	{
		retVal = mDagModifier.renameNode( tObj, i_transformName );
		sName = i_transformName;
    }
	else
	{
		retVal = mDagModifier.renameNode( tObj, i_nodeType );
		sName = i_nodeType;
	}

	// If the rename failed, undo the dag modifier and quit
	if ( !retVal )
	{
		mDagModifier.undoIt();
		return retVal;
	}

	mDagModifier.doIt();

	// Search for the shape node that was created, normally there will be only 1 shape but there are pathological
	// cases, some might not quite be handled here but most will be
	MDagPath tDagPath( MDagPath::getAPathTo( tObj ) );
	unsigned sEnd( 0 );
	tDagPath.numberOfShapesDirectlyBelow( sEnd );

	for ( unsigned si( 0 ); si != sEnd; ++si )
	{
		MDagPath sDagPath( tDagPath );
		sDagPath.extendToShapeDirectlyBelow( si );

		if ( MFnDagNode( sDagPath ).typeName() == i_nodeType )
		{
			// Add the %d suffix so sscanf can be used to check if Maya added a numeric suffix to the transform name
			const MString tScanfName( sName + "%d" );

			// Add the 'Shape' Suffix to the shape name
			sName += "Shape";

			// Get the actual name Maya assigned the node so it can be scanned for a Maya added numeric suffix
			const MString tName( MFnDependencyNode( tObj ).name() );
			int numericSuffix( 0 );

			switch ( sscanf( tName.asChar(), tScanfName.asChar(), &numericSuffix ) )
			{
			case 0:
			default:
				// There was no numeric suffix added by Maya, so nothing extra to add
				break;
			case 1:
				// Maya added a numeric suffix, add it to the shape name
				sName += numericSuffix;
				break;
			}

			// Rename the shape node so it now matches the transform, this operation can't really fail
			const MObject sObj( sDagPath.node() );

			// If the caller wants a copy of the created Transform MObject save it
			if ( o_pShapeObj )
				*o_pShapeObj = sObj;

			mDagModifier.renameNode( sObj, sName );
			mDagModifier.doIt();

			// Just do the first shape of the appropriate type found
			break;
		}
	}
			
	return retVal;
}


//-----------------------------------------------------------------------------
//
// Purpose: Determine if a given Maya Dag node is visible
// Input:	i_mDagPath				The node to check for visibility
// Output:	i_templateAsInvisible	If true, template objects are considered to be invisible
//
//-----------------------------------------------------------------------------
bool ValveMaya::IsNodeVisible( const MDagPath &mDagPath, bool bTemplateAsInvisible )
{
	const MFnDagNode dagFn( mDagPath );

	// All intermediate objects are considered invisible
	if ( dagFn.isIntermediateObject() )
	{
		return false;
	}

	// visibility off == invisible!
	bool visibleVal( false );
	dagFn.findPlug( "visibility" ).getValue( visibleVal );
	if ( !visibleVal )
		return false;

	// If template objects are to be treated as invisible, template == invisible!
	if ( bTemplateAsInvisible )
	{
		bool templateVal( false );
		dagFn.findPlug( "template" ).getValue( templateVal );
		if ( templateVal )
			return false;
	}

	bool overrideVal( false );
	dagFn.findPlug("overrideEnabled").getValue( overrideVal );
	if ( overrideVal )
	{
		// visibility off == invisible!
		dagFn.findPlug("overrideVisibility").getValue( visibleVal );
		if ( !visibleVal )
			return false;

		// If template objects are to be treated as invisible, template == invisible!
		if ( bTemplateAsInvisible )
		{
			int displayTypeVal( 0 );
			dagFn.findPlug( "overrideDisplayType" ).getValue( displayTypeVal );
			if ( displayTypeVal == 1 )
				return false;
		}
	}

	return true;
}


//-----------------------------------------------------------------------------
//
// Purpose: Determine if a given Maya Dag path is visible through
//			Simply makes sure the node and all of its ancestors are visible
// Input:	mDagPath				The node to check for visibility
// Output:	templateAsInvisible		If true, template objects are considered to be invisible
//
//-----------------------------------------------------------------------------
bool ValveMaya::IsPathVisible( MDagPath mDagPath, bool bTemplateAsInvisible )
{
	for ( ; mDagPath.length(); mDagPath.pop() ) 
	{
		if ( !IsNodeVisible( mDagPath, bTemplateAsInvisible ) )
			return false;
	}

	return true;
}
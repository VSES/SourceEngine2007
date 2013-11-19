//======= Copyright © 1996-2006, Valve Corporation, All rights reserved. ======
//
// Purpose:
//
//=============================================================================

#include <maya/MFnDagNode.h>
#include <maya/MFnMesh.h>
#include <maya/MPlug.h>
#include <maya/MPlugArray.h>

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


//-----------------------------------------------------------------------------
//
// Purpose: Remove the Maya namespace(s) from a given string
// Input:	mString					The string from which to remove any namespaces
// Output:	The string with any Maya namespaces removed
//
//-----------------------------------------------------------------------------

MString ValveMaya::RemoveNameSpace( const MString &mString )
{
	const int cIndex( mString.rindex( ':' ) );
	if ( cIndex >= 0 )
	{
		const int l( mString.length() );
		if ( cIndex + 1 < l )
		{
			return mString.substring( cIndex + 1, l - 1 );
		}
	}

	return mString;
}


//-----------------------------------------------------------------------------
//
// Purpose: Converts all of the backslashes (/) in the given string to slashes (/)
// Input:	mString	The string to change all \'s to /'s in
// Output:			The input string with all \'s changed to /'s
//
//-----------------------------------------------------------------------------

MString &ValveMaya::BackslashToSlash( MString &mString )
{
	char *pC( const_cast< char * >( mString.asChar() ) );
	const char *const pCEnd( pC + mString.length() );
	while ( pC != pCEnd )
	{
		if ( *pC == '\\' )
		{
			*pC = '/';
		}
	}

	return mString;
}


//-----------------------------------------------------------------------------
// Returns the next available logical index for the given array plug
//-----------------------------------------------------------------------------
uint ValveMaya::NextAvailable(
	MPlug &mPlug )
{
	MIntArray lIndices;
	mPlug.evaluateNumElements();
	mPlug.getExistingArrayAttributeIndices( lIndices );
	const uint nIndices( lIndices.length() );
	if ( nIndices == 0U )
		return 0U;

	uint nBubble( nIndices - 1U );
	int tmpVal;

	// Bubble sort because I think that lIndices should be in order
	// but it's not documented as such, so just to be safe
	for ( bool swapped( false ); swapped; swapped = false, --nBubble )
	{
		for ( uint i( 0U ); i != nBubble; ++i )
		{
			if ( lIndices[ i ] > lIndices[ i + 1 ] )
			{
				tmpVal = lIndices[ i ];
				lIndices[ i ] = lIndices[ i + 1 ];
				lIndices[ i + 1 ] = tmpVal;
				swapped = true;
			}
		}
	}

	for ( uint i( 0U ); i != nIndices; ++i )
	{
		if ( lIndices[ i ] != static_cast< int >( i ) )
			return i;
	}

	return nIndices;
}


//-----------------------------------------------------------------------------
// Adds a color set to the input history of the passed mesh
//-----------------------------------------------------------------------------
MObject ValveMaya::AddColorSetToMesh(
	const MString &colorSetName,
	const MDagPath &mDagPath,
	MDagModifier &mDagModifier )
{
	if ( !mDagPath.hasFn( MFn::kMesh ) )
		return MObject::kNullObj;

	MFnMesh meshFn( mDagPath );
	MString uniqueColorSetName;

	{
		MStringArray colorSetNames;
		meshFn.getColorSetNames( colorSetNames );
		const uint nColorSets( colorSetNames.length() );
		for ( int i( 0 ); uniqueColorSetName.length() == 0; ++i )
		{
			uniqueColorSetName = colorSetName;
			if ( i > 0 )
			{
				uniqueColorSetName += i;
			}
			for ( uint j( 0U ); j != nColorSets; ++j )
			{
				if ( uniqueColorSetName == colorSetNames[ j ] )
				{
					uniqueColorSetName.clear();
					break;
				}
			}
		}
	}

	// Create a 'createColorSet' node
	MObject ccsObj( mDagModifier.MDGModifier::createNode( "createColorSet" ) );
	mDagModifier.doIt();
	const MFnDependencyNode ccsFn( ccsObj );
	MPlug csnP( ccsFn.findPlug( "colorSetName" ) );
	csnP.setValue( uniqueColorSetName );

	// Insert it in the history of the mesh
	MPlug inMeshP( meshFn.findPlug( "inMesh" ) );
	MPlugArray mPlugArray;
	if ( inMeshP.connectedTo( mPlugArray, true, false ) && mPlugArray.length() )
	{
		mDagModifier.disconnect( mPlugArray[ 0 ], inMeshP );
		mDagModifier.connect( mPlugArray[ 0 ], ccsFn.findPlug( "inputGeometry" ) );
	}

	mDagModifier.connect( ccsFn.findPlug( "outputGeometry" ), inMeshP );
	mDagModifier.doIt();

	return ccsObj;
}


//-----------------------------------------------------------------------------
// Changes any \'s to /'s in place and then splits on /
//-----------------------------------------------------------------------------
uint SplitPath(
	const MString &path,
	MStringArray &splitPath )
{
	char *pC( const_cast< char *>( path.asChar() ) );
	for ( const char *const pEnd( pC + path.length() ); pC != pEnd; ++pC )
	{
		if ( *pC == '\\' )
		{
			*pC = '/';
		}
	}

	splitPath.clear();
	path.split( '/', splitPath );

	return splitPath.length();
}


//-----------------------------------------------------------------------------
// If there is a '.' in the string, removes everything from the end of the
// string up to the first '.' from the end of the string in place.  Returns
// a reference to the same string that was passed
//-----------------------------------------------------------------------------
MString &StripExtension(
	MString &path )
{
	const int ext( path.rindex( '.' ) - 1 );
	if ( ext >= 0 )
	{
		path = path.substring( 0, ext );
	}

	return path;
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
bool PathFromProj(
	const MStringArray &p,
	uint pLen,
	const MStringArray &v,
	uint vLen,
	MString &path )
{
	if ( vLen && vLen > pLen )
	{
		for ( uint pi( 0 ); pi != vLen; ++pi )
		{
			const MString &vStr( v[ pi ] );
			const MString &pStr( p[ pi ] );
			if ( vStr != pStr && ( stricmp( vStr.asChar(), "game" ) || stricmp( pStr.asChar(), "content" ) ) )
				return false;
		}

		path = p[ vLen ];
		for ( uint pi( vLen + 1 ); pi != pLen; ++pi )
		{
			path = path + "/" + p[ pi ];
		}

		return true;
	}

	return false;
}


//-----------------------------------------------------------------------------
// Figures out the material path name from the maya shading group
//-----------------------------------------------------------------------------
MString ValveMaya::GetMaterialPath(
	const MObject &shadingGroupObj,
	MObject *pFileObj,
	MObject *pPlace2dTextureObj,
	MObject *pVmtObj,
	bool *pbTransparent,
	MString *pDebugWhy )
{
	MString materialPath( "debug/debugEmpty" );

	const MObject surfaceShaderObj( FindInputNode( shadingGroupObj, "surfaceShader" ) );
	if ( surfaceShaderObj.isNull() )
	{
		if ( pDebugWhy )
		{
			*pDebugWhy = MString( "Can't find surfaceShader node from shadingGroup: No input to " ) +
				MFnDependencyNode( shadingGroupObj ).name() + ".surfaceShader";
		}

		return materialPath;
	}

	if ( MFnDependencyNode( surfaceShaderObj ).typeName() == "vsVmt" )
	{
		MPlug vmtP = MFnDependencyNode( surfaceShaderObj ).findPlug( "vmtPath" );
		if ( !vmtP.isNull() )
		{
			vmtP.getValue( materialPath );
			return materialPath;
		}
	}


	const MObject fileObj( FindInputNodeOfType( surfaceShaderObj, "file", "color" ) );
	if ( fileObj.isNull() )
	{
		if ( pDebugWhy )
		{
			*pDebugWhy = MString( "Can't find file texture node from surfaceShader: No input to " ) +
				MFnDependencyNode( surfaceShaderObj ).name() + ".color";
		}

		return materialPath;
	}

	if ( pFileObj )
	{
		*pFileObj = fileObj;
	}

	if ( pbTransparent )
	{
		const MObject transObj( FindInputNodeOfType( surfaceShaderObj, "file", "transparency" ) );
		if ( fileObj == transObj )
		{
			*pbTransparent = true;
		}
	}

	if ( pPlace2dTextureObj )
	{
		MObject place2dTextureObj( FindInputNodeOfType( fileObj, "place2dTexture", "uvCoord" ) );
		if ( !place2dTextureObj.isNull() )
		{
			const MPlug uvCoordP( MFnDependencyNode( place2dTextureObj ).findPlug( "uvCoord" ) );
			if ( !( uvCoordP.isNull() || uvCoordP.isConnected() || uvCoordP.isLocked() ) )
			{
				*pPlace2dTextureObj = place2dTextureObj;
			}
		}
	}

	// Check to see if a vsVmtToTex node is driving the filename
	const MObject vsVmtToTexObj = FindInputNodeOfType( fileObj, "vsVmtToTex" );
	if ( !vsVmtToTexObj.isNull() )
	{
		if ( pVmtObj )
		{
			*pVmtObj = vsVmtToTexObj;
		}

		const MPlug materialPathP = MFnDependencyNode( vsVmtToTexObj ).findPlug( "materialPath" );
		if ( !materialPathP.isNull() )
		{
			materialPathP.getValue( materialPath );
			if ( materialPath.length() )
			{
				return materialPath;
			}
		}
	}

	// Otherwise do path munging to figure out 
	MString fileTexture;
	int fLen( 0 );

	const MFnDependencyNode fileFn( fileObj );
	const MPlug fileP( fileFn.findPlug( "fileTextureName" ) );
	fileP.getValue( fileTexture );
	fLen = fileTexture.length();
	if ( fLen == 0 )
	{
		if ( pDebugWhy )
		{
			*pDebugWhy = MString( "No texture filename specified on file texture node: " ) +
				MFnDependencyNode( fileObj ).name() + ".fileTextureName is empty";
		}

		return materialPath;
	}

	MStringArray path;
	const uint pLen( SplitPath( StripExtension( fileTexture ), path ) );

	if ( pLen == 0 )
		return fileTexture.asChar();

	materialPath = path[ pLen - 1 ];
	// Make the path into a relative path
	for ( int i = pLen - 2; i >= 0; --i )
	{
		if ( i > 1 && ( !stricmp( path[ i - 1 ].asChar(), "game" ) || !stricmp( path[ i - 1 ].asChar(), "content" ) ) )
			break;

		if ( !stricmp( path[ i ].asChar(), "materials" ) || !stricmp( path[ i ].asChar(), "materialsrc" ) )
			break;

		materialPath = path[ i ] + "/" + materialPath;
	}

	return materialPath;
}


//-----------------------------------------------------------------------------
// Returns the node MObject that is connected as an input to the specified attribute on the specified node
//-----------------------------------------------------------------------------
MObject ValveMaya::FindInputNode( const MObject &dstNodeObj, const MString &dstPlugName )
{
	const MFnDependencyNode dstNodeFn( dstNodeObj );
	const MPlug dstPlug( dstNodeFn.findPlug( dstPlugName ) );

	if ( dstPlug.isNull() )
		return MObject::kNullObj;

	MPlugArray mPlugArray;
	if ( !( dstPlug.connectedTo( mPlugArray, true, false ) && mPlugArray.length() ) )
		return  MObject::kNullObj;

	return mPlugArray[ 0 ].node();
}


//-----------------------------------------------------------------------------
// Finds a node of the specified type in the history of the specified node
// by following the inputs on the specified attribute of the specified node
// backwards until the first node of the specified type is found.
//-----------------------------------------------------------------------------
MObject ValveMaya::FindInputNodeOfType(
	const MObject &dstNodeObj,
	const MString &typeName,
	const MString &dstPlugName )
{
	const MFnDependencyNode dstNodeFn( dstNodeObj );
	const MPlug dstPlug( dstNodeFn.findPlug( dstPlugName ) );

	if ( dstPlug.isNull() )
		return MObject::kNullObj;

	MPlugArray mPlugArray;
	if ( !( dstPlug.connectedTo( mPlugArray, true, false ) && mPlugArray.length() ) )
		return  MObject::kNullObj;

	const MObject srcObj( mPlugArray[ 0 ].node() );
	if ( MFnDependencyNode( srcObj ).typeName() == typeName )
		return srcObj;

	return FindInputNodeOfType( srcObj, typeName, dstPlugName );
}


//-----------------------------------------------------------------------------
// Finds a node of the specified type in the history of the specified node
// by following all inputs of the specified node backwards until the first node
// of the specified type is found.
//-----------------------------------------------------------------------------
MObject ValveMaya::FindInputNodeOfType(
	const MObject &dstNodeObj,
	const MString &typeName )
{
	const MFnDependencyNode dstNodeFn( dstNodeObj );

	MPlugArray connections;
	if ( dstNodeFn.getConnections( connections ) )
	{
		MPlugArray mPlugArray;
		MObject srcObj;

		// Check all input nodes to see if they are of the correct type
		const uint nConnections = connections.length();
		for ( uint i = 0U; i != nConnections; ++i )
		{
			assert( connections[ i ].node() == dstNodeObj );
			if ( !( connections[ i ].connectedTo( mPlugArray, true, false ) && mPlugArray.length() ) )
				continue;

			srcObj = mPlugArray[ 0 ].node();

			if ( MFnDependencyNode( srcObj ).typeName() == typeName )
				return srcObj;
		}

		// If not, recurse through all input nodes
		for ( uint i = 0U; i != nConnections; ++i )
		{
			if ( !( connections[ i ].connectedTo( mPlugArray, true, false ) && mPlugArray.length() ) )
				continue;

			srcObj = FindInputNodeOfType( mPlugArray[ 0 ].node(), typeName );

			if ( srcObj != MObject::kNullObj )
				return srcObj;
		}
	}

	return MObject::kNullObj;
}
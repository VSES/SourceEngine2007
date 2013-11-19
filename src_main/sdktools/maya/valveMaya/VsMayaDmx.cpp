//====== Copyright © 1996-2005, Valve Corporation, All rights reserved. =======
//
// Purpose:
//
//=============================================================================

#include <maya/MFloatPointArray.h>
#include <maya/MFnMesh.h>
#include <maya/MFnNumericData.h>
#include <maya/MFnSet.h>
#include <maya/MFnSingleIndexedComponent.h>
#include <maya/MFnSkinCluster.h>
#include <maya/MItDependencyNodes.h>
#include <maya/MItMeshPolygon.h>
#include <maya/MPlug.h>
#include <maya/MPlugArray.h>
#include <maya/MString.h>

#include "valveMaya.h"
#include "VsMayaDmx.h"
#include "movieobjects/DmeFaceSet.h"
#include "movieobjects/DmeMaterial.h"

//-----------------------------------------------------------------------------
// Given a Maya shading group, this function maps it to a Valve material path
//-----------------------------------------------------------------------------
void GetMaterialInfo(
	const MObject &shadingGroupObj,
	const MString &shapeName,
	bool scratchVmt,
	DmFileId_t fileId,
	MString &materialName,
	MString &materialPath,
	MString &debugWhy )
{
	Assert( shadingGroupObj.apiType() == MFn::kShadingEngine );

	// Set defaults for now

	const MFnSet shaderFn( shadingGroupObj );

	materialName = shapeName + "Material";
	materialPath = "debug/debugempty";

	// Get the surface shader

	MPlugArray mPlugArray;

	const MFnDependencyNode shadingGroupFn( shadingGroupObj );
	const MPlug surfaceShaderP( shadingGroupFn.findPlug( "surfaceShader" ) );

	if ( !( surfaceShaderP.connectedTo( mPlugArray, true, false ) && mPlugArray.length() ) )
	{
		// Can't find a shader node attached to .surfaceShader, return debug/debugempty
		debugWhy = MString( "No surfaceShader associated with shadingGroup \"" ) + shadingGroupFn.name() + "\"";
		return;
	}

	// Get VPROJECT, going to need it regardless
	MString vproject( getenv( "VPROJECT" ) );
	vm::BackslashToSlash( vproject );
	const int vLen( vproject.length() );

	// Split up VPROJECT into usable chunks
	MStringArray va;
	vproject.split( '/', va );

	// Look for a node driving color
	const MFnDependencyNode surfaceShaderFn( mPlugArray[ 0 ].node() );
	const MPlug colorP( surfaceShaderFn.findPlug( "color" ) );

	MString fileTexture;
	int fLen( 0 );

	if ( colorP.connectedTo( mPlugArray, true, false ) && mPlugArray.length() && mPlugArray[ 0 ].node().hasFn( MFn::kFileTexture ) )
	{
		// There's a fileTexture node driving color
		const MFnDependencyNode fileFn( mPlugArray[ 0 ].node() );
		const MPlug fileP( fileFn.findPlug( "fileTextureName" ) );
		fileP.getValue( fileTexture );
		fLen = fileTexture.length();
		if ( fLen == 0 )
		{
			debugWhy = MString( "No filename specified on fileTexture node \"" ) + fileFn.name() + "\"";
			return;
		}

		vm::BackslashToSlash( fileTexture );

		// Split up filename into the pathname components
		MStringArray fa;
		fileTexture.split( '/', fa );

		// See if things are how we expect them...  va should be something like:
		// ... / <somedir> / game / <somegame>
		// Where <somedir> / 
		// e.g.
		// pf: U:/dev/main/content/tf2/materialsrc/models/player/hvyweapon/mouth.tga
		// pv: D:/DEV/ValveGames/main/game/tf2
		//
		// Means vmt file is:
		//
		// D:/DEV/ValveGames/main/game/tf2/materials/models/player/hvyweapon/mouth.vmt

		const int faLen( fa.length() );
		const int vaLen( va.length() );

		if ( vaLen >= 3 && va[ vaLen - 2 ] == "game" )
		{
			const int vSomeDir( vaLen - 3 );
			int fSomeDir( 0 );
			for ( const int fSomeDirEnd( faLen - 1 ); fSomeDir < fSomeDirEnd; ++fSomeDir )
			{
				if ( fa[ fSomeDir ]  == va[ vSomeDir ] &&
					fa[ fSomeDir + 1 ] == "content" &&
					va[ vSomeDir + 1 ] == "game" &&
					fa[ fSomeDir + 3 ] == "materialsrc" )
				{
					const int fSomeGame( fSomeDir + 2 );
					const int vSomeGame( vSomeDir + 2 );

					MStringArray fsa;
					fa[ faLen - 1 ].split( '.', fsa );

					MString vmtPath( va[ 0 ] );

					for ( int vi( 1 ); vi <= vSomeGame; ++vi )
					{
						vmtPath += "/";
						vmtPath += va[ vi ];
					}

					vmtPath += "/materials";

					for ( int fi( fSomeGame + 2 ); fi < ( faLen - 1 ); ++fi )
					{
						vmtPath += "/";
						vmtPath += fa[ fi ];
					}

					vmtPath += "/";
					vmtPath += MString( fsa[ 0 ] + ".vmt" );

					FILE *fp = fopen( vmtPath.asChar(), "r" );
					if ( fp )
					{
						MStringArray vmta;

						char buf[ 1024 ];
						char tex[ 1024 ];

						while ( !feof( fp ) )
						{
							if ( fgets( buf, sizeof( buf ), fp ) )
							{
								// Remove any leading /'s or \'s from the material path
								// I think that's a good thing to do anyway...
								if ( sscanf( buf, " \"$basetexture\" %*[\"/\\]%[^\"]\" ", tex ) == 1 )
								{
									// Flip the backslashes forward
									for ( char *cp( tex ); *cp != '\0'; ++cp )
									{
										if ( *cp == '\\' )
										{
											*cp = '/';
										}
									}
									materialPath = tex;

									for ( char *cp( tex ); *cp != '\0'; ++cp )
									{
										if ( *cp == '/' )
										{
											*cp = '_';
										}
									}
									materialName = tex;

									break;
								}
							}
						}
						fclose( fp );
					}
					else
					{
						debugWhy = MString( "Cannot open vmt file \"" ) + vmtPath + "\"";
					}
					break;
				}
			}
		}
	}
	else
	{
		// A fileTexture node wasn't driving color
		if ( scratchVmt )
		{
			if ( vLen )
			{
				// Get the color value!
				MObject colorObj;
				colorP.getValue( colorObj );
				MFnNumericData colorFn( colorObj );
				float r, g, b;
				colorFn.getData(r, g, b);

				const int vaLen( va.length() );
				MString scratchPath;
				for ( int vai( 0 ); vai < vaLen; ++vai )
				{
					scratchPath += va[ vai ];
					scratchPath += "/";
				}
				scratchPath += "materials";

				const MString templatePath( scratchPath );
				scratchPath += "/scratch";

				// Windows is annoying me so just always try to create this...
				// Check for failure by not being able to open the file
				CreateDirectoryEx( templatePath.asChar(), scratchPath.asChar(), NULL );

				scratchPath += "/";
				scratchPath += shapeName;
				scratchPath += "_";
				scratchPath += vm::RemoveNameSpace( shadingGroupFn.name() );
				scratchPath += ".vmt";

				FILE *pVmt( fopen( scratchPath.asChar(), "w" ) );
				if ( !pVmt )
				{
					debugWhy = MString( "Cannot open scratch vmt file \"" ) + scratchPath + "\" for writing";
					return;
				}

				fprintf( pVmt, "\"VertexLitGeneric\"\n" );
				fprintf( pVmt, "{\n" );
				fprintf( pVmt, "   \"$color\" \"{ %d %d %d }\"\n",
					clamp( static_cast<int>( floor( r * 255.0f ) ), 0, 255 ),
					clamp( static_cast<int>( floor( g * 255.0f ) ), 0, 255 ),
					clamp( static_cast<int>( floor( b * 255.0f ) ), 0, 255 ) );
				fprintf( pVmt, "}\n" );

				fclose( pVmt );

				materialPath = "scratch/";
				materialPath += shapeName;
				materialPath += "_";
				materialPath += vm::RemoveNameSpace( shadingGroupFn.name() );
			}
			else
			{
				debugWhy = MString( "No fileTexture node driving color on surfaceShader \"" ) + surfaceShaderFn.name() + "\", -scratchVmt specified but VPROJECT is empty so don't know where to write scratchVmt";
			}
		}
		else
		{
			// Leave debug/debugempty because scratchVmt wasn't on
			debugWhy = MString( "No fileTexture node driving color on surfaceShader \"" ) + surfaceShaderFn.name() + "\" and -scratchVmt wasn't specified during export";
		}
	}
}


//-----------------------------------------------------------------------------
// Given a Maya shading group, this function creates a CDmeMaterial representation
//-----------------------------------------------------------------------------
CDmeMaterial *MayaShadingGroupToDmeMaterial(
	const MDagPath &mDagPath,
	const MObject &shadingGroupObj,
	bool scratchVmt,
	DmFileId_t fileId )
{
	const MString shapeName( vm::RemoveNameSpace( MFnDependencyNode( mDagPath.transform() ).name() ) );

	MString materialName( shapeName + "Material" );
	MString materialPath( "debug/debugempty" );
	MString debugWhy;

	if ( shadingGroupObj.isNull() || shadingGroupObj.apiType() != MFn::kShadingEngine )
	{
		materialName = shapeName + "Material";
		debugWhy = "No shading group assigned in Maya";
	}
	else
	{
		GetMaterialInfo( shadingGroupObj, shapeName, scratchVmt, fileId, materialName, materialPath, debugWhy );
	}

	CDmeMaterial *pMaterial(
		CreateElement< CDmeMaterial >(
		materialName.asChar(),
		fileId ) );

	if ( materialPath == "debug/debugempty" && debugWhy.length() )
	{
		pMaterial->SetValue( "debugWhy", debugWhy.asChar() );
	}

	pMaterial->SetMaterial( materialPath.asChar() );

	return pMaterial;
}


//-----------------------------------------------------------------------------
// Converts a specific face set in a given Maya mesh to a DmeFaceSet
//-----------------------------------------------------------------------------
void MayaFaceSetToDmeFaceSet(
	const MDagPath &mDagPath,
	MObject &mayaFaceSetObject,
	CUtlVector< CUtlVector< int > * > &vertexData,
	int &globalFaceVertexIndex,
	CDmeFaceSet *pFaceSet )
{
	CUtlVector< int > &positionIndices(	*vertexData[ 0 ] );
	CUtlVector< int > &normalIndices(	*vertexData[ 1 ] );
	CUtlVector< int > &uvIndices(		*vertexData[ 2 ] );
	int hasUVs( uvIndices.Count() );
	CUtlVector< int > &colorIndices(	*vertexData[ 3 ] );
	int hasColors( colorIndices.Count() );

	// A vector for Face Set indices
	CUtlVector< int > faceSets;

	for ( MItMeshPolygon pIt( mDagPath, mayaFaceSetObject ); !pIt.isDone(); pIt.next() )
	{
		// MItMeshFaceVertex should be used but it can fail (crash) when asking for
		// the normalId when there aren't faceVarying normals assigned

		// lfv = localFaceVertex( Index | End )
		const int lfvEnd( pIt.polygonVertexCount() );

		if ( pIt.hasUVs() && pIt.hasColor() )
		{
			// Vertices, normals, UVs & Colors!
			for ( int lfvi( 0 ); lfvi < lfvEnd; ++lfvi, ++globalFaceVertexIndex )
			{
				positionIndices[ globalFaceVertexIndex ] = pIt.vertexIndex( lfvi );
				normalIndices[ globalFaceVertexIndex ] = pIt.normalIndex( lfvi );
				pIt.getUVIndex( lfvi, uvIndices[ globalFaceVertexIndex ] );
				pIt.getColorIndex( lfvi, colorIndices[ globalFaceVertexIndex ] );
				faceSets.AddToTail( globalFaceVertexIndex );
			}
		}
		else if ( pIt.hasUVs() )
		{
			// Just vertices, normals & Uvs
			if ( hasColors )
			{
				Warning(
					"Face %d on %s ( %s.f[%d] ) has no colors assigned but other faces in the mesh do."
					"  This is not supported in Dme, not exporting per vertex colors\n",
					pIt.index(),
					mDagPath.partialPathName().asChar(),
					mDagPath.partialPathName().asChar(),
					pIt.index() );

				hasColors = false;
				colorIndices.Purge();
			}

			for ( int lfvi( 0 ); lfvi < lfvEnd; ++lfvi, ++globalFaceVertexIndex )
			{
				positionIndices[ globalFaceVertexIndex ] = pIt.vertexIndex( lfvi );
				normalIndices[ globalFaceVertexIndex ] = pIt.normalIndex( lfvi );
				pIt.getUVIndex( lfvi, uvIndices[ globalFaceVertexIndex ] );
				faceSets.AddToTail( globalFaceVertexIndex );
			}
		}
		else if ( pIt.hasColor() )
		{
			// Just vertices, normals & colors
			if ( hasUVs )
			{
				Warning(
					"Face %d on %s ( %s.f[%d] ) has no UVs assigned but other faces in the mesh do."
					"  This is not supported in Dme, not exporting color\n",
					pIt.index(),
					mDagPath.partialPathName().asChar(),
					mDagPath.partialPathName().asChar(),
					pIt.index() );

				hasUVs = false;
				uvIndices.Purge();
			}

			for ( int lfvi( 0 ); lfvi < lfvEnd; ++lfvi, ++globalFaceVertexIndex )
			{
				positionIndices[ globalFaceVertexIndex ] = pIt.vertexIndex( lfvi );
				normalIndices[ globalFaceVertexIndex ] = pIt.normalIndex( lfvi );
				pIt.getColorIndex( lfvi, colorIndices[ globalFaceVertexIndex ] );
				faceSets.AddToTail( globalFaceVertexIndex );
			}
		}
		else
		{
			// Just vertices & normals
			for ( int lfvi( 0 ); lfvi < lfvEnd; ++lfvi, ++globalFaceVertexIndex )
			{
				positionIndices[ globalFaceVertexIndex ] = pIt.vertexIndex( lfvi );
				normalIndices[ globalFaceVertexIndex ] = pIt.normalIndex( lfvi );
				faceSets.AddToTail( globalFaceVertexIndex );
			}
		}

		faceSets.AddToTail( -1 );
	}

	pFaceSet->AddIndices( faceSets.Count() );
	pFaceSet->SetIndices( 0, faceSets.Count(), faceSets.Base() );
}


//-----------------------------------------------------------------------------
// Converts all of the face sets in a given Maya Mesh to DmeFaceSet's
//-----------------------------------------------------------------------------
void MayaFaceSetsToDmeFaceSets(
	const MDagPath &mDagPath,
	CDmeMesh *pDmeMesh,
	bool scratchVmt,
	DmFileId_t fileId,
	CUtlVector< CUtlVector< int > * > &vertexData,
	int &globalFaceVertexIndex )
{
	MObjectArray shaders;
	MIntArray faceShaderIndices;

	MFnMesh meshFn( mDagPath );
	meshFn.getConnectedShaders( mDagPath.instanceNumber(), shaders, faceShaderIndices );

	// Build an MFnSingleIndexedComponent Object for each shader plus one for
	// faces which have no shader.  Since 'faceShaderIndices' will contain -1
	// for faces without a shading group assignment, offset the indices by
	// +1.  If Maya's lying about -1 and it's -2 or something, things will be bad

	CUtlVector< MFnSingleIndexedComponent * > faceSetFns;

	// Create shaders.length() + 1 MFnSingleIndexedComponents for each faceSet
	const uint shaderEnd( shaders.length() );
	for ( uint shaderIndex( 0 ); shaderIndex <= shaderEnd; ++shaderIndex )
	{
		// faceSetFns[ 0 ] is for faces with no shading group assigned
		// faceSetFns[ n ] is for shader n - 1
		faceSetFns.AddToTail( new MFnSingleIndexedComponent );
		faceSetFns.Tail()->create( MFn::kMeshPolygonComponent );
	}

	// Now loop through faceShaderIndices and construct the single indexed components
	// this way, it's guaranteed that every face is accounted for
	{
		const uint faceEnd( faceShaderIndices.length() );
		Assert( faceEnd == static_cast< uint >( meshFn.numPolygons() ) );

		for ( uint faceIndex( 0 ); faceIndex != faceEnd; ++faceIndex )
		{
			faceSetFns[ faceShaderIndices[ faceIndex ] + 1 ]->addElement( faceIndex );
		}
	}

	// Now loop through all of the face sets which aren't empty and
	// create a DmeFaceSet for them

	{
		const uint faceSetFnsEnd( faceSetFns.Count() );
		for ( uint faceSetFnsIndex( 0 ); faceSetFnsIndex != faceSetFnsEnd; ++faceSetFnsIndex )
		{
			const MFnSingleIndexedComponent &faceSetFn( *faceSetFns[ faceSetFnsIndex ] );
			if ( faceSetFn.isEmpty() )
			{
				// Ignore empty
				continue;
			}

			CDmeMaterial *pMaterial(
				faceSetFnsIndex == 0 ?
				MayaShadingGroupToDmeMaterial( mDagPath, MObject::kNullObj, scratchVmt, fileId ) :
				MayaShadingGroupToDmeMaterial( mDagPath, shaders[ faceSetFnsIndex - 1 ], scratchVmt, fileId ) );

			const MString faceSetName(
				faceSetFnsIndex == 0 ?
				vm::RemoveNameSpace( MFnDependencyNode( mDagPath.transform() ).name() ) + "FaceSet" :
				vm::RemoveNameSpace( MFnDependencyNode( shaders[ faceSetFnsIndex - 1 ] ).name() ) );

			// Create the CDmeFaceSet
			CDmeFaceSet *pDmeFaceSet( CreateElement< CDmeFaceSet >( faceSetName.asChar(), fileId ) );

			if ( !pDmeFaceSet )
			{
				Error( "Cannot create DmeFaceSet \"%s\"\n", faceSetName.asChar() );
				continue;
			}

			// Convert the Maya Face Set To DmeFaceSet
			MayaFaceSetToDmeFaceSet( mDagPath, faceSetFn.object(), vertexData, globalFaceVertexIndex, pDmeFaceSet );

			pDmeFaceSet->SetMaterial( pMaterial );

			// Add the face set to the mesh
			pDmeMesh->AddFaceSet( pDmeFaceSet );
		}
	}

	// Delete all of the MFnSingleIdexedComponents
	faceSetFns.PurgeAndDeleteElements();
}


CDmeMesh *MayaMeshToDmeMesh(
	const MDagPath &mDagPath,
	bool scratchVmt,
	DmFileId_t fileId )
{
	MFnMesh meshFn;

	bool isSkinned( false );

	// See if the mesh has history, and if it does, if it's a
	// skin cluster... if it's something else, warn the user.
	{
		MObject skinClusterObj;

		MPlug imP( MFnDagNode( mDagPath ).findPlug( "inMesh" ) );
		if ( !imP.isNull() )
		{
			MPlugArray mPlugArray;
			if ( imP.connectedTo( mPlugArray, true, false ) && mPlugArray.length() )
			{
				MObjectArray skinnedObjects;
				const MObject meshObj( mDagPath.node() );
				MObject skinObj;

				// It has history so look for MFnSkinCluster objects
				for ( MItDependencyNodes dIt( MFn::kSkinClusterFilter ); skinClusterObj.isNull() && !dIt.isDone(); dIt.next() )
				{
					skinObj = dIt.thisNode();
					MFnSkinCluster( skinObj ).getOutputGeometry( skinnedObjects );
					const uint soEnd( skinnedObjects.length() );
					for ( uint soi( 0 ); soi != soEnd; ++soi )
					{
						if ( meshObj == skinnedObjects[ soi ] )
						{
							skinClusterObj = skinObj;
							break;
						}
					}
				}
			}
		}

		if ( skinClusterObj.isNull() )
		{
			meshFn.setObject( mDagPath );
		}
		else
		{
			isSkinned = true;

			MFnSkinCluster skinFn( skinClusterObj );
			MObjectArray inputGeometry;
			skinFn.getInputGeometry( inputGeometry );
			if ( inputGeometry.length() )
			{
				if ( inputGeometry.length() > 1 )
				{
					Warning( "Mesh %s is skinned to skin cluster %s with %d input geometries, mesh 0 will be exported\n",
						mDagPath.partialPathName().asChar(), 
						skinFn.name().asChar(),
						inputGeometry.length() );
				}

				meshFn.setObject( inputGeometry[ 0 ] );
			}
			else
			{
				Warning( "Mesh %s is skinned to skin cluster %s with no input geometry.  Mesh will be exported in current position\n",
					mDagPath.partialPathName().asChar(), 
					skinFn.name().asChar() );
				meshFn.setObject( mDagPath );
			}
		}
	}

	// Create the CDmeMesh element
	CDmeMesh *pDmeMesh( CreateElement< CDmeMesh >( vm::RemoveNameSpace( meshFn.name() ).asChar(), fileId ) );
	CDmeVertexData *pVertexData = pDmeMesh->FindOrCreateBaseState( "bind" );
	pDmeMesh->SetValue( "flipVCoordinates", true );

	// Just some checks
	const int numVertices( meshFn.numVertices() );
	const int numNormals( meshFn.numNormals() );
	int numColors( meshFn.numColors() );
	int numUVs( meshFn.numUVs() );
	const int numFaceVertices( meshFn.numFaceVertices() );

	CUtlVector< CUtlVector< int > * > vertexData;

	// A vector for position indices
	CUtlVector< int > positionIndices;
	positionIndices.EnsureCount( numFaceVertices );
	Assert( positionIndices.Count() == numFaceVertices );
	vertexData.AddToTail( &positionIndices );

	// A vector for normal indices
	CUtlVector< int > normalIndices;
	normalIndices.EnsureCount( numFaceVertices );
	Assert( normalIndices.Count() == numFaceVertices );
	vertexData.AddToTail( &normalIndices );

	// A vector for UV indices
	CUtlVector< int > uvIndices;
	uvIndices.EnsureCount( numUVs ? numFaceVertices : 0 );
	Assert( uvIndices.Count() == numFaceVertices || uvIndices.Count() == 0 );
	vertexData.AddToTail( &uvIndices );

	// A vector for Color indices
	CUtlVector< int > colorIndices;
	colorIndices.EnsureCount( numColors ? numFaceVertices : 0 );
	Assert( colorIndices.Count() == numFaceVertices || colorIndices.Count() == 0);
	vertexData.AddToTail( &colorIndices );

	int globalFaceVertexIndex( 0 );

	MayaFaceSetsToDmeFaceSets( mDagPath, pDmeMesh, scratchVmt, fileId, vertexData, globalFaceVertexIndex );

	// Everything together should add up to this...
	Assert( globalFaceVertexIndex == numFaceVertices );

	// Ask CDmeMesh where to store the mesh vertices
	const FieldIndex_t positionIndex( pVertexData->CreateField<Vector>( "positions" ) );

	{
		// Do vertices

		// Get the data from Maya
		MFloatPointArray mPoints;
		meshFn.getPoints( mPoints );
		Assert( static_cast<unsigned>( numVertices ) == mPoints.length() );

		// We have to put it into a temporary format for now - maybe can stuff it right into DME?
		CUtlVector< Vector > dPoints;
		dPoints.EnsureCount( numVertices );

		// Copy from Maya homogeneous to Dme cartesian
		for ( int pi( 0 ); pi < numVertices; ++pi )
		{
			const MFloatPoint &p( mPoints[ pi ] );
			// Just ignore W, should be 1... but could divide by W...
			dPoints[ pi ] = Vector( p.x, p.y, p.z );
		}

		// Add & set the data
		pVertexData->AddVertexData( positionIndex, numVertices );
		pVertexData->SetVertexData( positionIndex, 0, numVertices, AT_VECTOR3, dPoints.Base() );
	}

	// Ask CDmeMesh where to store the mesh normals
	const FieldIndex_t normalIndex( pVertexData->CreateField<Vector>( "normals" ) );

	{
		// Do normals

		// Get the data from Maya
		MFloatVectorArray mNormals;
		meshFn.getNormals( mNormals );
		Assert( static_cast<unsigned>( numNormals ) == mNormals.length() );

		// Add & set the data
		pVertexData->AddVertexData( normalIndex, numNormals );
		pVertexData->SetVertexData( normalIndex, 0, numNormals, AT_VECTOR3, &mNormals[ 0 ] );
	}

	// Field index for UVs, might not be used
	FieldIndex_t uvIndex( 0 );

	if ( numUVs )
	{
		// Do UVs

		// Get the data from Maya
		MFloatArray mUs;
		MFloatArray mVs;
		meshFn.getUVs( mUs, mVs );

		const float *pUSrc( &mUs[ 0 ] );
		const float *pVSrc( &mVs[ 0 ] );

		// Convert them into the format Dme wants
		CUtlVector< Vector2D > dUVs;
		dUVs.EnsureCount( numUVs );

		Assert( numUVs == static_cast<int>( mUs.length() ) );
		Assert( numUVs == static_cast<int>( mVs.length() ) );
		Assert( numUVs == dUVs.Count() );

		const Vector2D *const pUVEnd( dUVs.Base() + numUVs );

		for ( Vector2D *pUVDst( dUVs.Base() ); pUVDst < pUVEnd; ++pUVDst, ++pUSrc, ++pVSrc )
		{
			pUVDst->x = *pUSrc;
			pUVDst->y = *pVSrc;
			// Of course, this "fix" assumes UVs only run 0-1 but I'm guessing a lot more
			// would break if they didn't
		}

		// Ask CDmeMesh where to store the mesh UVs
		uvIndex = pVertexData->CreateField<Vector2D>( "textureCoordinates" );

		// Add & set the data
		pVertexData->AddVertexData( uvIndex, numUVs );
		pVertexData->SetVertexData( uvIndex, 0, numUVs, AT_VECTOR2, dUVs.Base() );
	}

	// Field index for Colors, might not be used
	FieldIndex_t colorIndex( 0 );

	if ( numColors )
	{
		// Do Colors

		// Get the data from Maya
		MColorArray mColorArray;
		meshFn.getColors( mColorArray );

		const MColor *pColorSrc( &mColorArray[ 0 ] );

		// Convert them into the format Dme wants
		CUtlVector< Color > dColors;
		dColors.EnsureCount( numColors );

		Assert( numColors == static_cast<int>( mColorArray.length() ) );
		Assert( numColors == dColors.Count() );

		const Color *const pColorEnd( dColors.Base() + numColors );

		float r, g, b, a;

		for ( Color *pColor( dColors.Base() ); pColor < pColorEnd; ++pColor, ++pColorSrc )
		{
			pColorSrc->get( MColor::kRGB, r, g, b, a );
			pColor->SetColor(
				clamp( static_cast<int>( floor( r * 255.0f ) ), 0, 255 ),
				clamp( static_cast<int>( floor( g * 255.0f ) ), 0, 255 ),
				clamp( static_cast<int>( floor( b * 255.0f ) ), 0, 255 ),
				clamp( static_cast<int>( floor( a * 255.0f ) ), 0, 255 ) );
		}

		// Ask CDmeMesh where to store the mesh colors
		colorIndex = pVertexData->CreateField<Color>( "colors" );

		// Add & set the data
		pVertexData->AddVertexData( colorIndex, numColors );
		pVertexData->SetVertexData( colorIndex, 0, numColors, AT_COLOR, dColors.Base() );
	}

	pVertexData->AddVertexIndices( numFaceVertices );
	pVertexData->SetVertexIndices( positionIndex, 0, numFaceVertices, positionIndices.Base() );
	pVertexData->SetVertexIndices( normalIndex, 0, numFaceVertices, normalIndices.Base() );

	if ( numUVs )
	{
		pVertexData->SetVertexIndices( uvIndex, 0, numFaceVertices, uvIndices.Base() );
	}

	if ( numColors )
	{
		pVertexData->SetVertexIndices( colorIndex, 0, numFaceVertices, colorIndices.Base() );
	}

	return pDmeMesh;
}
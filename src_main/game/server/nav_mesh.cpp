//========= Copyright © 1996-2005, Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
// $NoKeywords: $
//
//=============================================================================//
// NavMesh.cpp
// Implementation of Navigation Mesh interface
// Author: Michael S. Booth, 2003-2004

#include "cbase.h"
#include "filesystem.h"
#include "nav_mesh.h"
#include "nav_node.h"
#include "fmtstr.h"
#include "utlbuffer.h"
#include "tier0/vprof.h"

#define DrawLine( from, to, duration, red, green, blue )		NDebugOverlay::Line( from, to, red, green, blue, true, 0.1f )


/**
 * The singleton for accessing the navigation mesh
 */
CNavMesh *TheNavMesh = NULL;

ConVar nav_edit( "nav_edit", "0", FCVAR_GAMEDLL | FCVAR_CHEAT, "Set to one to interactively edit the Navigation Mesh. Set to zero to leave edit mode." );
ConVar nav_quicksave( "nav_quicksave", "0", FCVAR_GAMEDLL | FCVAR_CHEAT, "Set to one to skip the time consuming phases of the analysis.  Useful for data collection and testing." );
ConVar nav_show_approach_points( "nav_show_approach_points", "0", FCVAR_GAMEDLL | FCVAR_CHEAT, "Show Approach Points in the Navigation Mesh." );
ConVar nav_show_danger( "nav_show_danger", "0", FCVAR_GAMEDLL | FCVAR_CHEAT, "Show current 'danger' levels." );
ConVar nav_show_player_counts( "nav_show_player_counts", "0", FCVAR_GAMEDLL | FCVAR_CHEAT, "Show current player counts in each area." );


//--------------------------------------------------------------------------------------------------------------
CNavMesh::CNavMesh( void )
{
	m_grid = NULL;
	m_spawnName = NULL;
	m_gridCellSize = 300.0f;

	m_placeCount = 0;
	m_placeName = NULL;

	LoadPlaceDatabase();

	Reset();
}

//--------------------------------------------------------------------------------------------------------------
CNavMesh::~CNavMesh()
{
	delete [] m_grid;
	m_grid = NULL;

	if (m_spawnName)
		delete [] m_spawnName;

 // !!!!bug!!! why does this crash in linux on server exit
	for( unsigned int i=0; i<m_placeCount; ++i )
	{
		delete [] m_placeName[i];
	}

}

//--------------------------------------------------------------------------------------------------------------
/**
 * Reset the Navigation Mesh to initial values
 */
void CNavMesh::Reset( void )
{
	DestroyNavigationMesh();

	m_generationMode = GENERATE_NONE;
	m_currentNode = NULL;
	ClearWalkableSeeds();

	m_isFromCurrentMap = true;
	m_isEditing = false;
	m_navPlace = UNDEFINED_PLACE;
	m_markedArea = NULL;
	m_selectedArea = NULL;

	m_lastSelectedArea = NULL;
	m_navEditMode = NAV_EDIT_NORMAL;
	m_isPlacePainting = false;

	m_climbableSurface = false;
	m_isCreatingLadder = false;
	m_markedLadder = NULL;
	m_selectedLadder = NULL;

	if (m_spawnName)
	{
		delete [] m_spawnName;
	}

	m_spawnName = NULL;

	m_walkableSeedList.RemoveAll();
}


//--------------------------------------------------------------------------------------------------------------
/**
 * Free all resources of the mesh and reset it to empty state
 */
void CNavMesh::DestroyNavigationMesh( bool incremental )
{
	if ( !incremental )
	{
		// destroy all areas
		CNavArea::m_isReset = true;

			// remove each element of the list and delete them
			FOR_EACH_LL( TheNavAreaList, it )
			{
				delete TheNavAreaList[ it ];
			}

			TheNavAreaList.RemoveAll();

		CNavArea::m_isReset = false;


		// destroy ladder representations
		DestroyLadders();
	}
	else
	{
		FOR_EACH_LL( TheNavAreaList, it )
		{
			TheNavAreaList[ it ]->ResetNodes();
		}
	}

	// destroy all hiding spots
	DestroyHidingSpots();

	// destroy navigation nodes created during map generation
	CNavNode *node, *next;
	for( node = CNavNode::m_list; node; node = next )
	{
		next = node->m_next;
		delete node;
	}
	CNavNode::m_list = NULL;
	CNavNode::m_listLength = 0;
	CNavNode::m_nextID = 1;

	if ( !incremental )
	{
		// destroy the grid
		if (m_grid)
			delete [] m_grid;

		m_grid = NULL;
		m_gridSizeX = 0;
		m_gridSizeY = 0;
	}

	// clear the hash table
	for( int i=0; i<HASH_TABLE_SIZE; ++i )
	{
		m_hashTable[i] = NULL;
	}

	if ( !incremental )
	{
		m_areaCount = 0;
	}

	if ( !incremental )
	{
		// Reset the next area and ladder IDs to 1
		CNavArea::CompressIDs();
		CNavLadder::CompressIDs();
	}

	m_markedArea = NULL;
	m_selectedArea = NULL;
	m_lastSelectedArea = NULL;
	m_navEditMode = NAV_EDIT_NORMAL;
	m_climbableSurface = false;
	m_isCreatingLadder = false;
	m_markedLadder = NULL;
	m_selectedLadder = NULL;

	if ( !incremental )
	{
		m_isLoaded = false;
	}
}


//--------------------------------------------------------------------------------------------------------------
/**
 * Invoked on each game frame
 */
void CNavMesh::Update( void )
{
	VPROF( "CNavMesh::Update" );

	if (IsGenerating())
	{
		UpdateGeneration( 0.005 );
		return; // don't bother trying to draw stuff while we're generating
	}

	if (nav_edit.GetBool())
	{
		if (m_isEditing == false)
		{
			OnEditModeStart();
			m_isEditing = true;
		}

		DrawEditMode();
	}
	else
	{
		if (m_isEditing)
		{
			OnEditModeEnd();
			m_isEditing = false;
		}
	}

	if (nav_show_danger.GetBool())
	{
		DrawDanger();
	}

	if (nav_show_player_counts.GetBool())
	{
		DrawPlayerCounts();
	}

	// draw any walkable seeds that have been marked
	FOR_EACH_LL( m_walkableSeedList, it )
	{
		WalkableSeedSpot spot = m_walkableSeedList[ it ];

		const float height = 50.0f;
		const float width = 25.0f;
		DrawLine( spot.pos, spot.pos + height * spot.normal, 3, 255, 0, 255 ); 
		DrawLine( spot.pos + Vector( width, 0, 0 ), spot.pos + height * spot.normal, 3, 255, 0, 255 ); 
		DrawLine( spot.pos + Vector( -width, 0, 0 ), spot.pos + height * spot.normal, 3, 255, 0, 255 ); 
		DrawLine( spot.pos + Vector( 0, width, 0 ), spot.pos + height * spot.normal, 3, 255, 0, 255 ); 
		DrawLine( spot.pos + Vector( 0, -width, 0 ), spot.pos + height * spot.normal, 3, 255, 0, 255 ); 
	}
}


//--------------------------------------------------------------------------------------------------------------
/**
 * Allocate the grid and define its extents
 */
void CNavMesh::AllocateGrid( float minX, float maxX, float minY, float maxY )
{
	if (m_grid)
	{
		// destroy the grid
		if (m_grid)
			delete [] m_grid;

		m_grid = NULL;
	}

	m_minX = minX;
	m_minY = minY;

	m_gridSizeX = (int)((maxX - minX) / m_gridCellSize) + 1;
	m_gridSizeY = (int)((maxY - minY) / m_gridCellSize) + 1;

	m_grid = new NavAreaList[ m_gridSizeX * m_gridSizeY ];
}

//--------------------------------------------------------------------------------------------------------------
/**
 * Add an area to the mesh
 */
void CNavMesh::AddNavArea( CNavArea *area )
{
	if ( !m_grid )
	{
		// If we somehow have no grid (manually creating a nav area without loading or generating a mesh), don't crash
		AllocateGrid( 0, 0, 0, 0 );
	}

	// add to grid
	const Extent &extent = area->GetExtent();

	int loX = WorldToGridX( extent.lo.x );
	int loY = WorldToGridY( extent.lo.y );
	int hiX = WorldToGridX( extent.hi.x );
	int hiY = WorldToGridY( extent.hi.y );

	for( int y = loY; y <= hiY; ++y )
	{
		for( int x = loX; x <= hiX; ++x )
		{
			m_grid[ x + y*m_gridSizeX ].AddToTail( const_cast<CNavArea *>( area ) );
		}
	}

	// add to hash table
	int key = ComputeHashKey( area->GetID() );

	if (m_hashTable[key])
	{
		// add to head of list in this slot
		area->m_prevHash = NULL;
		area->m_nextHash = m_hashTable[key];
		m_hashTable[key]->m_prevHash = area;
		m_hashTable[key] = area;
	}
	else
	{
		// first entry in this slot
		m_hashTable[key] = area;
		area->m_nextHash = NULL;
		area->m_prevHash = NULL;
	}

	if ( area->GetAttributes() & NAV_MESH_TRANSIENT )
	{
		m_transientAreas.AddToTail( area );
	}

	++m_areaCount;
}

//--------------------------------------------------------------------------------------------------------------
/**
 * Remove an area from the mesh
 */
void CNavMesh::RemoveNavArea( CNavArea *area )
{
	// add to grid
	const Extent &extent = area->GetExtent();

	int loX = WorldToGridX( extent.lo.x );
	int loY = WorldToGridY( extent.lo.y );
	int hiX = WorldToGridX( extent.hi.x );
	int hiY = WorldToGridY( extent.hi.y );

	for( int y = loY; y <= hiY; ++y )
	{
		for( int x = loX; x <= hiX; ++x )
		{
			m_grid[ x + y*m_gridSizeX ].FindAndRemove( area );
		}
	}

	// remove from hash table
	int key = ComputeHashKey( area->GetID() );

	if (area->m_prevHash)
	{
		area->m_prevHash->m_nextHash = area->m_nextHash;
	}
	else
	{
		// area was at start of list
		m_hashTable[key] = area->m_nextHash;

		if (m_hashTable[key])
		{
			m_hashTable[key]->m_prevHash = NULL;
		}
	}

	if (area->m_nextHash)
	{
		area->m_nextHash->m_prevHash = area->m_prevHash;
	}

	if ( area->GetAttributes() & NAV_MESH_TRANSIENT )
	{
		BuildTransientAreaList();
	}

	--m_areaCount;
}

//--------------------------------------------------------------------------------------------------------------
void CNavMesh::BuildTransientAreaList( void )
{
	m_transientAreas.RemoveAll();

	FOR_EACH_LL( TheNavAreaList, it )
	{
		CNavArea *area = TheNavAreaList[ it ];

		if ( area->GetAttributes() & NAV_MESH_TRANSIENT )
		{
			m_transientAreas.AddToTail( area );
		}
	}
}

//--------------------------------------------------------------------------------------------------------------
/**
 * Given a position, return the nav area that IsOverlapping and is *immediately* beneath it
 */
CNavArea *CNavMesh::GetNavArea( const Vector &pos, float beneathLimit ) const
{
	VPROF( "CNavMesh::GetNavArea" );

	if (m_grid == NULL)
		return NULL;

	// get list in cell that contains position
	int x = WorldToGridX( pos.x );
	int y = WorldToGridY( pos.y );
	NavAreaList *list = &m_grid[ x + y*m_gridSizeX ];


	// search cell list to find correct area
	CNavArea *use = NULL;
	float useZ = -99999999.9f;
	Vector testPos = pos + Vector( 0, 0, 5 );

	FOR_EACH_LL( (*list), it )
	{
		CNavArea *area = (*list)[ it ];

		// check if position is within 2D boundaries of this area
		if (area->IsOverlapping( testPos ))
		{
			// project position onto area to get Z
			float z = area->GetZ( testPos );

			// if area is above us, skip it
			if (z > testPos.z)
				continue;

			// if area is too far below us, skip it
			if (z < pos.z - beneathLimit)
				continue;

			// if area is higher than the one we have, use this instead
			if (z > useZ)
			{
				use = area;
				useZ = z;
			}
		}
	}

	return use;
}


//--------------------------------------------------------------------------------------------------------------
void CNavMesh::GridToWorld( int gridX, int gridY, Vector *pos ) const
{
	gridX = clamp( gridX, 0, m_gridSizeX-1 );
	gridY = clamp( gridY, 0, m_gridSizeY-1 );

	pos->x = m_minX + gridX * m_gridCellSize;
	pos->y = m_minY + gridY * m_gridCellSize;
}


//--------------------------------------------------------------------------------------------------------------
/**
 * Given a position in the world, return the nav area that is closest
 * and at the same height, or beneath it.
 * Used to find initial area if we start off of the mesh.
 */
CNavArea *CNavMesh::GetNearestNavArea( const Vector &pos, bool anyZ, float maxDist, bool checkLOS ) const
{
	VPROF( "CNavMesh::GetNearestNavArea" );

	if (m_grid == NULL)
		return NULL;

	CNavArea *close = NULL;
	float closeDistSq = maxDist * maxDist;

	// quick check
	close = GetNavArea( pos );
	if (close)
	{
		return close;
	}

	// ensure source position is well behaved
	Vector source;
	source.x = pos.x;
	source.y = pos.y;
	if (GetGroundHeight( pos, &source.z ) == false)
	{
		return NULL;
	}

	source.z += HalfHumanHeight;


	// find closest nav area
	CNavArea::MakeNewMarker();


	// get list in cell that contains position
	int originX = WorldToGridX( pos.x );
	int originY = WorldToGridY( pos.y );

	int shiftLimit = maxDist / m_gridCellSize;

	//
	// Search in increasing rings out from origin, starting with cell
	// that contains the given position.
	// Once we find a close area, we must check one more step out in
	// case our position is just against the edge of the cell boundary
	// and an area in an adjacent cell is actually closer.
	// 
	for( int shift=0; shift <= shiftLimit; ++shift )
	{
		for( int x = originX - shift; x <= originX + shift; ++x )
		{
			if (x < 0 || x >= m_gridSizeX)
				continue;

			for( int y = originY - shift; y <= originY + shift; ++y )
			{
				if (y < 0 || y >= m_gridSizeY)
					continue;

				// only check these areas if we're on the outer edge of our spiral
				if ( x > originX - shift &&
					 x < originX + shift &&
					 y > originY - shift &&
					 y < originY + shift )
					continue;

				NavAreaList *list = &m_grid[ x + y*m_gridSizeX ];

				// find closest area in this cell
				FOR_EACH_LL( (*list), it )
				{
					CNavArea *area = (*list)[ it ];

					// skip if we've already visited this area
					if (area->IsMarked())
						continue;

					area->Mark();

					Vector areaPos;
					area->GetClosestPointOnArea( source, &areaPos );

					float distSq = (areaPos - source).LengthSqr();

					// keep the closest area
					if (distSq < closeDistSq)
					{
						// check LOS to area
						// REMOVED: If we do this for !anyZ, it's likely we wont have LOS and will enumerate every area in the mesh
						// It is still good to do this in some isolated cases, however
						if ( checkLOS )
						{
							trace_t result;
							UTIL_TraceLine( source, areaPos + Vector( 0, 0, HalfHumanHeight ), MASK_PLAYERSOLID_BRUSHONLY, NULL, COLLISION_GROUP_NONE, &result );
							if (result.fraction != 1.0f)
							{
								continue;
							}
						}
								
						closeDistSq = distSq;
						close = area;

						// look one more step outwards
						shiftLimit = shift+1;
					}
				}
			}
		}
	}

	return close;
}

//--------------------------------------------------------------------------------------------------------------
/**
 * Given an ID, return the associated area
 */
CNavArea *CNavMesh::GetNavAreaByID( unsigned int id ) const
{
	if (id == 0)
		return NULL;

	int key = ComputeHashKey( id );

	for( CNavArea *area = m_hashTable[key]; area; area = area->m_nextHash )
	{
		if (area->GetID() == id)
		{
			return area;
		}
	}

	return NULL;
}

//--------------------------------------------------------------------------------------------------------------
/**
 * Given an ID, return the associated ladder
 */
CNavLadder *CNavMesh::GetLadderByID( unsigned int id ) const
{
	if (id == 0)
		return NULL;

	FOR_EACH_LL( m_ladderList, it )
	{
		CNavLadder *ladder = m_ladderList[it];
		if ( ladder->GetID() == id )
		{
			return ladder;
		}
	}

	return NULL;
}

//--------------------------------------------------------------------------------------------------------------
/**
 * Return radio chatter place for given coordinate
 */
unsigned int CNavMesh::GetPlace( const Vector &pos ) const
{
	CNavArea *area = GetNearestNavArea( pos, true );

	if (area)
	{
		return area->GetPlace();
	}

	return UNDEFINED_PLACE;
}

//--------------------------------------------------------------------------------------------------------------
/**
 * Load the place names from a file
 */
void CNavMesh::LoadPlaceDatabase( void )
{
	m_placeCount = 0;

	CUtlBuffer buf( 0, 0, CUtlBuffer::TEXT_BUFFER );
	filesystem->ReadFile("NavPlace.db", "GAME", buf);

	if (!buf.Size())
		return;

	const int maxNameLength = 128;
	char buffer[ maxNameLength ];

	CUtlVector<char*> placeNames;

	// count the number of places
	while( true )
	{
		buf.GetLine( buffer, maxNameLength );

		if ( !buf.IsValid() )
			break;

		int len = V_strlen( buffer );
		if ( len >= 2 )
		{
			if ( buffer[len-1] == '\n' )
				buffer[len-1] = 0;
			
			if ( buffer[len-2] == '\r' )
				buffer[len-2] = 0;

			char *pName = new char[ len + 1 ];
			V_strncpy( pName, buffer, len+1 );
			placeNames.AddToTail( pName );
		}
	}

	// allocate place name array
	m_placeCount = placeNames.Count();
	m_placeName = new char * [ m_placeCount ];
	
	for ( unsigned int i=0; i < m_placeCount; i++ )
	{
		m_placeName[i] = placeNames[i];
	}
}

//--------------------------------------------------------------------------------------------------------------
/**
 * Given a place, return its name.
 * Reserve zero as invalid.
 */
const char *CNavMesh::PlaceToName( Place place ) const
{
	if (place >= 1 && place <= m_placeCount)
		return m_placeName[ (int)place - 1 ];

	return NULL;
}


//--------------------------------------------------------------------------------------------------------------
/**
 * Given a place name, return a place ID or zero if no place is defined
 * Reserve zero as invalid.
 */
Place CNavMesh::NameToPlace( const char *name ) const
{
	for( unsigned int i=0; i<m_placeCount; ++i )
	{
		if (FStrEq( m_placeName[i], name ))
			return i+1;
	}

	return 0;
}


//--------------------------------------------------------------------------------------------------------------
/**
 * Given the first part of a place name, return a place ID or zero if no place is defined, or the partial match is ambiguous
 */
Place CNavMesh::PartialNameToPlace( const char *name ) const
{
	Place found = UNDEFINED_PLACE;
	bool isAmbiguous = false;
	for( unsigned int i=0; i<m_placeCount; ++i )
	{
		if (!strnicmp( m_placeName[i], name, strlen( name ) ))
		{
			// check for exact match in case of subsets of other strings
			if (!stricmp( m_placeName[i], name ))
			{
				found = NameToPlace( m_placeName[i] );
				isAmbiguous = false;
				break;
			}

			if (found != UNDEFINED_PLACE)
			{
				isAmbiguous = true;
			}
			else
			{
				found = NameToPlace( m_placeName[i] );
			}
		}
	}

	if (isAmbiguous)
		return UNDEFINED_PLACE;

	return found;
}


//--------------------------------------------------------------------------------------------------------------
/**
 * Given a partial place name, fill in possible place names for ConCommand autocomplete
 */
int CNavMesh::PlaceNameAutocomplete( char const *partial, char commands[ COMMAND_COMPLETION_MAXITEMS ][ COMMAND_COMPLETION_ITEM_LENGTH ] )
{
	int numMatches = 0;
	partial += Q_strlen( "nav_use_place " );
	int partialLength = Q_strlen( partial );

	for( unsigned int i=0; i<m_placeCount; ++i )
	{
		if ( !Q_strnicmp( m_placeName[i], partial, partialLength ) )
		{
			// Add the place name to the autocomplete array
			Q_snprintf( commands[ numMatches++ ], COMMAND_COMPLETION_ITEM_LENGTH, "nav_use_place %s", m_placeName[i] );

			// Make sure we don't try to return too many place names
			if ( numMatches == COMMAND_COMPLETION_MAXITEMS )
				return numMatches;
		}
	}

	return numMatches;
}


//--------------------------------------------------------------------------------------------------------------
typedef const char * SortStringType;
int StringSort (const SortStringType *s1, const SortStringType *s2)
{
	return strcmp( *s1, *s2 );
}


//--------------------------------------------------------------------------------------------------------------
/**
 * Output a list of names to the console
 */
void CNavMesh::PrintAllPlaces( void ) const
{
	if (m_placeCount == 0)
	{
		Msg( "There are no entries in the Place database.\n" );
		return;
	}

	unsigned int i;

	CUtlVector< SortStringType > placeNames;
	for ( i=0; i<m_placeCount; ++i )
	{
		placeNames.AddToTail( m_placeName[i] );
	}
	placeNames.Sort( StringSort );

	for( i=0; i<(unsigned int)placeNames.Count(); ++i )
	{
		if (NameToPlace( placeNames[i] ) == GetNavPlace())
			Msg( "--> %-26s", placeNames[i] );
		else
			Msg( "%-30s", placeNames[i] );

		if ((i+1) % 3 == 0)
			Msg( "\n" );
	}

	Msg( "\n" );
}


//--------------------------------------------------------------------------------------------------------------
/**
 * Return the ground height below this point in "height".
 * Return false if position is invalid (outside of map, in a solid area, etc).
 */
bool CNavMesh::GetGroundHeight( const Vector &pos, float *height, Vector *normal ) const
{
	VPROF( "CNavMesh::GetGroundHeight" );

	Vector to;
	to.x = pos.x;
	to.y = pos.y;
	to.z = pos.z - 9999.9f;

	float offset;
	Vector from;
	trace_t result;
	CBaseEntity *ignore = NULL;

	const float maxOffset = 100.0f;
	const float inc = 10.0f;

	#define MAX_GROUND_LAYERS 16
	struct GroundLayerInfo
	{
		float ground;
		Vector normal;
	}
	layer[ MAX_GROUND_LAYERS ];
	int layerCount = 0;

	for( offset = 1.0f; offset < maxOffset; offset += inc )
	{
		from = pos + Vector( 0, 0, offset );

		CTraceFilterWalkableEntities filter( ignore, COLLISION_GROUP_NONE, WALK_THRU_EVERYTHING );
		UTIL_TraceLine( from, to, MASK_PLAYERSOLID_BRUSHONLY, &filter, &result );

		// if the trace came down thru a door, ignore the door and try again
		// also ignore breakable floors
		if (result.DidHitNonWorldEntity() && result.m_pEnt)
		{
			if (FClassnameIs( result.m_pEnt, "prop_door" ) ||
				FClassnameIs( result.m_pEnt, "prop_door_rotating" ) ||
				FClassnameIs( result.m_pEnt, "func_breakable" ))// && VARS( result.pHit )->takedamage == DAMAGE_YES))
			{
				ignore = result.m_pEnt;
				// keep incrementing to avoid infinite loop if more than one entity is along the traceline...
				/// @todo Deal with multiple ignore entities in a single TraceLine()
				//offset -= inc;
				continue;
			}
		}

		if (result.startsolid == false)
		{
			// if we didnt start inside a solid area, the trace hit a ground layer

			// if this is a new ground layer, add it to the set
			if (layerCount == 0 || result.endpos.z > layer[ layerCount-1 ].ground)
			{
				layer[ layerCount ].ground = result.endpos.z;
				if (result.plane.normal.IsZero())
					layer[ layerCount ].normal = Vector( 0, 0, 1 );
				else
					layer[ layerCount ].normal = result.plane.normal;

				++layerCount;
						
				if (layerCount == MAX_GROUND_LAYERS)
					break;
			}
		}
	}

	if (layerCount == 0)
		return false;

	// find the lowest layer that allows a player to stand or crouch upon it
	int i;
	for( i=0; i<layerCount-1; ++i )
	{
		if (layer[i+1].ground - layer[i].ground >= HalfHumanHeight)
			break;		
	}

	*height = layer[ i ].ground;

	if (normal)
		*normal = layer[ i ].normal;

	return true;
}

//--------------------------------------------------------------------------------------------------------------
/**
 * Return the "simple" ground height below this point in "height".
 * This function is much faster, but less tolerant. Make sure the give position is "well behaved".
 * Return false if position is invalid (outside of map, in a solid area, etc).
 */
bool CNavMesh::GetSimpleGroundHeight( const Vector &pos, float *height, Vector *normal ) const
{
	Vector to;
	to.x = pos.x;
	to.y = pos.y;
	to.z = pos.z - 9999.9f;

	trace_t result;

	UTIL_TraceLine( pos, to, MASK_PLAYERSOLID_BRUSHONLY, NULL, COLLISION_GROUP_NONE, &result );

	if (result.startsolid)
		return false;

	*height = result.endpos.z;

	if (normal)
		*normal = result.plane.normal;

	return true;
}

//--------------------------------------------------------------------------------------------------------------
/**
 * Show danger levels for debugging
 */
void CNavMesh::DrawDanger( void ) const
{
	FOR_EACH_LL( TheNavAreaList, it )
	{
		CNavArea *area = TheNavAreaList[ it ];

		Vector center = area->GetCenter();
		Vector top;
		center.z = area->GetZ( center );

		float danger = area->GetDanger( 0 );
		if (danger > 0.1f)
		{
			top.x = center.x;
			top.y = center.y;
			top.z = center.z + 10.0f * danger;
			DrawLine( center, top, 3, 255, 0, 0 );
		}

		danger = area->GetDanger( 1 );
		if (danger > 0.1f)
		{
			top.x = center.x;
			top.y = center.y;
			top.z = center.z + 10.0f * danger;
			DrawLine( center, top, 3, 0, 0, 255 );
		}
	}
}


//--------------------------------------------------------------------------------------------------------------
/**
 * Zero player counts in all areas
 */
void CNavMesh::ClearPlayerCounts( void )
{
	FOR_EACH_LL( TheNavAreaList, it )
	{
		CNavArea *area = TheNavAreaList[ it ];

		area->ClearPlayerCount();
	}
}


//--------------------------------------------------------------------------------------------------------------
/**
 * Show current player counts for debugging.
 * NOTE: Assumes two teams.
 */
void CNavMesh::DrawPlayerCounts( void ) const
{
	CFmtStr msg;

	FOR_EACH_LL( TheNavAreaList, it )
	{
		CNavArea *area = TheNavAreaList[ it ];

		if (area->GetPlayerCount() > 0)
		{
			NDebugOverlay::Text( area->GetCenter(), msg.sprintf( "%d (%d/%d)", area->GetPlayerCount(), area->GetPlayerCount(1), area->GetPlayerCount(2) ), false, 0.1f );
		}
	}
}


//--------------------------------------------------------------------------------------------------------------
/**
 * Increase the danger of nav areas containing and near the given position
 */
void CNavMesh::IncreaseDangerNearby( int teamID, float amount, CNavArea *startArea, const Vector &pos, float maxRadius )
{
	if (startArea == NULL)
		return;

	CNavArea::MakeNewMarker();
	CNavArea::ClearSearchLists();

	startArea->AddToOpenList();
	startArea->SetTotalCost( 0.0f );
	startArea->Mark();
	startArea->IncreaseDanger( teamID, amount );

	while( !CNavArea::IsOpenListEmpty() )
	{
		// get next area to check
		CNavArea *area = CNavArea::PopOpenList();
		
		// explore adjacent areas
		for( int dir=0; dir<NUM_DIRECTIONS; ++dir )
		{
			int count = area->GetAdjacentCount( (NavDirType)dir );
			for( int i=0; i<count; ++i )
			{
				CNavArea *adjArea = area->GetAdjacentArea( (NavDirType)dir, i );

				if (!adjArea->IsMarked())
				{
					// compute distance from danger source
					float cost = (adjArea->GetCenter() - pos).Length();
					if (cost <= maxRadius)
					{
						adjArea->AddToOpenList();
						adjArea->SetTotalCost( cost );
						adjArea->Mark();
						adjArea->IncreaseDanger( teamID, amount * cost/maxRadius );
					}
				}
			}
		}
	}
}


//--------------------------------------------------------------------------------------------------------------
void CommandNavRemoveUnusedJumpAreas( void )
{
	if ( !UTIL_IsCommandIssuedByServerAdmin() )
		return;

	TheNavMesh->CommandNavRemoveUnusedJumpAreas();
}
static ConCommand nav_remove_unused_jump_areas( "nav_remove_unused_jump_areas", CommandNavRemoveUnusedJumpAreas, "Removes jump areas with at most 1 connection to a ladder or non-jump area.", FCVAR_GAMEDLL | FCVAR_CHEAT );


//--------------------------------------------------------------------------------------------------------------
void CommandNavDelete( void )
{
	if ( !UTIL_IsCommandIssuedByServerAdmin() )
		return;

	TheNavMesh->CommandNavDelete();
}
static ConCommand nav_delete( "nav_delete", CommandNavDelete, "Deletes the currently highlighted Area.", FCVAR_GAMEDLL | FCVAR_CHEAT );


//--------------------------------------------------------------------------------------------------------------
void CommandNavDeleteMarked( void )
{
	if ( !UTIL_IsCommandIssuedByServerAdmin() )
		return;

	TheNavMesh->CommandNavDeleteMarked();
}
static ConCommand nav_delete_marked( "nav_delete_marked", CommandNavDeleteMarked, "Deletes the currently marked Area (if any).", FCVAR_GAMEDLL | FCVAR_CHEAT );


//--------------------------------------------------------------------------------------------------------------
void CommandNavSplit( void )
{
	if ( !UTIL_IsCommandIssuedByServerAdmin() )
		return;

	TheNavMesh->CommandNavSplit();
}
static ConCommand nav_split( "nav_split", CommandNavSplit, "To split an Area into two, align the split line using your cursor and invoke the split command.", FCVAR_GAMEDLL | FCVAR_CHEAT );


//--------------------------------------------------------------------------------------------------------------
void CommandNavMakeSniperSpots( void )
{
	if ( !UTIL_IsCommandIssuedByServerAdmin() )
		return;

	TheNavMesh->CommandNavMakeSniperSpots();
}
static ConCommand nav_make_sniper_spots( "nav_make_sniper_spots", CommandNavMakeSniperSpots, "Chops the marked area into disconnected sub-areas suitable for sniper spots.", FCVAR_GAMEDLL | FCVAR_CHEAT );


//--------------------------------------------------------------------------------------------------------------
void CommandNavMerge( void )
{
	if ( !UTIL_IsCommandIssuedByServerAdmin() )
		return;

	TheNavMesh->CommandNavMerge();
}
static ConCommand nav_merge( "nav_merge", CommandNavMerge, "To merge two Areas into one, mark the first Area, highlight the second by pointing your cursor at it, and invoke the merge command.", FCVAR_GAMEDLL | FCVAR_CHEAT );


//--------------------------------------------------------------------------------------------------------------
void CommandNavMark( const CCommand &args )
{
	if ( !UTIL_IsCommandIssuedByServerAdmin() )
		return;

	TheNavMesh->CommandNavMark( args );
}
static ConCommand nav_mark( "nav_mark", CommandNavMark, "Marks the Area or Ladder under the cursor for manipulation by subsequent editing commands.", FCVAR_GAMEDLL | FCVAR_CHEAT );


//--------------------------------------------------------------------------------------------------------------
void CommandNavUnmark( void )
{
	if ( !UTIL_IsCommandIssuedByServerAdmin() )
		return;

	TheNavMesh->CommandNavUnmark();
}
static ConCommand nav_unmark( "nav_unmark", CommandNavUnmark, "Clears the marked Area or Ladder.", FCVAR_GAMEDLL | FCVAR_CHEAT );


//--------------------------------------------------------------------------------------------------------------
void CommandNavBeginArea( void )
{
	if ( !UTIL_IsCommandIssuedByServerAdmin() )
		return;

	TheNavMesh->CommandNavBeginArea();
}
static ConCommand nav_begin_area( "nav_begin_area", CommandNavBeginArea, "Defines a corner of a new Area or Ladder. To complete the Area or Ladder, drag the opposite corner to the desired location and issue a 'nav_end_area' command.", FCVAR_GAMEDLL | FCVAR_CHEAT );


//--------------------------------------------------------------------------------------------------------------
void CommandNavEndArea( void )
{
	if ( !UTIL_IsCommandIssuedByServerAdmin() )
		return;

	TheNavMesh->CommandNavEndArea();
}
static ConCommand nav_end_area( "nav_end_area", CommandNavEndArea, "Defines the second corner of a new Area or Ladder and creates it.", FCVAR_GAMEDLL | FCVAR_CHEAT );


//--------------------------------------------------------------------------------------------------------------
void CommandNavConnect( void )
{
	if ( !UTIL_IsCommandIssuedByServerAdmin() )
		return;

	TheNavMesh->CommandNavConnect();
}
static ConCommand nav_connect( "nav_connect", CommandNavConnect, "To connect two Areas, mark the first Area, highlight the second Area, then invoke the connect command. Note that this creates a ONE-WAY connection from the first to the second Area. To make a two-way connection, also connect the second area to the first.", FCVAR_GAMEDLL | FCVAR_CHEAT );


//--------------------------------------------------------------------------------------------------------------
void CommandNavDisconnect( void )
{
	if ( !UTIL_IsCommandIssuedByServerAdmin() )
		return;

	TheNavMesh->CommandNavDisconnect();
}
static ConCommand nav_disconnect( "nav_disconnect", CommandNavDisconnect, "To disconnect two Areas, mark an Area, highlight a second Area, then invoke the disconnect command. This will remove all connections between the two Areas.", FCVAR_GAMEDLL | FCVAR_CHEAT );


//--------------------------------------------------------------------------------------------------------------
void CommandNavSplice( void )
{
	if ( !UTIL_IsCommandIssuedByServerAdmin() )
		return;

	TheNavMesh->CommandNavSplice();
}
static ConCommand nav_splice( "nav_splice", CommandNavSplice, "To splice, mark an area, highlight a second area, then invoke the splice command to create a new, connected area between them.", FCVAR_GAMEDLL | FCVAR_CHEAT );


//--------------------------------------------------------------------------------------------------------------
void CommandNavCrouch( void )
{
	if ( !UTIL_IsCommandIssuedByServerAdmin() )
		return;

	TheNavMesh->CommandNavToggleAttribute( NAV_MESH_CROUCH );
}
static ConCommand nav_crouch( "nav_crouch", CommandNavCrouch, "Toggles the 'must crouch in this area' flag used by the AI system.", FCVAR_GAMEDLL | FCVAR_CHEAT );


//--------------------------------------------------------------------------------------------------------------
void CommandNavPrecise( void )
{
	if ( !UTIL_IsCommandIssuedByServerAdmin() )
		return;

	TheNavMesh->CommandNavToggleAttribute( NAV_MESH_PRECISE );
}
static ConCommand nav_precise( "nav_precise", CommandNavPrecise, "Toggles the 'dont avoid obstacles' flag used by the AI system.", FCVAR_GAMEDLL | FCVAR_CHEAT );


//--------------------------------------------------------------------------------------------------------------
void CommandNavJump( void )
{
	if ( !UTIL_IsCommandIssuedByServerAdmin() )
		return;

	TheNavMesh->CommandNavToggleAttribute( NAV_MESH_JUMP );
}
static ConCommand nav_jump( "nav_jump", CommandNavJump, "Toggles the 'traverse this area by jumping' flag used by the AI system.", FCVAR_GAMEDLL | FCVAR_CHEAT );


//--------------------------------------------------------------------------------------------------------------
void CommandNavNoJump( void )
{
	if ( !UTIL_IsCommandIssuedByServerAdmin() )
		return;

	TheNavMesh->CommandNavToggleAttribute( NAV_MESH_NO_JUMP );
}
static ConCommand nav_no_jump( "nav_no_jump", CommandNavNoJump, "Toggles the 'dont jump in this area' flag used by the AI system.", FCVAR_GAMEDLL | FCVAR_CHEAT );


//--------------------------------------------------------------------------------------------------------------
void CommandNavStop( void )
{
	if ( !UTIL_IsCommandIssuedByServerAdmin() )
		return;

	TheNavMesh->CommandNavToggleAttribute( NAV_MESH_STOP );
}
static ConCommand nav_stop( "nav_stop", CommandNavStop, "Toggles the 'must stop when entering this area' flag used by the AI system.", FCVAR_GAMEDLL | FCVAR_CHEAT );


//--------------------------------------------------------------------------------------------------------------
void CommandNavWalk( void )
{
	if ( !UTIL_IsCommandIssuedByServerAdmin() )
		return;

	TheNavMesh->CommandNavToggleAttribute( NAV_MESH_WALK );
}
static ConCommand nav_walk( "nav_walk", CommandNavWalk, "Toggles the 'traverse this area by walking' flag used by the AI system.", FCVAR_GAMEDLL | FCVAR_CHEAT );


//--------------------------------------------------------------------------------------------------------------
void CommandNavRun( void )
{
	if ( !UTIL_IsCommandIssuedByServerAdmin() )
		return;

	TheNavMesh->CommandNavToggleAttribute( NAV_MESH_RUN );
}
static ConCommand nav_run( "nav_run", CommandNavRun, "Toggles the 'traverse this area by running' flag used by the AI system.", FCVAR_GAMEDLL | FCVAR_CHEAT );


//--------------------------------------------------------------------------------------------------------------
void CommandNavAvoid( void )
{
	if ( !UTIL_IsCommandIssuedByServerAdmin() )
		return;

	TheNavMesh->CommandNavToggleAttribute( NAV_MESH_AVOID );
}
static ConCommand nav_avoid( "nav_avoid", CommandNavAvoid, "Toggles the 'avoid this area when possible' flag used by the AI system.", FCVAR_GAMEDLL | FCVAR_CHEAT );


//--------------------------------------------------------------------------------------------------------------
void CommandNavTransient( void )
{
	if ( !UTIL_IsCommandIssuedByServerAdmin() )
		return;

	TheNavMesh->CommandNavToggleAttribute( NAV_MESH_TRANSIENT );
}
static ConCommand nav_transient( "nav_transient", CommandNavTransient, "Toggles the 'area is transient and may become blocked' flag used by the AI system.", FCVAR_GAMEDLL | FCVAR_CHEAT );


//--------------------------------------------------------------------------------------------------------------
void CommandNavDontHide( void )
{
	if ( !UTIL_IsCommandIssuedByServerAdmin() )
		return;

	TheNavMesh->CommandNavToggleAttribute( NAV_MESH_DONT_HIDE );
}
static ConCommand nav_dont_hide( "nav_dont_hide", CommandNavDontHide, "Toggles the 'area is not suitable for hiding spots' flag used by the AI system.", FCVAR_GAMEDLL | FCVAR_CHEAT );


//--------------------------------------------------------------------------------------------------------------
void CommandNavStand( void )
{
	if ( !UTIL_IsCommandIssuedByServerAdmin() )
		return;

	TheNavMesh->CommandNavToggleAttribute( NAV_MESH_STAND );
}
static ConCommand nav_stand( "nav_stand", CommandNavStand, "Toggles the 'stand while hiding' flag used by the AI system.", FCVAR_GAMEDLL | FCVAR_CHEAT );


//--------------------------------------------------------------------------------------------------------------
void CommandNavNoHostages( void )
{
	if ( !UTIL_IsCommandIssuedByServerAdmin() )
		return;

	TheNavMesh->CommandNavToggleAttribute( NAV_MESH_NO_HOSTAGES );
}
static ConCommand nav_no_hostages( "nav_no_hostages", CommandNavNoHostages, "Toggles the 'hostages cannot use this area' flag used by the AI system.", FCVAR_GAMEDLL | FCVAR_CHEAT );


//--------------------------------------------------------------------------------------------------------------
void CommandNavStrip( void )
{
	if ( !UTIL_IsCommandIssuedByServerAdmin() )
		return;

	TheNavMesh->StripNavigationAreas();
}
static ConCommand nav_strip( "nav_strip", CommandNavStrip, "Strips all Hiding Spots, Approach Points, and Encounter Spots from the current Area.", FCVAR_GAMEDLL | FCVAR_CHEAT );


//--------------------------------------------------------------------------------------------------------------
void CommandNavSave( void )
{
	if ( !UTIL_IsCommandIssuedByServerAdmin() )
		return;

	if (TheNavMesh->Save())
	{
		Msg( "Navigation map '%s' saved.\n", TheNavMesh->GetFilename() );
	}
	else
	{
		const char *filename = TheNavMesh->GetFilename();
		Msg( "ERROR: Cannot save navigation map '%s'.\n", (filename) ? filename : "(null)" );
	}
}
static ConCommand nav_save( "nav_save", CommandNavSave, "Saves the current Navigation Mesh to disk.", FCVAR_GAMEDLL | FCVAR_CHEAT );


//--------------------------------------------------------------------------------------------------------------
void CommandNavLoad( void )
{
	if ( !UTIL_IsCommandIssuedByServerAdmin() )
		return;

	if (TheNavMesh->Load() != NAV_OK)
	{
		Msg( "ERROR: Navigation Mesh load failed.\n" );
	}
}
static ConCommand nav_load( "nav_load", CommandNavLoad, "Loads the Navigation Mesh for the current map.", FCVAR_GAMEDLL | FCVAR_CHEAT );


//--------------------------------------------------------------------------------------------------------------
static int PlaceNameAutocompleteCallback( char const *partial, char commands[ COMMAND_COMPLETION_MAXITEMS ][ COMMAND_COMPLETION_ITEM_LENGTH ] )
{
	return TheNavMesh->PlaceNameAutocomplete( partial, commands );
}


//--------------------------------------------------------------------------------------------------------------
void CommandNavUsePlace( const CCommand &args )
{
	if ( !UTIL_IsCommandIssuedByServerAdmin() )
		return;

	if (args.ArgC() == 1)
	{
		// no arguments = list all available places
		TheNavMesh->PrintAllPlaces();
	}
	else
	{
		// single argument = set current place
		Place place = TheNavMesh->PartialNameToPlace( args[ 1 ] );

		if (place == UNDEFINED_PLACE)
		{
			Msg( "Ambiguous\n" );
		}
		else
		{
			Msg( "Current place set to '%s'\n", TheNavMesh->PlaceToName( place ) );
			TheNavMesh->SetNavPlace( place );
		}
	}
}
static ConCommand nav_use_place( "nav_use_place", CommandNavUsePlace, "If used without arguments, all available Places will be listed. If a Place argument is given, the current Place is set.", FCVAR_GAMEDLL | FCVAR_CHEAT, PlaceNameAutocompleteCallback );


//--------------------------------------------------------------------------------------------------------------
void CommandNavPlaceReplace( const CCommand &args )
{
	if ( !UTIL_IsCommandIssuedByServerAdmin() )
		return;

	if (args.ArgC() != 3)
	{
		// no arguments
		Msg( "Usage: nav_place_replace <OldPlace> <NewPlace>\n" );
	}
	else
	{
		// two arguments - replace the first place with the second
		Place oldPlace = TheNavMesh->PartialNameToPlace( args[ 1 ] );
		Place newPlace = TheNavMesh->PartialNameToPlace( args[ 2 ] );

		if ( oldPlace == UNDEFINED_PLACE || newPlace == UNDEFINED_PLACE )
		{
			Msg( "Ambiguous\n" );
		}
		else
		{
			FOR_EACH_LL( TheNavAreaList, it )
			{
				CNavArea *area = TheNavAreaList[it];
				if ( area->GetPlace() == oldPlace )
				{
					area->SetPlace( newPlace );
				}
			}
		}
	}
}
static ConCommand nav_place_replace( "nav_place_replace", CommandNavPlaceReplace, "Replaces all instances of the first place with the second place.", FCVAR_GAMEDLL | FCVAR_CHEAT );


//--------------------------------------------------------------------------------------------------------------
void CommandNavPlaceList( void )
{
	if ( !UTIL_IsCommandIssuedByServerAdmin() )
		return;

	CUtlVector< Place > placeDirectory;

	FOR_EACH_LL( TheNavAreaList, nit )
	{
		CNavArea *area = TheNavAreaList[ nit ];

		Place place = area->GetPlace();

		if ( place )
		{
			if ( !placeDirectory.HasElement( place ) )
			{
				placeDirectory.AddToTail( place );
			}
		}
	}

	Msg( "Map uses %d place names:\n", placeDirectory.Count() );
	for ( int i=0; i<placeDirectory.Count(); ++i )
	{
		Msg( "    %s\n", TheNavMesh->PlaceToName( placeDirectory[i] ) );
	}
}
static ConCommand nav_place_list( "nav_place_list", CommandNavPlaceList, "Lists all place names used in the map.", FCVAR_GAMEDLL | FCVAR_CHEAT );


//--------------------------------------------------------------------------------------------------------------
void CommandNavTogglePlaceMode( void )
{
	if ( !UTIL_IsCommandIssuedByServerAdmin() )
		return;

	TheNavMesh->CommandNavTogglePlaceMode();
}
static ConCommand nav_toggle_place_mode( "nav_toggle_place_mode", CommandNavTogglePlaceMode, "Toggle the editor into and out of Place mode. Place mode allows labelling of Area with Place names.", FCVAR_GAMEDLL | FCVAR_CHEAT );


//--------------------------------------------------------------------------------------------------------------
void CommandNavSetPlaceMode( const CCommand &args )
{
	if ( !UTIL_IsCommandIssuedByServerAdmin() )
		return;

	bool on = true;
	if ( args.ArgC() == 2 )
	{
		on = (atoi( args[ 1 ] ) != 0);
	}

	if ( on != TheNavMesh->IsPlaceMode() )
	{
		TheNavMesh->CommandNavTogglePlaceMode();
	}
}
static ConCommand nav_set_place_mode( "nav_set_place_mode", CommandNavSetPlaceMode, "Sets the editor into or out of Place mode. Place mode allows labelling of Area with Place names.", FCVAR_GAMEDLL | FCVAR_CHEAT );


//--------------------------------------------------------------------------------------------------------------
void CommandNavPlaceFloodFill( void )
{
	if ( !UTIL_IsCommandIssuedByServerAdmin() )
		return;

	TheNavMesh->CommandNavPlaceFloodFill();
}
static ConCommand nav_place_floodfill( "nav_place_floodfill", CommandNavPlaceFloodFill, "Sets the Place of the Area under the cursor to the curent Place, and 'flood-fills' the Place to all adjacent Areas. Flood-filling stops when it hits an Area with the same Place, or a different Place than that of the initial Area.", FCVAR_GAMEDLL | FCVAR_CHEAT );


//--------------------------------------------------------------------------------------------------------------
void CommandNavPlacePick( void )
{
	if ( !UTIL_IsCommandIssuedByServerAdmin() )
		return;

	TheNavMesh->CommandNavPlacePick();
}
static ConCommand nav_place_pick( "nav_place_pick", CommandNavPlacePick, "Sets the current Place to the Place of the Area under the cursor.", FCVAR_GAMEDLL | FCVAR_CHEAT );


//--------------------------------------------------------------------------------------------------------------
void CommandNavTogglePlacePainting( void )
{
	if ( !UTIL_IsCommandIssuedByServerAdmin() )
		return;

	TheNavMesh->CommandNavTogglePlacePainting();
}
static ConCommand nav_toggle_place_painting( "nav_toggle_place_painting", CommandNavTogglePlacePainting, "Toggles Place Painting mode. When Place Painting, pointing at an Area will 'paint' it with the current Place.", FCVAR_GAMEDLL | FCVAR_CHEAT );


//--------------------------------------------------------------------------------------------------------------
void CommandNavMarkUnnamed( void )
{
	if ( !UTIL_IsCommandIssuedByServerAdmin() )
		return;

	TheNavMesh->CommandNavMarkUnnamed();
}
static ConCommand nav_mark_unnamed( "nav_mark_unnamed", CommandNavMarkUnnamed, "Mark an Area with no Place name. Useful for finding stray areas missed when Place Painting.", FCVAR_GAMEDLL | FCVAR_CHEAT );


//--------------------------------------------------------------------------------------------------------------
void CommandNavCornerSelect( void )
{
	if ( !UTIL_IsCommandIssuedByServerAdmin() )
		return;

	TheNavMesh->CommandNavCornerSelect();
}
static ConCommand nav_corner_select( "nav_corner_select", CommandNavCornerSelect, "Select a corner of the currently marked Area. Use multiple times to access all four corners.", FCVAR_GAMEDLL | FCVAR_CHEAT );


//--------------------------------------------------------------------------------------------------------------
void CommandNavCornerRaise( void )
{
	if ( !UTIL_IsCommandIssuedByServerAdmin() )
		return;

	TheNavMesh->CommandNavCornerRaise();
}
static ConCommand nav_corner_raise( "nav_corner_raise", CommandNavCornerRaise, "Raise the selected corner of the currently marked Area.", FCVAR_GAMEDLL | FCVAR_CHEAT );


//--------------------------------------------------------------------------------------------------------------
void CommandNavCornerLower( void )
{
	if ( !UTIL_IsCommandIssuedByServerAdmin() )
		return;

	TheNavMesh->CommandNavCornerLower();
}
static ConCommand nav_corner_lower( "nav_corner_lower", CommandNavCornerLower, "Lower the selected corner of the currently marked Area.", FCVAR_GAMEDLL | FCVAR_CHEAT );


//--------------------------------------------------------------------------------------------------------------
void CommandNavCornerPlaceOnGround( const CCommand &args )
{
	if ( !UTIL_IsCommandIssuedByServerAdmin() )
		return;

	TheNavMesh->CommandNavCornerPlaceOnGround( args );
}
static ConCommand nav_corner_place_on_ground( "nav_corner_place_on_ground", CommandNavCornerPlaceOnGround, "Places the selected corner of the currently marked Area on the ground.", FCVAR_GAMEDLL | FCVAR_CHEAT );


//--------------------------------------------------------------------------------------------------------------
void CommandNavWarpToMark( void )
{
	if ( !UTIL_IsCommandIssuedByServerAdmin() )
		return;

	TheNavMesh->CommandNavWarpToMark();
}
static ConCommand nav_warp_to_mark( "nav_warp_to_mark", CommandNavWarpToMark, "Warps the player to the marked area.", FCVAR_GAMEDLL | FCVAR_CHEAT );


//--------------------------------------------------------------------------------------------------------------
void CommandNavLadderFlip( void )
{
	if ( !UTIL_IsCommandIssuedByServerAdmin() )
		return;

	TheNavMesh->CommandNavLadderFlip();
}
static ConCommand nav_ladder_flip( "nav_ladder_flip", CommandNavLadderFlip, "Flips the selected ladder's direction.", FCVAR_GAMEDLL | FCVAR_CHEAT );


//--------------------------------------------------------------------------------------------------------------
void CommandNavGenerate( void )
{
	if ( !UTIL_IsCommandIssuedByServerAdmin() )
		return;

	TheNavMesh->BeginGeneration();
}
static ConCommand nav_generate( "nav_generate", CommandNavGenerate, "Generate a Navigation Mesh for the current map and save it to disk.", FCVAR_GAMEDLL | FCVAR_CHEAT );


//--------------------------------------------------------------------------------------------------------------
void CommandNavGenerateIncremental( void )
{
	if ( !UTIL_IsCommandIssuedByServerAdmin() )
		return;

	TheNavMesh->BeginGeneration( true );
}
static ConCommand nav_generate_incremental( "nav_generate_incremental", CommandNavGenerateIncremental, "Generate a Navigation Mesh for the current map and save it to disk.", FCVAR_GAMEDLL | FCVAR_CHEAT );


//--------------------------------------------------------------------------------------------------------------
void CommandNavAnalyze( void )
{
	if ( !UTIL_IsCommandIssuedByServerAdmin() )
		return;

	TheNavMesh->BeginAnalysis();
}
static ConCommand nav_analyze( "nav_analyze", CommandNavAnalyze, "Re-analyze the current Navigation Mesh and save it to disk.", FCVAR_GAMEDLL | FCVAR_CHEAT );


//--------------------------------------------------------------------------------------------------------------
void CommandNavMarkWalkable( void )
{
	if ( !UTIL_IsCommandIssuedByServerAdmin() )
		return;

	Vector pos;

	if (nav_edit.GetBool())
	{
		// we are in edit mode, use the edit cursor's location
		pos = TheNavMesh->GetEditCursorPosition();
	}
	else
	{
		// we are not in edit mode, use the position of the local player
		CBasePlayer *player = UTIL_GetListenServerHost();

		if (player == NULL)
		{
			Msg( "ERROR: No local player!\n" );
			return;
		}

		pos = player->GetAbsOrigin();
	}

	// snap position to the sampling grid
	pos.x = TheNavMesh->SnapToGrid( pos.x, true );
	pos.y = TheNavMesh->SnapToGrid( pos.y, true );

	Vector normal;
	if (TheNavMesh->GetGroundHeight( pos, &pos.z, &normal ) == false)
	{
		Msg( "ERROR: Invalid ground position.\n" );
		return;
	}

	TheNavMesh->AddWalkableSeed( pos, normal );

	Msg( "Walkable position marked.\n" );
}
static ConCommand nav_mark_walkable( "nav_mark_walkable", CommandNavMarkWalkable, "Mark the current location as a walkable position. These positions are used as seed locations when sampling the map to generate a Navigation Mesh.", FCVAR_GAMEDLL | FCVAR_CHEAT );


//--------------------------------------------------------------------------------------------------------------
void CommandNavClearWalkableMarks( void )
{
	if ( !UTIL_IsCommandIssuedByServerAdmin() )
		return;

	TheNavMesh->ClearWalkableSeeds();
}
static ConCommand nav_clear_walkable_marks( "nav_clear_walkable_marks", CommandNavClearWalkableMarks, "Erase any previously placed walkable positions.", FCVAR_GAMEDLL | FCVAR_CHEAT );


//--------------------------------------------------------------------------------------------------------------
void CommandNavCompressID( void )
{
	if ( !UTIL_IsCommandIssuedByServerAdmin() )
		return;

	CNavArea::CompressIDs();
	CNavLadder::CompressIDs();
}
static ConCommand nav_compress_id( "nav_compress_id", CommandNavCompressID, "Re-orders area and ladder ID's so they are continuous.", FCVAR_GAMEDLL | FCVAR_CHEAT );


//--------------------------------------------------------------------------------------------------------------
void CommandNavShowLadderBounds( void )
{
	if ( !UTIL_IsCommandIssuedByServerAdmin() )
		return;

	CInfoLadder *ladder = NULL;
	while( (ladder = dynamic_cast< CInfoLadder * >(gEntList.FindEntityByClassname( ladder, "info_ladder" ))) != NULL )
	{
		NDebugOverlay::Box( vec3_origin, ladder->mins, ladder->maxs, 0, 255, 0, 0, 600 );
		NDebugOverlay::Box( ladder->GetAbsOrigin(), ladder->mins, ladder->maxs, 0, 0, 255, 0, 600 );
	}
}
static ConCommand nav_show_ladder_bounds( "nav_show_ladder_bounds", CommandNavShowLadderBounds, "Draws the bounding boxes of all func_ladders in the map.", FCVAR_GAMEDLL | FCVAR_CHEAT );


//--------------------------------------------------------------------------------------------------------------
void CommandNavBuildLadder( void )
{
	if ( !UTIL_IsCommandIssuedByServerAdmin() )
		return;

	TheNavMesh->CommandNavBuildLadder();
}
static ConCommand nav_build_ladder( "nav_build_ladder", CommandNavBuildLadder, "Attempts to build a nav ladder on the climbable surface under the cursor.", FCVAR_GAMEDLL | FCVAR_CHEAT );


/* IN PROGRESS:
//--------------------------------------------------------------------------------------------------------------
void CommandNavPickArea( void )
{
	if ( !UTIL_IsCommandIssuedByServerAdmin() )
		return;

	TheNavMesh->CommandNavPickArea();
}
static ConCommand nav_pick_area( "nav_pick_area", CommandNavPickArea, "Marks an area (and corner) based on the surface under the cursor.", FCVAR_GAMEDLL | FCVAR_CHEAT );


//--------------------------------------------------------------------------------------------------------------
void CommandNavResizeHorizontal( void )
{
	if ( !UTIL_IsCommandIssuedByServerAdmin() )
		return;

	TheNavMesh->CommandNavResizeHorizontal();
}
static ConCommand nav_resize_horizontal( "nav_resize_horizontal", CommandNavResizeHorizontal, "TODO", FCVAR_GAMEDLL | FCVAR_CHEAT );


//--------------------------------------------------------------------------------------------------------------
void CommandNavResizeVertical( void )
{
	if ( !UTIL_IsCommandIssuedByServerAdmin() )
		return;

	TheNavMesh->CommandNavResizeVertical();
}
static ConCommand nav_resize_vertical( "nav_resize_vertical", CommandNavResizeVertical, "TODO", FCVAR_GAMEDLL | FCVAR_CHEAT );


//--------------------------------------------------------------------------------------------------------------
void CommandNavResizeEnd( void )
{
	if ( !UTIL_IsCommandIssuedByServerAdmin() )
		return;

	TheNavMesh->CommandNavResizeEnd();
}
static ConCommand nav_resize_end( "nav_resize_end", CommandNavResizeEnd, "TODO", FCVAR_GAMEDLL | FCVAR_CHEAT );
*/


//--------------------------------------------------------------------------------------------------------------
/**
 * Destroy ladder representations
 */
void CNavMesh::DestroyLadders( void )
{
	FOR_EACH_LL( m_ladderList, it )
	{
		delete m_ladderList[ it ];
	}

	m_ladderList.RemoveAll();

	m_markedLadder = NULL;
	m_selectedLadder = NULL;
}

//--------------------------------------------------------------------------------------------------------------
/**
 * Strip the "analyzed" data out of all navigation areas
 */
void CNavMesh::StripNavigationAreas( void )
{
	FOR_EACH_LL( TheNavAreaList, it )
	{
		CNavArea *area = TheNavAreaList[ it ];

		area->Strip();
	}
}

//--------------------------------------------------------------------------------------------------------------

HidingSpotList TheHidingSpotList;
unsigned int HidingSpot::m_nextID = 1;
unsigned int HidingSpot::m_masterMarker = 0;


//--------------------------------------------------------------------------------------------------------------
/**
 * Hiding Spot factory
 */
HidingSpot *CNavMesh::CreateHidingSpot( void ) const
{
	return new HidingSpot;
}


//--------------------------------------------------------------------------------------------------------------
void CNavMesh::DestroyHidingSpots( void )
{
	// remove all hiding spot references from the nav areas
	FOR_EACH_LL( TheNavAreaList, it )
	{
		CNavArea *area = TheNavAreaList[ it ];

		area->m_hidingSpotList.RemoveAll();
	}

	HidingSpot::m_nextID = 0;

	// free all the HidingSpots
	FOR_EACH_LL( TheHidingSpotList, hit )
	{
		delete TheHidingSpotList[ hit ];
	}

	TheHidingSpotList.RemoveAll();
}

//--------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------
/**
 * Construct a Hiding Spot.  Assign a unique ID which may be overwritten if loaded.
 */
HidingSpot::HidingSpot( void )
{
	m_pos = Vector( 0, 0, 0 );
	m_id = m_nextID++;
	m_flags = 0;
	m_area = NULL;

	TheHidingSpotList.AddToTail( this );
}


//--------------------------------------------------------------------------------------------------------------
void HidingSpot::Save( FileHandle_t file, unsigned int version ) const
{
	filesystem->Write( &m_id, sizeof(unsigned int), file );
	filesystem->Write( &m_pos, 3 * sizeof(float), file );
	filesystem->Write( &m_flags, sizeof(unsigned char), file );
}


//--------------------------------------------------------------------------------------------------------------
void HidingSpot::Load( FileHandle_t file, unsigned int version )
{
	filesystem->Read( &m_id, sizeof(unsigned int), file );
	filesystem->Read( &m_pos, 3 * sizeof(float), file );
	filesystem->Read( &m_flags, sizeof(unsigned char), file );

	// update next ID to avoid ID collisions by later spots
	if (m_id >= m_nextID)
		m_nextID = m_id+1;
}


//--------------------------------------------------------------------------------------------------------------
/**
 * Hiding Spot post-load processing
 */
NavErrorType HidingSpot::PostLoad( void )
{
	// set our area
	m_area = TheNavMesh->GetNavArea( m_pos + Vector( 0, 0, HalfHumanHeight ) );

	AssertMsg( m_area, UTIL_VarArgs("A Hiding Spot is off of the Nav Mesh at (%.0f %.0f %.0f)", m_pos.x, m_pos.y, m_pos.z) );

	return NAV_OK;
}


//--------------------------------------------------------------------------------------------------------------
/**
 * Given a HidingSpot ID, return the associated HidingSpot
 */
HidingSpot *GetHidingSpotByID( unsigned int id )
{
	FOR_EACH_LL( TheHidingSpotList, it )
	{
		HidingSpot *spot = TheHidingSpotList[ it ];

		if (spot->GetID() == id)
			return spot;
	}	

	return NULL;
}



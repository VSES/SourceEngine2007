//========= Copyright © 1996-2005, Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
// $NoKeywords: $
//
//=============================================================================//
// nav_generate.cpp
// Auto-generate a Navigation Mesh by sampling the current map
// Author: Michael S. Booth (mike@turtlerockstudios.com), 2003

#include "cbase.h"
#include "util_shared.h"
#include "nav_mesh.h"
#include "nav_node.h"
#include "nav_pathfind.h"
#include "viewport_panel_names.h"

enum { MAX_BLOCKED_AREAS = 256 };
static unsigned int blockedID[ MAX_BLOCKED_AREAS ];
static int blockedIDCount = 0;
static float lastMsgTime = 0.0f;


ConVar nav_slope_limit( "nav_slope_limit", "0.7", FCVAR_GAMEDLL, "The ground unit normal's Z component must be greater than this for nav areas to be generated." );
ConVar nav_restart_after_analysis( "nav_restart_after_analysis", "1", FCVAR_GAMEDLL, "When nav nav_restart_after_analysis finishes, restart the server.  Turning this off can cause crashes, but is useful for incremental generation." );


//--------------------------------------------------------------------------------------------------------------
inline float round( float val, float unit )
{
	val = val + ((val < 0.0f) ? -unit*0.5f : unit*0.5f);
	return (float)( ((int)val) / (int)unit );
}


//--------------------------------------------------------------------------------------------------------------
/**
 * Shortest path cost, paying attention to "blocked" areas
 */
class ApproachAreaCost
{
public:
	float operator() ( CNavArea *area, CNavArea *fromArea, const CNavLadder *ladder )
	{
		// check if this area is "blocked"
		for( int i=0; i<blockedIDCount; ++i )
		{
			if (area->GetID() == blockedID[i])
			{
				return -1.0f;
			}
		}

		if (fromArea == NULL)
		{
			// first area in path, no cost
			return 0.0f;
		}
		else
		{
			// compute distance traveled along path so far
			float dist;

			if (ladder)
			{
				dist = ladder->m_length;
			}
			else
			{
				dist = (area->GetCenter() - fromArea->GetCenter()).Length();
			}

			float cost = dist + fromArea->GetCostSoFar();

			return cost;
		}
	}
};


//--------------------------------------------------------------------------------------------------------------
/**
 * Determine the set of "approach areas".
 * An approach area is an area representing a place where players 
 * move into/out of our local neighborhood of areas.
 * @todo Optimize by search from eye outward and modifying pathfinder to treat all links as bi-directional
 */
void CNavArea::ComputeApproachAreas( void )
{
	m_approachCount = 0;

	if (nav_quicksave.GetBool())
		return;

	// use the center of the nav area as the "view" point
	Vector eye = m_center;
	if (TheNavMesh->GetGroundHeight( eye, &eye.z ) == false)
		return;

	// approximate eye position
	if (GetAttributes() & NAV_MESH_CROUCH)
		eye.z += 0.9f * HalfHumanHeight;
	else
		eye.z += 0.9f * HumanHeight;

	enum { MAX_PATH_LENGTH = 256 };
	CNavArea *path[ MAX_PATH_LENGTH ];
	ApproachAreaCost cost;

	enum SearchType
	{
		FROM_EYE,		///< start search from our eyepoint outward to farArea
		TO_EYE,			///< start search from farArea beack towards our eye
		SEARCH_FINISHED
	};

	//
	// In order to *completely* enumerate all of the approach areas, we
	// need to search from our eyepoint outward, as well as from outwards
	// towards our eyepoint
	//
	for( int searchType = FROM_EYE; searchType != SEARCH_FINISHED; ++searchType )
	{
		//
		// In order to enumerate all of the approach areas, we need to
		// run the algorithm many times, once for each "far away" area
		// and keep the union of the approach area sets
		//
		int it;
		for( it = TheNavAreaList.Head(); it != TheNavAreaList.InvalidIndex(); it = TheNavAreaList.Next( it ) )
		{
			CNavArea *farArea = TheNavAreaList[ it ];

			blockedIDCount = 0;

			// skip the small areas
			const float minSize = 200.0f;		// 150
			const Extent &extent = farArea->GetExtent();
			if (extent.SizeX() < minSize || extent.SizeY() < minSize)
			{
				continue;
			}

			// if we can see 'farArea', try again - the whole point is to go "around the bend", so to speak
			if (farArea->IsVisible( eye ))
			{
				continue;
			}

			//
			// Keep building paths to farArea and blocking them off until we
			// cant path there any more.
			// As areas are blocked off, all exits will be enumerated.
			//
			while( m_approachCount < MAX_APPROACH_AREAS )
			{
				CNavArea *from, *to;

				if (searchType == FROM_EYE)
				{
					// find another path *to* 'farArea'
					// we must pathfind from us in order to pick up one-way paths OUT OF our area
					from = this;
					to = farArea;
				}
				else // TO_EYE
				{
					// find another path *from* 'farArea'
					// we must pathfind to us in order to pick up one-way paths INTO our area
					from = farArea;
					to = this;
				}

				// build the actual path
				if (NavAreaBuildPath( from, to, NULL, cost ) == false)
				{
					break;
				}

				// find number of areas on path
				int count = 0;
				CNavArea *area;
				for( area = to; area; area = area->GetParent() )
				{
					++count;
				}

				if (count > MAX_PATH_LENGTH)
				{
					count = MAX_PATH_LENGTH;
				}

				// if the path is only two areas long, there can be no approach points
				if (count <= 2)
				{
					break;
				}

				// build path starting from eye
				int i = 0;

				if (searchType == FROM_EYE)
				{
					for( area = to; i < count && area; area = area->GetParent() )
					{
						path[ count-i-1 ] = area;
						++i;
					}
				}
				else // TO_EYE
				{
					for( area = to; i < count && area; area = area->GetParent() )
					{
						path[ i++ ] = area;
					}
				}

				// traverse path to find first area we cannot see (skip the first area)
				for( i=1; i<count; ++i )
				{
					// if we see this area, continue on
					if (path[i]->IsVisible( eye ))
					{
						continue;
					}

					// we can't see this area - mark this area as "blocked" and unusable by subsequent approach paths
					if (blockedIDCount == MAX_BLOCKED_AREAS)
					{
						Msg( "Overflow computing approach areas for area #%d.\n", m_id );
						return;
					}

					// if the area to be blocked is actually farArea, block the one just prior
					// (blocking farArea will cause all subsequent pathfinds to fail)
					int block = (path[i] == farArea) ? i-1 : i;

					// dont block the start area, or all subsequence pathfinds will fail
					if (block == 0)
					{
						continue;
					}

					blockedID[ blockedIDCount++ ] = path[ block ]->GetID();

					// store new approach area if not already in set
					int a;
					for( a=0; a<m_approachCount; ++a )
					{
						if (m_approach[a].here.area == path[block-1])
						{
							break;
						}
					}

					if (a == m_approachCount)
					{
						m_approach[ m_approachCount ].prev.area = (block >= 2) ? path[block-2] : NULL;

						m_approach[ m_approachCount ].here.area = path[block-1];
						m_approach[ m_approachCount ].prevToHereHow = path[block-1]->GetParentHow();

						m_approach[ m_approachCount ].next.area = path[block];
						m_approach[ m_approachCount ].hereToNextHow = path[block]->GetParentHow();

						++m_approachCount;
					}

					// we are done with this path
					break;
				}
			}
		}
	}
}


//--------------------------------------------------------------------------------------------------------------
/**
 * Start at given position and find first area in given direction
 */
inline CNavArea *findFirstAreaInDirection( const Vector *start, NavDirType dir, float range, float beneathLimit, CBaseEntity *traceIgnore = NULL, Vector *closePos = NULL )
{
	CNavArea *area = NULL;

	Vector pos = *start;

	int end = (int)((range / GenerationStepSize) + 0.5f);

	for( int i=1; i<=end; i++ )
	{
		AddDirectionVector( &pos, dir, GenerationStepSize );

		// make sure we dont look thru the wall
		trace_t result;

		UTIL_TraceLine( *start, pos, MASK_PLAYERSOLID_BRUSHONLY, traceIgnore, COLLISION_GROUP_NONE, &result );

		if (result.fraction < 1.0f)
			break;

		area = TheNavMesh->GetNavArea( pos, beneathLimit );
		if (area)
		{
			if (closePos)
			{
				closePos->x = pos.x;
				closePos->y = pos.y;
				closePos->z = area->GetZ( pos.x, pos.y );
			}

			break;
		}
	}

	return area;
}


//--------------------------------------------------------------------------------------------------------------
/**
 * For each ladder in the map, create a navigation representation of it.
 */
void CNavMesh::BuildLadders( void )
{
	// remove any left-over ladders
	DestroyLadders();

	trace_t result;

	CInfoLadder *entity = NULL;
	while( (entity = dynamic_cast< CInfoLadder * >(gEntList.FindEntityByClassname( entity, "info_ladder" ))) != NULL )
	{
		CreateLadder( entity->WorldAlignMins(), entity->WorldAlignMaxs(), NULL );
	}
}


//--------------------------------------------------------------------------------------------------------------
/**
 * Create a navigation representation of a ladder.
 */
void CNavMesh::CreateLadder( const Vector& absMin, const Vector& absMax, const Vector2D *ladderDir )
{
	CNavLadder *ladder = new CNavLadder;

	// compute top & bottom of ladder
	ladder->m_top.x = (absMin.x + absMax.x) / 2.0f;
	ladder->m_top.y = (absMin.y + absMax.y) / 2.0f;
	ladder->m_top.z = absMax.z;

	ladder->m_bottom.x = ladder->m_top.x;
	ladder->m_bottom.y = ladder->m_top.y;
	ladder->m_bottom.z = absMin.z;

	// determine facing - assumes "normal" runged ladder
	float xSize = absMax.x - absMin.x;
	float ySize = absMax.y - absMin.y;
	trace_t result;
	if (xSize > ySize)
	{
		// ladder is facing north or south - determine which way
		// "pull in" traceline from bottom and top in case ladder abuts floor and/or ceiling
		Vector from = ladder->m_bottom + Vector( 0.0f, GenerationStepSize, GenerationStepSize/2 );
		Vector to = ladder->m_top + Vector( 0.0f, GenerationStepSize, -GenerationStepSize/2 );

		UTIL_TraceLine( from, to, MASK_PLAYERSOLID_BRUSHONLY, NULL, COLLISION_GROUP_NONE, &result );

		if (result.fraction != 1.0f || result.startsolid)
			ladder->SetDir( NORTH );
		else
			ladder->SetDir( SOUTH );

		ladder->m_width = xSize;
	}
	else
	{
		// ladder is facing east or west - determine which way
		Vector from = ladder->m_bottom + Vector( GenerationStepSize, 0.0f, GenerationStepSize/2 );
		Vector to = ladder->m_top + Vector( GenerationStepSize, 0.0f, -GenerationStepSize/2 );

		UTIL_TraceLine( from, to, MASK_PLAYERSOLID_BRUSHONLY, NULL, COLLISION_GROUP_NONE, &result );

		if (result.fraction != 1.0f || result.startsolid)
			ladder->SetDir( WEST );
		else
			ladder->SetDir( EAST );

		ladder->m_width = ySize;
	}

	// adjust top and bottom of ladder to make sure they are reachable
	// (cs_office has a crate right in front of the base of a ladder)
	Vector along = ladder->m_top - ladder->m_bottom;
	float length = along.NormalizeInPlace();
	Vector on, out;
	const float minLadderClearance = 32.0f;

	// adjust bottom to bypass blockages
	const float inc = 10.0f;
	float t;		
	for( t = 0.0f; t <= length; t += inc )
	{
		on = ladder->m_bottom + t * along;

		out = on + ladder->GetNormal() * minLadderClearance;

		UTIL_TraceLine( on, out, MASK_PLAYERSOLID_BRUSHONLY, NULL, COLLISION_GROUP_NONE, &result );

		if (result.fraction == 1.0f && !result.startsolid)
		{
			// found viable ladder bottom
			ladder->m_bottom = on;
			break;
		}
	}

	// adjust top to bypass blockages
	for( t = 0.0f; t <= length; t += inc )
	{
		on = ladder->m_top - t * along;

		out = on + ladder->GetNormal() * minLadderClearance;

		UTIL_TraceLine( on, out, MASK_PLAYERSOLID_BRUSHONLY, NULL, COLLISION_GROUP_NONE, &result );

		if (result.fraction == 1.0f && !result.startsolid)
		{
			// found viable ladder top
			ladder->m_top = on;
			break;
		}
	}

	ladder->m_length = (ladder->m_top - ladder->m_bottom).Length();

	ladder->SetDir( ladder->GetDir() );	// now that we've adjusted the top and bottom, re-check the normal

	ladder->m_bottomArea = NULL;
	ladder->m_topForwardArea = NULL;
	ladder->m_topLeftArea = NULL;
	ladder->m_topRightArea = NULL;
	ladder->m_topBehindArea = NULL;
	ladder->ConnectGeneratedLadder();

	// add ladder to global list
	m_ladderList.AddToTail( ladder );		
}


//--------------------------------------------------------------------------------------------------------------
void CNavLadder::ConnectGeneratedLadder( void )
{
	const float nearLadderRange = 75.0f;		// 50

	//
	// Find naviagtion area at bottom of ladder
	//

	// get approximate postion of player on ladder
	Vector center = m_bottom + Vector( 0, 0, GenerationStepSize );
	AddDirectionVector( &center, m_dir, HalfHumanWidth );

	m_bottomArea = TheNavMesh->GetNearestNavArea( center, true );
	if (!m_bottomArea)
	{
		DevMsg( "ERROR: Unconnected ladder bottom at ( %g, %g, %g )\n", m_bottom.x, m_bottom.y, m_bottom.z );
	}
	else
	{
		// store reference to ladder in the area
		m_bottomArea->AddLadderUp( this );
	}

	//
	// Find adjacent navigation areas at the top of the ladder
	//

	// get approximate postion of player on ladder
	center = m_top + Vector( 0, 0, GenerationStepSize );
	AddDirectionVector( &center, m_dir, HalfHumanWidth );

	float beneathLimit = min( 120.0f, m_top.z - m_bottom.z + HalfHumanWidth );

	// find "ahead" area
	m_topForwardArea = findFirstAreaInDirection( &center, OppositeDirection( m_dir ), nearLadderRange, beneathLimit, NULL );
	if (m_topForwardArea == m_bottomArea)
		m_topForwardArea = NULL;

	// find "left" area
	m_topLeftArea = findFirstAreaInDirection( &center, DirectionLeft( m_dir ), nearLadderRange, beneathLimit, NULL );
	if (m_topLeftArea == m_bottomArea)
		m_topLeftArea = NULL;

	// find "right" area
	m_topRightArea = findFirstAreaInDirection( &center, DirectionRight( m_dir ), nearLadderRange, beneathLimit, NULL );
	if (m_topRightArea == m_bottomArea)
		m_topRightArea = NULL;

	// find "behind" area - must look farther, since ladder is against the wall away from this area
	m_topBehindArea = findFirstAreaInDirection( &center, m_dir, 2.0f*nearLadderRange, beneathLimit, NULL );
	if (m_topBehindArea == m_bottomArea)
		m_topBehindArea = NULL;

	// can't include behind area, since it is not used when going up a ladder
	if (!m_topForwardArea && !m_topLeftArea && !m_topRightArea)
		DevMsg( "ERROR: Unconnected ladder top at ( %g, %g, %g )\n", m_top.x, m_top.y, m_top.z );

	// store reference to ladder in the area(s)
	if (m_topForwardArea)
		m_topForwardArea->AddLadderDown( this );

	if (m_topLeftArea)
		m_topLeftArea->AddLadderDown( this );

	if (m_topRightArea)
		m_topRightArea->AddLadderDown( this );

	if (m_topBehindArea)
	{
		m_topBehindArea->AddLadderDown( this );
		Disconnect( m_topBehindArea );
	}

	// adjust top of ladder to highest connected area
	float topZ = m_bottom.z + 5.0f;
	bool topAdjusted = false;
	CNavArea *topAreaList[4];
	topAreaList[0] = m_topForwardArea;
	topAreaList[1] = m_topLeftArea;
	topAreaList[2] = m_topRightArea;
	topAreaList[3] = m_topBehindArea;

	for( int a=0; a<4; ++a )
	{
		CNavArea *topArea = topAreaList[a];
		if (topArea == NULL)
			continue;

		Vector close;
		topArea->GetClosestPointOnArea( m_top, &close );
		if (topZ < close.z)
		{
			topZ = close.z;
			topAdjusted = true;
		}
	}

	if (topAdjusted)
	{
		m_top.z = topZ;
	}

	//
	// Determine whether this ladder is "dangling" or not
	// "Dangling" ladders are too high to go up
	//
	if (m_bottomArea)
	{
		Vector bottomSpot;
		m_bottomArea->GetClosestPointOnArea( m_bottom, &bottomSpot );
		if (m_bottom.z - bottomSpot.z > HumanHeight)
		{
			m_bottomArea->Disconnect( this );
		}
	}
}


//--------------------------------------------------------------------------------------------------------------
/**
 * Mark all areas that require a jump to get through them.
 * This currently relies on jump areas having extreme slope.
 */
void CNavMesh::MarkJumpAreas( void )
{
	FOR_EACH_LL( TheNavAreaList, it )
	{
		CNavArea *area = TheNavAreaList[ it ];
		if ( !area->HasNodes() )
			continue;

		Vector normal, otherNormal;
		area->ComputeNormal( &normal );
		area->ComputeNormal( &otherNormal, true );

		if (normal.z < nav_slope_limit.GetFloat() ||
			otherNormal.z < nav_slope_limit.GetFloat())
		{
			area->SetAttributes( area->GetAttributes() | NAV_MESH_JUMP );
		}
	}
}

//--------------------------------------------------------------------------------------------------------------
/**
 * Jump areas that are connected to only one non-jump area won't be used.  Delete them.
 */
void CNavMesh::RemoveUnusedJumpAreas( void )
{
	CUtlLinkedList< CNavArea *, int > unusedAreas;

	FOR_EACH_LL( TheNavAreaList, it )
	{
		CNavArea *testArea = TheNavAreaList[ it ];
		if ( !(testArea->GetAttributes() & NAV_MESH_JUMP) )
		{
			continue;
		}

		NavConnect connectedArea;
		NavLadderConnect connectedLadder;
		connectedArea.area = NULL;
		connectedLadder.ladder = NULL;

		bool doubleConnected = false;

		// Look at testArea->ladder connections
		int i;
		for ( i=0; i<CNavLadder::NUM_LADDER_DIRECTIONS; ++i )
		{
			const NavLadderConnectList *ladderList = testArea->GetLadderList( (CNavLadder::LadderDirectionType)i );
			if ( !ladderList )
			{
				continue;
			}

			if ( ladderList->Count() == 0 )
			{
				continue;
			}

			if ( ladderList->Count() > 1 )
			{
				doubleConnected = true;
				break;
			}

			NavLadderConnect ladderConnect = (*ladderList)[ ladderList->Head() ];
			if ( connectedArea.area || (connectedLadder.ladder && connectedLadder.ladder != ladderConnect.ladder) )
			{
				doubleConnected = true;
				break;
			}

			connectedLadder = ladderConnect;
		}

		if ( doubleConnected )
		{
			continue;
		}

		// Look at testArea->area connections
		for ( i=0; i<NUM_DIRECTIONS; ++i )
		{
			const NavConnectList *areaList = testArea->GetAdjacentList( (NavDirType)i );
			if ( !areaList )
			{
				continue;
			}

			if ( areaList->Count() == 0 )
			{
				continue;
			}

			FOR_EACH_LL ( (*areaList), ait )
			{
				NavConnect areaConnect = (*areaList)[ ait ];
				if ( areaConnect.area->GetAttributes() & NAV_MESH_JUMP )
				{
					continue;
				}

				if ( connectedLadder.ladder || (connectedArea.area && connectedArea.area != areaConnect.area) )
				{
					doubleConnected = true;
					break;
				}

				connectedArea = areaConnect;
			}
		}

		if ( doubleConnected )
		{
			continue;
		}

		// Look at ladder->testArea connections
		FOR_EACH_LL( m_ladderList, lit )
		{
			CNavLadder *ladder = m_ladderList[lit];
			if ( !ladder )
			{
				continue;
			}

			if ( !ladder->IsConnected( testArea, CNavLadder::NUM_LADDER_DIRECTIONS ) )
			{
				continue;
			}

			if ( connectedArea.area )
			{
				doubleConnected = true;
				break;
			}

			if ( ladder == connectedLadder.ladder )
			{
				continue;
			}

			if ( connectedLadder.ladder )
			{
				doubleConnected = true;
				break;
			}

			connectedLadder.ladder = ladder;
		}

		if ( doubleConnected )
		{
			continue;
		}

		// Look at area->testArea connections
		FOR_EACH_LL( TheNavAreaList, ait )
		{
			CNavArea *area = TheNavAreaList[ait];
			if ( !area )
			{
				continue;
			}

			if ( area->GetAttributes() & NAV_MESH_JUMP )
			{
				continue;
			}

			if ( !area->IsConnected( testArea, NUM_DIRECTIONS ) )
			{
				continue;
			}

			if ( connectedLadder.ladder )
			{
				doubleConnected = true;
				break;
			}

			if ( area == connectedArea.area )
			{
				continue;
			}

			if ( connectedArea.area )
			{
				doubleConnected = true;
				break;
			}

			connectedArea.area = area;
		}

		if ( doubleConnected )
		{
			continue;
		}

		// Since we got here, at most one ladder or non-jump area is connected to us, so we can be deleted.
		unusedAreas.AddToTail( testArea );
	}

	FOR_EACH_LL( unusedAreas, uit )
	{
		CNavArea *areaToDelete = unusedAreas[ uit ];
		TheNavAreaList.FindAndRemove( areaToDelete );
		delete areaToDelete;
	}

	StripNavigationAreas();

	SetMarkedArea( NULL );			// unmark the mark area
	m_markedCorner = NUM_CORNERS;	// clear the corner selection
}


//--------------------------------------------------------------------------------------------------------------
void CNavMesh::CommandNavRemoveUnusedJumpAreas( void )
{
	RemoveUnusedJumpAreas();
}


//--------------------------------------------------------------------------------------------------------------
/**
 * Recursively chop area in half along X until child areas are roughly square
 */
static void splitX( CNavArea *area )
{
	if (area->IsRoughlySquare())
		return;

	float split = area->GetSizeX();
	split /= 2.0f;
	split += area->GetExtent().lo.x;

	split = TheNavMesh->SnapToGrid( split );

	const float epsilon = 0.1f;
	if (fabs(split - area->GetExtent().lo.x) < epsilon ||
		fabs(split - area->GetExtent().hi.x) < epsilon)
	{
		// too small to subdivide
		return;
	}

	CNavArea *alpha, *beta;
	if (area->SplitEdit( false, split, &alpha, &beta ))
	{
		// split each new area until square
		splitX( alpha );
		splitX( beta );
	}
}

//--------------------------------------------------------------------------------------------------------------
/**
 * Recursively chop area in half along Y until child areas are roughly square
 */
static void splitY( CNavArea *area )
{
	if (area->IsRoughlySquare())
		return;

	float split = area->GetSizeY();
	split /= 2.0f;
	split += area->GetExtent().lo.y;

	split = TheNavMesh->SnapToGrid( split );

	const float epsilon = 0.1f;
	if (fabs(split - area->GetExtent().lo.y) < epsilon ||
		fabs(split - area->GetExtent().hi.y) < epsilon)
	{
		// too small to subdivide
		return;
	}

	CNavArea *alpha, *beta;
	if (area->SplitEdit( true, split, &alpha, &beta ))
	{
		// split each new area until square
		splitY( alpha );
		splitY( beta );
	}
}

//--------------------------------------------------------------------------------------------------------------
/**
 * Split any long, thin, areas into roughly square chunks.
 */
void CNavMesh::SquareUpAreas( void )
{
	int it = TheNavAreaList.Head();

	while( it != TheNavAreaList.InvalidIndex() )
	{
		CNavArea *area = TheNavAreaList[ it ];

		// move the iterator in case the current area is split and deleted
		it = TheNavAreaList.Next( it );

		if (area->HasNodes() && !area->IsRoughlySquare())
		{
			// chop this area into square pieces
			if (area->GetSizeX() > area->GetSizeY())
				splitX( area );
			else
				splitY( area );
		}
	}
}


//--------------------------------------------------------------------------------------------------------------
/**
 * Determine if we can "jump down" from given point
 */
inline bool testJumpDown( const Vector *fromPos, const Vector *toPos )
{
	float dz = fromPos->z - toPos->z;

	// drop can't be too far, or too short (or nonexistant)
	if (dz <= JumpCrouchHeight || dz >= DeathDrop)
		return false;

	//
	// Check LOS out and down
	//
	// ------+
	//       |
	// F     |
	//       |
	//       T 
	//

	Vector from( fromPos->x, fromPos->y, fromPos->z + HumanHeight );
	Vector to( toPos->x, toPos->y, from.z );

	trace_t result;
	UTIL_TraceLine( from, to, MASK_PLAYERSOLID_BRUSHONLY, NULL, COLLISION_GROUP_NONE, &result );
	if (result.fraction != 1.0f || result.startsolid)
		return false;

	from = to;
	to.z = toPos->z + 2.0f;
	UTIL_TraceLine( from, to, MASK_PLAYERSOLID_BRUSHONLY, NULL, COLLISION_GROUP_NONE, &result );
	if (result.fraction != 1.0f || result.startsolid)
		return false;

	return true;
}


//--------------------------------------------------------------------------------------------------------------
inline CNavArea *findJumpDownArea( const Vector *fromPos, NavDirType dir )
{
	Vector start( fromPos->x, fromPos->y, fromPos->z + HalfHumanHeight );
	AddDirectionVector( &start, dir, GenerationStepSize/2.0f );

	Vector toPos;
	CNavArea *downArea = findFirstAreaInDirection( &start, dir, 4.0f * GenerationStepSize, DeathDrop, NULL, &toPos );

	if (downArea && testJumpDown( fromPos, &toPos ))
		return downArea;

	return NULL;
}


//--------------------------------------------------------------------------------------------------------------
/**
 * Define connections between adjacent generated areas
 */
void CNavMesh::ConnectGeneratedAreas( void )
{
	Msg( "Connecting navigation areas...\n" );

	FOR_EACH_LL( TheNavAreaList, it )
	{
		CNavArea *area = TheNavAreaList[ it ];

		// scan along edge nodes, stepping one node over into the next area
		// for now, only use bi-directional connections

		// north edge
		CNavNode *node;
		for( node = area->m_node[ NORTH_WEST ]; node != area->m_node[ NORTH_EAST ]; node = node->GetConnectedNode( EAST ) )
		{
			CNavNode *adj = node->GetConnectedNode( NORTH );

			if (adj && adj->GetArea() && adj->GetConnectedNode( SOUTH ) == node)
			{
				area->ConnectTo( adj->GetArea(), NORTH );
			}
			else
			{
				CNavArea *downArea = findJumpDownArea( node->GetPosition(), NORTH );
				if (downArea && downArea != area)
					area->ConnectTo( downArea, NORTH );
			}
		}

		// west edge
		for( node = area->m_node[ NORTH_WEST ]; node != area->m_node[ SOUTH_WEST ]; node = node->GetConnectedNode( SOUTH ) )
		{
			CNavNode *adj = node->GetConnectedNode( WEST );
			
			if (adj && adj->GetArea() && adj->GetConnectedNode( EAST ) == node)
			{
				area->ConnectTo( adj->GetArea(), WEST );
			}
			else
			{
				CNavArea *downArea = findJumpDownArea( node->GetPosition(), WEST );
				if (downArea && downArea != area)
					area->ConnectTo( downArea, WEST );
			}
		}

		// south edge - this edge's nodes are actually part of adjacent areas
		// move one node north, and scan west to east
		/// @todo This allows one-node-wide areas - do we want this?
		node = area->m_node[ SOUTH_WEST ];
		if ( node ) // pre-existing areas in incremental generates won't have nodes
		{
			node = node->GetConnectedNode( NORTH );
		}
		if (node)
		{
			CNavNode *end = area->m_node[ SOUTH_EAST ]->GetConnectedNode( NORTH );
			/// @todo Figure out why cs_backalley gets a NULL node in here...
			for( ; node && node != end; node = node->GetConnectedNode( EAST ) )
			{
				CNavNode *adj = node->GetConnectedNode( SOUTH );
				
				if (adj && adj->GetArea() && adj->GetConnectedNode( NORTH ) == node)
				{
					area->ConnectTo( adj->GetArea(), SOUTH );
				}
				else
				{
					CNavArea *downArea = findJumpDownArea( node->GetPosition(), SOUTH );
					if (downArea && downArea != area)
						area->ConnectTo( downArea, SOUTH );
				}
			}
		}

		// east edge - this edge's nodes are actually part of adjacent areas
		node = area->m_node[ NORTH_EAST ];
		if ( node ) // pre-existing areas in incremental generates won't have nodes
		{
			node = node->GetConnectedNode( WEST );
		}
		if (node)
		{
			CNavNode *end = area->m_node[ SOUTH_EAST ]->GetConnectedNode( WEST );
			for( ; node && node != end; node = node->GetConnectedNode( SOUTH ) )
			{
				CNavNode *adj = node->GetConnectedNode( EAST );
				
				if (adj && adj->GetArea() && adj->GetConnectedNode( WEST ) == node)
				{
					area->ConnectTo( adj->GetArea(), EAST );
				}
				else
				{
					CNavArea *downArea = findJumpDownArea( node->GetPosition(), EAST );
					if (downArea && downArea != area)
						area->ConnectTo( downArea, EAST );
				}
			}
		}
	}
}

//--------------------------------------------------------------------------------------------------------------
/**
 * Merge areas together to make larger ones (must remain rectangular - convex).
 * Areas can only be merged if their attributes match.
 */
void CNavMesh::MergeGeneratedAreas( void )
{
	Msg( "Merging navigation areas...\n" );

	bool merged;

	do
	{
		merged = false;

		FOR_EACH_LL( TheNavAreaList, it )
		{
			CNavArea *area = TheNavAreaList[ it ];
			if ( !area->HasNodes() )
				continue;

			// north edge
			FOR_EACH_LL( area->m_connect[ NORTH ], nit )
			{
				CNavArea *adjArea = area->m_connect[ NORTH ][ nit ].area;
				if ( !adjArea->HasNodes() ) // pre-existing areas in incremental generates won't have nodes
					continue;

				if (area->m_node[ NORTH_WEST ] == adjArea->m_node[ SOUTH_WEST ] &&
					area->m_node[ NORTH_EAST ] == adjArea->m_node[ SOUTH_EAST ] &&
					area->GetAttributes() == adjArea->GetAttributes() &&
					area->IsCoplanar( adjArea ))
				{
					// merge vertical
					area->m_node[ NORTH_WEST ] = adjArea->m_node[ NORTH_WEST ];
					area->m_node[ NORTH_EAST ] = adjArea->m_node[ NORTH_EAST ];

					merged = true;
					//CONSOLE_ECHO( "  Merged (north) areas #%d and #%d\n", area->m_id, adjArea->m_id );

					area->FinishMerge( adjArea );

					// restart scan - iterator is invalidated
					break;
				}
			}

			if (merged)
				break;

			// south edge
			FOR_EACH_LL( area->m_connect[ SOUTH ], sit )
			{
				CNavArea *adjArea = area->m_connect[ SOUTH ][ sit ].area;
				if ( !adjArea->HasNodes() ) // pre-existing areas in incremental generates won't have nodes
					continue;

				if (adjArea->m_node[ NORTH_WEST ] == area->m_node[ SOUTH_WEST ] &&
					adjArea->m_node[ NORTH_EAST ] == area->m_node[ SOUTH_EAST ] &&
					area->GetAttributes() == adjArea->GetAttributes() &&
					area->IsCoplanar( adjArea ))
				{
					// merge vertical
					area->m_node[ SOUTH_WEST ] = adjArea->m_node[ SOUTH_WEST ];
					area->m_node[ SOUTH_EAST ] = adjArea->m_node[ SOUTH_EAST ];

					merged = true;
					//CONSOLE_ECHO( "  Merged (south) areas #%d and #%d\n", area->m_id, adjArea->m_id );

					area->FinishMerge( adjArea );

					// restart scan - iterator is invalidated
					break;
				}

			}

			if (merged)
				break;


			// west edge
			FOR_EACH_LL( area->m_connect[ WEST ], wit )
			{
				CNavArea *adjArea = area->m_connect[ WEST ][ wit ].area;
				if ( !adjArea->HasNodes() ) // pre-existing areas in incremental generates won't have nodes
					continue;

				if (area->m_node[ NORTH_WEST ] == adjArea->m_node[ NORTH_EAST ] &&
						area->m_node[ SOUTH_WEST ] == adjArea->m_node[ SOUTH_EAST ] &&
						area->GetAttributes() == adjArea->GetAttributes() &&
						area->IsCoplanar( adjArea ))
				{
					// merge horizontal
					area->m_node[ NORTH_WEST ] = adjArea->m_node[ NORTH_WEST ];
					area->m_node[ SOUTH_WEST ] = adjArea->m_node[ SOUTH_WEST ];

					merged = true;
					//CONSOLE_ECHO( "  Merged (west) areas #%d and #%d\n", area->m_id, adjArea->m_id );

					area->FinishMerge( adjArea );

					// restart scan - iterator is invalidated
					break;
				}

			}

			if (merged)
				break;

			// east edge
			FOR_EACH_LL( area->m_connect[ EAST ], eit )
			{
				CNavArea *adjArea = area->m_connect[ EAST ][ eit ].area;
				if ( !adjArea->HasNodes() ) // pre-existing areas in incremental generates won't have nodes
					continue;

				if (adjArea->m_node[ NORTH_WEST ] == area->m_node[ NORTH_EAST ] &&
					adjArea->m_node[ SOUTH_WEST ] == area->m_node[ SOUTH_EAST ] &&
					area->GetAttributes() == adjArea->GetAttributes() &&
					area->IsCoplanar( adjArea ))
				{
					// merge horizontal
					area->m_node[ NORTH_EAST ] = adjArea->m_node[ NORTH_EAST ];
					area->m_node[ SOUTH_EAST ] = adjArea->m_node[ SOUTH_EAST ];

					merged = true;
					//CONSOLE_ECHO( "  Merged (east) areas #%d and #%d\n", area->m_id, adjArea->m_id );

					area->FinishMerge( adjArea );

					// restart scan - iterator is invalidated
					break;
				}
			}

			if (merged)
				break;
		}
	}
	while( merged );
}


//--------------------------------------------------------------------------------------------------------------
/** 
 * Check if an rectangular area of the given size can be
 * made starting from the given node as the NW corner.
 * Only consider fully connected nodes for this check.
 * All of the nodes within the test area must have the same attributes.
 * All of the nodes must be approximately co-planar w.r.t the NW node's normal, with the
 * exception of 1x1 areas which can be any angle.
 */
bool CNavMesh::TestArea( CNavNode *node, int width, int height )
{
	Vector normal = *node->GetNormal();
	float d = -DotProduct( normal, *node->GetPosition() );

	bool nodeCrouch = node->m_crouch[ SOUTH_EAST ];
	unsigned char nodeAttributes = node->GetAttributes() & ~NAV_MESH_CROUCH;

	const float offPlaneTolerance = 5.0f;

	CNavNode *vertNode, *horizNode;

	vertNode = node;
	for( int y=0; y<height; y++ )
	{
		horizNode = vertNode;

		for( int x=0; x<width; x++ )
		{
			//
			// Compute the crouch attributes for the test node, taking into account only the side(s) of the node
			// that are in the area
			bool horizNodeCrouch = false;
			bool westEdge = (x == 0);
			bool eastEdge = (x == width - 1);
			bool northEdge = (y == 0);
			bool southEdge = (y == height - 1);

			// Check corners first
			if ( northEdge && westEdge )
			{
				horizNodeCrouch = horizNode->m_crouch[ SOUTH_EAST ];
			}
			else if ( northEdge && eastEdge )
			{
				horizNodeCrouch = horizNode->m_crouch[ SOUTH_EAST ] || horizNode->m_crouch[ SOUTH_WEST ];
			}
			else if ( southEdge && westEdge )
			{
				horizNodeCrouch = horizNode->m_crouch[ SOUTH_EAST ] || horizNode->m_crouch[ NORTH_EAST ];
			}
			else if ( southEdge && eastEdge )
			{
				horizNodeCrouch = (horizNode->GetAttributes() & NAV_MESH_CROUCH) != 0;
			}
			// check sides next
			else if ( northEdge )
			{
				horizNodeCrouch = horizNode->m_crouch[ SOUTH_EAST ] || horizNode->m_crouch[ SOUTH_WEST ];
			}
			else if ( southEdge )
			{
				horizNodeCrouch = (horizNode->GetAttributes() & NAV_MESH_CROUCH) != 0;
			}
			else if ( eastEdge )
			{
				horizNodeCrouch = (horizNode->GetAttributes() & NAV_MESH_CROUCH) != 0;
			}
			else if ( westEdge )
			{
				horizNodeCrouch = horizNode->m_crouch[ SOUTH_EAST ] || horizNode->m_crouch[ NORTH_EAST ];
			}
			// finally, we have a center node
			else
			{
				horizNodeCrouch = (horizNode->GetAttributes() & NAV_MESH_CROUCH) != 0;
			}

			// all nodes must be crouch/non-crouch
			if ( nodeCrouch != horizNodeCrouch )
				return false;

			// all nodes must have the same non-crouch attributes
			unsigned char horizNodeAttributes = horizNode->GetAttributes() & ~NAV_MESH_CROUCH;
			if (horizNodeAttributes != nodeAttributes)
				return false;

			if (horizNode->IsCovered())
				return false;

			if (!horizNode->IsClosedCell())
				return false;

			horizNode = horizNode->GetConnectedNode( EAST );
			if (horizNode == NULL)
				return false;

			// nodes must lie on/near the plane
			if (width > 1 || height > 1)
			{
				float dist = (float)fabs( DotProduct( *horizNode->GetPosition(), normal ) + d );
				if (dist > offPlaneTolerance)
					return false;
			}					
		}

		vertNode = vertNode->GetConnectedNode( SOUTH );
		if (vertNode == NULL)
			return false;

		// nodes must lie on/near the plane
		if (width > 1 || height > 1)
		{
			float dist = (float)fabs( DotProduct( *vertNode->GetPosition(), normal ) + d );
			if (dist > offPlaneTolerance)
				return false;
		}					
	}

	// check planarity of southern edge
	if (width > 1 || height > 1)
	{
		horizNode = vertNode;

		for( int x=0; x<width; x++ )
		{
			horizNode = horizNode->GetConnectedNode( EAST );
			if (horizNode == NULL)
				return false;

			// nodes must lie on/near the plane
			float dist = (float)fabs( DotProduct( *horizNode->GetPosition(), normal ) + d );
			if (dist > offPlaneTolerance)
				return false;
		}
	}

	return true;
}

//--------------------------------------------------------------------------------------------------------------
/** 
 * Create a nav area, and mark all nodes it overlaps as "covered"
 * NOTE: Nodes on the east and south edges are not included.
 * Returns number of nodes covered by this area, or -1 for error;
 */
int CNavMesh::BuildArea( CNavNode *node, int width, int height )
{
	CNavNode *nwNode = node;
	CNavNode *neNode = NULL;
	CNavNode *swNode = NULL;
	CNavNode *seNode = NULL;

	CNavNode *vertNode = node;
	CNavNode *horizNode;

	int coveredNodes = 0;

	for( int y=0; y<height; y++ )
	{
		horizNode = vertNode;

		for( int x=0; x<width; x++ )
		{
			horizNode->Cover();
			++coveredNodes;
			
			horizNode = horizNode->GetConnectedNode( EAST );
		}

		if (y == 0)
			neNode = horizNode;

		vertNode = vertNode->GetConnectedNode( SOUTH );
	}

	swNode = vertNode;

	horizNode = vertNode;		
	for( int x=0; x<width; x++ )
	{
		horizNode = horizNode->GetConnectedNode( EAST );
	}
	seNode = horizNode;

	if (!nwNode || !neNode || !seNode || !swNode)
	{
		Error( "BuildArea - NULL node.\n" );
		return -1;
	}

	CNavArea *area = new CNavArea( nwNode, neNode, seNode, swNode );
	TheNavAreaList.AddToTail( area );

	// since all internal nodes have the same attributes, set this area's attributes
	area->SetAttributes( node->GetAttributes() );

	// Check that the node was crouch in the right direction
	bool nodeCrouch = node->m_crouch[ SOUTH_EAST ];
	if ( (area->GetAttributes() & NAV_MESH_CROUCH) && !nodeCrouch )
	{
		area->SetAttributes( area->GetAttributes() & ~NAV_MESH_CROUCH );
	}

	return coveredNodes;
}


//--------------------------------------------------------------------------------------------------------------
/**
 * This function uses the CNavNodes that have been sampled from the map to
 * generate CNavAreas - rectangular areas of "walkable" space. These areas
 * are connected to each other, proving information on know how to move from
 * area to area.
 *
 * This is a "greedy" algorithm that attempts to cover the walkable area 
 * with the fewest, largest, rectangles.
 */
void CNavMesh::CreateNavAreasFromNodes( void )
{
	// haven't yet seen a map use larger than 30...
	int tryWidth = 50;
	int tryHeight = 50;
	int uncoveredNodes = CNavNode::GetListLength();

	while( uncoveredNodes > 0 )
	{
		for( CNavNode *node = CNavNode::GetFirst(); node; node = node->GetNext() )
		{
			if (node->IsCovered())
				continue;

			if (TestArea( node, tryWidth, tryHeight ))
			{
				int covered = BuildArea( node, tryWidth, tryHeight );
				if (covered < 0)
				{
					Error( "Generate: Error - Data corrupt.\n" );
					return;
				}

				uncoveredNodes -= covered;
			}
		}

		if (tryWidth >= tryHeight)
			--tryWidth;
		else
			--tryHeight;

		if (tryWidth <= 0 || tryHeight <= 0)
			break;
	}

	if ( !TheNavAreaList.Count() )
	{
		// If we somehow have no areas, don't try to create an impossibly-large grid
		AllocateGrid( 0, 0, 0, 0 );
		return;
	}

	Extent extent;
	extent.lo.x = 9999999999.9f;
	extent.lo.y = 9999999999.9f;
	extent.hi.x = -9999999999.9f;
	extent.hi.y = -9999999999.9f;

	// compute total extent
	FOR_EACH_LL( TheNavAreaList, it )
	{
		CNavArea *area = TheNavAreaList[ it ];
		const Extent &areaExtent = area->GetExtent();

		if (areaExtent.lo.x < extent.lo.x)
			extent.lo.x = areaExtent.lo.x;
		if (areaExtent.lo.y < extent.lo.y)
			extent.lo.y = areaExtent.lo.y;
		if (areaExtent.hi.x > extent.hi.x)
			extent.hi.x = areaExtent.hi.x;
		if (areaExtent.hi.y > extent.hi.y)
			extent.hi.y = areaExtent.hi.y;
	}

	// add the areas to the grid
	AllocateGrid( extent.lo.x, extent.hi.x, extent.lo.y, extent.hi.y );

	FOR_EACH_LL( TheNavAreaList, git )
	{
		AddNavArea( TheNavAreaList[ git ] );
	}


	ConnectGeneratedAreas();
	MergeGeneratedAreas();
	SquareUpAreas();
	MarkJumpAreas();

	/// @TODO: incremental generation doesn't create ladders yet
	if ( m_generationMode != GENERATE_INCREMENTAL )
	{
		FOR_EACH_LL( m_ladderList, lit )
		{
			m_ladderList[lit]->ConnectGeneratedLadder();
		}
	}
}


//--------------------------------------------------------------------------------------------------------------
/**
 * Initiate the generation process
 */
void CNavMesh::BeginGeneration( bool incremental )
{
	IGameEvent *event = gameeventmanager->CreateEvent( "nav_generate" );
	if ( event )
	{
		gameeventmanager->FireEvent( event );
	}

	engine->ServerCommand( "bot_kick\n" );

	// Right now, incrementally-generated areas won't connect to existing areas automatically.
	// Since this means hand-editing will be necessary, don't do a full analyze.
	if ( incremental )
	{
		nav_quicksave.SetValue( 1 );
	}

	m_generationState = SAMPLE_WALKABLE_SPACE;
	m_sampleTick = 0;
	m_generationMode = (incremental) ? GENERATE_INCREMENTAL : GENERATE_FULL;
	lastMsgTime = 0.0f;

	// clear any previous mesh
	DestroyNavigationMesh( incremental );

	SetNavPlace( UNDEFINED_PLACE );

	// build internal representations of ladders, which are used to find new walkable areas
	if ( !incremental ) ///< @incremental update doesn't build ladders to avoid overlapping existing ones
	{
		BuildLadders();
	}

	// start sampling from a spawn point
	CBaseEntity *spawn = gEntList.FindEntityByClassname( NULL, GetPlayerSpawnName() );

	if (spawn && !incremental)
	{
		// snap it to the sampling grid
		Vector pos = spawn->GetAbsOrigin();
		pos.x = TheNavMesh->SnapToGrid( pos.x );
		pos.y = TheNavMesh->SnapToGrid( pos.y );

		Vector normal;
		if (GetGroundHeight( pos, &pos.z, &normal ))
		{
			m_currentNode = new CNavNode( pos, normal, NULL );
		}
	}
	else
	{
		// the system will see this NULL and select the next walkable seed
		m_currentNode = NULL;
	}

	// if there are no seed points, we can't generate
	if (m_walkableSeedList.Count() == 0 && m_currentNode == NULL)
	{
		m_generationMode = GENERATE_NONE;
		Msg( "No valid walkable seed positions.  Cannot generate Navigation Mesh.\n" );
		return;
	}

	// initialize seed list index
	m_seedIdx = m_walkableSeedList.Head();

	Msg( "Generating Navigation Mesh...\n" );
}


//--------------------------------------------------------------------------------------------------------------
/**
 * Re-analyze an existing Mesh.  Determine Hiding Spots, Encounter Spots, etc.
 */
void CNavMesh::BeginAnalysis( void )
{
	// Remove and re-add elements in TheNavAreaList, to ensure indices are useful for progress feedback
	NavAreaList tmpList;
	{
		FOR_EACH_LL( TheNavAreaList, it )
		{
			tmpList.AddToTail( TheNavAreaList[it] );
		}
	}
	TheNavAreaList.RemoveAll();
	{
		FOR_EACH_LL( tmpList, it )
		{
			TheNavAreaList.AddToTail( tmpList[it] );
		}
	}

	DestroyHidingSpots();
	m_generationState = FIND_HIDING_SPOTS;
	m_generationIndex = TheNavAreaList.Head();
	m_generationMode = GENERATE_ANALYSIS_ONLY;
	lastMsgTime = 0.0f;
}


//--------------------------------------------------------------------------------------------------------------
void ShowViewPortPanelToAll( const char * name, bool bShow, KeyValues *data )
{
	CRecipientFilter filter;
	filter.AddAllPlayers();
	filter.MakeReliable();

	int count = 0;
	KeyValues *subkey = NULL;

	if ( data )
	{
		subkey = data->GetFirstSubKey();
		while ( subkey )
		{
			count++; subkey = subkey->GetNextKey();
		}

		subkey = data->GetFirstSubKey(); // reset 
	}

	UserMessageBegin( filter, "VGUIMenu" );
		WRITE_STRING( name ); // menu name
		WRITE_BYTE( bShow?1:0 );
		WRITE_BYTE( count );
		
		// write additional data (be carefull not more than 192 bytes!)
		while ( subkey )
		{
			WRITE_STRING( subkey->GetName() );
			WRITE_STRING( subkey->GetString() );
			subkey = subkey->GetNextKey();
		}
	MessageEnd();
}


//--------------------------------------------------------------------------------------------------------------
static void AnalysisProgress( const char *msg, int ticks, int current, bool showPercent = true )
{
	const float MsgInterval = 10.0f;
	float now = Plat_FloatTime();
	if ( now > lastMsgTime + MsgInterval )
	{
		if ( showPercent && ticks )
		{
			Msg( "%s %.0f%%\n", msg, current*100.0f/ticks );
		}
		else
		{
			Msg( "%s\n", msg );
		}

		lastMsgTime = now;
	}

	KeyValues *data = new KeyValues("data");
	data->SetString( "msg",	msg );
	data->SetInt( "total", ticks );
	data->SetInt( "current", current );

	ShowViewPortPanelToAll( PANEL_NAV_PROGRESS, true, data );

	data->deleteThis();
}


//--------------------------------------------------------------------------------------------------------------
static void HideAnalysisProgress( void )
{
	KeyValues *data = new KeyValues("data");
	ShowViewPortPanelToAll( PANEL_NAV_PROGRESS, false, data );
	data->deleteThis();
}


//--------------------------------------------------------------------------------------------------------------
/**
 * Process the auto-generation for 'maxTime' seconds. return false if generation is complete.
 */
bool CNavMesh::UpdateGeneration( float maxTime )
{
	double startTime = Plat_FloatTime();

	switch( m_generationState )
	{
		//---------------------------------------------------------------------------
		case SAMPLE_WALKABLE_SPACE:
		{
			AnalysisProgress( "Sampling walkable space...", 100, m_sampleTick / 10, false );
			m_sampleTick = ( m_sampleTick + 1 ) % 1000;

			while ( SampleStep() )
			{
				if ( Plat_FloatTime() - startTime > maxTime )
				{
					return true;
				}
			}

			// sampling is complete, now build nav areas
			m_generationState = CREATE_AREAS_FROM_SAMPLES;

			return true;
		}

		//---------------------------------------------------------------------------
		case CREATE_AREAS_FROM_SAMPLES:
		{
			Msg( "Creating navigation areas from sampled data...\n" );

			CreateNavAreasFromNodes();
			DestroyHidingSpots();

			// Remove and re-add elements in TheNavAreaList, to ensure indices are useful for progress feedback
			NavAreaList tmpList;
			{
				FOR_EACH_LL( TheNavAreaList, it )
				{
					tmpList.AddToTail( TheNavAreaList[it] );
				}
			}
			TheNavAreaList.RemoveAll();
			{
				FOR_EACH_LL( tmpList, it )
				{
					TheNavAreaList.AddToTail( tmpList[it] );
				}
			}

			m_generationState = FIND_HIDING_SPOTS;
			m_generationIndex = TheNavAreaList.Head();
			return true;
		}

		//---------------------------------------------------------------------------
		case FIND_HIDING_SPOTS:
		{
			while( m_generationIndex != TheNavAreaList.InvalidIndex() )
			{
				CNavArea *area = TheNavAreaList[ m_generationIndex ];
				m_generationIndex = TheNavAreaList.Next( m_generationIndex );

				area->ComputeHidingSpots();

				// don't go over our time allotment
				if( Plat_FloatTime() - startTime > maxTime )
				{
					AnalysisProgress( "Finding hiding spots...", 100, 100 * m_generationIndex / TheNavAreaList.Count() );
					return true;
				}
			}

			Msg( "Finding hiding spots...DONE\n" );

			m_generationState = FIND_APPROACH_AREAS;
			m_generationIndex = TheNavAreaList.Head();
			return true;
		}

		//---------------------------------------------------------------------------
		case FIND_APPROACH_AREAS:
		{
			while( m_generationIndex != TheNavAreaList.InvalidIndex() )
			{
				CNavArea *area = TheNavAreaList[ m_generationIndex ];
				m_generationIndex = TheNavAreaList.Next( m_generationIndex );

				area->ComputeApproachAreas();

				// don't go over our time allotment
				if( Plat_FloatTime() - startTime > maxTime )
				{
					AnalysisProgress( "Finding approach areas...", 100, 100 * m_generationIndex / TheNavAreaList.Count() );
					return true;
				}
			}

			Msg( "Finding approach areas...DONE\n" );

			m_generationState = FIND_ENCOUNTER_SPOTS;
			m_generationIndex = TheNavAreaList.Head();
			return true;
		}

		//---------------------------------------------------------------------------
		case FIND_ENCOUNTER_SPOTS:
		{
			while( m_generationIndex != TheNavAreaList.InvalidIndex() )
			{
				CNavArea *area = TheNavAreaList[ m_generationIndex ];
				m_generationIndex = TheNavAreaList.Next( m_generationIndex );

				area->ComputeSpotEncounters();

				// don't go over our time allotment
				if( Plat_FloatTime() - startTime > maxTime )
				{
					AnalysisProgress( "Finding encounter spots...", 100, 100 * m_generationIndex / TheNavAreaList.Count() );
					return true;
				}
			}

			Msg( "Finding encounter spots...DONE\n" );

			m_generationState = FIND_SNIPER_SPOTS;
			m_generationIndex = TheNavAreaList.Head();
			return true;
		}

		//---------------------------------------------------------------------------
		case FIND_SNIPER_SPOTS:
		{
			while( m_generationIndex != TheNavAreaList.InvalidIndex() )
			{
				CNavArea *area = TheNavAreaList[ m_generationIndex ];
				m_generationIndex = TheNavAreaList.Next( m_generationIndex );

				area->ComputeSniperSpots();

				// don't go over our time allotment
				if( Plat_FloatTime() - startTime > maxTime )
				{
					AnalysisProgress( "Finding sniper spots...", 100, 100 * m_generationIndex / TheNavAreaList.Count() );
					return true;
				}
			}

			Msg( "Finding sniper spots...DONE\n" );

			m_generationState = FIND_EARLIEST_OCCUPY_TIMES;
			m_generationIndex = TheNavAreaList.Head();
			return true;
		}

		//---------------------------------------------------------------------------
		case FIND_EARLIEST_OCCUPY_TIMES:
		{
			while( m_generationIndex != TheNavAreaList.InvalidIndex() )
			{
				CNavArea *area = TheNavAreaList[ m_generationIndex ];
				m_generationIndex = TheNavAreaList.Next( m_generationIndex );

				area->ComputeEarliestOccupyTimes();

				// don't go over our time allotment
				if( Plat_FloatTime() - startTime > maxTime )
				{
					AnalysisProgress( "Finding earliest occupy times...", 100, 100 * m_generationIndex / TheNavAreaList.Count() );
					return true;
				}
			}

			Msg( "Finding earliest occupy times...DONE\n" );

			m_generationState = SAVE_NAV_MESH;
			m_generationIndex = TheNavAreaList.Head();
			return true;
		}

		//---------------------------------------------------------------------------
		case SAVE_NAV_MESH:
		{
			// generation complete!
			Msg( "Generation complete!\n" );
			m_generationMode = GENERATE_NONE;
			m_isLoaded = true;

			HideAnalysisProgress();

			if ( nav_restart_after_analysis.GetBool() )
			{
				// save the mesh
				if (Save())
				{
					Msg( "Navigation map '%s' saved.\n", GetFilename() );
				}
				else
				{
					const char *filename = GetFilename();
					Msg( "ERROR: Cannot save navigation map '%s'.\n", (filename) ? filename : "(null)" );
				}

				engine->ChangeLevel( STRING( gpGlobals->mapname ), NULL );
			}

			return false;
		}
	}

	return false;
}

//--------------------------------------------------------------------------------------------------------------
/**
 * Define the name of player spawn entities
 */
void CNavMesh::SetPlayerSpawnName( const char *name )
{
	if (m_spawnName)
	{
		delete [] m_spawnName;
	}

	m_spawnName = new char [ strlen(name) + 1 ];
	strcpy( m_spawnName, name );
}


//--------------------------------------------------------------------------------------------------------------
/**
 * Return name of player spawn entity
 */
const char *CNavMesh::GetPlayerSpawnName( void ) const
{
	if (m_spawnName)
		return m_spawnName;

	// default value
	return "info_player_start";
}

//--------------------------------------------------------------------------------------------------------------
/**
 * Add a nav node and connect it.
 * Node Z positions are ground level.
 */
CNavNode *CNavMesh::AddNode( const Vector &destPos, const Vector &normal, NavDirType dir, CNavNode *source )
{
	// check if a node exists at this location
	CNavNode *node = CNavNode::GetNode( destPos );
	
	// if no node exists, create one
	bool useNew = false;
	if (node == NULL)
	{
		node = new CNavNode( destPos, normal, source );
		useNew = true;
	}

	// connect source node to new node
	source->ConnectTo( node, dir );

	// optimization: if deltaZ changes very little, assume connection is commutative
	const float zTolerance = 50.0f;
	if (fabs( source->GetPosition()->z - destPos.z ) < zTolerance)
	{
		node->ConnectTo( source, OppositeDirection( dir ) );
		node->MarkAsVisited( OppositeDirection( dir ) );
	}

	if (useNew)
	{
		// new node becomes current node
		m_currentNode = node;
	}

	node->CheckCrouch();

	return node;
}

//--------------------------------------------------------------------------------------------------------------
inline CNavNode *LadderEndSearch( const Vector *pos, NavDirType mountDir )
{
	Vector center = *pos;
	AddDirectionVector( &center, mountDir, HalfHumanWidth );

	//
	// Test the ladder dismount point first, then each cardinal direction one and two steps away
	//
	for( int d=(-1); d<2*NUM_DIRECTIONS; ++d )
	{
		Vector tryPos = center;

		if (d >= NUM_DIRECTIONS)
			AddDirectionVector( &tryPos, (NavDirType)(d - NUM_DIRECTIONS), 2.0f*GenerationStepSize );
		else if (d >= 0)
			AddDirectionVector( &tryPos, (NavDirType)d, GenerationStepSize );

		// step up a rung, to ensure adjacent floors are below us
		tryPos.z += GenerationStepSize;

		tryPos.x = TheNavMesh->SnapToGrid( tryPos.x );
		tryPos.y = TheNavMesh->SnapToGrid( tryPos.y );

		// adjust height to account for sloping areas
		Vector tryNormal;
		if (TheNavMesh->GetGroundHeight( tryPos, &tryPos.z, &tryNormal ) == false)
			continue;

		// make sure this point is not on the other side of a wall
		const float fudge = 2.0f;
		trace_t result;
		UTIL_TraceLine( center + Vector( 0, 0, fudge ), tryPos + Vector( 0, 0, fudge ), MASK_PLAYERSOLID_BRUSHONLY, NULL, COLLISION_GROUP_NONE, &result );
		if (result.fraction != 1.0f || result.startsolid)
			continue;

		// if no node exists here, create one and continue the search
		if (CNavNode::GetNode( tryPos ) == NULL)
		{
			return new CNavNode( tryPos, tryNormal, NULL );
		}
	}

	return NULL;
}


//--------------------------------------------------------------------------------------------------------------
/**
 * Search the world and build a map of possible movements.
 * The algorithm begins at the bot's current location, and does a recursive search
 * outwards, tracking all valid steps and generating a directed graph of CNavNodes.
 *
 * Sample the map one "step" in a cardinal direction to learn the map.
 *
 * Returns true if sampling needs to continue, or false if done.
 */
bool CNavMesh::SampleStep( void )
{
	// take a step
	while( true )
	{
		if (m_currentNode == NULL)
		{
			// sampling is complete from current seed, try next one
			m_currentNode = GetNextWalkableSeedNode();

			if (m_currentNode == NULL)
			{
				// search is exhausted - continue search from ends of ladders
				FOR_EACH_LL( m_ladderList, it )
				{
					CNavLadder *ladder = m_ladderList[ it ];

					// check ladder bottom
					if ((m_currentNode = LadderEndSearch( &ladder->m_bottom, ladder->GetDir() )) != 0)
						break;

					// check ladder top
					if ((m_currentNode = LadderEndSearch( &ladder->m_top, ladder->GetDir() )) != 0)
						break;
				}

				if (m_currentNode == NULL)
				{
					// all seeds exhausted, sampling complete
					return false;
				}
			}
		}

		//
		// Take a step from this node
		//
		for( int dir = NORTH; dir < NUM_DIRECTIONS; dir++ )
		{
			if (!m_currentNode->HasVisited( (NavDirType)dir ))
			{
				// have not searched in this direction yet

				// start at current node position
				Vector pos = *m_currentNode->GetPosition();

				// snap to grid
				int cx = SnapToGrid( pos.x );
				int cy = SnapToGrid( pos.y );

				// attempt to move to adjacent node
				switch( dir )
				{
					case NORTH:		cy -= GenerationStepSize; break;
					case SOUTH:		cy += GenerationStepSize; break;
					case EAST:		cx += GenerationStepSize; break;
					case WEST:		cx -= GenerationStepSize; break;
				}

				pos.x = cx;
				pos.y = cy;

				m_generationDir = (NavDirType)dir;

				// mark direction as visited
				m_currentNode->MarkAsVisited( m_generationDir );

				// test if we can move to new position
				trace_t result;
				Vector from, to;

				// modify position to account for change in ground level during step
				to.x = pos.x;
				to.y = pos.y;
				Vector toNormal;
				if (GetGroundHeight( pos, &to.z, &toNormal ) == false)
				{
					return true;
				}

				from = *m_currentNode->GetPosition();

				Vector fromOrigin = from + Vector( 0, 0, HalfHumanHeight );
				Vector toOrigin = to + Vector( 0, 0, HalfHumanHeight );

				CTraceFilterWalkableEntities filter( NULL, COLLISION_GROUP_NONE, WALK_THRU_EVERYTHING );
				UTIL_TraceLine( fromOrigin, toOrigin, MASK_PLAYERSOLID_BRUSHONLY, &filter, &result );

				bool walkable;

				if (result.fraction == 1.0f && !result.startsolid)
				{
					// the trace didnt hit anything - clear

					float toGround = to.z;
					float fromGround = from.z;

					float epsilon = 0.1f;

					// check if ledge is too high to reach or will cause us to fall to our death
					if (toGround - fromGround > JumpCrouchHeight + epsilon || fromGround - toGround > DeathDrop)
					{
						walkable = false;
					}
					else
					{
						// check surface normals along this step to see if we would cross any impassable slopes
						Vector delta = to - from;
						const float inc = 2.0f;
						float along = inc;
						bool done = false;
						float ground;
						Vector normal;

						walkable = true;

						while( !done )
						{
							Vector p;

							// need to guarantee that we test the exact edges
							if (along >= GenerationStepSize)
							{
								p = to;
								done = true;
							}
							else
							{
								p = from + delta * (along/GenerationStepSize);
							}

							if (GetGroundHeight( p, &ground, &normal ) == false)
							{
								walkable = false;
								break;
							}

							// check for maximum allowed slope
							if (normal.z < nav_slope_limit.GetFloat())
							{
								walkable = false;
								break;
							}

							along += inc;					
						}
					}
				}
				else	// TraceLine hit something...
				{
					if (IsEntityWalkable( result.m_pEnt, WALK_THRU_EVERYTHING ))
					{
						walkable = true;
					}
					else
					{
						walkable = false;
					}
				}

				// if we're incrementally generating, don't overlap existing nav areas
				CNavArea *overlap = GetNavArea( to, HumanHeight );
				if ( overlap )
				{
					walkable = false;
				}

				if (walkable)
				{
					// we can move here
					// create a new navigation node, and update current node pointer
					AddNode( to, toNormal, m_generationDir, m_currentNode );
				}

				return true;
			}
		}

		// all directions have been searched from this node - pop back to its parent and continue
		m_currentNode = m_currentNode->GetParent();
	}
}


//--------------------------------------------------------------------------------------------------------------
/**
 * Add given walkable position to list of seed positions for map sampling
 */
void CNavMesh::AddWalkableSeed( const Vector &pos, const Vector &normal )
{
	WalkableSeedSpot seed;

	seed.pos = pos;
	seed.normal = normal;

	m_walkableSeedList.AddToTail( seed );
}

//--------------------------------------------------------------------------------------------------------------
/**
 * Return the next walkable seed as a node
 */
CNavNode *CNavMesh::GetNextWalkableSeedNode( void )
{	
	if (!m_walkableSeedList.IsValidIndex( m_seedIdx ))
		return NULL;

	WalkableSeedSpot spot = m_walkableSeedList.Element( m_seedIdx );

	m_seedIdx = m_walkableSeedList.Next( m_seedIdx );

	return new CNavNode( spot.pos, spot.normal, NULL );
}


//--------------------------------------------------------------------------------------------------------------
/**
 * Check LOS, ignoring any entities that we can walk through
 */
bool IsWalkableTraceLineClear( Vector &from, Vector &to, unsigned int flags )
{
	trace_t result;
	CBaseEntity *ignore = NULL;
	Vector useFrom = from;

	CTraceFilterWalkableEntities traceFilter( NULL, COLLISION_GROUP_NONE, flags );

	result.fraction = 0.0f;

	const int maxTries = 50;
	for( int t=0; t<maxTries; ++t )
	{
		UTIL_TraceLine( useFrom, to, MASK_PLAYERSOLID, &traceFilter, &result );

		// if we hit a walkable entity, try again
		if (result.fraction != 1.0f && IsEntityWalkable( result.m_pEnt, flags ))
		{
			ignore = result.m_pEnt;

			// start from just beyond where we hit to avoid infinite loops
			Vector dir = to - from;
			dir.NormalizeInPlace();
			useFrom = result.endpos + 5.0f * dir;
		}
		else
		{
			break;
		}
	}

	if (result.fraction == 1.0f)
		return true;

	return false;
}



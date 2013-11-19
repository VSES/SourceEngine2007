//========= Copyright © 1996-2005, Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
// $NoKeywords: $
//
//=============================================================================//
// nav_area.h
// Navigation areas
// Author: Michael S. Booth (mike@turtlerockstudios.com), January 2003

#ifndef _NAV_AREA_H_
#define _NAV_AREA_H_

#include "nav_ladder.h"

// BOTPORT: Clean up relationship between team index and danger storage in nav areas
enum { MAX_NAV_TEAMS = 2 };



//-------------------------------------------------------------------------------------------------------------------
/**
 * The NavConnect union is used to refer to connections to areas
 */
union NavConnect
{
	unsigned int id;
	CNavArea *area;

	bool operator==( const NavConnect &other ) const
	{
		return (area == other.area) ? true : false;
	}
};
typedef CUtlLinkedList<NavConnect, int> NavConnectList;


//-------------------------------------------------------------------------------------------------------------------
/**
 * The NavLadderConnect union is used to refer to connections to ladders
 */
union NavLadderConnect
{
	unsigned int id;
	CNavLadder *ladder;

	bool operator==( const NavLadderConnect &other ) const
	{
		return (ladder == other.ladder) ? true : false;
	}
};
typedef CUtlLinkedList<NavLadderConnect, int> NavLadderConnectList;


//--------------------------------------------------------------------------------------------------------------
/**
 * A HidingSpot is a good place for a bot to crouch and wait for enemies
 */
class HidingSpot
{
public:
	virtual ~HidingSpot()	{ }

	enum 
	{ 
		IN_COVER			= 0x01,							///< in a corner with good hard cover nearby
		GOOD_SNIPER_SPOT	= 0x02,							///< had at least one decent sniping corridor
		IDEAL_SNIPER_SPOT	= 0x04,							///< can see either very far, or a large area, or both
		EXPOSED				= 0x08							///< spot in the open, usually on a ledge or cliff
	};

	bool HasGoodCover( void ) const			{ return (m_flags & IN_COVER) ? true : false; }	///< return true if hiding spot in in cover
	bool IsGoodSniperSpot( void ) const		{ return (m_flags & GOOD_SNIPER_SPOT) ? true : false; }
	bool IsIdealSniperSpot( void ) const	{ return (m_flags & IDEAL_SNIPER_SPOT) ? true : false; }
	bool IsExposed( void ) const			{ return (m_flags & EXPOSED) ? true : false; }	

	int GetFlags( void ) const		{ return m_flags; }

	void Save( FileHandle_t file, unsigned int version ) const;
	void Load( FileHandle_t file, unsigned int version );
	NavErrorType PostLoad( void );

	const Vector &GetPosition( void ) const		{ return m_pos; }	///< get the position of the hiding spot
	unsigned int GetID( void ) const			{ return m_id; }
	const CNavArea *GetArea( void ) const		{ return m_area; }	///< return nav area this hiding spot is within

	void Mark( void )							{ m_marker = m_masterMarker; }
	bool IsMarked( void ) const					{ return (m_marker == m_masterMarker) ? true : false; }
	static void ChangeMasterMarker( void )		{ ++m_masterMarker; }


public:
	void SetFlags( int flags )				{ m_flags |= flags; }	///< FOR INTERNAL USE ONLY
	void SetPosition( const Vector &pos )	{ m_pos = pos; }		///< FOR INTERNAL USE ONLY

private:
	friend class CNavMesh;
	friend void ClassifySniperSpot( HidingSpot *spot );

	HidingSpot( void );										///< must use factory to create

	Vector m_pos;											///< world coordinates of the spot
	unsigned int m_id;										///< this spot's unique ID
	unsigned int m_marker;									///< this spot's unique marker
	CNavArea *m_area;										///< the nav area containing this hiding spot

	unsigned char m_flags;									///< bit flags

	static unsigned int m_nextID;							///< used when allocating spot ID's
	static unsigned int m_masterMarker;						///< used to mark spots
};
typedef CUtlLinkedList< HidingSpot *, int > HidingSpotList;
extern HidingSpotList TheHidingSpotList;

extern HidingSpot *GetHidingSpotByID( unsigned int id );


//--------------------------------------------------------------------------------------------------------------
/**
 * Stores a pointer to an interesting "spot", and a parametric distance along a path
 */
struct SpotOrder
{
	float t;						///< parametric distance along ray where this spot first has LOS to our path
	union
	{
		HidingSpot *spot;			///< the spot to look at
		unsigned int id;			///< spot ID for save/load
	};
};
typedef CUtlLinkedList< SpotOrder, int > SpotOrderList;

/**
 * This struct stores possible path segments thru a CNavArea, and the dangerous spots
 * to look at as we traverse that path segment.
 */
struct SpotEncounter
{
	NavConnect from;
	NavDirType fromDir;
	NavConnect to;
	NavDirType toDir;
	Ray path;									///< the path segment
	SpotOrderList spotList;						///< list of spots to look at, in order of occurrence
};
typedef CUtlLinkedList< SpotEncounter *, int > SpotEncounterList;


//-------------------------------------------------------------------------------------------------------------------
/**
 * A CNavArea is a rectangular region defining a walkable area in the environment
 */
class CNavArea
{
public:
	CNavArea( CNavNode *nwNode, CNavNode *neNode, CNavNode *seNode, CNavNode *swNode );
	CNavArea( void );
	CNavArea( const Vector &corner, const Vector &otherCorner );
	CNavArea( const Vector &nwCorner, const Vector &neCorner, const Vector &seCorner, const Vector &swCorner );

	~CNavArea();

	void ConnectTo( CNavArea *area, NavDirType dir );			///< connect this area to given area in given direction
	void Disconnect( CNavArea *area );							///< disconnect this area from given area

	void ConnectTo( CNavLadder *ladder );						///< connect this area to given ladder
	void Disconnect( CNavLadder *ladder );						///< disconnect this area from given ladder

	void Save( FileHandle_t file, unsigned int version ) const;
	void Load( FileHandle_t file, unsigned int version );
	NavErrorType PostLoad( void );

	unsigned int GetID( void ) const	{ return m_id; }		///< return this area's unique ID
	static void CompressIDs( void );							///< re-orders area ID's so they are continuous

	void SetAttributes( int bits )		{ m_attributeFlags = (unsigned short)bits; }
	int GetAttributes( void ) const		{ return m_attributeFlags; }

	void SetPlace( Place place )		{ m_place = place; }	///< set place descriptor
	Place GetPlace( void ) const		{ return m_place; }		///< get place descriptor

	void UpdateBlocked( void );									///< Updates the (un)blocked status of the nav area
	void CheckFloor( CBaseEntity *ignore );						///< Checks if there is a floor under the nav area, in case a breakable floor is gone
	bool IsBlocked( void ) const		{ return m_isBlocked; }
	void CheckWaterLevel( void );
	bool IsUnderwater( void ) const		{ return m_isUnderwater; }

	bool IsOverlapping( const Vector &pos, float tolerance = 0.0f ) const;	///< return true if 'pos' is within 2D extents of area.
	bool IsOverlapping( const CNavArea *area ) const;			///< return true if 'area' overlaps our 2D extents
	bool IsOverlappingX( const CNavArea *area ) const;			///< return true if 'area' overlaps our X extent
	bool IsOverlappingY( const CNavArea *area ) const;			///< return true if 'area' overlaps our Y extent
	float GetZ( const Vector &pos ) const;						///< return Z of area at (x,y) of 'pos'
	float GetZ( float x, float y ) const;						///< return Z of area at (x,y) of 'pos'
	bool Contains( const Vector &pos ) const;					///< return true if given point is on or above this area, but no others
	bool IsCoplanar( const CNavArea *area ) const;				///< return true if this area and given area are approximately co-planar
	void GetClosestPointOnArea( const Vector &pos, Vector *close ) const;	///< return closest point to 'pos' on this area - returned point in 'close'
	float GetDistanceSquaredToPoint( const Vector &pos ) const;	///< return shortest distance between point and this area
	bool IsDegenerate( void ) const;							///< return true if this area is badly formed
	bool IsRoughlySquare( void ) const;							///< return true if this area is approximately square
	bool IsFlat( void ) const;									///< return true if this area is approximately flat

	bool IsEdge( NavDirType dir ) const;						///< return true if there are no bi-directional links on the given side

	bool IsVisible( const Vector &eye, Vector *visSpot = NULL ) const;	///< return true if area is visible from the given eyepoint, return visible spot

	int GetAdjacentCount( NavDirType dir ) const	{ return m_connect[ dir ].Count(); }	///< return number of connected areas in given direction
	CNavArea *GetAdjacentArea( NavDirType dir, int i ) const;	/// return the i'th adjacent area in the given direction
	CNavArea *GetRandomAdjacentArea( NavDirType dir ) const;

	const NavConnectList *GetAdjacentList( NavDirType dir ) const	{ return &m_connect[dir]; }
	bool IsConnected( const CNavArea *area, NavDirType dir ) const;	///< return true if given area is connected in given direction
	bool IsConnected( const CNavLadder *ladder, CNavLadder::LadderDirectionType dir ) const;	///< return true if given ladder is connected in given direction
	float ComputeHeightChange( const CNavArea *area );			///< compute change in height from this area to given area

	const NavLadderConnectList *GetLadderList( CNavLadder::LadderDirectionType dir ) const	{ return &m_ladder[dir]; }

	void ComputePortal( const CNavArea *to, NavDirType dir, Vector *center, float *halfWidth ) const;		///< compute portal to adjacent area
	void ComputeClosestPointInPortal( const CNavArea *to, NavDirType dir, const Vector &fromPos, Vector *closePos ) const; ///< compute closest point within the "portal" between to adjacent areas
	NavDirType ComputeDirection( Vector *point ) const;			///< return direction from this area to the given point

	//- for hunting algorithm ---------------------------------------------------------------------------
	void SetClearedTimestamp( int teamID );						///< set this area's "clear" timestamp to now
	float GetClearedTimestamp( int teamID ) const;				///< get time this area was marked "clear"

	//- hiding spots ------------------------------------------------------------------------------------
	const HidingSpotList *GetHidingSpotList( void ) const	{ return &m_hidingSpotList; }
	void ComputeHidingSpots( void );							///< analyze local area neighborhood to find "hiding spots" in this area - for map learning
	void ComputeSniperSpots( void );							///< analyze local area neighborhood to find "sniper spots" in this area - for map learning

	SpotEncounter *GetSpotEncounter( const CNavArea *from, const CNavArea *to );	///< given the areas we are moving between, return the spots we will encounter
	void ComputeSpotEncounters( void );							///< compute spot encounter data - for map learning
	int GetSpotEncounterCount( void ) const				{ return m_spotEncounterList.Count(); }

	//- "danger" ----------------------------------------------------------------------------------------
	void IncreaseDanger( int teamID, float amount );			///< increase the danger of this area for the given team
	float GetDanger( int teamID );								///< return the danger of this area (decays over time)

	//- extents -----------------------------------------------------------------------------------------
	float GetSizeX( void ) const			{ return m_extent.hi.x - m_extent.lo.x; }
	float GetSizeY( void ) const			{ return m_extent.hi.y - m_extent.lo.y; }
	const Extent &GetExtent( void ) const	{ return m_extent; }
	const Vector &GetCenter( void ) const	{ return m_center; }
	const Vector &GetCorner( NavCornerType corner ) const;
	void SetCorner( NavCornerType corner, const Vector& newPosition );
	void ComputeNormal( Vector *normal, bool alternate = false ) const;	///< Computes the area's normal based on m_extent.lo.  If 'alternate' is specified, m_extent.hi is used instead.

	//- approach areas ----------------------------------------------------------------------------------
	struct ApproachInfo
	{
		NavConnect here;										///< the approach area
		NavConnect prev;										///< the area just before the approach area on the path
		NavTraverseType prevToHereHow;
		NavConnect next;										///< the area just after the approach area on the path
		NavTraverseType hereToNextHow;
	};
	const ApproachInfo *GetApproachInfo( int i ) const	{ return &m_approach[i]; }
	int GetApproachInfoCount( void ) const				{ return m_approachCount; }
	void ComputeApproachAreas( void );							///< determine the set of "approach areas" - for map learning

	//- occupy time ------------------------------------------------------------------------------------
	float GetEarliestOccupyTime( int teamID ) const;			///< returns the minimum time for someone of the given team to reach this spot from their spawn
	bool IsBattlefront( void ) const	{ return m_isBattlefront; }	///< true if this area is a "battlefront" - where rushing teams initially meet

	//- player counting --------------------------------------------------------------------------------
	void IncrementPlayerCount( int teamID );					///< add one player to this area's count
	void DecrementPlayerCount( int teamID );					///< subtract one player from this area's count
	void ClearPlayerCount( void );								///< set the player count to zero
	unsigned char GetPlayerCount( int teamID = 0 ) const;		///< return number of players of given team currently within this area (team of zero means any/all)

	//- A* pathfinding algorithm ------------------------------------------------------------------------
	static void MakeNewMarker( void )	{ ++m_masterMarker; if (m_masterMarker == 0) m_masterMarker = 1; }
	void Mark( void )					{ m_marker = m_masterMarker; }
	BOOL IsMarked( void ) const			{ return (m_marker == m_masterMarker) ? true : false; }
	
	void SetParent( CNavArea *parent, NavTraverseType how = NUM_TRAVERSE_TYPES )	{ m_parent = parent; m_parentHow = how; }
	CNavArea *GetParent( void ) const	{ return m_parent; }
	NavTraverseType GetParentHow( void ) const	{ return m_parentHow; }

	bool IsOpen( void ) const;									///< true if on "open list"
	void AddToOpenList( void );									///< add to open list in decreasing value order
	void UpdateOnOpenList( void );								///< a smaller value has been found, update this area on the open list
	void RemoveFromOpenList( void );
	static bool IsOpenListEmpty( void );
	static CNavArea *PopOpenList( void );						///< remove and return the first element of the open list													

	bool IsClosed( void ) const;								///< true if on "closed list"
	void AddToClosedList( void );								///< add to the closed list
	void RemoveFromClosedList( void );

	static void ClearSearchLists( void );						///< clears the open and closed lists for a new search

	void SetTotalCost( float value )	{ m_totalCost = value; }
	float GetTotalCost( void ) const	{ return m_totalCost; }

	void SetCostSoFar( float value )	{ m_costSoFar = value; }
	float GetCostSoFar( void ) const	{ return m_costSoFar; }

	//- editing -----------------------------------------------------------------------------------------
	void Draw( void ) const;					///< draw area for debugging & editing
	void DrawConnectedAreas( void ) const;
	void DrawHidingSpots( void ) const;
	bool SplitEdit( bool splitAlongX, float splitEdge, CNavArea **outAlpha = NULL, CNavArea **outBeta = NULL );	///< split this area into two areas at the given edge
	bool MergeEdit( CNavArea *adj );							///< merge this area and given adjacent area 
	bool SpliceEdit( CNavArea *other );							///< create a new area between this area and given area 
	void RaiseCorner( NavCornerType corner, int amount, bool raiseAdjacentCorners = true );	///< raise/lower a corner (or all corners if corner == NUM_CORNERS)
	void PlaceOnGround( NavCornerType corner, float inset = 0.0f );	///< places a corner (or all corners if corner == NUM_CORNERS) on the ground
	NavCornerType GetCornerUnderCursor( void ) const;
	bool GetCornerHotspot( NavCornerType corner, Vector hotspot[NUM_CORNERS] ) const;	///< returns true if the corner is under the cursor

	//- ladders -----------------------------------------------------------------------------------------
	void AddLadderUp( CNavLadder *ladder );
	void AddLadderDown( CNavLadder *ladder );

private:
	friend class CNavMesh;
	friend class CNavLadder;

	void Initialize( void );									///< to keep constructors consistent
	static bool m_isReset;										///< if true, don't bother cleaning up in destructor since everything is going away

	/*
	extent.lo
		nw           ne
		 +-----------+
		 | +-->x     |
		 | |         |
		 | v         |
		 | y         |
		 |           |
		 +-----------+
		sw           se
					extent.hi
	*/

	static unsigned int m_nextID;								///< used to allocate unique IDs
	unsigned int m_id;											///< unique area ID
	Extent m_extent;											///< extents of area in world coords (NOTE: lo.z is not necessarily the minimum Z, but corresponds to Z at point (lo.x, lo.y), etc
	Vector m_center;											///< centroid of area
	unsigned short m_attributeFlags;							///< set of attribute bit flags (see NavAttributeType)
	Place m_place;												///< place descriptor
	bool m_isBlocked;											///< if true, some part of the world is preventing movement through this nav area
	bool m_isUnderwater;										///< true if the center of the area is underwater

	/// height of the implicit corners
	float m_neZ;
	float m_swZ;

	//- for hunting -------------------------------------------------------------------------------------
	float m_clearedTimestamp[ MAX_NAV_TEAMS ];					///< time this area was last "cleared" of enemies

	//- "danger" ----------------------------------------------------------------------------------------
	float m_danger[ MAX_NAV_TEAMS ];							///< danger of this area, allowing bots to avoid areas where they died in the past - zero is no danger
	float m_dangerTimestamp[ MAX_NAV_TEAMS ];					///< time when danger value was set - used for decaying
	void DecayDanger( void );

	//- hiding spots ------------------------------------------------------------------------------------
	HidingSpotList m_hidingSpotList;
	bool IsHidingSpotCollision( const Vector &pos ) const;		///< returns true if an existing hiding spot is too close to given position

	//- encounter spots ---------------------------------------------------------------------------------
	SpotEncounterList m_spotEncounterList;						///< list of possible ways to move thru this area, and the spots to look at as we do
	void AddSpotEncounters( const CNavArea *from, NavDirType fromDir, const CNavArea *to, NavDirType toDir );	///< add spot encounter data when moving from area to area

	//- approach areas ----------------------------------------------------------------------------------
	enum { MAX_APPROACH_AREAS = 16 };
	ApproachInfo m_approach[ MAX_APPROACH_AREAS ];
	unsigned char m_approachCount;

	float m_earliestOccupyTime[ MAX_NAV_TEAMS ];				///< min time to reach this spot from spawn
	void ComputeEarliestOccupyTimes( void );
	bool m_isBattlefront;

	unsigned char m_playerCount[ MAX_NAV_TEAMS ];				///< the number of players currently in this area

	void Strip( void );											///< remove "analyzed" data from nav area

	//- A* pathfinding algorithm ------------------------------------------------------------------------
	static unsigned int m_masterMarker;
	unsigned int m_marker;										///< used to flag the area as visited
	CNavArea *m_parent;											///< the area just prior to this on in the search path
	NavTraverseType m_parentHow;								///< how we get from parent to us
	float m_totalCost;											///< the distance so far plus an estimate of the distance left
	float m_costSoFar;											///< distance travelled so far

	static CNavArea *m_openList;
	CNavArea *m_nextOpen, *m_prevOpen;							///< only valid if m_openMarker == m_masterMarker
	unsigned int m_openMarker;									///< if this equals the current marker value, we are on the open list

	//- connections to adjacent areas -------------------------------------------------------------------
	NavConnectList m_connect[ NUM_DIRECTIONS ];					///< a list of adjacent areas for each direction
	NavLadderConnectList m_ladder[ CNavLadder::NUM_LADDER_DIRECTIONS ];	///< list of ladders leading up and down from this area

	//---------------------------------------------------------------------------------------------------
	CNavNode *m_node[ NUM_CORNERS ];							///< nav nodes at each corner of the area

	void ResetNodes( void );									///< nodes are going away as part of an incremental nav generation
	bool HasNodes( void ) const;

	void FinishMerge( CNavArea *adjArea );						///< recompute internal data once nodes have been adjusted during merge
	void MergeAdjacentConnections( CNavArea *adjArea );			///< for merging with "adjArea" - pick up all of "adjArea"s connections
	void AssignNodes( CNavArea *area );							///< assign internal nodes to the given area

	void FinishSplitEdit( CNavArea *newArea, NavDirType ignoreEdge );	///< given the portion of the original area, update its internal data

	CUtlLinkedList<CNavArea *, int> m_overlapList;				///< list of areas that overlap this area

	void OnDestroyNotify( CNavArea *dead );						///< invoked when given area is going away
	void OnDestroyNotify( CNavLadder *dead );					///< invoked when given ladder is going away

	CNavArea *m_prevHash, *m_nextHash;							///< for hash table in CNavMesh
};

typedef CUtlLinkedList< CNavArea *, int > NavAreaList;
extern NavAreaList TheNavAreaList;


//--------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------
//
// Inlines
//

//--------------------------------------------------------------------------------------------------------------
inline bool CNavArea::IsDegenerate( void ) const
{
	return (m_extent.lo.x >= m_extent.hi.x || m_extent.lo.y >= m_extent.hi.y);
}

//--------------------------------------------------------------------------------------------------------------
inline CNavArea *CNavArea::GetAdjacentArea( NavDirType dir, int i ) const
{
	int iter;
	for( iter = m_connect[dir].Head(); iter != m_connect[dir].InvalidIndex(); iter=m_connect[dir].Next( iter ) )
	{
		if (i == 0)
			return m_connect[dir][iter].area;
		--i;
	}

	return NULL;
}

//--------------------------------------------------------------------------------------------------------------
inline bool CNavArea::IsOpen( void ) const
{
	return (m_openMarker == m_masterMarker) ? true : false;
}

//--------------------------------------------------------------------------------------------------------------
inline bool CNavArea::IsOpenListEmpty( void )
{
	return (m_openList) ? false : true;
}

//--------------------------------------------------------------------------------------------------------------
inline CNavArea *CNavArea::PopOpenList( void )
{
	if (m_openList)
	{
		CNavArea *area = m_openList;
	
		// disconnect from list
		area->RemoveFromOpenList();

		return area;
	}

	return NULL;
}

//--------------------------------------------------------------------------------------------------------------
inline bool CNavArea::IsClosed( void ) const
{
	if (IsMarked() && !IsOpen())
		return true;

	return false;
}

//--------------------------------------------------------------------------------------------------------------
inline void CNavArea::AddToClosedList( void )
{
	Mark();
}

//--------------------------------------------------------------------------------------------------------------
inline void CNavArea::RemoveFromClosedList( void )
{
	// since "closed" is defined as visited (marked) and not on open list, do nothing
}

//--------------------------------------------------------------------------------------------------------------
inline void CNavArea::SetClearedTimestamp( int teamID )
{
	m_clearedTimestamp[ teamID % MAX_NAV_TEAMS ] = gpGlobals->curtime;
}

//--------------------------------------------------------------------------------------------------------------
inline float CNavArea::GetClearedTimestamp( int teamID ) const
{ 
	return m_clearedTimestamp[ teamID % MAX_NAV_TEAMS ];
}

//--------------------------------------------------------------------------------------------------------------
inline float CNavArea::GetEarliestOccupyTime( int teamID ) const
{
	return m_earliestOccupyTime[ teamID % MAX_NAV_TEAMS ];
}


//--------------------------------------------------------------------------------------------------------------
inline bool CNavArea::IsVisible( const Vector &eye, Vector *visSpot ) const
{
	Vector corner;
	trace_t result;
	CTraceFilterNoNPCsOrPlayer traceFilter( NULL, COLLISION_GROUP_NONE );
	const float offset = 0.75f * HumanHeight;

	// check center first
	UTIL_TraceLine( eye, GetCenter() + Vector( 0, 0, offset ), MASK_BLOCKLOS_AND_NPCS, &traceFilter, &result );
	if (result.fraction == 1.0f)
	{
		// we can see this area
		if (visSpot)
		{
			*visSpot = GetCenter();
		}
		return true;
	}

	for( int c=0; c<NUM_CORNERS; ++c )
	{
		corner = GetCorner( (NavCornerType)c );
		UTIL_TraceLine( eye, corner + Vector( 0, 0, offset ), MASK_BLOCKLOS_AND_NPCS, &traceFilter, &result );
		if (result.fraction == 1.0f)
		{
			// we can see this area
			if (visSpot)
			{
				*visSpot = corner;
			}
			return true;
		}
	}

	return false;
}

inline void CNavArea::IncrementPlayerCount( int teamID )
{
	++m_playerCount[ teamID % MAX_NAV_TEAMS ];
}

inline void CNavArea::DecrementPlayerCount( int teamID )
{
	--m_playerCount[ teamID % MAX_NAV_TEAMS ];
}

inline void CNavArea::ClearPlayerCount( void )
{
	for( int i=0; i<MAX_NAV_TEAMS; ++i )
	{
		m_playerCount[ i ] = 0;
	}
}

inline unsigned char CNavArea::GetPlayerCount( int teamID ) const
{
	if (teamID)
	{
		return m_playerCount[ teamID % MAX_NAV_TEAMS ];
	}

	// sum all players
	unsigned char total = 0;
	for( int i=0; i<MAX_NAV_TEAMS; ++i )
	{
		total += m_playerCount[i];
	}

	return total;
}


#endif // _NAV_AREA_H_

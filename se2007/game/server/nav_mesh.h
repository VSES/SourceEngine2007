//========= Copyright © 1996-2005, Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
// $NoKeywords: $
//
//=============================================================================//
// nav_mesh.h
// The Navigation Mesh interface
// Author: Michael S. Booth (mike@turtlerockstudios.com), January 2003

//
// Author: Michael S. Booth (mike@turtlerockstudios.com), 2003
//
// NOTE: The Navigation code uses Doxygen-style comments. If you run Doxygen over this code, it will 
// auto-generate documentation.  Visit www.doxygen.org to download the system for free.
//

#ifndef _NAV_MESH_H_
#define _NAV_MESH_H_

#include "filesystem.h"

#include "nav.h"
#include "nav_area.h"
#include "nav_colors.h"


class CNavArea;
class CBaseEntity; 

extern ConVar nav_edit;
extern ConVar nav_quicksave;
extern ConVar nav_show_approach_points;
extern ConVar nav_show_danger;

//--------------------------------------------------------------------------------------------------------
/**
 * The CNavMesh is the global interface to the Navigation Mesh.
 * @todo Make this an abstract base class interface, and derive mod-specific implementations.
 */
class CNavMesh
{
public:
	CNavMesh( void );
	virtual ~CNavMesh();

	virtual HidingSpot *CreateHidingSpot( void ) const;					///< Hiding Spot factory

	virtual void Reset( void );											///< destroy Navigation Mesh data and revert to initial state
	virtual void Update( void );										///< invoked on each game frame

	virtual NavErrorType Load( void );									///< load navigation data from a file
	bool IsLoaded( void ) const		{ return m_isLoaded; }				///< return true if a Navigation Mesh has been loaded
	virtual bool Save( void ) const;									///< store Navigation Mesh to a file
	bool IsFromCurrentMap( void ) const	{ return m_isFromCurrentMap; }	///< return true if the Navigation Mesh was last edited with the current map version

	unsigned int GetNavAreaCount( void ) const	{ return m_areaCount; }	///< return total number of nav areas

	CNavArea *GetNavArea( const Vector &pos, float beneathLimt = 120.0f ) const;	///< given a position, return the nav area that IsOverlapping and is *immediately* beneath it
	CNavArea *GetNavAreaByID( unsigned int id ) const;
	CNavArea *GetNearestNavArea( const Vector &pos, bool anyZ = false, float maxDist = 10000.0f, bool checkLOS = false ) const;

	Place GetPlace( const Vector &pos ) const;							///< return Place at given coordinate
	const char *PlaceToName( Place place ) const;						///< given a place, return its name
	Place NameToPlace( const char *name ) const;						///< given a place name, return a place ID or zero if no place is defined
	Place PartialNameToPlace( const char *name ) const;					///< given the first part of a place name, return a place ID or zero if no place is defined, or the partial match is ambiguous
	void PrintAllPlaces( void ) const;									///< output a list of names to the console
	int PlaceNameAutocomplete( char const *partial, char commands[ COMMAND_COMPLETION_MAXITEMS ][ COMMAND_COMPLETION_ITEM_LENGTH ] );	///< Given a partial place name, fill in possible place names for ConCommand autocomplete

	bool GetGroundHeight( const Vector &pos, float *height, Vector *normal = NULL ) const;		///< get the Z coordinate of the topmost ground level below the given point
	bool GetSimpleGroundHeight( const Vector &pos, float *height, Vector *normal = NULL ) const;///< get the Z coordinate of the ground level directly below the given point


	/// increase "danger" weights in the given nav area and nearby ones
	void IncreaseDangerNearby( int teamID, float amount, CNavArea *area, const Vector &pos, float maxRadius );
	void DrawDanger( void ) const;										///< draw the current danger levels

	void ClearPlayerCounts( void );										///< zero player counts in all areas
	void DrawPlayerCounts( void ) const;								///< draw the current player counts for each area

	//-------------------------------------------------------------------------------------
	// Auto-generation
	//
	void BeginGeneration( bool incremental = false );					///< initiate the generation process
	void BeginAnalysis( void );											///< re-analyze an existing Mesh.  Determine Hiding Spots, Encounter Spots, etc.

	bool IsGenerating( void ) const		{ return m_generationMode != GENERATE_NONE; }	///< return true while a Navigation Mesh is being generated
	const char *GetPlayerSpawnName( void ) const;						///< return name of player spawn entity
	void SetPlayerSpawnName( const char *name );						///< define the name of player spawn entities
	void AddWalkableSeed( const Vector &pos, const Vector &normal );	///< add given walkable position to list of seed positions for map sampling
	void ClearWalkableSeeds( void )		{ m_walkableSeedList.RemoveAll(); }	///< erase all walkable seed positions

	//-------------------------------------------------------------------------------------
	// Edit mode
	//
	unsigned int GetNavPlace( void ) const			{ return m_navPlace; }
	void SetNavPlace( unsigned int place )			{ m_navPlace = place; }

	// Edit callbacks from ConCommands
	void CommandNavDelete( void );										///< delete current area
	void CommandNavDeleteMarked( void );								///< delete current marked area
	void CommandNavSplit( void );										///< split current area
	void CommandNavMerge( void );										///< merge adjacent areas
	void CommandNavMark( const CCommand &args );						///< mark an area for further operations
	void CommandNavUnmark( void );										///< removes the mark
	void CommandNavBeginArea( void );									///< begin creating a new nav area
	void CommandNavEndArea( void );										///< end creation of the new nav area
	void CommandNavConnect( void );										///< connect marked area to selected area
	void CommandNavDisconnect( void );									///< disconnect marked area from selected area
	void CommandNavSplice( void );										///< create new area in between marked and selected areas
	void CommandNavCrouch( void );										///< toggle crouch attribute on current area
	void CommandNavTogglePlaceMode( void );								///< switch between normal and place editing
	void CommandNavSetPlaceMode( void );								///< switch between normal and place editing
	void CommandNavPlaceFloodFill( void );								///< floodfill areas out from current area
	void CommandNavPlacePick( void );									///< "pick up" the place at the current area
	void CommandNavTogglePlacePainting( void );							///< switch between "painting" places onto areas
	void CommandNavMarkUnnamed( void );									///< mark an unnamed area for further operations
	void CommandNavCornerSelect( void );								///< select a corner on the current area
	void CommandNavCornerRaise( void );									///< raise a corner on the current area
	void CommandNavCornerLower( void );									///< lower a corner on the current area
	void CommandNavCornerPlaceOnGround( const CCommand &args );			///< position a corner on the current area at ground height
	void CommandNavWarpToMark( void );									///< warp a spectating local player to the selected mark
	void CommandNavLadderFlip( void );									///< Flips the direction a ladder faces
	void CommandNavToggleAttribute( NavAttributeType attribute );		///< toggle an attribute on current area
	void CommandNavMakeSniperSpots( void );								///< cuts up the marked area into individual areas suitable for sniper spots
	void CommandNavBuildLadder( void );									///< builds a nav ladder on the climbable surface under the cursor
	void CommandNavRemoveUnusedJumpAreas( void );						///< removes jump areas with at most 1 connection to a ladder or non-jump area

	// IN-PROGRESS COMMANDS FOR MANIPULATING EXISTING AREAS
	void CommandNavPickArea( void );
	void CommandNavResizeHorizontal( void );
	void CommandNavResizeVertical( void );
	void CommandNavResizeEnd( void );

	bool IsPlaceMode( void ) const { return m_navEditMode == NAV_EDIT_PLACE; }

	void GetEditVectors( Vector *pos, Vector *forward );						///< Gets the eye position and view direction of the editing player

	CNavArea *GetMarkedArea( void ) const		{ return m_markedArea; }		///< return area marked by user in edit mode
	CNavLadder *GetMarkedLadder( void ) const	{ return m_markedLadder; }		///< return ladder marked by user in edit mode

	CNavArea *GetSelectedArea( void ) const		{ return m_selectedArea; }		///< return area selected by user in edit mode
	CNavLadder *GetSelectedLadder( void ) const	{ return m_selectedLadder; }	///< return ladder selected by user in edit mode

	void CreateLadder( const Vector& mins, const Vector& maxs, const Vector2D *ladderDir );

	float SnapToGrid( float x, bool forceGrid = false ) const;					///< snap given coordinate to generation grid boundary
	Vector SnapToGrid( const Vector& in, bool snapX = true, bool snapY = true, bool forceGrid = false ) const;	///< snap given vector's X & Y coordinates to generation grid boundary

	const Vector &GetEditCursorPosition( void ) const	{ return m_editCursorPos; }	///< return position of edit cursor
	void StripNavigationAreas( void );
	const char *GetFilename( void ) const;								///< return the filename for this map's "nav" file
							

	//-------------------------------------------------------------------------------------
	/**
	 * Apply the functor to all navigation areas.
	 * If functor returns false, stop processing and return false.
	 */
	template < typename Functor >
	bool ForAllAreas( Functor &func )
	{
		FOR_EACH_LL( TheNavAreaList, it )
		{
			CNavArea *area = TheNavAreaList[ it ];

			if (func( area ) == false)
				return false;
		}

		return true;
	}

	NavLadderList& GetLadders( void ) { return m_ladderList; }	///< Returns the list of ladders
	CNavLadder *GetLadderByID( unsigned int id ) const;

	CUtlVector< CNavArea * >& GetTransientAreas( void ) { return m_transientAreas; }

private:
	friend class CNavArea;
	friend class CNavNode;

	NavAreaList *m_grid;
	float m_gridCellSize;										///< the width/height of a grid cell for spatially partitioning nav areas for fast access
	int m_gridSizeX;
	int m_gridSizeY;
	float m_minX;
	float m_minY;
	unsigned int m_areaCount;									///< total number of nav areas

	bool m_isLoaded;											///< true if a Navigation Mesh has been loaded
	bool m_isFromCurrentMap;									///< true if the Navigation Mesh was last saved with the current map

	enum { HASH_TABLE_SIZE = 256 };
	CNavArea *m_hashTable[ HASH_TABLE_SIZE ];					///< hash table to optimize lookup by ID
	int ComputeHashKey( unsigned int id ) const;				///< returns a hash key for the given nav area ID

	int WorldToGridX( float wx ) const;							///< given X component, return grid index
	int WorldToGridY( float wy ) const;							///< given Y component, return grid index
	void AllocateGrid( float minX, float maxX, float minY, float maxY );	///< clear and reset the grid to the given extents
	void GridToWorld( int gridX, int gridY, Vector *pos ) const;

	void AddNavArea( CNavArea *area );							///< add an area to the grid
	void RemoveNavArea( CNavArea *area );						///< remove an area from the grid

	void DestroyNavigationMesh( bool incremental = false );		///< free all resources of the mesh and reset it to empty state
	void DestroyHidingSpots( void );

	void ComputeBattlefrontAreas( void );						///< determine areas where rushing teams will first meet

	//----------------------------------------------------------------------------------
	// Place directory
	//
	char **m_placeName;											///< master directory of place names (ie: "places")
	unsigned int m_placeCount;									///< number of "places" defined in placeName[]
	void LoadPlaceDatabase( void );								///< load the place names from a file

	//----------------------------------------------------------------------------------
	// Edit mode
	//
	unsigned int m_navPlace;									///< current navigation place for editing
	void OnEditModeStart( void );								///< called when edit mode has just been enabled
	void DrawEditMode( void );									///< draw navigation areas
	void OnEditModeEnd( void );									///< called when edit mode has just been disabled
	bool m_isEditing;											///< true if in edit mode
	Vector m_editCursorPos;										///< current position of the cursor
	CNavArea *m_markedArea;										///< currently marked area for edit operations
	CNavArea *m_selectedArea;									///< area that is selected this frame
	CNavArea *m_lastSelectedArea;								///< area that was selected last frame
	NavCornerType m_markedCorner;								///< currently marked corner for edit operations
	Vector m_anchor;											///< first corner of an area being created
	bool m_isPlacePainting;										///< if true, we set an area's place by pointing at it
	bool m_splitAlongX;											///< direction the selected nav area would be split
	float m_splitEdge;											///< location of the possible split

	enum NavEditMode {
		NAV_EDIT_NORMAL = 0,
		NAV_EDIT_CREATE,										///< manually creating a new nav area
		NAV_EDIT_RESIZE_HORIZONTAL,
		NAV_EDIT_RESIZE_VERTICAL,
		NAV_EDIT_PLACE											///< place-editing mode
	};
	NavEditMode m_navEditMode;

	bool m_climbableSurface;									///< if true, the cursor is pointing at a climable surface
	Vector m_surfaceNormal;										///< Normal of the surface the cursor is pointing at
	Vector m_ladderAnchor;										///< first corner of a ladder being created
	Vector m_ladderNormal;										///< Normal of the surface of the ladder being created
	bool m_isCreatingLadder;									///< if true, we are manually creating a ladder
	CNavLadder *m_selectedLadder;								///< ladder that is selected this frame
	CNavLadder *m_lastSelectedLadder;							///< ladder that was selected last frame
	CNavLadder *m_markedLadder;									///< currently marked ladder for edit operations

	bool GetActiveNavArea( void );								///< Finds the area or ladder the local player is currently pointing at.  Returns true if a surface was hit by the traceline.

	void SetEditMode( bool isPlaceMode );						///< sets/clears place mode
	void SetPlacePaintingMode( bool painting );					///< Sets place-painting, if we're in place mode
	void SetMarkedLadder( CNavLadder *ladder );					///< mark ladder for further edit operations
	void SetMarkedArea( CNavArea *area );						///< mark area for further edit operations

	CountdownTimer m_showAreaInfoTimer;							///< Timer that controls how long area info is displayed

	//----------------------------------------------------------------------------------
	// Auto-generation
	//
	bool UpdateGeneration( float maxTime = 0.25f );				///< process the auto-generation for 'maxTime' seconds. return false if generation is complete.

	CNavNode *m_currentNode;									///< the current node we are sampling from
	NavDirType m_generationDir;
	CNavNode *AddNode( const Vector &destPos, const Vector &destNormal, NavDirType dir, CNavNode *source );		///< add a nav node and connect it, update current node

	NavLadderList m_ladderList;									///< list of ladder navigation representations
	void BuildLadders( void );
	void DestroyLadders( void );

	bool SampleStep( void );									///< sample the walkable areas of the map
	void CreateNavAreasFromNodes( void );						///< cover all of the sampled nodes with nav areas

	bool TestArea( CNavNode *node, int width, int height );		///< check if an area of size (width, height) can fit, starting from node as upper left corner
	int BuildArea( CNavNode *node, int width, int height );		///< create a CNavArea of size (width, height) starting fom node at upper left corner

	void RemoveUnusedJumpAreas( void );
	void MarkJumpAreas( void );
	void SquareUpAreas( void );
	void MergeGeneratedAreas( void );
	void ConnectGeneratedAreas( void );

	enum GenerationStateType
	{
		SAMPLE_WALKABLE_SPACE,
		CREATE_AREAS_FROM_SAMPLES,
		FIND_HIDING_SPOTS,
		FIND_APPROACH_AREAS,
		FIND_ENCOUNTER_SPOTS,
		FIND_SNIPER_SPOTS,
		FIND_EARLIEST_OCCUPY_TIMES,
		SAVE_NAV_MESH,

		NUM_GENERATION_STATES
	}
	m_generationState;											///< the state of the generation process
	enum GenerationModeType
	{
		GENERATE_NONE,
		GENERATE_FULL,
		GENERATE_INCREMENTAL,
		GENERATE_ANALYSIS_ONLY,
	}
	m_generationMode;						///< true while a Navigation Mesh is being generated
	int m_generationIndex;										///< used for iterating nav areas during generation process
	int m_sampleTick;											///< counter for displaying pseudo-progress while sampling walkable space

	char *m_spawnName;											///< name of player spawn entity, used to initiate sampling

	struct WalkableSeedSpot
	{
		Vector pos;
		Vector normal;
	};
	CUtlLinkedList< WalkableSeedSpot, int > m_walkableSeedList;	///< list of walkable seed spots for sampling

	CNavNode *GetNextWalkableSeedNode( void );					///< return the next walkable seed as a node
	int m_seedIdx;

	void BuildTransientAreaList( void );
	CUtlVector< CNavArea * > m_transientAreas;
};

// the global singleton interface
extern CNavMesh *TheNavMesh;



//--------------------------------------------------------------------------------------------------------------
inline int CNavMesh::ComputeHashKey( unsigned int id ) const
{
	return id & 0xFF;
}

//--------------------------------------------------------------------------------------------------------------
inline int CNavMesh::WorldToGridX( float wx ) const
{ 
	int x = (int)( (wx - m_minX) / m_gridCellSize );

	if (x < 0)
		x = 0;
	else if (x >= m_gridSizeX)
		x = m_gridSizeX-1;
	
	return x;
}

//--------------------------------------------------------------------------------------------------------------
inline int CNavMesh::WorldToGridY( float wy ) const
{ 
	int y = (int)( (wy - m_minY) / m_gridCellSize );

	if (y < 0)
		y = 0;
	else if (y >= m_gridSizeY)
		y = m_gridSizeY-1;
	
	return y;
}


//--------------------------------------------------------------------------------------------------------------
//
// Function prototypes
//

extern void ApproachAreaAnalysisPrep( void );
extern void CleanupApproachAreaAnalysisPrep( void );

#endif // _NAV_MESH_H_

//===== Copyright © 1996-2005, Valve Corporation, All rights reserved. ======//
//
// Purpose: 
//
// $NoKeywords: $
//===========================================================================//

// AI Navigation areas
// Author: Michael S. Booth (mike@turtlerockstudios.com), January 2003

#include "cbase.h"

#include "nav_mesh.h"
#include "nav_node.h"
#include "nav_pathfind.h"
#include "nav_colors.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

extern ConVar nav_area_bgcolor;

LINK_ENTITY_TO_CLASS( info_ladder, CInfoLadder );

BEGIN_DATADESC( CInfoLadder )

	DEFINE_FIELD( mins, FIELD_VECTOR ),
	DEFINE_FIELD( maxs, FIELD_VECTOR ),

END_DATADESC()

unsigned int CNavLadder::m_nextID = 1;

//--------------------------------------------------------------------------------------------------------------
bool CInfoLadder::KeyValue( const char *szKeyName, const char *szValue )
{
	if ( FStrEq( szKeyName, "mins.x" ) )
	{
		mins.x = atof( szValue );
		SetCollisionBounds( mins, maxs );
	}
	else if ( FStrEq( szKeyName, "mins.y" ) )
	{
		mins.y = atof( szValue );
		SetCollisionBounds( mins, maxs );
	}
	else if ( FStrEq( szKeyName, "mins.z" ) )
	{
		mins.z = atof( szValue );
		SetCollisionBounds( mins, maxs );
	}
	else if ( FStrEq( szKeyName, "maxs.x" ) )
	{
		maxs.x = atof( szValue );
		SetCollisionBounds( mins, maxs );
	}
	else if ( FStrEq( szKeyName, "maxs.y" ) )
	{
		maxs.y = atof( szValue );
		SetCollisionBounds( mins, maxs );
	}
	else if ( FStrEq( szKeyName, "maxs.z" ) )
	{
		maxs.z = atof( szValue );
		SetCollisionBounds( mins, maxs );
	}

	return BaseClass::KeyValue( szKeyName, szValue );
}


//--------------------------------------------------------------------------------------------------------------
void CNavLadder::CompressIDs( void )
{
	m_nextID = 1;

	if ( TheNavMesh )
	{
		FOR_EACH_LL( TheNavMesh->GetLadders(), id )
		{
			CNavLadder *ladder = TheNavMesh->GetLadders()[id];
			ladder->m_id = m_nextID++;
		}
	}
}


//--------------------------------------------------------------------------------------------------------------
CNavArea ** CNavLadder::GetConnection( LadderConnectionType dir )
{
	switch ( dir )
	{
	case LADDER_TOP_FORWARD:
		return &m_topForwardArea;
	case LADDER_TOP_LEFT:
		return &m_topLeftArea;
	case LADDER_TOP_RIGHT:
		return &m_topRightArea;
	case LADDER_TOP_BEHIND:
		return &m_topBehindArea;
	case LADDER_BOTTOM:
		return &m_bottomArea;
	}

	return NULL;
}


//--------------------------------------------------------------------------------------------------------------
void CNavLadder::OnSplit( CNavArea *original, CNavArea *alpha, CNavArea *beta )
{
	for ( int con=0; con<NUM_LADDER_CONNECTIONS; ++con )
	{
		CNavArea ** areaConnection = GetConnection( (LadderConnectionType)con );

		if ( areaConnection && *areaConnection == original )
		{
			float alphaDistance = alpha->GetDistanceSquaredToPoint( m_top );
			float betaDistance = beta->GetDistanceSquaredToPoint( m_top );

			if ( alphaDistance < betaDistance )
			{
				*areaConnection = alpha;
			}
			else
			{
				*areaConnection = beta;
			}
		}
	}
}


//--------------------------------------------------------------------------------------------------------------
/**
 * Connect this ladder to given area
 */
void CNavLadder::ConnectTo( CNavArea *area )
{
	float center = (m_top.z + m_bottom.z) * 0.5f;

	if (area->GetCenter().z > center)
	{
		// connect to top
		NavDirType dir;

		Vector dirVector = area->GetCenter() - m_top;
		if ( fabs( dirVector.x ) > fabs( dirVector.y ) )
		{
			if ( dirVector.x > 0.0f ) // east
			{
				dir = EAST;
			}
			else // west
			{
				dir = WEST;
			}
		}
		else
		{
			if ( dirVector.y > 0.0f ) // south
			{
				dir = SOUTH;
			}
			else // north
			{
				dir = NORTH;
			}
		}

		if ( m_dir == dir )
		{
			m_topBehindArea = area;
		}
		else if ( OppositeDirection( m_dir ) == dir )
		{
			m_topForwardArea = area;
		}
		else if ( DirectionLeft( m_dir ) == dir )
		{
			m_topLeftArea = area;
		}
		else
		{
			m_topRightArea = area;
		}
	}
	else
	{
		// connect to bottom
		m_bottomArea = area;
	}
}


//--------------------------------------------------------------------------------------------------------------
/**
 * Destructor
 */
CNavLadder::~CNavLadder()
{
	// tell the other areas we are going away
	FOR_EACH_LL( TheNavAreaList, it )
	{
		CNavArea *area = TheNavAreaList[ it ];

		area->OnDestroyNotify( this );
	}
}


//--------------------------------------------------------------------------------------------------------------
/**
 * invoked when given area is going away
 */
void CNavLadder::OnDestroyNotify( CNavArea *dead )
{
	Disconnect( dead );
}


//--------------------------------------------------------------------------------------------------------------
/**
 * Disconnect this ladder from given area
 */
void CNavLadder::Disconnect( CNavArea *area )
{
	if ( m_topForwardArea == area )
	{
		m_topForwardArea = NULL;
	}
	else if ( m_topLeftArea == area )
	{
		m_topLeftArea = NULL;
	}
	else if ( m_topRightArea == area )
	{
		m_topRightArea = NULL;
	}
	else if ( m_topBehindArea == area )
	{
		m_topBehindArea = NULL;
	}
	else if ( m_bottomArea == area )
	{
		m_bottomArea = NULL;
	}
}


//--------------------------------------------------------------------------------------------------------------
/**
 * returns true if given area is connected in given direction
 */
bool CNavLadder::IsConnected( const CNavArea *area, LadderDirectionType dir ) const
{
	if ( dir == LADDER_DOWN )
	{
		return area == m_bottomArea;
	}
	else if ( dir == LADDER_UP )
	{
		return ( area == m_topForwardArea ||
			area == m_topLeftArea ||
			area == m_topRightArea ||
			area == m_topBehindArea );
	}
	else
	{
		return ( area == m_bottomArea ||
			area == m_topForwardArea ||
			area == m_topLeftArea ||
			area == m_topRightArea ||
			area == m_topBehindArea );
	}
}


//--------------------------------------------------------------------------------------------------------------
void CNavLadder::SetDir( NavDirType dir )
{
	m_dir = dir;

	m_normal.Init();
	AddDirectionVector( &m_normal, m_dir, 1.0f );	// worst-case, we have the NavDirType as a normal

	Vector from = (m_top + m_bottom) * 0.5f + m_normal * 5.0f;
	Vector to = from - m_normal * 32.0f;

	trace_t result;
	UTIL_TraceLine( from, to, MASK_PLAYERSOLID_BRUSHONLY, NULL, COLLISION_GROUP_NONE, &result );

	if (result.fraction != 1.0f)
	{
		bool climbableSurface = physprops->GetSurfaceData( result.surface.surfaceProps )->game.climbable != 0;
		if ( !climbableSurface )
		{
			climbableSurface = (result.contents & CONTENTS_LADDER) != 0;
		}
		if ( climbableSurface )
		{
			m_normal = result.plane.normal;
		}
	}
}


//--------------------------------------------------------------------------------------------------------------
void CNavLadder::DrawLadder( void ) const
{
	CBasePlayer *player = UTIL_GetListenServerHost();
	if (player == NULL)
		return;

	Vector dir;
	const Vector &eye = player->EyePosition();
	AngleVectors( player->EyeAngles() + player->GetPunchAngle(), &dir );

	float dx = eye.x - m_bottom.x;
	float dy = eye.y - m_bottom.y;

	Vector2D eyeDir( dx, dy );
	eyeDir.NormalizeInPlace();
	bool isSelected = ( this == TheNavMesh->GetSelectedLadder() );
	bool isMarked = ( this == TheNavMesh->GetMarkedLadder() );
	bool isFront = DotProduct2D( eyeDir, GetNormal().AsVector2D() ) > 0;

	if ( TheNavMesh->IsPlaceMode() )
	{
		isSelected = isMarked = false;
		isFront = true;
	}

	// Draw 'ladder' lines ----------------------------------------------------
	NavEditColor ladderColor = NavNormalColor;
	if ( isFront )
	{
		if ( isMarked )
		{
			ladderColor = NavMarkedColor;
		}
		else if ( isSelected )
		{
			ladderColor = NavSelectedColor;
		}
		else
		{
			ladderColor = NavSamePlaceColor;
		}
	}
	else if ( isMarked )
	{
		ladderColor = NavMarkedColor;
	}
	else if ( isSelected )
	{
		ladderColor = NavSelectedColor;
	}

	Vector right(0, 0, 0), up( 0, 0, 0 );
	VectorVectors( GetNormal(), right, up );
	right *= m_width * 0.5f;

	Vector bottomLeft = m_bottom - right;
	Vector bottomRight = m_bottom + right;
	Vector topLeft = m_top - right;
	Vector topRight = m_top + right;

	int bgcolor[4];
	if ( 4 == sscanf( nav_area_bgcolor.GetString(), "%d %d %d %d", &(bgcolor[0]), &(bgcolor[1]), &(bgcolor[2]), &(bgcolor[3]) ) )
	{
		for ( int i=0; i<4; ++i )
			bgcolor[i] = clamp( bgcolor[i], 0, 255 );

		if ( bgcolor[3] > 0 )
		{
			Vector offset( 0, 0, 0 );
			AddDirectionVector( &offset, OppositeDirection( m_dir ), 1 );
			NDebugOverlay::Triangle( topLeft+offset, topRight+offset, bottomRight+offset, bgcolor[0], bgcolor[1], bgcolor[2], bgcolor[3], true, 0.15f );
			NDebugOverlay::Triangle( bottomRight+offset, bottomLeft+offset, topLeft+offset, bgcolor[0], bgcolor[1], bgcolor[2], bgcolor[3], true, 0.15f );
		}
	}

	NavDrawLine( topLeft, bottomLeft, ladderColor );
	NavDrawLine( topRight, bottomRight, ladderColor );

	while ( bottomRight.z < topRight.z )
	{
		NavDrawLine( bottomRight, bottomLeft, ladderColor );
		bottomRight += up * (GenerationStepSize/2);
		bottomLeft += up * (GenerationStepSize/2);
	}

	// Draw connector lines ---------------------------------------------------
	if ( !TheNavMesh->IsPlaceMode() )
	{
		Vector bottom = m_bottom;
		Vector top = m_top;

		NavDrawLine( top, bottom, NavConnectedTwoWaysColor );

		if (m_bottomArea)
		{
			float offset = GenerationStepSize;
			const Vector& areaBottom = m_bottomArea->GetCenter();

			 // don't draw the bottom connection too high if the ladder is very short
			if ( top.z - bottom.z < GenerationStepSize * 1.5f )
				offset = 0.0f;

			 // don't draw the bottom connection too high if the ladder is high above the area
			if ( bottom.z - areaBottom.z > GenerationStepSize * 1.5f )
				offset = 0.0f;

			NavDrawLine( bottom + Vector( 0, 0, offset ), areaBottom, ((m_bottomArea->IsConnected( this, LADDER_UP ))?NavConnectedTwoWaysColor:NavConnectedOneWayColor) );
		}

		if (m_topForwardArea)
			NavDrawLine( top, m_topForwardArea->GetCenter(), ((m_topForwardArea->IsConnected( this, LADDER_DOWN ))?NavConnectedTwoWaysColor:NavConnectedOneWayColor) );

		if (m_topLeftArea)
			NavDrawLine( top, m_topLeftArea->GetCenter(), ((m_topLeftArea->IsConnected( this, LADDER_DOWN ))?NavConnectedTwoWaysColor:NavConnectedOneWayColor) );

		if (m_topRightArea)
			NavDrawLine( top, m_topRightArea->GetCenter(), ((m_topRightArea->IsConnected( this, LADDER_DOWN ))?NavConnectedTwoWaysColor:NavConnectedOneWayColor) );

		if (m_topBehindArea)
			NavDrawLine( top, m_topBehindArea->GetCenter(), ((m_topBehindArea->IsConnected( this, LADDER_DOWN ))?NavConnectedTwoWaysColor:NavConnectedOneWayColor) );
	}
}


//--------------------------------------------------------------------------------------------------------------
void CNavLadder::DrawConnectedAreas( void )
{
	CUtlVector< CNavArea * > areas;
	if ( m_topForwardArea )
		areas.AddToTail( m_topForwardArea );
	if ( m_topLeftArea )
		areas.AddToTail( m_topLeftArea );
	if ( m_topRightArea )
		areas.AddToTail( m_topRightArea );
	if ( m_topBehindArea )
		areas.AddToTail( m_topBehindArea );
	if ( m_bottomArea )
		areas.AddToTail( m_bottomArea );

	for ( int i=0; i<areas.Count(); ++i )
	{
		CNavArea *adj = areas[i];

		adj->Draw();

		if ( ! TheNavMesh->IsPlaceMode() )
		{
			adj->DrawHidingSpots();
		}
	}
}


//--------------------------------------------------------------------------------------------------------------
/**
 * Save a navigation ladder to the opened binary stream
 */
void CNavLadder::Save( FileHandle_t file, unsigned int version ) const
{
	// save ID
	filesystem->Write( &m_id, sizeof(unsigned int), file );

	// save extent of ladder
	filesystem->Write( &m_width, sizeof(float), file );

	// save top endpoint of ladder
	filesystem->Write( &m_top, 3*sizeof(float), file );

	// save bottom endpoint of ladder
	filesystem->Write( &m_bottom, 3*sizeof(float), file );

	// save ladder length
	filesystem->Write( &m_length, sizeof(float), file );

	// save direction
	filesystem->Write( &m_dir, sizeof(m_dir), file );

	// save IDs of connecting areas
	unsigned int id;
	id = ( m_topForwardArea ) ? m_topForwardArea->GetID() : 0;
	filesystem->Write( &id, sizeof(id), file );

	id = ( m_topLeftArea ) ? m_topLeftArea->GetID() : 0;
	filesystem->Write( &id, sizeof(id), file );

	id = ( m_topRightArea ) ? m_topRightArea->GetID() : 0;
	filesystem->Write( &id, sizeof(id), file );

	id = ( m_topBehindArea ) ? m_topBehindArea->GetID() : 0;
	filesystem->Write( &id, sizeof(id), file );

	id = ( m_bottomArea ) ? m_bottomArea->GetID() : 0;
	filesystem->Write( &id, sizeof(id), file );
}


//--------------------------------------------------------------------------------------------------------------
/**
 * Load a navigation ladder from the opened binary stream
 */
void CNavLadder::Load( FileHandle_t file, unsigned int version )
{
	// load ID
	filesystem->Read( &m_id, sizeof(unsigned int), file );

	// update nextID to avoid collisions
	if (m_id >= m_nextID)
		m_nextID = m_id+1;

	// load extent of ladder
	filesystem->Read( &m_width, sizeof(float), file );

	// load top endpoint of ladder
	filesystem->Read( &m_top, 3*sizeof(float), file );

	// load bottom endpoint of ladder
	filesystem->Read( &m_bottom, 3*sizeof(float), file );

	// load ladder length
	filesystem->Read( &m_length, sizeof(float), file );

	// load direction
	filesystem->Read( &m_dir, sizeof(m_dir), file );
	SetDir( m_dir ); // regenerate the surface normal

	// load dangling status
	if ( version == 6 )
	{
		bool m_isDangling;
		filesystem->Read( &m_isDangling, sizeof(m_isDangling), file );
	}

	// load IDs of connecting areas
	unsigned int id;
	filesystem->Read( &id, sizeof(id), file );
	m_topForwardArea = TheNavMesh->GetNavAreaByID( id );

	filesystem->Read( &id, sizeof(id), file );
	m_topLeftArea = TheNavMesh->GetNavAreaByID( id );

	filesystem->Read( &id, sizeof(id), file );
	m_topRightArea = TheNavMesh->GetNavAreaByID( id );

	filesystem->Read( &id, sizeof(id), file );
	m_topBehindArea = TheNavMesh->GetNavAreaByID( id );

	filesystem->Read( &id, sizeof(id), file );
	m_bottomArea = TheNavMesh->GetNavAreaByID( id );
	if ( !m_bottomArea )
	{
		DevMsg( "ERROR: Unconnected ladder #%d bottom at ( %g, %g, %g )\n", m_id, m_bottom.x, m_bottom.y, m_bottom.z );
	}

	// can't include behind area, since it is not used when going up a ladder
	if (!m_topForwardArea && !m_topLeftArea && !m_topRightArea)
		DevMsg( "ERROR: Unconnected ladder #%d top at ( %g, %g, %g )\n", m_id, m_top.x, m_top.y, m_top.z );
}


//--------------------------------------------------------------------------------------------------------------
/**
 * Functor returns true if ladder is free, or false if someone is on the ladder
 */
class IsLadderFreeFunctor
{
public:
	IsLadderFreeFunctor( const CNavLadder *ladder, const CBasePlayer *ignore )
	{
		m_ladder = ladder;
		m_ignore = ignore;
	}

	bool operator() ( CBasePlayer *player )
	{
		if (player == m_ignore)
			return true;

		if (!player->IsOnLadder())
			return true;

		// player is on a ladder - is it this one?
		const Vector &feet = player->GetAbsOrigin();

		if (feet.z > m_ladder->m_top.z + HalfHumanHeight)
			return true;

		if (feet.z + HumanHeight < m_ladder->m_bottom.z - HalfHumanHeight)
			return true;

		Vector2D away( m_ladder->m_bottom.x - feet.x, m_ladder->m_bottom.y - feet.y );
		const float onLadderRange = 50.0f;
		return away.IsLengthGreaterThan( onLadderRange );
	}

	const CNavLadder *m_ladder;
	const CBasePlayer *m_ignore;
};

//--------------------------------------------------------------------------------------------------------------
/**
 * Return true if someone is on this ladder
 */
bool CNavLadder::IsInUse( const CBasePlayer *ignore ) const
{
	IsLadderFreeFunctor	isLadderFree( this, ignore );
	return !ForEachPlayer( isLadderFree );
}

//--------------------------------------------------------------------------------------------------------------
Vector CNavLadder::GetPosAtHeight( float height ) const
{
	if ( height < m_bottom.z )
	{
		return m_bottom;
	}

	if ( height > m_top.z )
	{
		return m_top;
	}

	if ( m_top.z == m_bottom.z )
	{
		return m_top;
	}

	float percent = ( height - m_bottom.z ) / ( m_top.z - m_bottom.z );

	return m_top * percent + m_bottom * ( 1.0f - percent );
}

//--------------------------------------------------------------------------------------------------------------

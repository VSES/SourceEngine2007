//========= Copyright © 1996-2005, Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
// $NoKeywords: $
//=============================================================================//

#include "cbase.h"
#include "filters.h"
#include "EntityList.h"
#include "info_act.h"

//-----------------------------------------------------------------------------
// Purpose: Filter that filters by act number
//-----------------------------------------------------------------------------
class FilterAct : public CBaseFilter
{
	DECLARE_CLASS( FilterAct, CBaseFilter );
public:
	DECLARE_DATADESC();

	FilterAct( void )
	{
		m_iFilterAct = -1;
	}

	int		m_iFilterAct;
	bool	PassesFilter(CBaseEntity *pEntity)
	{
		// -1 means filter true when no act is active
		if ( m_iFilterAct == -1 && !g_hCurrentAct )
			return true;
	 	return ( g_hCurrentAct->ActNumber() == m_iFilterAct );
	}
};

LINK_ENTITY_TO_CLASS( filter_act, FilterAct );

BEGIN_DATADESC( FilterAct )

	// Keyfields
	DEFINE_KEYFIELD( m_iFilterAct,	FIELD_INTEGER,	"filteract" ),

END_DATADESC()


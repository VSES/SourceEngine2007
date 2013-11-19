//========= Copyright © 1996-2006, Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
// $NoKeywords: $
//=============================================================================//

#ifndef C_OBJ_SAPPER_H
#define C_OBJ_SAPPER_H
#ifdef _WIN32
#pragma once
#endif

#include "tf_obj_baseupgrade_shared.h"

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
class C_ObjectSapper : public C_BaseObjectUpgrade
{
	DECLARE_CLASS( C_ObjectSapper, C_BaseObjectUpgrade );
public:
	DECLARE_CLIENTCLASS();

	C_ObjectSapper() {}

	virtual void ClientThink( void );
	virtual void OnDataChanged( DataUpdateType_t type );
	virtual void GetStatusText( wchar_t *pStatus, int iMaxStatusLen );

	virtual bool	IsHostileUpgrade( void ) { return true; }

private:
	C_ObjectSapper( const C_ObjectSapper & ); // not defined, not accessible
};


#endif // C_OBJ_SAPPER_H
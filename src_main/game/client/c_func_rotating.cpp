//===== Copyright © 1996-2005, Valve Corporation, All rights reserved. ======//
//
// Purpose: 
//
//===========================================================================//

#include "cbase.h"
#include <KeyValues.h>

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
class C_FuncRotating : public C_BaseEntity
{
public:
	DECLARE_CLASS( C_FuncRotating, C_BaseEntity );
	DECLARE_CLIENTCLASS();

	C_FuncRotating();

private:
};

extern void RecvProxy_SimulationTime( const CRecvProxyData *pData, void *pStruct, void *pOut );

IMPLEMENT_CLIENTCLASS_DT( C_FuncRotating, DT_FuncRotating, CFuncRotating )
	RecvPropVector( RECVINFO_NAME( m_vecNetworkOrigin, m_vecOrigin ) ),
	RecvPropQAngles( RECVINFO_NAME( m_angNetworkAngles, m_angRotation ) ),
	RecvPropInt( RECVINFO(m_flSimulationTime), 0, RecvProxy_SimulationTime ),
END_RECV_TABLE()


//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
C_FuncRotating::C_FuncRotating()
{
}

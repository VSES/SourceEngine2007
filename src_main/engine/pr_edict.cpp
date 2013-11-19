//========= Copyright © 1996-2005, Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
// $Workfile:     $
// $Date:         $
//
//-----------------------------------------------------------------------------
// $Log: $
//
// $NoKeywords: $
//=============================================================================//
#include "quakedef.h"
#include <stddef.h>
#include "vengineserver_impl.h"
#include "server.h"
#include "pr_edict.h"
#include "world.h"
#include "ispatialpartition.h"
#include "utllinkedlist.h"
#include "framesnapshot.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

// Edicts won't get reallocated for this many seconds after being freed.
#define EDICT_FREETIME	1.0



#ifdef _DEBUG
#include <malloc.h>
#endif // _DEBUG

static ConVar		sv_useexplicitdelete( "sv_useexplicitdelete", "1", FCVAR_DEVELOPMENTONLY, "Explicitly delete dormant client entities caused by AllowImmediateReuse()." );

/*
=================
ED_ClearEdict

Sets everything to NULL, done when new entity is allocated for game.dll
=================
*/
void ED_ClearEdict (edict_t *e)
{
	e->ClearFree();
	e->ClearStateChanged();
	e->SetChangeInfoSerialNumber( 0 );
	
	serverGameEnts->FreeContainingEntity(e);
	InitializeEntityDLLFields(e);

	e->m_NetworkSerialNumber = -1;  // must be filled by game.dll
}

/*
=================
ED_Alloc

Either finds a free edict, or allocates a new one.
Try to avoid reusing an entity that was recently freed, because it
can cause the client to think the entity morphed into something else
instead of being removed and recreated, which can cause interpolated
angles and bad trails.
=================
*/


edict_t *ED_Alloc( int iForceEdictIndex )
{
	if ( iForceEdictIndex >= 0 )
	{
		if ( iForceEdictIndex >= sv.num_edicts )
		{
			Warning( "ED_Alloc( %d ) - invalid edict index specified.", iForceEdictIndex );
			return NULL;
		}
		
		edict_t *e = &sv.edicts[iForceEdictIndex];
		if ( e->IsFree() )
		{
			ED_ClearEdict( e );
			return e;
		}
		else
		{
			return NULL;
		}
	}

	// Check the free list first.
	edict_t *pEdict = sv.edicts + sv.GetMaxClients() + 1;

	for ( int i=sv.GetMaxClients()+1; i < sv.num_edicts; i++ )
	{
		if ( pEdict->IsFree() && (pEdict->freetime < 2 || sv.GetTime() - pEdict->freetime >= EDICT_FREETIME) )
		{
			// If we have no freetime, we've had AllowImmediateReuse() called. We need
			// to explicitly delete this old entity.
			if ( pEdict->freetime == 0 && sv_useexplicitdelete.GetBool() )
			{
				//Warning("ADDING SLOT to snapshot: %d\n", i );
				framesnapshotmanager->AddExplicitDelete( i );
			}

			ED_ClearEdict( pEdict );
			return pEdict;
		}

		pEdict++;
	}
	
	// Allocate a new edict.
	if ( sv.num_edicts >= sv.max_edicts )
	{
		AssertMsg( 0, "Can't allocate edict" );
		if ( sv.max_edicts == 0 )
			Sys_Error( "ED_Alloc: No edicts yet" );
		Sys_Error ("ED_Alloc: no free edicts");
	}

	// Do this before clearing since clear now needs to call back into the edict to deduce the index so can get the changeinfo data in the parallel structure
	sv.num_edicts++;

	ED_ClearEdict( pEdict );
	
	return pEdict;
}


void ED_AllowImmediateReuse()
{
	edict_t *pEdict = sv.edicts + sv.GetMaxClients() + 1;
	for ( int i=sv.GetMaxClients()+1; i < sv.num_edicts; i++ )
	{
		if ( pEdict->IsFree() )
		{
			pEdict->freetime = 0;
		}

		pEdict++;
	}
}


/*
=================
ED_Free

Marks the edict as free
FIXME: walk all entities and NULL out references to this entity
=================
*/
void ED_Free (edict_t *ed)
{
	if (ed->IsFree())
	{
#ifdef _DEBUG
//		ConDMsg("duplicate free on '%s'\n", pr_strings + ed->classname );
#endif
		return;
	}

	// don't free player edicts
	if ( (ed - sv.edicts) >= 1 && (ed - sv.edicts) <= sv.GetMaxClients() )
		return;

	// release the DLL entity that's attached to this edict, if any
	serverGameEnts->FreeContainingEntity( ed );

	ed->SetFree();
	ed->freetime = sv.GetTime();

	// Increment the serial number so it knows to send explicit deletes the clients.
	ed->m_NetworkSerialNumber++; 
}

//
// 	serverGameEnts->FreeContainingEntity( pEdict )  frees up memory associated with a DLL entity.
// InitializeEntityDLLFields clears out fields to NULL or UNKNOWN.
// Release is for terminating a DLL entity.  Initialize is for initializing one.
//
void InitializeEntityDLLFields( edict_t *pEdict )
{
	// clear all the game variables
	size_t sz = offsetof( edict_t, m_pUnk ) + sizeof( void* );
	memset( ((byte*)pEdict) + sz, 0, sizeof(edict_t) - sz );
}

edict_t *EDICT_NUM(int n)
{
	if (n < 0 ||
		n >= sv.max_edicts)
		Sys_Error ("EDICT_NUM: bad number %i", n);
	return &sv.edicts[n];
}

int NUM_FOR_EDICT(const edict_t *e)
{
	int		b;
	
	b = e - sv.edicts;
	
	if (b < 0 || b >= sv.num_edicts)
		Sys_Error ("NUM_FOR_EDICT: bad pointer");
	return b;
}

// Special version which allows accessing unused sv.edictchangeinfo slots
int NUM_FOR_EDICTINFO( const edict_t * e )
{
	int		b;
	b = e - sv.edicts;
	
	if (b < 0 || b >= sv.max_edicts)
		Sys_Error ("NUM_FOR_EDICTINFO: bad pointer");
	return b;
}

IChangeInfoAccessor *CBaseEdict::GetChangeAccessor()
{
	int idx = NUM_FOR_EDICTINFO( (const edict_t * )this );
	return &sv.edictchangeinfo[ idx ];
}

const IChangeInfoAccessor *CBaseEdict::GetChangeAccessor() const
{
	int idx = NUM_FOR_EDICTINFO( (const edict_t * )this );
	return &sv.edictchangeinfo[ idx ];
}
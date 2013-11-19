//========= Copyright © 1996-2005, Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
// $NoKeywords: $
//
//=============================================================================//
#if !defined( PROTO_VERSION_H )
#define PROTO_VERSION_H
#ifdef _WIN32
#pragma once
#endif

// The current network protocol version.  Changing this makes clients and servers incompatible
#define PROTOCOL_VERSION    14

#define DEMO_BACKWARDCOMPATABILITY

// For backward compatability of demo files
#define PROTOCOL_VERSION_12		12

#endif
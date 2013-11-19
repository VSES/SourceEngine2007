//====== Copyright © 1996-2004, Valve Corporation, All rights reserved. =======
//
// Purpose: 
//
//=============================================================================

#ifndef DMSERIALIZERXML_H
#define DMSERIALIZERXML_H

#ifdef _WIN32
#pragma once
#endif


//-----------------------------------------------------------------------------
// Forward declarations
//-----------------------------------------------------------------------------
class IDataModel;


//-----------------------------------------------------------------------------
// Installation methods for standard serializers
//-----------------------------------------------------------------------------
void InstallXMLSerializer( IDataModel *pFactory );


#endif // DMSERIALIZERXML_H
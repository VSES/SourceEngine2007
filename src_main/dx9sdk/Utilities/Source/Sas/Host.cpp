//--------------------------------------------------------------------------------------
// Copyright (c) Microsoft Corporation. All rights reserved.
//--------------------------------------------------------------------------------------
#include "stdafx.h"
#include "Host.h"
using namespace std;
using namespace Sas;

BoundValueTable* Host::GetRootTable() 
{
	bool firstTime = true;
	if( firstTime )
	{
		firstTime = false;
		try
		{
			WCharToWString( L"Default", s_DefaultsTable.Name );
			WCharToWString( L"Root", s_RootTable.Name );
		}
		catch( ... )
		{}
	}

	return &s_RootTable; 
}

//--------------------------------------------------------------------------------------
IMessageListener* Host::s_pMessageListener = NULL;
#if defined(DEBUG) | defined(_DEBUG)
MessageLevel Host::s_pMessageLevelBar = LEVEL_MAX;
#else
MessageLevel Host::s_pMessageLevelBar = LEVEL_STATUS;
#endif
BoundValueTable Host::s_DefaultsTable;
BoundValueTable Host::s_RootTable( &s_DefaultsTable );
Integrator* Host::s_Integrator = NULL;
Integrator Host::s_DefaultIntegrator;
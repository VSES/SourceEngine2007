//========= Copyright © 1996-2005, Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
// $NoKeywords: $
//
//=============================================================================//
#include <stdafx.h>
#include "Uninstall.h"

extern "C" 
{
__declspec(dllexport) void UninstInitialize(void);
__declspec(dllexport) void UninstUnInitialize(void);
}


void UninstInitialize(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	//AfxMessageBox("UII Test");
	theApp.DoPreUninstallStuff();
}
void UninstUnInitialize(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	theApp.DoPostUninstallStuff();
}

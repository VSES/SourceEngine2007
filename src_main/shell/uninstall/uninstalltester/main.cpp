//========= Copyright © 1996-2005, Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
// $NoKeywords: $
//
//=============================================================================//
#include <windows.h>


void main(int argc, char* argv[])
{
	 if (argc != 2)
		return;
	
	char path[256]={0};

	 
	
	strcpy(path,(const char*) argv[1]);
	
	
	HINSTANCE hDLL=LoadLibrary(path);

	FARPROC init = GetProcAddress(hDLL,"UninstInitialize");
	DWORD error1=GetLastError();
	
	FARPROC uninit = GetProcAddress(hDLL,"UninstUnInitialize");
	DWORD error2=GetLastError();
	
	
	if (init)
		init();
	else
		MessageBox(NULL,"Couldn't link to UninstInitialize","Oops!",MB_OK|MB_ICONEXCLAMATION);
	
	if (uninit)
		uninit();
	else
		MessageBox(NULL,"Couldn't link to UninstUnInitialize","Oops!",MB_OK|MB_ICONEXCLAMATION);

	FreeLibrary(hDLL);
}
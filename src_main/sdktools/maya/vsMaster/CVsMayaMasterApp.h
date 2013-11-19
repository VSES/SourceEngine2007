//======= Copyright © 1996-2006, Valve Corporation, All rights reserved. ======
//
// Purpose:
//
//=============================================================================

#ifndef CVSMAYAMASTERAPP_H
#define CVSMAYAMASTERAPP_H
#if defined( _WIN32 )
#pragma once
#endif

#include "appframework/appframework.h"
#include "stringpool.h"
#include "utlvector.h"
#include "UtlStringMap.h"

class CVsMayaMasterApp : public CSteamAppSystemGroup
{
	typedef CSteamAppSystemGroup BaseClass;

public:
	// Get a list of required AppSystems from a secondary plugin
	void LoadPlugin( const char *pModuleName, const char *pModulePath );

	// Add a single AppSystem
    void AddAppSystem( const char *pModuleName, const char *pInterfaceName );

	// Add a bunch of AppSystems
	void AddAppSystems(	const AppSystemInfo_t *pAppSystemList );

	// Methods of IApplication
	virtual bool Create();
	virtual bool PreInit();
	virtual int Main() { return 1; }
	virtual void PostShutdown();
	virtual void Destroy();
	virtual int Startup();
	virtual void Shutdown();

private:
	// Are we running in interactive mode?
	bool IsInteractive() const;

	// Window creation
	void* Create3DWindow( char const *pTitle, bool bWindowed, int w, int h );

	CCountedStringPool m_stringPool;
	CUtlVector<AppSystemInfo_t> m_appSystems;
	CUtlStringMap<HMODULE> m_modules;	// Map from module name to handles to the modules (MLLs, i.e. maya Plugins) this thing loaded
};


#endif // CVSMAYAMASTERAPP_H

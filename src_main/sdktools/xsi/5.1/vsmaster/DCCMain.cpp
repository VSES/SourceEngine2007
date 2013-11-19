//===== Copyright © 1996-2005, Valve Corporation, All rights reserved. ======//
//
// Purpose: Class for dealing with vgui focus issues across all plugins
//
// $NoKeywords: $
//===========================================================================//

#include "idccmain.h"
#include "tier3/tier3.h"


//-----------------------------------------------------------------------------
// Externs
//-----------------------------------------------------------------------------
extern bool g_Initialized;


//-----------------------------------------------------------------------------
// Implementation class
//-----------------------------------------------------------------------------
class CDCCMain : public CTier3AppSystem< IDCCMain >
{
	typedef CTier3AppSystem< IDCCMain > BaseClass;

public:
	CDCCMain(void);
	virtual ~CDCCMain(void);

	// Inherited from IDCCMain
	virtual bool IsInitialized( ); 

private:
};


//-----------------------------------------------------------------------------
// Purpose: singleton accessor
//-----------------------------------------------------------------------------
static CDCCMain s_DCCMain;
EXPOSE_SINGLE_INTERFACE_GLOBALVAR( CDCCMain, IDCCMain, DCC_MAIN_INTERFACE_VERSION, s_DCCMain );


//-----------------------------------------------------------------------------
// Constructor, destructor
//-----------------------------------------------------------------------------
CDCCMain::CDCCMain(void) : BaseClass( false )
{
}

CDCCMain::~CDCCMain(void)
{
}


//-----------------------------------------------------------------------------
// Has the plugin system been set up for use?
//-----------------------------------------------------------------------------
bool CDCCMain::IsInitialized( )
{
	return g_Initialized;
}




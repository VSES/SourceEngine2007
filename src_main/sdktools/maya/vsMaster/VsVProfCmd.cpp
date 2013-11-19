//======= Copyright © 1996-2006, Valve Corporation, All rights reserved. ======
//
// Purpose: Implementation of CVsVProf
//
//=============================================================================

// Maya includes
#include <maya/MArgDatabase.h>
#include <maya/MArgList.h>
#include <maya/MGlobal.h>
#include <maya/MSyntax.h>

// Valve includes
#include "tier0/vprof.h"
#include "valveMaya.h"
#include "VsMayaMPxFactory.h"


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
SpewOutputFunc_t g_pOldSpewFunc( NULL );
FILE *g_pSpewFile( NULL );


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
class CVsVProf : public CVsMayaMPxCommand
{
public:
	// Inheritied from MPxCommand
	virtual MStatus doIt(
		const MArgList &mArgList );

	virtual bool isUndoable() const { return false; }

	virtual bool hasSyntax() const { return true; }

	// Inheritied from CVsMayaCommand
	void SpecifySyntax(
		MSyntax &mSyntax,
		vm::CMSyntaxHelp &help );
};


//-----------------------------------------------------------------------------
// Hook the command so we can see it in Maya
//-----------------------------------------------------------------------------
INSTALL_MAYA_MPXCOMMAND(
	CVsVProf,
	vsVProf,
	"Controls Valve profiling in Maya" );


//-----------------------------------------------------------------------------
// Purpose: MSyntax factory for the command
// Input  :
// Output : A Maya MSyntax class describing command's command line syntax
//-----------------------------------------------------------------------------

static const char *const kOptHelp( "h" );
static const char *const kOptStart( "sta" );
static const char *const kOptStop( "sto" );
static const char *const kOptFilename( "f" );

void CVsVProf::SpecifySyntax(
	MSyntax &mSyntax,
	ValveMaya::CMSyntaxHelp &help )
{
	help.AddFlag( mSyntax, kOptHelp, "help", "General",
		"Prints this information" );

	help.AddFlag( mSyntax, kOptStart, "start", "General",
		"Turns on Valve profiling" );

	help.AddFlag( mSyntax, kOptStop, "stop", "General",
		"Turns off Valve profiling, if previously turned on, and writes the report to the script editor" );

	help.AddFlag( mSyntax, kOptFilename, "filename", "General",
		"When turning off Valve profiling, if -filename is specified, the report is dumped to the specified file instead of the Maya script editor",
		MSyntax::kString );

	mSyntax.enableEdit( false );
}


//-----------------------------------------------------------------------------
// Spews stuff to a filename - note... because of the nature of Spew the
// file to spew to is defined by a global...
//-----------------------------------------------------------------------------
SpewRetval_t SpewToFileFunc(
	SpewType_t type,
	char const *pMsg )
{	
	if ( g_pSpewFile )
	{
		fputs( pMsg, g_pSpewFile );
		return SPEW_CONTINUE;
	}
	
	if ( g_pOldSpewFunc )
		return (*g_pOldSpewFunc)( type, pMsg );

	return SPEW_CONTINUE;
}


//-----------------------------------------------------------------------------
// Purpose: When this command is executed by Maya for the first time,
//          a new instance of the command class is created and this function
//          is called with the specified command line arguments
// Input  : The command line arguments specified by the user
// Output : MStatus indicating success or failure of the command
//          In order for the command to be kept in the undo stack
//          the return value must be MStatus::kSuccess
//          Also the virtual isUndoable() has to return true for the
//          command to be kept in the undo stack for later undoing
//-----------------------------------------------------------------------------

MStatus CVsVProf::doIt(
	const MArgList &mArgList )
{
	MStatus mStatus( MS::kSuccess );
	MArgDatabase mArgDatabase( syntax(), mArgList, &mStatus );

	if ( mStatus )
	{
		if ( mArgDatabase.isFlagSet( kOptHelp ) )
		{
			GetSyntaxHelp()->PrintHelp( GetName(), GetDesc() );
		}
		else if ( mArgDatabase.isFlagSet( kOptStart ) )
		{
			if ( g_VProfCurrentProfile.IsEnabled() )
			{
				mwarn << "vproj is already started" << std::endl;
				return MS::kSuccess;
			}
			g_VProfCurrentProfile.Start();
		}
		else if ( mArgDatabase.isFlagSet( kOptStop ) )
		{
			if ( !g_VProfCurrentProfile.IsEnabled() )
			{
				merr << "vproj isn't started" << std::endl;
				return MS::kFailure;
			}

			g_VProfCurrentProfile.MarkFrame();
			g_VProfCurrentProfile.Stop();

			if ( mArgDatabase.isFlagSet( kOptFilename ) )
			{
				MString optFilename;
				mArgDatabase.getFlagArgument( kOptFilename, 0, optFilename );
				g_pSpewFile = fopen( optFilename.asChar(), "w" );
				if ( g_pSpewFile )
				{
					g_pOldSpewFunc = GetSpewOutputFunc();
					SpewOutputFunc( SpewToFileFunc );
					g_VProfCurrentProfile.OutputReport();
					SpewOutputFunc( g_pOldSpewFunc );
					fflush( g_pSpewFile );
					fclose( g_pSpewFile );
					g_pSpewFile = NULL;
					g_pOldSpewFunc = NULL;

					setResult( optFilename );
				}
				else
				{
					merr << "Cannot open " << optFilename << " for writing" << std::endl;
					g_VProfCurrentProfile.OutputReport();
				}
			}
			else
			{
				g_VProfCurrentProfile.OutputReport();
			}

			g_VProfCurrentProfile.Reset();
		}
	}

	return mStatus;
}
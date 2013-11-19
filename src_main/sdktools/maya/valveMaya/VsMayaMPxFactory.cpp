//======= Copyright © 1996-2006, Valve Corporation, All rights reserved. ======
//
// Purpose:
//
//=============================================================================

#include <assert.h>

#pragma warning( disable : 4702 )

#include <map>
#include <string>

// Tells Maya not to define the version string when including MFnPlugin.h
#define MNoVersionString 1

// Maya includes
#include <maya/MFnPlugIn.h>
#include <maya/MGlobal.h>

// Valve includes
#include "valveMaya.h"
#include "VsMayaMPxFactory.h"

//-----------------------------------------------------------------------------
//
// Automatic Plugin Factory Maya command to print out information about what's
// in the plugin.  All plugins will get a command named the same as the plugin
//
//-----------------------------------------------------------------------------

class CVsMayaMPxFactoryCommand : public CVsMayaMPxCommand
{
public:
	virtual MStatus doIt( const MArgList & );

	// Inheritied from CVsMayaMPxCommand
	virtual void SpecifySyntax( MSyntax &mSyntax, vm::CMSyntaxHelp &help ) { }
};


// Name & Desc are irrelevant at this point, they need to be redefined at runtime
INSTALL_MAYA_MPXCOMMAND( CVsMayaMPxFactoryCommand, vsMayaMPxFactory, "" );


//-----------------------------------------------------------------------------
// Display information about this plugin and what sort of stuff it contains
//-----------------------------------------------------------------------------
MStatus CVsMayaMPxFactoryCommand::doIt( const MArgList & )
{
	CVsMayaMPxFactoryBase::DisplaySummary( GetName() );

	return MS::kSuccess;
}


//-----------------------------------------------------------------------------
//
// Helper class to install MPx derived stuff
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Static members
//-----------------------------------------------------------------------------
CVsMayaMPxFactoryBase *CVsMayaMPxFactoryBase::s_pFirstFactory( NULL );


//-----------------------------------------------------------------------------
// Constructor
//-----------------------------------------------------------------------------
CVsMayaMPxFactoryBase::CVsMayaMPxFactoryBase()
{
	// Hook into the list of translator factories
	m_pNextFactory = s_pFirstFactory;
	s_pFirstFactory = this;
}


//-----------------------------------------------------------------------------
// Register everything
//-----------------------------------------------------------------------------
MStatus CVsMayaMPxFactoryBase::RegisterEverything( MFnPlugin &pluginFn )
{
	MStatus retVal;

	// Set the name & description of the CVsMayaMPxFactoryCommand making sure not to conflict with
	// an already existing command

	MString pluginCommandName( pluginFn.name() );

	bool nameOk( true );
	for ( const CVsMayaMPxFactoryBase *pFactory( s_pFirstFactory ); pFactory; pFactory = pFactory->m_pNextFactory )
	{
		if ( pFactory->GetName() == pluginCommandName )
		{
			pluginCommandName += "PluginInfo";
			nameOk = false;
			break;
		}
	}

	for ( int suffix( 0 ); !nameOk; nameOk = true, ++suffix )
	{
		for ( const CVsMayaMPxFactoryBase *pFactory( s_pFirstFactory ); pFactory; pFactory = pFactory->m_pNextFactory )
		{
			if ( pFactory->GetName() == pluginCommandName )
			{
				pluginCommandName += suffix;
				nameOk = false;
				continue;
			}
		}
	}

	CVsMayaMPxCommandDecorator< CVsMayaMPxFactoryCommand >::s_name = pluginCommandName;

	CVsMayaMPxCommandDecorator< CVsMayaMPxFactoryCommand >::s_desc =
		MString( "Displays information about and what's inside of the " ) + pluginFn.name() + " plugin.";

	for ( const CVsMayaMPxFactoryBase *pFactory( s_pFirstFactory ); pFactory; pFactory = pFactory->m_pNextFactory )
	{
		const MStatus regStat( pFactory->Register( pluginFn ) );

		if ( !regStat )
		{
			MGlobal::displayWarning(
				pluginFn.name() + ": Couldn't Register \"" + pFactory->GetName() + "\"" );
			retVal = regStat;
		}
	}

	return retVal;
}

//-----------------------------------------------------------------------------
// Deregister everything
//-----------------------------------------------------------------------------
MStatus CVsMayaMPxFactoryBase::DeregisterEverything( MFnPlugin &pluginFn )
{
	MStatus retVal;

	for ( const CVsMayaMPxFactoryBase *pFactory( s_pFirstFactory ); pFactory; pFactory = pFactory->m_pNextFactory )
	{
		const MStatus regStat( pFactory->Deregister( pluginFn ) );

		if ( !regStat )
		{
			MGlobal::displayWarning(
				pluginFn.name() + ": Couldn't Deregister \"" + pFactory->GetName() + "\"" );
			retVal = regStat;
		}
	}

	return retVal;
}


void CVsMayaMPxFactoryBase::DisplaySummary( const MString &pluginName )
{
	minfo << "\n";
	minfo << "Maya Plugin " << pluginName.asChar() << " Contains The Following:\n";

	typedef std::multimap< std::string, CVsMayaMPxFactoryBase * > ByType_t;
	ByType_t byType;

	// Categorize all of the installed stuff by the type of thing
	for ( CVsMayaMPxFactoryBase *pF( s_pFirstFactory ); pF; pF = pF->m_pNextFactory )
	{
		byType.insert( ByType_t::value_type( pF->GetTypeName().asChar(), pF ) );
	}

	// Now loop through each category, organizing everything by the name of the thing

	typedef std::map< std::string, CVsMayaMPxFactoryBase * > ByName_t;

	for ( ByType_t::const_iterator bti( byType.begin() ); bti != byType.end(); )
	{
		minfo << "\n";
		minfo << bti->first << "s" << ":\n";
		minfo << "\n";

		ByName_t byName;
		for ( const ByType_t::const_iterator ubi( byType.upper_bound( bti->first ) ); bti != ubi; ++bti )
		{
			byName.insert( ByName_t::value_type( bti->second->GetName().asChar(), bti->second ) );
		}

		for ( ByName_t::const_iterator bni( byName.begin() ); bni != byName.end(); ++bni )
		{
			minfo << " * " << bni->first << ": " << bni->second->GetDesc().asChar() << "\n";
		}
	}

	minfo << "\n";
}


//-----------------------------------------------------------------------------
// Map a Type to a string
//-----------------------------------------------------------------------------
MString CVsMayaMPxFactoryBase::GetTypeName() const
{
	// NOTE: Ensure this array matches up to CVsMayaMpxFactoryBase::Type enum

	static const char *const typeNames[] =
	{
		"Command",
		"File Translator",
		"Dependency Node",
		"Shader Node",
		"Transform Node",
		// Insert new ones here
		"Unknown"
	};

	return MString( typeNames[ static_cast< int >( clamp( GetType(), kCommand, kUnknown ) ) ] );
}

//=============================================================================
//
// Base class for Valve Maya commands ( CVsMayaMPxCommand )
//
//=============================================================================

//-----------------------------------------------------------------------------
// Default implementation
//-----------------------------------------------------------------------------
void CVsMayaMPxCommand::SpecifySyntax( MSyntax & /* syntax */, vm::CMSyntaxHelp &help ) 
{
	assert( !hasSyntax() );
}


//-----------------------------------------------------------------------------
// Register
//-----------------------------------------------------------------------------
MStatus CVsMayaMPxCommand::Register(
	MFnPlugin &pluginFn,
	const MString &name,
	MCreatorFunction creatorFunction,
	MCreateSyntaxFunction createSyntaxFunction )
{
	MString commandName( name );
	MStatus regStat = pluginFn.registerCommand( commandName, creatorFunction, createSyntaxFunction );

	if ( !regStat )
		return regStat;

	commandName.toLowerCase();
	return pluginFn.registerCommand( commandName, creatorFunction, createSyntaxFunction );
}


//-----------------------------------------------------------------------------
// Deregister
//-----------------------------------------------------------------------------
MStatus CVsMayaMPxCommand::Deregister(
	MFnPlugin &pluginFn,
	const MString &name )
{
	MString commandName( name );
	MStatus regStat = pluginFn.deregisterCommand( commandName );

	if ( !regStat )
		return regStat;

	commandName.toLowerCase();
	return pluginFn.deregisterCommand( commandName );
}


//=============================================================================
//
// Base class for Valve Maya file translators ( CVsMayaMPxFileTranslator )
//
//=============================================================================

//-----------------------------------------------------------------------------
// Register
//-----------------------------------------------------------------------------
MStatus CVsMayaMPxFileTranslator::Register(
	MFnPlugin &pluginFn,
	const MString &name,
	const MString &guiName,
	MCreatorFunction creatorFunction )
{
	const MString pixmapName( name + ".xpm" );
	const MString optionsScriptName( name + "TranslatorOptions" );

	return pluginFn.registerFileTranslator(
		guiName,
		const_cast<char *>( pixmapName.asChar() ),
		creatorFunction,
		const_cast<char *>( optionsScriptName.asChar() ),
		"",
		false );
}


//-----------------------------------------------------------------------------
// Deregister
//-----------------------------------------------------------------------------
MStatus CVsMayaMPxFileTranslator::Deregister(
	MFnPlugin &pluginFn,
	const MString &name )
{
	return pluginFn.deregisterFileTranslator( name );
}


//=============================================================================
//
// Base class for Valve Maya Dependency Nodes ( CVsMayaMPxNode )
//
//=============================================================================

//-----------------------------------------------------------------------------
// Register
//-----------------------------------------------------------------------------
MStatus CVsMayaMPxNode::Register(
	MFnPlugin &pluginFn,
	const MString &name,
	const MTypeId &typeId,
	MCreatorFunction creatorFunction,
	MInitializeFunction initFunction,
	const MString &classification )
{
	return pluginFn.registerNode(
		name,
		typeId,
		creatorFunction,
		initFunction,
		MPxNode::kDependNode,
		classification.length() ? &classification : NULL );
}


//-----------------------------------------------------------------------------
// Deregister
//-----------------------------------------------------------------------------
MStatus CVsMayaMPxNode::Deregister(
	MFnPlugin &pluginFn,
	const MTypeId &typeId )
{
	return pluginFn.deregisterNode( typeId );
}


//=============================================================================
//
// Base class for Valve Maya Transform Nodes ( CVsMayaMPxTransform )
//
//=============================================================================

//-----------------------------------------------------------------------------
// Register
//-----------------------------------------------------------------------------
MStatus CVsMayaMPxTransform::Register(
	MFnPlugin &pluginFn,
	const MString &name,
	const MTypeId &typeId,
	MCreatorFunction creatorFunction,
	MInitializeFunction initFunction,
	MCreatorFunction xformCreatorFunction,
	const MTypeId &xformTypeId,
	const MString *classification )
{
	return pluginFn.registerTransform(
		name,
		typeId,
		creatorFunction,
		initFunction,
		xformCreatorFunction,
		xformTypeId,
		NULL );
}


//-----------------------------------------------------------------------------
// Deregister
//-----------------------------------------------------------------------------
MStatus CVsMayaMPxTransform::Deregister(
	MFnPlugin &pluginFn,
	const MTypeId &typeId )
{
	return pluginFn.deregisterNode( typeId );
}


//=============================================================================
//
// Base class for Valve Maya Locator Nodes ( CVsMayaMPxLocatorNode )
//
//=============================================================================

//-----------------------------------------------------------------------------
// Register
//-----------------------------------------------------------------------------
MStatus CVsMayaMPxLocatorNode::Register(
	MFnPlugin &pluginFn,
	const MString &name,
	const MTypeId &typeId,
	MCreatorFunction creatorFunction,
	MInitializeFunction initFunction )
{
	return pluginFn.registerNode(
		name,
		typeId,
		creatorFunction,
		initFunction,
		MPxNode::kLocatorNode );
}


//-----------------------------------------------------------------------------
// Deregister
//-----------------------------------------------------------------------------
MStatus CVsMayaMPxLocatorNode::Deregister(
	MFnPlugin &pluginFn,
	const MTypeId &typeId )
{
	return pluginFn.deregisterNode( typeId );
}


//=============================================================================
//
// Base class for Valve Maya Drag And Drop Behaviors ( CVsMayaMPxDragAndDropBehavior )
//
//=============================================================================

//-----------------------------------------------------------------------------
// Register
//-----------------------------------------------------------------------------
MStatus CVsMayaMPxDragAndDropBehavior::Register(
	MFnPlugin &pluginFn,
	const MString &name,
	MCreatorFunction creatorFunction )
{
	return pluginFn.registerDragAndDropBehavior(
		name,
		creatorFunction );
}


//-----------------------------------------------------------------------------
// Deregister
//-----------------------------------------------------------------------------
MStatus CVsMayaMPxDragAndDropBehavior::Deregister(
	MFnPlugin &pluginFn,
	const MString &name )
{
	return pluginFn.deregisterDragAndDropBehavior( name );
}


//=============================================================================
//
// Base class for Valve Maya Shape Nodes ( CVsMayaMPxShapeNode )
//
//=============================================================================

//-----------------------------------------------------------------------------
// Register
//-----------------------------------------------------------------------------
MStatus CVsMayaMPxShapeNode::Register(
	MFnPlugin &pluginFn,
	const MString &name,
	const MTypeId &typeId,
	MCreatorFunction creatorFunction,
	MInitializeFunction initFunction,
	MCreatorFunction uiCreatorFunction )
{
	return pluginFn.registerShape(
		name,
		typeId,
		creatorFunction,
		initFunction,
		uiCreatorFunction );
}


//-----------------------------------------------------------------------------
// Deregister
//-----------------------------------------------------------------------------
MStatus CVsMayaMPxShapeNode::Deregister(
	MFnPlugin &pluginFn,
	const MTypeId &typeId )
{
	return pluginFn.deregisterNode( typeId );
}


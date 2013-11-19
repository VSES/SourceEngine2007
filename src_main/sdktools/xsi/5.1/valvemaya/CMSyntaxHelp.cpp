//======= Copyright © 1996-2006, Valve Corporation, All rights reserved. ======
//
// Purpose: CMSyntaxHelp Defintion
//
//			A helper class for Maya's MSyntax.  Stores extra help information
//			and prints out help messages, etc...
//
//=============================================================================

// C++ Includes

#include <iostream>

// Maya includes

#include <maya/MStringArray.h>
#include <maya/MGlobal.h>

// Local includes

#include "valveMaya.h"

//-----------------------------------------------------------------------------
//
// Purpose: Clear all of the help data from the system
// Input:
// Output:
//
//-----------------------------------------------------------------------------

void
ValveMaya::CMSyntaxHelp::Clear()
{
	m_groupOrder.Purge();
	m_groupedHelp.Clear();

	m_helpCount = 0;
	m_shortNameLength = 0;
}


//-----------------------------------------------------------------------------
//
// Purpose:	Add an argument and its documentation to the help system
// Input:	i_mSyntax	The Maya MSyntax class that's holding this flag
//			i_shortName	The short flag name
//			i_longName	The long flag name
//			i_group		The group this flag belongs to (for help formatting only, can be the empty string, or NULL)
//			i_help		The help for this flag, any kind of text is allowed
//			i_argType1	The Maya MSyntax::MArgType for the first arg
//			i_argType2	The Maya MSyntax::MArgType for the second arg
//			i_argType3	The Maya MSyntax::MArgType for the third arg
//			i_argType4	The Maya MSyntax::MArgType for the fourth arg
//			i_argType5	The Maya MSyntax::MArgType for the fifth arg
//			i_argType6	The Maya MSyntax::MArgType for the sixth arg
// Output:	MS::kSuccess if everything is ok, MS::kFailure otherwise
//
//-----------------------------------------------------------------------------

MStatus
ValveMaya::CMSyntaxHelp::AddFlag(
	MSyntax &i_mSyntax,
	const char *i_shortName,
	const char *i_longName,
	const char *i_group,
	const char *i_help,
	const MSyntax::MArgType i_argType1,
	const MSyntax::MArgType i_argType2,
	const MSyntax::MArgType i_argType3,
	const MSyntax::MArgType i_argType4,
	const MSyntax::MArgType i_argType5,
	const MSyntax::MArgType i_argType6)
{
	// Validate passed data
	if ( !i_shortName )
		return MS::kFailure;	// Invalid

	if ( !i_longName )
		i_longName = i_shortName;

	i_mSyntax.addFlag(i_shortName, i_longName, i_argType1, i_argType2, i_argType3, i_argType4, i_argType5, i_argType6);

	// If group is NULL or the empty string set to "default", otherwise use the passed value
	i_group = i_group ? Q_strlen( i_group ) ? i_group : "default" : "default";

	// See if the group already exists, if not add it to the group order list
	if ( !m_groupedHelp.Defined( i_group ) )
	{
		if ( Q_strcmp( i_group, "default" ) == 0 )
		{
			m_groupOrder.AddToHead( "default" );
		}
		else
		{
			m_groupOrder.AddToTail( m_stringPool.ReferenceString( i_group ) );
		}
	}

	// Now store the help data, sorted by group
	CUtlVector<HelpData_t> &helpDataVector( m_groupedHelp[ i_group ] );
	HelpData_t &helpData( helpDataVector[ helpDataVector.AddToTail() ] );
	helpData.m_shortName =	m_stringPool.ReferenceString( i_shortName );
	helpData.m_longName =	m_stringPool.ReferenceString( i_longName );
	helpData.m_help =		i_help ? m_stringPool.ReferenceString( i_help ) : "";

	CUtlVector<MSyntax::MArgType> &argTypes( helpData.m_argTypes );

	MSyntax::MArgType iArgTypes[6];
	iArgTypes[0] = i_argType1;
	iArgTypes[1] = i_argType2;
	iArgTypes[2] = i_argType3;
	iArgTypes[3] = i_argType4;
	iArgTypes[4] = i_argType5;
	iArgTypes[5] = i_argType6;

	for ( int ati( 0 ); ati < 6; ++ati )
	{
		const MSyntax::MArgType argType( iArgTypes[ati] );

		if ( argType != MSyntax::kInvalidArgType && argType != MSyntax::kNoArg ) 
		{
			if ( argType == MSyntax::kLastArgType && argTypes.Count() ) 
			{
				argTypes.AddToTail( argTypes[argTypes.Count() - 1] );
			}
			else 
			{
				argTypes.AddToTail( argType );
			}
		}
	}

	if ( Q_strlen( i_shortName ) > m_shortNameLength - 1 )
	{
		m_shortNameLength = Q_strlen( i_shortName ) + 1;
	}

	++m_helpCount;

	return MS::kSuccess;
}


//-----------------------------------------------------------------------------
//
// Purpose:	Outputs the collected help information in a nice format
// Input:	i_cmdName		The name of the command (printed first)
//			i_cmdDesc		A description of what the command does
//			i_lineLength	How long the lines should be allowed to be before
//							wrapping if 0 and Maya is interactive will figure
//							out based on the size of the script editor window
//							(roughly), if Maya is not interactive will be 79
// Output:
//
//-----------------------------------------------------------------------------

void ValveMaya::CMSyntaxHelp::PrintHelp(
	const char *const i_cmdName,
	const char *const i_cmdDesc,
	int i_lineLength)
{
	// See if caller wants the line length to be computed automatically

	if ( i_lineLength == 0 )
	{
		if ( MGlobal::mayaState() == MGlobal::kInteractive )
		{
			int w( 0 );
			MGlobal::executeCommand( "window -q -w CommandWindow", w );
			if ( w > 0 )
			{
				i_lineLength = ( w - 60 ) / 6;	// Works on my Maya but fonts maybe change?
			}
		}
		else
		{
			i_lineLength = 79;
		}
	}

	std::ostream &out( minfo );

	// Shortcuts

	const char *const nl("\n");	// Newline
	const char *const bs("");	// Bold Start
	const char *const be("");	// Bold End
	const char *const is("");	// Italics Start
	const char *const ie("");	// Italics End
	const char *const gt(">");	// Greater Than
	const char *const lt("<");	// Less Than
	const char *const ns(" ");	// Non-Breaking Space

	// Output command name

	out << nl << i_cmdName << nl;

	// Output the command description

	out << nl << bs << "DESCRIPTION" << be << ":" << nl << nl;

	out << i_cmdDesc << nl;

	// Output the command line switches

	out << nl << bs << "SWITCHES" << be << ":" << nl;

	if ( m_helpCount <= 1 )
	{
		out << nl;
	}

	for ( int gi( 0 ); gi != m_groupOrder.Count(); ++gi )
	{
		// Output group name if it isn't default

		if ( Q_strcmp( m_groupOrder[ gi ], "default" ) )
		{
			out << nl << m_groupOrder[ gi ] << " Switches:" << nl << nl;
		}

		const CUtlVector<HelpData_t> &groupedHelp( m_groupedHelp[ m_groupOrder[ gi ] ] );

		for ( int hi( 0 ); hi < groupedHelp.Count(); ++hi )
		{
			const HelpData_t &helpData( groupedHelp[ hi ] );
			out << '-' << helpData.m_shortName << ns << '-' << helpData.m_longName;

			const CUtlVector<MSyntax::MArgType> &argTypes( helpData.m_argTypes );

			for ( int ati( 0 ); ati < argTypes.Count(); ++ati )
			{
				out << ns << lt << is;

				switch ( argTypes[ ati ] )
				{
				case MSyntax::kBoolean:
					out << "Boolean";
					break;
				case MSyntax::kLong:
					out << "Long";
					break;
				case MSyntax::kDouble:
					out << "Double";
					break;
				case MSyntax::kString:
					out << "String";
					break;
				case MSyntax::kUnsigned:
					out << "Unsigned";
					break;
				case MSyntax::kDistance:
					out << "Distance";
					break;
				case MSyntax::kAngle:
					out << "Angle";
					break;
				case MSyntax::kTime:
					out << "Time";
					break;
				case MSyntax::kSelectionItem:
					out << "SelectionItem";
					break;
				default:
					out << "Unknown";
					break;
				}

				out << ie << gt;
			}

			out << nl << " * " << helpData.m_help << nl;
		}
	}
}
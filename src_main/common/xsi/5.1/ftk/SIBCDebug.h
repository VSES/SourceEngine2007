//***************************************************************************************
//
// File supervisor: Softimage Games/Interactive team
//
// (c) Copyright 2001 Avid Technology, Inc. . All rights reserved.
//
// @doc
//
// @module      CSIDebug.h | Main header file for CSIDebug implementation
//***************************************************************************************

/****************************************************************************************
THIS CODE IS PUBLISHED AS A SAMPLE ONLY AND IS PROVIDED "AS IS".
IN NO EVENT SHALL SOFTIMAGE, AVID TECHNOLOGY, INC. AND/OR THEIR RESPECTIVE 
SUPPLIERS BE LIABLE FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR ANY
DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN
CONNECTION WITH THE USE OR PERFORMANCE OF THIS CODE . 
 
COPYRIGHT NOTICE. Copyright © 1999-2001 Avid Technology Inc. . All rights reserved. 

SOFTIMAGE is a registered trademark of Avid Technology Inc. or its subsidiaries 
or divisions. Windows NT is a registered trademark of Microsoft Corp. All other
trademarks contained herein are the property of their respective owners. 
****************************************************************************************/

// ******************* OBJECT SCRIPTING DOCUMENTATION ******************************
// <object id="CSIDebug" introduced="X.XX">
// <description> Debug mechanism. </description>
// <methods>
//  <!-- method - - - - - - - - - - - - - - - - - - - - - - - - -->
// <method id="CSIDebug.CSIDebug">
// <description> CSIDebug Default constructor </description>
// <parameters>
// <parameter name="void" required="yes" io="Input">
// 	<type>Void</type>
// 	<description> Void </description>
// </parameter>
// </parameters>
// <return>Void</return>
// </method>
//  <!-- method - - - - - - - - - - - - - - - - - - - - - - - - -->
// <method id="CSIDebug.Dump">
// <description> Dump debugging information. </description>
// <parameters>
// <parameter name="void" required="yes" io="Input">
// 	<type>Void</type>
// 	<description> Void </description>
// </parameter>
// </parameters>
// <return>Void</return>
// </method>
//  <!-- method - - - - - - - - - - - - - - - - - - - - - - - - -->
// <method id="CSIDebug.RegisterLabel">
// <description> Register Label. </description>
// <parameters>
// <parameter name="in_sLabel" required="yes" io="Input">
// 	<type>char*</type>
// 	<description> Label </description>
// </parameter>
// <parameter name="in_sDescription" required="yes" io="Input">
// 	<type>char</type>
// 	<description> Description </description>
// </parameter>
// <parameter name="in_bSet" required="yes" io="Input">
// 	<type>bool</type>
// 	<description> Set </description>
// </parameter>
// </parameters>
// <return>Returns number of used Labels. (int)</return>
// </method>
//  <!-- method - - - - - - - - - - - - - - - - - - - - - - - - -->
// <method id="CSIDebug.CheckLabel">
// <description> Check the label. </description>
// <parameters>
// <parameter name="in_iLabel" required="yes" io="Input">
// 	<type>int</type>
// 	<description> Label </description>
// </parameter>
// </parameters>
// <return>Returns bool. (SI_Bool)</return>
// </method>
//  <!-- method - - - - - - - - - - - - - - - - - - - - - - - - -->
// <method id="CSIDebug.LookupLabel" status="not-impl">
// <description> Get label index </description>
// <parameters>
// <parameter name="in_sLabel" required="yes" io="Input">
// 	<type>char*</type>
// 	<description> Label </description>
// </parameter>
// </parameters>
// <return>Returns label index. (int)</return>
// </method>
//  <!-- method - - - - - - - - - - - - - - - - - - - - - - - - -->
// <method id="CSIDebug.Printf">
// <description> Print out debugginf information. </description>
// <parameters>
// <parameter name="in_iLabel" required="yes" io="Input">
// 	<type>int</type>
// 	<description> Label </description>
// </parameter>
// <parameter name="in_sFormat" required="yes" io="Input">
// 	<type>char*</type>
// 	<description> Character string. </description>
// </parameter>
// <parameter name="..." required="yes" io="Input">
// 	<type></type>
// 	<description> Argument list </description>
// </parameter>
// </parameters>
// <return>Returns 0 if no info.</return>
// </method>
//  <!-- method - - - - - - - - - - - - - - - - - - - - - - - - -->
// <method id="CSIDebug.ParseArguments">
// <description> Parses arguments. </description>
// <parameters>
// <parameter name="io_nArgs" required="yes" io="Input">
// 	<type>int*</type>
// 	<description> Number of I/O arguments. </description>
// </parameter>
// <parameter name="io_asArhs" required="yes" io="Input">
// 	<type>char**</type>
// 	<description> I/O arguments. </description>
// </parameter>
// </parameters>
// <return>Void</return>
// </method>
//  <!-- method - - - - - - - - - - - - - - - - - - - - - - - - -->
// <method id="CSIDebug.Set">
// <description> Set the Label. </description>
// <parameters>
// <parameter name="in_sLabel" required="yes" io="Input">
// 	<type>int</type>
// 	<description> Label </description>
// </parameter>
// </parameters>
// <return>Void</return>
// </method>
// </methods>
// </object>



#ifndef __CSIBCDebug_H__
#define __CSIBCDebug_H__

///**************************************************************************************
// Includes
///**************************************************************************************
#ifndef _WIN32_WCE
#include <assert.h>
#else
#endif

#include "SIBCArray.h"
#include "SIBCString.h"


//**************************************************************************************
// @class CSIDebug | Debug mechanism.
//**************************************************************************************

#if defined( DEBUG ) && defined( _PSX2 )

class CSIDebug
{
public:
	CSIDebug();																		// Constructor
	~CSIDebug();																	// Destructor
	void Dump();																	// Prints out all labels
	int RegisterLabel( char * in_sLabel, char * in_sDescription, bool in_bSet );	// Label registration
	int LookupLabel( char * in_sLabel );											// Get label index
	bool CheckLabel( int in_iLabel );												// Check if label exist
	int Printf( int in_iLabel, char *, ... );										// Print method
	void ParseArguments( int * in_nArgs, char ** in_asArgs );						// Parse arguments
	void Set( int in_sLabel );														// Activate label
	
private:
	CSIBCArray < CSIBCString >	m_LabelArray;
	CSIBCArray < CSIBCString >	m_DescriptionArray;
	CSIBCArray < bool >	m_ValueArray;
};

extern int			g_iVerbose;
extern CSIDebug	* 	g_pSIDebug;

///**************************************************************************************
// Defines
///**************************************************************************************
#define CSIDebugRegisterLabel( v, l, d ) v = g_pSIDebug->RegisterLabel( l, d, false )
#define CSIDebugLookupLabel( l ) g_pSIDebug->CheckLabel( i )
#define CSIDebugCheck( i ) g_pSIDebug->CheckLabel( i )
#define CSIDebugDump( i ) g_pSIDebug->Dump()
#define CSIDebugPrintf g_pSIDebug->Printf
#define CSIDebugPrintf0( i, f ) g_pSIDebug->Printf( i, f )
#define CSIDebugPrintf1( i, f, a ) g_pSIDebug->Printf( i, f, a )
#define CSIDebugPrintf2( i, f, a, b ) g_pSIDebug->Printf( i, f, a, b )
#define CSIDebugPrintf3( i, f, a, b, c ) g_pSIDebug->Printf( i, f, a, b, c )
#define CSIDebugPrintf4( i, f, a, b, c, d ) g_pSIDebug->Printf( i, f, a, b, c, d )
#define CSIDebugLabel( name ) static int name
#define CSIDebugParseArguments( n, a ) g_pSIDebug->ParseArguments( n, a )
#define CSIDebugDeclare() CSIDebug	* g_pSIDebug
#define CSIDebugInit() g_pSIDebug = new CSIDebug()
#define CSIDebugCleanUp() delete g_pSIDebug
#define CSIDebugSet( i ) g_pSIDebug->Set( i );
#else
#define CSIDebugRegisterLabel( v, l, d ) 0
#define CSIDebugLookupLabel( l ) 0
#define CSIDebugCheck( i ) 0
#define CSIDebugDump( i )
#define CSIDebugSet( i )
#define CSIDebugPrintf This shouldnt compile
#define CSIDebugPrintf0( i, f )
#define CSIDebugPrintf1( i, f, a )
#define CSIDebugPrintf2( i, f, a, b )
#define CSIDebugPrintf3( i, f, a, b, c )
#define CSIDebugPrintf4( i, f, a, b, c, d )
#define CSIDebugLabel( name )
void CSIDebugParseArguments( int * in_nArgs, char ** in_asArgs );
#define CSIDebugDeclare()
#define CSIDebugInit()
#define CSIDebugCleanUp()
#endif

///**************************************************************************************
// Inline code
///**************************************************************************************





#endif // __CSIBCDebug_H__

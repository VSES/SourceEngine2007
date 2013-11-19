/****************************************************************************************
THIS CODE IS PUBLISHED AS A SAMPLE ONLY AND IS PROVIDED "AS IS".
IN NO EVENT SHALL SOFTIMAGE, AVID TECHNOLOGY, INC. AND/OR THEIR RESPECTIVE 
SUPPLIERS BE LIABLE FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR ANY
DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN
CONNECTION WITH THE USE OR PERFORMANCE OF THIS CODE . 
 
COPYRIGHT NOTICE. Copyright © 1999-2002 Avid Technology Inc. . All rights reserved. 

SOFTIMAGE is a registered trademark of Avid Technology Inc. or its subsidiaries 
or divisions. Windows NT is a registered trademark of Microsoft Corp. All other
trademarks contained herein are the property of their respective owners. 
****************************************************************************************/

// XSIViewerOptions.h: interface for the CXSIViewerOptions class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XSIVIEWEROPTIONS_H__452867F7_25DB_11D4_B5CA_009027BC3804__INCLUDED_)
#define AFX_XSIVIEWEROPTIONS_H__452867F7_25DB_11D4_B5CA_009027BC3804__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "SIBCUtil.h"
#include "SIBCArray.h"
#include "SIBCString.h"

/** 
* This class help the reading and writing of XSI Viewer configuration files.
*
* \b Usage
*
\code
		CXSIViewerOptions	options;
		options.m_Path = "c:\\MyConfigFile.cfg";
		options.Read();

		int nbOptions = options.Tokens().GetUsed();
		int loop;

		//
		// Print out the tokens and their values
		//

		for(loop = 0; loop < nbOptions; loop++)
		{
			printf ( "Token: %s",options.Tokens()[loop].GetText() );
			printf ( "= %s\n",options.Values()[loop].GetText() );
		}

		//
		// Add a new token
		//

		options.Tokens().Extend(1);
		options.Values().Extend(1);

		options.Tokens()[options.Tokens().GetUsed() - 1] = new CSIBCString("MY_NEW_TOKEN");
		options.Values()[options.Values().GetUsed() - 1] = new CSIBCString("MY_NEW_TOKEN_VALUE");

		//
		// Write new config file
		//

		options.Write();

\endcode
*
* In addition to using the CXSIViewOptions class directly as shown above, you may get access to the default XSIViewer options by retrieving the CXSIViewerOptions object from the CSIViewerApplication.
*
* \sa CSIViewerApplication::GetViewerOptions
*/
class XSIEXPORT CXSIViewerOptions  
{
public:
	//***********************************************************************
	// Constructor/destructor
	//***********************************************************************
	//! constructor for CXSIViewerOptions
	/*! 
	\return A pointer to the new CXSIViewerOptions
	*/ 
	CXSIViewerOptions();

	//! destructor for CXSIViewerOptions
	/*! 
	\return nothing
	*/ 
	virtual ~CXSIViewerOptions();

	//! Read the configuration files and build a list of tokens and values
	/*! 
	\return SI_Error	success or failure code
	*/ 
	SI_Error		Read();

	//! Write the tokens and their value to the configuration file
	/*! 
	\return SI_Error	success or failure code
	*/ 
	SI_Error		Write();

	//! Returns the array of tokens
	/*! 
	\return CSIBCArray<CSIBCString *>&	Array of tokens
	*/ 
	CSIBCArray<CSIBCString *>	&Tokens() {return m_Tokens;}

	//! Returns the array of values
	/*! 
	\return CSIBCArray<CSIBCString *>&	Array of values
	*/ 
	CSIBCArray<CSIBCString *>	&Values() {return m_Values;}

	//! Find a token by name
	/*! 
	\param	SI_Char*	The token name
	\return int			Index into the array of tokens returned by CXSIViewerOptions::Tokens()
	*/ 
	int							FindToken(SI_Char *in_pToken);

	//! Path to the configuration file
	CSIBCString					m_Path;
private:

	CSIBCArray<CSIBCString *>	m_Tokens;
	CSIBCArray<CSIBCString *>	m_Values;

};

#endif // !defined(AFX_XSIVIEWEROPTIONS_H__452867F7_25DB_11D4_B5CA_009027BC3804__INCLUDED_)

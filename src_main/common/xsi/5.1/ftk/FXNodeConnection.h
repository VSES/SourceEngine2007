//***************************************************************************************
//
// File supervisor: Softimage 3D Games & 3D Bridge team
//
// (c) Copyright 2001-2002 Avid Technology, Inc. . All rights reserved.
//
//***************************************************************************************

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

#ifndef _FXOPERATORCONNECTION_H
#define _FXOPERATORCONNECTION_H

#include "Template.h"
#include "Scene.h"

class CSLFXOperator;

//! Representation of a connection in the FX Tree
class XSIEXPORT CSLFXOperatorConnection
{
	friend CSLTemplate *CSLScene::XSIFXOperatorCallback(CdotXSITemplate *in_pTemplate, SLContext *in_pContext);
public:

	//! Defines the various types of connection
	enum EConnectionType
	{
		SI_NONE,			//!<not connected
		SI_FXOPERATOR,		//!<connected to an effect operator
	};
	
	/*! Constructor
	* \param in_pTemplate Parent Template
	* \param in_nValueIndex Index
	* \return an instance of CSLFCOperatorConnection
	*/	
	CSLFXOperatorConnection
	( 
		CdotXSITemplate *in_pTemplate, 
		SI_Int in_nValueIndex 
	);

	/*! Commits semantic layer data
	* \return SI_Error Whether or not commit was a success
	*/
	SI_Error Synchronize();

	/*! Gets the name of the connection
	* \return SI_Char Name of the connection
	*/
	SI_Char* GetName();

	/*! Sets the name of the connection
	* \param in_szName The name of the connection
	*/
	SI_Void SetName( SI_Char* in_szName );

	/*! Gets the refered node
	* \return CSLFXOperator Pointer to the refered node or null if none
	*/
	CSLFXOperator* GetFXOperator();

	/*! Sets the refered node
	* \param in_pNode Pointer to the refered node or null if none
	*/
	SI_Void SetFXOperator( CSLFXOperator *in_pNode);

	/*! Gets the connection type
	* \return EConnectionType Type of connection
	*/
	EConnectionType GetConnectionType();

	/*! Sets the connection type
	* \param in_Type New type of connection
	*/
	SI_Void SetConnectionType( EConnectionType in_Type );

	//! Destructor
    virtual ~CSLFXOperatorConnection();

private:
	SI_Char* GetFXOperatorName();

    CSLFXOperator *m_pReferedNode;
	EConnectionType m_ConnectionType;
	CdotXSIParam *m_pNameParam;
	CdotXSIParam *m_pTypeParam;

	SI_Void *m_pReserved;

};

#endif

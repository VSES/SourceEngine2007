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

#ifndef _ENVELOPELIST_H
#define _ENVELOPELIST_H

#include "Template.h"

// Forward declaration
class CSLEnvelope;
 
//! Describes the envelope list.
class XSIEXPORT CSLEnvelopeList
	: public CSLTemplate
{
public:
	/*! Constructor
	* \param in_pScene	Pointer to the scene containing the envelope
	* \param in_pModel	Pointer to the model containing the envelope
	* \param in_pTemplate	Pointer to the CdotXSITemplate containing the envelope
	* \return an instance of CSLEnvelope
	*/
	CSLEnvelopeList(CSLScene* in_pScene, CSLModel *in_pModel, CdotXSITemplate* in_pTemplate);

	/*! Destructor
	*/
	virtual ~CSLEnvelopeList();

	/*!	Add a new envelope to the list.
	* \return	CSLEnvelope*	Pointer to the newly created envelope object.
	*/
	CSLEnvelope* AddEnvelope();

	/*!	Return the number of envelopes in the list.
	* \return	SI_Int	Number of envelopes.
	*/
	SI_Int GetEnvelopeCount();

	/*!	Return an array of all envelopes in the list.
	* \return	CSLEnvelope**	Array of pointers to envelope objects.
	*/
	CSLEnvelope** Envelopes();

	/*!	Remove a single envelope from the list.
	* \param	in_nIndex	Index of the envelope to remove.
	* \return	SI_Error	Error code.
	*/
	SI_Error RemoveEnvelope(SI_Int in_nIndex);

	/*!	Remove a single envelope from the list.
	* \param	in_pEnvelope	Pointer to the envelope to remove.
	* \return	SI_Error	Error code.
	*/
	SI_Error RemoveEnvelope(CSLEnvelope* in_pEnvelope);

	/*!	Remove all envelopes from the list.
	* \return	SI_Error	Error code.
	*/
	SI_Error ClearEnvelopes();

	// Connection functions
	/*! Connects a new envelope
	* \param in_pNewEnvelope the envelope to connect
	* \return CSLEnvelope the connected envelope
	*/
	CSLEnvelope* ConnectEnvelope(CSLEnvelope* in_pNewEnvelope);

	/*! Synchronizes this envelope list instance with its IO layer representation
	* \return SI_Error
	* \retval SI_SUCCESS this function always returns SI_SUCCESS
	*/
    virtual SI_Error Synchronize();

	/*! Get the template type
	* \return ETemplateType the template type of the envelope list
	*/
	virtual ETemplateType Type() { return SI_ENVELOPE_LIST; }

private:
	CSIBCArray<CSLEnvelope *> m_Envelopes;

	void *m_pReserved;	// reserved for future extension
};

#endif

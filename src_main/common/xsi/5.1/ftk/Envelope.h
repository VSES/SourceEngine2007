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

#ifndef _ENVELOPE_H
#define _ENVELOPE_H

#include "Template.h"



//! Describes the weight of a vertex.
struct SLVertexWeight
{
	SI_Float m_fVertexIndex;
	SI_Float m_fWeight;
};

#ifndef _SL_VERTEX_WEIGHT_ARRAY_PROXY_EXPORT_
#define _SL_VERTEX_WEIGHT_ARRAY_PROXY_EXPORT_

//XSITEMPLATEEXP template class XSIEXPORT CSLArrayProxy<SLVertexWeight, SI_Float, 2>;

#endif // _SL_VERTEX_WEIGHT_ARRAY_PROXY_EXPORT_



//! Describes an envelope.
class XSIEXPORT CSLEnvelope
	: public CSLTemplate
{
public:
	/**
	* Proxy class that allows modifying a list of SLVertexWeight like an array.
	*/
	typedef CSLArrayProxy<SLVertexWeight, SI_Float, 2> CSLVertexWeightArray;

	/*! Constructor
	* \param in_pScene	Pointer to the scene containing the envelope
	* \param in_pModel	Pointer to the model containing the envelope
	* \param in_pTemplate	Pointer to the CdotXSITemplate containing the envelope
	* \return an instance of CSLEnvelope
	*/
	CSLEnvelope(CSLScene* in_pScene, CSLModel *in_pModel, CdotXSITemplate* in_pTemplate);

	/*! Destructor
	*/
	virtual ~CSLEnvelope();

	/*!	Return the model that deforms this envelope.
	* \return	CSLModel*	Pointer to the deformer model.
	*/
	CSLModel* GetDeformer();

	/*!	Change the model deforming the envelope.
	* \param	in_pNewDeformer	New deforming model object.
	* \return	None
	*/
	SI_Void SetDeformer(CSLModel* in_pNewDeformer);

	/*!	Return the model that represent this envelope.
	* \return	CSLModel*	Pointer to the enveloped model.
	*/
	CSLModel* GetEnvelope();

	/*!	Change the model represented by the envelope.
	* \param	in_pNewEnvelope	New envelope model object.
	* \return	None
	*/
	SI_Void SetEnvelope(CSLModel* in_pNewEnvelope);

	/*! Return the number of vertex weights.
	* \return SI_Int	Number of vertex weights.
	*/
	SI_Int GetVertexWeightCount();

	/*! Return the vertex weights in an array proxy.
	* \return CSLVertexWeightArray*	Pointer to an array proxy of vertex weights.
	*/
	CSLVertexWeightArray* GetVertexWeightList();

	/*!	Returns an array of SLVertexWeight containing the vertex weights.
	* \return SLVertexWeight*	Array of vertex weights.
	*/
	SLVertexWeight* GetVertexWeightListPtr();

	/*! Synchronizes this envelope instance with its IO layer representation
	* \return SI_Error
	* \retval SI_SUCCESS this function always returns SI_SUCCESS
	*/
    virtual SI_Error Synchronize();

	/*! Get the template type
	* \return ETemplateType the template type of the envelope
	*/
	virtual ETemplateType Type() { return SI_ENVELOPE; }

private:
	CSLModel* m_pEnvelope;
	CSLModel* m_pDeformer;
	CSLVertexWeightArray m_VertexWeights;

	void *m_pReserved;	// reserved for future extension
};

#endif


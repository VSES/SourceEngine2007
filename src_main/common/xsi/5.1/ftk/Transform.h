/******************************************************************************\
*
* File:          Transform.h
* Creation date: January 15, 2002
* Purpose:       Declaration of class 'Transform'
*
\******************************************************************************/
#ifndef _TRANSFORM_H
#define _TRANSFORM_H

#include "Template.h"

/** 
* CSLTransform class
* This class handles the transformation matrix of a model. It provides functions to set
* the local scaling, rotation and position of the model and also provides easy and fast 
* access to the model's global tranform.
*/
class XSIEXPORT CSLTransform
    : public CSLTemplate
{
public:
    CSLTransform(CSLScene* in_pScene, CSLModel *in_pModel, CdotXSITemplate* in_pTemplate);
    virtual ~CSLTransform();

	/*!	Return the transformation matrix.
		\return	CSIBCMatrix4x4	Transformation matrix.
	*/
	CSIBCMatrix4x4 GetMatrix();

	/*!	Change the transformation matrix.
		\param	CSIBCMatrix4x4&	New Transformation matrix.
		\return	None
	*/
	SI_Void SetMatrix(CSIBCMatrix4x4 &in_rValue);

	/*!	Return the actual rotation in euler representation.
		\return	CSIBCVector3D	Rotation value.
	*/
	CSIBCVector3D GetEulerRotation();

	/*!	Change the rotation value in euler representation.
		\param	CSIBCVector3D&	New rotation value
		\return	None
	*/
	SI_Void SetEulerRotation(CSIBCVector3D &in_rValue);

	/*!	Return the actual scale.
		\return	CSIBCVector3D	Scale value.
	*/
	CSIBCVector3D GetScale();

	/*!	Change the scale value.
		\param	CSIBCVector3D&	New scale value
		\return	None
	*/
	SI_Void SetScale(CSIBCVector3D &in_rValue);

	/*!	Return the actual translation.
		\return	CSIBCVector3D	Translation value.
	*/
	CSIBCVector3D GetTranslation();

	/*!	Change the translation value.
		\param	CSIBCVector3D&	New translation value
		\return	None
	*/
	SI_Void SetTranslation(CSIBCVector3D &in_rValue);

	/*!	Computes this CSLTransform's local matrix.
	\return	Error code
	*/
	SI_Error ComputeLocalMatrix();

	/*!	Computes this CSLTransform's global matrix.
	\return	Error code
	*/
	SI_Error ComputeGlobalMatrix();

	/*!	Returns the global matrix
		\return	CSIBCMatrix4x4 The global matrix
	*/
	CSIBCMatrix4x4 &GetGlobalMatrix();

	/*!	Sets the global transformation matrix.
		\param	CSIBCMatrix4x4&	New Transformation matrix.
		\return	None
	*/
	SI_Void SetGlobalMatrix(CSIBCMatrix4x4 &in_rValue);

	virtual CSLAnimatableType* ParameterFromName(SI_Char *in_szName);
    virtual CSLAnimatableType* ParameterFromType(EFCurveType in_Type, SI_Char *in_szParameterName);
    virtual ETemplateType Type();

private:
    CSLVector3DProxy m_Scale;
    CSLVector3DProxy m_Rotation;
    CSLVector3DProxy m_Translation;
	CSIBCMatrix4x4 m_TransformMatrix;
	CSIBCMatrix4x4 m_GlobalMatrix;

	void *m_pReserved;	// reserved for future extension
};

#endif

/******************************************************************************\
*
* File:          Transform.h
* Creation date: January 15, 2002
* Purpose:       Declaration of class 'Transform'
*
\******************************************************************************/
#ifndef _XSITRANSFORM_H
#define _XSITRANSFORM_H

#include "Template.h"

/** 
* CSLXSITransform class
* This class handles the XSI transformation matrix of a model. It provides functions to set
* all aspects of the XSI transformation like SRT, pivots, shearing, etc.
*/
class XSIEXPORT CSLXSITransform
    : public CSLTemplate
{
public:
    CSLXSITransform(CSLScene* in_pScene, CSLModel *in_pModel, CdotXSITemplate* in_pTemplate);
    virtual ~CSLXSITransform();

	/*!	Return the transformation matrix.
		\return	CSIBCMatrix4x4	Transformation matrix.
	*/
	CSIBCMatrix4x4 GetMatrix();

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

	/*!	Return the shear.
		\return	CSIBCVector3D	Shear value.
	*/
	CSIBCVector3D GetShear();

	/*!	Change the translation value.
		\param	CSIBCVector3D&	New translation value
		\return	None
	*/
	SI_Void SetShear(CSIBCVector3D &in_rValue);

	/*!	Return the PivotPosition.
		\return	CSIBCVector3D	PivotPosition value.
	*/
	CSIBCVector3D GetPivotPosition();

	/*!	Change the translation value.
		\param	CSIBCVector3D&	New translation value
		\return	None
	*/
	SI_Void SetPivotPosition(CSIBCVector3D &in_rValue);


CSIBCVector3D GetPivotRotation();
SI_Void SetPivotRotation(CSIBCVector3D &in_rValue);
CSIBCVector3D GetPivotScale();
SI_Void SetPivotScale(CSIBCVector3D &in_rValue);
SI_Void SetPivotCompPosition(CSIBCVector3D &in_rValue);
CSIBCVector3D GetPivotCompPosition();
CSIBCVector3D GetPivotCompRotation();
SI_Void SetPivotCompRotation(CSIBCVector3D &in_rValue);
CSIBCVector3D GetPivotCompScale();
SI_Void SetPivotCompScale(CSIBCVector3D &in_rValue);
SI_Void SetNeutralPosePosition(CSIBCVector3D &in_rValue);
CSIBCVector3D GetNeutralPosePosition();
CSIBCVector3D GetNeutralPoseRotation();
SI_Void SetNeutralPoseRotation(CSIBCVector3D &in_rValue);
CSIBCVector3D GetNeutralPoseScale();
SI_Void SetNeutralPoseScale(CSIBCVector3D &in_rValue);
CSIBCVector3D GetNeutralPoseShear();
SI_Void SetNeutralPoseShear(CSIBCVector3D &in_rValue);
SI_Bool GetHierarchicalScaling();
SI_Void SetHierarchicalScaling(SI_Bool in_bValue);

CSLTemplate::ERotationOrder	GetRotationOrder();
const SI_Char*				GetRotationOrderAsString();

SI_Error					SetRotationOrder(CSLTemplate::ERotationOrder);
SI_Error					SetRotationOrderAsString(const SI_Char*);


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

	virtual CSLAnimatableType* ParameterFromName(SI_Char *in_szName);
    virtual CSLAnimatableType* ParameterFromType(EFCurveType in_Type, SI_Char *in_szParameterName);
    virtual ETemplateType Type();

	virtual SI_Error Synchronize();

	/*!	Return the name (string) of a specified sub parameter.
		\param	in_pParameter	The sub parameter
		\return	SI_Char*	Name of the sub parameter.
	*/
    virtual const SI_Char* GetParameterName(CSLAnimatableType* in_pParameter);

private:
	CSLStrEnumProxy<ERotationOrder, SI_XYZ> m_RotationOrder;

    CSLVector3DProxy m_Scale;
	CSLBoolProxy	 m_HierarchicalScaling;
    CSLVector3DProxy m_Rotation;
    CSLVector3DProxy m_Translation;
	CSLVector3DProxy m_Shear;
	
	CSLVector3DProxy m_PivotPosition;
	CSLVector3DProxy m_PivotRotation;
	CSLVector3DProxy m_PivotScale;

	CSLVector3DProxy m_PivotCompPosition;
	CSLVector3DProxy m_PivotCompRotation;
	CSLVector3DProxy m_PivotCompScale;

	CSLVector3DProxy m_NeutralPoseScale;
    CSLVector3DProxy m_NeutralPoseRotation;
    CSLVector3DProxy m_NeutralPosePosition;
	CSLVector3DProxy m_NeutralPoseShear;

	CSIBCMatrix4x4 m_TransformMatrix;
	CSIBCMatrix4x4 m_GlobalMatrix;

	void *m_pReserved;	// reserved for future extension
};

/** 
* CSLXSIBasePose class
* This class handles the XSI base pose matrix of a model. It provides functions to set
* all aspects of the XSI base pose like SRT, pivots, shearing, etc.
*/
class XSIEXPORT CSLXSIBasePose
    : public CSLTemplate
{
public:
    CSLXSIBasePose(CSLScene* in_pScene, CSLModel *in_pModel, CdotXSITemplate* in_pTemplate);
    virtual ~CSLXSIBasePose();

	/*!	Return the BasePoseation matrix.
		\return	CSIBCMatrix4x4	Base pose matrix.
	*/
	CSIBCMatrix4x4 GetMatrix();

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

	virtual CSLAnimatableType* ParameterFromName(SI_Char *in_szName);
    virtual CSLAnimatableType* ParameterFromType(EFCurveType in_Type, SI_Char *in_szParameterName);
    virtual ETemplateType Type();

	virtual SI_Error Synchronize();

	/*!	Return the name (string) of a specified sub parameter.
		\param	in_pParameter	The sub parameter
		\return	SI_Char*	Name of the sub parameter.
	*/
    virtual const SI_Char* GetParameterName(CSLAnimatableType* in_pParameter);

	/*!	Computes this CSLBasePose's local matrix.
	\return	Error code
	*/
	SI_Error ComputeMatrix();

private:
    CSLVector3DProxy m_Scale;
    CSLVector3DProxy m_Rotation;
    CSLVector3DProxy m_Translation;
	CSIBCMatrix4x4 m_BasePoseMatrix;

	void *m_pReserved;	// reserved for future extension
};

#endif

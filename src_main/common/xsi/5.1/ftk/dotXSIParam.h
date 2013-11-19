//***************************************************************************************
//
// File supervisor: Softimage 3D Games & 3D Bridge team
//
// (c) Copyright 1999-2002 Avid Technology, Inc. . All rights reserved.
//
// dotXSIParam.h | Main header file for CdotXSIParam implementation
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

//***************************************************************************************
// Defines
//***************************************************************************************
#ifndef __DOTXSIPARAM_H
#define __DOTXSIPARAM_H

//***************************************************************************************
// Includes
//***************************************************************************************
#include "SIBCNode.h"
#include "SIBCString.h"

//***************************************************************************************
// CdotXSIParam | holds a prameter value of all the supported types (SI_TinyVariant)
// public | CSIBCNode
//***************************************************************************************

//! Class representing a parameter within a CdotXSITemplate.
/*! A CdotXSIParam object is the interface used to access type, value, and tweaking information
	for a parameter within a CdotXSITemplate. The class provides accessor and mutator functions
	that use the SI_TinyInvariant structure.
*/
class XSIEXPORT CdotXSIParam : public CSIBCNode
{
public:

	enum 
	{
		UNFORMATTED = 1
	};

	/** Default constructor.
		Sets the parameter to a dummy type, and initializes its value.
	*/
	CdotXSIParam();

	/** Constructor.
		Sets the parameter name to the string given by \p i_sName and the parameter type
		given by \p i_nVariantType. The constructor then initializes the parameter value
		to its default value, based on the type given. See the defines in @link vptypes_group 
		Variant Parameter types@endlink	for information on which values this parameter can take on.

		\param	i_sName			Name of the new parameter.
		\param	i_nVariantType	Type of the new parameter. (See @link vptypes_group Variant Parameter types@endlink).
	*/
	CdotXSIParam( CSIBCString *i_sName, SI_Int i_nVariantType );

	/** Constructor.
		Sets the parameter name to the string given by \p i_sName and the parameter type
		given by \p i_nVariantType. The constructor then initializes the parameter value
		to its default value, based on the type given. See the defines in @link vptypes_group 
		Variant Parameter types@endlink	for information on which values this parameter can take on.

		\param	i_sName			Name of the new parameter.
		\param	i_nVariantType	Type of the new parameter. (See @link vptypes_group Variant Parameter types@endlink).
	*/
	CdotXSIParam( SI_Char *i_sName, SI_Int i_nVariantType );

	/** Constructor.
		Sets the parameter name to the string given by \p i_sName, the parameter type
		given by \p i_nVariantType, and the Tweak type given by \p i_nTweakType. The constructor 
		then initializes the parameter value to its default value, based on the type given. See the 
		defines in @link vptypes_group Variant Parameter types@endlink for information on which values 
		this parameter can take on. See the defines in @link vtwktypes_group Variant Parameter Tweak types@endlink
		for information on wihch values this parameter can take on.

		\param	i_sName			Name of the new parameter.
		\param	i_nVariantType	Type of the new parameter. (See @link vptypes_group Variant Parameter types@endlink).
		\param	i_nTweakType	Type of tweaking control. (See @link vtwktypes_group Variant Parameter Tweak types@endlink).
	*/
	CdotXSIParam( CSIBCString *i_sName, SI_Int i_nVariantType, SI_Int i_nTweakType );

	/** Constructor.
		Sets the parameter name to the string given by \p i_sName, the parameter type
		given by \p i_nVariantType, and the Tweak type given by \p i_nTweakType. The constructor 
		then initializes the parameter value to its default value, based on the type given. See the 
		defines in @link vptypes_group Variant Parameter types@endlink for information on which values 
		this parameter can take on. See the defines in @link vtwktypes_group Variant Parameter Tweak types@endlink
		for information on wihch values this parameter can take on.

		\param	i_sName			Name of the new parameter.
		\param	i_nVariantType	Type of the new parameter. (See @link vptypes_group Variant Parameter types@endlink).
		\param	i_nTweakType	Type of tweaking control. (See @link vtwktypes_group Variant Parameter Tweak types@endlink).
	*/
	CdotXSIParam( SI_Char *i_sName, SI_Int i_nVariantType, SI_Int i_nTweakType );

	/** Constructor.
		Sets the parameter name to the string given by \p i_sName. The parameter type, tweak type, and
		number of elements for the parameter is copied from the \p i_vValue parameter. The constructor
		the initializes the parameter value to its default value, based on the type in \p i_vValue. Note:
		the values from i_vValue are \b not copied from the \p i_vValue parameter.

		\param	i_sName			Name of the new parameter.
		\param	i_vValue		Parameter value to use the type, tweak type and number of elements information from.
	*/			
	CdotXSIParam( CSIBCString *i_sName, SI_TinyVariant i_vValue );

	/** Constructor.
		Sets the parameter name to the string given by \p i_sName. The parameter type, tweak type, and
		number of elements for the parameter is copied from the \p i_vValue parameter. The constructor
		the initializes the parameter value to its default value, based on the type in \p i_vValue. Note:
		the values from i_vValue are \b not copied from the \p i_vValue parameter.

		\param	i_sName			Name of the new parameter.
		\param	i_vValue		Parameter value to use the type, tweak type and number of elements information from.
	*/			
	CdotXSIParam( SI_Char *i_sName, SI_TinyVariant i_vValue );

	/** Destructor
		Clears the variant parameter. This destructor frees the arrays located within its parameters' value.
	*/
	virtual ~CdotXSIParam();

	/** Returns the value of this parameter in the \p o_vValue parameter
		\param	o_vValue	Pointer to recieve the value of this parameter.
	*/
    SI_Void GetValue( SI_TinyVariant *o_vValue );

	/** Sets the value of this parameter to the value of the parameter given by \p i_vValue.
		\param	i_vValue	Contains the new value for this parameter. (includes type, tweak type, 
				and number of elements).
	*/
	SI_Void SetValue( SI_TinyVariant i_vValue );

	/** Returns a pointer to the SI_TinyVariant structure that contains the type and value
		information for this parameter.
		\return	SI_TinyVariant*	Pointer to the structure that contains type and value information.
	*/
	SI_TinyVariant* GetVariantPtr(){ return &m_vValue; }								// Get Ptr to the stored variant.

    /** Initializes this parameter's value and type information with the information contained
		in the \p i_pValue parameter (includes type, tweak type, and number of elements).
		\param	i_pValue	Pointer to the structure containing type and value information to set.
	*/
    SI_Void InitializeVarValue( SI_TinyVariant *i_pValue );

	/** Initializes this parameter's value to the default value, based on the type of the parameter.
		In general, the default value is zero (or equivalent for pointer types).
	*/
	SI_Void InitializeDefaultVarValue();
	
private:
	SI_Void ClearVariant();										// Clears the content of the variant

	SI_TinyVariant  m_vValue;
};

//***************************************************************************************
// Global Functions
//***************************************************************************************
XSIEXPORT CdotXSIParam* CreatedotXSIParam();								// Allocate a CdotXSIParam
XSIEXPORT void			AllocateLocalXSIArray(SI_TinyVariant*, SI_Long);	// Allocate a Val array in the biven TinyVariant

#endif // __DOTXSIPARAM_H

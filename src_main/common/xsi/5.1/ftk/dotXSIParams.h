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

//***************************************************************************************
// Defines
//***************************************************************************************
#ifndef __DOTXSIPARAMS_H
#define __DOTXSIPARAMS_H

//***************************************************************************************
// Includes
//***************************************************************************************
#include "dotXSIParam.h"
#include "SIBCArray.h"
#include "SIBCString.h"


/*! container for an array of CdotXSIParam elements
*/
class XSIEXPORT CdotXSIParams  
{
public:

	/*! Constructor
	*/
	CdotXSIParams();

	/*! Destructor
	*/
	virtual ~CdotXSIParams();

	/*! Get the number of elements
	* \return SI_Int the number of CdotXSIParam in this collection
	*/
	SI_Int GetCount();

	/*! Add an element to this collection
	* \param i_Param the CdotXSIParam to add to this collection
	* \param i_nPosition the position to add this element at
	* \return SI_Void
	*/
	SI_Void Add( CdotXSIParam *i_Param, SI_Int i_nPosition );

	/*! Get an item
	* \param i_nPosition position to get the item at
	* \param o_Param a pointer to a CdotXSIParam* to store a reference to the item
	* \return SI_Void
	*/
	SI_Void Item( SI_Int i_nPosition, CdotXSIParam **o_Param );

	/*! Get an item
	* \param i_nPosition position to get the item at
	* \return CdotXSIParam* a pointer to the requested item
	*/
    CdotXSIParam *Item( SI_Int i_nPosition );

	/*! Remove the specified element
	* \param i_nPosition the position to remove the item at
	* \return SI_Void
	*/
    SI_Void Remove( SI_Int i_nPosition );

	/*! Clear the entire collection of all elements
	* \return SI_Void
	*/
    SI_Void Clear();

	/*! Find an element by name
	* \param i_sParamName the name of the parameter to find
	* \param o_Param a pointer to a CdotXSIParam* to store a reference to the item
	* \return SI_Bool whether the element has been found or not
	*/
	SI_Bool Find( CSIBCString *i_sParamName, CdotXSIParam **o_pParam );

private:
	SI_Int                          m_nCount;
    CSIBCArray< CdotXSIParam * >    m_Params;
};

//***************************************************************************************
// Global Functions
//***************************************************************************************

/*! \addtogroup glbfunc_param Global dotXSI parameter collections functions
*  @{
*/

/*! Create a collection of CdotXSIParam
* \return CdotXSIParams* the newly created collection
*/
XSIEXPORT CdotXSIParams* CreatedotXSIParams();

/* @} */

#endif // __DOTXSIPARAMS_H

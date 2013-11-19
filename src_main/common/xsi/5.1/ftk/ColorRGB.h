#ifndef __COLORRGB_H__
#define __COLORRGB_H__

#include "SL_Float.h"	// CSLFloatProxy
#include <SIBCVector.h>	// CSIBCVector3D
#include <SIBCColor.h>	// CSIBCColorf

//! CSLColorRGBProxy class
/** 
* Class that allows modification of a RGB parameter in a template 
*/
class XSIEXPORT CSLColorRGBProxy
{
public:

	/*! Constructor
		\param in_pTemplate Template that contains the RGB values
		\param in_lIndexR Index in the template for the red value
		\param in_lIndexG Index in the template for the green value
		\param in_lIndexB Index in the template for the blue value
	*/
	CSLColorRGBProxy
	( 
		CdotXSITemplate *in_pTemplate,
		SI_Long in_lIndexR,
		SI_Long in_lIndexG,
		SI_Long in_lIndexB
	);

	/*! Sets the RGB value
		\param in_rColor	The new color
		\return CSLColorRGBAProxy&	Reference to this color
	*/
    CSLColorRGBProxy& operator =(const CSIBCColorf &in_rColor);

	/*! Conversion operator. Converts the color in this object to a CSIBCColorf object
	*/
    operator CSIBCColorf();

	/*! Gets the red value
		\return CSLFloatProxy Parameter containing the red value of this color
	*/
	CSLFloatProxy& R();

	/*! Gets the green value
		\return CSLFloatProxy Parameter containing the green value of this color
	*/
	CSLFloatProxy& G();

	/*! Gets the blue value
		\return CSLFloatProxy Parameter containing the blue value of this color
	*/
	CSLFloatProxy& B();

protected:
	CSLFloatProxy m_fR;
	CSLFloatProxy m_fG;
	CSLFloatProxy m_fB;
};

#endif //__COLORRGB_H__

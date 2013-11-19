#ifndef __COLORRGBA_H__
#define __COLORRGBA_H__

#include "ColorRGB.h"

//! CSLColorRGBAProxy class
/** 
* Class that allows modification of a RGBA parameter in a template 
*/
class CSLColorRGBAProxy : public CSLColorRGBProxy
{
public:
	//! Constructor
	/**
		\param in_pTemplate Template that contains the RGB values
		\param in_lIndexR Index in the template for the red value
		\param in_lIndexG Index in the template for the green value
		\param in_lIndexB Index in the template for the blue value
		\param in_lIndexA Index in the template for the alpha value
	*/
	CSLColorRGBAProxy
	( 
		CdotXSITemplate *in_pTemplate,
		SI_Long in_lIndexR,
		SI_Long in_lIndexG,
		SI_Long in_lIndexB,
		SI_Long in_lIndexA
	);

	/*! Sets the RGBA value
		\param in_rColor	The new color
		\return CSLColorRGBAProxy&	Reference to this color
	*/
    CSLColorRGBAProxy& operator =(const CSIBCColorf &in_rColor);

	/*! Conversion operator. Converts the color in this object to a CSIBCColorf object.		
	*/
    operator CSIBCColorf();

	/*! Gets the alpha value
		\return CSLFloatProxy Parameter containing the alpha value of this color
	*/
	CSLFloatProxy& A();

private:
	CSLFloatProxy m_fA;
};

#endif //__COLORRGBA_H__

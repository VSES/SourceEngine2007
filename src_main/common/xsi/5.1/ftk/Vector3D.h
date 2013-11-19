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

#ifndef __VECTOR3D_H__
#define __VECTOR3D_H__

#include "SL_Float.h"	// CSLFloatProxy
#include <SIBCVector.h>	// CSIBCVector3D

/** 
* Class CSLVector3DProxy
*
* Class that allows modification of a 3D vector parameter in a template 
*
*/
class CSLVector3DProxy
{
public:
	//! Constructor
	/**
		\param in_pTemplate Template that contains the vector values
		\param in_lIndexX Index in the template for the X value
		\param in_lIndexY Index in the template for the Y value
		\param in_lIndexZ Index in the template for the Z value
	*/
	CSLVector3DProxy
	( 
		CdotXSITemplate *in_pTemplate,
		SI_Long in_lIndexX,
		SI_Long in_lIndexY,
		SI_Long in_lIndexZ
	);

	//! Set the 3D vector
    CSLVector3DProxy& operator =(const CSIBCVector3D &in_rVector);

	//! Get the 3D vector
    operator CSIBCVector3D();

	//! Get the X value
	CSLFloatProxy& X();

	//! Get the Y value
	CSLFloatProxy& Y();

	//! Get the Z value
	CSLFloatProxy& Z();

protected:
	CSLFloatProxy m_fX;
	CSLFloatProxy m_fY;
	CSLFloatProxy m_fZ;
};

#endif //__VECTOR3D_H__

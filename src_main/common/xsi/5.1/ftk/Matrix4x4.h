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
#ifndef __MATRIX4X4_H__
#define __MATRIX4X4_H__

#include "SL_Float.h"	// CSLFloatProxy
#include <SIBCMatrix.h>	// CSIBCMatrix4x4

/** 
* CSLMatrix4x4Proxy class
* Class that allows modification of a 4x4 matrix parameter in a template 
*/
class XSIEXPORT CSLMatrix4x4Proxy
{
public:
	CSLMatrix4x4Proxy
	( 
		CdotXSITemplate *in_pTemplate,
		SI_Long in_lIndex11,
		SI_Long in_lIndex12,
		SI_Long in_lIndex13,
		SI_Long in_lIndex14,
		SI_Long in_lIndex21,
		SI_Long in_lIndex22,
		SI_Long in_lIndex23,
		SI_Long in_lIndex24,
		SI_Long in_lIndex31,
		SI_Long in_lIndex32,
		SI_Long in_lIndex33,
		SI_Long in_lIndex34,
		SI_Long in_lIndex41,
		SI_Long in_lIndex42,
		SI_Long in_lIndex43,
		SI_Long in_lIndex44
	);

	//! Set the matrix
    CSLMatrix4x4Proxy& operator =(const CSIBCMatrix4x4 &in_rMatrix);

	//! Get the matrix
    operator CSIBCMatrix4x4();

	//! Get the (row,column) element
	CSLFloatProxy& Element(SI_Long in_lColumn, SI_Long in_lRow);

protected:
	CSLFloatProxy m_f11;
	CSLFloatProxy m_f12;
	CSLFloatProxy m_f13;
	CSLFloatProxy m_f14;
	CSLFloatProxy m_f21;
	CSLFloatProxy m_f22;
	CSLFloatProxy m_f23;
	CSLFloatProxy m_f24;
	CSLFloatProxy m_f31;
	CSLFloatProxy m_f32;
	CSLFloatProxy m_f33;
	CSLFloatProxy m_f34;
	CSLFloatProxy m_f41;
	CSLFloatProxy m_f42;
	CSLFloatProxy m_f43;
	CSLFloatProxy m_f44;
};

#endif //__MATRIX4X4_H__

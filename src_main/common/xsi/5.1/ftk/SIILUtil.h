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
#ifndef __SIILUTIL_H__
#define __SIILUTIL_H__


//***************************************************************************************
// Includes
//***************************************************************************************
#include <SIBCUtil.h>


//***************************************************************************************
// Defines
//***************************************************************************************
#ifdef _WIN32

#ifndef LITTLE_ENDIAN
#define SWAP_SHORT(n) (((((SI_UShort)(n))&0xff00)>>8)|((((SI_UShort)(n))&0x00ff)<<8))
#define SWAP_LONG(n) ((((SI_ULong)(n))>>24)|(((SI_ULong)(n))>>8&0xff00)|(((SI_ULong)(n))<<8&0xff0000)|(((SI_ULong)(n))<<24))
#define SWAP_FLOAT(n) SI_ImageSwapFloat(n)
#define INV_SWAP_SHORT(n) (n)
#define INV_SWAP_LONG(n) (n)
#define INV_SWAP_FLOAT(n) (n)
#else
#define SWAP_SHORT(n) (n)
#define SWAP_LONG(n) (n)
#define SWAP_FLOAT(n) (n)
#define INV_SWAP_SHORT(n) (((((SI_UShort)(n))&0xff00)>>8)|((((SI_UShort)(n))&0x00ff)<<8))
#define INV_SWAP_LONG(n) ((((SI_ULong)(n))>>24)|(((SI_ULong)(n))>>8&0xff00)|(((SI_ULong)(n))<<8&0xff0000)|(((SI_ULong)(n))<<24))
#define INV_SWAP_FLOAT(n) SI_ImageSwapFloat(n)
#endif

#else

#ifndef BYTE_ORDER

#ifndef LITTLE_ENDIAN
#define SWAP_SHORT(n) (n)
#define SWAP_LONG(n) (n)
#define SWAP_FLOAT(n) (n)
#define INV_SWAP_SHORT(n) (((((SI_UShort)(n))&0xff00)>>8)|((((SI_UShort)(n))&0x00ff)<<8))
#define INV_SWAP_LONG(n) ((((SI_ULong)(n))>>24)|(((SI_ULong)(n))>>8&0xff00)|(((SI_ULong)(n))<<8&0xff0000)|(((SI_ULong)(n))<<24))
#define INV_SWAP_FLOAT(n) SI_ImageSwapFloat(n)
#else
#define SWAP_SHORT(n) (((((SI_UShort)(n))&0xff00)>>8)|((((SI_UShort)(n))&0x00ff)<<8))
#define SWAP_LONG(n) ((((SI_ULong)(n))>>24)|(((SI_ULong)(n))>>8&0xff00)|(((SI_ULong)(n))<<8&0xff0000)|(((SI_ULong)(n))<<24))
#define SWAP_FLOAT(n) SI_ImageSwapFloat(n)
#define INV_SWAP_SHORT(n) (n)
#define INV_SWAP_LONG(n) (n)
#define INV_SWAP_FLOAT(n) (n)
#endif

#else

#if ( BYTE_ORDER == LITTLE_ENDIAN )
#define SWAP_SHORT(n) (n)
#define SWAP_LONG(n) (n)
#define SWAP_FLOAT(n) (n)
#define INV_SWAP_SHORT(n) (((((SI_UShort)(n))&0xff00)>>8)|((((SI_UShort)(n))&0x00ff)<<8))
#define INV_SWAP_LONG(n) ((((SI_ULong)(n))>>24)|(((SI_ULong)(n))>>8&0xff00)|(((SI_ULong)(n))<<8&0xff0000)|(((SI_ULong)(n))<<24))
#define INV_SWAP_FLOAT(n) SI_ImageSwapFloat(n)
#else
#define SWAP_SHORT(n) (((((SI_UShort)(n))&0xff00)>>8)|((((SI_UShort)(n))&0x00ff)<<8))
#define SWAP_LONG(n) ((((SI_ULong)(n))>>24)|(((SI_ULong)(n))>>8&0xff00)|(((SI_ULong)(n))<<8&0xff0000)|(((SI_ULong)(n))<<24))
#define SWAP_FLOAT(n) SI_ImageSwapFloat(n)
#define INV_SWAP_SHORT(n) (n)
#define INV_SWAP_LONG(n) (n)
#define INV_SWAP_FLOAT(n) (n)
#endif

#endif

#endif


//***************************************************************************************
// Global Function
//***************************************************************************************
/*! byte swaps a float value
* \param value float value to byte swap
* \return SI_Float
*/
extern SI_Float SI_ImageSwapFloat(SI_Float value);


#endif // __SIILUTIL_H__

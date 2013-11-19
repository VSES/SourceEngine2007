//***************************************************************************************
//
// File supervisor: Games S.W.A.T. Team
//
// (c) Copyright 2001 Avid Technology, Inc. . All rights reserved.
//
// $Archive: /Games/XSI/v1_0/src/Core/Sibc/QuatTypes.h $
// $Revision: 2 $ $Date: 10/19/99 2:26p $
// Checkin by: $Author: Rcleven $
//
// @doc
//
// @module      QuatTypes.h | 
//***************************************************************************************

/**** QuatTypes.h - Basic type declarations ****/
#ifndef _H_QuatTypes
#define _H_QuatTypes
/*** Definitions ***/
typedef struct {float x, y, z, w;} Quat; /* Quaternion */
enum QuatPart {X, Y, Z, W};
typedef float HMatrix[4][4]; /* Right-handed, for column vectors */
typedef Quat EulerAngles;    /* (x,y,z)=ang 1,2,3, w=order code  */
#endif


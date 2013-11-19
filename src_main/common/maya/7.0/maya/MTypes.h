#ifndef _MTypes
#define _MTypes
//
//-
// ==========================================================================
// Copyright (C) 1995 - 2005 Alias Systems Corp. and/or its licensors.  All 
// rights reserved.
// 
// The coded instructions, statements, computer programs, and/or related 
// material (collectively the "Data") in these files contain unpublished 
// information proprietary to Alias Systems Corp. ("Alias") and/or its 
// licensors, which is protected by Canadian and US federal copyright law and 
// by international treaties.
// 
// The Data may not be disclosed or distributed to third parties or be copied 
// or duplicated, in whole or in part, without the prior written consent of 
// Alias.
// 
// THE DATA IS PROVIDED "AS IS". ALIAS HEREBY DISCLAIMS ALL WARRANTIES RELATING 
// TO THE DATA, INCLUDING, WITHOUT LIMITATION, ANY AND ALL EXPRESS OR IMPLIED 
// WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY AND/OR FITNESS FOR A 
// PARTICULAR PURPOSE. IN NO EVENT SHALL ALIAS BE LIABLE FOR ANY DAMAGES 
// WHATSOEVER, WHETHER DIRECT, INDIRECT, SPECIAL, OR PUNITIVE, WHETHER IN AN 
// ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, OR IN EQUITY, 
// ARISING OUT OF ACCESS TO, USE OF, OR RELIANCE UPON THE DATA.
// ==========================================================================
//+
// *****************************************************************************
//
// FILE:    MTypes.h
//
// *****************************************************************************
//
// DESCRIPTION (MTypes)
//
//		This file contains the definitions for NULL, numeric array types, type 
//		bool, and constants true and false
//
// *****************************************************************************

#if ( defined (OSMac_) && !defined (MAC_PLUGIN) )
#include "HOS.h"
#endif
#if defined __cplusplus


// *****************************************************************************

class MStatus;
class MSyntax;

// *****************************************************************************

// DECLARATIONS

// *****************************************************************************

// Define a CPP variable that reflects the current Maya API version.
// This variable is designed to be an integer so that one can do
// CPP arithmetic and comparisons on it.  The digits in the number
// are derived by taking the Maya version number and deleting the '.'
// characters. Dot releases do not update this value so a release with
// version #.#.# would havethe same number as #.#. Thus, for example
//   Maya 1.0.1  would be 100
//   Maya 1.5    would be 150
//   Maya 2.0    would be 200
//   Maya 2.5    would be 250
//   Maya 3.0    would be 300
//   Maya 4.0    would be 400
//   Maya 4.0.1  would be 400
//   Maya 4.0.2  would be 400
//   Maya 4.0.3  would be 400
//   Maya 4.5    would be 450
//   Maya 5.0    would be 500
//   Maya 6.0    would be 600
//   Maya 6.5    would be 650
//   Maya 7.0    would be 700
//   etc.
//
// since this variable did not exist in the Maya 1.0 API, it will
// default to zero in that release.  Thus a construct such as
//
//    #if MAYA_API_VERSION > 100
//
// will be true in all post 1.0 versions of the API.
// 
#define MAYA_API_VERSION 700

#ifdef _WIN32

#ifndef PLUGIN_EXPORT
#define PLUGIN_EXPORT _declspec( dllexport ) 
#endif // PLUGIN_EXPORT

#ifndef FND_EXPORT
#define FND_EXPORT _declspec( dllimport ) 
#endif // FND_EXPORT

#ifndef IMAGE_EXPORT
#define IMAGE_EXPORT _declspec( dllimport )
#endif // IMAGE_EXPORT

#ifndef OPENMAYA_EXPORT
#define OPENMAYA_EXPORT _declspec( dllimport )
#endif // OPENMAYA_EXPORT

#ifndef OPENMAYAUI_EXPORT
#define OPENMAYAUI_EXPORT _declspec( dllimport )
#endif // OPENMAYAUI_EXPORT

#ifndef OPENMAYAANIM_EXPORT
#define OPENMAYAANIM_EXPORT _declspec( dllimport )
#endif // OPENMAYAANIM_EXPORT

#ifndef OPENMAYAFX_EXPORT
#define OPENMAYAFX_EXPORT _declspec( dllimport )
#endif // OPENMAYAFX_EXPORT

#ifndef OPENMAYARENDER_EXPORT
#define OPENMAYARENDER_EXPORT _declspec( dllimport )
#endif // OPENMAYARENDER_EXPORT

#if defined(NT_PLUGIN) || defined(NT_APP)
#ifndef _BOOL
#define _BOOL
#endif

#include "windows.h"

#ifndef NT_APP
extern HINSTANCE MhInstPlugin;
#endif


#define uint UINT
#define ulong ULONG
#define strcasecmp stricmp

#define	M_PI		3.14159265358979323846	/* pi */

#define		fsqrt(x)	(float)sqrt((double)(x))
#define		fsin(x)		(float)sin((double)(x))
#define		fcos(x)		(float)cos((double)(x))
#define		ftan(x)		(float)tan((double)(x))
#define		ffloor(x)	(float)floor((double)(x))
#define		fatan2(y,x)	(float)atan2((double)(y),(double)(x))
#define		fatan(x)	(float)atan((double)(x))
#define		flog(x)		(float)log((double)(x))
#define		fexp(x)		(float)exp((double)(x))
#define		fexpm1(x)	(float)(expm1(x))
#define		isnan		_isnan
#define		isnanf		_isnan
#define		ftrunc(d)	((float)trunc(d))
#define		facos(d)	((float)acos((double)d))
#endif // NT_PLUGIN

#else // !_WIN32

#if defined (OSMac_) && defined (__MWERKS__) && !defined (M_PI)
#define	M_PI		3.14159265358979323846	/* pi */
#endif

#ifndef PLUGIN_EXPORT
#define PLUGIN_EXPORT
#endif
#ifndef FND_EXPORT 
#define FND_EXPORT 
#endif
#ifndef IMAGE_EXPORT
#define IMAGE_EXPORT
#endif
#ifndef OPENMAYA_EXPORT
#define OPENMAYA_EXPORT 
#endif
#ifndef OPENMAYAUI_EXPORT
#define OPENMAYAUI_EXPORT
#endif
#ifndef OPENMAYAANIM_EXPORT
#define OPENMAYAANIM_EXPORT
#endif
#ifndef OPENMAYAFX_EXPORT
#define OPENMAYAFX_EXPORT
#endif
#ifndef OPENMAYARENDER_EXPORT
#define OPENMAYARENDER_EXPORT
#endif

#if !defined (MAC_PLUGIN)
#include <sys/types.h>
#endif // MAC_PLUGIN
#include <sys/stat.h>

#endif // _WIN32

#if (defined (NT_PLUGIN) || defined(NT_APP) || (defined(MAC_PLUGIN) && !defined(__MACH__)))
#ifdef __cplusplus
extern "C" {
#endif

//from /usr/include/math.h
FND_EXPORT extern double	drand48(void);
FND_EXPORT extern double	erand48(unsigned short [3]);
FND_EXPORT extern long		lrand48(void);
FND_EXPORT extern long		nrand48(unsigned short [3]);
FND_EXPORT extern long		mrand48(void);
FND_EXPORT extern long		jrand48(unsigned short [3]);
FND_EXPORT extern void	srand48(long);
FND_EXPORT extern unsigned short * seed48(unsigned short int [3]);
FND_EXPORT extern void	lcong48(unsigned short int [7]);
FND_EXPORT extern long	 random(void);
FND_EXPORT extern int srandom( unsigned x );

#ifdef __cplusplus
}
#endif
#endif // NT_PLUGIN || MAC_PLUGIN

#ifndef NULL
#	define NULL	0L
#endif 

#ifndef _BOOL
typedef unsigned char bool;
#if !defined(true) && !defined(false)
#ifndef TRUE_AND_FALSE_DEFINED
#define TRUE_AND_FALSE_DEFINED
	enum {false,true};
#endif // TRUE_AND_FALSE_DEFINED
#endif // true and false

#endif // _BOOL

typedef signed short short2[2];
typedef signed short short3[3];
typedef signed int   long2[2];
typedef signed int   long3[3];
typedef signed int   int2[2];
typedef signed int   int3[3];
typedef float        float2[2];
typedef float        float3[3];
typedef double       double2[2];
typedef double       double3[3];
typedef double       double4[4];

#if defined(_WIN32)
	typedef unsigned __int64 MUint64;
#elif defined(sgi) || defined(linux)
#if defined(Bits64_)
	typedef unsigned long MUint64;
#else
	typedef unsigned long long MUint64;
#endif
#elif defined(OSMac_)
	typedef unsigned long long MUint64;
#else
	Unknown OS: need to provide an implementation here
#endif

#if defined(_WIN32)
#	include <basetsd.h>
	typedef INT_PTR		MIntPtrSz;
	typedef UINT_PTR	MUintPtrSz;
#elif defined(__unix) || defined(__MACH__)
#	include <inttypes.h>
// On Linux stdarg.h is not included in inttypes.h, so do it here instead
#if defined(linux)
#include <stdarg.h>
#endif
	typedef intptr_t		MIntPtrSz;
	typedef uintptr_t		MUintPtrSz;
#else
#	error	"__FILE__:__LINE__ No pointer-as-int types defined for this O/S"
#endif

typedef void *   (*MCreatorFunction)(); 
typedef MSyntax	 (*MCreateSyntaxFunction)(); 
typedef MStatus  (*MInitializeFunction)();

/// Space transformation identifiers
/**
 MSpace encompasses all of the types of transformation possible.  The MSpace
 identifiers are used to determine the space in which the user is applying or
 querying transformation or component (i.e. vertex positions) data of a Maya
 object.

 Note that not all the MSpace types can be passed into all methods which take
 an MSpace as a parameter.  The MSpace enumerated type can be split into two
 parts, types which can be passed into MFnTransform and MTransformationMatrix
 classes (kTransform, kPreTransform and kPostTransform) and types which can
 be passed into shape classes such as MFnMesh, MFnNurbsSurface and MFnCamera
 (kWorld and kObject).

 The following is a description of each of the MSpace types.

 {\bf kInvalid}

  There are currently no methods in the API that return a result of type MSpace.
  This may be used for user defined methods that return a result of type MSpace
  to signify an invalid result.

 {\bf kTransform}

 The data applied or queried using the kTransform type represents a transform's 
 local (or relative) coordinates system.  This type is valid for methods of 
 MFnTransform and MTransformationMatrix classes.

 {\bf kPreTransform}

 The data applied or queried using the kPreTransform type represents 
 pre-transformed matrix data.  Given a matrix which has had other matrix 
 operations applied to it, this type is used to obtain a matrix that does 
 not taken into consideration any other matrix operations.  This type is 
 valid for methods of the MTransformationMatrix classes.

 {\bf kPostTransform}

 The data applied or queried using the kPostTransform type represents 
 post-transformed matrix data.  Given a matrix which has had other matrix 
 operations applied to it, this type is used to obtain a matrix that has 
 taken into consideration all other matrix operations.  This type is valid 
 for methods of the MTransformationMatrix classes.

 {\bf kWorld}

 The data applied or queried using the kWorld type represents the data for 
 the objects world coordinates system.  The results of this type will take 
 into consideration all the transforms in the objects hierarchy.  This type 
 can be used with shape classes such as MFnMesh, MFnNurbsSurface and MFnCamera 
 and the MFnTransform class.  Note that to get the proper world space 
 transformation data with MSpace::kWorld you {\bf MUST} use the class
 constructor which initializes the function set with an MDagPath object.  

 {\bf kObject}

 The data applied or queried using the kObject type represents the data for 
 the objects local coordinates system.  The results of this type will be in 
 the local space of the object (object space).  This enum can only be used 
 with shape classes such as MFnMesh, MFnNurbsSurface and MFnCamera.

  {\bf kLast}

 This simply signifies the end of the MSpace enumerated types.  All values that precede
 this value represent a coordinate system (with the exception of kInvalid which 
 represents an invalid result.)

  {\bf Comparative Description}

  There are four spaces in which to apply a transformation.

	World Space - This is the space that an object is in after it has had all of
    its transformations applied to it. A world space transformation is the
    the very last transformation that would be applied to the object.

	Object Space - This is the space in which an object is defined. An object
    space transformation would be the first transformation that would be applied
    to an object. This is the same as pre-transform space.

	Post Transform Space - This is the space in which the object lives after having
    the transformations of its transform node applied. For example, when a
    primitive object is created in Maya, there is a shape node which defined the
    geometry and a transform node which positions/orients the geometry. The space
    that the geometry is in is object space. After applying the transformations of
    the transform node to the geometry, the transformed geometry is in post transform
    space.

	Transform Space - This space depends on the type of transformation being applied.
    Transform nodes define a fixed sequence of affine transformations. Basically,
    there is scale followed by rotation and finally translation. There are four
    spaces here: 

		1) The space before applying the scale. 
		2) The space after the scale but before the rotation. 
		3) The space between the rotation and translation.
		4) The space after the translation. 
		
		  Space 1) is pre-transform space. 
		  Space 4) is post-transform space. 
		  Spaces 2) and 3) define transform spaces. 
		  
	If applying a transform space scale, this transform will be applied after the 
	current scale but before the rotation (space 2). If applying a transform space
	rotation, this transform will be applied after the current rotation but before 
	the translation.  Applying a translation in transform space, the translation 
	will be applied after the current translation.


*/
class MSpace {
public:
	///
	enum Space {
		///
		kInvalid = 0,
		///	Transform matrix (relative) space
	 	kTransform = 1,
		///	Pre-transform matrix (geometry)
 		kPreTransform = 2,
		///	Post-transform matrix (world) space
 		kPostTransform = 3,
		///	transform in world space
 		kWorld = 4,
		///	Same as pre-transform space
 		kObject = kPreTransform,
		///
		kLast = 5
	};

};

// stat does not work reliably on NT for directories.  Trailing '\'s cause an 
// error on NT4.0 and trailing '/'s and '\'s cause an error on Windows 2000.
// Use the following STAT macro to get consistent behavior on NT and UNIX.
//
#ifndef STAT
#ifdef _WIN32
#   define STAT statNT
	extern FND_EXPORT int statNT (const char *path, struct stat *buffer );
#elif defined(OSMac_CFM_)
#	define STAT statMac
	extern "C" int statMac (const char *path, struct stat *buffer);
#elif defined(OSMac_MachO_)
	#ifdef __cplusplus
		#define STAT statMachO
	
		__inline int	statMachO (const char *path, struct stat *buffer)
		{
			return (stat(path, buffer));
		}
	#else	/* The inline definition cause multiple definition for C files. */
		#define STAT(path,buffer)		stat((path),(buffer))
	#endif
#else
#   define STAT statSGI

__inline int statSGI (const char *path, struct stat *buffer )
{
	return stat(path, buffer);
}
#endif
#endif
// *****************************************************************************
#endif /* __cplusplus */
#endif /* _MTypes */

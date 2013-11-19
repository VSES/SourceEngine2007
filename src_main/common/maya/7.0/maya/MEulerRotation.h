#ifndef _MEulerRotation
#define _MEulerRotation
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
//
// CLASS:    MEulerRotation
//
// *****************************************************************************
//
// CLASS DESCRIPTION (MEulerRotation)
//
// This class provides methods for working with euler angle rotations.
// Euler angles are described by rotations in radians around the x, y,
// and z axes, and the order in which those rotations occur.
//
// *****************************************************************************

#if defined __cplusplus

// *****************************************************************************

// INCLUDED HEADER FILES



#include <maya/MTypes.h>
#include <maya/MTransformationMatrix.h>
#include <maya/MIOStreamFwd.h>

// *****************************************************************************

// DECLARATIONS

class MMatrix;
class MQuaternion;
#define kEulerRotationEpsilon 1.0e-10

// *****************************************************************************

// CLASS DECLARATION (MEulerRotation)

/// Euler Rotation Math
/**
This class provides methods for working with euler rotations.
*/
#ifdef _WIN32
#pragma warning(disable: 4522)
#endif // _WIN32

class OPENMAYA_EXPORT MEulerRotation  
{
public:

	///
	enum RotationOrder {
		///
		kXYZ, 
		///
		kYZX, 
		///
		kZXY, 
		///
		kXZY, 
		///
		kYXZ, 
		///
		kZYX
	};

	///
						MEulerRotation();
	///
						MEulerRotation(const MEulerRotation &src);
	///
						MEulerRotation(const MVector &v, 
									   RotationOrder ord = kXYZ);
	///
						MEulerRotation(double xx, double yy, double zz, 
									   RotationOrder ord = kXYZ);
	///
						~MEulerRotation();
	///
	MEulerRotation &	operator=(const MEulerRotation &src);
	///
 	MEulerRotation &	operator=(const MQuaternion &q);
	///
 	MEulerRotation &	operator=(const MMatrix &m);
	///
 	MEulerRotation &	operator=(const MVector &v);
	///
	MEulerRotation &	setValue(const MVector &v, 
								 RotationOrder ord = kXYZ);
	///
	MEulerRotation &	setValue(double xx, double yy, double zz,
								 RotationOrder ord = kXYZ);
	///
	MQuaternion			asQuaternion() const;
	///
	MMatrix				asMatrix() const;
	///
 	MVector				asVector() const;
	///
	double &			operator[](unsigned i);
	///
	double				operator[](unsigned i) const;
	///
 	MEulerRotation   	operator+(const MEulerRotation &other) const;
	///
	MEulerRotation &	operator+=(const MEulerRotation &other);
	///
 	MEulerRotation   	operator-(const MEulerRotation &other) const;
	///
	MEulerRotation &	operator-=(const MEulerRotation &other);
	///
 	MEulerRotation   	operator-() const;
	///
	MEulerRotation		operator*(const MEulerRotation &other) const;
	///
	MEulerRotation		operator*(const MQuaternion &other) const;
	///
	MEulerRotation		operator*(double other) const;
	///
 	friend OPENMAYA_EXPORT MEulerRotation operator*(double scale, 
													const MEulerRotation 
													    &other);
	///
	MEulerRotation &	operator*=(const MEulerRotation &other);
	///
	MEulerRotation &	operator*=(const MQuaternion &other);
	///
	MEulerRotation &	operator*=(double other);
	///
 	bool           		operator==(const MEulerRotation &other) const;
	///
 	bool          		operator!=(const MEulerRotation &other) const;
	///
 	bool          		isEquivalent(const MEulerRotation &other, 
									 double tolerance = kEulerRotationEpsilon) 
		                             const;
	///
 	bool          		isZero(double tolerance = kEulerRotationEpsilon) const;
	///
	MEulerRotation &	incrementalRotateBy(const MVector &axis,
											double angle);
	///
	MEulerRotation		inverse() const;
	///
	MEulerRotation &	invertIt();
	///
	MEulerRotation		reorder(RotationOrder ord) const;
	///
	MEulerRotation &	reorderIt(RotationOrder ord);
	///
	static MEulerRotation bound(const MEulerRotation &src);
	///
	MEulerRotation		bound() const;
	///
	MEulerRotation &	boundIt(const MEulerRotation &src);
	///
	MEulerRotation &	boundIt();
	///
	static MEulerRotation alternateSolution(const MEulerRotation &src);
	///
	MEulerRotation		alternateSolution() const;
	///
	MEulerRotation &	setToAlternateSolution(const MEulerRotation &src);
	///
	MEulerRotation &	setToAlternateSolution();
	///
	static MEulerRotation closestSolution(const MEulerRotation &src,
										  const MEulerRotation &dst);
	///
	MEulerRotation		closestSolution(const MEulerRotation &dst) const;
	///
	MEulerRotation &	setToClosestSolution(const MEulerRotation &src,
											 const MEulerRotation &dst);
	///
	MEulerRotation &	setToClosestSolution(const MEulerRotation &dst);
	///
	static MEulerRotation closestCut(const MEulerRotation &src,
									 const MEulerRotation &dst);
	///
	MEulerRotation		closestCut(const MEulerRotation &dst) const;
	///
	MEulerRotation &	setToClosestCut(const MEulerRotation &src,
										const MEulerRotation &dst);
	///
	MEulerRotation &	setToClosestCut(const MEulerRotation &dst);
	///
	static MEulerRotation decompose(const MMatrix &matrix, RotationOrder ord);
	///
	friend OPENMAYA_EXPORT IOS_REF(ostream) &operator<<(IOS_REF(ostream) &os, 
											   const MEulerRotation &rotation);
	/// The euler rotation identity.  The rotation order is XYZ.
	static const MEulerRotation identity;

	/// The x component of the euler rotation in radians
	double x;
	/// The y component of the euler rotation in radians
	double y;
	/// The z component of the euler rotation in radians
	double z;
	/// The rotation order of the euler rotation
	RotationOrder order;
#if defined(LINUX)
	char			_padding[4];		// Pad out sizeof class
#endif

protected:
	// No protected members

private:
	// No private members
};

#if defined(_WIN32)
#pragma warning(default: 4522)
#endif // _WIN32

// *****************************************************************************
#endif /* __cplusplus */
#endif /* _MEulerRotation */

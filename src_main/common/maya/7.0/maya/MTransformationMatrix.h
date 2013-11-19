#ifndef _MTransformationMatrix
#define _MTransformationMatrix
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
// CLASS:    MTransformationMatrix
//
// *****************************************************************************
//
// CLASS DESCRIPTION (MTransformationMatrix)
//
// An MTransformationMatrix allows the manipulation of the individual
// transformation components (eg scale, rotation, shear, etc) of a four by
// four transformation matrix.
//
// *****************************************************************************

#if defined __cplusplus

// *****************************************************************************

// INCLUDED HEADER FILES


#include <maya/MStatus.h>
#include <maya/MTypes.h>

// *****************************************************************************

// DECLARATIONS

class MMatrix;
class MVector;
class MPoint;
class MQuaternion;
class MEulerRotation;

#define MTransformationMatrix_kTol 1.0e-10

// *****************************************************************************

// CLASS DECLARATION (MTransformationMatrix)

/// Transformation matrix
/**
	Class for constructing transformation matrices
*/
#ifdef _WIN32
#pragma warning(disable: 4522)
#endif // _WIN32

class OPENMAYA_EXPORT MTransformationMatrix  
{
public:
    ///
	enum RotationOrder {
        ///
		kInvalid,
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
		kZYX,
        ///
		kLast
	};
    ///
	~MTransformationMatrix();
    ///
	MTransformationMatrix();
    ///
	MTransformationMatrix( const MTransformationMatrix& src );
    ///
	MTransformationMatrix( const MMatrix& src );
    ///
	MTransformationMatrix&   	operator= ( const MTransformationMatrix &src );
    ///
	MTransformationMatrix&   	operator= ( const MMatrix &src );
    ///
	MMatrix		asMatrix()					const;
    ///
 	MMatrix		asMatrix(double percent)	const;
    ///
 	MMatrix		asMatrixInverse()			const;
    ///
	MMatrix		asScaleMatrix()				const;
    ///
 	MMatrix		asRotateMatrix()			const;
    ///
 	MStatus		getScale( double[3], MSpace::Space ) const;
    ///
 	MStatus		setScale( const double[3], MSpace::Space );
    ///
 	MStatus		addScale( const double[3], MSpace::Space );
	///
	MQuaternion	rotation() const;
	///
	MTransformationMatrix &rotateTo( const MQuaternion &q );
	///
	MTransformationMatrix &rotateBy( const MQuaternion &q, 
									 MSpace::Space space,
									 MStatus *ReturnStatus = NULL);
	///
	MEulerRotation	eulerRotation() const;
	///
	MTransformationMatrix &rotateTo( const MEulerRotation &e );
	///
	MTransformationMatrix &rotateBy( const MEulerRotation &e, 
									 MSpace::Space space,
									 MStatus *ReturnStatus = NULL);
    ///
 	MStatus		getRotationQuaternion( double &x, double &y, double &z,
									   double &w ) const;
    ///
 	MStatus		setRotationQuaternion( double x, double y, double z,
									   double w );
    ///
 	MStatus		addRotationQuaternion( double x, double y, double z,
									   double w, MSpace::Space );
    ///
 	MStatus		getRotation( double[3],
							 RotationOrder& order ) const;
    ///
 	MStatus		setRotation( const double rotation[3],
							 RotationOrder order );
    ///
	MStatus		addRotation( const double rotation[3],
							 RotationOrder order, MSpace::Space );
    ///
	RotationOrder	rotationOrder( MStatus* ReturnStatus = NULL ) const;
    ///
	MStatus	    reorderRotation( RotationOrder& order );
	///
	MStatus     setToRotationAxis( const MVector &axis, double rotation );
	///
	MQuaternion rotationOrientation() const;
	///
	MTransformationMatrix &setRotationOrientation( const MQuaternion &q );
    ///
	MVector		getTranslation( MSpace::Space, MStatus* ReturnStatus=NULL ) const;
    ///
 	MStatus		setTranslation( const MVector& vector, MSpace::Space );
    ///
	MStatus		addTranslation( const MVector& vector, MSpace::Space );
    ///
	MStatus		setShear( const double[3], MSpace::Space );
    ///
	MStatus		getShear( double[3], MSpace::Space ) const;
    ///
	MStatus		addShear( const double[3], MSpace::Space );
    ///
	MPoint		scalePivot( MSpace::Space, MStatus* ReturnStatus = NULL ) const;
    ///
	MStatus		setScalePivot( const MPoint&, MSpace::Space, bool balance );
    ///
	MVector		scalePivotTranslation(MSpace::Space,
									  MStatus* ReturnStatus=NULL )const;
    ///
	MStatus		setScalePivotTranslation( const MVector& vector,
										  MSpace::Space );
    ///
	MPoint		rotatePivot( MSpace::Space, MStatus* ReturnStatus=NULL ) const;
    ///
	MStatus		setRotatePivot( const MPoint& point,
								MSpace::Space, bool balance );
    ///
	MVector		rotatePivotTranslation( MSpace::Space space,
					MStatus* ReturnStatus=NULL ) const;
    ///
	MStatus		setRotatePivotTranslation( const MVector& vector,
										   MSpace::Space space );
    ///
 	bool		isEquivalent ( const MTransformationMatrix &other,
					double tolerance = MTransformationMatrix_kTol ) const;
    ///
 	bool		operator == (const MTransformationMatrix & other ) const;
    ///
	bool		operator != ( const MTransformationMatrix & other ) const;

    /// OBSOLETE
 	MStatus		getRotation( double[3],
							 RotationOrder& order, MSpace::Space ) const;
    /// OBSOLETE
 	MStatus		setRotation( const double rotation[3],
							 RotationOrder order, MSpace::Space );
    /// OBSOLETE
 	MStatus		getRotationQuaternion( double &x, double &y, double &z,
									   double &w, MSpace::Space ) const;
    /// OBSOLETE
 	MStatus		setRotationQuaternion( double x, double y, double z,
									   double w, MSpace::Space );
    /// OBSOLETE
	MVector		translation( MSpace::Space, MStatus* ReturnStatus=NULL ) const;

	///
	static const MTransformationMatrix identity;


protected:
// No protected members

private:
	const void*		data;
	const char*	className() const;
	MTransformationMatrix( const void* );
};

#ifdef _WIN32
#pragma warning(default: 4522)
#endif // _WIN32

// *****************************************************************************
#endif /* __cplusplus */
#endif /* _MTransformationMatrix */

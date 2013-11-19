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
// CLASS:    MGeometryData
//
// *****************************************************************************
//
// CLASS DESCRIPTION (MGeometryData)
//
// MGeometryData is a class which allows storage of arbitrary data which
// is formated to be specifically suitable for usage using a 3D display
// interface such as OpenGL.
//
// Format options include:
//		- Each element can current be of a 1, 2, 3, 4 tuple.
//		- Element type can be IEEE single or double precision float,
//		  signed or unsigned byte (character), 16 or 32 bit signed unsigned integers.
//		- There are specific "type" identifiers to provide a semantic on
//		  for the data. These include:
//			- position : vertex position vector
//			- normal : vertex normal vector
//			- texCoord : vertex texture coordinate vector
//			- color : vertex color vector
//			- weight : vertex skin weight vector
//			- tangent : vertex tangent vector
//			- binormal : vertex binormal vector
//			- velocity : vertex velocity vector
//			- primitiveCenter : center a primitive (e.g. a triangle)
//			- colorMask : colour existance mask. 1:1 correspondance with color
//			- useData : some user defined data. A semantic that Maya
//			  does not understand.
// 
//	Currently Maya only interprets a fixed format subset for data with recongnized semantics, 
//  This does not mean that the user cannot create any arbitrary format for data storage.
//  Support formats with semantics includes:
//
//		- 3 float position interpreted as (x,y,z).
//		- 3 float normal interpreted as (x,y,z).
//		- 2 float texture coordinate. Coorindate is interpreted as a (U,V) tuple.
//		- 4 float color. Color is interpreted as (Red, Green, Blue, Alpha) tuple.
//		- 3 float tangent interpreted as (x,y,z).
//		- 3 flaot binormal interpreted as (x,y,z).
//		- 3 float velocity interpreted as (x,y,z).
//		- 3 float primitive center position interpreted as (x,y,z).
//		- 1 float color mask interpreted as 1=mapped, and 0 = unmapped.
//
// Memory allocation of the correct size is left up to the user of this class.
// Memory can marked to be "owned" by this class or the user of ths class. Ownership
// by this class is the default behaviour specified in the constructor.
// If the data is marked as being owned by the class, it is assumed that the data 
// is created using a "new" operation, as the destructor of this class will use
// a "delete" operation to free memory.
//
// Internal Maya data which is passed to the user via this class is always assumed to be
// non-modifiable. If modified stability cannot be ensured.
//
// *****************************************************************************

#if defined __cplusplus

// *****************************************************************************

// INCLUDED HEADER FILES

#ifndef _MGeometryData
#define _MGeometryData

#include <maya/MTypes.h>

class MHardwareRenderer;

// *****************************************************************************

// CLASS DECLARATION (MGeometryData)

/// Geometric data container
/**
This class is a geometric data container
*/
#ifdef _WIN32
#pragma warning(disable: 4522)
#endif // _WIN32

class OPENMAYARENDER_EXPORT MGeometryData
{
public:
	/// Specifies the size or dimension of each data element of the storage.
	///
	enum ElementSize {
		///
		kInvalidElementSize = 0,
		/// Single value
		kOne = 1,
		/// 2-tuple
		kTwo,
		/// 3-tuple
		kThree,
		/// 4-tuple
		kFour
	};

	/// Specifies the data type of each data element of the storage.
	///
	enum ElementType {
		/// Invalid element type (default value)
		kInvalidElementType = -1,
		/// IEEE single precision floating point
		kFloat = 0,
		/// IEEE double precision floating point
		kDouble,
		/// Signed char
		kChar,
		/// Unsigned char
		kUnsignedChar,
		/// Signed 16-bit integer
		kInt16,
		/// Unsigned 16-bit integer
		kUnsignedInt16,
		/// Signed 32-bit integer
		kInt32,
		/// Unsigned 32-bit integer
		kUnsignedInt32
	};
	
	/// Specifies the data type of the storage array.
	///
	enum DataType {
		/// Invalid data type (default value)
		kInvalidDataType = 0, 
		/// Position vector
		kPosition,		    
		/// Normal vector
		kNormal,    		
		/// Texture coordinate tuple
		kTexCoord,	    	
		/// Color tuple
		kColor,		    	
		/// Vertex weighting data
		kWeight,	    	
		/// Separator to indicate native draw API supported types
		kAPISupported,  	
		/// Tangent vector
		kTangent,       	
		/// Bi-normal vector
		kBiNormal,	    	
		/// Velocity vector
		kVelocity,			
		/// Center of primitive 
		kPrimitiveCenter,	
		/// Mapped, unmapped color mask
		kColorMask,			
		/// Arbitrary "user data"
		kUserData,	    	
		/// Valid entries are < kMaxDataTypeIndex 
		kMaxDataTypeIndex	
	};

	/// Constructor
	///
	MGeometryData(
		const char *	dataName,
        DataType		dataType,
        ElementType		elementType,
        ElementSize		elementSize,
        unsigned int	elementCount,
        void*			dataPtr = NULL,
		bool			ownsData = true);

	/// Destructor
	///
	~MGeometryData();

	///
	const char *	objectName() const;	
	///
	int				uniqueID() const;	
	///
    DataType 		dataType() const;	
	///
    ElementType		elementType() const;	
	///
	unsigned int	elementTypeSize() const;
	///
    ElementSize		elementSize() const;	
	///
    unsigned int	elementCount() const;
	///
    void *			data() const;
	///
    void			setCollectionNumber(int cn); 
	///
    int				collectionNumber() const;
	///
	void			setObjectOwnsData(bool val);
	///
	bool			objectOwnsData() const;

protected:
	// Default constructor is protected
	MGeometryData();
	// Data destructor is protected
	void release();

	friend class MHardwareRenderer;

	void *_pGeometryData;

private:
// No private members

};

#endif
#endif // _MGeometryData

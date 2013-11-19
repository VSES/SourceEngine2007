//====== Copyright © 1996-2005, Valve Corporation, All rights reserved. =======
//
// Checks to see if a given Maya attribute is the default
//
//=============================================================================

// Maya includes

#include <maya/MFnAttribute.h>
#include <maya/MFnEnumAttribute.h>
#include <maya/MFnMatrixAttribute.h>
#include <maya/MFnMatrixData.h>
#include <maya/MFnNumericAttribute.h>
#include <maya/MFnTypedAttribute.h>
#include <maya/MFnUnitAttribute.h>
#include <maya/MMatrix.h>
#include <maya/MPlug.h>
#include <maya/MVector.h>

// Valve includes
#include "valveMaya.h"


//-----------------------------------------------------------------------------
// Determines if a Maya MFnEnumAttribute MPlug is the default value or not
//-----------------------------------------------------------------------------
template <>
bool ValveMaya::IsDefault(
	MFnEnumAttribute &aFn,
	const MPlug &aPlug )
{
	if ( aPlug.isNull() || aFn.isDynamic() )
		return false;

    short c( 0 );	// Current Value
    short d( 0 );	// Default Value

    aPlug.getValue( c );
    aFn.getDefault( d );

    return c != d;
}


//-----------------------------------------------------------------------------
// Determines if a Maya MFnNumericAttribute MPlug is the default value or not
//-----------------------------------------------------------------------------
template <>
bool ValveMaya::IsDefault(
	MFnNumericAttribute &aFn,
	const MPlug &aPlug )
{
	if ( aPlug.isNull() || aFn.isDynamic() )
		return false;

	MObject cObj;	// Current value MObject

	MVector c;		// Current Value
	MVector d;		// Default Value

	switch ( aFn.unitType() )
	{
	case MFnNumericData::kBoolean:
		{
			bool td;
			aFn.getDefault( td );
			d.x = td;
			aPlug.getValue( c.y );
		}
		break;
	case MFnNumericData::kByte:
	case MFnNumericData::kChar:
		{
			char td;
			aFn.getDefault( td );
			d.x = td;
			aPlug.getValue( c.y );
		}
		break;
	case MFnNumericData::kShort:
	case MFnNumericData::kInt:
		{
			int td;
			aFn.getDefault( td );
			d.x = td;
			aPlug.getValue( c.y );
		}
		break;
	case MFnNumericData::kFloat:
	case MFnNumericData::kDouble:
		{
			aFn.getDefault( d.x );
			aPlug.getValue( c.y );
		}
		break;
	case MFnNumericData::k2Short:
	case MFnNumericData::k2Int:
	case MFnNumericData::k2Float:
	case MFnNumericData::k2Double:
		{
			aFn.getDefault( d.x, d.y );
			aPlug.getValue( cObj );
			MFnNumericData( cObj ).getData( c.y, c.y );
		}
		break;
	case MFnNumericData::k3Short:
	case MFnNumericData::k3Int:
	case MFnNumericData::k3Float:
	case MFnNumericData::k3Double:
		{
			aFn.getDefault( d.x, d.y, d.z );
			aPlug.getValue( cObj );
			MFnNumericData( cObj ).getData( c.y, c.y, c.z );
		}
		break;
	default:
		break;
	}

	return c != d;
}


//-----------------------------------------------------------------------------
// Determines if a Maya MFnMatrixAttribute MPlug is the default value or not
//-----------------------------------------------------------------------------
template <>
bool ValveMaya::IsDefault(
	MFnMatrixAttribute &aFn,
	const MPlug &aPlug )
{
	if ( aPlug.isNull() || aFn.isDynamic() )
		return false;

	MObject cObj;	// Current Value
	MMatrix d;		// Current Value

	aFn.getDefault( d );
	aPlug.getValue( cObj );

	return MFnMatrixData( cObj ).matrix() != d;
}


//-----------------------------------------------------------------------------
// Determines if a Maya MFnTypedAttribute MPlug is the default value or not
//-----------------------------------------------------------------------------
template <>
bool ValveMaya::IsDefault(
	MFnTypedAttribute &aFn,
	const MPlug &aPlug )
{
	if ( aPlug.isNull() || aFn.isDynamic() )
		return false;

	MObject cObj;	// Current Value
	MObject dObj;	// Default Value

	aFn.getDefault( dObj );
	aPlug.getValue( cObj );

	return cObj != dObj;
}


//-----------------------------------------------------------------------------
// Determines if a Maya MFnUnitAttribute MPlug is the default value or not
//-----------------------------------------------------------------------------
template <>
bool ValveMaya::IsDefault(
	MFnUnitAttribute &aFn,
	const MPlug &aPlug )
{
	if ( aPlug.isNull() || aFn.isDynamic() )
		return false;

    double c( 0 );	// Current Value
    double d( 0 );	// Default Value

    aPlug.getValue( c );
    aFn.getDefault( d );

    return c != d;
}


//-----------------------------------------------------------------------------
// Default - false!
//-----------------------------------------------------------------------------
template < class T_t >
bool ValveMaya::IsDefault(
	T_t &,
	const MPlug & )
{
	return false;
}


//-----------------------------------------------------------------------------
// Determines if a Maya MPlug is the default value or not
//-----------------------------------------------------------------------------
bool ValveMaya::IsDefault(
	const MPlug &aPlug )
{
	if ( aPlug.isNull() )
		return false;

	const MObject aObj( aPlug.attribute() );

	if ( MFnAttribute( aObj ).isDynamic() )
		return false;

	switch ( aObj.apiType() )
	{
	case MFn::kEnumAttribute:
		return IsDefault( MFnEnumAttribute( aObj ), aPlug );
	case MFn::kMatrixAttribute:
		return IsDefault( MFnMatrixAttribute( aObj ), aPlug );
	case MFn::kNumericAttribute:
		return IsDefault( MFnNumericAttribute( aObj ), aPlug );
	case MFn::kTypedAttribute:
		return IsDefault( MFnTypedAttribute( aObj ), aPlug );
	case MFn::kUnitAttribute:
		return IsDefault( MFnUnitAttribute( aObj ), aPlug );
	default:
		break;
	}

	return false;
}
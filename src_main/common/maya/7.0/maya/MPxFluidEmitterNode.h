
#ifndef _MPxFluidEmitterNode
#define _MPxFluidEmitterNode
//
//-
// ==========================================================================
// Copyright (C) Alias Systems Corp., and/or its licensors ("Alias").  
// All rights reserved.  These coded instructions, statements, computer  
// programs, and/or related material (collectively, the "Material")
// contain unpublished information proprietary to Alias, which is
// protected by Canadian and US federal copyright law and by international
// treaties. This Material may not be disclosed to third parties, or be copied
// or duplicated, in whole or in part, without the prior written consent of
// Alias.  ALIAS HEREBY DISCLAIMS ALL WARRANTIES RELATING TO THE MATERIAL,
// INCLUDING, WITHOUT LIMITATION, ANY AND ALL EXPRESS OR IMPLIED WARRANTIES OF
// NON-INFRINGEMENT, MERCHANTABILITY AND/OR FITNESS FOR A PARTICULAR PURPOSE.
// IN NO EVENT SHALL ALIAS BE LIABLE FOR ANY DAMAGES WHATSOEVER, WHETHER DIRECT,
// INDIRECT, SPECIAL, OR PUNITIVE, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE
// OR OTHER TORTIOUS ACTION, OR IN EQUITY, ARISING OUT OF OR RELATED TO THE
// ACCESS TO, USE OF, OR RELIANCE UPON THE MATERIAL.
// ==========================================================================
//+
//
// CLASS:    MPxFluidEmitterNode
//
// *****************************************************************************
//
// CLASS DESCRIPTION (MPxFluidEmitterNode)
//
// MPxFluidEmitterNode allows the creation and manipulation of dependency graph
// nodes representing emitters.  This is the top level of a hierarchy of
// emitter node function sets.  It permits manipulation of the attributes
// common to all types of emitters.
//
// *****************************************************************************

#if defined __cplusplus

// *****************************************************************************

// INCLUDED HEADER FILES


#include <maya/MStatus.h>
#include <maya/MObject.h>
#include <maya/MPxNode.h>
#include <maya/MPxEmitterNode.h>
#include <maya/MPointArray.h>
#include <maya/MVectorArray.h>
#include <maya/MDoubleArray.h>
#include <maya/MFnFluid.h>
#include <maya/MColor.h>


// *****************************************************************************

// DECLARATIONS

// *****************************************************************************

// CLASS DECLARATION (MPxFluidEmitterNode)

/// Base class for user defined particle emitters (OpenMayaFX)
/**
*/

#ifdef _WIN32
#pragma warning(disable: 4522)
#endif // _WIN32


class OPENMAYAFX_EXPORT MPxFluidEmitterNode : public MPxEmitterNode
{
public:
	///
	MPxFluidEmitterNode();
	///
	virtual ~MPxFluidEmitterNode();
	///
	virtual MPxNode::Type	type() const;
	///
	virtual MStatus			compute(const MPlug& plug, MDataBlock& dataBlock);

	///
	virtual MStatus			fluidEmitter( MFnFluid& fluid, const MMatrix& worldMatrix, int plugIndex );

	/// density emission rate attribute
	static MObject mFluidDensityEmission;
	///	heat emission rate attribute
	static MObject mFluidHeatEmission;
	/// fuel emission rate attribute
	static MObject mFluidFuelEmission;
	/// color emission toggle attribute	
	static MObject mEmitFluidColor;
	/// emission color attribute
	static MObject mFluidColor;
	/// emission red color channel attribute
	static MObject mFluidColorR;
	/// emission green color channel attribute
	static MObject mFluidColorG;
	/// emission blue color channel attribute
	static MObject mFluidColorB;
	/// emission dropoff attribute
	static MObject mFluidDropoff;
	/// emission turbulence attribute
	static MObject mTurbulence;
	/// emission jitter attribute
	static MObject mFluidJitter;
	///	emission function attribute
	static MObject mEmissionFunction;

	///
	double fluidDensityEmission( MDataBlock& block );
	///
	double fluidHeatEmission( MDataBlock& block );
	///
	double fluidFuelEmission( MDataBlock& block );
	///
	bool   fluidEmitColor( MDataBlock& block );
	///
	MColor fluidColor( MDataBlock& block );
	
	///
	double fluidDropoff( MDataBlock& block );
	///
	double turbulence( MDataBlock& block );
	///
	bool   fluidJitter( MDataBlock& block );

protected:
// No protected members


private:
// No private members

	static void			initialSetup();
	static const char*	className();

};

#ifdef _WIN32
#pragma warning(default: 4522)
#endif // _WIN32

//****************************************************************************
#endif /* __cplusplus */
#endif /* _MPxFluidEmitterNode */



#ifndef _MPxIkSolverNode
#define _MPxIkSolverNode
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
// CLASS:    MPxIkSolverNode
//
// *****************************************************************************
//
// CLASS DESCRIPTION (MPxIkSolverNode)
//
//  The base class for user defined IK solvers.
//
// *****************************************************************************

#if defined __cplusplus

// *****************************************************************************

// INCLUDED HEADER FILES



#include <maya/MStatus.h>
#include <maya/MTypes.h>
#include <maya/MObject.h>
#include <maya/MPxNode.h>

// *****************************************************************************

// DECLARATIONS

class MString;
class MArgList;
class MIkHandleGroup;
class MMatrix;
class MDoubleArray;

// *****************************************************************************

// CLASS DECLARATION (MPxIkSolverNode)

/// Base class for user defined IK solvers  (OpenMayaAnim)
/**

Derive from this class to create user-defined IK solvers.

*/
#ifdef _WIN32
#pragma warning(disable: 4522)
#endif // _WIN32

class OPENMAYAANIM_EXPORT MPxIkSolverNode : public MPxNode
{
public:
	///
	virtual ~MPxIkSolverNode();
	///
	virtual MPxNode::Type type() const;

	///
	virtual MStatus		preSolve();
	///
	virtual MStatus		doSolve();
	///
	virtual MStatus		postSolve( MStatus );

	///
	virtual MString		solverTypeName() const;

	///
	bool				rotatePlane(MStatus *ReturnStatus = NULL) const;
	///
	MStatus				setRotatePlane(bool rotatePlane);
	///
	bool				singleChainOnly(MStatus *ReturnStatus = NULL) const;
	///
	MStatus				setSingleChainOnly(bool singleChainOnly);
	///
	bool				positionOnly(MStatus *ReturnStatus = NULL) const;
	///
	MStatus				setPositionOnly(bool positionOnly);
	///
	bool				supportJointLimits(MStatus *ReturnStatus = NULL) const;
	///
	MStatus				setSupportJointLimits(bool supportJointLimits);
	///
	bool				uniqueSolution(MStatus *ReturnStatus = NULL) const;
	///
	MStatus				setUniqueSolution(bool uniqueSolution);

	/// OBSOLETE
	virtual bool		isSingleChainOnly() const;
	/// OBSOLETE
	virtual bool		isPositionOnly() const;
	/// OBSOLETE
	virtual bool		hasJointLimitSupport() const;
	/// OBSOLETE
	virtual bool		hasUniqueSolution() const;
	/// OBSOLETE
	virtual bool		groupHandlesByTopology() const;

	///
	virtual MStatus		setFuncValueTolerance( double tolerance );
	///
	virtual MStatus		setMaxIterations( int value );

	///
	MIkHandleGroup * 	handleGroup() const;
	///
	virtual void 		setHandleGroup( MIkHandleGroup* );
	///
	const MMatrix *		toWorldSpace() const;
	///
	const MMatrix *		toSolverSpace() const;
	///
	double 				funcValueTolerance() const;
	///
	int 				maxIterations() const;
	///
	virtual void 		snapHandle( MObject& handle );

	void				create();

protected:

	///
	MStatus				getJointAngles( MDoubleArray& ) const;
	///
	MStatus				setJointAngles( const MDoubleArray& );
	///
	void				setToRestAngles();

	///
	MPxIkSolverNode();

	static const char*	className();

private:
	void*	instance;

};

#ifdef _WIN32
#pragma warning(default: 4522)
#endif // _WIN32

// *****************************************************************************
#endif /* __cplusplus */
#endif /* _MPxIkSolverNode */

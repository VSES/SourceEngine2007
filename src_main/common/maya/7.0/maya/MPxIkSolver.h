
#ifndef _MPxIkSolver
#define _MPxIkSolver
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
// CLASS:    MPxIkSolver
//
// *****************************************************************************
//
// CLASS DESCRIPTION (MPxIkSolver)
//
//  The base class for user defined ik solvers.
//
// *****************************************************************************

#if defined __cplusplus

// *****************************************************************************

// INCLUDED HEADER FILES



#include <maya/MStatus.h>
#include <maya/MTypes.h>
#include <maya/MObject.h>

// *****************************************************************************

// DECLARATIONS

class MString;
class MArgList;
class MIkHandleGroup;
class MMatrix;
class MDoubleArray;

// *****************************************************************************

// CLASS DECLARATION (MPxIkSolver)

/// OBSOLETE CLASS: Base class for user defined IK solvers  (OpenMayaAnim)
/**

MPxIkSolver is OBSOLETE, and will be removed in a future version of Maya.
The MPxIkSolver class has been replaced with the MPxIkSolverNode class.
This is an obsolete base class for writing user-defined IK solvers.

*/
#ifdef _WIN32
#pragma warning(disable: 4522)
#endif // _WIN32

class OPENMAYAANIM_EXPORT MPxIkSolver
{
public:
	///
	virtual ~MPxIkSolver();

	///
	static void			registerSolver( const MString & solverName,
								MCreatorFunction creatorFunction );


	///
	virtual MStatus		preSolve();
	///
	virtual MStatus		doSolve();
	///
	virtual MStatus		postSolve( MStatus );


	// These methods MUST be overridden by the user.
	//
	///
	virtual MString		solverTypeName() const;
	///
	virtual bool		isSingleChainOnly() const;
	///
	virtual bool		isPositionOnly() const;
	///
	virtual bool		hasJointLimitSupport() const;
	///
	virtual bool		hasUniqueSolution() const;
	///
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
	MPxIkSolver();

	virtual const char*	className() const;

private:
	void*	instance;

};

#ifdef _WIN32
#pragma warning(default: 4522)
#endif // _WIN32

// *****************************************************************************
#endif /* __cplusplus */
#endif /* _MPxIkSolver */

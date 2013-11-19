#ifndef _MDrawProcedureBase
#define _MDrawProcedureBase
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
// CLASS:    MDrawProcedureBase
//
// *****************************************************************************
//
// CLASS DESCRIPTION (MDrawProcedureBase)
//
// This class provides an interface through which a plugin can implement a class to provide 
// custom hardware drawing effects.
// 
// The derived class needs to be registered with using <to be completed>
// Please refer to documentation of <> for more details.
// 
// Each procedure has a user defined string name and can be enabled or disabled. 
// There is also a logical "call location" which defines when within a rendering loop that 
// the draw procedure will be called. Name, enabling and call location must be defined.
// 
// Additionally, the procedure can define a desired sequence number. All draw procedures
// are sorted by sequence number. Procedures with lower sequence number will be
// invoked first. The number 0 is the lowest supported sequence number and is the default.
// When more than one procedure has a sequence number, the order of registration
// will determine the invocation order.
// 
// All derived classes must over the execute() method. This is the method that 
// will be called by the hardware renderer to which the procedure is attached.
// The call will only be made if the procedure is enabled.
// 
// *****************************************************************************

#if defined __cplusplus

#ifdef _WIN32
#pragma warning(disable: 4522)
#endif // _WIN32

// INCLUDED HEADER FILES


// INCLUDES
#include <maya/MString.h>

// CLASS DECLARATION (MDrawProcedureBase)

/// Base user draw procedure class


class OPENMAYARENDER_EXPORT MDrawProcedureBase 
{
public:
	// constructor 
	///
	MDrawProcedureBase(const MString &name);
	
	// destructor 
	///
	virtual	~MDrawProcedureBase();

	// Method called to perform drawing effect. Derived classes must implement this.
	///
	virtual bool execute() = 0;

	// Set the procedure to be enabled or not.
	///
	void		setEnabled(bool value);

	// Returns if the procedure is enabled.
	///
	bool		enabled() const ;	

	//	Set the name of the procedure
	///
	void		setName( const MString &name );

	// Get the name of the procedure
	///
	const MString &name() const;

protected :
	// Protected default constructor
	MDrawProcedureBase();

	// Name of the procedure
	MString		fName;

	// Enable / disable toggle
	bool		fEnabled;
	
private:
	// No private members
};

#ifdef _WIN32
#pragma warning(default: 4522)
#endif // _WIN32

// *****************************************************************************

#endif /* __cplusplus */
#endif /* _MDrawProcedureBase */

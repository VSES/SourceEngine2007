//*****************************************************************************
/*!
   \file xsi_expression.h
   \brief Expression class declaration.

   © Copyright 1998-2003 Avid Technology, Inc. and its licensors. All rights
   reserved. This file contains confidential and proprietary information of
   Avid Technology, Inc., and is subject to the terms of the SOFTIMAGE|XSI
   end user license agreement (or EULA).
*/
//*****************************************************************************
#if (_MSC_VER > 1000) || defined(SGI_COMPILER)
#pragma once
#endif

#ifndef __XSIEXPRESSION_H__
#define __XSIEXPRESSION_H__

#include <xsi_operator.h>

namespace XSI {


//*****************************************************************************
/*! \class Expression xsi_expression.h
	\brief An expression string is a mathematical formula that controls any animatable parameter. It may include object
	and parameter names, mathematical operators, and tokens representing functions or constants.

	The	Expression object has an expression string (stored in the definition parameter) and can be created with the
	\xl AddExpr command. If a parameter value is driven by an expression you can get this expression using Parameter::GetSource.

	\sa Parameter::GetSource
	\since 4.0

	\eg
	\code
		using namespace XSI;
		Application app;
		Model root = app.GetActiveSceneRoot();

		Null myNull;
		root.AddNull( L"myNull", myNull );

		Parameter posx = myNull.GetParameter(L"posx");

		CValueArray args(2);
		CValue		outArg;
		args[0] = posx.GetFullName();
		args[1] = posx.GetFullName() + L" + 4.0";

		app.ExecuteCommand(L"AddExpr", args, outArg);

		Expression myPosxExpr = posx.GetSource();
		Parameter myDefinition = myPosxExpr.GetParameter(L"definition");

		app.LogMessage(L"The expression definition is " +
						myDefinition.GetValue().GetAsText());

	\endcode
 */
//*****************************************************************************
class SICPPSDKDECL Expression : public Operator
{
public:
	/*! Default constructor. */
	Expression();

	/*! Default destructor. */
	~Expression();

	/*! Constructor.
	\param in_ref constant reference object.
	*/
	Expression(const CRef& in_ref);

	/*! Copy constructor.
	\param in_obj constant class object.
	*/
	Expression(const Expression& in_obj);

	/*! Returns true if a given class type is compatible with this API class.
	\param in_ClassID class type.
	\return true if the class is compatible, false otherwise.
	*/
	bool IsA( siClassID in_ClassID) const;

	/*! Returns the type of the API class.
	\return The class type.
	*/
	siClassID GetClassID() const;

	/*! Creates an object from another object. The newly created object is set to
	empty if the input object is not compatible.
	\param in_obj constant class object.
	\return The new Expression object.
	*/
	Expression& operator=(const Expression& in_obj);

	/*! Creates an object from a reference object. The newly created object is
	set to empty if the input reference object is not compatible.
	\param in_ref constant class object.
	\return The new Expression object.
	*/
	Expression& operator=(const CRef& in_ref);

	private:
	Expression * operator&() const;
	Expression * operator&();
};

}

#endif // __XSIEXPRESSION_H__

//*****************************************************************************
/*!
   \file xsi_progressbar.h
   \brief ProgressBar class declaration.

   © Copyright 1998-2002 Avid Technology, Inc. and its licensors. All rights
   reserved. This file contains confidential and proprietary information of
   Avid Technology, Inc., and is subject to the terms of the SOFTIMAGE|XSI
   end user license agreement (or EULA).
*/
//*****************************************************************************

#if (_MSC_VER > 1000) || defined(SGI_COMPILER)
#pragma once
#endif

#ifndef __XSIPROGRESSBAR_H__
#define __XSIPROGRESSBAR_H__

#include <xsi_base.h>
#include <xsi_value.h>
#include <xsi_status.h>

#pragma warning(disable:4251)

namespace XSI {

//*****************************************************************************
/*! \class ProgressBar xsi_progressbar.h
	\brief A progress bar is a control that indicates the progress of a lengthy
	operation by displaying a colored bar inside a horizontal rectangle. The
	length of the bar, in relation to the length of the rectangle, corresponds
	to the percentage of the operation that is complete.

	Use UIToolkit::GetProgressBar to create a progress bar. The default minimum
	and maximum is between 0 and 100 with a step of 1.

	\since 4.0

	\eg Demonstrates the use of the progress bar.
	\code
		using namespace XSI;

		Application app;

		UIToolkit kit = app.GetUIToolkit();
		ProgressBar bar = kit.GetProgressBar();

		bar.PutMaximum( 50000 );
		bar.PutStep( 10 );
		bar.PutVisible( true );
		bar.PutCaption( L"Processing" );

		while (!bar.IsCancelPressed() && bar.GetValue() < bar.GetMaximum() )
		{
			CValue newVal(bar.Increment());
			bar.PutStatusText( L"Frame " + newVal.GetAsText() );
		}
	\endcode
*/
//*****************************************************************************

class SICPPSDKDECL ProgressBar : public CBase
{
public:
	/*! Default constructor. */
	ProgressBar();

	/*! Default destructor. */
	~ProgressBar();

	/*! Constructor.
	\param in_ref constant reference object.
	*/
	ProgressBar(const CRef& in_ref);

	/*! Copy constructor.
	\param in_obj constant class object.
	*/
	ProgressBar(const ProgressBar& in_obj);

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
	\return The new ProgressBar object.
	*/
	ProgressBar& operator=(const ProgressBar& in_obj);

	/*! Creates an object from a reference object. The newly created object is
	set to empty if the input reference object is not compatible.
	\param in_ref constant class object.
	\return The new ProgressBar object.
	*/
	ProgressBar& operator=(const CRef& in_ref);

	/*! Returns the upper limit of the progress bar's range. The default value is 100.
	\return The upper limit of the progress bar's range.
	*/
	LONG GetMaximum() const;

	/*! Sets the upper limit of the progress bar's range.

	\param in_val The maximum value to be set for the progress bar.
		Attempting to set the maximum value to be less than the minimum value
		will result in an "invalid argument" error.

	\return CStatus::OK success
	\return CStatus::InvalidArgument Invalid argument.
	*/
	CStatus PutMaximum( LONG in_val );

	/*! Returns the lower limit of the progress bar's range. The default value is 0.
	\return The lower limit of the progress bar's range.
	*/
	LONG GetMinimum() const;

	/*! Sets the lower limit of the progress bar's range.

	\param in_val The minimum value to be set for the progress bar.
		Attempting to set the minimum value to be greater than the maximum
		value will result in an "invalid argument" error.

	\return CStatus::OK success
	\return CStatus::InvalidArgument Invalid argument.
	*/
	CStatus PutMinimum( LONG in_val );

	/*! Returns the step value for the progress bar. The default value is 0.
	\return The step value for the progress bar.
	*/
	LONG GetStep() const;

	/*! Sets the step value, which is the amount the progress bar will be
	incremented per increment step.

	\param in_step The increment amount to be set for an increment step of
		the progress bar. Attempting to set the step value to be greater than
		the maximum value or a negative value will result in an "invalid argument" error.

	\return CStatus::OK success
	\return CStatus::InvalidArgument Invalid argument.
	*/
	CStatus PutStep( LONG in_step );

	/*! Returns the value of the progress bar. The default value is 0.
	\return The value of the progress bar.
	*/
	LONG GetValue() const;

	/*! Updates the value of the progress bar. If the visibility is set to
	true the ProgressBar is redrawn to reflect the new position.
	\param in_val Attempting to set the value to be greater than the maximum value
		will result in an "invalid argument" error.
	\return CStatus::OK success
	\return CStatus::InvalidArgument Invalid argument.
	*/
	CStatus PutValue( LONG in_val );

	/*! Returns the caption to be displayed during incrementation.
	\return The caption to be displayed during incrementation.
	*/
	CString GetCaption() const;

	/*! Sets the caption to be displayed as the progress bar increments.
	\param in_str Text to display in the caption.
	\return CStatus::OK success
	\return CStatus::Fail Failure.
	*/
	CStatus PutCaption( const CString& in_str );

	/*! Returns the status text to be displayed as the progress bar increments.
	\return The status text to be displayed as the progress bar increments.
	*/
	CString GetStatusText() const;

	/*! Sets the status text to be displayed during incrementation.
	\param in_str Test to display.
	\return CStatus::OK success
	\return CStatus::Fail Failure.
	*/
	CStatus PutStatusText( const CString& in_str );

	/*! Returns the visibility state for the progress bar.
	\return true if the progress bar is visible or false if hidden.
	*/
	bool GetVisible() const;

	/*! Sets the visibility of the progress bar. Use this property to make the
	progress bar appear or disappear on the screen.
	\param in_flag Display the progress bar if true or hide it if false.
	\return CStatus::OK success
	\return CStatus::Fail Failure.
	*/
	CStatus PutVisible( bool in_flag );

	/*! Returns true if the Cancel button on the progress bar has been clicked.
	\return true if the Cancel button on the progress bar has been clicked.
	*/
	bool IsCancelPressed() const;

	/*! Advances the current position for the progress bar control by the
	specified number of steps. If the visibility is set to true the
	progress bar is redrawn to reflect the new position.

	\param in_val The amount to increment the current value.
	\return The new incremented value or -1 if the input value is less than zero.
	*/
	LONG Increment( LONG in_val=1 );


	/*! Provides control over whether the progress bar will have a working Cancel button
	or not.  By default the Cancel button is enabled. For operations that cannot
	be stopped part way through it is good UI design to disable the Cancel button.
	\tip It is necessary to call this function before calling ProgressBar::PutVisible.
	\since 4.0
	*/
	CStatus PutCancelEnabled( bool in_bEnable ) ;

	/*! Returns true when the Cancel button on the progress bar is enabled */
	bool IsCancelEnabled() const ;


	private:
	ProgressBar * operator&() const;
	ProgressBar * operator&();
};

};
#endif // __XSIPROGRESSBAR_H__

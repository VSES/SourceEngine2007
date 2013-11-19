//*****************************************************************************
/*!
   \file xsi_gridwidget.h
   \brief GridWidget class declaration.

   © Copyright 1998-2003 Avid Technology, Inc. and its licensors. All rights
   reserved. This file contains confidential and proprietary information of
   Avid Technology, Inc., and is subject to the terms of the SOFTIMAGE|XSI
   end user license agreement (or EULA).
*/
//*****************************************************************************

#if (_MSC_VER > 1000) || defined(SGI_COMPILER)
#pragma once
#endif

#ifndef __XSIGRIDWIDGET_H__
#define __XSIGRIDWIDGET_H__

#include <xsi_base.h>
#include <xsi_ppgitem.h>
#include <xsi_value.h>
namespace XSI {

struct CColor;

//*****************************************************************************
/*! \class GridWidget xsi_gridwidget.h
	\brief The GridWidget object represents the user interface control that shows a GridData object inside a %Property Page.

	It exposes information that is transitive and directly related to the state of the control itself rather than the
	underlying GridData. For example, this object exposes information about which cells the user has clicked on.

	Unlike the actual GridData content, the selection state is lost when the %Property Page is closed and then reopened.

	\note The layout of the GridData on the property page is defined by the PPGItem object that represents the GridData on
		the PPGLayout for that property page. For example, the ::siUIGridSelectionMode attribute controls whether cells are
		selected individually, by row or column, or not at all (see ::siPPGItemAttribute).

	\sa GridData::GetGridWidget

	\since 4.2
*/
//*****************************************************************************

class SICPPSDKDECL GridWidget : public CBase
{
public:
	/*! Default constructor. */
	GridWidget();

	/*! Default destructor. */
	~GridWidget();

	/*! Constructor.
	\param in_ref constant reference object.
	*/
	GridWidget(const CRef& in_ref);

	/*! Copy constructor.
	\param in_obj constant class object.
	*/
	GridWidget(const GridWidget& in_obj);

	/*! Returns true if a given class type is compatible with this API class.
	\param in_ClassID class type.
	\return true if the class is compatible, false otherwise.
	*/
	bool IsA( siClassID in_ClassID) const;

	/*! Returns the type of the API class.
	\return The class type.
	*/
	siClassID GetClassID() const;

	/*! Creates an object from another object.
	\param in_obj constant class object.
	\return The new GridWidget object.
	*/
	GridWidget& operator=(const GridWidget& in_obj);

	/*! Creates an object from a reference object. The newly created object is
	set to empty if the input reference object is not compatible.
	\param in_ref constant class object.
	\return The new GridWidget object.
	*/
	GridWidget& operator=(const CRef& in_ref);

	/*! Tests whether a cell has been selected. If the Grid is in header selection mode then use
	GridWidget::IsRowSelected or GridWidget::IsColumnSelected instead. (For further information about
	configuring the selection mode see ::siUIGridSelectionMode in the ::siPPGItemAttribute enum
	documentation).
	\param in_Column		Index of the column where the cell is located.
	\param in_Row			Index of the row where the cell is located.
	\return true if the specified cell is selected
	*/
	bool IsCellSelected( LONG in_Column, LONG in_Row) const;

	/*! Tests whether an entire row has been selected
	\param in_Row			Index of the row to test.
	\return true if the specified row is selected
	*/
	bool IsRowSelected( LONG in_Row ) const ;

	/*! Tests whether an entire column has been selected
	\param in_Column		Index of the column to test.
	\return true if the specified column is selected
	*/
	bool IsColumnSelected( LONG in_Column ) const ;


	/*! Adds a cell to the selection.  In header selection mode, specify -1 as the row to select an
	entire column, or specify -1 as the column to select an entire row.
	\param in_Column		Index of the column where the cell is located.
	\param in_Row			Index of the row where the cell is located.
	\return Whether the selection change was successful or not.
	*/
	CStatus AddToSelection( LONG in_Column, LONG in_Row ) ;

	/*! Ensures that no cells are selected on the GridWidget.
	\return Whether clearing the selection was successful or not.
	\since 5.0
	*/
	CStatus ClearSelection() ;

	/*! Sorts the specified column in the direction indicated.
	\param in_Column		Index of the column to sort.
	\param in_bAscending	True to sort in ascending order; false for descending.
	\return Whether the sort was successful or not.
	\since 5.0
	*/
	CStatus SortColumn( LONG in_Column, bool in_bAscending);

	private:
	GridWidget * operator&() const;
	GridWidget * operator&();
};

};
#endif // __XSIGRIDWIDGET_H__

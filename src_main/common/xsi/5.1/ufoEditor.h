/*******************************************************************************
 * 
 * ufoEditor.h
 * 
 * Copyright (c) 3 Space Software Ltd. 1994, 1995.
 *				 Avid Technology Inc.  1996.
 * 
 ******************************************************************************/


#ifndef	__ufoEditor
#define	__ufoEditor


/******************************************************************************
 * ufoEditor.h:
 *
 * For all editor instance handles.
 *
 * Function	Declarations:
 *
 * ufoEditorSetUserData():		   Set user	data associated	with editor	instance
 * ufoEditorGetUserData():		   Get user	data associated	with editor	instance
 *
 * ufoEditorCreateStandardEditor():(NOT IMPLEMENTED IN XSI )
 *                                 Create a	standard editor	for	a parameter	within the custom GUI
 *
 * ufoEditorGetOverlayIndex():	   Get correct colour index	for	overlay	drawing
 * ufoEditorGetOverlayIndexRange():Get minimum and maximum colour index	which can be used
 *								   when	drawing	into the overlay
 * ufoEditorSetOverlayColour():	   Set a colour	for	a particular overlay index
 *
 * ufoEditorGetPixelSize():		   Get scaling from	render size	to display size
 *
 * ufoEditorDraw():				   Cause editor	to setup context for OpenGL
 *								   drawing and call	ufoEditorOverlayDraw or
 *								   ufoEditorRGBDraw
 *
 * ufoEditorDrawSubImage()		   (XSI) Draw part of a raster on the viewer.   
 *
 * ufoEditorNotifyEdited():		   Notify of changes to	parameters as a	result
 *								   of editing
 *							
 * ufoEditorGetRasterViewed():	   Get raster handle for currently displayed
 *								   raster of edited	process
 *
 * ufoEditorForceRender():		   Force the process to	render
 *
 * ufoEditorGetCurrentViewType():  Get the current editor view type	and	number
 *
 * ufoEditorForceUpdateUI():	   (NOT IMPLEMENTED IN XSI )
 *                                 Force the keybars interface to update to	represent
 *								   current parameter visibility	status set with
 *								   ufoProcessHideParam or ufoProcessShowParam. 
 *
 * ufoEditorSetCurrentFrame():	   Force the current frame to change.
 *
 * ufoEditorSetStatusBar():		   Place a message on the main status bar.
 ******************************************************************************/


#include <stdlib.h>
#include "ufoTypes.h"
#include "ufoRaster.h"
#include "ufoProcess.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef	void * ufoEditor;


/*******************************************************************************
 * ufoEditorSetUserData():
 *	  Sets user	data to	be associated with this	instance of	the	editor.
 *	  This should be used to store a reference to structure(s) associated with
 *	  the UFO Editor,  typically to	store the GUI or screen	interaction	state.
 * 
 *	  -	May	be called within any editor	user function.
 ******************************************************************************/
UFOAPI
void ufoEditorSetUserData(
	ufoEditor		editor_instance, 
	void			*user_data);


/*******************************************************************************
 * ufoEditorGetUserData():
 *	  Gets user	data associated	with this editor instance.
 * 
 *	  -	May	be called within any editor	user function.
 ******************************************************************************/
UFOAPI
void* ufoEditorGetUserData(
	ufoEditor		editor_instance);


/*******************************************************************************
 * ufoEditorCreateStandardEditor():
 *	  Creates a	standard invocation	of a parameter editor within a UFO custom GUI.
 *	  Forinstance a	slider for a ufoFloatParam,	an option menu for a ufoEnumParam,
 *	  a	colour swatch for a	ufoColourRGBParam, e.t.c.
 *
 *	  NB for ganged	parameters you only	need to	call this function for the first
 *	  parameter	in a ganged	sequence of	parameters.
 * 
 *	  This RETURNS the child window	pointer
 *	  The user is responsible for positioning the child	window within the parent
 *
 *	  -	May	be called within ufoEditorCreate().
 ******************************************************************************/
UFOAPI
void *NOTIMPL_ufoEditorCreateStandardEditor(
	ufoEditor		editor_instance,
	void			*parent_window,	
	int				param_index);
	

/*******************************************************************************
 * This	following declaration supports a typo in a previous	pre-release. It
 * directly	calls ufoEditorCreateStandardEditor() with the same	arguments and 
 * returns its return value.
 ******************************************************************************/
UFOAPI
void *NOTIMPL_ufoCreateStandardEditor(
	ufoEditor		editor_instance,
	void			*parent_window,	
	int				param_index);

	
/*******************************************************************************
 * ufoEditorGetOverlayIndex():
 *	  Gets correct colour index	to be used for setting overlay draw	colours	in
 *	  OpenGL. For example if an	index is set to	a particular colour	using
 *	  ufoEditorSetOverlayColour(index, r, b, g),  then any use of this index
 *	  within an	Open GL	graphics call should be	made through
 *	  ufoEditorGetOverlayIndex.	eg:
 *				graphics_call(ufoEditorGetOverlayIndex(index));
 * 
 *	  This is necessary	as the application splits the overlay plane	and	reserves
 *	  some bits	for	other specific tools, therefore	requiring an offset	to be taken
 *	  into consideration when drawing in the user half.
 * 
 *	  Index	0 is reserved and only ever	relates	to the transparent colour.
 *	  Index	1, 2, and 3	relate to colours accessed through the application
 *	  preferences interface,  which	are	user-definable
 * 
 *	  -	Must be	called within ufoEditorOverlayDraw() whenever a	reference to
 *		a colour index is made.
 ******************************************************************************/
UFOAPI
int	ufoEditorGetOverlayIndex(
	ufoEditor		editor_instance, 
	int				colour_index);
	
	
/*******************************************************************************
 * ufoEditorGetOverlayIndexRange():
 * 
 * Get range of	valid index	to colour values which can be set and used
 * when	drawing	into the overlay.
 * 
 * Index 0 is reserved and only	ever relates to	the	transparent	colour.
 * Index 1,	2, and 3 relate	to user-definable colours, accessed	through
 * the application preferences interface. These	can	be over-ridden if
 * desired.
 * 
 *	- May be called	within ufoEditorOverlayDraw().
 ******************************************************************************/
UFOAPI
void ufoEditorGetOverlayIndexRange(
	ufoEditor		editor_instance, 
	int				*min_index,	
	int				*max_index);	


/*******************************************************************************
 * ufoEditorSetOverlayColour():
 * 
 * Assign a	colour to a	particular colour index, to	be used	when drawing into the
 * overlay plane.
 * 
 * Index 0 is reserved and only	ever relates to	the	transparent	colour.
 * Index 1,	2, and 3 relate	to user-definable colours, accessed	through
 * the application preferences interface. These	can	be over-ridden if
 * desired.
 * 
 * - Must be called	within ufoEditorOverlayDraw() if colours other than	those
 *	 accessed through index	1, 2 and 3 are required.
 ******************************************************************************/
UFOAPI
void ufoEditorSetOverlayColour(
	ufoEditor		editor_instance, 
	int				colour_index, 
	double			red, 
	double			green, 
	double			blue);	  


/*******************************************************************************
 * ufoEditorGetPixelSize():
 *	  Gets the scaling in the viewer from the size of a	pixel at full render
 *	  resolution to	the	display.
 * 
 *	  -	May	be called within any editor	user function.
 ******************************************************************************/
UFOAPI
void ufoEditorGetPixelSize(
	ufoEditor		editor_instance,
	 
	double*			x_size,
	double*			y_size);


/*******************************************************************************
 * ufoEditorDraw():
 *	  Cause	editor to setup	context	for	OpenGL drawing and call
 *	  ufoEditorOverlayDraw or ufoEditorRGBDraw.
 * 
 *	  NB: if it	is required	to call	from within	event functions	in the GUI then
 *	  the UFO will need	to store the ufoEditor pointer passed to it	in
 *	  ufoEditorCreateGui().
 * 
 *	  -	May	be called within ufoEditorMouseDown(), ufoEditorMouseUp(), 
 *		ufoEditorMouseDrag(), ufoEditorMousePosition(),	ufoEditorKeyEvent(), 
 *		or from	within any event function in the editor	GUI.
 *	  
 ******************************************************************************/
UFOAPI
void ufoEditorDraw(
	ufoEditor		editor_instance);


/*******************************************************************************
 * == new in XSI
 * ufoEditorDrawSubImage 
 *	  (XSI) Draw part of a raster on the viewer.  
 *
 *	  -	May	be called within ufoEditorMouseDown(), ufoEditorMouseUp(), 
 *		ufoEditorMouseDrag(), ufoEditorMousePosition(),	ufoEditorKeyEvent(), 
 *		or from	within any event function in the editor	GUI.
 *     set all four ROI coordinates to -1 to draw the entier image
 ******************************************************************************/
UFOAPI
void ufoEditorDrawSubImage(
	ufoEditor		editor_instance,
	double			x_pos,
	double			y_pos,
	int				x1Roi,		int		y1Roi, 
	int				x2Roi,		int		y2Roi,
	ufoRaster		raster_instance
	);


/*******************************************************************************
 * ufoEditorNotifyEdited():
 *	  Notify application of	changes	to parameters as a result of some edit.
 * 
 *	  For interactive changes, e.g.	when dragging sliders, thumbwheels or
 *	  dragging on-screen tools,	pass ufoEditorDrag as the ufo_event_type.
 *	  For release changes,	e.g. on	release	of sliders,	thumbwheels	or on-screen
 *	  tools	pass ufoEditorRelease as the ufo_event_type.
 * 
 *	  -	May	be called within ufoEditorOverlayDraw, ufoEditorRGBDraw,
 *		ufoEditorMouseDown,	ufoEditorMouseUp, ufoEditorMouseDrag,
 *		ufoEditorMousePosition and ufoEditorKeyEvent.
 *
 ******************************************************************************/
UFOAPI
void ufoEditorNotifyEdited(
	ufoEditor				editor_instance,
	 
	int*					param_index_array,
	int						number_of_parameters, 
	ufoEditorEventType		ufo_event_type);


/*******************************************************************************
 * ufoEditorGetRasterViewed():
 *	  Get raster handle	for	currently displayed	raster.
 *	  See ufoRaster.h for all utility functions	which can be called	with
 *	  ufoRaster	handles.
 *
 *	  All raster handles returned should be	tested against NULL, as	under certain
 *	  circumstances	such as	prior to processing	or getting from	unconnected	process	
 *	  inputs, the raster may not be	available. If a	returned handle	is NULL	then
 *	  there	is no raster available to get.
 * 
 *	  -	May	be called within ufoEditorMouseDown(), ufoEditorMouseUp(), 
 *		ufoEditorMouseDrag(), ufoEditorMousePosition(),	ufoEditorKeyEvent(), 
 *		or from	within any event function in the editor	GUI.
 ******************************************************************************/
UFOAPI
ufoRaster ufoEditorGetRasterViewed(
	ufoEditor		editor_instance);

/*******************************************************************************
 * ufoEditorForceRender():
 *	  Force	the	process	to render from within a	UFO	Editor.	
 *
 *	  -	May	be called within ufoEditorMouseDown(), ufoEditorMouseUp(), 
 *		ufoEditorMouseDrag(), ufoEditorMousePosition(),	ufoEditorKeyEvent(), 
 *		or from	within any event function in the editor	GUI. 
 ******************************************************************************/
UFOAPI
void ufoEditorForceRender(
	ufoEditor		editor_instance);

/*******************************************************************************
 * ufoEditorGetCurrentViewType():
 *	  Inquire what view	type is	currently set in the editor	(Input,	Output or Mask)
 *	  and the view number (eg input	0 or input 1).
 * 
 *	  Returns 1	if current view	is valid, 0	if invalid.
 * 
 *	  ufoViewType is defined in	ufoTypes.h and is one of:
 *		ufoInput, ufoOutput	or ufoMask
 *
 *	  -	May	be called within any editor	user function.
 ******************************************************************************/
UFOAPI
int	ufoEditorGetCurrentViewType(
	ufoEditor		editor_instance, 
	ufoViewType		*io_type, 
	int				*io_num);
	
/*******************************************************************************
 * ufoEditorForceUpdateUI():
 * Force the keybars interface to update to	represent current parameter
 * visibility status set with ufoProcessHideParam or ufoProcessShowParam.
 ******************************************************************************/
UFOAPI
void NOTIMPL_ufoEditorForceUpdateUI(
	ufoEditor		editor_instance);	 
	
/*******************************************************************************
 * ufoEditorSetCurrentFrame():
 * Change the current frame	- as if	the	slider had been	moved...
 * event_type is as	in ufoEditorNotifyEdited. Set event_type to	ufoEditorForce 
 * to force	a reprocess	even if	the	user has set Illusion to render	only on	
 * pressing	process.
 ******************************************************************************/
UFOAPI
void ufoEditorSetCurrentFrame(
	ufoEditor			editor_instance,
	double				frame,
	ufoEditorEventType	event_type);
 
/*******************************************************************************
 * ufoEditorSetStatusBar():
 * Allow editor	to put a message on	the	main status	bar. Can pass in NULL to clear
 * the status bar.
 ******************************************************************************/
UFOAPI
void ufoEditorSetStatusBar(ufoEditor	editor_instance, const char *msg);



#ifdef __cplusplus
}
#endif

#endif


/*******************************************************************************
 * End of file ufoEditor.h
 ******************************************************************************/

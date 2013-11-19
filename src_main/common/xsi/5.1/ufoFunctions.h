/*******************************************************************************
 * "ufoFunctions.h"
 * 
 * 
 * Copyright (c) 3 Space Software Ltd. 1994, 1995.
 *               Avid Technology Inc.  1996.
 * 
 ******************************************************************************/


#ifndef __ufoFunctions
#define __ufoFunctions


/*******************************************************************************
 * ufoFunctions.h:
 *
 *    This contains the declarations for all possible user supplied functions.
 *    Each function declaration is split onto 3 lines:
 *
 *    1. A string definition of the user function symbol which is searched
 *       for in the ufo process DSO.
 *    2. A function pointer typedef for the user function.
 *    3. The user function declaration. The ufo process code should use this
 *       to check the typing of all supplied user function definitions.
 *
 *    1 and 2 are only used internally by the host software.
 *
 * Function declarations:
 *
 * ufoProcessDefine():                  definition function
 * ufoProcessCopyUserData():            user data copy function
 * ufoProcessDeleteUserData():          user data delete function
 * ufoProcessPreSequenceRender():       pre render sequence function
 * ufoProcessPostSequenceRender():      post render sequence function
 * ufoProcessCanAvoidRendering():       pre frame render function to determine
 *                                      if processing can be skipped
 *
 * ufoProcessPreRender():               pre render function
 * ufoProcessPostRender():              post render function
 * ufoProcessPreRenderPass():           pre render pass function
 * ufoProcessPostRenderPass():          post render pass function
 * ufoProcessRenderPixel():             pixel render function
 * ufoProcessRenderLine():              line render function
 * ufoProcessRenderRectangle():         rectangle render function
 * ufoProcessCalcDefinedRectangle():    calculate rectangle which can be defined
 *                                      on output(s)
 * ufoProcessCalcNeedRectangles():      calculate rectangles which are required
 *                                      on inputs
 * ufoProcessCalcDefinedPixelType():    calculate the "best" pixel type given the supplied 
 *                                      input pixel types
 * ufoProcessSpecifyConvertPixelTypes():given pixel types of inputs and outputs supplied to UFO
 *                                      specify what types they need to be converted to for rendering
 *
 * ufoProcessWriteAsciiData():          write user ascii data string to process
 * ufoProcessReadAsciiData():           read user ascii data string from process
 * ufoProcessWriteBinaryData():         write user binary data string to process
 * ufoProcessReadBinaryData():          read user binary data string from process
 *
 * ufoProcessParamsEdited():            callback routine on parameter edits. 
 *                                      can set other params to be changed too.
 * ufoProcessGroupReset():              callback routine on group or whole process reset
 *
 * Editor Graphical User Interface(GUI) and Screen Interaction Functions
 *
 * ufoEditorCreate():                   (NOT IMPLEMENTED IN XSI )  create GUI & any user data for
 *                                      (NOT IMPLEMENTED IN XSI )  process  editor
 * ufoEditorDelete():                   (NOT IMPLEMENTED IN XSI )  destroy  GUI & any user data for
 *                                      (NOT IMPLEMENTED IN XSI )  process  editor
 *
 * ufoEditorShowGui():                  (NOT IMPLEMENTED IN XSI )  show GUI (i.e. manage pop-ups etc)
 * ufoEditorHideGui():                  (NOT IMPLEMENTED IN XSI )  hide GUI (i.e. unmanage  pop-ups etc)
 * ufoEditorUpdateGui():                (NOT IMPLEMENTED IN XSI )  update GUI to show parameter values
 *
 * ufoEditorBeginViewerEditing():       (XSI) called when a process is being edited in a viewer,
 *                                      redraw and mouse callbacks can be expected to be called.
 *                                      A processor could be edited in multiple viewers at once
 * ufoEditorEndViewerEditing():         (XSI) called when a process is no longer being edited in a viewer
 *
 * ufoEditorRGBDraw():                  RGB Draw into screen
 * ufoEditorOverlayDraw():              overlay Draw into screen
 * ufoEditorMouseDown():                receive mouse down event on screen
 * ufoEditorMouseUp():                  receive mouse up event on screen
 * ufoEditorMouseDrag():                receive mouse drag event on screen
 * ufoEditorMousePosition():            receive mouse move event on screen
 *                                      (no buttons down)
 * ufoEditorKeyEvent():                 receive key event on screen
 *
 * ufoEditorSetParamGroup():            set/change parameter group to be
 *                                      drawn/displayed in GUI
 *
 *
 * The only functions which MUST be supplied are ufoProcessDefine() and
 * at least one of ufoProcessRenderPixel(), ufoProcessRenderLine() and
 * ufoProcessRenderRectangle(). Not defining these will result in an error
 * and the ufo process dso will not be loaded.
 *
 * To have UFO screen interaction, then at least ufoEditorCreate() and one of 
 * ufoEditorRGBDraw(), ufoEditorOverlayDraw(), ufoEditorMouseDown(), 
 * ufoEditorMouseUp(), ufoEditorMouseDrag(), ufoEditorMousePosition(), and 
 * ufoEditorKeyEvent() must be supplied.
 *
 * To have your own X Motif GUI, then ufoEditorCreate() and
 * ufoEditorUpdateGui() must be supplied.
 ******************************************************************************/

#include <stdlib.h>
#include "ufoProcess.h"
#include "ufoEditor.h"

#ifdef __cplusplus
extern "C" {
#endif


/*******************************************************************************
 * ufoProcessDefine():
 *    This function must be supplied to provide the definition of a ufo process
 *    The process instance handle returned from the ufoProcessCreate() function
 *    call should be returned from this function.
 * 
 *    This gets called only once, immediately after the UFO Dynamic Shared
 *    Object (DSO) is loaded.
 ******************************************************************************/
#define ufoProcessDefineStr "ufoProcessDefine"
typedef ufoProcess (*ufoProcessDefineDef) (void);

UFOEXPORT
ufoProcess ufoProcessDefine(void);


/*******************************************************************************
 * ufoProcessCopyUserData():
 *    This function should be supplied if user data is setup in a ufo process at
 *    any time with the ufoProcessSetUserData() function. The function should
 *    return the pointer to "copied" user data.
 * 
 *    This gets called whenever an instance of a UFO process is copied. 
 ******************************************************************************/
#define ufoProcessCopyUserDataStr "ufoProcessCopyUserData"
typedef void* (*ufoProcessCopyUserDataDef) (ufoProcess,  void*);

UFOEXPORT
void* ufoProcessCopyUserData(
                ufoProcess  process_instance,
                void        *user_data);


/*******************************************************************************
 * ufoProcessDeleteUserData():
 *    This function should be supplied if user data is setup in a ufo process at
 *    any time with the ufoProcessSetUserData() function. This function allows
 *    the UFO to delete the user data associated with a process instance
 *    that is being deleted
 * 
 *    This gets whenever an instance of a UFO process is deleted. 
 ******************************************************************************/
#define ufoProcessDeleteUserDataStr "ufoProcessDeleteUserData"
typedef void (*ufoProcessDeleteUserDataDef) (ufoProcess,  void*);
UFOEXPORT
void ufoProcessDeleteUserData(
                ufoProcess  process_instance,
                void        *user_data);


/*******************************************************************************
 * ufoProcessPreSequenceRender():
 *    This function is optional.
 *    If supplied, it will be called before the render of a sequence of frames.
 *    This is particularly of use if any specific setup or allocation of data
 *    needs to be performed before a sequence of renders.
 ******************************************************************************/
#define ufoProcessPreSequenceRenderStr "ufoProcessPreSequenceRender"
typedef void (*ufoProcessPreSequenceRenderDef) (ufoProcess);
UFOEXPORT
void ufoProcessPreSequenceRender(
                ufoProcess  process_instance);


/*******************************************************************************
 * ufoProcessPostSequenceRender():
 *    This function is optional.
 *    If supplied, it will be called after the render of a sequence of frames.
 *    This is particularly of use if any specific de-allocation of data needs
 *    to be performed after a sequence of renders.
 ******************************************************************************/
#define ufoProcessPostSequenceRenderStr "ufoProcessPostSequenceRender"
typedef void (*ufoProcessPostSequenceRenderDef) (ufoProcess);
UFOEXPORT
void ufoProcessPostSequenceRender(
                ufoProcess  process_instance);


/*******************************************************************************
 * ufoProcessPreRender():
 *    This function is optional.
 *    If supplied, it will be called every time directly before the rendering
 *    of any rectangular region (of the UFO process outputs). At this point all
 *    raster inputs, outputs,  masks and parameters will be set up in the
 *    process such that all of their attributes can be inquired.
 * 
 *    This is particularly of use if specific setups or allocation of data needs
 *    to be performed before a render.
 ******************************************************************************/
#define ufoProcessPreRenderStr "ufoProcessPreRender"
typedef void (*ufoProcessPreRenderDef) (ufoProcess, int, int, int, int);
UFOEXPORT
void ufoProcessPreRender(
                ufoProcess  process_instance,
                int         x1,
                int         y1,
                int         x2,
                int         y2);


/*******************************************************************************
 * ufoProcessPostRender():
 *    This function is optional.
 *    If supplied, it will be called every time directly after the rendering
 *    any rectangular region (of the UFO process outputs).
 * 
 *    This is particularly of use if specific de-allocation of data needs to be
 *    performed after a render.
 ******************************************************************************/
#define ufoProcessPostRenderStr "ufoProcessPostRender"
typedef void (*ufoProcessPostRenderDef) (ufoProcess);
UFOEXPORT
void ufoProcessPostRender(
                ufoProcess  process_instance);


/*******************************************************************************
 * ufoProcessPreRenderPass():
 *    This function is optional.
 *    If supplied, it will be called every time directly before each pass of
 *    rendering of any rectangular region of the UFO process outputs.
 * 
 *    This is particularly of use if specific setups or allocations of data need
 *    to be performed before a particular rendering pass, where multiple
 *    rendering passes are required (see ufoProcessSetNumRenderPasses()).
 ******************************************************************************/
#define ufoProcessPreRenderPassStr "ufoProcessPreRenderPass"
typedef void (*ufoProcessPreRenderPassDef) (ufoProcess, int );
UFOEXPORT
void ufoProcessPreRenderPass(
                ufoProcess  process_instance,
                int         pass_number);


/*******************************************************************************
 * ufoProcessPostRenderPass():
 *    This function is optional.
 *    If supplied, it will be called every time directly after each pass of
 *    rendering of any rectangular region of the UFO process outputs.
 * 
 *    This is particularly of use if specific de-allocation of data needs to be
 *    performed after a particular rendering pass, where multiple rendering
 *    passes are required (see ufoProcessSetNumRenderPasses()).
 ******************************************************************************/
#define ufoProcessPostRenderPassStr "ufoProcessPostRenderPass"
typedef void (*ufoProcessPostRenderPassDef) (ufoProcess, int );
UFOEXPORT
void ufoProcessPostRenderPass(
                ufoProcess  process_instance,
                int         pass_number);



/*******************************************************************************
 * ufoProcessRenderPixel():
 * ufoProcessRenderLine():
 * ufoProcessRenderRectangle():
 *    At least one of these functions should be supplied.
 *    If supplied, it will be called every pixel, line or rectangle,
 *    resepctively within a single render pass.
 ******************************************************************************/

#define ufoProcessRenderPixelStr "ufoProcessRenderPixel"
typedef void (*ufoProcessRenderPixelDef) (ufoProcess, int, int);
UFOEXPORT
void ufoProcessRenderPixel(
                ufoProcess  process_instance,
                int         x,
                int         y);

#define ufoProcessRenderLineStr "ufoProcessRenderLine"
typedef void (*ufoProcessRenderLineDef) (ufoProcess, int, int, int);
UFOEXPORT
void ufoProcessRenderLine(
                ufoProcess  process_instance,
                int         x1,
                int         x2,
                int         y);

#define ufoProcessRenderRectangleStr "ufoProcessRenderRectangle"
typedef void (*ufoProcessRenderRectangleDef) (ufoProcess, int, int, int, int);
UFOEXPORT
void ufoProcessRenderRectangle(
                ufoProcess  process_instance,
                int         x1,
                int         y1,
                int         x2,
                int         y2);


/*******************************************************************************
 * ufoProcessCalcDefinedRectangle():
 *    If supplied, it will be called before the rendering of the process at a
 *    frame. Given the "defined" rectangles of the input rasters to be supplied,
 *    the function should calculate the "defined" rectangle which is the
 *    sensible maximum sized area of output to which it can render.
 * 
 *    If not supplied, the output "defined" rectangle defaults to the same
 *    size as the first input.
 *
 * ufoProcessCalcNeedRectangles():
 *    If supplied, it will be called before the rendering of the process at a
 *    frame. Given the output rectangle to be rendered, the function should
 *    calculate the required "need" rectangles which are the sensible maximum
 *    sized areas of input which are required in order to properly render to
 *    that output rectangle.
 *
 *    The supplied needed_input_rectangle array is pre-set to  maximum "defined"
 *    rectangle as provided in the ufoProcessCalcDefinedRectangle() function.
 *    (This is useful as if you cannot determine a finite "needed region",  you
 *    can clip to or leave it as it is). If not supplied, the input "need"
 *    rectangles default to the same as the output.
 *
 *    These functions are very important if you want the UFO process to behave
 *    properly when there is any cropping (automatically or manually) occurring
 *    during rendering.
 *    
 *    NB: The input and output rectangles are not scaled by the working render
 *    resolution scaling (as returned by ufoProcessGetRenderScale()) and the
 *    returned rectangles should not be scaled either.
 ******************************************************************************/

#define ufoProcessCalcDefinedRectangleStr "ufoProcessCalcDefinedRectangle"
typedef void (*ufoProcessCalcDefinedRectangleDef) (ufoProcess, ufoRectangle*, ufoRectangle*);
UFOEXPORT
void ufoProcessCalcDefinedRectangle(
                ufoProcess      process_instance,
                ufoRectangle*   input_rectangles,           /*supplied array of input rectangles - to UFO*/               
                ufoRectangle*   defined_output_rectangle);  /*returned rectangle - from UFO*/   
                
#define ufoProcessCalcNeedRectanglesStr "ufoProcessCalcNeedRectangles"
typedef void (*ufoProcessCalcNeedRectanglesDef) (ufoProcess, ufoRectangle*, ufoRectangle*);
UFOEXPORT
void ufoProcessCalcNeedRectangles(
                ufoProcess      process_instance,
                ufoRectangle*   output_rectangle,
                ufoRectangle*   needed_input_rectangle);


/*******************************************************************************
 * ufoProcessCalcDefinedPixelType():
 *    If supplied, it will be called before the rendering of the process at a
 *    frame. Given the pixel types of the input rasters to be supplied,
 *    the function should calculate the best "defined" output pixel type 
 *    of output to which it can render.
 *    This function is also called periodically during tree editing in order to
 *    colour code the input and output connection lines of the tree line UI. 
 *    In this case there may be inputs which are
 *    not connected. If this happens then the corresponding input_pixel_types element
 *    will be set to ufoUndefinedPixelType. 
 *
 *    If an input is optional, then you can test for whether it is selected using 
 *    ufoProcessIsRasterInDefined().
 * 
 *    If not supplied, the output "defined" pixel type is calculated using the
 *    an appplication default method.
 *
 *    This function should be supplied where the "defined" output pixel type 
 *    varies depending on the state of some parameters in the process.
 *
 * ufoProcessSpecifyConvertPixelTypes():
 *    If supplied, it will be called before the rendering of the process at a
 *    frame. 
 *    The pixel types of the rasters that will be supplied to the UFO process inputs and output
 *    are passed in the input_convert_pixel_types array and output_convert_pixel_type
 *    variable, resepctively. If you would like the application to pre-convert
 *    the input rasters or supply a different type output to be converted after rendering
 *    then change their values.
 *
 *    NB. This function is a far better alternative to the ufoProcessSetPixelTypeCombinations()
 *    function if ufoProcess.h, which essentially tries to do the same thing.
 *
 ******************************************************************************/


#define ufoProcessCalcDefinedPixelTypeStr "ufoProcessCalcDefinedPixelType"
typedef void (*ufoProcessCalcDefinedPixelTypeDef) (ufoProcess, ufoPixelType*, ufoPixelType*);
UFOEXPORT
void ufoProcessCalcDefinedPixelType(
                ufoProcess      process_instance,
                ufoPixelType*   input_pixel_types,          /*supplied array of input pixel types - to UFO*/               
                ufoPixelType*   best_output_pixel_type);    /*returned output pixel type - from UFO*/   
                
#define ufoProcessSpecifyConvertPixelTypesStr "ufoProcessSpecifyConvertPixelTypes"
typedef void (*ufoProcessSpecifyConvertPixelTypesDef) (ufoProcess, ufoPixelType*, ufoPixelType*);
UFOEXPORT
void ufoProcessSpecifyConvertPixelTypes(
                ufoProcess      process_instance,
                ufoPixelType*   input_convert_pixel_types,      /* supplied/returned array of input pixel types - to/from UFO*/               
                ufoPixelType*   output_convert_pixel_type);     /* supplied/returned output pixel type - to/from UFO*/   



/*******************************************************************************
 * ufoProcessCanAvoidRendering():
 *    If supplied, it will be called before the rendering any rectangular region
 *    (of the ufo process outputs). When called, all parameters will be set up
 *    and therefore can be used to determine whether the process will have any
 *    effect. If processing can be avoided then the function takes a parameter to 
 *    be set to the input index which the UFO has no effect on.
 *
 *    The return value is 1: has effect, 0: has no effect.
 ******************************************************************************/
#define ufoProcessCanAvoidRenderingStr "ufoProcessCanAvoidRendering"
typedef int (*ufoProcessCanAvoidRenderingDef) (ufoProcess, int *);
UFOEXPORT
int ufoProcessCanAvoidRendering(
                ufoProcess      process_instance,
                int             *no_effect_input_index);


/*******************************************************************************
 * ufoProcessWriteAsciiData():
 * ufoProcessReadAsciiData():
 *
 *  These functions can be used to store a user-defined ascii
 *  data string within the UFO process. This can be used as a multi-purpose
 *  data store,  which will be saved with and read from the UFO process
 *  by the application.
 *
 *  The stored data string must contain no quotes and be NULL terminated.
 *
 *  If supplied,  ufoProcessReadAsciiData() will be called when the UFO process
 *  definition is read into the application, only if the definition contains a
 *  user-defined ascii data string. The string is passed in ascii_data_string, 
 *  which should be copied if required to be used outside of this function.
 *  
 *  If supplied, ufoProcessWriteAsciiData() will be called when a process
 *  definition is about to be saved by the application. It must return the ascii
 *  data string which is required to be saved within the process definition.
 *  If no user ascii data is required to be saved,  the function should return
 *  NULL.
 ******************************************************************************/
#define ufoProcessReadAsciiDataStr "ufoProcessReadAsciiData"
typedef void (*ufoProcessReadAsciiDataDef) (ufoProcess, char*);
UFOEXPORT
void ufoProcessReadAsciiData(
    ufoProcess      process_instance, 
    char            *ascii_data_string);

#define ufoProcessWriteAsciiDataStr "ufoProcessWriteAsciiData"
typedef char* (*ufoProcessWriteAsciiDataDef) (ufoProcess);
UFOEXPORT
char* ufoProcessWriteAsciiData(
    ufoProcess      process_instance);
    
/*******************************************************************************
 * ufoProcessWriteBinaryData():
 * ufoProcessReadBinaryData():
 *
 *  These functions can be used to save and restore user-defined binary
 *  data within the UFO process. The binary data will be saved with and 
 *  read from the UFO process by the application.
 *
 *  The stored data string must contain no quotes and be NULL terminated.
 *
 *  If supplied,  ufoProcessReadBinaryData() will be called when the UFO process
 *  definition is read into the application, only if the definition contains
 *  user-defined binary data already. The data is data_length bytes LONG and is 
 *  passed in data_buffer, and should be copied if required to be used 
 *  outside of this function.
 *  
 *  If supplied, ufoProcessWriteBinaryDataDef() will be called when a process
 *  definition is about to be saved by the application. The data is specified by setting
 *  the data_buffer and data_length arguments. Also the function must say whether the
 *  application should free up the data buffer immediately after use with the free_data_buffer
 *  flag (1 = application frees buffer, using free, 0 = plugin is responsible for freeing buffer)
 *  If no user data is required to be saved, the function should set the data_buffer pointer to NULL
 *  NULL.
 ******************************************************************************/
#define ufoProcessReadBinaryDataStr "ufoProcessReadBinaryData"
typedef void (*ufoProcessReadBinaryDataDef) (ufoProcess, size_t, void*);
UFOEXPORT
void ufoProcessReadBinaryData(
    ufoProcess      process_instance,
    size_t          data_length,
    void            *data_buffer);

#define ufoProcessWriteBinaryDataStr "ufoProcessWriteBinaryData"
typedef void (*ufoProcessWriteBinaryDataDef) (ufoProcess, size_t *, void **, int *);
UFOEXPORT
void ufoProcessWriteBinaryData(
    ufoProcess      process_instance,
    size_t          *data_length,
    void            **data_buffer,
    int             *free_data_buffer);
    

/*******************************************************************************
 * ufoProcessParamsEdited():
 *
 *  This function is called whenever any parameters is edited by the user and 
 *  supplys the list of parameter indeces that have been changed. The user can then 
 *  respond to the edit by changing additional parameter values. The list of
 *  additional parameters changed should be returned, so that undo and redo can be 
 *  performed and other application GUIs can be properly updated.
 *
 *  This function is useful, for example, for controlling presets on a set of
 *  parameters by a ufoEnumParam parameter.
 *
 *  This function is called on changes to parameters from any application supplied
 *  parameter editor, and also from ufo custom editors 
 *  as informed by the ufoEditorNotifyEdited() library function.
 *  The UFO library does not have access to the previous values of parameters, so 
 *  it is up to the user to keep a record of those as required.
 *  NB this function is NOT called on undo,redo,reset and load operations. 
 *  
 ******************************************************************************/
#define ufoProcessParamsEditedStr "ufoProcessParamsEdited"
typedef void (*ufoProcessParamsEditedDef) (ufoProcess, int, int *, int *, int *);
UFOEXPORT
void ufoProcessParamsEdited(
    ufoProcess      process_instance,
    int             number_params_edited,
    int             *param_index_array,
    int             *number_additional_changed_params,
    int             *additional_changed_param_index_array);
    


/*******************************************************************************
 * ufoProcessGroupReset():
 *
 *  This function is called whenever a UFO parameter group or all parameter
 *  groups are explictly reset. This gives the user the chance to clean up any
 *  relating private data, or change the values to which the parameters have
 *  been reset depending on some private data.
 *
 *  If the all_groups_reset argument is 0, then the single group reset will be
 *  passed in the which_group argument.
 *
 *
 ******************************************************************************/
#define ufoProcessGroupResetStr "ufoProcessGroupReset"
typedef void (*ufoProcessGroupResetDef) (ufoProcess, int, int);
UFOEXPORT
void ufoProcessGroupReset(
    ufoProcess      process_instance,
    int             all_groups_reset,
    int             which_group);
    

/*******************************************************************************
 * ufoEditorCreate():
 *    This will be called the first time the UFO process is edited.
 *    When called, any GUI (Graphical User Interface) for the UFO process
 *    interface should be built and its base window must be returned. 
 * 
 *    The window system uses X Motif, so the (void*) window type can be
 *    to be cast to the X widget type: Widget. If no GUI, but only screen
 *    interaction is required then the function should return NULL.
 * 
 *    The supplied screen_interact_groups array is initialised to 1 for each
 *    parameter group. If you do not want to provide screen interaction for a
 *    particular parameter group, then the respective element for the group in
 *    the array should be set to 0.
 * 
 *    The supplied gui_groups array is initialised to 1 for each parameter
 *    group. If you do not want to provide the GUI for a particular parameter
 *    group, then the respective element for the group in the array should be
 *    set to 0.
 *****************************************************************************/

#define ufoEditorCreateStr "ufoEditorCreate"
typedef void* (*NOTIMPL_ufoEditorCreateDef) (ufoEditor, void*, int*, int*);
UFOEXPORT
void* NOTIMPL_ufoEditorCreate(
                ufoEditor       editor_instance,
                void*           parent_window,
                int*            screen_interact_groups,     /*array of parameter groups the screen interaction handles*/ 
                int*            gui_groups);                /*array of parameter groups the GUI handles*/


/*******************************************************************************
 * ufoEditorDelete():
 *    If supplied, it will be called whenever it is required to free up the
 *    memory associated with the supplied UFO editor and any GUI.
 ******************************************************************************/
#define ufoEditorDeleteStr "ufoEditorDelete"
typedef void (*NOTIMPL_ufoEditorDeleteDef) (ufoEditor);
UFOEXPORT
void NOTIMPL_ufoEditorDelete(
                ufoEditor       editor_instance);


/*******************************************************************************
 * ufoEditorShowGui():
 *    If supplied, it will be called whenever it is required to show the GUI.
 *    You need to supply this function if your GUI contains pop-up dialogs.
 ******************************************************************************/
#define ufoEditorShowGuiStr "ufoEditorShowGui"
typedef void (*NOTIMPL_ufoEditorShowGuiDef) (ufoEditor);
UFOEXPORT
void NOTIMPL_ufoEditorShowGui(
                ufoEditor       editor_instance);


/*******************************************************************************
 * ufoEditorHideGui():
 *    If supplied, it will be called whenever it is required to hide the GUI.
 *    You need to supply this function if your GUI contains popup dialogs.
 ******************************************************************************/
#define ufoEditorHideGuiStr "ufoEditorHideGui"
typedef void (*ufoEditorHideGuiDef) (ufoEditor);
UFOEXPORT
void NOTIMPL_ufoEditorHideGui(
                ufoEditor       editor_instance);


/*******************************************************************************
 * ufoEditorUpdateGui():
 *    If you supply a UFO GUI, then you should supply this function to ensure
 *    it represents up to date parameter values of the currently edited
 *    UFO process.
 * 
 *    For handling setting parameters from event callbacks etc., make a refernce
 *    of the process_instance supplied.
 ******************************************************************************/
#define ufoEditorUpdateGuiStr "ufoEditorUpdateGui"
typedef void (*ufoEditorUpdateGuiDef) (ufoEditor, ufoProcess);
UFOEXPORT
void NOTIMPL_ufoEditorUpdateGui(
                ufoEditor       editor_instance,
                ufoProcess      process_instance);


/*******************************************************************************
 * ufoEditorSetParamGroup():
 *    If supplied, it will be called whenever a UFO process is edited and
 *    whenever the the parameter group is changed.
 ******************************************************************************/
#define ufoEditorSetParamGroupStr "ufoEditorSetParamGroup"
typedef void (*ufoEditorSetParamGroupDef) (ufoEditor, ufoProcess, int);
UFOEXPORT
void ufoEditorSetParamGroup(
                ufoEditor       editor_instance,
                ufoProcess      process_instance, 
                int             parameter_group);


/*******************************************************************************
 * == new in XSI
 * ufoEditorBeginViewerEditing():
 *    (XSI) called when a process is being edited in a viewer,
 *    redraw and mouse callbacks can be expected to be called.
 *    A processor could be edited in multiple viewers at once. 
 ******************************************************************************/
#define ufoEditorBeginViewerEditingStr "ufoEditorBeginViewerEditing"
typedef void (*ufoEditorBeginViewerEditingDef) (ufoEditor, ufoProcess);
UFOEXPORT
void ufoEditorBeginViewerEditing(
                ufoEditor       editor_instance,
                ufoProcess      process_instance);
 
/*******************************************************************************
 * == new in XSI
 * ufoEditorEndViewerEditing():
 *    (XSI) called when a process is no longer being edited in a viewer
 ******************************************************************************/
#define ufoEditorEndViewerEditingStr "ufoEditorEndViewerEditing"
typedef void (*ufoEditorEndViewerEditingDef) (ufoEditor, ufoProcess);
UFOEXPORT
void ufoEditorEndViewerEditing(
                ufoEditor       editor_instance,
                ufoProcess      process_instance);


/*******************************************************************************
 * ufoEditorRGBDraw():
 *    If supplied,  any graphics drawing into the Open GL RGB Plane during
 *    editing should be performed within this function. OpenGL projections
 *    are set up for projection of the full/master raster pixel resolution
 *    coordinate space on the screen.
 ******************************************************************************/
#define ufoEditorRGBDrawStr "ufoEditorRGBDraw"
typedef void (*ufoEditorRGBDrawDef) (ufoEditor, ufoProcess);
UFOEXPORT
void ufoEditorRGBDraw(
                ufoEditor       editor_instance,
                ufoProcess      process_instance);


/*******************************************************************************
 * ufoEditorOverlayDraw():
 *    If supplied, any graphics drawing into the Open GL Overlay Plane during
 *    editing should be performed within this function. OpenGL projections
 *    are set up for projection of the full/master raster pixel resolution
 *    coordinate space on the screen.
 ******************************************************************************/
#define ufoEditorOverlayDrawStr "ufoEditorOverlayDraw"
typedef void (*ufoEditorOverlayDrawDef) (ufoEditor, ufoProcess);
UFOEXPORT
void ufoEditorOverlayDraw(
                ufoEditor       editor_instance,
                ufoProcess      process_instance);


/*******************************************************************************
 * ufoEditorMouseDown():
 *    If supplied, this function will be called whenever a mouse down event occurs
 *    within the viewer during editing (when a mouse button is pressed). 
 *    The supplied x,y position will be in full/master raster resolution pixel
 *    coordinates.
 ******************************************************************************/
#define ufoEditorMouseDownStr "ufoEditorMouseDown"
typedef void (*ufoEditorMouseDownDef) (ufoEditor, ufoProcess, double, double, ufoMouseEvent*);
UFOEXPORT
void ufoEditorMouseDown(
                ufoEditor       editor_instance,
                ufoProcess      process_instance, 
                double          x, 
                double          y, 
                ufoMouseEvent*  event);


/*******************************************************************************
 * ufoEditorMouseUp():
 *    If supplied, this function will be called whenever a mouse up event occurs
 *    within the viewer during editing (when a release of a mouse button).
 *    The supplied x,y position will be in full/master raster resolution pixel
 *    coordinates.
 ******************************************************************************/
#define ufoEditorMouseUpStr "ufoEditorMouseUp"
typedef void (*ufoEditorMouseUpDef) (ufoEditor, ufoProcess, double, double, ufoMouseEvent*);
UFOEXPORT
void ufoEditorMouseUp(
                ufoEditor       editor_instance,
                ufoProcess      process_instance, 
                double          x, 
                double          y, 
                ufoMouseEvent*  event);


/*******************************************************************************
 * ufoEditorMouseDrag():
 *    If supplied, this function will be called whenever a mouse drag event
 *    occurs within the viewer during editing (when a mouse move with mouse
 *    button pressed).
 *    The supplied x,y position will be in full/master raster resolution pixel
 *    coordinates.
 ******************************************************************************/
#define ufoEditorMouseDragStr "ufoEditorMouseDrag"
typedef void (*ufoEditorMouseDragDef) (ufoEditor, ufoProcess, double, double, ufoMouseEvent*);
UFOEXPORT
void ufoEditorMouseDrag(
                ufoEditor       editor_instance,
                ufoProcess      process_instance, 
                double          x, 
                double          y, 
                ufoMouseEvent*  event);


/*******************************************************************************
 * ufoEditorMousePosition():
 *    If supplied, this function will be called whenever a mouse move event
 *    occurs within the viewer during editing (when a mouse move with no mouse
 *    buttton pressed).
 *    The supplied x,y position will be in full/master raster resolution pixel
 *    coordinates.
 ******************************************************************************/
#define ufoEditorMousePositionStr "ufoEditorMousePosition"
typedef void (*ufoEditorMousePositionDef) (ufoEditor, ufoProcess, double, double, ufoMouseEvent*);
UFOEXPORT
void ufoEditorMousePosition(
                ufoEditor       editor_instance,
                ufoProcess      process_instance, 
                double          x, 
                double          y, 
                ufoMouseEvent*  event);


/*******************************************************************************
 * ufoEditorKeyEvent():
 * This function is optional. If supplied, it will be called whenever a key event
 * occurs during editing (when a key or combination of keys is pressed).
 ******************************************************************************/
#define ufoEditorKeyEventStr "ufoEditorKeyEvent"
typedef void (*ufoEditorKeyEventDef) (ufoEditor, ufoProcess, void*);
UFOEXPORT
void ufoEditorKeyEvent(
                ufoEditor       editor_instance,
                ufoProcess      process_instance, 
                void*           event);
                


#ifdef __cplusplus
}
#endif

#endif


/*******************************************************************************
 * End of file ufoFunctions.h
 ******************************************************************************/

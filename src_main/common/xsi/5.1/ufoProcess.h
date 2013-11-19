/*******************************************************************************
 * 
 * ufoProcess.h
 * 
 * Copyright (c) 3 Space Software Ltd. 1994, 1995.
 *               Avid Technology Inc.  1996.
 * 
 ******************************************************************************/


#ifndef __ufoProcess
#define __ufoProcess


/*******************************************************************************
 * ufoProcess.h:
 *
 * For all process instance handles.
 *
 * Function Declarations:
 *
 * ufoProcessCreate():                      Create the initial UFO process definition
 * ufoProcessSetEffectType():               Set type of effect (only undefined/transition so far)
 * ufoProcessSetHelpInfo():					Set the help file and ID.
 * ufoProcessSetPixelTypeCombinations():    Set supported input/output pixel combinations
 * ufoProcessSetUserData():                 Set user data associated with process instance
 * ufoProcessGetUserData():                 Get user data associated with process instance
 * ufoProcessSetMPSafe():                   Set multi-processing safe rendering status
 * ufoProcessSetTilingAllow():              Set tiled/sliced rendering-allowed status
 * ufoProcessSetNumberRenderPasses():       Set number of passes a render requires
 * ufoProcessGetRenderPass():               Get current render pass
 * ufoProcessSetRenderPassMode():           Set horizontal/vertical scan for render pass
 * ufoProcessRasterInDefine():              Define input raster to ufo process
 * ufoProcessSetRasterInOptional():         Set input raster optional 
 * ufoProcessSetRasterInOptionalDefault():  Set input raster default option
 * ufoProcessSetRasterInEdgeMode():         Set input raster edge clamping mode
 * ufoProcessRasterOutDefine():             Define output raster to ufo process
 * ufoProcessGetRasterIn():                 Get input raster handle
 * ufoProcessIsRasterInDirty():             Inquire if input is "dirty" - i.e. different to when last rendered
 * ufoProcessResetRasterInDirty():          reset "dirty" flag of input to "clean"
 * ufoProcessIsRasterInDefined():           Inquire if (optional) input raster has been connected and selected
 * ufoProcessGetObeyMatte():                Get obey matte raster handle
 * ufoProcessIsObeyMatteDirty():            Inquire if obey matte is "dirty" - i.e. different to when last rendered
 * ufoProcessResetObeyMatteDirty():         reset "dirty" flag of obey matte to "clean"
 * ufoProcessIsObeyMatteDefined():          Inquire if (optional) obey matte has been connected and selected
 * ufoProcessIsObeyMatteReversed():         Inquire if (optional) obey matte has been reversed
 * ufoProcessHandleObeyMatte():             used to define whether the ufo handles the obey matte protection
 * ufoProcessGetShapeListBoundingBox()      Calculates the bounding box of a shape list
 * ufoProcessRenderShapeList():             Renders a shape list to a raster
 * ufoProcessIsRasterOutDefined():          Inquire if (optional) output raster has been connected and selected
 * ufoProcessGetRasterOut():                Get output raster handle
 * ufoProcessGetRasterInAtFrame():          Get input raster handle at a specified frame 
 * ufoProcessReleaseRasterInAtFrame():      Release input raster handle at a specified frame (allows to be uncached)
 * ufoProcessIsRasterInTemporal():          Inquire if the input can be got temporally, ie if ufoProcessGetRasterInAtFrame can be called
 * ufoProcessGetRasterInTemporalLimits():   Inquire temporal frame limits of input
 * ufoProcessGetRenderMode():               Get current render mode (edit/full, single/sequence)
 * ufoProcessGetRenderScale():              Get X and Y scale of current render
 * ufoProcessGetDefinedRectangle():         Get maximum defined rectangle for supplied input image
 * ufoProcessParamDefine():                 Define parameter in ufo process
 * ufoProcessEnumParamDefine():             Define enumerated parameter in ufo process
 * ufoProcessParamGroupDefine():            Define parameter group in ufo process
 * ufoProcessSetParamColumnHint():          Give hint as to which column in the (auto-generated) UI the param should
 * ufoProcessSetParamGang():                Indicate that the next 'n_params' should be ganged together in the UI.
 * ufoProcessSetParamGangLabel():           Give label for set of params ganged together using ufoProcessSetParamGangHint
 * ufoProcessSetParamLimits():              Set parameter limits
 * ufoProcessSetParamAnimAllow():           Set parameter animation flag
 * ufoProcessSetParamAutoTransition():      Set parameter into "auto-transition" mode
 * ufoProcessSetParamDefaultValue():        Set parameter default value
 * ufoProcessSetParamDefaultValues():       Set parameter default values
 * ufoProcessSetStringParamDefaultValue():  Set string parameter default value
 * ufoProcessSetParamValue():               Set parameter value at current time
 * ufoProcessSetParamValues():              Set parameter values at current time
 * ufoProcessSetStringParamValue():         Set string parameter value at current time
 * ufoProcessSetParamDefaultKey():          Set parameter default key
 * ufoProcessSetParamDefaultKeys():         Set parameter default keys
 * ufoProcessSetStringParamDefaultKey():    Set string parameter default key
 * ufoProcessResetParam():                  Reset parameter to default value
 * ufoProcessGetParamValue():               Get parameter value at current time
 * ufoProcessGetParamValues():              Get parameter values at current time
 * ufoProcessGetStringParamValue():         Get string parameter value at current time
 * ufoProcessGetParamValueAtTime():         Get parameter value at specified time
 * ufoProcessGetParamValuesAtTime():        Get parameter values at specified time
 * ufoProcessGetStringParamValueAtTime():   Get string parameter values at specified time
 * ufoProcessSetParamKey():                 Set parameter key value at specified time
 * ufoProcessSetParamKeys():                Set parameter key values at specified time
 * ufoProcessSetStringParamKey():           Set string parameter key value at specified time
 * ufoProcessIsParamSet():                  Inquire if parameter has been set (from its default)
 * ufoProcessGetMin/MaxKeyTime():           Get minimum and maximum key frame time values for parameter channel
 * ufoProcessGetTime():                     Get current render frame time.
 * ufoProcessGetMin/MaxEffectLimit():       Get current start/end frame times of process effect limits.
 * ufoProcessGetFullRenderRectangle():      Get the default full render rectangle size (un-scaled and un-cropped)
 * ufoProcessGetAspectRatioCorrection():    Get aspect ratio correction to be applied during rendering
 * ufoProcessHideParam():                   Mark a particular parameter to be hidden in the application interface
 * ufoProcessShowParam():                   Mark a particular parameter to be visible in the application interface
 * 
 * ufoProcessFieldRendering():              Inquire the current sequence is being field rendered.
 * ufoProcessWhichField():                  Inquire the current field (in time).
 *
 * ufoProcessCheckAbort():                  Check for user termination of process
 *
 * ufoMalloc():                             Host memory allocation routines 
 * ufoRealloc():                            (uses host memory management)
 * ufoFree():
 *
 * ufoMPFork():                             Host multi-processing routines (obsolete - please use ufoMPIterate)
 * ufoMPForkMyId():                         (uses host multi-processing management)
 * ufoMPForkNumProcs():
 * ufoMPPotentialNumProcs():
 * ufoMPSetIterateFunction():               Host multi-processing routines (new)
 * ufoMPIterate():
 *
 * ufoCheckInBackground():                  Check if in foreground application or background application
 *
 * ufoCheckLicense();                       Check permission is set for UFO in license file
 *
 ******************************************************************************/

#include <stdlib.h>
#include <stdarg.h>
#include "ufoUnixCompat.h"
#include "ufoTypes.h"
#include "ufoRaster.h"


#ifdef __cplusplus
extern "C" {
#endif

typedef void * ufoProcess;

/*******************************************************************************
 * ufoProcessCreate():
 *    Creates the initial UFO process definition. All used instances of this
 *    process are copied from this one. Returns definition process
 *    instance handle to be used in subsequent function calls.
 * 
 *    - MUST be called within ufoProcessDefine() to get initial process
 *      instance handle.
 ******************************************************************************/
UFOAPI  
ufoProcess ufoProcessCreate(                    
    char            *name,                      /*user interface name*/    

    int             number_inputs,              /*number of image/mask/rgba inputs*/
    int             number_outputs,             /*number of image/mask/rgba outputs*/

    int             number_parameters,          /*number of parameters*/
    int             number_parameter_groups);   /*number of groups the parameters are divided into*/


/*******************************************************************************
 * ufoProcessSetEffectType():
 *    Sets type of effect group that the UFO belongs to. Currently this is only to inform
 *    whether the effect type is a transition or not, so the only two values are 
 *    ufoUndefinedEffect, or ufoTransitionEffect.
 *    (NB If you want parameter key frames to automatically scale in duration during certain
 *    invocations in the application then you should use ufoProcessSetParamAutoTransition()
 *    passing the specific parameter index). 
 *
 *    By default the process effect type is UfoUndefinedEffect.
 * 
 *    - May be called within ufoProcessDefine() 
 ******************************************************************************/
UFOAPI  
void ufoProcessSetEffectType(
    ufoProcess      process_instance,           /*process instance handle         */    
    
    ufoEffectType   effect_type);               /*effect type*/


/*******************************************************************************
 * ufoProcessSetHelpInfo():
 *    Sets the help file and index which will pop up if the help (?) button on the
 *    PPG is clicked.
 * 
 ******************************************************************************/
UFOAPI  
void ufoProcessSetHelpInfo(
	ufoProcess		process_instance,			/*process instance handle		  */	
	char *			pszFile,					/* help file name */
	unsigned int	helpID );					/* help index within file */


/*******************************************************************************
 * ufoProcessSetPixelTypeCombinations():
 *    Sets the combinations of input/output/mask pixel type combinations
 *    supported by the UFO process render functions. The mask_combinations
 *    argument should be set to NULL except when the UFO process can override
 *    the default Mask Obey mechanism.
 * 
 *    By default the process supports any pixel type on all inputs and outputs.
 * 
 *    - May be called within ufoProcessDefine()
 ******************************************************************************/
UFOAPI  
void ufoProcessSetPixelTypeCombinations(
    ufoProcess      process_instance,           /*process instance handle         */    
    
    int             number_combinations,        /*number of combinations supported*/
    ufoPixelType    *input_combinations,        /*2D array [number_inputs]X[number_combinations] */
    ufoPixelType    *output_combinations,       /*2D array [number_outputs]X[number_combinations]*/
    ufoPixelType    *mask_combinations);        /*1D array [number_combinations]                 */
                                                /*NB: set mask_combinations to NULL if process   */
                                                /*does not support its own mask obey.            */

/*******************************************************************************
 * ufoProcessSetMPSafe():
 *    Sets whether multiprocessing the render routines is safe (MP safe) or not.
 * 
 *    By default MPSafe is true, and on multiprocessor machines (such as SGI
 *    Onyx) UFO render routines will be called in parallel. If your render
 *    routine(s) are not MP safe then you should call this routine with mp_safe
 *    set to 0.
 *
 *    - May be called within ufoProcessDefine(), to set for all renders, or
 *      within ufoProcessPreRender() to set before each frame, or
 *      ufoProcessPreRenderPass() to set before each render pass.
 ******************************************************************************/
UFOAPI  
void ufoProcessSetMPSafe(
    ufoProcess      process_instance,           /*process instance handle*/    
    int             mp_safe);


/*******************************************************************************
 * ufoProcessSetTilingAllow():
 *    Sets whether Tiling or Slicing is allowed when rendering UFO.
 *      
 *    If tiling is not allowed then the rendering of a rectangle area will
 *    never be segmented into slices or tiles.
 * 
 *    If allowed, slicing is done on processes which are slow to allow polling
 *    of a user interrupt, and Tiling is optionally performed at the users
 *    request to subdivide a high resolution render into a set of lower
 *    resolution renders.
 * 
 *    By default TilingAllow is true.
 * 
 *    - May be called within ufoProcessDefine(), to set for all renders, or
 *      within ufoProcessPreRender() to set before each frame, or
 *      ufoProcessPreRenderPass() to set before each render pass.
 ******************************************************************************/
UFOAPI  
void ufoProcessSetTilingAllow(
    ufoProcess      process_instance,           /*process instance handle*/   
    int             tiling_allow);

    
/*******************************************************************************
 * ufoProcessSetUserData():
 *    Sets user data to be associated with this instance of the process.
 * 
 *    If this is used then the user should supply the ufoProcessCopyUserData()
 *    and ufoProcessDeleteUserData() functions to allow copying and deleting of
 *    user data in duplicate process instances and deletion of process
 *    instances.
 * 
 *    - May be called within any user function.
 ******************************************************************************/
UFOAPI  
void ufoProcessSetUserData(
    ufoProcess      process_instance,
    void            *user_data);


/*******************************************************************************
 * ufoProcessGetUserData():
 *    Gets user data associated with this instance of the process.
 * 
 *    - May be called within any user function.
 ******************************************************************************/
UFOAPI  
void* ufoProcessGetUserData(
    ufoProcess      process_instance);


/*******************************************************************************
 * ufoProcessSetNumberRenderPasses():
 *    Sets the number of required separate rendering passes of any area the
 *    process renders.
 * 
 *    - May be called within ufoProcessDefine() to set number of passes in all
 *      renders or within ufoProcessPreRender() to affect the subsequent render.
 ******************************************************************************/
UFOAPI  
void ufoProcessSetNumberRenderPasses(
    ufoProcess      process_instance,           /*process instance handle*/
    int             number_passes);             /*number of render passes required*/


/*******************************************************************************
 * ufoProcessSetRenderPassMode():
 *    Sets the rendering of a particular pass to be either horizontal (the
 *    default) or vertical. (There may be more options in the future).
 * 
 *    (NB: if a pass is vertical then the X and Y values passed to all UFO user
 *     functions are SWAPPED.)
 *    
 *    - May be called within ufoProcessDefine() to affect all renders or within 
 *      ufoProcessPreRender() to affect the subsequent render.
 ******************************************************************************/
UFOAPI  
void ufoProcessSetRenderPassMode(
    ufoProcess      process_instance,           /*process instance handle*/
       
    int             passes_number,              /*pass number (0..number_passes)*/
    ufoPassMode     pass_mode);                 /*ufoPassHorizontal (default), ufoPassVertical*/


/*******************************************************************************
 * ufoProcessGetRenderPass():
 *    Returns the current render pass number.
 * 
 *    - May be called within
 *      ufoProcessRenderPixel(), ufoProcessRenderLine(),
 *      ufoProcessRenderRectangle(), ufoProcessPreRenderPass(),  
 *      ufoProcessPostRenderPass().
 ******************************************************************************/
UFOAPI  
int ufoProcessGetRenderPass(                    
    ufoProcess      process_instance);          /*process instance handle*/


/*******************************************************************************
 * ufoProcessRasterInDefine():
 *    Define raster inputs of the ufo process (one of 0..number_inputs - as set
 *    in ufoProcessCreate()).
 * 
 *    - MUST be called within ufoProcessDefine() for each raster input.
 ******************************************************************************/
UFOAPI  
void ufoProcessRasterInDefine(
    ufoProcess      process_instance,                 /*process instance handle*/   
    
    int             input_index,                      /*index (0..number_inputs) of raster input*/
    char            *id,                              /*unique id of input (unique within process) - NO SPACES)*/
    char            *name,                            /*user interface title of input*/
    ufoCompComb     suggested_component_combination); /*suggested raster component cobination RGB/A/RGBA*/
    
    
/*******************************************************************************
 * ufoProcessSetRasterInOptional():
 *    Set input raster to be optional. (By default all raster inputs must be set in a process
 *    before it can render, ie NOT optional.)
 * 
 *    - May be called within ufoProcessDefine()
 ******************************************************************************/
UFOAPI  
void ufoProcessSetRasterInOptional(
    ufoProcess      process_instance,           /*process instance handle*/
    
    int             input_index,                /*index (0..number_inputs) of raster input*/
    int             optional);                  /*0: Not Optional (Default), 1: Optional*/
     
/*******************************************************************************
 * ufoProcessSetRasterInOptionalDefault():
 *    Set input raster default option selection. (By default optional inputs are not selected.)
 * 
 *    - May be called within ufoProcessDefine()
 ******************************************************************************/
UFOAPI  
void ufoProcessSetRasterInOptionalDefault(
    ufoProcess      process_instance,           /*process instance handle*/
    
    int             input_index,                /*index (0..number_inputs) of raster input*/
    int             option_select);             /*0: not selected (Default), 1: selected*/
     
/*******************************************************************************
 * ufoProcessSetRasterInEdgeMode():
 *    Set input edge mode. By default this is ufoEdgeClamp.
 * 
 *    If an input has its EdgeMode set to ufoEdgeClamp, then an ouput render
 *    will automatically be clamped to the size of the input. If it is set to
 *    ufoEdgeNoClamp, then the process should be able to render an area to the
 *    output(s) even though input does not cover it, in which case the process
 *    must perform its own limits checks on this input when rendering.
 * 
 *    - May be called within ufoProcessDefine().
 ******************************************************************************/
UFOAPI  
void ufoProcessSetRasterInEdgeMode(
    ufoProcess      process_instance,           /*process instance handle*/
    
    int             input_index,                /*index (0..number_inputs) of raster input*/
    ufoEdgeMode     edge_mode);                 /*ufoEdgeClamp (Default), ufoEdgeNoClamp*/
    

/*******************************************************************************
 * ufoProcessRasterOutDefine():
 *    Define raster  outputs of the UFO process.
 *    (one of 0..number_outputs - as set in ufoProcessCreate())
 * 
 *    - MUST be called within ufoProcessDefine() for each raster output.
 ******************************************************************************/
UFOAPI  
void ufoProcessRasterOutDefine(
    ufoProcess      process_instance,                   /*process instance handle*/
    
    int             output_index,                       /*index (0..number_outputs) of raster output*/
    char            *id,                                /*unique id of output (unique within process) - NO SPACES)*/
    char            *name,                              /*user interface title of output*/
    ufoCompComb     suggested_component_combination);   /*suggested raster component cobination RGB/A/RGBA*/
    
/*******************************************************************************
 * ufoProcessSetRasterOutOptional():
 *    ** NOT SUPPORTED **: Set output raster to be optional. (By default all raster outputs must be
 *    set in a process before it can render.)
 * 
 *    - May be called within ufoProcessDefine()
 ******************************************************************************/
UFOAPI  
void ufoProcessSetRasterOutOptional(
    ufoProcess      process_instance,           /*process instance handle*/
    
    int             output_index,               /*index (0..number_outputs) of raster output*/
    int             optional);                  /*0: Not Optional (Default) 1: Optional*/
     
         
/*******************************************************************************
 * ufoProcessIsRasterInDirty():
 *    Inquire if raster input is "dirty". A raster input is NOT dirty if it
 *    hasn't changed since the last render. This may occur during consequetive
 *    renders at the same frame during editing, or if there is a hold (or
 *    "freeze") on the input.
 * 
 *    - May be called within 
 *      ufoProcessRenderPixel(), ufoProcessRenderLine(),
 *      ufoProcessRenderRectangle(), ufoProcessHasEffect(), 
 *      ufoProcessPreRender(), ufoProcessPostRender(), 
 *      ufoProcessPreRenderPass(), ufoProcessPostRenderPass(), 
 *      ufoProcessCalcDefinedRectangle(), ufoProcessCalcNeedRectangles().
 ******************************************************************************/
UFOAPI  
int ufoProcessIsRasterInDirty(
    ufoProcess      process_instance,           /*process instance handle*/
    
    int             input_index);               /*index (0..number_inputs) of raster input*/


/*******************************************************************************
 * ufoProcessResetRasterInDirty():
 *    Set raster input to not "dirty". i.e. "clean". Use after inquiring if
 *    input is dirty and intermediary result has been cached.
 * 
 *    - May be called within 
 *      ufoProcessRenderPixel(), ufoProcessRenderLine(),
 *      ufoProcessRenderRectangle(), ufoProcessHasEffect(), 
 *      ufoProcessPreRender(), ufoProcessPostRender(), 
 *      ufoProcessPreRenderPass(), ufoProcessPostRenderPass(), 
 *      ufoProcessCalcDefinedRectangle(), ufoProcessCalcNeedRectangles().
 ******************************************************************************/
UFOAPI  
void ufoProcessResetRasterInDirty(
    ufoProcess      process_instance,           /*process instance handle*/
    int             input_index);               /*index (0..number_inputs) of raster input*/


/*******************************************************************************
 * ufoProcessIsRasterInDefined():
 *
 * ufoProcessIsRasterOutDefined(): ** NOT REQUIRED AS outputs cannot be optional **
 *
 *    Inquire if optional raster input is defined.
 *    (For those inputs set optional with ufoProcessSetRasterInOptional)
 * 
 *    - May be called within 
 *      ufoProcessRenderPixel(), ufoProcessRenderLine(),
 *      ufoProcessRenderRectangle(), ufoProcessHasEffect(), 
 *      ufoProcessPreRender(), ufoProcessPostRender(), 
 *      ufoProcessPreRenderPass(), ufoProcessPostRenderPass(), 
 *      ufoProcessCalcDefinedRectangle(), ufoProcessCalcNeedRectangles().
 ******************************************************************************/
UFOAPI  
int ufoProcessIsRasterInDefined(
    ufoProcess      process_instance,           /*process instance handle*/
    int             input_index);               /*index (0..number_inputs) of raster input*/

UFOAPI  
int ufoProcessIsRasterOutDefined(
    ufoProcess      process_instance,           /*process instance handle*/
    int             output_index);              /*index (0..number_outputs) of raster output*/


/*******************************************************************************
 * ufoProcessGetRasterIn():
 * ufoProcessGetRasterOut():
 * ufoProcessGetObeyMatte():
 *    Get raster handle for raster input, output or obey matte (respectively).
 *    See ufoRaster.h for all utility functions which can be called with
 *    ufoRaster handles.
 * 
 *    All raster handles returned should be tested against NULL, as under certain
 *    circumstances such as prior to processing or getting from unconnected process 
 *    inputs, the raster may not be available. If a returned handle is NULL then
 *    there is no raster available to get.
 * 
 *    - May be called within 
 *      ufoProcessRenderPixel(), ufoProcessRenderLine(),
 *      ufoProcessRenderRectangle(), ufoProcessPreRender(),
 *      ufoProcessPostRender(), ufoProcessPreRenderPass(),
 *      ufoProcessPostRenderPass().
 ******************************************************************************/
UFOAPI  
ufoRaster ufoProcessGetRasterIn(
    ufoProcess      process_instance,           /*process instance handle*/
    int             input_index);

UFOAPI  
ufoRaster ufoProcessGetRasterOut(
    ufoProcess      process_instance,           /*process instance handle*/
    int             output_index);

UFOAPI  
ufoRaster ufoProcessGetObeyMatte(
    ufoProcess      process_instance);          /*process instance handle*/


/*******************************************************************************
 * ufoProcessGetRasterInAtFrame():
 * ufoProcessReleaseRasterInAtFrame():
 *    Get raster handle for raster input at a specified frame and field.
 *    The field argument is only used when field rendering in which case it
 *    should be set to 1 or 2.
 *    See ufoRaster.h for all utility functions which can be called with
 *    ufoRaster handles.
 *
 *    These functions are here to allow for interframe effects like retiming and 
 *    other motion based effects.
 *
 *    If your effect is a motion base effect than if you can sequentially access
 *    your input rasters, using ufoProcessReleaseRasterInAtFrame() to release inputs
 *    when you are finished with them. Each call to ufoProcessGetRasterInAtFrame with a
 *    unique frame will lock a raster in memory by the host memory manager until 
 *    ufoProcessReleaseRasterInAtFrame() is subsequently called with that frame.
 *    This is obviously more important to do the more frames are being accessed 
 *    and the larger the image size.
 *    NB if you don't call ufoProcessReleaseRasterInAtFrame() then all accessed frame rasters
 *    will remain locked until after the frame has rendered.
 * 
 *    All raster handles returned should be tested against NULL, as under certain
 *    circumstances such as prior to processing or getting from unconnected process 
 *    inputs, the raster may not be available. If a returned handle is NULL then
 *    there is no raster available to get.
 *
 *    If you want to check that the input can be processed temporally then use the
 *    function ufoProcessIsRasterInTemporal() (see below).
 * 
 *    - May be called within 
 *      ufoProcessRenderPixel(), ufoProcessRenderLine(),
 *      ufoProcessRenderRectangle(), ufoProcessPreRender(),
 *      ufoProcessPostRender(), ufoProcessPreRenderPass(),
 *      ufoProcessPostRenderPass().
 ******************************************************************************/
UFOAPI  
ufoRaster ufoProcessGetRasterInAtFrame(
    ufoProcess      process_instance,           /*process instance handle*/
    int             input_index,
    int             frame,
    int             field);

UFOAPI  
void ufoProcessReleaseRasterInAtFrame(
    ufoProcess      process_instance,           /*process instance handle*/
    int             input_index,
    int             frame,
    int             field);
    

/*******************************************************************************
 * ufoProcessIsRasterInTemporal():
 * 
 *    Returns: 1:input can be accessed temporally using ufoProcessGetRasterInAtFrame()
 *             0:ufoProcessGetRasterInAtFrame() will just return 'current' frame as with'
 *               ufoProcessGetRasterIn().
 *
 * ufoProcessGetRasterInTemporalLimits():
 *    Return frame limits of temporal access. If ufoProcessIsRasterInTemporal() returns false
 *    then the limits will be the current frame only.
 *
 *    - May be called within 
 *      ufoProcessRenderPixel(), ufoProcessRenderLine(),
 *      ufoProcessRenderRectangle(), ufoProcessPreRender(),
 *      ufoProcessPostRender(), ufoProcessPreRenderPass(),
 *      ufoProcessPostRenderPass(),
 *      ufoProcessCalcDefinedRectangle(), ufoProcessCalcNeedRectangles(), 
 *      ufoProcessPreSequenceRender(), ufoProcessPostSequenceRender().
 ******************************************************************************/
UFOAPI  
int ufoProcessIsRasterInTemporal(
    ufoProcess      process_instance,
    int             input_index);               

UFOAPI  
void ufoProcessGetRasterInTemporalLimits(
    ufoProcess      process_instance,
    int             input_index,
    int             *first_frame,
    int             *last_frame);               




/*******************************************************************************
 * ufoProcessIsObeyMatteDirty():
 *
 *    Returns: 1:obey matte raster is dirty 0:not dirty
 *      
 *    Inquire if obey matte raster input is dirty. An input is not dirty if it
 *    hasn't chnaged since the last render and its dirty status was reset with
 *    ufoProcessResetObeyMatteDirty.
 *      
 *    - May be called within:
 *      ufoProcessRenderPixel, ufoProcessRenderLine, ufoProcessRenderRectangle,
 *      ufoProcessHasEffect, ufoProcessPreRender, ufoProcessPostRender,
 *      ufoProcessPreRenderPass, ufoProcessPostRenderPass.

 ******************************************************************************/
UFOAPI  
int ufoProcessIsObeyMatteDirty(
    ufoProcess      process_instance);          


/*******************************************************************************
 * ufoProcessResetObeyMatteDirty():
 * 
 *    Resets dirty status of obey matte raster.
 *      
 *    - May be called within:
 *      ufoProcessRenderPixel, ufoProcessRenderLine, ufoProcessRenderRectangle,
 *      ufoProcessHasEffect, ufoProcessPreRender, ufoProcessPostRender,
 *      ufoProcessPreRenderPass, ufoProcessPostRenderPass.
 ******************************************************************************/
UFOAPI  
void ufoProcessResetObeyMatteDirty(
    ufoProcess      process_instance);          


/*******************************************************************************
 * ufoProcessIsObeyMatteDefined():
 * 
 *     Inquire if optional obey matte raster is connected and set.
 *      
 *     - May be called within
 *       ufoProcessRenderPixel, ufoProcessRenderLine, ufoProcessRenderRectangle,
 *       ufoProcessHasEffect, ufoProcessPreRender, ufoProcessPostRender,
 *       ufoProcessPreRenderPass, ufoProcessPostRenderPass.
 ******************************************************************************/
UFOAPI  
int ufoProcessIsObeyMatteDefined(
    ufoProcess      process_instance);          


/*******************************************************************************
 * ufoProcessIsObeyMatteReversed():
 * 
 *    Inquire if optional obey matte is reversed.
 *      
 *    - May be called within:
 *      ufoProcessRenderPixel, ufoProcessRenderLine, ufoProcessRenderRectangle,
 *      ufoProcessHasEffect, ufoProcessPreRender, ufoProcessPostRender,
 *      ufoProcessPreRenderPass, ufoProcessPostRenderPass.      
 ******************************************************************************/
UFOAPI  
int ufoProcessIsObeyMatteReversed(
    ufoProcess      process_instance);          


/*******************************************************************************
 * ufoProcessHandleObeyMatte():
 * (replaces ufoProcessHandleOwnProtection() - which could not revert the feature)
 * 
 *    Set the UFO process to handle its own obey matte protection, bypassing
 *    Media Illusion's internal obey matte functionality.
 *      
 *    - May be called within:
 *      ufoProcessDefine
 *      ufoProcessPreRender, ufoProcessPostRender
 *      ufoProcessPreSequenceRender(), ufoProcessPostSequenceRender().
 ******************************************************************************/
UFOAPI  
void ufoProcessHandleObeyMatte(
    ufoProcess      process_instance,
    int             handle_obey_matte);         
UFOAPI  
void ufoProcessHandleOwnProtection(
    ufoProcess      process_instance);          


/*******************************************************************************
 * == new in XSI
 * ufoProcessGetShapeListBoundingBox():
 * 
 *    Returns 1 and the bounding box for the given shape list 
 *    or 0 if the shape list has no visible shapes
 *    Uses the render rectangle to handle inverted mask shape list or color shapes
 *    lists that contain flood fill commands, in which case the render result would
 *    be the entier render rectangle.
 *
 *    - May be called within any user function.
 ******************************************************************************/

UFOAPI  
int ufoProcessGetShapeListBoundingBox(
    ufoProcess      process_instance,           /* process instance handle */
    int             param_index,                /* index (0..number_parameters) of shape list parameter */
    int             *x1,                        /* calculcate bounding box */
    int             *y1,
    int             *x2,
    int             *y2,
    int             x1_render_rect,              /* maximum render rectangle */
    int             y1_render_rect,
    int             x2_render_rect,
    int             y2_render_rect,
    double          time );                     /* time at which animation is sampled */

/*******************************************************************************
 * == new in XSI
 * ufoProcessRenderShapeList():
 * 
 *    Renders the paint shape list in the given raster
 *    The raster must be allocated 
 *      
 *    - May be called within any user function.
 ******************************************************************************/

UFOAPI  
void ufoProcessRenderShapeList(
    ufoRaster       raster_instance,            /* destination raster */
    ufoProcess      process_instance,           /* process instance handle */
    int             param_index,                /* index (0..number_parameters) of shape list parameter */
    int             x1,                         /* region of interest */
    int             y1,
    int             x2,
    int             y2,
    double          time,                       /* time at which animation is sampled */
    double          x_render_scale,             /* x render scale to use */
    double          y_render_scale);            /* y render scale to use */


/*******************************************************************************
 * ufoProcessGetRenderScale():
 *    Gets the the x and y scale of the current render.
 * 
 *    - May be called within 
 *      ufoProcessRenderPixel(), ufoProcessRenderLine(),
 *      ufoProcessRenderRectangle(), ufoProcessHasEffect(), 
 *      ufoProcessPreRender(), ufoProcessPostRender(), 
 *      ufoProcessPreRenderPass(), ufoProcessPostRenderPass(), 
 *      ufoProcessCalcDefinedRectangle(), ufoProcessCalcNeedRectangles(), 
 *      ufoProcessPreSequenceRender(), ufoProcessPostSequenceRender().
 ******************************************************************************/
UFOAPI  
void ufoProcessGetRenderScale(
    ufoProcess      process_instance,           /*process instance handle*/
    
    double          *x_scale, 
    double          *y_scale);


/*******************************************************************************
 * ufoProcessGetRenderMode():
 *    Get current render mode (edit/full, single/sequence).
 *    See ufoTypes.h for enumeration.
 * 
 *    - May be called within 
 *      ufoProcessRenderPixel(), ufoProcessRenderLine(),
 *      ufoProcessRenderRectangle(), ufoProcessHasEffect(), 
 *      ufoProcessPreRender(), ufoProcessPostRender(), 
 *      ufoProcessPreRenderPass(), ufoProcessPostRenderPass(), 
 *      ufoProcessCalcDefinedRectangle(), ufoProcessCalcNeedRectangles(), 
 *      ufoProcessPreSequenceRender(), ufoProcessPostSequenceRender().
 ******************************************************************************/
UFOAPI  
ufoRenderMode ufoProcessGetRenderMode(
    ufoProcess      process_instance);          /*process instance handle*/


/******************************************************************************
 * ufoProcessGetDefinedRectangle():
 *    Get the defined maximum rectangle for the raster that can be supplied to a
 *    specific process input, regardless of the actual size of rectangle
 *    supplied.
 *    
 *    This will be the same rectangle which is supplied to the input_rectangles
 *    and needed_input_rectangle arguments supplied to
 *    ufoProcessCalcDefinedRectangle() and ufoProcessCalcNeedRectangles().
 * 
 *    - May be called within 
 *      ufoProcessRenderPixel(), ufoProcessRenderLine(),
 *      ufoProcessRenderRectangle(), ufoProcessPreRender(),
 *      ufoProcessPostRender(), ufoProcessPreRenderPass(),
 *      ufoProcessPostRenderPass().
 ******************************************************************************/
UFOAPI  
ufoRectangle ufoProcessGetDefinedRectangle(
    ufoProcess      process_instance,           /*process instance handle*/
    int             input_index);


/****************************************************************************************
 * ufoProcessParamDefine():
 *    Define parameter, parameters to ufo process.
 *    (one of 0..number_parameters - as set in ufoProcessCreate())
 * 
 *    - MUST be called within ufoProcessDefine() for each parameter, EXCEPT for
 *      parameters which are type ufoEnumParam which have a special define
 *      function, ufoProcessEnumParamDefine().
 ****************************************************************************************/
UFOAPI  
void ufoProcessParamDefine(
    ufoProcess      process_instance,           /*process instance handle*/
    
    int             param_index,                /*index (0..number_parameters) of parameter*/
    int             group_index,                /*index (0..number_parameter_groups) of parameter's group*/
    char            *id,                        /*unique id of parameter (unique within process) - NO SPACES)*/
    char            *name,                      /*user interface title of parameter*/
    ufoParamType    type);                      /*parameter type*/


/*******************************************************************************
 * ufoProcessEnumParamDefine():
 *    Define enumerated parameter of UFO process.  
 *    (one of 0..number_parameters - as set in ufoProcessCreate())
 * 
 *    - MUST be called within ufoProcessDefine() for each enumerated parameter.
 ******************************************************************************/
UFOAPI  
void ufoProcessEnumParamDefine(
    ufoProcess      process_instance,           /*process instance handle*/
    
    int             param_index,                /*index (0..number_parameters) of parameter*/
    int             group_index,                /*index (0..number_parameter_groups) of parameter's group*/
    char            *id,                        /*unique id of parameter (unique within process) - NO SPACES)*/
    char            *name,                      /*user interface title of parameter*/
    int             number_values,              /*number of values in enumeration*/
    char**          enum_value_text);           /*user interface strings representing enumeration values (0..number_values)*/


/*******************************************************************************
 * ufoProcessParamGroupDefine():
 *    Define parameter group to UFO process.
 *    (one of 0..number_parameter_groups - as set in ufoProcessCreate())
 * 
 *    - SHOULD be called within ufoProcessDefine() for each parameter group. 
 *      Since grouping is only used as a means to partition large numbers of
 *      parameters, group defining is not mandatory, although it is recommended.
 ******************************************************************************/
UFOAPI  
void ufoProcessParamGroupDefine(
    ufoProcess      process_instance,           /*process instance handle*/
    
    int             group_index,                /*index (0..number_parameter_groups) of parameter's group*/
    char            *id,                        /*unique id of parameter group (unique within process) - NO SPACES)*/
    char            *name);                     /*user interface title of parameter group*/

/*******************************************************************************
 * ufoProcessSetParamLimits():
 *    Set limits for parameter.
 * 
 *    - SHOULD be called within ufoProcessDefine() after ufoProcessParamDefine()
 *      for parameters of types ufoIntegerParam, ufoFloatParam, ufoAngleParam,
 *      where the limits need to be defined for user interface purposes. The
 *      clamp flags will ensure that values can be clamped when inquired.
 ******************************************************************************/
UFOAPI  
void ufoProcessSetParamLimits(
    ufoProcess      process_instance,           /*process instance handle*/

    int             param_index,                /*index (0..number_parameters) of parameter*/
    ufoChannelIndex channel_index,              /*index into particular channel (eg. ufoColourRGBChannelIndexR*/
                                                /*for parameter of type ufoColourRGBParam*/
    double          minimum,                    /*minimum limit*/
    double          maximum,                    /*maximum limit*/
    ufoLimitsClamp  clamp_flags);               /*clamp flags*/

                    
/******************************************************************************
 * ufoProcessSetParamAnimAllow():
 *    Allow/disallow animation of a particular parameter.
 * 
 *    - May be called within ufoProcessDefine() after ufoProcessParamDefine()
 *      for a given parameter.
 ******************************************************************************/
UFOAPI  
void ufoProcessSetParamAnimAllow(
    ufoProcess      process_instance,           /* process instance handle */
    
    int             param_index,                /* index (0..number_parameters) of parameter */
    int             anim_allow);                /* 1: allow animation (Default) 0: don't */

/****************************************************************************************
 * ufoProcessSetParamColumnHint():
 *    Give hint as to which column in the (auto-generated) UI the param should
 *    appear. N.B. This hint may be ignored by the application.
 * 
 *    - May be called within ufo function ufoProcessDefine() after ufoProcessParamDefine()
 *      for a given parameter
 ****************************************************************************************/
UFOAPI  
void ufoProcessSetParamColumnHint(
    ufoProcess      process_instance,           /* process instance handle */
    
    int             param_index,                /* index (0..number_parameters) of parameter */
    int             column);                    /* column (starting from 0) in which it should appear*/

/****************************************************************************************
 * ufoProcessSetParamGang():
 *    Indicate that the next 'n_params' should be ganged together in the UI.
 *    N.B. This is ignored unless the parameters are all of "slider	type" (Integer, Float
 *    or Normalised),  and all of the same type (e.g. all Integer).
 * 
 *    - May be called within ufo function ufoProcessDefine() after ufoProcessParamDefine()
 *      for a given parameter. It should be called only for the FIRST param in the group
 *      to be ganged.
 ****************************************************************************************/
UFOAPI  
void ufoProcessSetParamGang(
    ufoProcess      process_instance,           /* process instance handle */
    int             param_index,                /* index (0..number_parameters) of parameter */
    int             n_params);                  /* number of params to gang*/

/****************************************************************************************
 * ufoProcessSetParamGangLabel():
 *    Give label for set of params ganged together using ufoProcessSetParamGangHint
 * 
 *    - May be called within ufo function ufoProcessDefine() after ufoProcessParamDefine()
 *      for a given parameter. It should be called only for the FIRST param in the group
 *      to be ganged.
 ****************************************************************************************/
UFOAPI  
void ufoProcessSetParamGangLabel(
    ufoProcess      process_instance,           /* process instance handle */
    int             param_index,                /* index (0..number_parameters) of parameter */
    char            *label);                    /* UI label */

            
/*******************************************************************************
 * ufoProcessSetParamAutoTransition():
 *    Set parameter animation to be automatically fitted to a transition period
 *    when first used. (Thereafter the animation will have to be manually
 *    adjusted.)
 * 
 *    - May be called within ufoProcessDefine() after ufoProcessParamDefine()
 *      for a given parameter
 ******************************************************************************/
UFOAPI  
void ufoProcessSetParamAutoTransition(
    ufoProcess      process_instance,           /*process instance handle*/
    
    int             param_index,                /*index (0..number_parameters) of parameter*/
    int             auto_transition);           /*0: off (Default) 1: on*/

    
/****************************************************************************************
 * ufoProcessSetParamDefaultValue():
 * ufoProcessSetParamDefaultValues():
 * ufoProcessSetStringParamDefaultValue():
 *    Set a default value(s) for a parameter. Use ufoProcessSetParamDefaultValues()
 *    for setting multiple default values for parameters which have more than one channel.
 *    - May be called within ufo function ufoProcessDefine() after ufoProcessParamDefine()
 *      for a given parameter
 ****************************************************************************************/
UFOAPI  
void ufoProcessSetParamDefaultValue(
    ufoProcess      process_instance,           /* process instance handle */
    
    int             param_index,                /* index (0..number_parameters) of parameter */
    ufoChannelIndex channel_index,              /* index into particular channel */
    double          value);                     /* value  */

UFOAPI  
void ufoProcessSetParamDefaultValues(
    ufoProcess      process_instance,           /* process instance handle */
    
    int             param_index,                /* index (0..number_parameters) of parameter */
    double          values[]);                  /* values - must be correct length (eg 3 for ufoColourRGBParam) */
    
UFOAPI  
void ufoProcessSetStringParamDefaultValue(
    ufoProcess      process_instance,           /* process instance handle */
    
    int             param_index,                /* index (0..number_parameters) of parameter */
    const char*     value);                     /* value  */

    
/*******************************************************************************
 * ufoProcessSetParamDefaultKey():
 * ufoProcessSetParamDefaultKeys():
 * ufoProcessSetStringParamDefaultKey():
 *    Set a default animation key frame value for a parameter. Use
 *    ufoProcessSetParamDefaultValues() for setting multiple default values for
 *    parameters which have more than one channel.
 * 
 *    - May be called within ufoProcessDefine() after ufoProcessParamDefine()
 *      for a given parameter. 
 ******************************************************************************/
UFOAPI  
void ufoProcessSetParamDefaultKey(
    ufoProcess      process_instance,           /* process instance handle */
    
    int             param_index,                /* index (0..number_parameters) of parameter */
    ufoChannelIndex channel_index,              /* index into particular channel */
    double          value,                      /* value  */
    double          time);                      /* time at which key is added */

UFOAPI  
void ufoProcessSetParamDefaultKeys(
    ufoProcess      process_instance,           /* process instance handle */
    
    int             param_index,                /* index (0..number_parameters) of parameter */
    double          values[],                   /* values - must be correct length (eg 3 for ufoColourRGBParam) */
    double          time);                      /* time at which key is added */
    
UFOAPI  
void ufoProcessSetStringParamDefaultKey(
    ufoProcess      process_instance,           /* process instance handle */
    
    int             param_index,                /* index (0..number_parameters) of parameter */
    const char*     value,                      /* value  */
    double          time);                      /* time at which key is added */


    
    
/*******************************************************************************
 * ufoProcessGetParamValue():
 * ufoProcessGetParamValues():
 * ufoProcessGetStringParamValue():
 *    Get parameter values(s) at current time. Use ufoProcessGetParamValues() 
 *    for getting multiple values for parameters which have more than one
 *    channel.
 *    ufoProcessGetStringParamValue returns a character string allocated with malloc
 *    which the user has the responsibilty to free
 * 
 *    - May be called within 
 *      ufoProcessRenderPixel(), ufoProcessRenderLine(),
 *      ufoProcessRenderRectangle(), ufoProcessHasEffect(), 
 *      ufoProcessPreRender(), ufoProcessPostRender(), 
 *      ufoProcessPreRenderPass(), ufoProcessPostRenderPass(), 
 *      ufoProcessCalcDefinedRectangle(), ufoProcessCalcNeedRectangles(), 
 ******************************************************************************/
UFOAPI  
double ufoProcessGetParamValue(                 /* RETURNS value  */
    ufoProcess      process_instance,           /* process instance handle */
    
    int             param_index,                /* index (0..number_parameters) of parameter */
    ufoChannelIndex channel_index);             /* index into particular channel */

UFOAPI  
void ufoProcessGetParamValues(
    ufoProcess      process_instance,           /* process instance handle */
    
    int             param_index,                /* index (0..number_parameters) of parameter */
    double          values[]);                  /* values - must be correct length (eg 3 for ufoColourRGBParam) */

UFOAPI  
char * ufoProcessGetStringParamValue(           /* RETURNS value  */
    ufoProcess      process_instance,           /* process instance handle */
    
    int             param_index);               /* index (0..number_parameters) of parameter */



/*******************************************************************************
 * ufoProcessGetParamValueAtTime():
 * ufoProcessGetParamValuesAtTime():
 * ufoProcessGetStringParamValueAtTime():
 *    Get parameter values(s) at specified time. Use
 *    ufoProcessGetParamValuesAtTime() for getting multiple values for
 *    parameters which have more than one channel.
 *    ufoProcessGetStringParamValueAtTime returns a character string allocated with malloc
 *    which the user has the responsibilty to free
 * 
 *    - May be called within 
 *      ufoProcessRenderPixel(), ufoProcessRenderLine(),
 *      ufoProcessRenderRectangle(), ufoProcessHasEffect(), 
 *      ufoProcessPreRender(), ufoProcessPostRender(), 
 *      ufoProcessPreRenderPass(), ufoProcessPostRenderPass(), 
 *      ufoProcessCalcDefinedRectangle(), ufoProcessCalcNeedRectangles(), 
 *      ufoProcessPreSequenceRender(), ufoProcessPostSequenceRender().
 ******************************************************************************/
UFOAPI  
double ufoProcessGetParamValueAtTime(           /* RETURNS animation value value  */
    ufoProcess      process_instance,           /* process instance handle */
    
    int             param_index,                /* index (0..number_parameters) of parameter */
    ufoChannelIndex channel_index,              /* index into particular channel */
    double          time);                      /* time at which animation is sampled */

UFOAPI  
void ufoProcessGetParamValuesAtTime(
    ufoProcess      process_instance,           /* process instance handle */
    
    int             param_index,                /* index (0..number_parameters) of parameter */
    double          values[],                   /* values - must be correct length (eg 3 for ufoColourRGBParam) */
    double          time);                      /* time at which animation is sampled */

UFOAPI  
char * ufoProcessGetStringParamValueAtTime(     /* RETURNS animation value value  */
    ufoProcess      process_instance,           /* process instance handle */
    
    int             param_index,                /* index (0..number_parameters) of parameter */
    double          time);                      /* time at which animation is sampled */


/*******************************************************************************
 * ufoProcessSetParamValue():
 * ufoProcessSetParamValues():
 * ufoProcessSetStringParamValue():
 *    Set values(s) for a parameter at current time. 
 *    Use ufoProcessSetParamValues() for getting multiple values for parameters
 *    which have more than one channel.
 * 
 *    - May be called within ufoEditorMouseDown(), ufoEditorMouseUp(), 
 *      ufoEditorMouseDrag(), ufoEditorMousePosition(), ufoEditorKeyEvent(), 
 *      or from within any event function in the UFO editor GUI.
 ******************************************************************************/
UFOAPI  
void ufoProcessSetParamValue(
    ufoProcess      process_instance,           /* process instance handle */
    
    int             param_index,                /* index (0..number_parameters) of parameter */
    ufoChannelIndex channel_index,              /* index into particular channel */
    double          value);                     /* value */

UFOAPI  
void ufoProcessSetParamValues(
    ufoProcess      process_instance,           /* process instance handle */
    
    int             param_index,                /* index (0..number_parameters) of parameter */
    double          values[]);                  /* values - must be correct length (eg 3 for ufoColourRGBParam) */

UFOAPI  
void ufoProcessSetStringParamValue(
    ufoProcess      process_instance,           /* process instance handle */
    
    int             param_index,                /* index (0..number_parameters) of parameter */
    const char*     value);                     /* value */


/*******************************************************************************
 * ufoProcessSetParamKey():
 * ufoProcessSetParamKeys():
 * ufoProcessSetStringParamKey():
 *    Set animation key values(s) for a parameter. Use ufoProcessSetParamKeys()
 *    for setting multiple values for parameters which have more than one
 *    channel.
 * 
 *    (This function is intended for use in user supplied interfaces)
 ******************************************************************************/
UFOAPI  
void ufoProcessSetParamKey(
    ufoProcess      process_instance,           /* process instance handle */
    
    int             param_index,                /* index (0..number_parameters) of parameter */
    ufoChannelIndex channel_index,              /* index into particular channel */
    double          value,                      /* value */
    double          time);                      /* time at which key is added */
    
    
UFOAPI  
void ufoProcessSetParamKeys(
    ufoProcess      process_instance,           /* process instance handle */
    
    int             param_index,                /* index (0..number_parameters) of parameter */
    double          values[],                   /* values - must be correct length (eg 3 for ufoColourRGBParam) */
    double          time);                      /* time at which key is added */

UFOAPI  
void ufoProcessSetStringParamKey(
    ufoProcess      process_instance,           /* process instance handle */
    
    int             param_index,                /* index (0..number_parameters) of parameter */
    const char *    value,                      /* value */
    double          time);                      /* time at which key is added */
    
/*******************************************************************************
 * ufoProcessResetParam():
 *    Reset a parameter to its default value/animation. Resets all channels.
 * 
 *    (This function is intended for use in user supplied interfaces)
 ******************************************************************************/
UFOAPI  
void ufoProcessResetParam(
    ufoProcess      process_instance,           /* process instance handle */
    int             param_index);               /* index (0..number_parameters) of parameter */

/*******************************************************************************
 * ufoProcessIsParamSet():
 *    Inquire whether a parameter has actually had its values set yet in any
 *    user interface.
 * 
 *    - May be called within 
 *      ufoProcessRenderPixel(), ufoProcessRenderLine(),
 *      ufoProcessRenderRectangle(), ufoProcessHasEffect(), 
 *      ufoProcessPreRender(), ufoProcessPostRender(), 
 *      ufoProcessPreRenderPass(), ufoProcessPostRenderPass(), 
 *      ufoProcessCalcDefinedRectangle(), ufoProcessCalcNeedRectangles(), 
 *      ufoProcessPreSequenceRender(), ufoProcessPostSequenceRender().
 ******************************************************************************/
UFOAPI  
int ufoProcessIsParamSet(
    ufoProcess      process_instance,           /* process instance handle */   
    int             param_index);               /* index (0..number_parameters) of parameter */


/*******************************************************************************
 * ufoProcessGetTime():
 *    Get current frame time.
 *    This returns a double - but values will very likely always be integer when
 *    doing frame rendering, or integer on the first field and integer+0.5 on the
 *    second when doing field rendering.
 * 
 *    - May be called within 
 *      ufoProcessRenderPixel(), ufoProcessRenderLine(),
 *      ufoProcessRenderRectangle(), ufoProcessHasEffect(), 
 *      ufoProcessPreRender(), ufoProcessPostRender(), 
 *      ufoProcessPreRenderPass(), ufoProcessPostRenderPass(), 
 *      ufoProcessCalcDefinedRectangle(), ufoProcessCalcNeedRectangles(), 
 ******************************************************************************/
UFOAPI  
double ufoProcessGetTime(
    ufoProcess      process_instance);          /* process instance handle */

/*******************************************************************************
 * ufoProcessFieldRendering():
 *    Inquire the current sequence is being field rendered.
 *
 * ufoProcessWhichField():
 *    Inquire the current field (in time).
 *    This returns 1 or 2 if field rendering is on, else 0.
 *    1 is for the first field in time (odd), 
 *    2 is for the second field in time (even).
 *    To find out whether this is NTSC or PAL look at the height of rectangle
 *    obtained from ufoProcessGetFullRenderRectangle().
 *    NB The NTSC/PAL test is needed to determine which scanlines
 *    correspond to the temporal field.
 *    For NTSC, field 1 is even scanlines, field 2 is odd scan lines, 
 *    For PAL,  field 1 is odd scan lines, field 2 even scan lines.
 *    where the bottom scan line is 0 (even).
 *
 * 
 *    - May be called within 
 *      ufoProcessRenderPixel(), ufoProcessRenderLine(),
 *      ufoProcessRenderRectangle(), ufoProcessHasEffect(), 
 *      ufoProcessPreRender(), ufoProcessPostRender(), 
 *      ufoProcessPreRenderPass(), ufoProcessPostRenderPass(), 
 *      ufoProcessCalcDefinedRectangle(), ufoProcessCalcNeedRectangles()
 ******************************************************************************/
UFOAPI  
int ufoProcessFieldRendering(
    ufoProcess      process_instance);          /* process instance handle */

UFOAPI  
int ufoProcessGetField(
    ufoProcess      process_instance);          /* process instance handle */

/*******************************************************************************
 * ufoProcessGetMinEffectLimit():
 * ufoProcessGetMaxEffectLimit():
 *    Get current render effect start and end frame limits.
 *    These returns a double - but values will very likely always be integer.
 *    
 *    - May be called within 
 *      ufoProcessRenderPixel(), ufoProcessRenderLine(),
 *      ufoProcessRenderRectangle(), ufoProcessHasEffect(), 
 *      ufoProcessPreRender(), ufoProcessPostRender(), 
 *      ufoProcessPreRenderPass(), ufoProcessPostRenderPass(), 
 *      ufoProcessCalcDefinedRectangle(), ufoProcessCalcNeedRectangles(), 
 *      or from within any event function in the UFO editor GUI.
 ******************************************************************************/
UFOAPI  
double ufoProcessGetMinEffectLimit(
    ufoProcess      process_instance);          /* process instance handle */
    
UFOAPI  
double ufoProcessGetMaxEffectLimit(
    ufoProcess      process_instance);          /* process instance handle */

/*******************************************************************************
 * ufoProcessGetParamMinMaxKeyTime():
 *    Get minimum and maximum key frame time values over all parameter channels.
 * ufoProcessGetChanMinMaxKeyTime():
 *    Get minimum and maximum key frame time values for a parameter channel. 
 *
 *    - May be called within 
 *      ufoProcessRenderPixel(), ufoProcessRenderLine(),
 *      ufoProcessRenderRectangle(), ufoProcessHasEffect(), 
 *      ufoProcessPreRender(), ufoProcessPostRender(), 
 *      ufoProcessPreRenderPass(), ufoProcessPostRenderPass(), 
 *      ufoProcessCalcDefinedRectangle(), ufoProcessCalcNeedRectangles(), 
 *      or from within any event function in the UFO editor GUI.
 ******************************************************************************/
UFOAPI  
void ufoProcessGetParamMinMaxKeyTime(
    ufoProcess      process_instance,           /*process instance handle*/
    
    int             param_index,                /* index (0..number_parameters) of parameter */
    double          *min_time, 
    double          *max_time);
    
UFOAPI  
void ufoProcessGetChanMinMaxKeyTime(
    ufoProcess      process_instance,           /*process instance handle*/
    
    int             param_index,                /* index (0..number_parameters) of parameter */
    ufoChannelIndex channel_index,              /* index into particular channel */
    double          *min_time, 
    double          *max_time);


/*******************************************************************************
 * ufoProcessGetFullRenderRectangle():
 *    Get the default full render rectangle size. (NB: un-scaled and un-cropped).
 *    For identifying principle area to base effects around.
 * 
 *    - May be called within 
 *      ufoProcessRenderPixel(), ufoProcessRenderLine(), 
 *      ufoProcessRenderRectangle(), ufoProcessHasEffect(), 
 *      ufoProcessPreRender(), ufoProcessPostRender(), 
 *      ufoProcessPreRenderPass(), ufoProcessPostRenderPass(), 
 *      ufoProcessCalcDefinedRectangle(), ufoProcessCalcNeedRectangles(), 
 *      ufoProcessPreSequenceRender(), ufoProcessPostSequenceRender(),
 *      or from within any event function in the UFO editor GUI.
 ******************************************************************************/
UFOAPI  
void ufoProcessGetFullRenderRectangle(
    ufoProcess      process_instance,           /* process instance handle */
    
    int             *x1, 
    int             *y1, 
    int             *x2, 
    int             *y2);
    
UFOAPI  
void ufoProcessGetFullRenderRectangleStruct(
    ufoProcess      process_instance,           /* process instance handle */
    
    ufoRectangle    *rectangle);
            

/*******************************************************************************
 * ufoProcessGetAspectRatioCorrection():
 *    Get the aspect ratio correction factor to be applied during rendering.
 *    The value to be returned is the scale to be applied to the x pixel dimension
 *    in order to display it with the correct aspect.
 *    e.g. if a PAL 720x576 image is to be displayed at 16:9 then the aspect
 *    ratio correction returned will be (576/720)*(16/9) = 1.42222
 *    Any effects involving transformations such as rotation will need to take 
 *    this aspect ratio into account in order to appear correct when displayed
 *    in the final aspect ratio.
 * 
 *    - May be called within 
 *      ufoProcessRenderPixel(), ufoProcessRenderLine(), 
 *      ufoProcessRenderRectangle(), ufoProcessHasEffect(), 
 *      ufoProcessPreRender(), ufoProcessPostRender(), 
 *      ufoProcessPreRenderPass(), ufoProcessPostRenderPass(), 
 *      ufoProcessCalcDefinedRectangle(), ufoProcessCalcNeedRectangles(), 
 *      ufoProcessPreSequenceRender(), ufoProcessPostSequenceRender(),
 *      or from within any event function in the UFO editor GUI.
 ******************************************************************************/
UFOAPI  
double ufoProcessGetAspectRatioCorrection(
    ufoProcess      process_instance);          /* process instance handle */


/*******************************************************************************
 * ufoProcessCheckAbort():
 *    Check for user interrupting processing. You should supply an estimate of the
 *    percentage of the render done for display purposes.
 *    This is for developers who chose to not use the default slicing method provided.
 *    If the UFO is using multi-threading, only call this from the parent thread
 *    with which ufoProcessRenderRectangle() was called.
 *
 *    Returns 0: continue rendering
 *            1: Stop rendering
 * 
 *    - May be called within ufoProcessRenderRectangle().
 ******************************************************************************/
UFOAPI  
int ufoProcessCheckAbort(
    ufoProcess      process_instance,           /* process instance handle */
    int             percentage_render_done);

/*******************************************************************************
 * ufoMalloc():
 * ufoRealloc():
 * ufoFree():
 *    Host memory allocation routines.
 * 
 *    These use the memory management features of the host.
 *    - May be called within any UFO user functions.
 ******************************************************************************/
UFOAPI  
void* ufoMalloc(
    size_t          size);                      /* size in bytes */
    
UFOAPI  
void* ufoRealloc(
    void            *ptr,                       /* ufoMalloc'ed ufoRealloc'ed memory */
    size_t          size);                      /* size in bytes */
    
UFOAPI  
void ufoFree(
    void            *ptr);                      /* ufoMalloc'ed ufoRealloc'ed memory */

/*******************************************************************************
 * ufoCheckInBackground():
 *    Check if the main application is running in foreground mode (with
 *    interactive user interface) or in background mode (with no user interface).
 * 
 *    - May be called within any UFO user functions
 ******************************************************************************/  
UFOAPI  
int ufoCheckInBackground(void);

/*******************************************************************************
 * ufoCheckLicense():
 *    Check permission is set for ufo option in license file.
 *    Returns 1 if ufo_Option_Id is specified in valid license
 *    Returns 0 if ufo_Option_Id is not a valid licensed option, NULL, or empty string 
 *    - May be called within any UFO user functions.
 * 
 *    Note: Usage of this function requires a UFO licensing agreement
 *          with AVID Technology. For more information please contact
 *          the AVID product specialist for your product. 
 ******************************************************************************/    
UFOAPI  
int ufoCheckLicense(
    char*           ufo_Option_Id);             /* string identifying ufo option in license file */


/****************************************************************************************
 * ufoProcessHideParam():
 *    Mark a particular parameter to be hidden in the timeline keybars interface.
 *    Note: The keybars interface will only be updated when the viewed parameter
 *    group is changed or when ufoEditorForceUpdateUI() is called. 
 ****************************************************************************************/
UFOAPI  
void ufoProcessHideParam(
    ufoProcess      process_instance,
    int             param_index);


/****************************************************************************************
 * ufoProcessShowParam():
 *   Mark a particular parameter to be visible in the timeline keybars interface.
 *   Note: The keybars interface will only be updated when the viewed parameter
 *   group is changed or when ufoEditorForceUpdateUI() is called. 
 ****************************************************************************************/
UFOAPI  
void ufoProcessShowParam(
    ufoProcess      process_instance,
    int             param_index);


/*******************************************************************************
* == new in XSI
*
* simple methods of user-defined MP processing with indices
* can be used without modification on single CPU machine
*
* Do not use this in functions which are already called in worker threads,
* such as ufoProcessRenderLine.  Return false from ufoProcessSetMPSafe()
* to insure you do not get called from a worker thread.
*
*  call ufoMPSetIterateFunction to set the worker functions
*  and then call ufoMPIterate to run and wait for the function to do the processing
*
/*******************************************************************************/
typedef enum
{
    UFOMP_SCHEDULE_SIMPLE,          /* splits iterations amongst n processors into n complete blocks of contiguous data. */
    UFOMP_SCHEDULE_INTERLEAVE,      /* interleaves iterations */
} ufoMPScheduleType;


/*******************************************************************************
* Types of the functions.
*******************************************************************************/
typedef void (*ufoMPThreadFunc)     (void *);       /* definition for basic thread function */
typedef void (*ufoMPIterateThreadFunc)   (int, void *); /* definition for iterate thread functions */

/****************************************************************************************
* == new in XSI
* ufoMPSetIterateFunction():
*
*   Set the functions to be launched by ufoMPIterate
*   initialise and terminate functions can be NULL   
****************************************************************************************/

UFOAPI
void    ufoMPSetIterateFunction( 
                                    ufoMPIterateThreadFunc iterate_func, 
                                    ufoMPThreadFunc initialise_func,    /* can be null */
                                    ufoMPThreadFunc terminate_func      /* can be null */
                                    );
/****************************************************************************************
* == new in XSI
* ufoMPIterate():
*
*   Runs and wait for the completion of the functions set by ufoMPSetIterateFunction
*   The functions are called with the index they are given, between i_start_index
*   and including i_end_index
*
* example of function calls :
* 4 processors over 32 iterations
* there are 4 threads created.  each thread calls the interate function with 6 different 
* indices
*  
*  UFOMP_SCHEDULE_SIMPLE_:
*      11111111222222223333333344444444
* 
*  UFOMP_SCHEDULE_INTERLEAVE_:
*      12341234123412341234123412341234
*
****************************************************************************************/
UFOAPI
void    ufoMPIterate(
                        int i_start_index, 
                        int i_end_index, 
                        void *user_data, 
                        ufoMPScheduleType schedule_type);

/****************************************************************************************
 * ufoMPFork():
 *      Creates processes that execute the supplied func in parallel with
 *      the calling process, managed by the application multi-threader.
 *
 *      force_exactly_n should be set to:
 *          0: for standard behaviour of creating n-1 additional processes to the 
 *             calling process where n is the number of system processors.
 * 
 *          n: to force creation of exactly n-1 additional processes.
 *
 *      num_args should be set to:
 *          the number of arguments being passed to func in the following
 *          argument list. NOTE: a MAXIMUM of 6 arguments can be passed.
 *
 *      ufoMPForkFunc typedef for function to be called in parallel is supplied in
 *      ufoTypes.h
 *
 *      NOTE: ufoMPFork cannot be called within a function which itself is being
 *      multi-processed. If ufoProcessSetMPSafe() is set to false, then the application
 *      will not attempt to call any of the UFO functions in parallel and ufoMPFork
 *      can be called in any UFO user function. However, if ufoProcessSetMPSafe() is
 *      set to true (the default behavior),  then ufoMPFork must NOT be called within
 *      ufoProcessRenderLine() or ufoProcessesPixel(),  as on a multi-processor machine
 *      these functions may be called in parallel. 
 ****************************************************************************************/

UFOAPI  
void ufoMPFork(
    ufoMPForkFunc   func, 
    int             force_exactly_n,
    int             num_args, 
    ... );          /*list of arguments to be passed to function func*/


/****************************************************************************************
 * ufoMPForkMyId():
 *      When forked processes are created with ufoMPFork, each is given a 
 *      unique thread id,  which can be obtained via ufoMPForkMyId(). It will be
 *      an integer in the range 0..n-1.
 *
 *      Returns -1 on error.
 *
 *      -should be called within the (ufoMPForkFunc)func supplied to ufoMPFork. 
 ****************************************************************************************/
UFOAPI  
int ufoMPForkMyId(void);


/****************************************************************************************
 * ufoMPForkNumProcs():
 *      Query the current number of forked threads.
 *
 *      Returns -1 on error.
 *
 *      -should be called within the (ufoMPForkFunc)func supplied to ufoMPFork.
 ****************************************************************************************/
UFOAPI  
int ufoMPForkNumProcs(void);


/****************************************************************************************
 * ufoMPPotentialNumProcs():
 *      Query the number of potential threads available to processes.
 *      This may be restricted by:
 *            Machine processors present
 *            Application multi-processor licensing
 *            Application command line arguments specifying number of threads to use
 * 
 *      Can be used to determine potential maximum number of threads available for
 *      licensing purposes during process define.
 *      
 *      NOTE: ufoMPForkGetNumProcs() should be used to inqure the actual number of
 *      forked threads during a ufoMPFork() call, as this may be different to the
 *      potential maximum given by ufoMPPotentialNumProcs().
 * 
 *      -may be called within ufoProcessDefine()
 ****************************************************************************************/
UFOAPI   
int ufoMPPotentialNumProcs(void);
 
 
#ifdef __cplusplus
}
#endif

#endif


/*******************************************************************************
 * End of file ufoProcess.h
 ******************************************************************************/

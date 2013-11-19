//*****************************************************************************
/*!
   ile xsi_decl.h 
   rief Constants and enums declarations.

   © Copyright 1998-2002 Avid Technology, Inc. and its licensors. All rights 
   reserved. This file contains confidential and proprietary information of 
   Avid Technology, Inc., and is subject to the terms of the SOFTIMAGE|XSI 
   end user license agreement (or EULA).
*/
//*****************************************************************************

#ifndef _XSIDECL_H_
#define _XSIDECL_H_

#ifndef XSIDECL
#if !defined(unix)
	#ifdef SICORESDKIMPL
		#define XSIDECL __declspec(dllexport)
	#else
		#define XSIDECL __declspec(dllimport)
	#endif
#else
#define XSIDECL
#endif
#endif

#ifdef SI_HAS_PRAGMA_ONCE
# pragma once
#endif

#include <stdlib.h>

namespace XSI {

/*! Error constants. To use with Err object. */
typedef   
enum siErrorValueEnum
    {	siErrCancelled	= 0x4c7L		/*!< Command was cancelled. */
    } 	siErrorValueEnum;

/*! Time format values which are used by siTimeFormatDefaultFrameFormat <object idref="UserPreference"/>. */
typedef   
enum siDefaultTimeFormat
    {	siDefaultFILM24	= 7,		/*!< FILM 24 frames per second. */
	siDefaultPAL25	= 8,		/*!< PAL 25 frames per second. */
	siDefaultNTSC30	= 10,		/*!< NTSC 29.97 frames per second. */
	siDefaultUserTimeFormat	= 11,		/*!< User ticks per second. */
	siDefault30	= 19		/*!< HDTV 30 frames per seconde interlaced. */
    } 	siDefaultTimeFormat;

/*! Time format values which are used by siTimeDisplayFormatUserFormat <object idref="UserPreference"/>. */
typedef   
enum siDisplayTimeFormat
    {	siDisplayMilliSeconds	= 2,		/*!< Used for display format, to show the milliseconds equivalent of a frame rate. */
	siDisplayFILM24	= 7,		/*!< FILM 24 frames per second. */
	siDisplayPAL25	= 8,		/*!< PAL 25 frames per second. */
	siDisplayNTSCDF	= 9,		/*!< NTSC Drop Frame display format. */
	siDisplayNTSC30	= 10,		/*!< NTSC 29.97 frames per second (non-drop frame). */
	siDisplaySampleRate	= 12		/*!< Corresponds to the audio sample rate. */
    } 	siDisplayTimeFormat;

/*! Different modes for SIFilter command 
 *  \sa SIFilter 
 */
typedef   
enum siFilterMode
    {	siQuickSearch	= 0,		/*!< look up filter from installed filters */
	siSearchFamilies	= 1,		/*!< look up filter name in registered families */
	siSearchObjects	= 2,		/*!< look up filter name in registered objects (not implemented) */
	siFullSearch	= 3		/*!< siSearchFamilies and siSearchObjects */
    } 	siFilterMode;

/*! Delta modes for transformation commands (Translate, Rotate, Scale) 
 *  \sa Translate Rotate Scale 
 */
typedef   
enum siDeltaMode
    {	siAbsolute	= 0,		/*!< Transformation moves the object to an absolute position */
	siRelative	= 1		/*!< Transformation moves the object a relative distance */
    } 	siDeltaMode;

/*! Texture effects bit flags, to be used in conjunction with <object idref="Texture.GetTransformValues"/> */
typedef   
enum siTextureEffectsComputation
    {	siTextureComputeTransformation	= 1,		/*!< Uses the transformation of the Texture projection definition. */
	siTextureComputeCropping	= 2,		/*!< Uses the Image effect applied to the image clip. */
	siTextureComputeUVRemap	= 4,		/*!< Remaps the UVW domains using the data stored into the Advanced tab of the texture shader. */
	siTextureComputeFlip	= 8,		/*!< Uses the Flip information on the image clipto swap the UV values. */
	siTextureComputeRepeats	= 16,		/*!< Uses the repeat information of the texture shader.This flag applies to polygon mesh objects only. Artifacts may appear if the texture ends in the middle of a polygon. */
	siTextureComputeAlternate	= 32,		/*!< Only applies if the Repeat is on. Uses the alternate flag stored on the texture shader.  This flag applies to polygon mesh objects only. */
	siTextureComputeWrapping	= 64		/*!< Uses the flags stored on the Texture projection definition.This flag applies to polygon mesh objects only. Artifacts may appear if the texture ends in the middle of a polygon. */
    } 	siTextureEffectsComputation;

/*! Reference modes for transformation commands (Translate, Rotate, Scale). <br/>Note: "COG" indicates an additional translation about the Centre-Of-Geometry. "Sym" indicates a symmetrical translation. These indicators can also be used in combination. */
typedef   
enum siRefMode
    {	siGlobal	= 1,		/*!< Global (world) relative movement or position */
	siLocal	= 2,		/*!< Local movement relative to center of each object/component */
	siObjCtr	= 3,		/*!< By Reference movement relative to center of referenced object or an absolute position where 0,0,0 is the center of the referenced object. This corresponds to Ref mode on the MCP panel. */
	siView	= 4,		/*!< By Reference movement relative to the view.  */
	siParent	= 6,		/*!< Object/Parent movement is relative to the center of the parent (for an object) or the object (for a component). */
	siPlane	= 7,		/*!< By Reference movement relative to the active reference plane. This corresponds to Plane mode on the MCP panel. */
	siPivot	= 8,		/*!< Object/Pivot movement is relative to the object's pivot. If the object has not pivot or neutral pose, siPivot is the same as siParent. */
	siAdd	= 70,		/*!< Additive rotation when manipulating  (rotation only) */
	siGlobalCOG	= 17,		/*!< Global + COG */
	siLocalCOG	= 18,		/*!< Local + COG */
	siObjectCOG	= 19,		/*!< Object + COG */
	siViewCOG	= 20,		/*!< View + COG */
	siParentCOG	= 22,		/*!< Parent + COG */
	siPlaneCOG	= 23,		/*!< Plane + COG */
	siPivotCOG	= 24,		/*!< Pivot + COG */
	siAddCOG	= 86,		/*!< Add + COG */
	siGlobalSym	= 2049,		/*!< Global + Sym */
	siLocalSym	= 2050,		/*!< Local + Sym */
	siObjectSym	= 2051,		/*!< Object + Sym */
	siViewSym	= 2052,		/*!< View + Sym */
	siParentSym	= 2054,		/*!< Parent + Sym */
	siPlaneSym	= 2055,		/*!< Plane + Sym */
	siPivotSym	= 2056,		/*!< Pivot + Sym */
	siAddSym	= 2118,		/*!< Add + Sym */
	siGlobalSymCOG	= 2065,		/*!< Global + Sym + COG */
	siLocalSymCOG	= 2066,		/*!< Local + Sym + COG */
	siObjectSymCOG	= 2067,		/*!< Object + Sym + COG */
	siViewSymCOG	= 2068,		/*!< View + Sym + COG */
	siParentSymCOG	= 2070,		/*!< Parent + Sym + COG */
	siPlaneSymCOG	= 2071,		/*!< Plane + Sym + COG */
	siPivotSymCOG	= 2072,		/*!< Pivot + Sym + COG */
	siAddSymCOG	= 2134		/*!< Add + Sym + COG */
    } 	siRefMode;

/*! Reference modes for transformation commands. */
typedef   
enum siCenterMode
    {	siObj	= 0,		/*!< Object/component will be translated */
	siCtr	= 1		/*!< Center of the object will be translated */
    } 	siCenterMode;

/*! Axes filters for transformation commands (Translate, Rotate, Scale). <br/>Note: This flag is only used for absolute transformations. */
typedef   
enum siAxesFilter
    {	siX	= 1,		/*!< Filter in X */
	siY	= 2,		/*!< Filter in Y */
	siXY	= 3,		/*!< Filter in X &amp; Y */
	siZ	= 4,		/*!< Filter in Z */
	siXZ	= 5,		/*!< Filter in X &amp; Z */
	siYZ	= 6,		/*!< Filter in Y &amp; Z */
	siXYZ	= 7		/*!< Filter in X &amp; Y &amp; Z */
    } 	siAxesFilter;

/*! Axes filters for transformation commands (TranslateUVW, RotateUVW, ScaleUVW) */
typedef   
enum siUVWAxesFilter
    {	siU	= 1,		/*!< Filter in U */
	siV	= 2,		/*!< Filter in V */
	siUV	= 3,		/*!< Filter in U &amp; V */
	siW	= 4,		/*!< Filter in W */
	siUW	= 5,		/*!< Filter in U &amp; W */
	siVW	= 6,		/*!< Filter in V &amp; W */
	siUVW	= 7		/*!< Filter in U &amp; V &amp; W */
    } 	siUVWAxesFilter;

/*! Transformation type filters (Translation, Rotation, Scaling) */
typedef   
enum siTransformFilter
    {	siScl	= 1,		/*!< Scaling */
		/*!< Scaling */
	siRot	= 2,		/*!< Rotation */
		/*!< Rotation */
	siTrn	= 4,		/*!< Translation */
		/*!< Translation */
	siSR	= 8,		/*!< Scaling &amp; Rotation */
		/*!< Scaling &amp; Rotation */
	siST	= 16,		/*!< Scaling &amp; Translation */
		/*!< Scaling &amp; Translation */
	siRT	= 32,		/*!< Rotation &amp; Translation */
		/*!< Rotation &amp; Translation */
	siSRT	= 64		/*!< Scaling &amp; Rotation &amp; Translation */
		/*!< Scaling &amp; Rotation &amp; Translation */
    } 	siTransformFilter;

/*! Parameter filter (Marked, Animated, Scaling, Rotation, Translation, Weight, etc..) */
typedef   
enum siParameterFilter
    {	siNoParam	= 0,
	siMarkedParam	= 1,
	siAnimatedParam	= 3,
	siSelectedParam	= 4,
	siScalingParam	= 5,
	siRotationParam	= 6,
	siTranslationParam	= 7,
	siTransformParam	= 8,
	siAllParam	= 128
    } 	siParameterFilter;

/*! Parent-Child PSet relationships */
typedef   
enum siPSetRelationship
    {	siContainedPSet	= 1,		/*!< Child is contained part of parent e.g. Kinematics */
	siSharedPSet	= 2,		/*!< Child is an inherited PSet */
	siLocalPSet	= 4,		/*!< Child is a local PSet */
	siObjectPSet	= 8,		/*!< Child is a member of the parent e.g. Object in Group */
	siPortPSet	= 16,		/*!< Child is connected to the parent PSet e.g. Shader/Operator */
	siParameterPSet	= 32		/*!< Child is a leaf component part of the parent e.g. Object.Name. (not implemented) */
    } 	siPSetRelationship;

/*! Property propagation types */
typedef   
enum siPropagationType
    {	siDefaultPropagation	= 0,		/*!< based on propagation of selection type */
	siBranchPropagation	= 1,		/*!< branch propagation */
	siNodePropagation	= 2		/*!< node propagation */
    } 	siPropagationType;

/*! Duplicate Hierarchy options */
typedef   
enum siDupHierarchy
    {	siNodeDuplicate	= 0,		/*!< duplicate node only */
	siBranchDuplicate	= 1,		/*!< duplicate branch */
	siDefaultDuplicate	= 3		/*!< duplicate based on selection type */
    } 	siDupHierarchy;

/*! Duplicate History options */
typedef   
enum siDupHistory
    {	siCurrentHistory	= 0,		/*!< Duplicate final resulting geometry of the source object */
	siOriginalHistory	= 1,		/*!< Duplicate the original geometry of the source object (not implemented) */
	siDuplicateHistory	= 2,		/*!< Duplicate whole construction history if the source object */
	siSharedHistory	= 3		/*!< Share the construction history of the source object such that changes to the source object effect the copied object (same as instance) */
    } 	siDupHistory;

/*! Duplicate Parenting options */
typedef   
enum siDupParent
    {	siNoParent	= 0,		/*!< parent the copied object under the scene root */
	siSharedParent	= 1,		/*!< share the parent of the source object */
	siNewNullParent	= 2		/*!< create a null to act as the parent of this object */
    } 	siDupParent;

/*! Duplicate Grouping options */
typedef   
enum siDupGroup
    {	siNoGrouping	= 0,		/*!< ignore grouping */
	siShareGrouping	= 1		/*!< add Duplicate to the same groups as the source (not implemented) */
    } 	siDupGroup;

/*! Duplicate Property options */
typedef   
enum siDupProperty
    {	siDuplicateProperties	= 0,		/*!< copy properties from the source */
	siSharedProperties	= 1,		/*!< share properties with the source (not implemented) */
	siNoProperties	= 2		/*!< ignore properties */
    } 	siDupProperty;

/*! Duplicate Animation options */
typedef   
enum siDupAnimation
    {	siDuplicateAnimation	= 0,		/*!< copy animation from the source and share the mixer source */
	siNoAnimation	= 1,		/*!< ignore animation */
	siDupAnimationCopySource	= 2		/*!< copy animation from the source and copy the mixer source */
    } 	siDupAnimation;

/*! Duplicate Constraint options */
typedef   
enum siDupConstraint
    {	siDuplicateConstraints	= 0,		/*!< copy constraints from the source */
	siShareConstraints	= 1,		/*!< share constraints with the source (not implemented) */
	siNoConstraints	= 2		/*!< ignore constraints */
    } 	siDupConstraint;

/*! Duplicate Texture Supports options */
typedef   
enum siDupTextureSupport
    {	siDuplicateSelectedTextureSupports	= 0,		/*!< Projection definition is linked to a duplicated texture support object if the object was selected for duplication, shared otherwise */
	siDuplicateTextureSupports	= 1,		/*!< Texture support object is duplicated for each projection */
	siShareTextureSupports	= 2,		/*!< Texture support object is shared by the duplicated projections */
	siFreezeAllProjections	= 3		/*!< Duplicated projections are frozen and disconnected from the texture support object */
    } 	siDupTextureSupport;

/*! Duplicate Selection options */
typedef   
enum siDupSelection
    {	siSetSelection	= 0,		/*!< sets the current selection to the copied object */
	siNoSelection	= 1,		/*!< do not change the current selection */
	siAddToSelection	= 2		/*!< extend the current selection to include the copied object */
    } 	siDupSelection;

/*! Duplicate Transform options */
typedef   
enum siDupTransform
    {	siGlobalXForm	= 0,		/*!< apply same global transform as source object */
	siApplyRepeatXForm	= 1		/*!< apply and repeat transform argument on duplicated objects */
    } 	siDupTransform;

/*! Keyboard modifier states */
typedef   
enum siKeyboardState
    {	siShiftMask	= 1,		/*!< SHIFT key bit mask. */
	siCtrlMask	= 2,		/*!< CTRL key bit mask. */
	siAltMask	= 4		/*!< ALT key bit mask. */
    } 	siKeyboardState;

/*! Inspection Dialog Modes */
typedef   
enum siInspectMode
    {	siRecycle	= 1,		/*!< Recycle dialog */
	siFollow	= 2,		/*!< Recycle dialog if object of same family */
	siLock	= 3,		/*!< Prevent dialog from being recycled */
	siModal	= 4,		/*!< Popup a modal dialog */
	siLockAndForceNew	= 5		/*!< Force a new inspector to open and prevent new dialog from being recycled */
    } 	siInspectMode;

/*! Constants that define the way to browse the inspection history */
typedef   
enum siInspectBrowseMode
    {	siNext	= 1,		/*!< Browse to the next inspection in the history ( newer ) */
	siPrevious	= 2,		/*!< Browse to the previous inspection in the history ( older ) */
	siLast	= 3,		/*!< Browse to the last inspection in the history ( newest ) */
	siFirst	= 4		/*!< Browse to the first inspection in the history ( oldest ) */
    } 	siInspectBrowseMode;

/*! Animation Source Types */
typedef   
enum siSourceType
    {	siFCurveSource	= 1,		/*!< include fcurves */
	siExpressionSource	= 2,		/*!< include expressions */
	siAnySource	= 4,		/*!< any source */
	siMixerSource	= 8,		/*!< mixer source */
	siConstraintSource	= 16,		/*!< constraint source */
	siScriptedOperatorSource	= 32,		/*!< scripted operator source */
	siShapeAnimationSource	= 64		/*!< shape animation source */
    } 	siSourceType;

/*! The branch flag is used to determine whether we are refering to the object as a node or as a branch. */
typedef   
enum siBranchFlag
    {	siNode	= 0,		/*!< node */
	siBranch	= 1,		/*!< branch */
	siUnspecified	= 3		/*!< unspecified */
    } 	siBranchFlag;

/*! The operation mode is used to determine if we are in immediate mode or persistent mode */
typedef   
enum siOperationMode
    {	siPersistentOperation	= 0,		/*!< Persistent Operation */
	siImmediateOperation	= 1		/*!< Immediate Operation */
    } 	siOperationMode;

/*! This mode is used to determine if we should keep, hide or delete the inputs after a generator is applied */
typedef   
enum siGeneratorInputsDisposalMode
    {	siKeepGenOpInputs	= 0,		/*!< Keep the inputs */
	siHideGenOpInputs	= 1,		/*!< Hide the inputs */
	siDeleteGenOpInputs	= 2		/*!< Delete the inputs */
    } 	siGeneratorInputsDisposalMode;

/*! This mode is used to determine if the command should only apply to visible components */
typedef   
enum siComponentVisibilityMode
    {	siIgnoreComponentVisibility	= 0,		/*!< Ignore Visibility */
	siCheckComponentVisibility	= 1		/*!< Check Visibility */
    } 	siComponentVisibilityMode;

/*! This indicates what to do when local materials overlap */
typedef   
enum siActionWhenLocalMaterialsOverlap
    {	siPromptUser	= 0,		/*!< Prompt the user that local materials overlap (used by ApplyShader only) */
	siRemoveFromSelectedClusters	= 1,		/*!< Remove the common elements from the selected clusters */
	siRemoveFromExistingClusters	= 2,		/*!< Remove the common elements from the existing clusters */
	siLetLocalMaterialsOverlap	= 3,		/*!< Let the local materials overlap (used by ApplyShader only) */
	siJustReturnExistingClusters	= 4		/*!< Return the existing clusters overlapping with the selected ones (used by ClusterNonOverlapEnforcement only) */
    } 	siActionWhenLocalMaterialsOverlap;

/*! Split type for polygons */
typedef   
enum siSplitPolygonType
    {	siByTheVertices	= 0,		/*!< ByTheVertices */
	siByTheEdgesMidPoints	= 1,		/*!< ByTheEdgesMidPoints */
	siCross	= 2,		/*!< Cross */
	siDiamond	= 3		/*!< Diamond */
    } 	siSplitPolygonType;

/*! Constants that define the different options for updating selection list when working with TopoOp */
typedef   
enum siSelectionUpdate
    {	siLeaveSelection	= 0,		/*!< Leave Selection */
	siCleanSelection	= 1,		/*!< Clean Selection */
	siUpdateSelection	= 2		/*!< Update Selection */
    } 	siSelectionUpdate;

/*! Texture creation type constants. To use with the texturing commands. */
typedef   
enum siTxtCreationType
    {	siTxtPlanarXY	= 0,		/*!< XY Planar projection. */
	siTxtPlanarXZ	= 1,		/*!< XZ Planar projection. */
	siTxtPlanarYZ	= 2,		/*!< YZ Planar projection. */
	siTxtCylindrical	= 3,		/*!< Cylindrical projection. */
	siTxtSpherical	= 4,		/*!< Spherical projection. */
	siTxtSpatial	= 5,		/*!< Spatial projection. */
	siTxtCamera	= 6,		/*!< Camera projection. */
	siTxtUV	= 7,		/*!< UV projection. */
	siTxtLollipop	= 8,		/*!< Lollipop projection. */
	siTxtPureImplicit	= 9,		/*!< Purely implicit projection. */
	siTxtCubic	= 10,		/*!< Cubic projection. */
	siTxtUniqueUV	= 11		/*!< Unique UV projection. */
    } 	siTxtCreationType;

/*! Default texture creation type constants. To use with the texturing commands. */
typedef   
enum siTxtDefaultCreationType
    {	siTxtDefaultPlanarXY	= 0,		/*!< XY Planar projection. */
	siTxtDefaultPlanarXZ	= 1,		/*!< XZ Planar projection. */
	siTxtDefaultPlanarYZ	= 2,		/*!< YZ Planar projection. */
	siTxtDefaultCylindrical	= 3,		/*!< Cylindrical projection. */
	siTxtDefaultSpherical	= 4,		/*!< Spherical projection. */
	siTxtDefaultSpatial	= 5,		/*!< Spatial projection. */
	siTxtDefaultCubic	= 6,		/*!< Cubic projection. */
	siTxtDefaultLollipop	= 8		/*!< Lollipop projection. */
    } 	siTxtDefaultCreationType;

/*! Texture projection type constants. To use with the texturing commands. */
typedef   
enum siTxtProjType
    {	siProjPlanar	= 0,		/*!< Planar projection. */
	siProjCylindrical	= 1,		/*!< Cylindrical projection. */
	siProjSpherical	= 2,		/*!< Spherical projection. */
	siProjLollipop	= 3,		/*!< Lollipop projection. */
	siProjSpatial	= 4,		/*!< Spatial projection. */
	siProjCubic	= 5		/*!< Cubic projection. */
    } 	siTxtProjType;

/*! Texture projection plane constants. To use with the texturing commands. */
typedef   
enum siTxtProjPlane
    {	siPlaneXY	= 0,		/*!< XY Plane. */
	siPlaneXZ	= 1,		/*!< XZ Plane. */
	siPlaneYZ	= 2		/*!< YZ Plane. */
    } 	siTxtProjPlane;

/*! Texture support positioning constants. To use with the texturing commands. */
typedef   
enum siTxtProjFitMethod
    {	siRelDefault	= 0,		/*!< Perform default behavior for input: siRelBestFit for subcomponents, siRelObject for objects. */
	siRelBestFit	= 1,		/*!< Choose the best fit for the projection. */
	siRelObject	= 2,		/*!< Align relative to the object's coordinate system. */
	siRelWorld	= 3,		/*!< Align relative to the world's coordinate system. */
	siRelObjectPosObject	= 4,		/*!< Align relative to the object's coordinate system, and position/scale relative to the object's bounding box. */
	siRelWorldPosObject	= 5		/*!< Align relative to the world's coordinate system, and position/scale relative to the object's bounding box. */
    } 	siTxtProjFitMethod;

/*! Actions for the ChangeUVInput command */
typedef   
enum siChangeUVInputActions
    {	siChangeUVInputActionSwim	= 0,		/*!< Cause the UV to swim (move to top of operator stack). */
	siChangeUVInputActionStick	= 1,		/*!< If the UV operator is swimming (top of operator stack), cause the operator to stick (top of modeling). */
	siChangeUVInputActionReproject	= 2		/*!< If the UV operator is sticking (modeling region), force the operator to the top of the modeling stack. */
    } 	siChangeUVInputActions;

/*! Modes for the align command */
typedef   
enum siAlignModes
    {	siAlignGMIN	= 0,		/*!< Global minimum bounding value of the whole selection */
	siAlignGMIDDLE	= 1,		/*!< Global center bounding value of the whole selection */
	siAlignGMAX	= 2,		/*!< Global maximum bounding value of the whole selection */
	siAlignFMIN	= 3,		/*!< Minimum bounding value of the first selected object */
	siAlignFMIDDLE	= 4,		/*!< Center bounding value of the first selected object */
	siAlignFMAX	= 5		/*!< Maximum bounding value of the first selected object */
    } 	siAlignModes;

/*! Presets for the PresetWeight command */
typedef   
enum siWeightPresets
    {	siWeightEaseIn	= 1,		/*!< Ease-in preset */
	siWeightEaseOut	= 2,		/*!< Ease-out preset */
	siWeightLinearEaseIn	= 3,		/*!< Linear ease-in preset */
	siWeightLinearEaseOut	= 4,		/*!< Linear ease-out preset */
	siWeightExponentialIn	= 5,		/*!< Exponential-in preset */
	siWeightExponentialOut	= 6,		/*!< Exponential-out presett */
	siWeightGaussian	= 7,		/*!< Gaussian preset */
	siWeightGaussianReverse	= 8,		/*!< Reverse gaussian preset */
	siWeightSquare	= 9,		/*!< Square preset */
	siWeightSquareReverse	= 10,		/*!< Reverse square preset */
	siWeightOscillate	= 11,		/*!< Oscillation preset */
	siWeightConstant	= 12,		/*!< Constant preset */
	siWeightFlipX	= 13,		/*!< Flip X of weight curve */
	siWeightFlipY	= 14		/*!< Flip Y of weight curve */
    } 	siWeightPresets;

typedef   
enum siCutClipOptions

    {	siCut	= 1,		/*!< Splits the clip in two distincts clips */
	siTrimBefore	= 2,		/*!< Trims the clip keeping the tail */
	siTrimAfter	= 3		/*!< Trims the clip keeping the head */
    } 	siCutClipOptions;

/*! Edit clip options for the EditClip command */
typedef   
enum siEditClipOptions
    {	siClipNudge	= 1,		/*!< Nudge clip */
	siClipSlip	= 2,		/*!< Slip clip */
	siClipTrim	= 3,		/*!< Trim clip */
	siClipScale	= 4,		/*!< Scale clip */
	siClipStartAt	= 5,		/*!< Start clip at frame */
	siClipEndAt	= 6,		/*!< End clip at frame */
	siClipScaleTo	= 7		/*!< Scale clip to frame */
    } 	siEditClipOptions;

/*! Preset locations */
typedef   
enum siPresetLocation
    {	siDefaultLocation	= 0,		/*!< Default location */
	siFactory	= 1,		/*!< Factory */
	siUser	= 2,		/*!< User */
	siWorkgroup	= 3,		/*!< Workgroup */
	siProject	= 4		/*!< Project */
    } 	siPresetLocation;

/*! The variant type determines the kind of value that can be stored in a parameter.<br/>Note: Only a subset of these types are fully supported and recommended for use: these are siString, siBool, siDouble, siFloat, siInt4, siInt2, siUInt4, siUInt2, siByte, and siUByte. (See <link idref="DataTypes"/> for more information). */
typedef   
enum siVariantType
    {	siEmpty	= 0,		/*!< no type */
	siInt2	= 2,		/*!< 2 bytes signed integer number (-32768..32767) */
	siInt4	= 3,		/*!< 4 bytes signed integer number (-2147483648..2147483647) (See <link idref="Long"/>) */
	siFloat	= 4,		/*!< <link idref="Float"/>: single precision real number (-3.402823466e+38F..3.402823466e+38F) */
	siDouble	= 5,		/*!< <link idref="Double"/>: double precision real number (-1.7976931348623158e+308..1.7976931348623158e+308) */
	siString	= 8,		/*!< Text represented as a BSTR.  See <link idref="String"/>. */
	siDispatch	= 9,		/*!< dispatch object.  This is an object which exposes methods and properties to scripting.  This is the variant type of all objects in the Object Model. */
	siBool	= 11,		/*!< <link idref="Boolean"/> */
	siUnknown	= 13,		/*!< Pointer to a COM object.  This object cannot be manipulated from scripting and such an object is not normally exposed in the Object Model. */
	siByte	= 16,		/*!< byte (-128..127) */
	siUByte	= 17,		/*!< unsigned byte (0..255) */
	siUInt2	= 18,		/*!< 2 bytes unsigned integer number (0..65535) */
	siUInt4	= 19,		/*!< 4 bytes unsigned integer number (0..4294967295) */
	siInt	= 22,		/*!< signed machine integer.  This variant type is not supported, use siInt4 instead. */
	siUInt	= 23,		/*!< unsigned machine integer.  This variant type is not supported, use siUInt4 instead. */
	siWStr	= 31		/*!< Null-terminated wide character string.  This variant type is rarely encountered because siString is the recommended representation of all XSI strings. */
    } 	siVariantType;

/*! This determines the classification of a <object idref="Parameter"/>. This gives XSI some hint about the purpose of the Parameter. In most cases it is acceptable to use the default, siClassifUnknown.*/
typedef   
enum siParamClassification
    {	siClassifVisualization	= 1,		/*!< Parameter affects the UI visualization of an object (ex: level of detail) */
	siClassifPositioning	= 2,		/*!< Parameter affects the location, size and position of an object (ex: scaling) */
	siClassifAppearance	= 4,		/*!< Parameter affects the visual appearance of the default representation of an object (ex: color) */
	siClassifMetaData	= 8,
	siClassifModel	= 16,		/*!< Parameter affects the mathematical model of an object (ex: interpolation change of a surface) */
	siClassifTopo	= 16481,
	siClassifGeometry	= 16449,
	siClassifUnknown	= 32768		/*!< Default classification for normal parameters. */
    } 	siParamClassification;

/*! Port choice for SIGetMaterialShader command */
typedef   
enum siMaterialPorts
    {	siMaterialPortSurface	= 1,		/*!< surface shader port */
	siMaterialPortVolume	= 2,		/*!< volume shader port */
	siMaterialPortEnvironment	= 3,		/*!< environment shader port */
	siMaterialPortContour	= 4,		/*!< contour shader port */
	siMaterialPortDisplacement	= 5,		/*!< displacement shader port */
	siMaterialPortShadow	= 6,		/*!< shadow shader port */
	siMaterialPortPhoton	= 7,		/*!< photon shader port */
	siMaterialPortPhotonVol	= 8,		/*!< photon volume shader port */
	siMaterialPortBumpMap	= 9		/*!< bump map shader port */
    } 	siMaterialPorts;

/*! Defines the material shading model */
typedef   
enum siShadingModel
    {	siUndefinedShadingModel	= -1,		/*!< undefined shading model */
	siShadingModelConstant	= 0,		/*!< constant shading */
	siShadingModelLambert	= 1,		/*!< lambert shading */
	siShadingModelBlinn	= 2,		/*!< blinn shading */
	siShadingModelPhong	= 3,		/*!< phong shading */
	siShadingModelShadow	= 4,		/*!< shadow shading */
	siShadingModelColorVertex	= 5		/*!< color vertex shading */
    } 	siShadingModel;

/*! Obsolete as of version 5.0. Please use <object idref="siShaderParameterType"/> indicated values instead. */
typedef   
enum siShaderOutputType
    {	siUnknownOutputType	= 0,		/*!< see siUnknownParameterType */
	siBooleanOutputType	= 1,		/*!< see siBooleanParameterType */
	siIntegerOutputType	= 2,		/*!< see siIntegerParameterType */
	siScalarOutputType	= 3,		/*!< see siScalarParameterType */
	siColorOutputType	= 4,		/*!< see siColorParameterType */
	siVectorOutputType	= 5,		/*!< see siVectorParameterType */
	siTextureSpaceOutputType	= 6,		/*!< see siTextureSpaceParameterType */
	siTextureOutputType	= 7,		/*!< see siTextureParameterType */
	siStringOutputType	= 8,		/*!< see siStringParameterType */
	siFilenameOutputType	= 9,		/*!< see siStringParameterType */
	siLensOutputType	= 10,		/*!< see siLensParameterType */
	siLightOutputType	= 11,		/*!< see siLightParameterType */
	siMaterialOutputType	= 12,		/*!< see siMaterialParameterType */
	siModelOutputType	= 13,		/*!< see siModelParameterType */
	siLightmapOutputType	= 20		/*!< lightmap shader output type */
    } 	siShaderOutputType;

/*! Defines the <object idref="Shader"/> parameter type for a <object idref="Parameter"/> object belonging to <object idref="Shader"/>, <object idref="Material"/>, <object idref="Light"/> or <object idref="Camera"/>. Shader parameter types only apply to parameters that support the siTexturable capability. This can be tested using the <object idref="Parameter.Capabilities"/> property. The output type of a shader can be check using the <object idref="Shader.OutputType"/> property. */
typedef   
enum siShaderParameterType
    {	siUnknownParameterType	= 0,		/*!< unknown parameter type */
	siBooleanParameterType	= 1,		/*!< boolean shader parameter type */
	siColorParameterType	= 4,		/*!< color shader parameter type */
	siDataParameterType	= 14,		/*!< data shader parameter type */
	siIntegerParameterType	= 2,		/*!< integer shader parameter type */
	siLensParameterType	= 10,		/*!< lens shader parameter type */
	siLightParameterType	= 11,		/*!< light shader parameter type */
	siMaterialParameterType	= 12,		/*!< material shader parameter type */
	siMatrixParameterType	= 15,		/*!< transform matrix shader parameter type */
	siModelParameterType	= 13,		/*!< model parameter type, maps shader output type that can write to geometry shader. */
	siRealTimeParameterType	= 16,		/*!< realtime shader parameter type */
	siReferenceParameterType	= 17,		/*!< reference shader parameter type, input type only. */
	siScalarParameterType	= 3,		/*!< scalar shader parameter type; <link idref="Float"/> */
	siShaderReferenceParameterType	= 18,		/*!< shader reference shader parameter type */
	siStringParameterType	= 8,		/*!< string shader parameter type */
	siStructParameterType	= 19,		/*!< struct shader parameter type */
	siTextureParameterType	= 7,		/*!< texture shader parameter type; normally a <object idref="ImageClip"/> object (in the object model) or ImageClip2 class (in the C++ API). */
	siTextureSpaceParameterType	= 6,		/*!< texturespace shader parameter type; ; such as a vertexcolor, UV texture, weightmap <object idref="ClusterProperty"/> object or a texture map <object idref="Property"/>. */
	siVectorParameterType	= 5,		/*!< vector3d shader parameter type */
	siLightmapParameterType	= 20		/*!< Lightmap shader parameter type */
    } 	siShaderParameterType;

/*! Defines the types of light */
typedef   
enum siLightType
    {	siLightPoint	= 0,		/*!< point light */
	siLightInfinite	= 1,		/*!< infinite light */
	siLightSpot	= 2,		/*!< spot light */
	siLightSun	= 3,		/*!< sun light */
	siUndefinedLightType	= 4		/*!< undefined light */
    } 	siLightType;

/*! Types of groups in connection set */
typedef   
enum siCnxGroupType
    {	siCreatedObjects	= 1		/*!< created objects group */
    } 	siCnxGroupType;

/*! Types of connection sets */
typedef   
enum siCnxSetType
    {	siGroupLevel	= 0,		/*!< group level specifies objects containing the components that will be connected to the an operator's group of ports */
	siPortLevel	= 1		/*!< port level specifies objects that will be directly connected to the operator ports */
    } 	siCnxSetType;

/*! Types of partition */
typedef   
enum siPartitionType
    {	siObjectPartition	= 1,		/*!< object partition */
	siLightPartition	= 2		/*!< light partition */
    } 	siPartitionType;

/*! Levels of lock. Each level is a bit field. */
typedef   
enum siLockLevel
    {	siLockLevelNone	= 0,		/*!< Not locked */
	siLockLevelConstruction	= 1,		/*!< Cannot Change the hierarchy and topology */
	siLockLevelAnimation	= 2,		/*!< Cannot Animate */
	siLockLevelManipulation	= 4,		/*!< Cannot change the values */
	siLockLevelNavigation	= 8,
	siLockLevelAll	= 0xffff		/*!< All levels */
    } 	siLockLevel;

/*! Type of lock owner. The lock owner type indicated who or why an object is locked, Users can modify only the locks of type siLockTypeUser. This is a read-only */
typedef   
enum siLockType
    {	siLockTypeNone	= 0,		/*!< Not Locked */
	siLockTypeSystem	= 1,		/*!< Locked by the system */
	siLockTypeUser	= 2,		/*!< Locked by a user */
	siLockTypeRefModel	= 3,		/*!< Locked by a referenced model */
	siLockTypeOverride	= 4,		/*!< Locked by an override */
	siLockTypeAll	= 0xffff
    } 	siLockType;

/*!  These are the different levels of tags */
typedef   
enum siTags
    {	siTagNone	= 0,		/*!< Not tagged */
	siTag1	= 1,		/*!< User defined tag 1 */
	siTag2	= 2,		/*!< User defined tag 2 */
	siTag3	= 4,		/*!< User defined tag 3 */
	siTag4	= 8,		/*!< User defined tag 4 */
	siTag5	= 16,		/*!< User defined tag 5 */
	siTag6	= 32,		/*!< User defined tag 6 */
	siTag7	= 64,		/*!< User defined tag 7 */
	siTag8	= 128,		/*!< User defined tag 8 */
	siTag9	= 256,		/*!< User defined tag 9 */
	siTag10	= 512,		/*!< User defined tag 10 */
	siTagAll	= 0xffff		/*!< All tags */
    } 	siTags;

/*! Type of geometry */
typedef   
enum siGeometryType
    {	siNurbsSurfaceMesh	= 0,		/*!< nurbs surface */
	siNurbsCurveList	= 1,		/*!< nurbs curve */
	siPolygonMesh	= 2		/*!< mesh surface */
    } 	siGeometryType;

/*! Types of bounding volumes */
typedef   
enum siBoundingVolume
    {	siBoundingBox	= 0		/*!< bounding box */
		/*!< Collision is calculated with the bounding box of the obstacle */
    } 	siBoundingVolume;

/*! Enumerates the different kinds of ports. */
typedef   
enum siPortType
    {	siPortInput	= 0,		/*!< Input port. */
	siPortOutput	= 1		/*!< Output port. */
    } 	siPortType;

/*! Type of Modification on a referenced model. This is a bit field, so many values can  be combined. */
typedef   
enum siModificationType
    {	siModificationTypeNone	= 0,		/*!< None */
	siModificationTypeStaticPosition	= 1,		/*!< Static Posistion */
	siModificationTypeFCurves	= 2,		/*!< Animation FCurves */
	siModificationTypeExpression	= 4,		/*!< Expressions */
	siModificationTypeConstraints	= 8,		/*!< Constraints */
	siModificationTypeAddProp	= 16,		/*!< Adding properties */
	siModificationTypeAnimMixer	= 32,		/*!< Adding Animation mixer */
	siModificationTypeOtherConnections	= 64,
	siModificationTypeAll	= 65535		/*!< All types */
    } 	siModificationType;

/*! Type of Mixer Modification on a referenced model. This is a bit field, so many values can  be combined. */
typedef   
enum siModificationMixerType
    {	siModificationMixerNone	= 0,		/*!< None */
	siModificationMixerAnimation	= 1,		/*!< Animation tracks */
	siModificationMixerShape	= 2,		/*!< Shape tracks */
	siModificationMixerAudio	= 4,		/*!< Audio tracks */
	siModificationMixerTypeAll	= 65535		/*!< All types */
    } 	siModificationMixerType;

/*! Resolution description of a file for a referenced model */
typedef   
enum siModelResolution
    {	siModelResolutionOffLoaded	= 0,		/*!< Model not loaded */
	siModelResolutionLowRes	= 1,		/*!< Low resolution */
	siModelResolutionMediumRes	= 2,		/*!< Medium resolution */
	siModelResolutionHighRes	= 3		/*!< High resolution */
    } 	siModelResolution;

/*! Type of input objects for the Sequence feature */
typedef   
enum siSequenceType
    {	siMarkedParams	= 1,		/*!< Marked parameters of selected objects(s) */
	siAnimParams	= 2,		/*!< Animated parameters of selected objects(s) */
	siAllAnimParams	= 3		/*!< All animated parameters of all objects in scene */
    } 	siSequenceType;

/*! Mode to use for the Sequence feature */
typedef   
enum siSequenceMode
    {	siFCurves	= 1,		/*!< Process Fcurves */
	siFCurvesAnimationSources	= 2,		/*!< Process Fcurves and Animation Sources */
	siFCurvesClips	= 3,		/*!< Process Fcurves and clips */
	siAnimationSources	= 4,		/*!< Process Animation Sources */
	siClips	= 5		/*!< Process Clips */
    } 	siSequenceMode;

/*! Type of input objects for clip effect features */
typedef   
enum siEffectInputType
    {	siSelectedParams	= 1,		/*!< Marked parameters of selected objects(s) */
	siAllParamsInClip	= 2,		/*!< Single clip - operate on all parameters */
	siPoseOffsetClip	= 3		/*!< Single clip - operate on pose offset */
    } 	siEffectInputType;

/*! Types of neighboring */
typedef   
enum siNeighborType
    {	siAdjacent	= 1,		/*!< adjacent neighboring */
	siGrow	= 2,		/*!< grow neighboring */
	siNextCompo	= 3,		/*!< next component */
	siPrevCompo	= 4,		/*!< prev component */
	siFirstCompo	= 5,		/*!< first component */
	siLastCompo	= 6		/*!< last component */
    } 	siNeighborType;

/*! Modes for the SelectNeighborObj command */
typedef   
enum siNavigDirection
    {	siNavigUp	= 0,		/*!< Navigate Up */
	siNavigDown	= 1,		/*!< Navigate Down */
	siNavigLeft	= 2,		/*!< Navigate Left */
	siNavigRight	= 3,		/*!< Navigate Right */
	siNavigFirst	= 4,		/*!< Navigate to First Item */
	siNavigLast	= 5		/*!< Navigate to Last Item */
    } 	siNavigDirection;

/*! Direction of the next Node */
typedef   
enum siNavigationDirection
    {	siParentNode	= 1,		/*!< Ask for parent of the current node */
	siChildNode	= 2,		/*!< Ask for parent of the current node */
	siNextNode	= 3,		/*!< Ask for next sibling of the current node */
	siPreviousNode	= 4,		/*!< Ask for previous sibling of the current node */
	siFirstNode	= 5,		/*!< Ask for first sibling of the current node */
	siLastNode	= 6		/*!< Ask for last sibling of the current node */
    } 	siNavigationDirection;

/*! Direction of the next Node */
typedef   
enum siNavigationFilter
    {	siNavigationFilterScene	= 1,		/*!< Scene */
	siNavigationFilter3DObject	= 2,		/*!< 3D Object */
	siNavigationFilterOperator	= 3,		/*!< Operator */
	siNavigationFilterPrimitive	= 4,		/*!< Primitive */
	siNavigationFilterProperty	= 5,		/*!< Container or attribute */
	siNavigationFilterCluster	= 6,		/*!< Cluster */
	siNavigationFilterObjectSubElems	= 7,		/*!< Subcomponents */
	siNavigationFilterGroup	= 8,		/*!< Group */
	siNavigationFilterGenericComObject	= 9,		/*!< Any COM Object */
	siNavigationFilterAll	= 10,		/*!< All type */
	siNavigationFilterSame	= 11		/*!< Same type */
    } 	siNavigationFilter;

/*! SubType of node wanted */
typedef   
enum siNavigationSubFilter
    {	siNavigationSubFilterAll	= 0,		/*!< NavigationSubFilterAll */
	siNavigationSubFilterSame	= 1		/*!< NavigationSubFilterSame */
    } 	siNavigationSubFilter;

/*! Type of views. This type is obsolete, you must use <object idref="si2DChainAlignType"/> instead. */
typedef   
enum siViewType
    {	siNoView	= 0,
	siViewUser	= 1,		/*!< User viewport.  */
	siViewTop	= 2,		/*!< Top viewport. */
	siViewLeft	= 3,		/*!< Left viewport. */
	siViewRight	= 4		/*!< Right viewport. */
    } 	siViewType;

/*! Type of views. */
typedef   
enum si2DChainAlignType
    {	si2DChainTop	= 0,		/*!< Align with top viewport.  */
	si2DChainFront	= 1,		/*!< Align with front viewport.  */
	si2DChainRight	= 2,		/*!< Align with right viewport. */
	si2DChainNormalRadian	= 3,		/*!< Align with plane normal, value must be specified in radian units. */
	si2DChainNormalDegree	= 4		/*!< Align with plane normal, value must be specified in degree units. */
    } 	si2DChainAlignType;

/*! Type of chain bones. */
typedef   
enum siChainBoneType
    {	siChainBoneUndefined	= 0,
	siChainBonePin	= 1,		/*!< Pin bone.  */
	siChainBoneBallJoint	= 2		/*!< Ball joint bone. */
    } 	siChainBoneType;

/*! Operator connection types */
typedef   
enum siConnectionType
    {	siReadConnection	= 1,		/*!< read */
	siWriteConnection	= 2,		/*!< write */
	siReadWriteConnection	= 3		/*!< read/write */
    } 	siConnectionType;

/*! Bitmask for types of axes */
typedef   
enum siAxisType
    {	siXAxis	= 1,		/*!< X axis */
	siYAxis	= 2,		/*!< Y axis */
	siZAxis	= 3		/*!< Z axis */
    } 	siAxisType;

/*! Type of knot parameterization type for NURBS curves. */
typedef   
enum siKnotParameterization
    {	siUniformParameterization	= 0,		/*!< uniform */
	siNonUniformParameterization	= 1,		/*!< non-uniform */
	siChordLengthParameterization	= 2,		/*!< chord_length */
	siCentripetalParameterization	= 3		/*!< centripetal */
    } 	siKnotParameterization;

/*! Types of mapping rules. */
typedef   
enum siMappingRuleType
    {	siConnectionMappingRule	= 0,		/*!< ConnectionMappingRule */
	siValueMappingRule	= 1		/*!< ValueMappingRule */
    } 	siMappingRuleType;

/*! Types of nurbs direction parameters. */
typedef   
enum siNurbsDirection
    {	siUDirection	= 0,		/*!< nurbs direction in U */
	siVDirection	= 1		/*!< nurbs direction in V */
    } 	siNurbsDirection;

/*! Types of nurbs format. */
typedef   
enum siNurbsFormat
    {	siSINurbs	= 0,		/*!< Softimage nurbs format */
	siIGESNurbs	= 1		/*!< IGES nurbs format */
    } 	siNurbsFormat;

/*! Installation path directories. */
typedef   
enum siInstallationPath
    {	siProjectPath	= 0,		/*!< Project directory path. */
	siUserPath	= 1,		/*!< User directory path. */
	siWorkgroupPath	= 2,		/*!< Workgroup directory path. See <object idref="XSIApplication.Workgroups"/>. */
	siFactoryPath	= 3,		/*!< Factory directory path. */
	siAddonPath	= 4,		/*!< Add-on directory path. */
	siUserAddonPath	= 5,		/*!< User Add-on directory path. */
	siWorkgroupAddonPath	= 6,		/*!< Workgroup Add-on directory path. */
	siUnknownPath	= 7,		/*!< Unknown installation path. */
	siCustomPath	= 8		/*!< The custom plugin directory path. This value maps to the XSI_PLUGINS environment variable which may contain multiple folders. */
    } 	siInstallationPath;

/*! Shape key instantiation mode. */
typedef   
enum siShapeInstanceMode
    {	siShapeInstanceOnlyMode	= 0,		/*!< Only instance the action. */
	siShapeMixedWeightMode	= 1,		/*!< Instance in mixed weight mode. */
	siShapeTransitionMode	= 2,		/*!< Instance in transition mode. */
	siShapeTransitionCardinalMode	= 3,		/*!< Instance in transition cardinal mode. */
	siShapeBlendedWeightMode	= 4		/*!< Instance in mixed weight mode without keying the weight. */
    } 	siShapeInstanceMode;

/*! Shape key reference mode. */
typedef   
enum siShapeReferenceMode
    {	siShapeLocalReferenceMode	= 0,		/*!< Shapes are relative to reference frame of the control vertices. */
	siShapeObjectReferenceMode	= 1,		/*!< Shapes are relative to the primitive reference frame. */
	siShapeAbsoluteReferenceMode	= 2		/*!< Shapes are absolute. */
    } 	siShapeReferenceMode;

/*! Indicates which deforms will be stored in the created shape. */
typedef   
enum siShapeContent
    {	siShapeContentUseConstructionMode	= 0,
	siShapeContentPrimaryShape	= 1,		/*!< Store the primary shape deformations. */
	siShapeContentSecondaryShape	= 2,		/*!< Store the secondary shape deformations. */
	siShapeContentAllDeforms	= 3		/*!< Store all except for modeling deforms. */
    } 	siShapeContent;

/*! The construction mode indicates the purpose of the deformations. */
typedef   
enum siConstructionMode
    {	siConstructionModeModeling	= 0,		/*!< The deformations are used to model the geometry. */
	siConstructionModePrimaryShape	= 1,		/*!< The deformations are used to author shapes based on the base geometry. */
	siConstructionModeAnimation	= 2,		/*!< The deformations are used to animate the geometry. */
	siConstructionModeSecondaryShape	= 3,		/*!< The deformations are used to author shapes based on the animated geometry. */
	siConstructionModeDefault	= 4		/*!< Use the current user preference. */
    } 	siConstructionMode;

/*! Resolution Plane Type for Chain joints */
typedef   
enum siResolutionPlane
    {	siDefaultResolutionPlane	= 0,		/*!< default resolution plane */
	siPreferredAxisPlane	= 1,
	siUpVectorPlane	= 2
    } 	siResolutionPlane;

/*! Texture creation type constants. To use with the texturing commands. */
typedef   
enum siTextureProjectionType
    {	siTexturePlanarXY	= 0,		/*!< XY Planar projection. */
	siTexturePlanarXZ	= 1,		/*!< XZ Planar projection. */
	siTexturePlanarYZ	= 2,		/*!< YZ Planar projection. */
	siTextureCylindrical	= 3,		/*!< Cylindrical projection. */
	siTextureSpherical	= 4,		/*!< Spherical projection. */
	siTextureSpatial	= 5,		/*!< Spatial projection. */
	siTextureCamera	= 6,		/*!< Camera projection. */
	siTextureUV	= 7,		/*!< UV projection. */
	siTextureLollipop	= 8,		/*!< Lollipop projection. */
	siTexturePureImplicit	= 9		/*!< Purely implicit projection. */
    } 	siTextureProjectionType;

/*! UV generation type used by the texture operator, used only if siTextureProjectionType:siTextureUV is specified as the projection type. */
typedef   
enum siUVTextureType
    {	siUVTexturePlanarXY	= 0,		/*!< XY Planar projection. */
	siUVTexturePlanarXZ	= 1,		/*!< XZ Planar projection. */
	siUVTexturePlanarYZ	= 2,		/*!< YZ Planar projection. */
	siUVTextureCylindrical	= 3,		/*!< Cylindrical projection. */
	siUVTextureSpherical	= 4,		/*!< Spherical projection. */
	siUVTextureSpatial	= 5,		/*!< Spatial projection. */
	siUVTextureLollipop	= 6		/*!< Lollipop projection. */
    } 	siUVTextureType;

/*! Determines the capabilities of a parameter. They can be combined by adding them: to have a parameter animatable (1) and persistable (4), use 5. */
typedef   
enum siCapabilities
    {	siAnimatable	= 1,		/*!< Animatable */
	siReadOnly	= 2,		/*!< Cannot be written to */
	siPersistable	= 4,		/*!< Is saved with its parameter set */
	siNotInspectable	= 8,		/*!< Does not show up in the UI */
	siSilent	= 16,		/*!< Does not notify if its value is changed */
	siNotPresetPersistable	= 128,		/*!< Will not be saved in presets */
	siTexturable	= 256,		/*!< Parameter support connection to rendering node */
	siKeyable	= 2048,		/*!< Parameter is visible in 'Keying Panel' and keyable */
	siNonKeyableVisible	= 4096		/*!< Parameter is only visible in 'Keying Panel' */
    } 	siCapabilities;

/*! Types of function curves (fcurves) */
typedef   
enum siFCurveType
    {	siUnknownFCurve	= 0,		/*!< Unknown function curve type */
	siDefaultFCurve	= 0,		/*!< Use the fcurve type associated with parameter type */
	siBooleanFCurve	= 10,		/*!< Boolean function curve mapping a double parameter to {0, 1}. */
	siIntegerFCurve	= 15,		/*!< Integer function curve. */
	siStandardFCurve	= 20,		/*!< Function curve mapping a double parameter to a double value. */
	siRawDataFCurve	= 30		/*!< Raw data function curve. */
    } 	siFCurveType;

/*! The extrapolation of an FCurve determines its shape before the first <object idref="FCurveKey"/> and after the last <object idref="FCurveKey"/>. Extrapolation extends the curve's ends based on the position of these two keys. */
typedef   
enum siFCurveExtrapolation
    {	siConstantExtrapolation	= 1,		/*!< constant extrapolation */
	siLinearExtrapolation	= 2,		/*!< linear extrapolation */
	siPeriodicExtrapolation	= 3,		/*!< periodic extrapolation */
	siPeriodicRelativeExtrapolation	= 4		/*!< constant extrapolation relative to an offset */
    } 	siFCurveExtrapolation;

/*! Function curve key constraints. The last seven tangent constraints apply only to standard function curves. FCurve key constraints can be set and retrieved via the <object idref="FCurveKey.Constraint"/> property. The constraints are not mutally exclusive for example if you have the siParameterConstraint set and clear the siLockConstraint you will also clear the siParameterConstraint. You cannot set constraints on a raw function curve. They will be ignored. */
typedef   
enum siFCurveKeyConstraint
    {	siParameterConstraint	= 1,		/*!< If on, the parameter (or key time) is locked. */
	siLeftRightValuesConstraint	= 2,		/*!< If on, the left and right values are locked. */
	siG1ContinuousConstraint	= 4,		/*!< If on, the function curve is G1 continuous at this key. */
	siLeftRightTangentDirectionConstraint	= 8,		/*!< If on, the left and right tangent directions are locked. */
	siLeftRightTangentLengthConstraint	= 16,		/*!< If on, the left and right tangent lengths are locked. */
	siLockConstraint	= 32,		/*!< If on, the parameter, values, and tangents are locked. */
	siHorizontalTangentConstraint	= 64,		/*!< If on, the tangents are considered horizontal. */
	siExtremumHorizontalTangentConstraint	= 128,		/*!< If on, the tangents are considered as horizontal when the key is a local extremum. */
	siAdjustedTangentConstraint	= 256,		/*!< If on, the tangents are adjusted automatically when neighbouring keys are created or moved. */
	siZeroLengthTangentConstraint	= 512,		/*!< If on, the left and right tangent lengths are constrained to be zero. */
	siSameLengthTangentConstraint	= 1024		/*!< If on, the left and right tangent lengths are the same. */
    } 	siFCurveKeyConstraint;

typedef   siFCurveKeyConstraint siFCurveKeyFlags;

/*! Specifies the interpolation type for a specific key on an fcurve. */
typedef   
enum siFCurveKeyInterpolation
    {	siDefaultKeyInterpolation	= 0,		/*!< Existing function. */
	siConstantKeyInterpolation	= 1,		/*!< Constant function */
	siLinearKeyInterpolation	= 2,		/*!< Linear function */
	siCubicKeyInterpolation	= 3		/*!< Bezier cubic function */
    } 	siFCurveKeyInterpolation;

/*! The interpolation of an FCurve determines how the value will be evaluated between the FCurveKey's. */
typedef   
enum siFCurveInterpolation
    {	siDefaultInterpolation	= 0,
	siConstantInterpolation	= 1,		/*!< Constant function */
	siLinearInterpolation	= 2,		/*!< Linear function */
	siCubicInterpolation	= 3		/*!< Bezier cubic function */
    } 	siFCurveInterpolation;

/*! Specifies the kind of filter used for smoothing function curves. */
typedef   
enum siFCurveSmoothFilterType
    {	siFCurveAverageFilterType	= 1,		/*!< Moving Average filter. Use this filter to smooth the function curve by applying a moving average. The weights of the averages are uniformly distributed. */
	siFCurveGaussianFilterType	= 2		/*!< Gaussian filter. Use this filter to smooth the function curve by applying a moving average. The weights of the averages are distributed as a bell curve. This usually gives better results than the average filter. */
    } 	siFCurveSmoothFilterType;

#define	siPI	( 3.14159 )

/*! Information on the context in which a plugin is being evaluated. */
typedef   
enum siUpdateContextState
    {	siUpdateContextNormalEval	= 0,		/*!< Normal evaluation */
	siUpdateContextRenderEval	= 1		/*!< Evaluation during a render pass */
    } 	siUpdateContextState;

/*! Information on the direction of an argument indicating how it is passed to or from a command.<br/> Note: Output and InputOutput arguments are not supported by some script languages, including JScript. For this reason only siArgumentInput is supported on custom <object idref="Command"/>s.*/
typedef   
enum siArgumentFlags
    {	siArgumentInput	= 2,		/*!< the parameter is passed as input to the command. */
	siArgumentOutput	= 4,		/*!< the parameter is used to return information from the command. */
	siArgumentInputOutput	= 6		/*!< the parameter is used to pass information both to and from a command. */
    } 	siArgumentFlags;

/*! <object idref="Command"/>s can be defined from a pre-defined category. Each category specifies the base arguments of a command and the location of the menu item for that command in XSI. XSI automatically installs a menu item for each command created for a specific category.<br/> Note: An alternative, and more powerful, way to add custom commands to menus is through the <object idref="Menu"/> API. */
typedef   
enum siCommandCategory
    {	siImportCategory	= 0,
	siExportCategory	= 1,
	siSettingsCategory	= 2,
	siHelpCategory	= 3,
	siNoCategory	= 4,
	siFCurveCategory	= 5
    } 	siCommandCategory;

/*! Defines the option flags when importing a model or merging a scene. */
typedef   
enum siImportShareOptions
    {	siImportShareOptionsNone	= 0,
	siImportShareOptionsImageClips	= 1,
	siImportShareOptionsMaterials	= 2,
	siImportShareOptionsLayers	= 4,
	siImportShareOptionsPartitions	= 8,
	siImportShareOptionsAll	= 0xffff
    } 	siImportShareOptions;

/*! This enumerator provides the identifiers for the XSI events, they can be used for implementing C++ event handlers in order to identify the events fired by XSI. */
typedef   
enum siEventID
    {	siOnBeginNewScene	= 542179328,		/*!< OnBeginNewScene event identifier. */
	siOnEndNewScene	= 542179329,		/*!< OnEndNewScene event identifier. */
	siOnBeginSceneOpen	= 542179330,		/*!< OnBeginSceneOpen event identifier. */
	siOnEndSceneOpen	= 542179331,		/*!< OnEndSceneOpen event identifier. */
	siOnBeginSceneSave	= 542179332,		/*!< siOnBeginSceneSave event identifier. */
	siOnEndSceneSave	= 542179333,		/*!< siOnEndSceneSave event identifier. */
	siOnBeginSceneSaveAs	= 542179334,		/*!< OnBeginSceneSaveAs event identifier. */
	siOnEndSceneSaveAs	= 542179335,		/*!< OnEndSceneSaveAs event identifier. */
	siOnTerminate	= 542179336,		/*!< OnTerminate event identifier. */
	siOnSelectionChange	= 542179337,		/*!< OnSelectionChange event identifier. */
	siOnBeginFrame	= 542179338,		/*!< siOnBeginFrame event identifier. */
	siOnEndFrame	= 542179339,		/*!< siOnEndFrame event identifier. */
	siOnBeginSequence	= 542179340,		/*!< siOnBeginSequence event identifier. */
	siOnEndSequence	= 542179341,		/*!< siOnEndSequence event identifier. */
	siOnKeyUp	= 542179342,		/*!< OnKeyUp event identifier. */
	siOnKeyDown	= 542179343,		/*!< OnKeyDown event identifier. */
	siOnValueChange	= 542179344,		/*!< OnValueChange event identifier. This event occurs when a parameter value is changed. */
	siOnTimeChange	= 542179345,		/*!< OnTimeChange event identifier. This event occurs when the time line changes. */
	siOnWindowEvent	= 542179346,		/*!< OnWindowEvent event identifier. This event occurs when a custom display changes position or get refreshed. */
	siOnBeginFileExport	= 542179347,		/*!< siOnBeginFileExport event identifier. This event occurs at the beginning of a file export. */
	siOnEndFileExport	= 542179348,		/*!< siOnEndFileExport event identifier. This event occurs at the end of a file export. */
	siOnBeginFileImport	= 542179349,		/*!< siOnBeginFileImport event identifier. This event occurs at the beginning of a file import. */
	siOnEndFileImport	= 542179350,		/*!< siOnEndFileImport event identifier. This event occurs at the end of a file import. */
	siOnStartup	= 542179351,		/*!< siOnStartup event identifier. This event is fired when XSI starts up. */
	siOnSourcePathChange	= 542179352,		/*!< siOnSourcePathChange event identifier. This event is fired when an XSI source path is changed. */
	siOnObjectAdded	= 542179353,		/*!< OnObjectAdded event identifier. This event occurs when a object has been added to the scene. */
	siOnObjectRemoved	= 542179354,		/*!< OnObjectRemoved event identifier. This event occurs when a object has been removed from the scene. */
	siOnTimer	= 542179355,		/*!< siOnTimer event identifier. This event occurs when a user timer elapses. */
	siOnBeginSceneSave2	= 542179356,		/*!< siOnBeginSceneSave2 event identifier. */
	siOnEndSceneSave2	= 542179357,		/*!< siOnEndSceneSave2 event identifier. */
	siOnCloseScene	= 542179358,		/*!< siOnCloseScene event identifier. This event occurs when a scene gets closed upon NewScene, OpenScene or a new project. */
	siOnActivate	= 542179359,		/*!< siOnActivate event identifier. Occurs when the XSI application gets activated or de-activated. */
	siOnMaximize	= 542179360,		/*!< siOnMaximize event identifier. Occurs when the XSI application is maximized. */
	siOnMinimize	= 542179361,		/*!< siOnMinimize event identifier. Occurs when the XSI application is minimized. */
	siOnChangeProject	= 542179362		/*!< siOnChangeProject event identifier. Occurs when the current XSI project has been changed for a new one. */
    } 	siEventID;

/*! Specifies the type of operation performed to update the selection. */
typedef   
enum siSelectionChangeType
    {	siAddedToSelection	= 0,		/*!< one or more objects added to selection. */
	siRemovedFromSelection	= 1		/*!< one or more objects removed from selection. */
    } 	siSelectionChangeType;

/*! Specifies the render field setting.*/
typedef   
enum siRenderFieldType
    {	siRenderFieldNone	= 0,		/*!< no field setting. */
	siRenderFieldOdd	= 1,		/*!< odd field setting. */
	siRenderFieldEven	= 2		/*!< even field setting. */
    } 	siRenderFieldType;

typedef   
enum siRenderingType

    {	siRenderSequence	= 0,		/*!< render sequence. */
	siRenderFramePreview	= 1,		/*!< frame preview rendering. */
	siRenderEchoToMi	= 2		/*!< echo to an mi file. */
    } 	siRenderType;

/*! Specifies the subdivision rule used for an approximated geometry selection. */
typedef   
enum siSubdivisionRuleType
    {	siCatmullClark	= 0,		/*!< Catmull-Clark subdivision rule. */
	siXSIDooSabin	= 2,		/*!< XSI-Doo-Sabin subdivision rule. */
	siLinearSubdivision	= 3		/*!< Linear subdivision rule. */
    } 	siSubdivisionRuleType;

/*! Direction type for navigating geometry components. */
typedef   
enum siNavigateComponentType
    {	siNextComponent	= 0,		/*!< next component */
	siPreviousComponent	= 1,		/*!< previous component */
	siFirstComponent	= 2,		/*!< first component */
	siLastComponent	= 3		/*!< last component */
    } 	siNavigateComponentType;

/*! Types of particle shapes */
typedef   
enum siParticleShapeType
    {	siNoneType	= 0,		/*!< No specified shape type */
	siStepType	= 1,		/*!< Step shape type */
	siSineType	= 2,		/*!< Sine shape type */
	siStarType	= 3,		/*!< Star shape type */
	siBeamType	= 4,		/*!< Beam shape type */
	siSymmetryPatternType	= 5,		/*!< Symmetry pattern shape type */
	siNoiseType	= 6,		/*!< Noise shape type */
	siTurbulenceType	= 7,		/*!< Turbulence shape type */
	siFractalType	= 8		/*!< Fractal shape type */
    } 	siParticleShapeType;

/*! Enumeration of particle render types */
typedef   
enum siParticleRenderType
    {	siBillboardType	= 0,		/*!< Billboard render type */
	siSphereType	= 1,		/*!< Sphere render type */
	siBlobType	= 2		/*!< Blob render type */
    } 	siParticleRenderType;

/*! Types of particle billboard shapes */
typedef   
enum siParticleBillboardShapeType
    {	siSquareType	= 0,		/*!< Square billboard shape type */
	siRectangularType	= 1,		/*!< Rectangular billboard shape type */
	siCircularType	= 2		/*!< Circular billboard shape type */
    } 	siParticleBillboardShapeType;

/*! Types of particle billboard texture projections */
typedef   
enum siParticleBillboardTextureProjectionType
    {	siPlanarProjection	= 0,		/*!< Planar billboard texture projection */
	siParticleLocalSpace	= 1,		/*!< Particle local billboard texture projection */
	siCloudLocalSpace	= 2,		/*!< Cloud local billboard texture projection */
	siWorldSpace	= 3		/*!< World billboard texture projection */
    } 	siParticleBillboardTextureProjectionType;

/*! Types of particle billboard surface normals */
typedef   
enum siParticleBillboardSurfaceNormalType
    {	siBillboardSurfaceNormal	= 0,		/*!< Billboard surface normals */
	siSphericalSurfaceNormal	= 1		/*!< Spherical surface normals */
    } 	siParticleBillboardSurfaceNormalType;

/*! Types of particle billboard face directions */
typedef   
enum siParticleBillboardFaceDirectionsType
    {	siCamera	= 0,		/*!< Billboard camera face direction */
	siIncomingRay	= 1,		/*!< Billboard incoming ray face direction */
	siCameraAndLights	= 2,		/*!< Billboard camera and lights face direction */
	siUseRotation	= 3		/*!< Billboard use particle rotations face direction */
    } 	siParticleBillboardFaceDirectionsType;

/*! Types of particle shape falloffs */
typedef   
enum siParticleShapeFalloffType
    {	siNone	= 0,		/*!< No particle shape falloff */
	siLinear	= 1,		/*!< Linear particle shape falloff */
	siSquare	= 2,		/*!< Square particle shape falloff */
	siSmooth	= 3,		/*!< Smooth particle shape falloff */
	siCubic	= 4,		/*!< Cubic particle shape falloff */
	siGaussian	= 5		/*!< Gaussian particle shape falloff */
    } 	siParticleShapeFalloffType;

/*! Types of particle emission generations */
typedef   
enum siParticleEmissionGenerationType
    {	siPoint	= 0,		/*!< Point particle emission */
	siLine	= 1,		/*!< Line particle emission */
	siSurface	= 2,		/*!< Surface particle emission */
	siVolume	= 3,		/*!< Volume particle emission */
	siFluid	= 4		/*!< Fluid particle emission */
    } 	siParticleEmissionGenerationType;

/*! Types of particle emission directions */
typedef   
enum siParticleEmissionDirectionType
    {	siEmissionDirectionNormal	= 0,		/*!< Emission along normal at emission point */
	siEmissionDirectionLocal	= 1,		/*!< Emission along Y axis of emitter object */
	siEmissionDirectionGlobal	= 2		/*!< Emission along Y axis of world */
    } 	siParticleEmissionDirectionType;

/*! Types of simultion parameter distributions /*! Types of simultion parameter animation controls */
typedef   
enum siSimulationParameterDistributionType
    {	siDistributionUniform	= 0,		/*!< Uniform distribution */
	siDistributionGaussian	= 1		/*!< Gaussian distribution */
    } 	siSimulationParameterDistributionType;

typedef   
enum siSimulationParameterAnimationControlType

    {	siParamControlBirth	= 0,		/*!< Parameter animation controls value at birth */
	siParamControlAge	= 1,		/*!< Parameter animation controls value over age */
	siParamControlAbsolute	= 2,		/*!< Parameter animation controls value over simulation time */
	siParamControlAgePercentage	= 3		/*!< Parameter animation controls value over percentage of age */
    } 	siSimulationParameterAnimationControlType;

/*! Types of particle sprite sequence controls */
typedef   
enum siParticleSpriteSequenceControlType
    {	siSpriteControlSpriteControlBirth	= 0,
	siSpriteControlAge	= 1,		/*!< Sprite index increments over age over age */
	siSpriteControlAbsolute	= 2,		/*!< Sprite index increments over simulation time */
	siSpriteControlAgePercentage	= 3,		/*!< Sprite index increments over percentage of age */
	siSpriteControlRandom	= 4,		/*!< Sprite index is randomly assigned at particle birth */
	siSpriteControlJitter	= 5,		/*!< Sprite index is randomly assigned at each frame */
	siSpriteControlRenderTime	= 6		/*!< Sprite index increments over render time */
    } 	siParticleSpriteSequenceControlType;

/*! Types of particle event triggers */
typedef   
enum siParticleEventTriggerType
    {	siAgePercentage	= 0,		/*!< Event is triggered when the age percentage reaches the trigger value */
	siAge	= 1,		/*!< Event is triggered when the age reaches the trigger value */
	siEveryNParticles	= 2,		/*!< Event is triggered for every N particles emitted (N defined by the trigger value) */
	siEveryNFrames	= 3,		/*!< Event is triggered every N simulation frames (N defined by the trigger value) */
	siXPosition	= 4,		/*!< Event is triggered when a particle X position reaches the trigger value */
	siYPosition	= 5,		/*!< Event is triggered when a particle Y position reaches the trigger value */
	siZPosition	= 6,		/*!< Event is triggered when a particle Z position reaches the trigger value */
	siXSpeed	= 7,		/*!< Event is triggered when a particle velocity in X reaches the trigger value */
	siYSpeed	= 8,		/*!< Event is triggered when a particle velocity in Y reaches the trigger value */
	siZSpeed	= 9,		/*!< Event is triggered when a particle velocity in Z reaches the trigger value */
	siSpeed	= 10,		/*!< Event is triggered when a particle velocity reaches the trigger value */
	siCollision	= 11		/*!< Event is triggered when a particle collides with an obstacle */
    } 	siParticleEventTriggerType;

/*! Types of particle event actions. These values represent the Action option list in the PEvent (particle event) property page. */
typedef   
enum siParticleEventActionType
    {	siEmit	= 0,		/*!< When triggered the event will emit new particles from the trigger particles */
	siDisappear	= 1,		/*!< When triggered the event will make the trigger particles dissappear */
	siEmitAndDisappear	= 2,		/*!< When triggered the event will emit new particles from the trigger particles and make the trigger particles dissapear */
	siBounce	= 3,		/*!< When triggered the trigger particles will bounce on the obstacle (requires obstacle trigger) */
	siBounceAndEmit	= 4,		/*!< When triggered the trigger particles will bounce on the obstacle and emit new particles (requires obstacle trigger) */
	siStick	= 5,		/*!< When triggered the trigger particles will stick on the obstacle (requires obstacle trigger) */
	siScript	= 6		/*!< When triggered a script will be executed */
    } 	siParticleEventActionType;

/*! Types of obstacles */
typedef   
enum siObstacleType
    {	siObstacleBoundingPlane	= 0,
	siObstacleBoundingBox	= 1,
	siObstacleBoundingSphere	= 2,
	siObstacleActualShape	= 3
    } 	siObstacleType;

/*! Types of particle simulation modes */
typedef   
enum siParticleSimulationMode
    {	siLiveMode	= 0,		/*!< Live Mode */
	siStandardNoCachingMode	= 1,		/*!< Standard no caching mode */
	siStandardCachingMode	= 2		/*!< Standard caching mode */
    } 	siParticleSimulationMode;

/*! Enumeration of the different attributes that are available on the FCurveEditor object. */
typedef   
enum siFCurveEditorAttribute
    {	siRegionStartFrame	= 0,		/*!< If a region has been selected this returns the start frame.  If no region has been selected then 0.0 is returned. */
	siRegionEndFrame	= 1,		/*!< End frame of the region selection, or 0.0 if no region has been selected. */
	siRegionMin	= 2,		/*!< Minimum value of the keys in the region, or 0.0 if no region has been selected. */
	siRegionMax	= 3		/*!< Maximum value of the keys in the region, or 0.0 if no region has been selected. */
    } 	siFCurveEditorAttribute;

/*! Determines the severity types of a message.  They can be combined. */
typedef   
enum siSeverityType
    {	siFatalMsg	= 1,		/*!< Fatal error has occurred; possibly leading to an unstable system state. Displayed as Message Box and in History window. */
	siErrorMsg	= 2,		/*!< An error has occurred; possibly leading data loss. Displayed in History window and Status Bar. */
	siWarningMsg	= 4,		/*!< An error has occurred but is recoverable. Displayed in History window and Status Bar. */
	siInfoMsg	= 8,		/*!< Log an information message. Displayed in History window and Status Bar. */
	siVerboseMsg	= 16,		/*!< Log a diagnostic information message. Displayed in History window. */
	siCommentMsg	= 32		/*!< Log an information message without a prefix to describe its type */
    } 	siSeverityType;

/*! Information which is passed to the custom compiled operators through its Init andterm functions.*/
typedef   
enum siCustomCompiledOpFlags
    {	siDLLLoadingUnloading	= 1		/*!< Flag passed to the operator in the Init or Term to notify that the DLL is Offloading/Reloading . */
    } 	siCustomCompiledOpFlags;

/*! A device driver contains a set of channels that can be configured. This enumeration is the type of action to perform for a channel. */
typedef   
enum siChannelAction
    {	siChannelActionNone	= 0,		/*!< Nothing to do with the channel. */
	siChannelActionDrive	= 1,		/*!< Drive a parameter of a scene object. If you choose this type of action, you can set the <object idref="Channel.Target"/> property to any valid expression (for example, "current.kine.local.posx").  */
	siChannelActionCommand	= 2,		/*!< Fire a command. If you choose this type of action, you can set the <object idref="Channel.Target"/> property to the name of the command to launch through this channel. This can be either a native XSI command or a custom command.  */
	siChannelActionSpecial	= 3		/*!< Other action (Special). If you choose this type of action, you can set the <object idref="Channel.Target"/> property to any of these special keywords:TIMELINE (drives the timeline)TIME_START (goes to the first frame)TIME_END (goes to the last frame)TIME_STEP_FORWARD (goes to the next frame)TIME_STEP_BACK (goes to the previous frame)TIME_STOP (stops the playback)TIME_PLAY_NORMAL (real time playback from current frame)TIME_PLAY_FRAME (real time playback from first frame)TIME_PLAY_SLIDE (plays all frames from current frame on)TIME_SPECIFY,&lt;frame&gt;ACTIVATE,&lt;driver_ID&gt; (enables a device driver)DEACTIVATE,&lt;driver_ID&gt; (disables a device driver) */
    } 	siChannelAction;

/*! Specifies how to select objects in a hierarchy. */
typedef   
enum siSelectMode
    {	siSelectDefault	= 0,		/*!< Select the object as is. */
	siSelectNode	= 1,		/*!< Select the object in node. */
	siSelectBranch	= 2,		/*!< Select the object and its children. */
	siSelectTree	= 3,		/*!< Select the entire hierarchy of objects. */
	siSelectModel	= 4		/*!< Select the model and the hierarchy of objects. If that model is the scene root then this is the same as siSelectTree. */
    } 	siSelectMode;

/*! Specifies the filter to use in a transient explorer. */
typedef   
enum siTransientExplorerFilter
    {	siSEFilterAllNodes	= 0,		/*!< Display all nodes. */
	siSEFilterAllNodesNoParams	= 1,		/*!< Display all nodes without leaf nodes. */
	siSEFilterGeometryOperators	= 2,		/*!< Display the operators and primitives. */
	siSEFilterObjectsOnly	= 3,		/*!< Display the objects. */
	siSEFilterAllProperties	= 4,		/*!< Display all the properties. */
	siSEFilterPropertiesAndPrimitives	= 5,		/*!< Display all the properties and primitives. */
	siSEFilterMaterialsAndShaders	= 6,		/*!< Display all the materials and shaders. */
	siSEFilterModelsOnly	= 7,		/*!< Display only the models. */
	siSEFilterGroupsOnly	= 8,		/*!< Display only the groups. */
	siSEFilterImageClips	= 9		/*!< Display only the image clips. */
    } 	siTransientExplorerFilter;

/*! The class ID of XSI objects. This enumerator is used by the C++ API to manage references between the object model and C++. For example, the CRef::IsA function uses the class ID to determine whether or not the class type is compatible with the underlying reference object. */
typedef   
enum siClassID
    {	siActionSourceID	= 48,
	siAddonID	= 119,		/*!< Addon object id. */
	siAnimationSourceID	= 105,		/*!< AnimationSource object id. */
	siAnimationSourceItemID	= 146,		/*!< AnimationSourceItem object id. */
	siApplicationID	= 5,		/*!< Application object id. */
	siArgumentCollectionID	= 160,		/*!< Argument Collection id. */
	siArgumentHandlerID	= 148,		/*!< Argument Handler object id. */
	siArgumentID	= 6,		/*!< Argument object id. */
	siArrayParameterID	= 135,		/*!< ArrayParameter object id. */
	siCameraID	= 80,		/*!< Camera object id. */
	siCameraRigID	= 84,		/*!< CameraRig object id. */
	siChainBoneID	= 77,		/*!< ChainBone object id. */
	siChainEffectorID	= 78,		/*!< ChainEffector object id. */
	siChainElementID	= 75,		/*!< ChainElement object id. */
	siChainRootID	= 76,		/*!< ChainRoot object id. */
	siChannelID	= 110,		/*!< Channel object id. */
	siClipContainerID	= 147,		/*!< ClipContainer object id. */
	siClipEffectID	= 140,		/*!< ClipEffect object id. */
	siClipEffectItemID	= 141,		/*!< ClipEffectItem object id. */
	siClipID	= 8,		/*!< Clip object id. */
	siClipRelationID	= 162,		/*!< ClipRelation object id. */
	siClusterID	= 67,		/*!< Cluster object id. */
	siClusterPropertyID	= 58,		/*!< ClusterProperty object id. */
	siClusterPropertyBuilderID	= 172,		/*!< ClusterPropertyBuilder object id. */
	siCollectionItemID	= 177,		/*!< CollectionItem object id. */
	siCommandCollectionID	= 159,		/*!< Command object id. */
	siCommandID	= 7,		/*!< Command object id. */
	siCompoundParameterID	= 45,		/*!< CompoundParameter object id. */
	siConnectionPointID	= 27,		/*!< ConnectionPoint object id. */
	siConstraintID	= 59,		/*!< Constraint object id. */
	siConstraintWithUpVectorID	= 60,		/*!< ConstraintWithUpVector object id. */
	siConstructionHistoryID	= 28,		/*!< ConstructionHistory object id. */
	siContextID	= 138,		/*!< Context object id. */
	siControlPointCollectionID	= 112,		/*!< ControlPointCollection object id. */
	siControlPointID	= 14,		/*!< ControlPoint object id. */
	siCustomOperatorID	= 128,		/*!< CustomOperator object id. */
	siCustomPropertyID	= 61,		/*!< CustomProperty object id. */
	siDataSourceID	= 104,		/*!< DataSource object id. */
	siDesktopID	= 130,		/*!< Desktop object id. */
	siDeviceCollectionID	= 108,		/*!< Device collection object id. */
	siDeviceID	= 109,		/*!< Device object id. */
	siDictionaryID	= 29,		/*!< Dictionary object id. */
	siDirectedID	= 79,		/*!< Directed object id. */
	siEdgeCollectionID	= 101,		/*!< EdgeCollection object id. */
	siEdgeID	= 100,		/*!< Edge object id. */
	siEnvelopeID	= 54,		/*!< Envelope object id. */
	siEnvelopeWeightID	= 168,		/*!< EnvelopeWeight property object id. */
	siEventInfoID	= 30,		/*!< EventInfo object id. */
	siExpressionID	= 49,		/*!< Expression object id. */
	siFacetCollectionID	= 97,		/*!< FacetCollection object id. */
	siFacetID	= 11,		/*!< Facet object id. */
	siFactoryID	= 158,		/*!< Factory object id. */
	siFCurveID	= 31,		/*!< FCurve object id. */
	siFCurveKeyID	= 33,		/*!< FCurveKey object id. */
	siFileBrowserID	= 34,		/*!< FileBrowser object id. */
	siFileReferenceID	= 171,		/*!< X3DObject object id. */
	siFilterID	= 154,		/*!< Filter object id. */
	siFxOperatorID	= 90,		/*!< FxOperator object id. */
	siFxTreeID	= 89,		/*!< FxTree object id. */
	siGeometryID	= 22,		/*!< Geometry object id. */
	siGraphicSequencerContextID	= 161,		/*!< Graphic Sequencer Core id. */
	siGridDataID	= 125,		/*!< GridData object id. */
	siGridWidgetID	= 166,		/*!< GridWidget object id. */
	siGroupID	= 68,		/*!< Group object id. */
	siHairPrimitiveID	= 174,		/*!< HairPrimitive object id. */
	siImageClipID	= 50,		/*!< ImageClip object id. */
	siImageID	= 35,		/*!< Image object id. */
	siInputPortID	= 38,		/*!< InputPort object id. */
	siJointID	= 62,		/*!< Joint object id. */
	siKinematicsID	= 63,		/*!< Kinematics object id. */
	siKinematicStateID	= 86,		/*!< KinematicState object id. */
	siKnotCollectionID	= 111,		/*!< KnotCollection object id. */
	siLayerID	= 69,		/*!< Layer object id. */
	siLayoutID	= 131,		/*!< Layout object id. */
	siLibraryID	= 164,		/*!< Library id. */
	siLightID	= 81,		/*!< Light object id. */
	siLightRigID	= 85,		/*!< LightRigobject id. */
	siMappedItemID	= 36,		/*!< MappedItem object id. */
	siMaterialID	= 64,		/*!< Material object id. */
	siMaterialLibraryID	= 173,		/*!< MaterialLibrary id. */
	siMenuID	= 149,		/*!< Menu object id. */
	siMenuItemID	= 150,		/*!< Menu item object id. */
	siMeshBuilderID	= 169,		/*!< MeshBuilder object id. */
	siMixerID	= 163,		/*!< Mixer item object id. */
	siModelID	= 74,		/*!< Model object id. */
	siNestedFCurveID	= 32,		/*!< NestedFCurve object id. */
	siNullID	= 82,		/*!< Null object id. */
	siNurbsCurveCollectionID	= 113,		/*!< NurbsCurveCollection object id. */
	siNurbsCurveControlPointID	= 15,		/*!< NurbsCurveControlPoint object id. */
	siNurbsCurveID	= 18,		/*!< NurbsCurve object id. */
	siNurbsCurveListControlPointID	= 16,		/*!< NurbsCurveListControlPoint object id. */
	siNurbsCurveListID	= 24,		/*!< NurbsCurveList object id. */
	siNurbsSampleCollectionID	= 115,		/*!< NurbsSampleCollection object id. */
	siNurbsSampleID	= 20,		/*!< NurbsSample object id. */
	siNurbsSurfaceCollectionID	= 114,		/*!< NurbsSurfaceCollection object id. */
	siNurbsSurfaceControlPointID	= 17,		/*!< NurbsSurfaceControlPoint object id. */
	siNurbsSurfaceID	= 12,
	siNurbsSurfaceMeshID	= 23,		/*!< NurbsSurfaceMesh object id. */
	siObjectID	= 0,		/*!< Generic object id. */
	siOGLLightID	= 4,		/*!< OGLLight object id. */
	siOGLMaterialID	= 2,		/*!< OGLMaterial object id. */
	siOGLTextureID	= 3,		/*!< OGLTexture object id. */
	siOperatorID	= 53,		/*!< Operator object id. */
	siOperatorContextID	= 178,		/*!< Operator context object id. */
	siOutputPortID	= 106,		/*!< OuptputPort object id. */
	siParamDefID	= 157,		/*!< ParamDef object id. */
	siParameterID	= 44,		/*!< Parameter object id. */
	siParticleAttributeID	= 124,		/*!< ParticleAttribute object id. */
	siParticleCloudID	= 73,		/*!< ParticleCloud object id. */
	siParticleCloudPrimitiveID	= 56,		/*!< ParticleCloudPrimitive object id. */
	siParticleID	= 118,		/*!< Particle object id. */
	siParticleTypeID	= 123,		/*!< ParticleType object id. */
	siPassID	= 70,		/*!< Pass object id. */
	siPluginID	= 151,		/*!< Plugin object id. */
	siPluginItemID	= 152,		/*!< PluginItem object id. */
	siPluginRegistrarID	= 153,		/*!< PluginRegistrar object id. */
	siPointCollectionID	= 95,		/*!< PointCollection object id. */
	siPointID	= 13,		/*!< Point object id. */
	siPointLocatorDataID	= 176,		/*!< PointLocatorData object id. */
	siPolygonFaceCollectionID	= 103,		/*!< PolygonFaceCollection object id. */
	siPolygonFaceID	= 102,		/*!< PolygonFace object id. */
	siPolygonMeshID	= 25,		/*!< PolygonMesh object id. */
	siPolygonNodeCollectionID	= 94,		/*!< PolygonNodeCollection object id. */
	siPolygonNodeID	= 21,		/*!< PolygonNode object id. */
	siPortGroupID	= 129,		/*!< PortGroup object id. */
	siPortID	= 37,		/*!< Port object id. */
	siPPGEventContextID	= 139,		/*!< PPG Event Context id. */
	siPPGItemID	= 121,
	siPPGLayoutID	= 120,		/*!< PPG Layout object id. */
	siPreferencesID	= 122,		/*!< Preferences object id. */
	siPrimitiveID	= 55,		/*!< Primitive object id. */
	siProgressBarID	= 156,		/*!< ProgressBar object id. */
	siProjectID	= 46,		/*!< Project object id. */
	siProjectItemID	= 47,		/*!< ProjectItem object id. */
	siPropertyID	= 57,		/*!< Property object id. */
	siProxyParameterID	= 107,		/*!< Proxy Parameter object id. */
	siRenderHairAccessorID	= 175,		/*!< RenderHairAccessor object id. */
	siRigID	= 83,		/*!< Rig object id. */
	siSampleCollectionID	= 93,		/*!< SampleCollection object id. */
	siSampleID	= 19,		/*!< Sample object id. */
	siSceneID	= 51,		/*!< Scene object id. */
	siSceneItemID	= 66,		/*!< SceneItem object id. */
	siSegmentCollectionID	= 170,		/*!< SegmentCollection object id. */
	siSegmentID	= 26,		/*!< Segment object id. */
	siSelectionID	= 39,		/*!< Selection object id. */
	siShaderID	= 52,		/*!< Shader object id. */
	siShapeClipID	= 9,		/*!< ShapeClip object id. */
	siShapeKeyID	= 142,		/*!< ShapeKey object id. */
	siSimulationEnvironmentID	= 165,		/*!< SimulationEnvironment object id. */
	siSIObjectID	= 1,		/*!< SIObject object id. */
	siSourceID	= 143,		/*!< Source object id. */
	siStaticKinematicStateID	= 65,		/*!< StaticKinematicState object id. */
	siStaticSourceID	= 40,		/*!< StaticSource object id. */
	siSubComponentID	= 10,		/*!< SubComponent object id. */
	siGeometryAccessorID	= 167,		/*!< Geometry accessor object id. */
	siTextureID	= 116,		/*!< Texture object id. */
	siTextureLayerID	= 126,		/*!< TextureLayer object id. */
	siTextureLayerPortID	= 127,		/*!< TextureLayerPort object id. */
	siTimeControlID	= 144,		/*!< TimeControl object id. */
	siTimerEventID	= 179,		/*!< TimerEvent object id. */
	siTrackID	= 87,		/*!< Track object id. */
	siTransitionID	= 145,		/*!< Transition object id. */
	siTriangleCollectionID	= 91,		/*!< TriangleCollection object id. */
	siTriangleID	= 41,		/*!< Triangle object id. */
	siTriangleVertexCollectionID	= 92,		/*!< TriangleVertexCollection object id. */
	siTriangleVertexID	= 42,
	siUIObjectID	= 132,		/*!< UIObject object id. */
	siUIPersistableID	= 133,		/*!< UIPersistable object id. */
	siUIToolkitID	= 155,		/*!< UIToolkit object id. */
	siUnknownClassID	= 0,		/*!< Unknown object id. */
	siUpdateContextID	= 43,		/*!< UpdateContext object id. */
	siUserDataBlobID	= 117,		/*!< UserDataBlob object id. */
	siUserDataMapID	= 88,		/*!< UserDataMap object id. */
	siUserGroupID	= 71,		/*!< UserGroup object id. */
	siVertexCollectionID	= 99,		/*!< VertexCollection object id. */
	siVertexID	= 98,		/*!< Vertex object id. */
	siViewContextID	= 137,		/*!< X3DObject object id. */
	siViewID	= 134,		/*!< View object id. */
	siX3DObjectID	= 72		/*!< X3DObject object id. */
    } 	siClassID;

/*! This enumeration type specifies how BlendInPresets will apply the new shaders to the parameters selected by the command. */
typedef   
enum siBlendInPresetModes
    {	siBlendInPreset	= 0,		/*!< Blend the new shaders in with any existing shaders on the ports, or the parameter value if there are no shaders. */
	siReplaceAndBlendInPreset	= 1,		/*!< Remove any existing shaders from the parameters, and blend with the parameter value.  */
	siReplaceNoBlend	= 2		/*!< Remove any existing shaders and connect the new shaders directly to the parameters, without blending. */
    } 	siBlendInPresetModes;

/*! This enumeration type specifies how BlendInTextureLayerss will apply the new shaders to the parameters selected by the command. */
typedef   
enum siBlendInTextureLayersModes
    {	siBlendInTextureLayers	= 0,		/*!< Blend the new shaders in with any existing shaders on the ports, or the parameter value if there are no shaders. */
	siReplaceAndBlendInTextureLayers	= 1,		/*!< Remove any existing shaders and layers from the parameters, and blend with the parameter value.  */
	siReplaceAndNoBlendInTextureLayers	= 2		/*!< Remove any existing shaders and layers and connect the new shaders directly to the parameters, without blending. */
    } 	siBlendInTextureLayersModes;

/*! This enumeration type specifies what kind of shader port connections you are interested in, particularly for things like removal using <command idref="RemoveAllShadersFromCnxPoint"/>. */
typedef   
enum siShaderCnxPointType
    {	siShaderCnxPointNone	= 0,		/*!< Reference none of the connections on a parameter. (not normally used) */
	siShaderCnxPointBasePorts	= 1,		/*!< Reference only regular shader connections (not including texture layer port connections). "Regular connections" refers to connections of one shader to another. */
	siShaderCnxPointTextureLayerPorts	= 2,		/*!< Reference texture layer port connections only. */
	siShaderCnxPointAll	= 65535		/*!< Reference all types of shader connections, including both regular and texture layer port connections. */
    } 	siShaderCnxPointType;

/*! This enumeration type specifies the type of item supported by add-ons. Depending on which type you use with the <object idref="Addon.AddItem"/> method, you need to supply only a name, or a path and file name, or you may be able to use either as indicated below. */
typedef   
enum siAddonItemType
    {	siLayoutAddonItemType	= 1,		/*!< Layout. To specify a layout, use either the name of the layout or the full path and file name.The name appears in the File &gt; Addons &gt; Package dialog (eg., "myLayout").The full path and file name is similar to this: InstallationPath( siUserPath ) &amp; "\Data\Layouts\myLayout.xsily" */
	siToolbarAddonItemType	= 2,
	siKeyMapAddonItemType	= 3,		/*!< Key Map. To specify a keymap, use either the name of the keymap or the full path and file name.The name appears in the File &gt; Addons &gt; Package dialog (eg., "myKeyMap").The full path and file nameis similar to this: InstallationPath( siUserPath ) &amp; "\Data\Preferences\KeyMaps\myKeyMap.DSKeymap" */
	siScriptCmdAddonItemType	= 4,		/*!< Custom Command (script based or compiled). To specify a custom command, use the name of the command as it appears in the history pane of the Script Editor (eg., "myCustomCommand").  If the command is part of a self-installed plug-in use siPluginAddonItemType instead. */
	siPresetAddonItemType	= 5,		/*!< Preset. To specify a preset, use the full path and file name (eg., InstallationPath( siUserPath ) &amp; "\Data\DSPresets\Properties\MyCustomPreset.Preset").	When the Preset refers to a SPDL based custom property then the SPDL will automatically be packaged as well.  When the Preset refers to a custom compiled operator or real-time shader then both the SPDL file and binary (.dll or .so) will automatically be packaged. */
	siSpdlAddonItemType	= 6,
	siLibraryAddonItemType	= 7,
	siOtherAddonItemType	= 8,		/*!< Other kind of file (eg., an HTML or CHM file containing help pages for your application, an image file used by your application, etc.). To specify an additional file, use the full path and file name (eg., InstallationPath( siUserPath ) &amp; "\Data\VCCProject\HelpPages\MyHTMLHelp.html"). */
	siEventAddonItemType	= 9,		/*!< Event. To specify an event, use the name of the event as it appears in the File &gt; Addons &gt; Package dialog (eg., "myCustomEvent"). */
	siDeviceAddonItemType	= 10,
	siShelfAddonItemType	= 11,
	siPluginAddonItemType	= 12,		/*!< A self-installed <object idref="Plugin"/>.  This is a single script file or dll which can contain one or more Commands, Custom Properties or other items. You can specify this type of addon item either by plug-in name or by its full path. */
	siHelpAddonItemType	= 13,
	siViewAddonItemType	= 14		/*!< A custom relational view. */
    } 	siAddonItemType;

/*! Types of Columns supported by siControlGrid (see <object idref="siPPGControlType"/> and<object idref="GridData.SetColumnType"/>) */
typedef 
enum siGridWidgetColumnType
    {	siColumnStandard	= 0,		/*!< A column of text.  Numeric values are shown in decimal format without a slider. */
	siColumnBool	= 1,		/*!< A check mark is shown if the value of the zero is non-zero */
	siColumnCombo	= 2		/*!< The value of the cell is chosen from a drop down list */
    } 	siGridWidgetColumnType;

/*! Selection Mode for the cells of the GridWidget, see siUIGridSelectionMode in <object idref="siPPGItemAttribute"/>.*/
typedef 
enum siGridSelectionMode
    {	siSelectionCell	= 0,		/*!< Individual cells are selectable (Default behavior) */
	siSelectionHeader	= 1,		/*!< Entire columns or rows are selected by clicking in the header cells */
	siSelectionNone	= 2		/*!< Nothing can be selected */
    } 	siGridSelectionMode;

/*! Polygon cluster contour stretching method used to generate texture coordinates in conjunction with operator FlattenPolygonCluster. */
typedef   
enum siTxtContourStretchingMethod
    {	siContourStretchingWalkingOnMesh	= 0,		/*!< Texture coordinates obtained by accumulated distance from contours while walking along mesh edges. */
	siContourStretchingSpatialDistance	= 1,		/*!< Texture coordinates obtained by accumulated spatial distance from contours to the mesh vertices. */
	siContourStretchingReqularQuads	= 2,		/*!< Texture coordinates obtained by accumulated distances along grid lines obtained from contours. */
	siContourStretchingProjectionOnSurface	= 3		/*!< Texture coordinates obtained by projection on a parameteric surface built from contours. */
    } 	siTxtContourStretchingMethod;

/*! The list of scopes supported by the Scene Explorer. */
typedef   
enum siExplorerScope
    {	siExplorerScopeDefault	= 0,		/*!< Default scope. */
	siExplorerScopeProject	= 1,		/*!< Project (Content Container) scope. */
	siExplorerScopePreferences	= 2,		/*!< Preferences scope. */
	siExplorerScopeScene	= 3,		/*!< Scene scope. */
	siExplorerScopeLayers	= 4,		/*!< Layers scope. */
	siExplorerScopePasses	= 5,		/*!< Passes scope. */
	siExplorerScopeEnvironments	= 6,		/*!< Environments scope. */
	siExplorerScopeParTypes	= 7,		/*!< Particle Types scope. */
	siExplorerScopeSelection	= 8,		/*!< Selection scope. */
	siExplorerScopeCustom	= 9,		/*!< Custom scope. */
	siExplorerScopeCurrentLayer	= 10,		/*!< Current Layer scope. */
	siExplorerScopeCurrentPass	= 11,		/*!< Current Pass scope. */
	siExplorerScopeCurrentEnvironment	= 12,		/*!< Current Environment scope. */
	siExplorerScopeCurrentParType	= 13,		/*!< Current Particle Type scope. */
	siExplorerScopeSourcesClips	= 14,		/*!< Sources and Clips scope. */
	siExplorerScopeSourcesClipsAnimation	= 15,		/*!< Sources and Clips (Animation Only) scope. */
	siExplorerScopeSourcesClipsAudio	= 16,		/*!< Sources and Clips (Audio Only) scope. */
	siExplorerScopeSourcesClipsImages	= 17,		/*!< Sources and Clips (Images Only) scope. */
	siExplorerScopeSourcesClipsMaterials	= 18,		/*!< Sources and Clips (Materials Only) scope. */
	siExplorerScopeSourcesClipsModels	= 19,		/*!< Sources and Clips (Models Only) scope. */
	siExplorerScopeCommands	= 20		/*!< This scope is not currently supported. */
    } 	siExplorerScope;

/*! States for the time change notification for custom views. */
typedef   
enum siTimeState
    {	siTimeStateNull	= 0,		/*!< Null state. */
	siTimeStateStop	= 0x1,		/*!< Stop state. */
	siTimeStatePreStop	= 0x2,		/*!< Pre-Stop state. */
	siTimeStatePlay	= 0x4,		/*!< Play state. */
	siTimeStatePrePlay	= 0x8,		/*!< Pre-Play state. */
	siTimeStateRecord	= 0x10,		/*!< Record state. */
	siTimeStatePreRecord	= 0x20,		/*!< Pre-Record state. */
	siTimeStateFast	= 0x40,		/*!< Fast state. */
	siTimeStatePreFast	= 0x80,		/*!< Pre-Fast state. */
	siTimeStatePreRoll	= 0x100,		/*!< Pre-Roll state. */
	siTimeStateStep	= 0x200,		/*!< Step state. */
	siTimeStatePostStep	= 0x400,		/*!< Post-Step state. */
	siTimeStatePause	= 0x800,		/*!< Pause state. */
	siTimeStatePrePause	= 0x1000,		/*!< Pre-Pause state. */
	siTimeStateSeek	= 0x2000,		/*!< Seek state. */
	siTimeNotifyTime	= 0x4000,		/*!< Time notification. */
	siTimeNotifyScrub	= 0x10000,		/*!< Scrub notification. */
	siTimeNotifySeek	= 0x20000,		/*!< Seek notification. */
	siTimeNotifyPortChange	= 0x40000,		/*!< Port Change notification. */
	siTimeNotifyFrameDrop	= 0x80000		/*!< Frame Drop notification. */
    } 	siTimeState;

/*! Types of display callbacks used by the Open Graphic Sequencer. */
typedef   
enum siDisplayCallback
    {	siUndefined	= 0,		/*!< Undefined. */
	siPreBeginFrame	= 1,		/*!< called just before BeginFrame. */
	siBeginFrame	= 2,		/*!< called when a new frame begins. */
	siPostBeginFrame	= 3,		/*!< called after the frame has begun. */
	siPrePass	= 4,		/*!< called before each pass. */
	siPass	= 5,		/*!< XGS display pass callback. */
	siPostPass	= 6,		/*!< called after each pass. */
	siPreEndFrame	= 7,		/*!< called just before the frame ends. */
	siEndFrame	= 8,		/*!< called when the frame ends. */
	siPostEndFrame	= 9,		/*!< called after the frame has ended. */
	siRenderFrame	= 10		/*!< called after MentalRay renders a frame, or after a render reguion is finished rendering a frame. */
    } 	siDisplayCallback;

/*! Context flags for the Graphic Sequencer. */
typedef 
enum siXGSContextFlag
    {	siXGSContextNormal	= 1 << 1,		/*!< Normal 3D viewport context. */
	siXGSContextCapture	= 1 << 2,		/*!< Capture rendering context. */
	siXGSContextHWRender	= 1 << 3		/*!< Hardware rendering context. */
    } 	siXGSContextFlag;

/*! Type of object passed to an Open Graphic Sequencer callback. */
typedef   
enum siXGSClassType
    {	siXGSPluginManager	= 0		/*!< The Open Graphic Sequencer plug-in manager. */
    } 	siXGSClassType;

/*! Flags for XGS callbacks. */
typedef   
enum siXGSFlag
    {	siInactive	= 0,		/*!< Callback inactive. */
	siActive	= 1		/*!< Callback active. */
    } 	siXGSFlag;

/*! Pass flags for XGS callbacks. */
typedef   
enum siXGSPassFlag
    {	siInsertBefore	= 0,		/*!< Insert before a callback. */
	siInsertAfter	= 1,		/*!< Insert after a callback. */
	siInsertBeforeAll	= 2,		/*!< Insert before all callbacks. */
	siInsertAfterAll	= 3		/*!< Insert after all callbacks. */
    } 	siXGSPassFlag;

/*! XGS state types. */
typedef   
enum siXGSStateType
    {	siXGSUnknown	= 0,		/*!< Unknown type. */
	siXGSInteger	= 1,		/*!< Integer state type. */
	siXGSFloat	= 2,		/*!< Float state type. */
	siXGSHandle	= 3,		/*!< Handle state type. */
	siXGSVector4	= 4,		/*!< Vector4 state type. */
	siXGSMatrix4	= 5		/*!< Matrix4 state type. */
    } 	siXGSStateType;

/*! XGS render flags. */
typedef 
enum siXGSRenderFlag
    {	siRenderDefault	= 1 << 1,		/*!< Render normally. */
	siRenderUnsorted	= 1 << 2,		/*!< Render unsorted objects. */
	siRenderSorted	= 1 << 3,		/*!< Render unsorted objects. */
	siRenderSortedNear	= 1 << 4,		/*!< Render objects sorted by nearest point in bounding box. */
	siRenderSortedCenter	= 1 << 5,		/*!< Render objects sorted by center of bounding box. */
	siRenderSortedFar	= 1 << 6,		/*!< Render objects sorted by farthest point of bounding box. */
	siRenderSortFront	= 1 << 7,		/*!< Sorted object are rendered from front to back (instead of the default back to front). */
	siRenderSelected	= 1 << 8,
	siTexEnvReplace	= 1 << 9,
	siRenderNoRTSOverride	= 1 << 10
    } 	siXGSRenderFlag;

/*! Viewport display modes. */
typedef   
enum siViewMode
    {	siAll	= 0,		/*!< All modes. */
	siBoundingBoxMode	= 12,		/*!< Bounding box. */
	siWireframe	= 13,		/*!< Wireframe. */
	siDepthCue	= 14,		/*!< Depth Cue. */
	siHiddenLineRemoval	= 15,		/*!< Hidden Line Removal. */
	siConstant	= 16,		/*!< Constant. */
	siShaded	= 17,		/*!< Shaded. */
	siTextured	= 18,		/*!< Textured. */
	siTexturedDecal	= 19,		/*!< Textured Decal. */
	siCustom	= 20,		/*!< Custom view mode. */
	siRealtimePortMaterial	= 21		/*!< Custom view mode. */
    } 	siViewMode;

/*! The available types for user defined per particle <object idref="ParticleAttribute"/>. */
typedef   
enum siParticleAttributeType
    {	siPAVector4	= 0,		/*!< Vector that contains 4 double precision floating point type. */
	siPAVector3	= 1,		/*!< Vector that contains 3 double precision floating point type. */
	siPAFloat	= 2,		/*!< Floating point type. */
	siPAInt	= 3,		/*!< Integer type. */
	siPAULong	= 4,		/*!< ULONG integer type. */
	siPAUShort	= 5,		/*!< Unsigned short integer type. */
	siPABool	= 6,		/*!< Boolean type. */
	siPAUndefined	= 7		/*!< Error type returned when requesting the type for a non-defined attribute. */
    } 	siParticleAttributeType;

/*! This is the enumeration of all the different types of storage a source can have. */
typedef   
enum siSourceStorageType
    {	siSourceStorageTypeInternal	= 0,		/*!< The source is saved in the scene file. */
	siSourceStorageTypeExternalDotXSIBin	= 1,		/*!< The source is saved in a dotXSI file (.xsi) of type binary. */
	siSourceStorageTypeExternalDotXSIText	= 2,		/*!< The source is saved in a dotXSI file (.xsi) of type Text. */
	siSourceStorageTypeExternalBin	= 3		/*!< The source is saved in a native XSI file format but in a separate file of type .eani. */
    } 	siSourceStorageType;

/*! This represents the different persistence status that the <object idref="ActionSource"/> can have. */
typedef   
enum siActionStatus
    {	siActionStatusOffloaded	= 0,		/*!< The action source is offloaded on disk. */
	siActionStatusPartiallyLoaded	= 1,		/*!< The action source contains section that are offloaded. See method <object idref="ActionSource.PartialOffload"/> */
	siActionStatusLoaded	= 2		/*!< The action source is loaded. */
    } 	siActionStatus;

/*! Specifies the different type of filters that are used for implementing and categorizing custom <object idref="Filter"/>s. Custom filters of types 3D object, SubComponentEdge, SubComponentPoint, SubComponentPolygon and Property are automatically added to the Main Command Panel filter menus and integrated with the selection tool. */
typedef   
enum siFilterType
    {	siFilterUndefined	= 0,		/*!< Undefined filter type. */
	siFilterObject	= 1,		/*!< Represents generic filters that are not object specifics. */
	siFilter3DObject	= 2,		/*!< Kind of filter used with 3D objects. */
	siFilterProperty	= 3,		/*!< Kind of filter used with property objects. */
	siFilterSubComponentEdge	= 4,		/*!< Kind of filter used with the edge sub components of a geometry. */
	siFilterSubComponentPoint	= 5,		/*!< Kind of filter used with the point sub components of a geometry. */
	siFilterSubComponentPolygon	= 6,		/*!< Kind of filter used with the polygon sub components of a geometry. */
	siFilterSubComponentKnot	= 7		/*!< Kind of filter used with the knot sub components of a geometry. */
    } 	siFilterType;

/*! Determines the capabilities of a command. */
typedef   
enum siCommandCapabilities
    {	siSupportsKeyAssignment	= 8,		/*!< Supports key assignment. This flag is on by default for any Custom Command. */
	siCannotBeUsedInBatch	= 256,		/*!< Cannot be used in batch mode.  Use this flag to tag commands that rely on user input and cannot be executed in batch mode. */
	siNoLogging	= 8388608,
	siAllowNotifications	= 1073741824
    } 	siCommandCapabilities;

/*! Determines the severity types of a message.  They can be combined. */
typedef   
enum siSeverity
    {	siFatal	= 1,		/*!< Fatal error has occurred; possibly leading to an unstable system state. Displayed as Message Box and in History window. */
	siError	= 2,		/*!< An error has occurred; possibly leading data loss. Displayed in History window and Status Bar. */
	siWarning	= 4,		/*!< An error has occurred but is recoverable. Displayed in History window and Status Bar. */
	siInfo	= 8,		/*!< Log an information message. Displayed in History window and Status Bar. */
	siVerbose	= 16,		/*!< Log a diagnostic information message. Displayed in History window. */
	siComment	= 32		/*!< Log an information message without adding a prefix to describe its type */
    } 	siSeverity;

/*! Flags values supported by the <object idref="XSIUIToolkit.MsgBox"/>command. These are identical the arguments supported by the vbscript MsgBox and Win32 ::MessageBox functions. */
typedef   
enum siMsgBoxFlags
    {	siMsgOkOnly	= 0,		/*!< Display OK button only. */
	siMsgOkCancel	= 1,		/*!< Display OK and Cancel buttons. */
	siMsgAbortRetryIgnore	= 2,		/*!< Display Abort, Retry, and Ignore buttons. */
	siMsgYesNoCancel	= 3,		/*!< Display Yes, No, and Cancel buttons. */
	siMsgYesNo	= 4,		/*!< Display Yes and No buttons. */
	siMsgRetryCancel	= 5,		/*!< Display Retry and Cancel buttons. */
	siMsgCritical	= 16,		/*!< Display Critical Message icon.  */
	siMsgQuestion	= 32,		/*!< Display Query icon. */
	siMsgExclamation	= 48,		/*!< Display Warning Message icon. */
	siMsgInformation	= 64,		/*!< Display Information Message icon. */
	siMsgDefaultButton1	= 0,		/*!< First button is default. */
	siMsgDefaultButton2	= 256,		/*!< Second button is default. */
	siMsgDefaultButton3	= 512		/*!< Third button is default. */
    } 	siMsgBoxFlags;

/*! Return values from <object idref="XSIUIToolkit.MsgBox"/> thatindicates which button was pressed by the user. */
typedef   
enum siMsgButtonPressed
    {	siMsgOk	= 1,		/*!< OK */
	siMsgCancel	= 2,		/*!< Cancel */
	siMsgAbort	= 3,		/*!< Abort */
	siMsgRetry	= 4,		/*!< Retry */
	siMsgIgnore	= 5,		/*!< Ignore */
	siMsgYes	= 6,		/*!< Yes */
	siMsgNo	= 7		/*!< No */
    } 	siMsgButtonPressed;

/*! Specifies the view state information of the <object idref="View"/> object, such as whether the view is closed, whether the view is minimized, and whether the view is in a normal state. */
typedef   
enum siViewState
    {	siNormal	= 0,		/*!< Normal */
	siClosed	= 1,		/*!< Closed  */
	siMinimized	= 2		/*!< Minimized */
    } 	siViewState;

/*! Types of toolbar controls to use when building a toolbar. Toolbar controls help you to tweak the appearance of your toolbar by adding some cosmetic features, such as separators and line breaks. */
typedef   
enum siToolbarControl
    {	siTBCtrlBlankSeparator	= 0,		/*!< blank separator */
	siTBCtrlGroupHeader	= 1,		/*!< group header */
	siTBCtrlLineBreak	= 2,		/*!< line break */
	siTBCtrlMenu	= 3,		/*!< cascading command menu button */
	siTBCtrlPreset	= 4,		/*!< cascading command preset menu button */
	siTBCtrlLineSeparator	= 5		/*!< line separator */
    } 	siToolbarControl;

/*! Specifies the type of time applicability of a <object idref="ClipEffect"/> on a <object idref="Clip"/>. */
typedef   
enum siTimeReferenceType
    {	siOriginalClip	= 0,		/*!< The effect will repeat itself over time. */
	siExtrapolatedClip	= 1		/*!< the effect	spans over extrapolation. */
    } 	siTimeReferenceType;

/*! Specifies a reduction to apply to an image. For example a 800x400 image reduced by a 4x1 ratio will have a resolution of 200x100. */
typedef   
enum siImageRatio
    {	siImageRatio1x1	= 0,		/*!< Full resolution issue. */
	siImageRatio2x1	= 1,		/*!< Image reduced to 1/2 resolution (e.g. 1/4 the memory consumption). */
	siImageRatio4x1	= 2,		/*!< Image reduced to 1/4 resolution. */
	siImageRatio8x1	= 3,		/*!< Image reduced to 1/8 resolution. */
	siImageRatio16x1	= 4,		/*!< Image reduced to 1/16 resolution. */
	siImageRatioThumbnail	= 5		/*!< Image reduced to Thumbnail resolution. */
    } 	siImageRatio;

/*! The siTimeControlExtrapolationType determines how a <object idref="Clip"/> contributes to the animation outside of the frames on which it is defined. */
typedef   
enum siTimeControlExtrapolationType
    {	siTimeControlExtrapolationNone	= 0,		/*!< Results in no extrapolation. Use this to delete an extrapolation. */
	siTimeControlExtrapolationHold	= 1,		/*!< Results in the first or last values being held for the number of frames specified by <object idref="TimeControl.ExtrapolationBeforeValue"/> or <object idref="TimeControl.ExtrapolationAfterValue"/>. */
	siTimeControlExtrapolationCycle	= 2,		/*!< Repeats the clip for the number of times specified by <object idref="TimeControl.ExtrapolationBeforeValue"/> or <object idref="TimeControl.ExtrapolationAfterValue"/>. */
	siTimeControlExtrapolationBounce	= 3		/*!< Repeats the clip forward, then backward, and so on for the number of times specified by <object idref="TimeControl.ExtrapolationBeforeValue"/> or <object idref="TimeControl.ExtrapolationAfterValue"/>.  */
    } 	siTimeControlExtrapolationType;

/*! Specifies the styles of a menu item. */
typedef   
enum siMenuItemStyle
    {	siMenuItemUndefined	= 0,		/*!< Undefined menu item style. */
	siMenuItem	= siMenuItemUndefined + 1,		/*!< Regular menu item. */
	siMenuItemSubmenu	= siMenuItem + 1,		/*!< Defines item as a submenu. */
	siMenuItemSeparator	= siMenuItemSubmenu + 1		/*!< Defines item as a separator. */
    } 	siMenuItemStyle;

/*! Represents the XSI menu anchor points. */
typedef   
enum siMenuAnchorPoints
    {	siMenuUndefinedID	= 0,		/*!< Undefined menu id. */
	siMenuTbGetPrimitiveCurveID	= 1,		/*!< Primitive curve menu (model toolbar). */
	siMenuTbGetPrimitivePolygonMeshBasicID	= siMenuTbGetPrimitiveCurveID + 1,		/*!< Basic section in the primitive polyggon mesh menu (model toolbar). */
	siMenuTbGetPrimitivePolygonMeshHedronID	= siMenuTbGetPrimitivePolygonMeshBasicID + 1,		/*!< Hedron section in the primitive polyggon mesh menu (model toolbar). */
	siMenuTbGetPrimitivePolygonMeshID	= siMenuTbGetPrimitivePolygonMeshHedronID + 1,		/*!< Primitive polyggon mesh menu (model toolbar). */
	siMenuTbGetPrimitiveSurfaceID	= siMenuTbGetPrimitivePolygonMeshID + 1,		/*!< Primitive surface menu (model toolbar). */
	siMenuTbGetPrimitiveImplicitCurveID	= siMenuTbGetPrimitiveSurfaceID + 1,		/*!< Curve section in the primitive implicit menu (model toolbar). */
	siMenuTbGetPrimitiveImplicitID	= siMenuTbGetPrimitiveImplicitCurveID + 1,		/*!< Primitive implicit menu (model toolbar). */
	siMenuTbGetPrimitiveControlObjectID	= siMenuTbGetPrimitiveImplicitID + 1,		/*!< Primitive control object menu (model toolbar). */
	siMenuTbGetPrimitiveModelID	= siMenuTbGetPrimitiveControlObjectID + 1,		/*!< Primitive model menu (model toolbar). */
	siMenuTbGetPrimitiveID	= siMenuTbGetPrimitiveModelID + 1,		/*!< Primitive menu (model toolbar). */
	siMenuTbGetCameraID	= siMenuTbGetPrimitiveID + 1,		/*!< Camera menu (model toolbar). */
	siMenuTbGetLightID	= siMenuTbGetCameraID + 1,		/*!< Light menu (model toolbar). */
	siMenuTbGetMaterialID	= siMenuTbGetLightID + 1,		/*!< Material menu (model toolbar). */
	siMenuTbGetMaterialBasicID	= siMenuTbGetMaterialID + 1,		/*!< Basic section in the material menu (model toolbar). */
	siMenuTbGetMaterialSimpleID	= siMenuTbGetMaterialBasicID + 1,		/*!< Simple section in the material menu (model toolbar). */
	siMenuTbGetMaterialRealTimeID	= siMenuTbGetMaterialSimpleID + 1,		/*!< Realtime section in the material menu (model toolbar). */
	siMenuTbGetPropertyID	= siMenuTbGetMaterialRealTimeID + 1,		/*!< Property menu (model toolbar). */
	siMenuTbGetPropertyTextureProjectionID	= siMenuTbGetPropertyID + 1,		/*!< Texture projection menu in the property menu (model toolbar). */
	siMenuTbGetPropertyTextureMapID	= siMenuTbGetPropertyTextureProjectionID + 1,		/*!< Texture map menu in the property menu (model toolbar). */
	siMenuTbGetPropertyTextureID	= siMenuTbGetPropertyTextureMapID + 1,		/*!< Texture section in the property menu (model toolbar). */
	siMenuTbGetPropertyUserDataMapID	= siMenuTbGetPropertyTextureID + 1,		/*!< Data map menu in the property menu (model toolbar). */
	siMenuTbGetPropertyMapPaintID	= siMenuTbGetPropertyUserDataMapID + 1,		/*!< Map paint menu in the property menu (model toolbar). */
	siMenuTbGetPropertyPSetID	= siMenuTbGetPropertyMapPaintID + 1,		/*!< Property set section in the property menu (model toolbar). */
	siMenuTbDeformSpatialID	= siMenuTbGetPropertyPSetID + 1,		/*!< Spatial section in the deform menu (model toolbar). */
	siMenuTbDeformID	= siMenuTbDeformSpatialID + 1,		/*!< Deform menu (model toolbar). */
	siMenuTbCreateSkeletonID	= siMenuTbDeformID + 1,		/*!< Skeleton menu (model toolbar). */
	siMenuTbCreateSkeletonDrawID	= siMenuTbCreateSkeletonID + 1,		/*!< Draw section in the skeleton menu (model toolbar). */
	siMenuTbCreateSkeletonInspectID	= siMenuTbCreateSkeletonDrawID + 1,		/*!< Inspect section in the skeleton menu (model toolbar). */
	siMenuTbModelCreateCurveID	= 100,		/*!< Create curve menu (model toolbar). */
	siMenuTbModelCreateSurfaceMeshID	= siMenuTbModelCreateCurveID + 1,		/*!< Create surface mesh menu (model toolbar). */
	siMenuTbModelCreatePolygonMeshID	= siMenuTbModelCreateSurfaceMeshID + 1,		/*!< Create polygon mesh menu (model toolbar). */
	siMenuTbModelCreateModelID	= siMenuTbModelCreatePolygonMeshID + 1,		/*!< Create model menu (model toolbar). */
	siMenuTbModelCreateTextID	= siMenuTbModelCreateModelID + 1,		/*!< Create text menu (model toolbar). */
	siMenuTbModelModifyComponentID	= siMenuTbModelCreateTextID + 1,		/*!< Modify component menu (model toolbar). */
	siMenuTbModelModifyCurveID	= siMenuTbModelModifyComponentID + 1,		/*!< Modify curve menu (model toolbar). */
	siMenuTbModelModifySurfaceMeshID	= siMenuTbModelModifyCurveID + 1,		/*!< Modify surface mesh menu (model toolbar). */
	siMenuTbModelModifyPolygonMeshID	= siMenuTbModelModifySurfaceMeshID + 1,		/*!< Modify polygon mesh menu (model toolbar). */
	siMenuTbModelModifyModelID	= siMenuTbModelModifyPolygonMeshID + 1,		/*!< Modify model menu (model toolbar). */
	siMenuTbAnimateCreateParameterID	= 200,		/*!< Create parameter menu (animate toolbar). */
	siMenuTbAnimateCreateParameterCustomPSetID	= siMenuTbAnimateCreateParameterID + 1,		/*!< Custom pset section in the parameter menu (animate toolbar). */
	siMenuTbAnimateCreateParameterCustomParamID	= siMenuTbAnimateCreateParameterCustomPSetID + 1,		/*!< Custom parameter section in the parameter menu (animate toolbar). */
	siMenuTbAnimateCreateParameterExpressionID	= siMenuTbAnimateCreateParameterCustomParamID + 1,		/*!< Expression section in the parameter menu (animate toolbar). */
	siMenuTbAnimateCreateParameterProxyID	= siMenuTbAnimateCreateParameterExpressionID + 1,		/*!< Proxy parameter section in the parameter menu (animate toolbar). */
	siMenuTbAnimateCreateCurveID	= siMenuTbAnimateCreateParameterProxyID + 1,		/*!< Curve menu (animate toolbar). */
	siMenuTbAnimateCreatePathID	= siMenuTbAnimateCreateCurveID + 1,		/*!< Path menu (animate toolbar). */
	siMenuTbAnimateCreateCharacterID	= siMenuTbAnimateCreatePathID + 1,		/*!< Character menu (animate toolbar). */
	siMenuTbAnimateDeformShapeID	= siMenuTbAnimateCreateCharacterID + 1,		/*!< Shape menu (animate toolbar). */
	siMenuTbAnimateDeformShapeKeyID	= siMenuTbAnimateDeformShapeID + 1,		/*!< Shape key section in the shape menu (animate toolbar). */
	siMenuTbAnimateDeformEnvelopeID	= siMenuTbAnimateDeformShapeKeyID + 1,		/*!< Envelope menu (animate toolbar). */
	siMenuTbAnimateActionsStoreID	= siMenuTbAnimateDeformEnvelopeID + 1,		/*!< Action store menu (animate toolbar). */
	siMenuTbAnimateActionsApplyID	= siMenuTbAnimateActionsStoreID + 1,		/*!< Action apply menu (animate toolbar). */
	siMenuTbAnimateActionsTemplatesID	= siMenuTbAnimateActionsApplyID + 1,		/*!< Action templates menu (animate toolbar). */
	siMenuTbAnimateToolsPlotID	= siMenuTbAnimateActionsTemplatesID + 1,		/*!< Plot menu (animate toolbar). */
	siMenuTbAnimateToolsDevicesID	= siMenuTbAnimateToolsPlotID + 1,		/*!< Devices menu (animate toolbar). */
	siMenuTbAnimateToolsImportID	= siMenuTbAnimateToolsDevicesID + 1,		/*!< Import section in the import/export menu (animate toolbar). */
	siMenuTbAnimateToolsExportID	= siMenuTbAnimateToolsImportID + 1,		/*!< Export section in the import/export menu (animate toolbar). */
	siMenuTbRenderRegionID	= 300,		/*!< Render region menu (render toolbar). */
	siMenuTbRenderPreviewID	= siMenuTbRenderRegionID + 1,		/*!< Render preview menu (render toolbar). */
	siMenuTbRenderRenderID	= siMenuTbRenderPreviewID + 1,		/*!< Render menu (render toolbar). */
	siMenuTbRenderExportMIID	= siMenuTbRenderRenderID + 1,		/*!< Render export to MI2 menu (render toolbar). */
	siMenuTbRenderPassEditID	= siMenuTbRenderExportMIID + 1,		/*!< Render pass edit menu (render toolbar). */
	siMenuTbRenderPassEditNewPassID	= siMenuTbRenderPassEditID + 1,		/*!< New pass section in the render pass edit menu (render toolbar). */
	siMenuTbRenderPassPartitionID	= siMenuTbRenderPassEditNewPassID + 1,		/*!< Render pass partition menu (render toolbar). */
	siMenuTbGetForceID	= 400,		/*!< Force menu (simulate toolbar). */
	siMenuTbCreateParticlesID	= siMenuTbGetForceID + 1,		/*!< Create particles menu (simulate toolbar). */
	siMenuTbCreateParticlesFluidID	= siMenuTbCreateParticlesID + 1,		/*!< Fluid menu in the create particles menu (simulate toolbar). */
	siMenuTbCreateParticlesExplosionID	= siMenuTbCreateParticlesFluidID + 1,		/*!< Explosion menu in the create particles menu (simulate toolbar). */
	siMenuTbCreateParticlesNewPTypeID	= siMenuTbCreateParticlesExplosionID + 1,		/*!< New particle type menu in the create particles menu (simulate toolbar). */
	siMenuTbCreateRigidBodyID	= siMenuTbCreateParticlesNewPTypeID + 1,		/*!< Rigid body menu (simulate toolbar). */
	siMenuTbCreateRigidBodyConstraintID	= siMenuTbCreateRigidBodyID + 1,		/*!< Rigid constraint menu in the create rigid body menu (simulate toolbar). */
	siMenuTbCreateClothID	= siMenuTbCreateRigidBodyConstraintID + 1,		/*!< Create cloth menu (simulate toolbar). */
	siMenuTbModifyParticlesID	= siMenuTbCreateClothID + 1,		/*!< Modify particles menu (simulate toolbar). */
	siMenuTbModifyRigidBodyID	= siMenuTbModifyParticlesID + 1,		/*!< Modify rigid body menu (simulate toolbar). */
	siMenuTbModifyEnvironmentID	= siMenuTbModifyRigidBodyID + 1,		/*!< Modify environment menu (simulate toolbar). */
	siMenuMainFileSceneID	= 1000,		/*!< Scene section in the main file menu. */
	siMenuMainFileProjectID	= siMenuMainFileSceneID + 1,		/*!< Project section in the main file menu. */
	siMenuMainFileExportID	= siMenuMainFileProjectID + 1,		/*!< Export menu in the main file menu. */
	siMenuMainFileImportID	= siMenuMainFileExportID + 1,		/*!< Import menu in the main file menu. */
	siMenuMainWindowID	= siMenuMainFileImportID + 1,		/*!< Windows menu in the main menu bar. */
	siMenuMainHelpID	= siMenuMainWindowID + 1,		/*!< Help menu in the main menu bar. */
	siMenuMainApplicationID	= siMenuMainHelpID + 1,		/*!< Application menu in the main menu bar. */
	siMenuMainApplicationPropertySetsID	= siMenuMainApplicationID + 1,		/*!< End of "Property Set" section in main Application menu. */
	siMenuMainApplicationLayoutsID	= siMenuMainApplicationPropertySetsID + 1,		/*!< End of "Layouts" section in main Application menu. */
	siMenuMainApplicationViewsID	= siMenuMainApplicationLayoutsID + 1,		/*!< End of "Views" section in main Application menu. */
	siMenuMainApplicationToolbarsID	= siMenuMainApplicationViewsID + 1,		/*!< End of "Toolbars" section in main Application menu. */
	siMenuMainApplicationCommandsID	= siMenuMainApplicationToolbarsID + 1,		/*!< End of "Commands" section in main Application menu. */
	siMenuMainApplicationKeyMappingsID	= siMenuMainApplicationCommandsID + 1,		/*!< End of "Key Mappings" section in main Application menu. */
	siMenuMainApplicationPreferencesID	= siMenuMainApplicationKeyMappingsID + 1,		/*!< "Preferences" section in main File menu. */
	siMenuMainTopLevelID	= siMenuMainApplicationPreferencesID + 1,		/*!< Menu item in the main menu bar. Menus registered with this identifier are added to the XSI main menu bar as top level menus. */
	siMenuMCPSelectTopID	= 2000,		/*!< Top of Select menu in the MCP. */
	siMenuMCPSelectBottomID	= siMenuMCPSelectTopID + 1,		/*!< Bottom of Select menu in the MCP. */
	siMenuMCPSelectExploreBtnID	= siMenuMCPSelectBottomID + 1,		/*!< Explore menu in the MCP. */
	siMenuMCPSelectSelBtnContextID	= siMenuMCPSelectExploreBtnID + 1,		/*!< Context menu displayed when a user right-clicks the Selection button in the MCP. */
	siMenuMCPTransformTopID	= siMenuMCPSelectSelBtnContextID + 1,		/*!< Top of Transform menu in the MCP. */
	siMenuMCPTransformBottomID	= siMenuMCPTransformTopID + 1,		/*!< Bottom of Transform menu in the MCP. */
	siMenuMCPSnapID	= siMenuMCPTransformBottomID + 1,		/*!< Snap menu in the MCP. */
	siMenuMCPConstrainID	= siMenuMCPSnapID + 1,		/*!< Constrain menu in the MCP. */
	siMenuMCPEditID	= siMenuMCPConstrainID + 1,		/*!< Edit menu in the MCP. */
	siMenuMCPAnimationID	= siMenuMCPEditID + 1,		/*!< Animation menu in the MCP. */
	siMenuMCPPlaybackID	= siMenuMCPAnimationID + 1,		/*!< Playback menu in the MCP. */
	siMenuVMViewTypeID	= 3000,		/*!< View type menu in the View Manager. */
	siMenuVMCameraID	= siMenuVMViewTypeID + 1,		/*!< Camera menu in the View Manager. */
	siMenuVMVisibilityID	= siMenuVMCameraID + 1,		/*!< Visibility menu in the View Manager. */
	siMenuVMDisplayID	= siMenuVMVisibilityID + 1,		/*!< Display menu in the View Manager. */
	siMenu3DViewClusterContextID	= 3100,		/*!< Cluster section in the 3D view contextual menu. */
	siMenu3DViewComponentContextID	= siMenu3DViewClusterContextID + 1,		/*!< Sub-component section in the 3D view contextual menu. */
	siMenu3DViewComponentEditContextID	= siMenu3DViewComponentContextID + 1,		/*!< Sub-component edit section in the 3D view contextual menu. */
	siMenu3DViewGeneralContextID	= siMenu3DViewComponentEditContextID + 1,		/*!< General section in the 3D view contextual menu. */
	siMenu3DViewObjectContextID	= siMenu3DViewGeneralContextID + 1,		/*!< Object section in the 3D view contextual menu. */
	siMenu3DViewObjectEditContextID	= siMenu3DViewObjectContextID + 1,		/*!< Object edit section in the 3D view contextual menu. */
	siMenu3DViewObjectGroupContextID	= siMenu3DViewObjectEditContextID + 1,		/*!< Object group section in the 3D view contextual menu. */
	siMenu3DViewObjectMeshContextID	= siMenu3DViewObjectGroupContextID + 1,		/*!< Object mesh section in the 3D view contextual menu. */
	siMenu3DViewObjectSelectContextID	= siMenu3DViewObjectMeshContextID + 1,		/*!< Object selection section in the 3D view contextual menu. */
	siMenuSEGeneralContextID	= 3200,		/*!< General contextual menu in the Scene Explorer. */
	siMenuSELayersContextID	= siMenuSEGeneralContextID + 1,		/*!< Layer contextual menu in the Scene Explorer. */
	siMenuSEPassesContextID	= siMenuSELayersContextID + 1,		/*!< Pass contextual menu in the Scene Explorer. */
	siMenuSEPartitionsContextID	= siMenuSEPassesContextID + 1,		/*!< Partition contextual menu in the Scene Explorer. */
	siMenuSEObjectContextID	= siMenuSEPartitionsContextID + 1,		/*!< Object contextual menu in the Scene Explorer. */
	siMenuSEGroupContextID	= siMenuSEObjectContextID + 1,		/*!< Group contextual menu in the Scene Explorer. */
	siMenuSEAnimContextID	= siMenuSEGroupContextID + 1,		/*!< Animation contextual menu in the Scene Explorer. */
	siMenuSEClusterContextID	= siMenuSEAnimContextID + 1,		/*!< Cluster contextual menu in the Scene Explorer. */
	siMenuSEOperatorContextID	= siMenuSEClusterContextID + 1,		/*!< Operator contextual menu in the Scene Explorer. */
	siMenuSEConstraintContextID	= siMenuSEOperatorContextID + 1,		/*!< Constraint contextual menu in the Scene Explorer. */
	siMenuSEPreferenceContextID	= siMenuSEConstraintContextID + 1,		/*!< Preference contextual menu in the Scene Explorer. */
	siMenuPluginMgrPluginFileID	= 3300,		/*!< File Menu in the Plugin Manager Plug-in and Plug-in Item views.  The "target" attribute of the <object idref="Context"/> argument contains the Plug-in Manager <object idref="View"/>. */
	siMenuPluginMgrTreeFileID	= siMenuPluginMgrPluginFileID + 1,		/*!< File Menu in the Plugin Manager Tree view.  The "target" attribute of the <object idref="Context"/> argument contains the Plug-in Manager <object idref="View"/>. */
	siMenuPluginMgrContextPluginID	= siMenuPluginMgrTreeFileID + 1,		/*!< Context menu in the Plugin Manager Tree and Plug-in Tab for a specific self-installed plug-in.  The "target" attribute of the <object idref="Context"/> argument contains the Plug-in Manager <object idref="View"/>. */
	siMenuPluginMgrContextPluginLocationID	= siMenuPluginMgrContextPluginID + 1,		/*!< Context menu in the Plugin Manager for an Addon or other location where plug-ins can be stored. The "target" attribute of the <object idref="Context"/> argument contains the Plug-in Manager <object idref="View"/>. */
	siMenuPluginMgrContextUserRootID	= siMenuPluginMgrContextPluginLocationID + 1,		/*!< Context menu associated with the top folder of the user path (siUserPath). The "target" attribute of the <object idref="Context"/> argument contains the Plug-in Manager <object idref="View"/>. */
	siMenuPluginMgrContextFactoryRootID	= siMenuPluginMgrContextUserRootID + 1,		/*!< Context menu associated with the top folder of the XSI installation path (siFactoryPath) */
	siMenuPluginMgrContextWorkgroupRootID	= siMenuPluginMgrContextFactoryRootID + 1,		/*!< Context menu associated with the top folder of a workgroup path.  The "target" attribute of the <object idref="Context"/> argument contains the Plug-in Manager <object idref="View"/>. */
	siMenuPluginMgrContextAddonID	= siMenuPluginMgrContextWorkgroupRootID + 1,		/*!< Context menu associated with the top folder of a particular addon.  The "target" attribute of the <object idref="Context"/> argument contains the Plug-in Manager <object idref="View"/>. */
	siMenuPluginMgrContextCustomLocationID	= siMenuPluginMgrContextAddonID + 1,		/*!< Context menu associated with a custom plug-in location.  The "target" attribute of the <object idref="Context"/> argument contains the Plug-in Manager <object idref="View"/>. */
	siMenuPluginMgrSPDLFileID	= siMenuPluginMgrContextCustomLocationID + 1,		/*!< File menu on the Plug-in Manager SPDL tab. The "target" attribute of the <object idref="Context"/> argument contains the Plugin Manager <object idref="View"/>. */
	siMenuPluginMgrContextSPDLID	= siMenuPluginMgrSPDLFileID + 1,		/*!< Context menu for a selected SPDL in the Plug-in Manager. The "target" attribute of the <object idref="Context"/> argument contains the Plugin Manager <object idref="View"/>. */
	siMenuPluginMgrContextSPDLLocationID	= siMenuPluginMgrContextSPDLID + 1,		/*!< Context menu for the location that stores SPDL files. The "target" attribute of the <object idref="Context"/> argument contains the Plugin Manager <object idref="View"/>. */
	siMenuPluginMgrContextPluginItemID	= siMenuPluginMgrContextSPDLLocationID + 1,		/*!< Context menu in the Plug-in Manager Tree and Plug-in Item Tab for a <object idref="PluginItem"/>. */
	siMenuPluginMgrContextViewLocationID	= siMenuPluginMgrContextPluginItemID + 1,		/*!< Context menu for the location that stores custom relational views. */
	siMenuPluginMgrContextViewID	= siMenuPluginMgrContextViewLocationID + 1,		/*!< Context menu for a custom view in the Plug-in Manager Tree. */
	siMenuPluginMgrContextToolbarLocationID	= siMenuPluginMgrContextViewID + 1,		/*!< Context menu for the location that stores custom toolbars */
	siMenuPluginMgrContextToolbarID	= siMenuPluginMgrContextToolbarLocationID + 1,		/*!< Context menu for a toolbar in the Plug-in Manager Tree. */
	siMenuWorkgroupMgrFileID	= 3350,		/*!< File menu on the Workgroup Manager.  The "target" attribute of the <object idref="Context"/> argument contains the Plug-in Manager <object idref="View"/> */
	siMenuWorkgroupMgrContextID	= siMenuWorkgroupMgrFileID + 1,		/*!< Context menu for a selected Workgroup.  The "target" attribute of the <object idref="Context"/> argument contains the Plug-in Manager <object idref="View"/> */
	siMenuScriptEditContextID	= 3400,		/*!< Context menu which appears when you right click in the text window of the script (or Text) editor view.  The "target" attribute of the <object idref="Context"/> argument contains the Script Editor <object idref="View"/>. */
	siMenuScriptHistoryContextID	= 3401,		/*!< Context menu which appears when you right click in the script history window.  The "target" attribute of the <object idref="Context"/> argument contains the selected text, or, if no text is selected, the text of the current line. */
	siMenuPropertyPageContextID	= 3600,		/*!< Context menu which appears when you right click in the top area of a Property Page.  The "target" attribute of the <object idref="Context"/> argument contains the inspected object. */
	siMenuShapeManagerEditID	= 3700,		/*!< The main Edit menu in the Shape Manager. */
	siMenuShapeManagerCreateContextSingleSelectionID	= siMenuShapeManagerEditID + 1,		/*!< The contextual menu in the create tab of Shape Manager that is opened when only a single shape is selected. */
	siMenuShapeManagerCreateContextMultiSelectionID	= siMenuShapeManagerCreateContextSingleSelectionID + 1,		/*!< The contextual menu in the create tab of Shape Manager that is opened when multiple shapes are selected. */
	siMenuShapeManagerAnimateContextSingleSelectionID	= siMenuShapeManagerCreateContextMultiSelectionID + 1,		/*!< The contextual menu in the animate tab of Shape Manager that is opened when only a single shape is selected */
	siMenuShapeManagerAnimateContextMultiSelectionID	= siMenuShapeManagerAnimateContextSingleSelectionID + 1,		/*!< The contextual menu in the animate tab of Shape Manager that is opened when multiple shapes are selected */
	siMenuShapeManagerAnimateID	= siMenuShapeManagerAnimateContextMultiSelectionID + 1		/*!< The main Animate menu in the Shape Manager. */
    } 	siMenuAnchorPoints;

/*! This enum specifies the operator port flags. These flags are used to determine the behaviour and characteristics of the port. */
typedef   
enum siPortFlags
    {	siDefaultPort	= 0,		/*!< The port is a regular port and must be connected. */
	siOptionalInputPort	= 16,		/*!< The port is optional as does not need to be connected. */
	siCreatedOutputPort	= 256,		/*!< The object connected to the output port is created when the operator is connected (not implemented for custom operators). */
	siBranchGroupPort	= 1024		/*!< The port can be connected to a group of a hierarchy of objects. */
    } 	siPortFlags;

/*! This enum is used to determine the behavior and look of a Custom Display */
typedef   
enum siCustomDisplayFlags
    {	siSupportCommandMap	= 1 << 1,		/*!< Enables the XSI keyboard shortcuts for this Custom Display. */
	siWindowNotifications	= 1 << 2		/*!< The Custom Display will get notified of all Window positining events such as resizing, moving, etc. */
    } 	siCustomDisplayFlags;

/*! This enum specifies what has changed during a Window Notification for Custom Displays */
typedef   
enum siWindowChangeState
    {	siWindowSize	= 1,		/*!< The size or position of the window has changed. */
	siWindowPaint	= 2,		/*!< The window is being repainted. */
	siWindowSetFocus	= 3,		/*!< The window is getting focus. */
	siWindowLostFocus	= 4		/*!< The window lost focus. */
    } 	siWindowChangeState;

/*! This enum is used to determine the type of a file. */
typedef   
enum siFileType
    {	siFileTypeDefault	= 0,		/*!< Unknown type. */
	siFileTypeScene	= 1,		/*!< Scene. */
	siFileTypeModel	= 2,		/*!< Model. */
	siFileTypeImage	= 3,		/*!< Image. */
	siFileTypeAudio	= 4,		/*!< Audio. */
	siFileTypeSI3D	= 5,		/*!< SI3D scenes. */
	siFileTypeScript	= 6,		/*!< Script. */
	siFileTypeFcurve	= 7,		/*!< Animation file. */
	siFileTypeExpression	= 8,		/*!< Expression. */
	siFileTypeRenderPicture	= 9,		/*!< Rendered Pictures. */
	siFileTypeAction	= 10,		/*!< Action files (.eani). */
	siFileTypeShader	= 11,		/*!< Shader file. */
	siFileTypeBackup	= 12,		/*!< Scene Backup. */
	siFileTypeMI	= 13,		/*!< Mental Image (.mi) file. */
	siFileTypeIgnore	= 14,		/*!< Internal info. */
	siFileTypeSimulation	= 15,		/*!< Simulation cache. */
	siFileTypeReferencedModel	= 16,		/*!< Referenced model. */
	siFileTypeQuery	= 17,		/*!< Query. */
	siFileTypeSynoptic	= 18,		/*!< Synoptic. */
	siFileTypeDotXSI	= 19,		/*!< dot XSI. */
	siFileTypeRenderMap	= 20,		/*!< Render Map. */
	siFileTypeAddon	= 21,		/*!< Addon. */
	siFileTypeMatLib	= 22,		/*!< Material Library. */
	siFileTypeThumbnail	= 23,		/*!< Thumbnail. */
	siFileTypeMixer	= 24,		/*!< Mixer (.mixer). */
	siFileTypeIges	= 25,		/*!< IGES. */
	siFileTypeLast	= 26		/*!< Number of items total. */
    } 	siFileType;

/*! This enum specifies the type of clusters in XSI. */
typedef   
enum siClusterType
    {	siClusterVertexType	= 0,		/*!< Vertex cluster type. */
	siClusterNodeType	= 1,		/*!< Node cluster type. */
	siClusterEdgeType	= 2,		/*!< Edge cluster type. */
	siClusterPolygonType	= 3		/*!< Polygon cluster type. */
    } 	siClusterType;

/*! This enum specifies the type of cluster properties in XSI. */
typedef   
enum siClusterPropertyType
    {	siClusterPropertyEnvelopeWeightType	= 0,		/*!< Vertex cluster property envelope weight type. */
	siClusterPropertyShapeKeyType	= 1,		/*!< Vertex cluster property Shape Key type. */
	siClusterPropertyWeightMapType	= 2,		/*!< Vertex cluster property Weight Map type. */
	siClusterPropertyUVType	= 3,		/*!< Node cluster property UV type. */
	siClusterPropertyVertexColorType	= 4,		/*!< Node cluster property vertex color type. */
	siClusterPropertyUserNormalType	= 5		/*!< Node cluster property user normal type. */
    } 	siClusterPropertyType;

/*! This enum specifies the mode of manipulation of a curve isopoint in XSI. */
typedef   
enum siCurveIsopointManipMode
    {	siCurveIsopointPositionManipMode	= 0,		/*!< Curve isopoint position manipulation mode.  To change the isopoint position with the tangent(s) free to update. */
	siCurveIsopointPositionAndTangentsManipMode	= 1,		/*!< Curve isopoint position-with-tangents-locked manipulation mode.  To change the isopoint position with the tangent(s) locked. But in the case where two consecutive knots are linked by a linear segment (i.e.: corresponding control points are aligned) the tangents will update such that the curve segment remains linear. */
	siCurveIsopointFwdTangentManipMode	= 2,		/*!< CurveIsopoint forward tangent manipulation mode. To change the forward tangent of the isopoint. */
	siCurveIsopointBwdTangentManipMode	= 3,		/*!< CurveIsopoint backward tangent manipulation mode.  To change the backward tangent of the isopoint. The backward tangent only makes sense for a full-multiplicity knot (e.g.: 3 for a cubic curve), also known as a Bezier knot, where a discontinuity of the tangents on both sides of the knot is supported. In the case where both tangents are aligned the backward tangent has the opposite direction as the forward tangent. */
	siCurveIsopointFwdThenBwdTangentManipMode	= 4,		/*!< CurveIsopoint forward-and-backward tangent manipulation mode. Same as siCurveIsopointFwdTangentManipMode but the backward tangent is also updated by the same delta. */
	siCurveIsopointBwdThenFwdTangentManipMode	= 5,		/*!< CurveIsopoint backward-and-forward tangent manipulation mode. Same as siCurveIsopointBwdTangentManipMode but the forward tangent is also updated by the same delta. */
	siCurveIsopointCVManipMode	= 6		/*!< CurveIsopoint CV position manipulation mode. To change the position of the CV corresponding to the isopoint. For a knot of multiplicity > 1 there are more than one CVs so this mode is ignored. */
    } 	siCurveIsopointManipMode;

/*! This enum specifies the type of alignment for a tangent of a curve knot in XSI. */
typedef   
enum siCurveKnotTangentAlignment
    {	siCurveKnotAlignFwdTangentWithBwd	= 0,		/*!< Align the forward tangent with the backward tangent. */
	siCurveKnotAlignBwdTangentWithFwd	= 1,		/*!< Align the backward tangent with the forward tangent. */
	siCurveKnotAlignFwdAndBwdTangentsTogether	= 2,		/*!< Rotate the forward and backward tangents by the same angle so they become aligned. */
	siCurveKnotLinearizeNextSegment	= 3,		/*!< .Align all the control points between this bezier knot and the next one so the curve segment becomes a line. */
	siCurveKnotLinearizePrevSegment	= 4,		/*!< .Align all the control points between this bezier knot and the previous one so the curve segment becomes a line. */
	siCurveKnotLinearizeBothSegments	= 5,		/*!< .Equivalent of doing siCurveKnotLinearizePrevSegment and siCurveKnotLinearizeNextSegment. */
	siCurveKnotLinearizeInterSegments	= 6,		/*!< .Align all the control points between consecutive knots. */
	siCurveKnotMakeCorner	= 7		/*!< .Rotate both tangents enough to cause a significant discontinuity but with minimum curve distortion. */
    } 	siCurveKnotTangentAlignment;

/*! Specifies the rigid body dynamics engine used for simulation. There can be only one rigid body dynamics engine per <object idref="SimulationEnvironment"/>. */
typedef   
enum siRBDEngine
    {	siPhysXEngine	= 0,		/*!< Ageia physX engine  */
	siOpenDynamicsEngine	= 1		/*!< ODE engine  */
    } 	siRBDEngine;

/*! Keyable attribute types */
typedef   
enum siKeyableAttributeType
    {	siKeyableAttributeClear	= 0,		/*!< clear the keyable/non-keyable visible capabilities */
	siKeyableAttributeKeyable	= 1,		/*!< set the keyable capability */
	siKeyableAttributeNonKeyableVisible	= 2		/*!< set the non-keyable visible capability */
    } 	siKeyableAttributeType;

/*! Method for finding closest locations on a geometry */
typedef   
enum siClosestLocationMethod
    {	siClosestVertexOrKnot	= 0,		/*!< Finds the closest vertex (<object idref="PolygonMesh"/>) or surface knot (<object idref="NurbsSurfaceMesh"/>). */
	siClosestSurface	= 1,		/*!< Finds the exact closest surface location. */
	siClosestSmoothedSurface	= 2		/*!< Similar to siClosestVertexOrKnot, but the point locators are relaxed in order to have a more well-distributed (more uniform) spatial mapping between the geometry and the surrounding volume. This can avoid having important volumes corresponding to the same surface location. This method only applies to <object idref="PolygonMesh"/> (equivalent to siClosestLocationMethod for <object idref="NurbsSurfaceMesh"/> ). */
    } 	siClosestLocationMethod;

/*! Method for computing normals on a geometry. These only apply to <object idref="PolygonMesh"/> objects(for <object idref="NurbsSurfaceMesh"/> the normal is always the exact surface normal). */
typedef   
enum siNormalComputationMethod
    {	siInterpolatedVertexGeometricNormals	= 0,		/*!< Interpolated vertex normals, each vertex normal being the normalized sum of normalized cross products of edge vector pairs bounding adjacent polygons. */
	siInterpolatedVertexAngleBasedGeometricNormals	= 1,
	siInterpolatedShadingNormals	= 2,
	siInterpolatedAngleBasedNodeShadingNormals	= 3		/*!< Similar to siInterpolatedShadingNormals, but weights the adjacent polygons' normals proportionally to the angles formed by neighbor polygons. Slower than method siInterpolatedNodeShadingNormals but gives higher quality, particularly with polygon meshes having many triangles. */
    } 	siNormalComputationMethod;

/*! This enum lists the allowed channel types in writable images. */
typedef   
enum siImageChannelType
    {	siImageRGBAChannelType	= 0,		/*!< Combined color and alpha type. */
	siImageRGBChannelType	= 1,		/*!< Color only type. */
	siImageAlphaChannelType	= 2,		/*!< Alpha only type. */
	siImageIntensityChannelType	= 3,		/*!< RGB intensity type. */
	siImageRGBEChannelType	= 4,		/*!< Color with fourth channel as exponent type. */
	siImageDepthChannelType	= 5,		/*!< Depth value type. */
	siImageNormalChannelType	= 6,		/*!< Normal vector type. */
	siImageMotionChannelType	= 7,		/*!< Motion vector type. */
	siImageTagChannelType	= 8		/*!< Object tag type. */
    } 	siImageChannelType;

/*! This enum lists the filters for allowed user-selectable channel types in the UI. */
typedef   
enum siImageChannelFilter
    {	siImageRGBAChannelFilter	= 1,		/*!< Combined color and alpha filter. */
	siImageRGBChannelFilter	= 2,		/*!< Color only filter. */
	siImageColorChannelFilter	= 3,		/*!< Color with optional alpha filter. */
	siImageAlphaChannelFilter	= 4,		/*!< Alpha only filter. */
	siImageIntensityChannelFilter	= 8,		/*!< RGB intensity filter. */
	siImageRGBEChannelFilter	= 16,		/*!< Color with fourth channel as exponent filter. */
	siImageDepthChannelFilter	= 32,		/*!< Depth value filter. */
	siImageNormalChannelFilter	= 64,		/*!< Normal vector filter. */
	siImageMotionChannelFilter	= 128,		/*!< Motion vector filter. */
	siImageTagChannelFilter	= 256,		/*!< Object tag filter. */
	siImageChannelFilterAll	= 511		/*!< All. */
    } 	siImageChannelFilter;

/*! This enumeration lists the allowed bit depths for each channel of an image with the specified channel types. Not all bit depths are allowed for all channel types. */
typedef   
enum siImageBitDepth
    {	siImageBitDepthInteger8	= 3,		/*!< 8 integer bits per channel. */
	siImageBitDepthInteger16	= 4,		/*!< 16 integer bits per channel. */
	siImageBitDepthInteger32	= 5,		/*!< 32 integer bits per channel. */
	siImageBitDepthFloat16	= 20,		/*!< 16 bit floating point value per channel (half float). */
	siImageBitDepthFloat32	= 21		/*!< 32 bit floating point value per channel (full float). */
    } 	siImageBitDepth;

/*! Specifies the method used to calculate the center of a bounding volume. */
typedef   
enum siVolumeCenterMethod
    {	siVolumeCenterMethodCOG	= 0,		/*!< Use center of gravity (averaged location of all points)  */
	siVolumeCenterMethodBBoxCenter	= 1,		/*!< Use center of the bounding box  */
	siVolumeCenterMethodObjectCenter	= 2		/*!< Use the object's local center  */
    } 	siVolumeCenterMethod;

/*! Specifies the method used to calculate the long axis of a bounding cylinder. */
typedef   
enum siBoundingCapsuleMethod
    {	siBoundingCapsuleMethodXAxis	= 0,		/*!< Use the object's local X axis  */
	siBoundingCapsuleMethodYAxis	= 1,		/*!< Use the object's local Y axis  */
	siBoundingCapsuleMethodZAxis	= 2,		/*!< Use the object's local Z axis  */
	siBoundingCapsuleMethodBestAxis	= 3		/*!< Use the tightest fitting axis  */
    } 	siBoundingCapsuleMethod;

/*! Specifies the source path being set for the siOnSourcePathChange event (see <object idref="siEventID"/>). */
typedef   
enum siSourcePathType
    {	siSourcePathImageSource	= 0,		/*!< An image source's source path is being changed  */
	siSourcePathWritableImageSource	= 1,		/*!< A writable image source's source path is being changed  */
	siSourcePathModelSource	= 3		/*!< An model source's source path is being changed  */
    } 	siSourcePathType;


/*! Filter by boundaries (on active NURBS objects) */
extern XSIDECL const wchar_t* siBoundaryFilter;

/*! Filter by cameras */
extern XSIDECL const wchar_t* siCameraFilter;

/*! Filter by chain elements */
extern XSIDECL const wchar_t* siChainElementFilter;

/*! Filter by clusters */
extern XSIDECL const wchar_t* siClusterFilter;

/*! Filter by constraints */
extern XSIDECL const wchar_t* siConstraintFilter;

/*! Filter by control objects */
extern XSIDECL const wchar_t* siControlFilter;

/*! Filter by curves */
extern XSIDECL const wchar_t* siCurveFilter;

/*! Filter by edges (on active 3D objects) */
extern XSIDECL const wchar_t* siEdgeFilter;

/*! Filter by chain effectors */
extern XSIDECL const wchar_t* siEffectorFilter;

/*! Filter by any object */
extern XSIDECL const wchar_t* siGenericObjectFilter;

/*! Filter by any type of geometric 3D object */
extern XSIDECL const wchar_t* siGeometryFilter;

/*! Filter by groups */
extern XSIDECL const wchar_t* siGroupFilter;

/*! Filter by implicit primitives */
extern XSIDECL const wchar_t* siImplicitFilter;

/*! Filter by isopoints (on active 3D objects) */
extern XSIDECL const wchar_t* siIsopointFilter;

/*! Filter by knots (on active 3D objects) */
extern XSIDECL const wchar_t* siKnotFilter;

/*! Filter by implicit lattices */
extern XSIDECL const wchar_t* siLatticeFilter;

/*! Filter by layers */
extern XSIDECL const wchar_t* siLayerFilter;

/*! Filter by lights */
extern XSIDECL const wchar_t* siLightFilter;

/*! Filter by light partitions */
extern XSIDECL const wchar_t* siLightPartitionFilter;

/*! Filter by models */
extern XSIDECL const wchar_t* siModelFilter;

/*! Filter by nulls */
extern XSIDECL const wchar_t* siNullFilter;

/*! Filter by 3D objects only */
extern XSIDECL const wchar_t* siObjectFilter;

/*! Filter by object partitions */
extern XSIDECL const wchar_t* siObjectPartitionFilter;

/*! Filter by operators */
extern XSIDECL const wchar_t* siOperatorFilter;

/*! Filter by points */
extern XSIDECL const wchar_t* siPointFilter;

/*! Filter by polygons */
extern XSIDECL const wchar_t* siPolygonFilter;

/*! Filter by polygon meshes */
extern XSIDECL const wchar_t* siPolyMeshFilter;

/*! Filter by primitives */
extern XSIDECL const wchar_t* siPrimitiveFilter;

/*! Filter by properties */
extern XSIDECL const wchar_t* siPropertyFilter;

/*! Filter by scenes */
extern XSIDECL const wchar_t* siSceneFilter;

/*! Filter by shaders. */
extern XSIDECL const wchar_t* siShaderFilter;

/*! Filter by skeleton joint properties */
extern XSIDECL const wchar_t* siSkeletonJointFilter;

/*! Filter by subcomponents */
extern XSIDECL const wchar_t* siSubComponentFilter;

/*! Filter by subsurfaces (on active 3D objects) */
extern XSIDECL const wchar_t* siSubSurfaceFilter;

/*! Filter by surface curves (on active 3D objects) */
extern XSIDECL const wchar_t* siSurfaceCurveFilter;

/*! Filter by surface meshes */
extern XSIDECL const wchar_t* siSurfaceMeshFilter;

/*! Filter by texture controls */
extern XSIDECL const wchar_t* siTextureControlFilter;

/*! Filter by trim curves (on active 3D objects) */
extern XSIDECL const wchar_t* siTrimCurveFilter;

/*! Filter by U isolines (on active 3D objects) */
extern XSIDECL const wchar_t* siUIsolineFilter;

/*! Filter by V isolines (on active 3D objects) */
extern XSIDECL const wchar_t* siVIsolineFilter;

/*! Filter by U knot curves (on active 3D objects) */
extern XSIDECL const wchar_t* siUKnotCurveFilter;

/*! Filter by V knot curves (on active 3D objects) */
extern XSIDECL const wchar_t* siVKnotCurveFilter;

/*! Filter by wave controls */
extern XSIDECL const wchar_t* siWaveFilter;

extern XSIDECL const wchar_t* siConditionalObjectFilter;


/*! TRUE to force the property dialogs to automatically pop up on object creation.
 *  Corresponds to User Preferences < Interaction < General < Automatically Popup Property Editors on Node Creation in the User Interface. */
extern XSIDECL const wchar_t* siAutoInspect;

/*! Toggle the Comp button in the Constrain panel.
 *  Possible values are:
 *  0 = off
 *  1 = on
 *  Corresponds to Constrain < Compensation in the main-menu. */
extern XSIDECL const wchar_t* siCompensationFlag;

/*! This preference enables or disables the custom command library cache. Disabling the cache allows developers to recompile an add-on library without having to unload it explicitly. Disabling a cache can drastically affect the performance of XSI, therefore it is recommended to disable it only in the context of add-ons development. For performance reasons, the state of the cache is not persisted with the scene. By default the cache is enabled at startup, therefore users have to disable it at each session. */
extern XSIDECL const wchar_t* siCustomCommandLibCache;

/*! This preference enables or disables the custom operator library cache. Disabling the cache allows developers to recompile an add-on library without having to unload it explicitly. Disabling a cache can drastically affect the performance of XSI, therefore it is recommended to disable it only in the context of add-ons development. For performance reasons, the state of the cache is not persisted with the scene. By default the cache is enabled at startup, therefore users have to disable it at each session. */
extern XSIDECL const wchar_t* siCustomOperatorLibCache;

/*! This preference enables or disables the event library cache. Disabling the cache allows developers to recompile an add-on library without having to unload it explicitly. Disabling a cache can drastically affect the performance of XSI, therefore it is recommended to disable it only in the context of add-ons development. For performance reasons, the state of the cache is not persisted with the scene. By default the cache is enabled at startup, therefore users have to disable it at each session. */
extern XSIDECL const wchar_t* siEventLibCache;

/*! This preference enables or disables the Realtime shader library cache. Disabling the cache allows developers to recompile an add-on library without having to unload it explicitly. Disabling a cache can drastically affect the performance of XSI, therefore it is recommended to disable it only in the context of add-ons development. For performance reasons, the state of the cache is not persisted with the scene. By default the cache is enabled at startup, therefore users have to disable it at each session. */
extern XSIDECL const wchar_t* siRTShaderLibCache;

/*! This preference enables or disables the custom menu library cache. Disabling the cache allows developers to recompile a plug-in library without having to unload it explicitly. Disabling a cache can drastically affect the performance of XSI, therefore it is recommended to disable it only in the context of add-ons development. For performance reasons, the state of the cache is not persisted with the scene. By default the cache is enabled at startup, therefore users have to disable it at each session. */
extern XSIDECL const wchar_t* siMenuLibCache;

/*! This preference enables or disables the custom filter library cache. Disabling the cache allows developers to recompile a plug-in library without having to unload it explicitly. Disabling a cache can drastically affect the performance of XSI, therefore it is recommended to disable it only in the context of add-ons development. For performance reasons, the state of the cache is not persisted with the scene. By default the cache is enabled at startup, therefore users have to disable it at each session. */
extern XSIDECL const wchar_t* siFilterLibCache;

/*! This preference enables or disables the custom property library cache. Disabling the cache allows developers to recompile a plug-in library without having to unload it explicitly. Disabling a cache can drastically affect the performance of XSI, therefore it is recommended to disable it only in the context of add-ons development. For performance reasons, the state of the cache is not persisted with the scene. By default the cache is enabled at startup, therefore users have to disable it at each session. */
extern XSIDECL const wchar_t* siPropertyLibCache;

/*! This preference enables or disables the custom display callback library cache. Disabling the cache allows developers to recompile a plug-in library without having to unload it explicitly. Disabling a cache can drastically affect the performance of XSI, therefore it is recommended to disable it only in the context of add-ons development. For performance reasons, the state of the cache is not persisted with the scene. By default the cache is enabled at startup, therefore users have to disable it at each session. */
extern XSIDECL const wchar_t* siDisplayCallbackLibCache;

/*! This preference enables or disables the custom display pass library cache. Disabling the cache allows developers to recompile a plug-in library without having to unload it explicitly. Disabling a cache can drastically affect the performance of XSI, therefore it is recommended to disable it only in the context of add-ons development. For performance reasons, the state of the cache is not persisted with the scene. By default the cache is enabled at startup, therefore users have to disable it at each session. */
extern XSIDECL const wchar_t* siDisplayPassLibCache;

/*! This preference enables or disables the custom view library cache. Disabling the cache allows developers to recompile a plug-in library without having to unload it explicitly. Disabling a cache can drastically affect the performance of XSI, therefore it is recommended to disable it only in the context of add-ons development. For performance reasons, the state of the cache is not persisted with the scene. By default the cache is enabled at startup, therefore users have to disable it at each session. */
extern XSIDECL const wchar_t* siDisplayLibCache;

/*! Set to True to enable command execution logging in the scripting history window. */
extern XSIDECL const wchar_t* siScrCommandLogEnabled;

/*! Use this user preference to specify the name of the file to use for logging commands and messages. Use siSrcCommandLogToFile to enable logging to file. */
extern XSIDECL const wchar_t* siScrCommandLogFileName;

/*! Use this user preference to get and set the maximum size for the scripting command/message log. */
extern XSIDECL const wchar_t* siScrCommandLogMaxSize;

/*! Set to True to log the scripting command/message to a file. Use siSrcCommandLogFileName to specify the log file name. */
extern XSIDECL const wchar_t* siScrCommandLogToFile;

/*! Set to True to allow the scripting command/message log to be of an unlimited size. */
extern XSIDECL const wchar_t* siScrCommandLogUnlimitedSize;

/*! Use this user preference to get and set the preferred scripting language. Possible values are: 
 *  "VB Script Language" 
 *   "JScript Language" 
 *  "PerlScript Language" 
 *  "Python ActiveX Scripting"
 *   */
extern XSIDECL const wchar_t* siScrLanguage;

/*! Set to True to enable message logging in the scripting history window. */
extern XSIDECL const wchar_t* siScrMessageLogEnabled;

/*! Set to True to enable real-time message logging in the scripting history window. Messages are delivered immediately as they arrive instead of reported at the end of the command execution. */
extern XSIDECL const wchar_t* siScrRealTimeMessagingEnabled;

/*! Set to True to display time as frame. */
extern XSIDECL const wchar_t* siTimeDisplayFormatDisplayAsFrames;

/*! Set to True to use the default frame format or false for a user selected format for display purposes. */
extern XSIDECL const wchar_t* siTimeDisplayFormatDisplayUserFormat;

/*! Use this user preference to specify the user display frame format. The value will be corresponding to <object idref="siDisplayTimeFormat"/>. */
extern XSIDECL const wchar_t* siTimeDisplayFormatUserFormat;

/*! Use this user preference to specify the default frame format. The value will be corresponding to <object idref="siDefaultTimeFormat"/>. */
extern XSIDECL const wchar_t* siTimeFormatDefaultFrameFormat;

/*! Use this user preference to access the default frame rate. */
extern XSIDECL const wchar_t* siTimeFormatDefaultFrameRate;

/*! Set the Transformation Axis Mode. 
 *  The values to provide are actually bitmasks with bit zero for X, bit one for Y, and bit two for Z.
 *   */
extern XSIDECL const wchar_t* siTransformAxisMode;

extern XSIDECL const wchar_t* siTransformRefMode;


/*! Use this user preference to specify the default current layout that will be used when starting XSI. */
extern XSIDECL const wchar_t* siUILayoutDefault;

/*! Reuse keywords in property page dialog */
extern XSIDECL const wchar_t* siCurrentKeyword;

/*! Animation property sets */
extern XSIDECL const wchar_t* siAnimationKeyword;

/*! Cluster property sets */
extern XSIDECL const wchar_t* siClusterKeyword;

/*! Constraint property sets */
extern XSIDECL const wchar_t* siConstraintKeyword;

/*! ControlObject property sets */
extern XSIDECL const wchar_t* siControlObjectKeyword;

/*! Converter property sets */
extern XSIDECL const wchar_t* siConverterKeyword;

/*! Curve property sets */
extern XSIDECL const wchar_t* siCurveKeyword;

/*! CustomPSet property sets */
extern XSIDECL const wchar_t* siCustomPSetKeyword;

/*! Deform property sets */
extern XSIDECL const wchar_t* siDeformKeyword;

/*! Dynamics property sets */
extern XSIDECL const wchar_t* siDynamicsKeyword;

/*! Expressions property sets */
extern XSIDECL const wchar_t* siExpressionsKeyword;

/*! General property sets */
extern XSIDECL const wchar_t* siGeneralKeyword;

/*! Generator property sets */
extern XSIDECL const wchar_t* siGeneratorKeyword;

/*! Geometry property sets */
extern XSIDECL const wchar_t* siGeometryKeyword;

/*! Hair property sets */
extern XSIDECL const wchar_t* siHairKeyword;

/*! IK property sets */
extern XSIDECL const wchar_t* siIKKeyword;

/*! Kinematics property sets */
extern XSIDECL const wchar_t* siKinematicsKeyword;

/*! Mesh property sets */
extern XSIDECL const wchar_t* siMeshKeyword;

/*! Mixer property sets */
extern XSIDECL const wchar_t* siMixerKeyword;

/*! Modeling property sets */
extern XSIDECL const wchar_t* siModelingKeyword;

/*! Operators property sets */
extern XSIDECL const wchar_t* siOperatorsKeyword;

/*! Particle property sets */
extern XSIDECL const wchar_t* siParticleKeyword;

/*! Primitive property sets */
extern XSIDECL const wchar_t* siPrimitiveKeyword;

/*! Projection property sets */
extern XSIDECL const wchar_t* siProjectionKeyword;

/*! Rendering property sets */
extern XSIDECL const wchar_t* siRenderingKeyword;

/*! Simulation property sets */
extern XSIDECL const wchar_t* siSimulationKeyword;

/*! Surface property sets */
extern XSIDECL const wchar_t* siSurfaceKeyword;

/*! Topology property sets */
extern XSIDECL const wchar_t* siTopologyKeyword;

/*! Topology Editor property sets */
extern XSIDECL const wchar_t* siTopologyEditorKeyword;

/*! Viewing property sets */
extern XSIDECL const wchar_t* siViewingKeyword;

/*! boundary cluster */
extern XSIDECL const wchar_t* siBoundaryCluster;

/*! edge cluster */
extern XSIDECL const wchar_t* siEdgeCluster;

/*! face cluster */
extern XSIDECL const wchar_t* siFaceCluster;

/*! iso line U cluster */
extern XSIDECL const wchar_t* siIsoLineUCluster;

/*! iso line V cluster */
extern XSIDECL const wchar_t* siIsoLineVCluster;

/*! iso point cluster */
extern XSIDECL const wchar_t* siIsoPointCluster;

/*! knot cluster */
extern XSIDECL const wchar_t* siKnotCluster;

/*! knot curve U cluster */
extern XSIDECL const wchar_t* siKnotCurveUCluster;

/*! knot curve V cluster */
extern XSIDECL const wchar_t* siKnotCurveVCluster;

/*! polygon cluster */
extern XSIDECL const wchar_t* siPolygonCluster;

/*! polygon node cluster */
extern XSIDECL const wchar_t* siPolygonNodeCluster;

/*! sampled point cluster */
extern XSIDECL const wchar_t* siSampledPointCluster;

/*! subcurve cluster */
extern XSIDECL const wchar_t* siSubCurveCluster;

/*! subsurface cluster */
extern XSIDECL const wchar_t* siSubSurfaceCluster;

/*! surface curve cluster */
extern XSIDECL const wchar_t* siSurfaceCurveCluster;

/*! trim curve cluster */
extern XSIDECL const wchar_t* siTrimCurveCluster;

/*! vertex cluster */
extern XSIDECL const wchar_t* siVertexCluster;

/*! 0D Component family */
extern XSIDECL const wchar_t* si0DComponentFamily;

/*! 1D Component family */
extern XSIDECL const wchar_t* si1DComponentFamily;

/*! 2D Component family */
extern XSIDECL const wchar_t* si2DComponentFamily;

/*! 3D Object family */
extern XSIDECL const wchar_t* si3DObjectFamily;

/*! Assignment operator family */
extern XSIDECL const wchar_t* siAssignmentOperatorFamily;

/*! Camera family */
extern XSIDECL const wchar_t* siCameraFamily;

/*! Chain Element family */
extern XSIDECL const wchar_t* siChainElementFamily;

/*! Clusterable Component family */
extern XSIDECL const wchar_t* siClusterableComponentFamily;

/*! Cluster family */
extern XSIDECL const wchar_t* siClusterFamily;

/*! Constraint property family */
extern XSIDECL const wchar_t* siConstraintFamily;

/*! Contour Contrast shader family */
extern XSIDECL const wchar_t* siContourContrastShaderFamily;

/*! Contour shader family */
extern XSIDECL const wchar_t* siContourShaderFamily;

/*! Contour Store shader family */
extern XSIDECL const wchar_t* siContourStoreShaderFamily;

/*! Control Object family */
extern XSIDECL const wchar_t* siControlObjectFamily;

/*! Control Object Nurbs Texture family */
extern XSIDECL const wchar_t* siControlObjectNurbsTextureFamily;

/*! Control Object Texture family */
extern XSIDECL const wchar_t* siControlObjectTextureFamily;

/*! Control Object Texture Projection family */
extern XSIDECL const wchar_t* siControlObjectTextureProjectionFamily;

/*! Converter operator family */
extern XSIDECL const wchar_t* siConverterOperatorFamily;

/*! Curve Geometry family */
extern XSIDECL const wchar_t* siCurveFamily;

/*! Deform operator family */
extern XSIDECL const wchar_t* siDeformOperatorFamily;

/*! Displacement shader family */
extern XSIDECL const wchar_t* siDisplacementShaderFamily;

/*! Environment shader family */
extern XSIDECL const wchar_t* siEnvironmentShaderFamily;

/*! Fx Operator family */
extern XSIDECL const wchar_t* siFxOperatorFamily;

/*! Generator operator family */
extern XSIDECL const wchar_t* siGeneratorOperatorFamily;

/*! Geometry family */
extern XSIDECL const wchar_t* siGeometryFamily;

/*! Geometry shader family */
extern XSIDECL const wchar_t* siGeometryShaderFamily;

/*! Group family */
extern XSIDECL const wchar_t* siGroupFamily;

/*! Image Operator family */
extern XSIDECL const wchar_t* siImageOperatorFamily;

/*! Implicit Geometry family */
extern XSIDECL const wchar_t* siImplicitGeometryFamily;

/*! KineInfo property family */
extern XSIDECL const wchar_t* siKineInfoFamily;

/*! Lattice family */
extern XSIDECL const wchar_t* siLatticeFamily;

/*! Lens shader family */
extern XSIDECL const wchar_t* siLensShaderFamily;

/*! Light Photon shader family */
extern XSIDECL const wchar_t* siLightPhotonShaderFamily;

/*! Light Primitive family */
extern XSIDECL const wchar_t* siLightPrimitiveFamily;

/*! Light shader family */
extern XSIDECL const wchar_t* siLightShaderFamily;

/*! Material property family */
extern XSIDECL const wchar_t* siMaterialFamily;

/*! Material shader family */
extern XSIDECL const wchar_t* siMaterialShaderFamily;

/*! Mesh Geometry family */
extern XSIDECL const wchar_t* siMeshFamily;

/*! Null Primitive family */
extern XSIDECL const wchar_t* siNullPrimitiveFamily;

/*! Nurbs CurveList Geometry family */
extern XSIDECL const wchar_t* siNurbsCurveListFamily;

/*! Nurbs Surface Mesh Geometry family */
extern XSIDECL const wchar_t* siNurbsSurfaceMeshFamily;

/*! Operator family */
extern XSIDECL const wchar_t* siOperatorFamily;

/*! Output shader family */
extern XSIDECL const wchar_t* siOutputShaderFamily;

/*! Particle Control Object family */
extern XSIDECL const wchar_t* siParticleControlObjectFamily;

/*! Particle family */
extern XSIDECL const wchar_t* siParticleFamily;

/*! Pass family */
extern XSIDECL const wchar_t* siPassFamily;

/*! Patch Network family */
extern XSIDECL const wchar_t* siPatchNetworkFamily;

/*! Photon shader family */
extern XSIDECL const wchar_t* siPhotonShaderFamily;

/*! Planar Object family */
extern XSIDECL const wchar_t* siPlanarObjectFamily;

/*! Property family */
extern XSIDECL const wchar_t* siPropertyFamily;

/*! RealTime shader family */
extern XSIDECL const wchar_t* siRealTimeShaderFamily;

/*! Shader family */
extern XSIDECL const wchar_t* siShaderFamily;

/*! Shadow shader family */
extern XSIDECL const wchar_t* siShadowShaderFamily;

/*! SubComponent family */
extern XSIDECL const wchar_t* siSubComponentFamily;

/*! Surface Curve Geometry family */
extern XSIDECL const wchar_t* siSurfaceCurveFamily;

/*! Surface Geometry family */
extern XSIDECL const wchar_t* siSurfaceFamily;

/*! Texture shader family */
extern XSIDECL const wchar_t* siTextureShaderFamily;

/*! Topology operator family */
extern XSIDECL const wchar_t* siTopologyOperatorFamily;

/*! Virtual Component family */
extern XSIDECL const wchar_t* siVirtualComponentFamily;

/*! Virtual Curve family */
extern XSIDECL const wchar_t* siVirtualCurveFamily;

/*! Volume shader family */
extern XSIDECL const wchar_t* siVolumeShaderFamily;

/*! Birail Operator type (generates a surface from 2 curves and a profile) */
extern XSIDECL const wchar_t* siBirailOpType;

/*! Bend Operator type */
extern XSIDECL const wchar_t* siBendOpType;

/*! Fold Operator type */
extern XSIDECL const wchar_t* siFoldOpType;

/*! Birail2Generators Operator Type */
extern XSIDECL const wchar_t* siBirail2GenOpType;

/*! Group type */
extern XSIDECL const wchar_t* siGroupType;

/*! Polygon Mesh type */
extern XSIDECL const wchar_t* siPolyMeshType;

/*! Two Points Constraint type */
extern XSIDECL const wchar_t* si2PntCnsType;

/*! 3D Object type */
extern XSIDECL const wchar_t* si3DObjectType;

/*! Three Points Constraint type */
extern XSIDECL const wchar_t* si3PntCnsType;

/*! Action Clip type (instanced action) */
extern XSIDECL const wchar_t* siActionClipType;

/*! Action type */
extern XSIDECL const wchar_t* siActionType;

/*! AddEdge Operator type (add an edge between an existing point and an existing edge) */
extern XSIDECL const wchar_t* siAddEdgeOpType;

/*! Add Mesh Points Operator type */
extern XSIDECL const wchar_t* siAddMeshPntOpType;

/*! Add Nurbs Curve Points Operator type */
extern XSIDECL const wchar_t* siAddNurbsCrvPntOpType;

/*! Add Nurbs Surface Points Operator type */
extern XSIDECL const wchar_t* siAddNurbsSrfPntOpType;

/*! Add Point Inside Curve Operator type */
extern XSIDECL const wchar_t* siAddPntMiddleCrvType;

/*! Add Point Inside Surface Operator type */
extern XSIDECL const wchar_t* siAddPntMiddleSrfOpType;

/*! Ambient Lighting type */
extern XSIDECL const wchar_t* siAmbientLightingType;

/*! Implicit Arc Primitive type */
extern XSIDECL const wchar_t* siArcPrimType;

/*! Attractor Control Object type (electric force) */
extern XSIDECL const wchar_t* siAttractorCtrlType;

/*! Automatic Cage Assignment Operator type */
extern XSIDECL const wchar_t* siAutoCageAssignOpType;

/*! Automatic Envelope Assignment Operator type */
extern XSIDECL const wchar_t* siAutoEnvAssignOpType;

/*! Ball Joint Constraint type */
extern XSIDECL const wchar_t* siBallJointCnsType;

/*! Base Path Constraint type */
extern XSIDECL const wchar_t* siBasePathCnsType;

/*! Bevel Operator type (bevel geometry component) */
extern XSIDECL const wchar_t* siBevelOpType;

/*! Blaster Operator type */
extern XSIDECL const wchar_t* siBlasterOpType;

/*! Blend Curves Operator type (create a blended curve between two existing curves) */
extern XSIDECL const wchar_t* siBlendCrvOpType;

/*! Blend Surfaces Operator type (blend surfaces) */
extern XSIDECL const wchar_t* siBlendSrfOpType;

/*! Boolean Generator Operator type */
extern XSIDECL const wchar_t* siBooleanGenOpType;

/*! Boolean Surface Operator type */
extern XSIDECL const wchar_t* siBooleanSrfTopoOpType;

/*! Bounding Plane Constraint type */
extern XSIDECL const wchar_t* siBPlaneCnsType;

/*! Bridge Op Operator type (bridge polygon) */
extern XSIDECL const wchar_t* siBridgeOpType;

/*! Brush Properties type */
extern XSIDECL const wchar_t* siBrushPropertiesType;

/*! Bulge Op Operator type */
extern XSIDECL const wchar_t* siBulgeOpType;

/*! Bounding Volume Constraint type */
extern XSIDECL const wchar_t* siBVolCnsType;

/*! Cage Deform Operator type */
extern XSIDECL const wchar_t* siCageDeformOpType;

/*! Camera Display type */
extern XSIDECL const wchar_t* siCamDispType;

/*! Camera Lens Flare type */
extern XSIDECL const wchar_t* siCameraLensFlareType;

/*! Camera Primitive type */
extern XSIDECL const wchar_t* siCameraPrimType;

/*! Camera Root primitive type */
extern XSIDECL const wchar_t* siCameraRootPrimType;

/*! CameraTxt Operator type (camera texture projection operator) */
extern XSIDECL const wchar_t* siCameraTxtOpType;

/*! Camera Visibility type */
extern XSIDECL const wchar_t* siCamVisType;

/*! CapOp type */
extern XSIDECL const wchar_t* siCapOpType;

/*! Center Operator type (center manipulation) */
extern XSIDECL const wchar_t* siCenterOpType;

/*! Chain Bone Primitive type */
extern XSIDECL const wchar_t* siChainBonePrimType;

/*! Chain End Effector Primitive type */
extern XSIDECL const wchar_t* siChainEffPrimType;

/*! Chain Root Primitive type */
extern XSIDECL const wchar_t* siChainRootPrimType;

/*! Implicit Circle Primitive type */
extern XSIDECL const wchar_t* siCirclePrimType;

/*! Clean Curve Operator type */
extern XSIDECL const wchar_t* siCleanCrvOpType;

/*! Clean Surface Operator type */
extern XSIDECL const wchar_t* siCleanSrfOpType;

/*! Cloth Operator type */
extern XSIDECL const wchar_t* siClothOpType;

/*! Cloth Property type */
extern XSIDECL const wchar_t* siClothPropType;

/*! Cloud Primitive type */
extern XSIDECL const wchar_t* siCloudPrimType;

/*! Cluster Color type */
extern XSIDECL const wchar_t* siClsColorType;

/*! Cluster Center Operator type (cluster center deformation) */
extern XSIDECL const wchar_t* siClsCtrOpType;

/*! Cluster Key type */
extern XSIDECL const wchar_t* siClsKeyType;

/*! Cluster Key Weight Map Operator type */
extern XSIDECL const wchar_t* siClsKeyWgtMapOpType;

/*! Cluster Pose type */
extern XSIDECL const wchar_t* siClsPoseType;

/*! Cluster Shape Combiner Operator type */
extern XSIDECL const wchar_t* siClsShapeCombinerOpType;

/*! Cluster Property UV Texture Projection type */
extern XSIDECL const wchar_t* siClsUVSpaceTxtType;

/*! CoBBoxSclOp type */
extern XSIDECL const wchar_t* siCoBBoxSclOpType;

/*! Collapse Operator type (collapse geometry component) */
extern XSIDECL const wchar_t* siCollapseOpType;

/*! Cone Primitive type */
extern XSIDECL const wchar_t* siConePrimType;

/*! CoPoseFCurveOp type */
extern XSIDECL const wchar_t* siCoPoseFCrvOpType;

/*! Copy Operator type (geometry copy operator for instances) */
extern XSIDECL const wchar_t* siCopyOpType;

/*! copyshape operator type */
extern XSIDECL const wchar_t* siCopyShapeOpType;

/*! CopyUVW Operator type */
extern XSIDECL const wchar_t* siCopyUVWOpType;

/*! Create Polygon Operator type */
extern XSIDECL const wchar_t* siCreatePolyOpType;

/*! Curve Creation Operator type */
extern XSIDECL const wchar_t* siCrvCreationOpType;

/*! CrvControlOp type */
extern XSIDECL const wchar_t* siCrvCtrlOpType;

/*! CrvCutOp type */
extern XSIDECL const wchar_t* siCrvCutOpType;

/*! Curve Deform Operator type */
extern XSIDECL const wchar_t* siCrvDeformOpType;

/*! NURBS Curve List Aggregate Primitive type */
extern XSIDECL const wchar_t* siCrvListAggregatePrimType;

/*! NURBS Curve List Primitive type */
extern XSIDECL const wchar_t* siCrvListPrimType;

/*! Curve To Mesh Converter Operator type (curve list to polygon mesh converter) */
extern XSIDECL const wchar_t* siCrvListToMeshOpType;

/*! Curve Net Operator type */
extern XSIDECL const wchar_t* siCrvNetOpType;

/*! Open/Close Curve Operator type (open or close a curve) */
extern XSIDECL const wchar_t* siCrvOpenCloseOpType;

/*! Reparameterize Curve Operator type */
extern XSIDECL const wchar_t* siCrvReparamOpType;

/*! Curve Shift Operator type */
extern XSIDECL const wchar_t* siCrvShiftOpType;

/*! Cube Primitive type */
extern XSIDECL const wchar_t* siCubePrimType;

/*! Custom Parameter Set type */
extern XSIDECL const wchar_t* siCustomParamSet;

/*! Cycleuvw Operator type */
extern XSIDECL const wchar_t* siCycleUVWType;

/*! Cylinder Primitive type */
extern XSIDECL const wchar_t* siCylinderPrimType;

/*! Deform By Spine Operator type (alternate envelope by spine operator) */
extern XSIDECL const wchar_t* siDeformBySpineOp2Type;

/*! Deform by spine operator type (envelope by spine) */
extern XSIDECL const wchar_t* siDeformBySpineOpType;

/*! Base Deform Operator type */
extern XSIDECL const wchar_t* siDeformOpType;

/*! Delete Component Operator type (delete geometry component) */
extern XSIDECL const wchar_t* siDeleteCompOpType;

/*! Delete Point From Curve Operator type */
extern XSIDECL const wchar_t* siDelPntCrvOpType;

/*! DGlow type */
extern XSIDECL const wchar_t* siDGlowType;

/*! Direction Constraint type */
extern XSIDECL const wchar_t* siDirCnsType;

/*! Disc Primitive type */
extern XSIDECL const wchar_t* siDiscPrimType;

/*! Display type */
extern XSIDECL const wchar_t* siDisplayType;

/*! Dissolve Component Operator type (dissolve geometry component) */
extern XSIDECL const wchar_t* siDissolveCompOpType;

/*! Distance Operator type */
extern XSIDECL const wchar_t* siDistanceOpType;

/*! Distance Constraint type */
extern XSIDECL const wchar_t* siDistCnsType;

/*! Dodecahedron Primitive type */
extern XSIDECL const wchar_t* siDodecahedronPrimType;

/*! Drag Control Primitive type */
extern XSIDECL const wchar_t* siDragCtrlPrimType;

/*! Eddy Control Primitive type */
extern XSIDECL const wchar_t* siEddyCtrlPrimType;

/*! Edit Polygon Operator type */
extern XSIDECL const wchar_t* siEditPolyOpType;

/*! Emission Property type */
extern XSIDECL const wchar_t* siEmissionPropType;

/*! Envelope Operator type */
extern XSIDECL const wchar_t* siEnvelopOpType;

/*! Envelope Selection Clusters Operator type */
extern XSIDECL const wchar_t* siEnvSelClsOpType;

/*! Envelope Weights type */
extern XSIDECL const wchar_t* siEnvWgtType;

/*! Explosion Operator type */
extern XSIDECL const wchar_t* siExplosionOpType;

/*! Extend Curve To Point Operator type */
extern XSIDECL const wchar_t* siExtendCrv2PntOpType;

/*! Extend To Curve Operator type (extend the surface to the selected curve) */
extern XSIDECL const wchar_t* siExtendToCrvOpType;

/*! Extract Curve Operator type */
extern XSIDECL const wchar_t* siExtractCrvOpType;

/*! Extract Curve Segment Operator type (extract a curve from a segment of an existing curve) */
extern XSIDECL const wchar_t* siExtractCrvSegOpType;

/*! Extract Polygons Operator type */
extern XSIDECL const wchar_t* siExtractPolyOpType;

/*! Extract Subcurve Operator type */
extern XSIDECL const wchar_t* siExtractSubCrvOpType;

/*! Extrusion 2 Profiles Operator type (extrude 2 profiles) */
extern XSIDECL const wchar_t* siExtrude2ProfsOpType;

/*! Extrude Component Axis Operator type (extrude geometry component) */
extern XSIDECL const wchar_t* siExtrudeCompAxisOpType;

/*! Extrude Component Operator type (extrude geometry component) */
extern XSIDECL const wchar_t* siExtrudeComponentOpType;

/*! Extrusion Operator type (extrude a curve to create a surface) */
extern XSIDECL const wchar_t* siExtrusionOpType;

/*! Fan Force Object type */
extern XSIDECL const wchar_t* siFanType;

/*! Fillet Curves Operator type (intersecting or not) */
extern XSIDECL const wchar_t* siFilletCrvOpType;

/*! Fillet Surfaces Operator type */
extern XSIDECL const wchar_t* siFilletSrfOpType;

/*! Fill Hole Operator type */
extern XSIDECL const wchar_t* siFillHoleOpType;

/*! Edge Filter Operator type */
extern XSIDECL const wchar_t* siFilterEdgeOpType;

/*! Polygon Filter Operator type */
extern XSIDECL const wchar_t* siFilterPolyOpType;

/*! Fit Curve Operator type (fit a new curve on to the selected curve) */
extern XSIDECL const wchar_t* siFitCrvOpType;

/*! Fit Surface Operator type (fit a new surface on to the selected surface) */
extern XSIDECL const wchar_t* siFitSrfOpType;

/*! Flip UVW Operator type */
extern XSIDECL const wchar_t* siFlipUVWOpType;

/*! Fluid Operator type */
extern XSIDECL const wchar_t* siFluidOpType;

/*! Four Sided Operator type */
extern XSIDECL const wchar_t* siFourSidedOpType;

/*! Fur Operator type (fur generator operator) */
extern XSIDECL const wchar_t* siFurOpType;

/*! Fur Primitive type */
extern XSIDECL const wchar_t* siFurPrimType;

/*! Fur Property type */
extern XSIDECL const wchar_t* siFurPropType;

/*! Fur Simulation Operator type */
extern XSIDECL const wchar_t* siFurSimOpType;

/*! Base Generator Operator type */
extern XSIDECL const wchar_t* siGeneratorOpType;

/*! Geometry Approximation type (tesselation) */
extern XSIDECL const wchar_t* siGeomApproxType;

/*! GeoShader Primitive Type */
extern XSIDECL const wchar_t* siGeoShaderPrimType;

/*! GeoTxtOp type (copy geometry xyz to uvw texture operator) */
extern XSIDECL const wchar_t* siGeoTxtOpType;

/*! Global Transform type */
extern XSIDECL const wchar_t* siGlobalType;

/*! Gravity Force Control Object type */
extern XSIDECL const wchar_t* siGravityCtrlType;

/*! Grid Primitive type */
extern XSIDECL const wchar_t* siGridPrimType;

/*! Group type */
extern XSIDECL const wchar_t* siGroup;

/*! Heal Operator type */
extern XSIDECL const wchar_t* siHealOpType;

/*! Heal UVW Operator type */
extern XSIDECL const wchar_t* siHealUVWOpType;

/*! Icosahedron Primitive type */
extern XSIDECL const wchar_t* siIcosahedronPrimType;

/*! Image Clip type */
extern XSIDECL const wchar_t* siImageClipType;

/*! Image Crop Operator type */
extern XSIDECL const wchar_t* siImageCropOpType;

/*! Image FX Operator type */
extern XSIDECL const wchar_t* siImageFXOpType;

/*! Image Object type */
extern XSIDECL const wchar_t* siImageObjectType;

/*! Image Source Operator type */
extern XSIDECL const wchar_t* siImageSourceOpType;

/*! Image Source type */
extern XSIDECL const wchar_t* siImageSourceType;

/*! Insert Curve Knot Operator type */
extern XSIDECL const wchar_t* siInsertCrvKnotOpType;

/*! Insert Surface Knot Operator type (inserts a knot in a nurbs surface) */
extern XSIDECL const wchar_t* siInsSrfKnotOpType;

/*! Intersect Surfaces Operator type (extract a curve from the intersection of two surfaces) */
extern XSIDECL const wchar_t* siIntersectSrfOpType;

/*! Inverse Curve Operator type (inverse curve direction) */
extern XSIDECL const wchar_t* siInvCrvOpType;

/*! Invert Polygon Operator type */
extern XSIDECL const wchar_t* siInvertPolyOpType;

/*! Name of the InvisiblePolygons cluster */
extern XSIDECL const wchar_t* siInvisiblePolygonsClusterName;

/*! Inverse Normals Operator type (inverse normals direction) */
extern XSIDECL const wchar_t* siInvSrfOpType;

/*! Kinematic Chain Properties type */
extern XSIDECL const wchar_t* siKineChainPropType;

/*! Kinematic Chain type */
extern XSIDECL const wchar_t* siKineChainType;

/*! Kinematic Constraint type */
extern XSIDECL const wchar_t* siKineCnsType;

/*! Kinematics Information type */
extern XSIDECL const wchar_t* siKineInfoType;

/*! Kinematic Joint type */
extern XSIDECL const wchar_t* siKineJointType;

/*! Lattice Operator type */
extern XSIDECL const wchar_t* siLatticeOpType;

/*! Lattice Primitive type */
extern XSIDECL const wchar_t* siLatticePrimType;

/*! Layers type */
extern XSIDECL const wchar_t* siLayersType;

/*! Light Lens Flare type */
extern XSIDECL const wchar_t* siLightLensFlareType;

/*! Light Primitive type */
extern XSIDECL const wchar_t* siLightPrimType;

/*! Line Constraint type */
extern XSIDECL const wchar_t* siLineCnsType;

/*! Local Transform type */
extern XSIDECL const wchar_t* siLocalType;

/*! Loft Generator Operator type */
extern XSIDECL const wchar_t* siLoftOpType;

/*! Component Map Operator type */
extern XSIDECL const wchar_t* siMapCompOpType;

/*! Mark Hard Edge/Vertex Operator type */
extern XSIDECL const wchar_t* siMarkHardEdgeVertexOpType;

/*! Material type */
extern XSIDECL const wchar_t* siMaterialType;

/*! Merge Curves Operator type */
extern XSIDECL const wchar_t* siMergeCrvOpType;

/*! Merge Meshes Operator type */
extern XSIDECL const wchar_t* siMergeMeshOpType;

/*! Merge Surfaces Operator type */
extern XSIDECL const wchar_t* siMergeSrfOpType;

/*! Mesh Local Subdivision Operator type */
extern XSIDECL const wchar_t* siMeshLocalSubdivisionOpType;

/*! Mesh Subdivision Operator type */
extern XSIDECL const wchar_t* siMeshSubdivOpType;

/*! Mesh Subdivide With Center Operator type (mesh subdivision with center) */
extern XSIDECL const wchar_t* siMeshSubdivWithCenterOpType;

/*! Mixer Animation Clip type */
extern XSIDECL const wchar_t* siMixerAnimClipType;

/*! Mixer Animation Action Track type */
extern XSIDECL const wchar_t* siMixerAnimTrackType;

/*! Mixer Audio Clip type */
extern XSIDECL const wchar_t* siMixerAudioClipType;

/*! Mixer Audio Track type */
extern XSIDECL const wchar_t* siMixerAudioTrackType;

/*! Mixer Property type */
extern XSIDECL const wchar_t* siMixerPropType;

/*! Mixer Shape Clip type */
extern XSIDECL const wchar_t* siMixerShapeClipType;

/*! Mixer Shape Track type */
extern XSIDECL const wchar_t* siMixerShapeTrackType;

/*! Mixer type (property) */
extern XSIDECL const wchar_t* siMixerType;

/*! 3D Model Clip type */
extern XSIDECL const wchar_t* siModelClipType;

/*! Model Null Primitive type */
extern XSIDECL const wchar_t* siModelNullPrimType;

/*! 3D Model type */
extern XSIDECL const wchar_t* siModelType;

/*! Motion Blur type */
extern XSIDECL const wchar_t* siMotionBlurType;

/*! Move Component Operator type */
extern XSIDECL const wchar_t* siMoveComponentOpType;

/*! N Points Constraint type */
extern XSIDECL const wchar_t* siNPntCnsType;

/*! Null Primitive type */
extern XSIDECL const wchar_t* siNullPrimType;

/*! Nurbs To Mesh Operator type */
extern XSIDECL const wchar_t* siNurbsToMeshOpType;

/*! Object To Cluster Constraint type */
extern XSIDECL const wchar_t* siObjClsCnsType;

/*! Obstacle Property type */
extern XSIDECL const wchar_t* siObstaclePropType;

/*! Octahedron Primitive type */
extern XSIDECL const wchar_t* siOctahedronPrimType;

/*! Offset Cluster Operator type */
extern XSIDECL const wchar_t* siOffsetClsOpType;

/*! Offset Curve Operator type */
extern XSIDECL const wchar_t* siOffsetCrvOpType;

/*! Offset Surface Operator type (offset a surface to create a new surface) */
extern XSIDECL const wchar_t* siOffsetSrfOpType;

/*! Open/Close Surface Operator type (open or close a surface) */
extern XSIDECL const wchar_t* siOpenCloseSrfOpType;

/*! Orientation Constraint type */
extern XSIDECL const wchar_t* siOriCnsType;

/*! Override type */
extern XSIDECL const wchar_t* siOverrideType;

/*! Particle decay type */
extern XSIDECL const wchar_t* siParDecayType;

/*! ParObstExtSparks type */
extern XSIDECL const wchar_t* siParObstExtSparksType;

/*! Particle Event type */
extern XSIDECL const wchar_t* siParticleEventType;

/*! Particles Operator type */
extern XSIDECL const wchar_t* siParticlesOpType;

/*! Path Constraint type */
extern XSIDECL const wchar_t* siPathCnsType;

/*! Pin Joint Constraint type */
extern XSIDECL const wchar_t* siPinJointCnsType;

/*! Plane Constraint type */
extern XSIDECL const wchar_t* siPlaneCnsType;

/*! PolyMeshPNetOp type */
extern XSIDECL const wchar_t* siPolyMeshPNetOpType;

/*! Position Constraint type */
extern XSIDECL const wchar_t* siPosCnsType;

/*! Pose Constraint type */
extern XSIDECL const wchar_t* siPoseCnsType;

/*! Project Curve Operator type (project a curve on to a surface) */
extern XSIDECL const wchar_t* siProjectCrvOpType;

/*! Proportional Modeling Operator type */
extern XSIDECL const wchar_t* siProportionalOpType;

/*! Proportional Volume Operator type */
extern XSIDECL const wchar_t* siPropVolumeOpType;

/*! Push Operator type */
extern XSIDECL const wchar_t* siPushOpType;

/*! QStretch Operator type (quick stretch) */
extern XSIDECL const wchar_t* siQStretchOpType;

/*! Randomize Operator type */
extern XSIDECL const wchar_t* siRandomizeOpType;

/*! Reference Plane type */
extern XSIDECL const wchar_t* siRefPlaneType;

/*! Relax UVW Operator type */
extern XSIDECL const wchar_t* siRelaxUVWOpType;

/*! Remove Curve Knot Operator type */
extern XSIDECL const wchar_t* siRemoveCrvKnotOpType;

/*! Remove Surface Knot Operator type (removes a knot from a nurbs surface) */
extern XSIDECL const wchar_t* siRemSrfKnotOpType;

/*! Render Map type */
extern XSIDECL const wchar_t* siRenderMapType;

/*! Revolution Operator type */
extern XSIDECL const wchar_t* siRevolutionOpType;

/*! Camera Rotoscopy type */
extern XSIDECL const wchar_t* siRotoscopeType;

/*! Scene Ambience type */
extern XSIDECL const wchar_t* siSceneAmbienceType;

/*! Scene Display Colors type */
extern XSIDECL const wchar_t* siSceneColorsType;

/*! Scaling Constraint type */
extern XSIDECL const wchar_t* siSclCnsType;

/*! SCM Fixer Operator type */
extern XSIDECL const wchar_t* siSCMFixerOpType;

/*! SCM Topology Operator type */
extern XSIDECL const wchar_t* siSCMTopologyOpType;

/*! Scripted Operator type (scripted operator host) */
extern XSIDECL const wchar_t* siScriptedOpType;

/*! Set Edge Crease Value Operator type */
extern XSIDECL const wchar_t* siSetEdgeCreaseValueOpType;

/*! Shape Action Compound Clip type (base compound clip) */
extern XSIDECL const wchar_t* siShapeActionCompoundClipType;

/*! Shape Action type */
extern XSIDECL const wchar_t* siShapeActionType;

/*! Shear Operator type */
extern XSIDECL const wchar_t* siShearOpType;

/*! Shrink Wrap Operator type */
extern XSIDECL const wchar_t* siShrinkWrapOpType;

/*! Skeleton Controller Operator type */
extern XSIDECL const wchar_t* siSkelCtrlOpPropType;

/*! Smooth Envelope Weight Operator type */
extern XSIDECL const wchar_t* siSmoothEnvelopeWgtOpType;

/*! Snap Boundary Operator type */
extern XSIDECL const wchar_t* siSnapBoundOpType;

/*! Snap To Curves Operator type */
extern XSIDECL const wchar_t* siSnapCrvOpType;

/*! Snip Surface Operator type */
extern XSIDECL const wchar_t* siSnipSrfOpType;

/*! SoftBody Operator type */
extern XSIDECL const wchar_t* siSoftBodyOpType;

/*! Sphere Primitive type */
extern XSIDECL const wchar_t* siSpherePrimType;

/*! Deform by Spine Operator 2 type (alternate operator) */
extern XSIDECL const wchar_t* siSpineDefOp2Type;

/*! Deform by Spine Operator type */
extern XSIDECL const wchar_t* siSpineDefOpType;

/*! Spine Weight Map Operator 3 type */
extern XSIDECL const wchar_t* siSpineWgtMapOp3Type;

/*! Deform by Spine Weight Map Operator 4 type */
extern XSIDECL const wchar_t* siSpineWgtMapOp4Type;

/*! Implicit Spiral Primitive type */
extern XSIDECL const wchar_t* siSpiralPrimType;

/*! Split Edge Operator type (split some edges) */
extern XSIDECL const wchar_t* siSplitEdgeOpType;

/*! Split Polygon Operator type (split some polygons) */
extern XSIDECL const wchar_t* siSplitPolyOpType;

/*! Spot Interest Primitive type */
extern XSIDECL const wchar_t* siSpotInterestPrimType;

/*! Spot Root Primitive type */
extern XSIDECL const wchar_t* siSpotRootPrimType;

/*! Implicit Square Primitive type */
extern XSIDECL const wchar_t* siSquarePrimType;

/*! Surface Constraint type */
extern XSIDECL const wchar_t* siSrfCnsType;

/*! Curve Constraint type */
extern XSIDECL const wchar_t* siCrvCnsType;

/*! Surface Curve Inverse Operator type */
extern XSIDECL const wchar_t* siSrfCrvInverseOpType;

/*! Surface Curve Shift Operator type */
extern XSIDECL const wchar_t* siSrfCrvShiftOpType;

/*! SrfControlOp type */
extern XSIDECL const wchar_t* siSrfCtrlOpType;

/*! Surface Deform Operator type */
extern XSIDECL const wchar_t* siSrfDefOpType;

/*! NURBS Surface Mesh Primitive type */
extern XSIDECL const wchar_t* siSrfMeshPrimType;

/*! SurfacePQ Operator type */
extern XSIDECL const wchar_t* siSrfPQOpType;

/*! Reparameterize Surface Operator type */
extern XSIDECL const wchar_t* siSrfReparamOpType;

/*! Surface Shift Operator type */
extern XSIDECL const wchar_t* siSrfShiftOpType;

/*! Surface Subdivision Operator type */
extern XSIDECL const wchar_t* siSrfSubdivisionOpType;

/*! Surface UV Control Operator type (Nurbs UV control) */
extern XSIDECL const wchar_t* siSrfUVCtrlOpType;

/*! Start Offset Operator type */
extern XSIDECL const wchar_t* siStartOffsetOpType;

/*! Stitch Curve To Curve Operator type (stitch curves together) */
extern XSIDECL const wchar_t* siStitchCrvToCrvOpType;

/*! Stitch Curve To Surface Operator type */
extern XSIDECL const wchar_t* siStitchCrvToSrfOpType;

/*! Stitch Surfaces Operator type (stitch surfaces and curves together) */
extern XSIDECL const wchar_t* siStitchSrfOpType;

/*! Gap Stroke Operator type */
extern XSIDECL const wchar_t* siStrokeOpType;

/*! Subcomponents type */
extern XSIDECL const wchar_t* siSubCompType;

/*! Subdivide Edge Operator type */
extern XSIDECL const wchar_t* siSubdivEdgeOpType;

/*! Subdivide Polygon Operator type */
extern XSIDECL const wchar_t* siSubdivPolyOpType;

/*! Swap Surface UVs Operator type */
extern XSIDECL const wchar_t* siSwapSrfUVOpType;

extern XSIDECL const wchar_t* siSymmetrizePolygon;


/*! Symmetry Constraint type */
extern XSIDECL const wchar_t* siSymmetryCnsType;

/*! Symmetry Map Genrator Operator type */
extern XSIDECL const wchar_t* siSymmetryMapOpType;

/*! Symmetry Map type */
extern XSIDECL const wchar_t* siSymmetryMapType;

/*! Synoptic View type */
extern XSIDECL const wchar_t* siSynopticViewType;

/*! Taper Operator type */
extern XSIDECL const wchar_t* siTaperOpType;

/*! Tetrahedron Primitive type */
extern XSIDECL const wchar_t* siTetrahedronPrimType;

/*! Text To CurveList Operator type */
extern XSIDECL const wchar_t* siTextToCrvListOpType;

/*! Torus Primitive type */
extern XSIDECL const wchar_t* siTorusPrimType;

/*! Trajectory Constraint type */
extern XSIDECL const wchar_t* siTrajCnsType;

/*! Transform Setup type */
extern XSIDECL const wchar_t* siTransformSetupType;

/*! Twist Operator type */
extern XSIDECL const wchar_t* siTwistOpType;

/*! Texture Map type */
extern XSIDECL const wchar_t* siTxtMapType;

/*! Texture Projection Operator type */
extern XSIDECL const wchar_t* siTxtOpType;

/*! Texture Support type */
extern XSIDECL const wchar_t* siTxtSupportType;

/*! Turbulence Control Primitive type */
extern XSIDECL const wchar_t* siTurbulenceCtrlPrimType;

/*! Up Vector Constraint type */
extern XSIDECL const wchar_t* siUpVctCnsType;

/*! User Normal Property type */
extern XSIDECL const wchar_t* siUserNormalType;

/*! UV Projection Definition type (definition of how to perform a texture projection) */
extern XSIDECL const wchar_t* siUVProjDefType;

/*! Vertex Color type */
extern XSIDECL const wchar_t* siVertexcolorType;

/*! Visibility type */
extern XSIDECL const wchar_t* siVisibilityType;

/*! Volume Deform type (implicit sphere volume) */
extern XSIDECL const wchar_t* siVolumeDeformType;

/*! Volumic Light Scene Local Property Set type */
extern XSIDECL const wchar_t* siVolumicLightSceneType;

/*! Volumic Light Effect type */
extern XSIDECL const wchar_t* siVolumicLightType;

/*! Vortex Control Object type (magnetic force) */
extern XSIDECL const wchar_t* siVortexCtrlType;

/*! Vertex Color Painter Operator type */
extern XSIDECL const wchar_t* siVtxColPainterOpType;

/*! Vertex Color Property Generator Operator type */
extern XSIDECL const wchar_t* siVtxColPropOpType;

/*! Wave Control Object type */
extern XSIDECL const wchar_t* siWaveCtrlType;

/*! Wave Operator type */
extern XSIDECL const wchar_t* siWaveOpType;

/*! Weld Edges Operator type (weld pair of edges together) */
extern XSIDECL const wchar_t* siWeldEdgesOpType;

/*! Weld Points Operator type (weld two points together) */
extern XSIDECL const wchar_t* siWeldPntOpType;

/*! Weight Map Connection Operator type */
extern XSIDECL const wchar_t* siWgtMapCnxOpType;

/*! Weight Map Generator Operator type */
extern XSIDECL const wchar_t* siWgtMapOpType;

/*! Weight Maps Mixer Operator type */
extern XSIDECL const wchar_t* siWgtMapsMixOpType;

/*! Weight Map type */
extern XSIDECL const wchar_t* siWgtMapType;

/*! Weight Painter Operator type */
extern XSIDECL const wchar_t* siWgtPainterOpType;

/*! Wind Force Object type */
extern XSIDECL const wchar_t* siWindType;

/*! Weight Stroke Operator type */
extern XSIDECL const wchar_t* siWtStrokeOpType;

extern XSIDECL const wchar_t* siControlNumber;


extern XSIDECL const wchar_t* siControlString;


extern XSIDECL const wchar_t* siControlBoolean;


extern XSIDECL const wchar_t* siControlCheck;


extern XSIDECL const wchar_t* siControlRadio;


extern XSIDECL const wchar_t* siControlCombo;


extern XSIDECL const wchar_t* siControlRGBA;


extern XSIDECL const wchar_t* siControlRGB;


extern XSIDECL const wchar_t* siControlIconList;


extern XSIDECL const wchar_t* siControlBitmap;


extern XSIDECL const wchar_t* siControlListBox;


extern XSIDECL const wchar_t* siControlButton;


extern XSIDECL const wchar_t* siControlStatic;


extern XSIDECL const wchar_t* siControlEdit;


extern XSIDECL const wchar_t* siControlFilePath;


extern XSIDECL const wchar_t* siControlFolder;


extern XSIDECL const wchar_t* siControlFCurve;


extern XSIDECL const wchar_t* siControlGrid;


extern XSIDECL const wchar_t* siControlSynoptic;


extern XSIDECL const wchar_t* siUICX;


extern XSIDECL const wchar_t* siUICY;


extern XSIDECL const wchar_t* siUINoLabel;


extern XSIDECL const wchar_t* siUIValueOnly;


extern XSIDECL const wchar_t* siUIAlignLeft;


extern XSIDECL const wchar_t* siUIAlignRight;


extern XSIDECL const wchar_t* siUIAlignCenter;


extern XSIDECL const wchar_t* siUIContinue;


extern XSIDECL const wchar_t* siUIShowFrame;


extern XSIDECL const wchar_t* siUISyncSlider;


extern XSIDECL const wchar_t* siUILogarithmic;


extern XSIDECL const wchar_t* siUIDecimals;


extern XSIDECL const wchar_t* siUINoSlider;


extern XSIDECL const wchar_t* siUIThumbWheel;


extern XSIDECL const wchar_t* siUICaption;


extern XSIDECL const wchar_t* siUIStyle;


extern XSIDECL const wchar_t* siUIColumnCnt;


extern XSIDECL const wchar_t* siUILineCnt;


extern XSIDECL const wchar_t* siUIColumnGap;


extern XSIDECL const wchar_t* siUILineGap;


extern XSIDECL const wchar_t* siUISelectionColor;


extern XSIDECL const wchar_t* siUIUseSelectionIcon;


extern XSIDECL const wchar_t* siUIFilePath;


extern XSIDECL const wchar_t* siUIFileFilter;


extern XSIDECL const wchar_t* siUIInitialDir;


extern XSIDECL const wchar_t* siUISubFolder;


extern XSIDECL const wchar_t* siUIAudioFile;


extern XSIDECL const wchar_t* siUIImageFile;


extern XSIDECL const wchar_t* siUIOpenFile;


extern XSIDECL const wchar_t* siUIFileMustExist;


extern XSIDECL const wchar_t* siUIFCurveLabelX;


extern XSIDECL const wchar_t* siUIFCurveLabelY;


extern XSIDECL const wchar_t* siUIFCurveViewMinX;


extern XSIDECL const wchar_t* siUIFCurveViewMinY;


extern XSIDECL const wchar_t* siUIFCurveViewMaxX;


extern XSIDECL const wchar_t* siUIFCurveViewMaxY;


extern XSIDECL const wchar_t* siUIFCurveGridSpaceX;


extern XSIDECL const wchar_t* siUIFCurveGridSpaceY;


extern XSIDECL const wchar_t* siUIFCurveNoGrid;


extern XSIDECL const wchar_t* siUIFCurveNoRulerX;


extern XSIDECL const wchar_t* siUIFCurveNoRulerY;


extern XSIDECL const wchar_t* siUIFCurveSnapX;


extern XSIDECL const wchar_t* siUIFCurveSnapY;


extern XSIDECL const wchar_t* siUIFCurveGhosting;


extern XSIDECL const wchar_t* siUIFCurveColorNonBijective;


extern XSIDECL const wchar_t* siUIFCurveShowTimeCursor;


extern XSIDECL const wchar_t* siUIGridColumnWidths;


extern XSIDECL const wchar_t* siUIGridReadOnlyColumns;


extern XSIDECL const wchar_t* siUIGridShowColumns;


extern XSIDECL const wchar_t* siUIGridLockColumnHeader;


extern XSIDECL const wchar_t* siUIGridLockRowHeader;


extern XSIDECL const wchar_t* siUIGridHideColumnHeader;


extern XSIDECL const wchar_t* siUIGridHideRowHeader;


extern XSIDECL const wchar_t* siUIGridSelectionMode;


extern XSIDECL const wchar_t* siUIButtonDisable;


extern XSIDECL const wchar_t* siUILabel;


extern XSIDECL const wchar_t* siUIItems;


extern XSIDECL const wchar_t* siUIType;


extern XSIDECL const wchar_t* siUIWidthPercentage;


extern XSIDECL const wchar_t* siUILabelPercentage;


extern XSIDECL const wchar_t* siUILabelMinPixels;


extern XSIDECL const wchar_t* siUIMultiSelectionListBox;


extern XSIDECL const wchar_t* siUILogic;


extern XSIDECL const wchar_t* siUILanguage;


extern XSIDECL const wchar_t* siUILogicPrefix;


extern XSIDECL const wchar_t* siUILogicFile;


extern XSIDECL const wchar_t* siUIHelpFile;


extern XSIDECL const wchar_t* siUIHelpID;


extern XSIDECL const wchar_t* siUIDictionary;


extern XSIDECL const wchar_t* siUIShowChildren;


extern XSIDECL const wchar_t* siArgHandlerFrame;


extern XSIDECL const wchar_t* siArgHandlerFrameRate;


extern XSIDECL const wchar_t* siArgHandlerSingleObj;


extern XSIDECL const wchar_t* siArgHandlerCollection;


extern XSIDECL const wchar_t* siArgHandlerMarkedParameters;


extern XSIDECL const wchar_t* siArgHandlerAnimatableParameters;


extern XSIDECL const wchar_t* siFilterObjectType;


extern XSIDECL const wchar_t* siFilter3DObjectType;


extern XSIDECL const wchar_t* siFilterSubComponentType;


extern XSIDECL const wchar_t* siFilterPropertyType;


extern XSIDECL const wchar_t* siClipAnimationType;


extern XSIDECL const wchar_t* siClipShapeType;


extern XSIDECL const wchar_t* siClipAudioType;


extern XSIDECL const wchar_t* siClipAnimCompoundType;


extern XSIDECL const wchar_t* siClipShapeCompoundType;


extern XSIDECL const wchar_t* siClipMixerType;


extern XSIDECL const wchar_t* siClipImageType;


extern XSIDECL const wchar_t* siTrackAnimationType;


extern XSIDECL const wchar_t* siTrackShapeType;


extern XSIDECL const wchar_t* siTrackAudioType;


extern XSIDECL const wchar_t* siTransitionStandardType;


extern XSIDECL const wchar_t* siTransitionCardinalType;


extern XSIDECL const wchar_t* siTransitionBridgeType;


extern XSIDECL const wchar_t* siFCurveAnimItem;


extern XSIDECL const wchar_t* siStaticValueAnimItem;


extern XSIDECL const wchar_t* siShapeKeyAnimItem;


extern XSIDECL const wchar_t* siExpressionAnimItem;


extern XSIDECL const wchar_t* siConstraintAnimItem;


extern XSIDECL const wchar_t* siAnimCompoundAnimItem;


extern XSIDECL const wchar_t* siShapeCompoundAnimItem;


extern XSIDECL const wchar_t* siFCurveMappedItem;


extern XSIDECL const wchar_t* siStaticValueMappedItem;


extern XSIDECL const wchar_t* siShapeKeyMappedItem;


extern XSIDECL const wchar_t* siExpressionMappedItem;


extern XSIDECL const wchar_t* siConstraintMappedItem;


extern XSIDECL const wchar_t* siCompoundMappedItem;


extern XSIDECL const wchar_t* siShapeCompoundMappedItem;


extern XSIDECL const wchar_t* siSEFilterSceneNodes;


extern XSIDECL const wchar_t* siSEFilterObjectNodes;


extern XSIDECL const wchar_t* siSEFilterOperatorNodes;


extern XSIDECL const wchar_t* siSEFilterPrimitiveNodes;


extern XSIDECL const wchar_t* siSEFilterPropertyNodes;


extern XSIDECL const wchar_t* siSEFilterClusterNodes;


extern XSIDECL const wchar_t* siSEFilterGroupNodes;


extern XSIDECL const wchar_t* siSEFilterShaderNodes;


extern XSIDECL const wchar_t* siSEFilterMaterialNodes;


extern XSIDECL const wchar_t* siSEFilterActionNodes;


extern XSIDECL const wchar_t* siSEFilterModelNodes;


extern XSIDECL const wchar_t* siSEFilterParameterNodes;


extern XSIDECL const wchar_t* siSEFilterAnimatableNodes;


extern XSIDECL const wchar_t* siSEFilterMiscellaneousNodes;


extern XSIDECL const wchar_t* siSEFilterAllNodeTypes;


extern XSIDECL const wchar_t* siSEFilterAnimatedNodes;


extern XSIDECL const wchar_t* siSEFilterKeywordGeneral;


extern XSIDECL const wchar_t* siSEFilterKeywordModeling;


extern XSIDECL const wchar_t* siSEFilterKeywordAnimation;


extern XSIDECL const wchar_t* siSEFilterKeywordRendering;


extern XSIDECL const wchar_t* siSEFilterKeywordViewing;


extern XSIDECL const wchar_t* siSEFilterKeywordCustom;


extern XSIDECL const wchar_t* siSEFilterKeywordKinematics;


extern XSIDECL const wchar_t* siSEFilterKeywordAll;



}; // XSI namespace


#endif /* _XSIDECL_H_ */

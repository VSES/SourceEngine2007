//====== Copyright © 1996-2004, Valve Corporation, All rights reserved. =======
//
// Purpose: See notes below
//
//=============================================================================

#include "movieobjects/movieobjects.h"

// YOU MUST INCLUDE THIS FILE INTO ANY PROJECT WHICH USES THE movieobjects.lib FILE
// This hack causes the class factories for the element types to be imported into the compiled code...

// Basic type (in datamodel)
USING_ELEMENT_FACTORY( CDmElement );

// Movie types
USING_ELEMENT_FACTORY( CDmeTransform );
USING_ELEMENT_FACTORY( CDmeTransformList );
USING_ELEMENT_FACTORY( CDmeVertexData );
USING_ELEMENT_FACTORY( CDmeMesh );
USING_ELEMENT_FACTORY( CDmeDag );
USING_ELEMENT_FACTORY( CDmeFaceSet );
USING_ELEMENT_FACTORY( CDmeModel );
USING_ELEMENT_FACTORY( CDmeJoint );
USING_ELEMENT_FACTORY( CDmeAttachment );
USING_ELEMENT_FACTORY( CDmeMakefile );
USING_ELEMENT_FACTORY( CDmeMDLMakefile );
USING_ELEMENT_FACTORY( CDmeDCCMakefile );
USING_ELEMENT_FACTORY( CDmeMayaMakefile );
USING_ELEMENT_FACTORY( CDmeXSIMakefile );
USING_ELEMENT_FACTORY( CDmeSourceDCCFile );
USING_ELEMENT_FACTORY( CDmeSourceMayaFile );
USING_ELEMENT_FACTORY( CDmeSourceXSIFile );
USING_ELEMENT_FACTORY( CDmeAnimationList );
USING_ELEMENT_FACTORY( CDmeClip );
USING_ELEMENT_FACTORY( CDmeChannelsClip );
USING_ELEMENT_FACTORY( CDmeChannel );
USING_ELEMENT_FACTORY( CDmeTimeFrame );
USING_ELEMENT_FACTORY( CDmeTrackGroup );
USING_ELEMENT_FACTORY( CDmeTrack );

USING_ELEMENT_FACTORY( CDmeCombinationDominationRule );
USING_ELEMENT_FACTORY( CDmeCombinationInputControl );
USING_ELEMENT_FACTORY( CDmeCombinationOperator );

USING_ELEMENT_FACTORY( CDmeIntLog );
USING_ELEMENT_FACTORY( CDmeFloatLog );
USING_ELEMENT_FACTORY( CDmeBoolLog );
USING_ELEMENT_FACTORY( CDmeColorLog );
USING_ELEMENT_FACTORY( CDmeVector2Log );
USING_ELEMENT_FACTORY( CDmeVector3Log );
USING_ELEMENT_FACTORY( CDmeVector4Log );
USING_ELEMENT_FACTORY( CDmeQAngleLog );
USING_ELEMENT_FACTORY( CDmeQuaternionLog );
USING_ELEMENT_FACTORY( CDmeVMatrixLog );

USING_ELEMENT_FACTORY( CDmeIntLogLayer );
USING_ELEMENT_FACTORY( CDmeFloatLogLayer );
USING_ELEMENT_FACTORY( CDmeBoolLogLayer );
USING_ELEMENT_FACTORY( CDmeColorLogLayer );
USING_ELEMENT_FACTORY( CDmeVector2LogLayer );
USING_ELEMENT_FACTORY( CDmeVector3LogLayer );
USING_ELEMENT_FACTORY( CDmeVector4LogLayer );
USING_ELEMENT_FACTORY( CDmeQAngleLogLayer );
USING_ELEMENT_FACTORY( CDmeQuaternionLogLayer );
USING_ELEMENT_FACTORY( CDmeVMatrixLogLayer );

USING_ELEMENT_FACTORY( CDmeIntCurveInfo );
USING_ELEMENT_FACTORY( CDmeFloatCurveInfo );
USING_ELEMENT_FACTORY( CDmeBoolCurveInfo );
USING_ELEMENT_FACTORY( CDmeColorCurveInfo );
USING_ELEMENT_FACTORY( CDmeVector2CurveInfo );
USING_ELEMENT_FACTORY( CDmeVector3CurveInfo );
USING_ELEMENT_FACTORY( CDmeVector4CurveInfo );
USING_ELEMENT_FACTORY( CDmeQAngleCurveInfo );
USING_ELEMENT_FACTORY( CDmeQuaternionCurveInfo );
USING_ELEMENT_FACTORY( CDmeVMatrixCurveInfo );

USING_ELEMENT_FACTORY( CDmeComponent );
USING_ELEMENT_FACTORY( CDmeSingleIndexedComponent );
USING_ELEMENT_FACTORY( CDmeDrawSettings );
USING_ELEMENT_FACTORY( CDmeEyePosition );
USING_ELEMENT_FACTORY( CDmeEyeball );

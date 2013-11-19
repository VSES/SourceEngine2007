//====== Copyright © 1996-2004, Valve Corporation, All rights reserved. =======
//
// Purpose: See notes below
//
//=============================================================================

#include "movieobjects/movieobjects.h"

#include "movieobjects/movieobjects_compiletools.cpp"

// YOU MUST INCLUDE THIS FILE INTO ANY PROJECT WHICH USES THE movieobjects.lib FILE
// This hack causes the class factories for the element types to be imported into the compiled code...

// Movie types
USING_ELEMENT_FACTORY( CDmeCamera );
USING_ELEMENT_FACTORY( CDmeSoundClip );
USING_ELEMENT_FACTORY( CDmeFilmClip );
USING_ELEMENT_FACTORY( CDmeMDL );
USING_ELEMENT_FACTORY( CDmeMaterial );
USING_ELEMENT_FACTORY( CDmeLight );
USING_ELEMENT_FACTORY( CDmeGameModel );
USING_ELEMENT_FACTORY( CDmeSound );
USING_ELEMENT_FACTORY( CDmeGameSound );

USING_ELEMENT_FACTORY( CDmeMorphOperator );
USING_ELEMENT_FACTORY( CDmeTransformOperator );
USING_ELEMENT_FACTORY( CDmeExpressionOperator );

USING_ELEMENT_FACTORY( CDmeGameModelInput );
USING_ELEMENT_FACTORY( CDmeGamePortal );
USING_ELEMENT_FACTORY( CDmeMouseInput );
USING_ELEMENT_FACTORY( CDmeKeyboardInput );

USING_ELEMENT_FACTORY( CDmeEditorAttributeInfo );
USING_ELEMENT_FACTORY( CDmeEditorChoicesInfo );
USING_ELEMENT_FACTORY( CDmeEditorType );
USING_ELEMENT_FACTORY( CDmeEditorTypeDictionary );

USING_ELEMENT_FACTORY( CDmePackColorOperator );
USING_ELEMENT_FACTORY( CDmePackVector2Operator );
USING_ELEMENT_FACTORY( CDmePackVector3Operator );
USING_ELEMENT_FACTORY( CDmePackVector4Operator );
USING_ELEMENT_FACTORY( CDmePackQAngleOperator );
USING_ELEMENT_FACTORY( CDmePackQuaternionOperator );
USING_ELEMENT_FACTORY( CDmePackVMatrixOperator );

USING_ELEMENT_FACTORY( CDmeUnpackColorOperator );
USING_ELEMENT_FACTORY( CDmeUnpackVector2Operator );
USING_ELEMENT_FACTORY( CDmeUnpackVector3Operator );
USING_ELEMENT_FACTORY( CDmeUnpackVector4Operator );
USING_ELEMENT_FACTORY( CDmeUnpackQAngleOperator );
USING_ELEMENT_FACTORY( CDmeUnpackQuaternionOperator );
USING_ELEMENT_FACTORY( CDmeUnpackVMatrixOperator );

USING_ELEMENT_FACTORY( CDmeAnimationSet );
USING_ELEMENT_FACTORY( CDmePhonemeMapping );
USING_ELEMENT_FACTORY( CDmeBalanceToStereoCalculatorOperator );
USING_ELEMENT_FACTORY( CDmeGlobalFlexControllerOperator );

USING_ELEMENT_FACTORY( CDmeTimeSelection );
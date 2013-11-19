//=============================================================================
//
// The copyright to the contents herein is the property of Valve, L.L.C.
// The contents may be used and/or copied only with the written permission of
// Valve, L.L.C., or in accordance with the terms and conditions stipulated in
// the agreement/contract under which the contents have been supplied.
//
// $Header: $
// $NoKeywords: $
//
// Converts from any one DMX file format to another
// Can also output SMD or a QCI header from DMX input
//
//=============================================================================



// Standard includes
#include <conio.h>
#include <stdarg.h>


// Valve includes
#include "vstdlib/cvar.h"
#include "tier1/tier1.h"
#include "tier2/tier2.h"
#include "tier2/tier2dm.h"
#include "tier3/tier3.h"
#include "filesystem.h"
#include "vstdlib/iprocessutils.h"
#include "tier0/icommandline.h"
#include "istudiorender.h"
#include "vphysics_interface.h"
#include "datacache/idatacache.h"
#include "datacache/imdlcache.h"
#include "appframework/AppFramework.h"
#include "dmserializers/idmserializers.h"
#include "datamodel/idatamodel.h"
#include "datamodel/dmelement.h"
#include "movieobjects/dmeanimationset.h"
#include "movieobjects/dmedag.h"
#include "movieobjects/dmemesh.h"
#include "movieobjects/dmevertexdata.h"
#include "movieobjects/dmeselection.h"
#include "movieobjects/dmecombinationoperator.h"
#include "movieobjects/dmobjserializer.h"
#include "movieobjects/dmmeshcomp.h"
#include "movieobjects/dmmeshutils.h"
#include "movieobjects/dmemodel.h"
#include "tier1/utlstring.h"
#include "tier1/utlbuffer.h"
#include "tier2/p4helpers.h"


// Lua includes
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>


// Local includes
#include "dmxedit.h"


//-----------------------------------------------------------------------------
// Standard spew functions
//-----------------------------------------------------------------------------
static SpewRetval_t SpewStdout( SpewType_t spewType, char const *pMsg )
{
	if ( !pMsg )
		return SPEW_CONTINUE;

	printf( pMsg );
	fflush( stdout );

	return ( spewType == SPEW_ASSERT ) ? SPEW_DEBUGGER : SPEW_CONTINUE; 
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
const CDmxEdit::CFalloffType CDmxEdit::STRAIGHT( CDmeMesh::STRAIGHT );
const CDmxEdit::CFalloffType CDmxEdit::LINEAR( CDmeMesh::STRAIGHT );
const CDmxEdit::CFalloffType CDmxEdit::BELL( CDmeMesh::BELL );
const CDmxEdit::CFalloffType CDmxEdit::SMOOTH( CDmeMesh::BELL );
const CDmxEdit::CFalloffType CDmxEdit::SPIKE( CDmeMesh::SPIKE );
const CDmxEdit::CFalloffType CDmxEdit::DOME( CDmeMesh::DOME );

const CDmxEdit::CSelectOp CDmxEdit::ADD( CDmxEdit::CSelectOp::kAdd );
const CDmxEdit::CSelectOp CDmxEdit::SUBTRACT( CDmxEdit::CSelectOp::kSubtract );
const CDmxEdit::CSelectOp CDmxEdit::TOGGLE( CDmxEdit::CSelectOp::kToggle );
const CDmxEdit::CSelectOp CDmxEdit::INTERSECT( CDmxEdit::CSelectOp::kIntersect );
const CDmxEdit::CSelectOp CDmxEdit::REPLACE( CDmxEdit::CSelectOp::kReplace );

const CDmxEdit::CSelectType CDmxEdit::ALL( CDmxEdit::CSelectType::kAll );
const CDmxEdit::CSelectType CDmxEdit::NONE( CDmxEdit::CSelectType::kNone );

const CDmxEdit::CObjType CDmxEdit::ABSOLUTE( CDmxEdit::CObjType::kAbsolute );
const CDmxEdit::CObjType CDmxEdit::RELATIVE( CDmxEdit::CObjType::kRelative );

const CDmxEdit::CDistanceType CDmxEdit::DIST_ABSOLUTE( CDmeMesh::DIST_ABSOLUTE );
const CDmxEdit::CDistanceType CDmxEdit::DIST_RELATIVE( CDmeMesh::DIST_RELATIVE );
const CDmxEdit::CDistanceType CDmxEdit::DIST_DEFAULT( CDmeMesh::DIST_DEFAULT );

const CDmxEdit::CAxisType CDmxEdit::XAXIS( CDmxEdit::CAxisType::kXAxis );
const CDmxEdit::CAxisType CDmxEdit::YAXIS( CDmxEdit::CAxisType::kYAxis );
const CDmxEdit::CAxisType CDmxEdit::ZAXIS( CDmxEdit::CAxisType::kXAxis );

const CDmxEdit::CHalfType CDmxEdit::LEFT( CDmxEdit::CHalfType::kLeft );
const CDmxEdit::CHalfType CDmxEdit::RIGHT( CDmxEdit::CHalfType::kRight );


//=============================================================================
// CDmxEdit definition
//=============================================================================


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
CDmxEdit::CDmxEdit()
: m_pRoot( NULL )
, m_pMesh( NULL )
, m_pCurrentSelection( NULL )
, m_distanceType( CDmeMesh::DIST_ABSOLUTE )
{}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
bool CDmxEdit::Load( const char *pFilename, const CObjType &loadType /* = DIST_ABSOLUTE */ )
{
	Unload();

	const int sLen = Q_strlen( pFilename );
	if ( sLen > 4 && !Q_stricmp( pFilename + sLen - 4, ".dmx" ) )
	{
		Msg( "// Loading DMX: \"%s\"\n", pFilename );
		g_pDataModel->RestoreFromFile( pFilename, NULL, NULL, &m_pRoot );

		if ( !m_pRoot )
		{
			Error( "// ERROR: DMX Load of \"%s\" Failed\n", pFilename );
			return false;
		}

		g_p4factory->AccessFile( pFilename )->Add();

		// Find the first mesh with any deltas
		// Look for model

		CDmeDag *pDag = m_pRoot->GetValueElement< CDmeDag >( "model" );
		if ( pDag )
		{
			CDmeMesh *pFirstMesh = NULL;

			CUtlStack< CDmeDag * > traverseStack;
			traverseStack.Push( pDag );

			while ( traverseStack.Count() )
			{
				traverseStack.Pop( pDag );
				if ( !pDag )
					continue;
				
				for ( int i = pDag->GetChildCount() - 1; i >= 0; --i )
				{
					traverseStack.Push( pDag->GetChild( i ) );
				}

				CDmeMesh *pMesh = CastElement< CDmeMesh >( pDag->GetShape() );
				if ( !pMesh )
					continue;

				if ( pFirstMesh == NULL )
				{
					pFirstMesh = pMesh;
				}

				if ( pMesh->DeltaStateCount() )
				{
					m_pMesh = pMesh;
					break;
				}
			}

			if ( !m_pMesh )
			{
				if ( pFirstMesh )
				{
					m_pMesh = pFirstMesh;
					Msg( "// Warning: Couldn't Find Any DmeMesh With Delta States in %s, Using First Mesh %s\n", pFilename, m_pMesh->GetName() );
				}
				else
				{
					Msg( "// Error: Couldn't Find Any DmeMeshes %s\n", pFilename );
				}
			}
		}
		else
		{
			Msg( "// ERROR: Couldn't Find Any DmeModel as an element of Dme Root object: %s in %s\n", m_pRoot->GetName(), pFilename );
		}

		Msg( "// DMX Loaded: \"%s\"\n", pFilename );
	}
	else
	{
		Msg( "// Loading OBJ: \"%s\"\n", pFilename );

		m_pRoot = CDmObjSerializer().ReadOBJ( pFilename, &m_pMesh, true, loadType() == CObjType::kAbsolute );

		if ( !m_pRoot )
		{
			Error( "// ERROR: OBJ Load of \"%s\" Failed\n", pFilename );
			return false;
		}

		Msg( "// OBJ Loaded: \"%s\"\n", pFilename );
	}

	m_filename = pFilename;

	if ( m_pMesh )
	{
		CDmeVertexData *pBind( m_pMesh->FindBaseState( "bind" ) );
		if ( pBind )
		{
			// Ensure "work" isn't saved!
			CDmeVertexData *pWork = m_pMesh->FindOrCreateBaseState( "__dmxEdit_work" );

			if ( pWork )
			{
				pBind->CopyTo( pWork );
				m_pMesh->SetCurrentBaseState( "__dmxEdit_work" );
			}
		}

		m_pCurrentSelection = CreateElement< CDmeSingleIndexedComponent >( "selection", m_pRoot->GetFileId() );
		m_pMesh->SetValue( "selection", m_pCurrentSelection );
	}

	return true;
}


//-----------------------------------------------------------------------------
// Imports the Dmx Model file into the current file
//-----------------------------------------------------------------------------
bool CDmxEdit::Import( const char *pFilename, const char *pParentName )
{
	if ( !m_pRoot )
	{
		return Load( pFilename );
	}

	CDmeModel *pDstModel = m_pRoot->GetValueElement< CDmeModel >( "model" );
	if ( !pDstModel )
	{
		Msg( "// ERROR: Import( \"%s\" ); - Failed: Can't find existing model node\n", pFilename );
		return false;
	}

	int nSkinningJointIndex = -1;

	if ( pParentName )
	{
		CDmeTransform *pTransform = NULL;

		const int nJointTransformCount = pDstModel->GetJointTransformCount();
		for ( int i = 0; i < nJointTransformCount; ++i )
		{
			pTransform = pDstModel->GetJointTransform( i );
			if ( !Q_stricmp( pTransform->GetName(), pParentName ) )
			{
				nSkinningJointIndex = i;
				break;
			}
		}

		if ( nSkinningJointIndex < 0 )
		{
			Warning( "// WARNING: Import( \"%s\" ); - Couldn't Find parent bone \"%s\"\n", pFilename, pParentName );
		}
	}

	Msg( "// Import( \"%s\" );\n", pFilename );

	CDisableUndoScopeGuard sgDisableUndo;

	CDmElement *pRoot = NULL;
	g_pDataModel->RestoreFromFile( pFilename, NULL, NULL, &pRoot, CR_FORCE_COPY );

	if ( !pRoot )
	{
		Msg( "// ERROR: Import( \"%s\" ); - Failed: Couldn't Load Dmx File\n", pFilename );
		return false;
	}

	g_p4factory->AccessFile( pFilename )->Add();

	CDmeDag *pSrcModel = pRoot->GetValueElement< CDmeDag >( "model" );
	if ( !pSrcModel )
	{
		Msg( "// ERROR: Import( \"%s\" ); - Failed: Couldn't Find \"model\" element on root\n", pFilename );
		g_pDataModel->UnloadFile( pRoot->GetFileId() );
		return false;
	}

//	CDmeCombinationOperator *pCombo = m_pRoot->GetValueElement< CDmeCombinationOperator >( "combinationOperator" );

	// Initialize traversal stack to just model's children (don't want to touch model)
	CUtlStack< CDmeDag * > traverseStack;
	for ( int i = pSrcModel->GetChildCount() - 1; i >= 0; --i )
	{
		traverseStack.Push( pSrcModel->GetChild( i ) );
	}

	CDmeDag *pDag;
	while ( traverseStack.Count() )
	{
		traverseStack.Pop( pDag );
		if ( !pDag )
			continue;

		for ( int i = pDag->GetChildCount() - 1; i >= 0; --i )
		{
			traverseStack.Push( pDag->GetChild( i ) );
		}

		CDmeMesh *pMesh = CastElement< CDmeMesh >( pDag->GetShape() );
		if ( pMesh )
		{
			CDmMeshUtils::Merge( pMesh, m_pMesh, nSkinningJointIndex );
		}
	}

	g_pDataModel->UnloadFile( pRoot->GetFileId() );

	return false;
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void CDmxEdit::ListDeltas()
{
	if ( !m_pMesh )
	{
		Error( "// ERROR: No base mesh is currently defined\n" );
		return;
	}

	const int nDeltas( m_pMesh->DeltaStateCount() );
	if ( nDeltas <= 0 )
	{
		Error( "// ERROR: No deltas defined for mesh: %s\n", m_pMesh->GetName() );
		return;
	}

	for ( int i( 0 ); i < nDeltas; ++i )
	{
		Msg( "// Delta %d: %s\n", i, m_pMesh->GetDeltaState( i )->GetName() );
	}
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void CDmxEdit::Unload()
{
	CDisableUndoScopeGuard sgDisableUndo;

	if ( m_pCurrentSelection )
	{
		g_pDataModel->DestroyElement( m_pCurrentSelection->GetHandle() );
	}

	if ( m_pRoot )
	{
		g_pDataModel->UnloadFile( m_pRoot->GetFileId() );
	}

	m_filename = "";
	m_pRoot = NULL;
	m_pMesh = NULL;
	m_pCurrentSelection = NULL;
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
bool CDmxEdit::ImportComboRules( const char *pFilename, bool bOverwrite /* = true */, bool bPurgeDeltas /* = false */ )
{
	if ( !m_pMesh )
	{
		Error( "// ERROR: ImportComboRules( \"%s\" ) Failed - No Mesh\n", pFilename );
		return false;
	}

	CDmeCombinationOperator *pDestCombo( FindReferringElement< CDmeCombinationOperator >( m_pMesh, "targets" ) );
	if ( !pDestCombo )
	{
		Error( "// ERROR: ImportComboRules( \"%s\" ) Failed - No Combination Operator On Mesh \"%s\"\n", pFilename, m_pMesh->GetName() );
		return false;
	}

	CDisableUndoScopeGuard sg;

	CDmElement *pRoot = NULL;
	g_pDataModel->RestoreFromFile( pFilename, NULL, NULL, &pRoot, CR_FORCE_COPY );

	if ( !pRoot )
	{
		Error( "// ERROR: ImportComboRules( \"%s\" ) Failed - File Cannot Be Read\n", pFilename );
		return false;
	}

	g_p4factory->AccessFile( pFilename )->Add();

	// Try to find a combination system in the file
	CDmeCombinationOperator *pCombo = CastElement< CDmeCombinationOperator >( pRoot );
	if ( !pCombo )
	{
		pCombo = pRoot->GetValueElement< CDmeCombinationOperator >( "combinationOperator" );
	}

	if ( !pCombo )
	{
		Error( "// ERROR: ImportComboRules( \"%s\" ) Failed - No Combination Operator Found In File\n", pFilename );
		return false;
	}

	ImportCombinationControls( pCombo, pDestCombo, bOverwrite );
	ImportDominationRules( pDestCombo, pCombo, bOverwrite );

	pDestCombo->SetToDefault();

	g_pDataModel->UnloadFile( pRoot->GetFileId() );

	if ( bPurgeDeltas )
	{
		CDmMeshUtils::PurgeUnusedDeltas( m_pMesh );
	}

	Msg( "// ImportComboRules( \"%s\" );\n", pFilename );

	return true;
}


//-----------------------------------------------------------------------------
// Import dominance rules from this operator
//-----------------------------------------------------------------------------
void CDmxEdit::ImportDominationRules( CDmeCombinationOperator *pDestComboOp, CDmeCombinationOperator *pSrcComboOp, bool bOverwrite )
{
	if ( bOverwrite )
	{
		pDestComboOp->RemoveAllDominationRules();
	}

	// TODO: Detect conflicts if not overwriting

	// Now deal with dominance rules
	int nRuleCount = pSrcComboOp->DominationRuleCount();
	for ( int i = 0; i < nRuleCount; ++i )
	{
		bool bMismatch = false;

		// Only add dominance rule if *all* raw controls are present and the rule doesn't already exist
		CDmeCombinationDominationRule *pSrcRule = pSrcComboOp->GetDominationRule( i );
		int nDominatorCount = pSrcRule->DominatorCount();
		for ( int j = 0; j < nDominatorCount; ++j )
		{
			const char *pDominatorName = pSrcRule->GetDominator( j );
			if ( !pDestComboOp->HasRawControl( pDominatorName ) )
			{
				bMismatch = true;
				break;
			}
		}

		int nSuppressedCount = pSrcRule->SuppressedCount();
		for ( int j = 0; j < nSuppressedCount; ++j )
		{
			const char *pSuppressedName = pSrcRule->GetSuppressed( j );
			if ( !pDestComboOp->HasRawControl( pSuppressedName ) )
			{
				bMismatch = true;
				break;
			}
		}

		if ( bMismatch )
			continue;

		pDestComboOp->AddDominationRule( pSrcRule );
	}
}


//-----------------------------------------------------------------------------
// Returns true if the the control has only 1 raw control and it's the same
// name as the control
//-----------------------------------------------------------------------------
bool IsDefaultControl( CDmeCombinationOperator *pCombo, int nIndex )
{
	if ( pCombo->GetRawControlCount( nIndex ) == 1 && !Q_strcmp( pCombo->GetControlName( nIndex ), pCombo->GetRawControlName( nIndex, 0 ) ) )
		return true;

	return false;
}


//-----------------------------------------------------------------------------
// Returns true if the all raw controls of A are found in B
//-----------------------------------------------------------------------------
bool IsASubsetOfB( CDmeCombinationOperator *pComboA, int nIndexA, CDmeCombinationOperator *pComboB, int nIndexB )
{
	const int nCountA = pComboA->GetRawControlCount( nIndexA );
	const int nCountB = pComboB->GetRawControlCount( nIndexB );

	for ( int i = 0; i < nCountA; ++i )
	{
		bool bFound = false;

		const char *pNameA = pComboA->GetRawControlName( nIndexA, i );

		for ( int j = 0; j < nCountB; ++j )
		{
			if ( !Q_stricmp( pNameA, pComboB->GetRawControlName( nIndexB, j ) ) )
			{
				bFound = true;
				break;
			}
		}

		if ( !bFound )
			return false;
	}

	return true;
}

//-----------------------------------------------------------------------------
// Returns true if the two combination controls have the same raw controls
//-----------------------------------------------------------------------------
bool AreControlsTheSame( CDmeCombinationOperator *pComboA, int nIndexA, CDmeCombinationOperator *pComboB, int nIndexB )
{
	const int nCountA = pComboA->GetRawControlCount( nIndexA );
	const int nCountB = pComboB->GetRawControlCount( nIndexB );

	if ( nCountA != nCountB )
		return false;

	// This is a test for equality now since we've verified that A & B has same number of elements
	return IsASubsetOfB( pComboA, nIndexA, pComboB, nIndexB );
}


//-----------------------------------------------------------------------------
// -1 for failure
//-----------------------------------------------------------------------------
int FindControlIndexFromRawControlName( CDmeCombinationOperator *pCombo, const char *pRawControlName )
{
	const int nControlCount = pCombo->GetControlCount();
	for ( int i = 0; i < nControlCount; ++i )
	{
		const int nRawCount = pCombo->GetRawControlCount( i );
		for ( int j = 0; j < nRawCount; ++j )
		{
			if ( !Q_stricmp( pRawControlName, pCombo->GetRawControlName( i, j ) ) )
			{
				return i;
			}
		}
	}

	return -1;
}


//-----------------------------------------------------------------------------
// Import combination rules from this operator
//-----------------------------------------------------------------------------
void CDmxEdit::ImportCombinationControls( CDmeCombinationOperator *pSrcComboOp, CDmeCombinationOperator *pDstComboOp, bool bOverwrite )
{
	if ( bOverwrite )
	{
		// Just remove all existing controls
		pDstComboOp->RemoveAllControls();
	}
	else
	{
		// Remove conflicting controls, i.e. controls in src which are controlling something that's already
		// controlled in dst

		bool bConflict = true;

		while ( bConflict )
		{
			bConflict = false;

			// Remove all controls from dst that conflict with src

			for ( int i = 0; !bConflict && i < pSrcComboOp->GetControlCount(); ++i )
			{
				const int nSrcRawCount = pSrcComboOp->GetRawControlCount( i );
				for ( int j = 0; !bConflict && j < nSrcRawCount; ++j )
				{
					const char *pSrcRawControlName = pSrcComboOp->GetRawControlName( i, j );

					const int nDstControlIndex = FindControlIndexFromRawControlName( pDstComboOp, pSrcRawControlName );
					if ( nDstControlIndex < 0 )
						continue;

					pDstComboOp->RemoveRawControl( nDstControlIndex, pSrcRawControlName );
					bConflict = true;
					break;
				}
			}
		}

		// Purge all empty ones

		bConflict = true;

		while ( bConflict )
		{
			bConflict = false;

			for ( int i = 0; !bConflict && i < pDstComboOp->GetControlCount(); ++i )
			{
				if ( pDstComboOp->GetRawControlCount( i ) == 0 )
				{
					pDstComboOp->RemoveControl( pDstComboOp->GetControlName( i ) );
					bConflict = true;
					break;
				}
			}
		}
	}

	// Iterate through all controls in the imported operator.
	// For each imported control that contains at least 1 raw control
	// for which a delta state exists, create that control
	// All conflicts were resolved previously

	for ( int i = 0; i < pSrcComboOp->GetControlCount(); ++i )
	{
		const char *pControlName = pSrcComboOp->GetControlName( i );

		int nRawControls = pSrcComboOp->GetRawControlCount( i );
		int nMatchCount = 0; 
		bool *pFoundMatch = (bool*)_alloca( nRawControls * sizeof(bool) );
		for ( int j = 0; j < nRawControls; ++j )
		{
			const char *pRawControl = pSrcComboOp->GetRawControlName( i, j );
			pFoundMatch[j] = pDstComboOp->DoesTargetContainDeltaState( pRawControl );
			nMatchCount += pFoundMatch[j];
		}

		// No match? Don't import
		if ( nMatchCount == 0 )
		{
			continue;
		}

		//		bool bPartialMatch = ( nMatchCount != nRawControls );

		// Found a match! Let's create the control and potentially raw control
		pDstComboOp->RemoveControl( pControlName );
		bool bIsStereo = pSrcComboOp->IsStereoControl( i );
		ControlIndex_t index = pDstComboOp->FindOrCreateControl( pControlName, bIsStereo );
		pDstComboOp->SetEyelidControl( index, pSrcComboOp->IsEyelidControl( i ) );
		for ( int j = 0; j < nRawControls; ++j )
		{
			if ( pFoundMatch[j] )
			{
				const char *pRawControl = pSrcComboOp->GetRawControlName( i, j );
				float flWrinkleScale = pSrcComboOp->GetRawControlWrinkleScale( i, j );

				pDstComboOp->AddRawControl( index, pRawControl );
				pDstComboOp->SetWrinkleScale( index, pRawControl, flWrinkleScale );
			}
		}
	}
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
bool CDmxEdit::SetState( const char *pDeltaName )
{
	if ( !m_pMesh )
	{
		Error( "// ERROR: SetState( \"%s\" ); Failed - No Mesh\n", pDeltaName );
		return false;
	}

	CDmeVertexDeltaData *pDelta = FindDeltaState( pDeltaName );
	if ( !pDelta )
	{
		Error( "// ERROR: SetState( \"%s\" ); Failed - Invalid Delta\n", pDeltaName );
		return false;
	}

	Msg( "// SetState( \"%s\" );\n", pDelta->GetName() );

	return m_pMesh->SetBaseStateToDelta( pDelta );
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
bool CDmxEdit::ResetState()
{
	if ( !m_pMesh )
	{
		Error( "// ERROR: ResetState() Failed - No Mesh\n" );
		return false;
	}

	Msg( "// ResetState();\n" );

	return m_pMesh->SetBaseStateToDelta( NULL );
}


//-----------------------------------------------------------------------------
// Disambiguation when select is called with a selection type of a string
//-----------------------------------------------------------------------------
bool CDmxEdit::Select( const char *pSelectTypeString, CDmeSingleIndexedComponent *pPassedSelection /* = NULL */, CDmeMesh *pPassedMesh /* = NULL */ )
{
	const CSelectType selectType( pSelectTypeString );

	if ( selectType() == CSelectType::kDelta )
	{
		CDmeVertexDeltaData *pDelta = FindDeltaState( pSelectTypeString, pPassedMesh );
		if ( pDelta)
			return Select( pDelta, pPassedSelection, pPassedMesh );

		Error( "// ERROR: Select( \"%s\" ) Failed - Invalid Delta\n", pSelectTypeString );
		return false;
	}

	return Select( selectType, pPassedSelection, pPassedMesh );
}


//-----------------------------------------------------------------------------
// Selects vertices of the working mesh that change position in the give delta
//-----------------------------------------------------------------------------
bool CDmxEdit::Select( CDmeVertexDeltaData *pDelta, CDmeSingleIndexedComponent *pPassedSelection /* = NULL */, CDmeMesh *pPassedMesh /* = NULL */ )
{
	if ( !pDelta )
	{
		Error( "// ERROR: Select() Failed - Invalid Delta\n" );
		return false;
	}

	CDmeMesh *pMesh = pPassedMesh ? pPassedMesh : m_pMesh;

	if ( !pMesh )
	{
		Error( "// ERROR: Select( \"%s\" ) Failed - No Mesh\n", pDelta->GetName() );
		return false;
	}

	CDmeSingleIndexedComponent *pSelection = pPassedSelection ? pPassedSelection : m_pCurrentSelection;

	if ( !pSelection )
	{
		Error( "// ERROR: Select( \"%s\" ) Failed - No Selection To Manipulate\n", pDelta->GetName() );
		return false;
	}

	Msg( "// Select( \"%s\" );\n", pDelta->GetName() );

	pMesh->SelectVerticesFromDelta( pDelta, pSelection );

	return true;
}


//-----------------------------------------------------------------------------
// Selects vertices of the working mesh that change position in the give delta
//-----------------------------------------------------------------------------
bool CDmxEdit::Select( const CSelectType &selectType, CDmeSingleIndexedComponent *pPassedSelection /* = NULL */, CDmeMesh *pPassedMesh /* = NULL */ )
{
	if ( selectType() == CSelectType::kDelta )
	{
		Error( "// ERROR: Select( \"%s\" ) called via CSelectType... Wacky!\n", static_cast< char * >( selectType ) );
		return false;
	}

	CDmeSingleIndexedComponent *pSelection = pPassedSelection ? pPassedSelection : m_pCurrentSelection;

	if ( !pSelection )
	{
		Error( "// ERROR: Select( \"%s\" ) Failed - No Selection To Manipulate\n", static_cast< char * >( selectType ) );
		return false;
	}

	if ( selectType() == CSelectType::kNone )
	{
		Msg( "// Select( \"%s\" );\n", static_cast< char * >( selectType ) );
		m_pCurrentSelection->Clear();
		return true;
	}

	if ( selectType() != CSelectType::kAll )
		return false;

	CDmeMesh *pMesh = pPassedMesh ? pPassedMesh : m_pMesh;

	if ( !pMesh )
	{
		Error( "// ERROR: Select( \"%s\" ) Failed - No Mesh\n", static_cast< char * >( selectType ) );
		return false;
	}

	Msg( "// Select( \"%s\" );\n", static_cast< char * >( selectType ) );
	pMesh->SelectAllVertices( pSelection );

	return true;
}


//-----------------------------------------------------------------------------
// Disambiguation when select is called with a selection type of a string
//-----------------------------------------------------------------------------
bool CDmxEdit::Select( const CSelectOp &selectOp, const char *pSelectTypeString,
					  CDmeSingleIndexedComponent *pPassedSelection /* = NULL */, CDmeMesh *pPassedMesh /* = NULL */ )
{
	const CSelectType selectType( pSelectTypeString );
	if ( selectType() == CSelectType::kDelta )
	{
		CDmeVertexDeltaData *pDelta = FindDeltaState( pSelectTypeString, pPassedMesh );
		if ( pDelta)
			return Select( selectOp, pDelta, pPassedSelection, pPassedMesh );

		Error( "// ERROR: Select( \"%s\", \"%s\" ); Failed - Invalid Delta\n", selectOp, pSelectTypeString );
		return false;
	}

	return Select( selectOp, selectType, pPassedSelection, pPassedMesh );
}


//-----------------------------------------------------------------------------
// Selects vertices of the working mesh that change position in the give delta
//-----------------------------------------------------------------------------
bool CDmxEdit::Select( const CSelectOp &selectOp, CDmeVertexDeltaData *pDelta, CDmeSingleIndexedComponent *pPassedSelection /* = NULL */, CDmeMesh *pPassedMesh /* = NULL */ )
{
	if ( !pDelta )
	{
		Error( "// ERROR: Select( \"%s\" ); Failed - Invalid Delta\n", selectOp );
		return false;
	}

	CDmeSingleIndexedComponent *pSelection = pPassedSelection ? pPassedSelection : m_pCurrentSelection;

	if ( !pSelection )
	{
		Error( "// ERROR: Select( \"%s\", \"%s\" ) Failed - No Selection To Manipulate\n", static_cast< char * >( selectOp ), pDelta->GetName() );
		return false;
	}

	CDmeSingleIndexedComponent *pTempSelection = CreateElement< CDmeSingleIndexedComponent >( "tempSelection", pSelection->GetFileId() );
	if ( !pTempSelection )
		return false;

	const bool retVal = Select( pDelta, pTempSelection, pPassedMesh );

	if ( retVal )
	{
		Select( selectOp, pSelection, pTempSelection );
	}

	g_pDataModel->DestroyElement( pTempSelection->GetHandle() );

	return retVal;
}


//-----------------------------------------------------------------------------
// Selects vertices of the working mesh that change position in the give delta
//-----------------------------------------------------------------------------
bool CDmxEdit::Select( const CSelectOp &selectOp, const CSelectType &selectType, CDmeSingleIndexedComponent *pPassedSelection /* = NULL */, CDmeMesh *pPassedMesh /* = NULL */ )
{
	CDmeSingleIndexedComponent *pSelection = pPassedSelection ? pPassedSelection : m_pCurrentSelection;

	if ( !pSelection )
	{
		Error( "// ERROR: Select( \"%s\", \"%s\" ) Failed - No Selection To Manipulate\n", static_cast< char * >( selectOp ), static_cast< char * >( selectType ) );
		return false;
	}

	CDmeSingleIndexedComponent *pTempSelection = CreateElement< CDmeSingleIndexedComponent >( "tempSelection", pSelection->GetFileId() );
	if ( !pTempSelection )
		return false;

	const bool retVal = Select( selectType, pTempSelection, pPassedMesh );

	if ( retVal )
	{
		Select( selectOp, pSelection, pTempSelection );
	}

	g_pDataModel->DestroyElement( pTempSelection->GetHandle() );

	return retVal;
}


//-----------------------------------------------------------------------------
// Combines the two selections via the selectOp and puts result into pOriginal
//-----------------------------------------------------------------------------
bool CDmxEdit::Select( const CSelectOp &selectOp, CDmeSingleIndexedComponent *pOriginal, const CDmeSingleIndexedComponent *pNew )
{
	if ( !pOriginal || !pNew )
		return false;

	switch ( selectOp() )
	{
	case CSelectOp::kAdd:
		pOriginal->Add( pNew );
		break;
	case CSelectOp::kSubtract:
		pOriginal->Subtract( pNew );
		break;
	case CSelectOp::kToggle:
		{
			CDmeSingleIndexedComponent *pIntersection = CreateElement< CDmeSingleIndexedComponent >( "intersection", pOriginal->GetFileId() );
			if ( !pIntersection )
				return false;

			CDmeSingleIndexedComponent *pNewCopy = CreateElement< CDmeSingleIndexedComponent >( "newCopy", pOriginal->GetFileId() );
			if ( !pNewCopy )
				return false;

			pOriginal->CopyAttributesTo( pIntersection );
			pIntersection->Intersection( pNew );
			pOriginal->Subtract( pIntersection );

			pNew->CopyAttributesTo( pNewCopy );
			pNewCopy->Subtract( pIntersection );
			pOriginal->Add( pNewCopy );
		}
		break;
	case CSelectOp::kIntersect:
		pOriginal->Intersection( pNew );
		break;
	case CSelectOp::kReplace:
		{
			CUtlString originalName = pOriginal->GetName();
			pNew->CopyAttributesTo( pOriginal );
			pOriginal->SetName( originalName );
		}
		break;
	}

	return true;
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
bool CDmxEdit::SelectHalf( const CSelectOp &selectOp, const CHalfType &halfType, CDmeSingleIndexedComponent *pPassedSelection /* = NULL */, CDmeMesh *pPassedMesh /* = NULL */ )
{
	CDmeSingleIndexedComponent *pSelection = pPassedSelection ? pPassedSelection : m_pCurrentSelection;

	if ( !pSelection )
	{
		Error( "// ERROR: SelectHalf( \"%s\", \"%s\" ) Failed - No Selection To Manipulate\n", static_cast< char * >( selectOp ), static_cast< char * >( halfType ) );
		return false;
	}

	CDmeSingleIndexedComponent *pTempSelection = CreateElement< CDmeSingleIndexedComponent >( "tempSelection", pSelection->GetFileId() );
	if ( !pTempSelection )
		return false;

	const bool retVal = SelectHalf( halfType, pTempSelection, pPassedMesh );

	if ( retVal )
	{
		Select( selectOp, pSelection, pTempSelection );
	}

	g_pDataModel->DestroyElement( pTempSelection->GetHandle() );

	return retVal;
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
bool CDmxEdit::SelectHalf( const CHalfType &halfType, CDmeSingleIndexedComponent *pPassedSelection /* = NULL */, CDmeMesh *pPassedMesh /* = NULL */ )
{
	CDmeSingleIndexedComponent *pSelection = pPassedSelection ? pPassedSelection : m_pCurrentSelection;

	if ( !pSelection )
	{
		Error( "// ERROR: SelectHalf( \"%s\" ) Failed - No Selection To Manipulate\n", static_cast< char * >( halfType ) );
		return false;
	}

	CDmeMesh *pMesh = pPassedMesh ? pPassedMesh : m_pMesh;

	if ( !pMesh )
	{
		Error( "// ERROR: SelectHalf( \"%s\" ) Failed - No Mesh\n", static_cast< char * >( halfType ) );
		return false;
	}

	Msg( "// SelectHalf( \"%s\" );\n", static_cast< char * >( halfType ) );

	pMesh->SelectHalfVertices( halfType() == CHalfType::kLeft ? CDmeMesh::kLeft : CDmeMesh::kRight, pSelection );

	return true;
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
bool CDmxEdit::GrowSelection( int nSize /* = 1 */ )
{
	if ( !m_pMesh )
	{
		Error( "// ERROR: GrowSelection( %d ) Failed - No Mesh\n", nSize );
		return false;
	}

	Msg( "// GrowSelection( %d );\n", nSize );

	m_pMesh->GrowSelection( nSize, m_pCurrentSelection, NULL );

	return true;
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
bool CDmxEdit::ShrinkSelection( int nSize /* = 1 */ )
{
	if ( !m_pMesh )
	{
		Error( "// ERROR: ShrinkSelection( %d ) Failed - No Mesh\n", nSize );
		return false;
	}

	Msg( "// ShrinkSelection( %d );\n", nSize );

	m_pMesh->ShrinkSelection( nSize, m_pCurrentSelection, NULL );

	return true;
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
bool CDmxEdit::Add( const CDmxEditProxy &e, float weight /* = 1.0f */, float featherDistance /* = 0.0f */, const CFalloffType &falloffType, const CDistanceType &passedDistanceType )
{
	if ( !m_pMesh )
	{
		Error( "// ERROR: Add( %f ); Failed - No Mesh\n", weight );
		return false;
	}

	if ( m_pCurrentSelection->Count() == 0 )
	{
		Error( "// ERROR: No Vertices selected for Add( %f );  Selecting All Vertices!\n", weight );
		Select( ALL );
	}

	const CDistanceType &distanceType = passedDistanceType() == CDmeMesh::DIST_DEFAULT ? m_distanceType : passedDistanceType;

	CDmeSingleIndexedComponent *pNewSelection = featherDistance > 0.0f ? m_pMesh->FeatherSelection( featherDistance, falloffType(), distanceType(), m_pCurrentSelection, NULL ) : NULL;

	Msg( "// Add( \"base\", %f, %f );\n", weight, featherDistance );
	const bool retVal = m_pMesh->AddMaskedDelta( NULL, NULL, weight, pNewSelection ? pNewSelection : m_pCurrentSelection );

	if ( pNewSelection )
	{
		g_pDataModel->DestroyElement( pNewSelection->GetHandle() );
	}

	return retVal;
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
bool CDmxEdit::Add( const char *pDeltaName, float weight /* = 1.0f */, float featherDistance /* = 0.0f */, const CFalloffType &falloffType, const CDistanceType &passedDistanceType )
{
	if ( !Q_stricmp( "BASE", pDeltaName ) )
		return Add( CDmxEditProxy( *this ), weight, featherDistance, falloffType );

	if ( !m_pMesh )
	{
		Error( "// ERROR: Add( %f ); Failed - No Mesh\n", weight );
		return false;
	}

	if ( m_pCurrentSelection->Count() == 0 )
	{
		Error( "// ERROR: No Vertices selected for Add( %f );  Selecting All Vertices!\n", weight );
		Select( ALL );
	}

	const CDistanceType &distanceType = passedDistanceType() == CDmeMesh::DIST_DEFAULT ? m_distanceType : passedDistanceType;

	CDmeSingleIndexedComponent *pNewSelection = featherDistance > 0.0f ? m_pMesh->FeatherSelection( featherDistance, falloffType(), distanceType(), m_pCurrentSelection, NULL ) : NULL;

	CDmeVertexDeltaData *pDelta = FindDeltaState( pDeltaName );
	if ( !pDelta )
	{
		Error( "// ERROR: Add( \"%s\", %f, %f ); Failed - Invalid Delta\n", pDeltaName, weight, featherDistance );
		return false;
	}

	Msg( "// Add( \"%s\", %f, %f );\n", pDelta->GetName(), weight, featherDistance );
	const bool retVal = m_pMesh->AddMaskedDelta( pDelta, NULL, weight, pNewSelection );

	if ( pNewSelection )
	{
		g_pDataModel->DestroyElement( pNewSelection->GetHandle() );
	}

	return retVal;
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
bool CDmxEdit::Interp( const CDmxEditProxy &e, float weight /* = 1.0f */, float featherDistance /* = 0.0f */, const CFalloffType &falloffType, const CDistanceType &passedDistanceType )
{
	if ( !m_pMesh )
	{
		Error( "// ERROR: Interp( %f ); Failed - No Mesh\n", weight );
		return false;
	}

	if ( m_pCurrentSelection->Count() == 0 )
	{
		Error( "// ERROR: No Vertices selected for Interp( %f );  Selecting All Vertices!\n", weight );
		Select( ALL );
	}

	const CDistanceType &distanceType = passedDistanceType() == CDmeMesh::DIST_DEFAULT ? m_distanceType : passedDistanceType;

	CDmeSingleIndexedComponent *pNewSelection = featherDistance > 0.0f ? m_pMesh->FeatherSelection( featherDistance, falloffType(), distanceType(), m_pCurrentSelection, NULL ) : NULL;

	Msg( "// Interp( \"base\", %f, %f );\n", weight, featherDistance );
	const bool retVal = m_pMesh->InterpMaskedDelta( NULL, NULL, weight, pNewSelection ? pNewSelection : m_pCurrentSelection );

	if ( pNewSelection )
	{
		g_pDataModel->DestroyElement( pNewSelection->GetHandle() );
	}

	return retVal;
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
bool CDmxEdit::Interp( const char *pDeltaName, float weight /* = 1.0f */, float featherDistance /* = 0.0f */, const CFalloffType &falloffType, const CDistanceType &passedDistanceType )
{
	if ( !Q_stricmp( "BASE", pDeltaName ) )
		return Interp( CDmxEditProxy( *this ), weight, featherDistance, falloffType );

	if ( !m_pMesh )
	{
		Error( "// ERROR: Interp( %f ); Failed - No Mesh\n", weight );
		return false;
	}

	if ( m_pCurrentSelection->Count() == 0 )
	{
		Error( "// ERROR: No Vertices selected for Interp( %f );  Selecting All Vertices!\n", weight );
		Select( ALL );
	}

	const CDistanceType &distanceType = passedDistanceType() == CDmeMesh::DIST_DEFAULT ? m_distanceType : passedDistanceType;

	CDmeSingleIndexedComponent *pNewSelection = featherDistance > 0.0f ? m_pMesh->FeatherSelection( featherDistance, falloffType(), distanceType(), m_pCurrentSelection, NULL ) : NULL;

	CDmeVertexDeltaData *pDelta = FindDeltaState( pDeltaName );
	if ( !pDelta )
	{
		Error( "// ERROR: Interp( \"%s\", %f, %f ); Failed - Invalid Delta\n", pDeltaName, weight, featherDistance );
		return false;
	}

	Msg( "// Interp( \"%s\", %f, %f );\n", pDelta->GetName(), weight, featherDistance );
	const bool retVal = m_pMesh->InterpMaskedDelta( pDelta, NULL, weight, pNewSelection ? pNewSelection : m_pCurrentSelection );

	if ( pNewSelection )
	{
		g_pDataModel->DestroyElement( pNewSelection->GetHandle() );
	}

	return retVal;
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
bool CDmxEdit::Save( const char *pFilename, const CObjType &saveType /* = ABSOLUTE */, const char *pDeltaName /* = NULL */ )
{
	if ( !pFilename )
	{
		Error( "// ERROR: Save() Failed - No filename\n" );
		return false;
	}

	if ( !m_pRoot )
	{
		Error( "// ERROR: Save() Failed - No dmx root object\n" );
		return false;
	}

	if ( m_pMesh )
	{
		// Cleanup
		m_pMesh->RemoveAttribute( "selection" );
		if ( m_pCurrentSelection )
		{
			g_pDataModel->DestroyElement( m_pCurrentSelection->GetHandle() );
			m_pCurrentSelection = NULL;
		}

		CDmeVertexData *pBind( m_pMesh->FindBaseState( "bind" ) );

		if ( pBind )
		{
			m_pMesh->SetCurrentBaseState( "bind" );
		}

		CDmeVertexData *pWork( m_pMesh->FindBaseState( "__dmxEdit_work" ) );

		if ( pWork )
		{
			m_pMesh->DeleteBaseState( "__dmxEdit_work" );
		}
	}

	bool retVal = false;

	const int sLen = Q_strlen( pFilename );
	if ( sLen > 4 && !Q_stricmp( pFilename + sLen - 4, ".dmx" ) )
	{
		Msg( "// DMX Save( \"%s\" );\n", pFilename );
		retVal = g_p4factory->AccessFile( pFilename )->Edit();
		if ( !retVal )
		{
			retVal = g_p4factory->AccessFile( pFilename )->Add();
		}

		retVal = g_pDataModel->SaveToFile( pFilename, NULL, "keyvalues2", "model", m_pRoot );
		if ( !retVal )
		{
			Error( "// DMX Save( \"%s\" ); - Failed, Cannot Write File\n", pFilename );
		}
	}
	else
	{
		bool absolute = true;
		if ( saveType() == CObjType::kRelative )
		{
			// Relative
			absolute = false;
			Msg( "// OBJ Save( \"%s\", \"relative\" );\n", pFilename );
		}
		else
		{
			Msg( "// OBJ Save( \"%s\", \"absolute\" );\n", pFilename );
		}

		if ( pDeltaName )
		{
			if ( !Q_stricmp( "base", pDeltaName ) )
			{
				retVal = CDmObjSerializer().WriteOBJ( pFilename, m_pRoot, false, NULL, absolute );
			}
			else
			{
				retVal = CDmObjSerializer().WriteOBJ( pFilename, m_pRoot, true, pDeltaName, absolute );
			}
		}
		else
		{
			retVal = CDmObjSerializer().WriteOBJ( pFilename, m_pRoot, true, NULL, absolute );
		}
	}

	if ( m_pMesh )
	{
		// Undo the cleanup work performed above
		CDmeVertexData *pBind = m_pMesh->FindBaseState( "bind" );

		if ( pBind )
		{
			// Ensure "work" isn't saved!
			CDmeVertexData *pWork = m_pMesh->FindOrCreateBaseState( "__dmxEdit_work" );

			if ( pWork )
			{
				pBind->CopyTo( pWork );
				m_pMesh->SetCurrentBaseState( "__dmxEdit_work" );
			}
		}

		m_pCurrentSelection = CreateElement< CDmeSingleIndexedComponent >( "selection", m_pRoot->GetFileId() );
		m_pMesh->SetValue( "selection", m_pCurrentSelection );
	}

	return retVal;
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
bool CDmxEdit::Merge( const char *pInFilename, const char *pOutFilename )
{
	CDmElement *pRoot = NULL;

	CDisableUndoScopeGuard guard0;
	g_pDataModel->RestoreFromFile( pInFilename, NULL, NULL, &pRoot );
	guard0.Release();

	if ( !pRoot )
	{
		Error( "// ERROR: Merge( \"%s\", \"%s\" ); Failed - \"%s\" Unchanged.  Couldn't Load \"%s\".\n", pInFilename, pOutFilename, pOutFilename, pInFilename );
		return false;
	}

	g_p4factory->AccessFile( pInFilename )->Add();

	const bool retVal = CDmMeshUtils::Merge( m_pMesh, pRoot );

	CDisableUndoScopeGuard guard1;
	if ( retVal )
	{
		Msg( "// Merge( \"%s\", \"%s\" );\n", pInFilename, pOutFilename );
		bool bPerforce = g_p4factory->AccessFile( pOutFilename )->Edit();
		if ( !bPerforce )
		{
			bPerforce = g_p4factory->AccessFile( pOutFilename )->Add();
		}

		if ( !g_pDataModel->SaveToFile( pOutFilename, NULL, "keyvalues2", "model", pRoot ) )
		{
			Error( "// ERROR: Merge( \"%s\", \"%s\" ); Failed - \"%s\" Unchanged.  Cannot write file.\n", pInFilename, pOutFilename, pOutFilename );
		}
	}
	else
	{
		Error( "// ERROR: Merge( \"%s\", \"%s\" ); Failed - \"%s\" Unchanged.\n", pInFilename, pOutFilename, pOutFilename );
	}

	g_pDataModel->UnloadFile( pRoot->GetFileId() );
	guard1.Release();

	return retVal;
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
CUtlString CDmxEdit::SaveDelta( const char *pDeltaName )
{
	if ( !m_pMesh )
	{
		Error( "// ERROR: SaveDelta( \"%s\" ) Failed - No Mesh\n", pDeltaName );
		return "";
	}

	Msg( "// SaveDelta( \"%s\" );\n", pDeltaName );

	CDmeVertexDeltaData *pDelta = m_pMesh->ModifyOrCreateDeltaStateFromBaseState( pDeltaName );
	if ( pDelta )
		return pDelta->GetName();

	Error( "// SaveDelta( \"%s\" )\n - Couldn't Create New Delta State From Base State \"bind\"\n", pDeltaName );
	return "";
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
bool CDmxEdit::DeleteDelta( const delta &d )
{
	if ( !m_pMesh )
	{
		Error( "// ERROR: DeleteDelta( \"%s\" ); Failed - No Mesh\n", static_cast< const char * >( d ) );
		return false;
	}

	if ( !d )
	{
		Error( "// ERROR: DeleteDelta( \"%s\" ); Failed - Invalid Delta\n", static_cast< const char * >( d ) );
		return false;
	}

	Msg( "// DeleteDelta( \"%s\" );\n", static_cast< const char * >( d ) );

	return m_pMesh->DeleteDeltaState( static_cast< const char * >( d ) );
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
bool CDmxEdit::RemapMaterial( int nMaterialIndex, const char *pNewMaterialName )
{
	Msg( "// RemapMaterial( %d, \"%s\" );\n", nMaterialIndex, pNewMaterialName );

	return CDmMeshUtils::RemapMaterial( m_pMesh, nMaterialIndex, pNewMaterialName );
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
bool CDmxEdit::RemapMaterial( const char *pOldMaterialName, const char *pNewMaterialName )
{
	Msg( "// RemapMaterial( \"%s\", \"%s\" );\n", pOldMaterialName, pNewMaterialName );
	return CDmMeshUtils::RemapMaterial( m_pMesh, pOldMaterialName, pNewMaterialName );
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
bool CDmxEdit::RemoveFacesWithMaterial( const char *pMaterialName )
{
	Msg( "// RemoveFacesWithMaterial( \"%s\", );\n", pMaterialName );
	return CDmMeshUtils::RemoveFacesWithMaterial( m_pMesh, pMaterialName );
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
bool CDmxEdit::RemoveFacesWithMoreThanNVerts( int nVertexCount )
{
	Msg( "// RemoveFacesWithMoreThanNVerts( %d );\n", nVertexCount );
	return CDmMeshUtils::RemoveFacesWithMoreThanNVerts( m_pMesh, nVertexCount );
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
bool CDmxEdit::Mirror( CAxisType axisType /* = XAXIS */ )
{
	Msg( "// Mirror( \"%s\" );\n", static_cast< const char * >( axisType ) );
	return CDmMeshUtils::Mirror( m_pMesh, axisType() );
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
bool CDmxEdit::ComputeNormals()
{
	Msg( "// ComputeNormals();\n" );
	m_pMesh->ComputeDeltaStateNormals();

	return true;
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
bool CDmxEdit::ComputeWrinkles()
{
	if ( !m_pMesh )
	{
		Error( "// ERROR: ComputeWrinkles(); Failed - No Mesh\n" );
		return false;
	}

	CDmeCombinationOperator *pCombo( FindReferringElement< CDmeCombinationOperator >( m_pMesh, "targets" ) );
	if ( !pCombo )
	{
		Error( "// ERROR: ComputeWrinkles(); Failed - No Combination Operator On Mesh\n" );
		return false;
	}

	Msg( "// ComputeWrinkles();\n" );
	pCombo->GenerateWrinkleDeltas();

	return true;
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
bool CDmxEdit::ComputeWrinkle( const char *pDeltaName, float flScale )
{
	if ( !m_pMesh )
	{
		Error( "// ERROR: ComputeWrinkle( \"%s\", %f ); Failed - No Mesh\n", pDeltaName, flScale );
		return false;
	}

	CDmeVertexDeltaData *pDelta = FindDeltaState( pDeltaName );
	if ( !pDelta )
	{
		Error( "// ERROR: ComputeWrinkle( \"%s\", %f ); Failed - Cannot Find Delta State\n", pDeltaName, flScale );
		return false;
	}

	Msg( "// ComputeWrinkle( \"%s\", %f );\n", pDeltaName, flScale );
	return CDmMeshUtils::CreateWrinkleDeltaFromBaseState( pDelta, flScale, m_pMesh );

	return true;
}




//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
bool CDmxEdit::CreateDeltasFromPresets(
	const char *pPresetFilename,
	bool bPurge /* = true */,
	const CUtlVector< CUtlString > *pPurgeAllButThese /* = NULL */,
	const char *pExpressionFilename /* = NULL */  )
{
	ClearPresetCache();
	CachePreset( pPresetFilename, pExpressionFilename );

	Msg( "// CreateDeltasFromPresets( \"%s\", %s );\n", pPresetFilename, bPurge ? "true" : "false" );
	const bool bRetVal = CDmMeshUtils::CreateDeltasFromPresets( m_pMesh, NULL, m_presetCache, bPurge, pPurgeAllButThese );

	ClearPresetCache();

	return bRetVal;
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
bool CDmxEdit::CachePreset( const char *pPresetFilename, const char *pExpressionFilename /* = NULL  */ )
{
	m_presetCache[ pPresetFilename ] = pExpressionFilename;
	return true;
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
bool CDmxEdit::ClearPresetCache()
{
	m_presetCache.Clear();
	return true;
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
bool CDmxEdit::CreateDeltasFromCachedPresets(
	bool bPurge /* = true */,
	const CUtlVector< CUtlString > *pPurgeAllButThese /* = NULL */ ) const
{
	Msg( "// CreateCachedDeltasFromPresets( %s );\n", bPurge ? "true" : "false" );
	return CDmMeshUtils::CreateDeltasFromPresets( m_pMesh, NULL, m_presetCache, bPurge, pPurgeAllButThese );
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
bool CDmxEdit::CreateExpressionFileFromPresets( const char *pPresetFilename, const char *pExpressionFilename )
{
	ClearPresetCache();
	CachePreset( pPresetFilename, pExpressionFilename );
	const bool bRetVal = CreateExpressionFilesFromCachedPresets();
	ClearPresetCache();

	return bRetVal;
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
bool CDmxEdit::CreateExpressionFilesFromCachedPresets() const
{
	bool bRetVal = true;

	CDisableUndoScopeGuard sgDisableUndo;

	char buf[ MAX_PATH ];
	char buf1[ MAX_PATH ];

	if ( !m_pMesh )
		return false;

	CDmeCombinationOperator *pComboOp( FindReferringElement< CDmeCombinationOperator >( m_pMesh, "targets" ) );
	if ( !pComboOp )
		return false;

	for ( int i = 0; i < m_presetCache.GetNumStrings(); ++i )
	{
		const char *pPresetFilename = m_presetCache.String( i );

		CDmElement *pRoot = NULL;
		g_p4factory->AccessFile( pPresetFilename )->Add();
		g_pDataModel->RestoreFromFile( pPresetFilename, NULL, NULL, &pRoot );

		if ( !pRoot )
		{
			bRetVal = false;
			continue;
		}

		CDmePresetGroup *pPresetGroup = CastElement< CDmePresetGroup >( pRoot );
		if ( pPresetGroup )
		{
			const CUtlString &expressionFilename = m_presetCache[ i ];
			if ( expressionFilename.IsEmpty() )
			{
				Warning( "// No expression file specified for preset %s\n", pPresetFilename );
				continue;
			}

			const char *pExpressionFilename = expressionFilename.Get();

			Q_strncpy( buf, pExpressionFilename, sizeof( buf ) );
			Q_SetExtension( buf, ".txt", sizeof( buf ) );
			Q_ExtractFilePath( buf, buf1, sizeof( buf1 ) );
			Q_FixSlashes( buf1 );
			g_pFullFileSystem->CreateDirHierarchy( buf1 );

			if ( !g_p4factory->AccessFile( buf )->Edit() )
			{
				g_p4factory->AccessFile( buf )->Add();
			}

			pPresetGroup->ExportToTXT( buf, NULL, pComboOp );

			Q_SetExtension( buf, ".vfe", sizeof( buf ) );
			Q_ExtractFilePath( buf, buf1, sizeof( buf1 ) );
			Q_FixSlashes( buf1 );
			g_pFullFileSystem->CreateDirHierarchy( buf1 );

			if ( !g_p4factory->AccessFile( buf )->Edit() )
			{
				g_p4factory->AccessFile( buf )->Add();
			}

			pPresetGroup->ExportToVFE( buf, NULL, pComboOp );
		}

		g_pDataModel->UnloadFile( pRoot->GetFileId() );
	}

	return bRetVal;
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
bool CDmxEdit::FixPresetFile(
	const char *pPresetFilename )
{
	CDisableUndoScopeGuard sgDisableUndo;

	Msg( "// FixPresetFile( \"%s\" );\n", pPresetFilename );

	CDmElement *pRoot = NULL;
	g_p4factory->AccessFile( pPresetFilename )->Add();
	g_pDataModel->RestoreFromFile( pPresetFilename, NULL, NULL, &pRoot );

	if ( !pRoot )
	{
		Error( "// FixPresetFile( \"%s\" ); - Failed, Can't Load Preset File\n", pPresetFilename );
		return false;
	}

	CDmePresetGroup *pPresetGroup = CastElement< CDmePresetGroup >( pRoot );
	if ( !pPresetGroup )
	{
		Error( "// FixPresetFile( \"%s\" ); - Failed, Root Element Is Not A CDmePresetGroup\n", pPresetFilename );
		return false;
	}

	const CDmaElementArray< CDmePreset > &presets = pPresetGroup->GetPresets();
	const int nPresetsCount = presets.Count();

	if ( nPresetsCount <= 0 )
		return true;	// Nothing to do

	for ( int i = 0; i < nPresetsCount; ++i )
	{
		CDmePreset *pPreset = presets[ i ];

		CDmElement *pCloseLid = NULL;
		CDmElement *pCloseLidV = NULL;
		CDmElement *pCloseLidLo = NULL;
		CDmElement *pCloseLidUp = NULL;

		CDmaElementArray< CDmElement > &controlValues = pPreset->GetControlValues();
		const int nControlValues = controlValues.Count();
		for ( int j = 0; j < nControlValues; ++j )
		{
			CDmElement *pControlPreset = controlValues[ j ];
			if ( !Q_strcmp( "CloseLid", pControlPreset->GetName() ) )
			{
				pCloseLid = pControlPreset;
			}
			else if ( !Q_strcmp( "CloseLidV", pControlPreset->GetName() ) )
			{
				pCloseLidV = pControlPreset;
			}
			else if ( !Q_strcmp( "CloseLidLo", pControlPreset->GetName() ) )
			{
				pCloseLidLo = pControlPreset;
			}
			else if ( !Q_strcmp( "CloseLidUp", pControlPreset->GetName() ) )
			{
				pCloseLidUp = pControlPreset;
			}
		}

		if ( pCloseLid )
		{
			if ( !pCloseLid->GetAttribute( "multilevel" ) )
			{
				if ( pCloseLidV )
				{
					pCloseLid->SetValue( "multilevel", pCloseLidV->GetValue( "value", 0.0f ) );
				}
				else
				{
					pCloseLid->SetValue( "multilevel", 0.5f );
				}
			}
		}
		else if ( pCloseLidLo && pCloseLidUp )
		{
			const float flCloseLidLo = pCloseLidLo->GetValue( "value", 0.0f );
			const float flCloseLidUp = pCloseLidUp->GetValue( "value", 0.0f );
			if ( flCloseLidLo != 0.0f || flCloseLidUp != 0.0f )
			{
				const float flBalance = ( pCloseLidLo->GetValue( "balance", 0.5f ) + pCloseLidUp->GetValue( "balance", 0.5f ) ) / 2.0f;

				int nControlIndex = controlValues.AddToTail( CreateElement< CDmElement >( "CloseLid", pRoot->GetFileId() ) );
				pCloseLid = controlValues[ nControlIndex ];
				if ( pCloseLid )
				{
					pCloseLid->SetValue( "value", flCloseLidLo / ( flCloseLidLo + flCloseLidUp ) );
					pCloseLid->SetValue( "multilevel", flCloseLidLo + flCloseLidUp );
					pCloseLid->SetValue( "balance", flBalance );
				}
				else
				{
					Error( "Couldn't Add A New Preset?!?!\n" );
				}
			}
		}
		else if ( pCloseLidV )
		{
			Msg( " * CloseLidV but no CloseLid?!?!? On %s\n", pPreset->GetName() );
		}

		// Purge the old junk
		bool bPurge = true;
		while ( bPurge )
		{
			bPurge = false;

			for ( int j = 0; j < controlValues.Count(); ++j )
			{
				CDmElement *pControlPreset = controlValues[ j ];
				if ( !Q_strcmp( "CloseLidV", pControlPreset->GetName() ) || !Q_strcmp( "CloseLidLo", pControlPreset->GetName() ) || !Q_strcmp( "CloseLidUp", pControlPreset->GetName() ) )
				{
					controlValues.Remove( j );
					bPurge = true;
					break;
				}
			}
		}
	}

	bool bStatus = g_p4factory->AccessFile( pPresetFilename )->Edit();
	if ( !bStatus )
	{
		g_p4factory->AccessFile( pPresetFilename )->Add();
	}

	bStatus = g_pDataModel->SaveToFile( pPresetFilename, NULL, "keyvalues2", "preset", pRoot );
	if ( !bStatus )
	{
		Error( "// FixPresetFile( \"%s\" ); - Couldn't Save File via DataModel\n", pPresetFilename );
	}

	g_pDataModel->UnloadFile( pRoot->GetFileId() );

	return bStatus;
}


void ScaleDeltaPositions( const CDmrArrayConst< Vector > &bindPosData, CDmeVertexDeltaData *pDelta, float sx, float sy, float sz )
{
	const int nPosIndex = pDelta->FindFieldIndex( CDmeVertexData::FIELD_POSITION );
	if ( nPosIndex < 0 )
		return;

	CDmrArray< Vector > posData = pDelta->GetVertexData( nPosIndex );
	const int nPosDataCount = posData.Count();
	if ( nPosDataCount <= 0 )
		return;

	Vector *pPosArray = reinterpret_cast< Vector * >( alloca( nPosDataCount * sizeof( Vector ) ) );

	Vector v;

	for ( int j = 0; j < nPosDataCount; ++j )
	{
		const Vector &s = posData.Get( j );
		Vector &d = pPosArray[ j ];
		d.x = s.x * sx;
		d.y = s.y * sy;
		d.z = s.z * sz;
	}

	posData.SetMultiple( 0, nPosDataCount, pPosArray );
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
bool CDmxEdit::Scale( float sx, float sy, float sz )
{
	int nArraySize = 0;
	Vector *pPosArray = NULL;

	const int nBaseStateCount = m_pMesh->BaseStateCount();
	for ( int i = 0; i < nBaseStateCount; ++i )
	{
		CDmeVertexData *pBase = m_pMesh->GetBaseState( i );
		const int nPosIndex = pBase->FindFieldIndex( CDmeVertexData::FIELD_POSITION );
		if ( nPosIndex < 0 )
			continue;

		CDmrArray< Vector > posData = pBase->GetVertexData( nPosIndex );
		const int nPosDataCount = posData.Count();
		if ( nPosDataCount <= 0 )
			continue;

		if ( nArraySize < nPosDataCount || pPosArray == NULL )
		{
			pPosArray = reinterpret_cast< Vector * >( alloca( nPosDataCount * sizeof( Vector ) ) );
			if ( pPosArray )
			{
				nArraySize = nPosDataCount;
			}
		}

		if ( nArraySize < nPosDataCount )
			continue;

		for ( int j = 0; j < nPosDataCount; ++j )
		{
			const Vector &s = posData.Get( j );
			Vector &d = pPosArray[ j ];
			d.x = s.x * sx;
			d.y = s.y * sy;
			d.z = s.z * sz;
		}

		posData.SetMultiple( 0, nPosDataCount, pPosArray );
	}

	{
		CDmeVertexData *pBind = m_pMesh->GetBindBaseState();
		const int nPosIndex = pBind ? pBind->FindFieldIndex( CDmeVertexData::FIELD_POSITION ) : -1;

		if ( !pBind || nPosIndex < 0 )
		{
			Warning( "// Can't Scale Deltas!\n" );
			return false;
		}

		const CDmrArrayConst< Vector > posData = pBind->GetVertexData( nPosIndex );

		const int nDeltaStateCount = m_pMesh->DeltaStateCount();
		for ( int i = 0; i < nDeltaStateCount; ++i )
		{
			ScaleDeltaPositions( posData, m_pMesh->GetDeltaState( i ), sx, sy, sz );
		}
	}

	Msg( "// Scale( %f %f %f );\n", sx, sy, sz );

	return true;
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
bool CDmxEdit::SetDistanceType( const CDistanceType &distanceType )
{
	m_distanceType = distanceType;
	return true;
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
bool CDmxEdit::Translate(
	Vector t,
	float featherDistance,
	const CFalloffType &falloffType,
	const CDistanceType &passedDistanceType,
	CDmeMesh *pPassedMesh,
	CDmeVertexData *pPassedBase,
	CDmeSingleIndexedComponent *pPassedSelection )
{
	const CDistanceType &distanceType = passedDistanceType() == CDmeMesh::DIST_DEFAULT ? m_distanceType : passedDistanceType;

	CDmeMesh *pMesh = pPassedMesh ? pPassedMesh : m_pMesh;
	if ( !m_pMesh )
	{
		Error( "// ERROR: Translate( %f %f %f, %f, %s, %s );\n", t.x, t.y, t.z, featherDistance, static_cast< char * >( falloffType ), static_cast< char * >( distanceType ) );
		return false;
	}

	CDmeVertexData *pBase = pPassedBase ? pPassedBase : pMesh->GetCurrentBaseState();

	CDmeSingleIndexedComponent *pSelection = pPassedSelection ? pPassedSelection : m_pCurrentSelection;
	CDmeSingleIndexedComponent *pTmpSelection = NULL;

	if ( !pSelection || pSelection->Count() == 0 )
	{
		pTmpSelection = CreateElement< CDmeSingleIndexedComponent >( "__selectAll" );
		pSelection = pTmpSelection;
		pMesh->SelectAllVertices( pSelection );
	}

	CDmeSingleIndexedComponent *pNewSelection = ( pSelection && featherDistance > 0.0f ) ? m_pMesh->FeatherSelection( featherDistance, falloffType(), distanceType(), pSelection, NULL ) : pSelection;

	int nArraySize = 0;
	Vector *pPosArray = NULL;

	const int nPosIndex = pBase->FindFieldIndex( CDmeVertexData::FIELD_POSITION );
	if ( nPosIndex < 0 )
		return false;

	CDmrArray< Vector > posData = pBase->GetVertexData( nPosIndex );
	const int nPosDataCount = posData.Count();
	if ( nPosDataCount <= 0 )
		return false;

	if ( nArraySize < nPosDataCount || pPosArray == NULL )
	{
		pPosArray = reinterpret_cast< Vector * >( alloca( nPosDataCount * sizeof( Vector ) ) );
		if ( pPosArray )
		{
			nArraySize = nPosDataCount;
		}
	}

	if ( nArraySize < nPosDataCount )
		return false;


	if ( distanceType() == CDmeMesh::DIST_RELATIVE )
	{
		Vector vCenter;
		float fRadius;

		pMesh->GetBoundingSphere( vCenter, fRadius, pBase, pSelection );

		t *= fRadius;
	}

	if ( pNewSelection )
	{
		memcpy( pPosArray, posData.Base(), nPosDataCount * sizeof( Vector ) );

		const int nSelectionCount = pNewSelection->Count();
		int nIndex;
		float fWeight;
		for ( int j = 0; j < nSelectionCount; ++j )
		{
			pNewSelection->GetComponent( j, nIndex, fWeight );
			const Vector &s = posData.Get( nIndex );
			Vector &d = pPosArray[ nIndex ];
			d = s + t * fWeight;
		}
	}
	else
	{
		for ( int j = 0; j < nPosDataCount; ++j )
		{
			const Vector &s = posData.Get( j );
			Vector &d = pPosArray[ j ];
			d = s + t;
		}
	}

	posData.SetMultiple( 0, nPosDataCount, pPosArray );

	Msg( "// Translate( %f %f %f, %f, %s, %s );\n", t.x, t.y, t.z, featherDistance, static_cast< char * >( falloffType ), static_cast< char * >( distanceType ) );

	return true;
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void CDmxEdit::Error( const tchar *pMsgFormat, ... )
{
	static Color outColor( 1, 1, 1, 1 );
	SetErrorState();
	va_list args;
	va_start( args, pMsgFormat );
	tchar pTempBuffer[5020];
	assert( _tcslen( pMsgFormat ) < sizeof( pTempBuffer) ); // check that we won't artificially truncate the string
	_vsntprintf( pTempBuffer, sizeof( pTempBuffer ) - 1, pMsgFormat, args );
	SpewStdout( SPEW_ERROR, pTempBuffer );
	va_end(args);

	if ( Plat_IsInDebugSession() )
	{
		Msg( "\nPress q To Quit, d To Debug Or Any Key To Continue After Error..." );
	}
	else
	{
		Msg( "\nPress q To Quit Or Any Key To Continue After Error..." );
	}

	int ch = getch();
	if ( ch == 'q' || ch == 'Q' )
	{
		Unload();
		exit( 1 );
	}
	else if ( Plat_IsInDebugSession() && ch == 'd' || ch == 'D' )
	{
		DebuggerBreakIfDebugging();
	}
	Msg( "\n" );
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
class CPauseOnScopeExit
{
public:
	CPauseOnScopeExit( CDmxEdit &dmxEdit )
	: m_dmxEdit( dmxEdit )
	{
	}

	~CPauseOnScopeExit()
	{
		m_dmxEdit.Unload();

		if ( m_dmxEdit.ErrorState() )
		{
			Msg( "\nPress Any Key To Continue...\n" );
			getch();
		}
	}

protected:

	CDmxEdit &m_dmxEdit;
};


//-----------------------------------------------------------------------------
// The application object
//-----------------------------------------------------------------------------
void CDmxEdit::DoIt()
{
	ResetErrorState();

	// Just for sugary sweet syntax
	CDmxEditProxy base( *this );

	CPauseOnScopeExit scopePause( *this );

	CDisableUndoScopeGuard guard;

	#include "doit.h"

	return;

	{
		base = "d:/dev/main/content/tf_movies/models/test/dmxedit/parts/obj/plane_zero.obj";

		ListDeltas();

		delta A = "A";
		delta B = "B";
		delta C = "C";
		delta D = "D";
		delta E = "E";

		Select( A );

		Add( C, 1.0 );
		SaveDelta( "A-to-C" );

		// Remember to ResetState or SetState!
		ResetState();
		Add( C, 1.0, 10.0 );
		SaveDelta( "A-to-C-Feather10" );

		ResetState();
		GrowSelection( 3 );
		Add( C, 1.0, 10.0 );
		SaveDelta( "A-to-C-Grow3-Feather10" );

		SetState( C );
		Select( B );
		Interp( base, 1.0, 24.0, STRAIGHT );
		delta straight = SaveDelta( "straight" );

		SetState( C );
		Select( B );
		Interp( base, 1.0, 24.0, BELL );
		delta bell = SaveDelta( "bell" );

		SetState( C );
		Select( B );
		Interp( base, 1.0, 24.0, DOME );
		delta dome = SaveDelta( "dome" );

		SetState( C );
		Select( B );
		Interp( base, 1.0, 24.0, SPIKE );
		delta spike = SaveDelta( "spike" );

		ResetState();
		Select( E );
		Interp( C, 1.0, 12.0, STRAIGHT );
		delta midStraight = SaveDelta( "midStraight" );

		ResetState();
		Select( E );
		Interp( C, 1.0, 12.0, BELL );
		delta midBell = SaveDelta( "midBell" );

		ResetState();
		Select( E );
		Interp( C, 1.0, 12.0, "DOME" );
		delta midDome = SaveDelta( "midDome" );

		ResetState();
		Select( E );
		Interp( C, 1.0, 12.0, SPIKE );
		delta midSpike = SaveDelta( "midSpike" );

		SetState( C );
		Select( C );
		Interp( D, 0.5 );
		SaveDelta( "C-to-50-D" );

		ResetState();
		Select( "F" );
		Select( "ADD", "G" );
		Interp( "C", 1.0 );
		SaveDelta( "F-ADD-G" );

		ResetState();
		Select( "F" );
		Select( "SUBTRACT", "G" );
		Interp( "C", 1.0 );
		SaveDelta( "F-SUB-G" );

		ResetState();
		Select( "F" );
		Select( TOGGLE, "G" );
		Interp( "C", 1.0 );
		SaveDelta( "F-TOG-G" );

		ResetState();
		Select( "F" );
		GrowSelection( 3 );
		Select( "INTERSECT", "G" );
		Interp( "C", 1.0 );
		SaveDelta( "F-INT-G" );

		ResetState();
		Select( "F" );
		Select( "REPLACE", "G" );
		Interp( "C", 1.0 );
		SaveDelta( "F-REP-G" );

//		ImportComboRules( "d:/dev/main/content/tf_movies/models/player/heavy/parts/dmx/heavy_head_rules.dmx" );

		// Can save to DMX as well
		Save( "c:/plane.dmx" );

		// Write out series of objs
//		Save( "c:/newplane_zero.obj", "ABSOLUTE" );
//		Save( "c:/blueplane_zero.obj", "RELATIVE" );
	}
}


//=============================================================================
// Lua Interface
//=============================================================================
CDmxEdit CDmxEditLua::m_dmxEdit;

CDmxEditLua::LuaFunc_s CDmxEditLua::s_luaFuncs[] =
{
	{	"Load",								Load,							"< $file.dmx | $file.obj >, [ $loadType ]",							"Replaces the current scene with the specified scene.  The current mesh will be set to the first mesh with a combination operator found in the new scene.  $loadType is one of \"absolute\" or \"relative\".  If not specified, \"absolute\" is assumed." },
	{	"Import",							Import,							"< $file.dmx >, [ $parentBone ]",									"Imports the specified DMX model into the scene.  The imported model can optionally be parented (which implictly skins it) to a specified bone in the existing scene via $parentBone." },
	{	"Save",								Save,							"[ $file.dmx | $file.obj ], [ $saveType ], [ $delta ]",				"Saves the current scene to either a single dmx file or a sequence of obj files.  If $file is not specified, the filename used in the last Load() command will be used to save the file.  $saveType is one of \"absolute\" or \"relative\".  If not specified, \"absolute\" is assumed.  If $delta is passed and the save type is OBJ, then only a single OBJ of that delta is saved.  \"base\" is the base state." },
	{	"ListDeltas",						ListDeltas,						"",																	"Prints a list of all of the deltas present in the current mesh" },
	{	"ImportComboRules",					ImportComboRules,				"< $rules.dmx >, [ #bOverwrite ], [ #bPurgeDeltas ]",				"Imports the combintion rules from the specified dmx file and replaces the current rules with those from the file is #bOverwrite specified or is true.  If #bOverwrite is false then any existing controls that are not in the imported rules file are preserved.  If #bPurgeDeltas is specified and is true, then any delta states which are no longer referred to by any combination rule or control will be purged. " },
	{	"ResetState",						ResetState,						"",																	"Resets the current mesh back to the default base state, i.e. no deltas active" },
	{	"SetState",							SetState,						"< $delta >",														"Sets the current mesh to the specified $delta" },
	{	"Select",							Select,							"< $delta >, [ $type ]",											"Changes the selection based on the vertices present in the specified $delta.  $type is one of \"add\", \"subtract\", \"toggle\", \"intersect\", \"replace\".  If $type is not specified, \"add\" is assumed." },
	{	"SelectHalf",						SelectHalf,						"< $halfType >",													"Selects all the vertices in half the mesh.  Half is one of left or right.  Left means the X value of the position is >= 0, right means that the X value of the position is <= 0.  So it's the character's left (assuming they're staring down -z)" },
	{	"GrowSelection",					GrowSelection,					"< #size >",														"Grows the current selection by the specified $size" },
	{	"ShrinkSelection",					ShrinkSelection,				"< #size >",														"Shrinks the current selection by the specified $size (integer)" },
	{	"Add",								Add,							"< $delta >, [ #weight ], [ #featherDistance ], < $falloffType >",	"Adds specified state to the current state of the mesh weighted and feathered by the specified #weight, #featherDistance & $falloffType.  $falloffType is one of \"straight\", \"spike\", \"dome\", \"bell\"" },
	{	"Interp",							Interp,							"< $delta >, [ #weight ], [ #featherDistance ], < $falloffType >",	"Interpolates the current state of the mesh towards the specified state, $weight, #featherDistance and $falloffType.  $falloffType is one of \"straight\", \"spike\", \"dome\", \"bell\"" },
	{	"SaveDelta",						SaveDelta,						"< $delta >",														"Saves the current state of the mesh to the named delta state" },
	{	"DeleteDelta",						DeleteDelta,					"< $delta >",														"Deletes the named delta state from the current mesh" },
	{	"Merge",							Merge,							"< $src.dmx >, < $dst.dmx >",										"Merges the current mesh onto the specified dmx file and saves to the second specified dmx file" },
	{	"RemapMaterial",					RemapMaterial,					"< #index | $srcMaterial >, < $dstMaterial >",						"Remaps the specified material to the new material, material can be specified by index or name" },
	{	"RemoveFacesWithMaterial",			RemoveFacesWithMaterial,		"< $material >",													"Removes faces from the current mesh which have the specified material" },
	{	"RemoveFacesWithMoreThanNVerts",	RemoveFacesWithMoreThanNVerts,	"< #vertexCount >",													"Removes faces from the current mesh which have more than the specified number of faces" },
	{	"Mirror",							Mirror,							"[ $axis ]",														"Mirrors the mesh in the specified axis.  $axis is one of \"x\", \"y\", \"z\". If $axis is not specified, \"x\" is assumed (i.e. \"x\" == mirror across YZ plane)" },
	{	"ComputeNormals",					ComputeNormals,					"",																	"Computes new smooth normals for the current mesh and all of its delta states." },
	{	"CreateDeltasFromPresets",			CreateDeltasFromPresets,		"< $preset.pre >, [ #bPurgeExisting = true ], [ { \"NoPurge1\", ... } ], [ $expression.txt ]",  "Creates a new delta state for every present defined in the specified preset file.  #bPurgeExisting specifies whether existing controls should be purged.  If #bPurgeExisting is true, then an optional array of deltas to not purge can be specified." },
	{	"CachePreset",						CachePreset,					"< $preset.pre >, [ $expression.txt ]",								"Caches the specified $preset file for later processing by a call to CreateDeltasFromCachedPresets().  If an $expression.txt file pathname is specified, an expression file will be written out for this preset file" },
	{	"ClearPresetCache",					ClearPresetCache,				"",																	"Removes all preset filenames that have been specified via CachePreset() calls" },
	{	"CreateDeltasFromCachedPresets",	CreateDeltasFromCachedPresets,	"[ #bPurgeExisting = true ], [ { \"NoPurge1\", ... } ]",			"Creates a new delta state for every preset defined in the preset files specified by previous CachePreset() calls.  Calling this calls ClearPresetCache().  #bPurgeExisting specifies whether existing controls should be purged.  If #bPurgeExisting is true, then an optional array of deltas to not purge can be specified." },
	{	"CreateExpressionFileFromPresets",	CreateExpressionFileFromPresets,	"< $preset.pre >, < $expression.txt >",							"Creates a faceposer expression.txt and expression.vfe file for the specified preset file" },
	{	"CreateExpressionFilesFromCachedPresets",	CreateExpressionFilesFromCachedPresets,	"",													"Creates a faceposer expression.txt and expression.vfe file for each of the preset/expression file pairs specified by previous CachePreset calls" },
	{	"ComputeWrinkles",					ComputeWrinkles,				"",																	"Computes new wrinkle/compress data based on the position deltas." },
	{	"ComputeWrinkle",					ComputeWrinkle,					"< $delta >, [ #scale = 1 ]",										"Computes new wrinkle/compress data for the specified delta based on the position deltas for the current state of the mesh scaled by the specified #scale.  If #scale it defaults to 1." },
	{	"Scale",							Scale,							"< #xScale >, [ #yScale, #zScale ]",								"Scales the position values by the specified amount.  If only 1 scale value is supplied a uniform scale in all three dimensions is applied" },
	{	"SetDistanceType",					SetDistanceType,				"< $distanceType >",												"Sets the way distances will be interpreted after the command.  $distanceType is one of \"absolute\" or \"relative\".  By default distances are \"absolute\".  All functions that work with distances (Add, Interp and Translate) work on the currently selected vertices.  \"absolute\" means use the distance as that number of units, \"relative\" means use the distance as a scale of the radius of the bounding sphere of the selected vertices." },
	{	"Translate",						Translate,						"< #xTranslate, #yTranslate, #zTranslate >, [ #falloffDistance, $falloffType ]",	"Translates the selected vertices of the mesh by the specified amount" },
	{	"FixPresetFile",					FixPresetFile,					"< $preset.pre >",													"Fixes the presets for CloseLid based on CloseLidLo, CloseLidUp or CloseLid, CloseLidV to new CloseLid multi-control" },
	{	"FileExists",						FileExists,						"< $filename >",													"Returns true if the given filename exists on disk, false otherwise" },
};


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
int CDmxEditLua::FunctionCount()
{
	return sizeof( s_luaFuncs ) / sizeof( s_luaFuncs[ 0 ] );
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
const CDmxEditLua::LuaFunc_s *CDmxEditLua::GetFunctionList()
{
	return &s_luaFuncs[ 0 ];
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
CDmxEditLua::CDmxEditLua()
: m_pLuaState( lua_open() )
{
	luaL_openlibs( m_pLuaState );

	for ( int i = 0; i < sizeof( s_luaFuncs ) / sizeof( s_luaFuncs[ 0 ] ); ++i )
	{
		lua_pushcfunction( m_pLuaState, s_luaFuncs[ i ].m_pFunc );
		lua_setglobal( m_pLuaState, s_luaFuncs[ i ].m_pFuncName );
	}

	const char *pLuaInit =
	"vsLuaDir = os.getenv( \"VPROJECT\" );\n"
	"vsLuaDir = string.gsub( vsLuaDir, \"[\\\\/]\", \"/\" );\n"
	"vsLuaDir = string.gsub( vsLuaDir, \"/*$\", \"\" );\n"
	"vsLuaDir = ( string.gsub( vsLuaDir, \"[^/]+$\", \"sdktools/lua\" ) .. \"/?.lua\" );\n"
	"package.path = ( package.path .. \";\" .. vsLuaDir );\n"
	"require( \"vs\" );\n";

	if ( luaL_loadstring( m_pLuaState, pLuaInit ) || lua_pcall( m_pLuaState, 0, LUA_MULTRET, 0 ) )
	{
		Error( "Error: %s\n", lua_tostring( m_pLuaState, -1 ) );
	}

}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
bool CDmxEditLua::DoIt( const char *pFilename )
{
	CDisableUndoScopeGuard guard;

	bool retVal = true;

	if ( luaL_loadfile( m_pLuaState, pFilename ) || lua_pcall( m_pLuaState, 0, LUA_MULTRET, 0 ) )
	{
		Error( "Error: %s\n", lua_tostring( m_pLuaState, -1 ) );
		retVal = false;
	}

	lua_close( m_pLuaState );

	m_dmxEdit.Unload();

	return retVal;
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
int CDmxEditLua::Load( lua_State *pLuaState )
{
	const char *pFilename = luaL_checkstring( pLuaState, 1 );

	const char *pLoadType = NULL;

	if ( lua_isboolean( pLuaState, 2 ) )
	{
		if ( lua_toboolean( pLuaState, 2 ) )
		{
			pLoadType = "Absolute";
		}
		else
		{
			pLoadType = "Relative";
		}
	}
	else if ( lua_isstring( pLuaState, 2 ) )
	{
		pLoadType = lua_tostring( pLuaState, 2 );
	}

	if ( pLoadType )
	{
		if ( m_dmxEdit.Load( pFilename, pLoadType ) )
			return 0;
	}
	else
	{
		if ( m_dmxEdit.Load( pFilename ) )
			return 0;
	}

	lua_pushnil( pLuaState );  /* return nil and ... */
	lua_pushstring( pLuaState, "Can't load file" );
	return 2;
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
int CDmxEditLua::Import( lua_State *pLuaState )
{
	const char *pFilename = luaL_checkstring( pLuaState, 1 );
	const char *pParentBone = lua_isstring( pLuaState, 2 ) ? lua_tostring( pLuaState, 2 ) : NULL;

	if ( m_dmxEdit.Import( pFilename, pParentBone ) )
		return 0;

	lua_pushnil( pLuaState );  /* return nil and ... */
	lua_pushstring( pLuaState, "Can't import file" );
	return 2;
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
int CDmxEditLua::ListDeltas( lua_State *pLuaState )
{
	m_dmxEdit.ListDeltas();
	return 0;
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
int CDmxEditLua::ImportComboRules( lua_State *pLuaState )
{
	const char *pFilename = luaL_checkstring( pLuaState, 1 );

	if ( lua_isboolean( pLuaState, 2 ) ) 
	{
		if ( lua_isboolean( pLuaState, 3 ) )
		{
			if ( m_dmxEdit.ImportComboRules( pFilename, lua_toboolean( pLuaState, 2 ) != 0, lua_toboolean( pLuaState, 3 ) != 0 ) )
				return 0;
		}
		else
		{
			if ( m_dmxEdit.ImportComboRules( pFilename, lua_toboolean( pLuaState, 2 ) != 0 ) )
				return 0;
		}
	}
	else
	{
		if ( m_dmxEdit.ImportComboRules( pFilename ) )
			return 0;
	}

	lua_pushnil( pLuaState );  /* return nil and ... */
	lua_pushstring( pLuaState, "Can't import combo rules" );
	return 2;
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
int CDmxEditLua::ResetState( lua_State *pLuaState )
{
	m_dmxEdit.ResetState();
	return 0;
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
int CDmxEditLua::SetState( lua_State *pLuaState )
{
	const char *pDeltaName = luaL_checkstring( pLuaState, 1 );

	if ( m_dmxEdit.SetState( pDeltaName ) )
		return 0;

	lua_pushnil( pLuaState );  /* return nil and ... */
	lua_pushstring( pLuaState, "Can't set state" );
	return 2;
}


//-----------------------------------------------------------------------------
// 1 or 2 string arguments
//-----------------------------------------------------------------------------
int CDmxEditLua::Select( lua_State *pLuaState )
{
	const char *pSelect1 = luaL_checkstring( pLuaState, 1 );
	const char *pSelect2 = lua_tostring( pLuaState, 2 );

	if ( pSelect2 )
	{
		if ( m_dmxEdit.Select( pSelect1, pSelect2 ) )
			return 0;
	}
	else
	{
		if ( m_dmxEdit.Select( pSelect1 ) )
			return 0;
	}

	lua_pushnil( pLuaState );  /* return nil and ... */
	lua_pushstring( pLuaState, "Can't Select" );
	return 2;
}


//-----------------------------------------------------------------------------
// 1 string argument
//-----------------------------------------------------------------------------
int CDmxEditLua::SelectHalf( lua_State *pLuaState )
{
	const char *pHalfType = luaL_checkstring( pLuaState, 1 );

	if ( pHalfType )
	{
		if ( m_dmxEdit.SelectHalf( pHalfType ) )
			return 0;
	}

	lua_pushnil( pLuaState );  /* return nil and ... */
	lua_pushstring( pLuaState, "Can't Select" );
	return 2;
}


//-----------------------------------------------------------------------------
// 1 or 2 string arguments
//-----------------------------------------------------------------------------
int CDmxEditLua::Save( lua_State *pLuaState )
{
	const char *pSave1 = lua_tostring( pLuaState, 1 );

	const char *pSaveType = NULL;

	if ( lua_isboolean( pLuaState, 2 ) )
	{
		if ( lua_toboolean( pLuaState, 2 ) )
		{
			pSaveType = "Absolute";
		}
		else
		{
			pSaveType = "Relative";
		}
	}
	else if ( lua_isstring( pLuaState, 2 ) )
	{
		pSaveType = lua_tostring( pLuaState, 2 );
	}

	if ( pSave1 )
	{
		if ( pSaveType )
		{
			const char *pDeltaName = lua_isstring( pLuaState, 3 ) ? lua_tostring( pLuaState, 3 ) : NULL;

			if ( m_dmxEdit.Save( pSave1, pSaveType, pDeltaName ) )
				return 0;
		}
		else
		{
			if ( m_dmxEdit.Save( pSave1 ) )
				return 0;
		}
	}
	else
	{
		if ( m_dmxEdit.Save() )
			return 0;
	}

	lua_pushnil( pLuaState );  /* return nil and ... */
	lua_pushstring( pLuaState, "Can't Save" );
	return 2;
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
int CDmxEditLua::GrowSelection( lua_State *pLuaState )
{
	if ( lua_isnumber( pLuaState, 1 ) )
	{
		const int nSize = lua_tointeger( pLuaState, 1 );
		if ( m_dmxEdit.GrowSelection( nSize ) )
			return 0;
	}
	else
	{
		if ( m_dmxEdit.GrowSelection() )
			return 0;
	}

	lua_pushnil( pLuaState );  /* return nil and ... */
	lua_pushstring( pLuaState, "Can't Grow Selection" );
	return 2;
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
int CDmxEditLua::ShrinkSelection( lua_State *pLuaState )
{
	if ( lua_isnumber( pLuaState, 1 ) )
	{
		const int nSize = lua_tointeger( pLuaState, 1 );
		if ( m_dmxEdit.ShrinkSelection( nSize ) )
			return 0;
	}
	else
	{
		if ( m_dmxEdit.ShrinkSelection() )
			return 0;
	}

	lua_pushnil( pLuaState );  /* return nil and ... */
	lua_pushstring( pLuaState, "Can't Shrink Selection" );
	return 2;
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
int CDmxEditLua::Add( lua_State *pLuaState )
{
	const char *pString1 = luaL_checkstring( pLuaState, 1 );
	float weight = lua_isnumber( pLuaState, 2 ) ? lua_tonumber( pLuaState, 2 ) : 1.0f;
	float featherDistance = lua_isnumber( pLuaState, 3 ) ? lua_tonumber( pLuaState, 3 ) : 0.0f;
	const char *pFalloffType = lua_isstring( pLuaState, 4 ) ? lua_tostring( pLuaState, 4 ) : "straight";

	if ( m_dmxEdit.Add( pString1, weight, featherDistance, pFalloffType ) )
		return 0;

	lua_pushnil( pLuaState );  /* return nil and ... */
	lua_pushstring( pLuaState, "Can't Add" );
	return 2;
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
int CDmxEditLua::Interp( lua_State *pLuaState )
{
	const char *pString1 = luaL_checkstring( pLuaState, 1 );
	float weight = lua_isnumber( pLuaState, 2 ) ? lua_tonumber( pLuaState, 2 ) : 1.0f;
	float featherDistance = lua_isnumber( pLuaState, 3 ) ? lua_tonumber( pLuaState, 3 ) : 0.0f;
	const char *pFalloffType = lua_isstring( pLuaState, 4 ) ? lua_tostring( pLuaState, 4 ) : "straight";

	if ( m_dmxEdit.Interp( pString1, weight, featherDistance, pFalloffType ) )
		return 0;

	lua_pushnil( pLuaState );  /* return nil and ... */
	lua_pushstring( pLuaState, "Can't Add" );
	return 2;
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
int CDmxEditLua::SaveDelta( lua_State *pLuaState )
{
	const char *pString1 = luaL_checkstring( pLuaState, 1 );
	if ( m_dmxEdit.SaveDelta( pString1 ) )
		return 0;

	lua_pushnil( pLuaState );  /* return nil and ... */
	lua_pushstring( pLuaState, "Can't SaveDelta" );
	return 2;
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
int CDmxEditLua::DeleteDelta( lua_State *pLuaState )
{
	const char *pString1 = luaL_checkstring( pLuaState, 1 );
	if ( m_dmxEdit.DeleteDelta( pString1 ) )
		return 0;

	lua_pushnil( pLuaState );  /* return nil and ... */
	lua_pushstring( pLuaState, "Can't Delete Delta" );
	return 2;
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
int CDmxEditLua::Merge( lua_State *pLuaState )
{
	const char *pString1 = luaL_checkstring( pLuaState, 1 );
	const char *pString2 = luaL_checkstring( pLuaState, 2 );
	if ( m_dmxEdit.Merge( pString1, pString2 ) )
		return 0;

	lua_pushnil( pLuaState );  /* return nil and ... */
	lua_pushstring( pLuaState, "Can't Merge" );
	return 2;
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
int CDmxEditLua::RemapMaterial( lua_State *pLuaState )
{
	const char *pString2 = luaL_checkstring( pLuaState, 2 );

	if ( lua_isnumber( pLuaState, 1 ) )
	{
		const int nMaterialIndex = luaL_checkinteger( pLuaState, 1 );
		if ( m_dmxEdit.RemapMaterial( nMaterialIndex, pString2 ) )
			return 0;
	}
	else
	{
		const char *pString1 = luaL_checkstring( pLuaState, 1 );
		if ( m_dmxEdit.RemapMaterial( pString1, pString2 ) )
			return 0;
	}

	lua_pushnil( pLuaState );  /* return nil and ... */
	lua_pushstring( pLuaState, "Can't Merge" );
	return 2;
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
int CDmxEditLua::RemoveFacesWithMaterial( lua_State *pLuaState )
{
	const char *pString1 = luaL_checkstring( pLuaState, 1 );
	if ( m_dmxEdit.RemoveFacesWithMaterial( pString1 ) )
		return 0;

	lua_pushnil( pLuaState );  /* return nil and ... */
	lua_pushstring( pLuaState, "Can't Merge" );
	return 2;
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
int CDmxEditLua::RemoveFacesWithMoreThanNVerts( lua_State *pLuaState )
{
	const int nVertsCount = luaL_checkinteger( pLuaState, 1 );
	if ( m_dmxEdit.RemoveFacesWithMoreThanNVerts( nVertsCount ) )
		return 0;

	lua_pushnil( pLuaState );  /* return nil and ... */
	lua_pushstring( pLuaState, "Can't Remove Faces With More Than N Verts" );
	return 2;
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
int CDmxEditLua::Mirror( lua_State *pLuaState )
{
	if ( lua_isstring( pLuaState, 1 ) )
	{
		const char *pString1 = luaL_checkstring( pLuaState, 1 );
		if ( m_dmxEdit.Mirror( pString1 ) )
			return 0;
	}
	else
	{
		if ( m_dmxEdit.Mirror() )
			return 0;
	}

	lua_pushnil( pLuaState );  /* return nil and ... */
	lua_pushstring( pLuaState, "Can't Mirror" );
	return 2;
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
int CDmxEditLua::ComputeNormals( lua_State *pLuaState )
{
	if ( m_dmxEdit.ComputeNormals() )
		return 0;

	lua_pushnil( pLuaState );  /* return nil and ... */
	lua_pushstring( pLuaState, "Can't Compute Normals" );
	return 2;
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
int CDmxEditLua::CreateDeltasFromPresets( lua_State *pLuaState )
{
	const char *pPresetFilename = luaL_checkstring( pLuaState, 1 );
	const bool bPurge = lua_isboolean( pLuaState, 2 ) ? lua_toboolean( pLuaState, 2 ) ? true : false : true;
	const char *pExpressionFilename = lua_isstring( pLuaState, 4 ) ? lua_tostring( pLuaState, 4 ) : NULL;

	CUtlVector< CUtlString > purgeAllBut;
	const CUtlVector< CUtlString > *pPurgeAllBut = NULL;

	if ( lua_istable( pLuaState, 3 ) )
	{
		pPurgeAllBut = &purgeAllBut;

		// table is in the stack at index '3'
		lua_pushnil( pLuaState );  //
		while (lua_next( pLuaState, 3 ) != 0) {
			// uses 'key' (at index -2) and 'value' (at index -1)
			/*
			Msg("%s - %s\n",
				lua_typename( pLuaState, lua_type( pLuaState, -2 ) ),
				lua_typename( pLuaState, lua_type( pLuaState, -1 ) ) );
			*/

			purgeAllBut.AddToTail( lua_tostring( pLuaState, -1 ) );

			// removes 'value'; keeps 'key' for next iteration */
			lua_pop( pLuaState, 1 );
		}
	}

	if ( m_dmxEdit.CreateDeltasFromPresets( pPresetFilename, bPurge, pPurgeAllBut, pExpressionFilename ) )
			return 0;

	lua_pushnil( pLuaState );  /* return nil and ... */
	lua_pushstring( pLuaState, "Can't Create Deltas From Presets" );
	return 2;
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
int CDmxEditLua::CreateDeltasFromCachedPresets( lua_State *pLuaState )
{
	const bool bPurge = lua_isboolean( pLuaState, 1 ) ? lua_toboolean( pLuaState, 1 ) ? true : false : true;

	CUtlVector< CUtlString > purgeAllBut;
	const CUtlVector< CUtlString > *pPurgeAllBut = NULL;

	if ( lua_istable( pLuaState, 2 ) )
	{
		pPurgeAllBut = &purgeAllBut;

		// table is in the stack at index '2'
		lua_pushnil( pLuaState );  //
		while (lua_next( pLuaState, 2 ) != 0) {
			// uses 'key' (at index -2) and 'value' (at index -1)
			/*
			Msg("%s - %s\n",
				lua_typename( pLuaState, lua_type( pLuaState, -2 ) ),
				lua_typename( pLuaState, lua_type( pLuaState, -1 ) ) );
			*/

			purgeAllBut.AddToTail( lua_tostring( pLuaState, -1 ) );

			// removes 'value'; keeps 'key' for next iteration */
			lua_pop( pLuaState, 1 );
		}
	}

	bool bRetVal = m_dmxEdit.CreateDeltasFromCachedPresets( bPurge, pPurgeAllBut );
	m_dmxEdit.ClearPresetCache();

	if ( bRetVal )
			return 0;

	lua_pushnil( pLuaState );  /* return nil and ... */
	lua_pushstring( pLuaState, "Can't Create Deltas From Cached Presets" );
	return 2;
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
int CDmxEditLua::CachePreset( lua_State *pLuaState )
{
	const char *pPresetFilename = luaL_checkstring( pLuaState, 1 );
	const char *pExpressionFilename = lua_isstring( pLuaState, 2 ) ? lua_tostring( pLuaState, 2 ) : NULL;

	if ( m_dmxEdit.CachePreset( pPresetFilename, pExpressionFilename ) )
		return 0;

	lua_pushnil( pLuaState );  /* return nil and ... */
	lua_pushstring( pLuaState, "Can't Create Deltas From Cached Presets" );
	return 2;
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
int CDmxEditLua::ClearPresetCache( lua_State *pLuaState )
{
	if ( m_dmxEdit.ClearPresetCache() )
			return 0;

	lua_pushnil( pLuaState );  /* return nil and ... */
	lua_pushstring( pLuaState, "Can't Create Deltas From Cached Presets" );
	return 2;
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
int CDmxEditLua::CreateExpressionFileFromPresets( lua_State *pLuaState )
{
	const char *pPresetFilename = luaL_checkstring( pLuaState, 1 );
	const char *pExpressionFilename = luaL_checkstring( pLuaState, 2 );

	if ( m_dmxEdit.CreateExpressionFileFromPresets( pPresetFilename, pExpressionFilename ) )
		return 0;

	lua_pushnil( pLuaState );  /* return nil and ... */
	lua_pushstring( pLuaState, "Can't Create Expression File From Presets" );
	return 2;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
int CDmxEditLua::CreateExpressionFilesFromCachedPresets( lua_State *pLuaState )
{
	if ( m_dmxEdit.CreateExpressionFilesFromCachedPresets() )
		return 0;

	lua_pushnil( pLuaState );  /* return nil and ... */
	lua_pushstring( pLuaState, "Can't Create Expression Files From Cached Presets" );
	return 2;
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
int CDmxEditLua::FixPresetFile( lua_State *pLuaState )
{
	const char *pPresetFilename = luaL_checkstring( pLuaState, 1 );

	if ( m_dmxEdit.FixPresetFile( pPresetFilename ) )
			return 0;

	lua_pushnil( pLuaState );  /* return nil and ... */
	lua_pushstring( pLuaState, "Can't Fix Preset File" );
	return 2;
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
int CDmxEditLua::FileExists( lua_State *pLuaState )
{
	const char *pFilename = luaL_checkstring( pLuaState, 1 );

	if ( g_pFullFileSystem->FileExists( pFilename ) )
	{
		lua_pushboolean( pLuaState, true );
	}
	else
	{
		lua_pushboolean( pLuaState, false );
	}

	return 1;
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
int CDmxEditLua::ComputeWrinkles( lua_State *pLuaState )
{
	if ( m_dmxEdit.ComputeWrinkles() )
		return 0;

	lua_pushnil( pLuaState );  /* return nil and ... */
	lua_pushstring( pLuaState, "Can't Compute Wrinkles" );
	return 2;
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
int CDmxEditLua::ComputeWrinkle( lua_State *pLuaState )
{
	const char *pDeltaName = luaL_checkstring( pLuaState, 1 );
	const float flScale = lua_isnumber( pLuaState, 2 ) ? lua_tonumber( pLuaState, 2 ) : 1.0f;

	if ( m_dmxEdit.ComputeWrinkle( pDeltaName, flScale ) )
		return 0;

	lua_pushnil( pLuaState );  /* return nil and ... */
	lua_pushstring( pLuaState, "Can't Compute Wrinkle" );
	return 2;
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
int CDmxEditLua::Scale( lua_State *pLuaState )
{
	const float sx = luaL_checknumber( pLuaState, 1 );
	float sy = sx;
	float sz = sx;

	if ( lua_isnumber( pLuaState, 2 ) && lua_isnumber( pLuaState, 3 ) )
	{
		sy = lua_tonumber( pLuaState, 2 );
		sz = lua_tonumber( pLuaState, 3 );
	}

	if ( m_dmxEdit.Scale( sx, sy, sz ) )
		return 0;

	lua_pushnil( pLuaState );  /* return nil and ... */
	lua_pushstring( pLuaState, "Can't Scale" );
	return 2;
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
int CDmxEditLua::Translate( lua_State *pLuaState )
{
	const float tx = luaL_checknumber( pLuaState, 1 );
	const float ty = luaL_checknumber( pLuaState, 2 );
	const float tz = luaL_checknumber( pLuaState, 3 );

	if ( lua_gettop( pLuaState ) < 4 )
	{
		if ( m_dmxEdit.Translate( Vector( tx, ty, tz ) ) )
			return 0;
	}
	else
	{
		float flFeatherDistance = 0.0f;
		const char *pFalloffType = "STRAIGHT";
		const char *pDistanceType = "DEFAULT";

		if ( lua_gettop( pLuaState ) == 4 && lua_isstring( pLuaState, 4 ) )
		{
			pDistanceType = lua_tostring( pLuaState, 4 );
		}
		else
		{
			flFeatherDistance = lua_isnumber( pLuaState, 4 ) ? lua_tonumber( pLuaState, 4 ) : flFeatherDistance;
			pFalloffType = lua_isstring( pLuaState, 5 ) ? lua_tostring( pLuaState, 5 ) : pFalloffType;
			pDistanceType = lua_isstring( pLuaState, 6 ) ? lua_tostring( pLuaState, 6 ) : pDistanceType;
		}

		if ( m_dmxEdit.Translate( Vector( tx, ty, tz ), flFeatherDistance, pFalloffType, pDistanceType ) )
			return 0;
	}

	lua_pushnil( pLuaState );  /* return nil and ... */
	lua_pushstring( pLuaState, "Can't Translate" );
	return 2;
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
int CDmxEditLua::SetDistanceType( lua_State *pLuaState )
{
	const char *pDistanceType = luaL_checkstring( pLuaState, 1 );

	if ( m_dmxEdit.SetDistanceType( pDistanceType ) )
		return 0;

	lua_pushnil( pLuaState );  /* return nil and ... */
	lua_pushstring( pLuaState, "Can't SetDistanceType" );
	return 2;
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void CDmxEditLua::SetVar( const CUtlString &var, const CUtlString &val )
{
	CUtlString sLuaCmd( var );
	sLuaCmd += " = ";
	sLuaCmd += val;
	sLuaCmd += ";";

	if ( luaL_loadstring( m_pLuaState, sLuaCmd.Get() ) || lua_pcall( m_pLuaState, 0, LUA_MULTRET, 0 ) )
	{
		Error( "Error: %s\n", lua_tostring( m_pLuaState, -1 ) );
	}
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void CDmxEditLua::SetGame( const CUtlString &game )
{
	CUtlString sLuaCmd( "vs.SetGame( " );
	sLuaCmd += game;
	sLuaCmd += " );";

	if ( luaL_loadstring( m_pLuaState, sLuaCmd.Get() ) || lua_pcall( m_pLuaState, 0, LUA_MULTRET, 0 ) )
	{
		Error( "Error: %s\n", lua_tostring( m_pLuaState, -1 ) );
	}
}
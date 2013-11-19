//======= Copyright © 1996-2006, Valve Corporation, All rights reserved. ======
//
// Purpose: Declaration of ValveMaya::CVsDmxImport
//
//=============================================================================

// Maya includes
#include <maya/MDagPath.h>
#include <maya/MFnMesh.h>

// Valve includes
#include "tier1/utlstack.h"
#include "ValveMaya/Undo.h"


class MArgDatabase;
class MDagModifier;
class CDmeDag;
class CDmeMesh;
class CDmeModel;
class CDmeAnimationList;


class CDmeImport
{
public:
	CDmeImport(
		ValveMaya::CUndo *pUndo );

	~CDmeImport();

	MDagPath DoIt(
		const MArgDatabase &mArgDatabase,
		CDmElement *pDmeRoot );

	MDagPath DoIt( CDmElement *pDmeRoot ) { return DoIt( MArgDatabase(), pDmeRoot ); }

	static void DoNormals(
		CDmeVertexData *pDmeVertexData,
		MFnMesh &meshFn );

	enum Type
	{
		kModel		= 1 << 0,
		kAnimation	= 1 << 1
	};

	static const char *const m_sOptImportType;
	static const char *const m_sLongOptImportType;
	static const char *const m_sOptSetTimeline;
	static const char *const m_sLongOptSetTimeline;

protected:
	void DmeDagToMayaDag(
		CDmeDag *pDmeDag,
		const MObject &parentObj = MObject::kNullObj );

	void DmeMeshToMayaMesh(
		CDmeMesh *pDmeMesh,
		const MObject &parentObj );

	void CreateTranslationCurves( const MDagPath &mDagPath, const CDmeVector3Log *pTLog, const MTime &mTimeOffset, MTime &minTime, MTime &maxTime );

	void CreateOrientationCurves( const MDagPath &mDagPath, const CDmeQuaternionLog *pOLog, const MTime &mTimeOffset, MTime &minTime, MTime &maxTime );

	void ProcessAnimation(
		const MDagPath &mDagRoot,
		CDmeDag *pSkeleton,
		CDmeAnimationList *pAnimationList,
		const MTime &mTimeOffset,
		bool bSetTimeSlider );

	ValveMaya::CUndo *m_pUndo;						// Pointer to an undo class
	bool m_bOwnUndo;								// Does this own the undo 

	uint m_type;									// Type of Data being imported
	MDagPath m_mDagRoot;							// The Maya root Dag node of the converted DmeData
	CUtlStack< CDmeModel * > m_dmeModelStack;		// Currently active DME model (should only be 1)

	class CMultiShadingGroup
	{
	public:
		MStatus Create( ValveMaya::CUndo *pUndo );

		MString m_materialName;
		MString m_materialPath;

		MSelectionList m_mSelectionList;
	};

	CUtlStringMap< CMultiShadingGroup > m_shadingGroups;	// Maps of shading groups so they are only created once
};
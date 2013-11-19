//======= Copyright © 1996-2006, Valve Corporation, All rights reserved. ======
//
// Purpose: Implementation of CVstSmdIOCommand
//
//=============================================================================

#ifndef VSTSMDIOCMD_H
#define VSTSMDIOCMD_H
#if defined( _WIN32 )
#pragma once
#endif


// Maya includes
#include <maya/MArgList.h>
#include <maya/MArgDatabase.h>
#include <maya/MDagModifier.h>
#include <maya/MPxCommand.h>
#include <maya/MSelectionList.h>
#include <maya/MSyntax.h>


// Valve includes
#include "valveMaya/Undo.h"


//-----------------------------------------------------------------------------
// SMD Input/Output Command Definition
//-----------------------------------------------------------------------------
class CVstSmdIOCmd : public MPxCommand
{
public:
	CVstSmdIOCmd();

	virtual ~CVstSmdIOCmd();

	static void *Factory();

	static MSyntax CVstSmdIOCmd::SyntaxFactory();

	virtual MStatus doIt(
		const MArgList &mArgList );

	virtual MStatus undoIt();

	virtual MStatus redoIt();

	virtual bool isUndoable() const { return m_undo.IsUndoable(); }

	virtual bool hasSyntax() const { return true; }

protected:
	static void PrintHelp();

	MStatus DoExport(
		const MArgDatabase &mArgDatabase );

	MStatus DoImport(
		const MArgDatabase &mArgDatabase );

	static MStatus GetOptSelection(
		const MArgDatabase &mArgDatabase,
		MSelectionList &mSelectionList );

	static uint GetExportType(
		const MArgDatabase &mArgDatabase );

	static uint GetExportFlags(
		const MArgDatabase &mArgDatabase );

	static void GetExportFrameRange(
		const MArgDatabase &mArgDatabase,
		double &optFs,
		double &optFe,
		double &optFi );

	static MString GetNodeAddPrefix(
		const MArgDatabase &mArgDatabase );

	static MString GetNodeDelPrefix(
		const MArgDatabase &mArgDatabase );

	ValveMaya::CUndo m_undo;				// The undo manager
};

#endif // VSTSMDIOCMD_H

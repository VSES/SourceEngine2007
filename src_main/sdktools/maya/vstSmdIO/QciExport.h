//======= Copyright © 1996-2006, Valve Corporation, All rights reserved. ======
//
// Purpose:
//
//=============================================================================

#ifndef QCIEXPORT_H
#define QCIEXPORT_H

#if defined( _WIN32 )
#pragma once
#endif


// Standard includes
#include <fstream>


// Maya includes
#include <maya/MDagPath.h>
#include <maya/MMatrix.h>
#include <maya/MStatus.h>
#include <maya/MString.h>
#include <maya/MSelectionList.h>


class CQciExport
{
public:

	MStatus DoIt(
		const MString &filename,
		const MSelectionList &exportList,
		MStringArray &result );

protected:
	void ExportAttachment( const MDagPath &mDagPath );

	void OpenStream();

	void PrettyPrint( const MVector &v );

	MString m_filename;
	std::ofstream m_os;
};


#endif // QCIEXPORT_H

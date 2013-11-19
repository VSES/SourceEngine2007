#include "pch.h"

MStatus DXCCPreviewChangedCommand::doIt ( const MArgList& Arg )
{
	MStatus stat= MS::kSuccess;

	

	g_PreviewPipeline.UI_SetPreviewState(g_PreviewPipeline.UI_GetPreviewState());

	

	return stat;
}


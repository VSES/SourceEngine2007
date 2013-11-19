#include "pch.h"

MStatus DCCRebuildDirtyCommand::doIt ( const MArgList& Arg)
{
	MStatus stat;

	g_PreviewPipeline.Scene_Update(false);



	return stat;
}

MStatus DXCCRebuildSceneCommand::doIt ( const MArgList& Arg)
{

	MStatus stat = MS::kSuccess;

	DXCC_DPF_REPORT(TEXT("REFRESH SCENE"));
	MGlobal::displayInfo("DirectX Rebuilding Scene");

	g_PreviewPipeline.TagGraph.DeregisterNodes();
	g_PreviewPipeline.TagGraph.RegisterNodes();
	g_PreviewPipeline.Scene_Update(false);

	return stat;
}

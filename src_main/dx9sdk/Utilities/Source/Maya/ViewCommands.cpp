#include "pch.h"


MStatus DXCCFrontViewCommand::doIt ( const MArgList& Arg)
{
	MStatus stat;
	stat= g_PreviewPipeline.BindViewerToPanel( "front");
	return stat;
}

MStatus DXCCSideViewCommand::doIt ( const MArgList& Arg)
{
	MStatus stat;
	stat= g_PreviewPipeline.BindViewerToPanel( "side");
	return stat;
}

MStatus DXCCTopViewCommand::doIt ( const MArgList& Arg)
{
	MStatus stat;
	stat= g_PreviewPipeline.BindViewerToPanel( "top");
	return stat;
}

MStatus DXCCFloatingViewCommand::doIt ( const MArgList& Arg)
{
	MStatus stat;
	stat= g_PreviewPipeline.BindViewerToPanel("floating");
	return stat;
}

MStatus DXCCCloseViewCommand::doIt ( const MArgList& Arg)
{
	g_PreviewPipeline.g_Viewer.Hide();
	return  MStatus::kSuccess;
}

#include "pch.h"

/*
MStatus DXCCExportSelectedCommand::doIt ( const MArgList& Arg)
{
	HRESULT hr= S_OK;
	OPENFILENAMEA open;
	ZeroMemory(&open, sizeof(open));
	char szFilename[MAX_PATH]= "";
	open.lStructSize= sizeof(OPENFILENAME); 
	open.hwndOwner= (HWND)M3dView::applicationShell();
	open.hInstance= (HINSTANCE)GetWindowInstance(open.hwndOwner); 
	open.lpstrFilter= "X-File\0*.x\0"; 
	open.lpstrFile= szFilename;
	open.nMaxFile= sizeof(szFilename);
	open.Flags=OFN_CREATEPROMPT|OFN_OVERWRITEPROMPT;



	if(GetSaveFileNameA(&open))
	{
		char* pExtension= strrchr(szFilename, '.');
		if(!pExtension || (0 != _stricoll(".x", pExtension)))
		{
			StringCchCatA(szFilename,MAX_PATH, ".x");
		};

		hr= g_PreviewPipeline.Scene_Export(szFilename, NULL, MPxFileTranslator::kSaveAccessMode);
	}

	return MS::kSuccess;
}
*/

MStatus DXCCExportOptionsCommand::doIt ( const MArgList& Arg)
{
	g_PreviewPipeline.GetExportOptions(true);
	return MS::kSuccess;
}

MStatus DXCCExportSceneCommand::doIt ( const MArgList& Arg)
{

	HRESULT hr= S_OK;
	OPENFILENAMEA open;
	ZeroMemory(&open, sizeof(open));
	char szFilename[MAX_PATH]= "";
	open.lStructSize= sizeof(OPENFILENAME); 
	open.hwndOwner= (HWND)M3dView::applicationShell();
	open.hInstance= (HINSTANCE)GetWindowInstance(open.hwndOwner); 
	open.lpstrFilter= "X-File\0*.x\0"; 
	open.lpstrFile= szFilename;
	open.nMaxFile= sizeof(szFilename);
	open.Flags=OFN_CREATEPROMPT|OFN_OVERWRITEPROMPT;


	bool found= false;
	if(GetSaveFileNameA(&open))
	{
		hr= g_PreviewPipeline.Scene_Export(szFilename, NULL, MPxFileTranslator::kSaveAccessMode);
	}

	return MS::kSuccess;
}
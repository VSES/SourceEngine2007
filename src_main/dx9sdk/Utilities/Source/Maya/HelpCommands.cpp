#include "pch.h"

MStatus	
DXMUiHelpCommand::doIt ( const MArgList& args)
{
	CHAR Sdk_Path[MAX_PATH]= "";

	if(GetEnvironmentVariableA("DXSDK_DIR" , Sdk_Path, MAX_PATH) > 0 )
	{
		CHAR cl_template[]= "explorer mk:@MSITStore:%sDocumentation/DirectX9/directx_sdk.chm::/directx/directxsdk/tools/content/dxextensions/mayaplugin.htm\"";

		int sdkLen= lstrlenA(Sdk_Path);
		for(int iC= 0; iC <sdkLen; iC++)
		{
			if( Sdk_Path[iC] == '\\')
				Sdk_Path[iC] = '/';
		}

		CHAR cl[MAX_PATH+sizeof(cl_template)];

		StringCbPrintfA(cl, sizeof(cl),  cl_template, Sdk_Path);

		STARTUPINFOA si;
		PROCESS_INFORMATION pi;

		ZeroMemory( &si, sizeof(si) );
		si.cb = sizeof(si);
		ZeroMemory( &pi, sizeof(pi) );

		CreateProcessA(
			NULL,
			cl,
			NULL,
			NULL,
			FALSE,
			0,
			NULL,
			NULL,
			&si,
			&pi);

		// Wait until child process exits.
		//WaitForSingleObject( pi.hProcess, INFINITE );

		// Close process and thread handles. 
		CloseHandle( pi.hProcess );
		CloseHandle( pi.hThread );


		return MStatus::kSuccess;
	}

	return MStatus::kFailure;
}
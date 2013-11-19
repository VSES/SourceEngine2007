
#include "StdAfx.h"
#include "resource.h"

typedef ClassDesc* (*GetClassDescMethod)();

ClassDesc* XFileExporter_GetClassDesc();
ClassDesc* SessionGup_GetClassDesc();

//

HINSTANCE Instance;
bool ControlsInitialized = false;

GetClassDescMethod Plugins[] =
{
	XFileExporter_GetClassDesc,
    SessionGup_GetClassDesc
};

//

__declspec( dllexport ) const TCHAR* LibDescription()
{
	return TEXT("Microsoft DirectX Extensions for Max");
}

__declspec( dllexport ) ULONG LibVersion()
{
	return VERSION_3DSMAX;
}

__declspec( dllexport ) ULONG CanAutoDefer()
{
	return 0;
}

__declspec( dllexport ) int LibNumberClasses()
{
	return sizeof(Plugins)/sizeof(Plugins[0]);
}

__declspec( dllexport ) ClassDesc* LibClassDesc(int i)
{
	if (i < LibNumberClasses() && Plugins[i])
	{
		return Plugins[i]();
	}
	else
	{
		return NULL;
	}
}

__declspec( dllexport ) void LibInit()
{
}

void AppendPathToEnv(const TCHAR* path)
{
    DWORD oldPathLen = GetEnvironmentVariable("PATH", NULL, 0);
    DWORD newPathLen = (oldPathLen+1) + (_tcslen(path)+1) + 1;    // account for NUL terms and semicolon

    TCHAR* newPath = (TCHAR*)_alloca(sizeof(TCHAR)*newPathLen);
    if (newPath)
    {
        DWORD cch = GetEnvironmentVariable(TEXT("PATH"), newPath, newPathLen);
        if (cch > 0 && cch < newPathLen)
        {
            StringCchCat(newPath, newPathLen, TEXT(";"));
            StringCchCat(newPath, newPathLen, path);

            SetEnvironmentVariable(TEXT("PATH"), newPath);
        }
    }
}

void AddDxccDllPath(void)
{
   
    HMODULE dll = GetModuleHandle("DXExtensionsMax.gup");
    TCHAR dllPath[MAX_PATH];
    DWORD dllPathLen = GetModuleFileName(dll, dllPath, MAX_PATH);

    if (dllPathLen != 0 && dllPathLen < MAX_PATH)
    {
        TCHAR fullPath[MAX_PATH];
        TCHAR* fileName;

        DWORD fullPathLen = GetFullPathName(dllPath, MAX_PATH, fullPath, &fileName);
        if (fullPathLen != 0 && fullPathLen < MAX_PATH && fileName)
        {
            fileName[0] = 0;    // lop off file name from path

            AppendPathToEnv(fullPath);

            PathAppend(fullPath, "..\\..\\..\\x86\\");

            AppendPathToEnv(fullPath);
        }
    }
}

BOOL WINAPI DllMain(HINSTANCE hinstDLL,ULONG fdwReason,LPVOID lpvReserved)
{
	Instance = hinstDLL;				// Hang on to this DLL's instance handle.

	if (!ControlsInitialized) 
	{
        AddDxccDllPath();

		ControlsInitialized = TRUE;
		InitCustomControls(Instance);	// Initialize MAX's custom controls
		InitCommonControls();			// Initialize Win95 controls
	}
			
	return (TRUE);
}


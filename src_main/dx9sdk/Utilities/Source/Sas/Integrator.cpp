#include "stdafx.h"
#include "Integrator.h"

using namespace Sas;

bool SasFileExists(LPCTSTR path)
{
	// Check to see if the file is found at that location
	WIN32_FIND_DATA findData;
	HANDLE handle = FindFirstFile( path, &findData );
	FindClose(handle);

	return (handle != INVALID_HANDLE_VALUE);
}

bool Integrator::FindFile(const WCHAR* find, const WCHAR* alternateDirectory, SourceInfo& found )
{
	if( NULL == find )
		return false;

	WCHAR InDir[MAX_PATH] = L"";
	if(MAX_PATH <= ExpandEnvironmentStrings(find, InDir, MAX_PATH) )
	{
		assert(false);
		return false;
	}	

	//get rid of '/'
	for(int i = 0; i < MAX_PATH; i++)
	{
		if(InDir[i] == L'/')
			InDir[i] = L'\\';
	}

	if( FALSE == PathIsRelative( InDir ) )
	{
		if( true == SasFileExists(InDir) )
		{
			found = SourceInfo(InDir);
			return true;
		}
	}
	else
	{
		WCHAR TmpDir[MAX_PATH] = L"";

		//Attempt alternate directory first
		PathCombine(TmpDir, alternateDirectory, InDir);
		if( true == SasFileExists(TmpDir) )
		{
			found = SourceInfo(TmpDir);
			return true;
		}

		WCHAR CurDir[MAX_PATH] = L"";
		GetCurrentDirectory(MAX_PATH, CurDir);
		PathCombine(TmpDir, CurDir, InDir);
		if( true == SasFileExists(TmpDir) )
		{
			found = SourceInfo(TmpDir);
			return true;
		}
	}

	return false;
}

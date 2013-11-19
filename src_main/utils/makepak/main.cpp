//====== Copyright © 1996-2004, Valve Corporation, All rights reserved. =======
//
// Purpose: 
//
//=============================================================================

#include "interface.h"
#include "UtlVector.h"
#include "utlrbtree.h"
#include "utlsymbol.h"
#include "FileSystem_Helpers.h"
#include <stdio.h>
#include <stdarg.h>
#include <assert.h>
#include <malloc.h>
#include <string.h>
#include <time.h>
#include <stdlib.h> // getenv
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <io.h>
#include <minmax.h>
#include <direct.h>

#include "packfile.h"

//-----------------------------------------------------------------------------
// Purpose: adds a list of files to the pak file
//-----------------------------------------------------------------------------
bool AddListOfFilesToPack(const char *pakfile, const char *filelist)
{
	// load in the text file
	FILE *f = fopen(filelist, "rt");
	if (!f)
	{
		printf("Could not open file '%s'\n", filelist);
		return false;
	}

	int iFileSize = _filelength(_fileno(f));
	char *pFileListText = new char[iFileSize + 1];
	fread( pFileListText, 1, iFileSize, f);
	fclose(f);
	pFileListText[iFileSize] = 0;
	char *pFileBytes = pFileListText;

	// parse out the file list
	bool bSuccess = true;
	while (1)
	{
		char token[512];
		pFileBytes = ParseFile(pFileBytes, token, NULL);
		if (!pFileBytes || !token[0])
			break;

		// strip out the path name (so any findfirst/next will work)
		char szPath[_MAX_PATH];
		strncpy(szPath, token, sizeof(szPath));
		char *slash = strrchr(szPath, '\\');
		if (slash)
		{
			slash[1] = 0;
		}

		// add in all permutations of the file
		_finddata_t finddata;
		bool bAddedFile = false;
		long findHandle = _findfirst(token, &finddata);
		if (findHandle != -1)
		{
			do
			{
				if (finddata.name[0] == '.')
					continue;

				// VSS-generated file, always ignore
				if (!stricmp(finddata.name, "vssver.scc"))
					continue;

				// add in the file
				bAddedFile = true;

				char fullPath[_MAX_PATH];
				_snprintf(fullPath, sizeof(fullPath), "%s%s", szPath, finddata.name);
				if (!Pack_AppendFile(pakfile, fullPath))
				{
					bSuccess = false;
				}
			}
			while (_findnext(findHandle, &finddata) == 0);
			_findclose(findHandle);
		}

		if (!bAddedFile)
		{
			printf("Couldn't find resource file '%s'\n", token);
			bSuccess = false;
		}
	}

	delete [] pFileListText;
	return bSuccess;
}

//-----------------------------------------------------------------------------
// Purpose: entry point
//-----------------------------------------------------------------------------
void main(int argc, char *argv[])
{
	if (argc == 2 && Pack_Validate(argv[1]))
	{
		return;
	}
	else if (argc < 3)
	{
		printf("usage:\n\t\tmakepak <pakfile> <filelist.txt> [working directory]\n");
		return;
	}

	char currentDir[_MAX_PATH];
	_getcwd(currentDir, sizeof(currentDir));

	if (argc >= 4)
	{
		// change directories
		_chdir(argv[3]);
	}

	// clear out the current pak file
	Pack_Clear(argv[1]);

	// append the file list
	bool bSuccess = true;
	bSuccess &= AddListOfFilesToPack(argv[1], argv[2]);
	// validate the file
	bSuccess &= Pack_Validate(argv[1]);

	if (bSuccess)
	{
		printf("makepak complete");
	}
	else
	{
		printf("makepak failed to create packfile %s.\n", argv[1]);
	}

	// reset the current directory
	_chdir(currentDir);
}


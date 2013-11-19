// QCParser.cpp: implementation of the QCParser class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "QCParser.h"

#include "SMDImporter.h"

extern void	XSILogMessage ( wchar_t *in_szMessage, XSI::siSeverityType in_eSeverity );

char g_fixed[1024];
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

QCParser::QCParser()
{

	
}

QCParser::~QCParser()
{

}

bool	QCParser::Import ( char *in_szFilename )
{

	char drive[1024];
	char directory[1024];

	_splitpath ( in_szFilename, drive, directory, NULL, NULL );
	
	if ( strlen(drive) > 1 )
	{
		sprintf ( drive, "%s\\%s", drive, directory );
		SetCurrentDirectory ( drive );
	}

	FILE *f = fopen ( in_szFilename, "rb" );

	if ( !f )
	{
		XSILogMessage ( L"SMDImport: Cannot open the file specified.", XSI::siErrorMsg );
		return false;
	}
	

	char *buffer = NULL;

	try 
	{
		fseek(f, 0, SEEK_END );
		long fl = ftell ( f );
		fseek (f, 0,SEEK_SET);
		buffer = new char [fl + 5];
		fread (buffer,fl,sizeof(char),f);
		
		buffer[fl] = 'E';
		buffer[fl+1] = 'N';
		buffer[fl+2] = 'D';
		buffer[fl+3] = 0;
		
		char seps[] = " \34\t\n\r";
		char *token = GetNextToken (buffer, seps);

		while (strcmp(token, "END"))
		{
			
			if ( !strcmp ( token, "studio"))
			{
				// found legacy studio model
				token = GetNextToken (NULL, seps);

				LPWSTR l_filename;
				DSA2W(&l_filename,FixFilenames(token));
				XSI::CString l_cstr = l_filename;
			
							
				m_filelist.Extend(1);
				m_filelist[m_filelist.GetUsed()-1] = l_cstr;

			}

			if ( !strcmp ( token, "$model"))
			{
				// found a hl2 model
				token = GetNextToken (NULL, seps);
				token = GetNextToken (NULL, seps);
				LPWSTR l_filename;
				DSA2W(&l_filename,FixFilenames(token));
				XSI::CString l_cstr = l_filename;
			
							
				m_filelist.Extend(1);
				m_filelist[m_filelist.GetUsed()-1] = l_cstr;

			}

			if ( !strcmp ( token, "$body"))
			{
				// found a hl2 model
				token = GetNextToken (NULL, seps);
				token = GetNextToken (NULL, seps);
				LPWSTR l_filename;
				DSA2W(&l_filename,FixFilenames(token));
				XSI::CString l_cstr = l_filename;
			
							
				m_filelist.Extend(1);
				m_filelist[m_filelist.GetUsed()-1] = l_cstr;

			}

			if ( !strcmp ( token, "$sequence"))
			{
				// found a sequence
				token = GetNextToken (NULL, seps);
				token = GetNextToken (NULL, seps);
				LPWSTR l_filename;
				DSA2W(&l_filename,FixFilenames(token));
				XSI::CString l_cstr = l_filename;
				
							
				m_filelist.Extend(1);
				m_filelist[m_filelist.GetUsed()-1] = l_cstr;

			}

			if ( !strcmp ( token, "$include"))
			{
				// found a sequence
				token = GetNextToken (NULL, seps);
				LPWSTR l_filename;
				DSA2W(&l_filename,FixFilenames(token, true));
				XSI::CString l_cstr = l_filename;
				
							
				m_includelist.Extend(1);
				m_includelist[m_includelist.GetUsed()-1] = l_cstr;

			}


			token = GetNextToken (NULL, seps);
		}
		
		//
		// import all smd in list
		//

		XSI::Application app;
				
		XSI::UIToolkit kit = app.GetUIToolkit();
		XSI::ProgressBar	m_pBar = kit.GetProgressBar();
		
		m_pBar.PutMaximum( 100 );
		m_pBar.PutMinimum( 1 );
		m_pBar.PutStep( 1 );
		m_pBar.PutValue( 1 );
		m_pBar.PutCaption( L"..." );
		m_pBar.PutStatusText( L"" );
		m_pBar.PutVisible( true );
		
		long progress_value = 0;
		XSI::X3DObject mesh;

		for (int e=0;e<m_filelist.GetUsed();e++)
		{
			progress_value = (long)(((float)e / (float)m_filelist.GetUsed()) * 100.0f);
			m_pBar.PutValue ( progress_value );					
			if ( m_pBar.IsCancelPressed() )
			{
				if ( MessageBox ( NULL, "Cancel the import of the this .QC file?.\n\n Are you sure?", "Cancel Export", MB_YESNO|MB_ICONWARNING ) == IDYES )
				{
					break;
				} else {
					m_pBar.PutVisible( true );
				}
							
			}

			XSI::CString l_cap = L"Processing ";

			LPWSTR l_zfilename;
			DSA2W(&l_zfilename,in_szFilename);
			l_cap += l_zfilename;
			l_cap += L" - Importing ";
			l_cap += m_filelist[e];
			m_pBar.PutCaption( l_cap);
			SMDImporter importer;

			char *pSzString = new char [ m_filelist[e].Length() + 1 ];
			W2AHelper ( pSzString, m_filelist[e].GetWideString() );

			importer.Import ( pSzString );

			if ( importer.m_pMesh.IsValid() )
			{
				//
				// keep a pointer on the mesh
				//
				
				mesh = importer.m_pMesh;
			} else {

				if ( mesh.IsValid() )
				{
					//
					// reset actor
					//
					
					XSI::Application app;
					XSI::CValue out;
					XSI::CValueArray args(2);
					
					
					args.Resize ( 2 );
					args[0] = mesh;
					args[1] = XSI::CValue();
					
					app.ExecuteCommand(L"ResetActor", args, out);
					
				}

			}

			
			delete [] pSzString;
			
			

		}

		

	}
	catch (...)
	{
		XSILogMessage ( L"SMDImport: Invalid or corrupted QC file.", XSI::siErrorMsg );
		return false;
	}

	fclose (f);

	delete [] buffer;

	return true;

}

char* QCParser::FixFilenames(char *in_szFilename, bool noext)
{
	memset (g_fixed, 0, 1024 ); 
	int iindex = 0;
	for (int v=0;v<strlen(in_szFilename);v++)
	{
		if ( in_szFilename[v] != 34 )
		{
			g_fixed[iindex] = in_szFilename[v];
			iindex++;
		}
	}

	if ( !noext )
	{
		char ext[256];
		_splitpath ( g_fixed, NULL, NULL, NULL, ext );

		if ( strstr ( ext, "smd"))
		{
			return g_fixed;
		}	

		sprintf ( g_fixed, "%s.smd", g_fixed);
	}

	return g_fixed;

}
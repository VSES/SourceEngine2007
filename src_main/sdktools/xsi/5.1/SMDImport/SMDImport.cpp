// MapImport.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"
#include "Commdlg.h"

#include <comdef.h>

#include "SMDImporter.h"
#include "QCParser.h"

// function declarations
void	XSILogMessage ( wchar_t *in_szMessage, XSI::siSeverityType in_eSeverity );
bool	ParseArguments(const XSI::ArgumentArray & in_arguments, char * io_filename, ULONG in_maxBytes);

char ___gTexturePathOverride[MAX_PATH];
bool g_iImportMaterials = true;

BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
	return TRUE;
}

HRESULT WINAPI XSIOnInit
( 
	HINSTANCE in_hInstModule
)
{


	return S_OK;
}

HRESULT WINAPI XSIOnTerminate()
{
	return S_OK;
}




namespace XSI
{
#ifdef unix
extern "C"
#endif

void SMDImport(char * in_exportFilename);

XSI::CStatus XSILoadPlugin( XSI::PluginRegistrar& in_reg )
{
	using namespace XSI;
	in_reg.PutAuthor( L"Softimage Corp" );
	in_reg.PutName( L"SMD Import Plug-in" );	in_reg.PutVersion( 1, 0 );

	// register the commands plugin item.
	in_reg.RegisterCommand( L"SMDImport", L"SMDImport" );
	// register property
	in_reg.RegisterProperty( L"SMDImportProperty" );

	// register the menu for the import 
	//in_reg.RegisterMenu(XSI::siMenuMainFileImportID, L"ImportSMD", false, false);
	//in_reg.RegisterMenu((XSI::siMenuAnchorPoints)1014, L"ImportSMD", false, false);

	return XSI::CStatus::OK;	
}

#ifdef unix
extern "C"
#endif
XSI::CStatus ImportSMD_Init( XSI::CRef& in_ref )
{
	Context ctxt = in_ref;
	Menu menu = ctxt.GetSource();

	CStatus st;
	MenuItem item;
	menu.AddCallbackItem(L"Import SMD...", L"OnSMDImportMenu", item);

	return CStatus::OK;	
}


#ifdef unix
extern "C"
#endif
XSI::CStatus OnSMDImportMenu( XSI::CRef& in_ref )
{
	Application app;
	CStatus st = CStatus::OK;
	CValueArray args(1);

	args[0] = L"";

	CValue ret;
	st = app.ExecuteCommand(L"SMDImport",args,ret);

	return st;
}

#ifdef unix
extern "C"
#endif
XSI::CStatus XSIUnloadPlugin( const XSI::PluginRegistrar& in_reg )
{
	Application app;
#ifdef _DEBUG
	app.LogMessage( in_reg.GetName() + L" has been unloaded.");
#endif
	return XSI::CStatus::OK;
}

#ifdef unix
extern "C"
#endif
CStatus SMDImport_Init( const XSI::CRef& in_context )
{
	Context ctx(in_context);
	Command cmd(ctx.GetSource());

	cmd.EnableReturnValue ( true );

	ArgumentArray args = cmd.GetArguments();

	args.Add( L"exportFilename" );
	args.Add( L"texturePath" );
	args.Add( L"useMaterials" );
	
	return XSI::CStatus::OK;
}

#ifdef unix
extern "C"
#endif

CStatus SMDImport_Execute( XSI::CRef& in_context )
{
	Application app;
	CStatus     st;

	// FileName
	char l_szFilename[MAX_PATH];
	memset ( l_szFilename, 0, MAX_PATH );
	memset ( ___gTexturePathOverride, 0, MAX_PATH );

	// check if we've got the necessary parameters as parameters
	Context ctxt( in_context );
	Command oCmd;
	oCmd = ctxt.GetSource();

	ArgumentArray oArgs; 
	oArgs = oCmd.GetArguments();
	if (ParseArguments(oArgs, l_szFilename, MAX_PATH) == false)
	{
		// if the parameters weren't passed in as arguments to the command then 
		// display the PPG to fill them in
		Property prop;
		prop = app.GetActiveSceneRoot().GetProperties().GetItem( L"SMDImportProperty" );
		if (!prop.IsValid())
		{
			prop = app.GetActiveSceneRoot().AddProperty( L"SMDImportProperty" ) ;
		}

		// in case we have partial parameters, fill in the values of the property page with
		// what we may or may not already have
		CString l_fileName;
		l_fileName.PutAsciiString(l_szFilename);
		prop.PutParameterValue(L"Filename", l_fileName);

		CString l_texturePath;
		l_texturePath.PutAsciiString(___gTexturePathOverride);
		prop.PutParameterValue(L"TexturePath", l_texturePath);
		prop.PutParameterValue(L"UseMaterials", CValue(g_iImportMaterials));

		CValueArray args(5);
		args[0] = prop;
		args[1] = L"";
		args[2] = L"SMDImportProperty";
		args[3] = (long)4;
		args[4] = true;
		CValue ret;

		// execute the command to display the PPG
		st = app.ExecuteCommand(L"InspectObj",args,ret);
		if ( CStatus::OK == st ) {
			// filename to export to
			Parameter parm = prop.GetParameters().GetItem(L"Filename" );
			CString	str = parm.GetValue();
			const wchar_t	* p = str.GetWideString();
			wcstombs( l_szFilename, p, wcslen (p));

			// path to the texture
			parm = prop.GetParameters().GetItem(L"TexturePath" );
			str = parm.GetValue();
			p = str.GetWideString();
			wcstombs( ___gTexturePathOverride, p, wcslen (p));

			// bool 
			parm = prop.GetParameters().GetItem(L"UseMaterials");
			g_iImportMaterials = (bool) parm.GetValue();
		}
		else
		{
			// implies a cancel from the PPG
			return CStatus::OK;
		}
	}

	SMDImport(l_szFilename);
	return CStatus::OK;
}

#ifdef unix
extern "C"
#endif
CStatus SMDImportProperty_Define( const CRef & in_Ctx )
{
	Application app ;
	CustomProperty prop = Context(in_Ctx).GetSource() ;
	Parameter param ;

	// Default capabilities for most of these parameters
	int caps = siPersistable  ;
	CValue dft ;	// Used for arguments we don't want to set

	prop.AddParameter(	L"Filename",CValue::siString, caps, 
						L"Filename", L"", 
						dft, param ) ;
	
	prop.AddParameter(	L"TexturePath",CValue::siString, caps, 
						L"Texture path", L"", 
						dft, param ) ;	

	dft = true;

	prop.AddParameter(	L"UseMaterials",CValue::siBool, caps, 
						L"Import Materials", L"", 
						dft, param ) ;	

	return CStatus::OK;	
}

#ifdef unix
extern "C"
#endif
CStatus SMDImportProperty_DefineLayout( const CRef & in_Ctx )
{
	PPGLayout oLayout = Context( in_Ctx ).GetSource() ;
	
	PPGItem item ;
	
	oLayout.Clear() ;
	
	
	item = oLayout.AddItem( L"SMDImportProperty" ) ;
	
	oLayout.AddGroup(L"File") ;
		oLayout.AddRow() ;
			item = oLayout.AddItem( L"Filename",L"Import File",siControlFilePath ) ;
			item.PutAttribute( siUIFileMustExist, true ) ;
			item.PutAttribute( siUIOpenFile, true ) ;
			item.PutAttribute( siUIFileFilter, L"Valve SMD Files (*.smd)|*.smd|Vertex Animation Files (*.vta)|*.vta|Valve .QC file (*.qc)|*.qc|All Files (*.*)|*.*||" ) ;
		oLayout.EndRow() ;

		oLayout.AddRow() ;
			item = oLayout.AddItem( L"TexturePath",L"Texture Path",siControlFolder ) ;
			item.PutAttribute( siUIInitialDir, L"user" ) ;
		oLayout.EndRow() ;
	oLayout.EndGroup() ;
	
	oLayout.AddGroup( L"Options" ) ;
		oLayout.AddRow();
		oLayout.AddRow();
			item = oLayout.AddItem( L"UseMaterials" ) ;
		oLayout.EndRow();
	oLayout.EndGroup() ;

	return CStatus::OK;	
}

void SMDImport(char * in_filename)
{
	Application app;

	CStatus	st;

	if (in_filename && strlen( in_filename ) > 0)
	{
		//
		// SMD or QC?
		//

		char ext[256];
		_splitpath ( in_filename, NULL, NULL, NULL, ext );

		if ( strstr ( ext, "smd"))
		{
			SMDImporter importer;
			importer.Import ( in_filename );
		} 

		if ( strstr ( ext, "vta"))
		{
			XSILogMessage ( L"didn't have time to implement support for VTAs yet... sorry ", XSI::siErrorMsg );
		} 

		if ( strstr ( ext, "qc"))
		{
			QCParser parser;
			parser.Import ( in_filename );

			//
			// import include
			//
			for (int i=0; i<parser.m_includelist.GetUsed(); i++)
			{
				char *pSzString = new char [ parser.m_includelist[i].Length() + 1 ];
				W2AHelper ( pSzString, parser.m_includelist[i].GetWideString() );
				
				QCParser parser2;
				parser2.Import ( pSzString );
		
				delete [] pSzString;
			}
		} 
	}
}
} // using namespace XSI


void	XSILogMessage ( wchar_t *in_szMessage, XSI::siSeverityType in_eSeverity )
{
	XSI::Application	app;

	app.LogMessage( in_szMessage, in_eSeverity ) ;
}


bool ParseArguments(const XSI::ArgumentArray & in_arguments, char * io_filename, ULONG in_maxBytes)
{
	XSI::CValue Arg0 = in_arguments[0].GetValue();
	XSI::CValue Arg1 = in_arguments[1].GetValue();
	XSI::CValue Arg2 = in_arguments[2].GetValue();

	// get the filename for export
	if ( Arg0.m_t != XSI::CValue::siString )
	{
		return false;
	}
	else
	{
		XSI::CString	str = Arg0;
		const wchar_t	* p = str.GetWideString();
		wcstombs( io_filename, p, wcslen (p));
	}

	// get the texture path for export
	if ( Arg1.m_t != XSI::CValue::siString )
	{
		// set it to a blank string
		___gTexturePathOverride[0] = '\0';
	}
	else
	{
		XSI::CString	str = Arg1;
		const wchar_t	* p = str.GetWideString();
		wcstombs( ___gTexturePathOverride, p, wcslen (p));
	}

	// get the boolean for usematerials
	if ( Arg2.m_t != XSI::CValue::siBool )
	{
		g_iImportMaterials = true;
	}
	else
	{
		g_iImportMaterials = (bool) Arg2;
	}

	return true;
}

// MapImport.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"
#include "VMFParser.h"
#include "Commdlg.h"

#include <comdef.h>

#include <si3dobjectmodel_id.c>		// IID_SIObject
#include <xsisdk.h>
#include <xsi_plugin.h>
#include <xsi_pluginitem.h>
#include <xsi_pluginregistrar.h>
#include <xsi_status.h>
#include <xsi_application.h>
#include <xsi_value.h>
#include <xsi_context.h>
#include <xsi_menuitem.h>
#include <xsi_menu.h>
#include <xsi_command.h>
#include <xsi_argument.h>
#include <xsi_ppglayout.h>
#include <xsi_ppgeventcontext.h>


HINSTANCE	___gParent = NULL;
void	ImportDotXSI();
void GetXSIParam ( SI_Int* out_p,  LPVARIANT in_p );
void	XSILogMessage ( wchar_t *in_szMessage, XSI::siSeverityType in_eSeverity );


char	___gTexturePath[256];
char	___gTexturePathOverride[256];
int		g_iImportMaterials = 1;

BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
	//
	// Initialize the texture path
	//

	//
	// First build the path to the INI file
	//

	char l_szIniFile[MAX_PATH];
	char l_szModule[MAX_PATH];
	char l_szDrive[MAX_PATH];
	char l_szDirectory[MAX_PATH];

	GetModuleFileName ( (HMODULE)hModule, l_szModule, MAX_PATH );

	_splitpath ( l_szModule, l_szDrive, l_szDirectory, NULL, NULL );
	sprintf ( l_szIniFile, "%s%sVMFImport.ini",l_szDrive,l_szDirectory );

	___gTexturePath[0] = 0;
	GetPrivateProfileString( "VMFImport","TexturePath","C:\\",___gTexturePath,256,l_szIniFile );

	if ( ___gTexturePath[ strlen(___gTexturePath) ] != '\\' )
	{
		strcat ( ___gTexturePath,"\\" );

	}


	return TRUE;
}

HRESULT WINAPI XSIOnInit
( 
	HINSTANCE in_hInstModule
)
{

	___gParent = in_hInstModule;

	return S_OK;
}

HRESULT WINAPI XSIOnTerminate()
{
	return S_OK;
}

// handler function
HRESULT WINAPI XSIOnCommand
( 
	LPCWSTR		in_pszName, 
	LPVARIANT	in_pArgs, 
	ULONG		in_ulCount,
	LPVARIANT	out_pReturnVar
)
{
	if ( V_VT(&in_pArgs[0]) != VT_BSTR ) return E_INVALIDARG;
	if ( V_VT(&in_pArgs[1]) != VT_BSTR ) return E_INVALIDARG;
	if ( V_VT(&in_pArgs[2]) != VT_I2 ) return E_INVALIDARG;

  	char l_szFilename[MAX_PATH];

	memset ( l_szFilename,0,MAX_PATH );
	wcstombs( l_szFilename, in_pArgs[0].bstrVal, wcslen (in_pArgs[0].bstrVal));

	memset ( ___gTexturePathOverride,0,MAX_PATH );
	wcstombs( ___gTexturePathOverride, in_pArgs[1].bstrVal, wcslen (in_pArgs[1].bstrVal));

	GetXSIParam ( &g_iImportMaterials, &in_pArgs[2] );

	CMapParser	p;
	p.Read ( l_szFilename );
	p.ConvertToSemanticLayer();

	return S_OK;
}

// event handler function
HRESULT WINAPI XSIOnEvent
( 
	LONG		in_lEventID, 
	LPVARIANT	in_pVar, 
	LONG		in_lNum
)
{
 	return S_OK;
}

void ImportDotXSI()
{

	CComPtr<XSIApplication>						l_pApplication;
	CLSID	lclsid;
	CLSIDFromProgID(L"XSI.Application", &lclsid );
	CoCreateInstance(lclsid, NULL, CLSCTX_INPROC, IID_XSIApplication, (void **)&l_pApplication);

	_variant_t args;  
	_variant_t v;  
	long l = 0 ;

	SAFEARRAY* psa = NULL;  
 
	const int cntArgs = 1 ;

	psa = ::SafeArrayCreateVector( VT_VARIANT, 0, cntArgs );  
 
	args.vt = VT_VARIANT | VT_ARRAY;  
	args.parray = psa;
 
	v = _bstr_t ( "c:\\__tmp.xsi" );
 
	SafeArrayPutElement(psa, &l, &v);  
	l++;  
 
	// more args?
	//

	VARIANT	l_vOutValue;
	l_pApplication->ExecuteScriptCommand( _bstr_t( L"SIImportDotXSIFile" ), args, &l_vOutValue ) ;


}

void GetXSIParam ( SI_Int* out_p,  LPVARIANT in_p )
{
	
	if ( in_p->vt == 2 )
	{
		
		*out_p = in_p->iVal;
	} else {
		*out_p = in_p->intVal;
	}
	
}



namespace XSI
{
#ifdef unix
extern "C"
#endif

void VMFImport();

XSIPLUGINCALLBACK XSI::CStatus XSILoadPlugin( XSI::PluginRegistrar& in_reg )
{
	using namespace XSI;
	in_reg.PutAuthor( L"Softimage Corp" );
	in_reg.PutName( L"VMF Import Plug-in" );	in_reg.PutVersion( 1, 0 );

	// register the commands plugin item.
	in_reg.RegisterCommand( L"VMFImport", L"VMFImport" );
	// register property
	in_reg.RegisterProperty( L"VMFImportProperty" );	//
	// register the menu for the import 
//	in_reg.RegisterMenu(XSI::siMenuMainFileImportID, L"CPPImportVMF", false, false);
//	in_reg.RegisterMenu((XSI::siMenuAnchorPoints)1014, L"CPPImportVMF", false, false);

	return XSI::CStatus::OK;	
}

#ifdef unix
extern "C"
#endif
XSI::CStatus CPPImportVMF_Init( XSI::CRef& in_ref )
{
	Context ctxt = in_ref;
	Menu menu = ctxt.GetSource();

	CStatus st;
	MenuItem item;
	menu.AddCallbackItem(L"Import VMF...", L"OnVMFImportMenu", item);

	return CStatus::OK;	
}


#ifdef unix
extern "C"
#endif
XSI::CStatus OnVMFImportMenu( XSI::CRef& in_ref )
{
	VMFImport();
	return CStatus::OK;
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
XSIPLUGINCALLBACK XSI::CStatus VMFImport_Init( const XSI::CRef& in_context )
{
	Context ctx(in_context);
	Command cmd(ctx.GetSource());

	cmd.EnableReturnValue ( true );


	ArgumentArray args = cmd.GetArguments();

	args.Add( L"arg0", (long)0 );
	
	return XSI::CStatus::OK;
}



#ifdef unix
extern "C"
#endif
XSIPLUGINCALLBACK XSI::CStatus VMFImport_Execute( XSI::CRef& in_context )
{
	VMFImport();
	return CStatus::OK;
}

#ifdef unix
extern "C"
#endif
XSIPLUGINCALLBACK XSI::CStatus VMFImportProperty_Define( const CRef & in_Ctx )
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
XSIPLUGINCALLBACK CStatus VMFImportProperty_DefineLayout( const CRef & in_Ctx )
{
	PPGLayout oLayout = Context( in_Ctx ).GetSource() ;
	
	PPGItem item ;
	
	oLayout.Clear() ;
	
	
	item = oLayout.AddItem( L"VMFImportProperty" ) ;
	
	oLayout.AddGroup(L"File") ;
	
	oLayout.AddRow() ;
	item = oLayout.AddItem( L"Filename",L"Import File",siControlFilePath ) ;
	item.PutAttribute( siUIFileMustExist, true ) ;
	item.PutAttribute( siUIOpenFile, true ) ;
	item.PutAttribute( siUIFileFilter, L"Valve VMF Files (*.vmf)|*.vmf|All Files (*.*)|*.*||" ) ;
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

void VMFImport()
{

	Application app;

	CStatus	st;

	Property prop;
	prop = app.GetActiveSceneRoot().GetProperties().GetItem( L"VMFImportProperty" );
	if (!prop.IsValid())
		prop = app.GetActiveSceneRoot().AddProperty( L"VMFImportProperty" ) ;

	CValueArray args(5);
	args[0] = prop;
	args[1] = L"";
	args[2] = L"VMFImportProperty";
	args[3] = (long)4;
	args[4] = true;
	CValue ret;
	st = app.ExecuteCommand(L"InspectObj",args,ret);
	if ( CStatus::OK == st ) {
		//
		// FileName
		//
		char l_szFilename[MAX_PATH];
		memset ( l_szFilename,0,MAX_PATH );

		Parameter parm = prop.GetParameters().GetItem(L"Filename" );
		CString	str = parm.GetValue();
		const wchar_t	* p = str.GetWideString();
		wcstombs( l_szFilename, p, wcslen (p));

		parm = prop.GetParameters().GetItem(L"TexturePath" );
		str = parm.GetValue();
		p = str.GetWideString();
		wcstombs( ___gTexturePathOverride, p, wcslen (p));

		

		//
		// Bools
		//
		parm = prop.GetParameters().GetItem(L"UseMaterials");
		g_iImportMaterials = (bool) parm.GetValue();

		if (l_szFilename && strlen( l_szFilename ) > 0)
		{
			CMapParser	p;
			p.Read ( l_szFilename );
			p.ConvertToSemanticLayer();

		}
		else
		{
			XSILogMessage ( L"Error - Invalid file specified", XSI::siErrorMsg );

		}

	}
}
} // using namespace XSI


void	XSILogMessage ( wchar_t *in_szMessage, XSI::siSeverityType in_eSeverity )
{
	XSI::Application	app;


	app.LogMessage( in_szMessage, in_eSeverity ) ;


}

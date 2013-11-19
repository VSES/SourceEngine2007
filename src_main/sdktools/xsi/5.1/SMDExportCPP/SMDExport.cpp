// SMDExport.cpp : Defines the entry point for the DLL application.
//
#include "stdafx.h"
#include <stdio.h>
#include "Commdlg.h"
#include <string.h>
#include <io.h>

#include <comdef.h>					// _variant_t
#include <si3dobjectmodel_id.c>		// IID_SIObject
#include <siobjectmodel_id.c>		// IID_Application
#include <xsisdk.h>
#include <xsi_transformation.h>
#include <xsi_vector3.h>
#include <xsi_status.h>
#include <xsi_application.h>
#include <xsi_plugin.h>
#include <xsi_pluginitem.h>
#include <xsi_pluginregistrar.h>
#include <xsi_value.h>
#include <xsi_status.h>
#include <xsi_application.h>
#include <xsi_plugin.h>
#include <xsi_pluginitem.h>
#include <xsi_pluginregistrar.h>
#include <xsi_pluginitem.h>
#include <xsi_command.h>
#include <xsi_argument.h>
#include <xsi_context.h>
#include <xsi_menuitem.h>
#include <xsi_menu.h>
#include <xsi_customproperty.h>
#include <xsi_ppglayout.h>
#include <xsi_ppgeventcontext.h>
#include <xsi_selection.h>
#include <xsi_comapihandler.h>
#include <xsi_model.h>
#include <xsi_decl.h>
#include <xsi_userdatablob.h>
#include <xsi_parameter.h>
#include <xsi_envelope.h>
#include <xsi_project.h>
#include <xsi_kinematicstate.h>
#include <xsi_primitive.h>
#include <xsi_geometry.h>
#include <xsi_polygonmesh.h>
#include <xsi_application.h>
#include <xsi_uitoolkit.h>
#include <xsi_progressbar.h>
#include <xsi_point.h>
#include <xsi_command.h>

#include "resource.h"

#include "SMDNode.h"
#include "SMDSkeleton.h"
#include "SMDTriangle.h"
#include "VertexAnimation.h"

#include <xsi_scene.h>


CSIBCArray<CachedObjectXfo>	g_ResetTranforms;

extern void W2AHelper( LPSTR out_sz, LPCWSTR in_wcs, int in_cch );

extern bool __declspec(dllimport) ValidateRegistrationCurrentKey( wchar_t * );
extern bool __declspec(dllimport) ValidateRegistrationNewKey( wchar_t *, wchar_t * web_path,wchar_t * welcome_text, wchar_t * title_text );
void GetPositionAndRotationAtFrame ( XSI::Kinematics l_pTransform, CSIBCVector3D& l_vPosition, CSIBCVector3D& l_vRotation, int k );

void	FixSymetry ( CSIBCArray<XSI::CRef>* in_pModelList );

bool	IsCommandInstalled( const char* in_szCmdName );
bool IsSelected( XSI::X3DObject in_obj );
bool IsPartOfABranchSelection( XSI::ProjectItem in_obj );

using namespace XSI;
using namespace MATH;

HINSTANCE	___gParent = NULL;
HINSTANCE	___gThis = NULL;

char	ROOT_TOKEN[256];
char	EFF_TOKEN[256];


void A2WHelper( wchar_t* out_wcs, const char* in_sz, int in_cch = -1 );



#ifdef A2W
#undef A2W
#endif
#define A2W(out_wcs, in_sz) \
	if (NULL == (const char *)(in_sz)) \
		*(out_wcs) = NULL; \
	else \
	{ \
		*(out_wcs) = (wchar_t*)alloca((strlen((in_sz)) + 1) * sizeof(wchar_t)); \
		A2WHelper(*(out_wcs), (in_sz)); \
	}

void A2WHelper( wchar_t* out_wcs, const char* in_sz, int in_cch )
{
	if ( out_wcs != NULL && 0 != in_cch )
	{
		out_wcs[0] = L'\0' ;

		if ( in_sz != NULL )
		{
			size_t l_iLen = 0;
			l_iLen = ::mbstowcs( out_wcs, in_sz, ( in_cch < 0 ) ? ::strlen(in_sz) + 1 : (size_t) in_cch ) ;
			assert( (int)l_iLen != -1 );

        		if ( in_cch > 0 ) { out_wcs[ in_cch - 1 ] = L'\0'; }
		}
	}
}

HRESULT	ExportDotXSI( char * in_szString );
bool IsRootOrEffector ( XSI::X3DObject );

/** 
* SMDExport module
*
* This is the main module. It handles communication with XSI,
* reads user options and builds the relevant data for either the
* .SMD or .VTA output.
*
*/

/*!	Converts a dotXSI file to a SMD or VTA file.
	\param	SI_Char*	Path to the dotXSI file to convert
	\return	nothing
*/
void	ConvertdotXSI ( SI_Char *in_szFilename );

/*!	Triggers a SetValue in XSI
	\param	SI_Char*	Path to the XSI parameter (mymodel.kine.local.posx)
	\param	long		Value
	\return	nothing
*/
void	SetXSIValue ( char *, long );

/*!	Logs a message in the XSI Script window
	\param	SI_Char*	Message
	\param	siSeverity	Severity code (siFatal, siError, siWarning, siInfo)(
	\return	nothing
*/
void	XSILogMessage ( char *in_szMessage, XSI::siSeverityType in_eSeverity );

/*!	prints comments at the head of the .smd file
	\param	FILE*	file that is being written to
	\return	nothing
*/
void	AddSMDComments (FILE* in_printfile);

/*!	Exports the skeleton data. The skeleton data is actually the animation key frames for all bones
	that affects an envelope.
	\param	CSLModel*		The bone's Semantic Layer Model object
	\param	SMDSkeleton*	A SMDSkeleton object that will hold the bone's keyframes
	\param	SMDNodeList*	A SMDNodeList object that hold a list of bones
	\return	nothing
*/
void	ExportSkeletonData ( XSI::X3DObject in_pModel, SMDSkeleton* in_pSkeleton, SMDNodeList* in_pNodeList  );
void	ExportSkeletonData ( CSIBCArray<XSI::X3DObject>& in_pModelList, SMDSkeleton* in_pSkeleton, SMDNodeList* in_pNodeList  );

/*!	Exports the triangle data. The triangle data is the topographical information for the enveloped mesh
	\param	CSLEnvelope*	A pointer to a Semantic Layer Envelope object
	\param	SMDTriangle*	A pointer to a SMDTriangle object. This will hold the triangle information
	\param	SMDNodeList*	A SMDNodeList object that hold a list of bones
	\return	nothing
*/
void ExportTriangleData ( XSI::Envelope in_pEnvelope, XSI::X3DObject in_pBone, SMDTriangle *in_pTriangle, SMDNodeList* in_pNodes );

/*!	Exports the triangle data. The triangle data is the topographical information for the enveloped mesh
	\param	CSLModel*		A pointer to a Semantic Layer model object
	\param	SMDTriangle*	A pointer to a SMDTriangle object. This will hold the triangle information
	\param	SMDNodeList*	A SMDNodeList object that hold a list of bones
	\return	nothing
*/
void ExportTriangleData ( XSI::X3DObject in_pModel, SMDTriangle *in_pTriangle, SMDNodeList* in_pNodes );


LRESULT CALLBACK OptionDlg(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

/*!	Given a CSLModel, this function will recurse into the model's hierarchy and try to find a ShapeAnimation object.
	\param	CSLModel*		Top level Semantic Layer Model object
	\return	CSLShapeAnimation*	A pointer to a Semantic Layer ShapeAnimation object
*/


/*!	Gets the minimum and maximum key times for a single FCurve
	\param	CSLBaseFCurve*		The FCurve
	\param	SI_Int&				Minimum
	\param	SI_Int&				Maximum
	\return	nothing
*/




void	ExportEntireScene ( SMDTriangle* in_pTriangles, SMDNodeList* in_pNodes, SMDSkeleton* in_pSkeleton);

void	BuildModelArray (XSI::CRefArray in_RefArray);






void	GetPrimitives ( CSIBCArray < XSI::X3DObject >& in_pPrimitiveList, BOOL in_bNeedUV = FALSE );

/** 
* SMD Types.
*/
#define SMD_MODEL				0	//* Reference SMD
#define SMD_PHYSBOX				1	//* Physbox SMD
#define SMD_SKELETAL_ANIMATION	2	//* Animation SMD
#define SMD_VERTEX_ANIMATION	3	//* VertexAnimation .VTA file

int SMDType = SMD_SKELETAL_ANIMATION;
bool ExportSelectedOnly = false;
bool ZeroRootTransforms = false;
int HRCActive = 0;
int	RemoveCharacterRigRoot = 0;
int	RemoveUnusedBones = 1;
CSIBCArray<XSI::CRef>	g_pModelArray;
CSIBCArray<XSI::CRef>	g_pEnvelopeArray;

char	g_szFilename[MAX_PATH];

namespace XSI
{
#ifdef unix
extern "C"
#endif
XSIPLUGINCALLBACK XSI::CStatus XSILoadPlugin( XSI::PluginRegistrar& in_reg )
{
	in_reg.PutAuthor( L"Softimage Corp" );
	in_reg.PutName( L"SMD Export Plug-in 2" );	in_reg.PutVersion( 1, 0 );

	// register the commands plugin item.
	in_reg.RegisterCommand( L"SMDExport2", L"SMDExport2" );
	in_reg.RegisterCommand( L"SMDExportBatch", L"SMDExportBatch" );

	// register property
	in_reg.RegisterProperty( L"SMDExportProperty2" );	//
	// register the menu for the import and export
	in_reg.RegisterMenu(siMenuMainFileExportID, L"CPPExportSMD", false, false);

	return XSI::CStatus::OK;	
}

#ifdef unix
extern "C"
#endif
XSIPLUGINCALLBACK XSI::CStatus CPPExportSMD_Init( XSI::CRef& in_ref )
{
	Context ctxt = in_ref;
	Menu menu = ctxt.GetSource();

	CStatus st;
	MenuItem item;
	menu.AddCallbackItem(L"Export SMD...", L"OnSMDExportMenu", item);

	return CStatus::OK;	
}


void	SMDExport();


#ifdef unix
extern "C"
#endif
XSIPLUGINCALLBACK XSI::CStatus OnSMDExportMenu( XSI::CRef& in_ref )
{
	
	XSI::CValueArray args(1);
	args[0] = XSI::CValue (0L);;
	XSI::CValue ret;
	XSI::Application app;
	XSI::CStatus st = app.ExecuteCommand(L"SMDExport2",args,ret);

	return st;
}

#ifdef unix
extern "C"
#endif
XSIPLUGINCALLBACK XSI::CStatus XSIUnloadPlugin( const XSI::PluginRegistrar& in_reg )
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
XSIPLUGINCALLBACK XSI::CStatus SMDExport2_Init( const XSI::CRef& in_context )
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
XSIPLUGINCALLBACK XSI::CStatus SMDExportBatch_Init( const XSI::CRef& in_context )
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
XSIPLUGINCALLBACK XSI::CStatus SMDExport2_Execute( XSI::CRef& in_context )
{

	Application app;
	CStatus	st;
	Property prop;
	prop = app.GetActiveSceneRoot().GetProperties().GetItem( L"SMDExportProperty2" );
	if (!prop.IsValid())
		prop = app.GetActiveSceneRoot().AddProperty( L"SMDExportProperty2" ) ;

	CValueArray args(5);
	args[0] = prop;
	args[1] = L"";
	args[2] = L"SMDExportProperty2";
	args[3] = (long)4;
	args[4] = true;
	CValue ret;
	st = app.ExecuteCommand(L"InspectObj",args,ret);

	if ( st == XSI::CStatus::OK )
	{
		SMDExport();
	}

	for (int r=0;r<g_ResetTranforms.GetUsed();r++)
	{
		g_ResetTranforms[r].m_kine.GetLocal().PutTransform(g_ResetTranforms[r].m_xfo);
	}
	g_ResetTranforms.DisposeData ();

	return CStatus::OK;
}

#ifdef unix
extern "C"
#endif
XSIPLUGINCALLBACK XSI::CStatus SMDExportBatch_Execute( XSI::CRef& in_context )
{
	SMDExport();
	return CStatus::OK;
}



void	SMDExport()
{

	Application app;


//#endif

	CStatus	st;

	Property prop;
	prop = app.GetActiveSceneRoot().GetProperties().GetItem( L"SMDExportProperty2" );
	if (!prop.IsValid())
		return;
	/*
		prop = app.GetActiveSceneRoot().AddProperty( L"SMDExportProperty2" ) ;

	CValueArray args(5);
	args[0] = prop;
	args[1] = L"";
	args[2] = L"SMDExportProperty2";
	args[3] = (long)4;
	args[4] = true;
	CValue ret;
	st = app.ExecuteCommand(L"InspectObj",args,ret);
	*/
	if ( 1 ) {
		//
		// FileName
		//
		memset ( g_szFilename,0, sizeof (char) * MAX_PATH );

		Parameter parm = prop.GetParameters().GetItem(L"Filename" );
		CString	str = parm.GetValue();
		const wchar_t	* p = str.GetWideString();
		wcstombs( g_szFilename, p, wcslen (p));
		//
		// Bools
		//
		parm = prop.GetParameters().GetItem(L"HRC");
		HRCActive = (bool) parm.GetValue();

		parm = prop.GetParameters().GetItem(L"RemoveBigRoot");
		RemoveCharacterRigRoot = (bool) parm.GetValue();

		parm = prop.GetParameters().GetItem(L"RemoveUnusedBones");
		RemoveUnusedBones = (bool) parm.GetValue();
		//
		// Ints
		//
		parm = prop.GetParameters().GetItem(L"SMDType");
		SMDType = (long) parm.GetValue();

		parm = prop.GetParameters().GetItem(L"SelectedOnly");
//		ExportSelectedOnly = (long) parm.GetValue();
		ExportSelectedOnly = (bool) parm.GetValue();

		parm = prop.GetParameters().GetItem(L"ZeroRootTransforms");
//		ZeroRootTransforms = (long) parm.GetValue();
		ZeroRootTransforms = (bool) parm.GetValue();

		 
		//
		// Now call what we need
		//
		if (g_szFilename && strlen( g_szFilename ) > 0)
		{
			//
			// test for readonly
			//

			if ( _access(g_szFilename,0) == 0 && _access(g_szFilename,2) == -1 )
			{
				if ( IsCommandInstalled("vstcheckoutfile") )
				{
					int answer = MessageBox(NULL, 
											"The file you are trying to overwrite is checked in source control.\n\nDo you want to check it out now?", 
											"Checkout file", 
											MB_YESNO|MB_ICONQUESTION);
					
					if ( answer == IDYES )
					{
						XSI::CValueArray args(1);
						XSI::CString fname;
						fname.PutAsciiString (g_szFilename);
						args[0] = CValue(fname);
						XSI::CValue retVal;
						app.ExecuteCommand( L"vstcheckoutfile", args, retVal );
						ExportDotXSI( g_szFilename );
					}

				} else {
					XSILogMessage ( "Error - Target file is readonly", XSI::siErrorMsg );
				}

			} else {

				// write location is valid
				ExportDotXSI( g_szFilename );
			}
		}
		else
		{
			XSILogMessage ( "Error - Invalid file specified", XSI::siErrorMsg );

		}
	}
}

#ifdef unix
extern "C"
#endif
XSIPLUGINCALLBACK XSI::CStatus SMDExportProperty2_Define( const CRef & in_Ctx )
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
	
	dft = true;

	prop.AddParameter(	L"HRC",CValue::siBool, caps, 
						L"Optimize Chains and Compensate", L"", 
						dft, param ) ;	

	dft = true;

	prop.AddParameter(	L"RemoveBigRoot",CValue::siBool, caps, 
						L"Remove Top-Level Bone on Character Rig", L"", 
						dft, param ) ;	

	dft = true;

	prop.AddParameter(	L"RemoveUnusedBones",CValue::siBool, caps, 
						L"Remove all unused bones", L"", 
						dft, param ) ;
	
	CValue dft2 ;

	prop.AddParameter(	L"SMDType",CValue::siUInt4 , caps, 
						L"File Type", L"", 
						dft2, param ) ;	

	
	dft = false;

	prop.AddParameter(	L"SelectedOnly",CValue::siBool, caps, 
						L"Export Selection Only", L"", 
						dft, param ) ;

	dft = false;

	prop.AddParameter(	L"ZeroRootTransforms",CValue::siBool, siPersistable|siReadOnly, 
						L"Zero Root Transforms", L"", 
						dft, param ) ;

	
	return CStatus::OK;	
}



inline void XSIA2WHelper( wchar_t* out_wcs, const char* in_sz, int in_cch = -1 )
{
	if ( out_wcs != NULL && 0 != in_cch )
	{
		out_wcs[0] = L'\0' ;

		if ( in_sz != NULL )
		{
			// Right now, we can't really test the return value because mbstowcs() is
			// mapped to strcpy() on the Irix o32 build. When this build is not done
			// anymore, the != 0 test should be updated.

			size_t l_iLen = 0;
			l_iLen = ::mbstowcs( out_wcs, in_sz, ( in_cch < 0 ) ? ::strlen(in_sz) + 1 : (size_t) in_cch ) ;
			assert( (int)l_iLen != -1 );

        		if ( in_cch > 0 ) { out_wcs[ in_cch - 1 ] = L'\0'; }
		}
	}
}



#define XSIA2W(out_wcs, in_sz) \
	if (NULL == (const char *)(in_sz)) \
		*(out_wcs) = NULL; \
	else \
	{ \
		*(out_wcs) = (wchar_t*)alloca((strlen((in_sz)) + 1) * sizeof(wchar_t)); \
		XSIA2WHelper(*(out_wcs), (in_sz)); \
	}

CStatus	RebuildLayout( PPGLayout & oLayout );

#ifdef unix
extern "C"
#endif
XSIPLUGINCALLBACK XSI::CStatus SMDExportProperty2_DefineLayout( const CRef & in_Ctx )
{
	PPGLayout oLayout = Context( in_Ctx ).GetSource() ;

	

	return RebuildLayout( oLayout );
}

CStatus	RebuildLayout( PPGLayout & oLayout )
{
	PPGItem item ;

	oLayout.Clear() ;


	item = oLayout.AddItem( L"SMDExportProperty2" ) ;

	oLayout.AddGroup(L"File") ;
		oLayout.AddRow() ;
				
		item = oLayout.AddItem( L"Filename",L"Export File",siControlFilePath ) ;
			item.PutAttribute( siUINoLabel, true ) ;
			item.PutAttribute( siUIFileFilter, L"Valve SMD Files (*.smd)|*.smd|Vertex Animation Files (*.vta)|*.vta|All Files (*.*)|*.*||" ) ;
		oLayout.EndRow() ;

		// add a button that will correct the smd export path based on the perforce client mapping of the XSI scene
		// some people have U:\dev\main, others have c:\main, etc.
		oLayout.AddSpacer(0, 3);
		oLayout.AddRow() ;
		item = oLayout.AddItem(L"smdPathFromProject", L"Set SMD Path to Current Project", siControlButton);
		item = oLayout.AddItem(L"smdPathFromProjectScene", L"Set SMD Path to Current Project and Scene", siControlButton);
		oLayout.EndRow() ;

	oLayout.EndGroup() ;

	oLayout.AddGroup( L"Options" ) ;
		oLayout.AddRow();
			CValueArray comboItems( 8 ) ;
			comboItems[0] = L"Model (.SMD)";  comboItems[1] = CValue(0L); ;
			comboItems[2] = L"Physbox (.SMD)"; comboItems[3] = CValue(1L) ;
			comboItems[4] = L"Skeletal Animation (.SMD)"; comboItems[5] = CValue(2L);
			comboItems[6] = L"Vertex Animation (.VTA)"; comboItems[7] = CValue(3L);
			item = oLayout.AddEnumControl( L"SMDType", comboItems, L"", siControlCombo ) ;
		oLayout.EndRow();
		oLayout.AddRow();
			item = oLayout.AddItem( L"HRC" ) ;
		oLayout.EndRow();
		oLayout.AddRow();
			item = oLayout.AddItem( L"RemoveBigRoot" );
		oLayout.EndRow();
		oLayout.AddRow();
			item = oLayout.AddItem( L"RemoveUnusedBones" );
		oLayout.EndRow();
		
		oLayout.AddSpacer() ;
		oLayout.AddRow();
			item = oLayout.AddItem( L"SelectedOnly" );
		oLayout.EndRow();
		oLayout.AddRow();
			item = oLayout.AddItem( L"ZeroRootTransforms" );
			item.PutAttribute( siUICX, (LONG)50 ) ;
		oLayout.EndRow();

	oLayout.EndGroup() ;

#ifdef _DEBUG
	oLayout.AddStaticText( L"USING DEBUG BUILD" ) ;
#endif
	return CStatus::OK;	
}


};

XSIPLUGINCALLBACK CStatus SMDExportProperty2_PPGEvent( const CRef& io_Ctx )
{
	XSI::Application app ;
	XSI::PPGEventContext ctx( io_Ctx ) ;

	XSI::PPGEventContext::PPGEvent eventID = ctx.GetEventID() ;
	if ( eventID == XSI::PPGEventContext::siButtonClicked )
	{
		XSI::CValue buttonPressed = ctx.GetAttribute( L"Button" ) ;	
		
		if ( buttonPressed.GetAsText() == L"smdPathFromProject" )
		{
			XSI::CValueArray args(1);
			args[0] = bool(false);
			XSI::CValue retVal;

			app.ExecuteCommand( L"smdPathFromProject", args, retVal) ;
		}
		else if ( buttonPressed.GetAsText() == L"smdPathFromProjectScene" )
		{
			XSI::CValueArray args(1);
			args[0] = bool(true);
			XSI::CValue retVal;

			app.ExecuteCommand( L"smdPathFromProject", args, retVal) ;
		}
	}

	XSI::Parameter changed = ctx.GetSource() ;
	XSI::CustomProperty prop = changed.GetParent() ;	

	XSI::Parameter ZeroXfo = prop.GetParameters().GetItem( L"ZeroRootTransforms" ) ;

	if ( eventID == PPGEventContext::siOnInit )
	{
			ZeroXfo.PutCapabilityFlag( XSI::siReadOnly, true );
	} else {
		ZeroXfo.PutCapabilityFlag( XSI::siReadOnly, !((bool)prop.GetParameterValue( L"SelectedOnly" )) );
	}
	return CStatus::OK;	
}

BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
	___gThis = (HINSTANCE)hModule;

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
	
	//
	// Get a file to export
	//
	
	if ( V_VT(&in_pArgs[0]) != VT_BSTR ) return E_INVALIDARG;
	if ( V_VT(&in_pArgs[1]) != VT_I2 ) return E_INVALIDARG;
	if ( V_VT(&in_pArgs[2]) != VT_I2 ) return E_INVALIDARG;
	if ( V_VT(&in_pArgs[3]) != VT_I2 ) return E_INVALIDARG;
	if ( V_VT(&in_pArgs[4]) != VT_I2 ) return E_INVALIDARG;

  	char l_szString[MAX_PATH];

	memset ( l_szString,0,MAX_PATH );
	wcstombs( l_szString, in_pArgs[0].bstrVal, wcslen (in_pArgs[0].bstrVal));

	if ( in_pArgs[1].vt == 2 )
	{

		SMDType = in_pArgs[1].iVal;
	} else {
		SMDType = in_pArgs[1].intVal;
	}

	if ( in_pArgs[2].vt == 2 )
	{

		HRCActive = in_pArgs[2].iVal;
	} else {
		HRCActive = in_pArgs[2].intVal;
	}

	if ( in_pArgs[3].vt == 2 )
	{

		RemoveCharacterRigRoot = in_pArgs[3].iVal;
	} else {
		RemoveCharacterRigRoot = in_pArgs[3].intVal;
	}

	if ( in_pArgs[4].vt == 2 )
	{

		RemoveUnusedBones = in_pArgs[4].iVal;
	} else {
		RemoveUnusedBones = in_pArgs[4].intVal;
	}
	return ExportDotXSI( l_szString );
}


HRESULT	ExportDotXSI( char * in_szString )
{
	memset ( ROOT_TOKEN, 0, 256 );
	memset ( EFF_TOKEN, 0, 256 );

	if ( HRCActive )
	{
		strcpy ( ROOT_TOKEN, "Root" );
		strcpy ( EFF_TOKEN, "eff" );
		
	} else {
		strcpy ( ROOT_TOKEN, "MADEBYXSI" );
		strcpy ( EFF_TOKEN, "MADEBYXSI" );
	}


/*
	//
	// First trigger an export of the entire scene
	//

	SetXSIValue ( "dotXSIExportOptions.output_file_format_version", 300 );

	SetXSIValue ( "dotXSIExportOptions.animall", 1 );

	if ( SMDType != SMD_VERTEX_ANIMATION )
	{
		SetXSIValue ( "dotXSIExportOptions.tesselation", 1 );
		
		if ( SMDType == SMD_MODEL )
		{
			SetXSIValue ( "dotXSIExportOptions.animplot", 0 );
			SetXSIValue ( "dotXSIExportOptions.animall", 0 );
			SetXSIValue ( "dotXSIExportOptions.constraint", 1 );
		} else {
			SetXSIValue ( "dotXSIExportOptions.animplot", 1 );
			SetXSIValue ( "dotXSIExportOptions.constraint", 0 );
		}

	} else {
		
		SetXSIValue ( "dotXSIExportOptions.tesselation", 1 );
		SetXSIValue ( "dotXSIExportOptions.animplot", 0 );
		

	}

	SetXSIValue ( "dotXSIExportOptions.custompset",0 );		// no custom pset

	SetXSIValue ( "dotXSIExportOptions.txtfiles",0 );		// dont copy textures

	SetXSIValue ( "dotXSIExportOptions.fcurveKind", 20 );

	SetXSIValue ( "dotXSIExportOptions.interpolation", 3 );

	SetXSIValue ( "dotXSIExportOptions.envelope", 1 );

	SetXSIValue ( "dotXSIExportOptions.ik", 0 );

	SetXSIValue ( "dotXSIExportOptions.txtformat", 4 );

	SetXSIValue ( "dotXSIExportOptions.textures", 4 );

	SetXSIValue ( "dotXSIExportOptions.output_file_format", 1 );

	XSI::Application app;

	XSI::Model		Root = app.GetActiveSceneRoot();

	XSI::CValueArray args(2);

	args[0] = CValue(Root);
#ifdef XSI351
	args[1] = CValue(L"c:\\__tmp.xsi");
#else
	args[1] = CValue(L"3DMEMORYBUFFER");
#endif


	XSI::CValue retVal;
	app.ExecuteCommand( L"SIExportDotXSIFile", args, retVal );
*/
	
	ConvertdotXSI( in_szString );
	
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

//const SI_Int CSIAccumString::k_InitialBufSize = 0x100;

void ConvertdotXSI ( SI_Char *in_szFilename )
{
	//CSLScene			Scene;
	SMDNodeList			Nodes;
	SMDSkeleton			Skeleton;
	SMDTriangle			Triangle;
	CVertexAnimation	VertexAnim;

	XSI::Application	app;
	XSI::Model root = app.GetActiveSceneRoot();


	XSI::Property PlayControl = app.GetActiveProject().GetProperties().GetItem(L"Play Control");
		
	float oldend = 0;
	float oldstart = 0;

	if ( SMDType == SMD_MODEL )
	{
		oldstart = PlayControl.GetParameterValue(L"in");
		oldend = PlayControl.GetParameterValue(L"out");
		PlayControl.PutParameterValue(L"in", 0.0f);
		PlayControl.PutParameterValue(L"out", 1.0f);
	}

	//
	// Tell the skeleton about it's reference list
	//
	Skeleton.SetReferenceList ( &Nodes );

	Triangle.SetNodeList ( &Nodes );
	
	SI_Char	l_szDrive[MAX_PATH];
	SI_Char	l_szPath[MAX_PATH];
	SI_Char l_szFilename[MAX_PATH];

	CSIBCString	l_szAnimFilename;
	CSIBCString	l_szRefFilename;

	_splitpath ( in_szFilename, l_szDrive, l_szPath,l_szFilename,NULL );

	l_szAnimFilename.Concat ( l_szDrive );
	l_szAnimFilename.Concat ( l_szPath );
	l_szAnimFilename.Concat ( l_szFilename );
	l_szAnimFilename.Concat ( "_anim.smd" );

	l_szRefFilename.Concat ( l_szDrive );
	l_szRefFilename.Concat ( l_szPath );
	l_szRefFilename.Concat ( l_szFilename );
	l_szRefFilename.Concat ( "_ref.smd" );
	
	int oldtype = SMDType;

	CSIBCArray<XSI::X3DObject>	m_pGlobalSkeletonList;

#ifdef XSI351
	//if(Scene.Open( "c:\\__tmp.xsi") == SI_SUCCESS)
#else
//	if(Scene.Open( "3DMEMORYBUFFER") == SI_SUCCESS)
#endif
	{
	//	Scene.Read();

		g_pModelArray.DisposeData();
		g_pEnvelopeArray.DisposeData();

		/*
		BuildModelArray ( root.FindChildren(L"",XSI::siCrvListPrimType,CStringArray()) );
		BuildModelArray ( root.FindChildren(L"",XSI::siNullPrimType,CStringArray()) );
		BuildModelArray ( root.FindChildren(L"",XSI::siPolyMeshType,CStringArray()) );
		BuildModelArray ( root.FindChildren(L"",XSI::siChainBonePrimType,CStringArray()) );
		BuildModelArray ( root.FindChildren(L"",XSI::siChainEffPrimType,CStringArray()) );
		BuildModelArray ( root.FindChildren(L"",XSI::siSrfMeshPrimType,CStringArray()) );
		BuildModelArray ( root.FindChildren(L"",XSI::siArcPrimType,CStringArray()) );
		BuildModelArray ( root.FindChildren(L"",XSI::siCirclePrimType,CStringArray()) );
		BuildModelArray ( root.FindChildren(L"",XSI::siConePrimType,CStringArray()) );
		BuildModelArray ( root.FindChildren(L"",XSI::siCubePrimType,CStringArray()) );
		BuildModelArray ( root.FindChildren(L"",XSI::siCylinderPrimType,CStringArray()) );
		BuildModelArray ( root.FindChildren(L"",XSI::siDiscPrimType,CStringArray()) );
		BuildModelArray ( root.FindChildren(L"",XSI::siDodecahedronPrimType,CStringArray()) );
		BuildModelArray ( root.FindChildren(L"",XSI::siGridPrimType,CStringArray()) );
		BuildModelArray ( root.FindChildren(L"",XSI::siSpherePrimType,CStringArray()) );
		BuildModelArray ( root.FindChildren(L"",XSI::siSquarePrimType,CStringArray()) );
		*/
		XSI::CStringArray l_Family;
		l_Family.Add ( XSI::siNurbsSurfaceMeshFamily );
		l_Family.Add ( XSI::siNurbsCurveListFamily );
		l_Family.Add ( XSI::siMeshFamily );
		l_Family.Add ( XSI::siCurveFamily );
		l_Family.Add ( XSI::siGeometryFamily );
		l_Family.Add ( XSI::siChainElementFamily );
		l_Family.Add ( XSI::siImplicitGeometryFamily );
		l_Family.Add ( XSI::siNullPrimitiveFamily );
		BuildModelArray ( root.FindChildren(L"",L"",l_Family) );
    
		
		//BuildModelArray ( root.FindChildren(L"",L"",CStringArray()) );
		
	
		//CSLEnvelopeList* l_pEnvelopes = Scene.EnvelopeList();

		FixSymetry ( &g_pModelArray );

		if ( g_pEnvelopeArray.GetUsed() )
		{

			if ( !RemoveUnusedBones )
			{
				ExportEntireScene ( &Triangle, &Nodes, &Skeleton );
			} else {
				XSI::Envelope	l_pEnv = (XSI::Envelope)g_pEnvelopeArray[0];
				XSI::CRefArray l_pDefList = l_pEnv.GetDeformers();
				long	l_iNbDeformers = l_pDefList.GetCount();

				//
				// export the enveloped mesh
				//

				XSI::CRefArray l_pOwners = (XSI::CRefArray)l_pEnv.GetOwners();
				XSI::Primitive l_pOnwerPrimitive = (XSI::Primitive)l_pOwners[0];
				XSI::X3DObject l_pOwnerMesh = l_pOnwerPrimitive.GetParent();
				
				XSI::Kinematics hc1,hc2;

				Nodes.AddNode ( l_pOwnerMesh.GetFullName(), -1, l_pOwnerMesh, hc1, hc2);
				//if ( e == 0 )
				{
					ExportSkeletonData ( l_pOwnerMesh, &Skeleton, &Nodes );
				}

				XSI::Application app;
				
				XSI::UIToolkit kit = app.GetUIToolkit();
				XSI::ProgressBar	m_pBar = kit.GetProgressBar();
				
				m_pBar.PutMaximum( 100 );
				m_pBar.PutMinimum( 1 );
				m_pBar.PutStep( 1 );
				m_pBar.PutValue( 1 );
				m_pBar.PutCaption( L"Analyzing scene..." );
				m_pBar.PutStatusText( L"" );
				m_pBar.PutVisible( true );
				
				long progress_value = 0;
				long last_progress_value = 0;
				
				for (int e=0;e<l_iNbDeformers;e++)
				{
					progress_value = (long)(((float)e / (float)l_iNbDeformers) * 100.0f);
					
					if ( progress_value != last_progress_value )
					{
						last_progress_value = progress_value;
						m_pBar.PutValue ( progress_value );
						
						if ( m_pBar.IsCancelPressed() )
						{
							if ( MessageBox ( NULL, "Cancelling the export will create a corrupted SMD file.\n\n Are you sure?", "Cancel Export", MB_YESNO|MB_ICONWARNING ) == IDYES )
							{
								break;
							} else {
								
								m_pBar.PutVisible( true );
							}
							
						}
						
					}
					
					XSI::X3DObject l_pBone = l_pDefList[e];

					if ( IsRootOrEffector(l_pBone) )
					{
						char *pName = new char [ l_pBone.GetFullName().Length() + 1 ];
						W2AHelper ( pName, l_pBone.GetFullName().GetWideString() );
						char mess[1024];

						sprintf ( mess, "Optimizing out %s", pName);
						XSILogMessage ( mess, XSI::siWarningMsg );
						continue;

					}

					XSI::X3DObject l_pParent;

					l_pParent = l_pBone.GetParent();

					XSI::X3DObject	l_szParentName;
					XSI::X3DObject	l_szGrandParentName;

					XSI::Kinematics	l_pHierarchicalCompensation;
					XSI::Kinematics	l_pHierarchicalCompensation2;

					if ( l_pParent.IsValid() )
					{
						l_szParentName = l_pParent;

						if ( IsRootOrEffector(l_szParentName) )
						{
							l_pHierarchicalCompensation = l_pParent.GetKinematics();
						
							XSI::X3DObject l_pGrandParent = l_pParent.GetParent();
							if ( l_pGrandParent.IsValid())
							{
								l_szGrandParentName = l_pGrandParent;

								if ( IsRootOrEffector( l_szGrandParentName ))
								{
									l_pHierarchicalCompensation2 = l_pGrandParent.GetKinematics();
								}

							}

							XSI::X3DObject nullobj;
							if ( l_pParent.GetParent().IsValid() )
							{
								l_szParentName = l_pParent.GetParent();

								if ( IsRootOrEffector(l_szParentName))
								{
									XSI::X3DObject pp = l_pParent.GetParent();

									if ( pp.GetParent().IsValid() )
									{
										l_szParentName = pp.GetParent();
									} else {
										
										l_szParentName = nullobj;
									}

								}

							} else {
								l_szParentName = nullobj;
							}

						}

						SMDNode*	l_pParentNode = Nodes.GetByName ( l_szParentName.GetFullName() );
						if ( l_pParentNode == NULL )
						{
							XSI::Application app;
							XSI::Model		Root = app.GetActiveSceneRoot();
							if ( Root != l_pParent )
							{
								if ( RemoveCharacterRigRoot )
								{
									if ( !l_pHierarchicalCompensation.IsValid() )
									{
										l_pHierarchicalCompensation = l_pParent.GetKinematics();
									} else {

										if ( l_pParent.GetKinematics() != l_pHierarchicalCompensation )
											l_pHierarchicalCompensation2 = l_pParent.GetKinematics();
									}
								} else {

									if ( !IsRootOrEffector(l_pParent))
									{
										XSI::Kinematics hc2;

										Nodes.AddNode ( l_szParentName.GetFullName(), -1,l_pParent, l_pHierarchicalCompensation, hc2);
										ExportSkeletonData ( l_pParent, &Skeleton, &Nodes );
									}
								}
							}
							
						}

					}

					



					Nodes.AddNode ( l_pBone.GetFullName(), l_szParentName.GetFullName(), l_pHierarchicalCompensation, l_pHierarchicalCompensation2 );

					if ( SMDType == SMD_SKELETAL_ANIMATION )
					{
						m_pGlobalSkeletonList.Extend(1);
						m_pGlobalSkeletonList[m_pGlobalSkeletonList.GetUsed()-1] = l_pBone;
					} else {
						SMDType = SMD_SKELETAL_ANIMATION;
						ExportSkeletonData ( l_pBone, &Skeleton, &Nodes );
					}

					ExportTriangleData ( l_pEnv, l_pBone, &Triangle, &Nodes );

					SMDType = oldtype;
													
				}
			}
		} else {

			if ( SMDType != SMD_VERTEX_ANIMATION )
			{
				//
				// Locate all mesh primitives and export that instead
				//

				CSIBCArray < XSI::X3DObject > l_pPrimitiveList;

				GetPrimitives (  l_pPrimitiveList, TRUE );
				
				if ( !l_pPrimitiveList.GetUsed() )
				{
					XSILogMessage ( "Error - Nothing to export", XSI::siErrorMsg );
				} else {

					XSI::Kinematics hc1, hc2;
					for (int p=0;p<l_pPrimitiveList.GetUsed();p++)
					{
						Nodes.AddNode ( l_pPrimitiveList[p].GetFullName(), -1 , l_pPrimitiveList[p], hc1, hc2);
					}

					for (int t=0;t<l_pPrimitiveList.GetUsed();t++)
					{
						ExportTriangleData ( l_pPrimitiveList[t], &Triangle, &Nodes );
					}

					Triangle.ConvertToRigid ();
					
					Skeleton.ConvertToModelSMD();
				}
			}

		}


		if ( SMDType == SMD_VERTEX_ANIMATION )
		{
			
			CSIBCArray < XSI::X3DObject > l_pPrimitiveList;
			
			GetPrimitives (  l_pPrimitiveList );
			
			if ( !l_pPrimitiveList.GetUsed() )
			{
				XSILogMessage ( "Error - Nothing to export", XSI::siErrorMsg );
			} else {
				
				for (int p=0;p<l_pPrimitiveList.GetUsed();p++)
				{
					XSI::Kinematics hc1, hc2;
					Nodes.AddNode ( l_pPrimitiveList[p].GetFullName(), -1 , l_pPrimitiveList[p], hc1, hc2);
				}
			}
			

			//
			// Find the model which has shape animation
			//

			XSI::X3DObject ShapeAnimated;
			for (int i =0; i<l_pPrimitiveList.GetUsed(); i++)
			{
				XSI::X3DObject obj = l_pPrimitiveList[i];
				
				bool	isAnimated = false;
				isAnimated = obj.IsAnimated (XSI::siShapeAnimationSource);
				
				if ( isAnimated )
				{
					ShapeAnimated = obj;
					break;
				}
			}

			if ( ShapeAnimated.IsValid() )
			{
				VertexAnim.SetModel ( ShapeAnimated );

				XSI::Application app;
				XSI::Property playControl = app.GetActiveProject().GetProperties().GetItem(L"Play Control");
				
				float startframe = playControl.GetParameterValue(L"in");
				float endframe = playControl.GetParameterValue(L"out" );
//				long l_iCount = (long)(endframe - startframe);

				float t1 = -9999.0f;
				float t2;

				
				//
				// we need to recompute the normals for all shapes
				//

				XSI::Primitive l_pPrim = ShapeAnimated.GetActivePrimitive();
				VertexAnim.AddAnimationFrame ( startframe, l_pPrim, startframe, startframe, true );
/*
				for (int c=0;c<l_iCount;c++)
				{
					CSLLinearKey*	l_pKey = &l_pAnimation->GetLinearKeyListPtr()[c];

					float l_time = l_pKey->m_fTime;
					float l_shape = l_pKey->m_fValue;

					CSLBaseShape* l_pCurrentShape = l_pShapeAnimModel->Shapes() [(int)l_shape];

//					RecomputeNormals ( l_pMesh, l_pCurrentShape );

				}
*/

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
				long last_progress_value = 0;
				
				for (int k=(int)startframe;k<(int)endframe;k++)
				{
					progress_value = (long)(((float)k / (float)endframe) * 100.0f);
					
					if ( progress_value != last_progress_value )
					{
						last_progress_value = progress_value;
						m_pBar.PutValue ( progress_value );
						
						if ( m_pBar.IsCancelPressed() )
						{
							if ( MessageBox ( NULL, "Cancelling the export will create a corrupted SMD file.\n\n Are you sure?", "Cancel Export", MB_YESNO|MB_ICONWARNING ) == IDYES )
							{
								break;
							} else {
								
								m_pBar.PutVisible( true );
							}
							
						}
						
					}
					
					char mess[MAX_PATH];
					sprintf ( mess, "Processing ShapeAnimation at frame %d", k );
					
					wchar_t*l_wszString = NULL;
					A2W(&l_wszString, mess);
					
					m_pBar.PutCaption( l_wszString );

					t1 = (float)k;
					t2 = (float)k + 1.0f;

					VertexAnim.AddAnimationFrame ( (float)(k+1), l_pPrim, t1, t2, false);

					t1 = t2;
				}
				
			} else {

				//
				// Error, scene doesn't use ShapeAnimation
				//

				XSILogMessage ( "Cannot export to .VTA: scene doesn't use ShapeAnimation ", XSI::siErrorMsg );


			}

		
			
		}

  


		if ( m_pGlobalSkeletonList.GetUsed() )
		{
			//
			// mass skeleton export
			//

			ExportSkeletonData ( m_pGlobalSkeletonList, &Skeleton, &Nodes );

		}

		//
		// Export animation file
		//

		
		XSI::Application app;
		XSI::Property playControl = app.GetActiveProject().GetProperties().GetItem(L"Play Control");
		
		float startframe = playControl.GetParameterValue(L"in");
		
		FILE *f;

		if ( SMDType == SMD_SKELETAL_ANIMATION )
		{
			
			f = fopen (in_szFilename,"wt");

			if ( !f )
			{
				XSILogMessage ( "Cannot open or create target file. Please verify the name of the file.", XSI::siWarningMsg );
				return ;
			} 

			AddSMDComments(f);

			fprintf ( f,"version 1\n" );

			
			Nodes.Write ( f );
			Skeleton.Write ( f, startframe );

			fclose (f);
		}

		if (( SMDType == SMD_MODEL ) || ( SMDType == SMD_PHYSBOX ))
		{

			//
			// Export reference file
			//

			f = fopen (in_szFilename,"wt");
			
			if ( !f )
			{
				XSILogMessage ( "Cannot open or create target file. Please verify the name of the file.", XSI::siWarningMsg );
				return ;
			} 

			AddSMDComments(f);

			fprintf ( f,"version 1\n" );

			Nodes.Write ( f );
			Skeleton.Write ( f, startframe );
			Triangle.Write ( f );

			fclose (f);
		}

		if ( SMDType == SMD_VERTEX_ANIMATION )
		{

			f = fopen (in_szFilename,"wt");
			
			if ( !f )
			{
				XSILogMessage ( "Cannot open or create target file. Please verify the name of the file.", XSI::siWarningMsg );
				return ;
			} 

			fprintf ( f,"version 1\n" );

			//Nodes.ConvertToModelSMD();
			Nodes.Write ( f );

	//		Skeleton.CreateDummyKeys ( VertexAnim.GetNbFrames() );
			Skeleton.Write ( f, startframe );
			VertexAnim.Write ( f, &Nodes );
			fclose (f);

		}
			
	}


	if ( SMDType == SMD_MODEL )
	{
		PlayControl.PutParameterValue(L"out", oldend);
		PlayControl.PutParameterValue(L"in", oldstart);
	}
}

void SetXSIValue ( char * in_szValueName , long in_lValue )
{
	CComPtr<XSIApplication>						l_pApplication;
	CLSID	lclsid;
	CLSIDFromProgID(L"XSI.Application", &lclsid );
	CoCreateInstance(lclsid, NULL, CLSCTX_INPROC, IID_XSIApplication, (void **)&l_pApplication);

	_variant_t args;  
	_variant_t v;  
	long l = 0 ;
	SAFEARRAY* psa = NULL;  
	const int cntArgs = 2 ;

	psa = ::SafeArrayCreateVector( VT_VARIANT, 0, cntArgs );  

	args.vt = VT_VARIANT | VT_ARRAY;  
	args.parray = psa;

	//
	// First arg
	//

	v = _bstr_t( in_szValueName );
	SafeArrayPutElement(psa, &l, &v);  
	l++;  
	v.Clear();

	//
	// Second arg
	//

	v = (long)in_lValue;

	SafeArrayPutElement(psa, &l, &v);  
	l++;  

	VARIANT	l_vOutValue;
	l_pApplication->ExecuteScriptCommand( _bstr_t( L"SetValue" ), args, &l_vOutValue ) ;

}

void ExportSkeletonData ( XSI::X3DObject in_pModel, SMDSkeleton* in_pSkeleton, SMDNodeList* in_pNodeList  )
{

	SMDNode*	l_pNode = in_pNodeList->GetByName ( in_pModel.GetFullName() );
	XSI::Kinematics	l_pTransform = in_pModel.GetKinematics();
	bool	l_bKeyAdded = false;

	float startframe;
	float endframe;
	float framerate;

	double x,y,z;
	l_pTransform.GetLocal().GetTransform().GetRotationFromXYZAnglesValues ( x,y,z );

	XSI::Application app;
	XSI::Property playControl = app.GetActiveProject().GetProperties().GetItem(L"Play Control");

	startframe = playControl.GetParameterValue(L"in");
	endframe = playControl.GetParameterValue(L"out" );
	framerate = playControl.GetParameterValue(L"rate");


	//
	// First check if this model is animated
	//

	if (( SMDType == SMD_SKELETAL_ANIMATION ))
	{

		//sf -= 1.0f;
		
		for (int k=(int)((float)startframe);k< (int) ((float)(endframe)+1.0f);k++)
		{
			CSIBCVector3D		l_vPosition;
			CSIBCVector3D		l_vRotation;
			

			GetPositionAndRotationAtFrame ( l_pTransform, l_vPosition, l_vRotation, k );

			//TickTransform ( l_pNode->GetHierarchicalCompensation (), (float)k );
			//TickTransform ( l_pNode->GetHierarchicalCompensation2 (), (float)k );

			if ( l_pNode->GetParentID() == -1 )
			{
				
			
				XSI::KinematicState l_gXatrix = l_pTransform.GetGlobal();
				XSI::MATH::CTransformation l_xTrans = l_gXatrix.GetTransform( k );
				
				l_xTrans.PutRotationOrder ( XSI::MATH::CRotation::siXYZ );
				l_xTrans = l_gXatrix.GetTransform( k );

				XSI::MATH::CMatrix4	l_gMatrix = l_xTrans.GetMatrix4();

				XSI::MATH::CTransformation l_mMatrix;
				l_mMatrix.SetIdentity();
				l_mMatrix.SetRotationFromXYZAnglesValues (1.5707963267948965923, 0.0f, 0.0f );

				XSI::MATH::CTransformation l_oMatrix;
				l_oMatrix.SetIdentity();
				l_oMatrix.SetTranslationFromValues ( l_vPosition.GetX(), l_vPosition.GetY(), l_vPosition.GetZ() );
				l_oMatrix.SetRotationFromXYZAnglesValues (l_vRotation.GetX(), l_vRotation.GetY(), l_vRotation.GetZ());
					
				CSIBCVector3D	s,r,t;

				double rx,ry,rz;
				double x,y,z;
				l_xTrans.GetRotationFromXYZAnglesValues ( rx,ry,rz );
				l_xTrans.GetTranslationValues(x,y,z);
				rx *= 57.29577951308232286465;
				ry *= 57.29577951308232286465;
				rz *= 57.29577951308232286465;
				
				if ( !l_pNode->GetHierarchicalCompensation ().IsValid() )
				{
					in_pSkeleton->AddKey (k,
											l_pNode->GetID (),
											CSIBCVector3D((float)x,(float)y,(float)z), 
											CSIBCVector3D((float)rx,(float)ry,(float)rz));
					l_bKeyAdded = true;
				}

			}

			CTransformation	l_mLocal = l_pTransform.GetLocal().GetTransform(k);
			l_mLocal.PutRotationOrder ( XSI::MATH::CRotation::siXYZ );
			l_mLocal = l_pTransform.GetLocal().GetTransform(k);
			

			if ( l_pNode->GetHierarchicalCompensation().IsValid() )
			{
				CTransformation	l_mHC = l_pNode->GetHierarchicalCompensation().GetLocal().GetTransform ( k );
				l_mHC.PutRotationOrder(XSI::MATH::CRotation::siXYZ);
				l_mHC = l_pNode->GetHierarchicalCompensation().GetLocal().GetTransform ( k );
			
				l_mLocal.Mul ( l_mLocal, l_mHC );

				if ( l_pNode->GetHierarchicalCompensation2().IsValid() )
				{
					CTransformation	l_mHC2 = l_pNode->GetHierarchicalCompensation2().GetLocal().GetTransform ( k );
					l_mHC2.PutRotationOrder(XSI::MATH::CRotation::siXYZ);
					l_mHC2 = l_pNode->GetHierarchicalCompensation2().GetLocal().GetTransform ( k );

					l_mLocal.Mul ( l_mLocal, l_mHC2 );

				}
			}

			CSIBCVector3D	l_vS, l_vR, l_vT;
			double rx,ry,rz;
			double tx,ty,tz;
			l_mLocal.GetRotationFromXYZAnglesValues ( rx, ry, rz );
			l_vR.m_fX = (float)rx * 57.29577951308232286465f;
			l_vR.m_fY = (float)ry * 57.29577951308232286465f;
			l_vR.m_fZ = (float)rz * 57.29577951308232286465f;

			l_mLocal.GetTranslationValues ( tx, ty, tz );

			l_vT.m_fX = (float)tx;
			l_vT.m_fY = (float)ty;
			l_vT.m_fZ = (float)tz;


			if ( !l_bKeyAdded )
			{
				in_pSkeleton->AddKey (k,
									l_pNode->GetID (),
									l_vT, 
									l_vR );
			}

			l_bKeyAdded = false;

		}

	} else {

		//
		// this model is not animated. So we use the transform as frame 0 as "animation"
		//


		if ( l_pNode->GetParentID() == -1 )
		{
			
			XSI::MATH::CTransformation l_pGlobalNonAnimated = l_pTransform.GetGlobal().GetTransform();
			l_pGlobalNonAnimated.PutRotationOrder(XSI::MATH::CRotation::siXYZ);
			l_pGlobalNonAnimated = l_pTransform.GetGlobal().GetTransform();

			double rx,ry,rz;
			double tx,ty,tz;
			l_pGlobalNonAnimated.GetRotationFromXYZAnglesValues ( rx, ry, rz );
			rx *= 57.29577951308232286465f;
			ry *= 57.29577951308232286465f;
			rz *= 57.29577951308232286465f;
			l_pGlobalNonAnimated.GetTranslationValues ( tx, ty, tz );

			// did the user want to zero out root nodes?
			if ( ( ExportSelectedOnly ) && (ZeroRootTransforms))
			{
				rx=ry=rz=tx=ty=tz=0;
			}
			
			//
			// get the first frame from the scene
			//

			//float sf = in_pModel->Scene()->SceneInfo()->GetStart();
		//	sf -= 1.0f;

			in_pSkeleton->AddKey (0,
								l_pNode->GetID (),
								CSIBCVector3D((float)tx,(float)ty,(float)tz), 
								CSIBCVector3D((float)rx,(float)ry,(float)rz) );

		} else {

			CTransformation	l_mLocal = l_pTransform.GetLocal().GetTransform();
			l_mLocal.PutRotationOrder ( XSI::MATH::CRotation::siXYZ);
			l_mLocal = l_pTransform.GetLocal().GetTransform();
			
			if ( l_pNode->GetHierarchicalCompensation().IsValid() )
			{
				CTransformation	l_mHC = l_pNode->GetHierarchicalCompensation().GetLocal().GetTransform();
				l_mHC.PutRotationOrder ( XSI::MATH::CRotation::siXYZ);
				l_mHC = l_pNode->GetHierarchicalCompensation().GetLocal().GetTransform();

				l_mLocal.Mul ( l_mLocal, l_mHC );

				if ( l_pNode->GetHierarchicalCompensation2().IsValid() )
				{
					CTransformation	l_mHC2 = l_pNode->GetHierarchicalCompensation2().GetLocal().GetTransform();
					l_mHC2.PutRotationOrder ( XSI::MATH::CRotation::siXYZ);
					l_mHC2 = l_pNode->GetHierarchicalCompensation2().GetLocal().GetTransform();
			
					l_mLocal.Mul ( l_mLocal, l_mHC2 );

				}

			}

			CSIBCVector3D	l_vS, l_vR, l_vT;
			double rx,ry,rz;
			double tx,ty,tz;
			l_mLocal.GetRotationFromXYZAnglesValues ( rx, ry, rz );
			l_vR.m_fX = (float)rx * 57.29577951308232286465f;;
			l_vR.m_fY = (float)ry * 57.29577951308232286465f;;
			l_vR.m_fZ = (float)rz * 57.29577951308232286465f;;

			l_mLocal.GetTranslationValues ( tx, ty, tz );

			l_vT.m_fX = (float)tx;
			l_vT.m_fY = (float)ty;
			l_vT.m_fZ = (float)tz;


			in_pSkeleton->AddKey (0,
								l_pNode->GetID (),
								l_vT, 
								l_vR );
		}

	}


}

void	ExportSkeletonData ( CSIBCArray<XSI::X3DObject>& in_pModelList, SMDSkeleton* in_pSkeleton, SMDNodeList* in_pNodeList  )
{

	XSI::CValue startframe;
	XSI::CValue endframe;
	XSI::CValue framerate;

	XSI::Application app;
	XSI::Property playControl = app.GetActiveProject().GetProperties().GetItem(L"Play Control");

	startframe = playControl.GetParameterValue(L"in");
	endframe = playControl.GetParameterValue(L"out" );
	framerate = playControl.GetParameterValue(L"rate");

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
	long last_progress_value = 0;


	//
	// First check if this model is animated
	//

	if (( SMDType == SMD_SKELETAL_ANIMATION ))
	{

		//sf -= 1.0f;
		
		for (int k=(int)((float)startframe);k< (int) ((float)(endframe)+1.0f);k++)
		{
			playControl.PutParameterValue(L"Current",(float)k);
			
			progress_value = (long)(((float)k / (float)endframe) * 100.0f);

			for (int m=0;m<in_pModelList.GetUsed();m++)
			{
				XSI::X3DObject in_pModel = in_pModelList[m];

				SMDNode*	l_pNode = in_pNodeList->GetByName ( in_pModel.GetFullName() );
				XSI::Kinematics	l_pTransform = in_pModel.GetKinematics();
				bool	l_bKeyAdded = false;

				if ( progress_value != last_progress_value )
				{
					last_progress_value = progress_value;
					m_pBar.PutValue ( progress_value );
					
					if ( m_pBar.IsCancelPressed() )
					{
						if ( MessageBox ( NULL, "Cancelling the export will create a corrupted SMD file.\n\n Are you sure?", "Cancel Export", MB_YESNO|MB_ICONWARNING ) == IDYES )
						{
							break;
						} else {
							
							m_pBar.PutVisible( true );
						}
						
					}
					char mess[MAX_PATH];
					sprintf ( mess, "Processing frame %d", k );
					
					wchar_t*l_wszString = NULL;
					A2W(&l_wszString, mess);
					
					m_pBar.PutCaption( l_wszString );
				}

			
				CSIBCVector3D		l_vPosition;
				CSIBCVector3D		l_vRotation;
				

				GetPositionAndRotationAtFrame ( l_pTransform, l_vPosition, l_vRotation, k );

				//TickTransform ( l_pNode->GetHierarchicalCompensation (), (float)k );
				//TickTransform ( l_pNode->GetHierarchicalCompensation2 (), (float)k );

				if ( l_pNode->GetParentID() == -1 )
				{
					
				
					XSI::KinematicState l_gXatrix = l_pTransform.GetGlobal();
					XSI::MATH::CTransformation l_xTrans = l_gXatrix.GetTransform( k );
					l_xTrans.PutRotationOrder ( XSI::MATH::CRotation::siXYZ);
					l_xTrans = l_gXatrix.GetTransform( k );

					XSI::MATH::CMatrix4	l_gMatrix = l_xTrans.GetMatrix4();

					XSI::MATH::CTransformation l_mMatrix;
					l_mMatrix.SetIdentity();
					l_mMatrix.SetRotationFromXYZAnglesValues (1.5707963267948965923, 0.0f, 0.0f );

					XSI::MATH::CTransformation l_oMatrix;
					l_oMatrix.SetIdentity();
					l_oMatrix.SetTranslationFromValues ( l_vPosition.GetX(), l_vPosition.GetY(), l_vPosition.GetZ() );
					l_oMatrix.SetRotationFromXYZAnglesValues (l_vRotation.GetX(), l_vRotation.GetY(), l_vRotation.GetZ());
						
					CSIBCVector3D	s,r,t;

					double rx,ry,rz;
					double x,y,z;
					l_xTrans.GetRotationFromXYZAnglesValues ( rx,ry,rz );
					l_xTrans.GetTranslationValues(x,y,z);
					rx *= 57.29577951308232286465;
					ry *= 57.29577951308232286465;
					rz *= 57.29577951308232286465;
					
					if ( !l_pNode->GetHierarchicalCompensation ().IsValid() )
					{
						in_pSkeleton->AddKey (k,
											l_pNode->GetID (),
											CSIBCVector3D((float)x,(float)y,(float)z), 
											CSIBCVector3D((float)rx,(float)ry,(float)rz));
						l_bKeyAdded = true;
					}

				}

				CTransformation	l_mLocal = l_pTransform.GetLocal().GetTransform(k);
				l_mLocal.PutRotationOrder ( XSI::MATH::CRotation::siXYZ);
				l_mLocal = l_pTransform.GetLocal().GetTransform(k);
				

				if ( l_pNode->GetHierarchicalCompensation().IsValid() )
				{
					CTransformation	l_mHC = l_pNode->GetHierarchicalCompensation().GetLocal().GetTransform ( k );
					l_mHC.PutRotationOrder ( XSI::MATH::CRotation::siXYZ);
					l_mHC = l_pNode->GetHierarchicalCompensation().GetLocal().GetTransform ( k );
				
					l_mLocal.Mul ( l_mLocal, l_mHC );

					if ( l_pNode->GetHierarchicalCompensation2().IsValid() )
					{
						CTransformation	l_mHC2 = l_pNode->GetHierarchicalCompensation2().GetLocal().GetTransform ( k );
						l_mHC2.PutRotationOrder(XSI::MATH::CRotation::siXYZ);
						l_mHC2 = l_pNode->GetHierarchicalCompensation2().GetLocal().GetTransform ( k );

						l_mLocal.Mul ( l_mLocal, l_mHC2 );

					}
				}

				CSIBCVector3D	l_vS, l_vR, l_vT;
				double rx,ry,rz;
				double tx,ty,tz;
				l_mLocal.GetRotationFromXYZAnglesValues ( rx, ry, rz );
				l_vR.m_fX = (float)rx * 57.29577951308232286465f;
				l_vR.m_fY = (float)ry * 57.29577951308232286465f;
				l_vR.m_fZ = (float)rz * 57.29577951308232286465f;

				l_mLocal.GetTranslationValues ( tx, ty, tz );

				l_vT.m_fX = (float)tx;
				l_vT.m_fY = (float)ty;
				l_vT.m_fZ = (float)tz;


				if ( !l_bKeyAdded )
				{
					in_pSkeleton->AddKey (k,
										l_pNode->GetID (),
										l_vT, 
										l_vR );
				}

				l_bKeyAdded = false;
			}

		}

	} 

}

void ExportTriangleData ( XSI::Envelope in_pEnvelope, XSI::X3DObject in_pBone, SMDTriangle *in_pTriangle, SMDNodeList* in_pNodes )
{
	XSI::CRefArray l_pOwners = (XSI::CRefArray)in_pEnvelope.GetOwners();
	XSI::Primitive l_pOnwerPrimitive = (XSI::Primitive)l_pOwners[0];
	XSI::X3DObject l_pOwnerMesh = l_pOnwerPrimitive.GetParent();

	in_pTriangle->AddEnvelope ( l_pOwnerMesh );

	XSI::X3DObject l_pDeformer = in_pBone;
	int l_iBoneID = in_pNodes->GetByName (l_pDeformer.GetFullName())->GetID();

	XSI::CDoubleArray l_pWeights = in_pEnvelope.GetDeformerWeights( l_pDeformer, 0.0 );
	XSI::CClusterElementArray l_cElements = in_pEnvelope.GetElements(0);
	XSI::CLongArray indexArray = l_cElements.GetArray();

	//SLVertexWeight* l_pWeights = in_pEnvelope->GetVertexWeightListPtr();
	for (int i=0;i<l_pWeights.GetCount();i++)
	{
		in_pTriangle->AddWeight ( l_pOwnerMesh, l_iBoneID, (SI_Int)indexArray[i], (float)l_pWeights[i] );
	}

}

void ExportTriangleData ( XSI::X3DObject in_pModel, SMDTriangle *in_pTriangle, SMDNodeList* in_pNodes )
{
	
	in_pTriangle->AddEnvelope ( in_pModel );

	int l_iBoneID = in_pNodes->GetByName (in_pModel.GetFullName())->GetID();

	XSI::Geometry geom(in_pModel.GetActivePrimitive().GetGeometry());
	XSI::PolygonMesh polygonmesh(geom);


	XSI::CPointRefArray pointRefArray(polygonmesh.GetPoints());

	
	for (long i=0;i<pointRefArray.GetCount();i++)
	{
		in_pTriangle->AddWeight ( in_pModel, l_iBoneID, (SI_Int)i, 100.0f );
	}
}

// Mesage handler for about box.
LRESULT CALLBACK OptionDlg(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		case WM_INITDIALOG:
		{
				SendDlgItemMessage(hDlg,IDC_ANIMATION, BM_SETCHECK ,BST_CHECKED,0); 
				SendDlgItemMessage(hDlg,IDC_REFERENCE, BM_SETCHECK ,BST_UNCHECKED,0); 

				return TRUE;
		}

		case WM_COMMAND:
			if (LOWORD(wParam) == IDC_OK || LOWORD(wParam) == IDCANCEL) 
			{
			   if ( SendDlgItemMessage ( hDlg, IDC_ANIMATION, BM_GETCHECK,0,0) == BST_CHECKED )
			   {
				   	SMDType = SMD_SKELETAL_ANIMATION;
			   }

			   if ( SendDlgItemMessage ( hDlg, IDC_REFERENCE, BM_GETCHECK,0,0 ) == BST_CHECKED )
			   {
				   	SMDType = SMD_MODEL;
			   }

				EndDialog(hDlg, LOWORD(wParam));
				return TRUE;
			}
			break;
	}
    return FALSE;
}
/*
CSLShapeAnimation*	FindShapeAnimModel ( CSLModel *in_pModel )
{

	CSLPrimitive*	l_pPrimitive = in_pModel->Primitive();

	if ( l_pPrimitive != NULL )
	{
		if ( l_pPrimitive->Type() == CSLTemplate::SI_MESH )
		{
			CSLGeometry*	l_pGeometry = (CSLGeometry*)l_pPrimitive;

			if ( l_pGeometry->ShapeAnimation() != NULL )
			{

				return l_pGeometry->ShapeAnimation();

			}
		}


	}

	CSLModel** l_pList = in_pModel->GetChildrenList();

	for (int c=0;c<in_pModel->GetChildrenCount();c++)
	{
		CSLShapeAnimation *l_pAnim = FindShapeAnimModel ( l_pList[c] );

		if ( l_pAnim != NULL )
			return l_pAnim;

	}

	return NULL;
}
*/
void	XSILogMessage ( char *in_szMessage, XSI::siSeverityType in_eSeverity )
{
	XSI::Application	app;

	wchar_t*l_wszString = NULL;
	A2W(&l_wszString, in_szMessage);

	app.LogMessage( l_wszString, in_eSeverity ) ;
}

void	AddSMDComments (FILE* in_printfile)
{
	// EDITOR=Softimage XSI 5.0.2005.1013-1
	// SCENE=U:\dev\main\content\episodic\models\Alyx_XSI\Scenes\Alyx_Reference.scn
	// DATE=01/17/2006  14:09
	// USERNAME=wade
	// VPROJECT=. . . .
	// UPAXIS=Y

	// write out the XSI version
	XSI::Application app;
	XSI::CString xsiCS = app.GetVersion();
	const char* xsiVersion = xsiCS.GetAsciiString();
	fprintf ( in_printfile,"// EDITOR=Softimage XSI %s\n", xsiVersion);

	// get the current scene name
	XSI::Project proj = app.GetActiveProject();
	XSI::Scene scn = proj.GetActiveScene();
	XSI::CParameterRefArray params = scn.GetParameters();
	XSI::Parameter p = params.GetItem( L"Filename" );
	XSI::CString sRealName = p.GetValue( double(1) );
	const char* sceneName = sRealName.GetAsciiString();
	fprintf ( in_printfile,"// SCENE=%s\n", sceneName );

	// get the time for each .smd
	SYSTEMTIME stUTC, stLocal;
	GetSystemTime(&stUTC);
	SystemTimeToTzSpecificLocalTime(NULL, &stUTC, &stLocal);
	fprintf( in_printfile, "// DATE=%02d/%02d/%d  %02d:%02d\n", stLocal.wMonth, stLocal.wDay, stLocal.wYear, stLocal.wHour, stLocal.wMinute);

	// get the USERNAME and VPROJECT environment variables
	const char* userVar = getenv("USERNAME");
	fprintf ( in_printfile,"// USERNAME=%s\n", userVar);
	const char* vprojectVar = getenv("VPROJECT");
	fprintf ( in_printfile,"// VPROJECT=%s\n", vprojectVar);

	// add the upaxis command for use in the .qc files
	fprintf ( in_printfile,"// UPAXIS=Z\n");

	// write out the export settings
	fprintf ( in_printfile,"// SMD Export Settings\n" );

	//"Filename"
	XSI::Property prop;
	prop = app.GetActiveSceneRoot().GetProperties().GetItem( L"SMDExportProperty2" );
	XSI::Parameter param = prop.GetParameters().GetItem(L"Filename" );
	XSI::CString	str = param.GetValue();
	const char* xsiFilename = str.GetAsciiString();
	fprintf ( in_printfile,"//\t\tFilename: %s\n", xsiFilename );

	//"File Type"
	//"Model (.SMD)" = 0,
	//"Physbox (.SMD)" = 1
	//"Skeletal Animation (.SMD)" = 2
	//"Vertex Animation (.VTA)" = 3
	param = prop.GetParameters().GetItem(L"SMDType");
	SMDType = (long) param.GetValue();
	switch (SMDType)
	{
		case 0:
		default:
			fprintf ( in_printfile,"//\t\tFile Type: Model (.SMD)\n" );
			break;
		case 1:	
			fprintf ( in_printfile,"//\t\tFile Type: Physbox (.SMD)\n" );
			break;
		case 2:	
			fprintf ( in_printfile,"//\t\tFile Type: Skeletal Animation (.SMD)\n" );
			break;
		case 3:
			fprintf ( in_printfile,"//\t\tFile Type: Vertex Animation (.VTA)\n" );
			break;
	}

	//"Optimize Chains and Compensate"=
	param = prop.GetParameters().GetItem(L"HRC");
	int HRC = (bool) param.GetValue();
	if ( HRC )
	{
		fprintf ( in_printfile,"//\t\tOptimize Chains and Compensate: ON\n");
	}
	else
	{
		fprintf ( in_printfile,"//\t\tOptimize Chains and Compensate: OFF\n");
	}

	//"Remove Top-Level Bone on Character Rig"=
	param = prop.GetParameters().GetItem(L"RemoveBigRoot");
	int RemoveBigRoot = (bool) param.GetValue();
	if ( RemoveBigRoot )
	{
		fprintf ( in_printfile,"//\t\tRemove Top-Level Bone on Character Rig: ON\n" );
	}
	else
	{
		fprintf ( in_printfile,"//\t\tRemove Top-Level Bone on Character Rig: OFF\n" );
	}

	//"Remove all unused bones"=
	param = prop.GetParameters().GetItem(L"RemoveUnusedBones");
	int RemoveUnusedBones = (bool) param.GetValue();
	if ( RemoveUnusedBones )
	{
		fprintf ( in_printfile,"//\t\tRemove all unused bones: ON\n" );
	}
	else
	{
		fprintf ( in_printfile,"//\t\tRemove all unused bones: OFF\n" );
	}

	//"Export Selection Only"=
	param = prop.GetParameters().GetItem(L"SelectedOnly");
	int SelectedOnly = (bool) param.GetValue();
	if ( SelectedOnly )
	{
		fprintf ( in_printfile,"//\t\tExport Selection Only: ON\n" );
	}
	else
	{
		fprintf ( in_printfile,"//\t\tExport Selection Only: OFF\n" );
	}

	//"Zero Root Transforms"=
	param = prop.GetParameters().GetItem(L"ZeroRootTransforms");
	int ZeroRoot = (bool) param.GetValue();
	if ( ZeroRoot )
	{
        fprintf ( in_printfile,"//\t\tZero Root Transforms: ON\n" );
	}
	else
	{
        fprintf ( in_printfile,"//\t\tZero Root Transforms: OFF\n" );
	}

}

void	GetPrimitives ( CSIBCArray < XSI::X3DObject >& in_pPrimitiveList, BOOL in_bNeedUV )
{
	XSI::Application app;

	XSI::Model root = app.GetActiveSceneRoot();

	XSI::CRefArray l_pArray = root.FindChildren(L"",XSI::siPolyMeshType,CStringArray());

	for (long c=0;c<l_pArray.GetCount();c++)
	{
		XSI::X3DObject	l_pObj = l_pArray[c];

		if (ExportSelectedOnly)
		{
			if (!IsSelected (l_pObj))
			{
				continue;
			}
		}
		
		if ( l_pObj.IsValid() )
		{
			XSI::Geometry geom(l_pObj.GetActivePrimitive().GetGeometry());
			XSI::PolygonMesh polygonmesh(geom);
			
			if ( in_bNeedUV )
			{
				if ( polygonmesh.IsValid())
				{
					
					//Take care of the UV's
					XSI::CRefArray		clusterRefArray;
					polygonmesh.GetClusters().Filter(XSI::siSampledPointCluster,CStringArray(),L"",clusterRefArray);
					
					XSI::Cluster		samplePointClusterUV;
					XSI::CRefArray       uvClusterPropertiesRefArray;
					
					for(int i=0;i < clusterRefArray.GetCount(); i++)
					{
						XSI::Cluster cluster(clusterRefArray[i]);		
						if(cluster.GetProperties().Filter(XSI::siClsUVSpaceTxtType,CStringArray(), L"",uvClusterPropertiesRefArray) == CStatus::OK)
						{
							samplePointClusterUV = cluster;			
							break;
						}
					}
					
					if ( samplePointClusterUV.IsValid() )
					{
						
						//
						// this mesh has uvs, add it to the list
						//
						
						in_pPrimitiveList.Extend(1);
						in_pPrimitiveList[in_pPrimitiveList.GetUsed()-1] = l_pObj;
						
						
					}
					
				} 
			} else {

				//
				// no need to have uvs
				//
				
				in_pPrimitiveList.Extend(1);
				in_pPrimitiveList[in_pPrimitiveList.GetUsed()-1] = l_pObj;
				
			}
		}
	}
	
}

void	FixSymetry ( CSIBCArray<XSI::CRef>* in_pModelList )
{
/*
	if ( in_pModelList->GetUsed() )
	{
		for (int e=0;e<in_pModelList->GetUsed();e++)
		{
		//	CSLEnvelope*	l_pEnvelope = in_pEnvList->Envelopes()[e];

			CSLModel* l_pBone = (*in_pModelList)[e];

			CSLTransform* l_pTransform = l_pBone->Transform();

			CSIBCVector3D	l_vScale = l_pTransform->GetScale ();

			if ( ( l_vScale.m_fX < 0.0f ) || ( l_vScale.m_fY < 0.0f ) || ( l_vScale.m_fZ < 0.0f ) )
			{
				CTransformation newTransfo;
				CTransformation mScaling;
				CTransformation mRotation;

				CVector3	V_Neg = CVector3 ( 1.0, -1.0, 1.0 );
				CVector3	V_Rot = CVector3 ( 0.0, (-180.0f * 0.01745329251994329547f), 0.0 );

				mScaling.SetScaling ( V_Neg );
				mRotation.SetRotationFromXYZAngles ( V_Rot );
				
				newTransfo.Mul ( mScaling, mRotation );

				// STEP 2 & 3 
				//CSIBCVector3D	V_Neg = CSIBCVector3D ( 1.0f, -1.0f, 1.0f );
				//CSIBCVector3D	V_Rot = CSIBCVector3D ( 0.0f, (-180.0f * 0.01745329251994329547f), 0.0f );
							
				//
				// Get the root's local transformation matrix
				//

				CTransformation l_xRootTransfo;
				CSIBCMatrix4x4	l_mRootTransfo;
				CSIBCVector3D	s,r,t;
				l_mRootTransfo = l_pTransform->GetMatrix();
				l_mRootTransfo.GetTransforms ( s,r,t );

				s = l_pTransform->GetScale();
				r = l_pTransform->GetEulerRotation();
				t = l_pTransform->GetTranslation();

				l_xRootTransfo.SetIdentity();
				l_xRootTransfo.SetScalingFromValues ( s.m_fX, s.m_fY, s.m_fZ );
				l_xRootTransfo.SetRotationFromXYZAnglesValues ( r.m_fX * 0.01745329251994329547f,r.m_fY * 0.01745329251994329547f,r.m_fZ* 0.01745329251994329547f );
				l_xRootTransfo.SetTranslationFromValues ( t.m_fX, t.m_fY, t.m_fZ );
				
				

				
				CTransformation l_xResultTransfo;

				l_xResultTransfo.SetIdentity();
				l_xResultTransfo.Mul (  newTransfo, l_xRootTransfo );
				
				double rx, ry, rz;
				l_xResultTransfo.GetRotationFromXYZAnglesValues ( rx, ry, rz );

				l_pTransform->SetScale ( CSIBCVector3D(1.0f, 1.0f, 1.0f) );
				l_pTransform->SetEulerRotation ( CSIBCVector3D((float)rx * 57.29577951308232286465f, (float)ry * 57.29577951308232286465f, (float)rz * 57.29577951308232286465f) );
				
				

				//
				// STEP 4 Recursivelly fix children
				//

				CSLModel** l_pList = l_pBone->GetChildrenList();

				for (int c=0;c<l_pBone->GetChildrenCount();c++)
				{

					FixSymetryChildren ( l_pList[c] );
				}

			}

		}
	}
*/	
}

/*
void	FixSymetryChildren ( CSLModel * in_pModel  )
{

	CSIBCVector3D	s,r,t;

	r = in_pModel->Transform()->GetEulerRotation();
	r.m_fX *= -1.0f;
	r.m_fZ *= -1.0f;

	t = in_pModel->Transform()->GetTranslation();
	t.m_fY *= -1.0f;
	in_pModel->Transform()->SetTranslation( t );

	in_pModel->Transform()->SetEulerRotation ( r );

	//
	// Fix FCurves
	//

	CSLBaseFCurve* l_pRX = (CSLBaseFCurve*)in_pModel->Transform()->GetSpecificFCurve ( CSLTemplate::SI_ROTATION_X );
	CSLBaseFCurve* l_pRZ = (CSLBaseFCurve*)in_pModel->Transform()->GetSpecificFCurve ( CSLTemplate::SI_ROTATION_Z );
	
	if ( l_pRX != NULL )
	{
		FixSymetryFCurve ( l_pRX );
	}

	if ( l_pRZ != NULL )
	{
		FixSymetryFCurve ( l_pRZ );
	}


	CSLModel** l_pList = in_pModel->GetChildrenList();

	for (int c=0;c<in_pModel->GetChildrenCount();c++)
	{

		FixSymetryChildren ( l_pList[c] );
	
	}

}
*/

/** 
*	Error messages and their meanings.
*	\errorcode	blablabla ablablabla
*	\errorcode	sdkfjh kdsfh ksdjhfk jdshfsd kjsdf
*	\errorcode	skdf dsf07 sdfkjs df0usd ofkj ldskf
*	\errorcode	sdkjfh ksdjfh sd98f uys9d8f sd
*/



/*
void	CreateXSITransformation ( CTransformation&	out_Transfo, CSLTransform* in_pTransform )
{
	CSIBCVector3D	s,r,t;
	s = in_pTransform->GetScale();
	r = in_pTransform->GetEulerRotation();
	t = in_pTransform->GetTranslation();

	r = r * 0.01745329251994329547f;

	out_Transfo.SetIdentity();
	out_Transfo.SetScalingFromValues ( s.m_fX, s.m_fY, s.m_fZ );
	out_Transfo.SetRotationFromXYZAnglesValues ( r.m_fX ,r.m_fY ,r.m_fZ );
	out_Transfo.SetTranslationFromValues ( t.m_fX, t.m_fY, t.m_fZ );

}

void	FixSymetryFCurve ( CSLBaseFCurve * in_pFCurve )
{
	long l_numKeys = in_pFCurve->GetKeyCount();

	for (int k=0;k<l_numKeys;k++)
	{
		switch (in_pFCurve->GetInterpolationType())
		{

		case CSLTemplate::SI_CONSTANT:
		{
			(*in_pFCurve->GetConstantKeyList())[k].m_fValue *= -1.0f;
			break;

		}

		case CSLTemplate::SI_LINEAR:
			{
				(*in_pFCurve->GetLinearKeyList())[k].m_fValue *= -1.0f;
				break;

			}

		case CSLTemplate::SI_HERMITE:
			{

				(*in_pFCurve->GetHermiteKeyList())[k].m_fValue *= -1.0f;
				break;

			}

		case CSLTemplate::SI_CUBIC:
			{
				(*in_pFCurve->GetCubicKeyList())[k].m_fValue *= -1.0f;
				break;

			}
		}


	}

}

void	TickTransform ( CSLTransform *in_pTransform , SI_Float in_fTime )
{
	if ( in_pTransform == NULL )
		return;

	CSLFCurve **l_pFCurvelist = in_pTransform->FCurves();

	for (int c=0;c<in_pTransform->GetFCurveCount();c++)
	{
		l_pFCurvelist[c]->Evaluate ( in_fTime );
	}

	//
	// Check if we need to fyx the symmetry bug now that we've ticked this transform
	// The symmetry bug is due to negative scaling on some chain roots and it's very annoying
	//

	CSIBCVector3D	l_vScale = in_pTransform->GetScale ();

	if ( ( l_vScale.m_fX < 0.0f ) || ( l_vScale.m_fY < 0.0f ) || ( l_vScale.m_fZ < 0.0f ) )
	{
		CTransformation newTransfo;
		CTransformation mScaling;
		CTransformation mRotation;

		CVector3	V_Neg = CVector3 ( 1.0, -1.0, 1.0 );
		CVector3	V_Rot = CVector3 ( 0.0, (-180.0f * 0.01745329251994329547f), 0.0 );

		mScaling.SetScaling ( V_Neg );
		mRotation.SetRotationFromXYZAngles ( V_Rot );
		
		newTransfo.Mul ( mScaling, mRotation );

		// STEP 2 & 3 
		//CSIBCVector3D	V_Neg = CSIBCVector3D ( 1.0f, -1.0f, 1.0f );
		//CSIBCVector3D	V_Rot = CSIBCVector3D ( 0.0f, (-180.0f * 0.01745329251994329547f), 0.0f );
					
		//
		// Get the root's local transformation matrix
		//

		CTransformation l_xRootTransfo;
		CSIBCMatrix4x4	l_mRootTransfo;
		CSIBCVector3D	s,r,t;
		l_mRootTransfo = in_pTransform->GetMatrix();
		l_mRootTransfo.GetTransforms ( s,r,t );

		s = in_pTransform->GetScale();
		r = in_pTransform->GetEulerRotation();
		t = in_pTransform->GetTranslation();

		l_xRootTransfo.SetIdentity();
		l_xRootTransfo.SetScalingFromValues ( s.m_fX, s.m_fY, s.m_fZ );
		l_xRootTransfo.SetRotationFromXYZAnglesValues ( r.m_fX * 0.01745329251994329547f,r.m_fY * 0.01745329251994329547f,r.m_fZ* 0.01745329251994329547f );
		l_xRootTransfo.SetTranslationFromValues ( t.m_fX, t.m_fY, t.m_fZ );
		
		

		
		CTransformation l_xResultTransfo;

		l_xResultTransfo.SetIdentity();
		l_xResultTransfo.Mul (  newTransfo, l_xRootTransfo );
		
		double rx, ry, rz;
		l_xResultTransfo.GetRotationFromXYZAnglesValues ( rx, ry, rz );

		in_pTransform->SetScale ( CSIBCVector3D(1.0f, 1.0f, 1.0f) );
		in_pTransform->SetEulerRotation ( CSIBCVector3D((float)rx * 57.29577951308232286465f, (float)ry * 57.29577951308232286465f, (float)rz * 57.29577951308232286465f) );
		
	}

}
*/
void	BuildModelArray (XSI::CRefArray in_RefArray)
{
	for (int c=0;c<in_RefArray.GetCount();c++)
	{

		XSI::SceneItem item = (XSI::SceneItem)in_RefArray[c];

		XSI::CString l_szType = item.GetType();
		char *pName = new char [ l_szType.Length() + 1 ];
		W2AHelper ( pName, l_szType.GetWideString() );


		XSI::CString l_szName  = item.GetFullName();
		char *pName2 = new char [ l_szName.Length() + 1 ];
		W2AHelper ( pName2, l_szName.GetWideString() );

		//
		// exclude some exceptions like cameras and models
		//

		if ( item.GetType().IsEqualNoCase( L"CameraRoot" ) ||
			item.GetType().IsEqualNoCase( L"CameraInterest" ) ||
			item.GetType().IsEqualNoCase( L"#model" ) )
		{
			continue;
		}

		//
		// handle selected only
		//

		if (ExportSelectedOnly)
		{
//			bool branch = item.GetSelected(XSI::siBranch);
//			bool node = item.GetSelected(XSI::siNode);

			XSI::X3DObject x3d(item);
			if (!IsSelected (x3d))
			{
				continue;
			}
		}
		g_pModelArray.Extend(1);
		g_pModelArray[g_pModelArray.GetUsed()-1] = in_RefArray[c];

		if ( item.IsValid() )
		{
			XSI::CRefArray l_pEnvelopes = item.GetEnvelopes();

			for (int e=0;e<l_pEnvelopes.GetCount();e++)
			{
				g_pEnvelopeArray.Extend(1);
				g_pEnvelopeArray[g_pEnvelopeArray.GetUsed()-1] = l_pEnvelopes[e];
			}


		}


	
	}
}

void	ExportEntireScene ( SMDTriangle* in_pTriangles, SMDNodeList* in_pNodes, SMDSkeleton* in_pSkeleton )
{
	int oldtype = SMDType;

	//
	// Add enveloped mesh
	//

	XSI::Envelope l_pEnvelope = g_pEnvelopeArray[0];
	XSI::CRefArray l_pOwners = (XSI::CRefArray)l_pEnvelope.GetOwners();
	
	XSI::Primitive l_pOnwerPrimitive = (XSI::Primitive)l_pOwners[0];
	XSI::X3DObject l_pOwnerMesh = l_pOnwerPrimitive.GetParent();

	XSI::Kinematics hc1, hc2;
	in_pNodes->AddNode ( l_pOwnerMesh.GetFullName(), -1, l_pOwnerMesh, hc1, hc2);
				
	ExportSkeletonData ( l_pOwnerMesh, in_pSkeleton, in_pNodes );

	XSI::Envelope	l_pEnv = (XSI::Envelope)g_pEnvelopeArray[0];
	XSI::CRefArray l_pDefList = l_pEnv.GetDeformers();
	long	l_iNbDeformers = l_pDefList.GetCount();

	XSI::Application app;

	XSI::UIToolkit kit = app.GetUIToolkit();
	XSI::ProgressBar	m_pBar = kit.GetProgressBar();
	
	m_pBar.PutMaximum( 100 );
	m_pBar.PutMinimum( 1 );
	m_pBar.PutStep( 1 );
	m_pBar.PutValue( 1 );
	m_pBar.PutCaption( L"Analyzing scene..." );
	m_pBar.PutStatusText( L"" );
	m_pBar.PutVisible( true );

	long progress_value = 0;
	long last_progress_value = 0;

	for (int e=0;e<g_pModelArray.GetUsed();e++)
	{
		progress_value = (long)(((float)e / (float)g_pModelArray.GetUsed()) * 100.0f);
		
		if ( progress_value != last_progress_value )
		{
			last_progress_value = progress_value;
			m_pBar.PutValue ( progress_value );
			
			if ( m_pBar.IsCancelPressed() )
			{
				if ( MessageBox ( NULL, "Cancelling the export will create a corrupted SMD file.\n\n Are you sure?", "Cancel Export", MB_YESNO|MB_ICONWARNING ) == IDYES )
				{
					break;
				} else {
					
					m_pBar.PutVisible( true );
				}	
			}	
		}
		
		XSI::X3DObject		l_pModel = g_pModelArray[e];

		if ( IsRootOrEffector ( l_pModel ))
		{
			char *pName = new char [ l_pModel.GetFullName().Length() + 1 ];
			W2AHelper ( pName, l_pModel.GetFullName().GetWideString() );
			char mess[1024];
			
			sprintf ( mess, "Optimizing out %s", pName);
			XSILogMessage ( mess, XSI::siWarningMsg );
			continue;
			
		}
		
		XSI::X3DObject l_pBone = l_pModel;
		XSI::X3DObject l_pParent;
		
		l_pParent = l_pBone.GetParent();
		
		XSI::X3DObject	l_szParentName;
		XSI::X3DObject	l_szGrandParentName;
		
		XSI::Kinematics	l_pHierarchicalCompensation;
		XSI::Kinematics	l_pHierarchicalCompensation2;
		
		if ( l_pParent.IsValid() )
		{
			l_szParentName = l_pParent;
			
			if ( IsRootOrEffector(l_szParentName))
			{
				l_pHierarchicalCompensation = l_pParent.GetKinematics();
				
				XSI::X3DObject l_pGrandParent = l_pParent.GetParent();
				if ( l_pGrandParent.IsValid() )
				{
					l_szGrandParentName = l_pGrandParent;
					
					if ( IsRootOrEffector( l_szGrandParentName))
					{
						l_pHierarchicalCompensation2 = l_pGrandParent.GetKinematics();
					}
					
				}
				
				
				if ( l_pParent.GetParent().IsValid() )
				{
					l_szParentName = l_pParent.GetParent();
					
					if ( IsRootOrEffector(l_szParentName))
					{
						if ( ((XSI::X3DObject)l_pParent.GetParent()).GetParent().IsValid() )
						{
							l_szParentName = ((XSI::X3DObject)l_pParent.GetParent()).GetParent();
						} else {
							XSI::X3DObject nullobj;
							l_szParentName = nullobj;
						}
						
					}
					
				} else {
					XSI::X3DObject nullobj;
					l_szParentName = nullobj;
				}
				
			}
			
			SMDNode*	l_pParentNode = in_pNodes->GetByName ( l_szParentName.GetFullName() );
			if ( l_pParentNode == NULL )
			{
				if ( l_pParent.GetKinematics().IsValid() )
				{
					if ( RemoveCharacterRigRoot )
					{
						if ( !l_pHierarchicalCompensation.IsValid() )
						{
							l_pHierarchicalCompensation = l_pParent.GetKinematics();
						} else {
							l_pHierarchicalCompensation2 = l_pParent.GetKinematics();
						}
					} else {
						
						if ( !IsRootOrEffector(l_pParent))
						{
							XSI::Kinematics hc2;
							in_pNodes->AddNode ( l_szParentName.GetFullName(), -1,l_pParent, l_pHierarchicalCompensation,hc2);
							ExportSkeletonData ( l_pParent, in_pSkeleton, in_pNodes );
						}
					}
				}
				
			}
			
		}
		
		in_pNodes->AddNode ( l_pBone.GetFullName(), l_szParentName.GetFullName(), l_pHierarchicalCompensation, l_pHierarchicalCompensation2 );
		
		int old_type = SMDType;
		if ( SMDType == SMD_VERTEX_ANIMATION )
		{
			SMDType = SMD_SKELETAL_ANIMATION;
		}
		ExportSkeletonData ( l_pBone, in_pSkeleton, in_pNodes );
		SMDType = old_type;

		//
		// Check if this model is used as a bone
		//

		
		
		for (int d=0;d<l_iNbDeformers;d++)
		{
			
			XSI::X3DObject l_pDeformer = (XSI::X3DObject) l_pDefList[d];
			
			if ( l_pDeformer == l_pBone )
			{
				
				ExportTriangleData ( l_pEnv, l_pBone, in_pTriangles, in_pNodes );
			}
		}
		

		SMDType = oldtype;
		
		}
			
			
}
/*
void	FindMinMaxTimes( CSLBaseFCurve* in_pFCurve, SI_Int& out_iMinimumm, SI_Int& out_iMaximum )
{
	
	switch ( in_pFCurve->GetInterpolationType() )
	{
		case CSLTemplate::SI_CONSTANT:
			{
				CSLConstantKey	*l_pKey = in_pFCurve->GetConstantKeyListPtr();

				for (int v=0;v<in_pFCurve->GetKeyCount();v++)
				{
					if ( l_pKey[v].m_fTime < (float)out_iMinimumm )
					{
						out_iMinimumm = (SI_Int)l_pKey[v].m_fTime;
					}
					
					if ( l_pKey[v].m_fTime > (float)out_iMaximum )
					{
						out_iMaximum = (SI_Int)l_pKey[v].m_fTime;
					}
					
				}
				
				break;
			}
		case CSLTemplate::SI_LINEAR:
			{
				CSLLinearKey	*l_pKey = in_pFCurve->GetLinearKeyListPtr();

				for (int v=0;v<in_pFCurve->GetKeyCount();v++)
				{
					if ( l_pKey[v].m_fTime < (float)out_iMinimumm )
					{
						out_iMinimumm = (SI_Int)l_pKey[v].m_fTime;
					}
					
					if ( l_pKey[v].m_fTime > (float)out_iMaximum )
					{
						out_iMaximum = (SI_Int)l_pKey[v].m_fTime;
					}
					
				}
				break;
			}
		case CSLTemplate::SI_HERMITE:
			{
				CSLHermiteKey*	l_pKey = in_pFCurve->GetHermiteKeyListPtr();
				for (int v=0;v<in_pFCurve->GetKeyCount();v++)
				{
					if ( l_pKey[v].m_fTime < (float)out_iMinimumm )
					{
						out_iMinimumm = (SI_Int)l_pKey[v].m_fTime;
					}
					
					if ( l_pKey[v].m_fTime > (float)out_iMaximum )
					{
						out_iMaximum = (SI_Int)l_pKey[v].m_fTime;
					}
					
				}
				break;
			}
		case CSLTemplate::SI_CUBIC:
			{
				CSLCubicKey*	l_pKey = in_pFCurve->GetCubicKeyListPtr();
				for (int v=0;v<in_pFCurve->GetKeyCount();v++)
				{
					if ( l_pKey[v].m_fTime < (float)out_iMinimumm )
					{
						out_iMinimumm = (SI_Int)l_pKey[v].m_fTime;
					}
					
					if ( l_pKey[v].m_fTime > (float)out_iMaximum )
					{
						out_iMaximum = (SI_Int)l_pKey[v].m_fTime;
					}
					
				}
				break;
			}
	}



}
*/
/*
void RecomputeNormals ( CSLMesh * in_pMesh, CSLBaseShape* in_pCurrentShape )
{
	SI_Long	l_lVertexCount = in_pCurrentShape->GetVertexCount();

	SLIndexedVector3D *l_pNormals	= in_pCurrentShape->GetIndexedNormalListPtr();

	//
	// Initialize all normals to zero
	//

	for (int z=0;z<l_lVertexCount;z++)
	{
		l_pNormals->m_Vector3D = CSIBCVector3D ( 0.0f,0.0f, 0.0f);
		l_pNormals++;

	}

	SLIndexedVector3D*	l_pPosition = NULL;
	
	l_pPosition = in_pCurrentShape->GetIndexedVertexListPtr();

	l_pNormals	= in_pCurrentShape->GetIndexedNormalListPtr();

	for (int t=0;t<in_pMesh->GetTriangleListCount();t++)
	{
		CSLTriangleList* l_pTriList = in_pMesh->TriangleLists()[t];

		SI_Int* l_pVertexList = l_pTriList->GetVertexIndicesPtr();
		
		int vii = 0;

		for (int v=0;v<l_pTriList->GetTriangleCount();v++)
		{
			CSIBCVector3D	v0 = l_pPosition[l_pVertexList[vii]].m_Vector3D;
			CSIBCVector3D	v1 = l_pPosition[l_pVertexList[vii+1]].m_Vector3D;
			CSIBCVector3D	v2 = l_pPosition[l_pVertexList[vii+2]].m_Vector3D;
			
			CSIBCVector3D A = (v1-v0);
			CSIBCVector3D B = (v2-v1);

			CSIBCVector3D Normal = A.CrossProduct ( B );
		
			l_pNormals [ l_pVertexList[vii] ].m_Vector3D = l_pNormals [ l_pVertexList[vii] ].m_Vector3D + Normal;
			l_pNormals [ l_pVertexList[vii+1] ].m_Vector3D = l_pNormals [ l_pVertexList[vii+1] ].m_Vector3D + Normal;
			l_pNormals [ l_pVertexList[vii+2] ].m_Vector3D = l_pNormals [ l_pVertexList[vii+2] ].m_Vector3D + Normal;

			vii += 3;

		}

	}

	//
	// Now normalize the list of vectors to get averaged normal
	//

	for (long n=0;n<l_lVertexCount;n++)
	{
		l_pNormals[n].m_Vector3D = l_pNormals[n].m_Vector3D.Normalize();
	}


}
*/
bool IsRootOrEffector ( XSI::X3DObject in_pObj )
{
	bool remove = false;
	
	if ( in_pObj.GetType().IsEqualNoCase(XSI::siChainEffPrimType) )
	{
		remove = true;
	}
	
	if ( in_pObj.GetType().IsEqualNoCase(XSI::siChainRootPrimType) )
	{
		remove = true;
	}

	if ( !remove )
	{
		XSI::CString l_szObjName = in_pObj.GetName();
		char *pName = new char [ l_szObjName.Length() + 1 ];
		W2AHelper ( pName, l_szObjName.GetWideString() );

		if ( in_pObj.GetType().IsEqualNoCase(XSI::siNullPrimType) )
		{
			if ( strstr ( pName, "Root") )
			{
				remove = true;
			}
		}
		delete [] pName;
	}

	return remove;
}

void GetPositionAndRotationAtFrame ( XSI::Kinematics l_pTransform, CSIBCVector3D& l_vPosition, CSIBCVector3D& l_vRotation, int k )
{

	XSI::KinematicState l_pState = l_pTransform.GetLocal();
	XSI::MATH::CTransformation l_pCTransform = l_pState.GetTransform ( k );

	l_pCTransform.PutRotationOrder (XSI::MATH::CRotation::siXYZ);

	l_pCTransform = l_pState.GetTransform ( k );
	

	double x,y,z;
	double rx,ry,rz;
	l_pCTransform.GetTranslationValues (x,y,z);
	l_pCTransform.GetRotationFromXYZAnglesValues (rx,ry,rz);

	rx *= 57.29577951308232286465f;
	ry *= 57.29577951308232286465f;
	rz *= 57.29577951308232286465f;

	l_vPosition = CSIBCVector3D ( (float)x,(float)y,(float)z );
	l_vRotation = CSIBCVector3D ( (float)rx,(float)ry,(float)rz );
}

bool	IsCommandInstalled( const char* in_szCmdName )
{
	XSI::Application app;
	XSI::CommandArray cmds = app.GetCommands();

	XSI::CString cmdname;
	cmdname.PutAsciiString (in_szCmdName);
	XSI::Command cmd = cmds.GetItem(cmdname);

	return cmd.IsValid();

}

bool IsSelected( XSI::X3DObject in_obj )
{
	if ( in_obj.GetSelected(XSI::siNode) || in_obj.GetSelected(XSI::siBranch))
	{
		return true;
	}

	return IsPartOfABranchSelection(in_obj);
}

bool IsPartOfABranchSelection( XSI::ProjectItem in_obj )
{
	XSI::Application app;
	if ( in_obj.GetSelected(XSI::siBranch) )
		return true;

	XSI::ProjectItem iParent = in_obj.GetParent3DObject();

	if ( iParent == app.GetActiveSceneRoot ())
		return false;

	return IsPartOfABranchSelection( iParent );
}

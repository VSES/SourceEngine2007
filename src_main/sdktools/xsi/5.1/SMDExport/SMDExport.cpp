// SMDExport.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"
#include <stdio.h>
#include "Commdlg.h"
#include <string.h>
#include "Scene.h"
#include "EnvelopeList.h"
#include "Envelope.h"
#include "Model.h"
#include "Transform.h"
#include "Template.h"
#include "BaseFCurve.h"
#include "BaseShape.h"
#include "TriangleList.h"
#include "BaseShape.h"
#include "Shape.h"
#include "Shape_35.h"
#include "UVCoordArray.h"
#include "SceneInfo.h"

#include "Mesh.h"
#include "ShapeAnimation.h"
#include "FCurve.h"

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
#include <xsi_project.h>
#include <xsiconv.h>

#include "resource.h"

#include "SMDNode.h"
#include "SMDSkeleton.h"
#include "SMDTriangle.h"
#include "VertexAnimation.h"
#include "expvalidate.h"

using namespace XSI;
using namespace MATH;

HINSTANCE	___gParent = NULL;
HINSTANCE	___gThis = NULL;

char	ROOT_TOKEN[256];
char	EFF_TOKEN[256];

HRESULT	ExportDotXSI( char * in_szString );


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
void	XSILogMessage ( wchar_t *in_szMessage, XSI::siSeverityType in_eSeverity );

/*!	Exports the skeleton data. The skeleton data is actually the animation key frames for all bones
	that affects an envelope.
	\param	CSLModel*		The bone's Semantic Layer Model object
	\param	SMDSkeleton*	A SMDSkeleton object that will hold the bone's keyframes
	\param	SMDNodeList*	A SMDNodeList object that hold a list of bones
	\return	nothing
*/
void	ExportSkeletonData ( CSLModel * in_pModel, SMDSkeleton* in_pSkeleton, SMDNodeList* in_pNodeList  );

/*!	Exports the triangle data. The triangle data is the topographical information for the enveloped mesh
	\param	CSLEnvelope*	A pointer to a Semantic Layer Envelope object
	\param	SMDTriangle*	A pointer to a SMDTriangle object. This will hold the triangle information
	\param	SMDNodeList*	A SMDNodeList object that hold a list of bones
	\return	nothing
*/
void	ExportTriangleData ( CSLEnvelope* in_pEnvelope, SMDTriangle *in_pTriangle, SMDNodeList* in_pNodes );

/*!	Exports the triangle data. The triangle data is the topographical information for the enveloped mesh
	\param	CSLModel*		A pointer to a Semantic Layer model object
	\param	SMDTriangle*	A pointer to a SMDTriangle object. This will hold the triangle information
	\param	SMDNodeList*	A SMDNodeList object that hold a list of bones
	\return	nothing
*/
void	ExportTriangleData ( CSLModel* in_pModel, SMDTriangle *in_pTriangle, SMDNodeList* in_pNodes );


LRESULT CALLBACK OptionDlg(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

/*!	Given a CSLModel, this function will recurse into the model's hierarchy and try to find a ShapeAnimation object.
	\param	CSLModel*		Top level Semantic Layer Model object
	\return	CSLShapeAnimation*	A pointer to a Semantic Layer ShapeAnimation object
*/
CSLShapeAnimation*	FindShapeAnimModel ( CSLModel * );

/*!	Gets the minimum and maximum key times for a single FCurve
	\param	CSLBaseFCurve*		The FCurve
	\param	SI_Int&				Minimum
	\param	SI_Int&				Maximum
	\return	nothing
*/
void				FindMinMaxTimes( CSLBaseFCurve* in_pFCurve, SI_Int& out_iMinimumm, SI_Int& out_iMaximum );
void				RecomputeNormals ( CSLMesh * in_pMesh, CSLBaseShape* in_pCurrentShape );


void	ExportEntireScene ( CSLScene* in_pScene, SMDTriangle* in_pTriangles, SMDNodeList* in_pNodes, SMDSkeleton* in_pSkeleton);

void	BuildModelArray ( CSLModel* in_pModel );
void	FixSymetry ( CSIBCArray<CSLModel*>* in_pModelList );
void	FixSymetryChildren ( CSLModel * in_pModel  );
void	FixSymetryFCurve ( CSLBaseFCurve * in_pFCurve );
void	TickTransform ( CSLTransform *in_pTransform , SI_Float in_fTime );

void	CreateXSITransformation ( CTransformation&	out_Transfo, CSLTransform* in_pTransform );
void	GetPrimitives ( CSLModel *in_pModel, CSIBCArray < CSLModel * >& in_pPrimitiveList, BOOL in_bNeedUV = FALSE );

/** 
* SMD Types.
*/
#define SMD_MODEL				0	//* Reference SMD
#define SMD_PHYSBOX				1	//* Physbox SMD
#define SMD_SKELETAL_ANIMATION	2	//* Animation SMD
#define SMD_VERTEX_ANIMATION	3	//* VertexAnimation .VTA file

int SMDType = SMD_SKELETAL_ANIMATION;
int HRCActive = 0;
int	RemoveCharacterRigRoot = 0;
int	RemoveUnusedBones = 0;
CSIBCArray<CSLModel*>	g_pModelArray;
char	g_szFilename[MAX_PATH];

namespace XSI
{
#ifdef unix
extern "C"
#endif
CStatus XSILoadPlugin( XSI::PluginRegistrar& in_reg )
{
	in_reg.PutAuthor( L"Softimage Corp" );
	in_reg.PutName( L"SMD Export Plug-in" );	in_reg.PutVersion( 1, 0 );

	// register the commands plugin item.
	in_reg.RegisterCommand( L"SMDExport", L"SMDExport" );
	// register property
	in_reg.RegisterProperty( L"SMDExportProperty" );	//
	// register the menu for the import and export
//	in_reg.RegisterMenu(siMenuMainFileExportID, L"CPPExportSMD", false, false);
	
	XSI::PluginItem pItem = in_reg.RegisterMenu(siMenuMainTopLevelID, L"ValveSource", false, false);

	if ( !pItem.IsValid() )
	{
		//
		// probably running the addon is 4.0
		//

		in_reg.RegisterMenu(siMenuMainFileExportID, L"ValveSource", false, false);

	}

	
	//	in_reg.RegisterMenu(siMenuMainTopLevelID, L"CPPImportVMF", false, false);
//	in_reg.RegisterMenu(XSI::siMenuMainFileImportID, L"CPPImportVMF", false, false);
	return XSI::CStatus::OK;	
}

#ifdef unix
extern "C"
#endif
XSI::CStatus ValveSource_Init( XSI::CRef& in_ref )
{
	Context ctxt = in_ref;
	Menu menu = ctxt.GetSource();

	CStatus st;
	MenuItem item;
	menu.AddCallbackItem(L"Export SMD...", L"OnSMDExportMenu", item);
	menu.AddCallbackItem(L"Import SMD...", L"OnSMDImportMenu", item );

	Menu	submenu;
	menu.AddItem(L"VMF Tools",siMenuItemSubmenu, submenu );
	submenu.AddCallbackItem(L"Import VMF...", L"OnVMFImportMenu", item);
	submenu.AddCallbackItem(L"Export VMF...", L"OnVMFExportMenu", item);
	submenu.AddCallbackItem(L"Show Concave Edges", L"OnShowEdgeMenu", item);

	menu.AddCallbackItem(L"WeightMap Import...", L"OnWMImportMenu", item );
	menu.AddCallbackItem(L"WeightMap Export...", L"OnWMExportMenu", item);
	menu.AddCallbackItem(L"Helper Bone Tool", L"OnShowHelperBoneTool", item );
	menu.AddCallbackItem(L"Shape Tools", L"OnShapeTools", item );

	menu.AddCallbackItem(L"Fix Broken FootRoll", L"OnFixBrokenFootRoll", item );

	Menu	submenu2;
	menu.AddItem(L"Blend Shape Keys",siMenuItemSubmenu, submenu2 );
	submenu2.AddCallbackItem(L"Set Cluster", L"OnSetCluster", item );
	submenu2.AddCallbackItem(L"Blend Shape", L"OnBlendShape", item );
	submenu2.AddCallbackItem(L"Help", L"OnBlendShapeHelp", item );

	Menu	submenu3;
	menu.AddItem(L"Characters",siMenuItemSubmenu, submenu3 );
	submenu3.AddCallbackItem(L"Biped Guide", L"OnBipedGuide", item );
	submenu3.AddCallbackItem(L"Create Rig From Guide", L"OnCreateRigFromGuide", item );


	return CStatus::OK;	
}

#ifdef unix
extern "C"
#endif
XSI::CStatus OnValveRemoveCenterOp( XSI::CRef& in_ref )
{
	Application app;
	CValueArray args(0);

	CValue ret;	app.ExecuteCommand(L"ValveRemoveCenterOp",args, ret);

	return CStatus::OK;
}


#ifdef unix
extern "C"
#endif
XSI::CStatus OnCopyShapeKey( XSI::CRef& in_ref )
{
	Application app;
	CValueArray args(0);

	CValue ret;	app.ExecuteCommand(L"ValveCopyShapeKey",args, ret);

	return CStatus::OK;
}


#ifdef unix
extern "C"
#endif
XSI::CStatus OnShowHelperBoneTool( XSI::CRef& in_ref )
{
	Application app;
	CValueArray args(0);

	CValue ret;	app.ExecuteCommand(L"ShowHelperBoneTool",args, ret);

	return CStatus::OK;
}



#ifdef unix
extern "C"
#endif
XSI::CStatus OnVMFImportMenu( XSI::CRef& in_ref )
{
	Application app;
	CValueArray args(0);

	CValue ret;	app.ExecuteCommand(L"VMFImport",args, ret);

	return CStatus::OK;
}


#ifdef unix
extern "C"
#endif
XSI::CStatus OnShowEdgeMenu( XSI::CRef& in_ref )
{
	Application app;
	CValueArray args(0);

	CValue ret;	app.ExecuteCommand(L"ShowConcaveEdges",args, ret);

	return CStatus::OK;
}


#ifdef unix
extern "C"
#endif
XSI::CStatus OnVMFExportMenu( XSI::CRef& in_ref )
{
	Application app;
	CValueArray args(0);

	CValue ret;	app.ExecuteCommand(L"VMFExport",args, ret);

	return CStatus::OK;
}

#ifdef unix
extern "C"
#endif
XSI::CStatus CPPExportSMD_Init( XSI::CRef& in_ref )
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
XSI::CStatus OnSMDImportMenu( XSI::CRef& in_ref )
{
	Application app;
	CValueArray args(0);

	CValue ret;	app.ExecuteCommand(L"SMDImport",args, ret);

	return CStatus::OK;
}

#ifdef unix
extern "C"
#endif
XSI::CStatus OnSMDExportMenu( XSI::CRef& in_ref )
{
	//
	// 
	//
	SMDExport();
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
CStatus SMDExport_Init( const XSI::CRef& in_context )
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
CStatus SMDExport_Execute( XSI::CRef& in_context )
{
	SMDExport();
	return CStatus::OK;
}

#ifdef unix
extern "C"
#endif
XSI::CStatus OnSetCluster( XSI::CRef& in_ref )
{
	Application app;
	CValueArray args(0);
	CValue ret;	app.ExecuteCommand(L"SetCopyShapeCluster",args, ret);

	return XSI::CStatus::OK;
}

#ifdef unix
extern "C"
#endif
XSI::CStatus OnBlendShape( XSI::CRef& in_ref )
{
	Application app;
	CValueArray args(0);
	CValue ret;	app.ExecuteCommand(L"CopyShape",args, ret);

	return XSI::CStatus::OK;
}

#ifdef unix
extern "C"
#endif
XSI::CStatus OnFixBrokenFootRoll( XSI::CRef& in_ref )
{
	Application app;
	CValueArray args(0);
	CValue ret;	app.ExecuteCommand(L"ValveFixBrokenFootRoll",args, ret);

	return XSI::CStatus::OK;
}

char helpmessage[] = { "Shape copy based on proportional mode\n\n\nThe first step is to select a cluster on the model and then hit 'Set Cluster'\
. This defines the cluster on which the shape key blending will occur. Points outside this cluster will never be changed.\n\
The second step is to move to some frame and do a proportional selection of points in the cluster. This defines the weight map for the blending.\n\
The third step is to move the key frame index (right click and drag in the timeline) to set the destination frame for the blending.\n\
Finally, hit 'Blend Shape'. This will update the shape key at the destination frame by blending the points between the two frames based on the weight map."};

#ifdef unix
extern "C"
#endif
XSI::CStatus OnBlendShapeHelp( XSI::CRef& in_ref )
{
	MessageBox ( GetActiveWindow(), helpmessage, "BlendShape Key Help", MB_OK );
	return XSI::CStatus::OK;
}


#ifdef unix
extern "C"
#endif
XSI::CStatus OnShapeTools( XSI::CRef& in_ref )
{
	Application app;
	CValueArray args(1);
	args[0] = XSI::CValue ( XSI::CString (L"ShapeTools"));

	CValue ret;	app.ExecuteCommand(L"OpenView",args, ret);

	return XSI::CStatus::OK;
}


#ifdef unix
extern "C"
#endif
XSI::CStatus OnWMImportMenu( XSI::CRef& in_ref )
{
	Application app;
	CValueArray args(0);

	CValue ret;	app.ExecuteCommand(L"WeightImport",args, ret);

	return XSI::CStatus::OK;
}

#ifdef unix
extern "C"
#endif
XSI::CStatus OnWMExportMenu( XSI::CRef& in_ref )
{
	Application app;
	CValueArray args(0);

	CValue ret;	app.ExecuteCommand(L"WeightExport",args, ret);

	return XSI::CStatus::OK;
}


#ifdef unix
extern "C"
#endif
XSI::CStatus OnBipedGuide( XSI::CRef& in_ref )
{
	Application app;
	CValueArray args(1);
	args[0] = XSI::CValue ( XSI::CString (L"ValveGuide.emdl"));

	CValue ret;	app.ExecuteCommand(L"LoadDefaultRig",args, ret);


	return XSI::CStatus::OK;
}

#ifdef unix
extern "C"
#endif
XSI::CStatus OnCreateRigFromGuide( XSI::CRef& in_ref )
{
	Application app;
	CValueArray args(0);

	CValue ret;	app.ExecuteCommand(L"hierachy_from_Biped",args, ret);

	return XSI::CStatus::OK;
}


void	SMDExport()
{

	Application app;

	if (app.GetLicense() == L"Mod Tool")
	{
		//
		//
		//
		// Check for the MOD registration key
		if (!ValidateRegistrationCurrentKey("ModTool")) {
			// Was not register or key is invalid. Ask for email + key
			if (!ValidateRegistrationNewKey("ModTool","ModTool",
				"Activate all your save and exporting options in three easy steps:\r\n\r\n1) Get your activation key from Softimage.com\r\nto enable saving and exporting your models.\r\n\r\n2) Load your key to permanently activate all\r\nyour export and save options.\r\n\r\n3) Download the game add-on of your choice.", 
				"Mod Tool Activation")) {
					return;
			}
		}
	}
	CStatus	st;

	Property prop;
	prop = app.GetActiveSceneRoot().GetProperties().GetItem( L"SMDExportProperty" );
	if (!prop.IsValid())
		prop = app.GetActiveSceneRoot().AddProperty( L"SMDExportProperty" ) ;

	CValueArray args(5);
	args[0] = prop;
	args[1] = L"";
	args[2] = L"SMDExportProperty";
	args[3] = (long)4;
	args[4] = true;
	CValue ret;
	st = app.ExecuteCommand(L"InspectObj",args,ret);
	if ( CStatus::OK == st ) {
		//
		// FileName
		//
		memset( g_szFilename, 0, MAX_PATH );
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
		//
		// Now call what we need
		//
		if (g_szFilename && strlen( g_szFilename ) > 0)
		{
			ExportDotXSI( g_szFilename );
		}
		else
		{
			XSILogMessage ( L"Error - Invalid file specified", XSI::siErrorMsg );

		}
	}
}

#ifdef unix
extern "C"
#endif
CStatus SMDExportProperty_Define( const CRef & in_Ctx )
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
	
	prop.AddParameter(	L"HRC",CValue::siBool, caps, 
						L"Optimize Chains and Compensate", L"", 
						(bool)-1, param ) ;	

	prop.AddParameter(	L"RemoveBigRoot",CValue::siBool, caps, 
						L"Remove Top-Level Bone on Character Rig", L"", 
						(bool)-1, param ) ;	

	prop.AddParameter(	L"RemoveUnusedBones",CValue::siBool, caps, 
						L"Remove all unused bones (use on character rig only)", L"", 
						(bool)true, param ) ;
	
	prop.AddParameter(	L"SMDType",CValue::siUInt4 , caps, 
						L"File Type", L"", 
						CValue(0L), param ) ;	
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
CStatus SMDExportProperty_DefineLayout( const CRef & in_Ctx )
{
	PPGLayout oLayout = Context( in_Ctx ).GetSource() ;

	

	return RebuildLayout( oLayout );
}

CStatus	RebuildLayout( PPGLayout & oLayout )
{
	PPGItem item ;

	oLayout.Clear() ;


	item = oLayout.AddItem( L"SMDExportProperty" ) ;

	oLayout.AddGroup(L"File") ;
		oLayout.AddRow() ;
				
		item = oLayout.AddItem( L"Filename",L"Export File",siControlFilePath ) ;
			item.PutAttribute( siUINoLabel, true ) ;
			item.PutAttribute( siUIFileFilter, L"Valve SMD Files (*.smd)|*.smd|Vertex Animation Files (*.vta)|*.vta|All Files (*.*)|*.*||" ) ;
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
	oLayout.EndGroup() ;

	return CStatus::OK;	
}


};


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



	//
	// First trigger an export of the entire scene
	//

	SetXSIValue ( "dotXSIExportOptions.output_file_format_version", 300 );

	SetXSIValue ( "dotXSIExportOptions.animall", 1 );

	float oldStart;
	float oldEnd;

	XSI::Application	app;
	XSI::Model root = app.GetActiveSceneRoot();
	XSI::Property PlayControl = app.GetActiveProject().GetProperties().GetItem(L"Play Control");
	
	if ( SMDType == SMD_MODEL )
	{	
		oldStart = PlayControl.GetParameterValue(L"in");
		oldEnd = PlayControl.GetParameterValue(L"out");
		PlayControl.PutParameterValue(L"in", 0.0f);
		PlayControl.PutParameterValue(L"out", 1.0f);
	}
	


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


	XSI::Model		Root = app.GetActiveSceneRoot();

	XSI::CValueArray args(2);

	args[0] = CValue(Root);

	if (app.GetLicense() == L"Mod Tool")
	{
		args[1] = CValue(L"3DMEMORYBUFFER");
	}
	else
	{
		args[1] = CValue(L"c:\\__tmp.xsi");
	}

	XSI::CValue retVal;
	app.ExecuteCommand( L"SIExportDotXSIFile", args, retVal );

	
	ConvertdotXSI( in_szString );
	
	if ( SMDType == SMD_MODEL )
	{
		PlayControl.PutParameterValue(L"out", oldEnd);
		PlayControl.PutParameterValue(L"in", oldStart);
	}
	
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

const SI_Int CSIAccumString::k_InitialBufSize = 0x100;

void ConvertdotXSI ( SI_Char *in_szFilename )
{
	CSLScene			Scene;
	SMDNodeList			Nodes;
	SMDSkeleton			Skeleton;
	SMDTriangle			Triangle;
	CVertexAnimation	VertexAnim;
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
	XSI::Application app;

	bool	l_bResult;
	if (app.GetLicense() == L"Mod Tool")
	{
		l_bResult = (Scene.Open( "3DMEMORYBUFFER") == SI_SUCCESS);
	}
	else
	{
		l_bResult = (Scene.Open( "c:\\__tmp.xsi") == SI_SUCCESS);
	}
	if (l_bResult)
	{
		Scene.Read();

		g_pModelArray.DisposeData();

		BuildModelArray ( Scene.Root());

		CSLEnvelopeList* l_pEnvelopes = Scene.EnvelopeList();

		FixSymetry ( &g_pModelArray );

		if ( l_pEnvelopes != NULL )
		{

			if ( !RemoveUnusedBones )
			{
				ExportEntireScene ( &Scene, &Triangle, &Nodes, &Skeleton );
			} else {
				
				for (int e=0;e<l_pEnvelopes->GetEnvelopeCount();e++)
				{
					CSLEnvelope*	l_pEnvelope = l_pEnvelopes->Envelopes()[e];

					if ( l_pEnvelope->GetEnvelope() == 0 )
					{
						XSILogMessage ( L"Error - Enveloping mesh not found", XSI::siErrorMsg );
						continue;
					}

					if ( _SI_STRSTR ( l_pEnvelope->GetDeformer()->GetName(), ROOT_TOKEN ) || _SI_STRSTR ( l_pEnvelope->GetDeformer()->GetName(), EFF_TOKEN ))
					{
						wchar_t mess[1024];
						swprintf ( mess, L"Removing root %s", XA2W( l_pEnvelope->GetDeformer()->GetName() ) );
						XSILogMessage ( mess, XSI::siWarningMsg );
						continue;

					}


					Nodes.AddNode ( l_pEnvelope->GetEnvelope()->GetName(), -1, l_pEnvelope->GetEnvelope() );
					//if ( e == 0 )
					{
						ExportSkeletonData ( l_pEnvelope->GetEnvelope(), &Skeleton, &Nodes );
					}

					CSLModel* l_pBone = l_pEnvelope->GetDeformer();
					CSLModel* l_pParent;

					l_pParent = l_pBone->ParentModel();

					SI_Char*	l_szParentName = NULL;
					SI_Char*	l_szGrandParentName = NULL;

					CSLTransform*	l_pHierarchicalCompensation = NULL;
					CSLTransform*	l_pHierarchicalCompensation2 = NULL;

					if ( l_pParent != NULL )
					{
						l_szParentName = l_pParent->GetName();

						if ( _SI_STRSTR ( l_szParentName, ROOT_TOKEN ) || _SI_STRSTR ( l_szParentName, EFF_TOKEN ) )
						{
							l_pHierarchicalCompensation = l_pParent->Transform();
						
							CSLModel* l_pGrandParent = l_pParent->ParentModel();
							if ( l_pGrandParent != NULL )
							{
								l_szGrandParentName = l_pGrandParent->GetName();

								if ( _SI_STRSTR ( l_szGrandParentName, ROOT_TOKEN ) || _SI_STRSTR ( l_szGrandParentName, EFF_TOKEN ) )
								{
									l_pHierarchicalCompensation2 = l_pGrandParent->Transform();
								}

							}


							if ( l_pParent->Parent() != NULL )
							{
								l_szParentName = l_pParent->Parent()->GetName();

								if ( _SI_STRSTR ( l_szParentName, ROOT_TOKEN ) || _SI_STRSTR ( l_szParentName, EFF_TOKEN ) )
								{
									if ( l_pParent->Parent()->Parent() != NULL )
									{
										l_szParentName = l_pParent->Parent()->Parent()->GetName();
									} else {
										l_szParentName = NULL;
									}

								}

							} else {
								l_szParentName = NULL;
							}

						}

						SMDNode*	l_pParentNode = Nodes.GetByName ( l_szParentName );
						if ( l_pParentNode == NULL )
						{
							if ( l_pParent->Transform() )
							{
								if ( RemoveCharacterRigRoot )
								{
									if ( l_pHierarchicalCompensation == NULL )
									{
										l_pHierarchicalCompensation = l_pParent->Transform();
									} else {

										if ( l_pParent->Transform() != l_pHierarchicalCompensation )
											l_pHierarchicalCompensation2 = l_pParent->Transform();
									}
								} else {

									if ( !_SI_STRSTR ( l_pParent->GetName(), ROOT_TOKEN ) && !_SI_STRSTR ( l_pParent->GetName(), EFF_TOKEN ))
									{

										Nodes.AddNode ( l_szParentName, -1,l_pParent, l_pHierarchicalCompensation );
										ExportSkeletonData ( l_pParent, &Skeleton, &Nodes );
									}
								}
							}
							
						}

					}

					



					Nodes.AddNode ( l_pBone->GetName(), l_szParentName, l_pHierarchicalCompensation, l_pHierarchicalCompensation2 );

					SMDType = SMD_SKELETAL_ANIMATION;
					ExportSkeletonData ( l_pBone, &Skeleton, &Nodes );

					ExportTriangleData ( l_pEnvelope, &Triangle, &Nodes );

					SMDType = oldtype;
													
				}
			}
		} else {

			if ( SMDType != SMD_VERTEX_ANIMATION )
			{
				//
				// Locate all mesh primitives and export that instead
				//

				CSIBCArray < CSLModel * > l_pPrimitiveList;

				GetPrimitives (  Scene.Root(), l_pPrimitiveList, TRUE );
				
				if ( !l_pPrimitiveList.GetUsed() )
				{
					XSILogMessage ( L"Error - Nothing to export", XSI::siErrorMsg );
				} else {

					for (int p=0;p<l_pPrimitiveList.GetUsed();p++)
					{
						Nodes.AddNode ( l_pPrimitiveList[p]->Name().GetText(), -1 , l_pPrimitiveList[p]);
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
			
			CSIBCArray < CSLModel * > l_pPrimitiveList;
			
			GetPrimitives (  Scene.Root(), l_pPrimitiveList );
			
			if ( !l_pPrimitiveList.GetUsed() )
			{
				XSILogMessage ( L"Error - Nothing to export", XSI::siErrorMsg );
			} else {
				
				for (int p=0;p<l_pPrimitiveList.GetUsed();p++)
				{
					Nodes.AddNode ( l_pPrimitiveList[p]->Name().GetText(), -1 , l_pPrimitiveList[p]);
				}
			}
			

			//
			// Find the model which has shape animation
			//

			CSLShapeAnimation* l_pShapeAnimModel = FindShapeAnimModel ( Scene.Root() );

			if ( l_pShapeAnimModel != NULL )
			{
				VertexAnim.SetModel ( l_pShapeAnimModel->ParentModel() );

				CSLBaseFCurve*	l_pAnimation = l_pShapeAnimModel->Animation();
				long l_iCount = l_pAnimation->GetKeyCount();

				CSLBaseShape*	l_pPreviousShape = NULL;

				//
				// we need to recompute the normals for all shapes
				//

				CSLMesh*	l_pMesh = (CSLMesh*) l_pShapeAnimModel->ParentModel()->Primitive();

				for (int c=0;c<l_iCount;c++)
				{
					CSLLinearKey*	l_pKey = &l_pAnimation->GetLinearKeyListPtr()[c];

					float l_time = l_pKey->m_fTime;
					float l_shape = l_pKey->m_fValue;

					CSLBaseShape* l_pCurrentShape = l_pShapeAnimModel->Shapes() [(int)l_shape];

//					RecomputeNormals ( l_pMesh, l_pCurrentShape );

				}

				for (int k=0;k<l_iCount;k++)
				{
					CSLLinearKey*	l_pKey = &l_pAnimation->GetLinearKeyListPtr()[k];

					float l_time = l_pKey->m_fTime;
					float l_shape = l_pKey->m_fValue;

					CSLBaseShape* l_pCurrentShape = l_pShapeAnimModel->Shapes() [(int)l_shape];

					VertexAnim.AddAnimationFrame ( l_time - 1, l_pMesh, l_pPreviousShape, l_pCurrentShape );

					//l_pPreviousShape = l_pCurrentShape;
					l_pPreviousShape = l_pShapeAnimModel->Shapes() [0];
				}
		
				
			} else {

				//
				// Error, scene doesn't use ShapeAnimation
				//

				XSILogMessage ( L"Cannot export to .VTA: scene doesn't use ShapeAnimation ", XSI::siErrorMsg );


			}

			
			
		}

		
		//
		// Export animation file
		//

		FILE *f;

		if ( SMDType == SMD_SKELETAL_ANIMATION )
		{
			
			f = fopen (in_szFilename,"wt");

			fprintf ( f,"version 1\n" );

			Nodes.Write ( f );
			Skeleton.Write ( f, Scene.SceneInfo()->GetStart() );

			fclose (f);
		}

		if (( SMDType == SMD_MODEL ) || ( SMDType == SMD_PHYSBOX ))
		{

			//
			// Export reference file
			//

			f = fopen (in_szFilename,"wt");

			fprintf ( f,"version 1\n" );

			Nodes.Write ( f );
			Skeleton.Write ( f, Scene.SceneInfo()->GetStart() );
			Triangle.Write ( f );

			fclose (f);
		}
/*
		if ( SMDType == SMD_ACTOR )
		{

			//
			// Export reference file
			//

			f = fopen (in_szFilename,"wt");

			fprintf ( f,"version 1\n" );

			Nodes.Write ( f );
			Skeleton.Write ( f );
			Triangle.Write ( f );

			fclose (f);
		}

		if (( SMDType == SMD_MODEL ) || ( SMDType == SMD_PHYSBOX))
		{

			//
			// Export reference file
			//

			f = fopen (in_szFilename,"wt");

			fprintf ( f,"version 1\n" );

			Nodes.ConvertToModelSMD();
			Nodes.Write ( f );

			Skeleton.ConvertToModelSMD ();
			Skeleton.Write ( f );

			//Triangle.ConvertToModelSMD ();
			Triangle.Write ( f );

			fclose (f);
		}
*/
		if ( SMDType == SMD_VERTEX_ANIMATION )
		{

			f = fopen (in_szFilename,"wt");

			fprintf ( f,"version 1\n" );

			//Nodes.ConvertToModelSMD();
			Nodes.Write ( f );

	//		Skeleton.CreateDummyKeys ( VertexAnim.GetNbFrames() );
			Skeleton.Write ( f, Scene.SceneInfo()->GetStart() );
			VertexAnim.Write ( f, &Nodes );
			fclose (f);

		}
			
		//Scene.Write();
		Scene.Close();
		
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

void ExportSkeletonData ( CSLModel * in_pModel, SMDSkeleton* in_pSkeleton, SMDNodeList* in_pNodeList  )
{

	SMDNode*	l_pNode = in_pNodeList->GetByName ( in_pModel->GetName() );
	CSLTransform*	l_pTransform = in_pModel->Transform();
	bool	l_bKeyAdded = false;

	//
	// First check if this model is animated
	//

	if (( SMDType == SMD_SKELETAL_ANIMATION ) && ( l_pTransform->GetFCurveCount() ))
	{
		//
		// Get the translation FCurves
		//

		CSLBaseFCurve* l_pTX = (CSLBaseFCurve*)l_pTransform->GetSpecificFCurve ( CSLTemplate::SI_TRANSLATION_X );
		CSLBaseFCurve* l_pTY = (CSLBaseFCurve*)l_pTransform->GetSpecificFCurve ( CSLTemplate::SI_TRANSLATION_Y );
		CSLBaseFCurve* l_pTZ = (CSLBaseFCurve*)l_pTransform->GetSpecificFCurve ( CSLTemplate::SI_TRANSLATION_Z );

		CSLBaseFCurve* l_pRX = (CSLBaseFCurve*)l_pTransform->GetSpecificFCurve ( CSLTemplate::SI_ROTATION_X );
		CSLBaseFCurve* l_pRY = (CSLBaseFCurve*)l_pTransform->GetSpecificFCurve ( CSLTemplate::SI_ROTATION_Y );
		CSLBaseFCurve* l_pRZ = (CSLBaseFCurve*)l_pTransform->GetSpecificFCurve ( CSLTemplate::SI_ROTATION_Z );
		

		SI_Int	l_iMinimumm = 9999;
		SI_Int	l_iMaximum = -9999;
		
		int kc = -1;
		int tk = 0;

		if ( l_pTX != NULL )
		{
			tk = l_pTX->GetKeyCount();
		}
		if ( tk > kc )
			kc = tk;

		if ( l_pTY != NULL )
		{
			tk = l_pTY->GetKeyCount();
		}
		if ( tk > kc )
			kc = tk;


		if ( l_pTZ != NULL )
		{
			tk = l_pTZ->GetKeyCount();
		}
		if ( tk > kc )
			kc = tk;

		if ( l_pRX != NULL )
		{
			tk = l_pRX->GetKeyCount();
		}
		if ( tk > kc )
			kc = tk;

		if ( l_pRY != NULL )
		{
			tk = l_pRY->GetKeyCount();
		}
		if ( tk > kc )
			kc = tk;
		
		if ( l_pRZ != NULL )
		{
			tk = l_pRZ->GetKeyCount();
		}
		if ( tk > kc )
			kc = tk;


		float sf = in_pModel->Scene()->SceneInfo()->GetStart();
		//sf -= 1.0f;
		
		for (int k=(int)sf;k< (int) (sf+kc);k++)
		{
			CSIBCVector3D		l_vPosition;
			CSIBCVector3D		l_vRotation;
			CSLAnimatableType*	l_pValue;

			l_vPosition = l_pTransform->GetTranslation();
			l_vRotation = l_pTransform->GetEulerRotation();

			if ( l_pTX != NULL )
			{
				l_pTX->Evaluate( (float)k );
				l_pValue = l_pTX->GetParameter();
				l_vPosition.m_fX = l_pValue->GetFloatValue();
			}

			if ( l_pTY != NULL )
			{
				l_pTY->Evaluate( (float)k );
				l_pValue = l_pTY->GetParameter();
				l_vPosition.m_fY = l_pValue->GetFloatValue();
			}

			if ( l_pTZ != NULL )
			{

				l_pTZ->Evaluate( (float)k );
				l_pValue = l_pTZ->GetParameter();
				l_vPosition.m_fZ = l_pValue->GetFloatValue();
			}

			if ( l_pRX != NULL )
			{
				l_pRX->Evaluate( (float)k );
				l_pValue = l_pRX->GetParameter();
				l_vRotation.m_fX = l_pValue->GetFloatValue();
			}

			if ( l_pRY != NULL )
			{
				l_pRY->Evaluate( (float)k );
				l_pValue = l_pRY->GetParameter();
				l_vRotation.m_fY = l_pValue->GetFloatValue();
			}

			if ( l_pRZ != NULL )
			{

				l_pRZ->Evaluate( (float)k );
				l_pValue = l_pRZ->GetParameter();
				l_vRotation.m_fZ = l_pValue->GetFloatValue();
			}

			TickTransform ( l_pNode->GetHierarchicalCompensation (), (float)k );
			TickTransform ( l_pNode->GetHierarchicalCompensation2 (), (float)k );

			if ( l_pNode->GetParentID() == -1 )
			{
				
				l_pTransform->ComputeLocalMatrix();
				l_pTransform->ComputeGlobalMatrix();

				CSIBCMatrix4x4	l_gMatrix = l_pTransform->GetGlobalMatrix();

				CSIBCMatrix4x4 l_mMatrix;
				l_mMatrix.SetIdentity();
				l_mMatrix.SetTransforms ( CSIBCVector3D(1.0f, 1.0f, 1.0f), CSIBCVector3D((const float)1.5707963267948965923, 0.0f, 0.0f), CSIBCVector3D(0.0f, 0.0f, 0.0f) );

				CSIBCMatrix4x4 l_oMatrix;
				l_oMatrix.SetIdentity();
				l_oMatrix.SetTransforms ( CSIBCVector3D(1.0f, 1.0f, 1.0f), l_vRotation, l_vPosition );
	
				CSIBCMatrix4x4 l_result = l_mMatrix.Multiply ( l_oMatrix );

				CSIBCVector3D	s,r,t;

				//l_result.GetTransforms (s,l_vRotation,l_vPosition);
				l_gMatrix.GetTransforms (s,l_vRotation,l_vPosition);

				//l_pNode->GetHierarchicalCompensation ().SetIdentity();
				//l_pNode->SetHierarchicalCompensation ( NULL );

				if ( l_pNode->GetHierarchicalCompensation () == NULL )
				{
					in_pSkeleton->AddKey (k,l_pNode->GetID (),l_vPosition, l_vRotation * 57.29577951308232286465f);
					l_bKeyAdded = true;
				}

			//	return;
			
				//l_vRotation = l_vRotation * 57.29577951308232286465f;
				/*
				l_pTransform->ComputeLocalMatrix();

				CSLModel* l_pParent = in_pModel->ParentModel();
				CSIBCMatrix4x4	l_lMyGlobal;
				l_lMyGlobal.SetIdentity();

				while ( l_pParent != NULL )
				{

					CSLTransform* l_pPT = l_pParent->Transform();

					if ( l_pPT != NULL )
					{
						l_lMyGlobal = l_lMyGlobal.Multiply ( l_pPT->GetMatrix() );
					}

					l_pParent = l_pParent->ParentModel();
				}

				l_lMyGlobal = l_lMyGlobal.Multiply ( l_pTransform->GetMatrix() );

				CSIBCVector3D	s,r,t;

				l_pTransform->GetMatrix().GetTransforms (s,l_vRotation,l_vPosition);
			
				l_vRotation = l_vRotation * 57.29577951308232286465f;
				*/	

			}

			CTransformation	l_mLocal;
			
			CreateXSITransformation ( l_mLocal, l_pTransform );

			if ( l_pNode->GetHierarchicalCompensation() )
			{
				CTransformation	l_mHC;
			
				CreateXSITransformation ( l_mHC, l_pNode->GetHierarchicalCompensation() );

				l_mLocal.Mul ( l_mLocal, l_mHC );

				if ( l_pNode->GetHierarchicalCompensation2() )
				{
					CTransformation	l_mHC2;
			
					CreateXSITransformation ( l_mHC2, l_pNode->GetHierarchicalCompensation2() );

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
/*
			CSIBCMatrix4x4	l_mLocal;
			l_mLocal.SetTransforms ( CSIBCVector3D(1.0f, 1.0f, 1.0f), l_vRotation * 0.01745329251994329547f, l_vPosition );
			
			if ( l_pNode->GetHierarchicalCompensation() != NULL )
			{
				l_mLocal = l_pNode->GetHierarchicalCompensation()->GetMatrix().Multiply ( l_mLocal );
			}

			CSIBCVector3D l_vS, l_vR, l_vT;

			l_mLocal.GetTransforms ( l_vS, l_vR, l_vT );

			l_vR = l_vR * 57.29577951308232286465f;

			CSIBCVector3D l_Td = l_vT - l_vPosition;
			CSIBCVector3D l_Rd = l_vR - l_vRotation;

			char mess[256];
			sprintf ( mess, "Compensated by :(%f, %f, %f) , (%f, %f, %f)", l_Td.m_fX, 
																			l_Td.m_fY, 
																			l_Td.m_fZ, 
																			l_Rd.m_fX, 
																			l_Rd.m_fY, 
																			l_Rd.m_fZ );
			XSILogMessage ( mess, siWarning );

			in_pSkeleton->AddKey (k,
								l_pNode->GetID (),
								l_vT, 
								l_vR );

*/
		}

	} else {

		//
		// this model is not animated. So we use the transform as frame 0 as "animation"
		//


		if ( l_pNode->GetParentID() == -1 )
		{
			l_pTransform->ComputeGlobalMatrix();
			CSIBCMatrix4x4 l_pMatrix = l_pTransform->GetGlobalMatrix();

			CSIBCVector3D	s,r,t;

			l_pMatrix.GetTransforms ( s,r,t );

			//
			// get the first frame from the scene
			//

			float sf = in_pModel->Scene()->SceneInfo()->GetStart();
		//	sf -= 1.0f;

			in_pSkeleton->AddKey ((int)sf,
								l_pNode->GetID (),
								t, 
								r * 57.29577951308232286465f );

		} else {

			/*
			CSIBCMatrix4x4& l_mHC = l_pNode->GetHierarchicalCompensation();
			l_pTransform->ComputeLocalMatrix();
			CSIBCMatrix4x4	l_mLocal = l_pTransform->GetMatrix();

			l_mLocal = l_mHC.Multiply ( l_mLocal );

			CSIBCVector3D	l_vS, l_vR, l_vT;

			l_mLocal.GetTransforms ( l_vS, l_vR, l_vT );

			l_vR = l_vR * 57.29577951308232286465f;

			CSIBCVector3D l_Td = l_vT - l_pTransform->GetTranslation();
			CSIBCVector3D l_Rd = l_vR - l_pTransform->GetEulerRotation();

			char mess[256];
			sprintf ( mess, "Compensated by :(%f, %f, %f) , (%f, %f, %f)", l_Td.m_fX, 
																			l_Td.m_fY, 
																			l_Td.m_fZ, 
																			l_Rd.m_fX, 
																			l_Rd.m_fY, 
																			l_Rd.m_fZ );
			XSILogMessage ( mess, siWarning );
			*/

			CTransformation	l_mLocal;
			
			CreateXSITransformation ( l_mLocal, l_pTransform );

			if ( l_pNode->GetHierarchicalCompensation() )
			{
				CTransformation	l_mHC;
			
				CreateXSITransformation ( l_mHC, l_pNode->GetHierarchicalCompensation() );

				l_mLocal.Mul ( l_mLocal, l_mHC );

				if ( l_pNode->GetHierarchicalCompensation2() )
				{
					CTransformation	l_mHC2;
			
					CreateXSITransformation ( l_mHC2, l_pNode->GetHierarchicalCompensation2() );

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

void ExportTriangleData ( CSLEnvelope* in_pEnvelope, SMDTriangle *in_pTriangle, SMDNodeList* in_pNodes )
{
	in_pTriangle->AddEnvelope ( in_pEnvelope->GetEnvelope() );

	int l_iBoneID = in_pNodes->GetByName (in_pEnvelope->GetDeformer()->GetName())->GetID();

	SLVertexWeight* l_pWeights = in_pEnvelope->GetVertexWeightListPtr();

	for (int i=0;i<in_pEnvelope->GetVertexWeightCount();i++)
	{
		in_pTriangle->AddWeight ( in_pEnvelope->GetEnvelope(), l_iBoneID, (SI_Int)l_pWeights[i].m_fVertexIndex, l_pWeights[i].m_fWeight );
	}

}

void ExportTriangleData ( CSLModel* in_pModel, SMDTriangle *in_pTriangle, SMDNodeList* in_pNodes )
{
	
	in_pTriangle->AddEnvelope ( in_pModel );

	int l_iBoneID = in_pNodes->GetByName (in_pModel->GetName())->GetID();

	CSLPrimitive* l_pPrimitive = in_pModel->Primitive();
	CSLMesh*	l_pMesh = (CSLMesh*)l_pPrimitive;
	CSLBaseShape*	l_pShape = l_pMesh->Shape();
	
	for (int i=0;i<l_pShape->GetVertexCount();i++)
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

void	XSILogMessage ( wchar_t *in_szMessage, XSI::siSeverityType in_eSeverity )
{
	XSI::Application	app;
	app.LogMessage( in_szMessage, in_eSeverity ) ;


}

void	GetPrimitives ( CSLModel *in_pModel, CSIBCArray < CSLModel * >& in_pPrimitiveList, BOOL in_bNeedUV )
{
	//
	// Check if this models has a mesh primitive attached to it
	//

	CSLPrimitive* l_pPrimitive = in_pModel->Primitive();

	if ( l_pPrimitive != NULL )
	{
		if ( l_pPrimitive->Type() == CSLTemplate::SI_MESH )
		{
			//
			// need UV?
			//
			
			
			if ( in_bNeedUV )
			{
				CSIBCVector2D*	l_pUV = NULL;
				CSLMesh*		l_pMesh = (CSLMesh*)l_pPrimitive;
				CSLBaseShape*	l_pShape = l_pMesh->Shape();
				if ( l_pShape->Type() == CSLTemplate::SI_SHAPE )
				{
					CSLShape*	l_p30Shape = (CSLShape*)l_pShape;
					
					l_pUV = l_p30Shape->GetUVCoordListPtr();
					
				} else {
					
					if ( l_pShape->Type() == CSLTemplate::SI_SHAPE35 )
					{
						CSLShape_35*	l_p35Shape = (CSLShape_35*)l_pShape;
						
						if ( l_p35Shape->GetUVCoordArrayCount() )
						{
							CSLUVCoordArray* l_pUVArray = l_p35Shape->UVCoordArrays()[0];
							
							if ( l_pUVArray != NULL )
							{
								l_pUV = l_pUVArray->GetUVCoordListPtr();
							}
							
						}
						
					}
				}

				if ( l_pUV != NULL )
				{
					in_pPrimitiveList.Extend(1);
					in_pPrimitiveList[in_pPrimitiveList.GetUsed()-1] = in_pModel;
				}
			
			} else {
			
				in_pPrimitiveList.Extend(1);
				in_pPrimitiveList[in_pPrimitiveList.GetUsed()-1] = in_pModel;
			}
			
		}
	}

	CSLModel** l_pList = in_pModel->GetChildrenList();

	for (int c=0;c<in_pModel->GetChildrenCount();c++)
	{
		GetPrimitives( l_pList[c], in_pPrimitiveList, in_bNeedUV);
	}

}

void	FixSymetry ( CSIBCArray<CSLModel*>* in_pModelList )
{
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
				
				
/*				
				CSIBCMatrix4x4 l_mTransfo;

				l_mTransfo.SetRotation ( V_Rot );
				l_mTransfo.SetScaling ( V_Neg );




				CSIBCMatrix4x4 l_mRootTransfo;
				l_mRootTransfo = l_pTransform->GetMatrix();

				CSIBCMatrix4x4 l_mResultTransfo =	l_mRootTransfo.Multiply ( l_mTransfo );

				CSIBCVector3D	s,r,t;
				l_mResultTransfo.GetTransforms ( s,r,t );

				l_pTransform->SetScale ( s );
				l_pTransform->SetEulerRotation ( r );
				l_pTransform->SetTranslation ( t );
				*/
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
	
}

void	FixSymetryChildren ( CSLModel * in_pModel  )
{
	/*
	child.kinematics.local.transform.GetRotationXYZAnglesValues x,y,z
	set new_null = parent_null.AddNull("null_"&child.name)
		SelectObj new_null
		
		if in_axis_to_invert = "Y" then
			Rotate , -XSIMath.RadiansToDegrees(x), XSIMath.RadiansToDegrees(y), -XSIMath.RadiansToDegrees(z), siAbsolute, siParent, siObj, siXYZ
		elseif in_axis_to_invert = "Z" then
			Rotate , -XSIMath.RadiansToDegrees(x), -XSIMath.RadiansToDegrees(y), XSIMath.RadiansToDegrees(z), siAbsolute, siParent, siObj, siXYZ
		else 
			Rotate , XSIMath.RadiansToDegrees(x), -XSIMath.RadiansToDegrees(y), -XSIMath.RadiansToDegrees(z), siAbsolute, siParent, siObj, siXYZ
		end if
	*/

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


/** 
*	Error messages and their meanings.
*	\errorcode	blablabla ablablabla
*	\errorcode	sdkfjh kdsfh ksdjhfk jdshfsd kjsdf
*	\errorcode	skdf dsf07 sdfkjs df0usd ofkj ldskf
*	\errorcode	sdkjfh ksdjfh sd98f uys9d8f sd
*/




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

void	BuildModelArray ( CSLModel* in_pModel )
{
	if ( in_pModel->Primitive() != NULL )
	{
		CSLTemplate::ETemplateType e = in_pModel->Primitive()->Type();

		if (( in_pModel->Primitive()->Type() == CSLTemplate::SI_CURVE_LIST ) || ( in_pModel->Primitive()->Type() == CSLTemplate::SI_NULL_OBJECT ) || ( in_pModel->Primitive()->Type() == CSLTemplate::SI_MESH ))
		{
			g_pModelArray.Extend(1);
			g_pModelArray[g_pModelArray.GetUsed()-1] = in_pModel;
		}
	}
	
	CSLModel** l_pList = in_pModel->GetChildrenList();
	
	for (int c=0;c<in_pModel->GetChildrenCount();c++)
	{
		BuildModelArray( l_pList[c] );
	}

}

void	ExportEntireScene ( CSLScene* in_pScene, SMDTriangle* in_pTriangles, SMDNodeList* in_pNodes, SMDSkeleton* in_pSkeleton )
{
	int oldtype = SMDType;

	CSLEnvelopeList* l_pEnvelopes = in_pScene->EnvelopeList();

	//
	// Add enveloped mesh
	//

	in_pNodes->AddNode ( l_pEnvelopes->Envelopes()[0]->GetEnvelope()->GetName(), -1, l_pEnvelopes->Envelopes()[0]->GetEnvelope() );
				
	ExportSkeletonData ( l_pEnvelopes->Envelopes()[0]->GetEnvelope(), in_pSkeleton, in_pNodes );

	for (int e=0;e<g_pModelArray.GetUsed();e++)
	{
		CSLModel*		l_pModel = g_pModelArray[e];

		if ( _SI_STRSTR ( l_pModel->GetName(), ROOT_TOKEN ) || _SI_STRSTR ( l_pModel->GetName(), EFF_TOKEN ))
		{
			wchar_t mess[1024];
			swprintf ( mess, L"Removing root %s", XA2W( l_pModel->GetName() ) );
			XSILogMessage ( mess, XSI::siWarningMsg );
			continue;
			
		}
		
		CSLModel* l_pBone = l_pModel;
		CSLModel* l_pParent;
		
		l_pParent = l_pBone->ParentModel();
		
		SI_Char*	l_szParentName = NULL;
		SI_Char*	l_szGrandParentName = NULL;
		
		CSLTransform*	l_pHierarchicalCompensation = NULL;
		CSLTransform*	l_pHierarchicalCompensation2 = NULL;
		
		if ( l_pParent != NULL )
		{
			l_szParentName = l_pParent->GetName();
			
			if ( _SI_STRSTR ( l_szParentName, ROOT_TOKEN ) || _SI_STRSTR ( l_szParentName, EFF_TOKEN ) )
			{
				l_pHierarchicalCompensation = l_pParent->Transform();
				
				CSLModel* l_pGrandParent = l_pParent->ParentModel();
				if ( l_pGrandParent != NULL )
				{
					l_szGrandParentName = l_pGrandParent->GetName();
					
					if ( _SI_STRSTR ( l_szGrandParentName, ROOT_TOKEN ) || _SI_STRSTR ( l_szGrandParentName, EFF_TOKEN ) )
					{
						l_pHierarchicalCompensation2 = l_pGrandParent->Transform();
					}
					
				}
				
				
				if ( l_pParent->Parent() != NULL )
				{
					l_szParentName = l_pParent->Parent()->GetName();
					
					if ( _SI_STRSTR ( l_szParentName, ROOT_TOKEN ) || _SI_STRSTR ( l_szParentName, EFF_TOKEN ) )
					{
						if ( l_pParent->Parent()->Parent() != NULL )
						{
							l_szParentName = l_pParent->Parent()->Parent()->GetName();
						} else {
							l_szParentName = NULL;
						}
						
					}
					
				} else {
					l_szParentName = NULL;
				}
				
			}
			
			SMDNode*	l_pParentNode = in_pNodes->GetByName ( l_szParentName );
			if ( l_pParentNode == NULL )
			{
				if ( l_pParent->Transform() )
				{
					if ( RemoveCharacterRigRoot )
					{
						if ( l_pHierarchicalCompensation == NULL )
						{
							l_pHierarchicalCompensation = l_pParent->Transform();
						} else {
							l_pHierarchicalCompensation2 = l_pParent->Transform();
						}
					} else {
						
						if ( !_SI_STRSTR ( l_pParent->GetName(), ROOT_TOKEN ) && !_SI_STRSTR ( l_pParent->GetName(), EFF_TOKEN ))
						{
							
							in_pNodes->AddNode ( l_szParentName, -1,l_pParent, l_pHierarchicalCompensation );
							ExportSkeletonData ( l_pParent, in_pSkeleton, in_pNodes );
						}
					}
				}
				
			}
			
		}
		
		in_pNodes->AddNode ( l_pBone->GetName(), l_szParentName, l_pHierarchicalCompensation, l_pHierarchicalCompensation2 );
		
		SMDType = SMD_SKELETAL_ANIMATION;
		ExportSkeletonData ( l_pBone, in_pSkeleton, in_pNodes );

		//
		// Check if this model is used as a bone
		//

		for (int t=0;t<l_pEnvelopes->GetEnvelopeCount();t++)
		{
			CSLEnvelope*	l_pEnv = l_pEnvelopes->Envelopes()[t];

			if ( l_pEnv->GetDeformer() == l_pBone )
			{

				ExportTriangleData ( l_pEnvelopes->Envelopes()[t], in_pTriangles, in_pNodes );

			}
		}


		SMDType = oldtype;
		
		}
			
			
}

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




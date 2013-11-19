//********************************************************************
//
// File supervisor: dolaflam
//
// (c) Copyright 2002 Avid Corporation
//
// $Revision: 1 $  $Date: 17/10/02 2:24p $
// Checkin by: $Author: dolaflam $
//
// @doc	INTERNAL
//
// @module  WeightExport.cpp | <c WeightExport> implementation
//
// @devnote	Implements entry points to XSI and several other thingies.
//
//********************************************************************

//******************************************************************************
//
// Includes
//
//******************************************************************************

#include "stdafx.h"
#include "Commdlg.h"

#include <comdef.h>
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
#include <xsi_kinematics.h>

#include <xsi_transformation.h>
#include <xsi_vector3.h>
#include <initguid.h>
#include <SIBCUtil.h>
#include <SIBCArray.h>

HINSTANCE	___gParent = NULL;
char		__gszTempFilename[MAX_PATH*2];

void	GetXSIParam ( SI_Int* out_p,  LPVARIANT in_p );
void	XSILogMessage ( char *in_szMessage);
void DSW2AHelper( LPSTR out_sz, LPCWSTR in_wcs, int in_cch = -1 );
SI_Int	CompareProc( const void *arg1, const void *arg2 );

typedef struct tagWeightExportOptions
{
	char	m_szFilename[MAX_PATH];
	SI_Int	m_iFullname;

} WeightExportOptions;

typedef struct _singleWeight
{
	LONG	m_lIndex;
	double	m_dWeight;
} singleWeight;

WeightExportOptions	g_sOptions;

void ExportImp();

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

	___gParent = in_hInstModule;

	return S_OK;
}

HRESULT WINAPI XSIOnTerminate()
{
	return S_OK;
}

#ifdef unix
extern "C"
#endif
XSIPLUGINCALLBACK XSI::CStatus XSILoadPlugin( XSI::PluginRegistrar& in_reg )
{
	in_reg.PutAuthor( L"Softimage Corp" );
	in_reg.PutName( L"WeightExport Plug-in" );	in_reg.PutVersion( 1, 0 );

	// register the commands plugin item.
	in_reg.RegisterCommand( L"WeightExport", L"WeightExport" );
	// register property
	in_reg.RegisterProperty( L"WeightExportProperty" );	//
	// register the menu for the import and export
	in_reg.RegisterMenu(XSI::siMenuMainFileExportID, L"WeightExportMenu", false, false);

	return XSI::CStatus::OK;	
}

#ifdef unix
extern "C"
#endif
XSIPLUGINCALLBACK XSI::CStatus XSIUnloadPlugin( const XSI::PluginRegistrar& in_reg )
{
	
	return XSI::CStatus::OK;
}

#ifdef unix
extern "C"
#endif
XSIPLUGINCALLBACK XSI::CStatus WeightExportMenu_Init( XSI::CRef& in_ref )
{
	XSI::Context ctxt = in_ref;
	XSI::Menu menu = ctxt.GetSource();

	XSI::CStatus st;
	XSI::MenuItem item;
	menu.AddCallbackItem(L"Weight Map Export...", L"OnWeightExport", item);

	return XSI::CStatus::OK;	
}

#ifdef unix
extern "C"
#endif
XSIPLUGINCALLBACK XSI::CStatus OnWeightExport( XSI::CRef& in_ref )
{
	ExportImp();
	return XSI::CStatus::OK;
}

#ifdef unix
extern "C"
#endif
XSIPLUGINCALLBACK XSI::CStatus WeightExport_Init( const XSI::CRef& in_context )
{
	XSI::Context ctx(in_context);
	XSI::Command cmd(ctx.GetSource());

	cmd.EnableReturnValue ( true );


	XSI::ArgumentArray args = cmd.GetArguments();

	args.Add( L"arg0", (long)0 );
	
	return XSI::CStatus::OK;
}



#ifdef unix
extern "C"
#endif
XSIPLUGINCALLBACK XSI::CStatus WeightExport_Execute( XSI::CRef& in_context )
{
	ExportImp();
	return XSI::CStatus::OK;
}

#ifdef unix
extern "C"
#endif
XSIPLUGINCALLBACK XSI::CStatus WeightExportProperty_Define( const XSI::CRef & in_Ctx )
{
	XSI::Application app ;
	XSI::CustomProperty prop = XSI::Context(in_Ctx).GetSource() ;
	XSI::Parameter param ;

	// Default capabilities for most of these parameters
	int caps = XSI::siPersistable  ;
	XSI::CValue dft ;	// Used for arguments we don't want to set

	prop.AddParameter(	L"Filename",XSI::CValue::siString, caps, 
						L"Filename", L"", 
						dft, param ) ;
	
	dft = true;

	prop.AddParameter(	L"Fullname",XSI::CValue::siBool, caps, 
						L"Export Fullnames", L"", 
						dft, param ) ;	

	return XSI::CStatus::OK;	
}

#ifdef unix
extern "C"
#endif
XSIPLUGINCALLBACK XSI::CStatus WeightExportProperty_DefineLayout( const XSI::CRef & in_Ctx )
{
	XSI::PPGLayout oLayout = XSI::Context( in_Ctx ).GetSource() ;

	XSI::PPGItem item ;

	oLayout.Clear() ;


	item = oLayout.AddItem( L"WeightExportProperty" ) ;

	oLayout.AddGroup(L"File") ;
		oLayout.AddRow() ;
				
		item = oLayout.AddItem( L"Filename",L"Export File",XSI::siControlFilePath ) ;
			item.PutAttribute( XSI::siUINoLabel, true ) ;
			item.PutAttribute( XSI::siUIFileMustExist, false ) ;
			item.PutAttribute( XSI::siUIOpenFile, false) ;
			item.PutAttribute( XSI::siUIFileFilter, L"WeightMap Files (*.xsiwm)|*.xsiwm|All Files (*.*)|*.*||" ) ;
		oLayout.EndRow() ;

		oLayout.AddRow();
			item = oLayout.AddItem( L"Fullname" ) ;
		oLayout.EndRow();

	oLayout.EndGroup() ;

	return XSI::CStatus::OK;	

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
	using namespace XSI;

	//
	// Copy the options
	//

	memset ( &g_sOptions,0,sizeof(WeightExportOptions));

	if ( V_VT(&in_pArgs[0]) != VT_BSTR ) return E_INVALIDARG;
	if ( V_VT(&in_pArgs[1]) != VT_I2 ) return E_INVALIDARG;


	memset ( g_sOptions.m_szFilename,0,MAX_PATH );
	wcstombs( g_sOptions.m_szFilename, in_pArgs[0].bstrVal, (int)wcslen (in_pArgs[0].bstrVal));

	GetXSIParam ( &g_sOptions.m_iFullname, &in_pArgs[1] );
	
	if ( !strlen ( g_sOptions.m_szFilename ) )
		return E_FAIL;



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

void GetXSIParam ( SI_Int* out_p,  LPVARIANT in_p )
{
	
	if ( in_p->vt == 2 )
	{
		
		*out_p = in_p->iVal;
	} else {
		*out_p = in_p->intVal;
	}
	
}


void	XSILogMessage ( char *in_szMessage)
{
	
	XSI::Application app;
	app.LogMessage( XSI::CString(_bstr_t( in_szMessage )), XSI::siErrorMsg ) ;


}

void DSW2AHelper( LPSTR out_sz, LPCWSTR in_wcs, int in_cch )
{
	if ( out_sz != NULL && 0 != in_cch )
	{
		out_sz[0] = '\0' ;

		if ( in_wcs != NULL )
		{
			// Right now, we can't really test the return value because wcstombs() is
			// mapped to strcpy() on the Irix o32 build. When this build is not done
			// anymore, the != 0 test should be updated.

			size_t l_iLen = 0;
			l_iLen = ::wcstombs( out_sz, in_wcs, ( in_cch < 0 ) ? (int)wcslen(in_wcs) + 1 : (size_t) in_cch ) ;
			assert( (int)l_iLen != -1 );

        		if ( in_cch > 0 ) { out_sz[ in_cch - 1 ] = '\0'; }
		}
	}
}

SI_Int	CompareProc( const void *arg1, const void *arg2 )
{
	singleWeight* pItem1 = &((singleWeight*)arg1)[0];
	singleWeight* pItem2 = &((singleWeight*)arg2)[0];

	if ( pItem1->m_lIndex > pItem2->m_lIndex )
		return 1;

	if ( pItem1->m_lIndex < pItem2->m_lIndex )
		return -1;

	return 0;


}

void ExportImp()
{
	using namespace XSI;

	XSI::Application app;
	XSI::CStatus	st;
	
	XSI::Property prop;
	prop = app.GetActiveSceneRoot().GetProperties().GetItem( L"WeightExportProperty" );
	if (!prop.IsValid())
		prop = app.GetActiveSceneRoot().AddProperty( L"WeightExportProperty" ) ;
	
	XSI::CValueArray args(5);
	args[0] = prop;
	args[1] = L"";
	args[2] = L"WeightExportProperty";
	args[3] = (long)4;
	args[4] = true;
	XSI::CValue ret;
	st = app.ExecuteCommand(L"InspectObj",args,ret);
	if ( XSI::CStatus::OK == st ) {
		
		//
		// FileName
		//
		memset ( g_sOptions.m_szFilename,0,MAX_PATH );
		
		XSI::Parameter parm = prop.GetParameters().GetItem(L"Filename" );
		XSI::CString	str = parm.GetValue();
		const wchar_t	* p = str.GetWideString();
		wcstombs( g_sOptions.m_szFilename, p, wcslen (p));
		
		//
		// Bools
		//
		parm = prop.GetParameters().GetItem(L"Fullname");
		g_sOptions.m_iFullname = (bool) parm.GetValue();
		
		//
		if (g_sOptions.m_szFilename && strlen( g_sOptions.m_szFilename ) > 0)
		{
			//
			// Check selection (must be exactly one mesh selected!!!!
			//
			
			XSI::Application app;
			XSI::Selection sel = app.GetSelection();
			
			if ( sel.GetCount() != 1 )
			{
				XSILogMessage ("Invalid selection. Please select exactly 1 polymesh deformed by an envelope to export it's weights" );
				return;
			}
			
			XSI::SIObject obj(sel[0]);
			XSI::X3DObject	myX3d ( obj );
			
			if ( !myX3d.IsValid() )
			{
				XSILogMessage ("Invalid selection. not a polymesh" );
				return;
			}
			
			XSI::Primitive myPrim;
			myPrim = myX3d.GetActivePrimitive();
			
			XSI::CRefArray cra = myX3d.GetPrimitives();
			int count = cra.GetCount();
			
			if ( !myPrim.IsValid() )
			{
				XSILogMessage ("Invalid selection. not a polymesh" );
				return;
			}
			
			XSI::Geometry myGeo;
			myGeo = myPrim.GetGeometry();
			
			if ( !myGeo.IsValid() )
			{
				XSILogMessage ("Invalid selection. not a polymesh" );
				return;
			}
			
			XSI::CRefArray myEnvelopes;
			myEnvelopes = myX3d.GetEnvelopes(); 
			
			if ( !myEnvelopes.GetCount() )
			{
				XSILogMessage ("Mesh is not enveloped. Aborting." );
				return;
			}
			
			XSI::KinematicState geoKine(myX3d.GetKinematics().GetGlobal());
			
			XSI::Envelope	myEnvelope;
			myEnvelope = myEnvelopes[0];
			
			CRefArray		l_Deformers = myEnvelope.GetDeformers();

			long XSInumpoints = myGeo.GetPoints().GetCount();
/*
#define MODTOOL
#ifdef MODTOOL
			if ( XSInumpoints > 7500 )
			{
				XSILogMessage ("MOD Tool limit reached (your polymesh has more than 7500 vertices" );
				return;
			}
#endif
*/

			
			FILE*	fp = fopen ( g_sOptions.m_szFilename, "wt" );
			
			if ( !fp )
			{
				XSILogMessage ("unable to open output file for writing." );
				return;
			}
			
			fprintf ( fp, "XSIWM0100\n" );
			
						
			XSI::CPointRefArray points( myGeo.GetPoints() );
			
			fprintf ( fp, "%d\n", XSInumpoints );
			
			MATH::CTransformation xFo (geoKine.GetTransform());
			
			for (long p=0;p<XSInumpoints;p++)
			{
				bool valid;
				XSI::Point point(points.GetItem(p) );
				XSI::MATH::CVector3 pos( point.GetPosition() );
				pos *= xFo;
				
				float x = pos.GetX();
				float y = pos.GetY();
				float z = pos.GetZ();
				
				fprintf ( fp, "%f %f %f\n", x,y,z );
			}
			
			fprintf ( fp, "%d\n", l_Deformers.GetCount() );	// num bones
			
			for (int e=0;e<l_Deformers.GetCount();e++)
			{
				CSIBCArray<singleWeight>	l_pArray;
				
				XSI::X3DObject l_pCurrentDeformer = l_Deformers[e];
				
				XSI::CDoubleArray	myWeightArray = myEnvelope.GetDeformerWeights( l_pCurrentDeformer, 0.0 );
				XSI::CClusterElementArray l_cElements = myEnvelope.GetElements(0);
				
				int c = l_cElements.GetCount();
				
				CLongArray indexArray = l_cElements.GetArray();
				
				XSI::CString l_szDeformerName;
				
				if ( g_sOptions.m_iFullname )
				{
					l_szDeformerName = l_pCurrentDeformer.GetFullName();
				} else {
					l_szDeformerName = l_pCurrentDeformer.GetName();
				}
				
				long weightCount = myWeightArray.GetCount();
				
				
				char *l_szPrimitiveName = new char [ l_szDeformerName.Length() + 1 ];
				DSW2AHelper ( l_szPrimitiveName, l_szDeformerName.GetWideString() );
				
				fprintf ( fp, "%s\n", l_szPrimitiveName );
				fprintf ( fp, "%d\n", weightCount );
				
				int w;
				
				for (w=0;w<weightCount;w++)
				{
					//	fprintf ( fp, "%f\n", myWeightArray[w] );
					l_pArray.Extend(1);
					l_pArray[l_pArray.GetUsed()-1].m_lIndex = indexArray[w];
					l_pArray[l_pArray.GetUsed()-1].m_dWeight = myWeightArray[w];
					
				}
				
				qsort( l_pArray.ArrayPtr(), l_pArray.GetUsed(), sizeof( singleWeight ), CompareProc );
				
				for (w=0;w<weightCount;w++)
				{
					fprintf ( fp, "%f\n", l_pArray[w].m_dWeight);
					
				}
				
			}
			
			
			fclose(fp);
			
		}
		
	}
}
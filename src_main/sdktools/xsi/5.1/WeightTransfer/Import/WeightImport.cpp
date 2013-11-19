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
// @module  WeightImport.cpp | <c WeightImport> implementation
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

#include <xsi_transformation.h>
#include <xsi_vector3.h>
#include <initguid.h>
#include <SIBCUtil.h>
#include <SIBCArray.h>
#include <SIBCVector.h>

HINSTANCE	___gParent = NULL;
char		__gszTempFilename[MAX_PATH*2];

void	GetXSIParam ( SI_Int* out_p,  LPVARIANT in_p );
void	XSILogMessage ( char *in_szMessage, XSI::siSeverityType in_eSeverity );
void DSW2AHelper( LPSTR out_sz, LPCWSTR in_wcs, int in_cch = -1 );
const XSI::CRef*	GetCRefByName ( XSI::CRefArray allObjects, char*	in_szName );
int FindClosestPosition ( float x,float y,float z, CSIBCArray<CSIBCVector3D>* in_pVertexList );
void ImportImp();
typedef struct tagWeightExportOptions
{
	char	m_szFilename[MAX_PATH];
	SI_Int	m_iFullname;

} WeightExportOptions;

typedef struct tagBoneInfo
{
	const XSI::CRef*	m_pRef;
	CSIBCArray<float>	m_aWeights;
} BoneInfo;

WeightExportOptions	g_sOptions;

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
	in_reg.PutName( L"WeightImport Plug-in" );	in_reg.PutVersion( 1, 0 );

	// register the commands plugin item.
	in_reg.RegisterCommand( L"WeightImport", L"WeightImport" );
	// register property
	in_reg.RegisterProperty( L"WeightImportProperty" );	//
	// register the menu for the import and Import
	in_reg.RegisterMenu(XSI::siMenuMainFileImportID, L"WeightImportMenu", false, false);

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
XSIPLUGINCALLBACK XSI::CStatus WeightImportMenu_Init( XSI::CRef& in_ref )
{
	XSI::Context ctxt = in_ref;
	XSI::Menu menu = ctxt.GetSource();

	XSI::CStatus st;
	XSI::MenuItem item;
	menu.AddCallbackItem(L"Weight Map Import...", L"OnWeightImport", item);

	return XSI::CStatus::OK;	
}

#ifdef unix
extern "C"
#endif
XSIPLUGINCALLBACK XSI::CStatus OnWeightImport( XSI::CRef& in_ref )
{
	ImportImp();
	return XSI::CStatus::OK;
}

#ifdef unix
extern "C"
#endif
XSIPLUGINCALLBACK XSI::CStatus WeightImport_Init( const XSI::CRef& in_context )
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
XSIPLUGINCALLBACK XSI::CStatus WeightImport_Execute( XSI::CRef& in_context )
{
	ImportImp();
	return XSI::CStatus::OK;
}

#ifdef unix
extern "C"
#endif
XSIPLUGINCALLBACK XSI::CStatus WeightImportProperty_Define( const XSI::CRef & in_Ctx )
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
						L"Import Fullnames", L"", 
						dft, param ) ;	

	return XSI::CStatus::OK;	
}

#ifdef unix
extern "C"
#endif
XSIPLUGINCALLBACK XSI::CStatus WeightImportProperty_DefineLayout( const XSI::CRef & in_Ctx )
{
	XSI::PPGLayout oLayout = XSI::Context( in_Ctx ).GetSource() ;

	XSI::PPGItem item ;

	oLayout.Clear() ;


	item = oLayout.AddItem( L"WeightImportProperty" ) ;

	oLayout.AddGroup(L"File") ;
		oLayout.AddRow() ;
				
		item = oLayout.AddItem( L"Filename",L"Import File",XSI::siControlFilePath ) ;
			item.PutAttribute( XSI::siUINoLabel, true ) ;
			item.PutAttribute( XSI::siUIFileMustExist, true ) ;
			item.PutAttribute( XSI::siUIOpenFile, true) ;
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

	ImportImp();

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


void	XSILogMessage ( char *in_szMessage, XSI::siSeverityType in_eSeverity )
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
			l_iLen = ::wcstombs( out_sz, in_wcs, ( in_cch < 0 ) ? ::wcslen(in_wcs) + 1 : (size_t) in_cch ) ;
			assert( (int)l_iLen != -1 );

        		if ( in_cch > 0 ) { out_sz[ in_cch - 1 ] = '\0'; }
		}
	}
}

const XSI::CRef*	GetCRefByName ( XSI::CRefArray allObjects, char*	in_szName )
{
	for (int c=0;c<allObjects.GetCount();c++)
	{

		XSI::SIObject obj(allObjects[c]);
		XSI::X3DObject	myX3d ( obj );

		XSI::CString l_szDeformerName;
		l_szDeformerName = myX3d.GetFullName();
		char *l_szPrimitiveName = new char [ l_szDeformerName.Length() + 1 ];
		DSW2AHelper ( l_szPrimitiveName, l_szDeformerName.GetWideString() );

		if ( !_stricmp ( l_szPrimitiveName, in_szName ) )
		{
			return (const XSI::CRef*) &allObjects[c];
		}

	}


	return NULL;
}

int FindClosestPosition ( float x,float y,float z, CSIBCArray<CSIBCVector3D>* in_pVertexList )
{
	float l_fdistance = 99999.0f;

	CSIBCVector3D	l_vVector (x,y,z);
	int index = -1;

	for (int v=0;v<in_pVertexList->GetUsed();v++)
	{
		float dd = l_vVector.GetDistanceFrom ( (*in_pVertexList)[v] );

		if ( dd < l_fdistance )
		{
			index = v;
			l_fdistance = dd;
		}


	}

	return index;
}



void ImportImp()
{
	using namespace XSI;

	XSI::Application app;
	XSI::CStatus	st;
	
	XSI::Property prop;
	prop = app.GetActiveSceneRoot().GetProperties().GetItem( L"WeightImportProperty" );
	if (!prop.IsValid())
		prop = app.GetActiveSceneRoot().AddProperty( L"WeightImportProperty" ) ;
	
	XSI::CValueArray args(5);
	args[0] = prop;
	args[1] = L"";
	args[2] = L"WeightImportProperty";
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
				XSILogMessage ("Invalid selection. Please select exactly 1 polymesh on which you want the weights to be transfered.",XSI::siErrorMsg  );
				return;
			}
			
			XSI::SIObject obj(sel[0]);
			XSI::X3DObject	myX3d ( obj );
			
			if ( !myX3d.IsValid() )
			{
				XSILogMessage ("Invalid selection. not a polymesh",XSI::siErrorMsg  );
				return;
			}
			
			XSI::Primitive myPrim;
			myPrim = myX3d.GetActivePrimitive();
			
			CRefArray cra = myX3d.GetPrimitives();
			int count = cra.GetCount();
			
			if ( !myPrim.IsValid() )
			{
				XSILogMessage ("Invalid selection. not a polymesh",XSI::siErrorMsg );
				return;
			}
			
			XSI::Geometry myGeo;
			myGeo = myPrim.GetGeometry();
			
			if ( !myGeo.IsValid() )
			{
				XSILogMessage ("Invalid selection. not a polymesh",XSI::siErrorMsg );
				return;
			}
			
			CRefArray allObjects;
			CStringArray emptyArray;
			XSI::Model root( app.GetActiveSceneRoot() );
			allObjects += root.FindChildren( L"", L"", emptyArray, true );
			
			//
			// parse the file
			//
			
			char l_szString[1024];
			FILE* fp = fopen ( g_sOptions.m_szFilename, "rt" );
			fscanf ( fp,"%s",l_szString );
			
			if ( strcmp ( l_szString, "XSIWM0100") )
			{
				XSILogMessage ("Unrecognized file format",XSI::siErrorMsg );
				fclose(fp);
				return;
			}
			
			fscanf ( fp,"%s",l_szString );
			
			long numVertex = atoi ( l_szString );
			float x,y,z;
			
			CSIBCArray<CSIBCVector3D>	l_pVertexList;
			CSIBCArray<BoneInfo>		l_pBoneList;
			
			l_pVertexList.Extend(numVertex);
			for (long ii=0;ii<numVertex;ii++)
			{
				fscanf ( fp,"%f", &x );
				fscanf ( fp,"%f", &y );
				fscanf ( fp,"%f", &z );
				
				l_pVertexList[ii].Set ( x,y,z );
			}
			
			fscanf ( fp,"%s",l_szString );
			
			long numBones = atoi ( l_szString );
			
			for (long bb=0;bb<numBones;bb++)
			{
				fscanf ( fp,"%s",l_szString );	// bone name
				
				//
				// find it here
				//
				
				const XSI::CRef*	l_pCRef = GetCRefByName ( allObjects, l_szString );
				
				BoneInfo* l_pBoneInfo = NULL;
				
				if ( l_pCRef != NULL )
				{
					l_pBoneList.Extend(1);
					l_pBoneInfo = &l_pBoneList[l_pBoneList.GetUsed()-1];
					l_pBoneInfo->m_pRef = l_pCRef;
				} else {
					
					char mess[256];
					sprintf ( mess, "Deformer %s not found in scene. New weight map will be incomplete", l_szString );
					XSILogMessage (mess,XSI::siWarningMsg );
				}
				
				fscanf ( fp,"%s",l_szString );	// nbWeights
				long numWeights = atoi ( l_szString );
				
				for (long ww=0;ww<numWeights;ww++)
				{
					float weight;
					fscanf ( fp,"%f",&weight);
					
					if ( l_pBoneInfo != NULL )
					{
						l_pBoneInfo->m_aWeights.Extend(1);
						l_pBoneInfo->m_aWeights[l_pBoneInfo->m_aWeights.GetUsed()-1] = weight;
					}
					
				}
				
			}
			
			fclose(fp);
			
			//
			// Apply the envelope here
			//
			
			CRefArray deformers;
			
			for (int d=0;d<l_pBoneList.GetUsed();d++)
			{
				XSI::SIObject obj(*l_pBoneList[d].m_pRef);
				XSI::X3DObject	X3d ( obj );
				
				deformers.Add ( X3d );
			}
			
			XSI::Envelope myEnvelope;
			myX3d.ApplyEnvelope(	deformers, XSI::siUnspecified, XSI::siUnspecified, myEnvelope );
			
			//
			// Build a table of indices that maps this X3DObject vertex positions with the ones specified in the file.
			//
			
			CSIBCArray<int>	l_pIndexMap;
			
			long XSInumpoints = myGeo.GetPoints().GetCount();
			
			XSI::CPointRefArray points( myGeo.GetPoints() );
			
			for (long p=0;p<XSInumpoints;p++)
			{
				XSI::Point point(points.GetItem(p) );
				XSI::MATH::CVector3 pos( point.GetPosition() );
				
				float x = pos.GetX();
				float y = pos.GetY();
				float z = pos.GetZ();
				
				//
				// find the matching vertex in the list imported from the file
				//
				
				int l_iIndex = FindClosestPosition ( x,y,z, &l_pVertexList );
				
				l_pIndexMap.Extend(1);
				l_pIndexMap[l_pIndexMap.GetUsed()-1] = l_iIndex;
				
			}
			
			//
			// Set the weights
			//
			
			CSIBCArray<float>	totalWeighted;
			totalWeighted.Reserve ( XSInumpoints );
			
			int zz;
			for (zz=0;zz<totalWeighted.GetUsed();zz++)
			{
				totalWeighted[zz] = 0.0f;
			}
			
			int c;
			
			for (c=0;c<deformers.GetCount();c++)
			{
				
				XSI::X3DObject	X3d = deformers[c];
				
				CDoubleArray refWeights = myEnvelope.GetDeformerWeights( X3d, 0.0 );
				
				for (int wc=0;wc<refWeights.GetCount();wc++)
				{
					refWeights[wc] = 0.0;
				}
				
				myEnvelope.SetDeformerWeights( X3d, refWeights );
			}
			
			float debugfloat = 0.0f;
			
			for (c=0;c<deformers.GetCount();c++)
			{
				XSI::X3DObject	X3d = deformers[c];
				
				CDoubleArray refWeights = myEnvelope.GetDeformerWeights( X3d, 0.0 );
				
				BoneInfo* l_pBoneInfo;
				l_pBoneInfo = &l_pBoneList[c];
				
				XSI::CString l_szDeformerName;
				l_szDeformerName = X3d.GetFullName();
				char *l_szPrimitiveName = new char [ l_szDeformerName.Length() + 1 ];
				DSW2AHelper ( l_szPrimitiveName, l_szDeformerName.GetWideString() );
				
				for (int wc=0;wc<refWeights.GetCount();wc++)
				{
					int refIndex = l_pIndexMap[wc];
					float ww = l_pBoneInfo->m_aWeights[refIndex];
					refWeights[wc] = ww;
					
					if ( wc == 2305 )
					{
						char mess[1024];
						sprintf ( mess, "%s\t%f\n",l_szPrimitiveName,ww );
						
						debugfloat += ww;
						OutputDebugString ( mess );
					}
				}
				
				delete [] l_szPrimitiveName;
				
				myEnvelope.SetDeformerWeights( X3d, refWeights );
			}
			/*
			int zzz;
			for (zzz=0;zzz<deformers.GetCount();zzz++)
			{
			XSI::X3DObject	X3d = deformers[zzz];
			CDoubleArray refWeights = myEnvelope.GetDeformerWeights( X3d, 0.0 );
			for (int ccc=0;ccc<totalWeighted.GetUsed();ccc++)
			{
			totalWeighted[ccc] += refWeights[ccc];
			}
			}
			
			  CClusterPropertyElementArray weights = myEnvelope.GetWeights( 0.0);
			  CDoubleArray weightArray = weights.GetArray();
			  
				wchar_t msg[30];
				long weightArrayCount = weightArray.GetCount();
				long deformerCount = weights.GetValueSize();
				
				  for (int ff=0;ff<totalWeighted.GetUsed();ff++)
				  {
				  if ( ( 100.0f - totalWeighted[ff] ) > 0.00001f )
				  {
				  int refIndex = l_pIndexMap[ff];
				  
					for (int d=0;d<l_pBoneList.GetUsed();d++)
					{
					BoneInfo* l_pBoneInfo;
					l_pBoneInfo = &l_pBoneList[d];
					
					  float weight = l_pBoneInfo->m_aWeights[refIndex];
					  
						int i= ff * deformers.GetCount();
						int j = d;
						
						  double oldvalue = weightArray[ i + j ];
						  weightArray[ i + j ] = weight;
						  }
						  }
						  }
						  
							weights.PutArray ( weightArray );
			*/
			
		}
		
	}
}
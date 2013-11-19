// SMDExport.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"
#include <stdio.h>
#include "Commdlg.h"
#include <string.h>
#include <assert.h>

#include <comdef.h>					// _variant_t
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
#include <xsi_polygonface.h>
#include <xsi_vertex.h>
#include <xsi_kinematics.h>
#include <xsi_kinematicstate.h>
#include <xsi_edge.h>

#include <math.h>
#include <SIBCArray.h>

#include "resource.h"

#define	MAX_NUM_POLYGON_VERTS	6
#define	MAX_BRUSH_PLANES	(MAX_NUM_POLYGON_VERTS + 2)


#define vec3	XSI::MATH::CVector3
#define vec2	XSI::MATH::CVector3

typedef struct plane_t
{
	vec3		points[3];
	vec3		normal;

	char		shader[32];

//	vec3		texMatrix[2];
	vec2		translate;
	float		rotate;
	vec2		scale;

	int			flags;
	float		unused;
	float		unused2;
} plane_t;


typedef struct brush_t
{
	char		description[16];

	// max of 6 planes
	int			numPlanes;
	plane_t		planes[MAX_BRUSH_PLANES];
} brush_t;

extern void W2AHelper( LPSTR out_sz, LPCWSTR in_wcs, int in_cch );
void getNormalOfTriangle( vec3 p1, vec3 p2, vec3 p3, vec3& normal );

using namespace XSI;
using namespace MATH;

HINSTANCE	___gParent = NULL;
HINSTANCE	___gThis = NULL;

char	ROOT_TOKEN[256];
char	EFF_TOKEN[256];

void	XSILogMessage ( wchar_t *in_szMessage, XSI::siSeverityType in_eSeverity );
BOOL convertPolygonToBrush( XSI::PolygonMesh mesh, XSI::PolygonFace Poly, brush_t *brush );
bool IsConvex ( XSI::PolygonMesh mesh );
bool IsClosed ( XSI::PolygonMesh mesh );

void A2WHelper( wchar_t* out_wcs, const char* in_sz, int in_cch = -1 );
void DoIt ();

char g_szFilename[MAX_PATH];

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

bool CheckConvex = false;
bool CheckClosed = false;

namespace XSI
{
#ifdef unix
extern "C"
#endif
XSIPLUGINCALLBACK CStatus XSILoadPlugin( XSI::PluginRegistrar& in_reg )
{
	in_reg.PutAuthor( L"Softimage Corp" );
	in_reg.PutName( L"VMF Export Plug-in" );	in_reg.PutVersion( 1, 0 );

	// register the commands plugin item.
	in_reg.RegisterCommand( L"VMFExport", L"VMFExport" );
	in_reg.RegisterCommand( L"ShowConcaveEdges", L"ShowConcaveEdges" );
	// register property
	in_reg.RegisterProperty( L"VMFExportProperty" );	//
	// register the menu for the import and export
	in_reg.RegisterMenu(siMenuMainFileExportID, L"VMFExportMenu", false, false);

	return XSI::CStatus::OK;	
}

#ifdef unix
extern "C"
#endif
XSIPLUGINCALLBACK XSI::CStatus VMFExportMenu_Init( XSI::CRef& in_ref )
{
	Context ctxt = in_ref;
	Menu menu = ctxt.GetSource();

	CStatus st;
	MenuItem item;
	menu.AddCallbackItem(L"Export VMF...", L"OnVMFExportMenu", item);

	return CStatus::OK;	
}


void	SMDExport();


#ifdef unix
extern "C"
#endif
XSIPLUGINCALLBACK XSI::CStatus OnVMFExportMenu( XSI::CRef& in_ref )
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
XSIPLUGINCALLBACK CStatus VMFExport_Init( const XSI::CRef& in_context )
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
XSIPLUGINCALLBACK CStatus VMFExport_Execute( XSI::CRef& in_context )
{
	SMDExport();
	return CStatus::OK;
}



void	SMDExport()
{

	Application app;
	//
	//
	//
#ifdef SI_DEMO_VERSION
//#if	SI_DEMO_VERSION
	// Check for the MOD registration key
	if (!ValidateRegistrationCurrentKey(L"ValveSource")) {
		// Was not register or key is invalid. Ask for email + key
		if (!ValidateRegistrationNewKey(L"ValveSource",L"ValveSource",L"Welcome to blah", L"ValveSource Activation" )) {
		// [RLB]	return( E_FAIL );
		}
	}
#endif

//#endif

	CStatus	st;

	Property prop;
	prop = app.GetActiveSceneRoot().GetProperties().GetItem( L"VMFExportProperty" );
	if (!prop.IsValid())
		prop = app.GetActiveSceneRoot().AddProperty( L"VMFExportProperty" ) ;

	CValueArray args(5);
	args[0] = prop;
	args[1] = L"";
	args[2] = L"VMFExportProperty";
	args[3] = (long)4;
	args[4] = true;
	CValue ret;
	st = app.ExecuteCommand(L"InspectObj",args,ret);
	if ( CStatus::OK == st ) {
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
		parm = prop.GetParameters().GetItem(L"Convex");
		CheckConvex = (bool) parm.GetValue();

		parm = prop.GetParameters().GetItem(L"Closed");
		CheckClosed = (bool) parm.GetValue();

		//
		// Now call what we need
		//
		if (g_szFilename && strlen( g_szFilename ) > 0)
		{
			DoIt ();
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
XSIPLUGINCALLBACK CStatus VMFExportProperty_Define( const CRef & in_Ctx )
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

	prop.AddParameter(	L"Convex",CValue::siBool, caps, 
						L"Check Convexity", L"", 
						dft, param ) ;	

	dft = true;

	prop.AddParameter(	L"Closed",CValue::siBool, caps, 
						L"Ensure Meshes are Closed", L"", 
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
XSIPLUGINCALLBACK CStatus VMFExportProperty_DefineLayout( const CRef & in_Ctx )
{
	PPGLayout oLayout = Context( in_Ctx ).GetSource() ;

	

	return RebuildLayout( oLayout );
}

CStatus	RebuildLayout( PPGLayout & oLayout )
{
	PPGItem item ;

	oLayout.Clear() ;


	item = oLayout.AddItem( L"VMFExportProperty" ) ;

	oLayout.AddGroup(L"File") ;
		oLayout.AddRow() ;
				
		item = oLayout.AddItem( L"Filename",L"Export File",siControlFilePath ) ;
			item.PutAttribute( siUINoLabel, true ) ;
			item.PutAttribute( siUIFileFilter, L"Hammer VMF Files (*.vmf)|*.vmf|All Files (*.*)|*.*||" ) ;
		oLayout.EndRow() ;
	oLayout.EndGroup() ;

	oLayout.AddGroup( L"Validity Checks" ) ;
		oLayout.AddRow();
			item = oLayout.AddItem( L"Convex" ) ;
		oLayout.EndRow();
		oLayout.AddRow();
			item = oLayout.AddItem( L"Closed" );
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


void	XSILogMessage ( wchar_t *in_szMessage, XSI::siSeverityType in_eSeverity )
{
	XSI::Application	app;
	app.LogMessage( in_szMessage, in_eSeverity ) ;


}

char * formatstring ( char *one, char *two )
{
	static char buf[MAX_PATH];

	char d = '"';
	memset (buf,0,MAX_PATH);
	sprintf ( buf,"%c%s%c %c%s%c",d,one,d, d,two,d );
	return buf;
}

void DoIt ()
{
	float brushThickness = 8.0f;
	float brushScale = 64.0f;
	bool  isModTool = false;
	int	  groupID = 1;

	using namespace XSI;

	Application app;

	CRefArray l_Objects = app.GetActiveSceneRoot().FindChildren ( L"",XSI::siPolyMeshType,CStringArray() );

	if ( !l_Objects.GetCount() )
		return;
	

	long PolygonCount = 0;

	if (app.GetLicense() == L"Mod Tool")
	{
		isModTool = true;
	}

	//
	// output header
	//

	FILE *f;

	f = fopen ( g_szFilename, "wt" );

	fprintf ( f, "versioninfo\n{\n");
	fprintf ( f, "\t%s\n", formatstring ("editorversion","400"));
	fprintf ( f, "\t%s\n", formatstring ("editorbuild","2637"));
	fprintf ( f, "\t%s\n", formatstring ("mapversion","1"));
	fprintf ( f, "\t%s\n", formatstring ("prefab","0"));
	fprintf ( f, "}\n");

	fprintf ( f, "viewsettings\n{\n");
	fprintf ( f, "\t%s\n", formatstring ("bSnapToGrid","1"));
	fprintf ( f, "\t%s\n", formatstring ("bShowGrid","1"));
	fprintf ( f, "\t%s\n", formatstring ("nGridSpacing","32"));
	fprintf ( f, "\t%s\n", formatstring ("bShow3DGrid","0"));
	fprintf ( f, "}\n");

	fprintf ( f, "world\n{\n");
	fprintf ( f, "\t%s\n", formatstring ("id","0"));
	fprintf ( f, "\t%s\n", formatstring ("mapversion","1"));
	fprintf ( f, "\t%s\n", formatstring ("classname","worldspawn"));
	fprintf ( f, "\t%s\n", formatstring ("skyname","sky_wasteland02"));

	//
	// output solids
	//

	int id = 1;
	CSIBCArray<XSI::Model> groups;

	for (int s=0;s<l_Objects.GetCount();s++)
	{
		X3DObject x3d ( l_Objects[s] );

		XSI::Model parentModel = x3d.GetModel();

		int foundID = -1;

		if ( parentModel != app.GetActiveSceneRoot() )
		{
			// get the group ID for this model
			//
		
			for (int v=0;v<groups.GetUsed();v++)
			{
				if ( groups[v] == parentModel )
				{
					foundID = v;
					break;
				}
			}

			if ( foundID < 0 )
			{
				groups.Extend(1);
				groups[groups.GetUsed()-1] = parentModel;
				foundID = groups.GetUsed()-1;
			}

		}

		Geometry geom(x3d.GetActivePrimitive().GetGeometry());
		PolygonMesh polygonmesh(geom);

		if ( CheckConvex )
		{
			if ( !IsConvex ( polygonmesh ) )
			{
				//
				// concave mesh wont be compatible with Hammer
				//

				XSILogMessage ( (unsigned short*)XSI::CString(XSI::CString ( L"Found concave mesh. Skipping export of ") + x3d.GetFullName()).GetWideString(), XSI::siWarningMsg );
				continue;
			}
		}

		if ( CheckClosed )
		{
			if ( !IsClosed ( polygonmesh ))
			{
				//
				// open mesh wont be compatible with Hammer
				//

				XSILogMessage ( (unsigned short*)XSI::CString(XSI::CString ( L"Found an opened mesh. Skipping export of ") + x3d.GetFullName()).GetWideString(), XSI::siWarningMsg );
				continue;
			}
		}


		CPointRefArray pointRefArray(polygonmesh.GetPoints());

		char IDString[256];
		sprintf ( IDString, "%d", id );
		fprintf ( f, "\tsolid\n\t{\n");
		fprintf ( f, "\t\t%s\n", formatstring ("id",IDString));

		CPolygonFaceRefArray polygons = polygonmesh.GetPolygons();
		
		PolygonCount += polygons.GetCount();

		XSI::MATH::CTransformation flip;
		flip.SetRotationFromXYZAnglesValues ( 90.0 * 0.01745329251994329547, 0.0, 0.0);
		flip.MulInPlace (x3d.GetKinematics().GetGlobal().GetTransform());
		
		if ( (PolygonCount > 5000) && ( isModTool))
		{
			XSILogMessage ( L"Mod Tool polygon count limit reached (5000).", XSI::siWarningMsg);
			break;
		}

		for (int ff=0;ff<polygons.GetCount();ff++)
		{
			PolygonFace face = polygons[ff];

			brush_t brush;
			convertPolygonToBrush ( polygonmesh, face, &brush );

			CVertexRefArray verts = face.GetVertices();
			if ( verts.GetCount() < 3 )
			{
				continue;
			}

			//
			/// output a face
			//

			Vertex v1 = verts[0];
			Vertex v2 = verts[1];
			Vertex v3 = verts[2];
			
			MATH::CVector3 p1 = brush.planes[0].points[0];
			MATH::CVector3 p2 = brush.planes[0].points[1];
			MATH::CVector3 p3 = brush.planes[0].points[2];

			p1 *= flip;
			p2 *= flip;
			p3 *= flip;

			char Vertstring[MAX_PATH];
			sprintf ( Vertstring, "(%f %f %f) (%f %f %f) (%f %f %f)", p1.GetX(),p1.GetY(),p1.GetZ()
				, p2.GetX(),p2.GetY(),p2.GetZ()
				, p3.GetX(),p3.GetY(),p3.GetZ() );
			
			id++;
			sprintf ( IDString, "%d", id );
			
			
			fprintf ( f, "\t\tside\n\t\t{\n");
			fprintf ( f, "\t\t\t%s\n", formatstring ("id",IDString));
			fprintf ( f, "\t\t\t%s\n", formatstring ("plane",Vertstring));
			fprintf ( f, "\t\t\t%s\n", formatstring ("material","BRICK/BRICKFLOOR001A"));
			fprintf ( f, "\t\t\t%s\n", formatstring ("uaxis","[1 0 0 0] 0.25"));
			fprintf ( f, "\t\t\t%s\n", formatstring ("vaxis","[0 -1 0 0] 0.25"));
			fprintf ( f, "\t\t\t%s\n", formatstring ("rotation","0"));
			fprintf ( f, "\t\t\t%s\n", formatstring ("lightmapscale","16"));
			fprintf ( f, "\t\t\t%s\n", formatstring ("smoothing_groups","0"));
			fprintf ( f, "\t\t}\n");
			
			
		}
		
		if ( foundID >= 0 )
		{
			//
			// output group info
			//
			
			char grouptext[256];
			sprintf ( grouptext, "%d", foundID+1 );

			fprintf ( f, "\t\teditor\n\t\t{\n");
			fprintf ( f, "\t\t\t%s\n", formatstring ("color","186 239 0"));
			fprintf ( f, "\t\t\t%s\n", formatstring ("groupid",grouptext));
			fprintf ( f, "\t\t\t%s\n", formatstring ("visgroupshown","1"));
			fprintf ( f, "\t\t}\n");


		}



		fprintf ( f, "\t}\n");


	}

	//
	// output group info
	//

	for (int g=0;g<groups.GetUsed();g++)
	{
		char idstr[256];
		sprintf ( idstr, "%d", g+1 );

		fprintf ( f, "\tgroup\n\t{\n");
		fprintf ( f, "\t\t%s\n", formatstring ("id",idstr));
			fprintf ( f, "\t\teditor\n\t\t{\n");
			fprintf ( f, "\t\t\t%s\n", formatstring ("color","186 239 0"));
			fprintf ( f, "\t\t\t%s\n", formatstring ("visgroupshown","1"));
			fprintf ( f, "\t\t}\n");
		fprintf ( f, "\t}\n");

	}


	fprintf ( f, "}\n");
	
	fclose ( f );
}
//	"editorbuild" "2637"
//	"mapversion" "1"
//	"prefab" "0"
	


BOOL convertPolygonToBrush( XSI::PolygonMesh mesh, XSI::PolygonFace Poly, brush_t *brush )
{

	float brushThickness = 8.0f;
	float brushScale = 1.0f;

	int		i;

	vec3	polyNormal;
	vec3	sides[MAX_NUM_POLYGON_VERTS];

	vec3	thickness;


	// TODO: decription... dont really need it but it would be cool if it was there
	brush->description[0] = 0;
	
	CVertexRefArray verts = Poly.GetVertices();
	
	if( MAX_BRUSH_PLANES < verts.GetCount() + 2 )
	{
		XSILogMessage ( L"Too many vertices to convert a polygon to a brush.\n" , XSI::siErrorMsg );
		return FALSE;
	}

	CPointRefArray allPoints = mesh.GetPoints();
	MATH::CVector3Array allposarray = allPoints.GetPositionArray();


	brush->numPlanes = verts.GetCount() + 2;


	CPointRefArray polyPoints = Poly.GetPoints();

	for( i = 0; i < verts.GetCount(); i++ )
	{
		int index1 = polyPoints.GetIndexArray()[i];
		int index2 = polyPoints.GetIndexArray()[(i+1)%verts.GetCount()];

		vec3 v1 = allposarray[index1];
		vec3 v2 = allposarray[index2];

		v1 -= v2;
		sides[i] = v1;
	}

	getNormalOfTriangle( allposarray[polyPoints.GetIndexArray()[0]], 
						allposarray[polyPoints.GetIndexArray()[1]], 
						allposarray[polyPoints.GetIndexArray()[2]], 
						polyNormal );

	thickness.Scale ( -brushThickness, polyNormal );

	brush->planes[0].points[0].Scale ( brushScale, allposarray[polyPoints.GetIndexArray()[2]] );
	brush->planes[0].points[1].Scale ( brushScale, allposarray[polyPoints.GetIndexArray()[1]] );
	brush->planes[0].points[2].Scale ( brushScale, allposarray[polyPoints.GetIndexArray()[0]] );

	brush->planes[0].normal = polyNormal;

	brush->planes[1].points[0].Add (brush->planes[0].points[2], thickness);
	brush->planes[1].points[1].Add (brush->planes[0].points[1], thickness);
	brush->planes[1].points[2].Add (brush->planes[0].points[0], thickness);
	
	brush->planes[1].normal.Scale ( -1, polyNormal );

	for( i = 2; i < brush->numPlanes; i++ )
	{
		brush->planes[i].normal.Cross ( sides[i-2], polyNormal);

		brush->planes[i].points[0].Scale ( brushScale, allposarray[polyPoints.GetIndexArray()[i-2]]);
		brush->planes[i].points[1].Scale ( brushScale, allposarray[polyPoints.GetIndexArray()[(i-1)%verts.GetCount()]] );

		brush->planes[i].points[2].ScaleAdd ( brushScale, allposarray[polyPoints.GetIndexArray()[(i-1)%verts.GetCount()]], thickness );

		//vecMA( thickness, brushScale, mesh->verts[poly->vertIndices[(i-1)%poly->numVerts]], brush->planes[i].points[2] );
	//	brush->planes[i].scale[0] = 0.5f;
	//	brush->planes[i].scale[1] = 0.5f;
	}

	return TRUE;
}


void getNormalOfTriangle( vec3 p1, vec3 p2, vec3 p3, vec3& normal )
{
	vec3	side1, side2;

	side1.Sub (p1, p2);
	side2.Sub (p3, p1);
	
	
	normal.Cross( side1, side2);
	normal.NormalizeInPlace();

}

bool IsClosed ( XSI::PolygonMesh mesh )
{
	using namespace XSI;
	
	CEdgeRefArray edgeRefArray(mesh.GetEdges());

	for (int e=0;e<edgeRefArray.GetCount();e++)
	{
		Edge edge = edgeRefArray[e];

		if ( edge.GetNeighborPolygons().GetCount() == 1 )
		{
			//
			// found border edge
			//

			return false;
		}

	}
	
	return true;

}

bool IsConvex ( XSI::PolygonMesh mesh )
{
	using namespace XSI;
	
	CPointRefArray pointRefArray(mesh.GetPoints());
	CPolygonFaceRefArray polygons = mesh.GetPolygons();
	long PolygonCount = polygons.GetCount();

	enum PlaneSide
	{
		onplane,
		negative,
		positive
	};

	for (int p=0;p<PolygonCount;p++)
	{
		PolygonFace poly = polygons[p];
		CVertexRefArray verts = poly.GetVertices();

		vec3 normal;

		Vertex v1 = verts[0];
		Vertex v2 = verts[1];
		Vertex v3 = verts[2];


		getNormalOfTriangle ( v1.GetPosition(), v2.GetPosition(), v3.GetPosition(), normal);

		float tuple[4];
		tuple[0] = normal.GetX();
		tuple[1] = normal.GetY();
		tuple[2] = normal.GetZ();
		tuple[3] = normal.Dot(v1.GetPosition());

		for (long o=0;o<pointRefArray.GetCount();o++)
		{
			Point pnt = pointRefArray[o];

			vec3 pos = pnt.GetPosition();
		
			// get the distance
			float distance = tuple[0]*pos.GetX() + tuple[1]*pos.GetY() + tuple[2]*pos.GetZ() - tuple[3];

			PlaneSide side = onplane;	// je suis optimiste un ti peu.

			
			if ( distance > 0.0f )
			{
				side = positive;
			} else {
				
				if ( fabs ( distance ) > 0.001f )
				{
					side = negative;

					return false;
				}
			}

		}

	}

	return true;
}


#ifdef unix
extern "C"
#endif
XSIPLUGINCALLBACK CStatus ShowConcaveEdges_Init( const XSI::CRef& in_context )
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
XSIPLUGINCALLBACK CStatus ShowConcaveEdges_Execute( XSI::CRef& in_context )
{
	using namespace XSI;

	Application app;
	

	XSI::CString l_szGlobalSelection;

	if ( !app.GetSelection().GetCount() )
	{
		app.LogMessage ( L"Please select one or more polymesh before.", XSI::siErrorMsg );
		return CStatus::OK;
	}

	long l_lTotalFound = 0;

	for (int s=0;s<app.GetSelection().GetCount();s++)
	{
		X3DObject x3d = app.GetSelection()[s];
		Geometry geom(x3d.GetActivePrimitive().GetGeometry());
		PolygonMesh polygonmesh(geom);
	
		CEdgeRefArray edges(polygonmesh.GetEdges());
		
		LONG l_lNbEdges = edges.GetCount();
		
		
		LONG l_lCWPoly, l_lCCWPoly;
		BOOL l_bFoundConcaveEdge = FALSE;

		CSIBCArray<int> l_iEdgeList;
		
		for( LONG i = 0; i < l_lNbEdges; ++i )
		{
			Edge l_currentedge = edges[i];

			CPolygonFaceRefArray l_fNeighborPolygons = l_currentedge.GetNeighborPolygons();

			if ( l_fNeighborPolygons.GetCount() == 1 )
			{
				// border edge
				continue;
			}

			CVertexRefArray vertarray = l_currentedge.GetVertices();

			assert ( vertarray.GetCount() == 2 );

			MATH::CVector3 p1 = ((Vertex)vertarray[0]).GetPosition();
			MATH::CVector3 p2 = ((Vertex)vertarray[1]).GetPosition();

			p2 -= p1;

			p2.NormalizeInPlace();

			MATH::CVector3 n1;
			MATH::CVector3 n2;
			
			getNormalOfTriangle( ((Vertex)((PolygonFace)l_fNeighborPolygons[0]).GetVertices()[0]).GetPosition(),
						((Vertex)((PolygonFace)l_fNeighborPolygons[0]).GetVertices()[1]).GetPosition(),
						((Vertex)((PolygonFace)l_fNeighborPolygons[0]).GetVertices()[2]).GetPosition(),
						n1 );

			getNormalOfTriangle( ((Vertex)((PolygonFace)l_fNeighborPolygons[1]).GetVertices()[0]).GetPosition(),
						((Vertex)((PolygonFace)l_fNeighborPolygons[1]).GetVertices()[1]).GetPosition(),
						((Vertex)((PolygonFace)l_fNeighborPolygons[1]).GetVertices()[2]).GetPosition(),
						n2 );

			MATH::CVector3 cross;

			cross.Cross ( n1, n2 );

			double dot = cross.Dot ( p2 );

			if ( dot < 0.000001 )
			{	
				l_iEdgeList.Extend(1);
				l_iEdgeList[l_iEdgeList.GetUsed()-1] = i;

				l_lTotalFound++;

			}

		}

		if ( l_iEdgeList.GetUsed() )
		{
			l_szGlobalSelection += x3d.GetFullName();
			l_szGlobalSelection += L".edge[";

			wchar_t mess[2048];

			for (int v=0;v<l_iEdgeList.GetUsed();v++)
			{
				if ( v == l_iEdgeList.GetUsed()-1 )
				{
					swprintf ( mess, L"%d", l_iEdgeList[v] );
				} else {
					swprintf ( mess, L"%d,", l_iEdgeList[v] );
				}
				l_szGlobalSelection += mess;
			}

			l_szGlobalSelection += L"],";

		}

		
	}

	if ( l_szGlobalSelection.Length () > 0 )
	{
		CValueArray args(1);
		CValue retval;
		args[0]= l_szGlobalSelection;

		CStatus st = app.ExecuteCommand( L"SelectGeometryComponents", args, retval );

		char mess[2048];

		sprintf ( mess, " Total concave edges found: %d", l_lTotalFound );

		
		MessageBox ( NULL, mess, "ShowConcaveEdges", MB_OK ); 
	}


	
	return CStatus::OK;
}
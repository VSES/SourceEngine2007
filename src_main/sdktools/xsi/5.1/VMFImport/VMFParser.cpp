// MapParser.cpp: implementation of the CMapParser class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "VMFParser.h"
#include <assert.h>

#include <SIBCUtil.h>
#include <Scene.h>
#include <Angle.h>
#include <CoordinateSystem.h>
#include <FileInfo.h>
#include <SceneInfo.h>
#include <Model.h>
#include <Visibility.h>
#include <GlobalMaterial.h>
#include <Transform.h>
#include <MaterialLibrary.h>
#include <Material.h>
#include <Mesh.h>
#include <PolygonList.h>

#include "VMFTypes.h"
#include "VMFTemplate.h"
#include "VMFMaps.h"

#include <comdef.h>

#include <si3dobjectmodel_id.c>		// IID_SIObject
#include <xsisdk.h>

#include <xsi_application.h>
#include <xsi_model.h>

#include <xsi_ref.h>
#include <xsi_string.h>
#include <xsi_value.h>
#include <xsi_status.h>
#include <xsi_comapihandler.h>
#include <xsi_kinematicstate.h>
#include <xsi_kinematics.h>

void SetXSIValue ( char * in_szValueName , long in_lValue );
void ModelCount ( CSLModel* in_pModel,  int& in_iModels );
void CountTemplate ( VMFTemplate * in_pTemplate, int* count );
int g_count = 0;
extern int		g_iImportMaterials;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMapParser::CMapParser()
{

}

CMapParser::~CMapParser()
{
	for (int t=0;t<m_pTemplateList.GetUsed();t++)
	{
		VMFTemplate* l_pCurrent = m_pTemplateList[t];
		delete l_pCurrent;
	}


	m_pMaterialList.DisposeData();
	m_pTemplateList.DisposeData();


}

SI_Error CMapParser::Read ( SI_Char* in_szFilename )
{
	if ( !_SI_STRLEN (in_szFilename) )
		return SI_ERR_FILENOTFOUND;
	
	_SI_FILE	l_fptr = _SI_FOPEN ( in_szFilename, _SI_FILE_READ_BINARY );

	if ( !l_fptr )
		return SI_ERR_FILENOTFOUND;

	char l_szDrive[MAX_PATH];
	char l_szDirectory[MAX_PATH];

	_splitpath ( in_szFilename, l_szDrive, l_szDirectory, NULL, NULL );

	m_szPath = l_szDrive;
	m_szPath.Concat ( l_szDirectory );


	fseek(l_fptr, 0, SEEK_END );

	long fl = ftell ( l_fptr );

	fseek (l_fptr, 0,SEEK_SET);
	
	char *buffer = new char [fl + 5];

	fread (buffer,fl,sizeof(char),l_fptr);

	buffer[fl] = 'E';
	buffer[fl+1] = 'N';
	buffer[fl+2] = 'D';
	buffer[fl+3] = 0;

	_SI_FCLOSE ( l_fptr );

	//
	// Build templates
	//

	char seps[] = " \t\n\r";

	char *token = strtok (buffer, seps);

	while ( _SI_STRCMP ( token, "END" ) )
	{ 
		if ( ( token[0] != '\34' ) && ( token[0] != BEGIN_SECTION ) && ( token[0] != END_SECTION ) )
		{
			VMFTemplate*	l_pNewTemplate = new VMFTemplate;

			m_pTemplateList.Extend(1);
			m_pTemplateList[m_pTemplateList.GetUsed()-1] = l_pNewTemplate;

			l_pNewTemplate->SetName ( token );
			l_pNewTemplate->Read ( seps );
		}

		token = strtok (NULL, seps);

	}


	delete [] buffer;

//	ConvertEntityList();

	return SI_SUCCESS;
}

SI_Error CMapParser::BuildEntity ( _SI_FILE l_fptr, CEntity* in_pEntity )
{

	int c = _SI_FGETC ( l_fptr );
	
	while ( c != END_SECTION )
	{
		if ( c == BEGIN_INFO )
		{
			//
			// Found a property
			//

			CSIBCString	l_szName;
			CSIBCString	l_szValue;

			if ( ReadProperty ( l_fptr, l_szName, l_szValue ) == SI_SUCCESS )
			{
				in_pEntity->AddProperty ( l_szName, l_szValue );
			
			} else {

				return SI_ERR_ERROR_MSG;
			}
		} 

		if ( c == BEGIN_SECTION )
		{
			//
			// Found a brush
			//

			if ( ReadBrush  ( l_fptr, in_pEntity ) != SI_SUCCESS )
			{
				return SI_ERR_ERROR_MSG;
			} 

		}

		c = _SI_FGETC ( l_fptr );

	}

	return SI_SUCCESS;
}

SI_Error CMapParser::ReadProperty		( _SI_FILE l_fptr, CSIBCString& in_szName, CSIBCString& in_szValue)
{
	SI_Char c = _SI_FGETC ( l_fptr );
	
	//
	// Read in the name of the value
	//

	char	cc[2];
	cc[0] = cc[1] = 0;

	while ( c != BEGIN_INFO )
	{
		cc[0] = c;
		in_szName.Concat ( cc );
		c = _SI_FGETC ( l_fptr );
	}

	//
	// Skip to the value
	//

	c = _SI_FGETC ( l_fptr );
	while ( c != BEGIN_INFO )
	{
		c = _SI_FGETC ( l_fptr );
	}

	//
	// Now read the value
	//

	c = _SI_FGETC ( l_fptr );
	while ( c != BEGIN_INFO )
	{
		cc[0] = c;
		in_szValue.Concat ( cc );
		c = _SI_FGETC ( l_fptr );
	}

	return SI_SUCCESS;
}

SI_Error CMapParser::ReadBrush  ( _SI_FILE l_fptr, CEntity* in_pEntity )
{

	in_pEntity->BeginBrush();

	while ( !_SI_FEOF(l_fptr))
	{
		face_t	l_face;
		memset ( &l_face, 0, sizeof (face_t));

		//
		// Read the plane
		//

		l_face.planepts[0][0] = (float) GetNextInteger ( l_fptr );
		l_face.planepts[0][1] = (float) GetNextInteger ( l_fptr );
		l_face.planepts[0][2] = (float) GetNextInteger ( l_fptr );

		l_face.planepts[1][0] = (float) GetNextInteger ( l_fptr );
		l_face.planepts[1][1] = (float) GetNextInteger ( l_fptr );
		l_face.planepts[1][2] = (float) GetNextInteger ( l_fptr );

		l_face.planepts[2][0] = (float) GetNextInteger ( l_fptr );
		l_face.planepts[2][1] = (float) GetNextInteger ( l_fptr );
		l_face.planepts[2][2] = (float) GetNextInteger ( l_fptr );

		//
		// Read the texture name
		//

		GetNextString ( l_fptr, l_face.texture.texture );

		//
		// get an unknown vector
		//

		float l_fFloat;
		l_fFloat = GetNextFloat  ( l_fptr );
		l_fFloat = GetNextFloat  ( l_fptr );
		l_fFloat = GetNextFloat  ( l_fptr );

		//
		// Get texture offset in X
		//

		l_face.texture.shift[0] = (float)GetNextFloat  ( l_fptr );

		//
		// get an unknown vector
		//

		l_fFloat = (float)GetNextFloat  ( l_fptr );
		l_fFloat = (float)GetNextFloat  ( l_fptr );
		l_fFloat = (float)GetNextFloat  ( l_fptr );

		//
		// Get texture offset in Y
		//

		l_face.texture.shift[1] = (float)GetNextFloat  ( l_fptr );
		
		//
		// Get angle
		//

		l_face.texture.rotate = (float)GetNextFloat  ( l_fptr );
		
		//
		// Get scaling
		//

		l_face.texture.scale[0] = (float)GetNextFloat  ( l_fptr );
		l_face.texture.scale[1] = (float)GetNextFloat  ( l_fptr );
		

		//
		// Add the face to the current brush in the entity
		//

		in_pEntity->AddFace ( l_face );

		//
		// Forward to next line, or to end of section
		//

		int c;

		while ( !_SI_FEOF(l_fptr) )
		{

			c = _SI_FGETC ( l_fptr );

			if ( c == END_SECTION )
			{
				break;
			}

			if ( c == BEGIN_BRUSH_LINE )
			{
				break;
			}

		}

		if ( c == END_SECTION )
			break;

	}

	in_pEntity->EndBrush();

	return SI_SUCCESS;
}

SI_Int	 CMapParser::GetNextInteger ( _SI_FILE l_fptr )
{
	char c;
	char cc[2];
	cc[0] = cc[1] = 0;
	CSIBCString	l_szNumber;

	//
	// Find the first character
	//

	while ( !_SI_FEOF ( l_fptr ) )
	{
		c = _SI_FGETC ( l_fptr );

		if (( c == '-' ) || ( isdigit(c) ))
		{
			break;
		}
	}

	//
	// We are ready to read the number
	//

	while ( !_SI_FEOF ( l_fptr ) )
	{
		cc[0] = c;

		l_szNumber.Concat ( cc );

		c = _SI_FGETC ( l_fptr );

		if (( c != '.' ) && ( c != 'e' ) && ( c != '-' ))
		{
			if ( !isdigit(c) )
			{
				break;
			}
		}
	}

	return atoi ( l_szNumber.GetText() );
}

SI_Float	 CMapParser::GetNextFloat ( _SI_FILE l_fptr )
{
	char c;
	char cc[2];
	cc[0] = cc[1] = 0;
	CSIBCString	l_szNumber;

	//
	// Find the first character
	//

	while ( !_SI_FEOF ( l_fptr ) )
	{
		c = _SI_FGETC ( l_fptr );

		if (( c == '-' ) || ( isdigit(c) ))
		{
			break;
		}
	}

	//
	// We are ready to read the number
	//

	while ( !_SI_FEOF ( l_fptr ) )
	{
		cc[0] = c;

		l_szNumber.Concat ( cc );

		c = _SI_FGETC ( l_fptr );

		if (( c != '.' ) && ( c != 'e' ) && ( c != '-' ))
		{
			if ( !isdigit(c) )
			{
				break;
			}
		}
	}

	return (float)atof ( l_szNumber.GetText() );
}


SI_Error CMapParser::GetNextString ( _SI_FILE l_fptr, SI_Char *in_szString )
{
	char c;
	char cc[2];
	cc[0] = cc[1] = 0;
	CSIBCString	l_szTheString;

	//
	// Find the first character
	//

	while ( !_SI_FEOF ( l_fptr ) )
	{
		c = _SI_FGETC ( l_fptr );

		if ( isalpha(c) )
		{
			break;
		}
	}

	//
	// We are ready to read the number
	//

	while ( !_SI_FEOF ( l_fptr ) )
	{
		cc[0] = c;

		l_szTheString.Concat ( cc );

		c = _SI_FGETC ( l_fptr );

		if ( c == ' ' )
		{
			break;
		}
	}

	_SI_STRCPY ( in_szString, l_szTheString.GetText() );

	return SI_SUCCESS;
}

SI_Error CMapParser::ConvertEntityList	()
{
	for (int e=0;e<m_pEntityList.GetUsed();e++)
	{
		CEntity* l_pEntity = m_pEntityList[e];
		l_pEntity->Convert();

	}


	return SI_SUCCESS;
}

SI_Error CMapParser::ConvertToSemanticLayer		()
{
	CSLScene *Scene;
	Scene = new CSLScene;


	// Create a new Scene->
#ifdef DOTXSI35
	Scene->Create("c:\\__tmp.xsi", 3, 50);
#else
	Scene->Create("c:\\__tmp.xsi", 3, 00);
#endif


	//
	// Set default data
	//

	// Angle
	CSLAngle *l_pAngle = Scene->Angle();
	l_pAngle->SetAngleType(CSLAngle::SI_DEGREES);

	// Coordinate system
	Scene->CoordinateSystem()->SetCoordinateSystemStyle(CSLCoordinateSystem::SI_SOFTIMAGE_3D);

	// File Info
	CSLFileInfo *l_pFileInfo = Scene->FileInfo();
	l_pFileInfo->SetOriginator( "Softimage .VMF exporter" );
	l_pFileInfo->SetProjectName( ".VMF Exporter" );
	l_pFileInfo->SetUsername( "dolaflam" );

	// Scene Info
	CSLSceneInfo *l_pSceneInfo = Scene->SceneInfo();
	l_pSceneInfo->SetEnd( 100 );
	l_pSceneInfo->SetStart(0);
	l_pSceneInfo->SetFrameRate(30);
	l_pSceneInfo->SetTimingType(CSLSceneInfo::SI_FRAMES);

	CSLMaterial *l_pMaterial = Scene->GetMaterialLibrary()->AddMaterial ();
	l_pMaterial->SetName("Default");
	l_pMaterial->SetShadingModel ( CSLMaterial::PHONG );
	l_pMaterial->SetAmbientColor ( CSIBCColorf ( 0.3f, 0.3f, 0.3f, 1.0f ) );
	l_pMaterial->SetDiffuseColor ( CSIBCColorf ( 0.7f, 0.7f, 0.7f, 1.0f ) );
	l_pMaterial->SetSpecularColor( CSIBCColorf ( 1.0f, 1.0f, 1.0f, 1.0f ) );
	l_pMaterial->SetSpecularDecay ( 50.0f );
	
	//
	// Create root model (and adjust world axis)
	//

	CSLModel *l_pRootModel = Scene->Root()->AddModel();
	l_pRootModel->SetName("MapRoot");
	l_pRootModel->Visibility()->SetVisibility(SI_TRUE);
	l_pRootModel->Transform()->SetScale(CSIBCVector3D(1.0, 1.0, 1.0) );
	//l_pRootModel->Transform()->SetEulerRotation(CSIBCVector3D(-90.0f, 0.0f, 0.0f) );
	l_pRootModel->Transform()->SetEulerRotation(CSIBCVector3D(0.0f, 0.0f, 0.0f) );
	l_pRootModel->AddGlobalMaterial();
	l_pRootModel->GlobalMaterial()->SetPropagationType(CSLGlobalMaterial::SI_INHERITED);
	l_pRootModel->GlobalMaterial()->SetMaterial(Scene->GetMaterialLibrary()->GetMaterialList()[0] );

	//
	// Create a model for each entity
	//

	RegisterMaps();

	
	XSI::Application		app;
	XSI::Model			root = app.GetActiveSceneRoot();

	XSI::Model myModel;
	XSI::CRefArray emptyModel;
	root.AddModel( emptyModel, L"MapRoot", myModel );

	for (int n=0;n<m_pTemplateList.GetUsed();n++)
	{
		CountTemplate ( m_pTemplateList[n], &g_count );
	}

	XSI::CStatus st;

	XSI::CComAPIHandler xsiuitoolkit;

	xsiuitoolkit.CreateInstance( L"XSI.UIToolkit");

	XSI::CValue retval = xsiuitoolkit.GetProperty( L"ProgressBar" );

	// The retval contains a pointer to the progressbar
	// that has had its reference count incremented, the destructor of CValue 
	// takes care of dereferencing the pointer.
	XSI::CComAPIHandler progressbar( retval ); 	

	// set min/max
	progressbar.PutProperty( L"Minimum", (long)1 );
	progressbar.PutProperty( L"Maximum", (long)100 );
	progressbar.PutProperty( L"Value", (long)1 );
	progressbar.PutProperty( L"Caption", XSI::CValue(L"Importing...") );

	retval = progressbar.GetProperty( L"Value" );
	
	// show the progress bar
	progressbar.PutProperty( L"Visible", true );
/*
	while ( (progressbar.GetProperty( L"CancelPressed" ) == false) && 
		((long)retval < 200))
	{
		// your code here

		progressbar.Call( L"Increment", retval );
	}
*/
	// Just for good measure. If you put break points in the
	// code sometimes the progress dialog doesn't go away.
	progressbar.PutProperty( L"Visible", false );


	for (int c=0;c<m_pTemplateList.GetUsed();c++)
	{
		VMFTemplate *l_pCurrentTemplate = m_pTemplateList[c];

		l_pCurrentTemplate->Convert ( progressbar, retval, myModel, l_pRootModel, c == 0 ? 1 : 0 );

	}

	if ( g_iImportMaterials )
	{
		if ( m_pTemplateList.GetUsed() )
			m_pTemplateList[0]->CreateGroups( myModel );
	

		//
		// assign uvs
		//
/*
		for (int b=0;b<m_pTemplateList.GetUsed();b++)
		{
			VMFTemplate *l_pCurrentTemplate = m_pTemplateList[b];

			l_pCurrentTemplate->AssignUVs ();

		}
		*/
	}

	XSI::KinematicState  globalKinematicState = myModel.GetKinematics().GetGlobal();
	globalKinematicState.PutParameterValue(L"rotx", -90.0);

	//CleanMaterials();

	// Save the file.
	Scene->Write();

	
	// DEBUG
	// Count the number of models
	//

	int models = 0;

	ModelCount ( Scene->Root(), models );

	// Close the file.
	Scene->Close();

	delete Scene;

	return SI_SUCCESS;
}

SI_Error CMapParser::CleanMaterials()
{
	for (int c=0;c<m_pTemplateList.GetUsed();c++)
	{
		VMFTemplate *l_pCurrentTemplate = m_pTemplateList[c];

		CleanMaterial ( l_pCurrentTemplate );

	}
	
	return 0;
}

SI_Error CMapParser::CleanModelMaterial( CSLModel *in_pModel )
{
	CSIBCString l_pName;
	CSIBCString l_pName2;

	l_pName = in_pModel->ParentModel()->GetName();
	l_pName.Concat ( "." );
	l_pName.Concat ( in_pModel->GetName() );
	l_pName.Concat ( ".branchlocal.Material.wrapv" );

	l_pName2 = in_pModel->ParentModel()->GetName();
	l_pName2.Concat ( "." );
	l_pName2.Concat ( in_pModel->GetName() );
	l_pName2.Concat ( ".branchlocal.Material.wrapu" );

	m_pMaterialList.Extend(1);
	m_pMaterialList[m_pMaterialList.GetUsed()-1] = l_pName;
	m_pMaterialList.Extend(1);
	m_pMaterialList[m_pMaterialList.GetUsed()-1] = l_pName2;


	CSLMesh *l_pMesh = (CSLMesh *) in_pModel->Primitive();

	if ( l_pMesh != NULL )
	{
		l_pName = l_pMesh->ParentModel()->ParentModel()->ParentModel()->GetName();
		l_pName.Concat ( "." );
		l_pName.Concat ( l_pMesh->ParentModel()->GetName() );
		l_pName.Concat ( ".branchlocal.Material.wrapv" );

		l_pName2 = l_pMesh->ParentModel()->ParentModel()->ParentModel()->GetName();
		l_pName2.Concat ( "." );
		l_pName2.Concat ( l_pMesh->ParentModel()->GetName() );
		l_pName2.Concat ( ".branchlocal.Material.wrapu" );

		m_pMaterialList.Extend(1);
		m_pMaterialList[m_pMaterialList.GetUsed()-1] = l_pName;
		m_pMaterialList.Extend(1);
		m_pMaterialList[m_pMaterialList.GetUsed()-1] = l_pName2;

	}
	
	for (int c=0;c<in_pModel->GetChildrenCount();c++)
	{
		CleanModelMaterial ( in_pModel->GetChildrenList()[c] );
	}
	
	return SI_SUCCESS;

}


SI_Error CMapParser::CleanMaterial( VMFTemplate* in_pTemplate )
{
	if ( in_pTemplate->GetMeshModel () != NULL )
	{
		CSIBCString l_pName;
		CSIBCString l_pName2;

		CSLModel*	l_pMesh = in_pTemplate->GetMeshModel ();

		l_pName = l_pMesh->ParentModel()->GetName();
		l_pName.Concat ( "." );
		l_pName.Concat ( l_pMesh->GetName() );
		l_pName.Concat ( ".branchlocal.Material.wrapv" );

		l_pName2 = l_pMesh->ParentModel()->GetName();
		l_pName2.Concat ( "." );
		l_pName2.Concat ( l_pMesh->GetName() );
		l_pName2.Concat ( ".branchlocal.Material.wrapu" );

		m_pMaterialList.Extend(1);
		m_pMaterialList[m_pMaterialList.GetUsed()-1] = l_pName;
		m_pMaterialList.Extend(1);
		m_pMaterialList[m_pMaterialList.GetUsed()-1] = l_pName2;
		
		CSLPrimitive* l_pPrim = l_pMesh->Primitive();

		if ( l_pPrim != NULL )
		{
			CSLMesh*	l_pMeshPrim = (CSLMesh*)l_pPrim;

			for (int s=0;s<l_pMeshPrim->GetPolygonListCount();s++)
			{
				CSLPolygonList* l_pPolylist = l_pMeshPrim->PolygonLists()[s];

				l_pName = l_pMesh->ParentModel()->GetName();
				l_pName.Concat ( "." );
				l_pName.Concat ( l_pMesh->GetName() );

				l_pName.Concat ( ".polymsh.cls.Cls" );
				l_pName.Concat ( l_pPolylist->GetMaterial()->Name().GetText() );
				l_pName.Concat ( ".Material.wrapv" );

				l_pName2 = l_pMesh->ParentModel()->GetName();
				l_pName2.Concat ( "." );
				l_pName2.Concat ( l_pMesh->GetName() );

				l_pName2.Concat ( ".polymsh.cls.Cls" );
				l_pName2.Concat ( l_pPolylist->GetMaterial()->Name().GetText() );
				l_pName2.Concat ( ".Material.wrapu" );

				m_pMaterialList.Extend(1);
				m_pMaterialList[m_pMaterialList.GetUsed()-1] = l_pName;
				m_pMaterialList.Extend(1);
				m_pMaterialList[m_pMaterialList.GetUsed()-1] = l_pName2;
				
			}

		}

		CleanModelMaterial ( l_pMesh );
	}

	for (int c=0;c<in_pTemplate->GetNbChild();c++)
	{
		VMFTemplate *l_pCurrentTemplate = in_pTemplate->GetChild (c);

		CleanMaterial ( l_pCurrentTemplate );

	}

	return 0;
}

SI_Error CMapParser::CleanXSIMaterials()
{
	for (int c=0;c<m_pMaterialList.GetUsed();c++)
	{
		SetXSIValue ( m_pMaterialList[c].GetText(), 10497 );
	}

	return 0;
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

void ModelCount ( CSLModel* in_pModel,  int& in_iModels )
{
	in_iModels += in_pModel->GetChildrenCount();

	for (int c=0;c<in_pModel->GetChildrenCount();c++)
	{
		ModelCount ( in_pModel->GetChildrenList()[c],  in_iModels );
	}



}

void CountTemplate ( VMFTemplate * in_pTemplate, int* count )
{
	(*count)++;

	for (int c=0;c<in_pTemplate->GetNbChild();c++)
	{
		CountTemplate ( in_pTemplate->GetChild(c), count );
	}

}

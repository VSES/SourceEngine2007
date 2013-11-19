#include "stdafx.h"
#include "VMFMaps.h"

#include <sicppsdk.h>
#include <xsi_application.h>
#include <xsi_ogltexture.h>
#include <xsi_model.h>
#include <xsi_material.h>
#include <xsi_parameter.h>
#include <xsi_shader.h>
#include <xsi_group.h>
#include <xsi_application.h>
#include <xsi_model.h>
#include <xsi_geometry.h>
#include <xsi_primitive.h>
#include <xsi_polygonmesh.h>
#include <xsi_polygonface.h>
#include <xsi_vector3.h>
#include <xsi_point.h>
#include <xsi_vertex.h>
#include <xsi_polygonnode.h>
#include <xsi_clusterproperty.h>
#include <xsi_triangle.h>
#include <xsi_null.h>
#include <xsi_kinematicstate.h>
#include <xsi_kinematics.h>

#include <Model.h>
#include <Visibility.h>
#include <GlobalMaterial.h>
#include <Transform.h>
#include <MaterialLibrary.h>
#include <Material.h>
#include <Scene.h>
#include <CustomPSet.h>
#include <VariantParameter.h>



SI_Float	GetNextFloat ( SI_Char**	in_szBuffer );
SI_Float	GetNextFloat2 ( SI_Char**	in_szBuffer );

extern char	___gTexturePath[256];
extern char	___gTexturePathOverride[256];

VMFMapEntry	_gMapEntries[] = 
{ 
	"solid",SolidMap,
	"side",SideMap,
	"dispinfo",DispinfoMap,
	"normals",NormalsMap,
	"distances",DistancesMap,
	"offsets",OffsetsMap,
	"offset_normals",OffsetNormalsMap,
	"alphas",AlphasMap,
	"entity",EntityMap,
	NULL, NULL 
};

int	g_pGlobalMdlCounter = 0;


void	RegisterMaps	()
{
	g_pGlobalMdlCounter = 0;


}

VMFMap	GetVMFMap ( SI_Char *in_szMapName )
{
	int c = 0;
	while ( _gMapEntries[c].m_pCallback != NULL )
	{
		if ( !_SI_STRCMP (  _gMapEntries[c].m_szName , in_szMapName ) )
		{
			return _gMapEntries[c].m_pCallback;
		}
		c++;
	}

	return GenericMap;
}


CSLModel* GenericMap( VMFTemplate* in_pTemplate, CSLModel* in_pParent, XSI::Model in_pModel )
{

	if ( in_pTemplate->GetNbProperty () )
	{
		/*
		CSLCustomPSet *l_pNewPSet = in_pParent->AddCustomPSet();

		SI_Char l_szPropertyName [256];
		sprintf ( l_szPropertyName, "%s_%d",in_pTemplate->GetName(), ++g_pGlobalMdlCounter );
		l_pNewPSet->SetName(l_szPropertyName);

		for (int p=0;p<in_pTemplate->GetNbProperty ();p++)
		{
			CSLVariantParameter *l_pNewParameter = l_pNewPSet->AddParameter();

			CSIBCString l_szName;
			CSIBCString l_szValue;

			in_pTemplate->GetProperty ( p, l_szName, l_szValue );

			l_pNewParameter->SetName( l_szName.GetText());

			SI_TinyVariant l_Variant =
			{
				SI_VT_PCHAR,
				SI_TT_NONE,
				1
			};

			l_Variant.p_cVal = l_szValue.GetText();
			l_pNewParameter->SetValue(&l_Variant);

		}
		*/
	}

	return in_pParent;
}

CSLModel* EntityMap( VMFTemplate* in_pTemplate, CSLModel* in_pParent, XSI::Model in_pModel )
{
	XSI::Application	app;
	XSI::CValueArray	args(4);
	XSI::CValue			outArg;
	XSI::CStatus		st;

	//
	// find the name and it's position
	//

	CSIBCString	l_szEntityName;
	CSIBCString	l_szEntityPosition;
	CSIBCString	l_szEntityOrientation;

	for (int p=0;p<in_pTemplate->GetNbProperty();p++)
	{
		CSIBCString	l_szName;
		CSIBCString	l_szValue;

		in_pTemplate->GetProperty (p,l_szName,l_szValue);

		if ( !_stricmp ( l_szName.GetText(), "targetname") )
		{
			l_szEntityName = l_szValue;
		}

		if ( !_stricmp ( l_szName.GetText(), "origin") )
		{
			l_szEntityPosition = l_szValue;
		}

		if ( !_stricmp ( l_szName.GetText(), "angles") )
		{
			l_szEntityOrientation = l_szValue;
		}
	}

	float x,y,z;
	if ( l_szEntityPosition.GetLength() )
	{
		sscanf ( l_szEntityPosition.GetText(), "%f %f %f",&x,&y,&z);
	} else {

		x = y = z = 0.0f;
	}

	float rx,ry,rz;
	if ( l_szEntityOrientation.GetLength() )
	{
		sscanf ( l_szEntityOrientation.GetText(), "%f %f %f",&rx,&ry,&rz);
	} else {

		rx = ry = rz = 0;
	}

	// Create some geometry.
	LPWSTR l_wszGeomName;
	DSA2W(&l_wszGeomName,l_szEntityName.GetText());
	
	XSI::Null myNull;

	in_pModel.AddNull(l_wszGeomName,myNull);
	
	XSI::X3DObject	l_x3do ( myNull.GetRef() );

	//XSI::MATH::CVector3 refTranslation(2.0, 1.0, 0.0);
	//l_x3do.PutLocalTranslation(refTranslation);

	XSI::KinematicState kineState = myNull.GetKinematics().GetLocal();
	XSI::MATH::CTransformation cCamTransform;
	cCamTransform.SetRotationFromXYZAnglesValues ( ((float)rz+90) * 0.01745329251994329547f,
													((float)rx) * 0.01745329251994329547f,
													((float)ry) * 0.01745329251994329547f ); // invert
	
	//this is in the 5.0 version and not in the 5.1 version
	XSI::MATH::CTransformation rotY;
	rotY.SetRotationFromXYZAnglesValues(0,90* 0.01745329251994329547f,0);
	rotY.MulInPlace (cCamTransform);
	cCamTransform = rotY;

	
	cCamTransform.SetTranslationFromValues(x,y,z); 

	//XSI::MATH::CTransformation cConv;
	//cConv.SetRotationFromXYZAnglesValues ( ((float)-90.0f) * 0.01745329251994329547f,0.0f, 0.0f);
	//cCamTransform.MulInPlace ( cConv );

	kineState.PutTransform(cCamTransform);



	return in_pParent;
}

CSLModel* SolidMap( VMFTemplate* in_pTemplate, CSLModel* in_pParent, XSI::Model in_pModel )
{
	//CSLModel *l_pModel = in_pParent->AddModel();

	//SI_Char	l_szModelName[256];

	//sprintf ( l_szModelName, "%s_%d",in_pTemplate->GetName(), ++g_pGlobalMdlCounter );

	//l_pModel->SetName(l_szModelName);
	//l_pModel->Visibility()->SetVisibility(SI_TRUE);
	//l_pModel->Transform()->SetScale(CSIBCVector3D(1.0, 1.0, 1.0) );
	//l_pModel->GlobalMaterial()->SetPropagationType(CSLGlobalMaterial::SI_INHERITED);
	//l_pModel->GlobalMaterial()->SetMaterial(l_pModel->Scene()->GetMaterialLibrary()->GetMaterialList()[0] );


	//CSLModel *l_pMeshModel = in_pParent->AddMesh();

	//in_pTemplate->SetMeshModel ( l_pMeshModel );

	//SI_Char l_szMeshName [256];
		
	//sprintf ( l_szMeshName, "%s_Solid_%d",in_pParent->GetName(),++g_pGlobalMdlCounter );

	//l_pMeshModel->SetName(l_szMeshName);
	//l_pMeshModel->Visibility()->SetVisibility(SI_TRUE);
	//l_pMeshModel->Transform()->SetScale(CSIBCVector3D(1.0, 1.0, 1.0) );
/*
	if ( in_pTemplate->GetNbProperty () )
	{
		CSLCustomPSet *l_pNewPSet = l_pModel->AddCustomPSet();

		SI_Char l_szPropertyName [256];
		sprintf ( l_szPropertyName, "%s_PropertySet",l_pModel->GetName() );
		l_pNewPSet->SetName(l_szPropertyName);

		for (int p=0;p<in_pTemplate->GetNbProperty ();p++)
		{
			CSLVariantParameter *l_pNewParameter = l_pNewPSet->AddParameter();

			CSIBCString l_szName;
			CSIBCString l_szValue;

			in_pTemplate->GetProperty ( p, l_szName, l_szValue );

			l_pNewParameter->SetName( l_szName.GetText());

			SI_TinyVariant l_Variant =
			{
				SI_VT_PCHAR,
				SI_TT_NONE,
				1
			};

			l_Variant.p_cVal = l_szValue.GetText();
			l_pNewParameter->SetValue(&l_Variant);

		}
	}
*/
	return in_pParent;
}

CSLModel* SideMap( VMFTemplate* in_pTemplate, CSLModel* in_pParent, XSI::Model in_pModel )
{
	//
	// build a face from the properties
	//

	face_t	face;

	memset ( &face,0,sizeof(face_t) );

	for (int c=0;c<in_pTemplate->GetNbProperty();c++)
	{
		CSIBCString name;
		CSIBCString value;
		
		in_pTemplate->GetProperty ( c, name, value );

		if ( !_SI_STRCMP ( name.GetText(), "plane" ) )
		{
			char *buf = value.GetText();

			face.planepts[0][0] = (float) GetNextFloat ( &buf );
			face.planepts[0][1] = (float) GetNextFloat ( &buf );
			face.planepts[0][2] = (float) GetNextFloat ( &buf );

			face.planepts[1][0] = (float) GetNextFloat ( &buf );
			face.planepts[1][1] = (float) GetNextFloat ( &buf );
			face.planepts[1][2] = (float) GetNextFloat ( &buf );

			face.planepts[2][0] = (float) GetNextFloat ( &buf );
			face.planepts[2][1] = (float) GetNextFloat ( &buf );
			face.planepts[2][2] = (float) GetNextFloat ( &buf );

			float ss = fabs(face.planepts[0][0]) +
						fabs(face.planepts[0][1]) +
						fabs(face.planepts[0][2]) +
						fabs(face.planepts[1][0]) +
						fabs(face.planepts[1][1]) +
						fabs(face.planepts[1][2]) +
						fabs(face.planepts[2][0]) +
						fabs(face.planepts[2][1]) +
						fabs(face.planepts[2][2]);

			if ( ss < 0.001f )
			{
				int a = 10;
			}
		}

		if ( !_SI_STRCMP ( name.GetText(), "uaxis" ) )
		{
			char *buf = value.GetText();

			face.uaxis[0] = (float) GetNextFloat ( &buf );
			face.uaxis[1] = (float) GetNextFloat ( &buf );
			face.uaxis[2] = (float) GetNextFloat ( &buf );

			face.ushift = (float) GetNextFloat ( &buf );

			face.uscale = (float) GetNextFloat ( &buf );

			
		}

		if ( !_SI_STRCMP ( name.GetText(), "vaxis" ) )
		{
			char *buf = value.GetText();

			face.vaxis[0] = (float) GetNextFloat ( &buf );
			face.vaxis[1] = (float) GetNextFloat ( &buf );
			face.vaxis[2] = (float) GetNextFloat ( &buf );

			face.vshift = (float) GetNextFloat ( &buf );

			face.vscale = (float) GetNextFloat ( &buf );

			
		}

		if ( !_SI_STRCMP ( name.GetText(), "material" ) )
		{
			memset ( face.texturefile,0,256 );

			if ( strlen(___gTexturePathOverride) )
			{
				sprintf ( face.texturefile,"%s\\%s",___gTexturePathOverride,value.GetText());
			} else {
				sprintf ( face.texturefile,"%s%s",___gTexturePath,value.GetText());
			}


			strcat ( face.texturefile, ".tga" );
	
		}
	}

	in_pTemplate->GetParent()->GetBrush ()->AddFace ( face );

	return in_pParent;
}


SI_Float	GetNextFloat ( SI_Char**	in_szBuffer )
{
	char c;
	char cc[2];
	cc[0] = cc[1] = 0;
	CSIBCString	l_szNumber;

	int ptr = 0;

	char*	l_pBuffer = *in_szBuffer;
	//
	// Find the first character
	//

	while ( ptr < (int)strlen(l_pBuffer) )
	{
		c = l_pBuffer[++ptr];

		if (( c == '-' ) || ( isdigit(c) ))
		{
			break;
		}
	}

	//
	// We are ready to read the number
	//

	while ( ptr < (int)strlen(l_pBuffer) )
	{
		cc[0] = c;

		l_szNumber.Concat ( cc );

		c = l_pBuffer[++ptr];

		if (( c != '.' ) && ( c != 'e' ) && ( c != '-' ))
		{
			if ( !isdigit(c) )
			{
				break;
			}
		}
	}

	(*in_szBuffer)+= ptr;

	return (float)atof ( l_szNumber.GetText() );
}

CSLModel* DispinfoMap( VMFTemplate* in_pTemplate, CSLModel* in_pParent, XSI::Model in_pModel )
{
	//
	// Get the face that owns the dispinfo (always the last face added to the current brush)
	//

	CBrushConverter* l_OwnerBrush = in_pTemplate->GetParent()->GetParent()->GetBrush ();

	face_t *l_face = l_OwnerBrush->GetFace ( l_OwnerBrush->GetNbFace()-1 );

	dispinfo_t	*dispinfo = new dispinfo_t;

	memset ( dispinfo, 0, sizeof(dispinfo_t) );

	l_face->dispinfo = dispinfo;

	for (int c=0;c<in_pTemplate->GetNbProperty();c++)
	{
		CSIBCString name;
		CSIBCString value;
		
		in_pTemplate->GetProperty ( c, name, value );

		if ( !_SI_STRCMP ( name.GetText(), "power" ) )
		{
			char *buf = value.GetText();

			dispinfo->power = (float) atof( buf );

			int s = (int)(pow(2,(int)dispinfo->power) + 1);

				dispinfo->normals = new vec3_t [ s * s ];
				dispinfo->distances = new float [ s * s ];
				dispinfo->offsets = new vec3_t [ s * s ];
				dispinfo->offset_normals = new vec3_t [ s * s ];
				dispinfo->alphas = new float [ s * s ];

		}

		if ( !_SI_STRCMP ( name.GetText(), "startposition" ) )
		{
			char *buf = value.GetText();

			dispinfo->startposition[0] = (float) GetNextFloat ( &buf );
			dispinfo->startposition[1] = (float) GetNextFloat ( &buf );
			dispinfo->startposition[2] = (float) GetNextFloat ( &buf );

		}

		if ( !_SI_STRCMP ( name.GetText(), "elevation" ) )
		{
			char *buf = value.GetText();

			dispinfo->elevation = (float) atof ( buf );

		}

		if ( !_SI_STRCMP ( name.GetText(), "subdiv" ) )
		{
			char *buf = value.GetText();

			dispinfo->subdiv = (float) atof ( buf );

		}

	}

	return in_pParent;
}

CSLModel* NormalsMap( VMFTemplate* in_pTemplate, CSLModel* in_pParent, XSI::Model in_pModel )
{
	CBrushConverter* l_OwnerBrush = in_pTemplate->GetParent()->GetParent()->GetParent()->GetBrush ();

	face_t *l_face = l_OwnerBrush->GetFace ( l_OwnerBrush->GetNbFace()-1 );

	dispinfo_t*	l_pDispinfo = l_face->dispinfo;

	//
	// Fill up the normal data for this disp info
	//

	int vindex = 0;

	for (int i=0;i<in_pTemplate->GetNbProperty() ;i++)
	{
		CSIBCString name;
		CSIBCString value;
			
		in_pTemplate->GetProperty ( i, name, value );

		char *buf = value.GetText();

		for (int c=0;c<in_pTemplate->GetNbProperty() ;c++)
		{
			l_pDispinfo->normals[vindex][0] = (float) GetNextFloat2 ( &buf );
			l_pDispinfo->normals[vindex][1] = (float) GetNextFloat2 ( &buf );
			l_pDispinfo->normals[vindex][2] = (float) GetNextFloat2 ( &buf );
			vindex++;

		}
	}


	return in_pParent;
}

SI_Float	GetNextFloat2 ( SI_Char**	in_szBuffer )
{
	char c;
	char cc[2];
	cc[0] = cc[1] = 0;
	CSIBCString	l_szNumber;

	int ptr = 0;

	char*	l_pBuffer = *in_szBuffer;
	//
	// Find the first character
	//

	while ( ptr < (int)strlen(l_pBuffer) )
	{
		c = l_pBuffer[ptr];

		if (( c == '-' ) || ( isdigit(c) ))
		{
			break;
		}
		ptr++;
	}

	//
	// We are ready to read the number
	//

	while ( ptr < (int)strlen(l_pBuffer) )
	{
		cc[0] = c;

		l_szNumber.Concat ( cc );

		c = l_pBuffer[++ptr];

		if (( c != '.' ) && ( c != 'e' ) && ( c != '-' ))
		{
			if ( !isdigit(c) )
			{
				break;
			}
		}
	}

	(*in_szBuffer)+= ptr;

	return (float)atof ( l_szNumber.GetText() );
}

CSLModel* DistancesMap( VMFTemplate* in_pTemplate, CSLModel* in_pParent, XSI::Model in_pModel )
{
	CBrushConverter* l_OwnerBrush = in_pTemplate->GetParent()->GetParent()->GetParent()->GetBrush ();

	face_t *l_face = l_OwnerBrush->GetFace ( l_OwnerBrush->GetNbFace()-1 );

	dispinfo_t*	l_pDispinfo = l_face->dispinfo;

	//
	// Fill up the distances data for this disp info
	//

	int vindex = 0;

	for (int i=0;i<in_pTemplate->GetNbProperty() ;i++)
	{
		CSIBCString name;
		CSIBCString value;
			
		in_pTemplate->GetProperty ( i, name, value );

		char *buf = value.GetText();

		for (int c=0;c<in_pTemplate->GetNbProperty() ;c++)
		{
			l_pDispinfo->distances[vindex] = (float) GetNextFloat2 ( &buf );
			vindex++;

		}
	}


	return in_pParent;
}


CSLModel* OffsetsMap( VMFTemplate* in_pTemplate, CSLModel* in_pParent, XSI::Model in_pModel )
{
	CBrushConverter* l_OwnerBrush = in_pTemplate->GetParent()->GetParent()->GetParent()->GetBrush ();

	face_t *l_face = l_OwnerBrush->GetFace ( l_OwnerBrush->GetNbFace()-1 );

	dispinfo_t*	l_pDispinfo = l_face->dispinfo;

	//
	// Fill up the offsets data for this disp info
	//

	int vindex = 0;

	for (int i=0;i<in_pTemplate->GetNbProperty() ;i++)
	{
		CSIBCString name;
		CSIBCString value;
			
		in_pTemplate->GetProperty ( i, name, value );

		char *buf = value.GetText();

		for (int c=0;c<in_pTemplate->GetNbProperty() ;c++)
		{
			l_pDispinfo->offsets[vindex][0] = (float) GetNextFloat2 ( &buf );
			l_pDispinfo->offsets[vindex][1] = (float) GetNextFloat2 ( &buf );
			l_pDispinfo->offsets[vindex][2] = (float) GetNextFloat2 ( &buf );
			vindex++;

		}
	}


	return in_pParent;
}


CSLModel* OffsetNormalsMap( VMFTemplate* in_pTemplate, CSLModel* in_pParent, XSI::Model in_pModel )
{
	CBrushConverter* l_OwnerBrush = in_pTemplate->GetParent()->GetParent()->GetParent()->GetBrush ();

	face_t *l_face = l_OwnerBrush->GetFace ( l_OwnerBrush->GetNbFace()-1 );

	dispinfo_t*	l_pDispinfo = l_face->dispinfo;

	//
	// Fill up the offset_normals data for this disp info
	//

	int vindex = 0;

	for (int i=0;i<in_pTemplate->GetNbProperty() ;i++)
	{
		CSIBCString name;
		CSIBCString value;
			
		in_pTemplate->GetProperty ( i, name, value );

		char *buf = value.GetText();

		for (int c=0;c<in_pTemplate->GetNbProperty() ;c++)
		{
			l_pDispinfo->offset_normals[vindex][0] = (float) GetNextFloat2 ( &buf );
			l_pDispinfo->offset_normals[vindex][1] = (float) GetNextFloat2 ( &buf );
			l_pDispinfo->offset_normals[vindex][2] = (float) GetNextFloat2 ( &buf );
			vindex++;

		}
	}


	return in_pParent;
}

CSLModel* AlphasMap( VMFTemplate* in_pTemplate, CSLModel* in_pParent, XSI::Model in_pModel )
{
	CBrushConverter* l_OwnerBrush = in_pTemplate->GetParent()->GetParent()->GetParent()->GetBrush ();

	face_t *l_face = l_OwnerBrush->GetFace ( l_OwnerBrush->GetNbFace()-1 );

	dispinfo_t*	l_pDispinfo = l_face->dispinfo;

	//
	// Fill up the distances data for this disp info
	//

	int vindex = 0;

	for (int i=0;i<in_pTemplate->GetNbProperty() ;i++)
	{
		CSIBCString name;
		CSIBCString value;
			
		in_pTemplate->GetProperty ( i, name, value );

		char *buf = value.GetText();

		for (int c=0;c<in_pTemplate->GetNbProperty() ;c++)
		{
			l_pDispinfo->alphas[vindex] = (float) GetNextFloat2 ( &buf );
			vindex++;

		}
	}


	return in_pParent;
}
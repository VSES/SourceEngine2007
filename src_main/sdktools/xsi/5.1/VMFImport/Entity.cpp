// Entity.cpp: implementation of the CEntity class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Entity.h"

#include <Mesh.h>
#include <BaseShape.h>
#include <Model.h>
#include <Visibility.h>
#include <GlobalMaterial.h>
#include <Transform.h>
#include <MaterialLibrary.h>
#include <Material.h>
#include <Scene.h>
#include <PolygonList.h>
#include <CustomPSet.h>
#include <VariantParameter.h>
#include <Shape.h>
#include "VMFUtils.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CEntity::CEntity() :
m_pCurrentBrush (NULL)
{

}

CEntity::~CEntity()
{

	for (int c=0;c<m_pBrushList.GetUsed();c++)
	{
		CBrushConverter* l_pBrush = m_pBrushList[c];

		delete l_pBrush;
	}

	m_pBrushList.DisposeData();

}

SI_Error	CEntity::AddProperty ( CSIBCString& in_szProperty, CSIBCString&  in_szValue )
{
	m_pPropertyList.Extend(1);
	m_pPropertyList[m_pPropertyList.GetUsed()-1] = in_szProperty;

	m_pValueList.Extend(1);
	m_pValueList[m_pValueList.GetUsed()-1] = in_szValue;

	return SI_SUCCESS;
	
}

SI_Error	CEntity::BeginBrush()
{
	CBrushConverter*	l_pBrush = new CBrushConverter;

	m_pBrushList.Extend(1);
	m_pBrushList[m_pBrushList.GetUsed()-1] = l_pBrush;

	m_pCurrentBrush = l_pBrush;

	return SI_SUCCESS;
}

SI_Error	CEntity::AddFace ( face_t& in_face )
{
	assert ( m_pCurrentBrush != NULL );

	return m_pCurrentBrush->AddFace ( in_face );
}

SI_Error	CEntity::EndBrush()
{
	m_pCurrentBrush = NULL;

	return SI_SUCCESS;
}

SI_Error	CEntity::Convert ()
{
	for (int c=0;c<m_pBrushList.GetUsed();c++)
	{
		CBrushConverter* l_pBrush = m_pBrushList[c];

		l_pBrush->Convert(false);
	}

	return SI_SUCCESS;
}

SI_Error	CEntity::FillModel	( CSLModel *in_pModel )
{
	for (int c=0;c<m_pBrushList.GetUsed();c++)
	{

		CBrushConverter*	l_pCurrentBrush = m_pBrushList[c];

		//
		// Create a new mesh for this brush
		//

		CSLModel *l_pMeshModel = in_pModel->AddMesh();

		SI_Char l_szMeshName [256];
		
		sprintf ( l_szMeshName, "%s_Brush_%d",in_pModel->GetName(),c );

		l_pMeshModel->SetName(l_szMeshName);
		l_pMeshModel->Visibility()->SetVisibility(SI_TRUE);
		l_pMeshModel->Transform()->SetScale(CSIBCVector3D(1.0, 1.0, 1.0) );
		l_pMeshModel->GlobalMaterial()->SetPropagationType(CSLGlobalMaterial::SI_INHERITED);
		l_pMeshModel->GlobalMaterial()->SetMaterial(l_pMeshModel->Scene()->GetMaterialLibrary()->GetMaterialList()[0] );


		CSLMesh *l_pMesh = (CSLMesh *) l_pMeshModel->Primitive();
		CSLBaseShape *l_pShape = l_pMesh->Shape();

		//
		// Count the number of vertices in each face
		//
		int vertexcount = 0;
		int f;
		int p;

		for (f=0;f<l_pCurrentBrush->GetNbFace ();f++)
		{
			face_t *l_Face = l_pCurrentBrush->GetFace (f);
			 
			vertexcount += l_Face->w->numpoints;

		}

		// Add some position coordinates
		CSLBaseShape::CSLVector3DArray *l_Vertices = l_pShape->GetVertexList();
		l_Vertices->Resize(vertexcount);
		int vindex = 0;

		// Add some normal coordinates.
		CSLBaseShape::CSLVector3DArray &l_Normals = *l_pShape->CreateNormals();
		l_Normals.Resize(vertexcount);

		// Add some uv coordinates.
		//CSLShape::CSLVector2DArray &l_UVs = *((CSLShape*)l_pShape)->CreateUVCoords();
		//l_UVs.Resize(vertexcount);

	
		//add a polygon list
		CSLPolygonList* l_Poly = l_pMesh->AddPolygonList();
		l_Poly->SetMaterial ( l_pMeshModel->Scene()->GetMaterialLibrary()->GetMaterialList()[0] );

		l_Poly->GetPolygonVertexCountList()->Resize( l_pCurrentBrush->GetNbFace () );

		//
		// Need to count how many vertex index we will use
		//
		int vindexcount = 0;
		for (f=0;f<l_pCurrentBrush->GetNbFace ();f++)
		{
			face_t *l_Face = l_pCurrentBrush->GetFace (f);
			
			vindexcount += l_Face->w->numpoints;
		}

		l_Poly->GetVertexIndices()->Resize( vindexcount );
		SI_Int *l_pnDataArray = l_Poly->GetVertexIndices()->ArrayPtr();

		//add normals
		CSLPolygonList::CSLIntArray* l_pNormalArray = l_Poly->CreateNormalIndices();
		l_pNormalArray->Resize(vindexcount);

		//add UV
		//CSLPolygonList::CSLIntArray* l_pUVArray = l_Poly->AddUVArray();
		//l_pUVArray->Resize(vindexcount);

		SI_Int *l_normalDataArray = l_pNormalArray->ArrayPtr();
		//SI_Int *l_uvDataArray = l_pUVArray->ArrayPtr();
		
		for (f=0;f<l_pCurrentBrush->GetNbFace ();f++)
		{
			face_t *l_Face = l_pCurrentBrush->GetFace (f);
			
			(*l_Poly->GetPolygonVertexCountList() )[f] = l_Face->w->numpoints;

			for (p=0;p<l_Face->w->numpoints;p++)
			{
				l_pnDataArray[vindex] = vindex;
				l_normalDataArray[vindex] = f;
				//l_uvDataArray[vindex] = vindex;

				(*l_Vertices)[vindex].Set(l_Face->w->points[p][0], l_Face->w->points[p][1], l_Face->w->points[p][2]);
				
				l_Normals[vindex].Set (l_Face->plane.normal[0], l_Face->plane.normal[1], l_Face->plane.normal[2]);
				
				/*
				float u,v;

				GetUV ( l_Face->plane.normal, 
						l_Face->w->points[p], 
						u,
						v,
						l_Face->texture.rotate,
						l_Face->texture.shift[0],
						l_Face->texture.shift[1],
						l_Face->texture.scale[0],
						l_Face->texture.scale[1] );

				u = u / 128.0f;
				v = v / 128.0f;
				
				l_UVs[vindex].Set ( u,v );
				*/

				vindex++;

			}


		}

	}

	if ( m_pPropertyList.GetUsed() )
	{
		CSLCustomPSet *l_pNewPSet = in_pModel->AddCustomPSet();

		SI_Char l_szPropertyName [256];
		sprintf ( l_szPropertyName, "%s_PropertySet",in_pModel->GetName() );
		l_pNewPSet->SetName(l_szPropertyName);

		for (int p=0;p<m_pPropertyList.GetUsed();p++)
		{
			CSLVariantParameter *l_pNewParameter = l_pNewPSet->AddParameter();
			l_pNewParameter->SetName( m_pPropertyList[p].GetText() );

			SI_TinyVariant l_Variant =
			{
				SI_VT_PCHAR,
				SI_TT_NONE,
				1
			};

			l_Variant.p_cVal = m_pValueList[p].GetText();
			l_pNewParameter->SetValue(&l_Variant);

		}
	}



	
	return SI_SUCCESS;
}

SI_Char* CEntity::GetWadFile ()
{
	for (int p=0;p<m_pPropertyList.GetUsed();p++)
	{
		if ( !_SI_STRCMP( m_pPropertyList[p].GetText(), "wad" ))
		{

			return m_pValueList[p].GetText();

		} else {
			
			if ( !_SI_STRCMP( m_pPropertyList[p].GetText(), "WAD" ))
			{
				return m_pValueList[p].GetText();
			}
		}

	}

	return NULL;
}

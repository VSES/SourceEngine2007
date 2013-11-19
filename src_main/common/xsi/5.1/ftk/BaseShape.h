/******************************************************************************\
*
* File: 		 BaseShape.h
* Creation date: January 15, 2002
* Purpose:		 Declaration of class 'CSLBaseShape'
*
\******************************************************************************/
#ifndef _BASESHAPE_H
#define _BASESHAPE_H

#include "Template.h"


/** 
* Associates an index to a CSIBCVector2D object.
*/
struct SLIndexedVector2D
{
	SI_Float m_fIndex;			/*!< Vector index*/
	CSIBCVector2D m_Vector2D;	/*!< 2D vector*/
};


/** 
* Associates an index to a CSIBCVector3D object.
*/
struct SLIndexedVector3D
{
	SI_Float m_fIndex;			/*!< Vector index*/
	CSIBCVector3D m_Vector3D;	/*!< 3D vector*/
};


/** 
* Associates an index to a CSIBCColorf object.
*/
struct SLIndexedColor
{	
	SI_Float m_fIndex;			/*!< Color index*/
	CSIBCColorf m_Color;		/*!< Color vector*/
};


/** 
* Describes the shape of a mesh
*
* The CSLBaseShape class contains information about the position coordinates, normal coordinates
* and color values of a mesh.
*
* There are two possible types of shape: ordered & indexed.
* The set of functions that can be used depends on the type of shape that is
* described by each class instance.
*
* For example, the function CreateNormals will work on an ordered shape, but won't on an
* indexed shape. On the other hand, the function CreateIndexedNormals will work on an
* indexed shape, but won't on an ordered shape. However, the functions DestroyNormals
* and GetNormalCount will work with both types (ordered & indexed).
*
* <B> Example - Using CSLBaseShape </B>
*
\code

		CSLModel *l_pMeshModel = RootModel->AddMesh();

		//
		// Set some default values
		//

		l_pMeshModel->SetName("MyMesh");
		l_pMeshModel->Visibility()->SetVisibility(SI_TRUE);
		l_pMeshModel->Transform()->SetScale(CSIBCVector3D(1.0, 1.0, 1.0) );
		l_pMeshModel->GlobalMaterial()->SetPropagationType(CSLGlobalMaterial::SI_INHERITED);

		//
		// Set default material
		//

		l_pMeshModel->GlobalMaterial()->SetMaterial(l_pMeshModel->Scene()->GetMaterialLibrary()->GetMaterialList()[0] );

		//
		// Get the CSLMesh pointer from the model
		//
			
		CSLMesh *l_pMesh = (CSLMesh *) l_pMeshModel->Primitive();

		//
		// Get a pointer to the Mesh's shape
		//

		CSLBaseShape *l_pShape = l_pMesh->Shape();

		//
		// Add 8 vertices
		//

		CSLBaseShape::CSLVector3DArray *l_Vertices = l_pShape->GetVertexList();
		l_Vertices->Resize(8);

		//
		// Add 8 normals
		//

		CSLBaseShape::CSLVector3DArray *l_Normals = l_pShape->CreateNormals();
		l_Normals.Resize(8);

		//
		// Now set the positions / normal
		//

		for (int i=0;i<8;i++)
		{
			(*l_Vertices)[vindex].Set(0.0f, 0.0f, 0.0f);
				
			(*l_Normals)[vindex].Set (0.0f, 1.0f, 0.0f);
		}


\endcode
*/
class XSIEXPORT CSLBaseShape
	: public CSLTemplate
{
public:
	/**
	* Proxy class that allows modifying a list of CSIBCVector2D objects like an array.
	*/
	typedef CSLArrayProxy<CSIBCVector2D, SI_Float, 2>		CSLVector2DArray;

	/**
	* Proxy class that allows modifying a list of CSIBCVector3D objects like an array.
	*/
	typedef CSLArrayProxy<CSIBCVector3D, SI_Float, 3>		CSLVector3DArray;

	/**
	* Proxy class that allows modifying a list of CSIBCColorf objects like an array.
	*/
	typedef CSLArrayProxy<CSIBCColorf, SI_Float, 4> 		CSLColorArray;

	/**
	* Proxy class that allows modifying a list of SLIndexedVector2D objects like an array.
	*/
	typedef CSLArrayProxy<SLIndexedVector2D, SI_Float, 3>	CSLIndexedVector2DArray;

	/**
	* Proxy class that allows modifying a list of SLIndexedVector3D objects like an array.
	*/
	typedef CSLArrayProxy<SLIndexedVector3D, SI_Float, 4>	CSLIndexedVector3DArray;

	/**
	* Proxy class that allows modifying a list of SLIndexedColor objects like an array.
	*/
	typedef CSLArrayProxy<SLIndexedColor, SI_Float, 5>		CSLIndexedColorArray;

	/** 
	* The EShapeType enum contains possible types for a shape
	*/
	enum EShapeType
	{
		SI_ORDERED,	/*!< The shape is of ordered type */
		SI_INDEXED, /*!< The shape is of indexed type */
	};

	/*! Destructor
	*/
	virtual ~CSLBaseShape();

	/*!	Create a normal coordinates array.
		\return	CSLVector3DArray*	Pointer to the newly created array.
	*/
	CSLVector3DArray*				CreateNormals();

	/*!	Create an indexed normal coordinates array.
		\return	CSLIndexedVector3DArray*	Pointer to the newly created indexed array.
	*/
	CSLIndexedVector3DArray*		CreateIndexedNormals();

	/*!	Return the number of normal coordinates.
		\return	SI_Int	Number of normals.
	*/
	SI_Int							GetNormalCount();

	/*!	Return the normal coordinate array proxy.
		\return	CSLVector3DArray*	Pointer to an array proxy of normals.
	*/
	CSLVector3DArray*				GetNormalList();

	/*!	Returns an array of CSBCVector3D containing the normal coordinates.
		\return	CSIBCVector3D*		Array of CSIBCVector3D.
	*/
	CSIBCVector3D*					GetNormalListPtr();

	/*!	Return the indexed normal coordinate array proxy.
		\return	CSLIndexedVector3DArray*	Pointer to an array proxy of indexed normals.
	*/
	CSLIndexedVector3DArray*		GetIndexedNormalList();

	/*!	Returns an array of SLIndexedVector3D containing the indexed normal coordinates.
		\return	SLIndexedVector3D*		Array of SLIndexedVector3D.
	*/
	SLIndexedVector3D*				GetIndexedNormalListPtr();

	/*!	Destroy the normal coordinates array.
		\return	SI_Error	Error code.
	*/
	SI_Error						DestroyNormals();

	/*!	Create a color values array.
		\return	CSLColorArray*	Pointer to the newly created array.
	*/
	CSLColorArray*					CreateColors();

	/*!	Create an indexed color values array.
		\return	CSLIndexedColorArray*	Pointer to the newly created indexed array.
	*/
	CSLIndexedColorArray*			CreateIndexedColors();

	/*!	Return the number of color values.
		\return	SI_Int	Number of colors.
	*/
	SI_Int							GetColorCount();

	/*!	Return the color values array proxy.
		\return	CSLColorArray*	Pointer to an array proxy of colors.
	*/
	CSLColorArray*					GetColorList();

	/*!	Returns an array of CSIBCColorf containing the color values.
		\return	CSIBCColorf*		Array of CSIBCColorf.
	*/
	CSIBCColorf*					GetColorListPtr();

	/*!	Return the indexed color values array proxy.
		\return	CSLIndexedVector3DArray*	Pointer to an array proxy of indexed colors.
	*/
	CSLIndexedColorArray*			GetIndexedColorList();

	/*!	Returns an array of SLIndexedColor containing the indexed color values.
		\return	SLIndexedColor*		Array of SLIndexedColor.
	*/
	SLIndexedColor* 				GetIndexedColorListPtr();

	/*!	Destroy the color values array.
		\return	SI_Error	Error code.
	*/
	SI_Error						DestroyColors();

	/*!	Return the number of vertex coordinates.
		\return	SI_Int	Number of vertices.
	*/
	SI_Int							GetVertexCount();

	/*!	Return the vertex coordinate array proxy.
		\return	CSLVector3DArray*	Pointer to an array proxy of vertices.
	*/
	CSLVector3DArray*				GetVertexList();

	/*!	Returns an array of CSBCVector3D containing the vertex coordinates.
		\return	CSIBCVector3D*		Array of CSIBCVector3D.
	*/
	CSIBCVector3D*					GetVertexListPtr();

	/*!	Return the indexed vertex coordinate array proxy.
		\return	CSLIndexedVector3DArray*	Pointer to an array proxy of indexed vertices.
	*/
	CSLIndexedVector3DArray*		GetIndexedVertexList();

	/*!	Returns an array of SLIndexedVector3D containing the indexed vertex coordinates.
		\return	SLIndexedVector3D*		Array of SLIndexedVector3D.
	*/
	SLIndexedVector3D*				GetIndexedVertexListPtr();

	/*!	Indicate which type of shape this object describes.
		\return	EShapeType	Type of shape
	*/
	EShapeType						ShapeType();

	virtual SI_Error Fix();
	virtual SI_Error Synchronize();

protected:

	/*! Constructor
		\param in_pScene	Pointer to the scene containing the BaseShape
		\param in_pModel	Pointer to the model containing the BaseShape
		\param in_pTemplate	Pointer to the CdotXSITemplate containing the BaseShape
		\param in_Type	The shape type to be used in the BaseShape
	*/
	CSLBaseShape(CSLScene* in_pScene, CSLModel *in_pModel, CdotXSITemplate* in_pTemplate, EShapeType in_Type);

	CSLIntProxy 					m_NumArrays;
	CSLStringProxy					m_ArrayType;

	CSLVector3DArray*				m_pPositions;
	CSLVector3DArray*				m_pNormals;
	CSLColorArray*					m_pVertexColors;

	CSLIndexedVector3DArray*		m_pIndexedPositions;
	CSLIndexedVector3DArray*		m_pIndexedNormals;
	CSLIndexedColorArray*			m_pIndexedVertexColors;

	CSLIntProxy*					m_iNbPositions;
	CSLIntProxy*					m_iNbNormals;
	CSLIntProxy*					m_iNbColors;

	CSLStringProxy* 				m_pPositionStringProxy;
	CSLStringProxy* 				m_pNormalStringProxy;
	CSLStringProxy* 				m_pColorStringProxy;

	EShapeType						m_Type;

	void *m_pReserved;	// reserved for future extension
};

//XSITEMPLATEEXP template class XSIEXPORT CSLArrayProxy<CSIBCVector2D, SI_Float, 2>;
//XSITEMPLATEEXP template class XSIEXPORT CSLArrayProxy<CSIBCVector3D, SI_Float, 3>;
//XSITEMPLATEEXP template class XSIEXPORT CSLArrayProxy<CSIBCColorf, SI_Float, 4>;
//XSITEMPLATEEXP template class XSIEXPORT CSLArrayProxy<SLIndexedVector2D, SI_Float, 3>;
//XSITEMPLATEEXP template class XSIEXPORT CSLArrayProxy<SLIndexedVector3D, SI_Float, 4>;
//XSITEMPLATEEXP template class XSIEXPORT CSLArrayProxy<SLIndexedColor, SI_Float, 5>;

#endif


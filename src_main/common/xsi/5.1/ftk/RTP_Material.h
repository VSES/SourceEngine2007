/****************************************************************************************
THIS CODE IS PUBLISHED AS A SAMPLE ONLY AND IS PROVIDED "AS IS".
IN NO EVENT SHALL SOFTIMAGE, AVID TECHNOLOGY, INC. AND/OR THEIR RESPECTIVE 
SUPPLIERS BE LIABLE FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR ANY
DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN
CONNECTION WITH THE USE OR PERFORMANCE OF THIS CODE . 
 
COPYRIGHT NOTICE. Copyright © 1999-2002 Avid Technology Inc. . All rights reserved. 

SOFTIMAGE is a registered trademark of Avid Technology Inc. or its subsidiaries 
or divisions. Windows NT is a registered trademark of Microsoft Corp. All other
trademarks contained herein are the property of their respective owners. 
****************************************************************************************/

#ifndef _RTP_MATERIAL_H_
#define _RTP_MATERIAL_H_

#include <SIRTProcess.h>
#include <SIBaseApplication.h>
#include <Scene.h>
#include <TriangleList.h>
#include <PolygonList.h>
#include <BaseShape.h>
#include <Material.h>
#include <RTP_Shader.h>

#include <RTP_BasicProcessesTypes.h>

#ifndef _SI_LEGACY_MAIN_H_
/** 
* Structure holding property page values for legacy Softimage Materials
* \sa FTK Documentation on SI_Material
*/
typedef struct tagSI_MaterialPPG
{
	float	diffuse_red;		/*!< Red diffuse*/
	float	diffuse_green;		/*!< Green diffuse*/
	float	diffuse_blue;		/*!< Blue diffuse*/
	float	diffuse_alpha;		/*!< Alpha diffuse*/
	float	specular_decay;		/*!< Specular decay*/
	float	specular_red;		/*!< Specular Red*/
	float	specular_green;		/*!< Specular Green*/
	float	specular_blue;		/*!< Specular Blue*/
	float	emissive_red;		/*!< Emissive Red*/
	float	emissive_green;		/*!< Emissive Green*/
	float	emissive_blue;		/*!< Emissive Blue*/
	int	shading_model;		/*!< Shading model*/
	float	ambient_red;		/*!< Ambient Red*/
	float	ambient_green;		/*!< Ambient Green*/
	float	ambient_blue;		/*!< Ambient Blue*/
} SI_MaterialPPG;

/** 
* Structure holding property page values for legacy Softimage Texture
* \sa FTK Documentation on SI_Texture2D
*/
typedef struct tagSI_TexturePPG
{
	S3DTexture	textureInfo;	/*!< texture information*/
	int			mapping_type;	/*!< Texture mapping type*/
	int			width;			/*!< width*/
	int			height;			/*!< height*/
	int			cropUMin;		/*!< Texture cropping minimum U value*/
	int			cropUMax;		/*!< Texture cropping maximum U value*/
	int			cropVMin;		/*!< Texture cropping minimum V value*/
	int			cropVMax;		/*!< Texture cropping maximum V value*/
	int		uvswap;			/*!< swap U and V*/
	int			urepeat;		/*!< Repeat texture in U*/
	int			vrepeat;		/*!< Repeat texture in V*/
	int			ualternate;		/*!< Altenate U*/
	int			valternate;		/*!< Altenate V*/
	float		uscale;			/*!< U scale factor*/
	float		vscale;			/*!< V scale factor*/
	float		uoffset;		/*!< U offset */
	float		voffset;		/*!< V offset */
	float		matrix[16];		/*!< Texture matrix*/
	int			blendingType;	/*!< Material blending type*/
	float		blending;		/*!< Material blending*/
	float		ambient;		/*!< Ambient factor*/
	float		diffuse;		/*!< Diffuse factor*/
	float		specular;		/*!< Specular factor*/
	float		transparency;	/*!< Transparency factor*/
	float		reflectivity;	/*!< Reflectivity factor*/
	float		roughness;		/*!< Roughness factor*/
} SI_TexturePPG;
#endif

typedef struct tagSI_MaterialSortData
{
	CSIMaterialCluster					*m_pTransparentClusters;
	CSIBCArray<CSIRTP_ShaderDisplay *>	*m_pShaderCollections;
} SI_MaterialSortData;

SI_Error	RegisterSIMaterial ( CSIBaseApplication* in_pApplication, CSLScene *in_pCurrentScene );

/** 
* MaterialSortAndDisplay runtime process
* 
* This is similar to the CSIRTP_Material process, but instead will sort the clusters from back to front before drawing them.
*
*/
class XSIEXPORT CSIRTP_MaterialSortAndDisplay : public CSIRTProcess
{
public:
	//! constructor for the CSIRTP_Material class
	/*! 
	\return A pointer to the new CSIRTP_MaterialSortAndDisplay
	*/
	CSIRTP_MaterialSortAndDisplay();

	//! destructor for the CSIRTP_MaterialSortAndDisplay class
	/*! 
	\return nothing
	*/
	~CSIRTP_MaterialSortAndDisplay();

	//! destructor for the CSIRTProcess class
	/*! 
	\return the classification of the runtime process
	*/
	virtual CSIRTProcessType	GetType() {return CSIRTProcess::ZSORTRENDER;}

	/** Evaluates (renders) all triangle strips that refers this material
	* When a mesh gets evaluated, it sends its associated CSIRTP_Material
	* all it's triangle lists. These triangle lists are then sent to the 
	* Real-time shader by the CSIRTP_Material during this call.
	\return Error code
	*/
	virtual SI_Error			Evaluate();
	SI_Error					AddMaterialCluster ( CSIMaterialCluster *in_pCluster, CSIBCArray<CSIRTP_ShaderDisplay *> *pShaderCollection );
	SI_Error					Reset();

	/** class factory for runtime linked runtime processes
	* This is used when an external entity wants to create a process of this 
	* type without having to link at compile time. The mechanism is simple.
	* the external entity uses the class factory to create a new CSIRTProcess
	* and sets the initialization parameters in the user data "SIRTINIT".
	* The external entity then calls CSIRTProcess::Init to initialize the data
	* extracted from the user data
	*
	\return a new CSIRTProcess
	*/
	virtual SI_Error			CreateRTProcess( CSIRTProcess**out_pProcess );

private:
	CSIBCArray<SI_MaterialSortData>	m_SortData;
	SI_Int							m_lNbClusters;
	SI_Void							*m_pUnused;

};

/** 
* Material runtime process
* 
* When meshes are evaluated, they typically loop through their polygon and triangle list (tagMaterialCluster) and tell their associated 
* materials to draw them upon the evaluation of the MATERIAL pass. Once the RuntimeEngine fire the material pass, the CSIRTP_Material
* will loop through it's list of CSIMaterialCluster (added by the CSIRTP_Mesh process) and call each associated RealTime Shader to draw them.
*
* \sa tagMaterialCluster
* \sa CSIRuntimeEngine
* \sa CSIRTP_Mesh
* \sa Realtime Shader SDK Documentation
*/
class XSIEXPORT CSIRTP_Material : public CSIRTProcess
{
public:
	//! constructor for the CSIRTP_Material class
	/*! 
	\return A pointer to the new CSIRTP_Material
	*/
	CSIRTP_Material();

	//! destructor for the CSIRTP_Material class
	/*! 
	\return nothing
	*/
	~CSIRTP_Material();

	static	SLTemplateCallback	m_sSI_MaterialNativeCallbacks;
	static	SLTemplateCallback	m_sSI_Texture2DNativeCallbacks;
	static	SLTemplateCallback	m_sXSI_MaterialNativeCallbacks;

	//! destructor for the CSIRTProcess class
	/*! 
	\return the classification of the runtime process
	*/
	virtual CSIRTProcessType	GetType() {return CSIRTProcess::RENDER;}


	/** Evaluates (renders) all triangle strips that refers this material
	* When a mesh gets evaluated, it sends its associated CSIRTP_Material
	* all it's triangle lists. These triangle lists are then sent to the 
	* Realtime shader by the CSIRTP_Material during this call.
	\return Error code
	*/
	virtual SI_Error			Evaluate();
	//! Sets a pointer to the CSLMaterial used by this process
	/*! 
	\param	CSLBaseMaterial*	The Base material used by this process
	\return nothing
		*/
	SI_Void						SetMaterial ( CSLBaseMaterial *in_pMaterial ) { m_pMaterial = in_pMaterial; };

	//! Adds a single material cluster ( tri-stirp list) to the lists of cluster that will be drawn
	/*! 
	\param	CSIMaterialCluster*	The list of tri-strips to add (see CSIRTP_Mesh)
	\return nothing
	*/
	SI_Error					AddMaterialCluster ( CSIMaterialCluster *in_pCluster );

	//! Get the list of shaders display runtime processes
	/*!
	\return the list of shader display runtime processes
	*/
	CSIBCArray < CSIRTP_ShaderDisplay*>	*GetShaderDisplayProcesses();

	CSIBCString							&GetLastRealtimeShader();

	SI_Error							UpdateSI_MaterialShaders(SI_MaterialPPG *in_pMatPPG, SI_TexturePPG *in_pTexPPG);
	CSIBCArray<SI_ULong *>				&GetTextureDomain() { return m_pTextureDomains;};

	/** class factory for runtime linked runtime processes
	* This is used when an external entity wants to create a process of this 
	* type without having to link at compile time. The mechanism is simple.
	* the external entity uses the class factory to create a new CSIRTProcess
	* and sets the initialization parameters in the user data "SIRTINIT".
	* The external entity then calls CSIRTProcess::Init to initialize the data
	* extracted from the user data
	*
	\return a new CSIRTProcess
	*/
	virtual SI_Error			CreateRTProcess( CSIRTProcess**out_pProcess );

private:

	CSLBaseMaterial						*m_pMaterial;
	CSIBCArray < CSIMaterialCluster*>	m_pClusters;
	CSIBCArray < CSIRTP_ShaderDisplay*>	m_pShaders;
	CSIBCString							m_LastRealtimeShader;
	CSIRTP_MaterialSortAndDisplay		*m_pZsortBucket;
	SI_Void								*m_pUnused;
	SI_MaterialPPG						matPPG;
	SI_TexturePPG						texPPG[1024];
	SI_Bool								m_bCanCache;
	SI_Bool								m_bInitCache;
	SI_Long								m_lIsTransparent;
	CSIBCArray<SI_ULong *>				m_pTextureDomains;
	SI_Bool								m_bTextureDomainCached;


};




#endif

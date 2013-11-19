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

#ifndef _SCENE_H
#define _SCENE_H

#include "Template.h"
#include "Logger.h"

class CSLScene;
class CSLModel;
class CSLAmbience;
class CSLAngle;
class CSLCoordinateSystem;
class CSLFileInfo;
class CSLSceneInfo;
class CSLEnvelopeList;
class CSLMaterialLibrary;
class CSLFog;
class CSLActionFCurve;
class CSLFCurve;
class CSLTransform;
class CSLAction;
class CSLShapeAnimation;
class CSLVisibility;
class CSLXSIMaterial;
class CSLImageLibrary;
class CSLImage;
class CSLXSIShader;

/** 
* CSLUnresolvedElement Class
* 
* This class is used internaly when parsing the dotXSI file. It is used to resolved template references..
*/
class XSIEXPORT CSLUnresolvedElement
{
public:
	CSLUnresolvedElement
	( 
		CSLTemplate *in_pUnresolvedObject,	
		CSLTemplate *in_pUnresolvedParent = 0,	
		SI_Char *in_pReferenceName = 0,
		void *in_pUserData = 0
	);

	/*! Get the unresolved temlpate.
		\return CSLTemplate*	The unresolved template.
	*/
	inline CSLTemplate* UnresolvedObject();

	/*! Get the unresolved parent temlpate.
		\return CSLTemplate*	The unresolved parent template.
	*/
	inline CSLTemplate* UnresolvedParent();

	/*! Get the reference name.
		\return SI_Char*	The name of the reference.
	*/
	inline SI_Char*	 ReferenceName();

	/*! Get the user data attached to this object.
		\return void*	User data.
	*/
	inline void*		 UserData();

private:
	CSLTemplate *m_pUnresolvedObject;
	CSLTemplate *m_pUnresolvedParent;
	SI_Char *m_pReferenceName;
	void *m_pUserData;

	// This member is here to allow future improvement of the class.
	void *m_pReserved;
};

inline CSLTemplate* CSLUnresolvedElement::UnresolvedObject()
{
	return m_pUnresolvedObject;
}

inline CSLTemplate* CSLUnresolvedElement::UnresolvedParent()
{
	return m_pUnresolvedParent;
}
inline SI_Char* CSLUnresolvedElement::ReferenceName()
{
	return m_pReferenceName;
}

inline void* CSLUnresolvedElement::UserData()
{
	return m_pUserData;
}

#ifdef	FTK_KERNEL
typedef std::multimap<SI_Char *, CSLUnresolvedElement, StrLessPred> CSLUnresolvedMap;
typedef CSLUnresolvedMap::iterator CSLUnresolvedMapIterator;
typedef std::pair<CSLUnresolvedMapIterator,CSLUnresolvedMapIterator> CSLUnresolvedMapRange;

typedef SI_StrMap<CSLModel *> CSLModelMap;
typedef CSLModelMap::iterator CSLModelMapIterator;
#else
typedef	void CSLUnresolvedMap;
typedef	void CSLModelMap;
#endif

/** 
* SLContext Structure
* 
* This struct is used to keep track of things when a scene if first constructed during a Load call. The SLContext
* is passed to template callback handlers so the callbacks know the current model, the current parent etc.
*/
struct XSIEXPORT SLContext
{
public:
	SLContext();
	SI_Void AddOrphan(CSLTemplate *in_pOrphanTemplate); /*!< Add a new orphan template to the global orphan list. This list will be resolved a the very end of the load function*/

	CSLTemplate *m_pCurrentTemplate;					/*!< The current template*/
	CSLModel *m_pCurrentModel;							/*!< The current model*/
	CSLScene *m_pScene;									/*!< Pointer to the scene*/
	CSLMaterialLibrary *m_pMaterialLibrary;				/*!< Pointer to the material library*/
	CSLXSIMaterial *m_pCurrentMaterial;					/*!< The current material*/
	CSIBCArray<CSLTemplate *> *m_pOrphanList;			/*!< The global orphan list*/
	void *m_pTemplateUserData;							/*!< User data associated with templates*/
	CSLUnresolvedMap *m_pUnresolvedMap;					/*!< The unresolved template map*/
	CSLModelMap *m_pModelMap;							/*!< A list of all models*/
	CSLImageLibrary *m_pImageLibrary;					/*!< A pointer to the image library*/
	CSLImage *m_pCurrentImage;							/*!< The current image*/
	CSLXSIShader *m_pCurrentXSIShader;					/*!< The current shader*/
	void *m_pUserData;									/*!< Generic user data*/

private:
	// This member is here to allow future improvement of the class.
	void *m_pReserved;
};

typedef CSLTemplate *(*CSLCALLBACK) (CdotXSITemplate *in_pTemplate, SLContext *in_pContext);
typedef SI_Error (*CSLPOSTCHILDRENCALLBACK) (CSLTemplate *in_pTemplate, SLContext *in_pContext);
typedef SI_Error (*CSLLOADCALLBACK) (SLContext *in_pContext);

/** 
* SLTemplateCallback Structure
*
* A dotXSI template callback. This structure allows to specify callbacks that must be call when a specific template
* is found. You may specify a callback for the following events:
* - PreLoadCallback ( Will be called before the entire is parsed)
*	- This is useful for initialization needs
* - Callback		( Will be called when the template is found )
*	- This is where most of the work should be done. This is where Semantic Layer object are created using the template passed and are attached to the Scene.
* - PostChilrenCallback ( Will be called when the template's children have been called )
*	- This useful for resolving items within a model. For instance, if a parent template references on of it's children, you should resolve the references here.
* - PostLoadCallback ( Will be called when the file has been loaded entirly )
*	- This is useful for resolving items within the entire scene.
*
* \sa CSLScene::RegisterCallback
**/
struct XSIEXPORT SLTemplateCallback
{
	SLTemplateCallback();

	CSLLOADCALLBACK m_PreLoadCallback;
	CSLCALLBACK m_Callback;
	CSLPOSTCHILDRENCALLBACK m_PostChildrenCallback;
	CSLLOADCALLBACK m_PostLoadCallback;
	void *m_pUserData;

private:
	// This member is here to allow future improvement of the class.
	void *m_pReserved;
};

/** 
* CSLScene Class
* 
* This is the top level object for a dotXSI abstraction using the SemanticLayer. From this class, you can
* access all the models, primitives, material etc. contained in the dotXSI.
*/

class XSIEXPORT CSLScene
{
public:
	///////////////////////////////////////////////////////////////////////////
	// Construction/destruction
	///////////////////////////////////////////////////////////////////////////
	CSLScene();
	virtual ~CSLScene();

	///////////////////////////////////////////////////////////////////////////
	// File manipulation functions.
	///////////////////////////////////////////////////////////////////////////
	/*! Open a dotXSI file for reading and writing.
		The file must exist for the function to succeed.
		\param	SI_Char*	File to open.
		\return SI_Error	Error code.
	*/
	SI_Error Open(SI_Char *in_szFileName);

	/*! Open a dotXSI file for reading and writing.
		The file must exist for the function to succeed.
		\param	SI_Char*				File to open.
		\param	SI_READ_CALLBACK_FNC	read callback
		\return SI_Error				Error code.
	*/
	SI_Error Open(SI_Char *in_szFileName, SI_READ_CALLBACK_FNC in_pFunc);

	/*! Create a new dotXSI file.
		\param	SI_Char*	File to create.
		\param	SI_Int	Major version of the file.
		\param	SI_Int	Minro version of the file.
		\return SI_Error	Error code.
	*/
	SI_Error Create(SI_Char *in_szFileName, SI_Int in_nVersionMajor, SI_Int in_nVersionMinor);

	/*! Create a new dotXSI file.
		\param	SI_Int	Major version of the file.
		\param	SI_Int	Minro version of the file.
		\return SI_Error	Error code.
	*/
	SI_Error Create(SI_Int in_nVersionMajor, SI_Int in_nVersionMinor);

	/*! Create a new dotXSI file.
		\param	CXSIParser	Parser to use
		\return SI_Error	Error code.
	*/
	SI_Error Open(CXSIParser * in_parser);
	/*! Populate the semantic layer with data present in the file.
		\return SI_Error	Error code.
	*/
	SI_Error Read();

	/*! Write a dotXSI file containing the data present in the semantic layer.
		\return SI_Error	Error code.
	*/
	SI_Error Write();

	/*! Write a dotXSI file containing the data present in the semantic layer.
		\param	SI_Char*	File to create.
		\return SI_Error	Error code.
	*/
	SI_Error Write(SI_Char* in_szFileName);

	/*! Close the opened/created file.
		\return SI_Error	Error code.
	*/
	SI_Error Close();

	/*! Find a SI_Model by name
		\param	SI_Char*	The model's name
		\return CSLModel*	The model, or NULL if not found.
	*/
	CSLModel*			FindModel(SI_Char* in_szModelName);

	/*! Find a SI_Model, recursively.
		\param	SI_Char*	The model's name
		\param	CSLModel*	A Pointer to a CSLModel
		\return CSLModel*	The model, or NULL if not found.
	*/
	CSLModel*			FindModelRecursively(SI_Char* in_szModelName, CSLModel* in_pModel);

	/*! Find a parameter by name.
		\param	SI_Char*				The XSI path to the parameter (see below)
		\return	CSLAnimatableParameter*	The parameter.

		\note When we say "the XSI path to a parameter", we mean the scripting name for the parameter. For example
		"MyModel.kine.local.posx" would refer to the X component of the position in the model "MyModel" transformation matrix.
	*/
	CSLAnimatableType*	FindParameter(SI_Char* in_szParameterName);

	/*! Gets a parameter value.
		\param	SI_Char*				The XSI path to the parameter (see below)
		\param	SI_Float*				The parameter's value
		\return	SI_Error				Error code
	*/
	SI_Error			GetValue(SI_Char* in_szParameterName, SI_Float* out_pfValue);

	/*! Sets a parameter value.
		\param	SI_Char*				The XSI path to the parameter (see below)
		\param	SI_Float				The parameter's new value
		\return	SI_Error				Error code
	*/
	SI_Error			SetValue(SI_Char* in_szParameterName, SI_Float in_fValue);

	///////////////////////////////////////////////////////////////////////////
	// Accessors
	///////////////////////////////////////////////////////////////////////////
	
	/*! Gets the ambience object.
	*	\return CSLAmbience*	The ambience object
	*/
	CSLAmbience *Ambience();

	/*! Gets the coordinatesystem object
	*	\return CSLCoordinateSystem*	The CoordinateSystem object
	*/
	CSLCoordinateSystem* CoordinateSystem();

	/*! Gets the EnvelopeList object
	*	\return CSLEnvelopeList*	The EnvelopeList object
	*/
	CSLEnvelopeList* EnvelopeList();
	
	/*! Gets the Fog object
	*	\return CSLFog*	The Fog object
	*/
	CSLFog* Fog();

	/*! Gets the MaterialLibrary object
	*	\return CSLMaterialLibrary*	The MaterialLibrary object
	*/
	CSLMaterialLibrary* GetMaterialLibrary();

	/*! Gets the Root Model
	*	\return CSLModel*	The Root Model
	*/
	CSLModel *Root();

	/*! Gets the CXSIParser object
	*	\return CXSIParser*	the CXSIParser object
	*/
	CXSIParser* Parser();

	/*! Gets the Angle object
	*	\return CSLAngle*	The Angle object
	*/
	CSLAngle* Angle();
	
	/*! Gets the FileInfo object
	*	\return CSLFileInfo*	The FileInfo object
	*/
	CSLFileInfo* FileInfo();

	/*! Gets the SceneInfo object
	*	\return CSLSceneInfo*	The SceneInfo object
	*/
	CSLSceneInfo* SceneInfo();

	/*! Gets the Logger object
	*	\return CSLLogger*	The Logger object
	*/
	CSLLogger* Logger();

	/*! Gets the filename for the current scene
	*	\return CSIBCString&	The filename
	*/
	CSIBCString	&FileName();

	/*! Get the image library
		\return CSLImageLibrary Pointer to the image library or null if there's none
	*/
	CSLImageLibrary* GetImageLibrary();

	/*! Removes the Fog object
	*	\return SI_Error	Error code
	*/
	SI_Error DestroyFog();

	/*! Creation of Fog
		\return CSLFog*
	*/
	CSLFog* CreateFog();

	/*! Removes the EnvelopeList object
	*	\return SI_Error	Error code
	*/
	SI_Error DestroyEnvelopeList();

	/*! Creation of envelope list
		\return CSLEnvelopeList*
	*/
	CSLEnvelopeList* CreateEnvelopeList();

	/*! Creation of an Image Library
		\return CSLImageLibrary*
		\note Introduced in 3.6
	*/
	CSLImageLibrary* CreateImageLibrary();

	/*! Removal of the Image Library
		\return Error code
		\note Introduced in 3.6
	*/
	SI_Error DestroyImageLibrary();
	
	
	///////////////////////////////////////////////////////////////////////////
	// Connection functions.
	///////////////////////////////////////////////////////////////////////////
	
	/*! Connect an existing CSLMaterialLibrary object to the scene
		\param	CSLMaterialLibrary*	The new CSLMaterialLibrary
		\return CSLMaterialLibrary* The old CSLMaterialLibrary
	*/
	CSLMaterialLibrary* ConnectMaterialLibrary(CSLMaterialLibrary *in_pNewMaterialLibrary);

	/*! Connect an existing CSLAngle object to the scene
		\param	CSLAngle*	The new CSLAngle
		\return CSLAngle* The old CSLAngle
	*/
	CSLAngle* ConnectAngle(CSLAngle *in_pNewAngle);

	/*! Connect an existing CSLFileInfo object to the scene
		\param	CSLFileInfo*	The new CSLFileInfo
		\return CSLFileInfo* The old CSLFileInfo
	*/
	CSLFileInfo* ConnectFileInfo(CSLFileInfo *in_pNewFileInfo);

	/*! Connect an existing CSLSceneInfo object to the scene
		\param	CSLSceneInfo*	The new CSLSceneInfo
		\return CSLSceneInfo* The old CSLSceneInfo
	*/
	CSLSceneInfo* ConnectSceneInfo(CSLSceneInfo* in_pToConnect);

	/*! Connect an existing CSLFog object to the scene
		\param	CSLFog*	The new CSLFog
		\return CSLFog* The old CSLFog
	*/
	CSLFog* ConnectFog( CSLFog *in_pNewFog );

	/*! Connect an existing CSLAmbience object to the scene
		\param	CSLAmbience*	The new CSLAmbience
		\return CSLAmbience* The old CSLAmbience
	*/
	CSLAmbience* ConnectAmbience( CSLAmbience *in_pNewAmbience );

	/*! Connect an existing CSLCoordinateSystem object to the scene
		\param	CSLCoordinateSystem*	The new CSLCoordinateSystem
		\return CSLCoordinateSystem* The old CSLCoordinateSystem
	*/
	CSLCoordinateSystem* ConnectCoordinateSystem( CSLCoordinateSystem *in_pNewCoordSystem );

	/*! Connect an existing CSLEnvelopeList object to the scene
		\param	CSLEnvelopeList*	The new CSLEnvelopeList
		\return CSLEnvelopeList* The old CSLEnvelopeList
	*/
	CSLEnvelopeList* ConnectEnvelopeList( CSLEnvelopeList* in_pNewEnvList );

	/*! Connect Image library
		\param in_pImageLibrary Pointer to the pre-allocated image library to connect
		\return Pointer to the newly connected image library or null if there's already an image library or if the parser does not support 3.6 templates
		\note The parser must support 3.6 templates in order to connect an image library
	*/
	CSLImageLibrary* ConnectImageLibrary( CSLImageLibrary* in_pImageLibrary );

	/*! Register a new template callback
		\param	SI_Char*	The template name
		\param	SLTemplateCallback *	The template callback structure
		\return SI_Error	Error code

	* <B> Example - Adding a new template callback </B>
	\code 
		//
		// Remember the native callbacks
		//

		SLTemplateCallback* l_pOldCallback = Scene->Callback ( "SI_Mesh" );
		memcpy ( &m_sNativeCallbacks, l_pOldCallback, sizeof ( SLTemplateCallback ));

		//
		// Create new set of callbacks
		//

		SLTemplateCallback	l_NewCallbacks;
		memcpy ( &l_NewCallbacks, l_pOldCallback, sizeof ( SLTemplateCallback ));

		//
		// Add a PostChilren callback and keep the others to the default
		//

		l_NewCallbacks.m_PostChildrenCallback  = SIMeshPostRTPCallback;
		
		//
		// Register the new callbacks
		//

		in_pCurrentScene->RegisterCallback ( "SI_Mesh", &l_NewCallbacks );
	\endcode
	*/
	SI_Error RegisterCallback(SI_Char *in_pName, SLTemplateCallback *in_pTemplateCallback);

	/*! Unregister a template callback
		\param	SI_Char*	The template name
		\return SI_Error	Error code
	*/
	SI_Error UnregisterCallback(SI_Char *in_pName);

	/*! Unregister all template callback
		\return SI_Error	Error code
	*/
	SI_Error UnregisterAllCallbacks();

	/*! Get a callback by name
		\param  SI_Char*	Template name
		\return SLTemplateCallback*	Pointer to a callback struct
	*/
	SLTemplateCallback *Callback(SI_Char *in_pName);

	///////////////////////////////////////////////////////////////////////////
	// Coherancy functions
	///////////////////////////////////////////////////////////////////////////
	SI_Error Fix();
	SI_Error Synchronize();

	///////////////////////////////////////////////////////////////////////////
	// Utility functions
	///////////////////////////////////////////////////////////////////////////
	SI_Error RemoveTemplate(CdotXSITemplate *in_pElement);
	SI_Error ClearSLObjectArray(CSIBCArray<CSLTemplate *> *in_pArray);

	/*! Checks if the parser of the scene supports templates of a specific version of the dotXSI format
		\param in_nMajor Major version of the parser
		\param in_nMinor Minor version of the parser
		\return SI_Bool  True if the version is supported, false otherwise
	*/
	SI_Bool SupportsTemplates( SI_Int in_nMajor, SI_Int in_nMinor );

	/*! Sets the callback that receives warning messages
		\param in_pfncCallback Callback function
	*/
	SI_Void SetWarningCallback ( SI_WARNING_CALLBACK_FNC in_pfncCallback ) { m_pfncCallback = in_pfncCallback; };

	/*! Sets the callback that gets call for reading every template
		\param in_pfncCallback Callback function
	*/
	SI_Void SetReadCallback ( SI_READ_CALLBACK_FNC in_pfncCallback ) { m_pfncReadCallback = in_pfncCallback; };
	/*! Gets the refcount (how many others uses the specified template)
		\param	in_ppObjects	which objects to check
		\param	out_ppRefCounts	Counts for each one
		\param	in_iObjectCount	How many objects in the array
		\param	in_eType		Types (either CSLTemplate::SI_MODEL or CSLTemplate::SI_MATERIAL)
	*/
	SI_Error	GetRefCount( CSLTemplate				** in_ppObjects,
							 SI_Int						** out_ppRefCounts,
							 SI_Int					    in_iObjectCount,
							 CSLTemplate::ETemplateType in_eType );


	static CSLScene * CreateScene();
protected:

	SI_Error	GetRefCountRecursively		( CSLModel* in_pModel, 
													CSLTemplate** in_ppObjects, 
													SI_Int**	out_ppRefCounts, 
													SI_Int		in_iObjectCount, 
													CSLTemplate::ETemplateType in_eType );


	///////////////////////////////////////////////////////////////////////////
	// Callbacks
	///////////////////////////////////////////////////////////////////////////
public:
	static CSLTemplate *SIAmbienceCallback(CdotXSITemplate *in_pTemplate, SLContext *in_pContext);
	static CSLTemplate *SIAngleCallback(CdotXSITemplate *in_pTemplate, SLContext *in_pContext);
	static CSLTemplate *SICameraCallback(CdotXSITemplate *in_pTemplate, SLContext *in_pContext);
	static CSLTemplate *SIClusterCallback(CdotXSITemplate *in_pTemplate, SLContext *in_pContext);
	static CSLTemplate *SIConstraintCallback(CdotXSITemplate *in_pTemplate, SLContext *in_pContext);
	static CSLTemplate *SICurveListCallback(CdotXSITemplate *in_pTemplate, SLContext *in_pContext);
	static CSLTemplate *SIEnvelopeCallback(CdotXSITemplate *in_pTemplate, SLContext *in_pContext);
	static CSLTemplate *SIEnvelopeListCallback(CdotXSITemplate *in_pTemplate, SLContext *in_pContext);
	static CSLTemplate *SIFCurveCallback(CdotXSITemplate *in_pTemplate, SLContext *in_pContext);
	static CSLTemplate *SIFileInfoCallback(CdotXSITemplate *in_pTemplate, SLContext *in_pContext);
	static CSLTemplate *SIGlobalMaterialCallback(CdotXSITemplate *in_pTemplate, SLContext *in_pContext);
	static CSLTemplate *SIIKEffectorCallback(CdotXSITemplate *in_pTemplate, SLContext *in_pContext);
	static CSLTemplate *SIIKJointCallback(CdotXSITemplate *in_pTemplate, SLContext *in_pContext);
	static CSLTemplate *SIIKRootCallback(CdotXSITemplate *in_pTemplate, SLContext *in_pContext);
	static CSLTemplate *SIInstanceCallback(CdotXSITemplate *in_pTemplate, SLContext *in_pContext);
	static CSLTemplate *SILightCallback(CdotXSITemplate *in_pTemplate, SLContext *in_pContext);
	static CSLTemplate *SIMaterialCallback(CdotXSITemplate *in_pTemplate, SLContext *in_pContext);
	static CSLTemplate *SIMaterialLibraryCallback(CdotXSITemplate *in_pTemplate, SLContext *in_pContext);
	static CSLTemplate *SIMeshCallback(CdotXSITemplate *in_pTemplate, SLContext *in_pContext);
	static CSLTemplate *SIModelCallback(CdotXSITemplate *in_pTemplate, SLContext *in_pContext);
	static CSLTemplate *SINurbsCurveCallback(CdotXSITemplate *in_pTemplate, SLContext *in_pContext);
	static CSLTemplate *SINurbsSurfaceCallback(CdotXSITemplate *in_pTemplate, SLContext *in_pContext);
	static CSLTemplate *SIPolygonListCallback(CdotXSITemplate *in_pTemplate, SLContext *in_pContext);
	static CSLTemplate *SIShape35Callback(CdotXSITemplate *in_pTemplate, SLContext *in_pContext);
	static CSLTemplate *SIShapeAnimationCallback(CdotXSITemplate *in_pTemplate, SLContext *in_pContext);
	static CSLTemplate *SIShapeCallback(CdotXSITemplate *in_pTemplate, SLContext *in_pContext);
	static CSLTemplate *SITransformCallback(CdotXSITemplate *in_pTemplate, SLContext *in_pContext);
	static CSLTemplate *SITriangleListCallback(CdotXSITemplate *in_pTemplate, SLContext *in_pContext);
	static CSLTemplate *SITriangleStripCallback(CdotXSITemplate *in_pTemplate, SLContext *in_pContext);
	static CSLTemplate *SITriangleStripListCallback(CdotXSITemplate *in_pTemplate, SLContext *in_pContext);
	static CSLTemplate *SISceneInfoCallback(CdotXSITemplate *in_pTemplate, SLContext *in_pContext);
	static CSLTemplate *SISurfaceMeshCallback(CdotXSITemplate *in_pTemplate, SLContext *in_pContext);
	static CSLTemplate *SIVisibilityCallback(CdotXSITemplate *in_pTemplate, SLContext *in_pContext);
	static CSLTemplate *XSIShaderCallback(CdotXSITemplate *in_pTemplate, SLContext *in_pContext);
	static CSLTemplate *XSIShaderInstanceDataCallback(CdotXSITemplate *in_pTemplate, SLContext *in_pContext);
	static CSLTemplate *XSIMaterialCallback(CdotXSITemplate *in_pTemplate, SLContext *in_pContext);	
	static CSLTemplate *XSICustomPSetCallback(CdotXSITemplate *in_pTemplate, SLContext *in_pContext);
	static CSLTemplate *XSIUserDataBlobCallback(CdotXSITemplate *in_pTemplate, SLContext *in_pContext);
	static CSLTemplate *XSITransformCallback(CdotXSITemplate *in_pTemplate, SLContext *in_pContext);
	static CSLTemplate *XSIBasePoseCallback(CdotXSITemplate *in_pTemplate, SLContext *in_pContext);
	static CSLTemplate *XSILimitCallback(CdotXSITemplate *in_pTemplate, SLContext *in_pContext);

	static CSLTemplate *XSIStaticValueCallback(CdotXSITemplate *in_pTemplate, SLContext *in_pContext);
	static CSLTemplate *XSITrackCallback(CdotXSITemplate *in_pTemplate, SLContext *in_pContext);
	static CSLTemplate *XSIMixerCallback(CdotXSITemplate *in_pTemplate, SLContext *in_pContext);
	static CSLTemplate *XSITimeControlCallback(CdotXSITemplate *in_pTemplate, SLContext *in_pContext);
	static CSLTemplate *XSIExtrapolationCallback(CdotXSITemplate *in_pTemplate, SLContext *in_pContext);
	static CSLTemplate *XSIActionClipCallback(CdotXSITemplate *in_pTemplate, SLContext *in_pContext);
	static CSLTemplate *XSIActionCallback(CdotXSITemplate *in_pTemplate, SLContext *in_pContext);
	
	static CSLTemplate *SICoordinateSystemCallback(CdotXSITemplate *in_pTemplate, SLContext *in_pContext);
	static CSLTemplate *SIFogCallback(CdotXSITemplate *in_pTemplate, SLContext *in_pContext);
	static CSLTemplate *SINullCallback(CdotXSITemplate *in_pTemplate, SLContext *in_pContext);
	static CSLTemplate *SITexture2DCallback(CdotXSITemplate *in_pTemplate, SLContext *in_pContext);

	static CSLTemplate *XSIImageLibraryCallback(CdotXSITemplate *in_pTemplate, SLContext *in_pContext);
	static CSLTemplate *XSIImageCallback(CdotXSITemplate *in_pTemplate, SLContext *in_pContext);
	static CSLTemplate *XSIImageDataCallback(CdotXSITemplate *in_pTemplate, SLContext *in_pContext);
	static CSLTemplate *XSIImageFXCallback(CdotXSITemplate *in_pTemplate, SLContext *in_pContext);
	static CSLTemplate *SILightInfoCallback(CdotXSITemplate *in_pTemplate, SLContext *in_pContext);
	static CSLTemplate *XSIMaterialInfoCallback(CdotXSITemplate *in_pTemplate, SLContext *in_pContext);
	static CSLTemplate *XSINurbsProjectionCallback(CdotXSITemplate *in_pTemplate, SLContext *in_pContext);
	static CSLTemplate *XSINurbsTrimCallback(CdotXSITemplate *in_pTemplate, SLContext *in_pContext);
	static CSLTemplate *XSICustomParamInfoCallback(CdotXSITemplate *in_pTemplate, SLContext *in_pContext);
	static CSLTemplate *XSIFXTreeCallback(CdotXSITemplate *in_pTemplate, SLContext *in_pContext);
	static CSLTemplate *XSIFXOperatorCallback(CdotXSITemplate *in_pTemplate, SLContext *in_pContext);
	static CSLTemplate *XSIUserDataCallback(CdotXSITemplate *in_pTemplate, SLContext *in_pContext);
	static CSLTemplate *XSIUserDataListCallback(CdotXSITemplate *in_pTemplate, SLContext *in_pContext);
	static CSLTemplate *XSIMeshCallback(CdotXSITemplate *in_pTemplate, SLContext *in_pContext);
	static CSLTemplate *XSIShapeCallback(CdotXSITemplate *in_pTemplate, SLContext *in_pContext);
	static CSLTemplate *XSIShapeAnimationCallback(CdotXSITemplate *in_pTemplate, SLContext *in_pContext);
	static CSLTemplate *XSIPolygonListCallback(CdotXSITemplate *in_pTemplate, SLContext *in_pContext);
	static CSLTemplate *XSITriangleListCallback(CdotXSITemplate *in_pTemplate, SLContext *in_pContext);
	static CSLTemplate *XSITriangleStripListCallback(CdotXSITemplate *in_pTemplate, SLContext *in_pContext);
	static CSLTemplate *XSIVertexListCallback(CdotXSITemplate *in_pTemplate, SLContext *in_pContext);
	static CSLTemplate *XSISubComponentAttributeListCallback(CdotXSITemplate *in_pTemplate, SLContext *in_pContext);
	static CSLTemplate *XSIClusterInfoCallback(CdotXSITemplate *in_pTemplate, SLContext *in_pContext);
	
	// Post load callbacks
	static SI_Error DefaultPostChildrenCallback(CSLTemplate *in_pTemplate, SLContext *in_pContext);
	static SI_Error SIClusterPostLoadCallback(SLContext *in_pContext);
	static SI_Error SIConstraintPostLoadCallback(SLContext *in_pContext);
	static SI_Error SIEnvelopePostLoadCallback(SLContext *in_pContext);
	static SI_Error SIFCurvePostLoadCallback(SLContext *in_pContext);
	static SI_Error SIGlobalMaterialPostLoadCallback(SLContext *in_pContext);
	static SI_Error SIIKRootPostLoadCallback(SLContext *in_pContext);
	static SI_Error SIInstancePostLoadCallback(SLContext *in_pContext);
	static SI_Error SIMaterialPostLoadCallback(SLContext *in_pContext);
	static SI_Error SIPolygonListPostLoadCallback(SLContext *in_pContext);
	static SI_Error SITriangleListPostLoadCallback(SLContext *in_pContext);
	static SI_Error SITriangleStripListPostLoadCallback(SLContext *in_pContext);
	static SI_Error XSIShaderPostLoadCallback(SLContext *in_pContext);
	static SI_Error XSIShaderInstanceDataPostLoadCallback(SLContext *in_pContext);
	static SI_Error XSIMaterialPostLoadCallback(SLContext *in_pContext);
	static SI_Error XSIFXTreePostLoadCallback(SLContext *in_pContext);
	static SI_Error XSIPolygonListPostLoadCallback(SLContext *in_pContext);
	static SI_Error XSITriangleListPostLoadCallback(SLContext *in_pContext);
	static SI_Error XSITriangleStripListPostLoadCallback(SLContext *in_pContext);
	
private:
	CSLModel	* FindFullyQualifiedObject( SI_Char * in_pName, SI_Char ** out_pContainer, SI_Char ** out_pParameter );
	SI_Error PopulateScene();
	SI_Error ReadTemplateRecursively(CdotXSITemplate *in_pTemplate, SLContext *in_pContext);
	SI_Error ExecutePreLoadCallbacks(SLContext *in_pContext);
	SI_Error ExecutePostLoadCallbacks(SLContext *in_pContext);
	SI_Error RegisterAllCallbacks();
	SI_Error Clear();

#ifdef	FTK_KERNEL
	typedef SI_StrMap<SLTemplateCallback> CSLCallbackMap;
	typedef CSLCallbackMap::iterator CSLCallbackMapIterator;
#else
	typedef	void	CSLCallbackMap;
#endif

	CSLSceneInfo* m_pSceneInfo;
	CSLFileInfo* m_pFileInfo;
	CSLAmbience* m_pAmbience;
	CSLAngle* m_pAngle;
	CSLFog* m_pFog;
	CSLEnvelopeList* m_pEnvelopeList;
	CSLMaterialLibrary* m_pMaterialLibrary;
	CSLCoordinateSystem* m_pCoordinateSystem;
	CXSIParser* m_pParser;
	CSLCallbackMap* m_pCallbackMap;
	CSLModel *m_pRoot;
	CSLLogger m_Logger;
	CSLImageLibrary *m_pImageLibrary;

	CSIBCString m_FileName;

	SI_WARNING_CALLBACK_FNC	m_pfncCallback;
	SI_READ_CALLBACK_FNC	m_pfncReadCallback;

	// This member is here to allow future improvement of the class.
	void *m_pReserved;
};

#endif

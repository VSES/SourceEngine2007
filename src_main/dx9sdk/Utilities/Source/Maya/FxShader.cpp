
#include "pch.h"

//MTypeId LegacyId( 0x68038 );


MTypeId DirectXShader::id( 0x68038 );
MObject DirectXShader::aColor;
MObject DirectXShader::aColorR;
MObject DirectXShader::aColorG;
MObject DirectXShader::aColorB;
MObject DirectXShader::aOutColor;
MObject DirectXShader::aOutColorR;
MObject DirectXShader::aOutColorG;
MObject DirectXShader::aOutColorB;
MObject DirectXShader::aNormalCamera;
MObject DirectXShader::aNormalCameraX;
MObject DirectXShader::aNormalCameraY;
MObject DirectXShader::aNormalCameraZ;
//DIRECTX Attributes
MObject DirectXShader::aFile;
MObject DirectXShader::aScript;
MObject DirectXShader::aLoading;

MString DirectXShader::AnchorLongName("FxInternal");
MString DirectXShader::AnchorShortName("FxInternal");
MString DirectXShader::RootLongName("FxRoot");
MString DirectXShader::RootShortName("Fx");
CRBMap<FxInternal*, FxInternal*> DirectXShader::ActiveFx;


MCallbackId	DirectXShader::NodeRemovedCID= 0;
MCallbackId	DirectXShader::SceneOpenedCID= 0;

void DirectXShader::StaticInitialize()
{
	if(NodeRemovedCID==0)
		NodeRemovedCID= MDGMessage::addNodeRemovedCallback(DirectXShader::DispatchNodeRemoved);

	if(SceneOpenedCID==0)
		SceneOpenedCID= MSceneMessage::addCallback( MSceneMessage::kAfterOpen , DirectXShader::DispatchSceneOpened, NULL, NULL );

	LoadLegacySaves();
}

void DirectXShader::DispatchSceneOpened(void* clientData )
{
	LoadLegacySaves();
}

void DirectXShader::LoadLegacySaves()
{
	//kSurfaceShader doesnt work for this type
	for( MItDependencyNodes depItr;
		!depItr.isDone();
		depItr.next())
	{
		MObject obj= depItr.item();
		MFnDependencyNode depNode(obj);
		if(depNode.typeId() == DirectXShader::id)
		{
			MString filename;

			{//FILEPATH FILENAME
				MObject oFilePath;
				MPlug plugFilePath= depNode.findPlug("DXCC_FxFile_Path");
				if(plugFilePath.isNull())
					continue; 

				plugFilePath.getValue(oFilePath);

				MFnStringData sFilePathData(oFilePath);
				filename= sFilePathData.string();

				depNode.removeAttribute( plugFilePath.attribute() );
			}//END//FILEPATH


			MPlug newFilePlug( depNode.object(), DirectXShader::aFile);
			DXCC_ASSERT(!newFilePlug.isNull());


			DXCHECK_MSTATUS( newFilePlug.setValue(filename) );
		}
	}
}


void DirectXShader::StaticDestroy()
{
	if(NodeRemovedCID!=0)
	{
		MMessage::removeCallback(NodeRemovedCID);
		NodeRemovedCID= 0;
	}

	if(SceneOpenedCID!=0)
	{
		MMessage::removeCallback(SceneOpenedCID);
		SceneOpenedCID= 0;
	}

	for(POSITION pos= DirectXShader::ActiveFx.GetHeadPosition();
		pos != NULL;
		DirectXShader::ActiveFx.GetNext(pos))
	{
		FxInternal* fxInternal= DirectXShader::ActiveFx.GetValueAt(pos);
		fxInternal->Destroy();
		delete fxInternal;
	}

	DirectXShader::ActiveFx.RemoveAll();
}

void DirectXShader::DispatchNodeRemoved(MObject & node, void* clientData )
{
	MFnDependencyNode depNode(node);

	if(depNode.typeId() != DirectXShader::id )
		return;

	DXMAnchor* anchor= DXMAnchor::GetAnchor(node, DirectXShader::AnchorShortName );
	if(anchor)
	{
		anchor->Destroy();
		delete anchor;
	}
}


void* DirectXShader::creator()
{
	return new DirectXShader();
}


// The initialize method is called only once when the node is first
// registered with Maya. In this method you define the attributes of the
// node, what data comes in and goes out of the node that other nodes may
// want to connect to. 
//

MStatus DirectXShader::initialize()
{
	MFnNumericAttribute nAttr; 
	MFnTypedAttribute tAttr; 
	MFnCompoundAttribute cAttr; 

	MStatus status; 

	// Input Attributes
	aColorR = nAttr.create( "colorR", "cr",MFnNumericData::kFloat, 0, &status );
	DXCHECK_MSTATUS( status );
	DXCHECK_MSTATUS( nAttr.setKeyable( true ) );
	DXCHECK_MSTATUS( nAttr.setStorable( true ) );
	DXCHECK_MSTATUS( nAttr.setDefault( 0.0f ) );

	aColorG = nAttr.create( "colorG", "cg", MFnNumericData::kFloat, 0, &status );
	DXCHECK_MSTATUS( status );
	DXCHECK_MSTATUS( nAttr.setKeyable( true ) );
	DXCHECK_MSTATUS( nAttr.setStorable( true ) );
	DXCHECK_MSTATUS( nAttr.setDefault( 0.58824f ) );

	aColorB = nAttr.create( "colorB", "cb",MFnNumericData::kFloat, 0, &status );
	DXCHECK_MSTATUS( status );
	DXCHECK_MSTATUS( nAttr.setKeyable( true ) );
	DXCHECK_MSTATUS( nAttr.setStorable( true ) );
	DXCHECK_MSTATUS( nAttr.setDefault( 0.644f ) );

	aColor = nAttr.create( "color", "c", aColorR, aColorG, aColorB, &status );
	DXCHECK_MSTATUS( status );
	DXCHECK_MSTATUS( nAttr.setKeyable( true ) );
	DXCHECK_MSTATUS( nAttr.setStorable( true ) );
	DXCHECK_MSTATUS( nAttr.setDefault( 0.0f, 0.58824f, 0.644f ) );
	DXCHECK_MSTATUS( nAttr.setUsedAsColor( true ) );


	// Color Output
	aOutColorR = nAttr.create( "outColorR", "ocr", MFnNumericData::kFloat, 0, &status );
	DXCHECK_MSTATUS( status );
	
	aOutColorG = nAttr.create( "outColorG", "ocg", MFnNumericData::kFloat, 0, &status );
	DXCHECK_MSTATUS( status );
	
	aOutColorB = nAttr.create( "outColorB", "ocb", MFnNumericData::kFloat, 0, &status );
	DXCHECK_MSTATUS( status );
	
	aOutColor = nAttr.create( "outColor", "oc", aOutColorR, aOutColorG, aOutColorB, &status );
	DXCHECK_MSTATUS( status );
	
	DXCHECK_MSTATUS( nAttr.setHidden( false ) );
	DXCHECK_MSTATUS( nAttr.setReadable( true ) );
	DXCHECK_MSTATUS( nAttr.setWritable( false ) );

	// Camera Normals
	aNormalCameraX = nAttr.create( "normalCameraX", "nx", MFnNumericData::kFloat, 0, &status );
	DXCHECK_MSTATUS( status );
	DXCHECK_MSTATUS( nAttr.setStorable( false ) );
	DXCHECK_MSTATUS( nAttr.setDefault( 1.0f ) );

	aNormalCameraY = nAttr.create( "normalCameraY", "ny", MFnNumericData::kFloat, 0, &status );
	DXCHECK_MSTATUS( status );
	DXCHECK_MSTATUS( nAttr.setStorable( false ) );
	DXCHECK_MSTATUS( nAttr.setDefault( 1.0f ) );

	aNormalCameraZ = nAttr.create( "normalCameraZ", "nz", MFnNumericData::kFloat, 0, &status );
	DXCHECK_MSTATUS( status );
	DXCHECK_MSTATUS( nAttr.setStorable( false ) );
	DXCHECK_MSTATUS( nAttr.setDefault( 1.0f ) );

	aNormalCamera = nAttr.create( "normalCamera", "n", aNormalCameraX, aNormalCameraY, aNormalCameraZ, &status );
	DXCHECK_MSTATUS( status );
	DXCHECK_MSTATUS( nAttr.setStorable( false ) );
	DXCHECK_MSTATUS( nAttr.setDefault( 1.0f, 1.0f, 1.0f ) );
	DXCHECK_MSTATUS( nAttr.setHidden( true ) );



	aScript = tAttr.create( "FxUiScript", "FxScript",  MFnData::kString,  MObject::kNullObj , &status );
	DXCHECK_MSTATUS( status );
	AttributeMakePrivate(tAttr);


	aFile = tAttr.create( "FxFilename", "FxFile",  MFnData::kString,  MObject::kNullObj , &status );
	DXCHECK_MSTATUS( status );
	AttributeMakePublic(tAttr);
	DXCHECK_MSTATUS( tAttr.setInternal( true ) );

	aLoading = nAttr.create( "FxLoadingFlag", "FxLoading", MFnNumericData::kBoolean,  0 , &status );
	DXCHECK_MSTATUS( status );
	AttributeMakePrivate(nAttr);




	DXCHECK_MSTATUS( addAttribute( aColor ) );
	DXCHECK_MSTATUS( addAttribute( aOutColor ) );
	DXCHECK_MSTATUS( addAttribute( aNormalCamera ) );


	DXCHECK_MSTATUS( addAttribute( aScript ) );
	DXCHECK_MSTATUS( addAttribute( aFile ) );
	DXCHECK_MSTATUS( addAttribute( aLoading ) );




	DXCHECK_MSTATUS( attributeAffects( aColorR, aOutColor ) );
	DXCHECK_MSTATUS( attributeAffects( aColorG, aOutColor ) );
	DXCHECK_MSTATUS( attributeAffects( aColorB, aOutColor ) );
	DXCHECK_MSTATUS( attributeAffects( aColor, aOutColor ) );
	DXCHECK_MSTATUS( attributeAffects( aNormalCameraX, aOutColor ) );
	DXCHECK_MSTATUS( attributeAffects( aNormalCameraY, aOutColor ) );
	DXCHECK_MSTATUS( attributeAffects( aNormalCameraZ, aOutColor ) );
	DXCHECK_MSTATUS( attributeAffects( aNormalCamera, aOutColor ) );



	return( MS::kSuccess );
}

void DirectXShader::postConstructor( )
{
	setMPSafe( true );
}


bool DirectXShader::setInternalValue( const MPlug& plug, const MDataHandle& dataIn)
{
	HRESULT hr= S_OK;
	MStatus status= MStatus::kSuccess; 

	if(plug == aFile)
	{
		MString oldFile;
		plug.getValue(oldFile);
		LPCSTR oldFileStr= oldFile.asChar();
		MString& newFile= dataIn.asString();		
		LPCSTR newFileStr= newFile.asChar();

		//we disregard changes to null string.
		if(newFile == "")
			return true;//we handled it...no default handler to change the value
		
		
		FxInternal* effect= reinterpret_cast<FxInternal*>( DXMAnchor::GetAnchor( plug.node(), DirectXShader::AnchorShortName ) );
		if(!effect)
		{
			effect= new FxInternal();
			DXMAnchor::AddAnchor(plug.node(), DirectXShader::AnchorLongName, DirectXShader::AnchorShortName, effect);
		}


		CComPtr<IDXCCShaderProperties> pShaderProp;
		DirectXShader::CreateShaderProperties(plug.node(), false, NULL, &pShaderProp);

		bool result;

		MPlug loading(plug.node(), aLoading);
		loading.setValue(true);//SIGNAL LOADING



		CStringA KeepExistingValuesMsg;
		if(oldFile != "")
		{
			DirectXShader::CreateShaderProperties(plug.node(), false, NULL, &pShaderProp);

			if(newFile != oldFile)
				KeepExistingValuesMsg = "Would you like the new Effect to keep the values matching existing Effect parameters?";
			else
				KeepExistingValuesMsg = "Would you like the reloaded Effect to keep its existing values?  If the file has changes, only matching Effect parameters will be kept.";
		}

		LPCSTR KeepExistingValuesMsgPtr= KeepExistingValuesMsg.GetLength() == 0 ? NULL : KeepExistingValuesMsg.GetString();

		if( false == effect->EffectLoad(newFile.asChar(), pShaderProp, KeepExistingValuesMsgPtr) ) 
		{
			CStringA ErrorScriptMsg;

			bool bCreateErrorScript= false;

			if(oldFile == "")
			{
				MessageBoxA((HWND)M3dView::applicationShell(), "The Effect could not be loaded." , "DirectXShader File Loader", MB_ICONEXCLAMATION);
				//DO NOTHING
			}
			else if( newFile == oldFile)
			{
				bCreateErrorScript= true;

				ErrorScriptMsg = "The shader could not be reloaded.  Would you like to preserve the old parameters values for future use?";
				//PROVIDE ERROR IN AE SCRIPT
			}
			else
			{
				//attempt to restore old effect
				if( false == effect->EffectLoad(oldFile.asChar(), pShaderProp, NULL) )
				{
					bCreateErrorScript= true;
					ErrorScriptMsg = "The Effect could not be loaded and the last Effect could not be restored.  Would you like to preserve the parameters values from your last Effect for future use?";
					//PROVIDE ERROR IN AE SCRIPT
				}
				else
				{//msg on successful restore
					
					MessageBoxA((HWND)M3dView::applicationShell(), "The Effect could not be loaded.  The last Effect was restored successfully." , "DirectXShader File Loader", MB_ICONEXCLAMATION);
				}
			}
			//Add error message into AE
			if( bCreateErrorScript )
			{
				MString BadScript= "columnLayout -adjustableColumn true; text -backgroundColor 1.0 0.5 0.5 -label \"ERROR LOADING EFFECT FILE\"; ";

				MPlug scriptPlug( plug.node(), aScript);
				if(oldFile == ""
				|| IDNO == MessageBoxA((HWND)M3dView::applicationShell(), ErrorScriptMsg.GetString() , "DirectXShader File Loader", MB_YESNO|MB_ICONQUESTION))
				{	
					effect->EffectUnload();
				}
				else
				{
					MString OldScript;
					scriptPlug.getValue( OldScript );

					MString BadTest = OldScript.substring(0, BadScript.length()-1);

					if(BadScript == BadTest)
						BadScript = OldScript;
					else//if we dont have the BadScript prefix then generate it.
						BadScript = BadScript + "dxfxStackUp(\"Preserved Effect Parameters\", 0); " + OldScript + "dxfxStackDown(1);";
				}

				scriptPlug.setValue( BadScript );

				MGlobal::executeCommand( "resetAE;" );

			}

			result= true; //we handled it...we dont want the value to change so avoid the default handler	
		}
		else
		{
			//__super::setInternalValue(plug, dataIn);
			result= false; //let the default handler change the value;
		}

		loading.setValue(false);//UNSIGNAL LOADING

		return result;
	}
	else
		return false; // MPxNode::setInternalValue(plug, dataIn);
}


MStatus DirectXShader::compute( const MPlug& plug, MDataBlock& block ) 
{ 
	// The plug parameter will allow us to determine which output attribute
	// needs to be calculated.
	//
	if( plug == aOutColor
			|| plug == aOutColorR
			|| plug == aOutColorG
			|| plug == aOutColorB)
	{
		MStatus status= MS::kSuccess;
		MFloatVector resultColor( 0.0, 0.0, 0.0 ); 

	
		// Get surface shading parameters from input block
		//
		MFloatVector& surfaceNormal = block.inputValue( aNormalCamera,
				&status ).asFloatVector();
		DXCHECK_MSTATUS( status );

		MFloatVector& surfaceColor = block.inputValue( aColor,
				&status ).asFloatVector();
		DXCHECK_MSTATUS( status );

		// Factor incident light with surface color and add incandescence
		//

		float dot= -surfaceNormal[1];
		if(dot < 0) dot = 0;
		resultColor[0] = dot * surfaceColor[0];
		resultColor[1] = dot * surfaceColor[1];
		resultColor[2] = dot * surfaceColor[2];


		if ( plug == aOutColor || plug == aOutColorR || plug == aOutColorG
				|| plug == aOutColorB)
		{
			// Get the handle to the attribute
			//
			MDataHandle outColorHandle = block.outputValue( aOutColor,
					&status );
			DXCHECK_MSTATUS( status );
			MFloatVector& outColor = outColorHandle.asFloatVector();
		
			outColor = resultColor;		// Set the output value
			outColorHandle.setClean(); // Mark the output value as EraseInvalidTags
		}
	} 

	return( MS::kSuccess );
}


HRESULT 
DirectXShader::CreateShaderProperties(MObject& obj, bool bMakeRes, LPDXCCRESOURCE* ppRes, LPDXCCSHADERPROPERTIES* ppShaderProp)
{
	MFnDependencyNode depNode(obj);

	if(depNode.typeId() != DirectXShader::id)
		return E_FAIL;		

	FxInternal* fxInternal= reinterpret_cast<FxInternal*>(DXMAnchor::GetAnchor(obj, DirectXShader::AnchorShortName) );

	if(!fxInternal)
		return E_FAIL;		
		
	if(!fxInternal->Effect)
		return E_FAIL;

	MString fileName;
	MPlug filePlug(obj, DirectXShader::aFile);
	filePlug.getValue(fileName);		
	LPCSTR fileNameStr= fileName.asChar();
	if(fileName.length() == 0)
		return E_FAIL;

	
	CComPtr<IDXCCPropertyBag> pBag= NULL;
	DXCCCreateShaderProperties(ppShaderProp);
	(*ppShaderProp)->GetProperties(&pBag);

	if(bMakeRes)
	{
		CComPtr<IDXCCResource> pRes= NULL;

		g_PreviewPipeline.AccessManager()->CreateResource((*ppShaderProp), IID_IDXCCShaderProperties, true, &pRes);
		MString name= MFnDependencyNode( obj ).name();
		pRes->SetName( name.asChar() );

		if(ppRes)
		{
			pRes.CopyTo(ppRes);
		}
	}

	(*ppShaderProp)->SetType( "ID3DXEFFECT" );



	DXCCPROPERTY_KEY fileKey= NULL;
	pBag->AddProperty("DXCCResourcePath", &fileKey);
	pBag->SetPropertyValueAsData(fileKey, DXCCPD_STRING, fileName.asChar());

	    
	for(UINT i= 0; i < depNode.attributeCount(); i++)
	{

		MObject attrObj= depNode.attribute(i);
		MFnAttribute attrib(attrObj);

		if(attrib.parent().isNull())
		{
			MPlug plug(obj, attrObj);
			Internal_CreateShaderProperties_RecursePlugs(fxInternal, plug, pBag);
		}
	}

	return S_OK;
}

void 
DirectXShader::Internal_CreateShaderProperties_RecursePlugs(
	FxInternal* fxInternal,
	MPlug& plug, 
	LPDXCCPROPERTYBAG pBag)
{
    if( plug.isArray() )
	{
		for(UINT i= 0; i < plug.numElements(); i++)
		{
            Internal_CreateShaderProperties_RecursePlugs( fxInternal, plug.elementByLogicalIndex(i), pBag );
		}
	}
	else if(plug.isCompound())
	{
		for(UINT i= 0; i < plug.numChildren(); i++)
		{
            Internal_CreateShaderProperties_RecursePlugs( fxInternal, plug.child(i), pBag );
		}
	}

    CStringA plugStr= fxInternal->EncodePlug(plug);
    CStringA propertyPathName= fxInternal->TranscodePlugToHandle(plugStr);
    if(propertyPathName.GetLength() > 0)
	{
		D3DXHANDLE handle= fxInternal->DecodeHandle( propertyPathName );
		if(handle != NULL)
		{
			D3DXPARAMETER_DESC d3dxdesc;
			fxInternal->Effect->GetParameterDesc(handle, &d3dxdesc);

			DXCCPROPERTY_KEY key= 0;
			LPDXCCPROPERTYCOLLECTION pCollection= NULL;

			if(DXCC_SUCCEEDED( pBag->GetOrMakePath(propertyPathName, &pCollection, &key) ))
			{
				Internal_CreateShaderProperties_DoPlugExport(fxInternal, plug, handle, pCollection, key);
			}

			DXCC_RELEASE(pCollection);
		}
	}
}
    
void
DirectXShader::Internal_CreateShaderProperties_DoPlugExport(
	FxInternal* fxInternal,
	const MPlug& plug, 
	D3DXHANDLE& handle,
	LPDXCCPROPERTYCOLLECTION pCollection, 
	DXCCPROPERTY_KEY key)
{

	DXCCPROPERTY_DESC dxccdesc;

	D3DXPARAMETER_DESC d3dxdesc;
	fxInternal->Effect->GetParameterDesc( handle, &d3dxdesc);

	if(d3dxdesc.Type == D3DXPT_STRING
		|| d3dxdesc.Type == D3DXPT_TEXTURE
		|| d3dxdesc.Type == D3DXPT_TEXTURE1D
		|| d3dxdesc.Type == D3DXPT_TEXTURE2D
		|| d3dxdesc.Type == D3DXPT_TEXTURE3D
		|| d3dxdesc.Type == D3DXPT_TEXTURECUBE)
	{
		dxccdesc = DXCCPD_STRING;

		MString sValue;
		plug.getValue(sValue);
		pCollection->SetPropertyValueAsData(key, dxccdesc, sValue.asChar());
	}
	else if(d3dxdesc.Type == D3DXPT_BOOL
	|| d3dxdesc.Type == D3DXPT_INT
	|| d3dxdesc.Type == D3DXPT_FLOAT)
	{
		DXCCPROPERTY_CLASS pc;
		DXCCPROPERTY_TYPE pt= DXCCPT_FLOAT;

		if(d3dxdesc.Rows == 1 && d3dxdesc.Columns == 1)
			pc= DXCCPC_SCALAR;
		else if (d3dxdesc.Rows == 1)
			pc= DXCCPC_VECTOR;
		else
			pc= DXCCPC_MATRIX;

		dxccdesc= DXCCPD_CREATE(pt, pc, d3dxdesc.Rows, d3dxdesc.Columns);

		float fValues[16];
		if(plug.isArray())
		{
			for(UINT physicalIndex= 0; physicalIndex < plug.numElements() ; physicalIndex++)
			{
				MPlug subPlug= plug.elementByPhysicalIndex(physicalIndex);
				subPlug.getValue( fValues[physicalIndex] );
			}
		
			pCollection->SetPropertyValueAsData(key, dxccdesc, fValues);
		}
		else
		{
			plug.getValue(fValues[0]);
			pCollection->SetPropertyValueAsData(key, dxccdesc, fValues);
		}
	}

	return;
}




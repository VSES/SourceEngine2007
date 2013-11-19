#include "pch.h"

HRESULT FxAttributeFiller::Run(FxInternal* myShader, LPDXCCSHADERPROPERTIES pRestoreProps, LPCSTR RestoreMsg)
{
	HRESULT hr= S_OK;

	Shader= myShader;
	Restore= NULL;
	if(pRestoreProps)
	{
		if(RestoreMsg == NULL
		|| IDYES == MessageBoxA((HWND)M3dView::applicationShell(), RestoreMsg , "DirectX Effect Conversion", MB_YESNO|MB_ICONQUESTION) )
		{
			pRestoreProps->GetProperties( &Restore );
		}
	}

	{//ScriptGen
		Script= "";
	}//ScriptGen

	hr= DXCCEnumEffectParameters(Shader->Effect, this);

	{//ScriptGen
		MPlug plugScript(Shader->GetSite(), DirectXShader::aScript);
		plugScript.setValue(Script.GetString());
#ifdef DEBUG
		//MessageBoxA(NULL, Script.GetString(), "ScriptGen", 0); 
#endif
		Script= "";
	}//ScriptGen

	return hr;
}


HRESULT FxAttributeFiller::PreEnumParameter(LPD3DXEFFECT pEffect, DXCCEffectPath& parameter, BOOL& GoDeeper, BOOL& DoEnum, BOOL& DoPost)
{
	SasUiLabel = FxParameterGetUiLabel(pEffect, parameter);


	if( parameter.End->Description.Class == D3DXPC_OBJECT)
	{
		switch( parameter.End->Description.Type )
		{
		case D3DXPT_STRING:
		case D3DXPT_TEXTURE:
		case D3DXPT_TEXTURE1D:
		case D3DXPT_TEXTURE2D:
		case D3DXPT_TEXTURE3D:
		case D3DXPT_TEXTURECUBE:
			break;
		default:
			{
				GoDeeper= FALSE;
				DoEnum= FALSE;
				DoPost= FALSE;
				return S_OK;
			}
			break;
		}
	}



	if(parameter.Length == 1)
	{
		SasUiVisible = FxParameterGetUiVisible(pEffect, parameter);
		if(SasUiVisible == TRUE)
		{
			SasUiControl = FxParameterGetUiControl(pEffect, parameter);

			if(0 == lstrcmpiA( SasUiControl.asChar(), "None"))
				SasUiVisible= FALSE;
		}

		if(SasUiVisible == FALSE)
		{
			GoDeeper= FALSE;
			DoEnum= FALSE;
			DoPost= FALSE;
			return S_OK;
		}


		if(0 == lstrcmpiA( SasUiControl.asChar(), "Slider"))
		{
			bool validSlider= true;

			D3DXHANDLE hMin= DXCCFindEffectAnnotation(pEffect, parameter.Root->Handle, "SasUiMin");
			D3DXHANDLE hMax= DXCCFindEffectAnnotation(pEffect, parameter.Root->Handle, "SasUiMax");

			if(	hMin != NULL 
				&& hMax != NULL
				&& DXCC_SUCCEEDED( pEffect->GetFloat(hMin, &SasUiMin)) 				
				&& DXCC_SUCCEEDED( pEffect->GetFloat(hMax, &SasUiMax)) )
			{
				SasUiSteps= 100;
				D3DXHANDLE hSteps= DXCCFindEffectAnnotation(pEffect, parameter.Root->Handle, "SasUiSteps");
				if(hSteps != NULL)
					pEffect->GetInt(hSteps, &SasUiSteps);
			}
			else //FAILED TO GET VALID SLIDER
			{
				SasUiControl = "Any";
			}
		}
	}



	Script.AppendFormat("dxfxStackUp( \"%s\", %d );\n", SasUiLabel.asChar(), (parameter.End->NumChildren() == 0) );

	if(parameter.End->NumChildren() != 0)
	{
		DoEnum= FALSE;
		return S_OK;
	}


	return S_OK;

}

HRESULT FxAttributeFiller::EnumParameter(LPD3DXEFFECT pEffect, DXCCEffectPath& parameter, BOOL& GoDeeper, BOOL& DoPost)
{
	HRESULT hr= S_OK;
	MStatus stat= MS::kSuccess;

	MFnDependencyNode depNode(Shader->GetSite());
	MPlug Plug;


	

	Plug= Shader->DecodePlug( Shader->TranscodeHandleToPlug( CStringA(parameter.End->LongPathName) ) );
	if(Plug.isNull())
		return E_FAIL;

	FillPlug(parameter, Plug);
	FillScript(parameter, Plug);


	

//e_Exit:

	return hr;

}

bool FxAttributeFiller::FillScript( DXCCEffectPath& parameter, MPlug& plug )

{
	MString plugName= Shader->EncodePlug(plug);

	MString MELPlugName= MFnDependencyNode(plug.node()).name() + MString(".") + plugName;

	switch(parameter.End->Description.Class)
	{
	case D3DXPC_SCALAR:
		{
			if( 0 == lstrcmpiA( SasUiControl.asChar(), "Slider"))
			{
				Script.AppendFormat("\tdxfxControlSliderCreate(\"float\", \"%s\", \"%f\", \"%f\", \"%d\", \"%s\");\n", SasUiLabel.asChar(), SasUiMin, SasUiMax, SasUiSteps, MELPlugName.asChar() );
			}
			else
			{
				Script.AppendFormat("\tdxfxControlScalarCreate(\"float\", \"%s\", \"%s\");\n", SasUiLabel.asChar(), MELPlugName.asChar());
			}
		}
		break;
	case D3DXPC_VECTOR:
		{
			if( 0 == lstrcmpiA( SasUiControl.asChar(), "ColorPicker") 
				&& parameter.End->Description.Columns >= 3)
			{//COLOR
				Script.AppendFormat("\tdxfxControlColorCreate(\"%s\", \"%s\", %d);\n", SasUiLabel.asChar(), MELPlugName.asChar(), parameter.End->Description.Columns);
			}
			else
			{//VECTOR
				Script.AppendFormat("\tdxfxControlVectorCreate(\"float\", \"%s\", \"%s\", %d);\n", SasUiLabel.asChar(), MELPlugName.asChar(), parameter.End->Description.Columns);
			}
		}
		break;				
	case D3DXPC_MATRIX_ROWS:
	case D3DXPC_MATRIX_COLUMNS:
		{
			Script.AppendFormat("\tdxfxControlMatrixCreate(\"float\", \"%s\", \"%s\", %d, %d);\n", SasUiLabel.asChar(), MELPlugName.asChar(), parameter.End->Description.Rows, parameter.End->Description.Columns);
		}
		break;	
	case D3DXPC_OBJECT:
		switch(parameter.End->Description.Type)
		{
		case D3DXPT_STRING:
			{
				Script.AppendFormat("\tdxfxControlStringCreate(\"%s\", \"%s\");\n", SasUiLabel.asChar(), MELPlugName.asChar());
			}
			break;
		case D3DXPT_TEXTURE:
		case D3DXPT_TEXTURE1D:
		case D3DXPT_TEXTURE2D:
		case D3DXPT_TEXTURE3D:
		case D3DXPT_TEXTURECUBE:
			{
				Script.AppendFormat("\tdxfxControlTextureCreate(\"%s\", \"%s\");\n", SasUiLabel.asChar(), MELPlugName.asChar());
			}
			break;
		default:
			break;
		};
		break;
	default:
		break;
	};

	return true;
}

bool FxAttributeFiller::FillPlug( DXCCEffectPath& parameter, MPlug& plug )
{
	HRESULT hr= S_OK;
	LPDXCCRESOURCE pResource= NULL;
	LPDIRECT3DBASETEXTURE9 pTexture= NULL;
	D3DXPARAMETER_DESC desc= parameter.End->Description;
	float NumericVals[16]= {0.0f};
	MString StringVal;
	LPCSTR StringValStr= "";

	bool RestoreFound= false;
	LPCVOID pRestoreData= NULL;
	size_t RestoreSize= 0;
	CComPtr<IDXCCPropertyCollection> pCollection;
	DXCCPROPERTY_KEY key= 0;

	if(Restore)
	{
		if( DXCC_SUCCEEDED( Restore->GetPath( parameter.End->LongPathName, &pCollection, &key) ) )
		{
			pCollection->GetPropertyValueAsData(key, &pRestoreData);
			DXCCPROPERTY_DESC desc= pCollection->GetPropertyDesc(key);

			if(desc == DXCCPD_STRING)
			{
				StringVal= (LPCSTR)pRestoreData;
				StringValStr= (LPCSTR)pRestoreData;

				RestoreFound= true;
			}
			else
			{
				size_t RestoreSize= DXCCPropertySize( desc, pRestoreData );
				memcpy( NumericVals, pRestoreData, RestoreSize);

				RestoreFound= true;
			}
		}
	}



	if((desc.Type == D3DXPT_BOOL)
	||(desc.Type == D3DXPT_INT)
	||(desc.Type == D3DXPT_FLOAT))
	{

		if(!RestoreFound)
		{

			if(plug.isArray())
			{
				hr=  Shader->Effect->GetFloatArray(parameter.End->Handle, NumericVals, 16);
					if(DXCC_FAILED(hr))
						DXCC_GOTO_EXIT(e_Exit, TRUE);
			}
			else
			{
				hr=  Shader->Effect->GetFloat(parameter.End->Handle, NumericVals);
				if(DXCC_FAILED(hr))
					DXCC_GOTO_EXIT(e_Exit, TRUE);
			}
		}

		if(plug.isArray())
		{
			int count= desc.Rows * desc.Columns;
			for(int i= 0; i < count; i++)
			{
				MPlug subPlug= plug.elementByLogicalIndex(i);
				DXCHECK_MSTATUS(subPlug.setValue( NumericVals[i] ));
			}
		}
		else
		{
			DXCHECK_MSTATUS(plug.setValue( NumericVals[0] ));
		}
	}
	else if((desc.Type == D3DXPT_TEXTURE)
	||(desc.Type == D3DXPT_TEXTURE1D)
	||(desc.Type == D3DXPT_TEXTURE2D)
	||(desc.Type == D3DXPT_TEXTURE3D)
	||(desc.Type == D3DXPT_TEXTURECUBE))
	{

		if(!RestoreFound)
		{
			hr=  Shader->Effect->GetTexture(parameter.End->Handle, &pTexture);
			if(DXCC_SUCCEEDED(hr) && pTexture != NULL)
			{
				hr= g_PreviewPipeline.AccessManager()->FindResourceByPointer(
						(LPUNKNOWN)pTexture, 
						NULL, 
						&pResource);
				if(DXCC_SUCCEEDED(hr))
				{
					StringVal= pResource->GetResourcePath();
				}
			}
			else
			{
				D3DXHANDLE hResourceAddy= Shader->Effect->GetAnnotationByName( parameter.End->Handle, "SasResourceAddress");
				if(hResourceAddy)
				{
					LPCSTR resourceAddressStr= NULL;
					Shader->Effect->GetString(hResourceAddy, &resourceAddressStr); 

					StringVal= resourceAddressStr;
				}
			}
		}

		DXCHECK_MSTATUS(plug.setValue( StringVal ));
	}
	else if(desc.Type == D3DXPT_STRING)
	{
		if(!RestoreFound)
		{
			hr=  Shader->Effect->GetString(parameter.End->Handle, &StringValStr);
			StringVal= StringValStr;
		}

		DXCHECK_MSTATUS(plug.setValue( StringVal ));
	}
	else
	{
		DXCC_STATUS_EXIT(hr, E_FAIL, e_Exit, TRUE);
	}

e_Exit:
	DXCC_RELEASE(pResource);
	DXCC_RELEASE(pTexture);

	return DXCC_SUCCEEDED(hr);
}



HRESULT FxAttributeFiller::PostEnumParameter(LPD3DXEFFECT pEffect, DXCCEffectPath& parameter)
{
	Script += "dxfxStackDown( -1 );\n";
	return S_OK;
}

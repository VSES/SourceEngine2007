#include "pch.h"

//this for FxInternal::DecodePlug only
struct PathDecodeElement
{
	PathDecodeElement()
	{
		IsArray= false;
		Index= 0;
	}

	CStringA Name;
	MFnAttribute Attribute;
	bool IsArray;
	int Index;//maya only supports 1d arrays
};

FxInternal::FxInternal() : DXMAnchor() 
{
	Effect= NULL;
	DirectXShader::ActiveFx.SetAt(this, this);
}

FxInternal::~FxInternal()
{
	DXCC_ASSERT(Effect == NULL);
}

BOOL 
FxParameterGetUiVisible(LPD3DXEFFECT pEffect, DXCCEffectPath& parameter)
{
	BOOL result= TRUE;

	D3DXHANDLE handle= DXCCFindEffectAnnotation(pEffect, parameter.Root->Handle, "SasUiVisible");
	if(handle != NULL)
	{
		pEffect->GetBool(handle, &result);
	}
	return result;
}

MString 
FxParameterGetUiControl(LPD3DXEFFECT pEffect, DXCCEffectPath& parameter)
{
	MString result= "Any";

	D3DXHANDLE handle= DXCCFindEffectAnnotation(pEffect, parameter.Root->Handle, "SasUiControl");
	if(handle != NULL)
	{
		LPCSTR str;
		if(DXCC_SUCCEEDED(pEffect->GetString(handle, &str)))
			result = str;
	}

	return result;
}

MString 
FxParameterGetUiLabel(LPD3DXEFFECT pEffect, DXCCEffectPath& parameter)
{	
	HRESULT hr= S_OK;
	CStringA UIName(parameter.End->Description.Name); 
	if(parameter.Length == 1)
	{
		LPCSTR pName;
		D3DXHANDLE hUIName= DXCCFindEffectAnnotation(pEffect, parameter.Root->Handle, "SasUiLabel");
		if(hUIName != NULL)
		{
			if(DXCC_SUCCEEDED(pEffect->GetString(hUIName, &pName)))
				UIName= pName;
		}
	}
	else
	{
		DXCCEffectElement* parent= &parameter.Root[parameter.Length-2];
		if(parent->Description.Elements != 0)
			UIName.Format( "[%d]", parameter.End->Index);
	}

//e_Exit:

	return MString(UIName.GetString());
}


void FxInternal::Destroy() 
{
	DXCC_RELEASE(Effect);
	DirectXShader::ActiveFx.RemoveKey(this);
}





//THIS FUNCTION IS QUITE COMPLICATED BECAUSE 
//IT HAS TO DEAL WITH MAYA NAME MANGLING!
//tokenize the path.
//add each token into the element list which tracks the attributes and indices
//for each entry added make sure there are no parents left out from maya name mangling.
//after the list is populated and missing elements area added 
//we reiterate and produce the final result.
MPlug		FxInternal::DecodePlug(const CStringA& plugStr)
{

	MPlug result;
	MFnDependencyNode depNode(GetSite());

	CAtlList<PathDecodeElement> elementList;

	//tokenize the path
	int iLastPos=0;
	int iThisPos=0;
	CStringA subStr;
	while( iLastPos= iThisPos, 
		subStr=plugStr.Tokenize(".[]", iThisPos), 
		iThisPos != -1 )
	{
		//char lastChar= subStr[iLastPos];
		//char thisChar= subStr[iThisPos];

		//are we looking at a named portion?
		if(iLastPos == 0 
			|| plugStr[iLastPos-1]=='.'
			|| (plugStr[iLastPos-1]==']' && plugStr[iLastPos]=='.'))
		{
			//if the name is length zero then it must be the case when 
			// you do last[#].  The situation is caused by  the sequence "]."
			//if we dont have a name we can skip since only 1d arrays are allowed.

			if(subStr.GetLength() > 0)
			{
		
				//everything we add is going to be based on the current tail.
				//because we are adding parents, as we find parents we can continue
				//to add them to this position so that they will order themselves properly
				POSITION insertPos= elementList.GetTailPosition();

				//calculate the cancel condition.
				//it is the current tail's object if a tail exists otherwise NULL
				//NULL indicates go all the way to the root
				MObject cancelObj= MObject::kNullObj;
				if(elementList.GetCount() > 0)
				{
					cancelObj= elementList.GetTail().Attribute.object();
				}


				//get the object we are currently working with
				MObject thisObj= depNode.attribute(subStr.GetString());
				if(thisObj.isNull())
					return MPlug();//Critical element of the path was not found...return NULL


				//add it to the list so that we have something to insertBefore
				elementList.AddTail();
				elementList.GetTail().Name= subStr;
				elementList.GetTail().Attribute.setObject(thisObj);



				//walk through all of the parents until we reach cancel condition.
				//we can add  the current element onto the list in the same way.
				for( MFnAttribute itrAttr( elementList.GetTail().Attribute.parent() );
					!itrAttr.object().isNull()  && (cancelObj != itrAttr.object());
					itrAttr.setObject(itrAttr.parent()))
				{
					PathDecodeElement element;
					element.Attribute.setObject( itrAttr.object() );

					//we change the position so that the grandparent is inserted before the parent
					insertPos= elementList.InsertBefore( insertPos, element);
				}
			}
		}
		//are we looking at a numbered portion?
		else if(plugStr[iLastPos-1]=='[' && plugStr[iThisPos-1]==']')
		{
			//if so change the array index.
			elementList.GetTail().IsArray= true;
			elementList.GetTail().Index = atoi( subStr.GetString() );
		}
		else
			DXCC_ASSERT(false);//VERY POORLY FORMED STRING

	}

	//produce the result plug off of the elementList.
	bool first= true;
	for(POSITION pos= elementList.GetHeadPosition();
		pos != NULL;
		elementList.GetNext(pos))
	{
		PathDecodeElement& element= elementList.GetAt(pos);

		if(first)
		{//on first one we initialize the result 
			first= false;
			result= MPlug( GetSite(), element.Attribute.object() );
		}
		else
		{
			result= result.child( element.Attribute.object() );
		}

		if(element.IsArray)
		{
			result= result.elementByLogicalIndex(element.Index);
		}


		if(result.isNull())
			return MPlug();//Critical element of the path was not found...return NULL
	}


	return result;
}

CStringA	FxInternal::EncodePlug(const MPlug& plug)
{
	MString name= plug.partialName(false, true, false, false, true, true);
	return CStringA(name.asChar());
}

D3DXHANDLE	FxInternal::DecodeHandle(const CStringA& handleStr)
{
	return Effect->GetParameterByName(NULL, handleStr.GetString());
}

MPlug 
FxInternal::GetValuePlug(MPlug& plug)
{
	MFnDependencyNode depNode(GetSite());

	MObject oAttribute = plug.attribute();

	MFnAttribute fnAttribute(oAttribute);

	MString name = fnAttribute.name() + MString("Value");

	oAttribute= depNode.attribute(name);

	return plug.child(oAttribute);
}


CStringA	
FxInternal::TranscodePlugToHandle(const CStringA& plugStr)
{
	int iLastPos=0;
	int iThisPos=0;

	CStringA result;
	CStringA partial;


	int rootIdx= plugStr.Find(DirectXShader::RootLongName.asChar());
	if(rootIdx != 0)
		return CStringA();

	if( plugStr[(int)DirectXShader::RootLongName.length()] != '.' )
		return CStringA();

	
	CStringA rootlessPlugStr= plugStr.Mid(DirectXShader::RootLongName.length()+1);

	CStringA subStr;
	while( iLastPos= iThisPos, 
		subStr=rootlessPlugStr.Tokenize(".[]", iThisPos), 
		iThisPos != -1 )
	{
		if(iLastPos == 0)
		{

			partial= subStr;

			int prefixIdx= subStr.Find(DirectXShader::RootShortName.asChar());
			if(prefixIdx != 0)
				return CStringA();

			result= subStr.Mid(DirectXShader::RootShortName.length());
		}
		else if(rootlessPlugStr[iLastPos-1]=='.'
			|| (rootlessPlugStr[iLastPos-1]==']' && rootlessPlugStr[iLastPos]=='.'))
		{
			DXCC_ASSERT(subStr.Find(partial) == 0);

			CStringA uniquePart= subStr.Mid( partial.GetLength() );

			partial= subStr;

			result.AppendFormat(".%s", uniquePart);
		}
		else if(rootlessPlugStr[iLastPos-1]=='[' && rootlessPlugStr[iThisPos-1]==']')
		{
			result.AppendFormat("[%s]", subStr);
		}
		else
			DXCC_ASSERT(false);
	}


	//remove last array 
	if( result[result.GetLength()-1] == ']' )
	{
		int lastArrayAccess= result.ReverseFind('[');
		DXCC_ASSERT(lastArrayAccess != -1);

		result= result.Left(lastArrayAccess);
	}

	int lastMember= result.ReverseFind('.');
	if(lastMember == -1)
		return CStringA();

	if(result.Mid(lastMember+1) != "Value")
		return CStringA();

	result= result.Left(lastMember);
	return result;
}

void FxInternal::LoadLegacy()
{
	MStatus stat= MStatus::kSuccess;

	MString filename;
	int cParamCount=0;

	MFnDependencyNode depNode(GetSite());

	{//PARAMTER COUNT
		MPlug plugParamCount= depNode.findPlug("DXCC_FxParam_Count");
		if(plugParamCount.isNull())
			return; 

		DXCHECK_MSTATUS(plugParamCount.getValue(cParamCount));

		depNode.removeAttribute( plugParamCount.attribute() );

	}//END//PARAMTER COUNT

	for(int iParam= 0; iParam < cParamCount; iParam++)
	{
		MString szName;
		MObject oData;

		MPlug plugName= depNode.findPlug(MString("DXCC_FxParam_Name")+iParam, &stat);
		if(plugName.isNull())
			continue; 

		DXCHECK_MSTATUS( plugName.getValue(szName) );

		depNode.removeAttribute( plugName.attribute() );

		CStringA newParamPlugName= TranscodeHandleToPlug( szName.asChar() );
		
		MPlug newParamPlug= DecodePlug( newParamPlugName );
		if(newParamPlug.isNull())
			continue; 

		MPlug plugData= depNode.findPlug(MString("DXCC_FxParam_Data")+iParam, &stat);
		if(plugData.isNull())
			continue; 
		
		DXCHECK_MSTATUS( plugData.getValue(oData) );

		if(oData.hasFn(	MFn::kStringData ))
		{
			MString dataStr;
			DXCHECK_MSTATUS( plugData.getValue(dataStr) );

			newParamPlug.setValue( dataStr );
		}
		else if(oData.hasFn( MFn::kDoubleArrayData ))
		{
			MFnDoubleArrayData dArrayData(oData);
			MDoubleArray dArray= dArrayData.array();
				
			for(UINT iSub= 0; iSub < dArray.length(); iSub++)
			{
				MPlug subPlug= newParamPlug.elementByLogicalIndex( iSub );
				if(subPlug.isNull())
					continue; 

				subPlug.setValue( dArray[iSub] );
			}
		}

		depNode.removeAttribute( plugData.attribute() );
	}
}

CStringA	FxInternal::TranscodeHandleToPlug (const CStringA& handleStr)
{
	int iLastPos=0;
	int iThisPos=0;

	CStringA result(DirectXShader::RootLongName.asChar());
	CStringA partial(DirectXShader::RootShortName.asChar());

	CStringA subStr;
	while( iLastPos= iThisPos, 
		subStr=handleStr.Tokenize(".[]", iThisPos), 
		iThisPos != -1 )
	{
		if(iLastPos == 0 
			|| handleStr[iLastPos-1]=='.'
			|| (handleStr[iLastPos-1]==']' && handleStr[iLastPos]=='.'))
		{
			partial.Append(subStr.GetString());
			result.AppendFormat(".%s", partial.GetString());
		}
		else if(handleStr[iLastPos-1]=='[' && handleStr[iThisPos-1]==']')
		{
			result.AppendFormat("[%s]", subStr);
		}
		else
			DXCC_ASSERT(false);
	}


	D3DXPARAMETER_DESC		desc;
	D3DXHANDLE handle= DecodeHandle(handleStr);
	Effect->GetParameterDesc(handle, &desc);
	if( desc.StructMembers == 0 && desc.Elements == 0 )
	{
		partial.Append("Value");
		result.AppendFormat(".%s", partial);
	}

	return result;
}

bool FxInternal::EffectLoad(LPCSTR filename, LPDXCCSHADERPROPERTIES pRestoreProps, LPCSTR RestoreMsg)
{
	HRESULT hr= S_OK;
	MStatus status= MStatus::kSuccess; 
	LPDIRECT3DDEVICE9 pDevice= NULL;
	LPD3DXBUFFER pErrors= NULL;
	LPD3DXEFFECT pTmpEffect= NULL;


	hr= g_PreviewPipeline.AccessEngine()->GetD3DDevice( &pDevice );
	if(DXCC_FAILED(hr))
		DXCC_GOTO_EXIT(e_Exit, TRUE);

	try
	{
		D3DXMACRO sas_present[2]= { {"SAS_PRESENT", "1"}, {NULL, NULL} };
		if(DXCC_SUCCEEDED(D3DXCreateEffectFromFileA(
				pDevice,
				filename,
				sas_present,
				&g_SasIncluder,
				NULL,
				NULL,
				&pTmpEffect,
				&pErrors)))
		{
			EffectUnload();
		
			pTmpEffect->AddRef();
			Effect= pTmpEffect;
			DXCC_RELEASE(pTmpEffect);

			FxAttributeCreator AttributesCreator;

			D3DXHANDLE hGlobal= Effect->GetParameterBySemantic(NULL, "SasGlobal");
			if(hGlobal == NULL)
			{
				MGlobal::displayWarning( "DirectXShader: File is not SAS compliant (missing SasGlobal) and may not render properly FILE: \"" + MString(filename) + "\"" );
			}
			else
			{
				D3DXHANDLE hVersion= Effect->GetAnnotationByName(hGlobal, "SasVersion");
				if(hVersion == NULL)
				{
					MGlobal::displayWarning( "DirectXShader: File is not SAS compliant (missing SAS version annotation) and may not render properly FILE:\"" + MString(filename) + "\"" );
				}
				else
				{
					INT SasVersion[3] = {0,0,0};
					if(DXCC_SUCCEEDED(Effect->GetIntArray( hVersion, SasVersion, 3)))
					{
						if(SasVersion[0] != 1 
							|| SasVersion[1] != 1 
							|| SasVersion[2] != 0 )
						{
							MGlobal::displayWarning( "DirectXShader: File does not match supported SAS version(1.1.0) and may not render properly FILE:\"" + MString(filename) + "\"" );
						}
					}
				}
			}

			if(DXCC_SUCCEEDED(  AttributesCreator.Run(this) ) )
			{
				FxAttributeFiller AttributesFiller;	

				hr= AttributesFiller.Run(this, pRestoreProps, RestoreMsg);
				if(DXCC_FAILED(hr))
					DXCC_GOTO_EXIT(e_Exit, TRUE);

				LoadLegacy();
			}
			else
			{
				MGlobal::displayWarning( "DirectXShader Load Error: Unable to convert parameters to attributes on Shader Node." );

				DXCC_STATUS_EXIT(hr, E_FAIL, e_Exit, FALSE);

			}
		}
		else
		{
			MGlobal::displayWarning( MString( "DirectXShader Load Error: Compiler Output - ") + MString( (LPCSTR) pErrors->GetBufferPointer() )  );

			DXCC_STATUS_EXIT(hr, E_FAIL, e_Exit, FALSE);
		}
	}
	catch (...)
	{
		MGlobal::displayWarning( MString( "DirectXShader Load Error: Unexpected runtime exception thrown by D3DXCreateEffectFromFileA" )  );

		DXCC_STATUS_EXIT(hr, E_FAIL, e_Exit, TRUE);
	}

e_Exit:

	DXCC_RELEASE(pTmpEffect);
	DXCC_RELEASE(pDevice);
	DXCC_RELEASE(pErrors);

	return DXCC_SUCCEEDED(hr);
}

void FxInternal::EffectUnload()
{
	HRESULT hr= S_OK;
	MStatus status= MStatus::kSuccess; 

	MFnDependencyNode depNode(GetSite());

	DXCC_RELEASE(Effect);

	MPlug scriptPlug(GetSite(), DirectXShader::aScript);
	if(!scriptPlug.isNull())
		DXCHECK_MSTATUS(scriptPlug.setValue(""));

	MObject root = depNode.attribute(DirectXShader::RootShortName);
	if(!root.isNull())
		depNode.removeAttribute(root);


}


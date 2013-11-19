
#include "pch.h"

HRESULT FxAttributeCreator::Run(FxInternal* myShader)
{
	MStatus stat= MS::kSuccess;

	Shader  = myShader;


	MFnCompoundAttribute fnCompoundAttribute;
	RootAttribute= fnCompoundAttribute.create(DirectXShader::RootLongName, DirectXShader::RootShortName, &stat);
	DXCHECK_MSTATUS(stat);
	AttributeMakePublic(fnCompoundAttribute);

	arrayCount= 0;

	DXCCEnumEffectParameters(Shader->Effect, this);

	MFnDependencyNode depNode(Shader->GetSite());
	stat= depNode.addAttribute(RootAttribute);
	if(!DXMSUCCESS(stat))
		return E_FAIL;


	CAtlList<MObject> printList;
	printList.AddTail(RootAttribute);
	while(printList.GetCount() > 0)
	{
		MObject printObj= printList.GetHead();
		if( printObj.hasFn(MFn::kCompoundAttribute) )
		{
			MFnCompoundAttribute printCompound( printObj );
			for(UINT i= 0; i < (int)printCompound.numChildren(); i++)
			{
				printList.InsertAfter( printList.GetHeadPosition(), printCompound.child( i ) );
			}
		}

		if( g_DebugBasic )
		{
			MFnAttribute printAttrib( printObj );
			MString printName= printAttrib.name();
			DXCC_DPFA_REPORT("Added Attribute: %s", printName.asChar() );
		}

		printList.RemoveHead();
	}



	return S_OK;
}

HRESULT FxAttributeCreator::PreEnumParameter(LPD3DXEFFECT pEffect, DXCCEffectPath& parameter, BOOL& GoDeeper, BOOL& DoEnum, BOOL& DoPost)
{

	DXCC_ASSERT((oAttributeStack.GetCount()+arrayCount) == (parameter.Length - 1));

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
		BOOL SasUiVisible = FxParameterGetUiVisible(pEffect, parameter);
		MString SasUiControl = FxParameterGetUiControl(pEffect, parameter);

		if(0 == lstrcmpiA( SasUiControl.asChar(), "None"))
			SasUiVisible= FALSE;

		if(SasUiVisible == FALSE)
		{
			GoDeeper= FALSE;
			DoEnum= FALSE;
			DoPost= FALSE;
			return S_OK;
		}
	}

	if(parameter.EndIsArrayElement())
	{

		if(parameter.End->Index > 0)
		{
			GoDeeper= FALSE;
			DoEnum= FALSE;
			DoPost= FALSE;
		}
		else
			arrayCount++;

	}

	return S_OK;
}

HRESULT FxAttributeCreator::EnumParameter(LPD3DXEFFECT pEffect, DXCCEffectPath& parameter, BOOL& GoDeeper, BOOL& DoPost)
{
	HRESULT hr= S_OK;
	MStatus stat= MS::kSuccess;

	MFnDependencyNode depNode(Shader->GetSite());

	DXCC_ASSERT( !depNode.object().isNull() );

	CStringA CompoundName( DirectXShader::RootShortName.asChar() );
	MObject CompoundAttribute= MObject::kNullObj;
	MFnCompoundAttribute fnCompoundAttribute;
	//CStringA shortName;
	UINT index;

	//make the name
	for( index= 0; index < parameter.Length; index++ )
	{
		if(parameter.IsArrayElement(index) == FALSE)
			CompoundName.Append(parameter.Root[index].ShortPathName);
	}


	//we dont add any extra compounds when we are an array element as opposed to an array
	if(!parameter.EndIsArrayElement())
	{
		//if the attribute we want to make already exists then exit
		MObject oAttr= depNode.attribute(CompoundName.GetString());
		if(!oAttr.isNull())
			depNode.removeAttribute(oAttr);

		CompoundAttribute= fnCompoundAttribute.create(CompoundName.GetString(), MString(), &stat);
		DXCHECK_MSTATUS(stat);
		AttributeMakePublic(fnCompoundAttribute);
		DXCHECK_MSTATUS(fnCompoundAttribute.setArray(parameter.End->Description.Elements > 1));

		//add the attribute to the stack
		oAttributeStack.AddTail( CompoundAttribute );
		//depNode.addAttribute(CompoundAttribute);

	}

	//if we have no children then we are also a leaf
	//as a leaf we need to terminate it with a Value attribute
	if(parameter.End->NumChildren() == 0)
	{
		CStringA ValueName;
		MObject ValueAttribute= MObject::kNullObj;

		ValueName.Format("%sValue", CompoundName.GetString());

		//if the attribute we want to make already exists then exit
		MObject oAttr= depNode.attribute(ValueName.GetString());
		if(!oAttr.isNull())
			depNode.removeAttribute(oAttr);

		if( parameter.End->Description.Class == D3DXPC_SCALAR )
		{
			MFnNumericAttribute numeric;
			ValueAttribute= numeric.create(ValueName.GetString(), MString(), MFnNumericData::kDouble, 0.0, &stat);
			DXCHECK_MSTATUS(stat);
			AttributeMakePublic(numeric);
			DXCHECK_MSTATUS( numeric.setArray(false) );

		}
		else if( parameter.End->Description.Class == D3DXPC_VECTOR)
		{
			MFnNumericAttribute numeric;
			ValueAttribute= numeric.create(ValueName.GetString(), MString(), MFnNumericData::kDouble, 0.0, &stat);
			DXCHECK_MSTATUS(stat);
			AttributeMakePublic(numeric);
			DXCHECK_MSTATUS( numeric.setArray(true) );
		}
		else if( parameter.End->Description.Class == D3DXPC_MATRIX_ROWS
			|| parameter.End->Description.Class == D3DXPC_MATRIX_COLUMNS)
		{
			MFnNumericAttribute numeric;
			ValueAttribute= numeric.create(ValueName.GetString(), MString(), MFnNumericData::kDouble, 0.0, &stat);
			DXCHECK_MSTATUS(stat);
			AttributeMakePublic(numeric);
			DXCHECK_MSTATUS( numeric.setArray(true) );
		}
		else if( parameter.End->Description.Class == D3DXPC_OBJECT
			&& (parameter.End->Description.Type == D3DXPT_STRING
			|| parameter.End->Description.Type == D3DXPT_TEXTURE
			|| parameter.End->Description.Type == D3DXPT_TEXTURE1D
			|| parameter.End->Description.Type == D3DXPT_TEXTURE2D
			|| parameter.End->Description.Type == D3DXPT_TEXTURE3D
			|| parameter.End->Description.Type == D3DXPT_TEXTURECUBE))
		{
			MFnTypedAttribute typed;
			ValueAttribute= typed.create(ValueName.GetString(), MString(), MFnData::kString ,MObject::kNullObj, &stat);
			DXCHECK_MSTATUS(stat);
			AttributeMakePublic(typed);
			DXCHECK_MSTATUS( typed.setArray(false) );
		}
		else
		{
			DXCC_DPFA_ERROR("Unrecognized parameter type.");

			DXCCBreakPoint();
			return E_FAIL;
		}

		oAttributeStack.AddTail( ValueAttribute );
		//depNode.addAttribute(ValueAttribute);
	}

	return hr;
}

HRESULT FxAttributeCreator::PostEnumParameter(LPD3DXEFFECT pEffect, DXCCEffectPath& parameter)
{
	MFnDependencyNode depNode(Shader->GetSite());

	if(parameter.End->NumChildren() == 0)
	{
		DXCC_ASSERT((oAttributeStack.GetCount()+arrayCount) == (parameter.Length + 1));//+1 for the value attribute

		MObject oValue= oAttributeStack.RemoveTail();
		if(!oValue.isNull())
		{
			MObject oThis= oAttributeStack.GetTail();
			
			MFnCompoundAttribute ThisAttribute(oThis);

			DXCHECK_MSTATUS( ThisAttribute.addChild(oValue));

			depNode.addAttribute(oValue);
	
		}
	}
	
	if(!parameter.EndIsArrayElement())
	{
		MObject oThis= oAttributeStack.RemoveTail();
		//if we are at the bottom of the stack then parent to the root 
		MObject oParent= (parameter.Length == 1) ? RootAttribute : oAttributeStack.GetTail();
		
		MFnCompoundAttribute ParentAttribute(oParent);

		DXCHECK_MSTATUS( ParentAttribute.addChild(oThis) );

		depNode.addAttribute(oThis);

	}
	else
		arrayCount--;

	return S_OK;
}

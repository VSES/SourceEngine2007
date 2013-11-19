#include "pch.h"


VOID CALLBACK 
MayaDebugPrintfACallback(LPCSTR szFormat)
{
	int iLogger= 0;
	MGlobal::executeCommand("exists DXMLogInfo", iLogger);
	if(iLogger != 0)
	{
		CStringA msg(szFormat);
		msg.Remove('\n');
		msg.Remove('\r');
		msg.Remove('\f');

		CStringA cmd;
		cmd.Format("DXMLogInfo( \"%s\" )", msg.GetString() );

		MGlobal::executeCommand(cmd.GetString());
	}

	MGlobal::displayInfo(szFormat);
	OutputDebugStringA(szFormat);
}

VOID CALLBACK 
MayaDebugPrintfWCallback(LPCWSTR szFormat)
{
	CStringA printStr;
	printStr.Format("%S", szFormat);

	int iLogger= 0;
	MGlobal::executeCommand("exists DXMLogInfo", iLogger);
	if(iLogger != 0)
	{
		CStringA msg(printStr.GetString());
		msg.Remove('\n');
		msg.Remove('\r');
		msg.Remove('\f');

		CStringA cmd;
		cmd.Format("DXMLogInfo( \"%s\" )", msg.GetString() );

		MGlobal::executeCommand(cmd.GetString());
	}

	MGlobal::displayInfo(printStr.GetString());
	OutputDebugStringW(szFormat);
}



void AttributeMakePrivate( MFnAttribute& attribute)
{
	DXCHECK_MSTATUS( attribute.setReadable ( false ) );
	DXCHECK_MSTATUS( attribute.setWritable ( false ) );
	DXCHECK_MSTATUS( attribute.setConnectable ( false ) );
	DXCHECK_MSTATUS( attribute.setStorable( false  ) );
	DXCHECK_MSTATUS( attribute.setHidden( true ) );
	DXCHECK_MSTATUS( attribute.setCached ( true ) );
	DXCHECK_MSTATUS( attribute.setArray ( false ) );
	DXCHECK_MSTATUS( attribute.setKeyable( false ) );
	DXCHECK_MSTATUS( attribute.setInternal( false ) );

#if (( MAYA_MAJOR_VERSION == 6 && MAYA_MINOR_VERSION >= 5) || MAYA_MAJOR_VERSION > 6 )
	DXCHECK_MSTATUS( attribute.setChannelBox ( false ) );
#endif

}

void AttributeMakePublic( MFnAttribute& attribute)
{
	DXCHECK_MSTATUS( attribute.setReadable ( true ) );
	DXCHECK_MSTATUS( attribute.setWritable ( true ) );
	DXCHECK_MSTATUS( attribute.setConnectable ( true ) );
	DXCHECK_MSTATUS( attribute.setStorable( true  ) );
	DXCHECK_MSTATUS( attribute.setHidden( false ) );
	DXCHECK_MSTATUS( attribute.setCached ( true ) );
	DXCHECK_MSTATUS( attribute.setArray ( false ) );
	DXCHECK_MSTATUS( attribute.setKeyable( true ) );
	DXCHECK_MSTATUS( attribute.setInternal( false ) );

#if (( MAYA_MAJOR_VERSION == 6 && MAYA_MINOR_VERSION >= 5) || MAYA_MAJOR_VERSION > 6 )
	DXCHECK_MSTATUS( attribute.setChannelBox ( true ) );
#endif

}



bool VersionMatch(LPCSTR Type1, INT Ver1, LPCSTR Type2, INT Ver2, char* strError)
{
	if(Ver1 != Ver2
	|| 0 != lstrcmpiA(Type1, Type2))
	{
		//MessageBoxA(M3dView::applicationShell(), strError, "Type/Version Mismatch",  MB_ICONERROR);
		return false;
	}
	else
		return true;

}

MString MakeNameExportable(MString &in)
{
	const char *inConv= in.asChar();
	char outConv[MAX_PATH];
	for(UINT i= 0; i < in.length() && i < MAX_PATH; i++)
	{
		if(isalnum( inConv[i] )//alphanumeric//A – Z, a – z, or 0 – 9
		|| (inConv[i] == '_'))
			outConv[i]= inConv[i];
		else
			outConv[i]= '_';
	}

	if(in.length() < MAX_PATH)
		outConv[in.length()]= '\0';
	else
		outConv[MAX_PATH-1]= '\0';
	
	return MString(outConv);
}



//--------------------------------------------------------------------------------------
// Name: TransformMatrix
// Desc: Converts a Maya right-handed Y-up matrix to a Direct3D left-handed Y-up matrix.
//       This took a lot of thought to get right.
//--------------------------------------------------------------------------------------
void ATGTransformMatrix( D3DXMATRIX* pDestMatrix, const D3DXMATRIX* pSrcMatrix ) 
{
    D3DXMATRIX SrcMatrix;
    if( pSrcMatrix == pDestMatrix )
    {
        memcpy( &SrcMatrix, pSrcMatrix, sizeof( D3DXMATRIX ) );
        pSrcMatrix = &SrcMatrix;
    }
    memcpy( pDestMatrix, pSrcMatrix, sizeof( D3DXMATRIX ) );

    // What we're doing here is premultiplying by a left hand -> right hand matrix,
    // and then postmultiplying by a right hand -> left hand matrix.
    // The end result of those multiplications is that the third row and the third
    // column are negated (so element _33 is left alone).  So instead of actually
    // carrying out the multiplication, we just negate the 6 matrix elements.

    pDestMatrix->_13 = -pSrcMatrix->_13;
    pDestMatrix->_23 = -pSrcMatrix->_23;
    pDestMatrix->_43 = -pSrcMatrix->_43;

    pDestMatrix->_31 = -pSrcMatrix->_31;
    pDestMatrix->_32 = -pSrcMatrix->_32;
    pDestMatrix->_34 = -pSrcMatrix->_34;
}

/*
//--------------------------------------------------------------------------------------
// Name: TransformPosition
// Desc: Converts a Maya right-handed Y-up position to a Direct3D left-handed Y-up
//       position.
//--------------------------------------------------------------------------------------
void ATGTransformPosition( D3DXVECTOR3* pDestPosition, const D3DXVECTOR3* pSrcPosition )
{
    D3DXVECTOR3 SrcVector;
    if( pSrcPosition == pDestPosition )
    {
        SrcVector = *pSrcPosition;
        pSrcPosition = &SrcVector;
    }
    pDestPosition->x = pSrcPosition->x * m_fUnitScale;
    pDestPosition->y = pSrcPosition->y * m_fUnitScale;
    pDestPosition->z = -pSrcPosition->z * m_fUnitScale;
}
*/

//--------------------------------------------------------------------------------------
// Name: TransformDirection
// Desc: Converts a Maya right-handed Y-up direction to a Direct3D left-handed Y-up
//       direction.
//--------------------------------------------------------------------------------------
void ATGTransformDirection( D3DXVECTOR3* pDestDirection, const D3DXVECTOR3* pSrcDirection )
{
    D3DXVECTOR3 SrcVector;
    if( pSrcDirection == pDestDirection )
    {
        SrcVector = *pSrcDirection;
        pSrcDirection = &SrcVector;
    }
    pDestDirection->x = pSrcDirection->x;
    pDestDirection->y = pSrcDirection->y;
    pDestDirection->z = -pSrcDirection->z;
}

bool 
DAGIsVisible(
	MDagPath& dagPath, 
	MStatus* status)
{
	MDagPath path= dagPath;
	MFnDagNode dagNode(path);

	if(path.node().hasFn(MFn::kManipulator3D))
		return FALSE;

	do
	{
		if( !NodeIsVisible( path.node(), status))
			return false;
	} while( path.pop());

	return true;
}

bool 
NodeIsVisible(
	MObject& node,
	MStatus* status)
{
	//HRESULT	hr= S_OK;
	MStatus TempStatus;

	if(!status)
		status= &TempStatus;
	
		
	MFnDagNode DagNode(node); 

	bool	bIsVisible= false;

	bool	bPlugVisibility= true, 
			bPlugLodVisibility= true, 
			bPlugOverrideEnabled= false, 
			bPlugOverrideVisibility= true,
			bPlugIntermediate= false,
			bPlugUnderworld= false;

	MPlug	mpVisibility,
			mpLodVisibility,
			mpOverrideEnabled, 
			mpOverrideVisibility;
			//mpIntermediate;

	//VISIBILITY
	mpVisibility = DagNode.findPlug("visibility", status); // intermediateObject //template (these too?)
	if(!*status)
		DXCC_STATUS_EXIT(*status, MS::kFailure, e_Exit, FALSE);

	*status = mpVisibility.getValue(bPlugVisibility);
	if(!*status)
		DXCC_STATUS_EXIT(*status, MS::kFailure, e_Exit, FALSE);

	//LOD VISIBILITY
	mpLodVisibility = DagNode.findPlug("lodVisibility", status);
	if(!*status)
		DXCC_STATUS_EXIT(*status, MS::kFailure, e_Exit, FALSE);

	*status = mpLodVisibility.getValue(bPlugLodVisibility);
	if(!*status)
		DXCC_STATUS_EXIT(*status, MS::kFailure, e_Exit, FALSE);

	//Intermediate Object
	//mpIntermediate = DagNode.findPlug("intermediateObject", status);
	//if(!*status)
	//	DXCC_STATUS_EXIT(*status, MS::kFailure, e_Exit, FALSE);

	//*status = mpIntermediate.getValue(bPlugIntermediate);
	//if(!*status)
	//	DXCC_STATUS_EXIT(*status, MS::kFailure, e_Exit, FALSE);

	bPlugIntermediate= DagNode.isIntermediateObject();
	bPlugUnderworld= DagNode.inUnderWorld();

	//OVERRIDE ENABLED
	mpOverrideEnabled = DagNode.findPlug("overrideEnabled", status);
	if(!*status)
		DXCC_STATUS_EXIT(*status, MS::kFailure, e_Exit, FALSE);

	*status = mpOverrideEnabled.getValue(bPlugOverrideEnabled);
	if(!*status)
		DXCC_STATUS_EXIT(*status, MS::kFailure, e_Exit, FALSE);

	//OVERRIDE VISIBILITY
	mpOverrideVisibility = DagNode.findPlug("overrideVisibility", status);
	if(!*status)
		DXCC_STATUS_EXIT(*status, MS::kFailure, e_Exit, FALSE);

	*status = mpOverrideVisibility.getValue(bPlugOverrideVisibility);
	if(!*status)
		DXCC_STATUS_EXIT(*status, MS::kFailure, e_Exit, FALSE);

	bIsVisible= bPlugVisibility
		&& bPlugLodVisibility 
		&& !bPlugIntermediate
		&& !bPlugUnderworld 
		&& !(bPlugOverrideEnabled && !bPlugOverrideVisibility);

e_Exit:

	return bIsVisible;	
};

UINT MayaGetFPS()
{
	UINT nFPS;	// num frames per second

	// calculate the frames per second
	switch(MTime::uiUnit()) 
	{
		case MTime::kSeconds:		// 1 fps
			nFPS = 1;
			break;
		case MTime::kMilliseconds:	// 1000 fps
			nFPS = 1000;
			break;
		case MTime::kGames:			// 15 fps
			nFPS = 15;
			break;
		case MTime::kFilm:			// 24 fps
			nFPS = 24;
			break;
		case MTime::kPALFrame:		// 25 fps
			nFPS = 25;
			break;
		case MTime::kNTSCFrame:		// 30 fps
			nFPS = 30;
			break;
		case MTime::kShowScan:		// 48 fps
			nFPS = 48;
			break;
		case MTime::kPALField:		// 50 fps
			nFPS = 50;
			break;
		case MTime::kNTSCField:		// 60 fps
			nFPS = 60;
			break;
		default:
			nFPS = 1;
			break;
	};

	return nFPS;
}

void ConvertLocalMatrix(D3DXMATRIX& ToD3D, const MFloatMatrix& FromMaya)
{
	D3DXMATRIX Tmp;
	for (UINT i = 0; i < 4; i++)
	{
		for (UINT j = 0; j < 4; j++)
		{
			Tmp(i, j)= (FLOAT)FromMaya(i, j);
		}
	}

	ATGTransformMatrix(&ToD3D,&Tmp);
}

void ConvertLocalMatrix(D3DXMATRIX& ToD3D, const MMatrix& FromMaya)
{
	MFloatMatrix FromMayaFloat(FromMaya.matrix);
	ConvertLocalMatrix(ToD3D, FromMayaFloat);
}

void ConvertWorldMatrix(D3DXMATRIX& ToD3D, const MFloatMatrix& FromMaya)
{
/*
	D3DXMATRIX Local;
	ConvertLocalMatrix(Local, FromMaya);

	D3DXMATRIX RtoL;
	D3DXMatrixScaling(&RtoL, 1.0f, 1.0f, -1.0f);
	D3DXMatrixMultiply(&ToD3D, &Local, &RtoL);
*/
	ConvertLocalMatrix(ToD3D, FromMaya);

}

void ConvertWorldMatrix(D3DXMATRIX& ToD3D, const MMatrix& FromMaya)
{
	MFloatMatrix FromMayaFloat(FromMaya.matrix);
	ConvertWorldMatrix(ToD3D, FromMayaFloat);
}




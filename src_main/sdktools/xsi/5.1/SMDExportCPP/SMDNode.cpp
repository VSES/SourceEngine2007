// SMDNode.cpp: implementation of the SMDNode class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SMDNode.h"
#include "xsi_kinematicstate.h"

#include "xsi_application.h"
#include <string>
using namespace std;

extern bool ExportSelectedOnly;
extern bool ZeroRootTransforms;
extern void	XSILogMessage ( char *in_szMessage, XSI::siSeverityType in_eSeverity );
extern CSIBCArray<CachedObjectXfo>	g_ResetTranforms;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

SMDNode::SMDNode() :
m_iNodeID ( -1 ),
m_iNodeParentID ( -1 )
{

}

SMDNode::~SMDNode()
{

}

SI_Error SMDNode::Write( FILE *l_fptr )
{
	if ( ExportSelectedOnly )
	{
		char* nodename = GetName();
		string strNodename = nodename;

		if (!strncmp(nodename, "ValveBiped", 10))
		{
			int dotPosition = strNodename.find(".", 0);
			string newname = "ValveBiped";
			strNodename.replace(0, dotPosition, newname);
		}

		fprintf ( l_fptr, "%d %c%s%c %d\n",GetID(), 34, strNodename.c_str(), 34, GetParentID() );
	}
	else
	{
		fprintf ( l_fptr, "%d %c%s%c %d\n",GetID(), 34, GetName(), 34, GetParentID() );
	}

	return SI_SUCCESS;
}

void	SMDNode::SetHierarchicalCompensation( XSI::Kinematics in_pTransform )
{
	m_mHierarchyCompensation = in_pTransform;

}

void	SMDNode::SetHierarchicalCompensation2( XSI::Kinematics in_pTransform )
{
	m_mHierarchyCompensation2 = in_pTransform;

}

SMDNodeList::SMDNodeList()
{}

SMDNodeList::~SMDNodeList()
{
	for (int s=0;s<m_pNodeList.GetUsed();s++)
	{
		SMDNode* l_pNode = m_pNodeList[s];

		delete l_pNode;
	}

	m_pNodeList.DisposeData();

}

void SMDNodeList::AddNode ( XSI::CString in_szName, XSI::CString  in_szParentName,  XSI::Kinematics	l_pHierarchyCompensation, XSI::Kinematics l_pHierarchyCompensation2)
{
	char *pString1 = new char [ in_szName.Length() + 1 ];
	char *pString2 = new char [ in_szParentName.Length() + 1 ];

	W2AHelper ( pString1, in_szName.GetWideString() );
	W2AHelper ( pString2, in_szParentName.GetWideString() );

	AddNode ( pString1, pString2, l_pHierarchyCompensation, l_pHierarchyCompensation2);


	delete [] pString1;
	delete [] pString2;


}


void SMDNodeList::AddNodeS ( char* in_szString , int in_iParentID, XSI::X3DObject in_pModel, XSI::Kinematics	l_pHierarchyCompensation, XSI::Kinematics l_pHierarchyCompensation2)
{
	
	for (int n=0;n<m_pNodeList.GetUsed();n++)
	{
		if ( !strcmp ( m_pNodeList[n]->GetName(), in_szString ) )
		{
			return;
		}
	}

	// zero out compensation if the user is exporting with Zero Root Transforms option

	if ( (ExportSelectedOnly)&&(ZeroRootTransforms) && (in_iParentID==-1))
	{
		char l_szMessage[1024];
		sprintf(l_szMessage,"Zeroing out the parent transforms of root node %s", in_szString);
		XSILogMessage(l_szMessage, XSI::siWarningMsg );

		g_ResetTranforms.Extend(1);
		g_ResetTranforms[g_ResetTranforms.GetUsed()-1].m_kine = l_pHierarchyCompensation;
		g_ResetTranforms[g_ResetTranforms.GetUsed()-1].m_xfo = l_pHierarchyCompensation.GetLocal().GetTransform();

		g_ResetTranforms.Extend(1);
		g_ResetTranforms[g_ResetTranforms.GetUsed()-1].m_kine = l_pHierarchyCompensation2;
		g_ResetTranforms[g_ResetTranforms.GetUsed()-1].m_xfo = l_pHierarchyCompensation2.GetLocal().GetTransform();

		XSI::MATH::CTransformation zeroxfo;
		zeroxfo.SetScalingFromValues(1.0,1.0,1.0);

		l_pHierarchyCompensation.GetLocal().PutTransform(zeroxfo);
		l_pHierarchyCompensation2.GetLocal().PutTransform(zeroxfo);
	}

	SMDNode	*l_pNewNode = new SMDNode;

	l_pNewNode->SetName ( in_szString );
	l_pNewNode->SetParentID ( in_iParentID );
	l_pNewNode->SetModel ( in_pModel );
	l_pNewNode->SetHierarchicalCompensation ( l_pHierarchyCompensation );
	l_pNewNode->SetHierarchicalCompensation2 ( l_pHierarchyCompensation2 );
	

	m_pNodeList.Extend(1);
	m_pNodeList[m_pNodeList.GetUsed()-1] = l_pNewNode;

	l_pNewNode->SetID ( m_pNodeList.GetUsed()-1 );

}

void SMDNodeList::AddNode ( XSI::CString in_szName, int in_iParentID, XSI::X3DObject in_pModel, XSI::Kinematics	in_pHierarchyCompensation ,XSI::Kinematics	in_pHierarchyCompensation2 )
{

	char *pSzString = new char [ in_szName.Length() + 1 ];
	W2AHelper ( pSzString, in_szName.GetWideString() );
	
	AddNodeS ( pSzString, in_iParentID, in_pModel, in_pHierarchyCompensation ,in_pHierarchyCompensation2 );

	delete [] pSzString;

}

void SMDNodeList::AddNode ( char *in_szName, char *in_szParentName, XSI::Kinematics	l_pHierarchyCompensation, XSI::Kinematics	in_pHierarchyCompensation2 )
{
	SI_Int		l_iParentID;

	SMDNode*	l_pParent = GetByName ( in_szParentName );
	if ( l_pParent != NULL )
	{
		l_iParentID = l_pParent->GetID ();
	} else {
		l_iParentID = -1;
	}
	
	XSI::X3DObject obj;
	AddNodeS ( (char*)in_szName, l_iParentID, obj, l_pHierarchyCompensation, in_pHierarchyCompensation2 );
}

void SMDNodeList::Write   ( FILE* in_fptr )
{
	if ( m_pNodeList.GetUsed() )
	{
		fprintf ( in_fptr, "nodes\n" );

		for (int s=0;s<m_pNodeList.GetUsed();s++)
		{
			SMDNode* l_pNode = m_pNodeList[s];

			l_pNode->Write ( in_fptr );

		}
		
		fprintf ( in_fptr, "end\n" );
	}

}

SMDNode* SMDNodeList::GetByName ( XSI::CString in_szName )
{
	char *pSzString = new char [ in_szName.Length() + 1 ];
	W2AHelper ( pSzString, in_szName.GetWideString() );

	SMDNode* l_pRet = GetByName ( pSzString );
	
	delete [] pSzString;
	return l_pRet;

}

SMDNode*	SMDNodeList::GetByName ( char *in_szName )
{
	for (int s=0;s<m_pNodeList.GetUsed();s++)
	{
		SMDNode* l_pNode = m_pNodeList[s];

		if ( !_SI_STRCMP ( l_pNode->GetName(),in_szName) )
		{
			return l_pNode;
		}
	}

	return NULL;

}

SMDNode*	SMDNodeList::GetByID ( SI_Int in_iIndex )
{
	for (int s=0;s<m_pNodeList.GetUsed();s++)
	{
		SMDNode* l_pNode = m_pNodeList[s];

		if (l_pNode->GetID() == in_iIndex )
		{
			return l_pNode;
		}
	}

	return NULL;
}

SMDNode*	SMDNodeList::GetByIndex ( SI_Int in_iIndex )
{

	return m_pNodeList[in_iIndex];
}



void	SMDNodeList::ConvertToModelSMD ()
{
	//
	// Remove all nodes
	//

	for (int s=0;s<m_pNodeList.GetUsed();s++)
	{
		SMDNode* l_pNode = m_pNodeList[s];

		delete l_pNode;
	}

	m_pNodeList.DisposeData();

	//
	// Create single root node
	//

	XSI::X3DObject obj;
	XSI::Kinematics hc1,hc2;

	AddNodeS ( "root", -1,  obj, hc1, hc2);

}


void W2AHelper( LPSTR out_sz, LPCWSTR in_wcs, int in_cch )
{
	if ( out_sz != NULL && 0 != in_cch )
	{
		out_sz[0] = '\0' ;

		if ( in_wcs != NULL )
		{
			size_t l_iLen = 0;
			l_iLen = ::wcstombs( out_sz, in_wcs, ( in_cch < 0 ) ? ::wcslen(in_wcs) + 1 : (size_t) in_cch ) ;

			if ( in_cch > 0 ) { out_sz[ in_cch - 1 ] = '\0'; }
		}
	}
}
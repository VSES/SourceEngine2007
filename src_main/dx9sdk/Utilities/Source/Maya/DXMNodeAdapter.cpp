#include "pch.h"

DXMNodeAdapter::DXMNodeAdapter()
{ 
	Node= NULL; 
	CallbackState= false; 
}

DXMNodeAdapter::~DXMNodeAdapter()
{
}

DXMNode* DXMNodeAdapter::GetNode()
{ 
	return Node; 
}

DXMGraph* DXMNodeAdapter::GetGraph()
{ 
	return GetNode()->GetGraph(); 
}

DXMGraphAdapter* DXMNodeAdapter::GetOwnerGraphAdapter()
{ 
	return GetGraph()->FindAdapter(OwnerSignature()); 
}


void DXMNodeAdapter::SetCallbackState(bool Activate)
{ 
	CallbackState= Activate; 
}

bool DXMNodeAdapter::GetCallbackState()
{ 
	return CallbackState; 
}

void DXMNodeAdapter::Initialize(DXMNode* node)
{ 
	Node= node; 
	SetCallbackState(node->GetCallbackState()); 
}

void DXMNodeAdapter::Destroy()
{
	SetCallbackState(false); 

	GetOwnerGraphAdapter()->RemoveSyncRequest(this);
}

void DXMNodeAdapter::OnNameChange(const MString & prevName)
{
}

/*
void DXMNodeAdapter::OnAttributeChanged( MNodeMessage::AttributeMessage msg, MPlug& plug, MPlug& otherPlug)
{
}
*/


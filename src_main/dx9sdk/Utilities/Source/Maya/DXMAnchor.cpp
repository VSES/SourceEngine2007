#include "pch.h"

DXMAnchor::~DXMAnchor()
{
}


MObject				
DXMAnchor::GetSite() 
{ 
	if( !AnchorSite.isValid() || !AnchorSite.isAlive() )
		return MObject::kNullObj;

	return AnchorSite.objectRef(); 
}
	
MString	
DXMAnchor::GetPoint()
{
	return AnchorPoint;
}

/*
void 
DXMAnchor::DispatchAboutToDelete( MObject & node, void* clientData  )
{
	DXMAnchor* This= (DXMAnchor*)clientData;
	This->OnAboutToDelete();
}
	
void 
DXMAnchor::OnNodeRemoved()
{
	Destroy();
	delete this;
}
*/

void 
DXMAnchor::OnPostAddAnchor(const MString& longAnchorName, const MString& shortAnchorName) 
{
}

void 
DXMAnchor::OnPreRemoveAnchor(const MString& shortOrLongAnchorName) 
{ 
}

bool 
DXMAnchor::AddAnchor(MObject& site, const MString& longAnchorName, const MString& shortAnchorName, DXMAnchor* anchor)
{
	DXCC_ASSERT( DXMAnchor::GetAnchor(site, shortAnchorName) == NULL );

	DXCC_ASSERT(anchor != NULL);
	DXCC_ASSERT(anchor->GetSite().isNull());
	DXCC_ASSERT(anchor->GetPoint().length() == 0);

	DXCC_ASSERT(longAnchorName.length() > 0);
	DXCC_ASSERT(shortAnchorName.length() > 0);
	DXCC_ASSERT(!site.isNull());

	MFnDependencyNode depNode(site);
	MString name= depNode.name();
#ifdef DEBUG
	anchor->Name= name.asChar();
#endif

	if( g_DebugBasic )
	{
		DXCC_DPFA_REPORT("%s", name.asChar());
	}

	//this attribute may exist if you had asked for this node to be duplicated or instanced :(
	if( depNode.attribute( shortAnchorName ).isNull() )
	{
		MFnNumericAttribute numeric;
		MObject anchorAttrib= numeric.create(longAnchorName, shortAnchorName, MFnNumericData::kInt, 0);
		numeric.setReadable(false);
		numeric.setWritable(false);
		numeric.setConnectable(false);
		numeric.setStorable(false);
		numeric.setCached(true);
		numeric.setArray(false);
		numeric.setKeyable(false);
		numeric.setHidden(true);
		numeric.setUsedAsColor(false);
		numeric.setIndeterminant(true);
		numeric.setRenderSource(false);
		numeric.setInternal(false);

		DXCC_ASSERT(!anchorAttrib.isNull());
		DXCHECK_MSTATUS( depNode.addAttribute(anchorAttrib) );
	}

	MPlug anchorPlug= depNode.findPlug( shortAnchorName );
	DXCC_ASSERT(!anchorPlug.isNull());

	anchorPlug.setValue( *reinterpret_cast<int*>(&anchor) );

	anchor->AnchorSite= site;
	anchor->AnchorPoint= shortAnchorName;
	
//	anchor->NodeDestroyedCID= MNodeMessage::addNodeDestroyedCallback(site, DXMAnchor::DispatchNodeDestroyed, anchor);
//	anchor->AboutToDeleteCID= MNodeMessage::addNodeAboutToDeleteCallback(site, DXMAnchor::DispatchAboutToDelete, anchor);



	anchor->OnPostAddAnchor(longAnchorName, shortAnchorName);

	return true;
}
	
void			
DXMAnchor::Destroy()
{

	OnPreRemoveAnchor(AnchorPoint);

	//if(AnchorSite.isAlive() && AnchorSite.isValid())
	if( !AnchorSite.objectRef().isNull() )
	{
		MFnDependencyNode depNode(AnchorSite.objectRef());
		MPlug anchorPlug= depNode.findPlug( GetPoint() );
		

		if(!anchorPlug.isNull())
		{
			DXCHECK_MSTATUS( anchorPlug.setValue(0));
			DXCHECK_MSTATUS( depNode.removeAttribute(anchorPlug.attribute()) );
		}
	}
/*
	if(AboutToDeleteCID)
	{
		MMessage::removeCallback( AboutToDeleteCID );
		AboutToDeleteCID= 0;
	}
*/
	AnchorSite= MObject::kNullObj;
	AnchorPoint.clear();
	return;
}


DXMAnchor* 
DXMAnchor::GetAnchor(MObject& site, const MString& shortOrLongAnchorName)
{
	DXCC_ASSERT(shortOrLongAnchorName.length() > 0);
	DXCC_ASSERT(!site.isNull());

	DXMAnchor* anchor= NULL;

	MFnDependencyNode depNode(site);

	MPlug anchorPlug= depNode.findPlug( shortOrLongAnchorName );
	
	//couldnt find the plug
	if(anchorPlug.isNull())
		return NULL;

	DXCHECK_MSTATUS( anchorPlug.getValue(*reinterpret_cast<int*>(&anchor)) );

	//plug had no value
	if (anchor == NULL)
		return NULL;

	//check to see if we are a duplication or instancing ...
	//value does not actually match the requested obj 
	if(anchor->GetSite() != site)
		return NULL;

	return anchor;
}


#pragma once
#ifndef DXMANCHOR_H
#define DXMANCHOR_H

class DXMAnchor
{
private:

#ifdef DEBUG
	CStringA			Name;
#endif

	//MCallbackId			AboutToDeleteCID;
	MObjectHandle		AnchorSite;
	MString				AnchorPoint;

	//static void			DispatchAboutToDelete(MObject & node, void* clientData  );



protected:
	//virtual void		OnAboutToDelete();
	virtual void		OnPostAddAnchor(const MString& longAnchorName, const MString& shortAnchorName);
	virtual void		OnPreRemoveAnchor(const MString& shortOrLongAnchorName);

public:
	static bool			AddAnchor(MObject& site, const MString& longAnchorName, const MString& shortAnchorName, DXMAnchor* anchor);
	static DXMAnchor*	GetAnchor(MObject& site,const  MString& shortOrLongAnchorName);

	virtual				~DXMAnchor();
	virtual void		Destroy();

	MObject				GetSite();
	MString				GetPoint();

};

#endif
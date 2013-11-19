#pragma once
#ifndef VIEWCOMMANDS_H
#define VIEWCOMMANDS_H

#include "maya/MPxCommand.h"
#include "maya/MArgList.h"
#include "maya/MStatus.h"

#define DXCC_VIEW_TOP				"DXCCTopView"
#define DXCC_VIEW_SIDE				"DXCCSideView"
#define DXCC_VIEW_FRONT				"DXCCFrontView"
#define DXCC_VIEW_FLOATING			"DXCCFloatingView"
#define DXCC_VIEW_CLOSE				"DXCCCloseViewer"

class DXCCFrontViewCommand : public MPxCommand 
{							
public:															
	DXCCFrontViewCommand() {};								
	static void*	creator(){ return new DXCCFrontViewCommand; }									
	virtual MStatus	doIt ( const MArgList& args);			
};	

class DXCCSideViewCommand : public MPxCommand 
{							
public:															
	DXCCSideViewCommand() {};								
	static void*	creator(){ return new DXCCSideViewCommand; }									
	virtual MStatus	doIt ( const MArgList& args);			
};

class DXCCTopViewCommand : public MPxCommand 
{							
public:															
	DXCCTopViewCommand() {};								
	static void*	creator(){ return new DXCCTopViewCommand; }									
	virtual MStatus	doIt ( const MArgList& args);
};

class DXCCFloatingViewCommand : public MPxCommand 
{							
public:															
	DXCCFloatingViewCommand() {};								
	static void*	creator(){ return new DXCCFloatingViewCommand; }									
	virtual MStatus	doIt ( const MArgList& args);
};

class DXCCCloseViewCommand : public MPxCommand 
{							
public:															
	DXCCCloseViewCommand() {};								
	static void*	creator(){ return new DXCCCloseViewCommand; }									
	virtual MStatus	doIt ( const MArgList& args);
};
#endif
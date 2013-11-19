#pragma once
#ifndef REBUILDCOMMANDS_H
#define REBUILDCOMMANDS_H

#include "maya/MPxCommand.h"
#include "maya/MArgList.h"
#include "maya/MStatus.h"

#define DXCC_REBUILD_SCENE			"DXCCRebuildScene"
#define DXCC_REBUILD_DIRTY			"DXCCRebuildDirty"

class DXCCRebuildSceneCommand : public MPxCommand 
{							
public:															
	DXCCRebuildSceneCommand() {};								
	virtual MStatus	doIt ( const MArgList& args);
	static void*	creator(){ return new DXCCRebuildSceneCommand; }									
};		

class DCCRebuildDirtyCommand : public MPxCommand 
{							
public:															
	DCCRebuildDirtyCommand() {};								
	static void*	creator(){ return new DCCRebuildDirtyCommand; }									
	virtual MStatus	doIt ( const MArgList& args);			
};	

#endif
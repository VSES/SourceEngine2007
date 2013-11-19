#pragma once
#ifndef EXPORTCOMMANDS_H
#define EXPORTCOMMANDS_H

#define DXCC_EXPORT_SCENE			"DXCCExportScene"
#define DXCC_EXPORT_OPTIONS			"DXCCExportOptions"
//#define DXCC_EXPORT_SELECTED		"DXCCExportSelected"



class DXCCExportSceneCommand : public MPxCommand 
{							
public:															
	DXCCExportSceneCommand() {};								
	static void*	creator(){ return new DXCCExportSceneCommand; }									
	virtual MStatus	doIt ( const MArgList& args);				
};	

class DXCCExportOptionsCommand : public MPxCommand 
{							
public:															
	DXCCExportOptionsCommand() {};								
	static void*	creator(){ return new DXCCExportOptionsCommand; }									
	virtual MStatus	doIt ( const MArgList& args);				
};	

#endif
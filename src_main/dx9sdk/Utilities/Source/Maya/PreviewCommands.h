#pragma once
#ifndef PREVIEWCOMMANDS_H
#define PREVIEWCOMMANDS_H


#define DXCC_PREVIEW_CHANGED			"DXCCPreviewChanged"

class DXCCPreviewChangedCommand : public MPxCommand 
{							
public:															
	DXCCPreviewChangedCommand() {};								
	static void*	creator(){ return new DXCCPreviewChangedCommand; }									
	virtual MStatus	doIt ( const MArgList& args);				
};

#endif
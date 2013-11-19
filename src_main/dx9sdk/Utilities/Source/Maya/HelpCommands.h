#pragma once
#ifndef HELPCOMMANDS_H
#define HELPCOMMANDS_H


#define DXM_UIHELP "DXMUiHelp"

class DXMUiHelpCommand : public MPxCommand 
{							
public:															
	DXMUiHelpCommand() {};								
	static void*	creator(){ return new DXMUiHelpCommand; }									
	virtual MStatus	doIt ( const MArgList& args);			
};	

#endif
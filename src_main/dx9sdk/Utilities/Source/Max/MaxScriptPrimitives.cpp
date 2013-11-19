
#include "StdAfx.h"
#include "MAXScrpt/definsfn.h"

/*

 foreach group in list
	DxeSetExportProfile "e:\\maxtests\\group1\profile.xml"
	DosCommand "md e:\\maxtests\\group1\\xfiles"
	 
	foreach file in list
		LoadMaxFile "e:\\maxtests\\test.max"
		DxeSetExportProfileValue "OutputFilename" "e:\\maxtests\\group1\\xfiles\\test.x"
		DxeExportFile
 */


def_struct_primitive( DxeSetExportProfile, dxe, "SetExportProfile" );
Value* DxeSetExportProfile_cf(Value** argv, int argc)
{
	check_arg_count(DxeSetExportProfile, 1, argc);
//	XFileExporter_SetProfileName(argv[0]->to_string());
	return &true_value;
}

def_struct_primitive( DxeSetExportProfileValue, dxe, "SetExportProfileValue" );
Value* DxeSetExportProfileValue_cf(Value** argv, int argc)
{
	// DxeSetExportProfileValue Name0 Value0 Name1 Value1

	return &true_value;
}

def_struct_primitive( DxeShowViewer, dxe, "ShowViewer" );
Value* DxeShowViewer_cf(Value** argv, int argc)
{
	// DxeShowViewer PanelIndex

	return &true_value;
}


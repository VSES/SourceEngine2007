'##############################################################################
' Weight Export
' (c)Copyright 2003 Softimage Co.
' XSI version 3.5+
' $Author$	Dominic Laflamme
' $Date$
' $Revision$
'##############################################################################
Option Explicit

sub WeightExport(file)
	Dim oRoot,oDialog, oParams, oParam
	
	'Load the Export Dialog
	set oRoot = ActiveProject.ActiveScene.Root
	set oDialog = oRoot.AddProperty("WeightExport",,"Weight_Export")

	'Set the default filename
	if file <> "" then
		oDialog.parameters("Filename").value = file
	else
		oDialog.parameters("Filename").value = ""
	end if

	'Display the dialog box
	On Error Resume Next
	InspectObj oDialog,,"Weight Export",siModal
	
	If Err.Number <> 0 then
		DeleteObj oDialog
		exit sub
	else
		On Error Goto 0
		set oParams = oDialog.parameters
		WeightExportProc oParams("Filename").value, oParams("Fullname").value
	end if	
	
	DeleteObj oDialog

end sub

function WeightExportProc(filename, fullname)

	WeightExportBin filename, fullname

end function


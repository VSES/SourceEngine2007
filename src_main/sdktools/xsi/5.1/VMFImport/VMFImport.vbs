'##############################################################################
' VMF Import 
 '(c)Copyright 2003 Softimage Co.
' XSI version 2.02+
' $Author$	Dominic Laflamme
' $Date$
' $Revision$
'##############################################################################
Option Explicit

sub fVMFImport(file)
	Dim oRoot,oDialog, oParams, oParam
	
	'Load the Export Dialog
	set oRoot = ActiveProject.ActiveScene.Root
	set oDialog = oRoot.AddProperty("vmfImportOptions",,"VMFImport")

	'Set the default filename
	if file <> "" then
		oDialog.parameters("Filename").value = file
	else
		oDialog.parameters("Filename").value = ""
	end if

	'
	' load persisted options
	'
	on error resume next

	dim FSOI
	set FSOI = CreateObject ( "Scripting.FileSystemObject")

	err.clear
	dim oFI
	set oFI = FSOI.OpenTextFile ("C:\vmfimp.dat")

	if err.number = 0 then
	
		dim s
		s = oFI.ReadLine()
		oDialog.parameters("Filename").value = s
		s = oFI.ReadLine()
		oDialog.parameters("TexturePath").value = s

		oFI.Close
	
	end if
	
	on error goto 0

	'Display the dialog box
	On Error Resume Next
	InspectObj oDialog,,"VMF Import",siModal
	
	If Err.Number <> 0 then
		DeleteObj oDialog
		exit sub
	else
		On Error Goto 0
		set oParams = oDialog.parameters
		fVMFImportProc oParams("Filename").value,_
								oParams("TexturePath").value,_
								oParams("UseMaterials").value
								
	end if	
	
	'
	' Persit the options
	'

	dim FSO
	set FSO = CreateObject ( "Scripting.FileSystemObject")

	dim oF
	set oF = FSO.CreateTextFile ("C:\vmfimp.dat")
	oF.WriteLine oParams("Filename").value
	oF.WriteLine oParams("TexturePath").value
	oF.Close


	DeleteObj oDialog

end sub

function fVMFImportProc(_
ByRef filename, _
ByRef texturepath, _
ByRef usematerials _
)



	fVMFImportBin filename, texturepath, usematerials

end function


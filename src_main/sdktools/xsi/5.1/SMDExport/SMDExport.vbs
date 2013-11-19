'##############################################################################
' VALVE SMD EXPORT prototype
' (c)Copyright 2002 Dominic Laflamme for Valve Software
' XSI version 2.02+
' $Author$
' $Date$
' $Revision$
'##############################################################################
Option Explicit

const cSingleFile = 0
const cFilePerObject = 1
const cSequence = True
const cStatic = False
const cPoly = True

sub SMDExport(file)
	Dim oRoot,oDialog, oParams, oParam
	
	'Load the Export Dialog
	set oRoot = ActiveProject.ActiveScene.Root
	set oDialog = oRoot.AddProperty("SMDExportOptions",,"SMD_Export")

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
	set oFI = FSOI.OpenTextFile ("C:\smdexp.dat")

	if err.number = 0 then
	
		dim s
		s = oFI.ReadLine()
		oDialog.parameters("Filename").value = s
		s = oFI.ReadLine()
		oDialog.parameters("SMDType").value = int(s)

		s = oFI.ReadLine()
		if s = "False" then 
			oDialog.parameters("HRC").value = 0
		else 
			oDialog.parameters("HRC").value = 1
		end if

		s = oFI.ReadLine()
		if s = "False" then 
			oDialog.parameters("RemoveRigRoot").value = 0
		else 
			oDialog.parameters("RemoveRigRoot").value = 1
		end if

		s = oFI.ReadLine()
		if s = "False" then 
			oDialog.parameters("RemoveUnusedBones").value = 0
		else 
			oDialog.parameters("RemoveUnusedBones").value = 1
		end if


		oFI.Close
	
	end if

	on error goto 0

'oDialog.parameters("SMDType").value = itype

	'Display the dialog box
	On Error Resume Next
	InspectObj oDialog,,"Valve SMD Export",siModal
	
	If Err.Number <> 0 then
		DeleteObj oDialog
		exit sub
	else
		On Error Goto 0
		set oParams = oDialog.parameters
		SMDExportProc oParams("Filename").value, oParams("SMDType").value, oParams("HRC").value, oParams("RemoveRigRoot").value, oParams("RemoveUnusedBones").value
	end if	
	
	'
	' Persit the options
	'

	dim FSO
	set FSO = CreateObject ( "Scripting.FileSystemObject")

	dim oF
	set oF = FSO.CreateTextFile ("C:\smdexp.dat")
	oF.WriteLine oParams("Filename").value
	oF.WriteLine oParams("SMDType").value
	oF.WriteLine oParams("HRC").value
	oF.WriteLine oParams("RemoveRigRoot").value
	oF.WriteLine oParams("RemoveUnusedBones").value
	oF.Close


	DeleteObj oDialog

end sub

function SMDExportProc(filename,ismdtype, ishrc, removerigroot, removeunused )

	SMDExportBin filename, ismdtype, ishrc, removerigroot, removeunused

end function


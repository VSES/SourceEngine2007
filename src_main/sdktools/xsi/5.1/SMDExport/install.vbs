installDllExp
installScriptExp



function installDllExp
cmdname="ValveSMDExportB"
Application.RemoveCommand cmdname
set cmd = Application.CreateCommand(cmdname,siNoCategory)
cmd.ScriptingName = "SMDExportBin"
cmd.FileName = "V:\ThirdParty\ClientTools\Addons\ValveSource\src\SMDExport\SMDExport.dll"
cmd.Language = "CPP"
cmd.Description = "Export Valve Software SMD file (Compiled Function)"
cmd.arguments.add "Filename", siArgumentInput,"", siString
cmd.arguments.add "SMDType",siArgumentInput,0,siInt4
cmd.arguments.add "HRC",siArgumentInput,0,siInt4
cmd.arguments.add "RemoveRigRoot",siArgumentInput,0,siInt4
cmd.arguments.add "RemoveUnusedBones",siArgumentInput,0,siInt4
Application.AddCommand cmd 
end function

function installScriptExp
cmdname="SMD Export..."
Application.RemoveCommand cmdname
set cmd = Application.CreateCommand("SMD Export...", siExportCategory)
cmd.Description = "Exports the scene to a Valve .SMD file."
cmd.ScriptingName = "ValveSMDExport"
cmd.FileName = "s:\ThirdParty\ClientTools\Valve\SMDExport\SMDExport.vbs"
cmd.Language = "VBScript"
cmd.Handler = "SMDExport"
Application.AddCommand cmd
end function
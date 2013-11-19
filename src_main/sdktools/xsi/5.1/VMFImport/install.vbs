installDllExp
installScriptExp



function installDllExp
cmdname="fVMFImportB"
Application.RemoveCommand cmdname
set cmd = Application.CreateCommand(cmdname,siNoCategory)
cmd.ScriptingName = "fVMFImportBin"
cmd.FileName = "S:\ThirdParty\ClientTools\Valve\VMFImport\debug\VMFImport.dll"
cmd.Language = "CPP"
cmd.Description = "Import Valve .VMF file (Compiled Function)"
cmd.arguments.add "Filename", siArgumentInput,"", siString
cmd.arguments.add "TexturePath", siArgumentInput,"", siString
cmd.arguments.add "UseMaterials",siArgumentInput,0,siInt4

Application.AddCommand cmd 
end function

function installScriptExp
cmdname="VMF Import..."
Application.RemoveCommand cmdname
set cmd = Application.CreateCommand("VMF Import...", siImportCategory)
cmd.Description = "Import a Valve .VMF file"
cmd.ScriptingName = "fVMFImport"
cmd.FileName = "S:\ThirdParty\ClientTools\Valve\VMFImport\VMFImport.vbs"
cmd.Language = "VBScript"
cmd.Handler = "fVMFImport"
Application.AddCommand cmd
end function
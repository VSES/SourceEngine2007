installDllExp
installScriptExp



function installDllExp
cmdname="WeightExportBin"
Application.RemoveCommand cmdname
set cmd = Application.CreateCommand(cmdname,siNoCategory)
cmd.ScriptingName = "WeightExportBin"
cmd.FileName = "S:\Rayflex\Converters\WeightTransfer\Export\Debug\WeightExport.dll"
cmd.Language = "CPP"
cmd.Description = "Export Weight Information to a file (Compiled Function)"
cmd.arguments.add "Filename", siArgumentInput,"", siString
cmd.arguments.add "Fullname", siArgumentInput,0,siInt4
Application.AddCommand cmd 
end function

function installScriptExp
cmdname="Weight Export..."
Application.RemoveCommand cmdname
set cmd = Application.CreateCommand("Weight Export...", siExportCategory)
cmd.Description = "Exports the selected object's weight to a file."
cmd.ScriptingName = "WeightExport"
cmd.FileName = "S:\Rayflex\Converters\WeightTransfer\Export\WeightExport.vbs"
cmd.Language = "VBScript"
cmd.Handler = "WeightExport"
Application.AddCommand cmd
end function
installDllExp
installScriptExp



function installDllExp
cmdname="WeightImportBin"
Application.RemoveCommand cmdname
set cmd = Application.CreateCommand(cmdname,siNoCategory)
cmd.ScriptingName = "WeightImportBin"
cmd.FileName = "S:\Rayflex\Converters\WeightTransfer\Import\Debug\WeightImport.dll"
cmd.Language = "CPP"
cmd.Description = "Import Weight Information to a file (Compiled Function)"
cmd.arguments.add "Filename", siArgumentInput,"", siString
cmd.arguments.add "Fullname", siArgumentInput,0,siInt4
Application.AddCommand cmd 
end function

function installScriptExp
cmdname="Weight Import..."
Application.RemoveCommand cmdname
set cmd = Application.CreateCommand("Weight Import...", siImportCategory)
cmd.Description = "Imports the selected object's weight to a file."
cmd.ScriptingName = "WeightImport"
cmd.FileName = "S:\Rayflex\Converters\WeightTransfer\Import\WeightImport.vbs"
cmd.Language = "VBScript"
cmd.Handler = "WeightImport"
Application.AddCommand cmd
end function
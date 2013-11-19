Application.RemoveCommand "Valve .map"

set cmd = Application.CreateCommand("Valve .VMF", siImportCategory)
cmd.ScriptingName = "VMFImport"
cmd.FileName = "S:\ThirdParty\ClientTools\Valve\VMFImport\Debug\VMFImport.dll"
cmd.Language = "CPP"
Application.AddCommand cmd 



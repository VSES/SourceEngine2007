Application.RemoveCommand "DirectX .x Export"

set cmd = Application.CreateCommand("DirectX .x Export", siExportCategory)
cmd.ScriptingName = "DirectXExport"
cmd.FileName = "S:\Rayflex\Converters\DirectX\Export\Debug\DirectXExport.dll"
cmd.Language = "CPP"
Application.AddCommand cmd 



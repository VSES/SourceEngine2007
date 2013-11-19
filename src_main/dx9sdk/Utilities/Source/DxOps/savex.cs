//--------------------------------------------------------------------------------------
// Copyright (c) Microsoft Corporation. All rights reserved.
//--------------------------------------------------------------------------------------
using System;
using System.Collections;
using System.Collections.Specialized;
using System.Text;
using System.IO;
using System.Reflection;
using System.Text.RegularExpressions;
using Microsoft.DirectX.Direct3D;
using Microsoft.DirectX.Solutions;
using Microsoft.DirectX.Solutions.DxOps.Exceptions;
using Microsoft.DirectX.Solutions.DxOps.Commands.Arguments;


namespace Microsoft.DirectX.Solutions.DxOps.Commands
{
    namespace Arguments
    {
        public class SavexArguments
        {
            public string Filename = null;
            public XFileFormat saveType = XFileFormat.Text;

            public SavexArguments(OpsParsedStatement statement)
            {
                OpsParsedArgument fileArg = statement.FindArgument("filename");
                if(fileArg == null)
                {
                    fileArg = statement.FindArgument("");
                    if(fileArg == null)
                        throw OpsException.ArgRequired("filename");
                }
                Filename = fileArg.Value;
                if(!Path.IsPathRooted(Filename))
                {
                    Filename = Path.Combine(Directory.GetCurrentDirectory(), Filename);
                }

                OpsParsedArgument argX = statement.FindArgument("XFileType");
                if(argX != null)
                {
                    if( 0 == String.Compare( argX.Value, "text", true))
                        saveType = XFileFormat.Text;
                    else if( 0 == String.Compare( argX.Value, "binary", true))
                        saveType = XFileFormat.Binary;
                    else if( 0 == String.Compare( argX.Value, "compressed", true))
                        saveType = XFileFormat.Compressed;
                    else
                        throw new OpsException("Unrecognized XFile argument value.  Use (text|binary|compressed)");
                }

            }
        }
    }


    public class SavexCommand : IOpsCommand
    {

        public SavexCommand()
        {}

        //IOpsCommand
        public string Name 
        { 
            get{ return "SaveX"; } 
        }
        public ContentType Content {get { return ContentType.MODELS; }}
        //IOpsCommand
        public object ParseArguments(OpsParsedStatement statement)
        {
            return new SavexArguments(statement);
        }

        //IOpsCommand
        public void Run(OpsContext context, OpsStatement statement)
        {
            SavexArguments arguments = statement.Arguments as SavexArguments;

            ArrayList models = statement.GetContent(context);


            if(models.Count != 1)
            {
                string errorStr = string.Format( "Save requires only one model but found {0}.  Try changing/using the Src argument.", models.Count );
                throw new OpsException(errorStr);
            }

            OpsModel model = models[0] as OpsModel;

            OpsConsole.WriteLine( "Saving model: {0} to path: {1}", model.Name , arguments.Filename);

            if(model.HierarchyRoot == null)
                throw new OpsException("There is nothing currently in the application's context.  Must use a Load command first.");

            Mesh.SaveHierarchyToFile( arguments.Filename, arguments.saveType, model.HierarchyRoot, model.AnimationMixer, null);        
        }
    }
}

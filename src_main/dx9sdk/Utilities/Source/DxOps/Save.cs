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
    public class SaveCommand : IOpsCommand, IOpsRemappable
    {

        public SaveCommand()
        {}

                //IOpsCommand
        public string Name 
        { 
            get{ return "Save"; } 
        }

        public ContentType Content 
        {
            get 
            { 
                return ContentType.GENERIC; 
            }
        }
                
                
        public IOpsCommand RemapCommand(OpsParsedStatement statement)
        { 
            OpsParsedArgument fileArg = statement.FindArgument("filename");
            if(fileArg == null)
            {
                fileArg = statement.FindArgument("");
                if(fileArg == null)
                    throw OpsException.ArgRequired("filename");
            }
            string filename = fileArg.Value;

            string extension = System.IO.Path.GetExtension(filename);

            if(extension == null || extension[0] == '\0')
                throw new OpsException("Cannot find an appropriate loader for file: " + filename);

            if(extension[0] == '.')
                extension = extension.TrimStart( new char[]{ '.' });

            return Program.CommandLibrary.GetCommand( "Save" + extension );
        }



                //IOpsCommand
        public object ParseArguments(OpsParsedStatement statement)
        {
            throw new OpsException("This command must be remapped or implemented.");
        }

                //IOpsCommand
        public void Run(OpsContext context, OpsStatement statement)
        {
            throw new OpsException("This command must be remapped or implemented.");
        }

    }
}

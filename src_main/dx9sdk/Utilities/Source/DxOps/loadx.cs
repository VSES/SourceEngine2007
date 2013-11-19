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
    public class LoadxCommand : IOpsCommand
    {

        public LoadxCommand()
        {}

        //IOpsCommand
        public string Name 
        { 
            get{ return "Loadx"; } 
        }
        public ContentType Content {get { return ContentType.MODELS; }}
        //IOpsCommand
        public object ParseArguments(OpsParsedStatement statement)
        {
            OpsParsedArgument pathArg = statement.FindArgument("filename");
            if(pathArg == null)
            {
                pathArg = statement.FindArgument("");
                if(pathArg == null)
                    throw OpsException.ArgRequired("filename");
            }
            return pathArg.Value;
        }

        //IOpsCommand
        public void Run(OpsContext context, OpsStatement statement)
        {
            string[] paths = OpsHelpers.ResolvePathToMany( statement.Arguments as string );

            foreach(string path in paths )
            {
                OpsConsole.WriteLine("Loading model from file: \"{0}\"", path );

                OpsModel model = OpsModel.FromFile(context.Device, path);

                context.AddModel(model);
            }
        }

    }

}
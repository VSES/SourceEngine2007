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
using Microsoft.DirectX.Solutions.DxOps.Commands.Arguments;


namespace Microsoft.DirectX.Solutions.DxOps.Commands
{
    public class ResetCommand : IOpsCommand
    {

        public ResetCommand()
        {}

        //IOpsCommand
        public string Name 
        { 
            get{ return "Reset"; } 
        }
        public ContentType Content {get { return ContentType.GENERIC; }}
        //IOpsCommand
        public object ParseArguments(OpsParsedStatement statement)
        {
            return null;
        }

        //IOpsCommand
        public void Run(OpsContext context, OpsStatement statement)
        {
            OpsConsole.WriteLine( "Resetting the entire context");
            context.Reset();
        }
    }
}
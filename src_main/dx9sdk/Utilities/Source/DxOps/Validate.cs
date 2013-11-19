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

    public class ValidateCommand : IOpsCommand
    {

        public ValidateCommand()
        {}

        //IOpsCommand
        public string Name 
        { 
            get{ return "Validate"; } 
        }
        public ContentType Content {get { return ContentType.MODELS; }}
        //IOpsCommand
        public object ParseArguments(OpsParsedStatement statement)
        {
            return null;
        }

        //IOpsCommand
        public void Run(OpsContext context, OpsStatement statement)
        {
            OpsConsole.WriteLine("Validating models" );

            foreach( OpsModel model in statement.GetContent(context) )
            {
                foreach( OpsMeshContainer meshContainer in model.GetMeshEnumerator() )
                {    
                    string errorsAndWarnings;
                    meshContainer.MeshData.Mesh.Validate(meshContainer.GetAdjacencyStream(), out errorsAndWarnings);

                    if(errorsAndWarnings.Length != 0)
                    {
                        OpsConsole.WriteLine("Invalid " + meshContainer.FriendlyName(model) );
                        OpsConsole.WriteLine(errorsAndWarnings);
                    }
                }
            
            }
        }
    }
}
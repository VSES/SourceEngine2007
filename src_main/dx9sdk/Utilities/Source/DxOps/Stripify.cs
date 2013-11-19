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
using Microsoft.DirectX;
using Microsoft.DirectX.Direct3D;
using Microsoft.DirectX.Solutions;
using Microsoft.DirectX.Solutions.DxOps.Commands.Arguments;


namespace Microsoft.DirectX.Solutions.DxOps.Commands
{
    namespace Arguments
    {

        public class StripifyArguments
        {
            public MeshFlags meshFlags = MeshFlags.OptimizeStripeReorder;

            public StripifyArguments(OpsParsedStatement statement)
            {
                OpsParsedArgument splits = statement.FindArgument("splits");
                if( splits != null && int.Parse(splits.Value) == 0)
                    meshFlags |= MeshFlags.OptimizeDoNotSplit;
            }
        }
    }

    public class StripifyCommand : IOpsCommand
    {

        public StripifyCommand()
        {}

        //IOpsCommand
        public string Name 
        { 
            get{ return "Stripify"; } 
        }
        public ContentType Content {get { return ContentType.MODELS; }}
        //IOpsCommand
        public object ParseArguments(OpsParsedStatement statement)
        {
            return new StripifyArguments(statement);
        }

        //IOpsCommand
        public void Run(OpsContext context, OpsStatement statement)
        {

            StripifyArguments arguments = statement.Arguments as StripifyArguments;

            OpsConsole.WriteLine( "Stripifying models" );

            foreach( OpsModel model in statement.GetContent(context) )
            {
                foreach( OpsMeshContainer meshContainer in model.GetMeshEnumerator() )
                {    
                    int[] adjIn= null;
                    int[] adjOut= null;
                    int[] faceRemap= null;
                    GraphicsStream vertexRemapStream= null;

                    adjIn= meshContainer.GetAdjacency();
                    Mesh oldMesh = meshContainer.MeshData.Mesh;
                    Mesh newMesh = oldMesh.Optimize(arguments.meshFlags, adjIn, out adjOut, out faceRemap, out vertexRemapStream);
                    meshContainer.ReplaceMesh(newMesh);
                    meshContainer.SetAdjacency(adjOut);
                    meshContainer.RemapSkin(vertexRemapStream);
                }
            }
        }
    }
}
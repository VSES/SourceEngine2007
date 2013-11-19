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
        public class AdjacencyArguments
        {
            public bool Topological= true;
            public float Epsilon = 1.0e-6f;

            public AdjacencyArguments(OpsParsedStatement statement)
            {
                OpsParsedArgument type = statement.FindArgument("type");
                if(type != null)
                {
                    if(0==String.Compare( type.Value,"topological" , true))
                        Topological = true;
                    else if(0==String.Compare( type.Value,"geometric" , true))
                        Topological = false;
                    else
                        throw new OpsException("Type argument does not match a valid type (topological|geometric)");
                }

                OpsParsedArgument epsilon = statement.FindArgument("epsilon");
                if(epsilon!=null)
                    Epsilon= float.Parse(epsilon.Value);
            }
        }
    }


    public class AdjacencyCommand : IOpsCommand
    {

        public AdjacencyCommand()
        {}

        //IOpsCommand
        public string Name 
        { 
            get{ return "GenAdjacency"; } 
        }

        public ContentType Content 
        {
            get 
            { 
                return ContentType.MODELS; 
            }
        }

        //IOpsCommand
        public object ParseArguments(OpsParsedStatement statement)
        {
            return new AdjacencyArguments(statement);
        }

        //IOpsCommand
        public void Run(OpsContext context, OpsStatement statement)
        {
            AdjacencyArguments arguments = statement.Arguments as AdjacencyArguments;

            OpsConsole.WriteLine( "Generating Adjacencies");

            foreach( OpsModel model in statement.GetContent(context) )
            {
                foreach( OpsMeshContainer meshContainer in model.GetMeshEnumerator() )
                {        
                    int[] adj = null;
                    if(arguments.Topological)
                    {
                        adj = meshContainer.MeshData.Mesh.ConvertPointRepsToAdjacency((int[])null);
                    }
                    else
                    {
                        adj= new int[meshContainer.MeshData.Mesh.NumberFaces*3];
                        meshContainer.MeshData.Mesh.GenerateAdjacency(
                            arguments.Epsilon, 
                            adj);
                    }

                    meshContainer.SetAdjacency(adj);
                }
            }
        }
    }
}
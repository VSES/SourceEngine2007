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


    public class UVAtlasCommand : IOpsCommand
    {

        public UVAtlasCommand()
        {}

        //IOpsCommand
        public string Name 
        { 
            get{ return "uvatlas"; } 
        }
        public ContentType Content {get { return ContentType.MODELS; }}
        //IOpsCommand
        public object ParseArguments(OpsParsedStatement statement)
        {
            return new OpsUVAtlasArguments(statement);
        }

        //IOpsCommand
        public void Run(OpsContext context, OpsStatement statement)
        {
            OpsUVAtlasArguments options = statement.Arguments as OpsUVAtlasArguments;

            OpsConsole.WriteLine("UVAtlasing models.  This might take a while.");

            foreach( OpsModel model in statement.GetContent(context) )
            {
                foreach( OpsMeshContainer meshContainer in model.GetMeshEnumerator() )
                {    
                    OpsConsole.WriteLine( "\t\tBeginning " + meshContainer.FriendlyName(model));

                    VertexElement[] VEs= meshContainer.MeshData.Mesh.Declaration;
                    
                    int destinationIdx;
                    if( false == MeshDeclarationHelper.FindElement(VEs, DeclarationUsage.TextureCoordinate, options.TexIdx, out destinationIdx) )
                    {
                        throw new OpsException("Cannot generate UVAtlas without texture coordinates (use command AddVData)");
                    }

                    UVAtlasOutput uvout;
                    try
                    {
                        uvout = UVAtlas.Create(
                            meshContainer.MeshData.Mesh, 
                            options.MaxCharts, 
                            options.Stretch, 
                            options.Width, 
                            options.Height, 
                            options.Gutter, 
                            options.TexIdx, 
                            meshContainer.GetAdjacencyStream(), 
                            null,
                            null,
                            0.0f);    
                    }
                    catch(Microsoft.DirectX.Direct3D.InvalidMeshException e)
                    {
                        throw new OpsException("UVAtlas considers this mesh invalid.  This is most likely because the mesh is non-manifold.  Using the clean and optimize commands first to remove some non-manifold properties MAY fix the problem.  Also consider less restrictive argument values.", e);

                    }
                    catch(Exception e)
                    {
                        throw new OpsException("UVAtlas was unable to complete.  Try using more flexible argument values, cleaning, and/or optimizing models first.", e);
                        
                    }

                    meshContainer.ReplaceMesh( uvout.Mesh );
                    meshContainer.RemapSkin( uvout.VertexRemapArray );
                
                    OpsConsole.WriteLine( "\ttCompleted");
                }
            }
        }
    }


    namespace Arguments
    {
        public class OpsUVAtlasArguments
        {
            public int TexIdx = 0;
            public int MaxCharts = 0;
            public float Stretch = 0.1f;
            public int Gutter = 2;
            public int Width = 512;
            public int Height = 512;

            public OpsUVAtlasArguments(OpsParsedStatement statement)
            {
                OpsParsedArgument argTexIdx = statement.FindArgument("TexIdx");
                if(argTexIdx != null)
                    TexIdx = int.Parse(argTexIdx.Value);

                OpsParsedArgument argMaxcharts = statement.FindArgument("MaxCharts");
                if(argMaxcharts != null)
                    MaxCharts = int.Parse(argMaxcharts.Value);

                OpsParsedArgument argStretch = statement.FindArgument("Stretch");
                if(argStretch != null)
                    Stretch = float.Parse(argStretch.Value);

                OpsParsedArgument argGutter = statement.FindArgument("Gutter");
                if(argGutter != null)
                    Gutter = int.Parse(argGutter.Value);

                OpsParsedArgument argWidth = statement.FindArgument("Height");
                if(argWidth != null)
                    Width = int.Parse(argWidth.Value);

                OpsParsedArgument argHeight = statement.FindArgument("Height");
                if(argHeight != null)
                    Height = int.Parse(argHeight.Value);
            }

        }
    }
}
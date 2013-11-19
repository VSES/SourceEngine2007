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
    public enum WeldType
    {
        None,
        Epsilon,
        All
    }

    namespace Arguments
    {
        public class OptimizeArguments
        {

            public MeshFlags meshFlags = MeshFlags.OptimizeCompact | MeshFlags.OptimizeAttributeSort;
            public WeldType weldType = WeldType.Epsilon;
            public WeldEpsilonsFlags epsilonFlags= 0;
            public WeldEpsilons epsilons = new WeldEpsilons();


            public OptimizeArguments(OpsParsedStatement statement)
            {
                OpsParsedArgument splits = statement.FindArgument("splits");
                OpsParsedArgument nocache = statement.FindArgument("nocache");
                OpsParsedArgument weld = statement.FindArgument("weld");
                OpsParsedArgument e = statement.FindArgument("e");
                OpsParsedArgument P = statement.FindArgument("P");
                OpsParsedArgument W = statement.FindArgument("W");
                OpsParsedArgument N = statement.FindArgument("N");
                OpsParsedArgument T = statement.FindArgument("T");
                OpsParsedArgument B = statement.FindArgument("B");
                OpsParsedArgument UV = statement.FindArgument("UV");
                OpsParsedArgument D = statement.FindArgument("D");
                OpsParsedArgument S = statement.FindArgument("S");
                OpsParsedArgument PS = statement.FindArgument("PS");
                OpsParsedArgument F = statement.FindArgument("F");


                if( splits != null && int.Parse(splits.Value) == 0)
                    meshFlags |= MeshFlags.OptimizeDoNotSplit;

                if( nocache != null && int.Parse(nocache.Value) == 0)
                    meshFlags |= MeshFlags.OptimizeVertexCache;

                if( weld != null)
                {
                    if( 0 == String.Compare( weld.Value, "none", true) )
                    {
                        weldType = WeldType.None;
                    }
                    else if( 0 == String.Compare( weld.Value, "all", true) )
                    {
                        weldType = WeldType.All;
                        epsilonFlags |= WeldEpsilonsFlags.WeldAll;
                    }
                }


                if(weldType == WeldType.Epsilon)
                {
                    float delta = 1.0e-6f;

                    if(e != null)
                        delta = float.Parse( e.Value );

                    epsilons.Binormal=delta;
                    epsilons.BlendWeights=delta;
                    epsilons.Diffuse=delta;
                    epsilons.Normal=delta;
                    epsilons.PointSize=delta;
                    epsilons.Position=delta;
                    epsilons.Specular=delta;
                    epsilons.Tangent=delta;
                    epsilons.TessellateFactor=delta;

                    if(P != null)
                        epsilons.Position = float.Parse( P.Value );

                    if(W != null)
                        epsilons.BlendWeights = float.Parse( W.Value );

                    if(N != null)
                        epsilons.Normal = float.Parse( N.Value );

                    if(T != null)
                        epsilons.Tangent = float.Parse( T.Value );

                    if(B != null)
                        epsilons.Binormal = float.Parse( B.Value );

                    float uvDelta = (UV == null) ? delta : float.Parse( UV.Value );
                    float[] uvDeltaArray = epsilons.TextureCoordinate;
                    for( int i = 0; i < uvDeltaArray.Length; i++)
                        uvDeltaArray[i] = uvDelta;
                    epsilons.TextureCoordinate = uvDeltaArray;

                    if(D != null)
                        epsilons.Diffuse = float.Parse( D.Value );

                    if(S != null)
                        epsilons.Specular = float.Parse( S.Value );

                    if(PS != null)
                        epsilons.PointSize = float.Parse( PS.Value );

                    if(F != null)
                        epsilons.TessellateFactor = float.Parse( F.Value );
                }

            
            }


        }
    }

    public class OptimizeCommand : IOpsCommand
    {

        public OptimizeCommand()
        {}

        //IOpsCommand
        public string Name 
        { 
            get{ return "Optimize"; } 
        }
        public ContentType Content {get { return ContentType.MODELS; }}
        //IOpsCommand
        public object ParseArguments(OpsParsedStatement statement)
        {
            return new OptimizeArguments(statement);
        }

        //IOpsCommand
        public void Run(OpsContext context, OpsStatement statement)
        {
            OptimizeArguments arguments = statement.Arguments as OptimizeArguments;
            
            OpsConsole.WriteLine( "Optimizing models");

            foreach( OpsModel model in statement.GetContent(context) )
            {
                foreach( OpsMeshContainer meshContainer in model.GetMeshEnumerator() )
                {    
                    int[] adjIn= null;
                    int[] adjOut= null;
                    int[] faceRemap= null;
                    GraphicsStream vertexRemapStream= null;

                    if(arguments.weldType != WeldType.None)
                    {
                        adjIn= meshContainer.GetAdjacency();
                        meshContainer.MeshData.Mesh.WeldVertices(arguments.epsilonFlags, arguments.epsilons, adjIn, out adjOut, out faceRemap, out vertexRemapStream );
                        meshContainer.SetAdjacency(adjOut);
                        meshContainer.RemapSkin(vertexRemapStream);
                    }

                    adjIn= meshContainer.GetAdjacency();
                    Mesh newMesh = meshContainer.MeshData.Mesh.Optimize(arguments.meshFlags, adjIn, out adjOut, out faceRemap, out vertexRemapStream);
                    meshContainer.ReplaceMesh(newMesh);
                    meshContainer.SetAdjacency(adjOut);
                    meshContainer.RemapSkin(vertexRemapStream);
                }
            }
        }
    }




}

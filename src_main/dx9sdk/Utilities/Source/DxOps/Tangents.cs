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



    public class TangentsCommand : IOpsCommand
    {

        public TangentsCommand()
        {}

        //IOpsCommand
        public string Name 
        { 
            get{ return "GenTangentFrames"; } 
        }
        public ContentType Content {get { return ContentType.MODELS; }}
        //IOpsCommand
        public object ParseArguments(OpsParsedStatement statement)
        {
            return new TangentArguments(statement);
        }

        //IOpsCommand
        public void Run(OpsContext context, OpsStatement statement)
        {
            TangentArguments options = statement.Arguments as TangentArguments;

            OpsConsole.WriteLine( "Generating tangent frames" );

            foreach( OpsModel model in statement.GetContent(context) )
            {
                foreach( OpsMeshContainer meshContainer in model.GetMeshEnumerator() )
                {    
                    VertexElement[] VEs = meshContainer.MeshData.Mesh.Declaration;

                    int texIdx;
                    if( false == MeshDeclarationHelper.FindElement(VEs, DeclarationUsage.TextureCoordinate, 0, out texIdx) )
                    {
                        throw new OpsException("Cannot generate tangents without texture coordinates (use command uvatlas)");
                    }

                    int normIdx;
                    if( false == MeshDeclarationHelper.FindElement(VEs, DeclarationUsage.Normal, 0, out normIdx) )
                    {
                        throw new OpsException("Cannot generate tangents without normals (use command GenNormals)");
                    }

                    int tanIdx;
                    if( false == MeshDeclarationHelper.FindElement(VEs, DeclarationUsage.Tangent, 0, out tanIdx) )
                    {
                        throw new OpsException("Cannot generate tangents without a place to store Tangent0 (use command AddVData)");
                    }

                    int binormIdx;
                    if( false == MeshDeclarationHelper.FindElement(VEs, DeclarationUsage.BiNormal, 0, out binormIdx) )
                    {
                        throw new OpsException("Cannot generate tangents without a place to store BiNormal0 (use command AddVData)");
                    }                       
                    bool success = false;
                    try
                    {
                        GraphicsStream vertexRemapStream= null;

            
                        Mesh newMesh = Geometry.ComputeTangentFrame(
                            meshContainer.MeshData.Mesh,
                            (int)DeclarationUsage.TextureCoordinate,
                            options.TexIdx,
                            (int)DeclarationUsage.Tangent,
                            0,                            
                            (int)DeclarationUsage.BiNormal,
                            0,
                            (int)DeclarationUsage.Normal,
                            0,
                            options.Options,
                            meshContainer.GetAdjacencyStream(),
                            options.ThreshPE, 
                            options.ThreshSP, 
                            options.ThreshNE, 
                            out vertexRemapStream);
                    
                        if(newMesh != null)
                        {
                            meshContainer.ReplaceMesh(newMesh);
                            meshContainer.RemapSkin(vertexRemapStream);
                            success = true;
                        }
                        else
                        {
                            success = false;
                            OpsConsole.WriteLine("Failed in 1st attempt to generate tangents on " + meshContainer.FriendlyName(model) );

                        }
                    }
                    catch(Exception)
                    {
                        OpsConsole.WriteLine("Failed in 1st attempt to generate tangents on  " + meshContainer.FriendlyName(model) );
                        //eating exception
                    }

                    if(!success)
                    {
                        try
                        {                    
                            meshContainer.MeshData.Mesh.ComputeTangent(
                                options.TexIdx, 
                                0, 
                                0, 
                                0, meshContainer.GetAdjacencyStream());
                        }
                        catch(Exception)
                        {
                            OpsConsole.WriteLine( "Failed in final attempt to generate tangents on " + meshContainer.FriendlyName(model) );
                            throw;
                        }
                    }
                }
            }
        }
    }

    namespace Arguments
    {
        public class TangentArguments
        {

            public int TexIdx = 0;
            public TangentOptions Options = 0;
            public float ThreshPE = 0.8f;
            public float ThreshSP = 0.5f;
            public float ThreshNE = 0.8f;
    
            public TangentArguments(OpsParsedStatement statement)
            {
                OpsParsedArgument argTexIdx= statement.FindArgument( "TexIdx");
                if(argTexIdx != null)
                    TexIdx = int.Parse(argTexIdx.Value);

                OpsParsedArgument argOrtho= statement.FindArgument( "Ortho");
                if(argOrtho != null)
                {
                    if( 0 == String.Compare ( argOrtho.Value, "None", true ) )
                        Options |= TangentOptions.DontOrthogonalize;
                    else if( 0 == String.Compare ( argOrtho.Value, "U", true ) )
                        Options |= TangentOptions.OrthogonalizeFromU;
                    else if( 0 == String.Compare ( argOrtho.Value, "V", true ) )
                        Options |= TangentOptions.OrthogonalizeFromV;
                }

                OpsParsedArgument argNormalizePartials= statement.FindArgument( "NormalizePartials");
                if(argNormalizePartials != null)
                {
                    if ( 0 == int.Parse(argNormalizePartials.Value) )
                        Options |= TangentOptions.DontNormalizePartials;
                }

                OpsParsedArgument argWeight= statement.FindArgument( "Weight");
                if(argWeight != null)
                {
                    if( 0 == String.Compare ( argWeight.Value, "Area", true ) )
                        Options |= TangentOptions.WeightByArea;
                    else if( 0 == String.Compare ( argWeight.Value, "Equal", true ) )
                        Options |= TangentOptions.WeightEqual;
                }

                OpsParsedArgument argWarp= statement.FindArgument( "Wrap");
                if(argWarp != null)
                {
                    if( 0 == String.Compare ( argWarp.Value, "U", true ) )
                        Options |= TangentOptions.WrapU;
                    else if( 0 == String.Compare ( argWarp.Value, "V", true ) )
                        Options |= TangentOptions.WrapV;
                    else if( 0 == String.Compare ( argWarp.Value, "UV", true ) )
                        Options |= TangentOptions.WrapUV;
                }

                OpsParsedArgument argThreshPE= statement.FindArgument( "ThreshPE");
                if(argThreshPE != null)
                    ThreshPE = float.Parse(argThreshPE.Value);

                OpsParsedArgument argThreshSP= statement.FindArgument( "ThreshSP");
                if(argThreshSP != null)
                    ThreshSP = float.Parse(argThreshSP.Value);

                OpsParsedArgument argThreshNE= statement.FindArgument( "ThreshNE");
                if(argThreshNE != null)
                    ThreshNE = float.Parse(argThreshNE.Value);

            }
        }
    }
}
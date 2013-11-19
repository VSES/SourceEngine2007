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
        public class ShadeArgs
        {
            public string Shader = "main";
            public string File = null;
            public ArrayList Constants = new ArrayList();

            public ShadeArgs( OpsParsedStatement statement)
            {
                foreach(OpsParsedArgument arg in statement.Args)
                {
                    if(string.Compare(arg.Name, "Shader", true) == 0)
                    {
                        Shader = arg.Value;
                    }
                    else if(string.Compare(arg.Name, "filename", true) == 0)
                    {
                        File = arg.Value;
                    }
                    else if(string.Compare(arg.Name, "", true) == 0)
                    {
                        File = arg.Value;
                    }
                    else if(string.Compare(arg.Name, "Src", true) == 0)
                    {}//do nothing...this is just to make sure we don't make it a parameter for the psh
                    else
                    {
                        Constants.Add(arg);
                    }
                }

                if( File == null )
                    throw OpsException.ArgRequired("filename");

            }
        }
    }

    public class ShadeCommand : IOpsCommand
    {
        //IOpsCommand
        public string Name { get{ return "shade";} }
        public ContentType Content {get { return ContentType.TEXTURES; }}

        public object ParseArguments(OpsParsedStatement statement)
        {
            return  new ShadeArgs(statement);
        }

        public void Run(OpsContext context, OpsStatement statement)
        {
            ShadeArgs args = statement.Arguments as ShadeArgs;

            ConstantTable constantTable = SetupDevice(context, args);

            EffectHandle hTarget = constantTable.GetConstant(null, "Target");


            ArrayList containers = statement.GetContent(context);

            OpsConsole.WriteLine( "Shading textures with \"{0}\"", args.File);
            
            foreach(OpsTexture container in containers)
            {    
                if(hTarget != null)
                {
                    context.Device.SetTexture( 
                        constantTable.GetSamplerIndex(hTarget),
                        container.Texture);
                }


                if(container.Texture is Texture)
                {                
                    Texture oldTexture = container.Texture as Texture;
                    Texture newTexture = OpsTextureHelper.CloneTexture(oldTexture, Usage.None, Pool.Managed);

                    for( int mip = 0; mip < oldTexture.LevelCount; mip++)
                    {
                        SurfaceDescription sd = oldTexture.GetLevelDescription(mip);

                        CheckFormatValid(sd.Format);

                        Surface rt = context.Device.CreateRenderTarget(sd.Width, sd.Height, sd.Format, MultiSampleType.None, 0, true);
        
                        context.Device.SetRenderTarget(0, rt);

                        
                        ShadeVertex[] vb = new ShadeVertex[]
                                        {
                                            ShadeVertex.ForTexture(-1.0f, -1.0f, sd.Width, sd.Height),
                                            ShadeVertex.ForTexture( 1.0f, -1.0f, sd.Width, sd.Height),
                                            ShadeVertex.ForTexture(-1.0f,  1.0f, sd.Width, sd.Height),
                                            ShadeVertex.ForTexture( 1.0f,  1.0f, sd.Width, sd.Height),
                                        }; 

                        context.Device.BeginScene();
                        context.Device.DrawUserPrimitives(PrimitiveType.TriangleStrip, 2, vb);
                        context.Device.EndScene();

                        context.Device.SetRenderTarget(0, context.Device.GetBackBuffer(0, 0, BackBufferType.Mono) );

                        SurfaceLoader.FromSurface(newTexture.GetSurfaceLevel(mip), rt, Filter.None| (container.SRGB?Filter.SrgbOut:0), 0);
                    }
                
                    oldTexture.Dispose();
                    container.Texture = newTexture;
                }
                else if(container.Texture is VolumeTexture)
                {            
                    VolumeTexture oldTexture = container.Texture as VolumeTexture;
                    VolumeTexture newTexture = OpsTextureHelper.CloneVolume(oldTexture, Usage.None, Pool.Managed);

                    for( int mip = 0; mip < oldTexture.LevelCount; mip++)
                    {
                        VolumeDescription vd = oldTexture.GetLevelDescription(mip);

                        CheckFormatValid(vd.Format);

                        Surface sliceRT = context.Device.CreateRenderTarget(vd.Width, vd.Height, vd.Format, MultiSampleType.None, 0, true);

                        for(int slice = 0; slice < vd.Depth; slice++)
                        {
                            context.Device.SetRenderTarget(0, sliceRT);
                        
                            ShadeVertex[] vb = new ShadeVertex[]
                                            {
                                                ShadeVertex.ForVolume(-1.0f, -1.0f, slice, vd.Width, vd.Height, vd.Depth),
                                                ShadeVertex.ForVolume( 1.0f, -1.0f, slice, vd.Width, vd.Height, vd.Depth),
                                                ShadeVertex.ForVolume(-1.0f,  1.0f, slice, vd.Width, vd.Height, vd.Depth),
                                                ShadeVertex.ForVolume( 1.0f,  1.0f, slice, vd.Width, vd.Height, vd.Depth),
                                            }; 

                            context.Device.BeginScene();
                            context.Device.DrawUserPrimitives(PrimitiveType.TriangleStrip, 2, vb);
                            context.Device.EndScene();
                        
                            context.Device.SetRenderTarget(0, context.Device.GetBackBuffer(0, 0, BackBufferType.Mono) );

                            OpsTextureHelper.LoadVolumeSliceFromSurface(newTexture, mip, slice, Filter.None| (container.SRGB?Filter.SrgbOut:0), sliceRT);

                        }

                        sliceRT.Dispose();
                    }

                    oldTexture.Dispose();
                    container.Texture = newTexture;
                }
                else if(container.Texture is CubeTexture)
                {
                    CubeTexture oldTexture = container.Texture as CubeTexture;
                    CubeTexture newTexture = OpsTextureHelper.CloneCube(oldTexture, Usage.None, Pool.Managed);

                    for( int mip = 0; mip < oldTexture.LevelCount; mip++)
                    {
                        SurfaceDescription sd = oldTexture.GetLevelDescription(mip);

                        CheckFormatValid(sd.Format);
                        
                        Surface rt = context.Device.CreateRenderTarget( sd.Width, sd.Height, sd.Format, MultiSampleType.None, 0, true );
        
                        RenderCubeMapFace( context, newTexture, rt, CubeMapFace.PositiveX, mip, sd.Width, container.SRGB );
                        RenderCubeMapFace( context, newTexture, rt, CubeMapFace.PositiveY, mip, sd.Width, container.SRGB);
                        RenderCubeMapFace( context, newTexture, rt, CubeMapFace.PositiveZ, mip, sd.Width, container.SRGB);
                        RenderCubeMapFace( context, newTexture, rt, CubeMapFace.NegativeX, mip, sd.Width, container.SRGB);
                        RenderCubeMapFace( context, newTexture, rt, CubeMapFace.NegativeY, mip, sd.Width, container.SRGB);
                        RenderCubeMapFace( context, newTexture, rt, CubeMapFace.NegativeZ, mip, sd.Width, container.SRGB);
                    }

                    oldTexture.Dispose();

                    container.Texture = newTexture;                
                }
            }
        }

        static void CheckFormatValid( Format format)
        {
            switch(format)
            {
                case Format.Dxt1: 
                case Format.Dxt2: 
                case Format.Dxt3: 
                case Format.Dxt4: 
                case Format.Dxt5:
                    throw new OpsException(String.Format("Shade cannot run on the format:{0}.  Try reformat to a non-DXT format.", format.ToString()));
            };
        }


        private void RenderCubeMapFace(OpsContext context, CubeTexture newTexture, Surface rt, CubeMapFace face, int mip, int size, bool SRGB)
        {
            context.Device.SetRenderTarget(0, rt);
                        
            ShadeVertex[] vb = new ShadeVertex[]
                                {
                                    ShadeVertex.ForCube(-1.0f, -1.0f, size, face),
                                    ShadeVertex.ForCube( 1.0f, -1.0f, size, face),
                                    ShadeVertex.ForCube(-1.0f,  1.0f, size, face),
                                    ShadeVertex.ForCube( 1.0f,  1.0f, size, face),
                                };

            context.Device.BeginScene();
            context.Device.DrawUserPrimitives(PrimitiveType.TriangleStrip, 2, vb);
            context.Device.EndScene();

            context.Device.SetRenderTarget(0, context.Device.GetBackBuffer(0, 0, BackBufferType.Mono) );

            SurfaceLoader.FromSurface(newTexture.GetCubeMapSurface(face, mip), rt, Filter.None| (SRGB?Filter.SrgbOut:0), 0);
        }

        private ConstantTable SetupDevice(OpsContext context, ShadeArgs args)
        {
            string errStr = null;
            ConstantTable constantTable;
            GraphicsStream pshader;
            GraphicsStream vshader;

            try
            {
                ConstantTable dummyTable;
                errStr = null;
                vshader = ShaderLoader.CompileShader(ShadeVertex.VertexShader, "VertexShader", null, null, "vs_3_0", ShaderFlags.None, out errStr, out dummyTable);
            }
            finally
            {
                if (errStr != null && errStr.Length > 0)
                {
                    OpsConsole.WriteLine("Vertex Shader Compiler messages: " + errStr);
                    OpsConsole.WriteLine("If this message is regarding your entry point, it may be something other than the default 'main'.  Please use the argument 'Shader' to specify it.");
                }
            }

         
            try
            {    
                Macro dxopsMacro = new Macro();
                dxopsMacro.Name = "__DXOPS";
                dxopsMacro.Definition = "1";
                errStr = null;            
                pshader = ShaderLoader.CompileShaderFromFile(args.File, args.Shader, new Macro[]{ dxopsMacro } , null, "ps_3_0", ShaderFlags.None, out errStr, out constantTable);
            }
            finally
            {
                if (errStr != null && errStr.Length > 0)
                {
                    OpsConsole.WriteLine("Pixel Shader Compiler messages: " + errStr);
                    OpsConsole.WriteLine("If this message is regarding your entry point, it may be something other than the default 'main'.  Please use the argument 'Shader' to specify it." );
                }
            }

            context.Device.SetRenderState(RenderStates.CullMode, (int)Cull.None);
            context.Device.SetRenderState(RenderStates.FillMode, (int)FillMode.Solid);
            context.Device.SetRenderState(RenderStates.AlphaTestEnable, false);
            context.Device.SetRenderState(RenderStates.AlphaBlendEnable, false);
            context.Device.SetRenderState(RenderStates.StencilEnable, false);
            context.Device.SetRenderState(RenderStates.ZEnable, false);
            context.Device.SetRenderState(RenderStates.ZBufferWriteEnable, false);

            context.Device.DepthStencilSurface = null;

            VertexDeclaration decl = new VertexDeclaration(context.Device, ShadeVertex.VertexDeclaration);
            context.Device.VertexDeclaration = decl;
            
            VertexShader vs = new VertexShader(context.Device, vshader);
            context.Device.VertexShader = vs;
            
            PixelShader ps = new PixelShader(context.Device, pshader);
            context.Device.PixelShader = ps;



            constantTable.SetDefaults(context.Device);

            foreach(OpsParsedArgument constant in args.Constants)
            {
                EffectHandle h = constantTable.GetConstant(null, constant.Name);
                if (h == null)
                {
                    OpsConsole.WriteLine( "WARNING: Parameter '{0}' was not found in shader.", constant.Name);
                    continue;
                }

                ConstantDescription[] cds = constantTable.GetConstantDescription(h, 1);
                ConstantDescription cd = cds[0];
                switch (cd.Class)
                {
                    case ParameterClass.Object:
                    {//texture
                        switch (cd.ParameterType)
                        {
                            case ParameterType.Texture:
                            case ParameterType.Texture1D:
                            case ParameterType.Texture2D:
                            case ParameterType.Texture3D:
                            case ParameterType.TextureCube:
                            {
                                OpsTexture container = context.GetTexture(constant.Value);

                                int sampler = constantTable.GetSamplerIndex(h);
                                context.Device.SetTexture(sampler, container.Texture);
                            }
                                break;
                        }
                        break;
                    }
                    case ParameterClass.Scalar:
                    case ParameterClass.Vector:
                    case ParameterClass.MatrixColumns:
                    case ParameterClass.MatrixRows:
                    {
                        ArrayList floatList = new ArrayList();
                        string[] floatStrings = constant.Value.Split(new char[] { ' ', ',' });
                        foreach (string floatStr in floatStrings)
                        {
                            if (floatStr.Length > 0)
                            {
                                floatList.Add(float.Parse(floatStr));
                            }
                        }
                        float[] floats = (float[])floatList.ToArray(typeof(float));

                        constantTable.SetValue(context.Device, h, floats);
                    }
                        break;
                }
            }

            return constantTable;
        }
    }


    public struct ShadeVertex
    {
        public static VertexElement[] VertexDeclaration = new VertexElement[]
        {
            new VertexElement((short)0, (short)0, DeclarationType.Float2, DeclarationMethod.Default,DeclarationUsage.Position, (byte)0),
            new VertexElement((short)0, (short)8, DeclarationType.Float3, DeclarationMethod.Default,DeclarationUsage.TextureCoordinate, (byte)0),
            new VertexElement((short)0, (short)20, DeclarationType.Float3, DeclarationMethod.Default,DeclarationUsage.TextureCoordinate, (byte)1),
            VertexElement.VertexDeclarationEnd
        };


        public static string VertexShader =
            "void VertexShader( " +
            "  in float2 p : POSITION" + // full screen quad
            ", in float3 t0 : TEXCOORD0" + //texture coordinate
            ", in float3 ts : TEXCOORD1" + //texture size
            ", out float4 outP : POSITION" +
            ", out float3 outT0 : TEXCOORD0" +
            ", out float3 outT1 : TEXCOORD1" +
            " )" +
            "{ " + 
            " outP = float4( p, 0.0f, 1.0f); " +
            " outT0 = t0; " +
            " outT1 = ts; " +
            " }";        

        //Position
        public float PtX;
        public float PtY;
        //Texel Coord
        public float TcX;
        public float TcY;
        public float TcZ;
        //Texel Size
        public float TzX;
        public float TzY;
        public float TzZ;

        public static ShadeVertex ForTexture( 
            float ptX, float ptY,
            int width, int height)//texel size
        {
            ShadeVertex v = new ShadeVertex();

            float tzX = 1.0f/(float)width;
            float tzY = 1.0f/(float)height;

            v.PtX=ptX - tzX*0.5f;
            v.PtY=ptY + tzY*0.5f;

            v.TcX=(1.0f+ptX)*0.5f;
            v.TcY=(1.0f+ptY)*0.5f;
            v.TcZ=0;

            v.TzX=tzX;
            v.TzY=tzY;
            v.TzZ=0;

            return v;
        }

        public static ShadeVertex ForVolume( 
            float ptX, float ptY, 
            int slice, 
            int width, int height, int depth)//texel size
        {
            ShadeVertex v = new ShadeVertex();

            float tzX = 1.0f/(float)width;
            float tzY = 1.0f/(float)height;
            float tzZ = 1.0f/(float)depth;

            v.PtX=ptX - tzX*0.5f;
            v.PtY=ptY + tzY*0.5f;

            v.TcX=(1.0f+ptX)*0.5f;
            v.TcY=(1.0f+ptY)*0.5f;
            v.TcZ= (tzZ * slice) + (tzZ + 0.5f);

            v.TzX=tzX;
            v.TzY=tzY;
            v.TzZ=tzZ;

            return v;
        }

        public static ShadeVertex ForCube( 
            float ptX, float ptY,  
            int surfaceSize, 
            CubeMapFace face)
        {
            ShadeVertex v = new ShadeVertex();

            float pixelSize = 2.0f / (float)surfaceSize;

            v.PtX=ptX - pixelSize*0.5f;
            v.PtY=ptY + pixelSize*0.5f;


            float nX = ptX; 
            float nY = ptY; 

            switch( face )
            {
                case CubeMapFace.PositiveX:
                    v.TzX = 0;
                    v.TzY = pixelSize;
                    v.TzZ = pixelSize;
                    v.TcX =  1.0f;
                    v.TcY =  nY;
                    v.TcZ = -nX;
                    break;
                case CubeMapFace.NegativeX:
                    v.TzX = 0;
                    v.TzY = pixelSize;
                    v.TzZ = pixelSize;
                    v.TcX = -1.0f;
                    v.TcY =  nY;
                    v.TcZ =  nX;
                    break;
                case CubeMapFace.PositiveY:
                    v.TzX = pixelSize;
                    v.TzY = 0;
                    v.TzZ = pixelSize;
                    v.TcX =  nX;
                    v.TcY =  1.0f;
                    v.TcZ = -nY;
                    break;
                case CubeMapFace.NegativeY:
                    v.TzX = pixelSize;
                    v.TzY = 0;
                    v.TzZ = pixelSize;
                    v.TcX =  nX;
                    v.TcY = -1.0f;
                    v.TcZ =  nY;
                    break;
                case CubeMapFace.PositiveZ:
                    v.TzX = pixelSize;
                    v.TzY = pixelSize;
                    v.TzZ = 0;
                    v.TcX =  nX;
                    v.TcY =  nY;
                    v.TcZ =  1.0f;
                    break;
                case CubeMapFace.NegativeZ:
                    v.TzX = pixelSize;
                    v.TzY = pixelSize;
                    v.TzZ = 0;
                    v.TcX =  -nX;
                    v.TcY =  nY;
                    v.TcZ = -1.0f;
                    break;
                default:
                    throw new OpsException("Unknown Face Type");
            }

            return v;
        }


    }

}
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
        public class Slice2dArgs
        {
            public string Dst = null;
            public int Volume = 0;
            public CubeMapFace Face = CubeMapFace.PositiveX;
            public int Mips = 0;

            public Slice2dArgs( OpsParsedStatement statement)
            {
                OpsParsedArgument nameArg = statement.FindArgument("dst");
                if( nameArg == null )
                {
                    nameArg = statement.FindArgument("");
                    if( nameArg == null )
                        throw OpsException.ArgRequired("dst");
                }
                Dst = nameArg.Value;


                OpsParsedArgument volumeArg = statement.FindArgument("Volume");
                if( volumeArg != null )
                    Volume = int.Parse( volumeArg.Value );

                OpsParsedArgument faceArg = statement.FindArgument("Face");
                if( faceArg != null )
                {
                    if(0==string.Compare( faceArg.Value, "+x", true)) 
                        Face = CubeMapFace.PositiveX;
                    else if(0==string.Compare( faceArg.Value, "+y", true)) 
                        Face = CubeMapFace.PositiveY;
                    else if(0==string.Compare( faceArg.Value, "+z", true)) 
                        Face = CubeMapFace.PositiveZ;
                    else if(0==string.Compare( faceArg.Value, "-x", true)) 
                        Face = CubeMapFace.NegativeX;
                    else if(0==string.Compare( faceArg.Value, "-y", true)) 
                        Face = CubeMapFace.NegativeY;
                    else if(0==string.Compare( faceArg.Value, "-z", true)) 
                        Face = CubeMapFace.NegativeZ;
                    else
                        throw new OpsException("Face Argument was not recognized.  must be '[+-][xyz]'");
                }

                OpsParsedArgument mipsArg = statement.FindArgument("mipmap");
                if( mipsArg != null )
                    Mips = int.Parse( mipsArg.Value );
            }
        }
    }

    public class Slice2dCommand : IOpsCommand
    {
        //IOpsCommand
        public string Name { get{ return "Slice2d";} }
        public ContentType Content {get { return ContentType.TEXTURES; }}

        public object ParseArguments(OpsParsedStatement statement)
        {
            return  new Slice2dArgs(statement);
        }

        public void Run(OpsContext context, OpsStatement statement)
        {
            Slice2dArgs args = statement.Arguments as Slice2dArgs;
            
            ArrayList containers = statement.GetContent(context);
            if(containers.Count !=1 )
                throw new OpsException( "Src argument does not resolve to 1 texture.  Textures found: " + containers.Count );

            OpsTexture container = containers[0] as OpsTexture;

            OpsConsole.WriteLine( "Slicing from texture:{0} and saving as {1}", container.Name, args.Dst );

            OpsTexture result = new OpsTexture();
            result.Name = args.Dst;
            result.SRGB = container.SRGB;

            Texture newTexture = null;
            if(container.Texture is Texture)
            {
                Texture srcTexture = container.Texture as Texture;
                SurfaceDescription sd = srcTexture.GetLevelDescription(args.Mips);

                newTexture = new Texture(srcTexture.Device, sd.Width, sd.Height, 1, Usage.None, sd.Format, Pool.Managed);
                SurfaceLoader.FromSurface(newTexture.GetSurfaceLevel(0), srcTexture.GetSurfaceLevel(0), Filter.Dither|Filter.Triangle| (container.SRGB?Filter.Srgb:0), 0); 
            }
            else if(container.Texture is VolumeTexture)
            {
                VolumeTexture srcTexture = container.Texture as VolumeTexture;
                VolumeDescription vd = srcTexture.GetLevelDescription(args.Mips);

                newTexture = new Texture(srcTexture.Device, vd.Width, vd.Height, 1, Usage.None, vd.Format, Pool.Managed);
                OpsTextureHelper.LoadSurfaceFromVolumeSlice(srcTexture, args.Mips, args.Volume, Filter.Dither|Filter.Triangle| (container.SRGB?Filter.Srgb:0), newTexture.GetSurfaceLevel(0) );
            }
            else if(container.Texture is CubeTexture)
            {
                CubeTexture srcTexture = container.Texture as CubeTexture;
                SurfaceDescription sd = srcTexture.GetLevelDescription(args.Mips);

                newTexture = new Texture(srcTexture.Device, sd.Width, sd.Height, 1, Usage.None, sd.Format, Pool.Managed);
                SurfaceLoader.FromSurface(newTexture.GetSurfaceLevel(0), srcTexture.GetCubeMapSurface(args.Face,0), Filter.Dither|Filter.Triangle| (container.SRGB?Filter.Srgb:0), 0);
            }

            result.Texture = newTexture;

            context.AddTexture(result);
        }
    }
}
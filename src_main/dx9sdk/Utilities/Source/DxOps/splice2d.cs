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
        public class Splice2dArgs
        {
            public string Dst = null;
            public int Volume = 0;
            public CubeMapFace Face = CubeMapFace.PositiveX;
            public int Mips = 0;

            public Splice2dArgs( OpsParsedStatement statement)
            {
                OpsParsedArgument dstArg = statement.FindArgument("dst");
                if( dstArg == null )
                {
                     dstArg = statement.FindArgument("");
                    if( dstArg == null )
                        throw OpsException.ArgRequired("dst");
                }
                Dst = dstArg.Value;

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

    public class Splice2dCommand : IOpsCommand
    {
        //IOpsCommand
        public string Name { get{ return "Splice2d";} }
        public ContentType Content {get { return ContentType.TEXTURES; }}

        public object ParseArguments(OpsParsedStatement statement)
        {
            return  new Splice2dArgs(statement);
        }

        public void Run(OpsContext context, OpsStatement statement)
        {
            Splice2dArgs args = statement.Arguments as Splice2dArgs;
            
            ArrayList srcList = statement.GetContent(context);
            if(srcList.Count != 1)
                throw new OpsException("Could not find the source texture.  1 is require but found " + srcList.Count);

            OpsTexture srcContainer = ((OpsTexture)srcList[0]);

            if(!(srcContainer.Texture is Texture))
                throw new OpsException("Source texture is not 2D");

            Texture srcTexture = srcContainer.Texture as Texture;

            OpsConsole.WriteLine( "Splicing texture:{0} into one or more textures", srcContainer.Name );


            ArrayList dstContainers = context.FindTextures(args.Dst);

            foreach( OpsTexture dstContainer in dstContainers)
            {
                if(dstContainer.Texture is Texture)
                {
                    Texture dstTexture = dstContainer.Texture as Texture;

                    SurfaceLoader.FromSurface(dstTexture.GetSurfaceLevel(args.Mips), srcTexture.GetSurfaceLevel(0), Filter.Dither|Filter.Triangle| (srcContainer.SRGB?Filter.SrgbIn:0)| (dstContainer.SRGB?Filter.SrgbOut:0), 0); 
                }
                else if(dstContainer.Texture is VolumeTexture)
                {
                    VolumeTexture dstTexture = dstContainer.Texture as VolumeTexture;

                    OpsTextureHelper.LoadVolumeSliceFromSurface(dstTexture, args.Mips, args.Volume, Filter.Dither|Filter.Triangle| (srcContainer.SRGB?Filter.SrgbIn:0)| (dstContainer.SRGB?Filter.SrgbOut:0), srcTexture.GetSurfaceLevel(0) );

                }
                else if(dstContainer.Texture is CubeTexture)
                {
                    CubeTexture dstTexture = dstContainer.Texture as CubeTexture;

                    SurfaceLoader.FromSurface(dstTexture.GetCubeMapSurface(args.Face, args.Mips), srcTexture.GetSurfaceLevel(0), Filter.Dither|Filter.Triangle| (srcContainer.SRGB?Filter.SrgbIn:0)| (dstContainer.SRGB?Filter.SrgbOut:0), 0);
                }
            }
        }
    }
}
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
        public class TexFormatArgs
        {
            public Format Format = 0;

            public TexFormatArgs( OpsParsedStatement statement)
            {
                OpsParsedArgument formatArg = statement.FindArgument("format");
                if( formatArg == null )
                {
                    formatArg = statement.FindArgument("");
                    if( formatArg == null )
                        throw OpsException.ArgRequired("format");
                }
                this.Format = OpsFormatHelper.FindByFormat(formatArg.Value).Format;
            }
        }
    }


    public class TexFormatCommand : IOpsCommand
    {
        //IOpsCommand
        public string Name { get{ return "reformat";} }
        public ContentType Content {get { return ContentType.TEXTURES; }}

        public object ParseArguments(OpsParsedStatement statement)
        {
            return  new TexFormatArgs(statement);
        }

        public void Run(OpsContext context, OpsStatement statement)
        {
            TexFormatArgs args = statement.Arguments as TexFormatArgs;

            ArrayList containers = statement.GetContent(context);

            OpsConsole.WriteLine( "Changing texture formats to " + args.Format );

            foreach(OpsTexture container in containers)
            {    
                if(container.Texture is Texture)
                {
                    Texture oldTexture = container.Texture as Texture;
                    SurfaceDescription sd = oldTexture.GetLevelDescription(0);

                    container.Texture = OpsTextureHelper.CloneTexture(oldTexture, sd.Width, sd.Height, oldTexture.LevelCount, args.Format, Usage.None, Filter.Triangle|Filter.Dither| (container.SRGB?Filter.Srgb:0), Pool.Managed);
                }
                else if(container.Texture is VolumeTexture)
                {
                    VolumeTexture oldTexture = container.Texture as VolumeTexture;
                    VolumeDescription vd = oldTexture.GetLevelDescription(0);

                    container.Texture = OpsTextureHelper.CloneVolume(oldTexture, vd.Width, vd.Height, vd.Depth, oldTexture.LevelCount, args.Format, Usage.None, Filter.Triangle|Filter.Dither| (container.SRGB?Filter.Srgb:0), Pool.Managed );
                }
                else if(container.Texture is CubeTexture)
                {
                    CubeTexture oldTexture = container.Texture as CubeTexture;
                    SurfaceDescription sd = oldTexture.GetLevelDescription(0);

                    container.Texture = OpsTextureHelper.CloneCube(oldTexture, sd.Width, oldTexture.LevelCount, args.Format, Usage.None, Filter.Triangle|Filter.Dither| (container.SRGB?Filter.Srgb:0), Pool.Managed );
                }
            }
        }
    }
}
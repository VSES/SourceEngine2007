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


    public class TextureInfoCommand : IOpsCommand
    {
        //IOpsCommand
        public string Name { get{ return "Textures";} }
        public ContentType Content {get { return ContentType.TEXTURES; }}

        public object ParseArguments(OpsParsedStatement statement)
        {
            return  null;
        }

        private static string format = "{0,-20}{1,13}{2,8}{3,8}{4,8}{5,8}{6,14}";

        public void Run(OpsContext context, OpsStatement statement)
        {
            OpsConsole.WriteLine( "Texture information" );
            OpsConsole.WriteLine(format, "Names", "Type", "Width", "Height", "Depth", "Mips", "Format" );
        
            foreach( OpsTexture container in statement.GetContent(context) )
            {
                string name = container.Name;
                string type = container.Texture.GetType().Name;
                int width = 0;
                int height = 0;
                int depth = 0;                
                int mips = 0;                
                string pixelFormat = "";

                if(container.Texture is Texture)
                {
                    Texture texture = container.Texture as Texture;
                    SurfaceDescription sd = texture.GetLevelDescription(0);
                    width = sd.Width;
                    height = sd.Height;
                    mips = texture.LevelCount;
                    pixelFormat = sd.Format.ToString();
                }
                else if(container.Texture is VolumeTexture)
                {
                    VolumeTexture texture = container.Texture as VolumeTexture;
                    VolumeDescription vd = texture.GetLevelDescription(0);
                    width = vd.Width;
                    height = vd.Height;
                    depth = vd.Depth;
                    mips = texture.LevelCount;
                    pixelFormat = vd.Format.ToString();
                }
                else if(container.Texture is CubeTexture)
                {
                    CubeTexture texture = container.Texture as CubeTexture;
                    SurfaceDescription sd = texture.GetLevelDescription(0);
                    width = sd.Width;
                    height = sd.Height;
                    mips = texture.LevelCount;
                    pixelFormat = sd.Format.ToString();
                }

                OpsConsole.WriteLine( format,
                    name,
                    type,
                    width,
                    height,
                    depth,
                    mips,
                    pixelFormat);
            }
        }
    }

}
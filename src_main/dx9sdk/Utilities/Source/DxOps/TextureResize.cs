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
        public class TexResizeArgs
        {
            public int Width = -1;
            public int Height = -1;
            public int Depth = -1;
            public Filter Filter = Filter.Box; 

            public TexResizeArgs( OpsParsedStatement statement)
            {
                OpsParsedArgument widthArg = statement.FindArgument("Width");
                if( widthArg != null )
                    Width = int.Parse( widthArg.Value );

                OpsParsedArgument heightArg = statement.FindArgument("Height");
                if( heightArg != null )
                    Height = int.Parse( heightArg.Value );

                OpsParsedArgument depthArg = statement.FindArgument("Depth");
                if( depthArg != null )
                    Depth = int.Parse( depthArg.Value );



                OpsParsedArgument filterArg = statement.FindArgument("Filter");
                if( filterArg != null )
                {
                    if(0==string.Compare(filterArg.Value, "None", true) )
                        Filter |= Filter.None;
                    else if(0==string.Compare(filterArg.Value, "Point", true) )
                        Filter |= Filter.Point;
                    else if(0==string.Compare(filterArg.Value, "Linear", true) )
                        Filter |= Filter.Linear;
                    else if(0==string.Compare(filterArg.Value, "Triangle", true) )
                        Filter |= Filter.Triangle;
                    else if(0==string.Compare(filterArg.Value, "Box", true) )
                        Filter |= Filter.Box;
                    else
                        throw new OpsException("Filter Argument is invalid");
                }
                else
                    Filter |= Filter.Box;


                OpsParsedArgument ditherArg = statement.FindArgument("Dither");
                if( ditherArg != null )
                {
                    if(0!=int.Parse(ditherArg.Value) )
                        Filter |= Filter.Dither;
                }
            }
        }
    }

    public class TexResizeCommand : IOpsCommand
    {
        //IOpsCommand
        public string Name { get{ return "resize";} }
        public ContentType Content {get { return ContentType.TEXTURES; }}

        public object ParseArguments(OpsParsedStatement statement)
        {
            return  new TexResizeArgs(statement);
        }

        public void Run(OpsContext context, OpsStatement statement)
        {
            TexResizeArgs args = statement.Arguments as TexResizeArgs;

            ArrayList containers = statement.GetContent(context);
            
            OpsConsole.WriteLine( "Resizing textures to width:{0} height:{1} depth:{2}", args.Width, args.Height, args.Depth );
            
            foreach(OpsTexture container in containers)
            {
                if (container.Texture is Texture)
                {
                    Texture oldTexture = container.Texture as Texture;
                    SurfaceDescription sd = oldTexture.GetLevelDescription(0);

                    if (args.Width == -1)
                        args.Width = sd.Width;
                    if (args.Height == -1)
                        args.Height = sd.Height;

                    int mips = OpsTextureHelper.NumMips(args.Width, args.Height, 1);

                    container.Texture = OpsTextureHelper.CloneTexture(oldTexture, args.Width, args.Height, mips, sd.Format, Usage.None, args.Filter | (container.SRGB ? Filter.Srgb : 0), Pool.Managed);
                }
                else if (container.Texture is VolumeTexture)
                {
                    VolumeTexture oldTexture = container.Texture as VolumeTexture;
                    VolumeDescription vd = oldTexture.GetLevelDescription(0);

                    if (args.Width == -1)
                        args.Width = vd.Width;
                    if (args.Height == -1)
                        args.Height = vd.Height;
                    if (args.Depth == -1)
                        args.Depth = vd.Depth;

                    int mips = OpsTextureHelper.NumMips(args.Width, args.Height, args.Depth);

                    container.Texture = OpsTextureHelper.CloneVolume(oldTexture, args.Width, args.Height, args.Depth, mips, vd.Format, Usage.None, args.Filter | (container.SRGB ? Filter.Srgb : 0), Pool.Managed);
                }
                else if (container.Texture is CubeTexture)
                {
                    CubeTexture oldTexture = container.Texture as CubeTexture;
                    SurfaceDescription sd = oldTexture.GetLevelDescription(0);

                    if (args.Width == -1)
                        args.Width = sd.Width;

                    int mips = OpsTextureHelper.NumMips(args.Width, args.Width, 1);

                    container.Texture = OpsTextureHelper.CloneCube(oldTexture, args.Width, mips, sd.Format, Usage.None, args.Filter | (container.SRGB ? Filter.Srgb : 0), Pool.Managed);
                }
            }
        }
    }
}
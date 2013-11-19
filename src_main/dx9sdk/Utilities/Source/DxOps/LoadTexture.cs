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
    public class LoadBmpCommand : TexLoaderCommand
    {
        public override string Name
        { 
            get{ return "LoadBmp"; } 
        }
    }

    public class LoadDdsCommand : TexLoaderCommand
    {
        public override string Name
        { 
            get{ return "LoadDds"; } 
        }
    }

    public class LoadDibCommand : TexLoaderCommand
    {
        public override string Name
        { 
            get{ return "LoadDib"; } 
        }
    }

    public class LoadHdrCommand : TexLoaderCommand
    {
        public override string Name
        { 
            get{ return "LoadHdr"; } 
        }
    }

    public class LoadJpgCommand : TexLoaderCommand
    {
        public override string Name
        { 
            get{ return "LoadJpg"; } 
        }
    }

    public class LoadPfmCommand : TexLoaderCommand
    {
        public override string Name
        { 
            get{ return "LoadPfm"; } 
        }
    }

    public class LoadPngCommand : TexLoaderCommand
    {
        public override string Name
        { 
            get{ return "LoadPng"; } 
        }
    }

    public class LoadPpmCommand : TexLoaderCommand
    {
        public override string Name
        { 
            get{ return "LoadPpm"; } 
        }
    }

    public class LoadTgaCommand : TexLoaderCommand
    {
        public override string Name
        { 
            get{ return "LoadTga"; } 
        }
    }


    public abstract class TexLoaderCommand : IOpsCommand
    {
        //IOpsCommand
        public abstract string Name
        { 
            get;
        }
        public ContentType Content {get { return ContentType.TEXTURES; }}
        //IOpsCommand
        public object ParseArguments(OpsParsedStatement statement)
        {
            return new TexLoaderArgs(statement);
        }

        //IOpsCommand
        public void Run(OpsContext context, OpsStatement statement)
        {

            TexLoaderArgs texArgs = statement.Arguments as TexLoaderArgs;

            string[] paths = OpsHelpers.ResolvePathToMany( texArgs.Path as string );
            
            foreach(string path in paths )
            {
                OpsConsole.WriteLine("Loading texture from file: \"{0}\"", path );
                
                OpsTexture result = new OpsTexture();

                ImageInformation Info = TextureLoader.ImageInformationFromFile( path );
                result.SRGB = texArgs.SRGB;

                if( Info.ResourceType == ResourceType.Textures )
                {
                    result.Texture = TextureLoader.FromFile(context.Device, path);
                }
                else if( Info.ResourceType == ResourceType.VolumeTexture  )
                {
                    result.Texture = TextureLoader.FromVolumeFile(context.Device, path);
                }
                else if( Info.ResourceType == ResourceType.CubeTexture )
                {
                    result.Texture = TextureLoader.FromCubeFile(context.Device, path);
                }

                result.Name = System.IO.Path.GetFileNameWithoutExtension(path );

                context.AddTexture(result);
            }
        }

    }

    namespace Arguments
    {
        public class TexLoaderArgs
        {
            public string Path = null;
            public bool SRGB = false;

            public TexLoaderArgs(OpsParsedStatement statement)
            {
                OpsParsedArgument pathArg = statement.FindArgument("filename");
                if(pathArg == null)
                {
                    pathArg = statement.FindArgument("");
                    if(pathArg == null)
                        throw OpsException.ArgRequired("filename");
                }
                Path = pathArg.Value;


                OpsParsedArgument srgbArg = statement.FindArgument("srgb");
                if(srgbArg != null)
                {
                    SRGB = bool.Parse( srgbArg.Value );
                }
            }
        }
    }
}
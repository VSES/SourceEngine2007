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
    public class SaveBmpCommand : TexSaverCommand
    {
        public override string Name
        { 
            get{ return "SaveBmp"; } 
        }
    }

    public class SaveDdsCommand : TexSaverCommand
    {
        public override string Name
        { 
            get{ return "SaveDds"; } 
        }
    }

    public class SaveDibCommand : TexSaverCommand
    {
        public override string Name
        { 
            get{ return "SaveDib"; } 
        }
    }

    public class SaveHdrCommand : TexSaverCommand
    {
        public override string Name
        { 
            get{ return "SaveHdr"; } 
        }
    }

    public class SaveJpgCommand : TexSaverCommand
    {
        public override string Name
        { 
            get{ return "SaveJpg"; } 
        }
    }

    public class SavePfmCommand : TexSaverCommand
    {
        public override string Name
        { 
            get{ return "SavePfm"; } 
        }
    }

    public class SavePngCommand : TexSaverCommand
    {
        public override string Name
        { 
            get{ return "SavePng"; } 
        }
    }

    public class SavePpmCommand : TexSaverCommand
    {
        public override string Name
        { 
            get{ return "SavePpm"; } 
        }
    }

    public class SaveTgaCommand : TexSaverCommand
    {
        public override string Name
        { 
            get{ return "SaveTga"; } 
        }
    }

    /// <summary>
    /// Summary description for Save.
    /// </summary>
    public abstract class TexSaverCommand : IOpsCommand
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
            return new TexSaverArgs(statement);
        }

        //IOpsCommand
        public void Run(OpsContext context, OpsStatement statement)
        {
            TexSaverArgs texArgs = statement.Arguments as TexSaverArgs;

            ArrayList containers = statement.GetContent(context);
            if(containers.Count != 1)
                throw new OpsException("'src' argument resolves to "+containers.Count+" textures where it must be 1 texture");


            OpsTexture container = containers[0] as OpsTexture;

            OpsConsole.WriteLine("Saving texture: {0} to path: {1}", container.Name ,texArgs.Path );
            
            TextureLoader.Save(texArgs.Path, texArgs.Format, container.Texture); 
        }
    }

    namespace Arguments
    {

        public class TexSaverArgs
        {
            public string Path;
            public ImageFileFormat Format;

            public TexSaverArgs(OpsParsedStatement statement)
            {
                OpsParsedArgument fileArg = statement.FindArgument("filename");
                if(fileArg == null)
                {
                    fileArg = statement.FindArgument("");
                    if(fileArg == null)
                        throw OpsException.ArgRequired("filename");
                }
                this.Path = fileArg.Value;

                string extension = System.IO.Path.GetExtension(Path);
                extension = extension.TrimStart( new char[]{'.'} );

                this.Format = (ImageFileFormat)Enum.Parse(typeof(ImageFileFormat), extension, true);

            }
        }
    }
}
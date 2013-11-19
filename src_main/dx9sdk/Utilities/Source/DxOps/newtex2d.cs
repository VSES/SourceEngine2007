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
        public class NewTex2dArgs
        {
            public string Name = null;
            public Format Format = 0;
            public int Width = 0;
            public int Height = 0;
            public int Mips = 0;
            public string Src = null;

            public NewTex2dArgs( OpsParsedStatement statement)
            {
                OpsParsedArgument nameArg = statement.FindArgument("dst");
                if( nameArg == null )
                {
                    nameArg = statement.FindArgument("");
                    if( nameArg == null )
                        throw OpsException.ArgRequired("dst");
                }
                Name = nameArg.Value;

                OpsParsedArgument formatArg = statement.FindArgument("Format");
                if( formatArg == null )
                    throw OpsException.ArgRequired("format");
                this.Format = OpsFormatHelper.FindByFormat(formatArg.Value).Format;

                OpsParsedArgument widthArg = statement.FindArgument("Width");
                if( widthArg == null )
                    throw OpsException.ArgRequired("width");
                Width = int.Parse( widthArg.Value );

                OpsParsedArgument heightArg = statement.FindArgument("Height");
                if( heightArg == null )
                    throw OpsException.ArgRequired("height");
                Height = int.Parse( heightArg.Value );

                if(Width > 4096 || Height > 4096)
                    throw new OpsException("Dimensions must not exceed 4k");


                OpsParsedArgument mipsArg = statement.FindArgument("Mips");
                if( mipsArg != null )
                    Mips = int.Parse( mipsArg.Value );

                OpsParsedArgument srcArg = statement.FindArgument("Src");
                Src = ( srcArg == null ) ? null : srcArg.Value;

            }
        }
    }

    public class NewTex2dCommand : IOpsCommand
    {
        //IOpsCommand
        public string Name { get{ return "NewTex2d";} }
        public ContentType Content {get { return ContentType.TEXTURES; }}

        public object ParseArguments(OpsParsedStatement statement)
        {
            return  new NewTex2dArgs(statement);
        }

        public void Run(OpsContext context, OpsStatement statement)
        {
            NewTex2dArgs args = statement.Arguments as NewTex2dArgs;

            OpsConsole.WriteLine( "Creating a new texture named " + args.Name);

            OpsTexture result = new OpsTexture();
            result.Name = args.Name;
            Texture newTexture =  new Texture( context.Device, args.Width , args.Height , args.Mips , Usage.None , args.Format , Pool.Managed );
            result.Texture = newTexture;

            CopyToTexture(context, newTexture, args.Src, Filter.Triangle|Filter.Dither);

            context.AddTexture(result);
        }

        public static void CopyToTexture(OpsContext context, Texture newTexture, string srcArg, Filter filter)
        {
            if(srcArg == null || srcArg.Length == 0)
                return;
            OpsTexture src = context.GetTexture(srcArg);
                
            if(src == null)
                throw new OpsException("Could not find source texture: "+srcArg );

            if(!(src.Texture is Texture))
                throw new OpsException("Source texture is not a texture2D: "+srcArg);

            Texture srcTex = src.Texture as Texture;

            SurfaceLoader.FromSurface(newTexture.GetSurfaceLevel(0), srcTex.GetSurfaceLevel(0), filter| (src.SRGB?Filter.SrgbIn:0), 0); 

            newTexture.GenerateMipSubLevels();
        }

    }

}
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
        public class NewTex3dArgs
        {
            public string Name = null;
            public Format Format = 0;
            public int Width = 0;
            public int Height = 0;
            public int Depth = 0;
            public int Mips = 0;
            public string Src = null;

            public NewTex3dArgs( OpsParsedStatement statement)
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

                OpsParsedArgument depthArg = statement.FindArgument("Depth");
                if( depthArg == null )
                    throw OpsException.ArgRequired("depth");
                Depth = int.Parse( depthArg.Value );

                if(Width > 4096 || Height > 4096 || Depth > 4096)
                    throw new OpsException("Dimensions must not exceed 4k");

                OpsParsedArgument mipsArg = statement.FindArgument("Mips");
                if( mipsArg != null )
                    Mips = int.Parse( mipsArg.Value );

                OpsParsedArgument srcArg = statement.FindArgument("Src");
                Src = ( srcArg == null ) ? null : srcArg.Value;

            }
        }
    }

    public class NewTex3dCommand : IOpsCommand
    {
        //IOpsCommand
        public string Name { get{ return "NewTex3d";} }
        public ContentType Content {get { return ContentType.TEXTURES; }}

        public object ParseArguments(OpsParsedStatement statement)
        {
            return  new NewTex3dArgs(statement);
        }

        public void Run(OpsContext context, OpsStatement statement)
        {
            NewTex3dArgs args = statement.Arguments as NewTex3dArgs;

            OpsConsole.WriteLine( "Creating new volume texture named " + args.Name);

            OpsTexture result = new OpsTexture();
            result.Name = args.Name;
            VolumeTexture newTexture =  new VolumeTexture( context.Device, args.Width , args.Height , args.Depth , args.Mips , Usage.None , args.Format , Pool.Managed );
            result.Texture = newTexture;

            CopyToVolume(context, newTexture, args.Src, Filter.Triangle|Filter.Dither);

            context.AddTexture(result);
        }

        public static void CopyToVolume(OpsContext context, VolumeTexture newTexture, string srcArg, Filter filter )
        {
            if(srcArg == null || srcArg.Length == 0)
                return;

            ArrayList srcList = context.FindTextures(srcArg);
                
            if(srcList == null || srcList.Count == 0)
                throw new OpsException("Could not find source texture: "+srcArg );
            
            if(((OpsTexture)srcList[0]).Texture is VolumeTexture )
            {
                VolumeLoader.FromVolume(newTexture.GetVolumeLevel(0), ((VolumeTexture)((OpsTexture)srcList[0]).Texture).GetVolumeLevel(0), filter, 0);    
            }
            else
            {
                for( int i = 0; i < srcList.Count; i++)
                {
                    OpsTexture src = srcList[i] as OpsTexture;

                    if(!(src.Texture is Texture))
                        throw new OpsException("Source texture pattern is not 1 volume texture or all 2D textures: "+srcArg);

                    OpsTextureHelper.LoadVolumeSliceFromSurface(newTexture, 0, i, filter, ((Texture)src.Texture).GetSurfaceLevel(0) );
                }
            }

            newTexture.GenerateMipSubLevels();
        }


    }

}
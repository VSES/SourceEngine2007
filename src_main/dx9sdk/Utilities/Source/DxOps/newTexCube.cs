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
        public class NewTexCubeArgs
        {
            public string Name = null;
            public Format Format = 0;
            public int Size = 0;
            public int Mips = 0;
            public string SrcXP = null;
            public string SrcXM = null;
            public string SrcYP = null;
            public string SrcYM = null;
            public string SrcZP = null;
            public string SrcZM = null;

            public NewTexCubeArgs( OpsParsedStatement statement)
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

                OpsParsedArgument sizeArg = statement.FindArgument("Size");
                if( sizeArg == null )
                    throw OpsException.ArgRequired("size");
                Size = int.Parse( sizeArg.Value );

                if(Size > 4096)
                    throw new OpsException("Dimensions must not exceed 4k");

                OpsParsedArgument mipsArg = statement.FindArgument("Mips");
                if( mipsArg != null )
                    Mips = int.Parse( mipsArg.Value );

                OpsParsedArgument srcxpArg = statement.FindArgument("SrcXP");
                SrcXP = ( srcxpArg == null ) ? null : srcxpArg.Value;

                OpsParsedArgument srcxmArg = statement.FindArgument("SrcXM");
                SrcXM = ( srcxmArg == null ) ? null : srcxmArg.Value;

                OpsParsedArgument srcypArg = statement.FindArgument("SrcYP");
                SrcYP = ( srcypArg == null ) ? null : srcypArg.Value;

                OpsParsedArgument srcymArg = statement.FindArgument("SrcYM");
                SrcYM = ( srcymArg == null ) ? null : srcymArg.Value;

                OpsParsedArgument srczpArg = statement.FindArgument("SrcZP");
                SrcZP = ( srcypArg == null ) ? null : srczpArg.Value;

                OpsParsedArgument srczmArg = statement.FindArgument("SrcZM");
                SrcZM = ( srczmArg == null ) ? null : srczmArg.Value;
            }
        }
    }

    public class NewTexCubeCommand : IOpsCommand
    {
        //IOpsCommand
        public string Name { get{ return "NewTexCube";} }
        public ContentType Content {get { return ContentType.TEXTURES; }}

        public object ParseArguments(OpsParsedStatement statement)
        {
            return  new NewTexCubeArgs(statement);
        }

        public void Run(OpsContext context, OpsStatement statement)
        {
            NewTexCubeArgs args = statement.Arguments as NewTexCubeArgs;

            OpsConsole.WriteLine( "Creating new cube texture named" + args.Name);

            OpsTexture result = new OpsTexture();
            result.Name = args.Name;
            CubeTexture newTexture =  new CubeTexture( context.Device, args.Size , args.Mips , Usage.None , args.Format , Pool.Managed );
            result.Texture = newTexture;

            CopyToCubeSide( context, newTexture, args.SrcXP, CubeMapFace.PositiveX, Filter.Triangle|Filter.Dither);
            CopyToCubeSide( context, newTexture, args.SrcYP, CubeMapFace.PositiveY, Filter.Triangle|Filter.Dither);
            CopyToCubeSide( context, newTexture, args.SrcZP, CubeMapFace.PositiveZ, Filter.Triangle|Filter.Dither);
            
            CopyToCubeSide( context, newTexture, args.SrcXM, CubeMapFace.NegativeX, Filter.Triangle|Filter.Dither);
            CopyToCubeSide( context, newTexture, args.SrcYM, CubeMapFace.NegativeY, Filter.Triangle|Filter.Dither);
            CopyToCubeSide( context, newTexture, args.SrcZM, CubeMapFace.NegativeZ, Filter.Triangle|Filter.Dither);    

            context.AddTexture(result);
        }

        public static void CopyToCubeSide(OpsContext context, CubeTexture newTexture, string srcArg, CubeMapFace face, Filter filter)
        {
            if(srcArg == null || srcArg.Length == 0)
                return;

            OpsTexture src = context.GetTexture(srcArg);
                
            if(src == null)
                throw new OpsException("Could not find source texture: "+srcArg );

            if(src.Texture is CubeTexture)
            {
                SurfaceLoader.FromSurface(newTexture.GetCubeMapSurface(face, 0), ((CubeTexture)src.Texture).GetCubeMapSurface(face,0), filter| (src.SRGB?Filter.SrgbIn:0), 0);             
            }
            else if (src.Texture is Texture)
            {
                SurfaceLoader.FromSurface(newTexture.GetCubeMapSurface(face, 0), ((Texture)src.Texture).GetSurfaceLevel(0), filter| (src.SRGB?Filter.SrgbIn:0), 0);             
            }
            else
                throw new OpsException("Source texture is not a texture2D: "+srcArg);

        }
    }

}
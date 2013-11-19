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
        public class MipArgs
        {
            public int Start = 0;
            public Filter Filter = 0;

            public MipArgs( OpsParsedStatement statement)
            {
                OpsParsedArgument srcArg = statement.FindArgument("Start");
                if( srcArg != null )
                    Start = int.Parse( srcArg.Value );

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

    public class MipCommand : IOpsCommand
    {
        //IOpsCommand
        public string Name { get{ return "GenMipMaps";} }
        public ContentType Content {get { return ContentType.TEXTURES; }}

        public object ParseArguments(OpsParsedStatement statement)
        {
            return  new MipArgs(statement);
        }

        public void Run(OpsContext context, OpsStatement statement)
        {
            MipArgs args = statement.Arguments as MipArgs;

            ArrayList containers = statement.GetContent(context);
            
            OpsConsole.WriteLine( "Generating Mipmaps");

            foreach(OpsTexture container in containers)
            {
                TextureLoader.FilterTexture( container.Texture, args.Start, args.Filter | (container.SRGB?Filter.Srgb:0));
            }
        }
    }
}
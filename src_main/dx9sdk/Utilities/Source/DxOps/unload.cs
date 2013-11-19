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
using Microsoft.DirectX.Solutions.DxOps;
using Microsoft.DirectX.Solutions.DxOps.Exceptions;
using Microsoft.DirectX.Solutions.DxOps.Commands.Arguments;


namespace Microsoft.DirectX.Solutions.DxOps.Commands
{

    namespace Arguments
    {

        public class UnloadArguments
        {
            public string Src = null;
            public ContentType Type = ContentType.GENERIC;

            public UnloadArguments(OpsParsedStatement statement)
            {
                OpsParsedArgument srcArg = statement.FindArgument("src");
                if( srcArg == null )
                {
                    srcArg = statement.FindArgument("");
                    if( srcArg == null )
                        throw OpsException.ArgRequired("src");
                }
                Src = srcArg.Value;

                OpsParsedArgument filterArg = statement.FindArgument("Type");
                if( filterArg != null )
                {
                    if(0==string.Compare(filterArg.Value, "Texture", true) )
                        Type = ContentType.TEXTURES;
                    else if(0==string.Compare(filterArg.Value, "Textures", true) )
                        Type = ContentType.TEXTURES;
                    else if(0==string.Compare(filterArg.Value, "Model", true) )
                        Type = ContentType.MODELS;
                    else if(0==string.Compare(filterArg.Value, "Models", true) )
                        Type = ContentType.MODELS;
                    else if(0==string.Compare(filterArg.Value, "Both", true) )
                        Type = ContentType.GENERIC;
                    else
                        throw new OpsException("Type Argument is invalid.  Value must be (Textures|Models|Both).");
                }
            }

        }
    }

    public class UnloadCommand : IOpsCommand
    {

        public UnloadCommand()
        {}

        //IOpsCommand
        public string Name 
        { 
            get{ return "Unload"; } 
        }

        public ContentType Content 
        {
            get 
            { 
                return ContentType.GENERIC; 
            }
        }
        

        //IOpsCommand
        public object ParseArguments(OpsParsedStatement statement)
        {
            return new UnloadArguments(statement);
        }

        //IOpsCommand
        public void Run(OpsContext context, OpsStatement statement)
        {
    
            UnloadArguments args = statement.Arguments as UnloadArguments;


            if( args.Type == ContentType.GENERIC || args.Type == ContentType.MODELS )
            {
                ArrayList models = context.FindModels(args.Src);
                foreach( OpsModel model in models )
                {
                    OpsConsole.WriteLine( "Unloading model: '{0}'", model.Name );
                    context.RemoveModel(model.Name);
                }
            }

            if( args.Type == ContentType.GENERIC || args.Type == ContentType.TEXTURES )
            {
                ArrayList textures = context.FindTextures(args.Src);    
                foreach( OpsTexture texture in textures )
                {
                    OpsConsole.WriteLine( "Unloading texture: '{0}'", texture.Name );
                    context.RemoveTexture(texture.Name);
                }
            }
        }
    }
}

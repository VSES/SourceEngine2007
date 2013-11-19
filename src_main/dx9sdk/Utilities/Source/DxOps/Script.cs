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


namespace Microsoft.DirectX.Solutions.DxOps
{

    public class OpsStatement
    {
        public readonly OpsParsedStatement Parsed;
        public readonly string ContentRegex;

        public readonly IOpsCommand Command;
        public readonly object Arguments;


        public OpsStatement( OpsParsedStatement parsed, IOpsCommand command, object arguments)
        {
            Parsed = parsed;
            OpsParsedArgument arg = parsed.FindArgument("Src");
            ContentRegex = (arg == null) ? "(?# Default Expression ).*" : arg.Value;

            Command = command;
            Arguments = arguments;
        }

        public override string ToString(){ return Parsed.ToString(); }

        public void Run(OpsContext context)
        {
            Command.Run(context, this);
        }

        public ArrayList GetContent( OpsContext context )
        {
            if( Command.Content == ContentType.MODELS )
                return context.FindModels(ContentRegex);
            else if( Command.Content == ContentType.TEXTURES )
                return context.FindTextures(ContentRegex);

            string errorStr = "OpsStatement.GetContent should not be used with commands for content type: " + Command.Content;
            System.Diagnostics.Debug.Assert(false, errorStr);
            
            throw new OpsException(errorStr);
        }
    }

    
    public class OpsScript
    {  
        public OpsStatement[] Statements = null;

        
        public void Run(OpsContext context)
        {
            if(Statements == null)
                return;

            if(OpsConsole.Verbose)
                OpsConsole.WriteLine("Processing script starting");

            for( int i = 0; i < Statements.Length; i++) 
            {
                try
                {
                    if(OpsConsole.Verbose)
                        OpsConsole.WriteLine("Processing statement #{0}: \"{1}\"", i+1, Statements[i].Parsed);

                    Statements[i].Run(context);
                }
                catch(Exception e)
                {
                    OpsConsole.WriteError(i+1, e, "Failure processing statement.");
                    throw;
                }
            }

            if(OpsConsole.Verbose)
                OpsConsole.WriteLine("Processing script finished");
        }
        
        
        public OpsScript(OpsCommandLibrary library, string script)
        {

            OpsParsedStatement[] parsedStatements = OpsParser.ParseScript(script);
            if(parsedStatements != null)
            {
                if(OpsConsole.Verbose)
                    OpsConsole.WriteLine("Prevalidating script starting.");

                Statements = new OpsStatement[parsedStatements.Length];
                for( int i = 0; i < parsedStatements.Length; i++)
                {
                    OpsParsedStatement parsed = parsedStatements[i] as OpsParsedStatement;
                    IOpsCommand command = null;
                    object arguments = null;

                    if(OpsConsole.Verbose)
                        OpsConsole.WriteLine("Prevalidating statement #{0}: \"{1}\"", i, parsed);

                    
                    try
                    {
                        command = library.GetCommand(parsed.Command);
                    }
                    catch( Exception e )
                    {
                        OpsConsole.WriteError(i+1, e, "Unable to fetch command: " +    parsed.Command);
                        throw;
                    }

                    if(command is IOpsRemappable)
                    {
                        try
                        {
                            command = (command as IOpsRemappable).RemapCommand(parsed);
                        }
                        catch( Exception e )
                        {
                            OpsConsole.WriteError(i+1, e, "Unable to remap command: " + parsed.Command);
                            throw;
                        }
                    }

                    try
                    {
                        arguments = command.ParseArguments(parsed);    
                    }
                    catch( Exception e )
                    {
                        OpsConsole.WriteError(i+1, e, "Command failed to interpret the arguments.");
                        throw;
                    }

                    Statements[i] = new OpsStatement(parsed, command, arguments);

                }

                if(OpsConsole.Verbose)
                    OpsConsole.WriteLine("Prevalidating script finished.");
            }

        }
    }
}

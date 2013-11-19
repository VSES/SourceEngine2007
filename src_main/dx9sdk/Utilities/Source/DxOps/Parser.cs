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


namespace Microsoft.DirectX.Solutions.DxOps
{

    public class OpsParsedArgument
    {
        public string Name;
        public string Value;

        public override string ToString()
        {
            if (Name.Length != 0)
                return string.Format("{0}:'{1}'", Name, Value);
            else
                return string.Format("'{0}'", Value);
        }
    }

    public class OpsParsedStatement
    {
        public string Command;
        public OpsParsedArgument[] Args;

    

        public OpsParsedArgument FindArgument(string name)
        {
            int startAndTrack = 0;
            return FindArgumentAfter( name, ref startAndTrack);
        }
            
        public OpsParsedArgument FindArgumentAfter(string name, ref int startAndTrack )
        {
            for(int iArg= startAndTrack; iArg < Args.Length; iArg++)
            {
                OpsParsedArgument arg = Args[iArg];

                startAndTrack = iArg;

                if( 0 == String.Compare( arg.Name, name, true) )
                    return arg;
            }
            return null;
        }


        public override string ToString()
        {
            StringBuilder sb = new StringBuilder();

            if (Command != null && Command.Length != 0)
            {
                sb.Append(Command);
                foreach (OpsParsedArgument arg in Args)
                    sb.AppendFormat(" {0}", arg);
                sb.Append(";");
            }

            return sb.ToString();
        }
        
    } 
    
    
    public class OpsParser
    {      
        public static OpsParsedStatement[] ParseScript(string script)
        {
            if(script == null || script.Length == 0)
                return null;

            if(OpsConsole.Verbose)
                OpsConsole.WriteLine("Parsing script starting.");


            char lastChar = script[script.Length-1];
            if( lastChar != ';' && lastChar != '\n')
                script += '\n';

            ArrayList parsedStatements = new ArrayList();

            int iStatement = 0;
            for (Match statementMatch = StatementReg.Match(script); statementMatch.Success; iStatement++, statementMatch = statementMatch.NextMatch())
            {

                try
                {
                    OpsParsedStatement parsedStatement = ParseStatement(statementMatch);
                    
                    //statements can be null  such as a ;
                    if(parsedStatement != null)
                    {
                        parsedStatements.Add(parsedStatement);

                        if(OpsConsole.Verbose)
                            OpsConsole.WriteLine("Parsing statement: \"{0}\"", parsedStatement);
                    }
                }
                catch (Exception e)
                {
                    OpsConsole.WriteError( iStatement+1, e, "Could not parse statement: {0}", statementMatch.Result("${statement}")); 
                    throw;
                }
            }

            OpsParsedStatement[] result = new OpsParsedStatement[parsedStatements.Count];
            parsedStatements.CopyTo(result, 0);
            
        
            if(OpsConsole.Verbose)
            {
                OpsConsole.WriteLine("Found {0} statements.", result.Length);
                OpsConsole.WriteLine("Parsing script finished.");
            }

            return result;
        }        
        
        public static OpsParsedStatement ParseStatement(Match statementMatch)
        {

            string command = statementMatch.Result("${command}").Trim();
            string invalid = statementMatch.Result("${invalid}");

            if (command.Length == 0)
            {
                if(invalid.Length == 0)
                    return null; // no statement
                else
                    throw new OpsException( "Could not pattern match statement.\nCheck that command and argument identifier match " + IdentifierExp + "\nand argument value with spaces are surrounded by single quotes.");
            }

            OpsParsedStatement result = new OpsParsedStatement();

            result.Command = command;

            string args = statementMatch.Result("${args}");
            ParseArguments(result, args);

            return result;
        }

        public static void ParseArguments(OpsParsedStatement statement, string args)
        {
            ArrayList argList = new ArrayList();
            for (Match argMatch = ArgsReg.Match(args); argMatch.Success; argMatch = argMatch.NextMatch())
            {
                OpsParsedArgument arg = new OpsParsedArgument();

                arg.Name = argMatch.Result("${argName}").Trim();

                string UnescapedArgValue = argMatch.Result("${argVal}");
                arg.Value =  EscapeTickReg.Replace(UnescapedArgValue, "'").Trim();

                argList.Add(arg);
            }

            statement.Args = new OpsParsedArgument[argList.Count];
            argList.CopyTo(statement.Args);
        }


        private static string IdentifierExp = "([a-z_A-Z][a-z_A-Z0-9]*)";
        private static string ArgNameExp = "(((?<argName>" + IdentifierExp + ")\\s*:)?)";
        private static string ArgValueStdExp = "(?<argVal>[^;\\s]+)";
        private static string ArgValueQuotedExp = "('(?<argVal>(([^'])|((?<=\\\\)'))*)')";// "\\\\" resolves to \\ which regex resolves to \
        private static string ArgValueExp = "(" + ArgValueQuotedExp + "|" + ArgValueStdExp + ")";
        private static string ArgExp = "(\\s+" + ArgNameExp + "\\s*" + ArgValueExp + ")";
        private static string CommandExp = "(?<command>" + IdentifierExp + ")";
        
        private static string CommandStatementExp = "(" + CommandExp + "(?<args>" + ArgExp + "*)\\s*;)";
        private static string EmptyStatementsExp = "([;]+)";
        private static string CommentStatementExp = "(([#][^\r\n]*[\r\n])+)";
        private static string InvalidStatementExp = "(?<invalid>\\S+)";

        private static string StatementExp = "(\\s*(?<statement>" + CommandStatementExp + "|" + CommentStatementExp + "|" + EmptyStatementsExp + "|" + InvalidStatementExp + "))";
        
        private static Regex StatementReg = new Regex(StatementExp, RegexOptions.IgnoreCase);
        private static Regex ArgsReg = new Regex(ArgExp, RegexOptions.IgnoreCase);
        private static Regex EscapeTickReg = new Regex( "\\\\'", RegexOptions.IgnoreCase);    
    }
}

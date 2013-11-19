//--------------------------------------------------------------------------------------
// Copyright (c) Microsoft Corporation. All rights reserved.
//--------------------------------------------------------------------------------------
using System;
using System.Drawing;
using System.Windows.Forms;
using System.Collections;
using System.Collections.Specialized;
using System.Text;
using System.Text.RegularExpressions;
using System.IO;
using System.Reflection;
using Microsoft.DirectX;
using Microsoft.DirectX.Direct3D;
using Microsoft.DirectX.Solutions;
using Microsoft.DirectX.Solutions.DxOps.Commands;
using Microsoft.DirectX.Solutions.DxOps.Exceptions;

namespace Microsoft.DirectX.Solutions.DxOps
{
    public enum ContentType
    {
        MODELS,
        TEXTURES,
        GENERIC,
    }

    public interface IOpsCommand
    {
        string Name { get; }
        
        ContentType Content { get; }
        object ParseArguments(OpsParsedStatement statement);
        void Run(OpsContext context, OpsStatement statement);
    }

    public interface IOpsRemappable
    {
        IOpsCommand RemapCommand(OpsParsedStatement statement);
    }


    public class OpsCommandLibrary
    {
        private Hashtable CommandTable= System.Collections.Specialized.CollectionsUtil.CreateCaseInsensitiveHashtable();

        private void AddCommand(IOpsCommand commandObj)
        {
            if (!CommandTable.Contains(commandObj.Name))
                CommandTable.Add(commandObj.Name, commandObj);
            else
                throw new OpsException("A command must not map to more than one IOpsCommand object.");
        }

        public IOpsCommand GetCommand(string name)
        {

            IOpsCommand result = CommandTable[name] as IOpsCommand;;
    
            if (result == null)
                throw new OpsException("Could not find command: " + name );

            return result;
        }

        public void FindCommands()
        {
            //This code will hit a broader set of assemblies
            //I left it here encase it is needed by the user or for future work.
            //AppDomain myDomain = AppDomain.CurrentDomain;
            //Assembly[] assemblies = myDomain.GetAssemblies();
            // foreach (Assembly assembly in assemblies)
            //{
            Assembly assembly = Assembly.GetExecutingAssembly();

            try
            {

                Type[] types= assembly.GetTypes();

                if(types != null)
                {
                    foreach (Type type in types)
                    {
                        if( type.Namespace == "Microsoft.DirectX.Solutions.DxOps.Commands")
                        {
                            if (type.IsClass 
                                && !type.IsAbstract 
                                && typeof(IOpsCommand).IsAssignableFrom( type))
                            {
                                //valid command;
                                try
                                {
                                    IOpsCommand commandObj = Activator.CreateInstance(type) as IOpsCommand;
    
                                    AddCommand(commandObj);
                                }
                                catch
                                {
                                    OpsConsole.WriteError( null, "Could not create instance of type: " + type.FullName );
                                    throw;
                                }
                            }
                        }
                    }
                }
            }
            catch( System.Reflection.ReflectionTypeLoadException e )
            {
                OpsConsole.WriteError(e, "Could not get type from assembly: " + assembly.FullName );
                throw;
            }
            //}
        }
    }
}

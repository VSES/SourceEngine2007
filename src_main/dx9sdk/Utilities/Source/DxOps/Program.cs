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


//UnsafeCode hack
using System.Runtime.InteropServices;



namespace Microsoft.DirectX.Solutions.DxOps
{
    public class UnsafeCode 
    {
        [DllImportAttribute("kernel32", SetLastError=true)]
        public static extern unsafe IntPtr GetConsoleWindow ();
    }

    public class Program
    {
        public static OpsCommandLibrary CommandLibrary = new OpsCommandLibrary();
        public static Stack SearchPaths = new Stack();

        public static int Main(string[] args)
        {
            int result = 0;
            try
            {
                string unparsedScript = ParseCommandLine(args);

                CommandLibrary.FindCommands();

                OpsContext context = new OpsContext( CreateDevice());    

                OpsScript script = new OpsScript(CommandLibrary, unparsedScript);

                script.Run(context);

                if(OpsConsole.Verbose)
                    OpsConsole.WriteLine("Success");
            }

            catch
            {
                OpsConsole.WriteLine("Application cannot continue due to errors." );
                result = -1;
            }
            finally
            {
                Close();
            }

            return result;

        }

        private static string ParseCommandLine(string[] args)
        {
            StringBuilder scriptBuilder = new StringBuilder();
            bool help = false;

            ArrayList argList = new ArrayList(args);
            
            //force use of arglist isntead of args
            args = null;

            string arg1 = null;
            string arg2 = null;

            try
            {

                //expand all environment variables
                for (int iArg = 0; iArg < argList.Count; iArg++)
                {
                    arg1 = argList[iArg] as string;

                    argList[iArg] = System.Environment.ExpandEnvironmentVariables( arg1 );
                }

                //process verbose first so that 
                for (int iArg = 0; iArg < argList.Count; iArg++)
                {
                    arg1 = argList[iArg] as string;
                    arg2 = null;

                    if ( arg1 == "-verbose" || arg1 == "/verbose" )
                    {
                        OpsConsole.Verbose = true;

                        if(OpsConsole.Verbose)
                            OpsConsole.WriteLine("Activating verbose mode." );

                        argList.RemoveAt(iArg);
                        break;
                    }
                }

                if(OpsConsole.Verbose)
                    OpsConsole.WriteLine("Command-line processing starting.");

                //search for -log or -loga first so that console out is always the same
                //we do not support more than one -log or -loga
                for (int iArg = 0; iArg < argList.Count; iArg++)
                {
                    arg1 = argList[iArg] as string;
                    arg2 = null;

                    if (arg1 == "-log" || arg1 == "/log")
                    {
                        arg2 = ((string)argList[iArg+1]).Trim();

                        OpsConsole.RouteConsoleToFile(arg2, false);

                        if(OpsConsole.Verbose)
                            OpsConsole.WriteLine("Logging to new file: \"{0}\"",arg2);

                        argList.RemoveRange(iArg, 2);
                        break;
                    }
                    else if (arg1 == "-loga" || arg1 == "/loga")
                    {
                        arg2 = ((string)argList[iArg+1]).Trim();

                        OpsConsole.RouteConsoleToFile(arg2, true);

                        if(OpsConsole.Verbose)
                            OpsConsole.WriteLine("Logging to end of file: \"{0}\"",arg2);

                        argList.RemoveRange(iArg, 2);
                        break;
                    }
                }

                //process the rest of the commands
                for (int iArg = 0; iArg < argList.Count; iArg++)
                {
                    arg1 = argList[iArg] as string;
                    arg2 = null;

                    if (arg1 == "-?" || arg1 == "/?" || arg1 == "-help" || arg1 == "/help")
                    {
                        help = true;

                        if(OpsConsole.Verbose)
                            OpsConsole.WriteLine("Found help request.");
                    }
                    else if (arg1 == "-s" || arg1 == "/s")
                    {
                        arg2 = ((string)argList[++iArg]).Trim();
                    
                        scriptBuilder.Append( arg2 );
                        scriptBuilder.Append( ';' );

                        if(OpsConsole.Verbose)
                            OpsConsole.WriteLine("Adding command-line script.");
                    }
                    else if (arg1 == "-f" || arg1 == "/f")
                    {
                        arg2 = ((string)argList[++iArg]).Trim();
                    
                        StreamReader sr= File.OpenText(arg2);
                        if(sr == null)
                            throw new OpsException("Could not read script file:" + arg2 );
        
                        scriptBuilder.Append( sr.ReadToEnd().Trim() );
                        scriptBuilder.Append( ';' );            

                        if(OpsConsole.Verbose)
                            OpsConsole.WriteLine("Adding script from file: \"{0}\"",arg2);
                    }
                    else if (arg1 == "-m" || arg1 == "/m")
                    {
                        arg2 = ((string)argList[++iArg]).Trim();

                        string searchPath = System.IO.Path.GetFullPath( arg2 );
                    
                        if( System.IO.Directory.Exists(searchPath) )
                            SearchPaths.Push( searchPath );
                        else
                            OpsConsole.WriteLine("Search path does not exist: \"{0}\"", searchPath);

                        if(OpsConsole.Verbose)
                            OpsConsole.WriteLine("Adding search path: \"{0}\"", arg2);
                    }
                    else
                    {
                        switch(arg1)
                        {
                            case "-log":
                            case "/log":
                            case "-loga":
                            case "/loga":
                            case "-verbose":
                            case "/verbose":
                                throw new OpsException(String.Format("Duplicate command line argument not allowed: {0}", arg1));
                            default:
                                throw new OpsException(String.Format("Invalid command line argument: {0}", arg1));
                        };
                    }
                }
            
                if(OpsConsole.Verbose)
                    OpsConsole.WriteLine("Command-line processing finished.");

                if(help || (scriptBuilder.Length == 0) )
                    PrintHelp();
            }
            catch(Exception e)
            {
                OpsConsole.WriteError(e, String.Format("An error was found durring command-line processing of {0} {1}", arg1, arg2));
                throw;
            }

            return scriptBuilder.ToString();

        }

        
        
        private static bool PrintedHelpOnce = false;
        public static void PrintHelp()
        {
            if(!PrintedHelpOnce)
            {
                PrintedHelpOnce = true;

                Version version = Assembly.GetEntryAssembly().GetName().Version;

                string verStr = String.Format( "{0}.{1}.{2}.{3}", version.Major, version.Minor, version.Build, version.Revision.ToString("D4"));

                OpsConsole.WriteLine("Microsoft (R) DirectX Operations Tool v" + verStr );
                OpsConsole.WriteLine("Copyright (C) Microsoft Corporation 2005. All rights reserved.");
                OpsConsole.WriteLine("");
                OpsConsole.WriteLine("Usage: dxops [-?] [-log file] [[-s \"script\"] -s \"script2\"] [[-f file] -f file2]");
                OpsConsole.WriteLine("     -?              Shows this help message.");
                OpsConsole.WriteLine("     -log \"file\"     Specifies the location of the log output to create.");
                OpsConsole.WriteLine("     -loga \"file\"    Specifies the location of the log output to append.");
                OpsConsole.WriteLine("     -s \"script\"     Execute the specified script statements.");//dont remove the extra spaces... they make it line up.
                OpsConsole.WriteLine("     -f \"file\"       Execute the script in the specified file.");
                OpsConsole.WriteLine("     -m \"path\"       Add a media path to search for file specified without");
                OpsConsole.WriteLine("                     a full path.");
                OpsConsole.WriteLine("     -verbose        Verbose printing of info, warning, and errors.");
                OpsConsole.WriteLine("");
                OpsConsole.WriteLine("For details on this tool and its operations view the DirectX SDK documentation.");
            }
        }


        private static void Close()
        {
            OpsConsole.Close();

#if ( DEBUG_PRESSKEY )
            OpsConsole.WriteLine("Press ENTER to continue.");
            Console.ReadLine();
#endif 
        }


        private static Device CreateDevice()
        {
            Device device = null;              // Create rendering device

            PresentParameters presentParams = new PresentParameters();
            presentParams.Windowed=true;
            presentParams.SwapEffect = SwapEffect.Discard;

            IntPtr wnd= UnsafeCode.GetConsoleWindow();

            //device = new Device(0, DeviceType.NullReference, wnd, CreateFlags.SoftwareVertexProcessing, presentParams);
            device = new Device(0, DeviceType.Reference, wnd, CreateFlags.SoftwareVertexProcessing, presentParams);

            return device;
        }
    


    }


    
    public class OpsHelpers
    {
        public static string ResolvePathToOne(string filepath)
        {
            string[] result = ResolvePathToMany(filepath);
 
            return result[0];
        }

        public static string[] ResolvePathToMany(string filepath)
        {
            string[] foundFiles = null;

            foundFiles = ResolveFile(filepath);
            if( foundFiles != null && foundFiles.Length > 0 )
                return foundFiles;
                
            if( !System.IO.Path.IsPathRooted(filepath) )
            {
                string qualifiedPath = null;

                foreach(string searchPath in Program.SearchPaths)
                {
                    qualifiedPath = System.IO.Path.Combine( searchPath, filepath);

                    foundFiles = ResolveFile(qualifiedPath);

                    if( foundFiles != null && foundFiles.Length > 0 )
                        return foundFiles;    
                }

                // <-  x86/bin/utilities   ->  Samples\Media
                qualifiedPath = System.IO.Path.Combine( "..\\..\\..\\Samples\\Media", filepath);
                foundFiles = ResolveFile(qualifiedPath);
                if( foundFiles != null && foundFiles.Length > 0 )
                    return foundFiles;
            }

            throw new OpsException("Path could not be resolved: " + filepath);
        }

        private static string[] ResolveFile(string filepath)
        {
            //FOR DEBUG ONLY//OpsConsole.WriteLine("Attempting to resolve: \"{0}\"", filepath );
            try
            {
                string dir = Path.GetDirectoryName(filepath);
                if( dir == null || dir.Length == 0)
                    dir = Directory.GetCurrentDirectory();

                string file = Path.GetFileName(filepath);

                return Directory.GetFiles( dir, file );
            }
            catch
            {
                return null; 
            }
        }

    }


}

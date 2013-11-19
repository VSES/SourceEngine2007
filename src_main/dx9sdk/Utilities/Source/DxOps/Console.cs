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
    public interface IOpsConsoleSubHeader
    {
        string GetConsoleSubHeader();
    }

    public class OpsConsole
    {
        private static StreamWriter logStream = null;
        private static TextWriter Screen = null;
        
        public static bool Verbose = false;
        
        static OpsConsole()
        {
            Screen = Console.Out;
        }

        public static void RouteConsoleToFile(string filename, bool Append)
        {
            FileStream logFile = null;
            if( Append )
                logFile = new FileStream(filename, FileMode.Append);
            else
                logFile = new FileStream(filename, FileMode.Create);

            ASCIIEncoding encoder = new ASCIIEncoding();
            logStream = new StreamWriter(logFile, encoder);
            Console.SetOut(logStream);
        }

        public static void Close()
        {
            if (logStream != null)
            {
                logStream.Flush();
                logStream.Close();
                Console.SetOut(Screen);
            }
        }

        public static void WriteError(int statementNumber, Exception e, string format, params object[] args)
        {
            string newFormat = String.Format("(Statement #{0}) {1}", statementNumber, format);
            WriteError(e, newFormat, args);
        }

        public static void WriteError(Exception e, string format, params object[] args)
        {
            string message = "ERROR: ";
            if(message != null && message.Length != 0)
            {
                message += String.Format(format, args);
                if (e != null)
                    message += " ";
            }
            if (e != null)
                message += PrintableException(e);

            WriteLine(message);
        }


        public static string PrintableException(Exception e)
        {
            if(e == null)
                return null;

            StringBuilder sb = new StringBuilder();
            OpsConsole.PrintableExceptionInternal( sb, e );

            if(sb.Length == 0)
                sb.Append("Internal Error");

            return sb.ToString();
        }

        private static void PrintableExceptionInternal(StringBuilder sb, Exception e)
        {
            if(e is OpsException)
            {
                if( sb.Length != 0)
                    sb.Append( " -AND- ");

                if( e.Message != null )
                    sb.Append(e.Message);    
            }
#if ( DEBUG_DPFALL )
            else
                sb.Append( e.ToString() );
#endif 

            if(e.InnerException != null )
                OpsConsole.PrintableExceptionInternal(sb, e.InnerException);
        }

        public static void WriteLine(string format, params object[] arg)
        {
            Console.WriteLine( format, arg);
            if(OpsConsole.Screen != Console.Out)
                Screen.WriteLine( format, arg );
        }

        public static void Write(string format, params object[] arg)
        {
            Console.Write( format, arg);
            if(OpsConsole.Screen != Console.Out)
                Screen.Write( format, arg );
        }
    }
}

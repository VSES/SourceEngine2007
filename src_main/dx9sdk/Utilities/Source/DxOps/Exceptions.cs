//--------------------------------------------------------------------------------------
// Copyright (c) Microsoft Corporation. All rights reserved.
//--------------------------------------------------------------------------------------
using System;

namespace Microsoft.DirectX.Solutions.DxOps.Exceptions
{
    public class OpsException : Exception
    {
        public OpsException() : base(){}
        public OpsException(string s) : base(s){}
        //protected OpsStatementException(SerializationInfo s, StreamingOpsContext c){}
        public OpsException(string s, Exception e) : base(s,e){}

        public static OpsException ArgRequired(string arg)
        {
            return new OpsException(String.Format("Argument '{0}' is required", arg)); 
        }

    }    

}
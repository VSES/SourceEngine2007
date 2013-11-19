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
        public class AddVDataArguments
        {
            public DeclarationType type = 0;
            public DeclarationUsage usage = DeclarationUsage.TextureCoordinate;
            public int usageIdx = 0;

            public AddVDataArguments(OpsParsedStatement statement)
            {
                OpsParsedArgument typeArg = statement.FindArgument("type");
                if(typeArg == null)
                {
                    typeArg = statement.FindArgument("");
                    if(typeArg == null)
                        throw new OpsException("'type' argument argument");
                }
                type = MeshDeclarationHelper.DecodeType(typeArg.Value);

                OpsParsedArgument usageArg = statement.FindArgument("usage");
                if(usageArg != null)
                    usage = MeshDeclarationHelper.DecodeUsage(usageArg.Value);

                OpsParsedArgument usageIdxArg = statement.FindArgument("usageIdx");
                if(usageIdxArg != null)
                    usageIdx = int.Parse( usageIdxArg.Value );

            }

            public string FriendlyName
            {
                get
                {
                    return usage.ToString() + usageIdx.ToString();
                }
            }

        }
    }


    public class VertexElementAddCommand : IOpsCommand
    {

        public VertexElementAddCommand()
        {}

        //IOpsCommand
        public string Name 
        { 
            get{ return "AddVData"; } 
        }
        public ContentType Content {get { return ContentType.MODELS; }}
        //IOpsCommand
        public object ParseArguments(OpsParsedStatement statement)
        {
            return new AddVDataArguments(statement);
        }

        //IOpsCommand
        public void Run(OpsContext context, OpsStatement statement)
        {
            AddVDataArguments arguments = statement.Arguments as AddVDataArguments;
            
            OpsConsole.WriteLine( String.Format("Adding vertex element {0} of type {1}", arguments.FriendlyName, arguments.type) );

            foreach( OpsModel model in statement.GetContent(context) )
            {
                foreach( OpsMeshContainer meshContainer in model.GetMeshEnumerator() )
                {    

                    VertexElement[] OldVEs= meshContainer.MeshData.Mesh.Declaration.Clone() as VertexElement[];
                
                    int iExists;
                    if( false == MeshDeclarationHelper.FindElement(OldVEs, arguments.usage, arguments.usageIdx, out iExists) )
                    {
                        int addedIdx;
                        VertexElement[] VEs= MeshDeclarationHelper.AddElement(OldVEs, arguments.type, arguments.usage, arguments.usageIdx, out addedIdx );

                        Mesh newMesh = meshContainer.MeshData.Mesh.Clone( meshContainer.MeshData.Mesh.Options.Value, VEs, meshContainer.MeshData.Mesh.Device);
                        meshContainer.ReplaceMesh( newMesh );
                    }
                    else
                    {
                        OpsConsole.WriteLine( String.Format("Found existing vertex element {0}{1} on {2}", arguments.usage, arguments.usageIdx, meshContainer.FriendlyName(model) ));
                    }
                }
            }
        }
    }

    namespace Arguments
    {
        public class DelVDataArguments
        {
            public DeclarationUsage usage = 0;
            public int usageIdx = int.MinValue;
            public bool hasSrcUsageIdx = false;

            public DelVDataArguments(OpsParsedStatement statement)
            {
                OpsParsedArgument usageArg = statement.FindArgument("usage");    
                if(usageArg == null)
                {
                    usageArg = statement.FindArgument("");    
                    if(usageArg == null)
                        throw OpsException.ArgRequired("usage");
                }
                usage = MeshDeclarationHelper.DecodeUsage(usageArg.Value);

                OpsParsedArgument usageIdxArg = statement.FindArgument("usageIdx");
                if(usageIdxArg != null)
                {
                    hasSrcUsageIdx = true;
                    usageIdx = int.Parse( usageIdxArg.Value );
                }

            }

            public string FriendlyName
            {
                get
                {
                    return usage.ToString() + (hasSrcUsageIdx ? usageIdx.ToString() : "");
                }
            }
        }
    }

    public class VertexElementDelCommand : IOpsCommand
    {

        public VertexElementDelCommand()
        {}

        //IOpsCommand
        public string Name 
        { 
            get{ return "DelVData"; } 
        }
        public ContentType Content {get { return ContentType.MODELS; }}
        //IOpsCommand
        public object ParseArguments(OpsParsedStatement statement)
        {
            return new DelVDataArguments(statement);
        }

        //IOpsCommand
        public void Run(OpsContext context, OpsStatement statement)
        {
            DelVDataArguments arguments = statement.Arguments as DelVDataArguments;

            OpsConsole.WriteLine( String.Format("Deleting vertex element {0}", arguments.FriendlyName) );
            
            foreach( OpsModel model in statement.GetContent(context) )
            {
                foreach( OpsMeshContainer meshContainer in model.GetMeshEnumerator() )
                {    

                    VertexElement[] OldVEs= meshContainer.MeshData.Mesh.Declaration.Clone() as VertexElement[];

                    int srcUsageIdx = arguments.usageIdx; 
                    if( arguments.hasSrcUsageIdx == false)
                    {
                        for( int i = 0 ; i < OldVEs.Length; i++ )
                        {
                            if( OldVEs[i].DeclarationUsage == arguments.usage )
                            {
                                srcUsageIdx = OldVEs[i].UsageIndex;
                                break;
                            }
                        }                       
                    }

                    if(srcUsageIdx < 0)
                        continue;


                    int iRemove;
                    if(MeshDeclarationHelper.FindElement(OldVEs, arguments.usage, srcUsageIdx, out iRemove))
                    {
                        VertexElement[] VEs= MeshDeclarationHelper.RemoveElement(OldVEs, iRemove);

                        Mesh newMesh = meshContainer.MeshData.Mesh.Clone( meshContainer.MeshData.Mesh.Options.Value, VEs, meshContainer.MeshData.Mesh.Device);
                        meshContainer.ReplaceMesh( newMesh );
                    }
                    else
                    {
                        OpsConsole.WriteLine( String.Format("Could not find vertex element {0}{1} on {2}", arguments.usage, arguments.usageIdx, meshContainer.FriendlyName(model)) );
                    }
                }
            }
        }
    }

    namespace Arguments
    {
        public class CloneVDataArguments
        {
            public DeclarationUsage srcUsage = 0;
            public int srcUsageIdx = int.MinValue;
            public bool hasSrcUsageIdx = false;

            public DeclarationUsage dstUsage = DeclarationUsage.TextureCoordinate;
            public int dstUsageIdx = int.MinValue;
            public bool hasDstUsageIdx = false;


            public CloneVDataArguments(OpsParsedStatement statement)
            {
                OpsParsedArgument usageArg = statement.FindArgument("usage");    
                if(usageArg == null)
                {
                    usageArg = statement.FindArgument("");    
                    if(usageArg == null)
                        throw OpsException.ArgRequired("usage");
                }
                srcUsage = MeshDeclarationHelper.DecodeUsage(usageArg.Value);

                OpsParsedArgument usageIdxArg = statement.FindArgument("usageIdx");
                if( usageIdxArg != null )
                {
                    srcUsageIdx = int.Parse( usageIdxArg.Value );
                    hasSrcUsageIdx = true;
                }

                OpsParsedArgument dstUsageArg = statement.FindArgument("newusage");
                if(dstUsageArg != null)
                    dstUsage = MeshDeclarationHelper.DecodeUsage(dstUsageArg.Value);

                OpsParsedArgument dstUsageIdxArg = statement.FindArgument("newusageIdx");
                if( dstUsageIdxArg != null )
                {
                    dstUsageIdx = int.Parse( dstUsageIdxArg.Value );
                    hasDstUsageIdx = true;
                }
            }

            public string FriendlySrcName
            {
                get
                {
                    return srcUsage.ToString() + (hasSrcUsageIdx ? srcUsageIdx.ToString() : "");
                }
            }
        
            public string FriendlyDstName
            {
                get
                {
                    return dstUsage.ToString() + (hasDstUsageIdx ? dstUsageIdx.ToString() : "");
                }
            }
        }
    }


    public class VertexElementCloneCommand : IOpsCommand
    {

        public VertexElementCloneCommand()
        {}

        //IOpsCommand
        public string Name 
        { 
            get{ return "CloneVData"; } 
        }
        public ContentType Content {get { return ContentType.MODELS; }}
        //IOpsCommand
        public object ParseArguments(OpsParsedStatement statement)
        {
            return new CloneVDataArguments(statement);
        }

        //IOpsCommand
        public void Run(OpsContext context, OpsStatement statement)
        {
            CloneVDataArguments arguments = statement.Arguments as CloneVDataArguments;

            OpsConsole.WriteLine( String.Format("Cloning vertex element {0} as {1}", arguments.FriendlySrcName, arguments.FriendlyDstName) );

            foreach( OpsModel model in statement.GetContent(context) )
            {
                foreach( OpsMeshContainer meshContainer in model.GetMeshEnumerator() )
                {    

                    int srcIdx;
                    int dstIdx;

                    VertexElement[] OldVEs= meshContainer.MeshData.Mesh.Declaration.Clone() as VertexElement[];


                    int srcUsageIdx = arguments.srcUsageIdx; 
                    if( arguments.hasSrcUsageIdx == false)
                    {
                        for( int i = 0 ; i < OldVEs.Length; i++ )
                        {
                            if( OldVEs[i].DeclarationUsage == arguments.srcUsage )
                            {
                                srcUsageIdx = OldVEs[i].UsageIndex;
                                break;
                            }
                        }                       
                    }

                    if(srcUsageIdx < 0)
                        continue;

                    if(!MeshDeclarationHelper.FindElement( OldVEs, arguments.srcUsage, srcUsageIdx, out srcIdx))
                        throw new OpsException("Could not find source vertex element");

                    int dstUsageIdxPerMesh = arguments.dstUsageIdx;
                    if(!arguments.hasDstUsageIdx)
                    {
                        if(MeshDeclarationHelper.FindValidUsageIndex(OldVEs, arguments.dstUsage, out dstUsageIdxPerMesh, out dstIdx) )
                            throw new OpsException("Could not find open usage index for destination vertex element");
                    }

                    int copyIdx;
                    VertexElement[] VEs= MeshDeclarationHelper.AddElement(OldVEs, OldVEs[srcIdx].DeclarationType, arguments.dstUsage, dstUsageIdxPerMesh, out copyIdx);

                    if(!MeshDeclarationHelper.FindElement( VEs, arguments.srcUsage, srcUsageIdx, out srcIdx))
                        throw new OpsException("Could not find source vertex element");

                    if(!MeshDeclarationHelper.FindElement( VEs, arguments.dstUsage, dstUsageIdxPerMesh, out dstIdx))
                        throw new OpsException("Could not find destination vertex element");



                    Mesh newMesh = meshContainer.MeshData.Mesh.Clone( meshContainer.MeshData.Mesh.Options.Value, VEs, meshContainer.MeshData.Mesh.Device);

                    int sizeInBytes = MeshDeclarationHelper.GetTypeSize( VEs[srcIdx].DeclarationType );
                    int srcOffset = VEs[srcIdx].Offset;
                    int dstOffset = VEs[dstIdx].Offset;
                    byte[] copyBuffer = new byte[sizeInBytes];

                    VertexBuffer vb = newMesh.VertexBuffer;
                    GraphicsStream gs = vb.Lock(0, 0, LockFlags.None);
                    for(int iVertex = 0; iVertex < newMesh.NumberVertices; iVertex++)
                    {
                        gs.Seek(newMesh.NumberBytesPerVertex * iVertex + srcOffset, SeekOrigin.Begin);
                        gs.Read(copyBuffer, 0, sizeInBytes);

                        gs.Seek(newMesh.NumberBytesPerVertex * iVertex + dstOffset, SeekOrigin.Begin);
                        gs.Write(copyBuffer, 0, sizeInBytes);
                    }
                    vb.Unlock();
                    gs= null;
                    vb= null;

                    meshContainer.ReplaceMesh(newMesh);
                }
            }
        }
    }


}
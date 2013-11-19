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

    public class CleanCommand : IOpsCommand
    {

        public CleanCommand()
        {}

        //IOpsCommand
        public string Name 
        { 
            get{ return "Clean"; } 
        }
        public ContentType Content {get { return ContentType.MODELS; }}
        //IOpsCommand
        public object ParseArguments(OpsParsedStatement statement)
        {
            return null;
        }

        //IOpsCommand
        public void Run(OpsContext context, OpsStatement statement)
        {
            OpsConsole.WriteLine( "Cleaning models");

            foreach( OpsModel model in statement.GetContent(context) )
            {
                foreach( OpsMeshContainer meshContainer in model.GetMeshEnumerator() )
                {        
                    
                    string errorsAndWarnings;
                    meshContainer.MeshData.Mesh.Validate(meshContainer.GetAdjacencyStream(), out errorsAndWarnings);
                    if(errorsAndWarnings == null || errorsAndWarnings.Length == 0)
                        continue;
                    else
                    {
                        OpsConsole.WriteLine( "Cleaning '{0}'", meshContainer.FriendlyName(model) );
                        OpsConsole.WriteLine(errorsAndWarnings);
                    }

                    VertexElement[] OldVEs= null;
                    int offsetOfIndex = -1;
                    Mesh dirtyMesh = meshContainer.MeshData.Mesh;
                    if(meshContainer.SkinInformation != null)
                    {
                        int psizeUsageIdx = -1;
                        int remapIdx = -1;
                        OldVEs= meshContainer.MeshData.Mesh.Declaration.Clone() as VertexElement[];

                        if(!MeshDeclarationHelper.FindValidUsageIndex(OldVEs, DeclarationUsage.PointSize, out psizeUsageIdx))
                            throw new OpsException("Could not add remapping-indexing vertex element to declaration");

                        VertexElement[] VEs= MeshDeclarationHelper.AddElement(OldVEs, DeclarationType.Float1, DeclarationUsage.PointSize, psizeUsageIdx, out remapIdx);

                        dirtyMesh = meshContainer.MeshData.Mesh.Clone( meshContainer.MeshData.Mesh.Options.Value, VEs, meshContainer.MeshData.Mesh.Device);

                        offsetOfIndex = VEs[remapIdx].Offset;
                        VertexBuffer vb = dirtyMesh.VertexBuffer;
                        GraphicsStream gs = vb.Lock(0, 0,LockFlags.None);
                        for(int iVertex = 0; iVertex < dirtyMesh.NumberVertices; iVertex++)
                        {
                            gs.Seek(dirtyMesh.NumberBytesPerVertex * iVertex + offsetOfIndex, SeekOrigin.Begin);
                            gs.Write(iVertex);
                        }
                        vb.Unlock();
                        gs= null;
                        vb= null;
                    }

                    int[] adjOut;
                    Mesh cleanedMesh= Mesh.Clean(CleanType.BackFacing|CleanType.BowTies, dirtyMesh, meshContainer.GetAdjacency(), out adjOut, out errorsAndWarnings);

                    if(errorsAndWarnings != null && errorsAndWarnings.Length != 0)
                    {
                        OpsConsole.WriteLine("Remaining Errors and Warnings:");
                        OpsConsole.WriteLine(errorsAndWarnings);
                    }

                    if(meshContainer.SkinInformation != null)
                    {
                        int[] vertexRemap = new int[cleanedMesh.NumberVertices];

                        VertexBuffer vb = cleanedMesh.VertexBuffer;
                        GraphicsStream gs = vb.Lock(0, 0,LockFlags.None);
                        for(int iVertex = 0; iVertex < cleanedMesh.NumberVertices; iVertex++)
                        {
                            gs.Seek(cleanedMesh.NumberBytesPerVertex*iVertex+offsetOfIndex, SeekOrigin.Begin);
                            vertexRemap[iVertex] = (int)gs.Read(typeof(int));
                        }
                        vb.Unlock();
            
                        meshContainer.RemapSkin( vertexRemap );

                        cleanedMesh = cleanedMesh.Clone(cleanedMesh.Options.Value, OldVEs, cleanedMesh.Device);
                    }

                    meshContainer.SetAdjacency(adjOut);


                    meshContainer.ReplaceMesh(cleanedMesh);
                }
            }
        }
    }
}
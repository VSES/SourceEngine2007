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
using Microsoft.DirectX.Solutions.DxOps.Commands.Arguments;


namespace Microsoft.DirectX.Solutions.DxOps.Commands
{
    public class MeshInfoCommand : IOpsCommand
    {

        public MeshInfoCommand()
        {}

        //IOpsCommand
        public string Name 
        { 
            get{ return "Meshes"; } 
        }
        public ContentType Content {get { return ContentType.MODELS; }}
        //IOpsCommand
        public object ParseArguments(OpsParsedStatement statement)
        {
            return null;
        }

        private static string format = "{0,-20}{1,11}{2,11}{3,11}{4,16}";
        //IOpsCommand
        public void Run(OpsContext context, OpsStatement statement)
        {
            
            OpsConsole.WriteLine( "Mesh information");

            foreach( OpsModel model in statement.GetContent(context) )
            {
                OpsConsole.WriteLine( "MODEL: '{0}'", model.Name);
                OpsConsole.WriteLine(format, "Names", "Faces", "Vertices", "Attributes", "Boundings" );

                foreach( OpsMeshContainer meshContainer in model.GetMeshEnumerator() )
                {    
                    float radius = 0;

                    VertexElement[] VEs= meshContainer.MeshData.Mesh.Declaration;
                    int posIdx;
                    if( MeshDeclarationHelper.FindElement(VEs, DeclarationUsage.Position, 0, out posIdx) )
                    {
                        using (VertexBuffer vb = meshContainer.MeshData.Mesh.VertexBuffer)
                        {

                            GraphicsStream vertexData = vb.Lock( VEs[posIdx].Offset, vb.Description.Size - VEs[posIdx].Offset, LockFlags.None);
                        
                            Vector3 center;
                            radius = Geometry.ComputeBoundingSphere( vertexData,
                                meshContainer.MeshData.Mesh.NumberVertices,
                                meshContainer.MeshData.Mesh.NumberBytesPerVertex,
                                out center);

                            vb.Unlock();
                        }
                    }

                    OpsConsole.WriteLine( format,
                        meshContainer.Name,
                        meshContainer.MeshData.Mesh.NumberFaces,
                        meshContainer.MeshData.Mesh.NumberVertices,
                        meshContainer.MeshData.Mesh.NumberAttributes,
                        radius);
                }
            }
        }
    }
}
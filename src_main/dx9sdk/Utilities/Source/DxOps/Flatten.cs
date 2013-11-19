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
        public class FlattenArguments
        {
            public string NewModel;
        
            public FlattenArguments(OpsParsedStatement statement)
            {
                OpsParsedArgument newModelArg = statement.FindArgument("dst");
                if(newModelArg == null)
                {
                    newModelArg = statement.FindArgument("");
                    if(newModelArg == null)
                        throw OpsException.ArgRequired("dst");
                }

                NewModel = newModelArg.Value;
            }
        }
    }

    public class FlattenCommand : IOpsCommand
    {

        public FlattenCommand()
        {}

        //IOpsCommand
        public string Name 
        { 
            get{ return "Flatten"; } 
        }
        public ContentType Content {get { return ContentType.MODELS; }}
        //IOpsCommand
        public object ParseArguments(OpsParsedStatement statement)
        {
            return new FlattenArguments(statement);
        }


        //IOpsCommand
        public void Run(OpsContext context, OpsStatement statement)
        {
            FlattenArguments arguments = statement.Arguments as FlattenArguments;

            ArrayList models = statement.GetContent(context);

            OpsConsole.WriteLine( "Flattening models into '{0}'", arguments.NewModel);


            Mesh[] meshes;
            Matrix[] localToWorlds;
            int[][] adjacencies;
            ExtendedMaterial[] materials;
            EffectInstance[] effectInstances;

            GetLists( models, out meshes, out localToWorlds, out adjacencies, out materials, out effectInstances);

            Mesh newMesh = PrtEngine.ConcatenateMeshes( 
                meshes, 
                MeshFlags.SystemMemory|MeshFlags.Use32Bit,
                localToWorlds,
                null, 
                null, 
                context.Device);

            int[] newAdj = ConcatenateAdjacency( newMesh, meshes, adjacencies);

            OpsMeshContainer newMC = new OpsMeshContainer();
            newMC.Name = arguments.NewModel;
            newMC.ReplaceMesh( newMesh );
            newMC.SetMaterials( materials );
            newMC.SetEffectInstances( effectInstances );
            newMC.SetAdjacency( newAdj );

            OpsFrame newFrame = new OpsFrame();
            newFrame.Name = arguments.NewModel;
            newFrame.MeshContainer = newMC;

            OpsModel newModel = new OpsModel();
            newModel.Name = arguments.NewModel;
            newModel.HierarchyRoot = newFrame;

            context.AddModel(newModel);
        }


        public void GetLists( ArrayList models, out Mesh[] meshes,  out Matrix[] localToWorlds, out int[][] adjacencies, out ExtendedMaterial[] materials, out EffectInstance[] effectInstances)
        {
            ArrayList meshList = new ArrayList();
            ArrayList localToWorldList = new ArrayList();
            ArrayList adjacenciesList = new ArrayList();
            ArrayList materialList = new ArrayList();
            ArrayList effectInstanceList = new ArrayList();

            foreach( OpsModel model in models)
            {
                OpsFrame.ComputeLocalToWorld( model.HierarchyRoot as OpsFrame , Matrix.Identity );

                foreach( OpsMeshContainer mc in model.GetMeshEnumerator())
                {
                    meshList.Add( mc.MeshData.Mesh );
                    adjacenciesList.Add( mc.GetAdjacency() );
                    localToWorldList.Add( mc.LocalToWorld );    

                    ExtendedMaterial[] oldMtlList = mc.GetMaterials();
                    EffectInstance[] oldFxList = mc.GetEffectInstances();
                
                    System.Diagnostics.Debug.Assert( oldMtlList.Length == oldFxList.Length );

                    materialList.AddRange( oldMtlList );
                    effectInstanceList.AddRange( oldFxList );
                }
            }

            meshes = meshList.ToArray( typeof(Mesh) ) as Mesh[];
            localToWorlds = localToWorldList.ToArray( typeof(Matrix) ) as Matrix[];
            adjacencies = adjacenciesList.ToArray( typeof(int[]) ) as int[][];
            materials = materialList.ToArray( typeof(ExtendedMaterial) ) as ExtendedMaterial[];
            effectInstances = effectInstanceList.ToArray( typeof(EffectInstance) ) as EffectInstance[];
        }

        private int[] ConcatenateAdjacency(Mesh newMesh, Mesh[] meshes, int[][] adjacencies)
        {
            int[] result = new int[3*newMesh.NumberFaces];

            int BaseFace = 0;
            for(int iMesh = 0; iMesh < meshes.Length; iMesh++ )
            {
                Mesh mesh = meshes[iMesh];
                int[] adjacency = adjacencies[iMesh];

                adjacency.CopyTo(result, BaseFace*3);
                for(int iEdge = BaseFace*3; iEdge < (BaseFace*3+adjacency.Length); iEdge++)
                {
                    if(result[iEdge] != -1)
                        result[iEdge] += BaseFace;
                }
        
                BaseFace += mesh.NumberFaces;
            }

            System.Diagnostics.Debug.Assert(BaseFace*3 == result.Length);

            return result;
        }
    }
}
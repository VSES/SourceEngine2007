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

    public class OpsContext
    {
        private Device device=null;
        public Device Device { get{ return device; } }

        private HybridDictionary ModelTable= new HybridDictionary();
        public ArrayList FindModels( string regStr )
        {
            ArrayList result = new ArrayList();

            Regex reg = null;
            try
            {
                reg = new Regex(regStr, RegexOptions.IgnoreCase);
            }
            catch(Exception e)
            {
                throw new OpsException("Invalid regular expression used to find models", e);
            }

            foreach( OpsModel model in ModelTable.Values)
            {
                if(reg.IsMatch(model.Name))
                    result.Add(model);
            }

            return result;
        }

        public void AddModel( OpsModel model ){ ModelTable.Add(model.Name, model); }
        public void RemoveModel( string name ){ ModelTable.Remove(name); }
        public bool HasModel( string name ){ return ModelTable.Contains(name); }
        public OpsModel GetModel( string name ){ return ModelTable[name] as OpsModel; }
            
        private HybridDictionary TextureTable= new HybridDictionary();
        public ArrayList FindTextures( string regStr )
        {
            ArrayList result = new ArrayList();

            Regex reg = null;
            try
            {
                reg = new Regex(regStr, RegexOptions.IgnoreCase);
            }
            catch(Exception e)
            {
                throw new OpsException("Invalid regular expression used to find textures", e);
            }

            foreach( OpsTexture texture in TextureTable.Values)
            {
                if(reg.IsMatch(texture.Name))
                    result.Add(texture);
            }

            return result;
        }

        public void AddTexture( OpsTexture texture ){ TextureTable.Add(texture.Name, texture); }        
        public void RemoveTexture( string name ){ TextureTable.Remove(name); }
        public bool HasTexture( string name ){ return TextureTable.Contains(name); }
        public OpsTexture GetTexture( string name ){ return TextureTable[name] as OpsTexture; }

        public OpsContext(Device d3dDevice)
        {
            device = d3dDevice;

            Reset();
        }


        public void Reset()
        {        
            ModelTable.Clear();        
            TextureTable.Clear();
        }    

    }



        
    public class OpsModel : IOpsConsoleSubHeader
    {
        public string Name = null;
        public Frame HierarchyRoot = null; 
        public AnimationController AnimationMixer = null;

        public IEnumerable GetFrameEnumerator() { return new OpsFrameEnumerator(HierarchyRoot); }
        public IEnumerable GetMeshEnumerator() { return new OpsMeshEnumerator(HierarchyRoot); }

        public string GetConsoleSubHeader() { return string.Format("[Model:{0,-19}]", Name); }

        public static OpsModel FromFile( Device device, string path)
        {
            OpsModel result = new OpsModel();

            OpsAllocateHierarchy alloc = new OpsAllocateHierarchy();

            AnimationRootFrame arf= Mesh.LoadHierarchyFromFile( path, MeshFlags.SystemMemory|MeshFlags.Use32Bit, device, alloc, null);

            result.HierarchyRoot = arf.FrameHierarchy;
            result.AnimationMixer = arf.AnimationController;

            result.Name = System.IO.Path.GetFileNameWithoutExtension(path);

            return result;
        }


        private class OpsFrameEnumerator : IEnumerator, IEnumerable
        {
            private Frame Root = null;
            private Stack FrameStack = new Stack();
            private bool End = false;
                
            public OpsFrameEnumerator(Frame root)
            {
                Root = root;
                Reset();
            }

            public IEnumerator GetEnumerator(){ return this; }

            public void Reset()
            {
                FrameStack.Clear();
                End = false;
            }

            public bool MoveNext()
            {
                if(End)
                    return false;

                if(FrameStack.Count == 0)
                {
                    FrameStack.Push(Root);
                    return true;
                }

                while(FrameStack.Count > 0)
                {
                    Frame top = FrameStack.Peek() as Frame;

                    if(top.FrameFirstChild != null)
                    {
                        FrameStack.Push(top.FrameFirstChild);
                        return true;
                    }
                    else 
                    {
                        while(FrameStack.Count > 0)
                        {
                            top = FrameStack.Peek() as Frame;
                            FrameStack.Pop();

                            if(top.FrameSibling != null)
                            {
                                FrameStack.Push(top.FrameSibling);
                                return true;
                            }
                        }
                    }
                }

                End = true;
                return false;
            }


            public object Current
            {
                get
                {
                    return FrameStack.Peek();
                }
            }

        }

        private class OpsMeshEnumerator : IEnumerator, IEnumerable
        {
            private OpsFrameEnumerator FrameTracker = null;
            private MeshContainer MeshTracker = null;
            private bool End = false;
                
            public OpsMeshEnumerator(Frame root)
            {
                FrameTracker = new OpsFrameEnumerator(root);
                Reset();
            }

            public IEnumerator GetEnumerator(){ return this; }

            public void Reset()
            {
                FrameTracker.Reset();
                MeshTracker = null;
                End = false;
            }


            public bool MoveNext()
            {
                if(End)
                    return false;

                if( MeshTracker != null)
                {
                    MeshTracker = MeshTracker.NextContainer;
                    if(MeshTracker != null)
                        return true;
                }

                while(FrameTracker.MoveNext())
                {
                    Frame frame = FrameTracker.Current as Frame;
                    MeshTracker = frame.MeshContainer;
                    if(MeshTracker != null)
                        return true;
                }

                End = true;
                return false;
            }


            public object Current
            {
                get
                {
                    return MeshTracker;
                }
            }

        }


        private class OpsAllocateHierarchy : AllocateHierarchy
        {
            //AllocateHierarchy
            public override Frame CreateFrame(string name)
            { 
                OpsFrame frame= new OpsFrame();
                frame.Name = name;
                        
                return frame;
            }

            //AllocateHierarchy
            public override MeshContainer CreateMeshContainer(
                string name,
                MeshData meshData,
                ExtendedMaterial[] materials,
                EffectInstance[] effectInstances,
                GraphicsStream adjacency,
                SkinInformation skinInfo)
            { 
                if(meshData.Mesh == null )
                    throw new OpsException("Expecting Microsoft.DirectX.Direct3D.Mesh not Microsoft.DirectX.Direct3D.ProgressiveMesh or Microsoft.DirectX.Direct3D.PatchMesh.");

                OpsMeshContainer mc= new OpsMeshContainer();
                mc.Name= name;
                mc.MeshData= meshData;
                mc.SetMaterials(materials);
                mc.SetEffectInstances(effectInstances);
                mc.SetAdjacency(adjacency);
                mc.SkinInformation = skinInfo;

                return mc;
            }    
        }
    }

    public class OpsFrame : Frame, IOpsConsoleSubHeader
    {
        public OpsFrame() : base()
        {
            this.TransformationMatrix = Matrix.Identity;
        }

        public Matrix LocalToWorld = Matrix.Identity;
            
        public string GetConsoleSubHeader() { return string.Format("[Frame:{0,-20}]", Name); }

        public string FriendlyName(OpsModel model)
        {
            return String.Format("frame: {0} in model: {1}",
                ((Name == null || Name.Length == 0) ? "Unnamed" : Name),
                model.Name);
        }

        public static void ComputeLocalToWorld( OpsFrame root, Matrix ParentWorld )
        {
            root.LocalToWorld = root.TransformationMatrix * ParentWorld;

            for( OpsMeshContainer mc = root.MeshContainer as OpsMeshContainer; 
                mc != null; 
                mc = mc.NextContainer as OpsMeshContainer )
            {
                mc.LocalToWorld = root.LocalToWorld;
            }

            for( OpsFrame f = root.FrameFirstChild as OpsFrame; 
                f != null; 
                f = f.FrameSibling as OpsFrame )
            {
                ComputeLocalToWorld( f, root.LocalToWorld );
            }
        }

    }
        
    public class OpsMeshContainer : MeshContainer, IOpsConsoleSubHeader
    {
        public OpsMeshContainer() : base()
        {
        }
            
        public Matrix LocalToWorld = Matrix.Identity;

        public string GetConsoleSubHeader() { return string.Format("[Mesh:{0,-20}]", Name); }
        
        public string FriendlyName(OpsModel model)
        {
            return String.Format("mesh: {0} in model: {1}",
                ((Name == null || Name.Length == 0) ? "Unnamed" : Name),
                model.Name);
        }

        public void RemapSkin( GraphicsStream vertexRemapStream )
        {
            if(SkinInformation != null && vertexRemapStream != null)
            {
                int[] vertexRemap= vertexRemap= vertexRemapStream.Read( typeof(int), new int[]{ MeshData.Mesh.NumberVertices } ) as int[];
                SkinInformation.Remap(vertexRemap);
            }
        }

        public void RemapSkin( int[] vertexRemap )
        {
            if(SkinInformation != null && vertexRemap != null)
            {
                SkinInformation.Remap(vertexRemap);
            }
        }

        public void ReplaceMesh(Mesh mesh)
        {
            if(mesh == null)
                throw new OpsException();

            MeshData md = new MeshData();
            md.Mesh = mesh;
            //Disposing can cause double release problems
            //MeshData.Mesh.Dispose();
            MeshData = md;
        }

    }
     
    public class OpsTexture : IOpsConsoleSubHeader
    {
        public string Name = null;
        public bool SRGB = false;
        public BaseTexture Texture;

        public string GetConsoleSubHeader() { return string.Format("[Texture:{0,-17}]", Name); }
        
    }

}
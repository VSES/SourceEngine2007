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

namespace Microsoft.DirectX.Solutions.DxOps
{
    public struct OpsFormatHelper
    {
        public Format Format;
        public int SizeInBytes;



        public static OpsFormatHelper FindByFormat(Format format)
        {
            foreach (OpsFormatHelper helper in FormatMap)
            {
                if(helper.Format == format)
                    return helper;
            }

            throw new OpsException("Unsupported file format: " + format.ToString() );
        }

        public static OpsFormatHelper FindByFormat(string formatStr)
        {
            foreach (OpsFormatHelper helper in FormatMap)
            {
               if (0 == string.Compare(helper.Format.ToString(), formatStr, true))
                   return helper;
            }

            throw new OpsException("Unsupported file format: " + formatStr);
        }

        private OpsFormatHelper(Format format, int sizeInBytes)
        {
            this.Format = format;
            this.SizeInBytes = sizeInBytes;
        }

        private static OpsFormatHelper[] FormatMap = new OpsFormatHelper[]
        {
            new OpsFormatHelper( Format.A16B16G16R16, 8), 
            new OpsFormatHelper( Format.A16B16G16R16F, 8),
            new OpsFormatHelper( Format.A1R5G5B5, 2), 
            new OpsFormatHelper( Format.A2B10G10R10, 4), 
            new OpsFormatHelper( Format.A2R10G10B10, 4),
            new OpsFormatHelper( Format.A2W10V10U10, 4), 
            new OpsFormatHelper( Format.A32B32G32R32F, 16), 
            new OpsFormatHelper( Format.A4L4, 1), 
            new OpsFormatHelper( Format.A4R4G4B4, 2), 
            new OpsFormatHelper( Format.A8, 1), 
            new OpsFormatHelper( Format.A8B8G8R8, 4), 
            new OpsFormatHelper( Format.A8L8, 2), 
            new OpsFormatHelper( Format.A8R3G3B2, 2), 
            new OpsFormatHelper( Format.A8R8G8B8, 4), 
            new OpsFormatHelper( Format.Dxt1, 0), 
            new OpsFormatHelper( Format.Dxt2, 0), 
            new OpsFormatHelper( Format.Dxt3, 0), 
            new OpsFormatHelper( Format.Dxt4, 0), 
            new OpsFormatHelper( Format.Dxt5, 0),
            new OpsFormatHelper( Format.G16R16, 4), 
            new OpsFormatHelper( Format.G16R16F, 4), 
            new OpsFormatHelper( Format.G32R32F, 8), 
            new OpsFormatHelper( Format.L16, 2), 
            new OpsFormatHelper( Format.L8, 1), 
            new OpsFormatHelper( Format.R16F, 2), 
            new OpsFormatHelper( Format.R32F, 4), 
            new OpsFormatHelper( Format.R3G3B2, 1),
            new OpsFormatHelper( Format.R5G6B5, 2), 
            new OpsFormatHelper( Format.R8G8B8, 3), 
            new OpsFormatHelper( Format.X1R5G5B5, 2), 
            new OpsFormatHelper( Format.X4R4G4B4, 2), 
            new OpsFormatHelper( Format.X8B8G8R8, 4), 
            new OpsFormatHelper( Format.X8R8G8B8, 4), 
        };
    }

    public class OpsTextureHelper
    {
        private static void CopyTextureData(GraphicsStream srcData, int srcWidth, int srcHeight, OpsFormatHelper formatHelp, int srcPitch, GraphicsStream dstData, int dstPitch)
        {
            Byte[] rowData = new Byte[srcWidth * formatHelp.SizeInBytes];
            for (int row = 0; row < srcHeight; row++)
            {
                srcData.Position = row * srcPitch;
                dstData.Position = row * dstPitch;

                srcData.Read(rowData, 0, rowData.Length);
                dstData.Write(rowData, 0, rowData.Length);
            }
        }

        public static void LoadVolumeSliceFromSurface( VolumeTexture volumeTex, int mip, int slice, Filter filter, Surface surface)
        {
            VolumeDescription vd = volumeTex.GetLevelDescription(mip);
            OpsFormatHelper formatHelp = OpsFormatHelper.FindByFormat( vd.Format );

            Texture sliceTex = new Texture(volumeTex.Device, vd.Width, vd.Height, 1, Usage.None, formatHelp.Format, Pool.SystemMemory);

            Box box = new Box();
            box.Left = 0;
            box.Right = vd.Width;
            box.Top = 0;
            box.Bottom = vd.Height;
            box.Front = slice;
            box.Back = slice + 1;

            SurfaceLoader.FromSurface(sliceTex.GetSurfaceLevel(0), surface, filter, 0);

            int slicePitch;
            GraphicsStream sliceData = sliceTex.LockRectangle(0, LockFlags.ReadOnly, out slicePitch);
          
            LockedBox volumeLB;
            GraphicsStream volumeData = volumeTex.LockBox(mip, box, LockFlags.None, out volumeLB);

            CopyTextureData(sliceData, vd.Width, vd.Height, formatHelp, slicePitch, volumeData, volumeLB.RowPitch);

            sliceTex.UnlockRectangle(0);
            volumeTex.UnlockBox(mip);

            sliceTex.Dispose();
        }




        public static void LoadSurfaceFromVolumeSlice( VolumeTexture volumeTex, int mip, int slice, Filter filter, Surface surface)
        {
            VolumeDescription vd = volumeTex.GetLevelDescription(mip);
            OpsFormatHelper formatHelp = OpsFormatHelper.FindByFormat( vd.Format );

            Texture sliceTex = new Texture(volumeTex.Device, vd.Width, vd.Height, 1, Usage.None, formatHelp.Format, Pool.SystemMemory);

            Box box = new Box();
            box.Left = 0;
            box.Right = vd.Width;
            box.Top = 0;
            box.Bottom = vd.Height;
            box.Front = slice;
            box.Back = slice + 1;
            
            LockedBox volumeLB;
            GraphicsStream volumeData = volumeTex.LockBox(0, box, LockFlags.ReadOnly, out volumeLB);

            int slicePitch;
            GraphicsStream sliceData = sliceTex.LockRectangle(mip, LockFlags.None, out slicePitch);

            CopyTextureData(volumeData, vd.Width, vd.Height, formatHelp, volumeLB.RowPitch, sliceData, slicePitch);

            sliceTex.UnlockRectangle(0);
            volumeTex.UnlockBox(mip);

            SurfaceLoader.FromSurface(surface, sliceTex.GetSurfaceLevel(0), filter, 0);

            sliceTex.Dispose();
         }

        public static int NumMips(int width, int height, int depth)
        {
            int mips = 0;

            while (width != 1 || height != 1 || depth != 1)
            {
                mips++;

                if (width != 1)
                    width >>= 1;
                if (height != 1)
                    height >>= 1;
                if (depth != 1)
                    depth >>= 1;
            }

            return mips;
        }

        public static Texture CloneTexture( Texture oldTexture, Usage usage, Pool pool)
        {
            SurfaceDescription sd = oldTexture.GetLevelDescription(0);

            return CloneTexture(oldTexture, sd.Width, sd.Height, oldTexture.LevelCount, sd.Format, usage, Filter.None, pool);
        }

        public static Texture CloneTexture( Texture oldTexture, int width, int height, int mips, Format format, Usage usage, Filter filter, Pool pool)
        {
            Texture newTexture = new Texture(oldTexture.Device, width, height, mips, usage, format, pool);
            SurfaceLoader.FromSurface(newTexture.GetSurfaceLevel(0), oldTexture.GetSurfaceLevel(0), filter, 0);
            return newTexture;
        }


        public static VolumeTexture CloneVolume( VolumeTexture oldTexture, Usage usage, Pool pool )
        {
            VolumeDescription vd = oldTexture.GetLevelDescription(0);

            return CloneVolume(oldTexture, vd.Width, vd.Height, vd.Depth, oldTexture.LevelCount, vd.Format, usage, Filter.None, pool);
        }

        public static VolumeTexture CloneVolume( VolumeTexture oldTexture, int width, int height, int depth, int mips, Format format, Usage usage, Filter filter, Pool pool )
        {
            VolumeTexture newTexture = new VolumeTexture(oldTexture.Device, width, height, depth, mips, usage, format, pool);
            VolumeLoader.FromVolume(newTexture.GetVolumeLevel(0), oldTexture.GetVolumeLevel(0), filter, 0);
            return newTexture;
        }
        
        
        public static CubeTexture CloneCube( CubeTexture oldTexture, Usage usage, Pool pool)
        {
            SurfaceDescription sd = oldTexture.GetLevelDescription(0);

            return CloneCube(oldTexture, sd.Width, oldTexture.LevelCount, sd.Format, usage, Filter.None, pool);
        }

        public static CubeTexture CloneCube( CubeTexture oldTexture, int size, int mips, Format format, Usage usage, Filter filter , Pool pool)
        {
            CubeTexture newTexture = new CubeTexture(oldTexture.Device, size, mips, usage, format, pool);
            SurfaceLoader.FromSurface(newTexture.GetCubeMapSurface(CubeMapFace.NegativeX,0), oldTexture.GetCubeMapSurface(CubeMapFace.NegativeX,0), filter, 0);
            SurfaceLoader.FromSurface(newTexture.GetCubeMapSurface(CubeMapFace.NegativeY,0), oldTexture.GetCubeMapSurface(CubeMapFace.NegativeY,0), filter, 0);
            SurfaceLoader.FromSurface(newTexture.GetCubeMapSurface(CubeMapFace.NegativeZ,0), oldTexture.GetCubeMapSurface(CubeMapFace.NegativeZ,0), filter, 0);
            SurfaceLoader.FromSurface(newTexture.GetCubeMapSurface(CubeMapFace.PositiveX,0), oldTexture.GetCubeMapSurface(CubeMapFace.PositiveX,0), filter, 0);
            SurfaceLoader.FromSurface(newTexture.GetCubeMapSurface(CubeMapFace.PositiveY,0), oldTexture.GetCubeMapSurface(CubeMapFace.PositiveY,0), filter, 0);
            SurfaceLoader.FromSurface(newTexture.GetCubeMapSurface(CubeMapFace.PositiveZ,0), oldTexture.GetCubeMapSurface(CubeMapFace.PositiveZ,0), filter, 0);    
            return newTexture;
        }

    }




}
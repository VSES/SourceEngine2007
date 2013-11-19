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
    public class MeshDeclarationHelper
    {
        public static readonly int MaxLength = 65;
        public static readonly int MaxUsageIndex = 15;

        public static DeclarationUsage DecodeUsage(string str)
        {
            DeclarationUsage usage;
            if( 0 == String.Compare(str , "BiNormal", true) )
                usage=DeclarationUsage.BiNormal;
            else if( 0 == String.Compare(str , "BlendIndices", true))
                usage=DeclarationUsage.BlendIndices;
            else if( 0 == String.Compare(str , "BlendWeight", true))
                usage=DeclarationUsage.BlendWeight ;
            else if( 0 == String.Compare(str , "Color", true))
                usage=DeclarationUsage.Color;
            else if( 0 == String.Compare(str , "Depth", true))
                usage=DeclarationUsage.Depth;
            else if( 0 == String.Compare(str , "Fog", true))
                usage=DeclarationUsage.Fog;
            else if( 0 == String.Compare(str , "Normal", true))
                usage=DeclarationUsage.Normal;
            else if( 0 == String.Compare(str , "PSize", true))
                usage=DeclarationUsage.PointSize ;
            else if( 0 == String.Compare(str , "Position", true))
                usage=DeclarationUsage.Position;
            else if( 0 == String.Compare(str , "PositionT", true))
                usage=DeclarationUsage.PositionTransformed;
            else if( 0 == String.Compare(str , "Sample", true))
                usage=DeclarationUsage.Sample;
            else if( 0 == String.Compare(str , "Tangent", true))
                usage=DeclarationUsage.Tangent;
            else if( 0 == String.Compare(str , "TessFactor", true))
                usage=DeclarationUsage.TessellateFactor;
            else if( 0 == String.Compare(str , "TexCoord", true))
                usage=DeclarationUsage.TextureCoordinate;
            else
                throw new OpsException("unrecognized usage arg");

            return usage;
        }

        public static DeclarationType DecodeType(string str)
        {
            DeclarationType type;
            if( 0 == String.Compare(str , "Color", true))
                type=DeclarationType.Color;
            else if( 0 == String.Compare(str , "Dec3N", true))
                type=DeclarationType.Dec3N;
            else if( 0 == String.Compare(str , "Float1", true))
                type=DeclarationType.Float1;
            else if( 0 == String.Compare(str , "Float16_4", true))
                type=DeclarationType.Float16Four;
            else if( 0 == String.Compare(str , "Float16_2", true))
                type=DeclarationType.Float16Two;
            else if( 0 == String.Compare(str , "Float2", true))
                type=DeclarationType.Float2;
            else if( 0 == String.Compare(str , "Float3", true))
                type=DeclarationType.Float3;
            else if( 0 == String.Compare(str , "Float4", true))
                type=DeclarationType.Float4;
            else if( 0 == String.Compare(str , "Short2", true))
                type=DeclarationType.Short2;
            else if( 0 == String.Compare(str , "Short2N", true))
                type=DeclarationType.Short2N;
            else if( 0 == String.Compare(str , "Short4", true))
                type=DeclarationType.Short4;
            else if( 0 == String.Compare(str , "Short4N", true))
                type=DeclarationType.Short4N;
            else if( 0 == String.Compare(str , "Ubyte4", true))
                type=DeclarationType.Ubyte4;
            else if( 0 == String.Compare(str , "Ubyte4N", true))
                type=DeclarationType.Ubyte4N;
            else if( 0 == String.Compare(str , "UDec3", true))
                type=DeclarationType.UDec3;
            else if( 0 == String.Compare(str , "Unused", true))
                type=DeclarationType.Unused;
            else if( 0 == String.Compare(str , "UShort2N", true))
                type=DeclarationType.UShort2N;
            else if( 0 == String.Compare(str , "UShort4N", true))
                type=DeclarationType.UShort4N;
            else
                throw new OpsException("unrecognized type arg");

            return type;
        }

        public static int GetTypeSize(DeclarationType type)
        {
            int size;
            switch(type)
            {
                case DeclarationType.Color:
                    size=4;
                    break;
                case DeclarationType.Dec3N:
                    size=4;
                    break;
                case DeclarationType.Float1:
                    size=4;
                    break;
                case DeclarationType.Float16Four:
                    size=8;
                    break;
                case DeclarationType.Float16Two:
                    size=4;
                    break;
                case DeclarationType.Float2:
                    size=8;
                    break;
                case DeclarationType.Float3:
                    size=12;
                    break;
                case DeclarationType.Float4:
                    size=16;
                    break;
                case DeclarationType.Short2:
                    size=4;
                    break;
                case DeclarationType.Short2N:
                    size=4;
                    break;
                case DeclarationType.Short4:
                    size=8;
                    break;
                case DeclarationType.Short4N:
                    size=8;
                    break;
                case DeclarationType.Ubyte4:
                    size=4;
                    break;
                case DeclarationType.Ubyte4N:
                    size=4;
                    break;
                case DeclarationType.UDec3:
                    size=4;
                    break;
                case DeclarationType.Unused:
                    size=0;
                    break;
                case DeclarationType.UShort2N:
                    size=4;
                    break;
                case DeclarationType.UShort4N:
                    size=8;
                    break;
                default:
                    throw new OpsException("unrecognized type arg");
            }
            return size;
        }

        public static int GetLength(VertexElement[] decl)
        {
            for(int i = 0; i < decl.Length; i++)
            {
                if( decl[i].Stream == (byte)255 )
                    return i;
            }

            throw new OpsException("Unterminated declaration.");
        }

        public static bool FindElement(VertexElement[] decl, DeclarationUsage usage, int usageIdx, out int found)
        {
            for(int iExists = 0; decl[iExists].Stream != 255 && iExists < decl.Length; iExists++)
            {
                if( decl[iExists].DeclarationUsage == usage && decl[iExists].UsageIndex == usageIdx )
                {
                    found = iExists;
                    return true;
                }
            }    

            found = -1;
            return false;
        }    

        public static bool FindValidUsageIndex(VertexElement[] decl, DeclarationUsage usage, out int usageIndex, out int insertBefore)
        {
            int length= GetLength(decl);

            usageIndex = 0;
            insertBefore = length;

            for(int iExists = 0; iExists < length; iExists++)
            {
                if( decl[iExists].DeclarationUsage == usage && decl[iExists].UsageIndex >= usageIndex )
                {
                    insertBefore = iExists+1;
                    usageIndex = decl[iExists].UsageIndex+1;
                }
            }    

            return (usageIndex < MaxUsageIndex+1);
        }
    
        public static bool FindValidUsageIndex(VertexElement[] decl, DeclarationUsage usage, out int usageIndex)
        {
            int insertBefore;
            return FindValidUsageIndex( decl, usage, out usageIndex, out insertBefore);
        }    
    
        public static VertexElement[] RemoveElement(VertexElement[] decl, int remove)
        {
            VertexElement[] result= new VertexElement[MaxLength];

            short typeSize = (short)GetTypeSize(decl[remove].DeclarationType);

            int iCopyTo = 0; 
            for( int iCopyFrom = 0; decl[iCopyFrom].Stream != 255 && iCopyFrom < decl.Length; iCopyFrom++ )
            {
                VertexElement copyVE = decl[iCopyFrom];
                if(iCopyFrom >= remove)
                {
                    copyVE.Offset -= typeSize;
                }

                if(iCopyFrom != remove)
                {
                    result[iCopyTo] = copyVE;
                    iCopyTo++;
                }
            }
            result[iCopyTo] = VertexElement.VertexDeclarationEnd;
            return result;
        }

        public static VertexElement[] AddElement( VertexElement[] decl, DeclarationType type, DeclarationUsage usage, int usageIdx, out int insertBefore)
        {
            int length = GetLength(decl);
            if( length + 1 > MaxLength)
                throw new OpsException("Exceeded maximum declaration length: " + MaxLength);

            if( usageIdx + 1 > MaxUsageIndex)
                throw new OpsException("Exceeded MaxUsageIndex: " + MaxUsageIndex);

            int found;
            if( FindElement( decl, usage, usageIdx , out found) )
                throw new OpsException("VertexElement with that input usage+index already exists");


            VertexElement[] result= new VertexElement[MaxLength];
            
            insertBefore= length;
            for( int less = 1 ; less <= usageIdx ; less++ )
            {
                if( FindElement( decl, usage, usageIdx-less , out found) )
                {
                    //insertBefore should be 1 after where a previous is found...that way they are consecutive
                    //otherwise we just add it to the end.(see initialization insertBefore=length)
                    insertBefore = found+1;
                    break;
                }
            }

            //MOVE EVERYTHING OUT OF THE WAY FOR THE NEW ELEMENT
            short typeSize = (short)GetTypeSize(type);
            //go to decl.Length-1 so that we cant have room for the added element
            for( int iCopyFrom = 0, iCopyTo = 0; iCopyFrom < decl.Length-1; iCopyFrom++, iCopyTo++ )
            {

                //if we are the new element insert here
                if(iCopyFrom == insertBefore)
                {
                    VertexElement insertVE = new VertexElement();
                    insertVE.Stream = 0;
                    if(insertBefore < length)
                        insertVE.Offset = decl[iCopyFrom].Offset;
                    else if(length > 0)
                        insertVE.Offset = (short)((short)(decl[insertBefore-1].Offset) + (short)GetTypeSize( decl[insertBefore-1].DeclarationType ));
                    else
                        insertVE.Offset = 0;

                    insertVE.DeclarationType = type;
                    insertVE.DeclarationMethod = DeclarationMethod.Default;
                    insertVE.DeclarationUsage = usage;
                    insertVE.UsageIndex = (byte)usageIdx;


                    result[iCopyTo]=insertVE;

                    iCopyTo++;
                }


                VertexElement copyVE = decl[iCopyFrom];
                if(iCopyFrom >= insertBefore)
                {
                    copyVE.Offset += typeSize;
                }

                result[iCopyTo] = copyVE;
            }

            result[length+1]=VertexElement.VertexDeclarationEnd;
            return result;
        }    
    }
}
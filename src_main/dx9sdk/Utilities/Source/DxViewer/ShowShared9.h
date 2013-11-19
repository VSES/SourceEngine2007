//--------------------------------------------------------------------------------------
// Copyright (c) Microsoft Corporation. All rights reserved.
//--------------------------------------------------------------------------------------
#pragma once

struct FaceIndices
{
    union
    {
        struct
        {
            DWORD I0;
            DWORD I1;
            DWORD I2;
        };
    
        DWORD I[3];
    };

    static FaceIndices GetFace(void* pSrcIB, bool b32bit, DWORD iFace)
    {
        DWORD idxSize = b32bit ? sizeof(DWORD) : sizeof(WORD);
        void* pFace = ((BYTE*)pSrcIB) + (idxSize*3*iFace);

        FaceIndices face;
        if(b32bit)
        {
            memcpy( face.I, pFace, 3*sizeof(DWORD) );
        }
        else
        {
            face.I0= ((WORD*)pFace)[0];
            face.I1= ((WORD*)pFace)[1];
            face.I2= ((WORD*)pFace)[2];
        }

        return face;
    }
};

struct FaceAdjacencies
{
    union
    {
        struct
        {
            DWORD E0;
            DWORD E1;
            DWORD E2;
        };
    
        DWORD E[3];
    };
};

struct FaceLines3
{
    union
    {
        struct
        {
            DWORD L0[2];
            DWORD L1[2];
            DWORD L2[2];
        };
    
        DWORD L[6];
    };

};

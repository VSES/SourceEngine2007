//-------------------------------------------------------------------------------------
// Sas.fxh
// Microsoft(R) Standard Annotations and Semantics
// Version 1.0.0
// Standard include file
// Copyright (c) Microsoft Corporation. All rights reserved.
//-------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------
// SasHostParameterValues
// This structure is HLSL pseudocode and will not compile. It's provided here as a
// reference for the values which an application should expose for parameter binding.
//-------------------------------------------------------------------------------------

/*

struct SasHostParameterValues
{
    struct 
    {
        float Now;  // SasBindAddress = "Sas.Time.Now"
        float Last; // SasBindAddress = "Sas.Time.Last", etc...
        int FrameNumber;
    } 
    Time;

    textureCUBE EnvironmentMap; 
    
    SasCamera Camera;

    SasAmbientLight AmbientLight[ZeroOrMore];
    int NumAmbientLights;

    SasAmbientLight DirectionalLight[ZeroOrMore];
    int NumDirectionalLights;

    SasAmbientLight PointLight[ZeroOrMore];
    int NumPointLights;

    SasAmbientLight SpotLight[ZeroOrMore];
    int NumSpotLights;

    struct
    {
        float4x4 WorldToShadow;  
        texture2D ShadowMap; 
    }
    Shadow[ZeroOrMore];
    int NumShadows;
    
    struct
    {
        float4x4 MeshToJointToWorld[OneOrMore];
        int NumJoints;
    }
    Skeleton;
}
Sas;

*/

//-------------------------------------------------------------------------------------
// Macros
//-------------------------------------------------------------------------------------

// The implementation’s shadow_sampler() macro must be
// defined in the standard include as well.
//
// Host applications are permitted to change the 
// implementation of this macro as long as the macro
// evaluates to a sampler_state constructor.
//
#define shadow_sampler(ShadowMap) \
    sampler_state \
    { \
      Texture = (ShadowMap); \
    }


//-------------------------------------------------------------------------------------
// Structures
//-------------------------------------------------------------------------------------

struct SasCamera
{
    float4x4 WorldToView;	   
    float4x4 Projection;    
    float2 NearFarClipping; 
    float3 Position;
};

struct SasAmbientLight
{
    float3 Color;         // default: {0, 0, 0}
};

struct SasDirectionalLight
{
    float3 Color;         // default: {0, 0, 0}
    float3 Direction;     // default: {0, 0, 0}
};

struct SasPointLight
{
    float3 Color;         // default: {0, 0, 0}
    float3 Position;      // default: {0, 0, 0}
    float  Range;         // default: 0
};

struct SasSpotLight
{
    float3 Color;         // default: {0, 0, 0}
    float3 Position;      // default: {0, 0, 0}
    float  Direction;     // default: {0, 0, 0}
    float  Range;         // default: 0
    float  Theta;         // default: 0
    float  Phi;           // default: 0
};


//-------------------------------------------------------------------------------------
// Functions
//-------------------------------------------------------------------------------------

// Sas.fxh provides a set of helper functions for 
// calculating the inverse of a matrix. Non-square
// matrices don’t have a true mathematical inverse, 
// but several of the functions below accept non-square
// matrices as arguments. Non-square matrices are
// appended with an implicit extra row or column 
// vector [0, 0, 0, 1] to create a square matrix. 

 
// The following provide generalized inverse of a matrix.  
// They cannot assume anything about the matrix.

//-------------------------------------------------------------------------------------
float3x3 inverse(float3x3 m)
{
    float3x3 adj;
    
    // Calculate the adjoint matrix
    adj._11 = m._22*m._33 - m._23*m._32;
    adj._12 = m._13*m._32 - m._12*m._33;
    adj._13 = m._12*m._23 - m._13*m._22;
    
    adj._21 = m._23*m._31 - m._21*m._33;
    adj._22 = m._11*m._33 - m._13*m._31;
    adj._23 = m._13*m._21 - m._11*m._23;
    
    adj._31 = m._21*m._32 - m._22*m._31;
    adj._32 = m._12*m._31 - m._11*m._32;
    adj._33 = m._11*m._22 - m._12*m._21;
    
    // Calculate the determinant
    float det = determinant(m);
    
    float3x3 result = 0;
    if( 0.0f != det )
        result = 1.0f/det * adj;
        
    return result;
}

//-------------------------------------------------------------------------------------
float4x4 inverse(float4x4 m)
{
    float4x4 adj;
    
    // Calculate the adjoint matrix
    adj._11 = m._22*m._33*m._44 + m._23*m._34*m._42 + m._24*m._32*m._43 - m._22*m._34*m._43 - m._23*m._32*m._44 - m._24*m._33*m._42;
    adj._12 = m._12*m._34*m._43 + m._13*m._32*m._44 + m._14*m._33*m._42 - m._12*m._33*m._44 - m._13*m._34*m._42 - m._14*m._32*m._43;
    adj._13 = m._12*m._23*m._44 + m._13*m._24*m._42 + m._14*m._22*m._43 - m._12*m._24*m._43 - m._13*m._22*m._44 - m._14*m._23*m._42;
    adj._14 = m._12*m._24*m._33 + m._13*m._22*m._34 + m._14*m._23*m._32 - m._12*m._23*m._34 - m._13*m._24*m._32 - m._14*m._22*m._33;
    
    adj._21 = m._21*m._34*m._43 + m._23*m._31*m._44 + m._24*m._33*m._41 - m._21*m._33*m._44 - m._23*m._34*m._41 - m._24*m._31*m._43;
    adj._22 = m._11*m._33*m._44 + m._13*m._34*m._41 + m._14*m._31*m._43 - m._11*m._34*m._43 - m._13*m._31*m._44 - m._14*m._33*m._41;
    adj._23 = m._11*m._24*m._43 + m._13*m._21*m._44 + m._14*m._23*m._41 - m._11*m._23*m._44 - m._13*m._24*m._41 - m._14*m._21*m._43;
    adj._24 = m._11*m._23*m._34 + m._13*m._24*m._31 + m._14*m._21*m._33 - m._11*m._24*m._33 - m._13*m._21*m._34 - m._14*m._23*m._31;
    
    adj._31 = m._21*m._32*m._44 + m._22*m._34*m._41 + m._24*m._31*m._42 - m._21*m._34*m._42 - m._22*m._31*m._44 - m._24*m._32*m._41;
    adj._32 = m._11*m._34*m._42 + m._12*m._31*m._44 + m._14*m._32*m._41 - m._11*m._32*m._44 - m._12*m._34*m._41 - m._14*m._31*m._42;
    adj._33 = m._11*m._22*m._44 + m._12*m._24*m._41 + m._14*m._21*m._42 - m._11*m._24*m._42 - m._12*m._21*m._44 - m._14*m._22*m._41;
    adj._34 = m._11*m._24*m._32 + m._12*m._21*m._34 + m._14*m._22*m._31 - m._11*m._22*m._34 - m._12*m._24*m._31 - m._14*m._21*m._32;
    
    adj._41 = m._21*m._33*m._42 + m._22*m._31*m._43 + m._23*m._32*m._41 - m._21*m._32*m._43 - m._22*m._33*m._41 - m._23*m._31*m._42;
    adj._42 = m._11*m._32*m._43 + m._12*m._33*m._41 + m._13*m._31*m._42 - m._11*m._33*m._42 - m._12*m._31*m._43 - m._13*m._32*m._41;
    adj._43 = m._11*m._23*m._42 + m._12*m._21*m._43 + m._13*m._22*m._41 - m._11*m._22*m._43 - m._12*m._23*m._41 - m._13*m._21*m._42;
    adj._44 = m._11*m._22*m._33 + m._12*m._23*m._31 + m._13*m._21*m._32 - m._11*m._23*m._32 - m._12*m._21*m._33 - m._13*m._22*m._31;
    
    // Calculate the determinant
    float det = determinant(m);
    
    float4x4 result = 0;
    if( 0.0f != det )
        result = 1.0f/det * adj;
        
    return result;
}

//-------------------------------------------------------------------------------------
float4x3 inverse(float4x3 m)
{
    // Separate the rotation and translation components
    float3x3 R = (float3x3)m;
    float1x3 t = { m._41, m._42, m._43 };
    
    // Find the inverse matrix
    // A0 R0   
    // v1 t1 = I
    
    float3x3 A = inverse( R );
    float1x3 v = -mul( A, t );
    
    return float4x3( A._11, A._12, A._13,
                     A._21, A._22, A._23,
                     A._31, A._32, A._33,
                     v._11, v._12, v._13 );
}

//-------------------------------------------------------------------------------------
float3x4 inverse(float3x4 m)
{
    // Separate the rotation and translation components
    float3x3 R = (float3x3)m;
    float3x1 t = { m._14, m._24, m._34 };
    
    // Find the inverse matrix
    // Av Rt   
    // 01 01 = I
    
    float3x3 A = inverse( R );
    float3x1 v = -mul( A, t );
    
    return float3x4( A._11, A._12, A._13, v._11,
                     A._21, A._22, A._23, v._21,
                     A._31, A._32, A._33, v._31 );
}


// the following provide faster versions of inverse
// that assume the columns/rows of the rotation submatrix 
// are orthogonal.


//-------------------------------------------------------------------------------------
float3x3 inverse_orthogonal(float3x3 m)
{
    float length0 = length(m[0]);
    float length1 = length(m[1]);
    float length2 = length(m[2]);
    
    m[0] /= length0 * length0;
    m[1] /= length1 * length1;
    m[2] /= length2 * length2;
    
    return transpose(m);
}


//-------------------------------------------------------------------------------------
float4x3 inverse_orthogonal(float4x3 m)
{
    // Separate the rotation and translation components
    float3x3 R = (float3x3)m;
    float1x3 t = { m._41, m._42, m._43 };
    
    // Find the inverse matrix
    // A0 R0   
    // v1 t1 = I
    
    float3x3 A = inverse_orthogonal( R );
    float1x3 v = -mul( A, t );
    
    return float4x3( A._11, A._12, A._13,
                     A._21, A._22, A._23,
                     A._31, A._32, A._33,
                     v._11, v._12, v._13 );
}

//-------------------------------------------------------------------------------------
float3x4 inverse_orthogonal(float3x4 m)
{
    // Separate the rotation and translation components
    float3x3 R = (float3x3)m;
    float3x1 t = { m._14, m._24, m._34 };
    
    // Find the inverse matrix
    // Av Rt   
    // 01 01 = I
    
    float3x3 A = inverse_orthogonal( R );
    float3x1 v = -mul( A, t );
    
    return float3x4( A._11, A._12, A._13, v._11,
                     A._21, A._22, A._23, v._21,
                     A._31, A._32, A._33, v._31 );
}


// the following provide faster versions of inverse
// that assume the columns/rows of the rotation submatrix 
// are orthonormal.

//-------------------------------------------------------------------------------------
float3x3 inverse_orthonormal(float3x3 m)
{
    return transpose(m);
}


//-------------------------------------------------------------------------------------
float4x3 inverse_orthonormal(float4x3 m)
{
    // Separate the rotation and translation components
    float3x3 R = (float3x3)m;
    float1x3 t = { m._41, m._42, m._43 };
    
    // Find the inverse matrix
    // A0 R0   
    // v1 t1 = I
    
    float3x3 A = inverse_orthonormal( R );
    float1x3 v = -mul( A, t );
    
    return float4x3( A._11, A._12, A._13,
                     A._21, A._22, A._23,
                     A._31, A._32, A._33,
                     v._11, v._12, v._13 );
}

//-------------------------------------------------------------------------------------
float3x4 inverse_orthonormal(float3x4 m)
{
    // Separate the rotation and translation components
    float3x3 R = (float3x3)m;
    float3x1 t = { m._14, m._24, m._34 };
    
    // Find the inverse matrix
    // Av Rt   
    // 01 01 = I
    
    float3x3 A = inverse_orthonormal( R );
    float3x1 v = -mul( A, t );
    
    return float3x4( A._11, A._12, A._13, v._11,
                     A._21, A._22, A._23, v._21,
                     A._31, A._32, A._33, v._31 );
}


// The implementation’s shadow() sampling function must 
// be defined in the standard include as well.
//-------------------------------------------------------------------------------------
float shadow( sampler ShadowSampler, float4x4 WorldToShadow, float3 WorldPoint)
{
    return 0.0; 
}

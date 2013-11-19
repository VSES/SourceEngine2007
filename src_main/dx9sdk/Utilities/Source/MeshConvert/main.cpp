//----------------------------------------------------------------------------
// File: main.cpp
//
// Copyright (c) Microsoft Corp. All rights reserved.
//-----------------------------------------------------------------------------
#include "stdafx.h"

//-----------------------------------------------------------------------------
struct SETTINGS
{
    bool bVerbose;
    bool bInputFileProvided;
    bool bOutputFileProvided;
    bool bOverwrite;
    bool bGenTangents;
    bool bGenBinormals;
    bool bRetainAnimation;
    bool bNoOpt;
    UINT NumTexCoords;
    MESH_TYPE outputType;
    WCHAR strInputFile[MAX_PATH];
    WCHAR strOutputFile[MAX_PATH];
};


//-----------------------------------------------------------------------------
// Function-prototypes
//-----------------------------------------------------------------------------
bool ParseCommandLine( SETTINGS* pSettings );
IDirect3DDevice9* CreateNULLRefDevice();
void BuildVertexDecl( D3DVERTEXELEMENT9* pDecl, SETTINGS* pSettings );
void DisplayUsage();
bool IsNextArg( WCHAR*& strsettings, WCHAR* strArg );


//-----------------------------------------------------------------------------
// Name: main()
// Desc: Entry point for the application.  We use just the console window
//-----------------------------------------------------------------------------
int main(int argc, char* argv[])
{
    // Enable run-time memory check for debug builds.
#if defined(DEBUG) | defined(_DEBUG)
    _CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
#endif

    int nRet = 0;
    IDirect3DDevice9* pd3dDevice = NULL;
    SETTINGS settings;
    ZeroMemory( &settings, sizeof(SETTINGS) );
    settings.NumTexCoords = 1;

    if( argc < 2 )
    {
        DisplayUsage();
        goto LCleanup;
    }

    if( !ParseCommandLine( &settings ) )
    {
        nRet = 0;
        goto LCleanup;
    }

    if( settings.bVerbose )
    {
        wprintf( L"Input file: %s\n", settings.strInputFile );   
        wprintf( L"Output file: %s\n", settings.strOutputFile );   
        wprintf( L"Overwrite: %d\n", settings.bOverwrite );   
        switch( settings.outputType )
        {
            case MESH_TYPE_X_TEXT: wprintf( L"Output format: .x text\n" ); break;
            case MESH_TYPE_X_BINARY: wprintf( L"Output format: .x binary\n" ); break;
            case MESH_TYPE_SDKMESH: wprintf( L"Output format: sdkmesh\n" ); break;
        }
    }

    // Create NULLREF device 
    pd3dDevice = CreateNULLRefDevice(); 
    if( pd3dDevice == NULL )
    {
        wprintf( L"Error: Can not create NULLREF Direct3D device\n" );
        nRet = 1;
        goto LCleanup;
    }
    else
    {
        if( settings.bVerbose )
        {
            wprintf( L"NULLREF Direct3D 9 device created\n" );   
        }

        // Load the mesh
        CLoader loader;
        if( FAILED(loader.Load( settings.strInputFile, FTT_RELATIVE )) )
        {
            wprintf( L"Cannot Load specified input file\n" );
            goto LCleanup;
            nRet = 1;
        }

        // Convert to Little Endian
        loader.GetMesh()->ConvertToLittleEndian();

        // Create a decl that reflects our inputs
        D3DVERTEXELEMENT9 decl[MAX_VERTEX_ELEMENTS];
        BuildVertexDecl( decl, &settings );

        // Convert the mesh to this decl
        loader.GetMesh()->ConvertVertexData( pd3dDevice, decl, MAX_VERTEX_ELEMENTS );

        // fix up the mesh
        loader.GetMesh()->FixMesh();

        // if we need tangents or binormals, generate them
        if( settings.bGenTangents || settings.bGenBinormals )
        {
            if( !loader.GetMesh()->GenerateTangents( pd3dDevice ) )
                wprintf( L"Warning:  Cannot generate tangent frame information for mesh\n" );
        }

        // Save it out
        if( FAILED( loader.Save( pd3dDevice, settings.strOutputFile, settings.outputType ) ) )
        {
            wprintf( L"Cannot Save specified output file\n" );
            goto LCleanup;
            nRet = 1;
        }

        if( settings.bRetainAnimation )
        {
            WCHAR szOutput[MAX_PATH];
            StringCchCopy( szOutput, MAX_PATH, settings.strOutputFile );
            StringCchCat( szOutput, MAX_PATH, L"_anim" );
            if( FAILED( loader.SaveAnimationData( szOutput ) ) )
            {
                wprintf( L"Cannot Save animation data\n" );
            }
        }
    }

LCleanup:
    wprintf( L"\n" );

#ifdef DEBUG_PRESSKEY
    wprintf( L"Press ENTER to continue.");
    _getch();
#endif 

    // Cleanup
    SAFE_RELEASE( pd3dDevice );

    return nRet;
}

//--------------------------------------------------------------------------------------
// Parses the command line for parameters.  See DXUTInit() for list 
//--------------------------------------------------------------------------------------
bool ParseCommandLine( SETTINGS* pSettings )
{
    bool bDisplayError = false;
    bool bDisplayHelp = false;
    WCHAR* strCmdLine;
    WCHAR* strArg;

    int nNumArgs;
    WCHAR** pstrArgList = CommandLineToArgvW( GetCommandLine(), &nNumArgs );
    for( int iArg=1; iArg<nNumArgs; iArg++ )
    {
        strCmdLine = pstrArgList[iArg];

        // Handle flag args
        if( *strCmdLine == L'/' || *strCmdLine == L'-' )
        {
            strCmdLine++;

            if( IsNextArg( strCmdLine, L"y" ) )
            {
                pSettings->bOverwrite = true;
                continue;
            }

            if( IsNextArg( strCmdLine, L"t" ) )
            {
                pSettings->bGenTangents = true;
                continue;
            }

            if( IsNextArg( strCmdLine, L"tb" ) )
            {
                pSettings->bGenTangents = true;
                pSettings->bGenBinormals = true;
                continue;
            }

            if( IsNextArg( strCmdLine, L"tcount" ) )
            {
                if( iArg+1 < nNumArgs )
                {
                    strArg = pstrArgList[++iArg];
                    pSettings->NumTexCoords = _wtoi(strArg);
                    continue;
                }

                wprintf( L"Incorrect flag usage: /tcount\n" );
                bDisplayError = true;
                continue;
            }

            if( IsNextArg( strCmdLine, L"a" ) )
            {
                pSettings->bRetainAnimation = true;
                continue;
            }

            if( IsNextArg( strCmdLine, L"v" ) )
            {
                pSettings->bVerbose = true;
                continue;
            }

            if( IsNextArg( strCmdLine, L"xt" ) )
            {
                pSettings->outputType = MESH_TYPE_X_TEXT;
                continue;
            }

            if( IsNextArg( strCmdLine, L"x" ) )
            {
                pSettings->outputType = MESH_TYPE_X_BINARY;
                continue;
            }

            if( IsNextArg( strCmdLine, L"sdkmesh" ) )
            {
                pSettings->outputType = MESH_TYPE_SDKMESH;
                continue;
            }

            if( IsNextArg( strCmdLine, L"o" ) )
            {
                if( iArg+1 < nNumArgs )
                {
                    strArg = pstrArgList[++iArg];
                    pSettings->bOutputFileProvided = true;
                    StringCchCopy( pSettings->strOutputFile, 256, strArg );
                    continue;
                }

                wprintf( L"Incorrect flag usage: /o\n" );
                bDisplayError = true;
                continue;
            }

            if( IsNextArg( strCmdLine, L"?" ) )
            {
                DisplayUsage();
                return false;
            }

            // Unrecognized flag
            wprintf( L"Unrecognized or incorrect flag usage: %s\n", strCmdLine );
            bDisplayError = true;
        }
        else
        {
            // Handle non-flag args as seperate input file
            if( pSettings->bInputFileProvided )
            {
                wprintf( L"Too many input files provided: %s\n", strCmdLine );
                bDisplayError = true;
            }
            else
            {
                pSettings->bInputFileProvided = true;
                StringCchCopy( pSettings->strInputFile, 256, strCmdLine );
                continue;
            }
        }
    }

    if( !pSettings->bInputFileProvided )
        bDisplayHelp = true;

    if( pSettings->bInputFileProvided && !pSettings->bOutputFileProvided )
    {
        StringCchCopy( pSettings->strOutputFile, MAX_PATH, pSettings->strInputFile );

    }

    if( bDisplayHelp )
    {
        DisplayUsage();
        return false;
    }

    if( bDisplayError )
    {
        wprintf( L"Type \"MeshConvert.exe /?\" for a complete list of options\n" );
        return false;
    }

    return true;
}


//--------------------------------------------------------------------------------------
bool IsNextArg( WCHAR*& strCmdLine, WCHAR* strArg )
{
    int nArgLen = (int) wcslen(strArg);
    if( _wcsnicmp( strCmdLine, strArg, nArgLen ) == 0 && strCmdLine[nArgLen] == 0 )
        return true;

    return false;
}


//--------------------------------------------------------------------------------------
IDirect3DDevice9* CreateNULLRefDevice()
{
    HRESULT hr;
    IDirect3D9* pD3D = Direct3DCreate9( D3D_SDK_VERSION );
    if( NULL == pD3D )
        return NULL;

    D3DDISPLAYMODE Mode;
    pD3D->GetAdapterDisplayMode(0, &Mode);

    D3DPRESENT_PARAMETERS pp;
    ZeroMemory( &pp, sizeof(D3DPRESENT_PARAMETERS) ); 
    pp.BackBufferWidth  = 1;
    pp.BackBufferHeight = 1;
    pp.BackBufferFormat = Mode.Format;
    pp.BackBufferCount  = 1;
    pp.SwapEffect       = D3DSWAPEFFECT_COPY;
    pp.Windowed         = TRUE;

    IDirect3DDevice9* pd3dDevice;
    hr = pD3D->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_NULLREF, GetConsoleWindow(), 
                             D3DCREATE_HARDWARE_VERTEXPROCESSING, &pp, &pd3dDevice );
    SAFE_RELEASE( pD3D );
    if( FAILED(hr) || pd3dDevice == NULL )
        return NULL;

    return pd3dDevice;
}

//--------------------------------------------------------------------------------------
void BuildVertexDecl( D3DVERTEXELEMENT9* pDecl, SETTINGS* pSettings )
{
    UINT index = 0;
    WORD offset = 0;

    //Position
    pDecl[index].Stream = 0;
    pDecl[index].Offset = offset;
    pDecl[index].Type = D3DDECLTYPE_FLOAT3;
    pDecl[index].Method = D3DDECLMETHOD_DEFAULT;
    pDecl[index].Usage = D3DDECLUSAGE_POSITION;
    pDecl[index].UsageIndex = 0;
    index ++;
    offset += 12;

    if( pSettings->bRetainAnimation )
    {
        //BlendWeight
        pDecl[index].Stream = 0;
        pDecl[index].Offset = offset;
        pDecl[index].Type = D3DDECLTYPE_UBYTE4N;
        pDecl[index].Method = D3DDECLMETHOD_DEFAULT;
        pDecl[index].Usage = D3DDECLUSAGE_BLENDWEIGHT;
        pDecl[index].UsageIndex = 0;
        index ++;
        offset += 4;

        //BlendIndices
        pDecl[index].Stream = 0;
        pDecl[index].Offset = offset;
        pDecl[index].Type = D3DDECLTYPE_UBYTE4;
        pDecl[index].Method = D3DDECLMETHOD_DEFAULT;
        pDecl[index].Usage = D3DDECLUSAGE_BLENDINDICES;
        pDecl[index].UsageIndex = 0;
        index ++;
        offset += 4;
    }

    //Normal
    pDecl[index].Stream = 0;
    pDecl[index].Offset = offset;
    pDecl[index].Type = D3DDECLTYPE_FLOAT3;
    pDecl[index].Method = D3DDECLMETHOD_DEFAULT;
    pDecl[index].Usage = D3DDECLUSAGE_NORMAL;
    pDecl[index].UsageIndex = 0;
    index ++;
    offset += 12;

    for( UINT i=0; i<pSettings->NumTexCoords; i++ )
    {
        //Texcoord
        pDecl[index].Stream = 0;
        pDecl[index].Offset = offset;
        pDecl[index].Type = D3DDECLTYPE_FLOAT2;
        pDecl[index].Method = D3DDECLMETHOD_DEFAULT;
        pDecl[index].Usage = D3DDECLUSAGE_TEXCOORD;
        pDecl[index].UsageIndex = (BYTE)i;
        index ++;
        offset += 8;
    }

    if( pSettings->bGenTangents )
    {
        //Tangent
        pDecl[index].Stream = 0;
        pDecl[index].Offset = offset;
        pDecl[index].Type = D3DDECLTYPE_FLOAT3;
        pDecl[index].Method = D3DDECLMETHOD_DEFAULT;
        pDecl[index].Usage = D3DDECLUSAGE_TANGENT;
        pDecl[index].UsageIndex = 0;
        index ++;
        offset += 12;
    }

    if( pSettings->bGenBinormals )
    {
        //Binormals
        pDecl[index].Stream = 0;
        pDecl[index].Offset = offset;
        pDecl[index].Type = D3DDECLTYPE_FLOAT3;
        pDecl[index].Method = D3DDECLMETHOD_DEFAULT;
        pDecl[index].Usage = D3DDECLUSAGE_BINORMAL;
        pDecl[index].UsageIndex = 0;
        index ++;
        offset += 12;
    }

    //declend
    pDecl[index].Stream = 0xFF;
    pDecl[index].Offset = 0;
    pDecl[index].Type = D3DDECLTYPE_UNUSED;
    pDecl[index].Method = D3DDECLMETHOD_DEFAULT;
    pDecl[index].Usage = 0;
    pDecl[index].UsageIndex = 0;
}
//--------------------------------------------------------------------------------------
void DisplayUsage()
{
//    wprintf(APP_FULLNAME);
//    printf(APP_VERSION);
#ifdef DEBUG
    wprintf(L" DEBUG");
#endif

    wprintf(L"\n");
    wprintf(L"Usage: " APP_NAME L" <options> <input filename>\n");
    wprintf(L"\n");
    wprintf(L"   <input filename>    Input mesh filename.\n");
    wprintf(L"                       The input file can be of .x, .obj, or .sdkmesh format\n");    
    wprintf(L"   /o <file>           Optional output mesh filename\n");    
    wprintf(L"   /y                  Overwrite existing destination file\n");
    wprintf(L"   /t                  Generate tangents\n");
    wprintf(L"   /tb                 Generate tangents and binormals\n");
    wprintf(L"   /tcount <count>     Texcoord count for the output mesh\n");
    wprintf(L"   /a                  Output animation information\n");
    wprintf(L"   /op				 Vertex cache optimize the mesh before exporting (non-functional)\n");
    wprintf(L"   /sdkmesh            [default] convert to .sdkmesh binary file\n");  
    wprintf(L"   /x                  convert to .x binary file\n");    
    wprintf(L"   /xt                 convert to .x text file\n");    
    wprintf(L"   /v                  Verbose\n");    
    wprintf(L"\n");
    wprintf(L".SDKMesh is a simple custom mesh format used by new SDK samples\n");
}



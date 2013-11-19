#include "windows.h"
#include "htmlhelp.h"
#pragma warning(disable: 4995)
#include <strsafe.h>
#include <shlwapi.h>
#include <shlobj.h>
#pragma warning(default: 4995)
#include "d3d9.h"
#include "d3dx9.h"
#include "DXUT.h"
#include "DXUTenum.h"
#include <malloc.h>

#include "resource.h"
#include "App.h"
#include "StateBlock.h"
#include "Render.h"
#include "Render9.h"
#include "Render10.h"
#include "Camera.h"
#include "FileSystem.h"
#include "HUD.h"
#include "MainHUD.h"
#include "ViewHUD.h"
#include "EffectHUD.h"

#define STATUS_HEIGHT 84
#define STATUS_BORDER 4

DECLARE_SINGLETON_CPP( App );

DECLARE_SINGLETON_CPP( SasMsgListener );

INT WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
        // Enable run-time memory check for debug builds.
    #if defined(DEBUG) | defined(_DEBUG)
        _CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
    #endif
        
    return DXVGetApp().Main( hInstance, hPrevInstance, lpCmdLine, nCmdShow );
}



INT App::Main( 
    HINSTANCE hInstance,
    HINSTANCE hPrevInstance,
    LPSTR lpCmdLine,
    int nCmdShow )
{

    bool hasD3D10DLLS = DXUTIsD3D10Available();
#if defined(DEBUG) | defined(_DEBUG)
    HMODULE hD3DX10 = LoadLibrary(L"D3DX10d.DLL");
#else
    HMODULE hD3DX10 = LoadLibrary(L"D3DX10.DLL");
#endif

    STATE_SET( AllowD3D10, (hasD3D10DLLS!=false && hD3DX10!=NULL) );


    
    DXUTInit( true, true ); // Parse the command line, handle the default hotkeys, and show msgboxes
    DXUTSetCursorSettings( true, true );

    if(!CreateMainDialog())
        return FALSE;

    CreateRenderWindow();    
    
    Sas::Host::SetIntegrator( &DXVGetFileFinder() );
    Sas::Host::SetMessageListener( &DXVGetSasMsgListener() );
    //Register Callbacks
    RegisterCallbacks();

    DXVGetRender9().Startup();
    DXVGetRender10().Startup();
    DXVGetHudManager().Startup();
    DXVGetMainHUD().Startup();
    DXVGetViewHUD().Startup();
    DXVGetEffectHUD().Startup();

    DXVGetMainHUD().SetActive(true);


    ForceDeviceChange( STATE_GET(Version) );

    STATE_SET(Initializing, false);

    //Parse CL
    ParseCommandLine(lpCmdLine);


    DXUTMainLoop();

    DXUTShutdown();
    DXVGetViewHUD().Shutdown();
    DXVGetEffectHUD().Shutdown();
    DXVGetMainHUD().Shutdown();
    DXVGetHudManager().Shutdown();
    DXVGetRender9().Shutdown();
    DXVGetRender10().Shutdown();



    return DXUTGetExitCode();
}
void App::Shutdown()
{
    if(! STATE_GET(Initializing) )
        STATE_GET(Render)->Clear();

    Sas::Host::GetRootTable()->Clear();

    DXUTShutdown();
}
void App::ForceDeviceChangeByState()
{ 
    DXVRESET reset = STATE_GET(Reset);
    if(reset == DXVRESET_NONE)
        return;

    DXUTDeviceVersion newVer = STATE_GET(Version);
    if(STATE_GET(Reset) == DXVRESET_SWITCHDEVICE)
    {
        switch( newVer )
        {
        case DXUT_D3D9_DEVICE:
            newVer = DXUT_D3D10_DEVICE;
            break;
        case DXUT_D3D10_DEVICE:
            newVer = DXUT_D3D9_DEVICE;
            break;
        };
    }

    ForceDeviceChange( newVer ); 
}

void App::ForceDeviceChange(DXUTDeviceVersion newVer)
{

    STATE_SET(Version,   newVer );

    switch( newVer )
    {
    case DXUT_D3D9_DEVICE:
        DXUTSetD3DVersionSupport( true, false );
        STATE_SET(Render,   &DXVGetRender9() );
        break;
    case DXUT_D3D10_DEVICE:
        DXUTSetD3DVersionSupport( false, true );
        STATE_SET(Render,   &DXVGetRender10() );
        break;
    };


    DXUTCreateDevice( true );//windowed

    if(! STATE_GET(Initializing) )
        STATE_GET(Render)->Load();

    DXVGetCamera().Home();
}

void App::ParseShaderDefines(const TCHAR* defines)
{
    STATE_LOCK( Defines, DefineList );

    Defines.resize(0);

    const TCHAR* c = defines;

    while (c && *c)
    {
        // skip ws
        while (*c && iswspace(*c)) c++;
        if (*c == 0) break;

        // mark name start, find semicolon delimiter or end-of-string
        const TCHAR* ns = c;
        while (*c && *c != L';') c++;


        int length = ((int)(c - ns)) + 1;
        TCHAR* statement = (TCHAR*) alloca( sizeof( TCHAR ) * length );
        memcpy(statement, ns, sizeof( TCHAR ) * length );
        statement[length-1] = TEXT('\0');

        ParseShaderDefine(Defines, statement);

        if (*c == L';') 
            c++;
    }

    STATE_UNLOCK( Defines );
}

void App::ParseShaderDefine(DefineList& Defines, const TCHAR* statement)
{
    const TCHAR* start = statement;
    while (*start && iswspace(*start)) start++;

    const TCHAR* e = start;
    while (*e && (*e != '=') && !iswspace(*e)) e++;

    int length = ((int)(e - start))  + 1;
    TCHAR* name = (TCHAR*) alloca( sizeof( TCHAR ) * length );
    memcpy(name, start, sizeof( TCHAR ) * length );
    name[length-1] = TEXT('\0');

    if (*e == '=')
    {
        e++; 

        start = e;
        while (*start && iswspace(*start)) start++;
        e = start;
        while (*e && !iswspace(*e)) e++;

        length = ((int)(e - start))  + 1;
        TCHAR* value = (TCHAR*) alloca( sizeof( TCHAR ) * length );
        memcpy(value, start, sizeof( TCHAR ) * length );
        value[length-1] = TEXT('\0');


        AddShaderDefine(Defines, name, value);
        e++;    // e is value
    }
    else
    {
        AddShaderDefine(Defines, name, NULL);
    }
}

void App::AddShaderDefine(DefineList& Defines, const TCHAR* name, const TCHAR* value)
{
    Defines.push_back( Define( name, value ) );
}



bool App::CreateMainDialog()
{
    HWND dialog= ::CreateDialog( GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_MAIN), NULL, App::StaticDialogMsgProc );
    if( NULL == dialog )
    {
        ::MessageBox( NULL, L"Error creating the main application window", L"DirectX Viewer", MB_OK | MB_ICONERROR );
        return false;
    }

    STATE_SET(Dialog, dialog);
    return true;
}
void App::CreateRenderWindow()
{
    DXUTCreateWindow( L"DirectX Viewer", NULL, NULL, NULL, 0,0);
    HWND d3dHWND = DXUTGetHWNDDeviceWindowed();

    //FIX STYLE
    LONG style = ::GetWindowLong( d3dHWND, GWL_STYLE );
    style &= ~(WS_OVERLAPPEDWINDOW);
    style |= WS_CHILD|WS_TABSTOP;
    ::SetWindowLong( d3dHWND, GWL_STYLE, style );

    //FIX EXSTYLE
    LONG styleEx = ::GetWindowLong( d3dHWND, GWL_EXSTYLE );
    styleEx |= WS_EX_ACCEPTFILES;
    ::SetWindowLong( d3dHWND, GWL_EXSTYLE, styleEx );

    OnSize();
    ::SetFocus( d3dHWND );
}

INT_PTR App::OnDialogMsg( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
    switch( uMsg )
    {

        case WM_INITDIALOG:
        {
            STATE_SET( Dialog, hWnd);

            // Load and set the icon
            HICON hIcon = LoadIcon( GetModuleHandle(NULL), MAKEINTRESOURCE( IDI_MAIN_ICON ) );
            SendMessage( hWnd, WM_SETICON, ICON_BIG,   (LPARAM) hIcon );  // Set big icon
            SendMessage( hWnd, WM_SETICON, ICON_SMALL, (LPARAM) hIcon );  // Set small icon
            HideCaret( GetDlgItem(hWnd, IDC_STATUS) );
            return true;
        }
        break;

        case WM_CLOSE:
            DXVGetApp().Shutdown();
            PostQuitMessage(1);
            return true;

        case WM_DROPFILES:
            OnDropFile(wParam);
            return true;

        case WM_SIZE:
            OnSize();
            return true;
    };


    return 0;
}


void App::OnFrameMove( double fTime, float fElapsedTime )
{
    bool bReloadNeeded = false;


    if( timeGetTime() - DXVGetFileWatcher().GetTimeLastCheck() > 500 )
    {
        if( DXVGetFileWatcher().CheckForChanges() )
        {
            bReloadNeeded = true;
        }
    }

    DXVRESET reset = STATE_GET(Reset);

    if(reset == DXVRESET_RESETDEVICE
        || reset == DXVRESET_SWITCHDEVICE)
    {
        bReloadNeeded = false; //about to h
        ForceDeviceChangeByState();
    }
    else if(reset == DXVRESET_RELOADCONTENT || bReloadNeeded == true)
    {
        Reload();
    }
    STATE_SET(Reset, DXVRESET_NONE);


    if(DXVGetMainHUD().IsActive())
    {
        HWND foreground = GetForegroundWindow();
        if(foreground == STATE_GET( Dialog ) || foreground == DXUTGetHWND() )
        {
            bool bMerge = (0x80 & GetKeyState(VK_LCONTROL)) || (0x80 & GetKeyState(VK_RCONTROL));
            if(STATE_GET( MergeMode ) != bMerge)
            {
                STATE_SET( MergeMode, bMerge );
                DXVGetMainHUD().Refresh();
            }
        }
    }

    DXVGetCamera().FrameMove( fElapsedTime );

    STATE_GET(Render)->OnFrameMove( fTime, fElapsedTime );
}

void App::OnFrameRender( double fTime, float fElapsedTime) 
{ 
    STATE_GET(Render)->OnFrameRender(fTime,fElapsedTime); 

    LastRenderTime = (float)fTime;
    RenderCount++;

    DXVGetHudManager().OnRender(fElapsedTime);

    STATE_SET( ManualRefreshNeeded , false);

    if (STATE_GET( ExitOnLoad ))
        PostQuitMessage(0);
}

void App::ParseCommandLine(LPSTR args)
{
    struct 
    {
        LPWSTR Name;
        DWORD ResourceId;
    }
    SceneInfos[] =
    {
        { TEXT("Cube"), IDR_CUBEX },
        { TEXT("Plane"), IDR_PLANEX },
        { TEXT("Sphere"), IDR_SPHEREX },
        { NULL, 0 },
    };

    if( strlen(args) > 0 )
    {
        int argc = 0;
        LPWSTR* argv = CommandLineToArgvW(GetCommandLine(), &argc);
        LPWSTR* arg = argv;
        
        for (int a=1; a<argc; )
        {
            if (arg[a][0] == L'-' || arg[a][0] == L'/')
            {
                LPWSTR opt = arg[a]+1;
                a++;

                if (opt[0] == L'D')
                {
                    if (opt[1])
                    {
                        ParseShaderDefines(opt+1);
                    }
                    else
                    {
                        OutputW(L"-D command-line option requires immediate argument, use -Dmacro (no spaces) instead of -D macro\r\n");
                    }
                }
                else if (opt[0] == L'I')
                {
                    if (opt[1])
                    {
                        wstring incs = STATE_GET(Includes);

                        if (!incs.empty() && *incs.rbegin() != L';')
                            incs += L";";

                        incs += std::wstring(opt+1);

                        if (!incs.empty() && *incs.rbegin() != L';')
                            incs += L";";

                        STATE_SET(Includes, incs);
                    }
                    else
                    {
                        OutputW(L"-I command-line option requires immediate argument, use -Ipath (no spaces) instead of -I path\r\n");
                    }
                }
                else if (_wcsicmp(opt, L"shape") == 0)
                {
                    if (a == argc)
                        OutputW(L"-shape command-line option missing shape name, use one of [plane|cube|sphere]\r\n");

					int s;
                    for (s=0; SceneInfos[s].Name; s++)
                    {
                        if (_wcsicmp(arg[a], SceneInfos[s].Name) == 0)
                        {
                            STATE_SET( PreviewShape, SceneInfos[s].ResourceId );
                            OutputW(L"Preview shape set to '%s'\r\n", SceneInfos[s].Name);
                            break;
                        }
                    }

                    if (!SceneInfos[s].Name)
                        OutputW(L"-shape command-line option uses invalid shape name, use one of [plane|cube|sphere]\r\n");

                    a++;
                }
                else if (_wcsicmp(opt, L"Zi") == 0)
                {
                    STATE_SET( DebugVertexShaders , TRUE );
                    STATE_SET( DebugPixelShaders , TRUE );
                }
                else if (_wcsicmp(opt, L"Ziv") == 0)
                {
                    STATE_SET( DebugVertexShaders , TRUE );
                }
                else if (_wcsicmp(opt, L"Zip") == 0)
                {
                    STATE_SET( DebugPixelShaders , TRUE );
                }
                else if (_wcsicmp(opt, L"exitonload") == 0)
                {
                    STATE_SET( ExitOnLoad , TRUE ); // used in unit tests and automation.
                }
                else
                {
                    OutputW(L"Invalid command-line option: '%s'\r\n", arg[a]);
                }
            }
            else
            {
                DXV_FIND_DATA fileData;
                if(DXVGetFileFinder().FindFile( fileData, arg[a] ) )
                {
                    LoadFile(fileData.FullPath);
                }
                else    
                {
                    OutputW( L"Command line loader could not find file: %s", arg[a] );
                }

                a++;
            }
        }

        GlobalFree(argv);
    }
}

void App::RegisterCallbacks()
{
    DXUTSetCallbackMsgProc( App::StaticOnRenderMsg );
    DXUTSetCallbackKeyboard( App::StaticOnKeyboardMsg );
    
    DXUTSetCallbackFrameMove( App::StaticOnFrameMove );

    //device specific but call into a unified function
    DXUTSetCallbackD3D9FrameRender( App::StaticOnFrameRender9 );
    DXUTSetCallbackD3D10FrameRender( App::StaticOnFrameRender10 );
    DXUTSetCallbackDeviceChanging( App::StaticOnModifyDeviceSettings );

    DXUTSetCallbackD3D9DeviceCreated( App::StaticOnD3D9DeviceCreated );
    DXUTSetCallbackD3D9DeviceReset( App::StaticOnD3D9DeviceReset );
    DXUTSetCallbackD3D9DeviceLost( App::StaticOnD3D9DeviceLost );
    DXUTSetCallbackD3D9DeviceDestroyed( App::StaticOnD3D9DeviceDestroyed );


    DXUTSetCallbackD3D10DeviceCreated( App::StaticOnD3D10CreateDevice );
    DXUTSetCallbackD3D10SwapChainResized( App::StaticOnD3D10ResizedSwapChain );
    DXUTSetCallbackD3D10SwapChainReleasing( App::StaticOnD3D10ReleasingSwapChain );
    DXUTSetCallbackD3D10DeviceDestroyed( App::StaticOnD3D10DestroyDevice );

}


void App::OnDropFile(WPARAM wParam)
{
    WCHAR strFile[MAX_PATH];
    DragQueryFile( (HDROP)wParam, 0, strFile, MAX_PATH );
    DragFinish( (HDROP)wParam );

    OutputClear();
    LoadFile(strFile);

    return;
}

void App::LoadFile( LPCWSTR filepath )
{
    if( 0 == lstrcmpi( PathFindExtension(filepath), L".x"))
    {
        LoadX( filepath );
    }
    else if( 0 == lstrcmpi( PathFindExtension(filepath), L".fx"))
    {
        LoadFX( filepath );

    } 
    else if( 0 == lstrcmpi( PathFindExtension(filepath), L".dds"))
    {
        LoadDDS( filepath );

    }

    // Update window title
    TCHAR caption[255];

    caption[0] = 0;
    
    if (!STATE_GET(FileX).empty())
    {
        StringCchCat(caption, 255, PathFindFileName(STATE_GET(FileX).c_str()));
    }

    if (!STATE_GET(FileFX).empty())
    {
        if (caption[0] != 0)
            StringCchCat(caption, 255, TEXT(", "));

        StringCchCat(caption, 255, PathFindFileName(STATE_GET(FileFX).c_str()));
    }

    if (!STATE_GET(FileDDS).empty())
    {
        if (caption[0] != 0)
            StringCchCat(caption, 255, TEXT(", "));

        StringCchCat(caption, 255, PathFindFileName(STATE_GET(FileDDS).c_str()));
    }

    StringCchCat(caption, 255, TEXT(" - DirectX Viewer"));
    
    SetWindowText(STATE_GET(Dialog), caption);

}

void App::LoadDDS( LPCWSTR filepath )
{
    STATE_SET(FileX, std::wstring(L"") );
    STATE_SET(FileFX, std::wstring(L"") );        
    STATE_SET(FileDDS, std::wstring(filepath) );

    STATE_SET(TechniqueMode, DXVTECHNIQUEMODE_FIRSTVALID );
    STATE_SET(TechniqueSelected, 0 );
    STATE_SET(PassMode, DXVPASSMODE_ALL );
    STATE_SET(PassSelected, 0 );

    STATE_SET(Reset, DXVRESET_RELOADCONTENT);
}

void App::LoadFX( LPCWSTR filepath )
{

    STATE_SET(FileDDS, std::wstring(L"") );

    if(!STATE_GET( MergeMode ))
    {
        STATE_SET(FileX, std::wstring(L"") );
    }

    STATE_SET(FileFX, std::wstring(filepath) );

    STATE_SET(TechniqueMode, DXVTECHNIQUEMODE_FIRSTVALID );
    STATE_SET(TechniqueSelected, 0 );
    STATE_SET(PassMode, DXVPASSMODE_ALL );
    STATE_SET(PassSelected, 0 );

    STATE_SET(Reset, DXVRESET_RELOADCONTENT);
}

void App::LoadX( LPCWSTR filepath )
{
    STATE_SET(FileDDS, std::wstring(L"") );
    STATE_SET(FileX, std::wstring(filepath) );

    if(!STATE_GET( MergeMode ))
    {
        STATE_SET(FileFX, std::wstring(L"") );

        STATE_SET(TechniqueMode, DXVTECHNIQUEMODE_FIRSTVALID );
        STATE_SET(TechniqueSelected, 0 );
        STATE_SET(PassMode, DXVPASSMODE_ALL );
        STATE_SET(PassSelected, 0 );
    }
        
    STATE_SET(Reset, DXVRESET_RELOADCONTENT);
}

void App::Reload()
{
    STATE_GET(Render)->Clear();
    STATE_GET(Render)->Load();
    DXVGetCamera().Home();
}






void App::ShowEnvironmentDialog()
{


    OPENFILENAME ofn;
    ZeroMemory( &ofn, sizeof(ofn) );

    WCHAR selectedFile[MAX_PATH] = {0};
    ofn.lpstrFile = selectedFile;
    ofn.nMaxFile = MAX_PATH;
    ofn.lpstrFilter = L"DDS Files (*.dds)\0*.DDS\0";

    if( ShowFileDialog(ofn) )
    {
        STATE_SET( Environment, std::wstring( selectedFile ) );
    }
}

bool App::ShowDdsOrFxOrXDialog(bool bX, bool bFx, bool bDds)
{

    OPENFILENAME ofn;
    ZeroMemory( &ofn, sizeof(ofn) );

    WCHAR selectedFile[MAX_PATH] = {0};
    ofn.lpstrFile = selectedFile;
    ofn.nMaxFile = MAX_PATH;

    WCHAR filter[2048];
    ofn.lpstrFilter = filter;
    ZeroMemory( &filter, sizeof(WCHAR)*2048 );
    StringCchCat( filter, 2048, L"Supported Types (");
    bool first = true;
    if(bX)
    {
        if(!first)
            StringCchCat( filter, 2048, L"," );
        else
            first = false;
        
        StringCchCat( filter, 2048, L"*.x" );
    }
    if(bFx)
    {
        if(!first)
            StringCchCat( filter, 2048, L"," );
        else
            first = false;
        
        StringCchCat( filter, 2048, L"*.fx" );
    }
    if(bDds)
    {
        if(!first)
            StringCchCat( filter, 2048, L"," );
        else
            first = false;
    
        StringCchCat( filter, 2048, L"*.dds" );
    }
    StringCchCat( filter, 2048, L")\n" );

    if(bX)
    {    
        StringCchCat( filter, 2048, L"*.x;");
    }
    if(bFx)
    {

        StringCchCat( filter, 2048, L"*.fx;");
    }
    if(bDds)
    {
        StringCchCat( filter, 2048, L"*.dds;");
    }
    StringCchCat( filter, 2048, L"\n");

    if(bX)
    {    
        StringCchCat( filter, 2048, L"X Files (*.x)\n*.x\n");
    }
    if(bFx)
    {

        StringCchCat( filter, 2048, L"Effect Files (*.fx)\n*.fx\n");
    }
    if(bDds)
    {
        StringCchCat( filter, 2048, L"Direct Draw Surface Files (*.dds)\n*.dds\n");
    }

    for(int i = 0; i < 2048; i++)
    {
        if(filter[i] == L'\n')
            filter[i] = L'\0';
    }

    if( ShowFileDialog(ofn) )
    {
        OutputClear();
        LoadFile(ofn.lpstrFile);
    }

    return true;
}

bool App::ShowFileDialog(OPENFILENAME& ofn)
{
    ofn.lStructSize = sizeof(ofn);

    //proper ownership required for modal dialogs to stop input to their parents
    ofn.hwndOwner = STATE_GET(Dialog);
    ofn.hInstance = GetModuleHandle(NULL);

    //file better exit and we dont whant our current directory changing because of this dialog
    ofn.Flags |= OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_NOCHANGEDIR; 

    //fix the initial directory to the media path but leave it alone after that.
    static WCHAR lastDir[MAX_PATH] = L"";
    ofn.lpstrInitialDir = lastDir;
    if(lastDir[0] == L'\0')
    {
        WCHAR baseDir[MAX_PATH] = L"";
        GetCurrentDirectory(MAX_PATH, baseDir); 
        PathCombine(lastDir, baseDir, L"..\\..\\..\\Samples\\Media");//exiting 'utilities\bin\x86'
    }


    //if we were full screen, drop out of full screen to avoid 'top' ownership issues
    BOOL isWindowed = DXUTIsWindowed();
    WINDOWPLACEMENT oldPlacement;
    if(isWindowed==FALSE)
    {
        DXUTToggleFullScreen();

        ZeroMemory(&oldPlacement, sizeof(oldPlacement));
        oldPlacement.length = sizeof(oldPlacement);
        GetWindowPlacement( STATE_GET(Dialog), &oldPlacement);

        WINDOWPLACEMENT tmpPlacement = oldPlacement;
        tmpPlacement.showCmd = SW_SHOWMAXIMIZED;

        SetWindowPlacement(STATE_GET(Dialog), &tmpPlacement);
    }

    // Return if the user cancels the dialog
    BOOL result = GetOpenFileName( &ofn );

    //if we were full screen, return to full screen
    if(isWindowed==FALSE)
    {
        SetWindowPlacement(STATE_GET(Dialog), &oldPlacement);

        DXUTToggleFullScreen();
    }

    //if success fix the lastdir for the next time we open the dialog
    if(result != FALSE)
    {
        StringCchCopy(lastDir, MAX_PATH, ofn.lpstrFile);
        PathRemoveFileSpec(lastDir);
    }

    return result != FALSE;
}

void App::OnSize()
{

    RECT rcClient;
    GetClientRect( STATE_GET(Dialog) , &rcClient );
    int clientWidth = rcClient.right - rcClient.left;
    int clientHeight = rcClient.bottom - rcClient.top;
    int renderWidth = clientWidth;
    int renderHeight = clientHeight - STATUS_HEIGHT - STATUS_BORDER;
    MoveWindow( GetDlgItem( STATE_GET(Dialog), IDC_STATUS), 0, rcClient.bottom - STATUS_HEIGHT, clientWidth, STATUS_HEIGHT, true );
    MoveWindow( GetDlgItem(STATE_GET(Dialog), IDC_RENDER), 0, 0, renderWidth, renderHeight, true );
    MoveWindow( DXUTGetHWNDDeviceWindowed(), 0, 0, renderWidth, renderHeight, true );

    DXVGetCamera().Resize();

    STATE_SET(ManualRefreshNeeded, true);

}

void App::OutputA(const CHAR* message, ...)
{
    static CHAR bufferA[1024];

    va_list argList;
    va_start(argList, message);
    StringCchVPrintfA(bufferA, 1024, message, argList);
    va_end(argList);

   static WCHAR bufferW[1024] = {0};
   StringCchPrintfW(bufferW, 1024, L"%S", bufferA);

   OutputW( bufferW );
}

void App::OutputClear()
{
    HWND hwnd = GetDlgItem( STATE_GET(Dialog) , IDC_STATUS );
    SendMessage( hwnd, WM_SETTEXT, NULL, NULL );
}

void App::OutputW(const WCHAR* message, ...)
{
    static WCHAR buffer[1024];

    va_list argList;
    va_start(argList, message);
    StringCchVPrintfW(buffer, 1024, message, argList);
    va_end(argList);
#if defined(DEBUG) | defined(_DEBUG)
    OutputDebugStringW(buffer);
    OutputDebugStringW(L"\n");
#endif
    HWND hwnd = GetDlgItem( STATE_GET(Dialog) , IDC_STATUS );

    SendMessage( hwnd, EM_SETSEL, 0x8FFFFFFF, 0x8FFFFFFF );
    SendMessage( hwnd, EM_REPLACESEL, NULL, (LPARAM)buffer );
    SendMessage( hwnd, EM_REPLACESEL, NULL, (LPARAM)L"\015\012" );
}


void App::Help()
{
    TCHAR strPath[MAX_PATH];
    GetModuleFileName( NULL, strPath, MAX_PATH );

    PathRemoveFileSpec( strPath );
    PathAppend( strPath, TEXT("..") ); // up to bin
    PathAppend( strPath, TEXT("..") ); // up to utilities
    PathAppend( strPath, TEXT("..") ); // up to SDK
    PathAppend( strPath, TEXT("Documentation") );
    PathAppend( strPath, TEXT("DirectX9") );

    TCHAR strHelp[MAX_PATH];
    StringCchPrintf( strHelp, MAX_PATH, TEXT("%s\\directx_sdk.chm"), strPath );

    if( !PathFileExists( strHelp ) )
    {
        // Try the current directory instead 
        GetModuleFileName( NULL, strPath, MAX_PATH );
        PathRemoveFileSpec( strPath );
        StringCchPrintf( strHelp, MAX_PATH, TEXT("%s\\directx_sdk.chm"), strPath );

        if( !PathFileExists( strHelp ) )
        {
            OutputW( L"Help file not found." );
            return;
        }
    }

    TCHAR strUrl[512];
    StringCchPrintf(strUrl, 512, TEXT("%s::/DirectX_Viewer.htm"), strHelp);


    if(DXUTIsWindowed()==FALSE)
        DXUTToggleFullScreen();

    ::HtmlHelp( STATE_GET(Dialog) , strUrl, HH_DISPLAY_TOPIC, NULL );
}


bool App::OnModifyDeviceSettings( DXUTDeviceSettings* pDeviceSettings)
{
    return STATE_GET(Render)->OnModifyDeviceSettings(pDeviceSettings);
}

bool App::OnKeyboardMsg( UINT nChar, bool bKeyDown, bool bAltDown )
{
    if( DXVGetHudManager().OnKeyboardMsg( nChar, bKeyDown, bAltDown ) )
        return true;

    //if( DXVGetCamera().OnKeyboardMsg( nChar, bKeyDown, bAltDown ) )
    //    return true;


#if defined(DEBUG) | defined(_DEBUG)
    if(bAltDown)
    {
        switch( nChar )
        {
        case L'F':
            {
                ShowDdsOrFxOrXDialog(true, true, false);
            }
            break;
        };
    }
#endif

    return false;
}

LRESULT App::OnRenderMsg( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, bool* pbNoFurtherProcessing )
{
    switch( uMsg )
    {
    case WM_DROPFILES:
        OnDropFile(wParam);
        return true;//no more precessing necessary
    case WM_LBUTTONDOWN:
    case WM_MBUTTONDOWN:
    case WM_RBUTTONDOWN:
        {//TAKE CONTROL OF FOCUS FROM DIALOG
            if(GetFocus() != hWnd)
                SetFocus(hWnd);
            break;//allow further processing
        }
    };


    if( DXVGetHudManager().MsgProc( hWnd, uMsg, wParam, lParam ) )
        return 0;

    *pbNoFurtherProcessing= FALSE != DXVGetCamera().HandleMessages( hWnd, uMsg, wParam, lParam );
    if( *pbNoFurtherProcessing )
        return 0;

    return 0;
}

HRESULT CALLBACK App::StaticOnD3D9DeviceCreated( IDirect3DDevice9* pd3dDevice, const D3DSURFACE_DESC* pBackBufferSurfaceDesc, void* pUserContext )    
{
    HRESULT hr = S_OK;
    DISPATCH_FUNCTION_ITERATE_HR( LPDXUTCALLBACKD3D9DEVICECREATED , DXVGetApp().OnD3D9DeviceCreated, Itr, hr )
    {
        hr= Itr.GetFunction()(pd3dDevice, pBackBufferSurfaceDesc, Itr.GetUserData());
    }
    return hr;
}

HRESULT CALLBACK App::StaticOnD3D9DeviceReset( IDirect3DDevice9* pd3dDevice, const D3DSURFACE_DESC* pBackBufferSurfaceDesc, void* pUserContext )
{
    HRESULT hr = S_OK;
    DISPATCH_FUNCTION_ITERATE_HR( LPDXUTCALLBACKD3D9DEVICERESET , DXVGetApp().OnD3D9DeviceReset, Itr, hr )
    {
        hr= Itr.GetFunction()(pd3dDevice, pBackBufferSurfaceDesc, Itr.GetUserData());
    }
    return hr;
}

void    CALLBACK App::StaticOnD3D9DeviceLost( void* pUserContext )
{
    DISPATCH_FUNCTION_ITERATE( LPDXUTCALLBACKD3D9DEVICELOST , DXVGetApp().OnD3D9DeviceLost, Itr )
    {
        Itr.GetFunction()(Itr.GetUserData());
    }
}

void    CALLBACK App::StaticOnD3D9DeviceDestroyed( void* pUserContext )
{
    DISPATCH_FUNCTION_ITERATE( LPDXUTCALLBACKD3D9DEVICEDESTROYED , DXVGetApp().OnD3D9DeviceDestroyed, Itr)
    {
        Itr.GetFunction()(Itr.GetUserData());
    }
}

HRESULT CALLBACK App::StaticOnD3D10CreateDevice( ID3D10Device* pd3dDevice, const DXGI_SURFACE_DESC* pBackBufferSurfaceDesc, void* pUserContext ) 
{
    HRESULT hr = S_OK;
    DISPATCH_FUNCTION_ITERATE_HR( LPDXUTCALLBACKD3D10DEVICECREATED , DXVGetApp().OnD3D10DeviceCreated, Itr, hr )
    {
        hr= Itr.GetFunction()(pd3dDevice, pBackBufferSurfaceDesc, Itr.GetUserData());
    }
    return hr;
}

HRESULT CALLBACK App::StaticOnD3D10ResizedSwapChain( ID3D10Device* pd3dDevice, IDXGISwapChain *pSwapChain, const DXGI_SURFACE_DESC* pBackBufferSurfaceDesc, void* pUserContext )
{
    HRESULT hr = S_OK;
    DISPATCH_FUNCTION_ITERATE_HR( LPDXUTCALLBACKD3D10SWAPCHAINRESIZED , DXVGetApp().OnD3D10SwapChainResized, Itr, hr )
    {
        hr= Itr.GetFunction()(pd3dDevice, pSwapChain, pBackBufferSurfaceDesc, Itr.GetUserData());
    }
    return hr;
}

void    CALLBACK App::StaticOnD3D10ReleasingSwapChain( void* pUserContext )
{
    DISPATCH_FUNCTION_ITERATE( LPDXUTCALLBACKD3D10SWAPCHAINRELEASING , DXVGetApp().OnD3D10SwapChainReleasing, Itr )
    {
        Itr.GetFunction()(Itr.GetUserData());
    }
}

void    CALLBACK App::StaticOnD3D10DestroyDevice( void* pUserContext )
{
    DISPATCH_FUNCTION_ITERATE( LPDXUTCALLBACKD3D10DEVICEDESTROYED , DXVGetApp().OnD3D10DeviceDestroyed, Itr )
    {
        Itr.GetFunction()(Itr.GetUserData());
    }
}


void SasMsgListener::OnSasMessage( Sas::MessageLevel level, const WCHAR* message )
{
    DXVGetApp().OutputW( L"SAS: %s", message );
}

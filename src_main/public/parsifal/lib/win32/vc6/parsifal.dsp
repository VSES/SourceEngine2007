# Microsoft Developer Studio Project File - Name="parsifal" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=parsifal - Win32 Debug ICONV using MultiTh RTL DLL
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "parsifal.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "parsifal.mak" CFG="parsifal - Win32 Debug ICONV using MultiTh RTL DLL"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "parsifal - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "parsifal - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "parsifal - Win32 Release ICONV using MultiTh RTL DLL" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "parsifal - Win32 Debug ICONV using MultiTh RTL DLL" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "parsifal - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ".\Release"
# PROP Intermediate_Dir ".\Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "PARSIFAL_EXPORTS" /YX /FD /c
# ADD CPP /nologo /MT /W3 /O2 /I "../../include/libparsifal" /D "NDEBUG" /D XMLAPI=__declspec(dllexport) /D "WIN32" /D "_WINDOWS" /D "PARSIFAL_EXPORTS" /D "MAX_SPEED" /D "DTD_SUPPORT" /D "DTDVALID_SUPPORT" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x40b /d "NDEBUG"
# ADD RSC /l 0x40b /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /base:"0x628C0000" /dll /machine:I386 /out:"..\bin\parsifal.dll" /implib:"..\lib\parsifal.lib"
# SUBTRACT LINK32 /profile /pdb:none /incremental:yes /map /debug

!ELSEIF  "$(CFG)" == "parsifal - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ".\Debug"
# PROP Intermediate_Dir ".\Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "PARSIFAL_EXPORTS" /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "../../include/libparsifal" /D "_DEBUG" /D "_MBCS" /D "_USRDLL" /D "MAX_SPEED" /D "DTD_SUPPORT" /D "DTDVALID_SUPPORT" /D XMLAPI=__declspec(dllexport) /D "WIN32" /D "_WINDOWS" /D "PARSIFAL_EXPORTS" /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x40b /d "_DEBUG"
# ADD RSC /l 0x40b /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /map /debug /machine:I386
# SUBTRACT LINK32 /profile /incremental:no

!ELSEIF  "$(CFG)" == "parsifal - Win32 Release ICONV using MultiTh RTL DLL"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "parsifal___Win32_Release_ICONV_using_MultiTh_RTL_DLL"
# PROP BASE Intermediate_Dir "parsifal___Win32_Release_ICONV_using_MultiTh_RTL_DLL"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "parsifal___Win32_Release_ICONV_using_MultiTh_RTL_DLL"
# PROP Intermediate_Dir "parsifal___Win32_Release_ICONV_using_MultiTh_RTL_DLL"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /O2 /I "../../include/libparsifal" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D XMLAPI=__declspec(dllexport) /D "PARSIFAL_EXPORTS" /D "DTD_SUPPORT" /D "ICONV_SUPPORT" /D "HAS_STRICMP" /YX /FD /c
# ADD CPP /nologo /MD /W3 /O2 /I "../../include/libparsifal" /D "NDEBUG" /D "ICONV_SUPPORT" /D "HAS_STRICMP" /D "MAX_SPEED" /D "DTD_SUPPORT" /D "DTDVALID_SUPPORT" /D XMLAPI=__declspec(dllexport) /D "WIN32" /D "_WINDOWS" /D "PARSIFAL_EXPORTS" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x40b /d "NDEBUG"
# ADD RSC /l 0x40b /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib iconv.lib /nologo /base:"0x628C0000" /dll /machine:I386 /out:"..\bin\parsifal.dll" /implib:"..\lib\parsifal.lib"
# SUBTRACT BASE LINK32 /profile /pdb:none /incremental:yes /map /debug
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib iconv.lib /nologo /base:"0x628C0000" /dll /machine:I386 /out:"..\bin\parsifal.dll" /implib:"..\lib\parsifal.lib"
# SUBTRACT LINK32 /profile /pdb:none /incremental:yes /map /debug

!ELSEIF  "$(CFG)" == "parsifal - Win32 Debug ICONV using MultiTh RTL DLL"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "parsifal___Win32_Debug_ICONV_using_MultiTh_RTL_DLL0"
# PROP BASE Intermediate_Dir "parsifal___Win32_Debug_ICONV_using_MultiTh_RTL_DLL0"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "parsifal___Win32_Debug_ICONV_using_MultiTh_RTL_DLL0"
# PROP Intermediate_Dir "parsifal___Win32_Debug_ICONV_using_MultiTh_RTL_DLL0"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /ZI /Od /I "../../include/libparsifal" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D XMLAPI=__declspec(dllexport) /D "PARSIFAL_EXPORTS" /D "DTD_SUPPORT" /YX /FD /GZ /c
# SUBTRACT BASE CPP /Fr
# ADD CPP /nologo /MDd /W3 /Gm /ZI /Od /I "../../include/libparsifal" /D "_DEBUG" /D "ICONV_SUPPORT" /D "HAS_STRICMP" /D "MAX_SPEED" /D "DTD_SUPPORT" /D "DTDVALID_SUPPORT" /D XMLAPI=__declspec(dllexport) /D "WIN32" /D "_WINDOWS" /D "PARSIFAL_EXPORTS" /YX /FD /GZ /c
# SUBTRACT CPP /Fr
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x40b /d "_DEBUG"
# ADD RSC /l 0x40b /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /profile /map /debug /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib iconv.lib /nologo /dll /profile /map /debug /machine:I386

!ENDIF 

# Begin Target

# Name "parsifal - Win32 Release"
# Name "parsifal - Win32 Debug"
# Name "parsifal - Win32 Release ICONV using MultiTh RTL DLL"
# Name "parsifal - Win32 Debug ICONV using MultiTh RTL DLL"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\..\src\bistream.c
# End Source File
# Begin Source File

SOURCE=.\dllmain.c
# End Source File
# Begin Source File

SOURCE=..\..\src\dtdvalid.c
# End Source File
# Begin Source File

SOURCE=..\..\src\encoding.c
# End Source File
# Begin Source File

SOURCE=..\..\src\parsifal.c
# End Source File
# Begin Source File

SOURCE=.\parsifal.def
# End Source File
# Begin Source File

SOURCE=..\..\src\xmlhash.c
# End Source File
# Begin Source File

SOURCE=..\..\src\xmlpool.c
# End Source File
# Begin Source File

SOURCE=..\..\src\xmlsbuf.c
# End Source File
# Begin Source File

SOURCE=..\..\src\xmlvect.c
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\..\include\libparsifal\bistream.h
# End Source File
# Begin Source File

SOURCE=..\..\include\libparsifal\dtdvalid.h
# End Source File
# Begin Source File

SOURCE=..\..\include\libparsifal\isrcmem.h
# End Source File
# Begin Source File

SOURCE=..\..\include\libparsifal\nametab.h
# End Source File
# Begin Source File

SOURCE=..\..\include\libparsifal\optcfg.h
# End Source File
# Begin Source File

SOURCE=..\..\include\libparsifal\parsifal.h
# End Source File
# Begin Source File

SOURCE=..\..\include\libparsifal\xmlcfg.h
# End Source File
# Begin Source File

SOURCE=..\..\include\libparsifal\xmldef.h
# End Source File
# Begin Source File

SOURCE=..\..\include\libparsifal\xmldtd.h
# End Source File
# Begin Source File

SOURCE=..\..\include\libparsifal\xmlhash.h
# End Source File
# Begin Source File

SOURCE=..\..\include\libparsifal\xmlpool.h
# End Source File
# Begin Source File

SOURCE=..\..\include\libparsifal\xmlsbuf.h
# End Source File
# Begin Source File

SOURCE=..\..\include\libparsifal\xmlvect.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# End Target
# End Project

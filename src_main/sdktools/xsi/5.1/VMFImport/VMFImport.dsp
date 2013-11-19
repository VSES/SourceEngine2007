# Microsoft Developer Studio Project File - Name="VMFImport" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=VMFImport - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "VMFImport.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "VMFImport.mak" CFG="VMFImport - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "VMFImport - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "VMFImport - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName "VMFImport"
# PROP Scc_LocalPath "."
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "VMFImport - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
LIB32=link.exe -lib
F90=fl32.exe
InputPath=\RayFlex\Debug\bin\optasks.dll
TargetPath=\RayFlex\Debug\bin\optasks.dll
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "VMFIMPORT_EXPORTS" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "$(XSISDK_ROOT)\include" /I "$(XSIFTK_ROOT)\export\h\Core" /I "$(XSIFTK_ROOT)\export\h\FTK" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "VMFIMPORT_EXPORTS" /D "__XSI_PLUGIN_" /D "XSI_STATIC_LINK" /D "XSICORE_STATIC" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386
# ADD LINK32 XSIFTK_static.lib XSICore_static.lib zlib.lib sicppsdk.lib jpeglib_static.lib sicoresdk.lib /nologo /dll /machine:I386 /out:"..\..\Application\Plugins\VMFImport.dll" /libpath:"\rayflex\ship\lib" /libpath:"\Storm\ship\Win32\lib" /libpath:"$(XSISDK_ROOT)\lib\nt-x86" /libpath:"$(XSIFTK_ROOT)\export\lib"

!ELSEIF  "$(CFG)" == "VMFImport - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
LIB32=link.exe -lib
F90=fl32.exe
InputPath=\RayFlex\Debug\bin\optasks.dll
TargetPath=\RayFlex\Debug\bin\optasks.dll
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "VMFIMPORT_EXPORTS" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "$(XSISDK_ROOT)\include" /I "$(XSIFTK_ROOT)\export\h\Core" /I "$(XSIFTK_ROOT)\export\h\FTK" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "VMFIMPORT_EXPORTS" /D "__XSI_PLUGIN_" /D "XSI_STATIC_LINK" /D "XSICORE_IMPL" /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 XSIFTK_static.lib XSICore_static.lib zlib.lib sicppsdk.lib jpeglib_static.lib sicoresdk.lib /nologo /dll /debug /machine:I386 /out:"..\..\application\plugins\VMFImport.dll" /pdbtype:sept /libpath:"$(XSISDK_ROOT)\lib\nt-x86" /libpath:"$(XSIFTK_ROOT)\export\lib"

!ENDIF 

# Begin Target

# Name "VMFImport - Win32 Release"
# Name "VMFImport - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\BrushConverter.cpp
# End Source File
# Begin Source File

SOURCE=.\C3DVectorCompressor.cpp
# End Source File
# Begin Source File

SOURCE=.\DispInfoConverter.cpp
# End Source File
# Begin Source File

SOURCE=.\Entity.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\VMFImport.cpp
# End Source File
# Begin Source File

SOURCE=.\VMFImport.def
# End Source File
# Begin Source File

SOURCE=.\VMFMaps.cpp
# End Source File
# Begin Source File

SOURCE=.\VMFParser.cpp
# End Source File
# Begin Source File

SOURCE=.\VMFTemplate.cpp
# End Source File
# Begin Source File

SOURCE=.\VMFUtils.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\BrushConverter.h
# End Source File
# Begin Source File

SOURCE=.\C3DVectorCompressor.h
# End Source File
# Begin Source File

SOURCE=.\cmdlib.h
# End Source File
# Begin Source File

SOURCE=.\DispInfoConverter.h
# End Source File
# Begin Source File

SOURCE=.\Entity.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\VMFMaps.h
# End Source File
# Begin Source File

SOURCE=.\VMFParser.h
# End Source File
# Begin Source File

SOURCE=.\VMFTemplate.h
# End Source File
# Begin Source File

SOURCE=.\VMFTypes.h
# End Source File
# Begin Source File

SOURCE=.\VMFUtils.h
# End Source File
# Begin Source File

SOURCE=.\wadlib.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# Begin Source File

SOURCE=.\ImportVMF.vbs
# End Source File
# Begin Source File

SOURCE=.\install.vbs
# End Source File
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# Begin Source File

SOURCE=.\RegisterCmd.vbs
# End Source File
# Begin Source File

SOURCE=.\VMFImport.vbs
# End Source File
# Begin Source File

SOURCE=.\vmfImportOptions.spdl
# End Source File
# End Target
# End Project

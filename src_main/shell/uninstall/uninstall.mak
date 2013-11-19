# Microsoft Developer Studio Generated NMAKE File, Based on uninstall.dsp
!IF "$(CFG)" == ""
CFG=uninstall - Win32 Debug
!MESSAGE No configuration specified. Defaulting to uninstall - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "uninstall - Win32 Release" && "$(CFG)" !=\
 "uninstall - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "uninstall.mak" CFG="uninstall - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "uninstall - Win32 Release" (based on\
 "Win32 (x86) Dynamic-Link Library")
!MESSAGE "uninstall - Win32 Debug" (based on\
 "Win32 (x86) Dynamic-Link Library")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "uninstall - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

!IF "$(RECURSE)" == "0" 

ALL : "$(OUTDIR)\uninstall.dll"

!ELSE 

ALL : "$(OUTDIR)\uninstall.dll"

!ENDIF 

CLEAN :
	-@erase "$(INTDIR)\DirectoryCurser.obj"
	-@erase "$(INTDIR)\exports.obj"
	-@erase "$(INTDIR)\Items2DelDlg.obj"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\uninstall.obj"
	-@erase "$(INTDIR)\uninstall.pch"
	-@erase "$(INTDIR)\uninstall.res"
	-@erase "$(INTDIR)\vc50.idb"
	-@erase "$(OUTDIR)\uninstall.dll"
	-@erase "$(OUTDIR)\uninstall.exp"
	-@erase "$(OUTDIR)\uninstall.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D\
 "_WINDLL" /D "_USRDLL" /Fp"$(INTDIR)\uninstall.pch" /Yu"stdafx.h"\
 /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
CPP_OBJS=.\Release/
CPP_SBRS=.
MTL_PROJ=/nologo /D "NDEBUG" /mktyplib203 /o NUL /win32 
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\uninstall.res" /d "NDEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\uninstall.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=/nologo /subsystem:windows /dll /incremental:no\
 /pdb:"$(OUTDIR)\uninstall.pdb" /machine:I386 /def:".\uninstall.def"\
 /out:"$(OUTDIR)\uninstall.dll" /implib:"$(OUTDIR)\uninstall.lib" 
DEF_FILE= \
	".\uninstall.def"
LINK32_OBJS= \
	"$(INTDIR)\DirectoryCurser.obj" \
	"$(INTDIR)\exports.obj" \
	"$(INTDIR)\Items2DelDlg.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\uninstall.obj" \
	"$(INTDIR)\uninstall.res"

"$(OUTDIR)\uninstall.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "uninstall - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

!IF "$(RECURSE)" == "0" 

ALL : "$(OUTDIR)\uninstall.dll"

!ELSE 

ALL : "$(OUTDIR)\uninstall.dll"

!ENDIF 

CLEAN :
	-@erase "$(INTDIR)\DirectoryCurser.obj"
	-@erase "$(INTDIR)\exports.obj"
	-@erase "$(INTDIR)\Items2DelDlg.obj"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\uninstall.obj"
	-@erase "$(INTDIR)\uninstall.pch"
	-@erase "$(INTDIR)\uninstall.res"
	-@erase "$(INTDIR)\vc50.idb"
	-@erase "$(INTDIR)\vc50.pdb"
	-@erase "$(OUTDIR)\uninstall.dll"
	-@erase "$(OUTDIR)\uninstall.exp"
	-@erase "$(OUTDIR)\uninstall.ilk"
	-@erase "$(OUTDIR)\uninstall.lib"
	-@erase "$(OUTDIR)\uninstall.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MTd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS"\
 /D "_WINDLL" /D "_USRDLL" /Fp"$(INTDIR)\uninstall.pch" /Yu"stdafx.h"\
 /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
CPP_OBJS=.\Debug/
CPP_SBRS=.
MTL_PROJ=/nologo /D "_DEBUG" /mktyplib203 /o NUL /win32 
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\uninstall.res" /d "_DEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\uninstall.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=/nologo /subsystem:windows /dll /incremental:yes\
 /pdb:"$(OUTDIR)\uninstall.pdb" /debug /machine:I386 /def:".\uninstall.def"\
 /out:"$(OUTDIR)\uninstall.dll" /implib:"$(OUTDIR)\uninstall.lib" /pdbtype:sept 
DEF_FILE= \
	".\uninstall.def"
LINK32_OBJS= \
	"$(INTDIR)\DirectoryCurser.obj" \
	"$(INTDIR)\exports.obj" \
	"$(INTDIR)\Items2DelDlg.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\uninstall.obj" \
	"$(INTDIR)\uninstall.res"

"$(OUTDIR)\uninstall.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 

.c{$(CPP_OBJS)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(CPP_OBJS)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(CPP_OBJS)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(CPP_SBRS)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(CPP_SBRS)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(CPP_SBRS)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<


!IF "$(CFG)" == "uninstall - Win32 Release" || "$(CFG)" ==\
 "uninstall - Win32 Debug"
SOURCE=.\DirectoryCurser.cpp
DEP_CPP_DIREC=\
	".\DirectoryCurser.h"\
	".\Items2DelDlg.h"\
	".\uninstall.h"\
	

"$(INTDIR)\DirectoryCurser.obj" : $(SOURCE) $(DEP_CPP_DIREC) "$(INTDIR)"\
 "$(INTDIR)\uninstall.pch"


SOURCE=.\exports.cpp
DEP_CPP_EXPOR=\
	".\DirectoryCurser.h"\
	".\Items2DelDlg.h"\
	".\uninstall.h"\
	

"$(INTDIR)\exports.obj" : $(SOURCE) $(DEP_CPP_EXPOR) "$(INTDIR)"\
 "$(INTDIR)\uninstall.pch"


SOURCE=.\Items2DelDlg.cpp
DEP_CPP_ITEMS=\
	".\DirectoryCurser.h"\
	".\Items2DelDlg.h"\
	".\uninstall.h"\
	

"$(INTDIR)\Items2DelDlg.obj" : $(SOURCE) $(DEP_CPP_ITEMS) "$(INTDIR)"\
 "$(INTDIR)\uninstall.pch"


SOURCE=.\StdAfx.cpp
DEP_CPP_STDAF=\
	".\StdAfx.h"\
	

!IF  "$(CFG)" == "uninstall - Win32 Release"

CPP_SWITCHES=/nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D\
 "_WINDLL" /D "_USRDLL" /Fp"$(INTDIR)\uninstall.pch" /Yc"stdafx.h"\
 /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\uninstall.pch" : $(SOURCE) $(DEP_CPP_STDAF)\
 "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "uninstall - Win32 Debug"

CPP_SWITCHES=/nologo /MTd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D\
 "_WINDOWS" /D "_WINDLL" /D "_USRDLL" /Fp"$(INTDIR)\uninstall.pch" /Yc"stdafx.h"\
 /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\uninstall.pch" : $(SOURCE) $(DEP_CPP_STDAF)\
 "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\uninstall.cpp
DEP_CPP_UNINS=\
	".\DirectoryCurser.h"\
	".\Items2DelDlg.h"\
	".\uninstall.h"\
	

"$(INTDIR)\uninstall.obj" : $(SOURCE) $(DEP_CPP_UNINS) "$(INTDIR)"\
 "$(INTDIR)\uninstall.pch"


SOURCE=.\uninstall.rc
DEP_RSC_UNINST=\
	".\res\cb1.bmp"\
	".\res\cb2.bmp"\
	".\res\cb3.bmp"\
	".\res\cb4.bmp"\
	".\res\uninstall.rc2"\
	

"$(INTDIR)\uninstall.res" : $(SOURCE) $(DEP_RSC_UNINST) "$(INTDIR)"
	$(RSC) $(RSC_PROJ) $(SOURCE)



!ENDIF 


@echo off
set LIB=..\..\win32\lib;%LIB%
cl /W3 /nologo /DXMLAPI=__declspec(dllimport) /I..\..\include /Fe..\..\win32\bin\winurl winurl.c parsifal.lib urlmon.lib user32.lib
@echo Run it using: win32\bin\winurl

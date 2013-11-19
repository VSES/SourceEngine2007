@echo off
set LIB=..\..\win32\lib;%LIB%
cl /W3 /nologo /DXMLAPI=__declspec(dllimport) /I..\..\include /Fe..\..\win32\bin\zenstory zenstory.c parsifal.lib
@echo Run it using: win32\bin\zenstory ^< stories.xml

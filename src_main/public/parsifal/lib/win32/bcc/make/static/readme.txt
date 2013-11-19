This directory contains "Borland C++ Command Line Tools" makefile for building
static parsifal library.  (bcc version: 5.5) 


free download:
http://www.borland.com/products/downloads/download_cbuilder.html


files in libparsifal-0.9.1\win32\bcc\make\static\ :

parsifal.lib      pre-compiled library for bcc projects
makefile          makefile for borland make. contains some hard-coded paths but
                  should be trivial to change to any bcc environment


----------------------------------------------------------------------
Handling:
----------------------------------------------------------------------

First of all:
- be shure borland is configured (Path-Environment-Variables, and Config-Files in Borland-Path)
  (look in the readme.txt in Borland-Path)

Parsifal:
- Check the PATH-Variables in the makefile
- in command-window just run : > make 
- obj-files and parsifal.lib are stored in the make-directory (NOT in ../../../lib !)



----------------------------------------------------------------------
Problems:
----------------------------------------------------------------------

The Library-Linker warns like this, when the library doesnt exist:

 TLIB 4.5 Copyright (c) 1987, 1999 Inprise Corporation
 Warning: 'bistream' not found in library
 Warning: 'encoding' not found in library
 Warning: 'parsifal' not found in library
 Warning: 'xmlhash' not found in library
 Warning: 'xmlsbuf' not found in library
 Warning: 'xmlvect' not found in library

This is caused by the /u option <- think it stands for "update"...
but it is not an error...

----

i dont know how to include a smooth auto-clean ;-)
just type > make clean
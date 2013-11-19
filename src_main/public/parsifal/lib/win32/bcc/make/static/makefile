# Project: libparsifal
# Update: 13.02.2005
# For C++BUILDER COMMAND LINE TOOLS  V.5.5

BORLANDPATH = C:\Borland\bcc55
CC   = $(BORLANDPATH)\bin\bcc32
TLIB = $(BORLANDPATH)\bin\tlib

LINKOBJ  = bistream.obj encoding.obj parsifal.obj xmlhash.obj xmlsbuf.obj xmlvect.obj xmlpool.obj dtdvalid.obj
INCS = -I"../../../../include/libparsifal" 
BIN  = parsifal.lib
CFLAGS = -O2 $(INCS) -DDTD_SUPPORT=1 -DHAS_STRICMP=1 -DDTDVALID_SUPPORT=1

#.PHONY: all all-before all-after clean clean-custom

#all: all-before libparsifal.a all-after
all: parsifal.lib



clean: 
	del $(LINKOBJ) $(BIN)

$(BIN): $(LINKOBJ)
   $(TLIB) $@ /P32 /u $(LINKOBJ)


bistream.obj: ../../../../src/bistream.c
	$(CC) $(CFLAGS) -c ../../../../src/bistream.c 

encoding.obj: ../../../../src/encoding.c
	$(CC) $(CFLAGS) -c ../../../../src/encoding.c 

parsifal.obj: ../../../../src/parsifal.c
	$(CC) $(CFLAGS) -c ../../../../src/parsifal.c  

xmlhash.obj: ../../../../src/xmlhash.c
	$(CC) $(CFLAGS) -c ../../../../src/xmlhash.c  

xmlsbuf.obj: ../../../../src/xmlsbuf.c
	$(CC) $(CFLAGS) -c ../../../../src/xmlsbuf.c  

xmlvect.obj: ../../../../src/xmlvect.c
	$(CC) $(CFLAGS) -c ../../../../src/xmlvect.c  

xmlpool.obj: ../../../../src/xmlpool.c
	$(CC) $(CFLAGS) -c ../../../../src/xmlpool.c

dtdvalid.obj: ../../../../src/dtdvalid.c
	$(CC) $(CFLAGS) -c ../../../../src/dtdvalid.c

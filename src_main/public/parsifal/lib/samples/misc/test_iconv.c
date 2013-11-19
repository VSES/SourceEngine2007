/*
LIBICONV TEST
These functions should be included in your application for testing
that libiconv library is linked with same runtime library version,
singlethreaded or multithreaded, as your application - it that case 
calling IconvTest() returns 0 meaning "libiconv test successful".

Read also libiconv README for more information
http://www.gnu.org/software/libiconv/

You can make this standalone application by adding
#include "libparsifal/parsifal.h"
and
int main(int argc, char* argv[])
{
	switch (IconvTest()) {
		case 0: puts("\nlibiconv test successful."); break;
		case 1: puts("\nlibiconv test failed!\n"
			"Check that you're linking with same runtime lib as your libiconv."); break;
		case 2: puts("\nMemory alloc error!\n");	break;
		default: puts("\nUnknown error testing libiconv!\n");
	}
	return 0;
}
*/

#include "libparsifal/isrcmem.h"

int MemInputsrc(BYTE *buf, int cBytes, int *cBytesActual, void *inputData)
{			
	XMLMEMINPUTSRC_HANDLE
}

int IconvTest() {
	int ret = -1; /* UNKNOWN ERROR */
	char *t = "\xFE\xFF\x00\x3C\x00\x7A\x00\x3E\xDF\x0F\x00\x3C\x00\x2F\x00\x7A\x00\x3E";
	/* UTF-16 (big endian) string (18 bytes): BOM and characters 
	   <z>*</z> 
	   where * is illegal character (low surrogate 0xdf0f) */
	LPXMLPARSER p = XMLParser_Create(&p);
	XMLMEMINPUTSRC meminput;
	XMLMEMINPUTSRC_INIT(&meminput, t, 18);
	if (!p) return 2; /* MEMORY ALLOC ERROR */	
	XMLParser_Parse(p, MemInputsrc, &meminput, NULL);
	if (p->ErrorCode) {
		ret = (p->reader->encerr == EILSEQ) ? 0 /* SUCCESS */ : 1 /* FAILED */;
	}	
	XMLParser_Free(p);
	return ret;
}
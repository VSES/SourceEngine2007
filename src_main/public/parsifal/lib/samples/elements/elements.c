#include <stdio.h>
#include "libparsifal/parsifal.h"

int StartElement(void *UserData, const XMLCH *uri, const XMLCH *localName, const XMLCH *qName, LPXMLVECTOR atts);
int EndElement(void *UserData, const XMLCH *uri, const XMLCH *localName, const XMLCH *qName);
void ErrorHandler(LPXMLPARSER parser) {} /* dummy, only for switching ErrorString etc. on */

int cstream(BYTE *buf, int cBytes, int *cBytesActual, void *inputData);

int cstream(BYTE *buf, int cBytes, int *cBytesActual, void *inputData)
{
	*cBytesActual = fread(buf, 1, cBytes, (FILE*)inputData);	
	return (*cBytesActual < cBytes);
}

int StartElement(void *UserData, const XMLCH *uri, const XMLCH *localName, const XMLCH *qName, LPXMLVECTOR atts)
{
	int i;
  	int *depthPtr = UserData;
  	for (i = 0; i < *depthPtr; i++)
    	putchar('\t');
  	puts(qName);
  	*depthPtr += 1;
	return 0;
}

int EndElement(void *UserData, const XMLCH *uri, const XMLCH *localName, const XMLCH *qName)
{	
	int *depthPtr = UserData;
	*depthPtr -= 1;
	return 0;
}

int main(int argc, char* argv[])
{	
	LPXMLPARSER parser;		
	int depth = 0;

	if (!XMLParser_Create(&parser)) {
		printf("Error creating parser!\n");
		return 1;
	}

	parser->errorHandler = ErrorHandler;
	parser->startElementHandler = StartElement;
	parser->endElementHandler = EndElement;	
	parser->UserData = &depth;

	if (!XMLParser_Parse(parser, cstream, stdin, 0))
		printf("Error: %s\nLine: %d Col: %d\n", 
			parser->ErrorString, parser->ErrorLine, parser->ErrorColumn);
			
	XMLParser_Free(parser);
	return 0;
}


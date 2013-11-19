#include <stdio.h>
#include "windows.h"
#include "urlmon.h"
#include "libparsifal/parsifal.h"

#define MAX_URILEN 512
XMLCH gbase[MAX_URILEN] = {'\0'}; /* base uri - global for clarity */

int StartElement(void *UserData, const XMLCH *uri, const XMLCH *localName, const XMLCH *qName, LPXMLVECTOR atts);
int EndElement(void *UserData, const XMLCH *uri, const XMLCH *localName, const XMLCH *qName);
int PI(void *UserData, const XMLCH *target, const XMLCH *data);
int Characters(void *UserData, const XMLCH *chars, int cbChars);
int Comment(void *UserData, const XMLCH *chars, int cbChars);
int CharactersWide(void *UserData, const XMLCH *chars, int cbChars);
int StartCData(void *UserData);
int EndCData(void *UserData);
int DoctypeDecl(void *UserData, const XMLCH *name, const XMLCH *publicID, const XMLCH *systemID, int hasInternalSubset);
void ErrorHandler(LPXMLPARSER parser);
int ResolveEntity(void *UserData, LPXMLENTITY entity, LPBUFFEREDISTREAM reader);
int FreeInputData(void *UserData, LPXMLENTITY entity, LPBUFFEREDISTREAM reader);
int SkippedEntity(void *UserData, const XMLCH *name);
int urlstream(BYTE *buf, int cBytes, int *cBytesActual, void *inputData);
/* see samples/misc/helper.c for comments of the following routines: */
size_t GetBaseDir(XMLCH *dst, XMLCH *src);
XMLCH *ResolveBaseUri(LPXMLPARSER parser, XMLCH *systemID, XMLCH *base);
                
int StartElement(void *UserData, const XMLCH *uri, const XMLCH *localName, const XMLCH *qName, LPXMLVECTOR atts)
{	            
	if (*uri)   
		printf("\nStart tag: %s uri: %s localName: %s", qName, uri, localName);	
	else	
		printf("\nStart tag: %s", qName);		

	if (atts->length) {
		int i;	
		LPXMLRUNTIMEATT att;
		
		printf("\nhas %d attributes:", atts->length);
		for (i=0; i<atts->length; i++) {
			att = (LPXMLRUNTIMEATT)XMLVector_Get(atts, i);
			
			if (*att->uri)
				printf("\n  Name: %s Value: %s Prefix: %s LocalName: %s Uri: %s", 
					att->qname, att->value,
							att->prefix, att->localName, 
								att->uri);
			else 
				printf("\n  Name: %s Value: {%s}", 
					att->qname, att->value);		
		}
		
		/* this demonstrates XMLParser_GetNamedItem and XML_ABORT, just
			change the name "findthis" here to abort parsing when
			this attribute is encountered: */
		
		if (att = XMLParser_GetNamedItem(UserData, "findthis")) {
			printf("FOUND ATTRIBUTE %s value: %s\nAborting...\n", att->qname, att->value);
			return XML_ABORT;
		}
	}
	return 0;
}

int EndElement(void *UserData, const XMLCH *uri, const XMLCH *localName, const XMLCH *qName)
{	
	printf("\nEnd tag: %s", qName);
	return 0;
}

int Characters(void *UserData, const XMLCH *chars, int cbChars)
{	
	printf("\nText (%d bytes): {", cbChars);
	for (; cbChars; cbChars--, chars++) putc(*chars, stdout);
	putc('}', stdout);
	return 0;
}

int SkippedEntity(void *UserData, const XMLCH *name)
{
	printf("\nskipped entity: %s", name);
	return 0;
}

int DoctypeDecl(void *UserData, const XMLCH *name, const XMLCH *publicID, const XMLCH *systemID, int hasInternalSubset)
{
	printf("\nDOCTYPE Name: %s", name);
	if (publicID) printf(" publicID: %s", publicID);
	if (systemID) printf(" systemID: %s", systemID);
	printf(" hasInternalSubset: %d", hasInternalSubset);
	return 0;	
}

int StartCData(void *UserData) 
{ 
	printf("\nStart CData tag\n");
	/* will call Characters to report CDATA contents */
	return 0; 
}

int EndCData(void *UserData) 
{ 
	printf("\nEnd CData tag");
	return 0; 
}

int Comment(void *UserData, const XMLCH *chars, int cbChars)
{
	printf("\nComment (%d bytes): {", cbChars);
	for (; cbChars; cbChars--, chars++) putc(*chars, stdout);
	putc('}', stdout);
	return 0;
}

int PI(void *UserData, const XMLCH *target, const XMLCH *data)
{
	printf("\nPI tag - target: %s data: %s", target, ((*data) ? data : "no data"));
	return 0;
}

void ErrorHandler(LPXMLPARSER parser) 
{
	/* you should treat ERR_XMLP_ABORT as "user error" and give somekind of
	  description before returning from callbacks, otherwise we present parser error: */
	if (parser->ErrorCode != ERR_XMLP_ABORT) {
		XMLCH *SystemID = XMLParser_GetSystemID(parser);
		LPXMLENTITY curEnt = XMLParser_GetCurrentEntity(parser);		
		printf("\nParsing Error: %s\nCode: %d",
			parser->ErrorString, parser->ErrorCode);
		if (curEnt && !curEnt->systemID) printf("\nin entity: '%s'", curEnt->name);
		if (SystemID) printf("\nSystemID: '%s'", SystemID);
	}
	printf("\nLine: %d\nColumn: %d", parser->ErrorLine, parser->ErrorColumn);
}

/*
	Converts UTF-8 string to wchar_t string
	and shows converted string in MessageBoxW
	
	Set charactersHandler = CharactersWide if
	you want to test UTF-8 to wchar_t conversion for
	text content, you shouldn't run large documents with
	CharactersWide because those MessageBoxes can get annoying...
*/	
int CharactersWide(void *UserData, const XMLCH *chars, int cbChars)
{	
	wchar_t wstr[1024];
	int size;
		
	if (cbChars > 1023) cbChars = 1024;
	
	if ((size = MultiByteToWideChar(CP_UTF8, 0, chars, cbChars, wstr, 1024))) {
		*(wstr+size) = L'\0';
		MessageBoxW(NULL, wstr, L"WinUrl sample", MB_OK);
	}
	else {
		printf("Unicode conversion error!");
		return XML_ABORT;	
	}
	return 0;
}

size_t GetBaseDir(XMLCH *dst, XMLCH *src)
{
	XMLCH *s = strrchr(src, '/');	
#ifdef _WIN32
	if (!s) s = strrchr(src, '\\');
#endif
	if (s) {
		size_t i = (s-src)+1;
		memcpy(dst, src, i);
		dst[i] = '\0';
		return i;
	}
	dst[0] = '\0';
	return 0;
}

XMLCH *ResolveBaseUri(LPXMLPARSER parser, XMLCH *systemID, XMLCH *base)
{
	XMLCH *s=systemID;
	for (; *s; s++) {
		if (*s == ':') return systemID; /* probably absolute */ 
		if (*s == '/' || *s == '\\') break;
	}
	s = XMLParser_GetPrefixMapping(parser, "xml:base");
	return (s) ? s : base;
}

int ResolveEntity(void *UserData, LPXMLENTITY entity, LPBUFFEREDISTREAM reader)
{
	IStream *pStm = NULL; /* pointer to COM stream */
	HRESULT hr;
	XMLCH r[MAX_URILEN]; 
	XMLCH *uri = ResolveBaseUri((LPXMLPARSER)UserData, entity->systemID, gbase);
	if (uri != entity->systemID) {
		strcpy(r, uri);
		uri = strcat(r, entity->systemID);
	}
	printf("\nLoading external entity: %s uri: %s", entity->name, uri);
	hr = URLOpenBlockingStream(0, uri, &pStm, 0,0);  	
	if (!SUCCEEDED(hr)) {
		printf("\nError opening url '%s'", uri);
		if (pStm) pStm->lpVtbl->Release(pStm);
		return XML_ABORT;	
	}
	
	reader->inputData = pStm;
	return 0;
}

int FreeInputData(void *UserData, LPXMLENTITY entity, LPBUFFEREDISTREAM reader)
{	
	IStream *pStm = (IStream*)reader->inputData;
	if (pStm) pStm->lpVtbl->Release(pStm);
	return 0;
}

int urlstream(BYTE *buf, int cBytes, int *cBytesActual, void *inputData)
{	
	/* calls IStream.Read in C COM way: */
	HRESULT hr = ((IStream*)inputData)->lpVtbl->Read((IStream*)inputData, buf, cBytes, cBytesActual);		
	return (*cBytesActual < cBytes || !SUCCEEDED(hr));
}

int main(int argc, char* argv[])
{	
	LPXMLPARSER parser;		
	IStream *pStm = NULL; /* pointer to COM stream */
	HRESULT hr;
	char szUrl[MAX_URILEN];
	
	printf("\nWINURL Parsifal sample\n\nUrl to parse: ");
	gets(szUrl);
	
	/* open the url stream: */	
	hr = URLOpenBlockingStream(0, szUrl, &pStm, 0,0);  	
	if (!SUCCEEDED(hr)) {
		puts("Error opening url!");
		if (pStm) pStm->lpVtbl->Release(pStm);
		exit(1);	
	}
	
	GetBaseDir(gbase, szUrl);

	if (!XMLParser_Create(&parser)) {
		puts("Error creating parser!");
		exit(1);
	}

	parser->startElementHandler = StartElement;
	parser->endElementHandler = EndElement;	
	parser->charactersHandler = Characters; /* set to CharactersWide 
		to convert UTF-8 -> wchar_t */
	parser->processingInstructionHandler = PI;
	parser->commentHandler = Comment;
	parser->startCDATAHandler = StartCData;
	parser->endCDATAHandler = EndCData;
	parser->errorHandler = ErrorHandler;
	parser->startDTDHandler = DoctypeDecl;
	parser->skippedEntityHandler = SkippedEntity;
	parser->resolveEntityHandler = ResolveEntity;
	parser->externalEntityParsedHandler = FreeInputData;
	parser->UserData = parser;
	
	printf("XMLFlags:\n");
	printf("XMLFLAG_NAMESPACES: %d\n", _XMLParser_GetFlag(parser,XMLFLAG_NAMESPACES));
	printf("XMLFLAG_NAMESPACE_PREFIXES: %d\n", _XMLParser_GetFlag(parser,XMLFLAG_NAMESPACE_PREFIXES));
	printf("XMLFLAG_EXTERNAL_GENERAL_ENTITIES: %d\n", _XMLParser_GetFlag(parser,XMLFLAG_EXTERNAL_GENERAL_ENTITIES));
	printf("XMLFLAG_PRESERVE_GENERAL_ENTITIES: %d\n", _XMLParser_GetFlag(parser,XMLFLAG_PRESERVE_GENERAL_ENTITIES));
	printf("XMLFLAG_UNDEF_GENERAL_ENTITIES: %d\n", _XMLParser_GetFlag(parser,XMLFLAG_UNDEF_GENERAL_ENTITIES));
	printf("XMLFLAG_PRESERVE_WS_ATTRIBUTES: %d\n", _XMLParser_GetFlag(parser,XMLFLAG_PRESERVE_WS_ATTRIBUTES));
	
	XMLParser_Parse(parser, urlstream, pStm, NULL);
	
	if (pStm) pStm->lpVtbl->Release(pStm);
	XMLParser_Free(parser);
	
	return 0;
}



#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "libparsifal/parsifal.h"
#include "libparsifal/dtdvalid.h"

int StartElement(void *UserData, const XMLCH *uri, const XMLCH *localName, const XMLCH *qName, LPXMLVECTOR atts);
int StartElementSimple(void *UserData, const XMLCH *uri, const XMLCH *localName, const XMLCH *qName, LPXMLVECTOR atts);
int EndElement(void *UserData, const XMLCH *uri, const XMLCH *localName, const XMLCH *qName);
int Characters(void *UserData, const XMLCH *Chars, int cbChars);
int IgnorableWS(void *UserData, const XMLCH *Chars, int cbChars);
int StartDocument(void *UserData);
void ErrorHandler(LPXMLPARSER parser);
int cstream(BYTE *buf, int cBytes, int *cBytesActual, void *inputData);
/* see samples/misc/helper.c for comments of the following routines: */
size_t GetBaseDir(XMLCH *dst, XMLCH *src);
XMLCH *ResolveBaseUri(LPXMLPARSER parser, XMLCH *systemID, XMLCH *base);

#define NSVALIDP ((NSVALIDPARSER*)v->UserData)
#define ASSERT_MEM_ABORT(p) \
  if (!(p)) { printf("Out of memory! Line: %d\n", __LINE__); return XML_ABORT; }

typedef struct tagNSVALIDPARSER {
	int elemCount;
	LPXMLVECTOR filteredAtts;
	char *wantedUri;
	char base[FILENAME_MAX];
	char userDTD[FILENAME_MAX];
} NSVALIDPARSER;

int StartElement(void *UserData, const XMLCH *uri, const XMLCH *localName, const XMLCH *qName, LPXMLVECTOR atts)
{	
	LPXMLDTDVALIDATOR v = (LPXMLDTDVALIDATOR)UserData;
	if (!strcmp(uri, NSVALIDP->wantedUri)) {
		LPXMLVECTOR attsF;

		if (v->UserFlag) v->UserFlag = 0;
		NSVALIDP->elemCount++;
		
		if (!atts->length)
			attsF = atts;
		else {
			int i;
			LPXMLRUNTIMEATT a;
			
			if (NSVALIDP->filteredAtts->length) 
				_XMLVector_RemoveAll(NSVALIDP->filteredAtts);
			
			for (i=0; i<atts->length; i++) {
				a = XMLVector_Get(atts, i);
				if (!*a->uri || !strcmp(a->uri, NSVALIDP->wantedUri))
					a = XMLVector_Append(NSVALIDP->filteredAtts, a);
					ASSERT_MEM_ABORT(a);
			}
			attsF = NSVALIDP->filteredAtts;
		}
		
		return DTDValidate_StartElement(v, uri, localName, localName, attsF);
	}
	v->UserFlag++;
	return XML_OK;
}

int StartElementSimple(void *UserData, const XMLCH *uri, const XMLCH *localName, const XMLCH *qName, LPXMLVECTOR atts)
{	
	LPXMLDTDVALIDATOR v = (LPXMLDTDVALIDATOR)UserData;
	NSVALIDP->elemCount++;
	return XML_OK;
}

int EndElement(void *UserData, const XMLCH *uri, const XMLCH *localName, const XMLCH *qName)
{
	LPXMLDTDVALIDATOR v = (LPXMLDTDVALIDATOR)UserData;
	if (!strcmp(uri, NSVALIDP->wantedUri)) {
		if (v->UserFlag) v->UserFlag = 0;
		return DTDValidate_EndElement(v, uri, localName, localName);
	}
	v->UserFlag--;
	return XML_OK;
}

int Characters(void *UserData, const XMLCH *Chars, int cbChars)
{	
	LPXMLDTDVALIDATOR v = (LPXMLDTDVALIDATOR)UserData;
	return (v->UserFlag) ? XML_OK : DTDValidate_Characters(v, Chars, cbChars);
}

int IgnorableWS(void *UserData, const XMLCH *Chars, int cbChars)
{	
	LPXMLDTDVALIDATOR v = (LPXMLDTDVALIDATOR)UserData;
	return (v->UserFlag) ? XML_OK : DTDValidate_IgnorableWhitespace(v, Chars, cbChars);
}

void ErrorHandler(LPXMLPARSER parser) 
{	
	XMLCH *SystemID = XMLParser_GetSystemID(parser);
	LPXMLENTITY curEnt = XMLParser_GetCurrentEntity(parser);
	if (parser->ErrorCode == ERR_XMLP_VALIDATION) {
		LPXMLDTDVALIDATOR vp = (LPXMLDTDVALIDATOR)parser->UserData;
		printf("Validation Error: %s\nErrorLine: %d ErrorColumn: %d\n", 
			vp->ErrorString, vp->ErrorLine, vp->ErrorColumn);
	}
	else {
		printf("Parsing Error: %s\nErrorLine: %d ErrorColumn: %d\n", 
			parser->ErrorString, parser->ErrorLine, parser->ErrorColumn);
	}
	if (curEnt && !curEnt->systemID) printf("In entity: '%s'\n", curEnt->name);
	if (SystemID) printf("SystemID: '%s'\n", SystemID);
}

int cstream(BYTE *buf, int cBytes, int *cBytesActual, void *inputData)
{
	*cBytesActual = fread(buf, 1, cBytes, (FILE*)inputData);	
	return (*cBytesActual < cBytes);
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
	LPXMLDTDVALIDATOR v = (LPXMLDTDVALIDATOR)UserData;
	FILE *f;
	XMLCH r[FILENAME_MAX]; 
	XMLCH *filename = ResolveBaseUri(v->parser, entity->systemID, NSVALIDP->base);
	if (filename != entity->systemID) {
		strcpy(r, filename);
		filename = strcat(r, entity->systemID);
	}
	if (!(f = fopen(filename, "rb"))) {
		printf("error opening file '%s'!\n", filename);
		return XML_ABORT;
	}
	reader->inputData = f; 
	return XML_OK;
}

int FreeInputData(void *UserData, LPXMLENTITY entity, LPBUFFEREDISTREAM reader)
{	
	fclose((FILE*)reader->inputData);
	return XML_OK;
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

int main(int argc, char* argv[])
{	
	NSVALIDPARSER nsparser;
	LPXMLPARSER parser;	
	LPXMLDTDVALIDATOR vp;
	FILE *f;
	int i;
	XMLCH *filename = NULL;
	
	if (argc < 2 || argc > 4) goto USAGE;

	nsparser.elemCount = 0;
	nsparser.wantedUri = "";
	*nsparser.userDTD = '\0';
	*nsparser.base = '\0';
		
	for (i=1; i<argc; i++) {
		if (!strncmp(argv[i], "/DTD:", 5))
			strcpy(nsparser.userDTD, argv[i]+5);
		else if (!strncmp(argv[i], "/URI:", 5))
			nsparser.wantedUri = argv[i]+5;
		else {
			if (filename) goto USAGE;
			filename = argv[i];
		}
	}
	
	if (!filename) goto USAGE;
		
	if (!XMLParser_Create(&parser)) {
		puts("Out of memory!");
		return 1;
	}
	
	vp = XMLParser_CreateDTDValidator();
	if (!vp) {
		puts("Out of memory!");
		return 1;
	}

	if (!XMLVector_Create(&nsparser.filteredAtts, 0, sizeof(XMLRUNTIMEATT))) {
		puts("Out of memory!");
		return 1;
	}

	vp->UserData = &nsparser;
	parser->errorHandler = ErrorHandler;
	parser->resolveEntityHandler = ResolveEntity;
	parser->externalEntityParsedHandler = FreeInputData;

	if (*nsparser.userDTD) {
		int filepos = GetBaseDir(nsparser.base, nsparser.userDTD);
		if (filepos) strcpy(nsparser.userDTD, nsparser.userDTD+filepos);		
		XMLParser_SetExternalSubset(parser, NULL, nsparser.userDTD);
	}

	if (!*nsparser.base) 
		GetBaseDir(nsparser.base, filename);
	
	if (*nsparser.wantedUri) {
		vp->startElementHandlerFilter = StartElement;
		vp->endElementHandlerFilter = EndElement;
		vp->charactersHandlerFilter = Characters;
		vp->ignorableWhitespaceHandlerFilter = IgnorableWS;
	}
	else /* simple validator w/o namespace filter: */
		parser->startElementHandler = StartElementSimple;

	if (!(f = fopen(filename, "rb"))) {
		printf("Error opening file %s\n", filename);
		return 1;
	}
	
	if (XMLParser_ParseValidateDTD(vp, parser, cstream, f, 0)) {
		printf("Elements (total: %d) in namespace \"%s\" were valid\nin the" 
			" document %s\n", nsparser.elemCount, 
			((*nsparser.wantedUri) ? nsparser.wantedUri : "not specified"),
			filename);		
	}
		
	fclose(f);
	XMLVector_Free(nsparser.filteredAtts);
	XMLParser_FreeDTDValidator(vp);
	XMLParser_Free(parser);
	return 0;

USAGE:
	puts("Usage: nsvalid document.xml /DTD:<optional> /URI:<optional>\n"
		"Example: nsvalid my.xml /DTD:/home/my.dtd /URI:http://my.org");
	return 1;
}


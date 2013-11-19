#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "libparsifal/parsifal.h"

#define PFOUT (((CANONXMLPARSER*)UserData)->pfout)
#define XMLNSURI 	"http://www.w3.org/2000/xmlns/"
#define XMLURI		"http://www.w3.org/XML/1998/namespace"
#ifndef MAX_PATH
#define MAX_PATH 256
#endif

typedef struct tagCANONXMLPARSER {
	LPXMLPARSER parser;
	FILE *pfout;
	int hasIntSubset;
	char doctypeName[256];
	char xmlbase[MAX_PATH];
} CANONXMLPARSER;

/* parser events: */
int StartElement(void *UserData, const XMLCH *uri, const XMLCH *localName, const XMLCH *qName, LPXMLVECTOR atts);
int EndElement(void *UserData, const XMLCH *uri, const XMLCH *localName, const XMLCH *qName);
int EscCharacters(void *UserData, const XMLCH *chars, int cbChars);
int Characters(void *UserData, const XMLCH *chars, int cbChars);
int Comment(void *UserData, const XMLCH *chars, int cbChars);
int StartDTD(void *UserData, const XMLCH *Name, const XMLCH *publicId, const XMLCH *systemId, int hasInternalSubset);
int EndDTD(void *UserData);
int DummyEvent(void *UserData);
int XmlDecl(void *UserData, const XMLCH *version, const XMLCH *encoding, const XMLCH *standalone);
int Pi(void *UserData, const XMLCH *target, const XMLCH *data);
int ResolveEntity(void *UserData, LPXMLENTITY entity, LPBUFFEREDISTREAM reader);
int FreeInputData(void *UserData, LPXMLENTITY entity, LPBUFFEREDISTREAM reader);
int NotationDecl(void *UserData, const XMLCH *name, const XMLCH *publicID, const XMLCH *systemID);
int StartDTDforNotationDecl(void *UserData, const XMLCH *name, const XMLCH *publicId, const XMLCH *systemId, int hasInternalSubset);
int EndDTDforNotationDecl(void *UserData);

int cstream(BYTE *buf, int cBytes, int *cBytesActual, void *inputData);
LPXMLVECTOR CloneXMLVector(LPXMLVECTOR source);
int attcmp(const void *att1, const void *att2);
void PrintEsc(FILE *fp, const XMLCH *str, int len);

int cstream(BYTE *buf, int cBytes, int *cBytesActual, void *inputData)
{
	*cBytesActual = fread(buf, 1, cBytes, (FILE*)inputData);	
	return (*cBytesActual < cBytes);
}

int DummyEvent(void *UserData) 
{ 
	return 0; 
}

int StartElement(void *UserData, const XMLCH *uri, const XMLCH *localName, const XMLCH *qName, LPXMLVECTOR atts)
{	
	fprintf(PFOUT, "<%s", qName);
	if (atts->length) {
		int i;
		LPXMLRUNTIMEATT att;
		LPXMLVECTOR satts;
		
		if (atts->length > 1) {
			if (!(satts = CloneXMLVector(atts))) return XML_ABORT;
			qsort((void*)satts->array, satts->length, sizeof(XMLRUNTIMEATT), attcmp);
		}
		else {
			satts = atts;	
		}
			
		for (i=0; i<satts->length; i++)
		{
			att = XMLVector_Get(satts, i);
			fprintf(PFOUT, " %s=\"", att->qname);
			PrintEsc(PFOUT, att->value, strlen(att->value));
			putc('\"', PFOUT);
		}	
		if (atts->length > 1) XMLVector_Free(satts);	
	}
	putc('>', PFOUT);
	return 0;	
}

int EndElement(void *UserData, const XMLCH *uri, const XMLCH *localName, const XMLCH *qName)
{	
	fprintf(PFOUT, "</%s>", qName);
	return 0;
}

int EscCharacters(void *UserData, const XMLCH *chars, int cbChars)
{	
	PrintEsc(PFOUT, chars, cbChars);
	return 0;
}

int Characters(void *UserData, const XMLCH *chars, int cbChars)
{
	fwrite(chars, cbChars, 1, PFOUT);
	return 0;	
}

int NotationDecl(void *UserData, const XMLCH *name, const XMLCH *publicID, const XMLCH *systemID)
{	
	if (!((CANONXMLPARSER*)UserData)->hasIntSubset) {
		((CANONXMLPARSER*)UserData)->hasIntSubset = 1;
		StartDTD(UserData, ((CANONXMLPARSER*)UserData)->doctypeName, NULL, NULL, 1);
		putc('\xA', PFOUT);
	}
		
	fprintf(PFOUT, "<!NOTATION %s ", name);
	if (publicID) fprintf(PFOUT, "PUBLIC \'%s\'", publicID);
	else fputs("SYSTEM", PFOUT);
	
	if (systemID) fprintf(PFOUT, " \'%s\'>\xA", systemID);
	else fputs(">\xA", PFOUT);
	return 0;	
}       

int Comment(void *UserData, const XMLCH *chars, int cbChars)
{
	fputs("<!--", PFOUT);
	Characters(UserData, chars, cbChars);
	fputs("-->", PFOUT);
	return 0;
}

void PrintEsc(FILE *fp, const XMLCH *str, int len)
{
	for (; len--; str++) {
		switch(*str) {
			case '&': fputs("&amp;", fp); break;
			case '\"': fputs("&quot;", fp); break;
			/*case '\'': fprintf("&apos;", fp); break;*/
			case '<': fputs("&lt;", fp); break;
			case '>': fputs("&gt;", fp); break;
			case '\x9': fputs("&#9;", fp); break;
			case '\x0A': fputs("&#10;", fp); break;
			case '\x0D': fputs("&#13;", fp); break;
			default: fputc(*str, fp); break;
		}
	}
}

int Pi(void *UserData, const XMLCH *target, const XMLCH *data)
{
	fprintf(PFOUT, "<?%s %s?>", target, data);
	return 0;
}

int StartDTDforNotationDecl(void *UserData, const XMLCH *name, const XMLCH *publicId, const XMLCH *systemId, int hasInternalSubset)
{
	strcpy(((CANONXMLPARSER*)UserData)->doctypeName, name);
	return 0;
}

int EndDTDforNotationDecl(void *UserData)
{
	if (((CANONXMLPARSER*)UserData)->hasIntSubset) EndDTD(UserData);
	return 0;
}

int StartDTD(void *UserData, const XMLCH *name, const XMLCH *publicId, const XMLCH *systemId, int hasInternalSubset)
{
	fprintf(PFOUT, "<!DOCTYPE %s", name);
    
    if (publicId)
        fprintf(PFOUT, " PUBLIC \"%s\" \"%s\"", publicId, systemId);
    else if (systemId)
        fprintf(PFOUT, " SYSTEM \"%s\"", systemId);
    
    if (hasInternalSubset) {
    	((CANONXMLPARSER*)UserData)->hasIntSubset = 1;
    	fputs(" [", PFOUT);
    }	
    return 0;
}

int EndDTD(void *UserData)
{
	if (((CANONXMLPARSER*)UserData)->hasIntSubset) fputs("]>\xA", PFOUT);
	else fputs(">\xA", PFOUT);
	return 0;	
}

int ResolveEntity(void *UserData, LPXMLENTITY entity, LPBUFFEREDISTREAM reader)
{
	FILE *f;
	char file[MAX_PATH];
	
	strcpy(file, ((CANONXMLPARSER*)UserData)->xmlbase);
	strcat(file, entity->systemID);	
	 	
	if (!(f = fopen(file, "rb"))) {
		printf("error opening file '%s'!\n", file);
		return XML_ABORT;
	}	
	reader->inputData = f; 
	return 0;
}

/* note: externalEntityParsedHandler is never called unless reader->inputData
  parameter is given in resolveEntityHandler, thus it's safe to free
	the data without any checks.
   note also: this is called when parsing Error occurred too, so
   only thing you must ensure is that you SET VALID reader->inputData
   or NULL (default) in resolveEntityHandler */
int FreeInputData(void *UserData, LPXMLENTITY entity, LPBUFFEREDISTREAM reader)
{
	fclose((FILE*)reader->inputData);
	return 0;
}

LPXMLVECTOR CloneXMLVector(LPXMLVECTOR source)
{
	LPXMLVECTOR dest;
	if (!XMLVector_Create(&dest, source->length, source->itemSize)) return NULL;
	dest->length = source->length;
	memcpy(dest->array, source->array, dest->length * dest->itemSize);
	return dest;
}

/* attcmp for qsort 
  sorting attributes in canonical xml is quite tricky 'cos of namespace declarations.
  We handle some of it, not everything (like removing redundant xmlns="" attributes).
  here's what happens:
  - all xmlns/xmlns:xxx attributes are put before any other attribs
  - normal attributes e.g. att="val" are put before namespaced xxx:att attributes
  - if both attributes are in a namespace, they are sorted by uris, not qnames.
*/
int attcmp(const void *att1, const void *att2)
{
  	LPXMLRUNTIMEATT a1 = (LPXMLRUNTIMEATT)att1;
  	LPXMLRUNTIMEATT a2 = (LPXMLRUNTIMEATT)att2;
 	int n1 = (!(strcmp(a1->uri, XMLNSURI)));
 	int n2 = (!(strcmp(a2->uri, XMLNSURI)));
 	
 	if (n1 != n2) /* other one has XMLNSURI, it will be first: */
 		return (n2 - n1);
 	
 	if (!n1) { /* no XMLNSURI at all: */
	 	n1 = ((*a1->uri) != 0);
	  	n2 = ((*a2->uri) != 0);
	
	  	if (n1 != n2) /* other one has uri, it will be last: */
	  		return (n1 - n2);
	  	else if (n1) /* both have uris, compare them: */
	  		return strcmp((const XMLCH*)a1->uri, (const XMLCH*)a2->uri);  		
  	}
  	/* both have XMLNSURI or are normal attribs, compare qnames: */	
  	return strcmp((const XMLCH*)a1->qname, (const XMLCH*)a2->qname);
}

int main(int argc, char* argv[])
{	
	CANONXMLPARSER cxmlparser; /* UserData struct */
	LPXMLPARSER parser;		
	FILE *f;
	int ret;

	if (argc < 3 || argc > 4) {
		printf("Usage: canonxml infile.xml outfile.xml optionalbasedir/\n");
		return -1;	
	}
	
	if (argc == 4) 
		strcpy(cxmlparser.xmlbase, argv[3]);
	else 
		*(cxmlparser.xmlbase) = '\0';
	
	if (!XMLParser_Create(&parser)) {
		printf("Error creating parser in main()\n");
		return -1;
	}
	
	parser->UserData = &cxmlparser;
	parser->startElementHandler = StartElement;
	parser->endElementHandler = EndElement;	
	parser->charactersHandler = parser->ignorableWhitespaceHandler = EscCharacters;
	parser->startCDATAHandler = DummyEvent;
	parser->notationDeclHandler = NotationDecl;
	parser->startDTDHandler = StartDTDforNotationDecl;
	parser->endDTDHandler = EndDTDforNotationDecl;
	parser->processingInstructionHandler = Pi;
	parser->resolveEntityHandler = ResolveEntity;
	parser->externalEntityParsedHandler = FreeInputData;
	/*
	parser->commentHandler = Comment;
	parser->errorHandler = ErrorHandler;
	parser->defaultHandler = Characters;
	parser->startDTDHandler = StartDTD;
	parser->endDTDHandler = EndDTD;
	*/
	cxmlparser.hasIntSubset = 0;
	
	if (!(f = fopen(argv[1], "rb"))) {
		printf("Error opening file %s\n", argv[1]);
		return -1;
	}
	
	if (!(cxmlparser.pfout = fopen(argv[2], "wb"))) {
		printf("Error opening file %s\n", argv[2]);
		return -1;
	}
	
	_XMLParser_SetFlag(parser, XMLFLAG_NAMESPACE_PREFIXES, 1);
	
	XMLParser_Parse(parser, cstream, f, NULL);
	
	ret = (parser->ErrorCode != ERR_XMLP_ABORT) ? parser->ErrorCode : -1;	
	fclose(f);
	fclose(cxmlparser.pfout);
	XMLParser_Free(parser);
	return ret;
}


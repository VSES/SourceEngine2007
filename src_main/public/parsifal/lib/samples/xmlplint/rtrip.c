/* TODO: unicode sorting for attributes */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#ifdef _WIN32
#include <windows.h>
#endif
#include "libparsifal/parsifal.h"
#include "xmlplint.h"

#define XMLNSURI "http://www.w3.org/2000/xmlns/"

extern int xmlSortAtts;
extern int xmlOutputFormat;
extern int xmlDoValidate;

extern int xmlHasInternalSubset;
extern FILE *fout;
extern LPXMLPARSER parser;
static LPXMLVECTOR notations=NULL;
static int inDTD;
static XMLCH *docElement;

static LPXMLVECTOR CloneXMLVector(LPXMLVECTOR source)
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
static int attcmp(const void *att1, const void *att2)
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


static void PrintEsc(FILE *fp, const XMLCH *str, int len, int useRefLF)
{
	for (; (len==-1)?*str:len--; str++) {
		switch(*str) {
			case '&': fputs("&amp;", fp); break;
			case '\"': fputs("&quot;", fp); break;
			/*case '\'': fprintf("&apos;", fp); break;*/
			case '<': fputs("&lt;", fp); break;
			case '>': fputs("&gt;", fp); break;
			case '\x9': fputs("&#9;", fp); break;
			case '\x0A': 
				if (useRefLF) fputs("&#10;", fp);
				else fputc('\xA', fp);
				break;
			case '\x0D': fputs("&#13;", fp); break;
			default: fputc(*str, fp); break;
		}
	}
}

static int StartElement(void *UserData, const XMLCH *uri, const XMLCH *localName, const XMLCH *qName, LPXMLVECTOR atts)
{	
	fprintf(PFOUT, "<%s", qName);
	if (atts->length) {
		int i;
		LPXMLRUNTIMEATT att;
		LPXMLVECTOR satts;
		
		if (xmlSortAtts && atts->length > 1) {
			satts = CloneXMLVector(atts);
			ASSERT_MEM_ABORT(satts);
			qsort((void*)satts->array, satts->length, sizeof(XMLRUNTIMEATT), attcmp);
		}
		else {
			satts = atts;	
		}
			
		for (i=0; i<satts->length; i++) {
			att = XMLVector_Get(satts, i);
			fprintf(PFOUT, " %s=\"", att->qname);
			PrintEsc(PFOUT, att->value, -1, 1);
			putc('\"', PFOUT);
		}	
		if (xmlSortAtts && atts->length > 1) XMLVector_Free(satts);	
	}
	putc('>', PFOUT);
	return 0;	
}

static int EndElement(void *UserData, const XMLCH *uri, const XMLCH *localName, const XMLCH *qName)
{	
	fprintf(PFOUT, "</%s>", qName);
	return 0;
}

static int EscCharacters(void *UserData, const XMLCH *chars, int cbChars)
{	
	PrintEsc(PFOUT, chars, cbChars, xmlOutputFormat==XMLOUTPUTFORMAT_CANONICAL);
	return 0;
}

static int Characters(void *UserData, const XMLCH *chars, int cbChars)
{
	fwrite(chars, cbChars, 1, PFOUT);
	return 0;	
}

static int StartCDATA(void *UserData) 
{
	fputs("<![CDATA[", PFOUT);
	parser->charactersHandler = Characters;
	return 0;
}

static int EndCDATA(void *UserData)
{
	fputs("]]>", PFOUT);
	parser->charactersHandler = EscCharacters;
	return 0;
}

static int notationcmp(const void *n1, const void *n2)
{
	return strcmp(((struct XMLNOTATION*)n1)->name,
		((struct XMLNOTATION*)n2)->name);
}

static int EndDocumentCleanup(void *UserData)
{
	if (notations) {
		XMLVector_Free(notations);
		notations=NULL;	
	}
	free(docElement);
	return XML_OK;
}

static int NotationDecl2(void *UserData, const XMLCH *name, const XMLCH *publicID, const XMLCH *systemID)
{	
	struct XMLNOTATION *n;
	if (!notations) {
		notations = XMLVector_Create(&notations, 0, sizeof(struct XMLNOTATION));
		if (!notations) goto EXITNOMEM;
	}
	else { /* already exists? */
		int i;
		for (i=0; i<notations->length; i++) {
			n = XMLVector_Get(notations, i);
			if (!strcmp(name, n->name)) return XML_OK;
		}
	}

	if (!(n = XMLVector_Append(notations, NULL))) 
		goto EXITNOMEM;

	n->publicID = NULL;
	n->systemID = NULL;

	if (!(n->name = strdup(name))) 
		goto EXITNOMEM;

	if (publicID) {
		n->publicID = strdup(publicID);
		if (!n->publicID) goto EXITNOMEM;
	}

	if (systemID) {
		n->systemID = strdup(systemID);
		if (!n->systemID) goto EXITNOMEM;			
	}
	return XML_OK;
EXITNOMEM:
	fprintf(stderr, "Out of memory!\n");
	return XML_ABORT;
}

static int NotationDecl(void *UserData, const XMLCH *name, const XMLCH *publicID, const XMLCH *systemID)
{	
	fprintf(PFOUT, "<!NOTATION %s ", name);
	if (publicID) fprintf(PFOUT, "PUBLIC \'%s\'", publicID);
	else fputs("SYSTEM", PFOUT);
	
	if (systemID) fprintf(PFOUT, " \'%s\'>\xA", systemID);
	else fputs(">\xA", PFOUT);
	return 0;
}       

static int Comment(void *UserData, const XMLCH *chars, int cbChars)
{
	fputs("<!--", PFOUT);
	Characters(UserData, chars, cbChars);
	fputs("-->", PFOUT);
	if (inDTD) fputc('\xA', PFOUT);
	return 0;
}

static int Pi(void *UserData, const XMLCH *target, const XMLCH *data)
{
	fprintf(PFOUT, "<?%s %s?>", target, data);
	if (inDTD) fputc('\xA', PFOUT);
	return 0;
}

static int StartDTD(void *UserData, const XMLCH *name, const XMLCH *publicId, const XMLCH *systemId, int hasInternalSubset)
{	
	if (xmlOutputFormat != XMLOUTPUTFORMAT_CANONICAL) {
		fprintf(PFOUT, "<!DOCTYPE %s", name);
		xmlHasInternalSubset = hasInternalSubset;	
		if (publicId)
			fprintf(PFOUT, " PUBLIC \"%s\" \"%s\"", publicId, systemId);
		else if (systemId)
			fprintf(PFOUT, " SYSTEM \"%s\"", systemId);
		if (xmlHasInternalSubset) fputs(" [\xA", PFOUT);
	}
	else {
		if (!(docElement = strdup(name))) {
			fprintf(stderr, "Out of memory!\n");
			return XML_ABORT;
		}
	}
	inDTD = 1;
    return 0;
}

static int EndDTD(void *UserData)
{	
	if (notations) { /* xmlOutputFormat == XMLOUTPUTFORMAT_CANONICAL */
		int i;
		struct XMLNOTATION *n;
		fprintf(PFOUT, "<!DOCTYPE %s [\xA", docElement);
		qsort((void*)notations->array, notations->length, 
			sizeof(struct XMLNOTATION), notationcmp);
		for(i=0; i<notations->length; i++) {
			n = XMLVector_Get(notations, i);
			NotationDecl(NULL, n->name, n->publicID, n->systemID);
			if (n->name) free(n->name);
			if (n->publicID) free(n->publicID);
			if (n->systemID) free(n->systemID);
		}
		fputs("]>\xA", PFOUT);
	}
	else if (xmlOutputFormat != XMLOUTPUTFORMAT_CANONICAL)
		fputs((xmlHasInternalSubset) ? "]>\xA" : ">\xA", PFOUT);
	inDTD = 0;
	return 0;	
}

static int AttributeDecl(void *UserData, const XMLCH *eName,
	const XMLCH *aName, int type, const XMLCH *typeStr, int valueDef,
	const XMLCH *def)
{
	fprintf(PFOUT, "<!ATTLIST %s %s ", eName, aName);
	switch(type) {
		case XMLATTDECL_TYPE_CDATA: fputs("CDATA", PFOUT); break;
		case XMLATTDECL_TYPE_ID: fputs("ID", PFOUT); break;
		case XMLATTDECL_TYPE_IDREF: fputs("IDREF", PFOUT); break;        
		case XMLATTDECL_TYPE_IDREFS: fputs("IDREFS", PFOUT); break;             
		case XMLATTDECL_TYPE_ENTITY: fputs("ENTITY", PFOUT); break;       
		case XMLATTDECL_TYPE_ENTITIES: fputs("ENTITIES", PFOUT); break; 
		case XMLATTDECL_TYPE_NMTOKEN: fputs("NMTOKEN", PFOUT); break;
		case XMLATTDECL_TYPE_NMTOKENS: fputs("NMTOKENS", PFOUT); break;   
		case XMLATTDECL_TYPE_NOTATION: fputs("NOTATION ", PFOUT); /* fall through */
		case XMLATTDECL_TYPE_ENUMERATED: fputs(typeStr, PFOUT); break;      
	}
	switch(valueDef) {
		case XMLATTDECL_DEF_FIXED: fputs(" #FIXED", PFOUT); break;
		case XMLATTDECL_DEF_REQUIRED: fputs(" #REQUIRED", PFOUT); break;
		case XMLATTDECL_DEF_IMPLIED: fputs(" #IMPLIED", PFOUT); break;
	}
	
	if (def) fprintf(PFOUT, " \"%s\">\xA", def);
	else fputs(">\xA", PFOUT);
	return 0;
}    

static int ElementDecl(void *UserData, const XMLCH *name, void *contentModel)
{
	fprintf(PFOUT, "<!ELEMENT %s %s>\xA", name, (XMLCH*)contentModel);
	return 0;
}

static int EntityDecl(void *UserData, LPXMLENTITY entity)
{	
	fputs("<!ENTITY ", PFOUT);
	if (entity->type == XML_ENTITY_INT_PARAM || entity->type == XML_ENTITY_EXT_PARAM)
		fprintf(PFOUT, "%% %s", entity->name+1);
	else
		fputs(entity->name, PFOUT);
	
	if (entity->value)
		fprintf(PFOUT, " \"%s\">\xA", entity->value);
	else {
		if (entity->publicID) fprintf(PFOUT, " PUBLIC \"%s\" \"%s\"", entity->publicID, entity->systemID);
		else fprintf(PFOUT, " SYSTEM \"%s\"", entity->systemID);
		
		if (entity->notation) fprintf(PFOUT, " NDATA %s>\xA", entity->notation);
		else fputs(">\xA", PFOUT);
	}	
	return 0;
}

static void ContentModel2ToString(XMLCP *cp)
{
	XMLCP *n;	 
	fputc('(', PFOUT);
	for(n=cp->children; n; n=n->next) {	
		if (n->children) {
			ContentModel2ToString(n);
		}
		else {
			fputs(n->name, PFOUT);
			if (n->rep) fputc(n->rep, PFOUT);
		}
		if (n->next) {
			fputc(((cp->type == XMLCTYPE_CHOICE) ? '|' : ','), PFOUT);
		}
	}
	fputc(')', PFOUT);
	if (cp->rep) fputc(cp->rep, PFOUT);
}

void ContentModelToString(XMLCP *cp)
{
	XMLCP *n;
	switch(cp->type) {
		case XMLCTYPE_EMPTY: fputs("EMPTY", PFOUT); break;
		case XMLCTYPE_ANY: fputs("ANY", PFOUT); break;
		case XMLCTYPE_MIXED: 
			fputs("(#PCDATA", PFOUT);		
			for(n=cp->children; n; n=n->next) {
				fputc('|', PFOUT);
				fputs(n->name, PFOUT);
			}
			fputc(')', PFOUT);
			if (cp->rep) fputc(cp->rep, PFOUT);
			break;
		default: ContentModel2ToString(cp);
	}
}

/* we must build string representation of content particles (XMLCP) when
roundtripping in validating mode, otherwise simple ElementDecl() works */
static int ElementDecl2(void *UserData, const XMLCH *name, void *contentModel)
{
	fprintf(PFOUT, "<!ELEMENT %s ", name);
	ContentModelToString(contentModel);
	fputc('>', PFOUT);
	fputc('\xA', PFOUT);
	return 0;
}

void rtrip_SetHandlers(LPXMLPARSER p)
{	
	if (xmlOutputFormat == XMLOUTPUTFORMAT_CANONICAL) {
		xmlSortAtts = 1;
		p->notationDeclHandler = NotationDecl2;
		p->endDocumentHandler = EndDocumentCleanup;
	}
	else {
		p->elementDeclHandler = (!xmlDoValidate) ? ElementDecl : ElementDecl2;
		p->attributeDeclHandler = AttributeDecl;
		p->entityDeclHandler = EntityDecl;
		p->notationDeclHandler = NotationDecl;
		p->startCDATAHandler = StartCDATA;
		p->endCDATAHandler = EndCDATA;
		p->commentHandler = Comment;
		p->processingInstructionHandler = Pi;
		
	}
	p->startElementHandler = StartElement;
	p->endElementHandler = EndElement;	
	p->charactersHandler = p->ignorableWhitespaceHandler = EscCharacters;
	p->startDTDHandler = StartDTD;
	p->endDTDHandler = EndDTD;
}

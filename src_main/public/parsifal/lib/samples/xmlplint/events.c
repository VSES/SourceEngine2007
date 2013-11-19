#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#ifdef _WIN32
#include <windows.h>
#endif
#include "libparsifal/parsifal.h"
#include "xmlplint.h"

extern FILE *fout;
extern int xmlIgnorableWS;
extern int xmlDoValidate;
extern void ContentModelToString(XMLCP *cp); 

static int StartElement(void *UserData, const XMLCH *uri, const XMLCH *localName, const XMLCH *qName, LPXMLVECTOR atts)
{	
	if (*uri)
		fprintf(PFOUT, "startElement(qName {%s} uri {%s} localName {%s})\n",
			qName, uri, localName);
	else
		fprintf(PFOUT, "startElement(qName {%s})\n", qName);

	if (atts->length) {
		LPXMLRUNTIMEATT pAtt, pEnd;
		fprintf(PFOUT, "  %d attribute(s):\n", atts->length);
		pEnd = (LPXMLRUNTIMEATT)_XMLVector_GetIterP(atts, pAtt);
		for (; pAtt!=pEnd; pAtt++) {
			if (*pAtt->uri)
				fprintf(PFOUT, "    qname {%s} value {%s} uri {%s} localName {%s}\n", 
					pAtt->qname, pAtt->value, pAtt->uri, pAtt->localName);
			else
				fprintf(PFOUT, "    qname {%s} value {%s}\n", pAtt->qname, pAtt->value);
		}
	}
	return 0;
}

static int EndElement(void *UserData, const XMLCH *uri, const XMLCH *localName, const XMLCH *qName)
{
	if (*uri)
		fprintf(PFOUT, "endElement(qName {%s} uri {%s} localName {%s})\n",
			qName, uri, localName);
	else
		fprintf(PFOUT, "endElement(qName {%s})\n", qName);
	return 0;
}

static int Characters(void *UserData, const XMLCH *chars, int cbChars)
{	
	fputs("Text {", PFOUT);
	fwrite(chars, cbChars, 1, PFOUT);
	fputs("}\n", PFOUT);
	return 0;	
}

static int IgnorableWS(void *UserData, const XMLCH *chars, int cbChars)
{	
	fprintf(PFOUT, "ignorableWhitespace(%d bytes)\n", cbChars);
	return 0;
}

static void Simple(char *s)
{
	fprintf(PFOUT, "%s()\n", s);
}

static int StartCDATA(void *UserData) 
{
	Simple("startCDATA");
	return 0;
}

static int EndCDATA(void *UserData)
{
	Simple("endCDATA");
	return 0;
}

static int StartDocument(void *UserData)
{
	Simple("startDocument");
	return 0;
}

static int EndDocument(void *UserData)
{
	Simple("endDocument");
	return 0;
}

static int Comment(void *UserData, const XMLCH *chars, int cbChars)
{	
	fputs("Comment(Text {", PFOUT);
	fwrite(chars, cbChars, 1, PFOUT);
	fputs("})\n", PFOUT);
	return 0;
}

static int Pi(void *UserData, const XMLCH *target, const XMLCH *data)
{
	fprintf(PFOUT, "processingInstruction(target {%s} data {%s})\n", target, data);
	return 0;
}

static int SkippedEntity(void *UserData, const XMLCH *Name)
{
	fprintf(PFOUT, "skippedEntity(Name {%s})\n", Name);
	return 0;
}

static int XmlDecl(void *UserData, const XMLCH *version, const XMLCH *encoding, const XMLCH *standalone)
{
	fprintf(PFOUT, "xmlDecl(version {%s}", version);
	if (encoding) fprintf(PFOUT, " encoding {%s}", encoding);
	if (standalone) fprintf(PFOUT, " standalone {%s}", standalone);
	fputs(")\n", PFOUT);
	return 0;
}

void OutputEntity(const XMLCH *publicId, const XMLCH *systemId, 
						 const XMLCH *value, int type)
{
	if (publicId) fprintf(PFOUT, " publicID {%s}", publicId);
	if (systemId) fprintf(PFOUT, " systemID {%s}", systemId);
	else if (value) fprintf(PFOUT, " value {%s}", value);
	
	if (type!=-1) {
		char *t=NULL;
		switch(type) {
			case XML_ENTITY_INT_PARAM: t="XML_ENTITY_INT_PARAM"; break;
			case XML_ENTITY_INT_GEN: t="XML_ENTITY_INT_GEN"; break;
			case XML_ENTITY_EXT_PARAM: t="XML_ENTITY_EXT_PARAM"; break;
			case XML_ENTITY_EXT_GEN: t="XML_ENTITY_EXT_GEN"; break;
			case XML_ENTITY_UNPARSED: t="XML_ENTITY_UNPARSED"; break;
			case XML_ENTITY_DOCTYPE: t="XML_ENTITY_DOCTYPE"; break;
		}
		fprintf(PFOUT, " type {%s})\n", t);
	}
}

static int StartDTD(void *UserData, const XMLCH *Name, const XMLCH *publicId,
                             const XMLCH *systemId, int hasInternalSubset)
{
	fprintf(PFOUT, "startDTD(Name {%s}", Name);
	OutputEntity(publicId, systemId, NULL, -1);
	fprintf(PFOUT, " hasInternalSubset {%d})\n", hasInternalSubset);
	return 0;
}

static int EndDTD(void *UserData)
{
	Simple("endDTD");
	return 0;
}

static int StartEntity(void *UserData, LPXMLENTITY entity)
{
	fprintf(PFOUT, "startEntity(Name {%s}", entity->name);
	OutputEntity(entity->publicID, entity->systemID, 
		entity->value, entity->type);
	return 0;
}

static int EndEntity(void *UserData, LPXMLENTITY entity)
{
	fprintf(PFOUT, "endEntity(Name {%s}", entity->name);
	OutputEntity(entity->publicID, entity->systemID, 
		entity->value, entity->type);
	return 0;
}

static int ElementDecl(void *UserData, const XMLCH *name, void *contentModel)
{
	fprintf(PFOUT, "elementDecl(name {%s} contentModel {", name);
	if (xmlDoValidate) ContentModelToString(contentModel);
	else fputs((char*)contentModel, PFOUT);
	fputs("})\n", PFOUT);
	return 0;
}

static int AttributeDecl(void *UserData, const XMLCH *eName,
    const XMLCH *aName, int type, const XMLCH *typeStr, int valueDef,
    const XMLCH *def)
{
	fprintf(PFOUT, "attributeDecl(eName {%s} aName {%s} type {", eName, aName);
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
	fputs("} valueDef {", PFOUT);
	switch(valueDef) {
		case XMLATTDECL_DEF_FIXED: fputs("#FIXED}", PFOUT); break;
		case XMLATTDECL_DEF_REQUIRED: fputs("#REQUIRED}", PFOUT); break;
		case XMLATTDECL_DEF_IMPLIED: fputs("#IMPLIED}", PFOUT); break;
		default: fputs("0}", PFOUT);
	}	
	if (def) fprintf(PFOUT, " def {%s})\n", def);
	else fputs(")\n", PFOUT);
	return 0;
}

static int EntityDecl(void *UserData, LPXMLENTITY entity)
{	
	fprintf(PFOUT, "entityDecl(Name {%s}", entity->name);
	if (entity->notation) fprintf(PFOUT, " notation {%s}", entity->notation);
	OutputEntity(entity->publicID, entity->systemID, 
		entity->value, entity->type);
	return 0;
}

static int NotationDecl(void *UserData, const XMLCH *name,
    const XMLCH *publicID, const XMLCH *systemID)
{
	fprintf(PFOUT, "notationDecl(Name {%s}", name);
	OutputEntity(publicID, systemID, NULL, -1);
	fputs(")\n", PFOUT);
	return 0;
}

void events_SetHandlers(LPXMLPARSER p)
{
	p->startElementHandler = StartElement;
	p->endElementHandler = EndElement;
	p->charactersHandler = Characters;
	if (xmlIgnorableWS) p->ignorableWhitespaceHandler = IgnorableWS;
	p->startCDATAHandler = StartCDATA;
	p->endCDATAHandler = EndCDATA;
	p->commentHandler = Comment;
	p->startDocumentHandler = StartDocument;
	p->endDocumentHandler = EndDocument;
	p->processingInstructionHandler = Pi;
	p->skippedEntityHandler = SkippedEntity;
	p->xmlDeclHandler = XmlDecl;
	p->startDTDHandler = StartDTD;
	p->endDTDHandler = EndDTD;
	p->startEntityHandler = StartEntity;
	p->endEntityHandler = EndEntity;
	p->elementDeclHandler = ElementDecl;
	p->attributeDeclHandler = AttributeDecl;
	p->entityDeclHandler = EntityDecl;
	p->notationDeclHandler = NotationDecl;
}

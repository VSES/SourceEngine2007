#define PFOUT stdout

int AttributeDecl(void *UserData, const XMLCH *eName,
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
	
	if (def) fprintf(PFOUT, " \"%s\">\n", def);
	else fputs(">\n", PFOUT);
	return 0;
}    

int ElementDecl(void *UserData, const XMLCH *name, void *contentModel)
{
	fprintf(PFOUT, "<!ELEMENT %s %s>\n", name, (XMLCH*)contentModel);
	return 0;
}   

int EntityDecl(void *UserData, LPXMLENTITY entity)
{	
	fputs("<!ENTITY ", PFOUT);
	if (entity->type == XML_ENTITY_INT_PARAM || entity->type == XML_ENTITY_EXT_PARAM)
		fprintf(PFOUT, "%% %s", entity->name+1);
	else
		fputs(entity->name, PFOUT);
	
	if (entity->value)
		fprintf(PFOUT, " \"%s\">\n", entity->value);
	else {
		if (entity->publicID) fprintf(PFOUT, " PUBLIC \"%s\" \"%s\"", entity->publicID, entity->systemID);
		else fprintf(PFOUT, " SYSTEM \"%s\"", entity->systemID);
		
		if (entity->notation) fprintf(PFOUT, " NDATA %s>\n", entity->notation);
		else fputs(">\n", PFOUT);
	}	
	return 0;
}

int NotationDecl(void *UserData, const XMLCH *name, const XMLCH *publicID, const XMLCH *systemID)
{	
	fprintf(PFOUT, "<!NOTATION %s ", name);
	if (publicID) fprintf(PFOUT, "PUBLIC \"%s\"", publicID);
	else fputs("SYSTEM", PFOUT);
	
	if (systemID) fprintf(PFOUT, " \"%s\">\n", systemID);
	else fputs(">\n", PFOUT);
	return 0;	
}    

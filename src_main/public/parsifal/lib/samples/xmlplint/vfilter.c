#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#ifdef _WIN32
#include <windows.h>
#endif
#include "libparsifal/parsifal.h"
#include "libparsifal/dtdvalid.h"
#include "xmlplint.h"

static char *vuri;
static LPXMLVECTOR filteredAtts=NULL;

static int StartElement(void *UserData, const XMLCH *uri, const XMLCH *localName, const XMLCH *qName, LPXMLVECTOR atts)
{	
	LPXMLDTDVALIDATOR v = (LPXMLDTDVALIDATOR)UserData;
	if (!strcmp(uri, vuri)) {
		LPXMLVECTOR attsF;

		if (v->UserFlag) v->UserFlag = 0;
		
		if (!atts->length)
			attsF = atts;
		else {
			int i;
			LPXMLRUNTIMEATT a;
			
			if (filteredAtts->length) 
				_XMLVector_RemoveAll(filteredAtts);
			
			for (i=0; i<atts->length; i++) {
				a = XMLVector_Get(atts, i);
				if (!*a->uri || !strcmp(a->uri, vuri))
					a = XMLVector_Append(filteredAtts, a);
					ASSERT_MEM_ABORT(a);
			}
			attsF = filteredAtts;
		}
		
		return DTDValidate_StartElement(v, uri, localName, localName, attsF);
	}
	v->UserFlag++;
	return XML_OK;
}

static int EndElement(void *UserData, const XMLCH *uri, const XMLCH *localName, const XMLCH *qName)
{
	LPXMLDTDVALIDATOR v = (LPXMLDTDVALIDATOR)UserData;
	if (!strcmp(uri, vuri)) {
		if (v->UserFlag) v->UserFlag = 0;
		return DTDValidate_EndElement(v, uri, localName, localName);
	}
	v->UserFlag--;
	return XML_OK;
}

static int Characters(void *UserData, const XMLCH *Chars, int cbChars)
{	
	LPXMLDTDVALIDATOR v = (LPXMLDTDVALIDATOR)UserData;
	return (v->UserFlag) ? XML_OK : DTDValidate_Characters(v, Chars, cbChars);
}

static int IgnorableWS(void *UserData, const XMLCH *Chars, int cbChars)
{	
	LPXMLDTDVALIDATOR v = (LPXMLDTDVALIDATOR)UserData;
	return (v->UserFlag) ? XML_OK : DTDValidate_IgnorableWhitespace(v, Chars, cbChars);
}

int vfilter_Init(LPXMLDTDVALIDATOR dtd, char *uri)
{
	if (dtd) {
		vuri = uri;
		if (!XMLVector_Create(&filteredAtts, 0, sizeof(XMLRUNTIMEATT))) {
			fputs("Out of memory!", stderr);
			return 0;
		}
		dtd->startElementHandlerFilter = StartElement;
		dtd->endElementHandlerFilter = EndElement;
		dtd->charactersHandlerFilter = Characters;
		dtd->ignorableWhitespaceHandlerFilter = IgnorableWS;
	}
	return 1;
}

void vfilter_Cleanup(LPXMLDTDVALIDATOR dtd)
{
	if (dtd && filteredAtts) {
		XMLVector_Free(filteredAtts);
		filteredAtts = NULL;
	}
}


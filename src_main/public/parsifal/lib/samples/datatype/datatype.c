#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stddef.h>
#include "libparsifal/parsifal.h"
#include "libparsifal/dtdvalid.h"

#define ASSERT_MEM_ABORT(p) \
  if (!(p)) { printf("Out of memory! Line: %d\n", __LINE__); return XML_ABORT; }
#define ASSERT_SCANF_ABORT(r, items, field) \
  if (((r)==EOF || (r)!=(items))) { printf("Illegal value for %s\n", (field)); return XML_ABORT; }
#define SAFE_FREE(p) if(p!=NULL) free(p);
#define BOOKP ((BOOKPARSER*)v->UserData)

int StartElement(void *UserData, const XMLCH *uri, const XMLCH *localName, const XMLCH *qName, LPXMLVECTOR atts);
int EndElement(void *UserData, const XMLCH *uri, const XMLCH *localName, const XMLCH *qName);
int Characters(void *UserData, const XMLCH *Chars, int cbChars);
void ErrorHandler(LPXMLPARSER parser);
int cstream(BYTE *buf, int cBytes, int *cBytesActual, void *inputData);

typedef struct tagBOOK {
	int id;
	char *author, *title, *genre, *publish_date, *description;
	float price;
} BOOK;

typedef struct tagBOOKPARSER {
	XMLSTRINGBUF textBuf;
	LPXMLVECTOR books;
	BOOK *curBook;
	int fld;
} BOOKPARSER;

int StartElement(void *UserData, const XMLCH *uri, const XMLCH *localName, const XMLCH *qName, LPXMLVECTOR atts)
{	
	LPXMLDTDVALIDATOR v = (LPXMLDTDVALIDATOR)UserData;
	
	if (!strcmp(qName, "book")) {
		int ret;
		LPXMLRUNTIMEATT a; 
		/* append and initialize new book: */
		BOOKP->curBook = XMLVector_Append(BOOKP->books, NULL);
		ASSERT_MEM_ABORT(BOOKP->curBook);
		memset(BOOKP->curBook, 0, sizeof(struct tagBOOK));
		BOOKP->fld = -1;
		/* get book id: */
		a = XMLParser_GetNamedItem(v->parser, "id");
		/* a == NULL doesn't happen 'cos id is #REQUIRED */
		ret = sscanf(a->value, "bk%d", &BOOKP->curBook->id);
		ASSERT_SCANF_ABORT(ret, 1, a->qname);
	}
	else if (!strcmp(qName, "author")) BOOKP->fld = offsetof(BOOK, author);
	else if (!strcmp(qName, "title")) BOOKP->fld = offsetof(BOOK, title);
	else if (!strcmp(qName, "genre")) BOOKP->fld = offsetof(BOOK, genre);
	else if (!strcmp(qName, "price")) BOOKP->fld = offsetof(BOOK, price);
	else if (!strcmp(qName, "publish_date")) BOOKP->fld = offsetof(BOOK, publish_date);
	else if (!strcmp(qName, "description")) BOOKP->fld = offsetof(BOOK, description);
	return XML_OK;
}

int EndElement(void *UserData, const XMLCH *uri, const XMLCH *localName, const XMLCH *qName)
{
	LPXMLDTDVALIDATOR v = (LPXMLDTDVALIDATOR)UserData;
	if (BOOKP->fld != -1) {
		char *s = XMLStringbuf_ToString(&BOOKP->textBuf);
		ASSERT_MEM_ABORT(s);
		if (BOOKP->fld == offsetof(BOOK, price)) {
			int ret = sscanf(s, "%f", &BOOKP->curBook->price);
			ASSERT_SCANF_ABORT(ret, 1, qName);
		}
		else {
			char **sp = (char**)((char*)BOOKP->curBook+BOOKP->fld);
			*sp = strdup(s);
			ASSERT_MEM_ABORT(*sp);
		}
		ASSERT_MEM_ABORT(XMLStringbuf_SetLength(&BOOKP->textBuf, 0));
		BOOKP->fld = -1;
	}
	return XML_OK;
}

int Characters(void *UserData, const XMLCH *Chars, int cbChars)
{	
	LPXMLDTDVALIDATOR v = (LPXMLDTDVALIDATOR)UserData;
	ASSERT_MEM_ABORT(XMLStringbuf_Append(&BOOKP->textBuf, (XMLCH*)Chars, cbChars));
	return XML_OK;
}

void ErrorHandler(LPXMLPARSER parser) 
{
	if (parser->ErrorCode == ERR_XMLP_VALIDATION) {
		LPXMLDTDVALIDATOR vp = (LPXMLDTDVALIDATOR)parser->UserData;
		printf("Validation Error: %s\nErrorLine: %d ErrorColumn: %d\n", 
			vp->ErrorString, vp->ErrorLine, vp->ErrorColumn);
	}
	else {
		printf("Parsing Error: %s\nErrorLine: %d ErrorColumn: %d\n", 
			parser->ErrorString, parser->ErrorLine, parser->ErrorColumn);
	}
}

int cstream(BYTE *buf, int cBytes, int *cBytesActual, void *inputData)
{
	*cBytesActual = fread(buf, 1, cBytes, (FILE*)inputData);	
	return (*cBytesActual < cBytes);
}

int main(int argc, char* argv[])
{	
	BOOKPARSER bparser;
	BOOK *b;
	LPXMLPARSER parser;	
	LPXMLDTDVALIDATOR vp;
	int i, success;
	
	if (!XMLParser_Create(&parser)) {
		puts("Error creating parser!");
		return 1;
	}
	
	vp = XMLParser_CreateDTDValidator();
	if (!vp) {
		puts("Error creating DTDValidator in main()");
		return 1;
	}
	
	if (!XMLVector_Create(&bparser.books, 6, sizeof(BOOK))) {
		puts("Error creating books vector in main()");
		return 1;
	}
	
	/* init Stringbuf: blockSize 256, no pre-allocation: */
	XMLStringbuf_Init(&bparser.textBuf, 256, 0);

	vp->UserData = &bparser;
	
	parser->errorHandler = ErrorHandler;
	parser->startElementHandler = StartElement;
	parser->endElementHandler = EndElement;
	parser->charactersHandler = Characters;

	success = XMLParser_ParseValidateDTD(vp, parser, cstream, stdin, 0);
				
	for (i=0; i<bparser.books->length; i++) {
		b = XMLVector_Get(bparser.books, i);
		if (success)
			printf("id: %d\n"
			   "author: %s\n"
			   "title: %s\n"
			   "genre: %s\n"
			   "price: %f\n"
			   "publish_date: %s\n"
			   "description: %s\n\n",
			   b->id, b->author, b->title, b->genre, 
			   b->price, b->publish_date, b->description
			);
		SAFE_FREE(b->author);
		SAFE_FREE(b->title);
		SAFE_FREE(b->genre);
		SAFE_FREE(b->publish_date);
		SAFE_FREE(b->description);
	}
	
	XMLParser_FreeDTDValidator(vp);
	XMLParser_Free(parser);	
	XMLStringbuf_Free(&bparser.textBuf);
	XMLVector_Free(bparser.books);
	return 0;
}


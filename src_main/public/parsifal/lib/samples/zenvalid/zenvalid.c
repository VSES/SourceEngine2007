#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "libparsifal/parsifal.h"
#include "libparsifal/dtdvalid.h"

#define ASSERT_MEM_ABORT(p) \
  if (!(p)) { printf("Out of memory! Line: %d\n", __LINE__); return XML_ABORT; }
#define STORYP ((STORYPARSER*)v->UserData)

int StartElement(void *UserData, const XMLCH *uri, const XMLCH *localName, const XMLCH *qName, LPXMLVECTOR atts);
int EndElement(void *UserData, const XMLCH *uri, const XMLCH *localName, const XMLCH *qName);
int Characters(void *UserData, const XMLCH *Chars, int cbChars);
void ErrorHandler(LPXMLPARSER parser);
int cstream(BYTE *buf, int cBytes, int *cBytesActual, void *inputData);

typedef struct tagZENSTORY {
	char *title;
	char *author;
	char *body;
} ZENSTORY;

typedef struct tagSTORYPARSER {
	LPXMLPARSER parser;
	XMLSTRINGBUF textBuf;
	LPXMLVECTOR stories;
	ZENSTORY *curStory;
	char **curStr;
} STORYPARSER;

int StartElement(void *UserData, const XMLCH *uri, const XMLCH *localName, const XMLCH *qName, LPXMLVECTOR atts)
{	
	LPXMLDTDVALIDATOR v = (LPXMLDTDVALIDATOR)UserData;
	/* initialize new story: */	
	if (!strcmp(qName, "story")) {
		STORYP->curStory = XMLVector_Append(STORYP->stories, NULL);
		ASSERT_MEM_ABORT(STORYP->curStory);
		STORYP->curStory->title = (char*)NULL;
		STORYP->curStory->author = (char*)NULL;
		STORYP->curStory->body = (char*)NULL;
		STORYP->curStr = NULL;
	}
	else if (!strcmp(qName, "title")) 
		STORYP->curStr = &STORYP->curStory->title;
	else if (!strcmp(qName, "author"))
		STORYP->curStr = &STORYP->curStory->author;
	else if (!strcmp(qName, "body"))
		STORYP->curStr = &STORYP->curStory->body;
	return XML_OK;
}

int EndElement(void *UserData, const XMLCH *uri, const XMLCH *localName, const XMLCH *qName)
{	
	LPXMLDTDVALIDATOR v = (LPXMLDTDVALIDATOR)UserData;
	if (STORYP->curStr) {
		if (!strcmp(qName, "b"))
			return XML_OK; /* handles mixed content elements simply here */
			
		*STORYP->curStr = strdup(XMLStringbuf_ToString(&STORYP->textBuf));
		ASSERT_MEM_ABORT(*STORYP->curStr);
		ASSERT_MEM_ABORT(XMLStringbuf_SetLength(&STORYP->textBuf, 0));
		STORYP->curStr = NULL;
	}
	return XML_OK;
}

int Characters(void *UserData, const XMLCH *Chars, int cbChars)
{	
	LPXMLDTDVALIDATOR v = (LPXMLDTDVALIDATOR)UserData;
	ASSERT_MEM_ABORT(XMLStringbuf_Append(&STORYP->textBuf, (XMLCH*)Chars, cbChars));
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
	STORYPARSER sparser;
	LPXMLPARSER parser;	
	LPXMLDTDVALIDATOR vp;
	
	if (!XMLParser_Create(&parser)) {
		puts("Error creating parser!");
		return 1;
	}
	
	vp = XMLParser_CreateDTDValidator();
	if (!vp) {
		puts("Error creating DTDValidator in main()");
		return 1;
	}
	
	if (!XMLVector_Create(&sparser.stories, 6, sizeof(ZENSTORY))) {
		puts("Error creating stories vector in main()");
		return 1;
	}
	
	/* init Stringbuf: blockSize 256, no pre-allocation: */
	XMLStringbuf_Init(&sparser.textBuf, 256, 0);

	sparser.parser = parser;
	sparser.curStr = NULL;
	vp->UserData = &sparser;
	
	parser->errorHandler = ErrorHandler;
	parser->startElementHandler = StartElement;
	parser->endElementHandler = EndElement;
	parser->charactersHandler = Characters;

	if (XMLParser_ParseValidateDTD(vp, parser, cstream, stdin, 0)) {
		
		/* present the stories (we'll free the strings in the same loop) : */
		int i;
		ZENSTORY *story;
		for (i=0; i<sparser.stories->length; i++) {
			story = XMLVector_Get(sparser.stories, i);
			if (story) {
				if (story->title) {
					printf("title: %s\n", story->title);
					free(story->title);
				}
				if (story->author) {
					printf("author: %s\n", story->author);
					free(story->author);
				}
				if (story->body) {
					printf("story: %s\n",story->body);
					free(story->body);
				}
			}
		}
	}
	
	XMLParser_FreeDTDValidator(vp);
	XMLParser_Free(parser);	
	XMLStringbuf_Free(&sparser.textBuf);
	XMLVector_Free(sparser.stories);
	return 0;
}


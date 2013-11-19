#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "libparsifal/parsifal.h"
#include "zenstory.h"

int StartElement(void *UserData, const XMLCH *uri, const XMLCH *localName, const XMLCH *qName, LPXMLVECTOR atts)
{	
	STORYPARSER *parser = (STORYPARSER*)UserData;

	if (parser->inMixedContent) {	
		parser->inMixedContent++;
		/* could call mixed content legal tag check routine here e.g.
		if (!isvalidmixedcontenttag(qName, parser->state)) return sin(); */
		return XML_OK;
	}
	else {
		int *pstate = STACK_PEEK(parser->stateStack); 
		parser->state = (pstate) ? *pstate : NONE;
	}
	
	if (parser->state == NONE && !strcmp(qName, "stories")) {
		parser->state = STORIES; /* set initial state (root element) */
	}
	else if (parser->state == STORIES && !strcmp(qName, "story")) {
		/* initialize new story: */	
		parser->curStory = XMLVector_Append(parser->stories, NULL);
		ASSERT_MEM_ABORT(parser->curStory);
		parser->curStory->title = (char*)NULL;
		parser->curStory->author = (char*)NULL;
		parser->curStory->body = (char*)NULL;
		parser->state = STORY;
	}
	/* these would normally be done with a little more short circuiting: */
	else if (parser->state == STORY && !strcmp(qName, "title")) {
		parser->inContent++; /* STORY_TITLE tag allows normal content */
		parser->state = STORY_TITLE;
	}
	else if (parser->state == STORY && !strcmp(qName, "author")) {
		parser->inContent++; /* STORY_AUTHOR tag allows normal content */
		parser->state = STORY_AUTHOR;
	}
	else if (parser->state == STORY && !strcmp(qName, "body")) {	
		parser->inMixedContent++; /* STORY_BODY tag allows mixed content */
		parser->state = STORY_BODY;
	}
	else {
		printf("Unexpected tag: %s\nFile is not in zen story format!\n", qName);
		return XML_ABORT;
	}
	/* push the new state: */
	ASSERT_MEM_ABORT(STACK_PUSH(parser->stateStack, &parser->state));
	return XML_OK;
}

int EndElement(void *UserData, const XMLCH *uri, const XMLCH *localName, const XMLCH *qName)
{	
	STORYPARSER *parser = (STORYPARSER*)UserData;

	if (parser->inContent) 
		parser->inContent = 0;
	else if (parser->inMixedContent) {
		/* if this is last mixed content tag (the tag
		that put us in mixed content), we POP the state, 
		otherwise we just decrement the counter and return OK: */
		if ((--parser->inMixedContent) > 0) return XML_OK;		
	}

	if (STACK_POP(parser->stateStack, &parser->state)) {
		/* test the endTag here by popping and testing parser->state 
		(which is the state that is ending) */
		XMLCH *s = (XMLCH*)NULL;

		switch(parser->state) {
			case STORY_TITLE: /* you should check duplicate title etc. here */
				s = strdup(XMLStringbuf_ToString(&parser->textBuf));
				ASSERT_MEM_ABORT(s);
				parser->curStory->title = s; 
				break;
			case STORY_AUTHOR:
				s = strdup(XMLStringbuf_ToString(&parser->textBuf));
				ASSERT_MEM_ABORT(s);
				parser->curStory->author = s;
				break;
			case STORY_BODY:
				s = strdup(XMLStringbuf_ToString(&parser->textBuf));
				ASSERT_MEM_ABORT(s);
				parser->curStory->body = s;
				break;
		}
		if (s) {
			/* normalize buffer, note that XMLNormalizeBuf doesn't nul terminate the
			buffer: */
			int len = XMLNormalizeBuf(s, parser->textBuf.len);
			if (len < parser->textBuf.len) s[len] = '\0';
			/* we'll reuse Stringbuf just setting its length to 0: */
			ASSERT_MEM_ABORT(XMLStringbuf_SetLength(&parser->textBuf, 0));
		}
	}
	return XML_OK;
}

int Characters(void *UserData, const XMLCH *Chars, int cbChars)
{	
	STORYPARSER *parser = (STORYPARSER*)UserData;
	
	if (parser->inContent || parser->inMixedContent) {
		/* if either inContent or inMixedContent is set,
		   append data into Stringbuf: */
		ASSERT_MEM_ABORT(XMLStringbuf_Append(&parser->textBuf, 
			(XMLCH*)Chars, cbChars));
	}
	else {
		/* this tag cannot contain character data: */
		puts("Character data was not expected here!");
		return XML_ABORT;	
	}
	return XML_OK;
}

void ErrorHandler(LPXMLPARSER parser) 
{
	if (parser->ErrorCode != ERR_XMLP_ABORT)
		printf("Parsing error: %s\n", parser->ErrorString);
	printf("ErrorLine: %d ErrorColumn: %d\n", parser->ErrorLine, parser->ErrorColumn);
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

	if (!XMLParser_Create(&parser)) {
		puts("Error creating parser!");
		return 1;
	}
	
	if (!XMLVector_Create(&sparser.stateStack, 6, sizeof(int))) {
		puts("Error creating stateStack in main()");
		return 1;
	}
	
	if (!XMLVector_Create(&sparser.stories, 6, sizeof(ZENSTORY))) {
		puts("Error creating stories vector in main()");
		return 1;
	}
	
	/* init Stringbuf: blockSize 256, no pre-allocation: */
	XMLStringbuf_Init(&sparser.textBuf, 256, 0);

	sparser.parser = parser;
	sparser.inMixedContent = sparser.inContent = 0;
	parser->UserData = &sparser;
	
	parser->errorHandler = ErrorHandler;
	parser->startElementHandler = StartElement;
	parser->endElementHandler = EndElement;
	parser->charactersHandler = Characters;

	if (XMLParser_Parse(parser, cstream, stdin, 0)) {
		
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

	XMLParser_Free(parser);
	XMLStringbuf_Free(&sparser.textBuf);
	XMLVector_Free(sparser.stories);
	XMLVector_Free(sparser.stateStack);
	return 0;
}


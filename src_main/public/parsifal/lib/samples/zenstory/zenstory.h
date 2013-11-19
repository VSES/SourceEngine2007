int StartElement(void *UserData, const XMLCH *uri, const XMLCH *localName, const XMLCH *qName, LPXMLVECTOR atts);
int EndElement(void *UserData, const XMLCH *uri, const XMLCH *localName, const XMLCH *qName);
int Characters(void *UserData, const XMLCH *Chars, int cbChars);
void ErrorHandler(LPXMLPARSER parser);
int cstream(BYTE *buf, int cBytes, int *cBytesActual, void *inputData);

/* stack macros (from xmldef.h) */
#define STACK_PUSH(stack,item) (XMLVector_Append((stack), (item)))
#define STACK_PEEK(stack) (XMLVector_Get((stack),(stack)->length-1))
#define STACK_REMOVE(stack) (XMLVector_Remove((stack), (stack)->length-1))
#define STACK_POP(stack,item) \
( ((stack)->length) ? (memcpy((item), STACK_PEEK((stack)), (stack)->itemSize), \
STACK_REMOVE((stack)), (item)) : NULL)

#define ASSERT_MEM_ABORT(p) if (!(p)) { printf("Out of memory! Line: %d\n", __LINE__); return XML_ABORT; }

enum tagSTATES { NONE, STORIES, STORY, STORY_TITLE, STORY_AUTHOR, STORY_BODY } STATES;

typedef struct tagZENSTORY {
	char *title;
	char *author;
	char *body;
} ZENSTORY;

typedef struct tagSTORYPARSER {
	LPXMLPARSER parser;
	XMLSTRINGBUF textBuf;
	LPXMLVECTOR stateStack;
	int state;
	int inContent;
	int inMixedContent;
	LPXMLVECTOR stories;
	ZENSTORY *curStory;
} STORYPARSER;
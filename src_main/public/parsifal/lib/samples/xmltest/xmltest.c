#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "libparsifal/parsifal.h"
#include "libparsifal/dtdvalid.h"

#define MAKE_DIFFGRAM 1

#ifdef _MSC_VER
	#ifdef _DEBUG
		#include <crtdbg.h>
		#define _CRTDBG_MAP_ALLOC
	#endif
#endif

#define OUTDIR "pxpout/"

/* stack macros (from xmldef.h) */
#define STACK_PUSH(stack,item) (XMLVector_Append((stack), (item)))
#define STACK_PEEK(stack) (XMLVector_Get((stack),(stack)->length-1))
#define STACK_REMOVE(stack) (XMLVector_Remove((stack), (stack)->length-1))
#define STACK_POP(stack,item) \
( ((stack)->length) ? (memcpy((item), STACK_PEEK((stack)), (stack)->itemSize), \
STACK_REMOVE((stack)), (item)) : NULL)

#define EMPTY_COLS(num, pfile) { int i; for (i=0; i<(num); i++) fputs("<td>&nbsp;</td>", (pfile)); }

#ifndef MAX_PATH
#define MAX_PATH 256
#endif

enum tagSTATES { NONE, TESTSUITE, TESTCASES, TEST } STATES;

#define PFOUT (((XMLCONFPARSER*)UserData)->pfout)
#define PFERR stdout

typedef struct tagXMLCONFPARSER {
	LPXMLPARSER parser;
	LPXMLVECTOR stateStack;
	LPXMLDTDVALIDATOR v;
	int state;
	int inMixedContent;
	/* these are xmlconf specific: */
	LPXMLPARSER runParser;
	int testCount;
	int testSuccess;
	FILE *pfout, *pffailed;
} XMLCONFPARSER;

typedef struct tagRUNPARSERDATA {
	XMLCH systemID[MAX_PATH];
	XMLCH testbasedir[MAX_PATH];
	XMLCH intEnt[MAX_PATH];
} RUNPARSERDATA;

/* common routines: */
int cstream(BYTE *buf, int cBytes, int *cBytesActual, void *inputData);
int FreeInputData(void *UserData, LPXMLENTITY entity, LPBUFFEREDISTREAM reader);

/* TESTSUITE PARSER: */
void PrintEsc(FILE *fp, const XMLCH *str, int len);
int StartElement(void *UserData, const XMLCH *uri, const XMLCH *localName, const XMLCH *qName, LPXMLVECTOR atts);
int EndElement(void *UserData, const XMLCH *uri, const XMLCH *localName, const XMLCH *qName);
int Characters(void *UserData, const XMLCH *Chars, int cbChars);
void ErrorHandler(LPXMLPARSER parser);
int ResolveEntity(void *UserData, LPXMLENTITY entity, LPBUFFEREDISTREAM reader);

/* RUNTEST PARSER: */
int RunTest(XMLCONFPARSER *xcp, char *uri);
int RunTestResolveEntity(void *UserData, LPXMLENTITY entity, LPBUFFEREDISTREAM reader);
void RunTestErrorHandler(LPXMLPARSER parser);
void GetBaseDir(unsigned char *fullfile, unsigned char *targetdir);
int StartElementDetermineValidation(void *UserData, const XMLCH *uri, const XMLCH *localName, const XMLCH *qName, LPXMLVECTOR atts);
extern int fcompare(const char *fnam1, const char *fnam2);

int cstream(BYTE *buf, int cBytes, int *cBytesActual, void *inputData)
{
	*cBytesActual = fread(buf, 1, cBytes, (FILE*)inputData);	
	return (*cBytesActual < cBytes);
}

int FreeInputData(void *UserData, LPXMLENTITY entity, LPBUFFEREDISTREAM reader)
{
	fclose((FILE*)reader->inputData);
	return 0;
}

/* TESTSUITE PARSER BEGIN */

int StartElement(void *UserData, const XMLCH *uri, const XMLCH *localName, const XMLCH *qName, LPXMLVECTOR atts)
{	
	XMLCONFPARSER *xcp = (XMLCONFPARSER*)UserData;
	LPXMLRUNTIMEATT att;
	int *pstate = STACK_PEEK(xcp->stateStack);
	xcp->state = (pstate) ? *pstate : NONE;
	
	if (xcp->inMixedContent || xcp->state == TEST) { 
		/* + other tags that allow mixed content tested here */
		/* if we're in mixed content, we don't bother to use stack, just
		   incrementing (and decrementing in EndElement) the counter: */
		xcp->inMixedContent++;	
		/* could call mixed content legal tag check routine here e.g.
		if (!isvalidmixedcontent(state, qName)) return sin(); */
		fprintf(PFOUT, "<%s>", qName);
		return 0;
	}
	
	if (xcp->state == NONE && !strcmp(qName, "TESTSUITE")) { 
		
		if (att = XMLParser_GetNamedItem(xcp->parser, "PROFILE"))
			fprintf(PFOUT, "<h1><b>%s</b></h1><br><h3>Parsifal XML Parser %s</h3>", 
				att->value, XMLParser_GetVersionString());		
		xcp->state = TESTSUITE;
	}
	else if (xcp->state == TESTSUITE && !strcmp(qName, "TESTCASES")) {
		
		if (att = XMLParser_GetNamedItem(xcp->parser, "PROFILE")) {
			/* new testcase, spit out the profile header: */
			fprintf(PFOUT, "<br><br><h2>Testcase profile: <b>%s</b></h2><br>", att->value);
			fputs("<table cellspacing='0'>", PFOUT); /* open table for results */
		}
		xcp->state = TESTCASES;
	}
	else if (xcp->state == TESTCASES) {
		
		if (!strcmp(qName, "TEST")) {			
			if (att = XMLParser_GetNamedItem(xcp->parser, "URI")) {
				/* new test, run it: */
				if (!RunTest(xcp, att->value))
					fprintf(PFERR, "Fatal Error running test: %s\n", att->value);
			}
			xcp->state = TEST;
		}
		else if (!strcmp(qName, "TESTCASES")) { /* for some reason

			there's TESTCASES inside TESTCASES in ibm tests,
			so it must ust be handled here: */
			xcp->state = TESTCASES;
		}
	}
	else {
		fprintf(PFERR, "Unexpected tag: %s\n", qName);
		return XML_ABORT;
	}

	STACK_PUSH(xcp->stateStack, &xcp->state);
	return 0;
}

int EndElement(void *UserData, const XMLCH *uri, const XMLCH *localName, const XMLCH *qName)
{		
	XMLCONFPARSER *xcp = (XMLCONFPARSER*)UserData;
	if (xcp->inMixedContent) {
		xcp->inMixedContent--;
		fprintf(PFOUT, "</%s>", qName); /* EM or B tags */
	}
	else {
		if (STACK_POP(xcp->stateStack, &xcp->state)) {
			if (xcp->state == TEST) {
				/* close TEST description column and row: */
				fputs("</td>", PFOUT);
				EMPTY_COLS(3, PFOUT);
				fputs("</tr>", PFOUT);
			}
			else if (xcp->state == TESTCASES) {
				int *pstate = STACK_PEEK(xcp->stateStack);
				/* check is needed 'cos there can be TESTCASES inside TESTCASES */
				if (pstate && *pstate != TESTCASES) fputs("</table>", PFOUT);
			}
		}
	}	
	return 0;
}

void PrintEsc(FILE *fp, const XMLCH *str, int len)
{
	for (; len--; str++) {
		switch(*str) {
			case '&': fputs("&amp;", fp); break;
			case '\"': fputs("&quot;", fp); break;
			//case '\'': fprintf("&apos;", fp); break;
			case '<': fputs("&lt;", fp); break;
			case '>': fputs("&gt;", fp); break;
			case '\x9': fputs("&#9;", fp); break;
			case '\xA': fputs("&#10;", fp); break;
			case '\xD': fputs("&#13;", fp); break;
			default: fputc(*str, fp); break;
		}
	}
}

int Characters(void *UserData, const XMLCH *Chars, int cbChars)
{	
	XMLCONFPARSER *xcp = (XMLCONFPARSER*)UserData;
	if (xcp->state == TEST) 
		PrintEsc(PFOUT, Chars, cbChars);
	return 0;
}

void ErrorHandler(LPXMLPARSER parser) 
{	
	if (parser->ErrorCode != ERR_XMLP_ABORT) {
		XMLCH *sysID = XMLParser_GetSystemID(parser);

		if (sysID) fprintf(PFERR, "Parsing resource %s failed!\n", sysID);
		fprintf(PFERR, "Error: %s\nCode: %d\nLine: %d\nColumn: %d\n", 
			parser->ErrorString, parser->ErrorCode, 
			parser->ErrorLine, parser->ErrorColumn);
	}
}

int ResolveEntity(void *UserData, LPXMLENTITY entity, LPBUFFEREDISTREAM reader)
{
	FILE *f; 	
	if (!(f = fopen(entity->systemID, "rb"))) {
		fprintf(PFERR, "error opening file '%s'!\n", entity->systemID);
		return XML_ABORT;
	}	
	reader->inputData = f; 
	return 0;
}
/* TESTSUITE PARSER END */

/* RUNTEST PARSER BEGIN */
/* these are parser routines that run the test itself,
   we don't need many event handlers for it only
   resolveEntityHandler and related stuff */


#define TYPE_VALID 1
#define TYPE_INVALID 2
#define TYPE_OTHER 3

int RunTest(XMLCONFPARSER *xcp, char *uri) 
{
	LPXMLRUNTIMEATT att, tatt;
	XMLCH testuri[MAX_PATH];
	XMLCH xmlbase[MAX_PATH];
	XMLCH id[256];
	XMLCH *s;
	FILE *f;
	RUNPARSERDATA rdata;
	int result, expect;
	LPXMLPARSER parser = xcp->runParser;
	int type;

	if ((s = XMLParser_GetPrefixMapping(xcp->parser, "xml:base")))
		strcpy(xmlbase, s); /* we save current xmlbase
							   (although it shouldn't get modified 'cos
							   main parser isn't running during RunTest()) */
	else {
		/* rmt-e2e-18: 
		External entity containing start of entity declaration is 
		base URI for system identifier, so:  */
		XMLCH *sysID = XMLParser_GetSystemID(xcp->parser);
		if (!sysID) xmlbase[0] = '\0';
		else GetBaseDir(sysID, xmlbase);
	}
	
	strcpy(testuri, xmlbase);
	strcat(testuri, uri);

	puts(testuri);
	
	/* resolve basedir for external entities, DTD and for canonxml */
	GetBaseDir(testuri, rdata.testbasedir);
	
	tatt = XMLParser_GetNamedItem(xcp->parser, "TYPE");
	if (tatt) {
		/* "Nonvalidating parsers must also accept "invalid" testcases,
       but validating ones must reject them." */
		if (!strcmp(tatt->value, "valid")) type = TYPE_VALID;
		else if (!strcmp(tatt->value, "invalid")) type = TYPE_INVALID;
		else type = TYPE_OTHER; /* error, not-wf */
	}

	if ((f = fopen(testuri, "rb"))) {
#ifdef TEST_VALIDATING		
		xcp->v->UserData = &rdata;
		xcp->v->UserFlag = type;
		xcp->v->startElementHandlerFilter = StartElementDetermineValidation;
		result = XMLParser_ParseValidateDTD(xcp->v, parser, cstream, f, 0);
#else	
		parser->UserData = &rdata; 
		result = XMLParser_Parse(parser, cstream, f, 0);
#endif
		fclose(f);
	}
	else {
		fprintf(PFERR, "Error opening file %s\n", testuri);
		return 0;
	}

	xcp->testCount++;

	/* 1 row columns: ID, TYPE, PASS/FAIL, ERRORSTRING 
	   2 row columns: ENTITIES + OUTPUT in one col, 3 empty cols
	   3 row: test description, 3 empty cols
	*/	
	att = XMLParser_GetNamedItem(xcp->parser, "ID");
	strcpy(id, (att) ? att->value : "unknown");
	fputs((xcp->testCount % 2) ? "<tr bgcolor='#EEEEEE'>" : "<tr>", xcp->pfout);	
	fprintf(xcp->pfout, "<td><a href='%s'>%s</a></td>", testuri, id); 
	
	if (tatt) {
		if (type == TYPE_VALID) expect = 1; 
		else if (type == TYPE_INVALID)
#ifdef TEST_VALIDATING
			expect = 0;
#else
			expect = 1; 
#endif
		else expect = 0;
		fprintf(xcp->pfout, "<td>%s</td>", tatt->value);
		if (result == expect) xcp->testSuccess++;
	}
	else {
		EMPTY_COLS(1, xcp->pfout);
	}
	
	if (result != expect) fprintf(xcp->pffailed, "%s\n", id);

	fprintf(xcp->pfout, "<td><font color='%s'>", (result == expect) ? "#008000" : "#FF0000");
	if (result) 
		fprintf(xcp->pfout, "PASS</font></td><td>&nbsp;</td>");		
	else {
		fputs("FAIL</font></td><td>", xcp->pfout);
#ifdef TEST_VALIDATING
		if (parser->ErrorCode == ERR_XMLP_VALIDATION)
			PrintEsc(xcp->pfout, xcp->v->ErrorString, strlen(xcp->v->ErrorString));	
		else
#endif
		PrintEsc(xcp->pfout, parser->ErrorString, strlen(parser->ErrorString));	
		fputs("</td>", xcp->pfout);
	}
		
	fputs("</tr>", xcp->pfout);
	fputs((xcp->testCount % 2) ? "<tr bgcolor='#EEEEEE'>" : "<tr>", xcp->pfout);
	
	if ((att = XMLParser_GetNamedItem(xcp->parser, "ENTITIES")))
		fprintf(xcp->pfout, "<td><b>entities:</b> %s&nbsp;&nbsp;", att->value);
	else
		fputs("<td>", xcp->pfout);
	
	/* OUTPUT TEST */
	att = XMLParser_GetNamedItem(xcp->parser, "OUTPUT");
	if (att) {
		int compres;
		XMLCH cmd[2048];
		XMLCH outfile1[MAX_PATH], outfile2[MAX_PATH];
		
		strcpy(outfile1, xmlbase);
		strcat(outfile1, att->value);		
		sprintf(outfile2, "%s%s.xml", OUTDIR, id);
		sprintf(cmd, "canonxml %s %s %s", testuri, outfile2, rdata.testbasedir);
		system(cmd);

		compres = fcompare(outfile1, outfile2);
		
		fprintf(xcp->pfout, "<font color='%s'>OUTPUT:</font>", 
			(!compres) ? "#008000" : "#FF0000");
		if (compres == -1)
			fputs(" error", xcp->pfout);
		else {
#ifdef MAKE_DIFFGRAM
			if (compres) {
				XMLCH difffile[MAX_PATH];
				sprintf(difffile, "%s%s-diffgram.xml", OUTDIR, id);
				sprintf(cmd, "xmldiff %s %s %s", outfile1, outfile2, difffile);
				system(cmd);
				fprintf(xcp->pfout, " <a href='%s'>1</a> <a href='%s'>2</a> <a href='%s'>diff</a>", outfile1, outfile2, difffile);	
			}
			else
#endif
				fprintf(xcp->pfout, " <a href='%s'>1</a> <a href='%s'>2</a>", outfile1, outfile2);	
		}
	}
	else {
		fputs("</td>", xcp->pfout);
	}		
	/* OUTPUT TEST END */
	
	if (!parser->ErrorCode) {
		EMPTY_COLS(3, xcp->pfout);
	}
	else {
		EMPTY_COLS(2, xcp->pfout);
#ifdef TEST_VALIDATING
		if (parser->ErrorCode == ERR_XMLP_VALIDATION)
			fprintf(xcp->pfout, "<td>Line: %d Col: %d ", xcp->v->ErrorLine, xcp->v->ErrorColumn);
		else
#endif
		fprintf(xcp->pfout, "<td>Line: %d Col: %d ", parser->ErrorLine, parser->ErrorColumn);

		if (rdata.intEnt[0]) fprintf(xcp->pfout, "in entity: '%s' ", rdata.intEnt);
		if (rdata.systemID[0]) fprintf(xcp->pfout, "systemID: '%s'", rdata.systemID);
		fputs("</td>", xcp->pfout);
	}

	fputs("</tr>", xcp->pfout);
	/* open new table row for test description (reported via Characters): */
	fputs((xcp->testCount % 2) ? "<tr bgcolor='#EEEEEE'><td>" : "<tr><td>", xcp->pfout);
	return 1;
}

/* GetBaseDir gets path from <fullfile> string  into
<targetdir> string (which must be allocated for strlen(fullfile)+1)*/
void GetBaseDir(unsigned char *fullfile, unsigned char *targetdir)
{
	int slash = strlen(fullfile);
	while(slash && *(fullfile+slash) != '/') slash--;
	if (slash) {
		memcpy(targetdir, fullfile, slash+1);
		targetdir[slash+1] = '\0';
	}
	else {
		targetdir[0] = '\0';
	}
}

int RunTestResolveEntity(void *UserData, LPXMLENTITY entity, LPBUFFEREDISTREAM reader)
{
	char testuri[MAX_PATH];
	FILE *f;
#ifdef TEST_VALIDATING
	RUNPARSERDATA *rdata = (RUNPARSERDATA*)((LPXMLDTDVALIDATOR)UserData)->UserData;
#else
	RUNPARSERDATA *rdata = (RUNPARSERDATA*)UserData;
#endif
	strcpy(testuri, rdata->testbasedir); 
	strcat(testuri, entity->systemID);

	if (!(f = fopen(testuri, "rb"))) {
		fprintf(PFERR, "Error opening file %s\n", testuri);
		return XML_ABORT;
	}
	reader->inputData = f;
	return 0;
}

void RunTestErrorHandler(LPXMLPARSER parser) 
{	
	XMLCH *systemID;
	LPXMLENTITY curEnt;
#ifndef TEST_VALIDATING
	RUNPARSERDATA *rdata = (RUNPARSERDATA*)parser->UserData;	
#else
	RUNPARSERDATA *rdata = (RUNPARSERDATA*)((LPXMLDTDVALIDATOR)parser->UserData)->UserData;
	if (parser->ErrorCode == ERR_XMLP_VALIDATION &&
		_XMLParser_GetFlag(parser, XMLFLAG_VALIDATION_WARNINGS)) return;
#endif
	systemID = XMLParser_GetSystemID(parser);
	curEnt = XMLParser_GetCurrentEntity(parser);
	if (curEnt && !curEnt->systemID) strcpy(rdata->intEnt, curEnt->name);
	else rdata->intEnt[0] = '\0';
	if (systemID) strcpy(rdata->systemID, systemID);
	else rdata->systemID[0] = '\0';
}

int StartElementDetermineValidation(void *UserData, const XMLCH *uri, const XMLCH *localName, const XMLCH *qName, LPXMLVECTOR atts)
{	
	LPXMLDTDVALIDATOR v = (LPXMLDTDVALIDATOR)UserData;
	if (v->UserFlag == TYPE_VALID || v->UserFlag == TYPE_INVALID) {		
		_XMLParser_SetFlag(v->parser, XMLFLAG_VALIDATION_WARNINGS, 0);
	}
	else {
		_XMLParser_SetFlag(v->parser, XMLFLAG_VALIDATION_WARNINGS, 1);
	}
	v->startElementHandlerFilter = v->parser->startElementHandler = DTDValidate_StartElement;
	return DTDValidate_StartElement(UserData, uri, localName, qName, atts);
}

/* RUNTEST PARSER END */

int main(int argc, char* argv[])
{	
	LPXMLPARSER parser;		
	XMLCONFPARSER parserdata;
	FILE *f;
	char namePffailed[100];

	#ifdef _MSC_VER
		#ifdef _DEBUG
			int tmpFlag = _CrtSetDbgFlag( _CRTDBG_REPORT_FLAG );
			// Turn on leak-checking bit
			tmpFlag |= _CRTDBG_LEAK_CHECK_DF; /* dump leaks to output */
			// Set flag to the new value
			_CrtSetDbgFlag( tmpFlag ); 
		#endif
	#endif
	
	if (argc != 3) {
		fprintf(PFERR, "Give filenames e.g. C:\\XMLCONF\\XMLCONF.XML RESULTS.HTML\n");
		return 1;	
	}

	/* Initialize xmlconf.xml parser: */
	if (!XMLParser_Create(&parser)) {
		fprintf(PFERR, "Error creating main parser in main()\n");
		return 1;
	}
	
	parser->startElementHandler = StartElement;
	parser->endElementHandler = EndElement;	
	parser->charactersHandler = Characters;
	parser->errorHandler = ErrorHandler;
	parser->resolveEntityHandler = ResolveEntity;
	parser->externalEntityParsedHandler = FreeInputData;
	parser->UserData = &parserdata;
	
	parserdata.parser = parser;
	parserdata.inMixedContent = 0;
	parserdata.testCount = 0;
	parserdata.testSuccess = 0;

	/* Initialize runParser: */
	/* we could create new parser in RunTest every time we're running
	   new test, but for extra stress testing we're using the
	   same parser: */
	if (!XMLParser_Create(&parserdata.runParser)) {
		fprintf(PFERR, "Error creating runParser in main()\n");
		return 1;
	}
#ifdef TEST_VALIDATING
	parserdata.v = XMLParser_CreateDTDValidator();
	if (!parserdata.v) {
		fprintf(PFERR, "Error creating DTDvalidator in main()\n");
		return 1;
	}
#endif

	/* make parsifal to report undefined entities and not to use skippedEntity: */
	_XMLParser_SetFlag(parserdata.runParser, XMLFLAG_UNDEF_GENERAL_ENTITIES, 1);
	/* assign handlers: */
	parserdata.runParser->errorHandler = RunTestErrorHandler;
	parserdata.runParser->resolveEntityHandler = RunTestResolveEntity;
	parserdata.runParser->externalEntityParsedHandler = FreeInputData;

	if (!XMLVector_Create(&parserdata.stateStack, 6, sizeof(int))) {
		fprintf(PFERR, "Error creating stack in main()\n");
		return 1;
	}


	if (!(f = fopen(argv[1], "rb"))) {
		fprintf(PFERR, "Error opening input file %s\n", argv[1]);
		return 1;
	}
	
	if (!(parserdata.pfout = fopen(argv[2], "wb"))) {
		fprintf(PFERR, "Error opening output file %s\n", argv[2]);
		return 1;
	}
	
	sprintf(namePffailed, "%s-failed-%s.txt", argv[1], XMLParser_GetVersionString());
	if (!(parserdata.pffailed = fopen(namePffailed, "w"))) {
		fprintf(PFERR, "Error opening output file namePffailed\n", namePffailed);
		return 1;
	}
	
	fputs("<html><body>", parserdata.pfout);

	XMLParser_Parse(parser, cstream, f, NULL);
	
	fprintf(parserdata.pfout, "<br><br><b>%d</b> tests successful out of total <b>%d</b>.", 
		parserdata.testSuccess, parserdata.testCount);
	fputs("</body></html>", parserdata.pfout);
	
	fclose(f);
	fclose(parserdata.pfout);
	XMLVector_Free(parserdata.stateStack);
	XMLParser_Free(parserdata.runParser);
	XMLParser_Free(parser);
#ifdef TEST_VALIDATING
	XMLParser_FreeDTDValidator(parserdata.v);
#endif
	return 0;
}


/*===========================================================================
  parsifal.c
    This module contains the main XML parser logic
    see parsifal.h for copyright info
===========================================================================*/

#include <stdlib.h> /* malloc */
#include <string.h> /* memmove, strcmp */
#include <stdio.h> /* vsnprintf */
#include <stdarg.h>
#include <ctype.h>
#include "parsifal.h"
#include "nametab.h"
#include "xmldef.h"
#include "isrcmem.h" /* DTD_SUPPORT - defs only */
#include "xmlcfg.h"

#define PARSIFAL_VERSION "0.9.3"

#define CLEANUP_ATTS \
pEnd = (LPXMLRUNTIMEATT)_XMLVector_GetIterP(RT->atts, pAtt); \
for (; pAtt!=pEnd; pAtt++) { \
  XMLHTable_Remove(RT->namedAtts, pAtt->qname); \
  if (!_XMLParser_AttIsDefaulted(pAtt)) { \
    XMLStringbuf_Free(&pAtt->nameBuf); \
    XMLStringbuf_Free(&pAtt->valBuf); \
  } \
} \
_XMLVector_RemoveAll(RT->atts);

/* INIT_NSSCOPE is used in ParseAttributes.
   initializes new scope for pTag->Scope, copies namespaces from pTag->prevScope
   and sets RT->nsScope. After this we can refer to current namespace with
   RT->nsScope. pTag->Scope is only preserved to indicate that later this scope
   must be freed (at EndElement stage) */
#define INIT_NSSCOPE \
if (!pTag->Scope) { \
  pTag->Scope = XMLHTable_Create(pTag->Scope, 64); \
  if (!pTag->Scope) \
    return Er_(parser, ERR_XMLP_MEMORY_ALLOC); \
  if (pTag->prevScope != NULL) { \
    pTag->prevScope->userdata = pTag->Scope; \
    if ((XMLHTable_Enumerate(pTag->prevScope, CopyUriTableProc))) \
      return Er_(parser, ERR_XMLP_MEMORY_ALLOC); \
  } \
  RT->nsScope = pTag->Scope; \
}

static int ParseContent(LPXMLPARSER parser);
static int ParseComment(LPXMLPARSER parser, int skip);
static int ParsePI(LPXMLPARSER parser, int skip);
static int ParseCData(LPXMLPARSER parser);
static int ParseStartTag(LPXMLPARSER parser);
static int ParseEndTag(LPXMLPARSER parser);
static int ParseAttributes(LPXMLPARSER parser, LPXMLRUNTIMETAG pTag, 
						   int *isEmpty, LPXMLVECTOR DeclAtts);
static int ParseXmlDecl(LPXMLPARSER parser, int skip, LPXMLSTRINGBUF sbuf);
static int ParseTagNS(LPXMLPARSER parser, LPXMLRUNTIMETAG ptag, int namelen, int pos);
static int DetectEncoding(LPXMLPARSER parser, int skip);
static int TryXmlDecl(LPXMLPARSER parser, int charBytes, int skip);
static int SetEncoding(LPXMLPARSER parser, XMLCH *encodingName);
static void SetReaderFatal(LPXMLPARSER parser, int errval);
static const XMLCH *GetErrorString(XMLERRCODE code);
static int Er_(LPXMLPARSER parser, XMLERRCODE code, ...);
static int ErP_(LPXMLPARSER parser, XMLERRCODE code, int posOffset);
static void ParseInput(LPXMLPARSER parser);
static int DestroyUriTableProc(char *key, void *data, void *userdata);
static int CopyUriTableProc(char *key, void *data, void *userdata);
static XMLCH *prepUri(XMLCH *uri, int declHere);
static int ParseNameTok(LPXMLPARSER parser, int *len, 
						int *prefixLen, const XMLCH *endChars, int *cEndChars);
static XMLCH *ParseString(LPXMLPARSER parser, LPXMLSTRINGBUF sbuf, 
	LPXMLRUNTIMEATT pAtt, int Normalize, int entHandling);
static XMLCH *ReadCh(LPXMLPARSER parser, int *chSize);
static int SkipWS(LPXMLPARSER parser);
static int RequireCh(LPXMLPARSER parser, XMLCH ch, int allowWS);
static int TrieRaw(LPXMLPARSER parser, const struct trie *trie);
static int ParseDoctypeDecl(LPXMLPARSER parser);
static int ParseExternalID(LPXMLPARSER parser, LPXMLSTRINGBUF sbuf,
						   int *iPublicID, int *iSystemID, int allowPubidLit);
static LPXMLVECTOR GetDefAtts(LPXMLPARSER parser, XMLCH *e);

#ifdef DTD_SUPPORT
static int TryTok(LPXMLPARSER parser, XMLCH const *tok, int requireWS);
static int TrieTok(LPXMLPARSER parser, const struct trie *trie, int requireWS);
static int Require(LPXMLPARSER parser, XMLCH const *tok, int requireWS);
static int ParseDTD(LPXMLPARSER parser, int isExternal);
static int ParseContentDTD(LPXMLPARSER parser, int isExternal);
static void ParseEntityContent(LPXMLPARSER parser, LPXMLENTITY e);
static void ParseIntEntityAtt(LPXMLPARSER parser, LPXMLENTITY e, LPXMLRUNTIMEATT pAtt);
static int ParseEntityDecl(LPXMLPARSER parser);
static int GetEntityDecl(LPXMLPARSER parser, LPXMLENTITY e, LPXMLSTRINGBUF sbuf);
static int ResolveExternalDTD(LPXMLPARSER parser, LPXMLENTITY e);
static int ParseUserSubset(LPXMLPARSER parser);
static int MemInputsrc(BYTE *buf, int cBytes, int *cBytesActual, void *inputData);
static int GetAttlistDecl(LPXMLPARSER parser, LPXMLVECTOR **newAtts, LPXMLVECTOR *en);
static int ParseAttlistDecl(LPXMLPARSER parser);
static int DestroyDeclAttTableProc(char *key, void *data, void *userdata);
static int ParseElementDecl(LPXMLPARSER parser);
static int ParseChildren(LPXMLPARSER parser, XMLCP *cp);
static int ParseCp(LPXMLPARSER parser, XMLCP *cp);
static int ParseMixed(LPXMLPARSER parser, XMLCP *cp);
static int ParseNotationDecl(LPXMLPARSER parser);
static XMLCH *ReadPERefStart(LPXMLPARSER parser, int *chSize);
static XMLCH *ReadPERefEnd(LPXMLPARSER parser, int *chSize);
static XMLCH *RestoreReaderPeRef(LPXMLPARSER parser, int *chSize);
#endif

static const XMLCH *GetErrorString(XMLERRCODE code)
{
	static const XMLCH * const ParserErrMsg[] = {
	0,
	"Out of memory",
	"Fatal error (code: %d)",
	"Invalid token",
	"Invalid name",
	"End tag '%s' was not expected here",
	"Reference to undefined entity '%s'",
	"Whitespace not allowed here",
	"Whitespace required",
	"Tag '%s' was not closed",
	"Expected '%s', found '%s'",
	"Expected '%s'",
	"Only one top level element is allowed",
	"Invalid at the top level of the document",
	"Undefined namespace prefix '%s'",
	"Duplicate attribute '%s'",
	"Encoding error",
	"Unsupported encoding '%s'",
	"Invalid %s declaration",
	"Invalid value for attribute '%s'",
	"Parsing aborted",
	"Illegal character",
	"Recursive reference in entity '%s'",
	"I/O error",
	"Switch from encoding '%s' to '%s' is not supported"
	};
	return (ParserErrMsg[code]);
}

#include "optcfg.h"

/*===========================================================================
  FUNCTION
    Err
  PARAMS
    parser      this
    code        error code
    ...         va_list for error string
  RETURNS
    0
  NOTES
    Sets ErrorLine, ErrorString etc. only if errorHandler is specified.
===========================================================================*/
static int Er_(LPXMLPARSER parser, XMLERRCODE code, ...)
{	
	if (parser->ErrorCode == ERR_XMLP_VALIDATION) {
		if (HANDLER(error))	HANDLER(error)(parser);
		return 0;
	}
	parser->ErrorCode = code;
	if (HANDLER(error)) {		
		va_list aptr;
		va_start(aptr, code);
		
		if (vsnprintf((char*)parser->ErrorString, 128, GetErrorString(code), aptr) < 0)
			parser->ErrorString[127] = '\0';
		va_end(aptr);		
		parser->ErrorColumn = XMLParser_GetCurrentColumn(parser);	
		parser->ErrorLine = XMLParser_GetCurrentLine(parser);		
		HANDLER(error)(parser);	
	}
	return 0;
}

/*===========================================================================
  FUNCTION
    ErrP
  PARAMS
    parser      this
    code        error code
    posOffset   bytes to go back in the PREADER->buf (usually token/chSize)
  NOTES
    PREADERDATA->col can go below 0 when WS_NOT_ALLOWED and position
    contains LF/CR, we cannot go to previous line cos PREADERDATA->col
    for prev line isn't known.
===========================================================================*/
static int ErP_(LPXMLPARSER parser, XMLERRCODE code, int posOffset)
{	
	DPOS(posOffset);
	if(PREADERDATA->col < 0) PREADERDATA->col = 0;
	return Er_(parser, code);
}

/*===========================================================================
  FUNCTION
    SetReaderFatal
  DESC
    for EINPUT macro
===========================================================================*/
static void SetReaderFatal(LPXMLPARSER parser, int code)
{	
	switch(code) {
		case BIS_ERR_MEMALLOC:
			Er_(parser, ERR_XMLP_MEMORY_ALLOC);
		break;
		case BIS_ERR_ENCODING:
			Er_(parser, ERR_XMLP_ENCODING);
		break;
		case BIS_ERR_INPUT:
			Er_(parser, ERR_XMLP_IO);
		break;
		default: Er_(parser, ERR_XMLP_READER_FATAL, code);
	}
}

/*===========================================================================
  FUNCTION
    ParseCharRef
  DESC
    Parses character reference
  PARAMS
    parser      this
    ref         out buffer for char ref (max 4 bytes UTF-8 char)
  RETURNS
    Success: 1
    Failure: 0 (Error is set)
===========================================================================*/
static int ParseCharRef(LPXMLPARSER parser, XMLCH *ref)
{
	int chSize, base, n, i = 0;
	UINT32 code = 0;
	XMLCH *c;

	if (!(c = ReadCh(parser, &chSize))) goto ExitErr;
	
	if (*c == 'x')
		base = 16;
	else {
		DPOS(chSize);
		base = 10;
	}
	
	while ((c = ReadCh(parser, &chSize))) {
		if (chSize != 1) goto ExitErr;
		
		if (*c == ';') {
			if (!i) goto ExitErr;
			break;
		}
		else if((*c >= '0' && *c <= '9') ||
			(base == 16 && ((*c >= 'A' && *c <= 'F') ||
				(*c >= 'a' && *c <= 'f'))))
				i++;
		else goto ExitErr;		
	}

	if (!c) {
		if (!parser->ErrorCode) Er_(parser, ERR_XMLP_EXPECTED_TOKEN, ";");
		return 0;
	}
	c = PREADER->buf+PREADER->pos-(i + 1);

	while(i--) {
		n = *c++;
		if(n >= '0' && n <= '9')
			code = code * base + (n - '0');
		else if(n >= 'A' && n <= 'F')
			code = code * base + 10 + (n - 'A');
		else
			code = code * base + 10 + (n - 'a');
	}
	
	if (code < 0x80) { /* < UTF_BYTE1 max */
		if (ISILLBYTE(code))
			return ErP_(parser, ERR_XMLP_ILLEGAL_CHAR, 0);
		ref[0] = (char)code;
		return 1;
	}
	else if (code < 0x800) { /* < UTF_BYTE2 max */
		ref[0] = (char)((code >> 6) | UTF8_2BYTES);
		ref[1] = (char)((code & 0x3f) | 0x80);
		return 2;
	}
	else if (code < 0x10000) { /* < UTF_BYTE3 max */
		ref[0] = (char)((code >> 12) | UTF8_3BYTES);
		ref[1] = (char)(((code >> 6) & 0x3f) | 0x80);
		ref[2] = (char)((code & 0x3f) | 0x80);
		if (UTF8_ISILL3(ref))
			return ErP_(parser, ERR_XMLP_ILLEGAL_CHAR, 0);
		return 3;
	}
	else if (code < 0x110000) { /* < UTF_BYTE4 max */
		ref[0] = (char)((code >> 18) | UTF8_4BYTES);
		ref[1] = (char)(((code >> 12) & 0x3f) | 0x80);
		ref[2] = (char)(((code >> 6) & 0x3f) | 0x80);
		ref[3] = (char)((code & 0x3f) | 0x80);
		if (UTF8_ISILL4(ref))
			return ErP_(parser, ERR_XMLP_ILLEGAL_CHAR, 0);
		return 4;
	}
	
	return ErP_(parser, ERR_XMLP_ILLEGAL_CHAR, 0);
ExitErr:
if (!parser->ErrorCode)
	ErP_(parser, ERR_XMLP_INVALID_TOKEN, (chSize) ? chSize : 1);
return 0;
}

#define FLUSHCHARS(b,isWS) \
if ((b)->len) { \
if (isWS) { \
  if (RT->tagstack->length && HANDLER(ignorableWhitespace) && HANDLER(ignorableWhitespace) \
    (parser->UserData, (b)->str, (b)->len) == XML_ABORT) \
      return Er_(parser, ERR_XMLP_ABORT); \
} \
else { \
  if (HANDLER(characters) && HANDLER(characters) \
    (parser->UserData, (b)->str, (b)->len) == XML_ABORT) \
      return Er_(parser, ERR_XMLP_ABORT); \
} \
if (!XMLStringbuf_SetLength((b), 0)) \
  return Er_(parser, ERR_XMLP_MEMORY_ALLOC); \
}

/*===========================================================================
  FUNCTION
    ParseEntityRef
  DESC
    Parses and expands entity references
  PARAMS
    parser      this
    sbuf        output string buffer
    pAtt        indicates that current context is attribute value buffer
    Context     1: expand only char refs
  RETURNS
    Success: 1
    Failure: 0 (Error is set)
===========================================================================*/
static int ParseEntityRef(LPXMLPARSER parser, LPXMLSTRINGBUF sbuf,
						  LPXMLRUNTIMEATT pAtt, int Context, int *isWS)
{	
	int chSize, namepos, i;
	XMLCH *c;

	if (!(c = ReadCh(parser, &chSize))) {
		if (!parser->ErrorCode)	
			ErP_(parser, ERR_XMLP_INVALID_TOKEN, 1);
		return 0;
	}

	if (*c == '#') {
		XMLCH ref[4];
		namepos = PREADER->pos-2;
		if (!(i = ParseCharRef(parser, ref))) return 0;
		if (Context == 1 && i == 1 &&
			(ref[0] == (XMLCH)0xA || ref[0] == (XMLCH)0xD)) {
			/* expanding values in internal !ENTITY decl must preserve LF and CR
			note that &#13; can be something like &#00013; */
			if (!(XMLStringbuf_Append(sbuf, PREADER->buf+namepos,
				PREADER->pos-namepos)))
				return Er_(parser, ERR_XMLP_MEMORY_ALLOC);
		}
		else {
			if (isWS && *isWS && (i!=1 || !isspace(ref[0]))) *isWS = 0;
			if (!(XMLStringbuf_Append(sbuf, ref, i)))
				return Er_(parser, ERR_XMLP_MEMORY_ALLOC);
		}
	}
	else {
		int namelen, endChars;		
		DPOS(chSize);
		if (Context != 1 && ((i = TrieRaw(parser, TRstdEnt)) != -1)) { 
			/* expand predefined entities: */
			switch(i) {
				case 1: c=XMLStringbuf_AppendCh(sbuf, '>'); break;
				case 2: c=XMLStringbuf_AppendCh(sbuf, '<'); break;
				case 3: c=XMLStringbuf_AppendCh(sbuf, '&'); break;
				case 4: c=XMLStringbuf_AppendCh(sbuf, '\''); break;
				case 5: c=XMLStringbuf_AppendCh(sbuf, '\"'); break;
				default: return 0; /* error -2 */
			}
			if (isWS && *isWS) *isWS = 0;
			return (c) ? 1 : Er_(parser, ERR_XMLP_MEMORY_ALLOC);
		}

		endChars = 1;
		namepos = ParseNameTok(parser, &namelen, (int*)NULL, ";", &endChars);
		if (namepos == -1) return 0;
		if (!endChars)
			return ErP_(parser, ERR_XMLP_WS_NOT_ALLOWED, 1);
		
		if (Context == 1) {
			if (!(XMLStringbuf_Append(sbuf, PREADER->buf+(namepos-1), namelen+2)))
				return Er_(parser, ERR_XMLP_MEMORY_ALLOC);
			return 1;
		}
		
		/* we can use the actual buffer since name cannot end to CR or LF or
			to char that is significant to position functions */
		c = BUFTOSTR(PREADER->buf, namepos, namelen+namepos);

#ifdef DTD_SUPPORT
		if (PFLAG(XMLFLAG_PRESERVE_GENERAL_ENTITIES)) {
#endif		
			c = PREADER->buf+(namepos-1);
			c[namelen+1] = ';';
			if (!(XMLStringbuf_Append(sbuf, c, namelen+2)))
				return Er_(parser, ERR_XMLP_MEMORY_ALLOC);
			return 1;
#ifdef DTD_SUPPORT
		}
		else if ((i = (int)XMLHTable_Lookup(RT->entitiesTable, c))) {

			LPXMLENTITY e = XMLVector_Get(RT->entities, i-1);

			if (e->open)
				return Er_(parser, ERR_XMLP_RECURSIVE_ENTITY_REF, c);
			
			if (pAtt)
				ParseIntEntityAtt(parser, e, pAtt);
			else {
				FLUSHCHARS(sbuf, *isWS);
				if (e->type == XML_ENTITY_EXT_GEN &&
					(!PFLAG(XMLFLAG_EXTERNAL_GENERAL_ENTITIES))) {
					if (HANDLER(skippedEntity)) { /* report skippedEntity: */
						if (HANDLER(skippedEntity)(parser->UserData, c)==XML_ABORT)
							return Er_(parser, ERR_XMLP_ABORT);
					}
				}
				else {
					ParseEntityContent(parser, e);
				}
			}						
		}
		else { /* XMLHTable_Lookup didn't find entity: */
			if (PFLAG(XMLFLAG_UNDEF_GENERAL_ENTITIES))
				return Er_(parser, ERR_XMLP_UNDEF_ENTITY, c);

			if (!pAtt) {
				FLUSHCHARS(sbuf, *isWS);
				if (HANDLER(skippedEntity)) { /* report skippedEntity: */
					if (HANDLER(skippedEntity)(parser->UserData, c)==XML_ABORT)
						return Er_(parser, ERR_XMLP_ABORT);
				}
			}
		}
#endif /* DTD_SUPPORT */
	}
	return (parser->ErrorCode == 0);
}

static int ResetLine(LPXMLPARSER parser)
{	
	if (PREADER->pos > 4096) {
		if (EINPUT(BufferedIStream_ResetBuf(PREADER, PREADER->pos))) return 0;
		PREADERDATA->col=0;
	}
	else {
		int pos = PREADER->pos - PREADERDATA->col;	
		if (pos > 0) {
			if (EINPUT(BufferedIStream_ResetBuf(PREADER, pos))) return 0;
			PREADER->pos=PREADERDATA->col;
		}
	}
	return 1;
}

/*===========================================================================
  FUNCTION
    ParseContent
  DESC
    Parses reader buffer "outside tags"
  RETURNS
    0 failure
    1 success
===========================================================================*/
static int ParseContent(LPXMLPARSER parser)
{
	int chSize, isWS=1;
	XMLCH *c;

	while((c = ReadCh_Opt(parser, &chSize))) {		
		if (chSize == 1) {						
			if (*c == '<') break;
			
			if (*c == '&') {
				if (!RT->tagstack->length) 					
					return ErP_(parser,  ERR_XMLP_INVALID_AT_TOP, 1);
				if (!ParseEntityRef(parser, &RT->charsBuf,
					(LPXMLRUNTIMEATT)NULL,  2, &isWS)) return 0;
				continue;
			}
			else if (*c == '>' && RT->charsBuf.len > 1) {
				if (*(c-2) == ']' && *(c-1) == ']')
					return ErP_(parser, ERR_XMLP_INVALID_TOKEN, 3);
			}
			
			if (isWS && !isspace(*c)) {
				if (!RT->tagstack->length) 					
					return ErP_(parser,  ERR_XMLP_INVALID_AT_TOP, 1);
				isWS = 0;
			}
		}
		else if (isWS) {
			if (!RT->tagstack->length)				
				return ErP_(parser,  ERR_XMLP_INVALID_AT_TOP, chSize);
			isWS = 0;
		}
		if (!(XMLStringbuf_Append_Opt(&RT->charsBuf, c, chSize)))		
			return Er_(parser, ERR_XMLP_MEMORY_ALLOC);
	}
	
	if (parser->ErrorCode) return 0;
	
	FLUSHCHARS(&RT->charsBuf, isWS);
	
	if (c==NULL) return 0;
	if (PREADER->pos > 1000 
		&& !ResetLine(parser)) return 0;
	return 1;
	/* NOTE: we can exit returning 0 at any time, RT->charsBuf is emptied
	in error condition in _Parse. */
}

/*===========================================================================
  FUNCTION
    ParseEndTag
  DESC
    Parses end tag and compares it to tag at the top of tagstack.
  RETURNS
    0 failure
    1 success
===========================================================================*/
static int ParseEndTag(LPXMLPARSER parser)
{
	int namepos, namelen, endChars = 1;
	LPXMLRUNTIMETAG pTag;
	XMLCH *s;
	
	namepos = ParseNameTok(parser, &namelen, (int*)NULL, ">", &endChars);
	if (namepos == -1) return 0;
	
	if (!endChars) {	
		if (!namelen)			
			return ErP_(parser, ERR_XMLP_WS_NOT_ALLOWED, 1);
		if (!(RequireCh(parser, '>', 1))) return 0;
	}
	
	/* we get significant performance boost using BUFTOSTR instead of
	  xmlMemdup (no mallocs or frees) but have to be careful with these:
	  1) no bounds checking
	  2) buffer must have room for \0
	  3) no _Read operations when using BUFTOSTR buffer */	
	s = BUFTOSTR(PREADER->buf, namepos, namelen+namepos);

	if ((RT->tagstack->length-1) < PREADERDATA->stackLevel)
		return Er_(parser, ERR_XMLP_INVALID_END_TAG, s);

	if ((pTag = STACK_PEEK(RT->tagstack)))
	{
		if (!strcmp(pTag->qname, s)) {		
			if (HANDLER(endElement)) {
				if (HANDLER(endElement)(parser->UserData, pTag->uri,
					pTag->localName, pTag->qname) == XML_ABORT) {
						Er_(parser, ERR_XMLP_ABORT);
				}
			}
		}
		else {
			Er_(parser, ERR_XMLP_EXPECTED_FOUND, pTag->qname, s);
		}

		XMLStringbuf_Free(&pTag->nameBuf); /* frees localName too */
		if (pTag->Scope!=NULL)
			XMLHTable_Destroy(pTag->Scope, DestroyUriTableProc, 1);
		RT->nsScope = pTag->prevScope;
		STACK_REMOVE(RT->tagstack);
	}
	else {
		return Er_(parser, ERR_XMLP_EXPECTED_TOKEN, s);
	}

	return (parser->ErrorCode == 0);
}

/*===========================================================================
  FUNCTION
    ParseString
  DESC
    Parses string literal
  PARAMS
    parser      this
    sbuf        output string buffer
    pAtt        indicates that current context is attribute value buffer
    Normalize   0: none 1: normalize to CDATA 2: normalize all (trim start
                and end and turn all other whitespae to single space)
    entHandling 0: expand none, 1: expand char refs 2: expand all
  RETURNS
    Success: String
    Failure: NULL string (Error is set)
===========================================================================*/
static XMLCH *ParseString(LPXMLPARSER parser, LPXMLSTRINGBUF sbuf,
						  LPXMLRUNTIMEATT pAtt, int Normalize, int entHandling)
{
	int chSize;
	int startPos = sbuf->len;
	XMLCH *c, quote;
	LPBUFFEREDISTREAM rTok = NULL;
	
	if (SkipWS(parser) == -1)
		return (XMLCH*)NULL;

	quote = CURCHAR;	

	if (ISQUOTE(quote)) {
		IPOS(1);
		if (RT->dtd) {
			if (RT->dtd->inLiteral)
				/* inLiteral is already set, it means
				that PE expansion must be turned off */
				RT->dtd->expandPEs = 0;
			else
				RT->dtd->inLiteral = 1;
			rTok = PREADER;
		}

		while((c = ReadCh_Opt(parser, &chSize))) {
			if (chSize == 1) {
				if (isspace(*c)) {
					if (Normalize == 2) {
						if (sbuf->len == startPos) 
							continue; /* trim start */
						if (!(XMLStringbuf_AppendCh(sbuf, ' '))) {
							Er_(parser, ERR_XMLP_MEMORY_ALLOC);
							return (XMLCH*)NULL;
						}
						while((c = ReadCh(parser, &chSize))) { /* normalize */
							if (chSize != 1) break;
							if (!isspace(*c)) break;	
						}
						if (!c) break;
						if (chSize != 1) {
							if (!(XMLStringbuf_Append(sbuf, c, chSize))) {
								Er_(parser, ERR_XMLP_MEMORY_ALLOC);
								return (XMLCH*)NULL;
							}
							continue;
						}
					}
					else {
						if (Normalize) c = XMLStringbuf_AppendCh(sbuf, ' ');
						else c = XMLStringbuf_AppendCh(sbuf, *c);
						if (!c) { Er_(parser, ERR_XMLP_MEMORY_ALLOC);
							return (XMLCH*)NULL;
						}
						continue;
					}
				}

				if (*c == quote && (!RT->dtd || PREADER == rTok)) {			
					if (RT->dtd) {
						RT->dtd->inLiteral = 0;
						if (!RT->dtd->expandPEs) RT->dtd->expandPEs = 1;			
					}
					if (Normalize == 2 && sbuf->len && sbuf->str[sbuf->len-1] == ' ')
						sbuf->len--; /* trim end */
					if (!(XMLStringbuf_ToString(sbuf))) {
						Er_(parser, ERR_XMLP_MEMORY_ALLOC);
						return (XMLCH*)NULL;
					}
					return sbuf->str;
				}
				else if (entHandling && *c == '&') {
					if (!ParseEntityRef(parser, sbuf, pAtt, entHandling, NULL))
						return (XMLCH*)NULL;
					continue;
				}
				else if (*c == '<' && pAtt) {		
					ErP_(parser, ERR_XMLP_INVALID_TOKEN, 1);
					return (XMLCH*)NULL;
				}
			}
			if (!(XMLStringbuf_Append_Opt(sbuf, c, chSize))) {
				Er_(parser, ERR_XMLP_MEMORY_ALLOC);
				return (XMLCH*)NULL;
			}
		}		
	}
	if (!parser->ErrorCode)
		Er_(parser, ERR_XMLP_EXPECTED_TOKEN, "String");
	return (XMLCH*)NULL;
}

static LPBISREADERDATA InitEntityReader(LPXMLPARSER parser, LPBUFFEREDISTREAM r, 
							int blockSize, LPBISREADERDATA rd, int stackLevel, 
							LPXMLENTITY e, int iEnt)
{	
	if (!rd) {
		if (!(rd = (LPBISREADERDATA)malloc(sizeof(*rd)))) {
			Er_(parser, ERR_XMLP_MEMORY_ALLOC);
			return NULL;
		}	
	}
	BufferedIStream_Init(r, blockSize);
	if (e) {
		e->open = 1; /* set open flag for recursion tracking */
		if (e->systemID) { 
			/* see GetSystemID etc. */
			RT->publicID = e->publicID;
			RT->systemID = e->systemID;
		}
	}
	else {
		RT->publicID = RT->systemID = (XMLCH*)NULL;
	}

	rd->line = rd->col = rd->lEndian = 0;
	rd->EncodingName[0] = '\0';
	rd->parser = parser;
	rd->stackLevel = stackLevel;
	rd->context = 0; 
	r->userdata = rd;
	rd->iCurPE = iEnt; /* PEs need index to RT->entities vector, this also flags
		that this entity can be relocated while parsing DTD and thus must be
		retrieved via XMLVector_Get (see GetCurrentEntity()) */
	rd->curEnt = e;
	PREADER = r;
	return rd;
}

/*===========================================================================
  FUNCTION
    ReadCh
  DESC
    returns UTF-8 char from input
  PARAMS
    parser      this
    chSize      out: length of char
  RETURNS
    Success: XMLCH* to character
    Failure: NULL string (NOTE: Error might be set but not necessarily)
  NOTES
    ReadCh handles CR normalization + line and col stuff
    + checking of invalid UTF-8 sequences (see xmldef.h for more info)
    ReadCh is used as follows: if ReadCh returns NULL parser->errorCode
    is tested, if it's not set, set context specific error e.g. missing >
===========================================================================*/
static XMLCH *ReadCh(LPXMLPARSER parser, int *chSize)
{	
	XMLCH *c;
	int ret;

	if (!PREADER->buf || PREADER->pos >= PREADER->bytesavail) {
		ret = PEEKINPUT((const BYTE*)NULL, 1);
		if (EINPUT(ret) || ret) {
			#ifdef DTD_SUPPORT
			if (RT->dtd && !parser->ErrorCode && 
				RT->dtd->peStack->length &&
				((*((LPBUFFEREDISTREAM*)STACK_PEEK(RT->dtd->peStack)))) == PREADER)
				return ReadPERefEnd(parser, chSize);
			else {
			#endif
				*chSize = 0;
				return (XMLCH*)NULL;
			#ifdef DTD_SUPPORT
			}
			#endif
		}
	}
	c = PREADER->buf+PREADER->pos;
	UTF8LEN(c,*chSize);

	if (*chSize == 1) {
		if (ISILLBYTE(*c)) {
			*chSize = 0;
			ErP_(parser, ERR_XMLP_ILLEGAL_CHAR, 1);
			return (XMLCH*)NULL;
		}

		PREADER->pos++;
		if (*c == 0xD) {	
			PREADERDATA->line++;
			PREADERDATA->col=0;
			PREADER->buf[PREADER->pos-1] = 0xA;
			if (PREADER->pos >= PREADER->bytesavail) {
				ret = PEEKINPUT((const BYTE*)NULL, 1);
				if (EINPUT(ret)) {
					*chSize = 0;
					return (XMLCH*)NULL;
				}
				c = PREADER->buf+(PREADER->pos-1);
				if (ret) return(c);
			}
			if (CURCHAR == 0xA) PREADER->pos++;
		}
		else if (*c == 0xA) {
			PREADERDATA->line++;
			PREADERDATA->col=0;
		}
		#ifdef DTD_SUPPORT
		else if (*c == '%' && RT->dtd && RT->dtd->expandPEs) {
			PREADERDATA->col++;		
			c = ReadPERefStart(parser, chSize);
		}
		#endif
		else PREADERDATA->col++;
	}
	else {
		if (*chSize == 3 && UTF8_ISILL3(c)) {
			*chSize = 0;
			ErP_(parser, ERR_XMLP_ILLEGAL_CHAR, 0);
			return (XMLCH*)NULL;
		}
		else if (*chSize == 4 && UTF8_ISILL4(c)) {
			*chSize = 0;
			ErP_(parser, ERR_XMLP_ILLEGAL_CHAR, 0);
			return (XMLCH*)NULL;
		}
		PREADER->pos += *chSize;
		PREADERDATA->col += *chSize;
	}
	return(c);		
}

static int TrieRaw(LPXMLPARSER parser, const struct trie *trie)
{
	int found = -1;
	int pos = PREADER->pos;
	
	while(1) {
		if (!PREADER->buf || pos >= PREADER->bytesavail) {
			int o = pos-PREADER->pos;
			if ((o = BufferedIStream_Peek(PREADER, (const BYTE*)NULL, 1, o))) {	
				if (EINPUT(o)) return -2;
				break;
			}
		}
		if (PREADER->buf[pos] == trie->c) {		
			trie = trie->n;
			pos++;
			if (!trie->c) {
				found=pos-PREADER->pos;
				IPOS(found);
				found=(int)trie->n;			
				if (!trie->sib) break;
				trie = trie->sib;
			}	
		}
		else {
			if (!trie->sib) break;
			trie = trie->sib;
		}
	}
	return found;
}

/*===========================================================================
  FUNCTION
    SkipWS
  DESC
    skips whitespace and is used to determine whitespace too
  PARAMS
    parser      this
  RETURNS
    Success: number of whitespace bytes
    Failure: -1 (sets ERR_XMLP_INVALID_TOKEN when EOF encountered)
===========================================================================*/
static int SkipWS(LPXMLPARSER parser)
{
	int l=0, chSize;
	XMLCH *c;
	while((c = ReadCh(parser, &chSize))) {
		if (chSize != 1 || !isspace(*c)) {
			DPOS(chSize); /* ACHTUNG! */
			return l;
		}
		l++;
	}
	if (!parser->ErrorCode)
		ErP_(parser, ERR_XMLP_INVALID_TOKEN, 0);
	return -1;
}

/*===========================================================================
  FUNCTION
    RequireCh
  DESC
    requires char in the input
  PARAMS
    parser      this
    ch          character that's required
    allowWS     whether to allow whitespace to appear before char is found
  RETURNS
    Success: 1
    Failure: 0 (error is set)
===========================================================================*/
static int RequireCh(LPXMLPARSER parser, XMLCH ch, int allowWS)
{	
	XMLCH *c;
	int chSize;
	
	while((c = ReadCh(parser, &chSize))) {
		if (chSize == 1) {
			if (*c == ch) return 1;
			else if (isspace(*c)) {
				if (!allowWS)
					return ErP_(parser, ERR_XMLP_WS_NOT_ALLOWED, 1);
				continue;
			}
		}
		break;
	}
	if (!parser->ErrorCode) {/* ReadCh might set parser err */
		XMLCH tok[2];
		tok[0] = ch;
		tok[1] = '\0';
		if (chSize) { DPOS(chSize); }
		Er_(parser, ERR_XMLP_EXPECTED_TOKEN, tok);
	}
	return 0;
}

/*===========================================================================
  FUNCTION
    ParseNameTok
  DESC
    Parses name token
  PARAMS
    parser      this
    len         out pointer - length of name
    prefixLen   out pointer - length of prefix. Can be NULL pointer when
                prefix isn't needed
    endChars    chars that are legal to end name scanning (besides space chars)
    cEndChars   in/out pointer - in: count of endChars
                out: position of endChar found or 0 if whitespace
                was found instead
  RETURNS
    Success: start position of name in reader buffer
    Failure: -1 (Error is set)
  NOTES
    len might be 0 if skipWS isn't called before ParseNameTok call - this
    is utilized by ReadPERefstart()
===========================================================================*/
static int ParseNameTok(LPXMLPARSER parser, int *len, 
						   int *prefixLen, const XMLCH *endChars, int *cEndChars)
{	
	int chSize, spos = PREADER->pos;
	int prefix = (prefixLen) ? 0 : -1;
	XMLCH *c;
	*len = 0;
	
	while ((c = ReadCh_Opt(parser, &chSize))) {
		if (!*len || (prefix == *len)) {
			if (chSize == 1) {			
				if (!ISMAPCH(RT->nameStart, *c)) break;
			}
			else if (chSize == 2) {
				if (!utf8_isNmstrt2(c)) break;
			}
			else if (chSize == 3) {
				if (!utf8_isNmstrt3(c)) break;
			}
			else break;
		}
		else {
			if (chSize == 1) {
				if (!ISMAPCH(nameAscii, *c)) break;
				if (!prefix && *c == ':') prefix = *len+1;			
			}
			else if (chSize == 2) {
				if (!utf8_isName2(c)) break;
			}
			else if (chSize == 3) {
				if (!utf8_isName3(c)) break;		
			}
			/* else if ((*c & 0xf8) == 0xf0) { 4-byte sequence */
			else break;		
		}
		(*len)+=chSize;
	}

	if (!c) {
		if (!parser->ErrorCode) {
			XMLCH t[5] = "Name";
			if (*cEndChars && *len) {
				t[0] = endChars[(*cEndChars)-1];
				t[1] = '\0';
			}
			Er_(parser, ERR_XMLP_EXPECTED_TOKEN, t);
		}
		return -1;
	}
	
	while (*cEndChars) {
		if (*c == endChars[--(*cEndChars)]) {
			(*cEndChars)++;
			if (!*len) {
				Er_(parser, ERR_XMLP_EXPECTED_TOKEN, "Name");
				return -1;
			}
			break;
		}
	}

	if (!*cEndChars) {
		if (!isspace(*c)) {
			ErP_(parser, ERR_XMLP_INVALID_NAME, chSize);					
			return -1;
		}
	}
	if (prefixLen) *prefixLen = (prefix>0) ? prefix-1 : 0;
	return spos;
}

/* note: short circuit with RT->declAttTable!=NULL before calling */
static LPXMLVECTOR GetDefAtts(LPXMLPARSER parser, XMLCH *e)
{
	LPXMLVECTOR *d = XMLHTable_Lookup(RT->declAttTable, e);
	return (d && (*d)->length) ? *d : NULL;
}

/*===========================================================================
  FUNCTION
    ParseStartTag
  RETURNS
    0 failure
    1 success
===========================================================================*/
static int ParseStartTag(LPXMLPARSER parser)
{
	int namepos, namelen, prefixlen, isEmpty = 0;
	int endChars = 2;
	XMLCH *s;
	LPXMLRUNTIMETAG pTag;
	LPXMLRUNTIMEATT pEnd, pAtt;

	namepos = ParseNameTok(parser, &namelen, &prefixlen, "/>", &endChars);
	if (namepos == -1) return 0;
	
	if (!endChars) {
		if (!namelen) return ErP_(parser, ERR_XMLP_WS_NOT_ALLOWED, 1);
		/* empty check is handled by ParseAttributes */
	}
	else if (endChars == 1) {
		if (!(RequireCh(parser, '>', 0))) return 0;
		isEmpty = 1;
	}
	/* else tag is simple start tag */
	
	if (!(pTag = STACK_PUSH(RT->tagstack, NULL)))
		return Er_(parser, ERR_XMLP_MEMORY_ALLOC);

	if (!(XMLStringbuf_InitUsePool(&pTag->nameBuf, RT->strPool->itemSize, 0, RT->strPool)))
		return Er_(parser, ERR_XMLP_MEMORY_ALLOC);

	pTag->localName = pTag->prefix = EmptyStr;
	pTag->prevScope = RT->nsScope;
	pTag->Scope = (LPXMLHTABLE)NULL;
	
	pTag->qname = XMLStringbuf_Append_Opt(&pTag->nameBuf, PREADER->buf+namepos, namelen+1);
	if (!(pTag->qname)) return Er_(parser, ERR_XMLP_MEMORY_ALLOC);
	pTag->qname[namelen] = '\0';

	if (!parser->DocumentElement) {
		if (!((parser->DocumentElement = xmlMemdup(pTag->qname, namelen+1))))
			return Er_(parser, ERR_XMLP_MEMORY_ALLOC);
#ifdef DTD_SUPPORT
		if (RT->userSubset.systemID && !RT->doctypeParsed && 
			!ParseUserSubset(parser)) return 0;
#endif
	}
 	else if (RT->tagstack->length == 1)
		return Er_(parser, ERR_XMLP_MULTIPLE_TOP);
	
	/* if either ws OR some declared attributes was found for this element,
	  ParseAttributes is called: */
	if (!endChars) { 
		if (!ParseAttributes(parser, pTag, &isEmpty, NULL)) return 0;
	}
	else if (RT->declAttTable) {
		LPXMLVECTOR d = GetDefAtts(parser, pTag->qname);
		if (d && !ParseAttributes(parser, pTag, &isEmpty, d)) return 0;
	}

	if (PFLAG(XMLFLAG_NAMESPACES)) {
		if (prefixlen) {
			if (!ParseTagNS(parser, pTag, namelen, prefixlen)) return 0;
		}
		else if (RT->nsScope != NULL &&
			((s = XMLHTable_Lookup(RT->nsScope, "xmlns"))!=NULL)) {
			pTag->uri = s;
			pTag->localName = pTag->qname;
		}
		else {
			pTag->uri = EmptyStr;
		}
	}
	else 
		pTag->uri = EmptyStr;

	if (HANDLER(startElement)) {
		if (HANDLER(startElement)(parser->UserData, pTag->uri,
			pTag->localName, pTag->qname, RT->atts) == XML_ABORT)
				return Er_(parser, ERR_XMLP_ABORT);
	}

	if (isEmpty) {
		if (HANDLER(endElement)) {
			if (HANDLER(endElement)(parser->UserData, pTag->uri,
				pTag->localName, pTag->qname) == XML_ABORT)
				return Er_(parser, ERR_XMLP_ABORT);
		}		
		XMLStringbuf_Free(&pTag->nameBuf);
		if (pTag->Scope!=NULL)
			XMLHTable_Destroy(pTag->Scope, DestroyUriTableProc, 1);
			
		RT->nsScope = pTag->prevScope;
		STACK_REMOVE(RT->tagstack);
	}

	CLEANUP_ATTS;
	return 1;
}

/*===========================================================================
  FUNCTION
    ParseTagNS
  DESC
    Set tag's namespace uri, localName and prefix
  PARAMS
    parser      this
    pTag        tag from ParseStartTag()
    pos         position of ':' character in pTag->qname
  RETURNS
    0 failure
    1 success
===========================================================================*/
static int ParseTagNS(LPXMLPARSER parser, LPXMLRUNTIMETAG pTag, int namelen, int pos)
{	
	pTag->qname = XMLStringbuf_Append_Opt(&pTag->nameBuf, pTag->qname, namelen+1);
	if (!pTag->qname) return Er_(parser, ERR_XMLP_MEMORY_ALLOC);
	/* qname can be relocated! */

	pTag->prefix=pTag->qname+namelen+1;
	if (pos == 3 && ISXMLPREFIX(pTag->prefix))		
		return ErP_(parser, ERR_XMLP_INVALID_NAME, 3);

	pTag->prefix[pos] = '\0';
	pTag->localName = pTag->prefix+pos+1;

	if (RT->nsScope == NULL ||
		((pTag->uri = XMLHTable_Lookup(RT->nsScope, pTag->prefix))==NULL)) {
			pTag->uri = EmptyStr;
			return Er_(parser, ERR_XMLP_UNDEF_NSPREFIX, pTag->prefix);
	}
	return 1;
}

/*===========================================================================
  FUNCTION
    ParseAttributes
  DESC
    Parse attributes and resolve namespace declarations
  PARAMS
    parser      this
    pTag        tag from ParseStartTag or NULL if ns handling isn't needed.
    isEmpty     for ParseStartTag to indicate that element doesn't have any
                content.
    DeclAtts    Pointer to declared attributes for this element
                (ParseStartTag has encountered > already)
  RETURNS
    0 failure
    1 success
  TODO
    split into smaller routines? separate ns decl handling?
===========================================================================*/
static int ParseAttributes(LPXMLPARSER parser, LPXMLRUNTIMETAG pTag, 
						   int *isEmpty, LPXMLVECTOR DeclAtts)
{
	int attCount, i, nsDefdecl, namepos, namelen, prefixlen, endChars;
	int attType, iDeclAtts;
	int processNS = PFLAG(XMLFLAG_NAMESPACES);
	int NSprefixes = PFLAG(XMLFLAG_NAMESPACE_PREFIXES);
	XMLCH *uri, *s;
	LPXMLRUNTIMEATT pAtt;
	attCount = nsDefdecl = iDeclAtts = 0;
	
	while (1)
	{	
		if (!DeclAtts) {
			if ((i = SkipWS(parser)) == -1) return 0;

			if (CURCHAR == '>') {
				IPOS(1);
				*isEmpty = 0;
				if (!DeclAtts && RT->declAttTable) 
					DeclAtts = GetDefAtts(parser, pTag->qname);
				if (DeclAtts) continue;
				break;
			}
			else if (CURCHAR == '/') {
				IPOS(1);
				*isEmpty = 1;
				if (!(RequireCh(parser, '>', 0))) return 0;
				if (!DeclAtts && RT->declAttTable) 
					DeclAtts = GetDefAtts(parser, pTag->qname);
				if (DeclAtts) continue;
				break;
			}

			if (attCount++ && !i)
				return ErP_(parser, ERR_XMLP_WS_REQUIRED, 0);
		
 			endChars = 1;
			namepos = ParseNameTok(parser, &namelen, &prefixlen, "=", &endChars);
			if (namepos == -1) return 0; /* error is set */
			
			if (!endChars && !namelen)
				return Er_(parser, ERR_XMLP_EXPECTED_TOKEN, ">");

			if(!(pAtt = XMLVector_Append(RT->atts, NULL)))
				return Er_(parser, ERR_XMLP_MEMORY_ALLOC);
			pAtt->localName = pAtt->prefix = pAtt->uri = EmptyStr;
			pAtt->valBuf.str = NULL; /* for CLEANUPATTS */

			if (!XMLStringbuf_InitUsePool(&pAtt->nameBuf, RT->strPool->itemSize, 
				0, RT->strPool))
					return Er_(parser, ERR_XMLP_MEMORY_ALLOC);
			pAtt->qname = XMLStringbuf_Append_Opt(&pAtt->nameBuf, 
				PREADER->buf+namepos, namelen+1);
			if (!pAtt->qname)
				return Er_(parser, ERR_XMLP_MEMORY_ALLOC);
			pAtt->qname[namelen] = '\0';
			
			if (!endChars) {
				if (!(RequireCh(parser, '=', 1))) return 0;
			}
			
			if (!XMLStringbuf_InitUsePool(&pAtt->valBuf, RT->strPool->itemSize, 0, RT->strPool))
				return Er_(parser, ERR_XMLP_MEMORY_ALLOC);
			pAtt->value = ParseString(parser, &pAtt->valBuf, pAtt,
				(!(PFLAG(XMLFLAG_PRESERVE_WS_ATTRIBUTES))), 2);
 			if (!pAtt->value) return 0;
		
		}
#ifdef DTD_SUPPORT
		else { /* DeclAtts - handle default attributes and normalization: */
			for (pAtt=NULL; iDeclAtts < DeclAtts->length;) {
				LPXMLATTDECL pDeclAtt = XMLVector_Get(DeclAtts, iDeclAtts++);
				i = (int)XMLHTable_Lookup(RT->namedAtts, pDeclAtt->name);
				if (!i) {
					if (pDeclAtt->value) { /* has default value: */
						if(!(pAtt = XMLVector_Append(RT->atts, NULL)))
							return Er_(parser, ERR_XMLP_MEMORY_ALLOC);
						pAtt->localName = pAtt->prefix = pAtt->uri = EmptyStr;
						pAtt->qname = pDeclAtt->name;
						pAtt->value = pDeclAtt->value;
						/* we set stringbuf lengths to correct values: */
						pAtt->nameBuf.len = pDeclAtt->nameLen;
						pAtt->valBuf.len = pDeclAtt->valueLen;
						/* BUT set stringbuf strs to isDefaulted "flags": */
						pAtt->nameBuf.str = pAtt->valBuf.str = (XMLCH*)NULL;
						namelen = pDeclAtt->nameLen;
						prefixlen = pDeclAtt->prefixLen;
						break;
					}
				}
				else if (pDeclAtt->type != 1) { /* not CDATA, must be normalized: */
					pAtt = XMLVector_Get(RT->atts, i-1);
					if (!_XMLParser_AttIsDefaulted(pAtt)) { /* unless defaulted
						(which actually mean duplicate declaration) */
						i = XMLNormalizeBuf(pAtt->value, pAtt->valBuf.len);
						if (i < pAtt->valBuf.len) {
							pAtt->value[i] = '\0';
							pAtt->valBuf.len = i;
						}
					}
					pAtt = NULL;
				}
			}
			if (!pAtt) break;
		}
#endif /* DTD_SUPPORT */

		/* determine the type of attribute: */
		if (!processNS)
			attType = XMLATT_NORMAL;
		else {
			if (prefixlen) {
				if (prefixlen == 5 && ISXMLNSPREFIX(pAtt->qname)) 
					attType = XMLATT_PREFIXDECL;
				else if (prefixlen == 3 && ISXMLPREFIX(pAtt->qname))
					attType = XMLATT_XMLPREFIXDECL;
				else
					attType = XMLATT_WITHNS;
			}
			else {
				if (namelen == 5 && ISXMLNSPREFIX(pAtt->qname))
					attType = XMLATT_DEFAULTDECL;
				else
					attType = XMLATT_NORMAL;
			}
		}

		if (processNS) {

		if (attType == XMLATT_PREFIXDECL) {
			/* namespace declaration: */
			if (!pAtt->valBuf.len)
				return Er_(parser, ERR_XMLP_INVALID_ATT_VALUE, pAtt->qname);
		
			INIT_NSSCOPE; /* initialize RT->nsScope */
			
			if (!(uri = prepUri(pAtt->value, 1)))
				return Er_(parser, ERR_XMLP_MEMORY_ALLOC);
		
			s = XMLHTable_Insert(RT->nsScope,
				pAtt->qname+prefixlen+1, (void*)uri);
			if (!s) return Er_(parser, ERR_XMLP_MEMORY_ALLOC);
			else if (uri != s) {					
				if (*(s-1)) { /* check if declared in this element. see prepURI */
					free((s-1));
					if (_XMLParser_AttIsDefaulted(pAtt)) {
						XMLVector_Remove(RT->atts, RT->atts->length-1);
						continue;
					}
					return Er_(parser, ERR_XMLP_DUPL_ATTRIBUTE, pAtt->qname);
				}
				free((s-1));
			}
			if (!NSprefixes) {
				if (!_XMLParser_AttIsDefaulted(pAtt)) {
					XMLStringbuf_Free(&pAtt->nameBuf);
					XMLStringbuf_Free(&pAtt->valBuf);
				}
				XMLVector_Remove(RT->atts, RT->atts->length-1);
				continue;
			}
			pAtt->uri = uriXMLNS; /* note that xmlns or xml prefixes aren't set,
								only static uris */
		}
		else if (attType == XMLATT_XMLPREFIXDECL) {
			/* predefined xml namespace: */
			pAtt->localName = pAtt->qname+prefixlen+1;
			
			if (strcmp(pAtt->localName, "id")) { /* scope unless id: */
				if (!strcmp(pAtt->localName, "space")) {
					if (!(!strcmp(pAtt->value, "default") ||
						!strcmp(pAtt->value, "preserve")))
						return Er_(parser, ERR_XMLP_INVALID_ATT_VALUE, pAtt->qname);
				}
			
				INIT_NSSCOPE; /* initialize RT->nsScope */
			
				if (!(uri = prepUri(pAtt->value, 1)))
					return Er_(parser, ERR_XMLP_MEMORY_ALLOC);
				
				s = XMLHTable_Insert(RT->nsScope, pAtt->qname, (void*)uri);
				if (!s) return Er_(parser, ERR_XMLP_MEMORY_ALLOC);
				else if (uri != s) free((s-1)); /* note that pointer must be 
					decremented */
			}
			pAtt->uri = uriXML; /* static uri */
		}
		else if (attType == XMLATT_WITHNS) {
				/* set prefix and localName (they will be examined later when
				all atts are collected into vector) */
				if (!_XMLParser_AttIsDefaulted(pAtt)) {
					/* note that qname can be relocated */
					pAtt->qname = XMLStringbuf_Append(&pAtt->nameBuf, 
						pAtt->qname, namelen+1);
					if (!pAtt->qname)
						return Er_(parser, ERR_XMLP_MEMORY_ALLOC);
					pAtt->prefix=pAtt->qname+namelen+1;
					pAtt->prefix[prefixlen] = '\0';
				}
				else
					pAtt->prefix=pAtt->qname+namelen+1;
				
				pAtt->localName = pAtt->prefix+prefixlen+1;
		}
		/* test default declaration: */
		else if (attType == XMLATT_DEFAULTDECL) {

			if (nsDefdecl) {
				if (_XMLParser_AttIsDefaulted(pAtt)) {
					XMLVector_Remove(RT->atts, RT->atts->length-1);
					continue;
				}
				return Er_(parser, ERR_XMLP_DUPL_ATTRIBUTE, pAtt->qname);
			}
		
			nsDefdecl = 1;
			INIT_NSSCOPE;

			if (!pAtt->valBuf.len) { /* undeclaration: */			
				if ((uri = XMLHTable_Remove(RT->nsScope, "xmlns")) != NULL)
					free((uri-1)); /* note that pointer must be decremented */
			}
			else {
				if (!(uri = prepUri(pAtt->value, 1)))
					return Er_(parser, ERR_XMLP_MEMORY_ALLOC);

				s = XMLHTable_Insert(RT->nsScope, "xmlns", (void*)uri);
				if (!s) return Er_(parser, ERR_XMLP_MEMORY_ALLOC);	
				else if (uri != s) free((s-1));
			}
			if (!NSprefixes) {
				if (!_XMLParser_AttIsDefaulted(pAtt)) {
					XMLStringbuf_Free(&pAtt->nameBuf);
					XMLStringbuf_Free(&pAtt->valBuf);
				}
				XMLVector_Remove(RT->atts, RT->atts->length-1);
				continue;
			}
			pAtt->uri = uriXMLNS;
		}
		} /* processNS (namespaces on) */
		/* store attribute into namedAtts hashtable; we cannot store
			direct pointer to Vector item 'cos it might get relocated thus
			we have to store index to newly added
			vector item. Same thing applies to RT->entitiesTable.
			We just use void* hashtable item to hold index, this might change
			when hashtable gets changed/optimized. */
		i = (int)XMLHTable_Insert(RT->namedAtts, pAtt->qname, (void*)RT->atts->length);
		if (RT->atts->length != i) {
			if (!i) return Er_(parser, ERR_XMLP_MEMORY_ALLOC);
			return Er_(parser, ERR_XMLP_DUPL_ATTRIBUTE, pAtt->qname);
		}
	}

	if (processNS && RT->atts->length) {
		/* test Vector for declared namespace prefixes and set uris. Cannot
			test attribute prefixes "on the fly" 'cos declaration can follow
			the prefixed attribute i.e. x:att='val' xmlns:x='uri.org'
			is legal. */
		LPXMLRUNTIMEATT pEnd = (LPXMLRUNTIMEATT)_XMLVector_GetIterP(RT->atts, pAtt);
		for (; pAtt!=pEnd; pAtt++) {
			if (pAtt->prefix != EmptyStr) {
				if (RT->nsScope == NULL ||
					((uri = XMLHTable_Lookup(RT->nsScope, pAtt->prefix))==NULL))
				{
					pAtt->uri = EmptyStr;
					return Er_(parser, ERR_XMLP_UNDEF_NSPREFIX, pAtt->prefix);
				}
				pAtt->uri = uri;
			}
		}
	}
	return 1;
	/* NOTE: if there's attributes in the vector and error occurs,
	vector will be emptied in XMLParser_Parse */
}

/*===========================================================================
  FUNCTION
    ParseComment
  DESC
    Parses comment tag
  PARAMS
    parser      this
    skip        TRUE when parsing External DTD
  RETURNS
    0 failure
    1 success
===========================================================================*/
/* NOTE: RT->charsBuf will be emptied in _parse in error condition
  thus we can return 0 at any time in ParseComment, ParseCData etc. */
static int ParseComment(LPXMLPARSER parser, int skip)
{
	int chSize, startPos;
	XMLCH *c;
	int bReport = (!skip && HANDLER(comment));

	startPos = PREADER->pos + 1;
	
	while((c = ReadCh(parser, &chSize))) {				
		if (chSize == 1) {	
			if (*c == '-' && PREADER->pos > startPos) {
				if (*(c-1) == '-') {
					if (!RequireCh(parser, '>', 0)) break;
					if (bReport) {
						if (HANDLER(comment)(parser->UserData, RT->charsBuf.str,
							RT->charsBuf.len-1) == XML_ABORT)
								return Er_(parser, ERR_XMLP_ABORT);
						if (!XMLStringbuf_SetLength(&RT->charsBuf, 0))
							return Er_(parser, ERR_XMLP_MEMORY_ALLOC);
					}
					return 1;
				}
			}
		}
		if (bReport) {
			if (!(XMLStringbuf_Append(&RT->charsBuf, c, chSize)))
				return Er_(parser, ERR_XMLP_MEMORY_ALLOC);
		}
	}
	
	if (!parser->ErrorCode)
		Er_(parser, ERR_XMLP_EXPECTED_TOKEN, "-->");
	return 0;
}

/*===========================================================================
  FUNCTION
    ParseCData
  DESC
    Parses CData tag
  RETURNS
    0 failure
    1 success
===========================================================================*/
static int ParseCData(LPXMLPARSER parser)
{
	int chSize, startPos;
	XMLCH *c;
	
	if (!RT->tagstack->length)		
		return ErP_(parser,  ERR_XMLP_INVALID_AT_TOP, 9);
	startPos = PREADER->pos + 2; /* start testing ]] when > found */
	
	while((c = ReadCh(parser, &chSize))) {				
		if (chSize == 1) {	
			if (*c == '>' && PREADER->pos > startPos) {
				if (*(c-1) == ']' && *(c-2) == ']') {			
					if (HANDLER(startCDATA)) {
						if (HANDLER(startCDATA)(parser->UserData) == XML_ABORT)
							return Er_(parser, ERR_XMLP_ABORT);				
					}
					
					if (HANDLER(characters) && (RT->charsBuf.len - 2) > 0) {
							if (HANDLER(characters)(parser->UserData,
								RT->charsBuf.str, RT->charsBuf.len - 2) == XML_ABORT)
									return Er_(parser, ERR_XMLP_ABORT);
					}

					if (HANDLER(endCDATA)) {
						if (HANDLER(endCDATA)(parser->UserData) == XML_ABORT)
								return Er_(parser, ERR_XMLP_ABORT);
					}
					if (!XMLStringbuf_SetLength(&RT->charsBuf, 0))
						return Er_(parser, ERR_XMLP_MEMORY_ALLOC);
					return 1;
				}
			}
		}
		if (!(XMLStringbuf_Append(&RT->charsBuf, c, chSize)))
			return Er_(parser, ERR_XMLP_MEMORY_ALLOC);
	}
	
	if (!parser->ErrorCode)
		Er_(parser, ERR_XMLP_EXPECTED_TOKEN, "]]>");
	return 0;
}

/*===========================================================================
  FUNCTION 
    ParsePI
  DESC
    Parses processing instruction tag
  RETURNS
    0 failure
    1 success
===========================================================================*/
static int ParsePI(LPXMLPARSER parser, int skip)
{
	int namepos, namelen, endChars = 1;
	int bReport = (!skip && HANDLER(processingInstruction));
	XMLCH *target, *data = EmptyStr;
	
	namepos = ParseNameTok(parser, &namelen, (int*)NULL, "?", &endChars);
	if (namepos == -1) return 0; /* error is set */
	if (!namelen) return ErP_(parser, ERR_XMLP_WS_NOT_ALLOWED, 1);	
	target = BUFTOSTR(PREADER->buf, namepos, namepos+namelen);
	
	if (namelen == 3 && !stricmp(target, "xml"))
		return Er_(parser, ERR_XMLP_INVALID_DECL, "xml");

	if (endChars) {
		if (!RequireCh(parser, '>', 0)) return 0;
	}
	else {
		int startPos, chSize;
		XMLCH *c;

		if (SkipWS(parser) == -1) return 0;
		startPos = PREADER->pos + 1;

		while((c = ReadCh(parser, &chSize))) {				
			if (chSize == 1) {	
				if (*c == '>' && PREADER->pos > startPos) {
					if (*(c-1) == '?') {
						if (bReport && (RT->charsBuf.len - 1) > 0) {
							data = RT->charsBuf.str;
							data[RT->charsBuf.len-1] = '\0';
						}
						break;	
					}
				}
			}
			if (bReport) {
				if (!(XMLStringbuf_Append(&RT->charsBuf, c, chSize)))
					return Er_(parser, ERR_XMLP_MEMORY_ALLOC);
			}
		}
		if (parser->ErrorCode) return 0;
		if (!c) return Er_(parser, ERR_XMLP_EXPECTED_TOKEN, "?>");
	}

	if (bReport) {
		if (HANDLER(processingInstruction)(parser->UserData,
			target, data) == XML_ABORT) {
			return Er_(parser, ERR_XMLP_ABORT);
		}
		if (!XMLStringbuf_SetLength(&RT->charsBuf, 0))
			return Er_(parser, ERR_XMLP_MEMORY_ALLOC);
	}
	return 1;
}

/*===========================================================================
  FUNCTION
    SetEncoding
  DESC
    Sets parser's BUFFEREDISTREAM encode handler
  PARAMS
    parser          this
    encodingName    encoding name string
  RETURNS
    0 failure
    1 success
===========================================================================*/
static int SetEncoding(LPXMLPARSER parser, XMLCH *encodingName)
{
	LPFNENCODE fnEncode = NULL;
	
	if (!stricmp(encodingName, "UTF-8"))
		fnEncode = Utf8ToUtf8;
	else if (!stricmp(encodingName, "ISO-8859-1"))
		fnEncode = Latin1ToUtf8;
	else if (!stricmp(encodingName, "US-ASCII"))
		fnEncode = Latin1ToUtf8;
#ifdef ICONV_SUPPORT
	else if (!PREADER->encode) {
		PREADER->cd = iconv_open("UTF-8", encodingName);
		if (PREADER->cd != (iconv_t)(-1)) fnEncode = iconvWrapper;
		else PREADER->cd = (iconv_t)NULL; /* must be set to NULL */
	}
#endif

	if (!PREADER->encode) {
		if (!fnEncode)
			return Er_(parser, ERR_XMLP_UNSUP_ENCODING, encodingName);
		else if (fnEncode != Utf8ToUtf8) {
			int i;			
			for (i=0; *encodingName && i<19;)
				PREADERDATA->EncodingName[i++] = *encodingName++;
			PREADERDATA->EncodingName[i] = '\0';
		}
		PREADER->encode = fnEncode;
	}
	else { /* encoding is already set, we must check that new
		   one doesn't conflict with the previously set encoding: */
		if (fnEncode == Utf8ToUtf8 && !*PREADERDATA->EncodingName) return 1;
		else if (!stricmp(encodingName, PREADERDATA->EncodingName)) return 1;
#ifdef ICONV_SUPPORT
		else {
			int l1 = strlen(encodingName);
			int l2 = strlen(PREADERDATA->EncodingName);
			XMLCH *s;

			if (l1 > l2)
				s = encodingName;
			else {
				int i = l1;
				l1 = l2;
				l2 = i;
				s = PREADERDATA->EncodingName;
			}

			if (l1 - l2 == 2) {
				XMLCH c = s[l1-2];

				s[l1-2] = '\0';		
				l2 = stricmp(encodingName, PREADERDATA->EncodingName);
				s[l1-2] = c;

				if (!l2) {
					if (s == PREADERDATA->EncodingName) return 1;
					s += (l1-2);
					if (PREADERDATA->lEndian) {
						if (toupper(s[0]) == 'L' && toupper(s[1]) == 'E')
							return 1;
					}
					else {
						if (toupper(s[0]) == 'B' && toupper(s[1]) == 'E')
							return 1;
					}
				}
			}
		}
#endif
		return Er_(parser, ERR_XMLP_SWITCH_ENCODING,
			((PREADERDATA->EncodingName[0]) ?
			PREADERDATA->EncodingName : (XMLCH*)"UTF-8"), encodingName);
	}
	return 1;
}

/*===========================================================================
  FUNCTION
    ParseXmlDecl
  DESC
    Parses xmldecl / textdecl
  PARAMS
    parser      this
    skip        TRUE when parsing External entity or external DTD
	sbuf        using its own sbuf here for entitydecls etc.
                that use charsBuf
  RETURNS
    0 failure
    1 success
===========================================================================*/
static int ParseXmlDecl(LPXMLPARSER parser, int skip, LPXMLSTRINGBUF sbuf)
{
	int l, iver, ienc, ista, count=0;
	XMLCH *enc, *sta, *ver;
	iver = ienc = ista = -1;
	IPOS(5); /* skip <?xml */
	
	while (1) {	
		if ((l = SkipWS(parser)) == -1) return 0;

		if (CURCHAR == '?') {
			IPOS(1);
			if (!(RequireCh(parser, '>', 0))) return 0;
			break;
		}
		
		if (!l)
			return ErP_(parser, ERR_XMLP_WS_REQUIRED, 0);

		switch(TrieRaw(parser, TRxmlDecl)) {
			case 1: 
				if (iver != -1 || count) return 0; 
				iver = sbuf->len;
				break;
			case 2: 
				if (skip) {
					if (ienc != -1) return 0;
				}
				else {
					if (count != 1 || ienc != -1) return 0;
				}
				ienc = sbuf->len;
				break;
			case 3:
				if (iver == -1 || ista != -1 || skip) return 0;
				ista = sbuf->len;
				break;
			default: return 0;
		}

		if (!RequireCh(parser, '=', 1)) return 0;	
		if (!ParseString(parser, sbuf, NULL, 0, 0)) return 0;
		sbuf->len++; /* skip NUL in XMLStringbuf buffer */
		count++;
	}
	
	if (!count) return 0;

	ver = (iver == -1) ? (XMLCH*)NULL : sbuf->str+iver;
	enc = (ienc == -1) ? (XMLCH*)NULL : sbuf->str+ienc;
	sta = (ista == -1) ? (XMLCH*)NULL : sbuf->str+ista;

	if (ver) {
		if (strcmp(ver, "1.0")) return 0;
	}
	else {
		if (!skip) return 0;
	}

	if (enc && !SetEncoding(parser, enc)) return 0;
	if (sta) {
		if (strcmp(sta, "no")) {
			if (strcmp(sta, "yes")) return 0;
		}
	}

	if (!skip && HANDLER(xmlDecl)) {
		if (HANDLER(xmlDecl)(parser->UserData, ver, enc, sta) == XML_ABORT)
			return Er_(parser, ERR_XMLP_ABORT);
	}
	return 1;
}

/*===========================================================================
  FUNCTION
    TryXmlDecl
  DESC
    Checks the presence of XMLDecl
  PARAMS
    parser      this
    charBytes   char size detected from bom (2 or 4). EncodingDecl is mandatory
                when charBytes is specified.
    skip        for ParseXmlDecl call
  RETURNS
    0 failure
    1 success
  NOTES
    Called from DetectEncoding. If charBytes is specified, we have
    to turn unicode xmldecl into ascii buffer before ParseXMLDecl.
    This allows us to detect several encodings without boms as long as
    there is LE of BE specified or course. See XML spec appendix F.
    Assumes bytesavail contains whole XMLDecl.
===========================================================================*/
static int TryXmlDecl(LPXMLPARSER parser, int charBytes, int skip)
{	
	int ret, declBytes;
	XMLSTRINGBUF sbuf;

	if (!charBytes) {
		ret = PEEKINPUT(NULL, 6);
		if (EINPUT(ret)) return 0;
		if (ret || 
			PREADER->buf[0]!='<' || PREADER->buf[1]!='?' || 
			PREADER->buf[2]!='x' || PREADER->buf[3]!='m' ||
			PREADER->buf[4]!='l' || !isspace(PREADER->buf[5])) 
			return 1;
		declBytes = 0;
	}
	else {
		XMLCH *in, *out;
		UINT32 c;
		
		if (charBytes == 2) {
			ret = (PREADERDATA->lEndian) ? /* UCS-2 or UTF-16 */
				PEEKINPUT("<\0?\0x\0m\0l\0", 10) :
				PEEKINPUT("\0<\0?\0x\0m\0l", 10);
		}
		else { /* charBytes == 4, UCS-4 or UTF-32 */
			ret = (PREADERDATA->lEndian) ?
				PEEKINPUT("<\0\0\0?\0\0\0x\0\0\0m\0\0\0l\0\0\0", 20) :
				PEEKINPUT("\0\0\0<\0\0\0?\0\0\0x\0\0\0m\0\0\0l", 20);
		}

		if (EINPUT(ret)) return 0;
		if (ret) return Er_(parser, ERR_XMLP_EXPECTED_TOKEN, "EncodingDecl");
			
		in = out = PREADER->buf;
	
		while (1) {
			if (charBytes == 2) {
				c = (PREADERDATA->lEndian ?
					in[0] + (in[1] << 8) : (in[0] << 8) + in[1]);
			}
			else { /* charBytes == 4 */
				c = (PREADERDATA->lEndian ?
					in[0] + (in[1] << 8) + (in[2] << 16) + (in[3] << 24) :
					(in[0] << 24) + (in[1] << 16) + (in[2] << 8) + in[3]);
			}
			*out++ = (char)c;	
			in+=charBytes;
			if (c == 62) break;
			if ((in - PREADER->buf >= PREADER->bytesavail) || c > 127)
				return Er_(parser, ERR_XMLP_INVALID_DECL, "XML");				
		}						

		declBytes = in - PREADER->buf;
	}
	
	if (!XMLStringbuf_InitUsePool(&sbuf, RT->strPool->itemSize, 0, RT->strPool))
		return Er_(parser, ERR_XMLP_MEMORY_ALLOC);
	
	ret = ParseXmlDecl(parser, skip, &sbuf);
	XMLStringbuf_Free(&sbuf);
	if (!ret) {		
		if (!parser->ErrorCode)
			Er_(parser, ERR_XMLP_INVALID_DECL, "XML");
		return 0;
	}
				
	if (declBytes) PREADER->pos = declBytes;
	
	if (PREADER->pos) { /* skip xml declaration */
		if (EINPUT(BufferedIStream_ResetBuf(PREADER, PREADER->pos)))
			return 0;
	}
	
	if (charBytes && !PREADER->encode)
		return Er_(parser, ERR_XMLP_EXPECTED_TOKEN, "EncodingDecl");
	return 1;
}

/*===========================================================================
  FUNCTION
    DetectEncoding
  DESC
    Detects document encoding and xmldecl/textdecl
  PARAMS
    parser           this
    skip             TRUE this is external entity/DTD
                     FALSE this is main document entity (report xmldecl)
  RETURNS
    0 failure
    1 success
===========================================================================*/
static int DetectEncoding(LPXMLPARSER parser, int skip)
{
	int charBytes = 0; /* 2 or 4 means XMLDecl must be found */
	int ret = PEEKINPUT((const XMLCH*)NULL, 4);
	if (EINPUT(ret)) return 0;
	
	if (!ret) {
		XMLCH *e = NULL;
		XMLCH *b = PREADER->buf;

		if (b[0]==0xEF && b[1]==0xBB && b[2]==0xBF) {	
			/* UTF-8 bom must be skipped: */			
			if (EINPUT(BufferedIStream_ResetBuf(PREADER, 3))) return 0;
		}
		else if (b[0]==0xFF && b[1]==0xFE && b[2]==0x00 && b[3]==0x00) {
			PREADERDATA->lEndian = 1;		
			e = "UCS-4";
		}
		else if (b[0]==0x00 && b[1]==0x00 && b[2]==0xFE && b[3]==0xFF)
			e = "UCS-4";
		else if (b[0]==0xFF && b[1]==0xFE) {
			PREADERDATA->lEndian = 1;
			e = "UTF-16";
		}
		else if (b[0]==0xFE && b[1]==0xFF)
			e = "UTF-16";
		else if (b[0]==0x3C && b[1]==0x00 && b[2]==0x00 && b[3]==0x00) {
			PREADERDATA->lEndian = 1;
			charBytes = 4;
		}
		else if (b[0]==0x00 && b[1]==0x00 && b[2]==0x00 && b[3]==0x3C)		
			charBytes = 4;
		else if (b[0]==0x3C && b[1]==0x00) {
			PREADERDATA->lEndian = 1;
			charBytes = 2;
		}
		else if (b[0]==0x00 && b[1]==0x3C)		
			charBytes = 2;
		
		if (e && !PREADER->encode) {
			/* bom specified encoding, set it and encode buffer: */
			if (!SetEncoding(parser, e)) return 0;
			if ((ret = BufferedIStream_EncodeBuffer(PREADER))) {
				SetReaderFatal(parser, ret);
				return 0;
			}	
		}

		ret = TryXmlDecl(parser, ((PREADER->encode) ? 0 : charBytes), skip);
		if (!ret) return 0;
	}

	/* set default encoding if there wasn't any specified: */
	if (!PREADER->encode) PREADER->encode = Utf8ToUtf8;

	/* encode buffer if it isn't already encoded: */
	if (!PREADER->inbuf) {
		if ((ret = BufferedIStream_EncodeBuffer(PREADER))) {
			SetReaderFatal(parser, ret);
			return 0;
		}
	}
	return 1;
}

#ifndef HAS_STRICMP
int stricmp(const char *s1, const char *s2)
{
    char c1, c2;
    while(1)
    {
		c1 = toupper(*s1++);
		c2 = toupper(*s2++);
		if(c1 == 0 && c2 == 0)
			return 0;
		if(c1 == 0)
			return -1;
		if(c2 == 0)
			return 1;
		if(c1 < c2)
			return -1;
		if(c1 > c2)
			return 1;
    }
}
#endif /* HAS_STRICMP */

unsigned char* xmlMemdup(unsigned char* buf, int len)
{
	unsigned char *d = (unsigned char*)malloc(len*sizeof(unsigned char));
	if (d) memcpy(d, buf, len);
	return d;
}

/*===========================================================================
  FUNCTION
    DestroyUriTableProc
  DESC
    enumeration callback for removing nsScope hashtable keys and data.
  TODO
    endPrefixMapping handler must be implemented here...
=================================c==========================================*/
static int DestroyUriTableProc(char *key, void *data, void *userdata)
{	
	char *d = (char*)data;
	XMLHTable_Remove((LPXMLHTABLE)userdata, key);
	if (d) free(--d);
	return 0;
}

/*===========================================================================
  FUNCTION
    prepUri
  DESC
    duplicates uri string and sets "declared here" flag
  PARAMS
    uri       string to duplicate
    declHere  whether this is declared in this element (bool)
  RETURNS
    duplicate string, NULL if unsuccessful
  NOTES
    uri string's first char contains flag that specifies whether
    this uri is declared in this element or copied from previous scope.
    see CopyUriTableProc. Have to be careful with this hack...
===========================================================================*/
static XMLCH* prepUri(XMLCH *uri, int declHere)
{	
	int len = strlen(uri);
	XMLCH *dup = (XMLCH*)malloc((len + 2) * sizeof(XMLCH));
	if (!dup) return (XMLCH*)NULL;
	*(dup) = (char)declHere;
	memcpy(++dup, uri, len+1);
	return dup;
}

/*===========================================================================
  FUNCTION
    CopyUriTableProc
  DESC
    enumeration callback for copying nsScope hashtable.
===========================================================================*/
static int CopyUriTableProc(char *key, void *data, void *userdata)
{
	XMLCH *dup = prepUri((XMLCH*)data, 0);
	if (!dup) return 1;
	if (!(XMLHTable_Insert((LPXMLHTABLE)userdata, key, (void*)dup))) return 1;
	return 0;
}

/*===========================================================================
  FUNCTION
    XMLNormalizeBuf
  DESC
    Normalizes whitespace and line breaks in buffer and trims the buffer
  RETURNS
    new length of normalized buf
  NOTES
    Doesn't NUL terminate buf to new length - returns the length only.
===========================================================================*/
int XMLAPI XMLNormalizeBuf(XMLCH *buf, int len)
{	
	XMLCH *s1, *s2;	
	s1 = s2 = buf;

	for(; len; len--, s1++) {
		if (isspace(*s1)) {
			if (s2 != buf && *(s2-1) != 0x20)
				*s2++ = 0x20;
		}
		else *s2++ = *s1;
	}

	len = s2 - buf;
	return (len && *(s2-1) == 0x20) ? len-1 : len;
}

/*===========================================================================
  FUNCTION
    XMLParser_Create
  DESC
    XML parser create routine
  RETURNS
    Pointer to parser of NULL if there was a memory allocation problems.
===========================================================================*/
LPXMLPARSER XMLAPI XMLParser_Create(LPXMLPARSER *parser)
{	
	LPXMLPARSER p = ((LPXMLPARSER)*parser) = (LPXMLPARSER)NULL;		
	#ifdef _MSC_VER
		#ifdef _DEBUG
			int tmpFlag = _CrtSetDbgFlag( _CRTDBG_REPORT_FLAG );
			/* Turn on leak-checking bit for dumping leaks to output: */
			tmpFlag |= _CRTDBG_LEAK_CHECK_DF;
			_CrtSetDbgFlag( tmpFlag );
		#endif
	#endif
	
	p = (LPXMLPARSER)malloc(sizeof(XMLPARSER));
	if (!p) return (LPXMLPARSER)NULL;
	memset(p, 0, sizeof(XMLPARSER));

	p->reader = (LPBUFFEREDISTREAM)malloc(sizeof(BUFFEREDISTREAM));
	if (!p->reader) { free(p); return (LPXMLPARSER)NULL; }
	p->reader->buf = p->reader->inbuf = (XMLCH*)NULL;

	p->reader->userdata = (LPBISREADERDATA)malloc(sizeof(BISREADERDATA));
	if (!p->reader->userdata) { XMLParser_Free(p); return (LPXMLPARSER)NULL; }

	p->prt = (LPXMLPARSERRUNTIME)malloc(sizeof(XMLPARSERRUNTIME));
	if (!p->prt) { XMLParser_Free(p); return (LPXMLPARSER)NULL; }
	memset(p->prt, 0, sizeof(XMLPARSERRUNTIME));
		
	p->prt->atts = XMLVector_Create(&p->prt->atts, 16, sizeof(XMLRUNTIMEATT));
	p->prt->tagstack = XMLVector_Create(&p->prt->tagstack, 16, sizeof(XMLRUNTIMETAG));	
	if (!p->prt->atts || !p->prt->tagstack) {
		XMLParser_Free(p);
		return (LPXMLPARSER)NULL;
	}
	p->prt->atts->capacityIncrement = 16;
	p->prt->tagstack->capacityIncrement = 16;

	p->prt->namedAtts = XMLHTable_Create(p->prt->namedAtts, 255);
	if (!p->prt->namedAtts) { XMLParser_Free(p); return (LPXMLPARSER)NULL; }

#ifdef DTD_SUPPORT
	p->prt->entities = XMLVector_Create(&p->prt->entities, 16,
		sizeof(XMLENTITY));
	if (!p->prt->entities) { XMLParser_Free(p); return (LPXMLPARSER)NULL; }
	p->prt->entities->capacityIncrement = 16;
	
	p->prt->entitiesTable = XMLHTable_Create(p->prt->entitiesTable, 64);
	if (!p->prt->entitiesTable) { XMLParser_Free(p); return (LPXMLPARSER)NULL; }
#endif
	
	XMLStringbuf_Init(&p->prt->charsBuf, 4096, 4096);
	p->prt->strPool = XMLPool_Create(128, 8);

	if (!p->prt->charsBuf.str || !p->prt->strPool) {
		XMLParser_Free(p);
		return (LPXMLPARSER)NULL;
	}
	
	p->DocumentElement = (XMLCH*)NULL;
	p->XMLFlags = XMLFLAG_NAMESPACES | XMLFLAG_EXTERNAL_GENERAL_ENTITIES;
	p->prt->nameStart = nameStartAscii;
return (((LPXMLPARSER)*parser) = p);
}

/*===========================================================================
  FUNCTION
    XMLParser_Free
  DESC
    XML parser free routine
===========================================================================*/
void XMLAPI XMLParser_Free(LPXMLPARSER parser)
{
	if (parser) {
		if (PREADER) {
			if (PREADERDATA) free(PREADERDATA);
			free(PREADER);
		}
		if (RT) {
			if (RT->tagstack) XMLVector_Free(RT->tagstack);
			if (RT->atts) XMLVector_Free(RT->atts);
			if (RT->namedAtts) {
				if (RT->namedAtts->table)
					free(RT->namedAtts->table); /* should be empty by now so
						we don't need to call XMLHTable_Destroy... */
				free(RT->namedAtts);
			}			
#ifdef DTD_SUPPORT
			if (RT->entities) XMLVector_Free(RT->entities);		
			if (RT->entitiesTable) {
				if (RT->entitiesTable->table)
					free(RT->entitiesTable->table);
				free(RT->entitiesTable);
			}
#endif
			SAFE_FREESTR(parser->DocumentElement);
			XMLStringbuf_Free(&RT->charsBuf);
			if (RT->strPool) XMLPool_FreePool(RT->strPool);
			free(RT);
		}
		free(parser);
	}
}

/*===========================================================================
  FUNCTION
    XMLParser_GetNamedItem
  DESC
    Looks up an attribute by name
  PARAMS
    parser      this
    name        name to look for
  RETURNS
    attribute or NULL if not found
  NOTES
    this should be called only in startElement
===========================================================================*/
LPXMLRUNTIMEATT XMLAPI XMLParser_GetNamedItem(LPXMLPARSER parser,
											  const XMLCH *name)
{
	int index = (int)XMLHTable_Lookup(RT->namedAtts, (XMLCH*)name);
	return ((index) ? (LPXMLRUNTIMEATT)XMLVector_Get(RT->atts, index-1) :
		(LPXMLRUNTIMEATT)NULL);
}

XMLCH XMLAPI *XMLParser_GetPrefixMapping(LPXMLPARSER parser, const XMLCH *prefix)
{
	return (!parser || !RT->nsScope) ? (XMLCH*)NULL :
		(XMLCH*)XMLHTable_Lookup(RT->nsScope, (XMLCH*)prefix);	
}

LPXMLENTITY XMLAPI XMLParser_GetCurrentEntity(LPXMLPARSER parser) 
{
	if (!parser || !PREADERDATA || !PREADERDATA->curEnt) return NULL;
	return (!PREADERDATA->iCurPE) ? PREADERDATA->curEnt :
		XMLVector_Get(RT->entities, PREADERDATA->iCurPE-1);
}

XMLCH XMLAPI *XMLParser_GetSystemID(LPXMLPARSER parser)
{
#ifdef DTD_SUPPORT
	return (PREADER == RT->refReader) ? (XMLCH*)NULL : RT->systemID;
#else
	return (XMLCH*)NULL;
#endif
}

XMLCH XMLAPI *XMLParser_GetPublicID(LPXMLPARSER parser)
{
#ifdef DTD_SUPPORT
	return (PREADER == RT->refReader) ? (XMLCH*)NULL : RT->publicID;
#else
	return (XMLCH*)NULL;
#endif
}

int XMLAPI XMLParser_GetContextBytes(LPXMLPARSER parser, XMLCH **Bytes, int *cBytes)
{	
	if (!parser || !PREADER->bytesavail) return -1;
	else {
		int pos = PREADER->pos - PREADERDATA->col;
		if (pos < 0) return -1;
		if (Bytes) *Bytes = PREADER->buf + pos;
		if (cBytes) *cBytes = PREADER->bytesavail - pos;
		if (*cBytes < 1) return -1;
	}
	return PREADERDATA->col;
}

int XMLAPI XMLParser_GetCurrentLine(LPXMLPARSER parser)
{	
	return ((!parser || !PREADER->bytesavail) ? -1 : PREADERDATA->line+1);
}

int XMLAPI XMLParser_GetCurrentColumn(LPXMLPARSER parser)
{	
	if (!parser || !PREADER->bytesavail) return -1;
	if (!PREADERDATA->col) return 1;
	else { /* turn bytes PREADERDATA->col into character count: */ 
		XMLCH *b;
		int j, n, i = PREADER->pos-PREADERDATA->col;
		if (i<0) return PREADERDATA->col+1;
		b = PREADER->buf+i;
		for (i=0, n=1; i<PREADERDATA->col; i+=j, n++) {
			UTF8LEN((b+i), j);
		}
		return n;
	}
}

LPXMLENTITY XMLAPI XMLParser_SetExternalSubset(LPXMLPARSER parser, 
					const XMLCH *publicID, const XMLCH *systemID)
{		
	RT->userSubset.type = XML_ENTITY_DOCTYPE;
	RT->userSubset.publicID = (XMLCH*)publicID;
	RT->userSubset.systemID = (XMLCH*)systemID;
	return &RT->userSubset;
}

XMLCH XMLAPI *XMLParser_GetVersionString()
{	
	return PARSIFAL_VERSION;
}

/*===========================================================================
  FUNCTION
    XMLParser_Parse
  PARAMS
    parser      this
    inputSrc    input source callback of type LPFNINPUTSRC
    inputData   to be passed to input source callback
    encoding    overrides xml declaration or BOM marker encoding
  RETURNS
     0 failure (parser->ErrorCode was set)
     1 success
===========================================================================*/
int XMLAPI XMLParser_Parse(LPXMLPARSER parser, LPFNINPUTSRC inputSrc,
						   void *inputData, const XMLCH *encoding)
{	
	int started = 0;
	
	InitEntityReader(parser, PREADER, BIS_DEFAULT_BLOCKSIZE, 
		PREADERDATA, 0, NULL, 0);
	PREADER->inputsrc = inputSrc;
	PREADER->inputData = inputData;
	RT->refReader = PREADER; /* save ref to main reader */
	RT->nsScope = (LPXMLHTABLE)NULL;
	RT->cpNames = NULL;
	RT->cpNodesPool = NULL;
	RT->doctypeParsed = 0;

	SAFE_FREESTR(parser->DocumentElement);
	if (*parser->ErrorString) *parser->ErrorString = '\0';
	parser->ErrorCode = parser->ErrorLine = parser->ErrorColumn = 0;
	
	if (encoding && !SetEncoding(parser, (XMLCH*)encoding)) return 0;

	if (DetectEncoding(parser, 0)) {
		started = 1;
		if (HANDLER(startDocument)) {
			if (HANDLER(startDocument)(parser->UserData) == XML_ABORT) {
				BufferedIStream_Free(PREADER);
				return Er_(parser, ERR_XMLP_ABORT);
			}
		}
		ParseInput(parser);
	}

	while(RT->tagstack->length) {
		LPXMLRUNTIMETAG pTag = STACK_PEEK(RT->tagstack);
		if (!parser->ErrorCode) Er_(parser, ERR_XMLP_UNCLOSED_TAG, pTag->qname);
		XMLStringbuf_Free(&pTag->nameBuf); /* frees localName too */
		if (pTag->Scope!=NULL)
			XMLHTable_Destroy(pTag->Scope, DestroyUriTableProc, 1);
		STACK_REMOVE(RT->tagstack);
	}

	if (parser->ErrorCode) {
		LPXMLRUNTIMEATT pEnd, pAtt;
		CLEANUP_ATTS;
		XMLStringbuf_SetLength(&RT->charsBuf, 0);
		if (RT->nameStart != nameStartAscii) RT->nameStart = nameStartAscii;
	}
	else if (!parser->DocumentElement)
		Er_(parser, ERR_XMLP_EXPECTED_TOKEN, "document element");
	
	if (started && HANDLER(endDocument)) /* fire endDoc even there's an error */
		HANDLER(endDocument)(parser->UserData);

#ifdef DTD_SUPPORT
	if (RT->entities->length) {
		LPXMLENTITY e, pEnd;
		pEnd = (LPXMLENTITY)_XMLVector_GetIterP(RT->entities, e);
		for (; e!=pEnd; e++) {
			XMLHTable_Remove(RT->entitiesTable, e->name);
			free(e->name);
		}
		_XMLVector_RemoveAll(RT->entities);
	}

	if (RT->declAttTable) {
		XMLHTable_Destroy(RT->declAttTable, DestroyDeclAttTableProc, 1);
		RT->declAttTable = (LPXMLHTABLE)NULL;
	}
#endif	 
	BufferedIStream_Free(PREADER); /* doesn't free the parser->reader itself,
		only buffers */
	return (parser->ErrorCode == 0);
}

/*===========================================================================
  FUNCTION
    ParseInput
  DESC
    Main parsing loop
  PARAMS
    parser          this
===========================================================================*/
static void ParseInput(LPXMLPARSER parser)
{
	while (ParseContent(parser)) {	
		switch(TrieRaw(parser, TRxmlTok)) {
			case -1: 
				if (!ParseStartTag(parser)) return; 
				break;
			case 1:
				if (!ParseCData(parser)) return; 
				break;
			case 2:
				if (!ParsePI(parser, 0)) return; 
				break;
			case 3:
				if (!ParseComment(parser, 0)) return; 
				break;
			case 4: 
				if (!ParseEndTag(parser)) return; 
				break;
			case 5: 
				if (!ParseDoctypeDecl(parser)) return; 
				break;
			default: return;
		}
	}
}

/*===========================================================================
  FUNCTION
    ParseDoctypeDecl
  DESC
    Parses doctype declaration tag
  RETURNS
    0 failure
    1 success
  NOTES
    Calls resolveEntityHandler to get inputsrc for parsing external DTD
    (entity type XML_ENTITY_DOCTYPE). Internal subset is always parsed first.
===========================================================================*/
static int ParseDoctypeDecl(LPXMLPARSER parser)
{	
	int iSubset, l, namelen, prefixlen, endChars;
	int iPublicID, iSystemID;
	XMLSTRINGBUF sbuf;
	XMLENTITY ent;
	iSubset = ent.type = 0;
	iPublicID = iSystemID = -1;
	
	if (parser->DocumentElement || RT->doctypeParsed)
		return ErP_(parser, ERR_XMLP_INVALID_TOKEN, 9);
	
	RT->doctypeParsed = 1;
	if (!REQUIRE_WS(l)) return 0;
	endChars = 2;
	l = ParseNameTok(parser, &namelen, &prefixlen, ">[", &endChars);
	if (l == -1) return 0; /* error is set */
	
	XMLStringbuf_Init(&sbuf, 128, 0);
	ent.name = XMLStringbuf_Append(&sbuf, PREADER->buf+l, namelen+1);
	if (!ent.name) {
		Er_(parser, ERR_XMLP_MEMORY_ALLOC);
		goto ExitErr;
	}
	ent.name[namelen] = '\0';

	if (endChars == 1) /* allow <!DOCTYPE name> */
		iSubset = 0;
	else if (endChars == 2) { /* name was ended by [ (w/o any whitespace - this
		is legal) */
		iSubset = 1;
	}
	else { /* found whitespace, parse the rest: */
		while (1) {
			if (SkipWS(parser) == -1) goto ExitErr;
			/* we don't have to test whitespace cos things
			   like docnameSYSTEM "blahblah" will be illegal */
			
			if (CURCHAR == '>') {
				IPOS(1);
				break;
			}
			
			if (CURCHAR == '[') {
				iSubset = 1;
				if (!ent.type) ent.type = XML_ENTITY_DOCTYPE;
				IPOS(1); /* skip [ */
				break;
			}
			
			if (ent.type) goto ExitErr;
			if (!ParseExternalID(parser, &sbuf,  &iPublicID, &iSystemID, 0)) goto ExitErr;
 			ent.type = XML_ENTITY_DOCTYPE;
		}
	}

	ent.publicID = (iPublicID == -1) ? (XMLCH*)NULL : sbuf.str+iPublicID;
	ent.systemID = (iSystemID == -1) ? (XMLCH*)NULL : sbuf.str+iSystemID;

	if (HANDLER(startDTD)) {
		if (HANDLER(startDTD)(parser->UserData, ent.name,
			ent.publicID, ent.systemID, iSubset) == XML_ABORT)
			{
				Er_(parser, ERR_XMLP_ABORT);
				goto ExitErr;
		}
	}

#ifdef DTD_SUPPORT
	if (RT->userSubset.systemID && 
		!ResolveExternalDTD(parser, &RT->userSubset)) goto ExitErr;
	if (iSubset && !ParseDTD(parser, 0)) goto ExitErr;
	if (ent.systemID && !ResolveExternalDTD(parser, &ent)) goto ExitErr;
#else
	if (iSubset) goto ExitErr;
#endif

	if (HANDLER(endDTD)) {
		if (HANDLER(endDTD)(parser->UserData) == XML_ABORT) {
			Er_(parser, ERR_XMLP_ABORT);
			goto ExitErr;
		}
	}
	
	if (PFLAG(XMLFLAG_REPORT_DTD_EXT)) RT->doctypeStr = sbuf.str;
	else XMLStringbuf_Free(&sbuf);
	return 1;
ExitErr:
	if (!parser->ErrorCode) Er_(parser, ERR_XMLP_INVALID_DECL, "doctype");
	XMLStringbuf_Free(&sbuf);
	return 0;
}

static int ParseExternalID(LPXMLPARSER parser, LPXMLSTRINGBUF sbuf,
						   int *iPublicID, int *iSystemID, int allowPubidLit)
{
#ifdef DTD_SUPPORT
	int i = (RT->dtd) ? TrieTok(parser, TRexternalID, 0) :
		TrieRaw(parser, TRexternalID);
#else
	int i = TrieRaw(parser, TRexternalID);
#endif
	
	switch(i) {
		case -1: return Er_(parser, ERR_XMLP_EXPECTED_TOKEN, 
					 "PUBLIC or SYSTEM");
		case 1:
			if (!REQUIRE_WS(i)) return 0;
			*iPublicID = sbuf->len;
			if (RT->dtd) RT->dtd->inLiteral = 1;
			if (!ParseString(parser, sbuf, NULL, 2, 0)) return 0;
			if (allowPubidLit) {
				if ((i = SkipWS(parser)) == -1) return 0;
				if (!ISQUOTE(CURCHAR)) return 1;
				if (!i) return ErP_(parser, ERR_XMLP_WS_REQUIRED, 0);
			}
			else {
				if (!REQUIRE_WS(i)) return 0;
			}
			sbuf->len++;
			break;
		case 2:
			if (!REQUIRE_WS(i)) return 0;
			break;		
		default: return 0;
	}
	*iSystemID = sbuf->len;
	if (RT->dtd) RT->dtd->inLiteral = 1;
	if (!ParseString(parser, sbuf, NULL, 2, 0)) return 0;
	return 1;
}

#ifdef DTD_SUPPORT

/* Get inputsrc for external DTD: */
static int ResolveExternalDTD(LPXMLPARSER parser, LPXMLENTITY e)
{
	BUFFEREDISTREAM dtdReader;

	if (HANDLER(resolveEntity)) {	
		BISREADERDATA readerdata;
		XMLCH *dtdEntName = "[dtd]";

		InitEntityReader(parser, &dtdReader, BIS_DEFAULT_BLOCKSIZE,
			&readerdata, 0, e, 0);
		e->name = dtdEntName;
		
		if (HANDLER(startEntity)) {
			if (HANDLER(startEntity)(parser->UserData, e) == XML_ABORT) {
				Er_(parser, ERR_XMLP_ABORT);
				goto ExitErr;
			}
		}

		if (HANDLER(resolveEntity)(parser->UserData, e, &dtdReader) == XML_ABORT) {
			Er_(parser, ERR_XMLP_ABORT);
			goto ExitErr;
		}
		
		if (dtdReader.inputData) {
			if (!dtdReader.inputsrc)
				dtdReader.inputsrc = RT->refReader->inputsrc;
		
			if (DetectEncoding(parser, 1))
				ParseDTD(parser, 1);

			if (HANDLER(externalEntityParsed)) {
				if (HANDLER(externalEntityParsed)(parser->UserData, e,
					&dtdReader) == XML_ABORT)
						Er_(parser, ERR_XMLP_ABORT);
			}
			if (parser->ErrorCode) goto ExitErr;
		}
		
		if (PREADER != RT->refReader) PREADER = RT->refReader;
		
		if (HANDLER(endEntity)) {
			if (HANDLER(endEntity)(parser->UserData, e) == XML_ABORT) {
				Er_(parser, ERR_XMLP_ABORT);
				goto ExitErr;
			}
		}		
		BufferedIStream_Free(&dtdReader);
	}
	return 1;
ExitErr:
	if (PREADER != RT->refReader) PREADER = RT->refReader;
	BufferedIStream_Free(&dtdReader);
	return 0;
}

static int ParseUserSubset(LPXMLPARSER parser)
{	
	if (HANDLER(startDTD)) {
		if (HANDLER(startDTD)(parser->UserData, parser->DocumentElement,
			RT->userSubset.publicID, RT->userSubset.systemID, 0) == XML_ABORT)
				return Er_(parser, ERR_XMLP_ABORT);			
	}
	if (!ResolveExternalDTD(parser, &RT->userSubset)) return 0;
	if (HANDLER(endDTD)) {
		if (HANDLER(endDTD)(parser->UserData) == XML_ABORT)
			return Er_(parser, ERR_XMLP_ABORT);	
	}
	return 1;
}

static int TrieTok(LPXMLPARSER parser, const struct trie *trie, int requireWS)
{
	int chSize, read, found;
	XMLCH *c;

	DTDTOK_START(0);
	c = ReadCh(parser, &chSize);
	read = chSize;
	found = -1;

	if (c && chSize == 1) {	
		while(1) {
			if (*c == trie->c) {		
				trie = trie->n;
				if (!trie->c) {				
					found = (int)trie->n;
					read = 0;
					if (!trie->sib) break;
					trie = trie->sib;
				}		
				c = ReadCh(parser, &chSize);
				read += chSize;
				if (!c || chSize > 1) break;
			}
			else {
				if (!trie->sib) break;
				trie = trie->sib;
			}
		}
	}
	if (parser->ErrorCode) return -2;
	DTDTOK_END;
	if (read) { DPOS(read); }
	if (requireWS && found >= requireWS && !(REQUIRE_WS(requireWS))) 
		return -2;
	return found;
}

static int TryTok(LPXMLPARSER parser, XMLCH const *tok, int requireWS)
{
	int chSize, read=PREADER->pos;
	XMLCH *c;
	DTDTOK_START(0);
	for (; *tok; tok++) {
		if (!(c = ReadCh(parser, &chSize))) break;
		if (chSize > 1) break;
		if (*c != *tok) break;
	}
	DTDTOK_END;
	if (!*tok) {
		if (requireWS) {
			if (!(REQUIRE_WS(requireWS))) return -2;
		}
		return 0; /* found */
	}
	if (parser->ErrorCode) return -2;
	if ((read = PREADER->pos - read)) {
		DPOS_LF(read);
	}
	return -1;
}

static int Require(LPXMLPARSER parser, XMLCH const *tok, int requireWS)
{
	if ((TryTok(parser, tok, requireWS)) < 0) {
		if (!parser->ErrorCode) Er_(parser, ERR_XMLP_EXPECTED_TOKEN, tok);
		return 0;
	}
	return 1;
}

static int ParseCondSect(LPXMLPARSER parser)
{
	int r, chSize;
	XMLCH *c;
	if (SkipWS(parser) == -1) return 0;
	r = TryTok(parser, "IGNORE", 0);
	if (r < -1) return 0;
	if (!r) {
		int startPos;
		if (!RequireCh(parser, '[', 1)) return 0;
		if (SkipWS(parser) == -1) return 0;
		DTDTOK_START(0);
		startPos = PREADER->pos+2;
		r = 0;
		while ((c = ReadCh(parser, &chSize))) {
			if (*c == '>' && PREADER->pos > startPos) {
				if (*(c-2) == ']' && *(c-1) == ']') {
					if (!r--) break;
				}
			}
			else if (*c == '[' && PREADER->pos > startPos) {
				if (*(c-2) == '<' && *(c-1) == '!') r++;
			}
		}
		DTDTOK_END;
		return (c) ? 1 : Er_(parser, ERR_XMLP_EXPECTED_TOKEN, "]]>");
	}
	else {
		if (!Require(parser, "INCLUDE", 0)) return 0;
		if (!RequireCh(parser, '[', 1)) return 0;
		RT->dtd->inclSect++;
		return 1;
	}
}

static int ParseContentDTD(LPXMLPARSER parser, int isExternal)
{
	int chSize;
	XMLCH *c;

	if (PREADER->pos > 1000 && 
		!ResetLine(parser)) return 0;

	while((c = ReadCh(parser, &chSize))) {		
		if (!isspace(*c)) {
			if (*c == ']') {
				if (!isExternal) {
					if (!RequireCh(parser, '>', 1)) return 0;				
				}
				else if (RT->dtd->inclSect) {
					DPOS(1);
					if (!Require(parser, "]]>", 0)) return 0;
					RT->dtd->inclSect--;
					continue;
				}
				else ErP_(parser, ERR_XMLP_INVALID_TOKEN, 1);

				c = (XMLCH*)NULL;
				break;
			}
			else {
				DPOS(chSize);
				break;
			}
		}
	}
	if (parser->ErrorCode) return 0;
	return (c != (XMLCH*)NULL);
}

/*===========================================================================
  FUNCTION
    ParseDTD
  DESC
    Main parsing loop for DTD 
  RETURNS
    0 failure
    1 success
===========================================================================*/
static int ParseDTD(LPXMLPARSER parser, int isExternal)
{	
	struct tagDTD dtd;

	dtd.pePool =  XMLPool_Create(sizeof(BUFFEREDISTREAM), 4);
	if (!dtd.pePool) return Er_(parser, ERR_XMLP_MEMORY_ALLOC);
	dtd.peStack = XMLVector_Create(&dtd.peStack, 4, sizeof(LPBUFFEREDISTREAM));
	if (!dtd.peStack) {
		XMLPool_FreePool(dtd.pePool);
		return Er_(parser, ERR_XMLP_MEMORY_ALLOC);
	}
	
	XMLStringbuf_Init(&dtd.sbuf, 64, 0);
	XMLStringbuf_Init(&dtd.att.nameBuf, 64, 0);
	XMLStringbuf_Init(&dtd.att.valBuf, 64, 0);
	dtd.refReader = PREADER;
	dtd.expandPEs = 1;
	dtd.inclSect = dtd.inLiteral = 0;
	RT->dtd = &dtd;

	while (ParseContentDTD(parser, isExternal)) {
		int t = TrieTok(parser, TRdtdTok, 4);
		if (t == 1) {
			dtd.expandPEs = 0;
			if (!ParseComment(parser, isExternal)) break;
			dtd.expandPEs = 1;
		}
		else if (t == 2 && isExternal) {
			if (!ParseCondSect(parser)) break;
		}
		else if (t == 3) {
			dtd.expandPEs = 0;
			if (!ParsePI(parser, isExternal)) break;
			dtd.expandPEs = 1;
		}
		else if (t == 4) {
			if (!ParseEntityDecl(parser)) break;
		}
		else if (t == 5) {		
			if (!ParseElementDecl(parser)) break;
		}
		else if (t == 6) {
			if (!ParseAttlistDecl(parser)) break;
		}
		else if (t == 7) {
			if (!ParseNotationDecl(parser)) break;
		}
		else {
			if (!parser->ErrorCode) 
				ErP_(parser, ERR_XMLP_INVALID_TOKEN, 0);
			break;
		}	
	}
	
	if (parser->ErrorCode) {	
		while (dtd.peStack->length) {
			/* enumerate and free peStack: */
			PREADER = (*((LPBUFFEREDISTREAM*)STACK_PEEK(RT->dtd->peStack)));
			if (PREADER->inputsrc == MemInputsrc) 
				free(PREADER->inputData);
			else {
				if (HANDLER(externalEntityParsed) && PREADER->inputData) {
					PREADERDATA->curEnt = XMLParser_GetCurrentEntity(parser);
					HANDLER(externalEntityParsed)(parser->UserData, 
						PREADERDATA->curEnt, PREADER);
				}
			}
			if (PREADERDATA) free(PREADERDATA);
			BufferedIStream_Free(PREADER);
			STACK_REMOVE(dtd.peStack);
		}
		XMLStringbuf_SetLength(&RT->charsBuf, 0);
		/* make sure we have the base reader if error occurred, other
			entity readers must be cleaned up by now: */
		PREADER = RT->refReader;
	}
	XMLStringbuf_Free(&dtd.sbuf);
	XMLStringbuf_Free(&dtd.att.nameBuf);
	XMLStringbuf_Free(&dtd.att.valBuf);
	XMLPool_FreePool(dtd.pePool);
	XMLVector_Free(dtd.peStack);
	if (!PFLAG(XMLFLAG_REPORT_DTD_EXT)) {
		if (RT->cpNames) XMLHTable_Destroy(RT->cpNames, NULL, 1);
		if (RT->cpNodesPool) XMLPool_FreePool(RT->cpNodesPool);
		RT->cpNames = NULL;
		RT->cpNodesPool = NULL;
	}
	RT->dtd = NULL;
	RT->systemID = RT->publicID = NULL;
	return (parser->ErrorCode == 0);
}

/* note that sbuf len will include trailing \0 unlike *len */
static int ParseNameDTD(LPXMLPARSER parser, int *len, int *prefixLen,
						LPXMLSTRINGBUF sbuf, XMLCH *endChars, int cEndChars)
{	
	int r;
	LPBUFFEREDISTREAM rTok = PREADER;
	DTDTOK_START(1);
	r = ParseNameTok(parser, len, prefixLen, endChars, &cEndChars);
	if (r > -1) {
		if (sbuf) {
			int spos = sbuf->len;
			if (!XMLStringbuf_Append(sbuf, rTok->buf+r, (*len)+1)) {
				Er_(parser, ERR_XMLP_MEMORY_ALLOC);
				return -1;
			}
			sbuf->str[spos+*len] = '\0';
			if (prefixLen && *prefixLen) {
				spos = sbuf->len;
				if (!XMLStringbuf_Append(sbuf, sbuf->str, sbuf->len)) {
					Er_(parser, ERR_XMLP_MEMORY_ALLOC);
					return -1;
				}
				sbuf->str[spos+*prefixLen] = '\0';
			}
		}
		DPOS_LF(1); 
	}
	DTDTOK_END;
	return r;
}

/*
[59] Enumeration ::= '(' S? Nmtoken (S? '|' S? Nmtoken)* S? ')'
*/
static int ParseEnumeration(LPXMLPARSER parser, LPXMLVECTOR *en)
{
	int len;
	void **p;

	RT->nameStart = nameAscii;
	if (!XMLStringbuf_AppendCh(&RT->charsBuf, '('))
		return Er_(parser, ERR_XMLP_MEMORY_ALLOC);
	if (SkipWS(parser) == -1) return 0;
	if (ParseNameDTD(parser, &len, NULL, &RT->charsBuf, ")|", 2) == -1) 
		return 0;
	RT->charsBuf.len--;
	if (SkipWS(parser) == -1) return 0;

	if (PFLAG(XMLFLAG_REPORT_DTD_EXT)) {
		XMLVector_Create(en, 0, sizeof(void*));
		if (!*en) return Er_(parser, ERR_XMLP_MEMORY_ALLOC);
		p = XMLVector_Append(*en, NULL);
		if (!p) return Er_(parser, ERR_XMLP_MEMORY_ALLOC);
		*p = (void*)len;
	}

	while (CURCHAR!=')') {
		if (!RequireCh(parser, '|', 1)) return 0;
		if (!XMLStringbuf_AppendCh(&RT->charsBuf, '|'))
			return Er_(parser, ERR_XMLP_MEMORY_ALLOC);
		if (SkipWS(parser) == -1) return 0;
		if (ParseNameDTD(parser, &len, NULL, &RT->charsBuf, ")|", 2) == -1) 
			return 0;
		if (*en) {
			p = XMLVector_Append(*en, NULL);
			if (!p) return Er_(parser, ERR_XMLP_MEMORY_ALLOC);
			*p = (void*)len;
		}
		RT->charsBuf.len--;
		if (SkipWS(parser) == -1) return 0;
	}
	if (!XMLStringbuf_AppendCh(&RT->charsBuf, ')'))
		return Er_(parser, ERR_XMLP_MEMORY_ALLOC);
	if (!XMLStringbuf_ToString(&RT->charsBuf))
		return Er_(parser, ERR_XMLP_MEMORY_ALLOC);
	IPOS(1);
	RT->nameStart = nameStartAscii;
	return 1;
}

static int ParseNotationDecl(LPXMLPARSER parser)
{
	int i, iPublicID, iSystemID;	
	if (ParseNameDTD(parser, &i, NULL, &RT->charsBuf, NULL, 0) == -1) 
		return 0;
	iPublicID = iSystemID = -1;
	if (SkipWS(parser) == -1) return 0;
	if (!ParseExternalID(parser, &RT->charsBuf, &iPublicID, &iSystemID, 1)) return 0;
	if (!RequireCh(parser, '>', 1)) return 0;
	if (HANDLER(notationDecl)) {
		if (HANDLER(notationDecl)(parser->UserData, RT->charsBuf.str,
			(iPublicID == -1) ? NULL : RT->charsBuf.str+iPublicID,
			(iSystemID == -1) ? NULL : RT->charsBuf.str+iSystemID)==XML_ABORT)
				return Er_(parser, ERR_XMLP_ABORT);
	}
	XMLStringbuf_SetLength(&RT->charsBuf, 0);
	return 1;
}

static XMLCP *CpNew(LPXMLPARSER parser) {
	XMLCP *nn = XMLPool_Alloc(RT->cpNodesPool);
	if (nn) {
		nn->children = nn->next = NULL;
		nn->name = NULL;
		nn->type = XMLCTYPE_CHOICE; /* note! */
		nn->rep = '\0';
	}
	else Er_(parser, ERR_XMLP_MEMORY_ALLOC);
	return nn;
}

static XMLCP *CpNewChild(LPXMLPARSER parser, XMLCP *cp)
{
	XMLCP *nn = CpNew(parser);
	if (nn) {
		if (!cp->children) cp->children = nn;
		else cp->next->next = nn;
		cp->next = nn;
	}
	return nn;
}

static int ContentModel2ToString(LPXMLPARSER parser, XMLCP *cp)
{
	XMLCP *n;	 
	
	if (!XMLStringbuf_AppendCh(&RT->charsBuf, '('))
		return Er_(parser, ERR_XMLP_MEMORY_ALLOC);

	for(n=cp->children; n; n=n->next) {	
		if (n->children) {
			if (!ContentModel2ToString(parser, n)) return 0;
		}
		else {
			if (!XMLStringbuf_Append(&RT->charsBuf, n->name, strlen(n->name)))
				return Er_(parser, ERR_XMLP_MEMORY_ALLOC);
			if (n->rep) {
				if (!XMLStringbuf_AppendCh(&RT->charsBuf, n->rep))
					return Er_(parser, ERR_XMLP_MEMORY_ALLOC);
			}
		}
		if (n->next) {
			if (!XMLStringbuf_AppendCh(&RT->charsBuf, 
				(XMLCH)((cp->type == XMLCTYPE_CHOICE) ? '|' : ',')))
				return Er_(parser, ERR_XMLP_MEMORY_ALLOC);
		}
	}
	if (!XMLStringbuf_AppendCh(&RT->charsBuf, ')'))
		return Er_(parser, ERR_XMLP_MEMORY_ALLOC);
	if (cp->rep) {
		if (!XMLStringbuf_AppendCh(&RT->charsBuf, cp->rep))
			return Er_(parser, ERR_XMLP_MEMORY_ALLOC);
	}
	return 1;
}

static int ContentModelToString(LPXMLPARSER parser, XMLCP *cp)
{
	XMLCP *n;

	switch(cp->type) {
	case XMLCTYPE_EMPTY:
		if (!XMLStringbuf_Append(&RT->charsBuf, "EMPTY", 5))
			return Er_(parser, ERR_XMLP_MEMORY_ALLOC);
	break;
	case XMLCTYPE_ANY:
		if (!XMLStringbuf_Append(&RT->charsBuf, "ANY", 3))
			return Er_(parser, ERR_XMLP_MEMORY_ALLOC);
	break;
	case XMLCTYPE_MIXED:		
		if (!XMLStringbuf_Append(&RT->charsBuf, "(#PCDATA", 8))
			return Er_(parser, ERR_XMLP_MEMORY_ALLOC);
		for(n=cp->children; n; n=n->next) {
			if (!XMLStringbuf_AppendCh(&RT->charsBuf, '|'))
				return Er_(parser, ERR_XMLP_MEMORY_ALLOC);
			if (!XMLStringbuf_Append(&RT->charsBuf, n->name, strlen(n->name)))
				return Er_(parser, ERR_XMLP_MEMORY_ALLOC);
		}
		if (!XMLStringbuf_AppendCh(&RT->charsBuf, ')'))
			return Er_(parser, ERR_XMLP_MEMORY_ALLOC);
		if (cp->rep) {
			if (!XMLStringbuf_AppendCh(&RT->charsBuf, cp->rep))
				return Er_(parser, ERR_XMLP_MEMORY_ALLOC);
		}
	break;
	default: return ContentModel2ToString(parser, cp);
	}
	return 1;
}

/*
[45] elementdecl ::= '<!ELEMENT' S Name S contentspec S? '>'
[46] contentspec ::= 'EMPTY' | 'ANY' | Mixed | children
*/
static int ParseElementDecl(LPXMLPARSER parser)
{	
	int i;
	XMLCP *m;
	if (ParseNameDTD(parser, &i, NULL, &RT->dtd->sbuf, NULL, 0) == -1) 
		return 0;			
	if (SkipWS(parser) == -1) return 0;
	if (HANDLER(elementDecl)) {		
		if (!RT->cpNodesPool) {
			RT->cpNodesPool = XMLPool_Create(sizeof(XMLCP), 8);
			if (!RT->cpNodesPool) return Er_(parser, ERR_XMLP_MEMORY_ALLOC);
			if (!RT->cpNames) {
				RT->cpNames = XMLHTable_Create(RT->cpNames, 255);
				if (!RT->cpNames) return Er_(parser, ERR_XMLP_MEMORY_ALLOC);
			}
		}
		if (!(m = CpNew(parser))) return 0;	
	}
	else m = NULL;
	
	switch(TrieTok(parser, TRelTypes, 0)) {
		case 1:
			if (m) m->type = XMLCTYPE_EMPTY;
			break;
		case 2:
			if (m) m->type = XMLCTYPE_ANY;
			break;
		case 3:
			if (SkipWS(parser) == -1) return 0;
			if (CURCHAR=='#') {
				if (!ParseMixed(parser, m)) return 0;
			}
			else {
				if (!ParseChildren(parser, m)) return 0;
			}
			break;
		default: return (parser->ErrorCode) ? 0 : 
			Er_(parser, ERR_XMLP_INVALID_DECL, "ELEMENT");
	}
	if (!RequireCh(parser, '>', 1)) return 0;
	if (m) {
		if (PFLAG(XMLFLAG_REPORT_DTD_EXT)) {
			XMLCH *n = XMLHTable_Insert(RT->cpNames, RT->dtd->sbuf.str, parser->ErrorString);
			if (n != parser->ErrorString) return Er_(parser, ERR_XMLP_MEMORY_ALLOC);
			m->name = RT->cpNames->bucket;
			n = RT->cpNames->bucket->key;
			if (HANDLER(elementDecl)(parser->UserData, n, m)==XML_ABORT)
				return Er_(parser, ERR_XMLP_ABORT);
		}
		else {
			XMLStringbuf_SetLength(&RT->charsBuf, 0);
			if (!ContentModelToString(parser, m)) return 0;
			if (!XMLStringbuf_ToString(&RT->charsBuf))
				return Er_(parser, ERR_XMLP_MEMORY_ALLOC);
			if (RT->cpNodesPool->blocksAllocated > 23) {
				/* better destroy the pool before it grows too big. 
				   (block size is about 168 bytes) */			   
				XMLPool_FreePool(RT->cpNodesPool);
				RT->cpNodesPool = NULL;
			}
			if (HANDLER(elementDecl)(parser->UserData, 
				RT->dtd->sbuf.str, RT->charsBuf.str)==XML_ABORT)
					return Er_(parser, ERR_XMLP_ABORT);
		}
		
	}
	XMLStringbuf_SetLength(&RT->dtd->sbuf, 0);
	XMLStringbuf_SetLength(&RT->charsBuf, 0);
	return 1;
}

/*
[47] children    ::= (choice | seq) ('?' | '*' | '+')? 
[48] cp          ::= (Name | choice | seq) ('?' | '*' | '+')?       
[49] choice      ::= '(' S? cp ( S? '|' S? cp )+ S? ')'
[50] seq         ::= '(' S? cp ( S? ',' S? cp )* S? ')'
*/
static int ParseChildren(LPXMLPARSER parser, XMLCP *cp)
{	
	XMLCH sep[2] = {'\0','\0'};
	if (parser->ErrorCode) return 0;
	if (SkipWS(parser) == -1) return 0;
	if (!ParseCp(parser, cp)) return 0;
	if (SkipWS(parser) == -1) return 0;

	if(CURCHAR==')') {
		if (cp) cp->next = NULL;
		IPOS(1);
		if (SkipWS(parser)) return (parser->ErrorCode) ? 0 : 1;
		if (CURCHAR=='?'||CURCHAR=='*'||CURCHAR=='+') {
			if (cp) cp->rep = CURCHAR;
			IPOS(1);
		}
		return 1;
	}
	else if (CURCHAR=='|'||CURCHAR==',') {
		sep[0] = CURCHAR;
		IPOS(1);
	}
	else return ErP_(parser, ERR_XMLP_INVALID_TOKEN, 0);
	
	while(1) {
		if (SkipWS(parser) == -1) return 0;
		
		if (!ParseCp(parser, cp)) return 0;
		if (SkipWS(parser) == -1) return 0;
		if(CURCHAR==')') {
			if (cp) cp->next = NULL;
			IPOS(1);
			break;
		}
		else if (CURCHAR!=sep[0])
			return Er_(parser, ERR_XMLP_EXPECTED_TOKEN, sep);
		else {	
			IPOS(1);
		}
	}
	
	if (cp && sep[0]==',') 
		cp->type = XMLCTYPE_SEQ;
	if (SkipWS(parser)) return (parser->ErrorCode) ? 0 : 1;
	if (CURCHAR=='?'||CURCHAR=='*'||CURCHAR=='+') { /* no ws here */
		if (cp) cp->rep = CURCHAR;
		IPOS(1);
	}	
	return 1;
}

static int ParseCp(LPXMLPARSER parser, XMLCP *cp)
{		
	if (CURCHAR=='(') {
		IPOS(1);
		if (cp) {
			cp = CpNewChild(parser, cp); /* this can be "useless" parenthesis */
			if (!cp) return 0; 
		}
		if (!ParseChildren(parser, cp)) return 0;
	}
	else {
		int l;
		XMLStringbuf_SetLength(&RT->charsBuf, 0);
		if (ParseNameDTD(parser, &l, NULL, (cp)?&RT->charsBuf:NULL, ")|,?*+", 6) == -1) 
			return 0;
		if (cp) {
			cp->name = XMLHTable_Insert(RT->cpNames, RT->charsBuf.str, parser->ErrorString);
			if (cp->name != parser->ErrorString) return Er_(parser, ERR_XMLP_MEMORY_ALLOC);
			cp = CpNewChild(parser, cp);
			if (!cp) return 0;
			cp->name = RT->cpNames->bucket->key;
			cp->type = XMLCTYPE_NAME;
		}
		if (CURCHAR=='?'||CURCHAR=='*'||CURCHAR=='+') {
			if (cp) cp->rep = CURCHAR;
			IPOS(1);
		}
	}
	return 1;
}

/*
[51] Mixed ::= '(' S? '#PCDATA' (S? '|' S? Name)* S? ')*' | '(' S? '#PCDATA' S? ')'
*/
static int ParseMixed(LPXMLPARSER parser, XMLCP *cp)
{	
	int n=0, r;
	if (!Require(parser, "#PCDATA", 0)) return 0;
	if (SkipWS(parser) == -1) return 0;
	while (CURCHAR!=')') {
		if (!RequireCh(parser, '|', 1)) return 0;
		if (SkipWS(parser) == -1) return 0;
		XMLStringbuf_SetLength(&RT->charsBuf, 0);
		if (ParseNameDTD(parser, &n, NULL, (cp)?&RT->charsBuf:NULL, ")|", 2) == -1) 
			return 0;
		if (cp) {
			XMLCP *c = CpNewChild(parser, cp);
			if (!c) return 0;
			c->name = XMLHTable_Insert(RT->cpNames, RT->charsBuf.str, parser->ErrorString);
			if (c->name != parser->ErrorString) return Er_(parser, ERR_XMLP_MEMORY_ALLOC);
			c->name = RT->cpNames->bucket->key;
			c->type = XMLCTYPE_NAME;			
		}
		if (SkipWS(parser) == -1) return 0;
	}
	IPOS(1);
	if ((r = SkipWS(parser)) == -1) return 0;
	if (!r) {
		if (CURCHAR=='*') {
			IPOS(1);
		}
		else if (n) return Er_(parser, ERR_XMLP_EXPECTED_TOKEN, "*");		
	}
	if (cp) {
		cp->type = XMLCTYPE_MIXED;
		cp->next = NULL;
		if (n) cp->rep = '*';
	}
	return 1;
}

static XMLCH *ReadPERefEnd(LPXMLPARSER parser, int *chSize)
{	
	if (PREADERDATA->context & XMLREADERCTX_PE_SPACE) {
		/* return space: */
		PREADERDATA->context &= ~XMLREADERCTX_PE_SPACE;
		if (EINPUT(BufferedIStream_AppendBytes(PREADER, " ", 1))) 
			return (XMLCH*)NULL;
		*chSize = 1;
		IPOS(1);
		return PREADER->buf+PREADER->pos-1; 
	}
	else { /* this is inside literal i.e. RT->dtd->inLiteral
		   or after the XMLREADERCTX_PE_SPACE: */
		PREADERDATA->curEnt = XMLParser_GetCurrentEntity(parser);
		if (PREADERDATA->curEnt) PREADERDATA->curEnt->open = 0;

		if (PREADER->inputsrc == MemInputsrc) 
			free(PREADER->inputData);
		else {
			if (HANDLER(externalEntityParsed)) {
				if (HANDLER(externalEntityParsed)(parser->UserData, 
					PREADERDATA->curEnt, PREADER) == XML_ABORT)
						Er_(parser, ERR_XMLP_ABORT);
			}
		}
		return RestoreReaderPeRef(parser, chSize);	
	}
}

static XMLCH *RestoreReaderPeRef(LPXMLPARSER parser, int *chSize)
{
	free(PREADERDATA);
	BufferedIStream_Free(PREADER);
	XMLPool_Free(RT->dtd->pePool, PREADER);
	STACK_REMOVE(RT->dtd->peStack);		
	PREADER = (RT->dtd->peStack->length) ?
		(*((LPBUFFEREDISTREAM*)STACK_PEEK(RT->dtd->peStack))) : RT->dtd->refReader; 
	PREADERDATA->curEnt = XMLParser_GetCurrentEntity(parser);
	if (PREADERDATA->curEnt && PREADERDATA->curEnt->systemID) {
		RT->systemID = PREADERDATA->curEnt->systemID;
		RT->publicID = PREADERDATA->curEnt->publicID;
	}
	return (parser->ErrorCode) ? (XMLCH*)NULL : ReadCh(parser, chSize);
}

static XMLCH *ReadPERefStart(LPXMLPARSER parser, int *chSize)
{
	int n, l, i=1;
	XMLCH *s;
	DTDTOK_START(0);
	n = ParseNameTok(parser, &l, NULL, ";", &i);
	DTDTOK_END;
	if (n == -1) return (XMLCH*)NULL;

	if (!l) {
		/* S after %, must read S again for row increment etc. */
		DPOS_LF(1);
		return PREADER->buf+PREADER->pos-1; /* return % */
	}
	if (!i) {
		ErP_(parser, ERR_XMLP_WS_NOT_ALLOWED, 1);
		return (XMLCH*)NULL;
	}

	s = BUFTOSTR(PREADER->buf, n-1, n+l);
	
	if ((l = (int)XMLHTable_Lookup(RT->entitiesTable, s))) {

		LPXMLENTITY e = XMLVector_Get(RT->entities, l-1);
		LPBUFFEREDISTREAM *peReader;
		LPBISREADERDATA readerdata;

		if (e->open) {
			Er_(parser, ERR_XMLP_RECURSIVE_ENTITY_REF, s);
			return (XMLCH*)NULL;
		}

		peReader = STACK_PUSH(RT->dtd->peStack, NULL);
		if (!peReader) {
			Er_(parser, ERR_XMLP_MEMORY_ALLOC);
			return (XMLCH*)NULL;
		}
		if (!(*peReader = XMLPool_Alloc(RT->dtd->pePool))) {
			Er_(parser, ERR_XMLP_MEMORY_ALLOC);
			return (XMLCH*)NULL;
		}

		readerdata = InitEntityReader(parser, *peReader, BIS_DEFAULT_BLOCKSIZE,
			NULL, 0, e, l);
		if (!readerdata) return (XMLCH*)NULL;
		
		if (e->type == XML_ENTITY_INT_PARAM) {
			XMLMEMINPUTSRC *mementityinput = malloc(sizeof(XMLMEMINPUTSRC));
			if (mementityinput) {
				XMLMEMINPUTSRC_INIT(mementityinput, e->value, e->len);
				(*peReader)->inputData = mementityinput;
				(*peReader)->inputsrc = MemInputsrc;
			}
			else Er_(parser, ERR_XMLP_MEMORY_ALLOC);
		}
		else {
			if (HANDLER(resolveEntity)) {
				if (HANDLER(resolveEntity)(parser->UserData, e,
					*peReader) == XML_ABORT) 
						Er_(parser, ERR_XMLP_ABORT);
				else {
					if ((*peReader)->inputData) { 
						if (!(*peReader)->inputsrc)
							(*peReader)->inputsrc = RT->refReader->inputsrc;
						DetectEncoding(parser, 1);
					}
					else { /* skipped */
						return RestoreReaderPeRef(parser, chSize);
					}
				}
			}
			else {
				return RestoreReaderPeRef(parser, chSize);
			}
		}
		
		if (!parser->ErrorCode) {
			if (RT->dtd->inLiteral) /* inside string literal */
				return ReadCh(parser, chSize); 
				
			if (EINPUT(BufferedIStream_AppendBytes(PREADER, " ", 1))) 
				return (XMLCH*)NULL;
			readerdata->context |= XMLREADERCTX_PE_SPACE;
			IPOS(1);
			return PREADER->buf;
		}
		return (XMLCH*)NULL;
	}
	/* not found: */
	if (PFLAG(XMLFLAG_UNDEF_GENERAL_ENTITIES)) {
		Er_(parser, ERR_XMLP_UNDEF_ENTITY, s);
		return (XMLCH*)NULL;
	}
	if (!RT->dtd->inLiteral && HANDLER(skippedEntity)) {
		if (HANDLER(skippedEntity)(parser->UserData, s)==XML_ABORT) {
			Er_(parser, ERR_XMLP_ABORT);
			return (XMLCH*)NULL;
		}
	}
	return ReadCh(parser, chSize);
}

static int ParseAttlistDecl(LPXMLPARSER parser)
{
	LPXMLVECTOR *newAtts, en;
	if (!GetAttlistDecl(parser, &newAtts, &en)) {
		if (newAtts) DestroyDeclAttTableProc(NULL, newAtts, NULL);
		else if (en) XMLVector_Free(en);
		
		if (!parser->ErrorCode) Er_(parser, ERR_XMLP_INVALID_DECL, "ATTLIST");
		return 1;
	}
	return 1;
}

static int GetAttlistDecl(LPXMLPARSER parser, LPXMLVECTOR **newAtts, LPXMLVECTOR *en)
{	
	int i, unv;
	XMLATTDECL dAtt; 
	LPXMLVECTOR *oldAtts;
	LPXMLRUNTIMEATT pAtt = &RT->dtd->att; /* is used as temporary attribute 
		object for ParseString and ParseIntEntityAtt */
	*newAtts = NULL;
	*en = NULL;
	/* if there will be newAtts hanging around
	 on error return (0) it indicates that newAtts has been
	 allocated but not yet inserted into hashtable; must
	 call DestroyDeclAttTableProc(NULL, newAtts, NULL); at higher level,
	 same for en which is optionally created in ParseEnumeration 
	(XMLFLAG_REPORT_DTD_EXT)*/
	if (ParseNameDTD(parser, &dAtt.nameLen, NULL, 
		&RT->dtd->sbuf, ">", 1) == -1) return 0;	
	oldAtts = (RT->declAttTable) ? 
		XMLHTable_Lookup(RT->declAttTable, RT->dtd->sbuf.str) : NULL;	
	if (SkipWS(parser) == -1) return 0;

	while (CURCHAR!='>') {
		dAtt.name = dAtt.value = NULL;
		dAtt.pExt = NULL;		
		if (ParseNameDTD(parser, &dAtt.nameLen, &dAtt.prefixLen,
			&pAtt->nameBuf, NULL, 0) == -1) return 0;
		
		if (SkipWS(parser) == -1) return 0;
		if (CURCHAR == '(') {
			dAtt.type = XMLATTDECL_TYPE_ENUMERATED;
			IPOS(1);
			if (!ParseEnumeration(parser, en)) return 0;
			if (!REQUIRE_WS(i)) return 0;
		}
		else {
			dAtt.type = TrieTok(parser, TRattType, 1);		
			if (dAtt.type < 1) return 0;
			if (dAtt.type == XMLATTDECL_TYPE_NOTATION) {
				if (!RequireCh(parser, '(', 1)) return 0;
				if (!ParseEnumeration(parser, en)) return 0;
				if (!REQUIRE_WS(i)) return 0;
			}
		}
			
		dAtt.defaultDecl = 0;
		if (CURCHAR == '#') {
			dAtt.defaultDecl = TrieTok(parser, TRdefDecls, 0);
			if (dAtt.defaultDecl < 1) return 0;
			if ((i = SkipWS(parser)) == -1) return 0;
			if (!i && CURCHAR!='>') return ErP_(parser, ERR_XMLP_WS_REQUIRED, 0);
		}

		if (dAtt.defaultDecl < XMLATTDECL_DEF_REQUIRED) {
			pAtt->qname = pAtt->nameBuf.str; /* for ParseIntEntityAtt err handling */
			RT->dtd->inLiteral = 1;
			if (!(dAtt.value = ParseString(parser, &pAtt->valBuf, pAtt,
				(dAtt.type == XMLATTDECL_TYPE_CDATA) ? 1 : 2, 2))) return 0;
		}
		
		unv = (dAtt.value || dAtt.type != XMLATTDECL_TYPE_CDATA); /* useful
			for non-validating mode? */

		if (unv || PFLAG(XMLFLAG_REPORT_DTD_EXT)) {
			/* dAtt is stored into RT->declAttTable if it attribute
			  declaration contains default value OR its type is other
			  than CDATA (in this case it's stored for normalization rules) 
			  Changed for 0.9.0: RT->declAttTable contains pointers to
			  2 item array of LPXMLVECTOR, 1st one is for default attribute
			  processing and normalization, 2nd is greated when
			  XMLFLAG_REPORT_DTD_EXT is true - otherwise n[1] is NULL.
			  Note that 1st vector length can be 0 when XMLFLAG_REPORT_DTD_EXT
			  is set. See GetDefAtts and DestroyDeclAttTable.
			*/
			if (!oldAtts && !*newAtts) {
				LPXMLVECTOR *n = malloc(2*sizeof(LPXMLVECTOR));
				if (!n) return Er_(parser, ERR_XMLP_MEMORY_ALLOC);
				if (!XMLVector_Create(n, 2, sizeof(XMLATTDECL))) {
					free(n);
					return Er_(parser, ERR_XMLP_MEMORY_ALLOC);
				}
				n[1] = NULL;
				*newAtts = oldAtts = n;
				if (PFLAG(XMLFLAG_REPORT_DTD_EXT)) {
					if (!XMLVector_Create(n+1, 2, sizeof(XMLATTDECL)))
					return Er_(parser, ERR_XMLP_MEMORY_ALLOC);
				}
				 
			}
			dAtt.valueLen = pAtt->valBuf.len;

			dAtt.name = xmlMemdup(pAtt->nameBuf.str, pAtt->nameBuf.len);
			if (!dAtt.name) return Er_(parser, ERR_XMLP_MEMORY_ALLOC);					
			if (dAtt.value) {
				dAtt.value = xmlMemdup(pAtt->valBuf.str, dAtt.valueLen+1);
				if (!dAtt.value) {
					free(dAtt.name);
					return Er_(parser, ERR_XMLP_MEMORY_ALLOC);
				}
			}
		
			/* note that we store possible duplicate declarations here too,
			they won't have much effect in ParseAttributes though*/
			if (unv && !XMLVector_Append(*oldAtts, &dAtt))
				return Er_(parser, ERR_XMLP_MEMORY_ALLOC);
			if (PFLAG(XMLFLAG_REPORT_DTD_EXT)) {
				if (*en) {
					int i, j; /* construct enumeratedType atts from */
								/* LPXMLVECTOR collected in ParseEnumeration */
					void **p = XMLVector_Get(*en, 0);
					XMLCH *s = xmlMemdup(RT->charsBuf.str+1, RT->charsBuf.len-1);
					dAtt.pExt = *en;  
					if (!s) return Er_(parser, ERR_XMLP_MEMORY_ALLOC);
					for (i=0; i<(*en)->length; i++, p = XMLVector_Get(*en, i)) {
						j = (int)*p;
						*p = s;
						s[j] = 0;
						s += (j+1);
					}
					*en = NULL;
				}
				if (!XMLVector_Append(*(oldAtts+1), &dAtt))
					return Er_(parser, ERR_XMLP_MEMORY_ALLOC);
			}
		}

		if (SkipWS(parser) == -1) return 0;
		
		if (HANDLER(attributeDecl)) {
			if (HANDLER(attributeDecl)(parser->UserData, RT->dtd->sbuf.str, pAtt->nameBuf.str, dAtt.type, 
				((dAtt.type == XMLATTDECL_TYPE_ENUMERATED || 
					dAtt.type == XMLATTDECL_TYPE_NOTATION) ? RT->charsBuf.str : NULL),
					dAtt.defaultDecl, dAtt.value)==XML_ABORT)
						return Er_(parser, ERR_XMLP_ABORT);
		}

		XMLStringbuf_SetLength(&pAtt->nameBuf, 0);
		XMLStringbuf_SetLength(&pAtt->valBuf, 0);
		if (dAtt.type > XMLATTDECL_TYPE_NMTOKENS)
			XMLStringbuf_SetLength(&RT->charsBuf, 0);
	}

	IPOS(1);
	if (*newAtts) {
		if (!RT->declAttTable) {
			RT->declAttTable = XMLHTable_Create(RT->declAttTable, 64);
			if (!RT->declAttTable) return Er_(parser, ERR_XMLP_MEMORY_ALLOC);
		}
		if (!XMLHTable_Insert(RT->declAttTable, RT->dtd->sbuf.str, *newAtts))
			return Er_(parser, ERR_XMLP_MEMORY_ALLOC);
	}
	
	XMLStringbuf_SetLength(&RT->dtd->sbuf, 0);
	XMLStringbuf_SetLength(&pAtt->nameBuf, 0);
	XMLStringbuf_SetLength(&pAtt->valBuf, 0);
	XMLStringbuf_SetLength(&RT->charsBuf, 0);
	return 1;
}

/*===========================================================================
  FUNCTION
    DestroyDeclAttTableProc
  DESC
    enumeration callback for destroying DTD declared attributes hashtable
  NOTE
    This is also used for deallocating vector only when key and
	userdata (hashtable pointer) params are NULL
=================================c==========================================*/
static int DestroyDeclAttTableProc(char *key, void *data, void *userdata)
{	
    LPXMLVECTOR *atts = (LPXMLVECTOR*)data;
	LPXMLVECTOR v = (*(atts+1)) ? *(atts+1) : *atts;
	LPXMLATTDECL a, pEnd;
	pEnd = (LPXMLATTDECL)_XMLVector_GetIterP(v, a);
	for (; a!=pEnd; a++) {
		free(a->name);
		if (a->value) free(a->value);
		if (a->pExt) { /* currently enumeratedType only: */
			void **s = XMLVector_Get((LPXMLVECTOR)a->pExt, 0);
			if (*s) free(*s);
			XMLVector_Free((LPXMLVECTOR)a->pExt);
		}
	}
	XMLVector_Free(v);
	if (v != *atts) XMLVector_Free(*atts);
	if (userdata) XMLHTable_Remove((LPXMLHTABLE)userdata, key);
	free(atts);
	return 0;
}

static int GetEntityDecl(LPXMLPARSER parser, LPXMLENTITY e, LPXMLSTRINGBUF sbuf)
{
	int i, isParam, iPublicID, iSystemID, iNotation;
	if ((i = TryTok(parser, "%", 1)) < -1) return 0;
	isParam = (i == -1) ? 0 : 1;
	if (isParam && !XMLStringbuf_AppendCh(sbuf, '%'))
		return Er_(parser, ERR_XMLP_MEMORY_ALLOC);
	if (ParseNameDTD(parser, &i, NULL, sbuf, NULL, 0) == -1) 
		return 0;
	if (SkipWS(parser) == -1) return 0;

	if (ISQUOTE(CURCHAR)) {
		e->type = (isParam) ? XML_ENTITY_INT_PARAM : XML_ENTITY_INT_GEN;
		i = sbuf->len;
		if (!ParseString(parser, sbuf, NULL, 0, 1)) return 0;
		if (!RequireCh(parser, '>', 1)) return 0;
		if (!(e->name = xmlMemdup(sbuf->str, sbuf->len+1)))
			return Er_(parser, ERR_XMLP_MEMORY_ALLOC);
		e->value = e->name+i;
		e->len = sbuf->len-i;
		return 1;
	}
	
	iPublicID = iSystemID = iNotation = -1;
	if (!ParseExternalID(parser, sbuf, &iPublicID, &iSystemID, 0)) return 0;
	if ((i = SkipWS(parser)) == -1) return 0;

	if (CURCHAR != '>') {
		e->type = XML_ENTITY_UNPARSED;
		if (isParam)
			return Er_(parser, ERR_XMLP_EXPECTED_TOKEN, ">");

		if (!i) return ErP_(parser, ERR_XMLP_WS_REQUIRED, 0);
		if (!Require(parser, "NDATA", 1)) return 0;	
		iNotation = ++sbuf->len;
		i = ParseNameDTD(parser, &i, NULL, sbuf, ">", 1);
		if (i == -1) return 0;
		if (CURCHAR != '>' && !RequireCh(parser, '>', 1)) return 0;
	}
	else {
		e->type = (isParam) ? XML_ENTITY_EXT_PARAM : XML_ENTITY_EXT_GEN;	
	}
	
	IPOS(1);
	if (!(e->name = xmlMemdup(sbuf->str, sbuf->len+1)))
		return Er_(parser, ERR_XMLP_MEMORY_ALLOC);
	
	if (iPublicID != -1) e->publicID = e->name + iPublicID;
	if (iSystemID != -1) e->systemID = e->name + iSystemID;
	if (iNotation != -1) e->notation = e->name + iNotation;
	return 1;
}

static int ParseEntityDecl(LPXMLPARSER parser)
{	
	XMLENTITY e;
	e.type = e.len = e.open = 0;
	e.name = e.notation = e.publicID = e.systemID = e.value = (XMLCH*)NULL;

	if (!GetEntityDecl(parser, &e, &RT->charsBuf)) return 0;
	XMLStringbuf_SetLength(&RT->charsBuf, 0);
	
	if (HANDLER(entityDecl)) {
		if (HANDLER(entityDecl)(parser->UserData, &e)==XML_ABORT) {
			if (e.name) free(e.name);
			return Er_(parser, ERR_XMLP_ABORT);
		}
	}

	if (XMLHTable_Lookup(RT->entitiesTable, e.name)) {
		if (e.name) free(e.name);
	}	
	else {
		/* append entity into RT->entities vector and
		into RT->entitiesTable hashtable: */
		if (!(XMLVector_Append(RT->entities, &e)))
			return Er_(parser, ERR_XMLP_MEMORY_ALLOC);
		if (!(XMLHTable_Insert(RT->entitiesTable, e.name,
			(void*)RT->entities->length)))
			return Er_(parser, ERR_XMLP_MEMORY_ALLOC);
	}
	return 1;
}

/*===========================================================================
  FUNCTION
    ParseEntityContent
  DESC
    Prepares new reader and parses entity in content
  PARAMS
    parser      this
    e           entity to be parsed
  NOTES
    Every parsed entity needs its own entityReader 'cos calls might be
    recursive. There must also be tmpReader that saves PREADER and restores it
    after ParseInput call.
  TODO
    Using/initializing XMLMEMINPUTSRC for internal entities isn't the
    most robust approach but I guess perfomance hit isn't that devastating
    unless document contains huge number of internal entities... One
    approach might be "polymorphism" for ReadCh that it would also read
    straight from e->value memory buffer.
===========================================================================*/
static void ParseEntityContent(LPXMLPARSER parser, LPXMLENTITY e)
{
	BUFFEREDISTREAM entityReader; /* these are */
	BISREADERDATA readerdata; /* kept in local */
	LPBUFFEREDISTREAM tmpReader = PREADER; /* stack while parsing entity */

	if (parser->ErrorCode) return;

	if (HANDLER(startEntity)) {
		if (HANDLER(startEntity)(parser->UserData, e) == XML_ABORT) {
			Er_(parser, ERR_XMLP_ABORT);
			return;
		}
	}

	InitEntityReader(parser, &entityReader, BIS_DEFAULT_BLOCKSIZE,
		&readerdata, RT->tagstack->length, e, 0);

	if (e->type == XML_ENTITY_INT_GEN) {
		/* parse internal general entities from XMLMEMINPUTSRC: */
		XMLMEMINPUTSRC mementityinput;
		XMLMEMINPUTSRC_INIT(&mementityinput, e->value, e->len);
		entityReader.inputData = &mementityinput;
		entityReader.inputsrc = MemInputsrc;
		ParseInput(parser);
	}
	else /* if (e->type == XML_ENTITY_EXT_GEN)*/ {
		/* parse external general entities from user inputsrc: */
		if (HANDLER(resolveEntity)) {
			if (HANDLER(resolveEntity)(parser->UserData, e,
				&entityReader) == XML_ABORT) {
					Er_(parser, ERR_XMLP_ABORT);
			}
			else {
				if (entityReader.inputData) { /* parse it: */
					/* if inputsrc is NULL, using parser's main inputsrc */
					if (!entityReader.inputsrc)
						entityReader.inputsrc = RT->refReader->inputsrc;
					if (DetectEncoding(parser, 1))
						ParseInput(parser);
			
					if (HANDLER(externalEntityParsed)) {
						/* give user chance to do the cleaning up: */
						if (HANDLER(externalEntityParsed)(parser->UserData, e,
							&entityReader) == XML_ABORT)
								Er_(parser, ERR_XMLP_ABORT);
					}
				}
			}
		}
		if (tmpReader != RT->refReader) {
			RT->systemID = ((LPBISREADERDATA)tmpReader->userdata)->curEnt->systemID;
			RT->publicID = ((LPBISREADERDATA)tmpReader->userdata)->curEnt->publicID;
		}
	}

	if (!parser->ErrorCode) {
		if (RT->tagstack->length > PREADERDATA->stackLevel) {
			LPXMLRUNTIMETAG pTag = STACK_PEEK(RT->tagstack);
			Er_(parser, ERR_XMLP_UNCLOSED_TAG, pTag->qname);
		}
		else {
			if (HANDLER(endEntity)) {
				if (HANDLER(endEntity)(parser->UserData, e) == XML_ABORT)
					Er_(parser, ERR_XMLP_ABORT);
			}
		}
	}

	e->open = 0;
	BufferedIStream_Free(&entityReader);
	PREADER = tmpReader; /* restore parser's previous reader */
}

/* inputsrc for ParseEntityContent() internal entities: */
static int MemInputsrc(BYTE *buf, int cBytes, int *cBytesActual, void *inputData)
{
	XMLMEMINPUTSRC_HANDLE
}

/*===========================================================================
  FUNCTION
    ParseIntEntityAtt
  DESC
    Parses entities in attribute buffer
  PARAMS
    parser      this
    e           entity to be parsed
    pAtt        attribute for buffer manipulation
  NOTES
    Possibly calls itself recursively (thru ParseEntityRef)
    and appends data to pAtt's valBuf.
===========================================================================*/
static void ParseIntEntityAtt(LPXMLPARSER parser, LPXMLENTITY e,
							  LPXMLRUNTIMEATT pAtt)
{
	int chSize;
	XMLCH *c;
	XMLMEMINPUTSRC mementityinput;	
	BUFFEREDISTREAM entityReader;
	BISREADERDATA readerdata;
	LPBUFFEREDISTREAM tmpReader;

	if (parser->ErrorCode) return;

	if (e->type != XML_ENTITY_INT_GEN) {
		Er_(parser, ERR_XMLP_INVALID_ATT_VALUE, pAtt->qname);
		return;
	}

	XMLMEMINPUTSRC_INIT(&mementityinput, e->value, e->len);
	tmpReader = PREADER;
	InitEntityReader(parser, &entityReader, e->len, &readerdata, 0, e, 0);
	entityReader.inputData = &mementityinput;
	entityReader.inputsrc = MemInputsrc;

	while((c = ReadCh(parser, &chSize))) {
		if (chSize == 1) {
			if (isspace(*c)) {
				if (!(XMLStringbuf_AppendCh(&pAtt->valBuf, ' '))) {
					Er_(parser, ERR_XMLP_MEMORY_ALLOC);
					break;
				}
			}
			else if (*c == '&') {
				if (!(ParseEntityRef(parser, &pAtt->valBuf, pAtt,  2, NULL))) break;
			}
			else if (*c == '<') {
				ErP_(parser, ERR_XMLP_INVALID_TOKEN, 1);
				break;
			}
			else {
				if (!(XMLStringbuf_AppendCh(&pAtt->valBuf, *c))) {
					Er_(parser, ERR_XMLP_MEMORY_ALLOC);
					break;
				}
			}
		}
		else if (!(XMLStringbuf_Append(&pAtt->valBuf, c, chSize))) {
			Er_(parser, ERR_XMLP_MEMORY_ALLOC);
			break;
		}
	}
	e->open = 0;
	BufferedIStream_Free(&entityReader);
	PREADER = tmpReader;
}

#endif /* DTD_SUPPORT */


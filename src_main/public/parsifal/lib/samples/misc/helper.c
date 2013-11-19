/*
   function: UTF8BufToLatin1
   desc: converts buffer from UTF-8 to iso latin1
   note: modifies the actual buffer, doesn't nul terminate
   params:
     buf - buffer to be converted
     end - NULL if buf is nul terminated, buf+buflen otherwise
   returns: 
     new length of buffer (shorter or equal to passed buf len)
   note: NONLAT is character to be inserted when UTF8 char is > 255
     It is possible to modify the actual buffer in Parsifal characters
     handler i.e. 
     cbChars = UTF8BufToLatin1((XMLCH*)chars, (XMLCH*)chars+cbChars);
     but it's recommended to do the conversion using copy buffer
*/
#define NONLAT '?'
#define UTF8LEN(c) \
(!(c & 0x80)) ? 1 : \
((unsigned int)c <= 0xdf) ? 2 : \
((unsigned int)c <= 0xef) ? 3 : \
((unsigned int)c <= 0xf7) ? 4 : \
((unsigned int)c <= 0xfb) ? 5 : 6

size_t UTF8BufToLatin1(XMLCH *buf, XMLCH *end)
{	
	int c, len;
	XMLCH *o, *l;
	o = l = buf;
	for (len=0; *buf && buf!=end; buf+=len, o++) {
		len = UTF8LEN(*buf);
		if (len==1) *o = *buf;
		else if (len==2) {
			c = (*buf & 0x1F) << 6;
			c |= buf[1] & 0x3F;
			*o = (c>0xFF) ? NONLAT : (XMLCH)c;
		}
		else *o = NONLAT;
	}
	return o-l;
}

/*
   function: GetBaseDir
   desc: gets base directory from src 
   params:
     dst - destination buffer (must have space for directory FILENAME_MAX?)
     src - source usually pointer to some file/uri path
   returns: 
     index of file/resource in src for example if src is "http://myorg/index.xml"
     then returns 13 (src+13 points to index.xml)
     if there is no basedir, returns 0 (and dst will be empty string)
*/
size_t GetBaseDir(XMLCH *dst, XMLCH *src)
{
	XMLCH *s = strrchr(src, '/');	
#ifdef _WIN32
	if (!s) s = strrchr(src, '\\');
#endif
	if (s) {
		size_t i = (s-src)+1;
		memcpy(dst, src, i);
		dst[i] = '\0';
		return i;
	}
	dst[0] = '\0';
	return 0;
}

/*
   function: ResolveBaseUri
   desc: Can be used to resolve systemID in resolveEntityHandler
   params:
     parser - parser object
     systemID - usually entity->systemID from resolveEntityHandler
     base - usually base dir from GetBaseDir call (which is 
       called before parsing has been started)
   returns: 
     pointer to baseuri, i.e. the usage in resolveEntityHandler:
     XMLCH r[FILENAME_MAX]; 
     XMLCH *uri = ResolveBaseUri(parser, entity->systemID, docbasedir);
     if (uri != entity->systemID) {
         strcpy(r, uri);
         uri = strcat(r, entity->systemID);
     }
     ... open the resource here i.e. fopen(r) ...
   notes:
     Real accurate base uri handling needs of course somekind of stack,
     however ResolveBaseUri kind of approach should be sufficient for
     most cases
*/
XMLCH *ResolveBaseUri(LPXMLPARSER parser, XMLCH *systemID, XMLCH *base)
{
	XMLCH *s=systemID;
	for (; *s; s++) {
		if (*s == ':') return systemID; /* probably absolute */ 
		if (*s == '/' || *s == '\\') break;
	}
	s = XMLParser_GetPrefixMapping(parser, "xml:base");
	return (s) ? s : base;
}

/*
   function: GetFormattedContext
   desc: Builds formatted line info, example when called from errorHandler
   and document contains invalid token (< unescaped in attribute value):
   <e a="va<l"></e>
   --------^
   params:
     col (output) - column pos in bytes (unlike parser->ErrorColumn for 
     example which is column in characters) OR column pos in characters if 
     outputUTF8 is True.
     outputUTF8 - True: use character column value for output (output can
     show UTF-8 characters) - uses parser->ErrorColumn/dtd->ErrorColumn if
     parser->ErrorCode has been set.
     False: Result will be shown in bytes (output doesn't show UTF-8)
   returns: 
     Pointer to allocated format string - must be freed by the caller
     NULL if string can't be build/allocated.
   note: XMLParser_GetContextBytes data must be handled in a
     special way like this 'cos it might contain NULs and tabs or it
     can be all whitespace or input buffer might be too short for meaningful
     visualization. Note also that buffer might be truncated with NUL right 
     after column position (col) because parsifal doesn't ensure that input
     buffer contains full lines.
   usage:
     int col;
     XMLCH *s = GetFormattedContext(parser, &col, 0);
     if (s) { 
        printf("Context: %s\n", s);
        free(s);
     }
*/
XMLCH *GetFormattedContext(LPXMLPARSER parser, int *col, int outputUTF8)
{
	int i, w, colb, cBytes;
	XMLCH *s, *r;

	if ((colb = XMLParser_GetContextBytes(parser, &s, &cBytes)) < 0) 
		return NULL;

	for (i=0, w=1; i!=cBytes; i++) {
		if (!s[i] || s[i]==0xD || s[i]==0xA) break;
		if (w && !isspace(s[i])) w = 0; /* all-whitespace flag */
	}
	
	if (i < colb || i > 255 || w) return NULL;
	
	if (outputUTF8) {
		if (parser->ErrorCode) {
			*col = (parser->ErrorCode == ERR_XMLP_VALIDATION) ? 
				((LPXMLDTDVALIDATOR)parser->UserData)->ErrorColumn :
				parser->ErrorColumn;	
		}
		else *col = XMLParser_GetCurrentColumn(parser);
		if (*col==-1) return NULL;
		(*col)--;
	}
	else *col = colb;

	/* alloc row len + size for \n, arrow and NUL: */
	if (!(r = malloc(i + *col + 3))) return NULL; 
	memcpy(r, s, i);
	
	for (w=0; w<colb; w++) { /* remove tabs */
		if (r[w]==0x9) r[w]=0x20;
	}
	
	r[i++]='\n';
	if (*col) {
		memset(r+i, '-', *col);
		i+=*col;
	}
	r[i]='^';
	r[++i]=0;
	(*col)++; /* column is always 1-based */
	return r;
}
/*===========================================================================
  xmlsbuf.c
    This module contains simple StringBuffer for Parsifal XML Parser
    see parsifal.h for copyright info

  USAGE
    _Init buffer and _Append strings to it. when you're done, call:
    XMLCH *mystr = XMLStringbuf_ToString(&sb);
    if you want to keep the string and reuse StringBuf: call _Init again 
    (after that you must free mystr yourself of course) or
    call _SetLength (see _SetLength comment). 
    NOTE: you might want to use strdup(XMLStringbuf_ToString(&sb))
    to ensure that you get yourself string from local heap.
    All functions return sbuf->str or NULL on failure (except _SetLength
    which returns 0 on failure).

    You can use XMLPool for more efficient allocation of fixed length strings
    e.g. first allocate pool: xpool = XMLPool_Create(128, 8) and then
    init StringBuf(s) by calling _InitUsePool(..., xpool) remember to call
    XMLPool_PoolFree when you're done. See XMLPool.c
===========================================================================*/

#include <stdlib.h>
#include <string.h>
#include "xmlsbuf.h"
#include "xmlpool.h"
#include "xmlcfg.h"

#define GROWSBUF(l) \
if (sbuf->usePool) { \
  if ((sbuf->len + (l)) > sbuf->pool->itemSize) { \
    XMLCH *ts = sbuf->str; \
    sbuf->usePool = 0; \
    sbuf->capacity = COUNTBUFSIZE((sbuf->len + (l)), sbuf->blocksize); \
    sbuf->str = (XMLCH*)malloc(sbuf->capacity * sizeof(XMLCH)); \
    if (!sbuf->str) return ((XMLCH*)NULL); \
    memcpy(sbuf->str, ts, sbuf->len); \
    XMLPool_Free(sbuf->pool, ts); \
  } \
} \
else if ((sbuf->len + (l)) > sbuf->capacity) { \
  sbuf->capacity = COUNTBUFSIZE((sbuf->len + (l)), sbuf->blocksize); \
  sbuf->str = (XMLCH*)realloc(sbuf->str, sbuf->capacity * sizeof(XMLCH)); \
  if (!sbuf->str) return ((XMLCH*)NULL); \
}

XMLCH XMLAPI *XMLStringbuf_Append(LPXMLSTRINGBUF sbuf, XMLCH *str, int len)
{	
	GROWSBUF(len);
	if (len == 1) /* gives a slight performance gain */
		sbuf->str[sbuf->len++] = *str;
	else {
		memcpy(sbuf->str+sbuf->len, str, len);
		sbuf->len += len;
	}
	return (sbuf->str);
}

XMLCH XMLAPI *XMLStringbuf_AppendCh(LPXMLSTRINGBUF sbuf, XMLCH c)
{	
	GROWSBUF(1);
	sbuf->str[sbuf->len] = c;
	sbuf->len++;
	return (sbuf->str);
}

XMLCH XMLAPI *XMLStringbuf_Init(LPXMLSTRINGBUF sbuf, int blockSize, int initSize)
{	
	sbuf->len = sbuf->usePool = 0;
	sbuf->blocksize = blockSize;
	sbuf->pool = (LPXMLPOOL)NULL;
	sbuf->capacity = initSize;
	sbuf->str = (initSize) ? 
		(XMLCH*)malloc(sbuf->capacity * sizeof(XMLCH)) : (XMLCH*)NULL;
	return sbuf->str;
}

XMLCH XMLAPI *XMLStringbuf_InitUsePool(LPXMLSTRINGBUF sbuf, int blockSize, int initSize, LPXMLPOOL pool)
{
	if (initSize > pool->itemSize) 
		return XMLStringbuf_Init(sbuf, blockSize, initSize);
	
	sbuf->pool = pool;
	sbuf->usePool = 1;
	sbuf->len = sbuf->capacity = 0;
	sbuf->blocksize = blockSize;
	return (sbuf->str = XMLPool_Alloc(pool));
}

void XMLAPI XMLStringbuf_Free(LPXMLSTRINGBUF sbuf)
{	
	sbuf->len = sbuf->capacity = 0;
	if (!sbuf->str) return;
	if (sbuf->usePool) 
		XMLPool_Free(sbuf->pool, sbuf->str);
	else 
		if (sbuf->str) free(sbuf->str);
	sbuf->str = (XMLCH*)NULL;
}

/* SetLength can be used to truncate buffer to certain length or to
   initialize buffer to be reused (setting length to 0).
  _SetLength adjusts stringbuffer capacity unlike setting sbuf.len member directly.
  note: SetLength returns TRUE on success, FALSE on failure (mem alloc problem) */
int XMLAPI XMLStringbuf_SetLength(LPXMLSTRINGBUF sbuf, int len)
{	
	if (sbuf->usePool) {
		if (len > sbuf->pool->itemSize) return 0;
		sbuf->len = len;
	}
	else if (sbuf->str && len != sbuf->len) {
		sbuf->len = len;
		len = (len) ? (COUNTBUFSIZE(len, sbuf->blocksize)) : sbuf->blocksize;
		if (len != sbuf->capacity) {
			sbuf->capacity = len;
			sbuf->str = (XMLCH*)realloc(sbuf->str, sbuf->capacity * sizeof(XMLCH));
			return ((sbuf->str) ? -1 : 0);
		}
	}
	return -1;
}

XMLCH XMLAPI *XMLStringbuf_ToString(LPXMLSTRINGBUF sbuf)
{	
	GROWSBUF(1);
	if (sbuf->str[sbuf->len]) sbuf->str[sbuf->len] = '\0';
	return (sbuf->str);
}




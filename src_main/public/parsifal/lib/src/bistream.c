/*===========================================================================
  bistream.c
    This module contains BufferedIStream (buffered inputstream reader)
    see parsifal.h for copyright info
===========================================================================*/

#include <stdlib.h>
#include <string.h>
#include "bistream.h"
#include "xmlcfg.h"

#ifdef _MSC_VER
#pragma intrinsic(memcmp, memcpy, memset, strcmp, strlen)
#endif

static int EncodeInbuf(LPBUFFEREDISTREAM r, int cBytes);

#define BISFIXBUF(size) \
if (r->buf == (BYTE*)NULL) { \
    r->bytesavail = 0; \
    r->bufsize = r->blocksize*2; \
    if ((r->buf = (BYTE*)malloc(r->bufsize*sizeof(BYTE))) == NULL) { \
        r->bufsize = 0; \
		return BIS_ERR_MEMALLOC; \
	} \
} \
else if ((NewBufSize = COUNTBUFSIZE((size), r->blocksize))!= r->bufsize) { \
    if (NewBufSize > r->maxbufsize) return BIS_ERR_MAXBUF; \
    if ((r->buf = (BYTE*)realloc(r->buf, NewBufSize*sizeof(BYTE))) == NULL) { \
        r->bytesavail = 0; \
        return BIS_ERR_MEMALLOC; \
    } \
    r->bufsize = NewBufSize; \
}

/* BIS_MEMNEQ macro gives us about 20% performance increase!
   tests memory buffer for non-equality */
#define BIS_MEMNEQ(b1,b2,l) \
( ((l)==1) ? (*(b1)!=*(b2)) : (memcmp((b1),(b2),(l))) )

#define HANDLE_ENCODE \
l = EncodeInbuf(r, cBytesActual); \
if (l < 0) return (r->err=l); \
r->bytesavail += l;

/*===========================================================================
  FUNCTION
    BufferedIStream_EncodeBuffer
  DESC
    Encodes current reader buffer 
  PARAMS
    r           this
  NOTES
    _Peek handles buffer encoding (if LPFNENCODE is specified)
    so this isn't used very often but can be used for "late encoding"
    i.e. you read data without encoding it and then encode only
    some of it.
===========================================================================*/
int BufferedIStream_EncodeBuffer(LPBUFFEREDISTREAM r)
{	
	if (!r->bytesavail) return 0;
	if (r->encode) {
		int l, cBytesActual = r->bytesavail;
		
		r->bytesavail = 0;
		if (r->inbuf) return BIS_ERR_ENCODING;
		
		/* we need to put r->buf contents into inbuf and encode it,
			note that there might be more bytes in r->buf 
			than normal inbuf size which is r->blocksize+BIS_CHAR_MAX */
		l = (cBytesActual < r->blocksize) ? r->blocksize : cBytesActual;	
		r->inbuf = (BYTE*)malloc((l+BIS_CHAR_MAX)*sizeof(BYTE));
		if (!r->inbuf) return BIS_ERR_MEMALLOC;
		memcpy(r->inbuf+BIS_CHAR_MAX, r->buf, cBytesActual);
		
		HANDLE_ENCODE;

		if (cBytesActual > r->blocksize) { /* shrink inbuf if needed: */
			r->inbuf = (BYTE*)realloc(r->inbuf, (r->blocksize+BIS_CHAR_MAX)*sizeof(BYTE));
			if (!r->inbuf) return BIS_ERR_MEMALLOC;
		}
		return 0; /* success */
	}
	return BIS_ERR_ENCODING;
}

/*===========================================================================
  FUNCTION
    BufferedIStream_Peek
  DESC
    Peeks current buffer position
  PARAMS
    r           this
    tok         token to search (or NULL if you want to ensure len bytes)
    len         length of token
    offset      bytes to skip at current posision
 RETURNS
    0           token found or len bytes ensured
    -1          token not found
    < -1        some error (see BISTREAM.H)
===========================================================================*/
int BufferedIStream_Peek(LPBUFFEREDISTREAM r, 
						 const BYTE *tok, 
						 int len, 
						 int offset)
{	
	int pos;
	
	if (r->pos < 0) return r->pos;
	if ((pos = r->pos + offset) < 0) 
		return (r->err=BIS_ERR_INVALIDARG);

	if (r->buf == NULL || (pos + len) > r->bytesavail) {
		int cBytesActual, l;
		if (r->eof) return -1;

		if (r->encode) {
			if (!r->inbuf) {
				r->inbuf = (BYTE*)malloc((r->blocksize+BIS_CHAR_MAX)*sizeof(BYTE));
				if (!r->inbuf) return BIS_ERR_MEMALLOC;
			}

			r->eof = r->inputsrc(r->inbuf+BIS_CHAR_MAX, r->blocksize, 
				&cBytesActual, r->inputData);
			if (cBytesActual) {
				HANDLE_ENCODE;
			}
		}
		else {
			int NewBufSize;
			BISFIXBUF(r->bytesavail+r->blocksize);
			r->eof = r->inputsrc(r->buf+r->bytesavail, r->blocksize, 
				&cBytesActual, r->inputData);
			r->bytesavail+=cBytesActual;
		}
		
		if (r->eof == BIS_ERR_INPUT)
			return (r->err = BIS_ERR_INPUT);

		if ((pos + len) > r->bytesavail) {
			/* this is important: if we're in eof and there's
			   partial character in r->inbuf, return encoding error: */
			if (r->eof && !cBytesActual && r->inbufrest)
				return BIS_ERR_ENCODING;
			return -1;
		}
	}
	return ((tok) ? (BIS_MEMNEQ(r->buf + pos, tok, len) ? -1 : 0) : 0);
}

int BufferedIStream_AppendBytes(LPBUFFEREDISTREAM r, const BYTE *bytes, int len)
{
	if (r->bytesavail + len > r->bufsize) {
		int NewBufSize;
		BISFIXBUF(r->bytesavail + len);
	}
	if (r->bytesavail && r->pos<r->bytesavail) 
		memmove(r->buf+r->pos+len, r->buf+r->pos, r->bytesavail-r->pos);
	memcpy(r->buf+r->pos, bytes, len);
	r->bytesavail+=len;
	return 0;
}

/*===========================================================================
  FUNCTION
    BufferedIStream_ResetBuf
  DESC
    Resets BufferedIStream buffer
  PARAMS
    r           this
    numBytes    bytes to skip from start of buffer (usually r->pos)
  RETURNS
    TRUE        SUCCESS
    0           FAILURE
  NOTES
    _ResetBuf resets the position to 0 and ALWAYS
    skip bytes from the start of reader buffer.
===========================================================================*/
int BufferedIStream_ResetBuf(LPBUFFEREDISTREAM r, 
						 int numBytes)
{	
	int NewBufSize;
	if (!numBytes) return 0;
	if (numBytes > r->bytesavail) return BIS_ERR_INVALIDARG;
	r->pos = 0;
	r->bytesavail -= numBytes;
	if (r->bytesavail) memmove(r->buf, r->buf+numBytes, r->bytesavail);
	if (r->bytesavail <= r->blocksize) {
		/* keeping 2 blocks allocated in outbut buffer will result in fewer calls to 
		  realloc when encoding since encoding buffer (inbuf) size is equal to
		  r->blocksize */
		BISFIXBUF((2 * r->blocksize));
	}
	else {
		BISFIXBUF(r->bytesavail);
	}
	return 0; 
}

LPBUFFEREDISTREAM BufferedIStream_Init(LPBUFFEREDISTREAM r, int blocksize)
{
	r->buf = r->inbuf = (BYTE*)NULL;
	r->bufsize = r->bytesavail = r->pos = r->eof = 0;
	r->err = r->encerr = r->inbufrest = 0;
	r->inputData = NULL;
		/* note that we don't initialize userdata */
	r->inputsrc = (LPFNINPUTSRC)NULL;
	r->encode = (LPFNENCODE)NULL;
	r->blocksize = blocksize;
	r->maxbufsize = BIS_DEFAULT_MAXBUFSIZE;
#ifdef ICONV_SUPPORT
	r->cd=(iconv_t)NULL;
#endif
	return r;
}

void BufferedIStream_Free(LPBUFFEREDISTREAM r)
{
	if (r->buf != (BYTE*)NULL) {
		free(r->buf);	
		r->buf = (BYTE*)NULL;
	}
	if (r->inbuf != (BYTE*)NULL) {
		free(r->inbuf);	
		r->inbuf = (BYTE*)NULL;
	}
#ifdef ICONV_SUPPORT
	if (r->cd) {
		iconv_close(r->cd);
		r->cd=(iconv_t)NULL;
	}
#endif
}

static int EncodeInbuf(LPBUFFEREDISTREAM r, int cBytes) 
{	
	size_t inBytes = cBytes + r->inbufrest;
	size_t outBytes = r->bufsize - r->bytesavail;
	size_t l, outTotal;
	BYTE *in = r->inbuf + BIS_CHAR_MAX - r->inbufrest;
	BYTE *out;
	r->inbufrest = outTotal = 0;
	
	if (outBytes < inBytes) {
		l = ((int)inBytes > r->blocksize) ? r->blocksize * 2 : r->blocksize;
		r->buf = (BYTE*)realloc(r->buf, r->bufsize + l);
		if (!r->buf) return BIS_ERR_MEMALLOC;
		r->bufsize += l;
		outBytes += l;
	}
	
	out = r->buf + r->bytesavail;

	while (inBytes > 0) {	
		
		cBytes = outBytes;
		l = r->encode(r, (const BYTE**)&in, &inBytes, &out, &outBytes);
		outTotal += cBytes - outBytes;

		if (l == (size_t)(-1)) {
			if (r->encerr == EINVAL) {
				if (r->eof)
					return BIS_ERR_ENCODING;

				r->inbufrest = inBytes;
				if (inBytes) {			
					out = r->inbuf + BIS_CHAR_MAX - r->inbufrest;
					do { *out++ = *in++; } while (--inBytes > 0);				
				}
				return (outTotal);
			}
			else if (r->encerr == E2BIG) {
				r->buf = (BYTE*)realloc(r->buf, r->bufsize + r->blocksize);
				if (!r->buf) return BIS_ERR_MEMALLOC;
				r->bufsize += r->blocksize;
				outBytes += r->blocksize;
				out = r->buf + r->bytesavail + outTotal;
			}
			else { /* r->encerr == EILSEQ and others */
				return BIS_ERR_ENCODING;	
			}
		}
	}
	return (outTotal);
}


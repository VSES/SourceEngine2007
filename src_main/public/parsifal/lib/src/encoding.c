/*===========================================================================
  encoding.c
    This module contains encoding "plug-ins" for BufferedIStream
    see parsifal.h for copyright info
  TODO
    There's several inconsistencies between iconv size_t length vars
    and Parsifal signed int length vars. Shouldn't give to much problems
    'cos BIS_DEFAULT_MAXBUFSIZE = INT_MAX...
===========================================================================*/

#include <stdlib.h>
#include <string.h>
#include "bistream.h"
#include "xmlcfg.h"

#define ENC_ERR ((size_t)(-1))

/* encoding "plug-ins" for BufferedIStream: */
size_t Latin1ToUtf8 (LPBUFFEREDISTREAM r, const BYTE **inbuf, size_t *inbytesleft, BYTE **outbuf, size_t *outbytesleft);
size_t Utf8ToUtf8 (LPBUFFEREDISTREAM r, const BYTE **inbuf, size_t *inbytesleft, BYTE **outbuf, size_t *outbytesleft);
#ifdef ICONV_SUPPORT
size_t iconvWrapper (LPBUFFEREDISTREAM r, const BYTE **inbuf, size_t *inbytesleft, BYTE **outbuf, size_t *outbytesleft);
#endif

size_t Latin1ToUtf8 (LPBUFFEREDISTREAM r, const BYTE **inbuf, size_t *inbytesleft, BYTE **outbuf, size_t *outbytesleft)
{
	while (*inbytesleft) {
		if (!(**inbuf & 0x80)) { /* < 128 */
			if (!*outbytesleft) {
				r->encerr = E2BIG;
				return ENC_ERR;
			}
			*(*outbuf)++ = *(*inbuf)++;
			(*outbytesleft)--;
		}
		else {
			if (*outbytesleft < 2) {
				r->encerr = E2BIG;
				return ENC_ERR;
			}
			*(*outbuf)++ = ((**inbuf >> 6) | 0xc0);
			*(*outbuf)++ = ((**inbuf & 0x3f) | 0x80);
			(*inbuf)++;
			(*outbytesleft)-=2;
		}
		(*inbytesleft)--;
	}
	return 0;
}

size_t Utf8ToUtf8 (LPBUFFEREDISTREAM r, const BYTE **inbuf, size_t *inbytesleft, BYTE **outbuf, size_t *outbytesleft)
{
	size_t more, min;
	UINT32 c;

	while (*inbytesleft) {
		if (!(**inbuf & 0x80)) { /* < 128 */
			if (!*outbytesleft) {
				r->encerr = E2BIG;
				return ENC_ERR;
			}
			
			*(*outbuf)++ = *(*inbuf)++;
			(*inbytesleft)--;
			(*outbytesleft)--;
			continue; 
		}
		else {
		c = (UINT32)**inbuf;
		
		if(c < 0xc0 || c >= 0xfe) {
			/* printf("Illegal UTF-8 start byte"); */
			r->encerr = EILSEQ;
			return ENC_ERR;
		}
		
		if(c <= 0xdf) {
		    c &= 0x1f;
		    more = 1;
		    min = 0x80;
		}
		else if(c <= 0xef) {
		    c &= 0x0f;
		    more = 2;
		    min = 0x800;
		}
		else if(c <= 0xf7) {
		    c &= 0x07;
		    more = 3;
		    min = 0x10000;
		}
		else if(c <= 0xfb){
		    c &= 0x03;
		    more = 4;
		    min = 0x200000;
		}
		else {
		    c &= 0x01;
		    more = 5;
		    min = 0x4000000;
		}

		if (*inbytesleft < (more + 1)) {
			r->encerr = EINVAL;
			return ENC_ERR;
		}

		if (*outbytesleft < (more + 1)) {
			r->encerr = E2BIG;
			return ENC_ERR;
		}
		
		*(*outbuf)++ = *(*inbuf)++;
		(*inbytesleft)--;
		(*outbytesleft)--;

		while (more--)
		{
			if((**inbuf & 0xc0) != 0x80) { 
				/* sequence byte must start with 10xxxxxx 
				printf("Illegal UTF-8 byte"); */
				r->encerr = EILSEQ;
				return ENC_ERR;
			}

		    c = (c << 6) + (**inbuf & 0x3f); /* store actual char value (unicode) that
										   will be tested for overlong values */
			*(*outbuf)++ = *(*inbuf)++;
			(*inbytesleft)--;
			(*outbytesleft)--;
		}

		if (c < min) {
			/* printf("Overlong UTF-8 sequence"); */
			r->encerr = EILSEQ;
			return ENC_ERR;
		}
		} /* else > 127 */
	}
	return 0;
}

#ifdef ICONV_SUPPORT

size_t iconvWrapper (LPBUFFEREDISTREAM r, const BYTE **inbuf, size_t *inbytesleft, BYTE **outbuf, size_t *outbytesleft)
{
	size_t ret = iconv(r->cd, (const char**)inbuf, inbytesleft, (char**)outbuf, outbytesleft);
	if (ret == ENC_ERR) r->encerr = errno;
	return ret;
}

#endif /* ICONV_SUPPORT */

#undef ENC_ERR


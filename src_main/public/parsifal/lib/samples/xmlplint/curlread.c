#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "curlread.h"

#ifdef TEST_CURLREADER
#ifdef _MSC_VER
#ifdef _DEBUG
#include <crtdbg.h>
#define _CRTDBG_MAP_ALLOC
#endif
#endif
#endif

static size_t getdata(char *buffer, size_t size, size_t nitems, void *userp)
{	
	CURLREADER *r = (CURLREADER*)userp;
	if (r->pos) {
		size_t bytesLeft = r->sbuf.len-r->pos;
		if (bytesLeft) memmove(r->sbuf.str, r->sbuf.str+r->pos, bytesLeft);
		r->sbuf.len = bytesLeft;
		r->pos = 0;
	}
	/* note that we don't shrink the sbuf at all - relying on libcurl 
	keeping constant buffer sizes is probably the most efficient way 
	CURL_MAX_WRITE_SIZE 16384 */
	if (!XMLStringbuf_Append(&r->sbuf, buffer, size*nitems)) return 0;
	return nitems;
}

CURLREADER *Curlreader_Create(void)
{
	CURLREADER *r = malloc(sizeof(CURLREADER));
	if (r==NULL) return NULL;
	r->curlh = NULL;
	r->curlmh = NULL;
	r->flags = 0;
	XMLStringbuf_Init(&r->sbuf, 1024, 0);
	r->curlh = curl_easy_init();
	if (!r->curlh) {
		Curlreader_Free(r);
		return NULL;
	}
    curl_easy_setopt(r->curlh, CURLOPT_WRITEDATA, r);
    curl_easy_setopt(r->curlh, CURLOPT_WRITEFUNCTION, getdata);
	r->curlmh = curl_multi_init();
	if (!r->curlmh) {
		Curlreader_Free(r);
		return NULL;
	}
	return r;
}

/*
From libcurl mailing list:
curl_multi_info_read returns a pointer to a struct where 'info->msg' contains 
CURLMSG_DONE when a single easy-handle has moved over to the 'completed' state. 
'info->easy_handle' is then the CURL handle and the 'info->data.result' 
contains the 'CURLcode' of that particular transfer. 

Presently, there is no other things this can return. If it does, then it is a 
bug.*/
int Curlreader_ReadStatusCode(CURLREADER *r)
{
	CURLMsg *msg;
	int msgs_left;
	if (r && r->curlmh && r->curlh) {
		while ((msg = curl_multi_info_read(r->curlmh, &msgs_left))) {
			if (msg->msg == CURLMSG_DONE && msg->easy_handle == r->curlh)
				return msg->data.result;
		}
	}
	return CURL_LAST;
}

int Curlreader_Open(CURLREADER *r, char *url)
{	
	r->pos = 0;
	r->errE = curl_easy_setopt(r->curlh, CURLOPT_URL, url);
	if (r->errE != CURLE_OK) return 0;
	r->errM = curl_multi_add_handle(r->curlmh, r->curlh);
	if (r->errM == CURLM_OK || r->errM == CURLM_CALL_MULTI_PERFORM) {
		r->flags = CURLREADER_FLAGS_OPEN;
		while((r->errM = curl_multi_perform(r->curlmh, &r->still_running)) ==
			CURLM_CALL_MULTI_PERFORM); 
	}
	return (r->errM == CURLM_OK);
}

void Curlreader_Close(CURLREADER *r)
{
	curl_multi_remove_handle(r->curlmh, r->curlh);
	if (r->flags != CURLREADER_FLAGS_FREEING) 
		XMLStringbuf_SetLength(&r->sbuf, 0);
	r->flags = 0;
}

size_t Curlreader_Read(CURLREADER *r, void *ptr, size_t size, size_t nmemb)
{ 
	size*=nmemb;
    while (r->still_running && (r->sbuf.len - r->pos) < size)
    {	
		fd_set fdread;
		fd_set fdwrite;
		fd_set fdexcep;
		int maxfd;
		struct timeval timeout;

        FD_ZERO(&fdread);
        FD_ZERO(&fdwrite);
        FD_ZERO(&fdexcep);

        timeout.tv_sec = 10;
        timeout.tv_usec = 0;

        r->errM = curl_multi_fdset(r->curlmh, &fdread, &fdwrite, &fdexcep, &maxfd);
		if (r->errM != CURLM_OK) return 0;
		
        switch(select(maxfd+1, &fdread, &fdwrite, &fdexcep, &timeout)) {
        case -1:
			r->still_running = 0;
			#ifdef _DEBUG
				fputs("Curlreader select() error!", stderr);
			#endif
			break;   
        case 0: /* timeout */
        default:
			while((r->errM = curl_multi_perform(r->curlmh, &r->still_running)) ==
				CURLM_CALL_MULTI_PERFORM);
            if (r->errM != CURLM_OK) return 0;
            break;
        }
    }

	if (r->pos == (size_t)r->sbuf.len) return 0;
	
	nmemb = r->sbuf.len - r->pos;
	
	if (nmemb < size) {
		memcpy(ptr, r->sbuf.str+r->pos, nmemb);
		r->pos += nmemb;
		return nmemb;
	}	
	memcpy(ptr, r->sbuf.str+r->pos, size);
	r->pos += size;
	return size;
}

void Curlreader_Free(CURLREADER *r)
{
	if (r==NULL) return;
	if (r->curlh) { 
		if (r->flags & CURLREADER_FLAGS_OPEN) {
			r->flags = CURLREADER_FLAGS_FREEING;
			Curlreader_Close(r);
		}
		curl_easy_cleanup(r->curlh);
	}
	if (r->curlmh) curl_multi_cleanup(r->curlmh);
	XMLStringbuf_Free(&r->sbuf);
	free(r);
}

#ifdef TEST_CURLREADER

void ShowCurlReaderStatusErr(CURLREADER *r, char *ErrBuf)
{	
	int sc = Curlreader_ReadStatusCode(r);
	if (sc != CURLE_OK) {
		fputc('\n', stderr);
		if (sc == CURL_LAST || !*ErrBuf)
			fputs("Unknown error/timeout", stderr);
		else
			fputs(ErrBuf, stderr);
	}
}

int main(int argc, char* argv[])
{	
	char ErrBuf[CURL_ERROR_SIZE] = {'\0'};
	CURLREADER *r = Curlreader_Create();
	char buf[128];
	char buf2[256];
	size_t bytes;
	size_t i, j;
	char *urls[] = {	
		"http://www.google.com",
		"http://www.nohost",
		"http://www.w3c.org/home",
		"http://www.saunalahti.fi/~samiuus/toni/COPYING",
		"errproto://www.google.com"
	};
/*
	#ifdef _MSC_VER	
	#ifdef _DEBUG
			int tmpFlag = _CrtSetDbgFlag( _CRTDBG_REPORT_FLAG );
			tmpFlag |= _CRTDBG_LEAK_CHECK_DF;
			_CrtSetDbgFlag( tmpFlag );
	#endif
	#endif
*/	
	if (curl_global_init(CURL_GLOBAL_ALL)) {
		fputs("Error Initializing libcurl!", stderr);
		return 1;
	}

	if (!r) {
		/* should be allocation/fatal error */
		fputs("Error creating Curlreader!", stderr);
		return 1;
	}
	curl_easy_setopt(r->curlh, CURLOPT_ERRORBUFFER , ErrBuf);
	if (Curlreader_Open(r, urls[0])) {
		while ((bytes = Curlreader_Read(r, buf, 1, sizeof(buf)))) {
			printf("\n%d bytes:\n", bytes);
			for (i=0; i<bytes; i++) fputc(buf[i], stdout);
		}
	}

	ShowCurlReaderStatusErr(r, ErrBuf);
	Curlreader_Free(r);
	
	/* Test reuse: */

	r = Curlreader_Create();
	if (!r) {
		fputs("Error creating Curlreader!", stderr);
		return 1;
	}
	curl_easy_setopt(r->curlh, CURLOPT_ERRORBUFFER , ErrBuf);
	
	for (j=0; j<(sizeof(urls)/sizeof(urls[0])); j++) {
		*ErrBuf = 0;
		if (Curlreader_Open(r, urls[j])) {
			while ((bytes = Curlreader_Read(r, buf2, 1, sizeof(buf2)))) {
				printf("\n%d bytes:\n", bytes);
				for (i=0; i<bytes; i++) fputc(buf2[i], stdout);
			}
		}
		ShowCurlReaderStatusErr(r, ErrBuf);
		Curlreader_Close(r);
	}
	
	Curlreader_Free(r);
	curl_global_cleanup();
	return 0;
}
#endif /* TEST */



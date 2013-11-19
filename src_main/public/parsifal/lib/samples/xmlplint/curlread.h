#include "curl/curl.h"
#include "libparsifal/xmlsbuf.h"

/* Usage: see curlread.c main() */

typedef struct tagCURLREADER
{
  CURL *curlh;
  CURLM *curlmh;
  int still_running;
  int flags;
  size_t pos;
  CURLcode errE;
  CURLMcode errM;
  XMLSTRINGBUF sbuf;
} CURLREADER;

#define CURLREADER_FLAGS_FREEING -1
#define CURLREADER_FLAGS_OPEN 1

/* Allocates and initializes CURLREADER, 
initializes easyhandle r->curlh,
sets CURLOPT_WRITEDATA and CURLOPT_WRITEFUNCTION for it and
initializes multihandle r->curlm 
RETURNS new CURLREADER or NULL if some error occurred 
after _Create call you can set options for r->curlh etc. */ 
CURLREADER *Curlreader_Create(void);

/* sets CURLOPT_URL for r->curlh and adds r->curlh to r->curlmh,
calls curl_multi_perform .RETURNS 0 if some error occurred, 
it is legal to call _Read even when _Open returns 0 (it's a no op) */
int Curlreader_Open(CURLREADER *r, char *url);

/* removes r->curlh from r->curlmh, resets CURLREADER data for
reuse (for new _Open call) */
void Curlreader_Close(CURLREADER *r);

/* "fread" of CURLREADER */
size_t Curlreader_Read(CURLREADER *r, void *ptr, size_t size, size_t nmemb);

/* Frees CURLREADER and its resources (calls _Close if
curlh is open) */
void Curlreader_Free(CURLREADER *r);

/* Returns CURLE_OK or some other CURLcode, CURL_LAST if 
status cannot be determined */
int Curlreader_ReadStatusCode(CURLREADER *r);


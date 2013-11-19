#ifdef _WIN32
	#if !(defined(_WINSOCKAPI_) || defined(_WINSOCK_H))
	/* The check above prevents the winsock2 inclusion if winsock.h already was
	   included, since they can't co-exist without problems */
	#include <winsock2.h>
	#endif
	#include <time.h>
	int gettimeofday(struct timeval *tv, void *tz);
#else
	#include <sys/time.h>
#endif

#define STimer_Stop(st) \
  (gettimeofday(&(st)->end, NULL), \
  (st)->msResult = ((st)->end.tv_sec - (st)->begin.tv_sec) * 1000, \
  (st)->msResult += ((st)->end.tv_usec - (st)->begin.tv_usec) / 1000 )

typedef struct tagSTIMER 
{
	struct timeval begin;
	struct timeval end;
	long msResult;
} STIMER;

void STimer_Start(STIMER *stimer);

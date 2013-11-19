#ifndef TIMESTAMP_H
#define TIMESTAMP_H

#include "sys/timeb.h"
#include "sys/types.h"
#include "time.h"

#define DX_UPDATE_FREQUENCY			(1000.0f/15.0f)

class CTimeStamp
{
public:
	_timeb			Coarse;
	clock_t			Fine;

	CTimeStamp();
	void AcquireTime();
	bool RoughlyEqual(CTimeStamp &test);
};


#endif
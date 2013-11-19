#pragma warning(disable : 4996)

#include "pch.h"

CTimeStamp::CTimeStamp() 
{
	AcquireTime();
} 

void 
CTimeStamp::AcquireTime()
{
	_ftime(&Coarse);
	Fine= clock();
}

bool 
CTimeStamp::RoughlyEqual(CTimeStamp &test)
{
	if(this->Coarse.time == test.Coarse.time)
	{
		float micro= (float)test.Coarse.millitm - (float)this->Coarse.millitm;
		if(micro <= DX_UPDATE_FREQUENCY)
		{
			return true;
		}
	}
	return false;
}
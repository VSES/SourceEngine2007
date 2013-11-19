
#ifndef __ItemTracker_h__
#define __ItemTracker_h__
#pragma once

#define MIN_SYNC_INTERVAL (1000.0f/15.0f)

template <typename Dt>
class ItemTracker
{
public:

	ItemTracker(const Dt& item)
    {
        Item = item;
    }

	void Modified()
    {
        LastModifiedTick = GetTickCount();
    }

	void Updated()
    {
        LastUpdatedTick = GetTickCount();
    }

	bool IsExtStale() const
	{
		return LastModifiedTick > LastUpdatedTick;
	}

public: // data

	Dt Item;		// The item in the dxcc world

protected: // data

	DWORD LastModifiedTick;
	DWORD LastUpdatedTick;
};


#endif
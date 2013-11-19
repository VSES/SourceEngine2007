
#include "StdAfx.h"

#include "Pipeline.h"
#include "PipelineLock.h"

CPipelineLock::CPipelineLock(bool AutoUnlock)
{
	Type = TYPE_NONE; 
	Locked = false; 
	UnlockOnDestuction = AutoUnlock;
	Pipeline = NULL; 
};

CPipelineLock::~CPipelineLock()
{
	if(Locked && UnlockOnDestuction && Pipeline)
	{
		switch(Type)
		{

		case TYPE_SCENE_WRITE:
			Pipeline->SceneWriteUnlock(*this);
			break;

		case TYPE_SCENE_READ:
			Pipeline->SceneReadUnlock(*this);
			break;
		};
	}
}


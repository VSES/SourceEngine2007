#include "pch.h"

CManagerLock::CManagerLock(bool AutoUnlock)
{
	Type=TYPE_NONE; 
	Locked=false; 
	UnlockOnDestuction= AutoUnlock;
	pPreviewPipeline=NULL; 
};

CManagerLock::~CManagerLock()
{
	if(Locked && UnlockOnDestuction && pPreviewPipeline)
	{
		switch(Type)
		{

		case TYPE_SCENE_WRITE:
			pPreviewPipeline->SceneWriteUnlock(*this);
			break;
		case TYPE_SCENE_READ:
			pPreviewPipeline->SceneReadUnlock(*this);
			break;
		};
	}
}


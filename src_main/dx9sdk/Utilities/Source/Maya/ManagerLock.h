#ifndef LOCK_H
#define LOCK_H

class CManager;

class CManagerLock
{
	friend CManager;
	//friend CManagerEvents;
public:
	CManagerLock(bool AutoUnlock= true);
	~CManagerLock();

	long ofType(){ return Type; }
	bool isLocked(){ return Locked; }

private:
	enum
	{
		TYPE_NONE= 0,
		TYPE_SCENE_WRITE,
		TYPE_SCENE_READ,
		NUMBER_OF_TYPES
	}					Type;
	bool				Locked;
	bool				UnlockOnDestuction;
	CManager*	pPreviewPipeline;
};

#endif
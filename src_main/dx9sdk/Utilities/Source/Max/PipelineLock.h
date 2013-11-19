#ifndef __PipelineLock_h__
#define __PipelineLock_h__

class CPipeline;

class CPipelineLock
{
	friend CPipeline;
public:

	CPipelineLock(bool AutoUnlock= true);
	~CPipelineLock();

	long ofType(){ return Type; }
	bool isLocked(){ return Locked; }

private:
	enum
	{
		TYPE_NONE= 0,
		TYPE_SCENE_WRITE,
		TYPE_SCENE_READ,
		NUMBER_OF_TYPES
	}
	Type;
	
	bool		Locked;
	bool		UnlockOnDestuction;
	CPipeline*	Pipeline;
};

#endif
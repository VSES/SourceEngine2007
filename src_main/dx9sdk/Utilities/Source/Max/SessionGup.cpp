
#include "StdAfx.h"

#include "MaxPipeline.h"

#define SESSIONGUP_CLASS_ID Class_ID(0x7f8f4071, 0x3c7c05f5)

class SessionGup : public GUP 
{
public:
    SessionGup();
    ~SessionGup();		

	DWORD Start();
	void Stop();
};


SessionGup::SessionGup()
{
}

SessionGup::~SessionGup() 
{
}

DWORD SessionGup::Start()
{
    ThePipeline.Create();

    return GUPRESULT_KEEP;
}

void SessionGup::Stop()
{
    ThePipeline.Destroy();
}

class SessionGupClassDesc : public ClassDesc2 
{
public:

	int IsPublic() 
	{ 
		return TRUE; 
	}

	void* Create(BOOL loading = FALSE) 
	{ 
		return new SessionGup(); 
	}

	const TCHAR* ClassName() 
	{ 
		return TEXT("SessionGup"); 
	}

	SClass_ID SuperClassID() 
	{ 
        return GUP_CLASS_ID; 
	}

	Class_ID ClassID() 
	{ 
		return SESSIONGUP_CLASS_ID; 
	}

	const TCHAR* Category() 
	{ 
		return TEXT("Global Utility Plugins"); 
	}

	const TCHAR* InternalName() 
	{ 
		return _T("SessionGup"); 
	}	

	HINSTANCE HInstance() 
	{ 
		extern HINSTANCE Instance;
		return Instance; 
	}	
};

ClassDesc* SessionGup_GetClassDesc() 
{ 
	static SessionGupClassDesc Desc;
	return &Desc; 
}

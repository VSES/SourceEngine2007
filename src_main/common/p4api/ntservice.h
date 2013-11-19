/*
 * Copyright 1995, 2000 Perforce Software.  All rights reserved.
 *
 * This file is part of Perforce - the FAST SCM System.
 */

/*
 * NtService - Class allows Perforce server to run as a service on NT.
 *
 * Public methods:
 *
 *	NtService::NtService()
 *	    Constructor requires a name, and a pointer to the entry point for
 *	    the program which can not be main because main calls 
 *	    StartServiceControlDispatcher() and never returns.
 *
 *	NtService::Start()
 *	    This calls StartServiceControlDispatcher to connect the service to 
 *	    the SCM. This should be called as soon as possible after the 
 *	    program starts because the SCM will only wait 30 seconds for this
 *	    call.
 *
 *	NtService::SetStatus()
 *	    Informx SCM of changes in the services status. Required to tell 
 *	    SCM when service has successfully started.
 *
 */

#ifndef NTSERVICE_H__
#define NTSERVICE_H__

class NtService
{
    public:
	enum states 
	{  
	    stopped,
	    running,
	    start_pending,
	    stop_pending,
	    paused,
	    pause_pending,
	    continue_pending,
	    no_change 
	};

		    NtService();
    	virtual	    ~NtService();

	// Our caller's interface

    	virtual void Start( int (*entryPt)( DWORD, char ** ), char *svc );

    	virtual void SetStatus(   
			states state = no_change, 
			DWORD win32_exitcode = 0,
			DWORD specific_code = 0,
			DWORD wait_hint	= 0 );

    private:

	// SCM callbacks with Win32 interfaces.
	// Do not call them directly.
	// Because they are static, we have to remember
	// the (one) NtService in use.

	static NtService *global_this; 
	static void WINAPI ControlHandler( DWORD opcode );
	static void StaticRun( DWORD argc, char **argv );

	// Called by ControlHanlder

	virtual void    Run( DWORD argc, char **argv );
	virtual void    Stop();

	SERVICE_STATUS		status;
	SERVICE_STATUS_HANDLE	statusHandle;

	int ( *entry_point )( DWORD, char ** );

	char	svcName[32];
};

#endif // NTSERVICE_H__

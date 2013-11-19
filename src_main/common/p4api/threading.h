/*
 * Copyright 1995, 2000 Perforce Software.  All rights reserved.
 *
 * This file is part of Perforce - the FAST SCM System.
 */

/*
 * threading.h -- handle multiple users at the same time
 *
 * These classes are meant to take the vicious ifdef hacking required
 * to get threading (forking, whatever) to work on different platforms.  
 *
 * Some terms:
 *
 *	"thread": a process on UNIX; a thread on NT
 *	"leader": the parent process on UNIX; the only process on NT
 *
 * Classes defined:
 *
 *	Threading: caller interface to launch multiple threads
 *
 *	Threader: implementation of threading (not public) 
 *
 *  	Thread (abstract): glues caller's execution object so that Threading 
 *  	can run and then delete it.
 *
 *  	Process (abstract): callbacks into caller's environment to signal 
 *  	process related changes.
 *
 * Public methods:
 *
 *	Thread::Run() - do what was supposed to happen in the thread
 *	Thread::~Thread() - delete user's class, cleaning up thread exit
 *
 *	Process::Child() - indicates that Run() will be a child process
 *	Process::Cancel() - indicates that leader should stop launching
 *
 *	Threading::Launch() - create a thread/process and call Thread::Run().
 *	
 *	Threading::Cancelled() - returns true (in leader) if Cancel()
 *
 *	Threading::Cancel() - can be called from any thread to tell the 
 *			leader to stop; leader calls Process::Cancel()
 *
 *	Threading::Reap() - called in leader to kill children
 *
 * The current termination ritual:
 *
 *	Someone, somewhere calls Threading::Cancel(), which is static
 *	and always available.  It can get called by the leader
 *	catching SIGTERM, or by anyone at the user's request.
 *
 *	If a child gets Threading::Cancel() on UNIX, it sends a SIGTERM
 *	to its parent so that the leader gets Threading::Cancel() called.
 *
 *	In the leader, Threading::Cancel() sets the "cancelled" flag and
 *	calls Process::Cancel(), so that (in fact) the listen socket
 *	descriptor gets closed, breaking the accept() loop.
 *
 *	The leader, out of its thread creation loop, can call Reap()
 *	in order to kill and collect all the child processes.  It should
 *	only do that if the database is safely locked from child process
 *	access.
 */

enum ThreadMode {
	TmbSingle,	// just single threading
	TmbMulti,	// multi threading (fork, threads)
	TmbDaemon	// fork, then forking multi threading (UNIX)
} ;

class Thread {

    public:

	virtual		~Thread();
	virtual void	Run() = 0;

} ;

class Process {

    public:

	virtual		~Process();
	virtual void	Child() = 0;
	virtual void	Cancel() = 0;

} ;

class Threader {

    protected:

    friend class Threading;

			Threader() { cancelled = 0; }

	virtual		~Threader();
	virtual void	Launch( Thread *t );
	virtual void	Cancel();
	virtual void	Reap();

	int		cancelled;
	Process		*process;

} ;

class Threading {

    public:
			Threading( ThreadMode tmb, Process *p );
			~Threading() { delete threader; }

	void		Launch( Thread *t ) { threader->Launch( t ); }
	int		Cancelled() { return threader->cancelled; }
	void		Reap() { threader->Reap(); }

	static void	Cancel() { if( current ) current->Cancel(); }
	static int	WasCancelled() { if( current ) return current->cancelled; else return 0; }

    private:

	Threader 	*threader;

	static Threader *current;

} ;

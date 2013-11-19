/*
 * Copyright 1995, 1996 Perforce Software.  All rights reserved.
 *
 * This file is part of Perforce - the FAST SCM System.
 */

/*
 * Signaler.h - catch ^C and delete temp files
 *
 * A single Signaler is declared globally.
 *
 * Public methods:
 *
 *	Signaler::Block() -- don't catch the signal until Catch()
 *	Signaler::Catch() -- catch and handle SIGINT
 *	Signaler::OnIntr() -- call a designated function on SIGINT
 *	Signaler::DeleteOnIntr() -- undo OnIntr() call
 *
 *	Signaler::Intr() -- call functions registered by OnIntr()
 *
 * Requires cooperation from the TempFile objects to delete files.
 */

# ifdef OS_NT
typedef void *HANDLE;
# endif

struct SignalMan;

typedef void (*SignalFunc)( void *ptr );

class Signaler {

    public:
			Signaler();

	void		Block();
	void		Catch();

	void		OnIntr( SignalFunc callback, void *ptr );
	void		DeleteOnIntr( void *ptr );

	void		Intr();

    private:

	SignalMan	*list;

# ifdef OS_NT
	HANDLE		hmutex;
# endif // OS_NT

} ;

extern Signaler signaler;

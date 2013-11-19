/*
 * Copyright 1995, 1996 Perforce Software.  All rights reserved.
 *
 * This file is part of Perforce - the FAST SCM System.
 */

/*
 * DateTime - get and set the date as a string
 */

// size for Fmt, FmtDay, FmtTz

# define DateTimeBufSize 20 

class DateTime {

    public:
	int 	Compare( const DateTime &t2 ) const { return tval - t2.tval; };

	void	Fmt( char *buf ) const;
	void	FmtDay( char *buf ) const;
	void	FmtTz( char *buf ) const;
	void 	FmtElapsed( char *buf, const DateTime &t2 );
	void	Set( const char *date, Error *e );
	void	Set( const int date ) { tval = (time_t)date; }
	void	SetNow();

	int	Value() const { return tval; }
	int	Tomorrow() const { return tval + 24*60*60; }
	int	IsWholeDay() const { return wholeDay; }

	// for stat() and utime() conversion

	static time_t Localize( time_t centralTime );
	static time_t Centralize( time_t localTime );	

    private:
	time_t	tval;
	int	wholeDay;

};


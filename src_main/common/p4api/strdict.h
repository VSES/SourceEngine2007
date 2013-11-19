/*
 * Copyright 1995, 1996 Perforce Software.  All rights reserved.
 *
 * This file is part of Perforce - the FAST SCM System.
 */

/*
 * StrDict.h - a set/get dictionary interface
 *
 * Classes:
 *
 *	StrDict - a GetVar/PutVar dictionary interface
 *
 * Methods:
 *
 */

class Error;

class StrVarName : public StrRef {

    public:
		StrVarName( const char *buf, int length )
		{
		    memcpy( varName, buf, length );
		    varName[ length ] = 0;
		    Set( varName, length );
		}

		StrVarName( const StrPtr &name, int x );
		StrVarName( const StrPtr &name, int x, int y );

    private:
		char varName[64];
} ;

class StrDict {

    public:

	virtual	~StrDict();

	// Handy wrappers

	void	SetVar( const char *var );
	void	SetVar( const char *var, int value );
	void	SetVar( const char *var, const char *value );
	void	SetVar( const char *var, const StrPtr *value );
	void	SetVar( const char *var, const StrPtr &value );
	void	SetVar( const StrPtr &var, const StrPtr &value )
		{ VSetVar( var, value ); }

	void	SetVarV( const char *arg );
	void	SetArgv( int argc, char *const *argv );
	void	SetVar( const StrPtr &var, int x, const StrPtr &val );
	void	SetVar( const char *var, int x, const StrPtr &val );
	void	SetVar( const char *var, int x, int y, const StrPtr &val );

	StrPtr *GetVar( const char *var );
	StrPtr *GetVar( const char *var, Error *e );
	StrPtr *GetVar( const StrPtr &var, int x );
	StrPtr *GetVar( const StrPtr &var, int x, int y );
	StrPtr *GetVar( const StrPtr &var )
		{ return VGetVar( var ); }

	int	GetVar( int x, StrRef &var, StrRef &val )
		{ return VGetVarX( x, var, val ); }

	void	ReplaceVar( const char *var, const char *value );
	void	RemoveVar( const char *var );
	
	int 	Save( FILE * out );
	int 	Load( FILE * out );
	
    protected:

	// Get/Set vars, provided by subclass

	virtual StrPtr *VGetVar( const StrPtr &var ) = 0;
	virtual void	VSetVar( const StrPtr &var, const StrPtr &val );
	virtual void	VRemoveVar( const StrPtr &var );
	virtual int	VGetVarX( int x, StrRef &var, StrRef &val );
	virtual void	VSetError( const StrPtr &var, Error *e );

} ;

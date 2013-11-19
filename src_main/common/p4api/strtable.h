/*
 * Copyright 1995, 1996 Perforce Software.  All rights reserved.
 *
 * This file is part of Perforce - the FAST SCM System.
 */

/*
 * strtable.h - a string table, using StrDict interface
 *
 * Classes defined:
 *
 *	StrPtrDict - a dictionary whose values we don't own
 *	StrBufDict - a dictionary whose values we do own
 *
 * Public methods:
 *
 *	Clear() - reset table, making all variables unset
 *	GetVar() - look up variable, return value (or 0 if not set)
 *	SetVar() - set variable/value pair
 *
 * XXX Total dumb duplication of StrPtrDict into StrBufDict. 
 */

struct StrPtrEntry;
struct StrBufEntry;
class VarArray;

class StrPtrDict : public StrDict {

    public:
			StrPtrDict();
			~StrPtrDict();

	void		Clear()
			{
			    tabLength = 0;
			}

	// virtuals of StrDict

	StrPtr *	VGetVar( const StrPtr &var );
	void		VSetVar( const StrPtr &var, const StrPtr &val );
	void		VRemoveVar( const StrPtr &var );
	int		VGetVarX( int x, StrRef &var, StrRef &val );

    private:
	
	VarArray	*elems;
	int		tabSize;
	int		tabLength;

} ;

class StrBufDict : public StrDict {

    public:
			StrBufDict();
			StrBufDict( StrDict & dict );
			StrBufDict & operator =( StrDict & dict );
			~StrBufDict();

	void		Clear()
			{
			    tabLength = 0;
			}

	int		GetCount()
			{
			    return tabLength;
			}

	// virtuals of StrDict

	StrPtr *	VGetVar( const StrPtr &var );
	void		VSetVar( const StrPtr &var, const StrPtr &val );
	void		VRemoveVar( const StrPtr &var );
	int		VGetVarX( int x, StrRef &var, StrRef &val );

    private:
	
	void Set( StrDict & dict );
	
	VarArray	*elems;
	int		tabSize;
	int		tabLength;

} ;


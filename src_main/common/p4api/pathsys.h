/*
 * Copyright 1995, 2003 Perforce Software.  All rights reserved.
 *
 * This file is part of Perforce - the FAST SCM System.
 */

/*
 * PathSys.h - OS specific pathnames
 *
 * Public classes:
 *
 *	PathSys - a StrBuf with virtual path manipulations
 *
 * Public methods:
 *
 *	StrBuf::Set() - set value in local syntax
 *	StrBuf::Text() - get value in local syntax
 *
 *	PathSys::SetCanon() - combine (local) root and canonical path
 *	PathSys::SetLocal() - combine (local) root and local path
 *
 *		If root is empty, local is used.
 *		If local is empty, results are not defined.
 *		Local can begin with relative references.
 *
 *	PathSys::GetCanon() - strip root and return rest as canon
 *	PathSys::ToParent() - strip (and return) last element of path
 *
 * NB: SetLocal() can take "this" as root, but SetCanon() cannot.
 *
 * Static functions:
 *
 *	Create() - returns an appropriate PathSys, given an OS type flag.
 *	GetOS() - returns a string for the OS name
 */

class PathSys : public StrBuf {

    public:
    	virtual		~PathSys();

	virtual void	SetCanon( const StrPtr &root, const StrPtr &canon ) = 0;
	virtual void	SetLocal( const StrPtr &root, const StrPtr &local ) = 0;

	virtual int	GetCanon( const StrPtr &root, StrBuf &t ) = 0;
	virtual int	ToParent( StrBuf *file = 0 ) = 0;

	void		Expand();

	static PathSys *Create();
	static PathSys *Create( int os );
	static PathSys *Create( const StrPtr &os, Error *e );
	static const char *GetOS();
} ;


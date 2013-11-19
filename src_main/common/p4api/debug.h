/*
 * Copyright 1995, 2003 Perforce Software.  All rights reserved.
 *
 * This file is part of Perforce - the FAST SCM System.
 */

enum P4DebugType {
	DT_DB,		// DbOpen
	DT_DIFF,	// Diff
	DT_DM,		// Dm
	DT_FTP,		// Ftp Server
	DT_HANDLE,	// Handles
	DT_LBR,		// Lbr
	DT_MAP,		// MapTable
	DT_NET,		// Net
	DT_OPTIONS,	// Optional behavior
	DT_PROC,	// External process creation
	DT_RCS,		// RCS
	DT_RECORDS,	// VarRecords
	DT_RPC,		// Rpc
	DT_SERVER,	// Server
	DT_SPEC,	// Spec
	DT_LAST
}  ;

class P4Debug {

    public:

			P4Debug();

	void		SetLevel( int l );
	void		SetLevel( char *set );
	void		SetLevel( P4DebugType t, int l ) { level[t] = l ;}

	int		GetLevel( P4DebugType t ) const { return level[t]; }

    private:

	void		Unbuffer();

	int 		level[DT_LAST];
} ;

extern P4Debug p4debug;


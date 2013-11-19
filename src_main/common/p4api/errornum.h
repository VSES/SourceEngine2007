/*
 * Copyright 1995, 1996 Perforce Software.  All rights reserved.
 */

/*
 * Errornum.h - error number definitions
 */

enum ErrorGeneric {

	EV_NONE 	= 0,	// misc

	// The fault of the user

	EV_USAGE	= 0x01,	// request not consistent with dox
	EV_UNKNOWN	= 0x02,	// using unknown entity
	EV_CONTEXT	= 0x03,	// using entity in wrong context
	EV_ILLEGAL	= 0x04,	// trying to do something you can't
	EV_NOTYET	= 0x05,	// something must be corrected first
	EV_PROTECT	= 0x06,	// protections prevented operation

	// No fault at all

	EV_EMPTY	= 0x11,	// action returned empty results

	// not the fault of the user

	EV_FAULT	= 0x21,	// inexplicable program fault
	EV_CLIENT	= 0x22,	// client side program errors
	EV_ADMIN	= 0x23,	// server administrative action required
	EV_CONFIG	= 0x24,	// client configuration inadequate
	EV_UPGRADE	= 0x25,	// client or server too old to interact
	EV_COMM		= 0x26	// communications error

} ;

enum ErrorSubsystem {

	ES_OS		= 0,	// OS error
	ES_SUPP		= 1,	// Misc support
	ES_LBR		= 2,	// librarian
	ES_RPC		= 3,	// messaging
	ES_DB		= 4,	// database
	ES_DBSUPP	= 5,	// database support
	ES_DM		= 6,	// data manager
	ES_SERVER	= 7,	// top level of server
	ES_CLIENT	= 8,	// top level of client
	ES_INFO		= 9,	// pseudo subsystem for information messages
	ES_HELP		= 10,	// pseudo subsystem for help messages
	ES_SPEC		= 11,	// pseudo subsystem for spec/comment messages
	ES_FTPD		= 12	// P4FTP server
} ;

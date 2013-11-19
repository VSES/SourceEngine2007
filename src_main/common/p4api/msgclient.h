/*
 * Copyright 1995, 2000 Perforce Software.  All rights reserved.
 *
 * This file is part of Perforce - the FAST SCM System.
 */

/*
 * msgclient.h - definitions of errors for client subsystem.
 */

class MsgClient {

    public:

	static ErrorId Connect;
	static ErrorId BadFlag;
	static ErrorId Fatal;
	static ErrorId ClobberFile;
	static ErrorId MkDir;
	static ErrorId Eof;
	static ErrorId CantEdit;
	static ErrorId NoMerger;

	static ErrorId ToolServer2;
	static ErrorId ToolServer;
	static ErrorId ToolCmdCreate;
	static ErrorId ToolCmdSend;
	static ErrorId Memory;
	static ErrorId CantFindApp;
	static ErrorId BadSignature;

	static ErrorId ResolveManually;
	static ErrorId NonTextFileMerge;

	static ErrorId MergeMsg2;
	static ErrorId MergeMsg3;
	static ErrorId MergeMsg32;
	static ErrorId MergePrompt;
	static ErrorId MergePrompt2;
	static ErrorId MergePrompt2Edit;

	static ErrorId ConfirmMarkers;
	static ErrorId ConfirmEdit;
	static ErrorId Confirm;
} ;

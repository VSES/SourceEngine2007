/*
 * Copyright 2001 Perforce Software.  All rights reserved.
 *
 * This file is part of Perforce - the FAST SCM System.
 */

/*
 * NoEcho -- Turn terminal echoing off/on
 *
 * Declaring a NoEcho object turns off terminal echoing (if possible).
 * Deleting it turns it back on.
 */

struct EchoContext;

class NoEcho {

    public:
	NoEcho();
	~NoEcho();

    private:
	EchoContext *context;

} ;

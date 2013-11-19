/*
 * Copyright 1995, 1996 Perforce Software.  All rights reserved.
 *
 * This file is part of Perforce - the FAST SCM System.
 */

/*
 * This class has one method IsAlive() which can be called in either the
 * client or the server.
 * 
 * client:  
 *         KeepAlive is subclassed and an implementation of IsAlive() is 
 *         written that checks to see if the server request should be
 *         terminated or not.  Note, this feature can be programmed using
 *         the API but must be instantiated using SetBreak() after the call
 *         to client.Init().
 *
 * server:
 *         The server has a couple of places where long running queries can
 *         stop and check to see if the client is still waiting (i.e. has not
 *         been terminated). 
 */

class KeepAlive {

    public:
	virtual int	IsAlive() = 0;

};

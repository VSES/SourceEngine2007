//========= Copyright © 1996-2005, Valve Corporation, All rights reserved. ============//
//
// Purpose: Handles all the functions for implementing remote access to the engine
//
//=============================================================================//

#include "netadr.h"
#include "sv_ipratelimit.h"
#include "convar.h"
#include "utlrbtree.h"
#include "utlvector.h"
#include "filesystem.h"
#include "sv_log.h"

static ConVar	sv_max_queries_sec( "sv_max_queries_sec", "3.0", 0, "Maximum queries per second to respond to from a single IP address." );
static ConVar	sv_max_queries_window( "sv_max_queries_window", "30", 0, "Window over which to average queries per second averages." );
static ConVar	sv_max_queries_sec_global( "sv_max_queries_sec_global", "60", 0, "Maximum queries per second to respond to from anywhere." );
static ConVar	sv_logblocks("sv_logblocks", "0", 0, "If true when log when a query is blocked (can cause very large log files)");

class CIPRateLimit
{
public:
	CIPRateLimit();
	~CIPRateLimit();

	// updates an ip entry, return true if the ip is allowed, false otherwise
	bool CheckIP( netadr_t ip );

private:
	enum { MAX_TREE_SIZE = 1024, START_TREE_SIZE = 256, FLUSH_TIMEOUT = 120 };

	typedef int ip_t;
	struct iprate_s
	{
		ip_t ip;
		long lastTime;
		int count;
	};

	static bool LessIP( const struct CIPRateLimit::iprate_s &lhs, const struct CIPRateLimit::iprate_s &rhs );

	CUtlRBTree< struct iprate_s, ip_t > m_IPTree;
	int m_iGlobalCount;
	long m_lLastTime;
};

static CIPRateLimit rateChecker;

//-----------------------------------------------------------------------------
// Purpose: return false if this IP exceeds rate limits
//-----------------------------------------------------------------------------
bool CheckConnectionLessRateLimits( netadr_t & adr )
{
	bool ret = rateChecker.CheckIP(adr);
	if ( !ret && sv_logblocks.GetBool() == true )
	{
		g_Log.Printf("Traffic from %s was blocked for exceeding rate limits\n", adr.ToString() );	
	}
	return ret;
}

//-----------------------------------------------------------------------------
// Purpose: Constructor
//-----------------------------------------------------------------------------
CIPRateLimit::CIPRateLimit(): m_IPTree( 0, START_TREE_SIZE, LessIP)
{
	m_iGlobalCount = 0;
	m_lLastTime = -1;
}

//-----------------------------------------------------------------------------
// Purpose: Destructor
//-----------------------------------------------------------------------------
CIPRateLimit::~CIPRateLimit()
{
}

//-----------------------------------------------------------------------------
// Purpose: sort func for rb tree
//-----------------------------------------------------------------------------
bool CIPRateLimit::LessIP( const struct CIPRateLimit::iprate_s &lhs, const struct CIPRateLimit::iprate_s &rhs )
{
	return ( lhs.ip < rhs.ip );
}

//-----------------------------------------------------------------------------
// Purpose: return false if this IP has exceeded limits
//-----------------------------------------------------------------------------
bool CIPRateLimit::CheckIP( netadr_t adr )
{
	long curTime = (long)Plat_FloatTime();
	
	// check the per ip rate (do this first, so one person dosing doesn't add to the global max rate
	ip_t clientIP;
	memcpy( &clientIP, adr.ip, sizeof(ip_t) );
	
	if( m_IPTree.Count() > MAX_TREE_SIZE ) // if we have stored too many items
	{
		ip_t tmp = m_IPTree.LastInorder(); // we step BACKWARD's through the tree
		int i = m_IPTree.FirstInorder();
		while( (m_IPTree.Count() > (2*MAX_TREE_SIZE)/3) && i < m_IPTree.MaxElement() ) // trim 1/3 the entries from the tree and only traverse the max nodes
		{ 	
			if ( m_IPTree.IsValidIndex( tmp ) &&
			     (curTime - m_IPTree[ tmp ].lastTime) > FLUSH_TIMEOUT  &&
			      m_IPTree[ tmp ].ip != clientIP ) 
			{
				ip_t removeIPT = tmp;
				tmp = m_IPTree.PrevInorder( tmp );
				m_IPTree.RemoveAt( removeIPT );
				continue;
			}
			i++;
			tmp = m_IPTree.PrevInorder( tmp );
		}
	}

	// now find the entry and check if its within our rate limits
	struct iprate_s findEntry = { clientIP };
	ip_t entry = m_IPTree.Find( findEntry );

	if( m_IPTree.IsValidIndex( entry ) )
	{
		m_IPTree[ entry ].count++; // a new hit

		if( (curTime - m_IPTree[ entry ].lastTime) > sv_max_queries_window.GetFloat() )
		{
			m_IPTree[ entry ].lastTime = curTime;
			m_IPTree[ entry ].count = 1;
		}
		else
		{
			float query_rate = static_cast<float>( m_IPTree[ entry ].count) / sv_max_queries_window.GetFloat(); // add one so the bottom is never zero
			if( query_rate > sv_max_queries_sec.GetFloat() ) 
			{
				return false;
			}
		}
	}
	else
	{	
		// not found, insert this new guy
		struct iprate_s newEntry;
		newEntry.count = 1;
		newEntry.lastTime = curTime;
		newEntry.ip = clientIP;
		m_IPTree.Insert( newEntry );
	}

	// now check the global rate
	m_iGlobalCount++;

	if( (curTime - m_lLastTime) > sv_max_queries_window.GetFloat() )
	{
		m_lLastTime = curTime;
		m_iGlobalCount = 1;
	}
	else
	{
		float query_rate = static_cast<float>( m_iGlobalCount ) / sv_max_queries_window.GetFloat(); // add one so the bottom is never zero
		if( query_rate > sv_max_queries_sec_global.GetFloat() ) 
		{
			return false;
		}
	}

	return true;
}

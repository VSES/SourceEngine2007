//========= Copyright © 1996-2005, Valve Corporation, All rights reserved. ============//
//
// Purpose: rate limits OOB server queries
//
//=============================================================================//

// returns false if this IP exceeds rate limits
bool CheckConnectionLessRateLimits( netadr_t & adr );


//--------------------------------------------------------------------------------------
// Copyright (c) Microsoft Corporation. All rights reserved.
//--------------------------------------------------------------------------------------
#include "stdafx.h"
#include "unit.h"
using namespace Sas;

map<const WCHAR*, Unit, StringCompareInsensitive> UnitHelper::s_UnitMap;


//--------------------------------------------------------------------------------------
Unit UnitHelper::AbbreviationToUnit( const WCHAR* abbreviation )
{
    // Make sure the map is initialized
    if( 0 == s_UnitMap.size() )
        InitializeUnitMap();
    
    // TODO - Remove spaces

    // Search the map
    UnitMap::iterator it = s_UnitMap.find( abbreviation );
    if( s_UnitMap.end() == it )
        return Unit_Invalid;

    // Unit was found
    return (*it).second;
}


//--------------------------------------------------------------------------------------
const WCHAR* UnitHelper::UnitToAbbreviation( Unit unit ) 
{ 
    if( unit < 0 || unit >= Unit_END )
        return L"(INVALID)";

    return UnitAbbreviations[unit]; 
}


//--------------------------------------------------------------------------------------
void UnitHelper::InitializeUnitMap()
{
    for( Unit u=(Unit)0; u < Unit_END; u = (Unit)((int)u+1) )
    {
        s_UnitMap[ UnitAbbreviations[u] ] = u;
    }
}
//--------------------------------------------------------------------------------------
// Copyright (c) Microsoft Corporation. All rights reserved.
//--------------------------------------------------------------------------------------
#ifndef _UNIT_H_
#define _UNIT_H_
//#include <map>
using namespace std;

namespace Sas
{
	enum Unit
	{
        // Unitless
        Unit_None,
    
        // Distance
        Unit_Millimeters,
        Unit_Centimeters,
        Unit_Meters,
        Unit_Kilometers,

        // Angle
        Unit_Radians,

        // Time
        Unit_Milliseconds,
        Unit_Seconds,
        Unit_Minutes,
        Unit_Hours,

        // Linear velocity
        Unit_MillimetersPerSecond,
        Unit_CentimetersPerSecond,
        Unit_MetersPerSecond,
        Unit_MetersPerHour,
        Unit_KilometersPerHour,

        // Linear acceleration
        Unit_MillimetersPerSecondSquared,
        Unit_CentimetersPerSecondSquared,
        Unit_MetersPerSecondSquared,
        Unit_MetersPerHourSquared,
        Unit_KilometersPerHourSquared,

        // Angular velocity
        Unit_RadiansPerSecond,

        // Angular acceleration
        Unit_RadiansPerSecondSquared,

        // Area
        Unit_MillimetersSquared,
        Unit_CentimetersSquared,
        Unit_MetersSquared,
        Unit_KilometersSquared,

        // Volume
        Unit_MillimetersCubed,
        Unit_CentimetersCubed,
        Unit_MetersCubed,
        Unit_KilometersCubed,

        Unit_END,
        Unit_Invalid
	};


	static const WCHAR* UnitAbbreviations[] =
	{
        // Unitless
        L"none",

        // Distance
        L"mm", L"cm", L"m", L"km",

        // Angle
        L"rad",

        // Time
        L"ms", L"sec", L"min", L"hr",

        // Linear velocity
        L"mm/sec", L"cm/sec", L"m/sec", L"m/hr", L"km/hr",

        // Linear acceleration
        L"mm/sec^2", L"cm/sec^2", L"m/sec^2", L"m/hr^2", L"km/hr^2",

        // Angular velocity
        L"rad/sec",

        // Angular acceleration
        L"rad/sec^2",

        // Area
        L"mm^2", L"cm^2", L"m^2", L"km^2",
        
        // Volume
        L"mm^3", L"cm^3", L"m^3", L"km^3"
    };


	class UnitHelper
	{
	public:
		static Unit AbbreviationToUnit( const WCHAR* abbreviation );
		static const WCHAR* UnitToAbbreviation( Unit unit ); 

    private:
        static void InitializeUnitMap();

        typedef map<const WCHAR*, Unit, StringCompareInsensitive> UnitMap;
	    static UnitMap s_UnitMap;
	};
}

#endif //_UNIT_H_
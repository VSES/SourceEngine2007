//========= Copyright © 1996-2006, Valve Corporation, All rights reserved. ============//
//
// Purpose: VPC
//
//=====================================================================================//

#include "vpc.h"

bool VPC_Config_Resources_PreprocessorDefinitions( const char *pPropertyName )
{
	SET_STRING_PROPERTY( pPropertyName, g_spResources, get_PreprocessorDefinitions, put_PreprocessorDefinitions );
}

bool VPC_Config_Resources_Culture( const char *pPropertyName )
{
	char	buff[MAX_SYSTOKENCHARS];

	if ( !ParsePropertyValue( &g_pScriptData, g_pScriptLine, NULL, buff, sizeof( buff ) ) )
		return true;

	enumResourceLangID option = rcUseDefault;
	if ( !stricmp( buff, "Default" ) )
		option = rcUseDefault;
	else if ( !stricmp( buff, "Afrikaans (0x436)" ) )
		option = rcAfrikaans;
	else if ( !stricmp( buff, "Albanian (0x41c)" ) )
		option = rcAlbanian;
	else if ( !stricmp( buff, "Arabic (Saudi Arabia) (0x401)" ) )
		option = rcArabicSaudi;
	else if ( !stricmp( buff, "Arabic (Iraq) (0x801)" ) )
		option = rcArabicIraq;
	else if ( !stricmp( buff, "Arabic (Egypt) (0xc01)" ) )
		option = rcArabicEgypt;
	else if ( !stricmp( buff, "Arabic (Libya) (0x1001)" ) )
		option = rcArabicLibya;
	else if ( !stricmp( buff, "Arabic (Algeria) (0x1401)" ) )
		option = rcArabicAlgeria;
	else if ( !stricmp( buff, "Arabic (Morocco) (0x1801)" ) )
		option = rcArabicMorocco;
	else if ( !stricmp( buff, "Arabic (Tunisia) (0x1c01)" ) )
		option = rcArabicTunisia;
	else if ( !stricmp( buff, "Arabic (Oman) (0x2001)" ) )
		option = rcArabicOman;
	else if ( !stricmp( buff, "Arabic (Yemen) (0x2401)" ) )
		option = rcArabicYemen;
	else if ( !stricmp( buff, "Arabic (Syria) (0x2801)" ) )
		option = rcArabicSyria;
	else if ( !stricmp( buff, "Arabic (Jordan) (0x2c01)" ) )
		option = rcArabicJordan;
	else if ( !stricmp( buff, "Arabic (Lebanon) (0x3001)" ) )
		option = rcArabicLebanon;
	else if ( !stricmp( buff, "Arabic (Kuwait) (0x3401)" ) )
		option = rcArabicKuwait;
	else if ( !stricmp( buff, "Arabic (U.A.E.) (0x3801)" ) )
		option = rcArabicUnitedArabEmirates;
	else if ( !stricmp( buff, "Arabic (Bahrain) (0x3c01)" ) )
		option = rcArabicBahrain;
	else if ( !stricmp( buff, "Arabic (Qatar) (0x4001)" ) )
		option = rcArabicQatar;
	else if ( !stricmp( buff, "Basque (0x42d)" ) )
		option = rcBasque;
	else if ( !stricmp( buff, "Bulgarian (0x402)" ) )
		option = rcBulgarian;
	else if ( !stricmp( buff, "Belarusian (0x423)" ) )
		option = rcByelorussian;
	else if ( !stricmp( buff, "Catalan (0x403)" ) )
		option = rcCatalan;
	else if ( !stricmp( buff, "Chinese (Taiwan) (0x404)" ) )
		option = rcChineseTaiwan;
	else if ( !stricmp( buff, "Chinese (PRC) (0x804)" ) )
		option = rcChinesePRC;
	else if ( !stricmp( buff, "Chinese (Hong Kong S.A.R.) (0xc04)" ) )
		option = rcChineseHongKong;
	else if ( !stricmp( buff, "Chinese (Singapore) (0x1004)" ) )
		option = rcChineseSingapore;
	else if ( !stricmp( buff, "Croatian (0x41a)" ) )
		option = rcCroatian;
	else if ( !stricmp( buff, "Czech (0x405)" ) )
		option = rcCzech;
	else if ( !stricmp( buff, "Danish (0x406)" ) )
		option = rcDanish;
	else if ( !stricmp( buff, "Dutch (Netherlands) (0x413)" ) )
		option = rcDutchStandard;
	else if ( !stricmp( buff, "Dutch (Belgium) (0x813)" ) )
		option = rcDutchBelgium;
	else if ( !stricmp( buff, "English (United States) (0x409)" ) )
		option = rcEnglishUS;
	else if ( !stricmp( buff, "English (United Kingdom) (0x809)" ) )
		option = rcEnglishBritain;
	else if ( !stricmp( buff, "English (Australia) (0xc09)" ) )
		option = rcEnglishAustralia;
	else if ( !stricmp( buff, "English (Canada) (0x1009)" ) )
		option = rcEnglishCanada;
	else if ( !stricmp( buff, "English (New Zealand) (0x1409)" ) )
		option = rcEnglishNewZealand;
	else if ( !stricmp( buff, "English (Ireland) (0x1809)" ) )
		option = rcEnglishIreland;
	else if ( !stricmp( buff, "English (South Africa) (0x1c09)" ) )
		option = rcEnglishSouthAfrica;
	else if ( !stricmp( buff, "English (Jamaica) (0x2009)" ) )
		option = rcEnglishJamaica;
	else if ( !stricmp( buff, "English (Caribbean) (0x2409)" ) )
		option = rcEnglishCarribean;
	else if ( !stricmp( buff, "Estonian (0x425)" ) )
		option = rcEstonian;
	else if ( !stricmp( buff, "Farsi (0x429)" ) )
		option = rcFarsi;
	else if ( !stricmp( buff, "Finnish (0x40b)" ) )
		option = rcFinnish;
	else if ( !stricmp( buff, "French (France) (0x40c)" ) )
		option = rcFrenchStandard;
	else if ( !stricmp( buff, "French (Belgium) (0x80c)" ) )
		option = rcFrenchBelgium;
	else if ( !stricmp( buff, "French (Canada) (0xc0c)" ) )
		option = rcFrenchCanada;
	else if ( !stricmp( buff, "French (Switzerland) (0x100c)" ) )
		option = rcFrenchSwitzerland;
	else if ( !stricmp( buff, "French (Luxembourg) (0x140c)" ) )
		option = rcFrenchLuxembourg;
	else if ( !stricmp( buff, "German (Germany) (0x407)" ) )
		option = rcGermanStandard;
	else if ( !stricmp( buff, "German (Switzerland) (0x807)" ) )
		option = rcGermanSwitzerland;
	else if ( !stricmp( buff, "German (Austria) (0xc07)" ) )
		option = rcGermanAustria;
	else if ( !stricmp( buff, "German (Luxembourg) (0x1007)" ) )
		option = rcGermanLuxembourg;
	else if ( !stricmp( buff, "German (Liechtenstein) (0x1407)" ) )
		option = rcGermanLichtenstein;
	else if ( !stricmp( buff, "Greek (0x408)" ) )
		option = rcGreek;
	else if ( !stricmp( buff, "Hebrew (0x40d)" ) )
		option = rcHebrew;
	else if ( !stricmp( buff, "Hungarian (0x40e)" ) )
		option = rcHungarian;
	else if ( !stricmp( buff, "Icelandic (0x40f)" ) )
		option = rcIcelandic;
	else if ( !stricmp( buff, "Indonesian (0x421)" ) )
		option = rcIndonesian;
	else if ( !stricmp( buff, "Italian (Italy) (0x410)" ) )
		option = rcItalianStandard;
	else if ( !stricmp( buff, "Italian (Switzerland) (0x810)" ) )
		option = rcItalianSwitzerland;
	else if ( !stricmp( buff, "Japanese (0x411)" ) )
		option = rcJapanese;
	else if ( !stricmp( buff, "Korean (0x412)" ) )
		option = rcKorean;
	else if ( !stricmp( buff, "0x812" ) )
		option = rcKoreanJohab;
	else if ( !stricmp( buff, "Latvian (0x426)" ) )
		option = rcLatvian;
	else if ( !stricmp( buff, "Lithuanian (0x427)" ) )
		option = rcLithuanian;
	else if ( !stricmp( buff, "Norwegian (Bokmal) (0x414)" ) )
		option = rcNorwegianBokmal;
	else if ( !stricmp( buff, "Norwegian (Nynorsk) (0x814)" ) )
		option = rcNorwegianNynorsk;
	else if ( !stricmp( buff, "Polish (0x415)" ) )
		option = rcPolish;
	else if ( !stricmp( buff, "Portuguese (Brazil) (0x416)" ) )
		option = rcPortugueseBrazilian;
	else if ( !stricmp( buff, "Portuguese (Portugal) (0x816)" ) )
		option = rcPortugueseStandard;
	else if ( !stricmp( buff, "Romanian (0x418)" ) )
		option = rcRomanian;
	else if ( !stricmp( buff, "Russian (0x419)" ) )
		option = rcRussian;
	else if ( !stricmp( buff, "Slovak (0x41b)" ) )
		option = rcSlovak;
	else if ( !stricmp( buff, "Spanish (Traditional Sort) (0x40a)" ) )
		option = rcSpanishTraditional;
	else if ( !stricmp( buff, "Spanish (Mexico) (0x80a)" ) )
		option = rcSpanishMexico;
	else if ( !stricmp( buff, "Spanish (International Sort) (0xc0a)" ) )
		option = rcSpanishModern;
	else if ( !stricmp( buff, "Spanish (Guatemala) (0x100a)" ) )
		option = rcSpanishGuatemala;
	else if ( !stricmp( buff, "Spanish (Costa Rica) (0x140a)" ) )
		option = rcSpanishCostaRica;
	else if ( !stricmp( buff, "Spanish (Panama) (0x180a)" ) )
		option = rcSpanishPanama;
	else if ( !stricmp( buff, "Spanish (Dominican Republic) (0x1c0a)" ) )
		option = rcSpanishDominicanRepublic;
	else if ( !stricmp( buff, "Spanish (Venezuela) (0x200a)" ) )
		option = rcSpanishVenezuela;
	else if ( !stricmp( buff, "Spanish (Colombia) (0x240a)" ) )
		option = rcSpanishColombia;
	else if ( !stricmp( buff, "Spanish (Peru) (0x280a)" ) )
		option = rcSpanishPeru;
	else if ( !stricmp( buff, "Spanish (Argentina) (0x2c0a)" ) )
		option = rcSpanishArgentina;
	else if ( !stricmp( buff, "Spanish (Ecuador) (0x300a)" ) )
		option = rcSpanishEcuador;
	else if ( !stricmp( buff, "Spanish (Chile) (0x340a)" ) )
		option = rcSpanishChile;
	else if ( !stricmp( buff, "Spanish (Uruguay) (0x380a)" ) )
		option = rcSpanishUruguay;
	else if ( !stricmp( buff, "Spanish (Paraguay) (0x3c0a)" ) )
		option = rcSpanishParaguay;
	else if ( !stricmp( buff, "Spanish (Bolivia) (0x400a)" ) )
		option = rcSpanishBolivia;
	else if ( !stricmp( buff, "Swedish (0x41d)" ) )
		option = rcSwedish;
	else if ( !stricmp( buff, "Thai (0x41e)" ) )
		option = rcThai;
	else if ( !stricmp( buff, "Turkish (0x41f)" ) )
		option = rcTurkish;
	else if ( !stricmp( buff, "Ukrainian (0x422)" ) )
		option = rcUkrainian;
	else if ( !stricmp( buff, "Serbian (Latin) (0x81a)" ) )
		option = rcSerbian;
	else if ( !stricmp( buff, "Urdu (0x420)" ) )
		option = rcUrdu;
	else
		VPC_SyntaxError();

	SET_LIST_PROPERTY( pPropertyName, g_spResources, get_Culture, put_Culture, enumResourceLangID, option );
}

bool VPC_Config_Resources_AdditionalIncludeDirectories( const char *pPropertyName )
{
	SET_STRING_PROPERTY( pPropertyName, g_spResources, get_AdditionalIncludeDirectories, put_AdditionalIncludeDirectories );
}

bool VPC_Config_Resources_IgnoreStandardIncludePath( const char *pPropertyName )
{
	SET_BOOL_PROPERTY( pPropertyName, g_spResources, get_IgnoreStandardIncludePath, put_IgnoreStandardIncludePath );
}

bool VPC_Config_Resources_ShowProgress( const char *pPropertyName )
{
	SET_BOOL_PROPERTY( pPropertyName, g_spResources, get_ShowProgress, put_ShowProgress );
}

bool VPC_Config_Resources_ResourceFileName( const char *pPropertyName )
{
	SET_STRING_PROPERTY( pPropertyName, g_spResources, get_ResourceOutputFileName, put_ResourceOutputFileName );
}

bool VPC_Config_Resources_AdditionalOptions( const char *pPropertyName )
{
	SET_STRING_PROPERTY( pPropertyName, g_spResources, get_AdditionalOptions, put_AdditionalOptions );
}

property_t g_resourcesProperties[] =
{
	// General
	{"$PreprocessorDefinitions",		VPC_Config_Resources_PreprocessorDefinitions, PLATFORM_WIN32},
	{"$Culture",						VPC_Config_Resources_Culture, PLATFORM_WIN32},
	{"$AdditionalIncludeDirectories",	VPC_Config_Resources_AdditionalIncludeDirectories, PLATFORM_WIN32},
	{"$IgnoreStandardIncludePath",		VPC_Config_Resources_IgnoreStandardIncludePath, PLATFORM_WIN32},
	{"$ShowProgress",					VPC_Config_Resources_ShowProgress, PLATFORM_WIN32},
	{"$ResourceFileName",				VPC_Config_Resources_ResourceFileName, PLATFORM_WIN32},

	// Command Line
	{"$AdditionalOptions",				VPC_Config_Resources_AdditionalOptions, PLATFORM_WIN32},			
	{NULL}
};
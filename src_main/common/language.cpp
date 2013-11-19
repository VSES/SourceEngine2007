//====== Copyright © 1996-2004, Valve Corporation, All rights reserved. =======
//
// Purpose: languages definition
//
//=============================================================================

#include "language.h"
#include "tier0/dbg.h"
#include "tier1/strtools.h"

struct Language_t
{	
	char *m_pchName;
	char *m_pchShortName;
	char *m_pchVGUILocalizationName;
	ELanguage m_ELanguage;
};


static Language_t s_LanguageNames[] = 
{
	{	"None",					"none",			"None",										k_Lang_None },
	{	"English",				"english",		"#GameUI_Language_English",				k_Lang_English },
	{	"German",				"german",		"#GameUI_Language_German",				k_Lang_German } ,
	{	"French",				"french",		"#GameUI_Language_French",				k_Lang_French } ,
	{	"Italian",				"italian",		"#GameUI_Language_Italian",				k_Lang_Italian } ,
	{	"Korean",				"koreana",		"#GameUI_Language_Korean",				k_Lang_Korean } ,
	{	"Spanish",				"spanish",		"#GameUI_Language_Spanish",				k_Lang_Spanish },
	{	"Simplified_Chinese",	"schinese",		"#GameUI_Language_Simplified_Chinese",	k_Lang_Simplified_Chinese } ,
	{	"Traditional_Chinese",	"tchinese",		"#GameUI_Language_Traditional_Chinese",	k_Lang_Traditional_Chinese } ,
	{	"Russian",				"russian",		"#GameUI_Language_Russian",				k_Lang_Russian } ,
	{	"Thai",					"thai",			"#GameUI_Language_Thai",					k_Lang_Thai } ,
	{	"Japanese",				"japanese",		"#GameUI_Language_Japanese",				k_Lang_Japanese } ,
	{	"Portuguese",			"portuguese",	"#GameUI_Language_Portuguese",			k_Lang_Portuguese } ,
	{	"Polish",				"polish",		"#GameUI_Language_Polish",				k_Lang_Polish } ,
	{	"Danish",				"danish",		"#GameUI_Language_Danish",				k_Lang_Danish } ,
	{	"Dutch",				"dutch",		"#GameUI_Language_Dutch",					k_Lang_Dutch } ,
	{	"Finnish",				"finnish",		"#GameUI_Language_Finnish",				k_Lang_Finnish } ,
	{	"Norwegian",			"norwegian",	"#GameUI_Language_Norwegian",				k_Lang_Norwegian } ,
	{	"Swedish",				"swedish",		"#GameUI_Language_Swedish",				k_Lang_Swedish } ,
};


//-----------------------------------------------------------------------------
// STATIC
// Purpose: find the language by name
//-----------------------------------------------------------------------------
ELanguage PchLanguageToELanguage( const char *pchShortName )
{
	Assert( ARRAYSIZE(s_LanguageNames) == k_Lang_MAX + 1 );
	if ( !pchShortName )
		return k_Lang_English;

	for ( int iLang = 0; iLang < Q_ARRAYSIZE(s_LanguageNames); ++iLang )
	{
		if ( !Q_stricmp( pchShortName, s_LanguageNames[iLang].m_pchShortName ) )
		{
			return s_LanguageNames[iLang].m_ELanguage;
		}
	}

	// default to English
	return k_Lang_English;
}


//-----------------------------------------------------------------------------
// Purpose: return the short string name used for this language by SteamUI
//-----------------------------------------------------------------------------
const char *GetLanguageShortName( ELanguage eLang )
{
	Assert( Q_ARRAYSIZE(s_LanguageNames) == k_Lang_MAX + 1 );
	if ( s_LanguageNames[ eLang + 1 ].m_ELanguage == eLang )
	{
		return s_LanguageNames[ eLang + 1 ].m_pchShortName;
	}

	Assert( !"enum ELanguage order mismatched from Language_t s_LanguageNames, fix it!" );
	return s_LanguageNames[0].m_pchShortName;
}

//-----------------------------------------------------------------------------
// Purpose: return the short string name used for this language by SteamUI
//-----------------------------------------------------------------------------
const char *GetLanguageName( ELanguage eLang )
{
   Assert( Q_ARRAYSIZE(s_LanguageNames) == k_Lang_MAX + 1 );
   if ( s_LanguageNames[ eLang + 1 ].m_ELanguage == eLang )
   {
      return s_LanguageNames[ eLang + 1 ].m_pchName;
   }

   Assert( !"enum ELanguage order mismatched from Language_t s_LanguageNames, fix it!" );
   return s_LanguageNames[0].m_pchShortName;
}


//-----------------------------------------------------------------------------
// Purpose: return the short string name used for this language by SteamUI
//-----------------------------------------------------------------------------
const char *GetLanguageVGUILocalization( ELanguage eLang )
{
	Assert( Q_ARRAYSIZE(s_LanguageNames) == k_Lang_MAX + 1 );
	if ( s_LanguageNames[ eLang + 1 ].m_ELanguage == eLang )
	{
		return s_LanguageNames[ eLang + 1 ].m_pchVGUILocalizationName;
	}

	Assert( !"enum ELanguage order mismatched from Language_t s_LanguageNames, fix it!" );
	return s_LanguageNames[0].m_pchVGUILocalizationName;
}


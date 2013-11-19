//====== Copyright © 1996-2004, Valve Corporation, All rights reserved. =======
//
// Purpose: represent a canonical list of the languages we support, 
//
//=============================================================================

#ifndef LANG_H
#define LANG_H
#ifdef _WIN32
#pragma once
#endif

// if you change this enum also change language.cpp:s_LanguageNames
enum ELanguage
{
	k_Lang_None = -1,
	k_Lang_English = 0,
	k_Lang_German,
	k_Lang_French,
	k_Lang_Italian,
	k_Lang_Korean,
	k_Lang_Spanish,
	k_Lang_Simplified_Chinese,
	k_Lang_Traditional_Chinese,
	k_Lang_Russian,
	k_Lang_Thai,
	k_Lang_Japanese,
	k_Lang_Portuguese,
	k_Lang_Polish,
	k_Lang_Danish,
	k_Lang_Dutch,
	k_Lang_Finnish,
	k_Lang_Norwegian,
	k_Lang_Swedish,
	k_Lang_MAX
};

ELanguage PchLanguageToELanguage(const char *pchShortName);
const char *GetLanguageShortName( ELanguage eLang );
const char *GetLanguageVGUILocalization( ELanguage eLang );
const char *GetLanguageName( ELanguage eLang );

#endif /* LANG_H */
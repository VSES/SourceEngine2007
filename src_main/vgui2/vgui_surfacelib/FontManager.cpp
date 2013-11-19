//========= Copyright © 1996-2005, Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
//=====================================================================================//

#include <locale.h>
#include "vgui_surfacelib/BitmapFont.h"
#include "vgui_surfacelib/FontManager.h"
#include <vgui/ISurface.h>
#include <tier0/dbg.h>

// memdbgon must be the last include file in a .cpp file!!!
#include <tier0/memdbgon.h>

static CFontManager s_FontManager;

extern bool s_bSupportsUnicode;

#if !defined( _X360 )
#define MAX_INITIAL_FONTS	100
#else
#define MAX_INITIAL_FONTS	1
#endif

//-----------------------------------------------------------------------------
// Purpose: singleton accessor
//-----------------------------------------------------------------------------
CFontManager &FontManager()
{
	return s_FontManager;
}

//-----------------------------------------------------------------------------
// Purpose: Constructor
//-----------------------------------------------------------------------------
CFontManager::CFontManager()
{
	// add a single empty font, to act as an invalid font handle 0
	m_FontAmalgams.EnsureCapacity( MAX_INITIAL_FONTS );
	m_FontAmalgams.AddToTail();
	m_Win32Fonts.EnsureCapacity( MAX_INITIAL_FONTS );

	// setup our text locale
	setlocale( LC_CTYPE, "" );
	setlocale( LC_TIME, "" );
	setlocale( LC_COLLATE, "" );
	setlocale( LC_MONETARY, "" );

	m_pFileSystem = NULL;
	m_pMaterialSystem = NULL;
}

//-----------------------------------------------------------------------------
// Purpose: language setting for font fallbacks
//-----------------------------------------------------------------------------
void CFontManager::SetLanguage(const char *language)
{
	Q_strncpy(m_szLanguage, language, sizeof(m_szLanguage));
}

//-----------------------------------------------------------------------------
// Purpose: Destructor
//-----------------------------------------------------------------------------
CFontManager::~CFontManager()
{
	ClearAllFonts();
}

//-----------------------------------------------------------------------------
// Purpose: frees the fonts
//-----------------------------------------------------------------------------
void CFontManager::ClearAllFonts()
{
	// free the fonts
	for (int i = 0; i < m_Win32Fonts.Count(); i++)
	{
		delete m_Win32Fonts[i];
	}
	m_Win32Fonts.RemoveAll();
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
vgui::HFont CFontManager::CreateFont()
{
	int i = m_FontAmalgams.AddToTail();
	return i;
}

//-----------------------------------------------------------------------------
// Purpose: Sets the valid glyph ranges for a font created by CreateFont()
//-----------------------------------------------------------------------------
bool CFontManager::SetFontGlyphSet(HFont font, const char *windowsFontName, int tall, int weight, int blur, int scanlines, int flags)
{
	// ignore all but the first font added
	// need to rev vgui versions and change the name of this function
	if ( m_FontAmalgams[font].GetCount() > 0 )
	{
		// clear any existing fonts
		m_FontAmalgams[font].RemoveAll();
	}

	CWin32Font *winFont = CreateOrFindWin32Font( windowsFontName, tall, weight, blur, scanlines, flags );

	// cycle until valid english/asian font support has been created
	do
	{
		// add to the amalgam
		if ( IsFontForeignLanguageCapable( windowsFontName ) )
		{
			if ( winFont )
			{
				// font supports the full range of characters
				m_FontAmalgams[font].AddFont( winFont, 0x0000, 0xFFFF );
				return true;
			}
		}
		else
		{
			// font cannot provide asian glyphs and just supports the normal range
			// create the asian version of the font
			const char *localizedFontName = GetForeignFallbackFontName();
			if ( winFont && !stricmp( localizedFontName, windowsFontName ) )
			{
				// it's the same font and can support the full range
				m_FontAmalgams[font].AddFont( winFont, 0x0000, 0xFFFF );
				return true;
			}

			// create the asian support font
			CWin32Font *asianFont = CreateOrFindWin32Font( localizedFontName, tall, weight, blur, scanlines, flags );
			if ( winFont && asianFont )
			{
				// use the normal font for english characters, and the asian font for the rest
				m_FontAmalgams[font].AddFont( winFont, 0x0000, 0x00FF );
				m_FontAmalgams[font].AddFont( asianFont, 0x0100, 0xFFFF );
				return true;
			}
			else if ( asianFont )
			{
				// the normal font failed to create
				// just use the asian font for the full range
				m_FontAmalgams[font].AddFont( asianFont, 0x0000, 0xFFFF );
				return true;
			}
		}
		// no valid font has been created, so fallback to a different font and try again
	} 
	while ( NULL != ( windowsFontName = GetFallbackFontName( windowsFontName ) ) );

	// nothing successfully created
	return false;
}

//-----------------------------------------------------------------------------
// Purpose: adds glyphs to a font created by CreateFont()
//-----------------------------------------------------------------------------
bool CFontManager::SetBitmapFontGlyphSet(HFont font, const char *windowsFontName, float scalex, float scaley, int flags)
{
	if ( m_FontAmalgams[font].GetCount() > 0 )
	{
		// clear any existing fonts
		m_FontAmalgams[font].RemoveAll();
	}

	CBitmapFont *winFont = CreateOrFindBitmapFont( windowsFontName, scalex, scaley, flags );
	if ( winFont )
	{
		// bitmap fonts are only 0-255
		m_FontAmalgams[font].AddFont( winFont, 0x0000, 0x00FF );
		return true;
	}

	// nothing successfully created
	return false;
}

//-----------------------------------------------------------------------------
// Purpose: Creates a new win32 font, or reuses one if possible
//-----------------------------------------------------------------------------
CWin32Font *CFontManager::CreateOrFindWin32Font(const char *windowsFontName, int tall, int weight, int blur, int scanlines, int flags)
{
	// see if we already have the win32 font
	CWin32Font *winFont = NULL;
	int i;
	for (i = 0; i < m_Win32Fonts.Count(); i++)
	{
		if (m_Win32Fonts[i]->IsEqualTo(windowsFontName, tall, weight, blur, scanlines, flags))
		{
			winFont = m_Win32Fonts[i];
			break;
		}
	}

	// create the new win32font if we didn't find it
	if (!winFont)
	{
		MEM_ALLOC_CREDIT();

		i = m_Win32Fonts.AddToTail();

		m_Win32Fonts[i] = new CWin32Font();
		if (m_Win32Fonts[i]->Create(windowsFontName, tall, weight, blur, scanlines, flags))
		{
			// add to the list
			winFont = m_Win32Fonts[i];
		}
		else
		{
			// failed to create, remove
			delete m_Win32Fonts[i];
			m_Win32Fonts.Remove(i);
			return NULL;
		}
	}

	return winFont;
}

//-----------------------------------------------------------------------------
// Purpose: Creates a new win32 font, or reuses one if possible
//-----------------------------------------------------------------------------
CBitmapFont *CFontManager::CreateOrFindBitmapFont(const char *windowsFontName, float scalex, float scaley, int flags)
{
	// see if we already have the font
	CBitmapFont *winFont = NULL;
	int i;
	for ( i = 0; i < m_Win32Fonts.Count(); i++ )
	{
		CWin32Font *font = m_Win32Fonts[i];

		// Only looking for bitmap fonts
		int testflags = font->GetFlags();
		if ( ~testflags & vgui::ISurface::FONTFLAG_BITMAP )
			continue;

		CBitmapFont *bitmapFont = reinterpret_cast< CBitmapFont* >( font );
		if ( bitmapFont->IsEqualTo( windowsFontName, scalex, scaley, flags ) )
		{
			winFont = bitmapFont;
			break;
		}
	}

	// create the font if we didn't find it
	if ( !winFont )
	{
		MEM_ALLOC_CREDIT();

		i = m_Win32Fonts.AddToTail();

		CBitmapFont *bitmapFont = new CBitmapFont();
		if ( bitmapFont->Create( windowsFontName, scalex, scaley, flags ) )
		{
			// add to the list
			m_Win32Fonts[i] = bitmapFont;
			winFont = bitmapFont;
		}
		else
		{
			// failed to create, remove
			delete bitmapFont;
			m_Win32Fonts.Remove(i);
			return NULL;
		}
	}

	return winFont;
}

//-----------------------------------------------------------------------------
// Purpose: sets the scale of a bitmap font
//-----------------------------------------------------------------------------
void CFontManager::SetFontScale(vgui::HFont font, float sx, float sy)
{
	m_FontAmalgams[font].SetFontScale( sx, sy );
}

//-----------------------------------------------------------------------------
// Purpose: finds a font handle given a name, returns 0 if not found
//-----------------------------------------------------------------------------
vgui::HFont CFontManager::GetFontByName(const char *name)
{
	for (int i = 1; i < m_FontAmalgams.Count(); i++)
	{
		if (!stricmp(name, m_FontAmalgams[i].Name()))
		{
			return i;
		}
	}
	return 0;
}

//-----------------------------------------------------------------------------
// Purpose: gets the windows font for the particular font in the amalgam
//-----------------------------------------------------------------------------
CWin32Font *CFontManager::GetFontForChar(vgui::HFont font, wchar_t wch)
{
	return m_FontAmalgams[font].GetFontForChar(wch);
}

//-----------------------------------------------------------------------------
// Purpose: returns the abc widths of a single character
//-----------------------------------------------------------------------------
void CFontManager::GetCharABCwide(HFont font, int ch, int &a, int &b, int &c)
{
	CWin32Font *winFont = m_FontAmalgams[font].GetFontForChar(ch);
	if (winFont)
	{
		winFont->GetCharABCWidths(ch, a, b, c);
	}
	else
	{
		// no font for this range, just use the default width
		a = c = 0;
		b = m_FontAmalgams[font].GetFontMaxWidth();
	}
}

//-----------------------------------------------------------------------------
// Purpose: returns the max height of a font
//-----------------------------------------------------------------------------
int CFontManager::GetFontTall(HFont font)
{
	return m_FontAmalgams[font].GetFontHeight();
}

//-----------------------------------------------------------------------------
// Purpose: returns the ascent of a font
//-----------------------------------------------------------------------------
int CFontManager::GetFontAscent(HFont font, wchar_t wch)
{
	CWin32Font *winFont = m_FontAmalgams[font].GetFontForChar(wch);
	if ( winFont )
	{
		return winFont->GetAscent();
	}
	else
	{
		return 0;
	}
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
bool CFontManager::IsFontAdditive(HFont font)
{
	return ( m_FontAmalgams[font].GetFlags( 0 ) & vgui::ISurface::FONTFLAG_ADDITIVE ) ? true : false;
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
bool CFontManager::IsBitmapFont(HFont font)
{
	// A FontAmalgam is either some number of non-bitmap fonts, or a single bitmap font - so this check is valid
	return ( m_FontAmalgams[font].GetFlags( 0 ) & vgui::ISurface::FONTFLAG_BITMAP ) ? true : false;
}

//-----------------------------------------------------------------------------
// Purpose: returns the pixel width of a single character
//-----------------------------------------------------------------------------
int CFontManager::GetCharacterWidth(HFont font, int ch)
{
	if ( !iswcntrl( ch ) )
	{
		int a, b, c;
		GetCharABCwide(font, ch, a, b, c);
		return (a + b + c);
	}
	return 0;
}

//-----------------------------------------------------------------------------
// Purpose: returns the area of a text string, including newlines
//-----------------------------------------------------------------------------
void CFontManager::GetTextSize(HFont font, const wchar_t *text, int &wide, int &tall)
{
	wide = 0;
	tall = 0;
	
	if (!text)
		return;

	tall = GetFontTall(font);
	
	int xx = 0;
	for (int i = 0; ; i++)
	{
		wchar_t ch = text[i];
		if (ch == 0)
		{
			break;
		}
		else if (ch == '\n')
		{
			tall += GetFontTall(font);
			xx=0;
		}
		else if (ch == '&')
		{
			// underscore character, so skip
		}
		else
		{
			xx += GetCharacterWidth(font, ch);
			if (xx > wide)
			{
				wide = xx;
			}
		}
	}
}

// font validation functions
struct FallbackFont_t
{
	const char *font;
	const char *fallbackFont;
};

const char *g_szValidAsianFonts[] = { "Marlett", NULL };

// list of how fonts fallback
FallbackFont_t g_FallbackFonts[] =
{
	{ "Times New Roman", "Courier New" },
	{ "Courier New", "Courier" },
	{ "Verdana", "Arial" },
	{ "Trebuchet MS", "Arial" },
	{ "Tahoma", NULL },
	{ NULL, "Tahoma" },		// every other font falls back to this
};

//-----------------------------------------------------------------------------
// Purpose: returns true if the font is in the list of OK asian fonts
//-----------------------------------------------------------------------------
bool CFontManager::IsFontForeignLanguageCapable(const char *windowsFontName)
{
	for (int i = 0; g_szValidAsianFonts[i] != NULL; i++)
	{
		if (!stricmp(g_szValidAsianFonts[i], windowsFontName))
			return true;
	}

	// typeface isn't supported by asian languages
	return false;
}

//-----------------------------------------------------------------------------
// Purpose: fallback fonts
//-----------------------------------------------------------------------------
const char *CFontManager::GetFallbackFontName(const char *windowsFontName)
{
	int i;
	for ( i = 0; g_FallbackFonts[i].font != NULL; i++ )
	{
		if (!stricmp(g_FallbackFonts[i].font, windowsFontName))
			return g_FallbackFonts[i].fallbackFont;
	}

	// the ultimate fallback
	return g_FallbackFonts[i].fallbackFont;
}

struct Win98ForeignFallbackFont_t
{
	const char *language;
	const char *fallbackFont;
};

// list of how fonts fallback
Win98ForeignFallbackFont_t g_Win98ForeignFallbackFonts[] =
{
	{ "russian", "system" },
	{ "japanese", "win98japanese" },
	{ "thai", "system" },
	{ NULL, "Tahoma" },		// every other font falls back to this
};

//-----------------------------------------------------------------------------
// Purpose: specialized fonts
//-----------------------------------------------------------------------------
const char *CFontManager::GetForeignFallbackFontName()
{
	if ( s_bSupportsUnicode )
	{
		// tahoma has all the necessary characters for asian/russian languages for winXP/2K+
		return "Tahoma";
	}

	int i;
	for (i = 0; g_Win98ForeignFallbackFonts[i].language != NULL; i++)
	{
		if (!stricmp(g_Win98ForeignFallbackFonts[i].language, m_szLanguage))
			return g_Win98ForeignFallbackFonts[i].fallbackFont;
	}

	// the ultimate fallback
	return g_Win98ForeignFallbackFonts[i].fallbackFont;
}

#if defined( _X360 )
bool CFontManager::GetCachedXUIMetrics( const char *pFontName, int tall, int style, XUIFontMetrics *pFontMetrics, XUICharMetrics charMetrics[256] )
{
	// linear lookup is good enough
	CUtlSymbol fontSymbol = pFontName;
	bool bFound = false;
	int i;
	for ( i = 0; i < m_XUIMetricCache.Count(); i++ )
	{
		if ( m_XUIMetricCache[i].fontSymbol == fontSymbol && m_XUIMetricCache[i].tall == tall && m_XUIMetricCache[i].style == style )
		{
			bFound = true;
			break;
		}
	}
	if ( !bFound )
	{
		return false;
	}

	// get from the cache
	*pFontMetrics = m_XUIMetricCache[i].fontMetrics;
	V_memcpy( charMetrics, m_XUIMetricCache[i].charMetrics, 256 * sizeof( XUICharMetrics ) );
	return true;
}
#endif

#if defined( _X360 )
void CFontManager::SetCachedXUIMetrics( const char *pFontName, int tall, int style, XUIFontMetrics *pFontMetrics, XUICharMetrics charMetrics[256] )
{
	MEM_ALLOC_CREDIT();

	int i = m_XUIMetricCache.AddToTail();

	m_XUIMetricCache[i].fontSymbol = pFontName;
	m_XUIMetricCache[i].tall = tall;
	m_XUIMetricCache[i].style = style;
	m_XUIMetricCache[i].fontMetrics = *pFontMetrics;
	V_memcpy( m_XUIMetricCache[i].charMetrics, charMetrics, 256 * sizeof( XUICharMetrics ) );
}
#endif

void CFontManager::ClearTemporaryFontCache()
{
#if defined( _X360 )
	m_XUIMetricCache.Purge();

	// many fonts are blindly precached by vgui and never used
	// font will re-open if glyph is actually requested
	for ( int i = 0; i < m_Win32Fonts.Count(); i++ )
	{
		m_Win32Fonts[i]->CloseResource();
	}
#endif
}


//-----------------------------------------------------------------------------
// Purpose: returns the max height of a font
//-----------------------------------------------------------------------------
bool CFontManager::GetFontUnderlined( HFont font )
{
	return m_FontAmalgams[font].GetUnderlined();
}



#ifdef DBGFLAG_VALIDATE
//-----------------------------------------------------------------------------
// Purpose: Ensure that all of our internal structures are consistent, and
//			account for all memory that we've allocated.
// Input:	validator -		Our global validator object
//			pchName -		Our name (typically a member var in our container)
//-----------------------------------------------------------------------------
void CFontManager::Validate( CValidator &validator, char *pchName )
{
	validator.Push( "CFontManager", this, pchName );

	ValidateObj( m_FontAmalgams );
	for ( int iFont = 0; iFont < m_FontAmalgams.Count(); iFont++ )
	{
		ValidateObj( m_FontAmalgams[iFont] );
	}

	ValidateObj( m_Win32Fonts );
	for ( int iWin32Font = 0; iWin32Font < m_Win32Fonts.Count(); iWin32Font++ )
	{
		ValidatePtr( m_Win32Fonts[ iWin32Font ] );
	}

	validator.Pop();
}
#endif // DBGFLAG_VALIDATE


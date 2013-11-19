//===== Copyright © 1996-2005, Valve Corporation, All rights reserved. ======//
//
// Purpose: 
//
// $NoKeywords: $
//===========================================================================//


#pragma warning( disable: 4018 ) // '==' : signed/unsigned mismatch in rbtree
#if !defined( _X360 )
#include <windows.h>
#endif
#include <wchar.h>

#include "FileSystem.h"

#include "vgui_internal.h"
#include "vgui/ILocalize.h"
#include "vgui/ISystem.h"
#include "vgui/ISurface.h"

#include "tier1/UtlVector.h"
#include "tier1/UtlRBTree.h"
#include "tier1/UtlSymbol.h"
#include "tier1/UtlString.h"
#include "UnicodeFileHelpers.h"
#include "tier0/icommandline.h"
#include "byteswap.h"

#if defined( _X360 )
#include "xbox/xbox_win32stubs.h"
#endif

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

using namespace vgui;

#define MAX_LOCALIZED_CHARS	4096

//-----------------------------------------------------------------------------
// 
// Internal implementation
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Purpose: Maps token names to localized unicode strings
//-----------------------------------------------------------------------------
class CLocalizedStringTable : public vgui::ILocalize
{
public:
	CLocalizedStringTable();
	~CLocalizedStringTable();

	// adds the contents of a file to the localization table
	virtual bool AddFile( const char *fileName, const char *pPathID, bool bIncludeFallbackSearchPaths );

	// saves the entire contents of the token tree to the file
	bool SaveToFile( const char *fileName );

	// adds a single name/unicode string pair to the table
	void AddString(const char *tokenName, wchar_t *unicodeString, const char *fileName);

	// Finds the localized text for pName
	wchar_t *Find(const char *pName);

	// finds the index of a token by token name
	StringIndex_t FindIndex(const char *pName);
	
	// Remove all strings in the table.
	void RemoveAll();

	// converts an english string to unicode
	// returns the number of wchar_t in resulting string, including null terminator
	int ConvertANSIToUnicode(const char *ansi, wchar_t *unicode, int unicodeBufferSizeInBytes);

	// converts an unicode string to an english string
	// unrepresentable characters are converted to system default
	// returns the number of characters in resulting string, including null terminator
	int ConvertUnicodeToANSI(const wchar_t *unicode, char *ansi, int ansiBufferSize);

	void ConstructString(wchar_t *unicodeOutput, int unicodeBufferSizeInBytes, wchar_t *formatString, int numFormatParameters, ...);

	// iteration functions
	StringIndex_t GetFirstStringIndex();

	// returns the next index, or INVALID_STRING_INDEX if no more strings available
	StringIndex_t GetNextStringIndex(StringIndex_t index);

	// gets the values from the index
	const char *GetNameByIndex(StringIndex_t index);
	wchar_t *GetValueByIndex(StringIndex_t index);
	const char *GetFileNameByIndex(StringIndex_t index);

	// sets the value in the index
	// has bad memory characteristics, should only be used in the editor
	void SetValueByIndex(StringIndex_t index, wchar_t *newValue);

	// iterates the filenames
	int GetLocalizationFileCount();
	const char *GetLocalizationFileName(int index);

	// returns whether a file has already been loaded
	bool LocalizationFileIsLoaded( const char *name );

	void ConstructString(wchar_t *unicodeOutput, int unicodeBufferSizeInBytes, wchar_t *formatString, int numFormatParameters, va_list argList);

private:
	// for development only, reloads localization files
	virtual void ReloadLocalizationFiles( );
	virtual void ConstructString(wchar_t *unicodeOutput, int unicodeBufferSizeInBytes, const char *tokenName, KeyValues *dialogVariables);
	virtual void ConstructString(wchar_t *unicodeOutput, int unicodeBufferSizeInBytes, StringIndex_t unlocalizedTextSymbol, KeyValues *dialogVariables);

	bool AddAllLanguageFiles( const char *baseFileName );

	void BuildFastValueLookup();
	void DiscardFastValueLookup();
	int FindExistingValueIndex( const wchar_t *value );

	char m_szLanguage[64];
	bool m_bUseOnlyLongestLanguageString;

	struct localizedstring_t
	{
		StringIndex_t nameIndex;
		// nameIndex == INVALID_STRING_INDEX is used only for searches and implies
		// that pszValueString will be used from union fields.
		union
		{
			StringIndex_t valueIndex;		// Used when nameIndex != INVALID_STRING_INDEX
			char const * pszValueString;	// Used only if nameIndex == INVALID_STRING_INDEX
		};
		CUtlSymbol filename;
	};

	// Stores the symbol lookup
	CUtlRBTree<localizedstring_t, StringIndex_t> m_Lookup;
	
	// stores the string data
	CUtlVector<char> m_Names;
	CUtlVector<wchar_t> m_Values;
	CUtlSymbol m_CurrentFile;

	struct LocalizationFileInfo_t
	{
		CUtlSymbol	symName;
		CUtlSymbol	symPathID;
		bool		bIncludeFallbacks;

		static bool LessFunc( const LocalizationFileInfo_t& lhs, const LocalizationFileInfo_t& rhs )
		{
			int iresult = Q_stricmp( lhs.symPathID.String(), rhs.symPathID.String() );
			if ( iresult != 0 )
			{
				return iresult == -1;
			}

			return Q_stricmp( lhs.symName.String(), rhs.symName.String() ) < 0;
		}
	};

	CUtlVector< LocalizationFileInfo_t > m_LocalizationFiles;

	struct fastvalue_t
	{
		int				valueindex;
		const wchar_t	*search;

		static CLocalizedStringTable	*s_pTable;
	};

	CUtlRBTree< fastvalue_t, int >	m_FastValueLookup;

	static CLocalizedStringTable *s_pTable;

	// Less function, for sorting strings
	static bool SymLess( localizedstring_t const& i1, localizedstring_t const& i2 );

	static bool FastValueLessFunc( const fastvalue_t& lhs, const fastvalue_t& rhs );
};

// global instance of table
CLocalizedStringTable g_StringTable;

// expose the interface
EXPOSE_SINGLE_INTERFACE_GLOBALVAR(CLocalizedStringTable, ILocalize, VGUI_LOCALIZE_INTERFACE_VERSION, g_StringTable);


//-----------------------------------------------------------------------------
// Purpose: Constructor
//-----------------------------------------------------------------------------
CLocalizedStringTable::CLocalizedStringTable() : 
	m_Lookup( 0, 0, SymLess ), m_Names( 1024 ), m_Values( 2048 ), m_FastValueLookup( 0, 0, FastValueLessFunc )
{
	m_bUseOnlyLongestLanguageString = false;
}

//-----------------------------------------------------------------------------
// Purpose: Destructor
//-----------------------------------------------------------------------------
CLocalizedStringTable::~CLocalizedStringTable()
{
	m_Names.Purge();
	m_Values.Purge();
	m_LocalizationFiles.Purge();
}


//-----------------------------------------------------------------------------
// Purpose: Adds the contents of a file 
//-----------------------------------------------------------------------------
bool CLocalizedStringTable::AddFile( const char *szFileName, const char *pPathID, bool bIncludeFallbackSearchPaths )
{
	// use the correct file based on the chosen language
	static const char *const LANGUAGE_STRING = "%language%";
	static const char *const ENGLISH_STRING = "english";
	static const int MAX_LANGUAGE_NAME_LENGTH = 64;
	char language[MAX_LANGUAGE_NAME_LENGTH];
	char fileName[MAX_PATH];
	int offs = 0;
	bool success = false;

	memset( language, 0, sizeof(language) );

	Q_strncpy( fileName, szFileName, sizeof( fileName ) );

	const char *langptr = strstr(szFileName, LANGUAGE_STRING);
	if (langptr)
	{
		// LOAD THE ENGLISH FILE FIRST
		// always load the file to make sure we're not missing any strings
		// copy out the initial part of the string
		offs = langptr - szFileName;
		strncpy(fileName, szFileName, offs);
		fileName[offs] = 0;

		if ( vgui::g_pSystem->CommandLineParamExists("-all_languages") )
		{
			m_bUseOnlyLongestLanguageString = true;
			return AddAllLanguageFiles( fileName );
		}

		// append "english" as our default language
		Q_strncat(fileName, ENGLISH_STRING, sizeof( fileName ), COPY_ALL_CHARACTERS );

		// append the end of the initial string
		offs += strlen(LANGUAGE_STRING);
		Q_strncat(fileName, szFileName + offs, sizeof( fileName ), COPY_ALL_CHARACTERS);

		success = AddFile( fileName, pPathID, bIncludeFallbackSearchPaths );

		bool bValid;
		if ( IsPC() )
		{
			bValid = vgui::g_pSystem->GetRegistryString( "HKEY_CURRENT_USER\\Software\\Valve\\Steam\\Language", language, sizeof(language)-1 );
		}
		else
		{
			Q_strncpy( language, XBX_GetLanguageString(), sizeof( language ) );
			bValid = true;
		}

		// LOAD THE LOCALIZED FILE IF IT'S NOT ENGLISH
		// append the language
		if ( bValid )
		{
			if ( strlen(language) != 0 && stricmp(language, ENGLISH_STRING) != 0 )
			{
				// copy out the initial part of the string
				offs = langptr - szFileName;
				strncpy(fileName, szFileName, offs);
				fileName[offs] = 0;

				Q_strncat(fileName, language, sizeof( fileName ), COPY_ALL_CHARACTERS);

				// append the end of the initial string
				offs += strlen(LANGUAGE_STRING);
				Q_strncat(fileName, szFileName + offs, sizeof( fileName ), COPY_ALL_CHARACTERS );

				success &= AddFile( fileName, pPathID, bIncludeFallbackSearchPaths );
			}
		}
		return success;
	}

	// store the localization file name if it doesn't already exist
	LocalizationFileInfo_t search;
	search.symName = fileName;
	search.symPathID = pPathID ? pPathID : "";
	search.bIncludeFallbacks = bIncludeFallbackSearchPaths;

	int lfc = m_LocalizationFiles.Count();
	for ( int lf = 0; lf < lfc; ++lf )
	{
		LocalizationFileInfo_t& entry = m_LocalizationFiles[ lf ];
		if ( !Q_stricmp( entry.symName.String(), fileName ) )
		{
			m_LocalizationFiles.Remove( lf );
			break;
		}
	}

	m_LocalizationFiles.AddToTail( search );

	// This will give us a list of paths from highest to lowest precedence: e.g.:
	// for "GAME" when running -game episodic, it'll show:
	// "basedir/episodic/;basedir/hl2"
	// HACK HACK - why do this? Why not let the filesystem be smart?
	char searchPaths[ MAX_PATH*50 ]; // allow for 50 search paths 
	Verify( g_pFullFileSystem->GetSearchPath( pPathID, true, searchPaths, sizeof( searchPaths ) ) < sizeof(searchPaths) );

	CUtlSymbolTable				pathStrings;
	CUtlVector< CUtlSymbol >	searchList;

	bool bIsFullPath = false;
	if ( V_IsAbsolutePath( fileName ) )
	{
		bIsFullPath = true;
		CUtlSymbol sym = pathStrings.AddString( fileName );
		searchList.AddToHead( sym );
	}
	else
	{
		// We want to walk them in reverse order so newer files are "overrides" for older ones, so we add them to a list in reverse order
		for ( char *path = strtok( searchPaths, ";" ); path; path = strtok( NULL, ";" ) )
		{
			if ( IsX360() && ( g_pFullFileSystem->GetDVDMode() == DVDMODE_STRICT ) && !V_stristr( path, ".zip" ) )
			{
				// only want zip paths
				continue;
			} 

			char fullpath[MAX_PATH];
			Q_snprintf( fullpath, sizeof( fullpath ), "%s%s", path, fileName );
			Q_FixSlashes( fullpath );
			Q_strlower( fullpath );

			CUtlSymbol sym = pathStrings.AddString( fullpath );
			// Push them on head so we can walk them in reverse order
			searchList.AddToHead( sym );
		}
	}

	bool first = true;
	bool bLoadedAtLeastOne = false;

	for ( int sp = 0; sp < searchList.Count(); ++sp )
	{
		const char *fullpath = pathStrings.String( searchList[ sp ] );

		// parse out the file
		FileHandle_t file = g_pFullFileSystem->Open( fullpath, "rb" );
		if (!file)
		{
			continue;
		}

		if ( first )
		{
			first = false;
		}
		else if ( !bIncludeFallbackSearchPaths )
		{
			g_pFullFileSystem->Close(file);
			break;
		}

		bLoadedAtLeastOne = true;

		// this is an optimization so that the filename string doesn't have to get converted to a symbol for each key/value
		m_CurrentFile = fullpath;
		
		// read into a memory block
		int fileSize = g_pFullFileSystem->Size(file);
		int bufferSize = g_pFullFileSystem->GetOptimalReadSize( file, fileSize + sizeof(wchar_t) );
		wchar_t *memBlock = (wchar_t *)g_pFullFileSystem->AllocOptimalReadBuffer(file, bufferSize);
		bool bReadOK = ( g_pFullFileSystem->ReadEx(memBlock, bufferSize, fileSize, file) != 0 );

		// finished with file
		g_pFullFileSystem->Close(file);

		// null-terminate the stream
		memBlock[fileSize / sizeof(wchar_t)] = 0x0000;

		// check the first character, make sure this a little-endian unicode file
		wchar_t *data = memBlock;
		wchar_t signature = LittleShort( data[0] );
		if ( !bReadOK || signature != 0xFEFF )
		{
			Msg( "Ignoring non-unicode close caption file %s\n", fullpath );
			g_pFullFileSystem->FreeOptimalReadBuffer( memBlock );
			return false;
		}

		// ensure little-endian unicode reads correctly on all platforms
		CByteswap byteSwap;
		byteSwap.SetTargetBigEndian( false );
		byteSwap.SwapBufferToTargetEndian( data, data, fileSize / sizeof(wchar_t) );

		// skip past signature
		data++;

		// parse out a token at a time
		enum states_e
		{
			STATE_BASE,		// looking for base settings
			STATE_TOKENS,	// reading in unicode tokens
		};

		bool bQuoted;
		bool bEnglishFile = false;
		if ( strstr(fullpath, "_english.txt") )
		{
			bEnglishFile = true;
		}

		bool spew = false;
		if ( CommandLine()->FindParm( "-ccsyntax" ) )
		{
			spew = true;
		}

		BuildFastValueLookup();

		states_e state = STATE_BASE;
		while (1)
		{
			// read the key and the value
			wchar_t keytoken[128];
			data = ReadUnicodeToken(data, keytoken, 128, bQuoted);
			if (!keytoken[0])
				break;	// we've hit the null terminator

			// convert the token to a string
			char key[128];
			ConvertUnicodeToANSI(keytoken, key, sizeof(key));

			// if we have a C++ style comment, read to end of line and continue
			if (!strnicmp(key, "//", 2))
			{
				data = ReadToEndOfLine(data);
				continue;
			}

			if ( spew )
			{
				Msg( "%s\n", key );
			}

			wchar_t valuetoken[ MAX_LOCALIZED_CHARS ];
			data = ReadUnicodeToken(data, valuetoken, MAX_LOCALIZED_CHARS, bQuoted);
			if (!valuetoken[0] && !bQuoted)
				break;	// we've hit the null terminator
			
			if (state == STATE_BASE)
			{
				if (!stricmp(key, "Language"))
				{
					// copy out our language setting
					char value[MAX_LOCALIZED_CHARS];
					ConvertUnicodeToANSI(valuetoken, value, sizeof(value));
					strncpy(m_szLanguage, value, sizeof(m_szLanguage) - 1);
				}
				else if (!stricmp(key, "Tokens"))
				{
					state = STATE_TOKENS;
				}
				else if (!stricmp(key, "}"))
				{
					// we've hit the end
					break;
				}
			}
			else if (state == STATE_TOKENS)
			{
				if (!stricmp(key, "}"))
				{
					// end of tokens
					state = STATE_BASE;
				}
				else
				{
					// skip our [english] beginnings (in non-english files)
					if ( (bEnglishFile) || (!bEnglishFile && strnicmp(key, "[english]", 9)))
					{
						// Check for a conditional tag
						bool bAccepted = true;
						wchar_t conditional[ MAX_LOCALIZED_CHARS ];
						wchar_t *tempData = ReadUnicodeToken(data, conditional, MAX_LOCALIZED_CHARS, bQuoted);
						if ( !bQuoted && wcsstr( conditional, L"[$" ) )
						{
							// Evaluate the conditional tag
							char cond[MAX_LOCALIZED_CHARS];
							ConvertUnicodeToANSI(conditional, cond, sizeof(cond));
							bAccepted = EvaluateConditional( cond );
							data = tempData;
						}
						if ( bAccepted )
						{
							// add the string to the table
							AddString(key, valuetoken, NULL);
						}
					}
				}
			}
		}

		g_pFullFileSystem->FreeOptimalReadBuffer( memBlock );
	}

	if ( !bLoadedAtLeastOne )
	{
		Warning("ILocalize::AddFile() failed to load file \"%s\".\n", szFileName );
	}

	DiscardFastValueLookup();
	m_CurrentFile = UTL_INVAL_SYMBOL;
	return bLoadedAtLeastOne;
}

//-----------------------------------------------------------------------------
// Purpose: Load all the localized language strings, and uses the longest string from each language
//-----------------------------------------------------------------------------
bool CLocalizedStringTable::AddAllLanguageFiles( const char *baseFileName )
{
	bool success = true;

	// work out the path the files are in
	char szFilePath[MAX_PATH];
	Q_strncpy( szFilePath, baseFileName, sizeof(szFilePath) );
	char *lastSlash = strrchr( szFilePath, '\\' );
	if (!lastSlash)
	{
		lastSlash = strrchr( szFilePath, '/' );
	}
	if (lastSlash)
	{
		lastSlash[1] = 0;
	}
	else
	{
		szFilePath[0] = 0;
	}

	// iterate through and add all the languages (for development)
	// the longest string out of all the languages will be used
	char szSearchPath[MAX_PATH];
	Q_snprintf( szSearchPath, sizeof(szSearchPath), "%s*.txt", baseFileName );

	FileFindHandle_t hFind = NULL;
	const char *file = g_pFullFileSystem->FindFirst( szSearchPath, &hFind );
	while ( file )
	{
		// re-add in the search path
		char szFile[MAX_PATH];
		Q_snprintf( szFile, sizeof(szFile), "%s%s", szFilePath, file );

		// add the file
		success &= AddFile( szFile, NULL, true );

		// next file
		file = g_pFullFileSystem->FindNext( hFind );
	}
	g_pFullFileSystem->FindClose( hFind );
	return success;
}

//-----------------------------------------------------------------------------
// Purpose: saves the entire contents of the token tree to the file
//-----------------------------------------------------------------------------
bool CLocalizedStringTable::SaveToFile( const char *szFileName )
{
	// parse out the file
	FileHandle_t file = g_pFullFileSystem->Open(szFileName, "wb");
	if (!file)
		return false;

	// only save the symbols relevant to this file
	CUtlSymbol fileName = szFileName;

	// write litte-endian unicode marker
	unsigned short marker = 0xFEFF;
	marker = LittleShort( marker );
	g_pFullFileSystem->Write(&marker, sizeof( marker ), file);

	const char *startStr = "\"lang\"\r\n{\r\n\"Language\" \"English\"\r\n\"Tokens\"\r\n{\r\n";
	const char *endStr = "}\r\n}\r\n";

	// write out the first string
	static wchar_t unicodeString[1024];
	int strLength = ConvertANSIToUnicode(startStr, unicodeString, sizeof(unicodeString));
	if (!strLength)
		return false;

	g_pFullFileSystem->Write(unicodeString, wcslen( unicodeString ) * sizeof(wchar_t), file);

	// convert our spacing characters to unicode
//	wchar_t unicodeSpace = L' '; 
	wchar_t unicodeQuote = L'\"'; 
	wchar_t unicodeCR = L'\r'; 
	wchar_t unicodeNewline = L'\n'; 
	wchar_t unicodeTab = L'\t';

	// write out all the key/value pairs
	for (StringIndex_t idx = GetFirstStringIndex(); idx != INVALID_STRING_INDEX; idx = GetNextStringIndex(idx))
	{
		// only write strings that belong in this file
		if (fileName != m_Lookup[idx].filename)
			continue;

		const char *name = GetNameByIndex(idx);
		wchar_t *value = GetValueByIndex(idx);

		// convert the name to a unicode string
		ConvertANSIToUnicode(name, unicodeString, sizeof(unicodeString));

		g_pFullFileSystem->Write(&unicodeTab, sizeof(wchar_t), file);

		// write out
		g_pFullFileSystem->Write(&unicodeQuote, sizeof(wchar_t), file);
		g_pFullFileSystem->Write(unicodeString, wcslen( unicodeString ) * sizeof(wchar_t), file);
		g_pFullFileSystem->Write(&unicodeQuote, sizeof(wchar_t), file);

		g_pFullFileSystem->Write(&unicodeTab, sizeof(wchar_t), file);
		g_pFullFileSystem->Write(&unicodeTab, sizeof(wchar_t), file);

		g_pFullFileSystem->Write(&unicodeQuote, sizeof(wchar_t), file);
		g_pFullFileSystem->Write(value, wcslen(value) * sizeof(wchar_t), file);
		g_pFullFileSystem->Write(&unicodeQuote, sizeof(wchar_t), file);

		g_pFullFileSystem->Write(&unicodeCR, sizeof(wchar_t), file);
		g_pFullFileSystem->Write(&unicodeNewline, sizeof(wchar_t), file);
	}

	// write end string
	strLength = ConvertANSIToUnicode(endStr, unicodeString, sizeof(unicodeString));
	g_pFullFileSystem->Write(unicodeString, strLength * sizeof(wchar_t), file);

	g_pFullFileSystem->Close(file);
	return true;
}

//-----------------------------------------------------------------------------
// Purpose: for development, reloads localization files
//-----------------------------------------------------------------------------
void CLocalizedStringTable::ReloadLocalizationFiles( )
{
	// re-add all the localization files
	for (int i = 0; i < m_LocalizationFiles.Count(); i++)
	{
		LocalizationFileInfo_t& entry = m_LocalizationFiles[ i ];
		AddFile
		(
			entry.symName.String(), 
			entry.symPathID.String()[0] ? entry.symPathID.String() : NULL,
			entry.bIncludeFallbacks 
		);
	}
}

//-----------------------------------------------------------------------------
// Purpose: Used to sort strings
//-----------------------------------------------------------------------------
bool CLocalizedStringTable::SymLess(localizedstring_t const &i1, localizedstring_t const &i2)
{
	const char *str1 = (i1.nameIndex == INVALID_STRING_INDEX) ? i1.pszValueString :
											&g_StringTable.m_Names[i1.nameIndex];
	const char *str2 = (i2.nameIndex == INVALID_STRING_INDEX) ? i2.pszValueString :
											&g_StringTable.m_Names[i2.nameIndex];
	
	return stricmp(str1, str2) < 0;
}


//-----------------------------------------------------------------------------
// Purpose: Finds a string in the table
//-----------------------------------------------------------------------------
wchar_t *CLocalizedStringTable::Find(const char *pName)
{	
	StringIndex_t idx = FindIndex(pName);
	if (idx == INVALID_STRING_INDEX)
		return NULL;

	return &m_Values[m_Lookup[idx].valueIndex];
}

//-----------------------------------------------------------------------------
// Purpose: finds the index of a token by token name
//-----------------------------------------------------------------------------
StringIndex_t CLocalizedStringTable::FindIndex(const char *pName)
{
	if (!pName)
		return NULL;

	// strip the pound character (which is used elsewhere to indicate that it's a string that should be translated)
	if (pName[0] == '#')
	{
		pName++;
	}
	
	// Passing this special invalid symbol makes the comparison function
	// use the string passed in the context
	localizedstring_t invalidItem;
	invalidItem.nameIndex = INVALID_STRING_INDEX;
	invalidItem.pszValueString = pName;
	return m_Lookup.Find( invalidItem );
}

//-----------------------------------------------------------------------------
// Finds and/or creates a symbol based on the string
//-----------------------------------------------------------------------------
void CLocalizedStringTable::AddString(const char *pString, wchar_t *pValue, const char *fileName)
{
	if (!pString) 
		return;

	MEM_ALLOC_CREDIT();

	// see if the value is already in our string table
	int valueIndex = FindExistingValueIndex( pValue );
	if ( valueIndex == INVALID_STRING_INDEX )
	{
		int len = wcslen( pValue ) + 1;
		valueIndex = m_Values.AddMultipleToTail( len );
		memcpy( &m_Values[valueIndex], pValue, len * sizeof(wchar_t) );
	}

	// see if the key is already in the table
	StringIndex_t stridx = FindIndex( pString );
	localizedstring_t item;
	item.nameIndex = stridx;

	if ( stridx == INVALID_STRING_INDEX )
	{
		// didn't find, insert the string into the vector.
		int len = strlen(pString) + 1;
		stridx = m_Names.AddMultipleToTail( len );
		memcpy( &m_Names[stridx], pString, len * sizeof(char) );

		item.nameIndex = stridx;
		item.valueIndex = valueIndex;
		item.filename = fileName ? fileName : m_CurrentFile;

		m_Lookup.Insert( item );
	}
	else
	{
		// it's already in the table
		
		if ( m_bUseOnlyLongestLanguageString )
		{
			// check which string is longer
			wchar_t *newValue = pValue;
			wchar_t *oldValue = GetValueByIndex( stridx );

			// get the width of the string, using just the first font
			int newWide, oldWide, tall;
			vgui::g_pSurface->GetTextSize( 1, newValue, newWide, tall );
			vgui::g_pSurface->GetTextSize( 1, oldValue, oldWide, tall );
			
			// if the new one is shorter, don't let it be added
			if (newWide < oldWide)
				return;
		}

		// replace the current item
		item.nameIndex = GetNameByIndex( stridx ) - &m_Names[ 0 ];
		item.valueIndex = valueIndex;
		item.filename = fileName ? fileName : m_CurrentFile;
		m_Lookup[ stridx ] = item;
	}
}

//-----------------------------------------------------------------------------
// Remove all symbols in the table.
//-----------------------------------------------------------------------------
void CLocalizedStringTable::RemoveAll()
{
	m_Lookup.RemoveAll();
	m_Names.RemoveAll();
	m_Values.RemoveAll();
	m_LocalizationFiles.RemoveAll();
}

//-----------------------------------------------------------------------------
// Purpose: iteration functions
//-----------------------------------------------------------------------------
StringIndex_t CLocalizedStringTable::GetFirstStringIndex()
{
	return m_Lookup.FirstInorder();
}

//-----------------------------------------------------------------------------
// Purpose: returns the next index, or INVALID_STRING_INDEX if no more strings available
//-----------------------------------------------------------------------------
StringIndex_t CLocalizedStringTable::GetNextStringIndex(StringIndex_t index)
{
	StringIndex_t idx = m_Lookup.NextInorder(index);
	if (idx == m_Lookup.InvalidIndex())
		return INVALID_STRING_INDEX;
	return idx;
}

//-----------------------------------------------------------------------------
// Purpose: gets the name of the localization string by index
//-----------------------------------------------------------------------------
const char *CLocalizedStringTable::GetNameByIndex(StringIndex_t index)
{
	localizedstring_t &lstr = m_Lookup[index];
	return &m_Names[lstr.nameIndex];
}

//-----------------------------------------------------------------------------
// Purpose: gets the localized string value by index
//-----------------------------------------------------------------------------
wchar_t *CLocalizedStringTable::GetValueByIndex(StringIndex_t index)
{
	if (index == INVALID_STRING_INDEX)
		return NULL;

	localizedstring_t &lstr = m_Lookup[index];
	return &m_Values[lstr.valueIndex];
}


CLocalizedStringTable *CLocalizedStringTable::s_pTable = NULL;

bool CLocalizedStringTable::FastValueLessFunc( const fastvalue_t& lhs, const fastvalue_t& rhs )
{
	Assert( s_pTable );

	const wchar_t *w1 = lhs.search ? lhs.search : &s_pTable->m_Values[ lhs.valueindex ];
	const wchar_t *w2 = rhs.search ? rhs.search : &s_pTable->m_Values[ rhs.valueindex ];

	return ( wcscmp( w1, w2 ) < 0 ) ? true : false;
}

void CLocalizedStringTable::BuildFastValueLookup()
{
	m_FastValueLookup.RemoveAll();
	s_pTable = this;

	// Build it
	int c = m_Lookup.Count();
	for ( int i = 0; i < c; ++i )
	{
		fastvalue_t val;
		val.valueindex = m_Lookup[ i ].valueIndex;
		val.search = NULL;

		m_FastValueLookup.Insert( val );
	}
}

void CLocalizedStringTable::DiscardFastValueLookup()
{
	m_FastValueLookup.RemoveAll();
	s_pTable = NULL;
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
int CLocalizedStringTable::FindExistingValueIndex( const wchar_t *value )
{
	if ( !s_pTable )
		return INVALID_STRING_INDEX;

	fastvalue_t val;
	val.valueindex = -1;
	val.search = value;

	int idx = m_FastValueLookup.Find( val );
	if ( idx != m_FastValueLookup.InvalidIndex() )
	{
		return m_FastValueLookup[ idx ].valueindex;
	}
	return INVALID_STRING_INDEX;
}

//-----------------------------------------------------------------------------
// Purpose: returns which file a string was loaded from
//-----------------------------------------------------------------------------
const char *CLocalizedStringTable::GetFileNameByIndex(StringIndex_t index)
{
	localizedstring_t &lstr = m_Lookup[index];
	return lstr.filename.String();
}

//-----------------------------------------------------------------------------
// Purpose: sets the value in the index
//-----------------------------------------------------------------------------
void CLocalizedStringTable::SetValueByIndex(StringIndex_t index, wchar_t *newValue)
{
	// get the existing string
	localizedstring_t &lstr = m_Lookup[index];
	wchar_t *wstr = &m_Values[lstr.valueIndex];

	// see if the new string will fit within the old memory
	int newLen = wcslen(newValue);
	int oldLen = wcslen(wstr);

	if (newLen > oldLen)
	{
		// it won't fit, so allocate new memory - this is wasteful, but only happens in edit mode
		lstr.valueIndex = m_Values.AddMultipleToTail(newLen + 1);
		memcpy(&m_Values[lstr.valueIndex], newValue, (newLen + 1) * sizeof(wchar_t));
	}
	else
	{
		// copy the string into the old position
		wcscpy(wstr, newValue);		
	}
}

//-----------------------------------------------------------------------------
// Purpose: returns number of localization files currently loaded
//-----------------------------------------------------------------------------
int CLocalizedStringTable::GetLocalizationFileCount()
{
	return m_LocalizationFiles.Count();
}

//-----------------------------------------------------------------------------
// Purpose: returns localization filename by index
//-----------------------------------------------------------------------------
const char *CLocalizedStringTable::GetLocalizationFileName(int index)
{
	return m_LocalizationFiles[index].symName.String();
}

//-----------------------------------------------------------------------------
// Purpose: returns whether a localization file has been loaded already
//-----------------------------------------------------------------------------
bool CLocalizedStringTable::LocalizationFileIsLoaded(const char *name)
{
	int c = m_LocalizationFiles.Count();
	for ( int i = 0; i < c; ++i )
	{
		if ( !Q_stricmp( m_LocalizationFiles[ i ].symName.String(), name ) )
			return true;
	}

	return false;
}

//-----------------------------------------------------------------------------
// Purpose: converts an english string to unicode
//-----------------------------------------------------------------------------
int CLocalizedStringTable::ConvertANSIToUnicode(const char *ansi, wchar_t *unicode, int unicodeBufferSizeInBytes)
{
	int chars = ::MultiByteToWideChar(CP_UTF8, 0, ansi, -1, unicode, unicodeBufferSizeInBytes / sizeof(wchar_t));
	unicode[(unicodeBufferSizeInBytes / sizeof(wchar_t)) - 1] = 0;
	return chars;
}

//-----------------------------------------------------------------------------
// Purpose: converts an unicode string to an english string
//-----------------------------------------------------------------------------
int CLocalizedStringTable::ConvertUnicodeToANSI(const wchar_t *unicode, char *ansi, int ansiBufferSize)
{
	int result = ::WideCharToMultiByte(CP_UTF8, 0, unicode, -1, ansi, ansiBufferSize, NULL, NULL);
	ansi[ansiBufferSize - 1] = 0;
	return result;
}

#define va_argByIndex(ap,t,i)    ( *(t *)(ap + i * _INTSIZEOF(t)) )

//-----------------------------------------------------------------------------
// Purpose: builds a localized formatted string
//-----------------------------------------------------------------------------
void CLocalizedStringTable::ConstructString(wchar_t *unicodeOutput, int unicodeBufferSizeInBytes, wchar_t *formatString, int numFormatParameters, ...)
{
	if (!formatString)
	{
		unicodeOutput[0] = 0;
		return;
	}

	va_list argList;
	va_start(argList, numFormatParameters);
	ConstructString( unicodeOutput, unicodeBufferSizeInBytes, formatString, numFormatParameters, argList);
	va_end(argList);
}

//-----------------------------------------------------------------------------
// Purpose: construct string helper
//-----------------------------------------------------------------------------
void CLocalizedStringTable::ConstructString(wchar_t *unicodeOutput, int unicodeBufferSizeInBytes, wchar_t *formatString, int numFormatParameters, va_list argList)
{
	int unicodeBufferSize = unicodeBufferSizeInBytes / sizeof(wchar_t);
	wchar_t *searchPos = formatString;
	wchar_t *outputPos = unicodeOutput;

	//assumes we can't have %s10
	//assume both are 0 terminated?
	int formatLength = wcslen( formatString );

#ifdef _DEBUG
	int curArgIdx = 0;
#endif

	while ( searchPos[0] != '\0' && unicodeBufferSize > 0 )
	{
		if ( formatLength >= 3 && searchPos[0] == '%' && searchPos[1] == 's' )
		{
			//this is an escape sequence - %s1, %s2 etc, up to %s9

			int argindex = ( searchPos[2] ) - '0' - 1;

			if ( argindex < 0 || argindex > 9 )
			{
				Warning( "Bad format string in CLocalizeStringTable::ConstructString\n" );
				*outputPos = '\0';
				return;
			}

			if ( argindex < numFormatParameters )
			{
				wchar_t *param = NULL;
				if ( IsPC() )
				{
					param = va_argByIndex( argList, wchar_t *, argindex );
				}
				else
				{
					// X360TBD: convert string to new %var% format if this assert hits
					Assert( argindex == curArgIdx++ );
					param = va_arg( argList, wchar_t* );
				}

				if (!param)
				{
					Assert( !("CLocalizedStringTable::ConstructString - Found a %s# escape sequence who's index was more than the number of args.") );
					*outputPos = '\0';
				}
				

				int paramSize = wcslen(param);
				if (paramSize > unicodeBufferSize)
				{
					paramSize = unicodeBufferSize;
				}

				wcsncpy(outputPos, param, paramSize);

				unicodeBufferSize -= paramSize;
				outputPos += paramSize;

				searchPos += 3;
				formatLength -= 3;
			}
			else
			{
				//copy it over, char by char
				*outputPos = *searchPos;

				outputPos++;
				unicodeBufferSize--;

				searchPos++;
				formatLength--;
			}
		}
		else
		{
			//copy it over, char by char
			*outputPos = *searchPos;

			outputPos++;
			unicodeBufferSize--;

			searchPos++;
			formatLength--;
		}
	}

	// ensure null termination
	*outputPos = '\0';
}


//-----------------------------------------------------------------------------
// Purpose: Constructs a string, inserting variables where necessary
//-----------------------------------------------------------------------------
void CLocalizedStringTable::ConstructString(wchar_t *unicodeOutput, int unicodeBufferSizeInBytes, const char *tokenName, KeyValues *localizationVariables)
{
	StringIndex_t index = FindIndex(tokenName);

	if (index != INVALID_STRING_INDEX)
	{
		ConstructString(unicodeOutput, unicodeBufferSizeInBytes, index, localizationVariables);
	}
	else
	{
		// string not found, just return the token name
		ConvertANSIToUnicode(tokenName, unicodeOutput, unicodeBufferSizeInBytes);
	}
}

//-----------------------------------------------------------------------------
// Purpose: Constructs a string, inserting variables where necessary
//-----------------------------------------------------------------------------
void CLocalizedStringTable::ConstructString(wchar_t *unicodeOutput, int unicodeBufferSizeInBytes, StringIndex_t unlocalizedTextSymbol, KeyValues *localizationVariables)
{
	if (unicodeBufferSizeInBytes < 1)
		return;

	unicodeOutput[0] = 0;
	const wchar_t *searchPos = GetValueByIndex(unlocalizedTextSymbol);
	if (!searchPos)
	{
		wcsncpy(unicodeOutput, L"[unknown string]", unicodeBufferSizeInBytes / sizeof(wchar_t));
		return;
	}

	wchar_t *outputPos = unicodeOutput;

	//assumes we can't have %s10
	//assume both are 0 terminated?
	int unicodeBufferSize = unicodeBufferSizeInBytes / sizeof(wchar_t);

	while ( *searchPos != '\0' && unicodeBufferSize > 0 )
	{
		bool shouldAdvance = true;

		if ( *searchPos == '%' )
		{
			// this is an escape sequence that specifies a variable name
			if ( searchPos[1] == 's' && searchPos[2] >= '0' && searchPos[2] <= '9' )
			{
				// old style escape sequence, ignore
			}
			else if ( searchPos[1] == '%' )
			{
				// just a '%' char, just write the second one
				searchPos++;
			}
			else if ( localizationVariables )
			{
				// get out the variable name
				const wchar_t *varStart = searchPos + 1;
				const wchar_t *varEnd = wcschr( varStart, '%' );

				if ( varEnd && *varEnd == '%' )
				{
					shouldAdvance = false;

					// assume variable names must be ascii, do a quick convert
					char variableName[32];
					char *vset = variableName;
					for ( const wchar_t *pws = varStart; pws < varEnd && (vset < variableName + sizeof(variableName) - 1); ++pws, ++vset )
					{
						*vset = (char)*pws;
					}
					*vset = 0;

					// look up the variable name
					const wchar_t *value = localizationVariables->GetWString( variableName, L"[unknown]" );
					
					int paramSize = wcslen(value);
					if (paramSize > unicodeBufferSize)
					{
						paramSize = unicodeBufferSize;
					}

					wcsncpy(outputPos, value, paramSize);

					unicodeBufferSize -= paramSize;
					outputPos += paramSize;
					searchPos = varEnd + 1;
				}
			}
		}

		if (shouldAdvance)
		{
			//copy it over, char by char
			*outputPos = *searchPos;

			outputPos++;
			unicodeBufferSize--;

			searchPos++;
		}		
	}

	// ensure null termination
	*outputPos = '\0';
}

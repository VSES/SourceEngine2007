//========= Copyright © 1996-2005, Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
// $NoKeywords: $
//=============================================================================//

#include <vstdlib/IKeyValuesSystem.h>
#include <KeyValues.h>
#include "mempool.h"
#include "utlsymbol.h"
#include "tier0/threadtools.h"
#include "tier1/memstack.h"

// memdbgon must be the last include file in a .cpp file!!!
#include <tier0/memdbgon.h>

#ifdef NO_SBH // no need to pool if using tier0 small block heap
#define KEYVALUES_USE_POOL 1
#endif

//-----------------------------------------------------------------------------
// Purpose: Central storage point for KeyValues memory and symbols
//-----------------------------------------------------------------------------
class CKeyValuesSystem : public IKeyValuesSystem
{
public:
	CKeyValuesSystem();
	~CKeyValuesSystem();

	// registers the size of the KeyValues in the specified instance
	// so it can build a properly sized memory pool for the KeyValues objects
	// the sizes will usually never differ but this is for versioning safety
	void RegisterSizeofKeyValues(int size);

	// allocates/frees a KeyValues object from the shared mempool
	void *AllocKeyValuesMemory(int size);
	void FreeKeyValuesMemory(void *pMem);

	// symbol table access (used for key names)
	HKeySymbol GetSymbolForString( const char *name, bool bCreate );
	const char *GetStringForSymbol(HKeySymbol symbol);

	// returns the wide version of ansi, also does the lookup on #'d strings
	void GetLocalizedFromANSI( const char *ansi, wchar_t *outBuf, int unicodeBufferSizeInBytes);
	void GetANSIFromLocalized( const wchar_t *wchar, char *outBuf, int ansiBufferSizeInBytes );

	// for debugging, adds KeyValues record into global list so we can track memory leaks
	virtual void AddKeyValuesToMemoryLeakList(void *pMem, HKeySymbol name);
	virtual void RemoveKeyValuesFromMemoryLeakList(void *pMem);

private:
#ifdef KEYVALUES_USE_POOL
	CMemoryPool *m_pMemPool;
#endif
	int m_iMaxKeyValuesSize;

	// string hash table
	CMemoryStack m_Strings;
	struct hash_item_t
	{
		int stringIndex;
		hash_item_t *next;
	};
	CMemoryPool m_HashItemMemPool;
	CUtlVector<hash_item_t> m_HashTable;
	int CaseInsensitiveHash(const char *string, int iBounds);

	struct MemoryLeakTracker_t
	{
		int nameIndex;
		void *pMem;
	};
	static bool MemoryLeakTrackerLessFunc( const MemoryLeakTracker_t &lhs, const MemoryLeakTracker_t &rhs )
	{
		return lhs.pMem < rhs.pMem;
	}
	CUtlRBTree<MemoryLeakTracker_t, int> m_KeyValuesTrackingList;

	CThreadFastMutex m_mutex;
};

// EXPOSE_SINGLE_INTERFACE(CKeyValuesSystem, IKeyValuesSystem, KEYVALUES_INTERFACE_VERSION);

//-----------------------------------------------------------------------------
// Instance singleton and expose interface to rest of code
//-----------------------------------------------------------------------------
static CKeyValuesSystem g_KeyValuesSystem;

IKeyValuesSystem *KeyValuesSystem()
{
	return &g_KeyValuesSystem;
}

//-----------------------------------------------------------------------------
// Purpose: Constructor
//-----------------------------------------------------------------------------
CKeyValuesSystem::CKeyValuesSystem() : m_HashItemMemPool(sizeof(hash_item_t), 64, CMemoryPool::GROW_FAST, "CKeyValuesSystem::m_HashItemMemPool"), m_KeyValuesTrackingList(0, 0, MemoryLeakTrackerLessFunc)
{
	// initialize hash table
	m_HashTable.AddMultipleToTail(2047);
	for (int i = 0; i < m_HashTable.Count(); i++)
	{
		m_HashTable[i].stringIndex = 0;
		m_HashTable[i].next = NULL;
	}

	m_Strings.Init( 4*1024*1024, 64*1024, 0, 4 );
	char *pszEmpty = ((char *)m_Strings.Alloc(1));
	*pszEmpty = 0;

#ifdef KEYVALUES_USE_POOL
	m_pMemPool = NULL;
#endif
	m_iMaxKeyValuesSize = sizeof(KeyValues);
}

//-----------------------------------------------------------------------------
// Purpose: Destructor
//-----------------------------------------------------------------------------
CKeyValuesSystem::~CKeyValuesSystem()
{
#ifdef KEYVALUES_USE_POOL
#ifdef _DEBUG
	// display any memory leaks
	if (m_pMemPool && m_pMemPool->Count() > 0)
	{
		DevMsg("Leaked KeyValues blocks: %d\n", m_pMemPool->Count());
	}

	// iterate all the existing keyvalues displaying their names
	for (int i = 0; i < m_KeyValuesTrackingList.MaxElement(); i++)
	{
		if (m_KeyValuesTrackingList.IsValidIndex(i))
		{
			DevMsg("\tleaked KeyValues(%s)\n", &m_Strings[m_KeyValuesTrackingList[i].nameIndex]);
		}
	}
#endif

	delete m_pMemPool;
#endif
}

//-----------------------------------------------------------------------------
// Purpose: registers the size of the KeyValues in the specified instance
//			so it can build a properly sized memory pool for the KeyValues objects
//			the sizes will usually never differ but this is for versioning safety
//-----------------------------------------------------------------------------
void CKeyValuesSystem::RegisterSizeofKeyValues(int size)
{
	if (size > m_iMaxKeyValuesSize)
	{
		m_iMaxKeyValuesSize = size;
	}
}

static void KVLeak( char const *fmt, ... )
{
	va_list argptr; 
    char data[1024];
    
    va_start(argptr, fmt);
    Q_vsnprintf(data, sizeof( data ), fmt, argptr);
    va_end(argptr);

	Msg( data );
}

//-----------------------------------------------------------------------------
// Purpose: allocates a KeyValues object from the shared mempool
//-----------------------------------------------------------------------------
void *CKeyValuesSystem::AllocKeyValuesMemory(int size)
{
#ifdef KEYVALUES_USE_POOL
	// allocate, if we don't have one yet
	if (!m_pMemPool)
	{
		m_pMemPool = new CMemoryPool(m_iMaxKeyValuesSize, 1024, CMemoryPool::GROW_FAST, "CKeyValuesSystem::m_pMemPool" );
		m_pMemPool->SetErrorReportFunc( KVLeak );
	}

	return m_pMemPool->Alloc(size);
#else
	return malloc( size );
#endif
}

//-----------------------------------------------------------------------------
// Purpose: frees a KeyValues object from the shared mempool
//-----------------------------------------------------------------------------
void CKeyValuesSystem::FreeKeyValuesMemory(void *pMem)
{
#ifdef KEYVALUES_USE_POOL
	m_pMemPool->Free(pMem);
#else
	free( pMem );
#endif
}

//-----------------------------------------------------------------------------
// Purpose: symbol table access (used for key names)
//-----------------------------------------------------------------------------
HKeySymbol CKeyValuesSystem::GetSymbolForString( const char *name, bool bCreate )
{
	if ( !name )
	{
		return (-1);
	}

	AUTO_LOCK( m_mutex );

	int hash = CaseInsensitiveHash(name, m_HashTable.Count());
	int i = 0;
	hash_item_t *item = &m_HashTable[hash];
	while (1)
	{
		if (!stricmp(name, (char *)m_Strings.GetBase() + item->stringIndex ))
		{
			return (HKeySymbol)item->stringIndex;
		}

		i++;

		if (item->next == NULL)
		{
			if ( !bCreate )
			{
				// not found
				return -1;
			}

			// we're not in the table
			if (item->stringIndex != 0)
			{
				// first item is used, an new item
				item->next = (hash_item_t *)m_HashItemMemPool.Alloc(sizeof(hash_item_t));
				item = item->next;
			}

			// build up the new item
			item->next = NULL;
			char *pString = (char *)m_Strings.Alloc( strlen(name) + 1 );
			if ( !pString )
			{
				Error( "Out of keyvalue string space" );
				return -1;
			}
			item->stringIndex = pString - (char *)m_Strings.GetBase();
			strcpy(pString, name);
			return (HKeySymbol)item->stringIndex;
		}

		item = item->next;
	}

	// shouldn't be able to get here
	Assert(0);
	return (-1);
}

//-----------------------------------------------------------------------------
// Purpose: symbol table access
//-----------------------------------------------------------------------------
const char *CKeyValuesSystem::GetStringForSymbol(HKeySymbol symbol)
{
	if ( symbol == -1 )
	{
		return "";
	}
	return ((char *)m_Strings.GetBase() + (size_t)symbol);
}

//-----------------------------------------------------------------------------
// Purpose: adds KeyValues record into global list so we can track memory leaks
//-----------------------------------------------------------------------------
void CKeyValuesSystem::AddKeyValuesToMemoryLeakList(void *pMem, HKeySymbol name)
{
#ifdef _DEBUG
	// only track the memory leaks in debug builds
	MemoryLeakTracker_t item = { name, pMem };
	m_KeyValuesTrackingList.Insert(item);
#endif
}

//-----------------------------------------------------------------------------
// Purpose: used to track memory leaks
//-----------------------------------------------------------------------------
void CKeyValuesSystem::RemoveKeyValuesFromMemoryLeakList(void *pMem)
{
#ifdef _DEBUG
	// only track the memory leaks in debug builds
	MemoryLeakTracker_t item = { 0, pMem };
	int index = m_KeyValuesTrackingList.Find(item);
	m_KeyValuesTrackingList.RemoveAt(index);
#endif
}

//-----------------------------------------------------------------------------
// Purpose: generates a simple hash value for a string
//-----------------------------------------------------------------------------
int CKeyValuesSystem::CaseInsensitiveHash(const char *string, int iBounds)
{
	unsigned int hash = 0;

	for ( ; *string != 0; string++ )
	{
		if (*string >= 'A' && *string <= 'Z')
		{
			hash = (hash << 1) + (*string - 'A' + 'a');
		}
		else
		{
			hash = (hash << 1) + *string;
		}
	}
	  
	return hash % iBounds;
}

//========= Copyright © 1996-2005, Valve Corporation, All rights reserved. ============//
//
// Purpose: Memory allocation!
//
// $NoKeywords: $
//=============================================================================//


#include "pch_tier0.h"

#if !defined(STEAM) && !defined(NO_MALLOC_OVERRIDE)

#include <malloc.h>
#include <string.h>
#include "tier0/dbg.h"
#include "tier0/memalloc.h"
#include "mem_helpers.h"
#ifdef _WIN32
#include <crtdbg.h>
#endif
#include <map>
#include <set>
#include <limits.h>
#include "tier0/threadtools.h"
#ifdef _X360
#include "xbox/xbox_console.h"
#endif
#if ( !defined(_DEBUG) && defined(USE_MEM_DEBUG) )
#pragma message ("USE_MEM_DEBUG is enabled in a release build. Don't check this in!")
#endif
#if (defined(_DEBUG) || defined(USE_MEM_DEBUG))

#if defined(_WIN32) && ( !defined(_X360) && !defined(_WIN64) )
// #define USE_STACK_WALK
// or:
// #define USE_STACK_WALK_DETAILED
#endif

//-----------------------------------------------------------------------------

#ifndef _X360
#define DebugAlloc	malloc
#define DebugFree	free
#else
#define DebugAlloc	DmAllocatePool
#define DebugFree	DmFreePool
#endif

int g_DefaultHeapFlags = _CrtSetDbgFlag( _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG) | _CRTDBG_ALLOC_MEM_DF );

#if defined( _MEMTEST )
static char s_szStatsMapName[32];
static char s_szStatsComment[256];
#endif

//-----------------------------------------------------------------------------

#if defined( USE_STACK_WALK ) || defined( USE_STACK_WALK_DETAILED )
#include <dbghelp.h>

#pragma comment(lib, "Dbghelp.lib" )

#pragma auto_inline(off)
__declspec(naked) DWORD GetEIP()
{
	__asm 
	{
		mov eax, [ebp + 4]
		ret
	}
}

int WalkStack( void **ppAddresses, int nMaxAddresses, int nSkip = 0 )
{
	HANDLE hProcess = GetCurrentProcess();
	HANDLE hThread = GetCurrentThread();

	STACKFRAME64 frame;

	memset(&frame, 0, sizeof(frame));
	DWORD valEsp, valEbp;
	__asm
	{
		mov [valEsp], esp;
		mov [valEbp], ebp
	}
	frame.AddrPC.Offset    = GetEIP();
	frame.AddrStack.Offset = valEsp;
	frame.AddrFrame.Offset = valEbp;
	frame.AddrPC.Mode      = AddrModeFlat;
	frame.AddrStack.Mode   = AddrModeFlat;
	frame.AddrFrame.Mode   = AddrModeFlat;

	// Walk the stack.
	int nWalked = 0;
	nSkip++;
	while ( nMaxAddresses - nWalked > 0 ) 
	{
		if ( !StackWalk64(IMAGE_FILE_MACHINE_I386, hProcess, hThread, &frame, NULL, NULL, SymFunctionTableAccess64, SymGetModuleBase64, NULL ) ) 
		{
			break;
		}

		if ( nSkip == 0 )
		{
			if (frame.AddrFrame.Offset == 0) 
			{
				// End of stack.
				break;
			}

			*ppAddresses++ = (void *)frame.AddrPC.Offset;
			nWalked++;
			
			if (frame.AddrPC.Offset == frame.AddrReturn.Offset)
			{
				// Catching a stack loop
				break;
			}
		}
		else
		{
			nSkip--;
		}
	}

	if ( nMaxAddresses )
	{
		memset( ppAddresses, 0, ( nMaxAddresses - nWalked ) * sizeof(*ppAddresses) );
	}

	return nWalked;
}

bool GetModuleFromAddress( void *address, char *pResult )
{
	IMAGEHLP_MODULE   moduleInfo;

	moduleInfo.SizeOfStruct = sizeof(moduleInfo);

	if ( SymGetModuleInfo( GetCurrentProcess(), (DWORD)address, &moduleInfo ) )
	{
		strcpy( pResult, moduleInfo.ModuleName );
		return true;
	}

	return false;
}

bool GetCallerModule( char *pDest )
{
	static bool bInit;
	if ( !bInit )
	{
		PSTR psUserSearchPath = NULL;
		psUserSearchPath = "u:\\data\\game\\bin\\;u:\\data\\game\\episodic\\bin\\;u:\\data\\game\\hl2\\bin\\;\\\\perforce\\symbols";
		SymInitialize( GetCurrentProcess(), psUserSearchPath, true );
		bInit = true;
	}
	void *pCaller;
	WalkStack( &pCaller, 1, 2 );

	return ( pCaller != 0 && GetModuleFromAddress( pCaller, pDest ) );
}


#if defined( USE_STACK_WALK_DETAILED )

//
// Note: StackDescribe function is non-reentrant:
//		Reason:   Stack description is stored in a static buffer.
//		Solution: Passing caller-allocated buffers would allow the
//		function to become reentrant, however the current only client (FindOrCreateFilename)
//		is synchronized with a heap mutex, after retrieving stack description the
//		heap memory will be allocated to copy the text.
//

char * StackDescribe( void **ppAddresses, int nMaxAddresses )
{
	static char s_chStackDescription[ 32 * 1024 ];
	static char s_chSymbolBuffer[ sizeof( IMAGEHLP_SYMBOL64 ) + 1024 ];
	
	IMAGEHLP_SYMBOL64 &hlpSymbol = * ( IMAGEHLP_SYMBOL64 * ) s_chSymbolBuffer;
	hlpSymbol.SizeOfStruct = sizeof( IMAGEHLP_SYMBOL64 );
	hlpSymbol.MaxNameLength = 1024;
	DWORD64 hlpSymbolOffset = 0;

	IMAGEHLP_LINE64 hlpLine;
	hlpLine.SizeOfStruct = sizeof( IMAGEHLP_LINE64 );
	DWORD hlpLineOffset = 0;

	s_chStackDescription[ 0 ] = 0;
	char *pchBuffer = s_chStackDescription;

	for ( int k = 0; k < nMaxAddresses; ++ k )
	{
		if ( !ppAddresses[k] )
			break;

		pchBuffer += strlen( pchBuffer );
		if ( SymGetLineFromAddr64( GetCurrentProcess(), ( DWORD64 ) ppAddresses[k], &hlpLineOffset, &hlpLine ) )
		{
			char const *pchFileName = hlpLine.FileName ? hlpLine.FileName + strlen( hlpLine.FileName ) : NULL;
			for ( size_t numSlashesAllowed = 2; pchFileName > hlpLine.FileName; -- pchFileName )
			{
				if ( *pchFileName == '\\' )
				{
					if ( numSlashesAllowed -- )
						continue;
					else
						break;
				}
			}
			sprintf( pchBuffer, hlpLineOffset ? "%s:%d+0x%I32X" : "%s:%d", pchFileName, hlpLine.LineNumber, hlpLineOffset );
		}
		else if ( SymGetSymFromAddr64( GetCurrentProcess(), ( DWORD64 ) ppAddresses[k], &hlpSymbolOffset, &hlpSymbol ) )
		{
			sprintf( pchBuffer, ( hlpSymbolOffset > 0 && !( hlpSymbolOffset >> 63 ) ) ? "%s+0x%I64X" : "%s", hlpSymbol.Name, hlpSymbolOffset );
		}
		else
		{
			sprintf( pchBuffer, "#0x%08p", ppAddresses[k] );
		}

		pchBuffer += strlen( pchBuffer );
		sprintf( pchBuffer, "<--" );
	}
	*pchBuffer  = 0;

	return s_chStackDescription;
}

#endif // #if defined( USE_STACK_WALK_DETAILED )

#else

inline int WalkStack( void **ppAddresses, int nMaxAddresses, int nSkip = 0 )
{
	memset( ppAddresses, 0, nMaxAddresses * sizeof(*ppAddresses) );
	return 0;
}
#define GetModuleFromAddress( address, pResult ) ( ( *pResult = 0 ), 0)
#define GetCallerModule( pDest ) false
#endif


//-----------------------------------------------------------------------------

// NOTE: This exactly mirrors the dbg header in the MSDEV crt
// eventually when we write our own allocator, we can kill this
struct CrtDbgMemHeader_t
{
	unsigned char m_Reserved[8];
	const char *m_pFileName;
	int			m_nLineNumber;
	unsigned char m_Reserved2[16];
};

struct DbgMemHeader_t
#ifndef _DEBUG
	: CrtDbgMemHeader_t
#endif
{
	unsigned nLogicalSize;
	byte reserved[12];	// MS allocator always returns mem aligned on 16 bytes, which some of our code depends on
};

//-----------------------------------------------------------------------------

#ifdef _DEBUG
#define GetCrtDbgMemHeader( pMem ) ((CrtDbgMemHeader_t*)((DbgMemHeader_t*)pMem - 1) - 1)
#else
#define GetCrtDbgMemHeader( pMem ) ((DbgMemHeader_t*)pMem - 1)
#endif

inline void *InternalMalloc( size_t nSize, const char *pFileName, int nLine )
{
	DbgMemHeader_t *pInternalMem;
#ifndef _DEBUG
	pInternalMem = (DbgMemHeader_t *)malloc( nSize + sizeof(DbgMemHeader_t) );
	pInternalMem->m_pFileName = pFileName;
	pInternalMem->m_nLineNumber = nLine;
#else
	pInternalMem = (DbgMemHeader_t *)_malloc_dbg( nSize + sizeof(DbgMemHeader_t), _NORMAL_BLOCK, pFileName, nLine );
#endif
	pInternalMem->nLogicalSize = nSize;
	return pInternalMem + 1;
}

inline void *InternalRealloc( void *pMem, size_t nNewSize, const char *pFileName, int nLine )
{
	if ( !pMem )
		return InternalMalloc( nNewSize, pFileName, nLine );

	DbgMemHeader_t *pInternalMem = (DbgMemHeader_t *)pMem - 1;
#ifndef _DEBUG
	pInternalMem = (DbgMemHeader_t *)realloc( pInternalMem, nNewSize + sizeof(DbgMemHeader_t) );
	pInternalMem->m_pFileName = pFileName;
	pInternalMem->m_nLineNumber = nLine;
#else
	pInternalMem = (DbgMemHeader_t *)_realloc_dbg( pInternalMem, nNewSize + sizeof(DbgMemHeader_t), _NORMAL_BLOCK, pFileName, nLine );
#endif
	pInternalMem->nLogicalSize = nNewSize;
	return pInternalMem + 1;
}

inline void InternalFree( void *pMem )
{
	if ( !pMem )
		return;

	DbgMemHeader_t *pInternalMem = (DbgMemHeader_t *)pMem - 1;
#ifndef _DEBUG
	free( pInternalMem );
#else
	_free_dbg( pInternalMem, _NORMAL_BLOCK );
#endif
}

inline size_t InternalMSize( void *pMem )
{
	DbgMemHeader_t *pInternalMem = (DbgMemHeader_t *)pMem - 1;
#ifdef _LINUX
	return pInternalMem->nLogicalSize;
#else
#ifndef _DEBUG
	return _msize( pInternalMem ) - sizeof(DbgMemHeader_t);
#else
	return _msize_dbg( pInternalMem, _NORMAL_BLOCK ) - sizeof(DbgMemHeader_t);
#endif
#endif // _LINUX
}

inline size_t InternalLogicalSize( void *pMem )
{
	DbgMemHeader_t *pInternalMem = (DbgMemHeader_t *)pMem - 1;
	return pInternalMem->nLogicalSize;
}

#ifndef _DEBUG
#define _CrtDbgReport( nRptType, szFile, nLine, szModule, pMsg ) 0
#endif

//-----------------------------------------------------------------------------


// Custom allocator protects this module from recursing on operator new
template <class T>
class CNoRecurseAllocator
{
public:
	// type definitions
	typedef T        value_type;
	typedef T*       pointer;
	typedef const T* const_pointer;
	typedef T&       reference;
	typedef const T& const_reference;
	typedef std::size_t    size_type;
	typedef std::ptrdiff_t difference_type;

	CNoRecurseAllocator() {}
	CNoRecurseAllocator(const CNoRecurseAllocator&) {}
	template <class U> CNoRecurseAllocator(const CNoRecurseAllocator<U>&) {}
	~CNoRecurseAllocator(){}

	// rebind allocator to type U
	template <class U > struct rebind { typedef CNoRecurseAllocator<U> other; };

	// return address of values
	pointer address (reference value) const { return &value; }

	const_pointer address (const_reference value) const { return &value;}
	size_type max_size() const { return INT_MAX; }

	pointer allocate(size_type num, const void* = 0)  { return (pointer)DebugAlloc(num * sizeof(T)); }
	void deallocate (pointer p, size_type num) { DebugFree(p); }
	void construct(pointer p, const T& value) {	new((void*)p)T(value); }
	void destroy (pointer p) { p->~T(); }
};

template <class T1, class T2>
bool operator==(const CNoRecurseAllocator<T1>&, const CNoRecurseAllocator<T2>&)
{
	return true;
}

template <class T1, class T2>
bool operator!=(const CNoRecurseAllocator<T1>&, const CNoRecurseAllocator<T2>&)
{
	return false;
}

class CStringLess
{
public:
	bool operator()(const char *pszLeft, const char *pszRight ) const 
	{
		return ( stricmp( pszLeft, pszRight ) < 0 );
	}
};

//-----------------------------------------------------------------------------

#pragma warning( disable:4074 ) // warning C4074: initializers put in compiler reserved initialization area
#pragma init_seg( compiler )

//-----------------------------------------------------------------------------
// NOTE! This should never be called directly from leaf code
// Just use new,delete,malloc,free etc. They will call into this eventually
//-----------------------------------------------------------------------------
class CDbgMemAlloc : public IMemAlloc
{
public:
	CDbgMemAlloc();
	virtual ~CDbgMemAlloc();

	// Release versions
	virtual void *Alloc( size_t nSize );
	virtual void *Realloc( void *pMem, size_t nSize );
	virtual void  Free( void *pMem );
    virtual void *Expand_NoLongerSupported( void *pMem, size_t nSize );

	// Debug versions
    virtual void *Alloc( size_t nSize, const char *pFileName, int nLine );
    virtual void *Realloc( void *pMem, size_t nSize, const char *pFileName, int nLine );
    virtual void  Free( void *pMem, const char *pFileName, int nLine );
    virtual void *Expand_NoLongerSupported( void *pMem, size_t nSize, const char *pFileName, int nLine );

	// Returns size of a particular allocation
	virtual size_t GetSize( void *pMem );

    // Force file + line information for an allocation
    virtual void PushAllocDbgInfo( const char *pFileName, int nLine );
    virtual void PopAllocDbgInfo();

	virtual long CrtSetBreakAlloc( long lNewBreakAlloc );
	virtual	int CrtSetReportMode( int nReportType, int nReportMode );
	virtual int CrtIsValidHeapPointer( const void *pMem );
	virtual int CrtIsValidPointer( const void *pMem, unsigned int size, int access );
	virtual int CrtCheckMemory( void );
	virtual int CrtSetDbgFlag( int nNewFlag );
	virtual void CrtMemCheckpoint( _CrtMemState *pState );

	// FIXME: Remove when we have our own allocator
	virtual void* CrtSetReportFile( int nRptType, void* hFile );
	virtual void* CrtSetReportHook( void* pfnNewHook );
	virtual int CrtDbgReport( int nRptType, const char * szFile,
			int nLine, const char * szModule, const char * szFormat );

	virtual int heapchk();

	virtual bool IsDebugHeap() { return true; }

	virtual int GetVersion() { return MEMALLOC_VERSION; }

	virtual void CompactHeap() 
	{
#if defined( _X360 ) && defined( _DEBUG )
		HeapCompact( GetProcessHeap(), 0 );
#endif
	}

	virtual MemAllocFailHandler_t SetAllocFailHandler( MemAllocFailHandler_t pfnMemAllocFailHandler ) { return NULL; } // debug heap doesn't attempt retries

#if defined( _MEMTEST )
	void SetStatsExtraInfo( const char *pMapName, const char *pComment )
	{
		strncpy( s_szStatsMapName, pMapName, sizeof( s_szStatsMapName ) );
		s_szStatsMapName[sizeof( s_szStatsMapName ) - 1] = '\0';

		strncpy( s_szStatsComment, pComment, sizeof( s_szStatsComment ) );
		s_szStatsComment[sizeof( s_szStatsComment ) - 1] = '\0';
	}
#endif

	virtual size_t MemoryAllocFailed();
	void		SetCRTAllocFailed( size_t nMemSize );

	enum
	{
		BYTE_COUNT_16 = 0,
		BYTE_COUNT_32,
		BYTE_COUNT_128,
		BYTE_COUNT_1024,
		BYTE_COUNT_GREATER,

		NUM_BYTE_COUNT_BUCKETS
	};

private:
	struct MemInfo_t
	{
		MemInfo_t()
		{
			memset( this, 0, sizeof(*this) );
		}

		// Size in bytes
		int m_nCurrentSize;
		int m_nPeakSize;
		int m_nTotalSize;
		int m_nOverheadSize;
		int m_nPeakOverheadSize;

		// Count in terms of # of allocations
		int m_nCurrentCount;
		int m_nPeakCount;
		int m_nTotalCount;

		// Count in terms of # of allocations of a particular size
		int m_pCount[NUM_BYTE_COUNT_BUCKETS];

		// Time spent allocating + deallocating	(microseconds)
		int64 m_nTime;
	};

	struct MemInfoKey_t
	{
		MemInfoKey_t( const char *pFileName, int line ) : m_pFileName(pFileName), m_nLine(line) {}
		bool operator<( const MemInfoKey_t &key ) const
		{
			int iret = stricmp( m_pFileName, key.m_pFileName );
			if ( iret < 0 )
				return true;

			if ( iret > 0 )
				return false;

			return m_nLine < key.m_nLine;
		}

		const char *m_pFileName;
		int			m_nLine;
	};

	// NOTE: Deliberately using STL here because the UTL stuff
	// is a client of this library; want to avoid circular dependency

	// Maps file name to info
	typedef std::map< MemInfoKey_t, MemInfo_t, std::less<MemInfoKey_t>, CNoRecurseAllocator<std::pair<const MemInfoKey_t, MemInfo_t> > > StatMap_t;
	typedef StatMap_t::iterator StatMapIter_t;
	typedef StatMap_t::value_type StatMapEntry_t;

	typedef std::set<const char *, CStringLess, CNoRecurseAllocator<const char *> > Filenames_t;

	// Heap reporting method
	typedef void (*HeapReportFunc_t)( char const *pFormat, ... );

private:
	// Returns the actual debug info
	void GetActualDbgInfo( const char *&pFileName, int &nLine );

	// Finds the file in our map
	MemInfo_t &FindOrCreateEntry( const char *pFileName, int line );
	const char *FindOrCreateFilename( const char *pFileName );

	// Updates stats
	void RegisterAllocation( const char *pFileName, int nLine, int nLogicalSize, int nActualSize, unsigned nTime );
	void RegisterDeallocation( const char *pFileName, int nLine, int nLogicalSize, int nActualSize, unsigned nTime );

	void RegisterAllocation( MemInfo_t &info, int nLogicalSize, int nActualSize, unsigned nTime );
	void RegisterDeallocation( MemInfo_t &info, int nLogicalSize, int nActualSize, unsigned nTime );

	// Gets the allocation file name
	const char *GetAllocatonFileName( void *pMem );
	int GetAllocatonLineNumber( void *pMem );

	// FIXME: specify a spew output func for dumping stats
	// Stat output
	void DumpMemInfo( const char *pAllocationName, int line, const MemInfo_t &info );
	void DumpFileStats();
	void DumpStats();
	void DumpStatsFileBase( char const *pchFileBase );
	void DumpBlockStats( void *p );

private:
	StatMap_t m_StatMap;
	MemInfo_t m_GlobalInfo;
	CFastTimer m_Timer;
	bool		m_bInitialized;
	Filenames_t m_Filenames;

	HeapReportFunc_t m_OutputFunc;

	static int s_pCountSizes[NUM_BYTE_COUNT_BUCKETS];
	static const char *s_pCountHeader[NUM_BYTE_COUNT_BUCKETS];

	size_t				m_sMemoryAllocFailed;
};

static char const *g_pszUnknown = "unknown";

//-----------------------------------------------------------------------------

const int DBG_INFO_STACK_DEPTH = 32;

struct DbgInfoStack_t
{
	const char *m_pFileName;
	int m_nLine;
};

CThreadLocalPtr<DbgInfoStack_t> g_DbgInfoStack CONSTRUCT_EARLY;
CThreadLocalInt<>				g_nDbgInfoStackDepth CONSTRUCT_EARLY;

//-----------------------------------------------------------------------------
// Singleton...
//-----------------------------------------------------------------------------
static CDbgMemAlloc s_DbgMemAlloc CONSTRUCT_EARLY;

#ifndef TIER0_VALIDATE_HEAP
IMemAlloc *g_pMemAlloc = &s_DbgMemAlloc;
#else
IMemAlloc *g_pActualAlloc = &s_DbgMemAlloc;
#endif

//-----------------------------------------------------------------------------

CThreadMutex g_DbgMemMutex CONSTRUCT_EARLY;

#define HEAP_LOCK() AUTO_LOCK( g_DbgMemMutex )


//-----------------------------------------------------------------------------
// Byte count buckets
//-----------------------------------------------------------------------------
int CDbgMemAlloc::s_pCountSizes[CDbgMemAlloc::NUM_BYTE_COUNT_BUCKETS] = 
{
	16, 32, 128, 1024, INT_MAX
};

const char *CDbgMemAlloc::s_pCountHeader[CDbgMemAlloc::NUM_BYTE_COUNT_BUCKETS] = 
{
	"<=16 byte allocations", 
	"17-32 byte allocations",
	"33-128 byte allocations", 
	"129-1024 byte allocations",
	">1024 byte allocations"
};

//-----------------------------------------------------------------------------
// Standard output
//-----------------------------------------------------------------------------
static FILE* s_DbgFile;

static void DefaultHeapReportFunc( char const *pFormat, ... )
{
	va_list args;
	va_start( args, pFormat );
	vfprintf( s_DbgFile, pFormat, args );
	va_end( args );
}

//-----------------------------------------------------------------------------
// Constructor
//-----------------------------------------------------------------------------
CDbgMemAlloc::CDbgMemAlloc() : m_sMemoryAllocFailed( (size_t)0 )
{
	CClockSpeedInit::Init();

	m_OutputFunc = DefaultHeapReportFunc;
	m_bInitialized = true;

	if ( !IsDebug() && !IsX360() )
	{
		Plat_DebugString( "USE_MEM_DEBUG is enabled in a release build. Don't check this in!\n" );
	}
}

CDbgMemAlloc::~CDbgMemAlloc()
{
	Filenames_t::const_iterator iter = m_Filenames.begin();
	while(iter != m_Filenames.end())
	{
		char *pFileName = (char*)(*iter);
		free( pFileName );
		iter++;
	}
	m_bInitialized = false;
}


//-----------------------------------------------------------------------------
// Release versions
//-----------------------------------------------------------------------------

void *CDbgMemAlloc::Alloc( size_t nSize )
{
/*
	// NOTE: Uncomment this to find unknown allocations
	const char *pFileName = g_pszUnknown;
	int nLine;
	GetActualDbgInfo( pFileName, nLine );
	if (pFileName == g_pszUnknown)
	{
		int x = 3;
	}
*/
	char szModule[MAX_PATH];
	if ( GetCallerModule( szModule ) )
	{
		return Alloc( nSize, szModule, 0 );
	}
	else
	{
		return Alloc( nSize, g_pszUnknown, 0 );
	}
//	return malloc( nSize );
}

void *CDbgMemAlloc::Realloc( void *pMem, size_t nSize )
{
/*
	// NOTE: Uncomment this to find unknown allocations
	const char *pFileName = g_pszUnknown;
	int nLine;
	GetActualDbgInfo( pFileName, nLine );
	if (pFileName == g_pszUnknown)
	{
		int x = 3;
	}
*/
	// FIXME: Should these gather stats?
	char szModule[MAX_PATH];
	if ( GetCallerModule( szModule ) )
	{
		return Realloc( pMem, nSize, szModule, 0 );
	}
	else
	{
		return Realloc( pMem, nSize, g_pszUnknown, 0 );
	}
//	return realloc( pMem, nSize );
}

void CDbgMemAlloc::Free( void *pMem )
{
	// FIXME: Should these gather stats?
	Free( pMem, g_pszUnknown, 0 );
//	free( pMem );
}

void *CDbgMemAlloc::Expand_NoLongerSupported( void *pMem, size_t nSize )
{
	return NULL;
}


//-----------------------------------------------------------------------------
// Force file + line information for an allocation
//-----------------------------------------------------------------------------
void CDbgMemAlloc::PushAllocDbgInfo( const char *pFileName, int nLine )
{
	if ( g_DbgInfoStack == NULL )
	{
		g_DbgInfoStack = (DbgInfoStack_t *)DebugAlloc( sizeof(DbgInfoStack_t) * DBG_INFO_STACK_DEPTH );
		g_nDbgInfoStackDepth = -1;
	}

	++g_nDbgInfoStackDepth;
	Assert( g_nDbgInfoStackDepth < DBG_INFO_STACK_DEPTH );
	g_DbgInfoStack[g_nDbgInfoStackDepth].m_pFileName = FindOrCreateFilename( pFileName );
	g_DbgInfoStack[g_nDbgInfoStackDepth].m_nLine = nLine;
}

void CDbgMemAlloc::PopAllocDbgInfo()
{
	if ( g_DbgInfoStack == NULL )
	{
		g_DbgInfoStack = (DbgInfoStack_t *)DebugAlloc( sizeof(DbgInfoStack_t) * DBG_INFO_STACK_DEPTH );
		g_nDbgInfoStackDepth = -1;
	}

	--g_nDbgInfoStackDepth;
	Assert( g_nDbgInfoStackDepth >= -1 );
}


//-----------------------------------------------------------------------------
// Returns the actual debug info
//-----------------------------------------------------------------------------
void CDbgMemAlloc::GetActualDbgInfo( const char *&pFileName, int &nLine )
{
#if defined( USE_STACK_WALK_DETAILED )
	return;
#endif

	if ( g_DbgInfoStack == NULL )
	{
		g_DbgInfoStack = (DbgInfoStack_t *)DebugAlloc( sizeof(DbgInfoStack_t) * DBG_INFO_STACK_DEPTH );
		g_nDbgInfoStackDepth = -1;
	}

	if ( g_nDbgInfoStackDepth >= 0 && g_DbgInfoStack[0].m_pFileName)
	{
		pFileName = g_DbgInfoStack[0].m_pFileName;
		nLine = g_DbgInfoStack[0].m_nLine;
	}
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
const char *CDbgMemAlloc::FindOrCreateFilename( const char *pFileName )
{
	// If we created it for the first time, actually *allocate* the filename memory
	HEAP_LOCK();
	// This is necessary for shutdown conditions: the file name is stored
	// in some piece of memory in a DLL; if that DLL becomes unloaded,
	// we'll have a pointer to crap memory

	if ( !pFileName )
	{
		pFileName = g_pszUnknown;
	}

#if defined( USE_STACK_WALK_DETAILED )
{

	// Walk the stack to determine what's causing the allocation
	void *arrStackAddresses[ 10 ] = { 0 };
	int numStackAddrRetrieved = WalkStack( arrStackAddresses, 10, 0 );
	char *szStack = StackDescribe( arrStackAddresses, numStackAddrRetrieved );
	if ( szStack && *szStack )
	{
		pFileName = szStack;		// Use the stack description for the allocation
	}

}
#endif // #if defined( USE_STACK_WALK_DETAILED )

	char *pszFilenameCopy;
	Filenames_t::const_iterator iter = m_Filenames.find( pFileName );
	if ( iter == m_Filenames.end() )
	{
		int nLen = strlen(pFileName) + 1;
		pszFilenameCopy = (char *)DebugAlloc( nLen );
		memcpy( pszFilenameCopy, pFileName, nLen );
		m_Filenames.insert( pszFilenameCopy );
	}
	else
	{
		pszFilenameCopy = (char *)(*iter);
	}

	return pszFilenameCopy;
}

//-----------------------------------------------------------------------------
// Finds the file in our map
//-----------------------------------------------------------------------------
CDbgMemAlloc::MemInfo_t &CDbgMemAlloc::FindOrCreateEntry( const char *pFileName, int line )
{
	// Oh how I love crazy STL. retval.first == the StatMapIter_t in the std::pair
	// retval.first->second == the MemInfo_t that's part of the StatMapIter_t 
	std::pair<StatMapIter_t, bool> retval;
	retval = m_StatMap.insert( StatMapEntry_t( MemInfoKey_t( pFileName, line ), MemInfo_t() ) );
	return retval.first->second;
}


//-----------------------------------------------------------------------------
// Updates stats
//-----------------------------------------------------------------------------
void CDbgMemAlloc::RegisterAllocation( const char *pFileName, int nLine, int nLogicalSize, int nActualSize, unsigned nTime )
{
	HEAP_LOCK();
	RegisterAllocation( m_GlobalInfo, nLogicalSize, nActualSize, nTime );
	RegisterAllocation( FindOrCreateEntry( pFileName, nLine ), nLogicalSize, nActualSize, nTime );
}

void CDbgMemAlloc::RegisterDeallocation( const char *pFileName, int nLine, int nLogicalSize, int nActualSize, unsigned nTime )
{
	HEAP_LOCK();
	RegisterDeallocation( m_GlobalInfo, nLogicalSize, nActualSize, nTime );
	RegisterDeallocation( FindOrCreateEntry( pFileName, nLine ), nLogicalSize, nActualSize, nTime );
}

void CDbgMemAlloc::RegisterAllocation( MemInfo_t &info, int nLogicalSize, int nActualSize, unsigned nTime )
{
	++info.m_nCurrentCount;
	++info.m_nTotalCount;
	if (info.m_nCurrentCount > info.m_nPeakCount)
	{
		info.m_nPeakCount = info.m_nCurrentCount;
	}

	info.m_nCurrentSize += nLogicalSize;
	info.m_nTotalSize += nLogicalSize;
	if (info.m_nCurrentSize > info.m_nPeakSize)
	{
		info.m_nPeakSize = info.m_nCurrentSize;
	}

	for (int i = 0; i < NUM_BYTE_COUNT_BUCKETS; ++i)
	{
		if (nLogicalSize <= s_pCountSizes[i])
		{
			++info.m_pCount[i];
			break;
		}
	}

	Assert( info.m_nPeakCount >= info.m_nCurrentCount );
	Assert( info.m_nPeakSize >= info.m_nCurrentSize );

	info.m_nOverheadSize += (nActualSize - nLogicalSize);
	if (info.m_nOverheadSize > info.m_nPeakOverheadSize)
	{
		info.m_nPeakOverheadSize = info.m_nOverheadSize;
	}

	info.m_nTime += nTime;
}

void CDbgMemAlloc::RegisterDeallocation( MemInfo_t &info, int nLogicalSize, int nActualSize, unsigned nTime )
{
	--info.m_nCurrentCount;
	info.m_nCurrentSize -= nLogicalSize;

	for (int i = 0; i < NUM_BYTE_COUNT_BUCKETS; ++i)
	{
		if (nLogicalSize <= s_pCountSizes[i])
		{
			--info.m_pCount[i];
			break;
		}
	}

	Assert( info.m_nPeakCount >= info.m_nCurrentCount );
	Assert( info.m_nPeakSize >= info.m_nCurrentSize );
	Assert( info.m_nCurrentCount >= 0 );
	Assert( info.m_nCurrentSize >= 0 );

	info.m_nOverheadSize -= (nActualSize - nLogicalSize);

	info.m_nTime += nTime;
}


//-----------------------------------------------------------------------------
// Gets the allocation file name
//-----------------------------------------------------------------------------

const char *CDbgMemAlloc::GetAllocatonFileName( void *pMem )
{
	if (!pMem)
		return "";

	CrtDbgMemHeader_t *pHeader = GetCrtDbgMemHeader( pMem );
	if ( pHeader->m_pFileName )
		return pHeader->m_pFileName;
	else
		return g_pszUnknown;
}

//-----------------------------------------------------------------------------
// Gets the allocation file name
//-----------------------------------------------------------------------------
int CDbgMemAlloc::GetAllocatonLineNumber( void *pMem )
{
	if ( !pMem )
		return 0;

	CrtDbgMemHeader_t *pHeader = GetCrtDbgMemHeader( pMem );
	return pHeader->m_nLineNumber;
}

//-----------------------------------------------------------------------------
// Debug versions of the main allocation methods
//-----------------------------------------------------------------------------
void *CDbgMemAlloc::Alloc( size_t nSize, const char *pFileName, int nLine )
{
	HEAP_LOCK();

	if ( !m_bInitialized )
		return InternalMalloc( nSize, pFileName, nLine );

	if ( pFileName != g_pszUnknown )
		pFileName = FindOrCreateFilename( pFileName );

	GetActualDbgInfo( pFileName, nLine );

	/*
	if ( strcmp( pFileName, "class CUtlVector<int,class CUtlMemory<int> >" ) == 0)
	{
		GetActualDbgInfo( pFileName, nLine );
	}
	*/

	m_Timer.Start();
	void *pMem = InternalMalloc( nSize, pFileName, nLine );
	m_Timer.End();

	ApplyMemoryInitializations( pMem, nSize );

	RegisterAllocation( GetAllocatonFileName( pMem ), GetAllocatonLineNumber( pMem ), InternalLogicalSize( pMem), InternalMSize( pMem ), m_Timer.GetDuration().GetMicroseconds() );

	if ( !pMem )
	{
		SetCRTAllocFailed( nSize );
	}
	return pMem;
}

void *CDbgMemAlloc::Realloc( void *pMem, size_t nSize, const char *pFileName, int nLine )
{
	HEAP_LOCK();

	pFileName = FindOrCreateFilename( pFileName );

	if ( !m_bInitialized )
		return InternalRealloc( pMem, nSize, pFileName, nLine );

	if ( pMem != 0 )
	{
		RegisterDeallocation( GetAllocatonFileName( pMem ), GetAllocatonLineNumber( pMem ), InternalLogicalSize( pMem), InternalMSize( pMem ), 0 );
	}

	GetActualDbgInfo( pFileName, nLine );

	m_Timer.Start();
	pMem = InternalRealloc( pMem, nSize, pFileName, nLine );
	m_Timer.End();
	
	RegisterAllocation( GetAllocatonFileName( pMem ), GetAllocatonLineNumber( pMem ), InternalLogicalSize( pMem), InternalMSize( pMem ), m_Timer.GetDuration().GetMicroseconds() );
	if ( !pMem )
	{
		SetCRTAllocFailed( nSize );
	}
	return pMem;
}

void  CDbgMemAlloc::Free( void *pMem, const char * /*pFileName*/, int nLine )
{
	if ( !pMem )
		return;

	HEAP_LOCK();

	if ( !m_bInitialized )
	{
		InternalFree( pMem );
		return;
	}

	int nOldLogicalSize = InternalLogicalSize( pMem );
	int nOldSize = InternalMSize( pMem );
	const char *pOldFileName = GetAllocatonFileName( pMem );
	int oldLine = GetAllocatonLineNumber( pMem );

	m_Timer.Start();
	InternalFree( pMem );
 	m_Timer.End();

	RegisterDeallocation( pOldFileName, oldLine, nOldLogicalSize, nOldSize, m_Timer.GetDuration().GetMicroseconds() );
}

void *CDbgMemAlloc::Expand_NoLongerSupported( void *pMem, size_t nSize, const char *pFileName, int nLine )
{
	return NULL;
}


//-----------------------------------------------------------------------------
// Returns size of a particular allocation
//-----------------------------------------------------------------------------
size_t CDbgMemAlloc::GetSize( void *pMem )
{
	HEAP_LOCK();

	if ( !pMem )
		return CalcHeapUsed();

	return InternalMSize( pMem );
}


//-----------------------------------------------------------------------------
// FIXME: Remove when we make our own heap! Crt stuff we're currently using
//-----------------------------------------------------------------------------
long CDbgMemAlloc::CrtSetBreakAlloc( long lNewBreakAlloc )
{
#ifdef _LINUX
	return 0;
#else
	return _CrtSetBreakAlloc( lNewBreakAlloc );
#endif
}

int CDbgMemAlloc::CrtSetReportMode( int nReportType, int nReportMode )
{
#ifdef _LINUX
	return 0;
#else
	return _CrtSetReportMode( nReportType, nReportMode );
#endif
}

int CDbgMemAlloc::CrtIsValidHeapPointer( const void *pMem )
{
#ifdef _LINUX
	return 0;
#else
	return _CrtIsValidHeapPointer( pMem );
#endif
}

int CDbgMemAlloc::CrtIsValidPointer( const void *pMem, unsigned int size, int access )
{
#ifdef _LINUX
	return 0;
#else
	return _CrtIsValidPointer( pMem, size, access );
#endif
}

#define DBGMEM_CHECKMEMORY 1

int CDbgMemAlloc::CrtCheckMemory( void )
{
#ifndef DBGMEM_CHECKMEMORY
	return 1;
#else
	if ( !_CrtCheckMemory())
	{
		Msg( "Memory check failed!\n" );
		return 0;
	}
	return 1;
#endif
}

int CDbgMemAlloc::CrtSetDbgFlag( int nNewFlag )
{
#ifdef _LINUX
	return 0;
#else
	return _CrtSetDbgFlag( nNewFlag );
#endif
}

void CDbgMemAlloc::CrtMemCheckpoint( _CrtMemState *pState )
{
#ifdef _LINUX
	return 0;
#else
	_CrtMemCheckpoint( pState );
#endif
}

// FIXME: Remove when we have our own allocator
void* CDbgMemAlloc::CrtSetReportFile( int nRptType, void* hFile )
{
#ifdef _LINUX
	return 0;
#else
	return (void*)_CrtSetReportFile( nRptType, (_HFILE)hFile );
#endif
}

void* CDbgMemAlloc::CrtSetReportHook( void* pfnNewHook )
{
#ifdef _LINUX
	return 0;
#else
	return (void*)_CrtSetReportHook( (_CRT_REPORT_HOOK)pfnNewHook );
#endif
}

int CDbgMemAlloc::CrtDbgReport( int nRptType, const char * szFile,
		int nLine, const char * szModule, const char * pMsg )
{
#ifdef _LINUX
	return 0;
#else
	return _CrtDbgReport( nRptType, szFile, nLine, szModule, pMsg );
#endif
}

int CDbgMemAlloc::heapchk()
{
#ifdef _LINUX
	return 0;
#else
	return _HEAPOK;
#endif
}

void CDbgMemAlloc::DumpBlockStats( void *p )
{
	DbgMemHeader_t *pBlock = (DbgMemHeader_t *)p - 1;
	if ( !CrtIsValidHeapPointer( pBlock ) )
	{
		Msg( "0x%x is not valid heap pointer\n", p );
		return;
	}

	const char *pFileName = GetAllocatonFileName( p );
	int line = GetAllocatonLineNumber( p );

	Msg( "0x%x allocated by %s line %d, %d bytes\n", p, pFileName, line, GetSize( p ) );
}

//-----------------------------------------------------------------------------
// Stat output
//-----------------------------------------------------------------------------
void CDbgMemAlloc::DumpMemInfo( const char *pAllocationName, int line, const MemInfo_t &info )
{
	m_OutputFunc("%s, line %i\t%.1f\t%.1f\t%.1f\t%.1f\t%.1f\t%d\t%d\t%d\t%d",
		pAllocationName,
		line,
		info.m_nCurrentSize / 1024.0f,
		info.m_nPeakSize / 1024.0f,
		info.m_nTotalSize / 1024.0f,
		info.m_nOverheadSize / 1024.0f,
		info.m_nPeakOverheadSize / 1024.0f,
		(int)(info.m_nTime / 1000),
		info.m_nCurrentCount,
		info.m_nPeakCount,
		info.m_nTotalCount
		);

	for (int i = 0; i < NUM_BYTE_COUNT_BUCKETS; ++i)
	{
		m_OutputFunc( "\t%d", info.m_pCount[i] );
	}

	m_OutputFunc("\n");
}


//-----------------------------------------------------------------------------
// Stat output
//-----------------------------------------------------------------------------
void CDbgMemAlloc::DumpFileStats()
{
	StatMapIter_t iter = m_StatMap.begin();
	while(iter != m_StatMap.end())
	{
		DumpMemInfo( iter->first.m_pFileName, iter->first.m_nLine, iter->second );
		iter++;
	}
}

void CDbgMemAlloc::DumpStatsFileBase( char const *pchFileBase )
{
	HEAP_LOCK();

	char szFileName[MAX_PATH];
	static int s_FileCount = 0;
	if (m_OutputFunc == DefaultHeapReportFunc)
	{
		char *pPath = "";
		if ( IsX360() )
		{
			pPath = "D:\\";
		}

#if defined( _MEMTEST )
		char szXboxName[32];
		strcpy( szXboxName, "xbox" );
		DWORD numChars = sizeof( szXboxName );
		DmGetXboxName( szXboxName, &numChars ); 
		char *pXboxName = strstr( szXboxName, "_360" );
		if ( pXboxName )
		{
			*pXboxName = '\0';
		}

		SYSTEMTIME systemTime;
		GetLocalTime( &systemTime );
		_snprintf( szFileName, sizeof( szFileName ), "%s%s_%2.2d%2.2d_%2.2d%2.2d%2.2d_%d.txt", pPath, s_szStatsMapName, systemTime.wMonth, systemTime.wDay, systemTime.wHour, systemTime.wMinute, systemTime.wSecond, s_FileCount );
#else
		_snprintf( szFileName, sizeof( szFileName ), "%s%s%d.txt", pPath, pchFileBase, s_FileCount );
#endif

		++s_FileCount;

		s_DbgFile = fopen(szFileName, "wt");
		if (!s_DbgFile)
			return;
	}

	m_OutputFunc("Allocation type\tCurrent Size(k)\tPeak Size(k)\tTotal Allocations(k)\tOverhead Size(k)\tPeak Overhead Size(k)\tTime(ms)\tCurrent Count\tPeak Count\tTotal Count");

	for (int i = 0; i < NUM_BYTE_COUNT_BUCKETS; ++i)
	{
		m_OutputFunc( "\t%s", s_pCountHeader[i] );
	}

	m_OutputFunc("\n");

	DumpMemInfo( "Totals", 0, m_GlobalInfo );

	if ( IsX360() )
	{
		// add a line that has free memory
		MEMORYSTATUS stat;
		GlobalMemoryStatus( &stat );
		MemInfo_t info;
		// OS takes 32 MB, report our internal allocations only
		info.m_nCurrentSize = ( stat.dwTotalPhys - stat.dwAvailPhys ) - 32*1024*1024;
		DumpMemInfo( "Used Memory", 0, info );
	}

	DumpFileStats();

	if (m_OutputFunc == DefaultHeapReportFunc)
	{
		fclose(s_DbgFile);

#if defined( _X360 ) && !defined( _RETAIL )
		XBX_rMemDump( szFileName );
#endif
	}
}

//-----------------------------------------------------------------------------
// Stat output
//-----------------------------------------------------------------------------
void CDbgMemAlloc::DumpStats()
{
	DumpStatsFileBase( "memstats" );
}

void CDbgMemAlloc::SetCRTAllocFailed( size_t nSize )
{
	m_sMemoryAllocFailed = nSize;
}

size_t CDbgMemAlloc::MemoryAllocFailed()
{
	return m_sMemoryAllocFailed;
}


#endif // _DEBUG

#endif // !STEAM && !NO_MALLOC_OVERRIDE

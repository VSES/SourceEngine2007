#pragma once

#pragma warning(disable: 4995)
#include <list>
#include <string>
#include "Windows.h"
#include "Mmsystem.h"
#pragma warning(default: 4995)

#include "sas.h"
#include "macros.h"

struct DXV_FIND_DATA
{
    WIN32_FIND_DATA FileData;
    WCHAR FullPath[MAX_PATH];
};

class WatchedFile
{
public:
    std::wstring strPath;
    FILETIME lastWriteTime;

    WatchedFile();
    WatchedFile( DXV_FIND_DATA& findData );
};

class FileWatcher
{
public:
    FRIEND_SINGLETON_H( FileWatcher );

    HRESULT Add( const WCHAR* strFilename );
    HRESULT Remove( const WCHAR* strFilename );
    void Clear();

    bool CheckForChanges();
    DWORD GetTimeLastCheck() { return m_timeLastCheck; }

protected:

    bool FindWatcher( const WCHAR* strFilename , std::list<WatchedFile>::iterator& itr);

    DWORD m_timeLastCheck;
    std::list<WatchedFile> m_WatchedFiles;

    FileWatcher();
};

class FileFinder : public Sas::Integrator
{
public:
    FRIEND_SINGLETON_H( FileFinder );

    void AddSearchPath( const WCHAR* strFilename );
    void RemoveSearchPath( const WCHAR* strFilename );
    void ClearSearchPaths();

    //alternateDirectory is an alternate first search path to try for relative paths
    bool FindFile( DXV_FIND_DATA& outData, const WCHAR* path, const WCHAR* alternateDirectory = NULL );

    
    virtual bool FindFile(const WCHAR* find, const WCHAR* alternateDirectory, Sas::SourceInfo& found );


protected:
    static bool Resolve(DXV_FIND_DATA& outData, const WCHAR* path );
    std::list<std::wstring>::iterator FindSearchPath( const WCHAR* path );

    std::list<std::wstring> m_SearchPaths;

    FileFinder();
};

DECLARE_SINGLETON_H( FileWatcher );
DECLARE_SINGLETON_H( FileFinder );

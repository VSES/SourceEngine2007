#include "assert.h"
#include "windows.h"

#pragma warning(disable: 4995)
#include <strsafe.h>
#include <shlwapi.h>
#include <shlobj.h>
#pragma warning(default: 4995)

#include "FileSystem.h"


DECLARE_SINGLETON_CPP( FileWatcher );
DECLARE_SINGLETON_CPP( FileFinder );


bool FileFinder::Resolve(DXV_FIND_DATA& outData, const WCHAR* path )
{
    bool result = false;
    
    HANDLE handle = FindFirstFile( path, &outData.FileData );
    if( handle != INVALID_HANDLE_VALUE )
    {
        StringCchCopy( outData.FullPath, MAX_PATH, path);
        result = true;
    }

    FindClose(handle);

    return result;
}


bool FileFinder::FindFile(DXV_FIND_DATA& outData, const WCHAR* path, const WCHAR* alternateDirectory)
{

    WCHAR InDir[MAX_PATH] = L"";
    if(MAX_PATH <= ExpandEnvironmentStrings(path, InDir, MAX_PATH) )
    {
        assert(false);
        return false;
    }    
    
    //get rid of '/'
    for(int i = 0; i < MAX_PATH; i++)
    {
        if(InDir[i] == L'/')
            InDir[i] = L'\\';
    }

    {//remove "s
        wstring tmp(InDir);
        wstring::size_type first = tmp.find_first_of( L'\"' );
        if( first != wstring::npos )
        {
            wstring::size_type last = tmp.find_last_of( L'\"' );
            tmp = tmp.substr(first+1, last-first-1);
            StringCchCopy(InDir, MAX_PATH, tmp.c_str() );
        }
    }



    if( FALSE == PathIsRelative( InDir ) )
    {
        if( true == Resolve(outData, InDir) )
        {
            return true;
        }
    }
    else
    {
        WCHAR TmpDir[MAX_PATH] = L"";

        //Attempt alternate directory first
        PathCombine(TmpDir, alternateDirectory, InDir);
        if( true == Resolve(outData, TmpDir) )
        {
            return true;
        }

        WCHAR CurDir[MAX_PATH] = L"";
        GetCurrentDirectory(MAX_PATH, CurDir);
        PathCombine(TmpDir, CurDir, InDir);
        if( true == Resolve(outData, TmpDir) )
        {
            return true;
        }

        //Attempt search directory list
        for( std::list<std::wstring>::iterator itr = m_SearchPaths.begin();
            itr != m_SearchPaths.end();
            itr++)
        {
            PathCombine(TmpDir, itr->c_str(), InDir);

            if( true == Resolve(outData, TmpDir) )
            {
                return true;
            }            
        }
    }

    return false;
}

std::list<std::wstring>::iterator FileFinder::FindSearchPath( const WCHAR* path )
{
    std::list<std::wstring>::iterator itr;
    for(itr = m_SearchPaths.begin();
        itr != m_SearchPaths.end();
        itr++)
    {
        if( 0 == lstrcmpi(itr->c_str(), path) )
        {
            return itr;
        }
    }

    return itr;
}

void FileFinder::AddSearchPath( const WCHAR* path )
{
    std::list<std::wstring>::iterator itr = FindSearchPath(path);
    if(itr == m_SearchPaths.end())
    {
        std::wstring str = path;
        m_SearchPaths.push_back( str );
    }
}

void FileFinder::RemoveSearchPath( const WCHAR* path )
{
    std::list<std::wstring>::iterator itr = FindSearchPath(path);
    if(itr == m_SearchPaths.end())
    {
        m_SearchPaths.erase(itr);
    }

}

void FileFinder::ClearSearchPaths()
{
    m_SearchPaths.clear();
}

FileWatcher::FileWatcher()
{
    m_timeLastCheck = 0;
}

bool FileWatcher::FindWatcher( const WCHAR* strFilename , std::list<WatchedFile>::iterator& itr)
{
    for(itr = m_WatchedFiles.begin();
        itr != m_WatchedFiles.end();
        itr++)
    {
        if( 0 == lstrcmpi(itr->strPath.c_str(), strFilename) )
        {
            return true;
        }
    }

    return false;
}

HRESULT FileWatcher::Add( const WCHAR* strFilename )
{
    if(!strFilename)
        assert(false);

    DXV_FIND_DATA findData;    
    if( !DXVGetFileFinder().FindFile(findData, strFilename) )
        return E_FAIL;

    std::list<WatchedFile>::iterator itr;
    if(FindWatcher(strFilename /*findData.FullPath*/ , itr) )
    {
        itr->lastWriteTime = findData.FileData.ftLastWriteTime;
        return S_OK;
    }

    // Create a new entry for this file
    WatchedFile newWatchedFile(findData);
    m_WatchedFiles.push_back( newWatchedFile );

    return S_OK;
}

HRESULT FileWatcher::Remove( const WCHAR* strFilename )
{
    if(!strFilename)
        assert(false);


    std::list<WatchedFile>::iterator itr;
    if(FindWatcher(strFilename, itr) )
    {
        m_WatchedFiles.erase(itr);
        return S_OK;
    }
    else
    {
        DXV_FIND_DATA findData;    
        if( !DXVGetFileFinder().FindFile(findData, strFilename) )
            return E_FAIL;

        if(FindWatcher(findData.FullPath, itr) )
        {
            m_WatchedFiles.erase(itr);
            return S_OK;
        }
    }

    return E_FAIL;
}

void FileWatcher::Clear()
{
    m_WatchedFiles.clear();
}

WatchedFile::WatchedFile()
{
    strPath = L"";
    ZeroMemory( &lastWriteTime, sizeof(FILETIME) );
}

WatchedFile::WatchedFile( DXV_FIND_DATA& findData )
{
    strPath = findData.FullPath;
    lastWriteTime = findData.FileData.ftLastWriteTime;
}
    

    
FileFinder::FileFinder()
{
}

bool FileWatcher::CheckForChanges()
{
    bool result = false;
    m_timeLastCheck = timeGetTime();

    for(std::list<WatchedFile>::iterator itr = m_WatchedFiles.begin();
        itr != m_WatchedFiles.end();
        itr++)
    {
        DXV_FIND_DATA findData;
        if( !DXVGetFileFinder().FindFile( findData,  itr->strPath.c_str() ) )
        {
            return true;
        }
        else if( 1 == CompareFileTime( &findData.FileData.ftLastWriteTime, &itr->lastWriteTime ) )
        {
            return true;
        }
    }

    return false;
}

bool FileFinder::FindFile(const WCHAR* find, const WCHAR* alternateDirectory, Sas::SourceInfo& found )
{
    DXV_FIND_DATA fileData;
    bool result = FindFile(fileData, find, alternateDirectory);
    if(result)
        found.Set( fileData.FullPath );

    return result;
}


//======= Copyright © 1996-2006, Valve Corporation, All rights reserved. ======
//
// Purpose:
//
//=============================================================================

// General Microsoft pointlessness
#if _MSC_VER >= 1300
#pragma warning(disable : 4511)	// warning C4511: copy constructor could not be generated
#pragma warning(disable : 4512)	// warning C4512: assignment operator could not be generated
#pragma warning(disable : 4702)	// warning C4702: unreachable code
#endif //_MSV_VER >= 1300

// Standard includes
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <io.h>
#include <stdlib.h>

#include <algorithm>
#include <fstream>
#include <iostream>
#include <iterator>
#include <string>
#include <vector>


// Boost
#include <boost/regex.hpp>
#include <boost/algorithm/string.hpp>


// Local includes
#include "VProjectUtil.h"

// Valve includes
#include "valveMaya.h"


//-----------------------------------------------------------------------------
// Statics
//-----------------------------------------------------------------------------
const char *const CVProjectUtil::m_src( "content" );
const char *const CVProjectUtil::m_dst( "game" );
const char *const CVProjectUtil::m_materialSrc( "materialsrc" );
const char *const CVProjectUtil::m_materialDst( "materials" );
const char *const CVProjectUtil::m_modelSrc( "models" );
const char *const CVProjectUtil::m_modelDst( m_modelSrc );


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
CVProjectUtil::CVProjectUtil()
: m_flags( 0 )
{
	// SetPath calls Clear
	char buf[ BUFSIZ ];
	if ( GetEnvironmentVariable( "VPROJECT", buf, sizeof( buf ) ) )
	{
		SetPath( buf );
	}
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
CVProjectUtil::CVProjectUtil(
	unsigned int flags )
: m_flags( flags )
{
	// SetPath calls Clear
	char buf[ BUFSIZ ];
	if ( GetEnvironmentVariable( "VPROJECT", buf, sizeof( buf ) ) )
	{
		SetPath( buf );
	}
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
CVProjectUtil::CVProjectUtil(
	const std::string &path,
	unsigned int flags )
: m_flags( flags )
{
	// SetPath calls Clear
	SetPath( path );
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
CVProjectUtil::CVProjectUtil(
	const CVProjectUtil &rhs )
{
	*this = rhs;
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
CVProjectUtil &CVProjectUtil::operator=(
	const CVProjectUtil &rhs )
{
	m_flags = rhs.m_flags;
	m_rawPath = rhs.m_rawPath;
	m_fullPath = rhs.m_fullPath;
	m_prefix = rhs.m_prefix;
	m_game = rhs.m_game;
	m_path = rhs.m_path;
	m_file = rhs.m_file;
	m_subType = rhs.m_subType;

	return *this;
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void CVProjectUtil::Clear()
{
	m_rawPath.clear();
	m_fullPath.clear();
	m_prefix.clear();
	m_game.clear();
	m_path.clear();
	m_file.clear();
	m_subType.clear();
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
bool CVProjectUtil::SetPath(
	std::string path )
{
	Clear();

	m_rawPath = path;

	path = FullPath( path );

	m_fullPath = path;

	try {
		// Change the path separator to ensure that they are all /'s
		boost::replace_all( path, "\\", "/" );

		boost::smatch match;
		if ( boost::regex_match( path, match,
			boost::regex(
//				 1       2         3       4                                               5          6              7
				"(.*)/(?:(content)|(game))/([^/]+)(?:(?:/(?:(?(2)materialsrc|materials)(?:/(models))?|(models)))?(?:/(.*))?)?/*",
				boost::regex::icase ) ) )
		{
			// Matches one of the known path configurations

			m_prefix.assign( match[ 1 ].first, match[ 1 ].second );
			m_game.assign( match[ 4 ].first, match[ 4 ].second );

			if ( match.length( 5 ) )
			{
				m_subType.assign( match[ 5 ].first, match[ 5 ].second );
			}
			else if ( match.length( 6 ) )
			{
				m_subType.assign( match[ 6 ].first, match[ 6 ].second );
			}

			const std::string tmpPath( match[ 7 ].first, match[ 7 ].second );
			if ( boost::regex_match( tmpPath, match, boost::regex( "/?(.*)/([^/\\.]+)\\.[^/\\.]*" ) ) )
			{
				m_path.assign( match[ 1 ].first, match[ 1 ].second );
				m_file.assign( match[ 2 ].first, match[ 2 ].second );
			}
			else if ( boost::regex_match( tmpPath, match, boost::regex( "([^/\\.]+)\\.[^/\\.]*" ) ) )
			{
				m_file.assign( match[ 1 ].first, match[ 1 ].second );
			}
			else
			{
				// TODO: Perhaps check to see if it really is a directory? (But might be chicken/egg thing)
				//       or maybe the right thing to do isn't to add an implict file named the same as the directory
				//       but in general, having raw files in game or content is not generally advisable
				m_path = tmpPath;
				m_file = tmpPath;
			}
		}
		else
		{
			// Didn't match a known path configuration... default to the lame approach which is just the
			// basename and the filename

			if ( boost::regex_match( path, match, boost::regex( "(.*)/([^/\\.]+)\\.[^/\\.]*" ) ) )
			{
				m_prefix.assign( match[ 1 ].first, match[ 1 ].second );
				m_file.assign( match[ 2 ].first, match[ 2 ].second );
			}
			else
			{
				// This should never happen due to the _fullpath call
				m_file = boost::regex_replace( path, boost::regex( ".[^\\.]*$" ), "" );
			}
		}
	}
	catch ( const std::exception &e )
	{
		merr << "Error: " << e.what() << std::endl;
		return false;
	}

	return true;
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
std::string CVProjectUtil::ConstructPath(
	const std::string &srcDst,
	const std::string &type,
	const std::string &file,
	const std::string &ext,
	bool enforceExtension ) const
{
	try
	{
		// See if an extension was supplied in file, use it if supplied

		std::string newFile( File( file ) );
		std::string newExt( ext );
		boost::replace_all( newFile, "\\", "/" );

		boost::smatch match;
		if ( boost::regex_match( newFile, match, boost::regex( "([^\\.]+)\\.([^/\\.]+)" ) ) )
		{
			newFile.assign( match[ 1 ].first, match[ 1 ].second );

			if ( !enforceExtension || newExt.empty() )
			{
				newExt.assign( match[ 2 ].first, match[ 2 ].second );
			}
		}

		if ( newExt.empty() )
		{
			newExt = "unk";
		}

		if ( m_game.size() )
		{
			return FullPath( std::string( m_prefix +
				"/" + srcDst +
				( m_game.size() ? std::string( "/" ) + m_game : "" ) +
				"/" + type + ( ( type == m_subType ) ? ( "" ) : ( "/" + m_subType ) ) +
				( m_path.size() ? std::string( "/" ) + m_path : "" ) +
				"/" + newFile + "." + newExt ) );
		}

		return FullPath( std::string( m_prefix +
			( m_game.size() ? std::string( "/" ) + m_game : "" ) +
			( m_path.size() ? std::string( "/" ) + m_path : "" ) +
			"/" + newFile + "." + newExt ) );
	}
	catch ( ... )
	{
	}

	return std::string();
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
std::string CVProjectUtil::ModelDst(
	const std::string &file,
	const std::string &ext ) const
{

	std::string absoluteModel( ConstructPath( m_dst, m_modelDst, file, ext, true ) );

	return absoluteModel;
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
std::string CVProjectUtil::CopyTextureToMaterialSrc(
	const std::string &srcFile,
	const std::string &dstFile )
{
	try
	{
		std::string src( FullPath( srcFile ) );
		boost::replace_all( src, "\\", "/" );

		// See if the source file is accessible
		if ( _access( src.c_str(), 04 ) < 0 )
		{
			merr << "Cannot read source texture file: " << src << std::endl;
			return "";
		}

		boost::smatch match;
		if ( !boost::regex_match( src, match, boost::regex( ".*/([^/\\.]+)\\.([^/\\.]*)?" ) ) )
		{
			merr << "Cannot determine source texture filename from: " << src << std::endl;
			return "";
		}

		std::string file( match[ 1 ].first, match[ 1 ].second );
		std::string ext( match[ 2 ].first, match[ 2 ].second );

		const std::string dst( MaterialSrc( dstFile, ext ) );

		if ( _access( dst.c_str(), 00 ) == 0 )
		{
			merr << "Cannot overwrite destination file: " << dst << std::endl;
			return "";
		}

		if ( file.empty() )
		{
			merr << "Cannot determine source texture filename from: " << src << std::endl;
			return "";
		}

		if ( ext.empty() )
		{
			ext = "tga";
		}

		minfo << "Copying " << src << " -> " << dst << std::endl;

		if ( !CopyFile( src.c_str(), dst.c_str(), true ) )
		{
			merr << "Cannot copy Source texture from " << src << " to " << dst << std::endl;
			return "";
		}
	}
	catch ( ... )
	{
	}

	return dstFile;
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
std::string CVProjectUtil::ConvertMaterialSrcToVtf(
	const std::string &file )
{
	const std::string game( Prefix() + "/game/" + Game() );
	const std::string src( MaterialSrc( file ) );

	const std::string cmd( std::string( "vtex -mkdir -nopause -game \"" ) + game + std::string( "\" \"" ) + src + "\"" );

	FILE *vtex( _popen( cmd.c_str(), "rt" ) );
	if ( !vtex )
	{
		merr << "Couldn't execute vtex command: " << cmd << std::endl;
		return false;
	}

	char buf[ BUFSIZ ];

	while ( !feof( vtex ) )
	{
		if ( fgets( buf, BUFSIZ, vtex ) == NULL )
		{
			break;
		}

		minfo << "vtex: " << buf;
	}
	minfo << std::endl;

	_pclose( vtex );

	return MaterialDst( src );
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
bool CVProjectUtil::RunStudiomdl(
	const std::string &file )
{
	const std::string game( Prefix() + "/game/" + Game() );
	const std::string src( ModelSrc( file ) );

	const std::string cmd( std::string( "studiomdl -game \"" ) + game + std::string( "\" \"" ) + src + "\"" );

	FILE *cmdp( _popen( cmd.c_str(), "rt" ) );
	if ( !cmdp )
	{
		merr << "Couldn't execute studiomdl command: " << cmd << std::endl;

		return false;
	}

	char buf[ BUFSIZ ];

	while ( !feof( cmdp ) )
	{
		if ( fgets( buf, BUFSIZ, cmdp ) == NULL )
		{
			break;
		}

		minfo << "studiomdl: " << buf;
	}
	minfo << std::endl;

	_pclose( cmdp );

	return true;
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
std::string CVProjectUtil::WriteVmt(
	const std::string &file,
	const std::string &shader,
	bool translucent )
{
	// Get the pathname for the VMT file
	const std::string vmt( Vmt( file ) );
	const std::string dir( DirName( vmt ) );

	if ( _access( vmt.c_str(), 00 ) == 0 )
	{
		merr << "Cannot overwrite destination vmt: " << vmt << std::endl;
		return "";
	}

	CreateDirectoryEx( Prefix().c_str(), dir.c_str(), NULL );

	std::ofstream os( vmt.c_str() );
	if ( !os )
	{
		merr << "Couldn't write to vmt: " << vmt << std::endl;
		return "";
	}

	// Shader

	os << "\"";
	if ( shader.size() )
	{
		os << shader;
	}
	else {
		os << "VertexLitGeneric";
	}
	os << "\"\n";

	// Parameters
	os << "{\n";

	// Base Texture
	os << "\t\"$basetexture\" \"";

	const std::string path( Path() );
	if ( path.size() )
	{
		os << path + "/";
	}

	if ( file.size() )
	{
		os << file;
	}
	else if ( m_file.size() )
	{
		os << m_file;
	}
	else
	{
		os << "unknown";
	}

	os << "\"\n";

	// Translucent

	if ( translucent )
	{
		os << "\t\"$translucent\" \"1\"\n";
	}

	os << "}\n";

	os.close();

	return vmt;
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
std::string CVProjectUtil::WriteQc(
	const std::string &file,
	unsigned int qcFlags,
	const std::list< std::string > &qcStuff )
{
	// Get the pathname for the Qc file
	const std::string qc( Qc( file ) );
	const std::string dir( DirName( qc ) );

	if ( _access( qc.c_str(), 00 ) == 0 )
	{
		merr << "Cannot overwrite destination qc: " << qc << std::endl;
		return "";
	}

	CreateDirectoryEx( Prefix().c_str(), dir.c_str(), NULL );

	std::ofstream os( qc.c_str() );
	if ( !os )
	{
		merr << "Couldn't write to qc: " << qc << std::endl;
		return "";
	}

	if ( qcFlags & kQcYUp )
	{
		os << "$upaxis Y\n\n";
	}
	else if ( qcFlags & kQcZUp )
	{
		os << "$upaxis Z\n\n";
	}
	else if ( qcFlags & kQcXUp )
	{
		os << "$upaxis X\n\n";
	}

	if ( m_path.length() )
	{
		os << "$cdmaterials " << Path() << "\n\n";
	}

	if ( qcFlags & kQcCollapseBones )
	{
		os << "$collapsebones\n\n";
	}

	if ( qcFlags & kQcCenterBonesOnVerts )
	{
		os << "$centerbonesonverts\n\n";
	}

	if ( qcFlags & kQcSkipBoneInBBox )
	{
		os << "$skipboneinbbox\n\n";
	}

	std::copy( qcStuff.begin(), qcStuff.end(), std::ostream_iterator< std::string >( os, "\n" ) );

	// TODO: Ask for relative names...
	os << "\n";
	os << "$modelname " << ( m_path.empty() ? "" : m_path + "/" ) << File( file ) << ".mdl\n";
	os << "\n";
	os << "$model " << File( file ) << " \"" << File( file ) << ".smd\" {\n";
	os << "}\n";
	os << "\n";
	os << "$sequence idle \"" << File( file ) << ".smd\"" << std::endl;
	os << "\n";

	os.close();

	return qc;
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
std::string CVProjectUtil::FullPath(
	const std::string &path )
{
	char fullpath[ _MAX_PATH ];
	return std::string( _fullpath( fullpath, path.c_str(), sizeof( fullpath ) ) );
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
std::string CVProjectUtil::BaseName(
	const std::string &path )
{
	try
	{
		boost::smatch match;
		if ( boost::regex_search( path, match, boost::regex( "([^\\\\/]+)$" ) ) )
		{
			return match.str( 1 );
		}
	}
	catch ( ... )
	{
	}

	assert( 0 );
	return path;
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
std::string CVProjectUtil::DirName(
	const std::string &path )
{
	try
	{
		boost::smatch match;
		if ( boost::regex_match( path, match, boost::regex( "(.*)[\\\\/]([^\\\\/]+)" ) ) )
		{
			return match.str( 1 );
		}
	}
	catch ( ... )
	{
	}

	return ".";
}
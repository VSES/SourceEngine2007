//======= Copyright © 1996-2006, Valve Corporation, All rights reserved. ======
//
// Purpose:
//
//=============================================================================

#ifndef VPROJECTUTIL_H
#define VPROJECTUTIL_H
#if defined( _WIN32 )
#pragma once
#endif

// Standard includes
#include <list>
#include <string>

class CVProjectUtil
{
public:
	enum Flags
	{
		kFlagsDefault = 0
	};

	CVProjectUtil();

	CVProjectUtil(
		unsigned int flags );

	CVProjectUtil(
		const std::string &path,
		unsigned int flags = kFlagsDefault );

	CVProjectUtil(
		const CVProjectUtil &rhs );

	CVProjectUtil &operator=(
		const CVProjectUtil &rhs );

	void Clear();

	bool SetPath(
		std::string path );

	const std::string &Prefix() const
	{
		return m_prefix;
	}

	const std::string &Game() const
	{
		return m_game;
	}

	std::string Path( const std::string &type = "" ) const
	{
		return ( ( type == m_subType || m_subType.empty() ) ? "" : m_subType + ( m_path.empty() ? "" : "/" ) ) + m_path;
	}

	std::string File(
		const std::string &file = "" ) const
	{
		return ( file.empty() ? ( m_file.empty() ? "unknown" : m_file ) : file );
	}

	std::string MaterialSrc(
		const std::string &file = "",
		const std::string &ext = "tga" ) const
	{
		return ConstructPath( m_src, m_materialSrc, file, ext, false );
	}

	std::string Tga(
		const std::string &file = "" ) const
	{
		return MaterialSrc( file );
	}

	std::string MaterialDst(
		const std::string &file = "",
		const std::string &ext = "vmt" ) const
	{
		return ConstructPath( m_dst, m_materialDst, file, ext, true );
	}

	std::string Vmt(
		const std::string &file = "" ) const
	{
		return MaterialDst( file );
	}

	std::string Vtf(
		const std::string &file = "" ) const
	{
		return MaterialDst( file, "vtf" );
	}

	std::string ModelSrc(
		const std::string &file = "",
		const std::string &ext = "smd" ) const
	{
		return ConstructPath( m_src, m_modelSrc, file, ext, false );
	}

	std::string Smd(
		const std::string &file = "" ) const
	{
		return ModelSrc( file );
	}

	std::string Dmx(
		const std::string &file = "" ) const
	{
		return ModelSrc( file, "dmx" );
	}

	std::string Obj(
		const std::string &file = "" ) const
	{
		return ModelSrc( file, "obj" );
	}

	std::string ModelDst(
		const std::string &file = "",
		const std::string &ext = "mdl" ) const;

	std::string Mdl(
		const std::string &file = "" ) const
	{
		return ModelDst( file, "mdl" );
	}

	std::string Qc(
		const std::string &file = "" ) const
	{
		return ConstructPath( m_src, m_modelSrc, file, "qc", true );
	}

	std::string CopyTextureToMaterialSrc(
		const std::string &srcFile,
		const std::string &dstFile = "" );

	std::string ConvertMaterialSrcToVtf(
		const std::string &file = "" );

	std::string WriteVmt(
		const std::string &file,
		const std::string &shader = "VertexLitGeneric",
		bool transparent = false );

	enum QcFlags
	{
		kQcDefault =			0,
		kQcXUp =				1 << 0,
		kQcYUp =				1 << 1,
		kQcZUp =				1 << 2,
		kQcCenterBonesOnVerts =	1 << 3,
		kQcSkipBoneInBBox =		1 << 4,
		kQcCollapseBones =		1 << 5
	};

	std::string WriteQc(
		const std::string &file,
		unsigned int qcFlags,
		const std::list< std::string > &qcStuff );

	std::string WriteQc(
		unsigned int qcFlags = kQcDefault,
		const std::list< std::string > &qcStuff = std::list< std::string >() )
	{
		return WriteQc( "", qcFlags, qcStuff );
	}

	bool RunStudiomdl(
		const std::string &file = "" );

protected:
	static const char *const m_src;
	static const char *const m_dst;
	static const char *const m_materialSrc;
	static const char *const m_materialDst;
	static const char *const m_modelSrc;
	static const char *const m_modelDst;

	unsigned int m_flags;

	std::string m_rawPath;
	std::string m_fullPath;
	std::string m_prefix;
	std::string m_game;
	std::string m_path;
	std::string m_file;
	std::string m_subType;

	enum PathFlags
	{
		kSetFromVProject =	1 << 0,
		kSetFromUser =		1 << 1
	};

	std::string ConstructPath(
		const std::string &srcDst,
		const std::string &type,
		const std::string &file,
		const std::string &ext,
		bool enforceExtension ) const;

	static std::string FullPath(
		const std::string &path );

	static std::string BaseName(
		const std::string &path );

	static std::string DirName(
		const std::string &path );
};


#endif // VPROJECTUTIL_H
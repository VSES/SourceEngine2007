//======= Copyright © 1996-2006, Valve Corporation, All rights reserved. ======
//
// Purpose:
//
//=============================================================================


#ifndef VSTSMDIOTRANSLATOR_H
#define VSTSMDIOTRANSLATOR_H
#if defined( _WIN32 )
#pragma once
#endif


// Maya includes
#include <maya/MPxFileTranslator.h>

// Local includes
#include "SmdImport.h"
#include "SmdExport.h"


class CVstSmdIOTranslator : public MPxFileTranslator
{
public:
	virtual bool haveReferenceMethod() const { return true; }

	virtual bool haveReadMethod() const { return true; }

	virtual bool haveWriteMethod() const { return true; }

	virtual bool haveNamespaceSupport() const { return true; }

	virtual MString defaultExtension() const { return MString("smd"); }

	virtual MString filter() const { return MString("*.smd"); }

	virtual bool canBeOpened() const { return true; }

	virtual MFileKind identifyFile(
		const MFileObject &i_mFileObject,
		const char *i_buffer,
		short i_size) const;

protected:

	MStatus Reader(
		uint type,
		const MFileObject &i_mFileObj,
		const MString &i_optionsString,
		FileAccessMode i_fileAccessMode);

	MStatus Writer(
		uint type,
		const MFileObject &i_mFileObj,
		const MString &i_optionsString,
		FileAccessMode i_fileAccessMode);
};


//-----------------------------------------------------------------------------
// For Models
//-----------------------------------------------------------------------------
class CVstSmdModel : public CVstSmdIOTranslator
{
public:
	virtual MStatus reader(
		const MFileObject &mFileObj,
		const MString &optionsString,
		FileAccessMode fileAccessMode)
	{
		return Reader( CSmdExport::kModel, mFileObj, optionsString, fileAccessMode );
	}

	virtual MStatus writer(
		const MFileObject &mFileObj,
		const MString &optionsString,
		FileAccessMode fileAccessMode)
	{
		return Writer( CSmdExport::kModel, mFileObj, optionsString, fileAccessMode );
	}

	static void *Factory();
};


//-----------------------------------------------------------------------------
// For Physics Models
//-----------------------------------------------------------------------------
class CVstSmdPhysics : public CVstSmdIOTranslator
{
public:
	virtual MStatus reader(
		const MFileObject &mFileObj,
		const MString &optionsString,
		FileAccessMode fileAccessMode)
	{
		return Reader( CSmdExport::kPhysModel, mFileObj, optionsString, fileAccessMode );
	}

	virtual MStatus writer(
		const MFileObject &mFileObj,
		const MString &optionsString,
		FileAccessMode fileAccessMode)
	{
		return Writer( CSmdExport::kPhysModel, mFileObj, optionsString, fileAccessMode );
	}

	static void *Factory();
};


//-----------------------------------------------------------------------------
// For Animation
//-----------------------------------------------------------------------------
class CVstSmdAnimation : public CVstSmdIOTranslator
{
public:
	virtual MStatus reader(
		const MFileObject &mFileObj,
		const MString &optionsString,
		FileAccessMode fileAccessMode)
	{
		return Reader( CSmdExport::kSkeletalAnimation, mFileObj, optionsString, fileAccessMode );
	}

	virtual MStatus writer(
		const MFileObject &mFileObj,
		const MString &optionsString,
		FileAccessMode fileAccessMode)
	{
		return Writer( CSmdExport::kSkeletalAnimation, mFileObj, optionsString, fileAccessMode );
	}

	static void *Factory();
};


//-----------------------------------------------------------------------------
// For Vertex Animation
//-----------------------------------------------------------------------------
class CVstSmdVertexAnimation : public CVstSmdIOTranslator
{
public:
	virtual MStatus reader(
		const MFileObject &mFileObj,
		const MString &optionsString,
		FileAccessMode fileAccessMode)
	{
		return Reader( CSmdExport::kVertexAnimation, mFileObj, optionsString, fileAccessMode );
	}

	virtual MStatus writer(
		const MFileObject &mFileObj,
		const MString &optionsString,
		FileAccessMode fileAccessMode)
	{
		return Writer( CSmdExport::kVertexAnimation, mFileObj, optionsString, fileAccessMode );
	}

	virtual MString defaultExtension() const { return MString( "vta" ); }

	virtual MString filter() const { return MString( "*.vta" ); }

	static void *Factory();
};


//-----------------------------------------------------------------------------
// For Model & Animation Merged
//-----------------------------------------------------------------------------
class CVstSmdModelAnimation : public CVstSmdIOTranslator
{
public:
	virtual MStatus reader(
		const MFileObject &mFileObj,
		const MString &optionsString,
		FileAccessMode fileAccessMode)
	{
		return Reader(
			CSmdExport::kModel | CSmdExport::kSkeletalAnimation,
			mFileObj, optionsString, fileAccessMode );
	}

	virtual MStatus writer(
		const MFileObject &mFileObj,
		const MString &optionsString,
		FileAccessMode fileAccessMode)
	{
		return Writer(
			CSmdExport::kModel | CSmdExport::kSkeletalAnimation,
			mFileObj, optionsString, fileAccessMode );
	}

	static void *Factory();
};


#endif // VSTSMDIOTRANSLATOR_H

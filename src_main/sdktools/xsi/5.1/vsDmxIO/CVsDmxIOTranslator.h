//======= Copyright © 1996-2006, Valve Corporation, All rights reserved. ======
//
// Purpose: vsDmxIO Translator Declaration
//
//=============================================================================

#ifndef CVSDMXIOTRANSLATOR_H
#define CVSDMXIOTRANSLATOR_H
#if defined( _WIN32 )
#pragma once
#endif

#include <maya/MStatus.h>
#include <maya/MFileObject.h>
#include <maya/MPxFileTranslator.h>
#include <maya/MString.h>

class CVsDmxIOTranslator : public MPxFileTranslator
{
public:
	static void *Factory();

	virtual MStatus reader(
		const MFileObject &i_mFileObj,
		const MString &i_optionsString,
		FileAccessMode i_fileAccessMode);

	virtual MStatus writer(
		const MFileObject &i_mFileObj,
		const MString &i_optionsString,
		FileAccessMode i_fileAccessMode);

	virtual bool haveReferenceMethod() const { return true; }

	virtual bool haveReadMethod() const { return true; }

	virtual bool haveWriteMethod() const { return true; }

	virtual bool haveNamespaceSupport() const { return true; }

	virtual MString defaultExtension() const { return MString("dmx"); }

	virtual MString filter() const { return MString("*.dmx"); }

	virtual bool canBeOpened() const { return true; }

	virtual MFileKind identifyFile(
		const MFileObject &i_mFileObject,
		const char *i_buffer,
		short i_size) const;

public:
	static const char *const s_name;	// The name of the translator
};

#endif // CVSDMXIOTRANSLATOR_H

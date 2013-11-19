#pragma once
#ifndef FILETRANSLATOR_H
#define FILETRANSLATOR_H



#define DXCC_EXPORTER				"X-File"


class XFileTranslator: public MPxFileTranslator 
{
public:
	static void*		creator();
	virtual				~XFileTranslator ();	
	virtual MStatus		reader(const MFileObject& file, const MString& optionsString, MPxFileTranslator::FileAccessMode mode);
	virtual MStatus		writer(const MFileObject& file, const MString& optionsString, MPxFileTranslator::FileAccessMode mode);
	virtual bool		haveReadMethod() const;
	virtual bool		haveWriteMethod() const;
	virtual MString 	defaultExtension() const;
	virtual MString 	filter() const;
	virtual MFileKind	identifyFile( const MFileObject& fileName, const char* buffer, short size) const;
};

#endif
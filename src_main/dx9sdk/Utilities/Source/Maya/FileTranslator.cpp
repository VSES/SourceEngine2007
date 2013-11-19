#include "pch.h"

//-----------------------------------------------------------------------------
// XFILETRANSLATOR MEMBER FUNCTIONS
//-----------------------------------------------------------------------------
 void*		XFileTranslator::creator()					
{ 
	return new XFileTranslator(); 
};

MStatus		XFileTranslator::reader(const MFileObject& file, const MString& optionsString, MPxFileTranslator::FileAccessMode mode) 
{ 
	return MS::kSuccess; 
};

bool		XFileTranslator::haveReadMethod() const		
{ 
	return false; 
};

bool		XFileTranslator::haveWriteMethod() const		
{ 
	return true; 
};

MString 	XFileTranslator::defaultExtension() const	
{ 
	return "x"; 
}

MString 	XFileTranslator::filter() const				
{ 
	return "*.x"; 
}

MPxFileTranslator::MFileKind 
XFileTranslator::identifyFile(	
	const MFileObject& fileName,
	const char* buffer,
	short size) const
{ 
    LPSTR extension = PathFindExtensionA(fileName.name().asChar());
    if(0==lstrcmpA(extension, ".x"))
	{
		return kIsMyFileType;
	}
	else
	{
		return kNotMyFileType; 
	}
}

MStatus	
XFileTranslator::writer(
			const MFileObject& mfoXFile,			// save file object
			const MString& msOptions,				// options string
			MPxFileTranslator::FileAccessMode mode)	// options string
{
	HRESULT hr = S_OK;
	MStatus stat = MS::kSuccess;

	MString	msFile= mfoXFile.fullName();

	hr= g_PreviewPipeline.Scene_Export(msFile.asChar(), msOptions.asChar(), mode);
	if(FAILED(hr))
	{
		stat= MS::kFailure;
		goto e_Exit;
	};


e_Exit:
	return stat;
}

XFileTranslator::~XFileTranslator()
{
	return;
};	

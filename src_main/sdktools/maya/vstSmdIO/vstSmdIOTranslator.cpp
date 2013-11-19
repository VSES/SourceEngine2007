//======= Copyright © 1996-2006, Valve Corporation, All rights reserved. ======
//
// Purpose:
//
//=============================================================================

// Maya includees

#include <maya/MAnimControl.h>
#include <maya/MFileObject.h>
#include <maya/MGlobal.h>
#include <maya/MString.h>

// Local includes
#include "vstSmdIOTranslator.h"

//-----------------------------------------------------------------------------
//
// Purpose: This routine is called by Maya when it is necessary to save a file
//			of a type supported by this translator.
//			This virtual method must be overloaded in derived classes, as the
//			default version defined in MPxFileTranslator simply fails
//			unconditionally. The only parameter is an MFileObject that contains
//			the pathname of the file to be written to. This routine is
//			responsible for traversing all objects in the current Maya scene,
//			and writing a representation to the given file in the supported
//			format.
//			It is not essential that all information in the current Maya scene
//			be written out, although if it is not, the scene will not be
//			recreated when this file is read via the "reader" method.
// Input  : i_mFileObject		File the name of the file to write
//			i_optionsString		A string representation of any file options
//			i_fileAccessMode	Mode the method used to write the file - save,
//			export, or export active. 
// Output : MS::kSuccess if the write succeeds, failure code if it does not
//
//-----------------------------------------------------------------------------

MStatus CVstSmdIOTranslator::Writer(
	uint type,
	const MFileObject &i_mFileObject,
	const MString &i_optionsString,
	const FileAccessMode i_fileAccessMode )
{
	MString cmd( "vstSmdIO -export" );

	if ( type & CSmdExport::kPhysModel )
	{
		cmd += " -exportType physics";
	}

	if ( type & CSmdExport::kVertexAnimation )
	{
		cmd += " -exportType vertexAnimation";
	}

	if ( ( type & CSmdExport::kAnimation ) != 0 )
	{
		cmd += " -exportType skeletalAnimation";
	}

	if ( ( type & CSmdExport::kModel ) != 0 )
	{
		cmd += " -exportType model";
	}

	if ( i_optionsString.length() )
	{
		bool useTimeline( true );
		double fs( MAnimControl::minTime().as( MTime::uiUnit() ) );
		double fe( MAnimControl::maxTime().as( MTime::uiUnit() ) );
		double fi( 1.0 );
		bool addNodePrefix( false );
		MString nodePrefix;

		MStringArray options;
		if ( !i_optionsString.split( ';', options ) )
		{
			MGlobal::displayError( MString( "Can't split translator arguments" ) );
			return MS::kFailure;
		}

		bool automation( false );

		for ( unsigned int oi( 0 ); oi != options.length(); ++oi )
		{
			MStringArray keyValue;
			if ( options[ oi ].split( '=', keyValue ) && keyValue.length() == 2 )
			{
				const MString &key( keyValue[ 0 ] );
				const MString &val( keyValue[ 1 ] );

				if ( key == "upAxis" )
				{
					switch ( val.asInt() )
					{
					case 1:
						cmd += MString( " -upAxis X" );
						break;
					case 2:
						cmd += MString( " -upAxis Y" );
						break;
					case 3:
						cmd += MString( " -upAxis Z" );
						break;
					default:
						MGlobal::displayWarning( MString( "Unknown upAxis \"" ) + val + ( "\"" ) );
						break;
					}
					continue;
				}

				if ( key == "relativeMaterials" && val.asInt() )
				{
					cmd += " -relativeMaterials";
					continue;
				}

				if ( key == "exportInvisible" && val.asInt() )
				{
					cmd += " -exportInvisible";
					continue;
				}

				if ( key == "doubleSided" && val.asInt() )
				{
					cmd += " -doubleSided";
					continue;
				}

				if ( key == "world" && val.asInt() )
				{
					cmd += " -world";
					continue;
				}

				if ( key == "bindCurrent" && val.asInt() )
				{
					cmd += " -bindCurrent";
					continue;
				}

				if ( key == "addNodePrefix" )
				{
					addNodePrefix = ( val.asInt() != 0 );
					continue;
				}

				if ( key == "nodePrefix" )
				{
					nodePrefix = val;
					continue;
				}

				if ( key == "useTimeline" )
				{
					useTimeline = val.asInt() ? true : false;
					continue;
				}

				if ( !useTimeline )
				{
					if ( key == "fs" )
					{
						fs = val.asDouble();
						continue;
					}

					if ( key == "fe" )
					{
						fe = val.asDouble();
						continue;
					}

					if ( key == "fi" )
					{
						fi = val.asDouble();
						continue;
					}
				}

				if ( key == "automate" && val.asInt() )
				{
					automation = true;
					cmd += " -materialSrc";
					cmd += " -materialVtf";
					cmd += " -materialVmt";
					cmd += " -qcCreate";
					cmd += " -qcCollapseBones";
					cmd += " -qcCenterBonesOnVerts";
					cmd += " -studiomdl";
				}

				if ( !automation )
				{
					if ( key == "tCopy" && val.asInt() )
					{
						cmd += " -materialSrc";
					}

					if ( key == "tVtex" && val.asInt() )
					{
						cmd += " -materialVtf";
					}

					if ( key == "tVmt" && val.asInt() )
					{
						cmd += " -materialVmt";
					}

					if ( key == "qcCreate" && val.asInt() )
					{
						cmd += " -qcCreate";
					}

					if ( key == "qcCollapse" && val.asInt() )
					{
						cmd += " -qcCollapseBones";
					}

					if ( key == "qcCenter" && val.asInt() )
					{
						cmd += " -qcCenterBonesOnVerts";
					}

					if ( key == "studiomdl" && val.asInt() )
					{
						cmd += " -studiomdl";
					}
				}
			}	
		}

		if ( ( type & ( CSmdExport::kAnimation | CSmdExport::kVertexAnimation ) ) && !useTimeline )
		{
			cmd += " -frameStart ";
			cmd += fs;
			cmd += " -frameEnd ";
			cmd += fe;
			cmd += " -frameIncrement ";
			cmd += fi;
		}

		if ( addNodePrefix )
		{
			cmd += MString( " -nodePrefix \"" ) + nodePrefix + "\"";
		}
	}

	if ( i_fileAccessMode == MPxFileTranslator::kExportActiveAccessMode )
	{
		cmd += " -selection";
	}

	cmd += " -filename \"";
	cmd += i_mFileObject.fullName();
	cmd += "\";";

	return MGlobal::executeCommand( cmd, true, true );
}


//-----------------------------------------------------------------------------
//
// Purpose: This routine is called by Maya when it is necessary to load a file
//			of a type supported by this translator.
//			This virtual method must be overloaded in derived classes, as the
//			default version defined in MPxFileTranslator simply fails
//			unconditionally. The only parameter is an MFileObject that contains
//			the pathname of the file to be loaded. This routine is responsible
//			for reading the contents of the given file, and creating Maya
//			objects via API or MEL calls to reflect the data in the file.
// Input  :	i_mFileObject		Name of the file to read
//			i_optionsString		A string representation of any file options
//			i_fileAccessMode	Mode the method used to read the file
//								open or import 
// Output :	MS::kSuccess if the read succeeds, failure code if it does not 
//
//-----------------------------------------------------------------------------

MStatus CVstSmdIOTranslator::Reader(
	uint type,
	const MFileObject &i_mFileObject,
	const MString &i_optionsString,
	const FileAccessMode /* i_fileAccessMode */ )
{
	MString cmd( "vstSmdIO -import" );

	if ( ( type & CSmdExport::kAnimation ) != 0 )
	{
		cmd += " -importType animation";
	}

	if ( i_optionsString.length() )
	{
		MStringArray options;
		if ( !i_optionsString.split( ';', options ) )
		{
			MGlobal::displayError( "Can't split translator arguments" );
			return MS::kFailure;
		}

		for ( unsigned oi( 0 ); oi < options.length(); ++oi )
		{
			MStringArray keyValue;
			if ( options[ oi ].split( '=', keyValue ) && keyValue.length() == 2 )
			{
				const MString &key( keyValue[ 0 ] );
				const MString &val( keyValue[ 1 ] );

				if ( key == "timeOffset" )
				{
					cmd += " -timeOffset ";
					cmd += val.asDouble();
					continue;
				}

				if ( key == "importSkeleton" )
				{
					cmd += " -importSkeleton ";
					cmd += val.asInt() ? "true" : "false";
					continue;
				}
			}	
		}
	}

	cmd += " -filename \"";
	cmd += i_mFileObject.fullName();
	cmd += "\";";

	return MGlobal::executeCommand( cmd, true, true );
}


//-----------------------------------------------------------------------------
//
// Purpose: This routine is called by Maya when a file selection dialog
//			accesses a new directory. It will potentially be called once for
//			every file in the directory.
//			The routine is given an MFileObject indicating the file being
//			checked, as well as a pointer to the initial file contents.
//			The size parameter indicates the number of bytes available at the
//			pointer location.
//			This routine must use this data to determine if the file is of a
//			type supported by this translator. This virtual method must be
//			overloaded in derived classes, as the default version defined in
//			MPxFileTranslator unconditionally returns kNotMyFileType.
//			A typical use of this method is to check the file extension of the
//			file in question, and return a status indicating whether or not the
//			file is of the type expected by this MPxFileTranslator. See the
//			plugin example animImportExport for an example.
// Input  :	i_mFileObject	Name of the file to check
//			i_buffer		Pointer to the initial contents of the file
//			i_size			Number of valid bytes in the buffer 
// Output : MFileKind::kIsMyFileType		File understood by translator
//			MFileKind::kNotMyFileType		File not understood by translator
//			MFileKind::kCouldBeMyFileType	File understood by translator, but
//											this translator is not the best one
//											to use for reading or writing it 
//
//-----------------------------------------------------------------------------

MPxFileTranslator::MFileKind CVstSmdIOTranslator::identifyFile(
	const MFileObject &i_mFileObject,
	const char * /* i_buffer */,
	const short /* i_size */ ) const
{
	MFileKind retVal( kNotMyFileType );

	const MString &name( i_mFileObject.name() );

	if ( name.length() )
	{
		if ( name.substring( name.rindex( '.' ) + 1, name.length() - 1 ) == MString( "smd" ) )
		{
			retVal = kIsMyFileType;
		}
		else if ( name.substring( name.rindex( '.' ) + 1, name.length() - 1 ) == MString( "vta" ) )
		{
			retVal = kIsMyFileType;
		}
	}

	return retVal;
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void *CVstSmdModel::Factory()
{
	return new CVstSmdModel;
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void *CVstSmdPhysics::Factory()
{
	return new CVstSmdPhysics;
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void *CVstSmdAnimation::Factory()
{
	return new CVstSmdAnimation;
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void *CVstSmdVertexAnimation::Factory()
{
	return new CVstSmdVertexAnimation;
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void *CVstSmdModelAnimation::Factory()
{
	return new CVstSmdModelAnimation;
}
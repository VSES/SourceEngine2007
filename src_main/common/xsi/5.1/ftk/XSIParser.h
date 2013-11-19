// ***************************************************************************************
//
// File supervisor: Softimage Games/Interactive team
//
// (c) Copyright 2001 Avid Technology, Inc. . All rights reserved.
//
// @doc
//
// @module      CXSIParser.h | header file for CXSIParser implementation
//
// ***************************************************************************************
//
// THIS CODE IS PUBLISHED AS A SAMPLE ONLY AND IS PROVIDED "AS IS".
// IN NO EVENT SHALL SOFTIMAGE, AVID TECHNOLOGY, INC. AND/OR THEIR RESPECTIVE 
// SUPPLIERS BE LIABLE FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR ANY
// DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
// ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN
// CONNECTION WITH THE USE OR PERFORMANCE OF THIS CODE . 
//  
// COPYRIGHT NOTICE. Copyright © 1999-2001 Avid Technology Inc. . All rights reserved. 
// 
// SOFTIMAGE is a registered trademark of Avid Technology Inc. or its subsidiaries 
// or divisions. Windows NT is a registered trademark of Microsoft Corp. All other
// trademarks contained herein are the property of their respective owners. 
//
// ****************************************************************************************

//***************************************************************************************
// Defines
//***************************************************************************************
#ifndef __CXSIParser_H__
#define __CXSIParser_H__


//***************************************************************************************
// Includes
//***************************************************************************************
#include <SIBCUtil.h>
#include <SIBCString.h>

#include "dotXSITemplate.h"
#include "CXSIFilter.h"

#define FORMAT_TEXT     0
#define FORMAT_BINARY   1

#define OPEN_READ       0
#define OPEN_WRITE      1


//***************************************************************************************
// Typedefs
//***************************************************************************************
class CdotXSITemplate;
class CdotXSITemplates;

typedef struct
{
    SI_Int      nLen;
    SI_Char    *pLabel;
} TdotXSIDictEntry;

class CXSIParser;

typedef SI_Void (*SI_READ_CALLBACK_FNC)	( CXSIParser* );

//! Class used for the return of values from a nested parse if called explictly.
class XSIEXPORT CXSIReturn
{
	public:
		void			*m_pPointer;
		CSIBCString		m_Parameter;
		int				m_Flags;
};


//! Class used for dictionary entries (binary format).
class XSIEXPORT CdotXSIDict
{
public :
	//! Constructor
    CdotXSIDict();
	//! Destructor
    ~CdotXSIDict();
	/*! Initializes the dictionary with a preallocated number of elements.
	 * \param i_nCount	Number of Elements
	 */
    SI_Void		Init( SI_Int i_nCount );
	/*! Add an element (they must be preallocated first).
	 * \param i_pLabel	Adds a new entry
	 */
    SI_Void		Add( SI_Char *i_pLabel );
	/*! Get the number of elements.
	 *
	 */
    SI_Int		GetCount();
	/*! Get the specified element.
	 * \param i_nInd	Index of the element to retrieve
	 * \param o_pLabel	Returned label
	 */
    SI_Void		Find( SI_Int i_nInd, SI_Char **o_pLabel );
	/*! Get the specified element.
	 * \param i_pLabel	The label looked for
	 * \return the index of the element
	 */
    SI_Int		Find( SI_Char *i_pLabel );
	/*! read m_nTotalCount elements from a file.
	 * \param i_pFptr	File pointer to read from
	 */
    SI_Void		Read( _SI_FILE i_pFptr );
	/*! Write the used elements to file.
	 * \param i_pFptr	File pointer to write to.
	 */
    SI_Void		Write( _SI_FILE i_pFptr );

private :
    SI_Int              m_nTotalCount;
    SI_Int              m_nCount;
    TdotXSIDictEntry   *m_Dict;
};

//! Main class for the XSI parser
class XSIEXPORT CXSIParser
{
public:
	CXSIParser();									//!< Default Constructor
	~CXSIParser();									//!< Default Destructor

	/*! Opens a file for either reading or writing
	 *  \param CSIBCString input file name
	 */
	SI_Error	Open(const CSIBCString &in_filename);		
	/*! Checks whether we are at the end of file
	 *
	 */
	SI_Bool		EndOfFile();						// end of file ? 
	/*! Closes the file
	 *
	 */
	SI_Error	Close();							// Closes a file

	/*! Returns the current file name
	 * \return CSIBCString Filename
	 */
    CSIBCString &FileName() { return m_sFilename; }

	/*! returns the total number of supported templates.
	 * \return SI_Int	number
	 */
	SI_Int		GetNbSupportedTemplates();
	/*! returns the name of a supported template
	 * \return SI_Char * template name
	 */
	SI_Char		*GetSupportedTemplate(SI_Int in_lIndex);
	
	/*! returns the next token as a string
	 * \return SI_Char * token
	 */
	SI_Char		*GetNextTokenAsCharPtr();
	/*! returns the next token and turns it into a null terminated string and removes quotes
	 * \return SI_Char * token
	 */
	SI_Char		*GetNextTokenAsString();
	/*! returns the next token as a float
	 * \return SI_Float token
	 */
    SI_Float	GetNextTokenAsFloat();
	/*! returns the next token as an integer
	 * \return SI_Int token
	 */
	SI_Int		GetNextTokenAsInteger();
	/*! returns the next tokens into an array of floats
	 * \param numFloats number of floats to read
	 * \param fileStepCount number of sequential floats to read first
	 * \param fileSkipCount number of tokens to skip before reading the next sequence of floats
	 * \param arrayStepCount number of sequential floats to write to first
	 * \param arraySkipCount number of array indices to skip before writing the next sequence of floats
	 * \param i_pArray	array of floats to read.
	 * \return SI_Error 
	 */
	SI_Error	GetNextTokensAsFloatArray(	SI_Int numFloats,
											SI_Int fileStepCount,	//Step = number of sequential floats to read first, 
											SI_Int fileSkipCount,	//Skip = number of tokens to skip before reading the next sequence of floats
											SI_Int arrayStepCount,	//Step = number of sequential floats to write to first
											SI_Int arraySkipCount,	//Skip = number of array indices to skip before writing the next sequence of floats
											SI_Float *i_pArray );	//Get the next series of tokens 

	/*! returns the next tokens into an array of Ints
	 * \param numFloats number of ints to read
	 * \param fileStepCount number of sequential ints to read first
	 * \param fileSkipCount number of tokens to skip before reading the next sequence of ints
	 * \param arrayStepCount number of sequential ints to write to first
	 * \param arraySkipCount number of array indices to skip before writing the next sequence of ints
	 * \param i_pArray	array of ints to read.
	 * \return SI_Error 
	 */
	SI_Error	GetNextTokensAsIntegerArray(	SI_Int numInts,
												SI_Int fileStepCount,	//Step = number of sequential floats to read first, 
												SI_Int fileSkipCount,	//Skip = number of tokens to skip before reading the next sequence of floats
												SI_Int arrayStepCount,	//Step = number of sequential floats to write to first
												SI_Int arraySkipCount,	//Skip = number of array indices to skip before writing the next sequence of floats
												SI_Int *i_pArray );	//Get the next series of tokens 

	/*! returns the next tokens into an array of Unsigned Shorts
	 * \param numFloats number of Unsigned Shorts to read
	 * \param fileStepCount number of sequential Unsigned Shorts to read first
	 * \param fileSkipCount number of tokens to skip before reading the next sequence of Unsigned Shorts
	 * \param arrayStepCount number of sequential Unsigned Shorts to write to first
	 * \param arraySkipCount number of array indices to skip before writing the next sequence of Unsigned Shorts
	 * \param i_pArray	array of Unsigned Shorts to read.
	 * \return SI_Error 
	 */
	SI_Error	GetNextTokensAsUnsignedShortArray(	SI_Int numUShorts,
												SI_Int fileStepCount,	//Step = number of sequential floats to read first, 
												SI_Int fileSkipCount,	//Skip = number of tokens to skip before reading the next sequence of floats
												SI_Int arrayStepCount,	//Step = number of sequential floats to write to first
												SI_Int arraySkipCount,	//Skip = number of array indices to skip before writing the next sequence of floats
												SI_UShort *i_pArray );	//Get the next series of tokens 

	/*! Get the current token as a Char Pointer.
	 * \return SI_Char * current token.
	 */
	SI_Char		*GetCurrentTokenAsCharPtr();
	/*! Get the current token as a Float.
	 * \return SI_Float returns current token
	 */
	SI_Float	GetCurrentTokenAsFloat();

	/*! Get the current token as a Integer.
	 * \return SI_Int returns current token
	 */
	SI_Int		GetCurrentTokenAsInteger();

	/*! Gets the next pair as a floating point number 
	 * \param fX First value
	 * \param fY Second value
	 */

	SI_Error	GetNextPairAsFloat( SI_Float &fX, SI_Float &fY );
	/*! Gets the next pair as a Integer number 
	 * \param fX First value
	 * \param fY Second value
	 */
	SI_Error 	GetNextPairAsInt( SI_Int &nX, SI_Int &nY );

	/*!Gets the next tripplet as a floating point number. 
	 * \param fX First value
	 * \param fY Second value
	 * \param fZ Third value
	 */
	
	SI_Error	GetNextTrippletAsFloat( SI_Float &fX, SI_Float &fY, SI_Float &fZ );
	/*!Gets the next tripplet as a Integer number. 
	 * \param fX First value
	 * \param fY Second value
	 * \param fZ Third value
	 */
	SI_Error 	GetNextTrippletAsInt( SI_Int &nX, SI_Int &nY, SI_Int &nZ );
	
	/*! Sets the Open Mode
	 * \param i_nMode Mode
	 */
    SI_Void     SetOpenMode( SI_Int i_nMode );
	/*! Returns the Input Mode
	 * \return The current input mode.
	 */
    SI_Int      GetOpenMode() { return m_nOpenMode; }

    // dotXSIFile support
	
	/*! Check the next token to verify wheter they are children templates or not
	 *  \param i_pReadString place holder for the token
	 *	\return TRUE/FALSE
	 */
    SI_Int              CheckNextToken( CSIBCString *i_pReadString );
	/*! returns the the Major XSI File Version.
	 * \return SI_Long	Major version number
	 */
    SI_Long              GetdotXSIFileVersionMajor() { return m_dotXSIFileVersionMajor; } 
	/*! returns the the Minor XSI File Version.
	 * \return SI_Long	Minor version number
	 */
    SI_Long              GetdotXSIFileVersionMinor() { return m_dotXSIFileVersionMinor; }  
	/*! Returns the dotXSI file Format.
	 *  \return SI_Int (0 text, 1 binary)
	 */
    SI_Int              GetdotXSIFormat() { return m_dotXSIFormat; }					  //@mfunc SI_Int | CXSIParser | GetdotXSIFormat | Gets 
	/*! Sets the dotXSI file Format.
	 * \param Sets the format (0 - text, 1 - binary).
	 */
    SI_Void             SetdotXSIFormat( SI_Int dotXSIFormat ) { m_dotXSIFormat = dotXSIFormat; } //@mfunc SI_Void | CXSIParser | SetdotXSIFormat | Sets the dotXSI file Formation.
																								  // @parm SI_Int | dotXSIFormat | dotXSI format flag.
	/*! Returns the System Flags
	 * \return SI_Int	System flags
	 */
	SI_Int              GetdotXSISystemFlags() { return m_dotXSISystemFlags; }					//@mfunc SI_Int | CXSIParser | GetdotXSISystemFlags | Gets the dotXSI System Flags.
	/*! Sets the System flags
	 * \param SI_Int The system flags.
	 */
    SI_Void             SetdotXSISystemFlags( SI_Int dotXSISystemFlags ) { m_dotXSISystemFlags = dotXSISystemFlags; }	//@mfunc SI_Void | CXSIParser | SetdotXSISystemFlags | Sets the dotXSI System Flags.
																									// @parm SI_Int | dotXSIFormat | dotXSI format flag.
	/*! Sets the file version
	 *  \param SI_Int Major version
	 *  \param SI_Int Minor version
	 */
	SI_Int				SetdotXSIFileVersion( SI_Int, SI_Int );
	/*! Returns how many file version is supported
	 *  \return SI_Int number of supported version
	 */
	SI_Int				GetdotXSINumFileVersion();
	/*! Returns the Major & Minor number of a given supported version
	 * \param SI_Int Which version number to query
	 * \param SI_Long* Major version returned
	 * \param SI_Long* Minor version returned.
	 * 
	 */
	SI_Error			GetdotXSIFileVersion( int, int*, int* );
	/*! Reads the file as it was set in the parser.
	 * 
	 */
    SI_Error            Read();
	/*! Writes the file as specified.
	 * 
	 */
    SI_Error            Write();
	/*! Recursive function to read the templates.
	 * \param CSIBCString* name of template.
	 * \param CdotXSITemplate* Parent template.
	 * 
	 */
    SI_Bool             ReadTemplate( CSIBCString *i_sTemplateName, CdotXSITemplate *i_pParent );
	/*! Recursive function to write out the templates
	 * \param CdotXSITemplates* Parent template collection
	 * \param SI_Int Index of the children (which children in the collection we are exporting)
	 * \param SI_Int Indentation (how much to indent to keep the look indented as we nest down).
	 */
    SI_Void             WriteTemplate( CdotXSITemplates *dotXSITemplates, SI_Int i_nInd, SI_Int i_nLevel );
	/*! Returns the template collection of the Parser (only the toplevel ones).
	 * \return CdotXSITemplates* collection
	 * 
	 */
    CdotXSITemplates   *dotXSITemplate() { return m_dotXSITemplates; }	//@mfunc CdotXSITemplates* | CXSIParser | dotXSITemplate | Checks for dotXSI templates.
	/*! Returns the template collection supported for the current version
	 * \return CdotXSITemplates* collection
	 */
    CdotXSITemplates   *dotXSISupported() { return m_dotXSISupported; } //@mfunc CdotXSITemplates* | CXSIParser | ditXSISupported | Checks for dotXSI support.
	/*! Returns a string as read (should not be used)
	 * 
	 */
    SI_Int              GetString();
	/*! adds an escape character in front of invalid ones (ie. \" ) 
	 * \param in_pStr	Input String
	 * \param out_pStr	Resulting String.
	 */
	SI_Error			AddEscapeChar ( const SI_Char *in_pStr, SI_Char *out_pStr );						//@mfunc adds an escape character in front of invalid ones (ie. \" )

    // Read utility functions
	/*! This function is used to read the parameters which are generic (no special cases).
	 * Reads the parameters into the new template based on the parameter list from the current template.
	 * Generic parameter read. This is used by most of the callbacks because most
	 * of the templates have some generic parameters that can be read in batch.
	 * \param i_pCurrentTemplate	current template
	 * \param i_pNewTemplate		New template (starts empty)
	 * \param i_nNbParam			Number of parameters to read.
	 * \return SI_Error		Error code indicating the success or failure of this command.
	 *	<ul><br>Possible values:<br>
	 *		<li>SI_SUCCESS - The parameters were read correctly.
	 *		<li>SI_ERR_BAD_ELEMENT - There was an error reading a parameter.
	 *	</ul>
	 * 
	 */
    SI_Error ReadGenericParameters( CdotXSITemplate *i_pCurrentTemplate, CdotXSITemplate *i_pNewTemplate, SI_Int i_nNbParam );
	/*! Reads the next value based on the type expected in the Variant
	 * \param o_vValue			Variant to receive the value based on its type.
	 * \param i_nLastInt		unused.
	 * \return SI_Error		Error code indicating the success or failure of this command.
	 *	<ul><br>Possible values:<br>
	 *		<li>SI_SUCCESS - The parameter was read correctly.
	 *		<li>SI_ERR_BAD_ELEMENT - There was an error reading the parameter.
	 *	</ul>
	 */
    SI_Error ReadParameterValue( SI_TinyVariant *o_vValue, SI_Int i_nLastInt );
	/*! Reads an array of integers into a named parameter
	 * \param i_pNewTemplate	template to add the new parameter to
	 * \param i_nNbVertices		number of integers to read
	 * \param i_sName			Name of the new argument.
	 * \return SI_Error		Error code indicating the success or failure of this command.
	 *	<ul><br>Possible values:<br>
	 *		<li>SI_SUCCESS - The array was read correctly.
	 *		<li>SI_ERR_BAD_ELEMENT - There was an error reading the array.
	 *	</ul>
	 */
    SI_Error ReadIntegerArray( CdotXSITemplate *i_pNewTemplate, SI_Int i_nNbVertices, SI_Char *i_sName );

	/*! Reads an array of floats into a named parameter
	 * \param i_pNewTemplate	template to add the new parameter to
	 * \param i_nNbVertices		number of floats to read
	 * \param i_sName			Name of the new argument.
	 * \return SI_Error		Error code indicating the success or failure of this command.
	 *	<ul><br>Possible values:<br>
	 *		<li>SI_SUCCESS - The array was read correctly.
	 *		<li>SI_ERR_BAD_ELEMENT - There was an error reading the array.
	 *	</ul>
	 */
    SI_Error ReadFloatArray( CdotXSITemplate *i_pNewTemplate, SI_Int i_nNbVertices, SI_Char *i_sName );

	/*! Reads an array of doubles into a named parameter
	 * \param i_pNewTemplate	template to add the new parameter to
	 * \param i_nNbVertices		number of floats to read
	 * \param i_sName			Name of the new argument.
	 * \return SI_Error		Error code indicating the success or failure of this command.
	 *	<ul><br>Possible values:<br>
	 *		<li>SI_SUCCESS - The array was read correctly.
	 *		<li>SI_ERR_BAD_ELEMENT - There was an error reading the array.
	 *	</ul>
	 */
    SI_Error ReadDoubleArray( CdotXSITemplate *i_pNewTemplate, SI_Int i_nNbVertices, SI_Char *i_sName );

	// Write utility functions
	/*! Indent the output by so many levels. This is per line of output.
	 * \param i_nLevel		Number of levels to indent.
	 * 
	 */
    SI_Void WriteTabs( SI_Int i_nLevel );
	/*! Write out the parameters that are generic from a given template
	 * \param i_pCurrentTemplate	Template to write from
	 * \param i_nNbParams			Number of parameters to output
	 * \param i_nLevel				Current level (for indentation).
	 * 
	 */
    SI_Void WriteGenericParameters( CdotXSITemplate *i_pCurrentTemplate, SI_Int i_nNbParams, SI_Int i_nLevel );
	/*! Generic parameter write. This is used by most of the callbacks because most
	 *  of the templates have some generic parameters that can be written in batch.
	 *  \param i_vValue	Variant holding the value to output
	 *  \param i_nLevel	Current level for indentation
	 *  \param in_nFlag	Unused
	 */
    SI_Void WriteParameterValue( SI_TinyVariant *i_vValue, SI_Int i_nLevel, SI_Int in_nFlag = 0 );
	/*! Registers the supported templates for a given version
	 * \param i_lMajor	Major version requested
	 * \param i_lMinor	Minor version requested
	 * 
	 */
    SI_Void RegisterdotXSINativeTemplates( SI_Long i_lMajor, SI_Long i_lMinor );
	/*! Clears the list of supported template (this is called prior to RegisterdotXSINativeTemplates).
	 * \sa RegisterdotXSINativeTemplates
	 * 
	 */
    SI_Void CleardotXSINativeTemplates();
	/*! Retrieves the current list of native templates.
	 * 
	 */
    CdotXSITemplates* GetdotXSINativeTemplates();

	//
	// New centralized IO functions
	//
	/*! Function to return whether we are at the end of file.
	 *  \return 1 for eof, 0 otherwise
	 */
	SI_Int			FTK_Eof ();
	/*! Opens the file for filter for reading or writing
	 * \param in_szName	Filename
	 * \param in_Mode	Open Mode
	 * 
	 */
	SI_Int			FTK_Open (SI_Char* in_szName, _SI_FILE_MODE in_Mode );
	/*! Closes the filter
	 * 
	 */
	SI_Int			FTK_Close ();
	/*! Reads from the Filter
	 * \param out_pBuffer	Buffer to receive the read data
	 * \param size			number of bytes to read per object
	 * \param count			size of eash object
	 */
	SI_Long			FTK_Read (void *out_pBuffer, SI_Long size, SI_Long count);
	/*! Writes to the Filter
	 * \param in_pBuffer	Buffer to write to filter
	 * \param size			number of bytes to read per object
	 * \param count			size of eash object
	 */
	SI_Int			FTK_Write (void *in_pBuffer, SI_Long size, SI_Long count);
	/*! Prints an already formatted string as output.
	 * 
	 */
	SI_Int			FTK_Printf (SI_Char* in_szData );
	/*! Seeks to a given position
	 * \param in_lCount		Number of bytes to seek
	 * \param in_iMode		How to move (relative to start, relative to end, relative to current)
	 * 
	 */
	SI_Int			FTK_Seek (SI_Long in_lCount, SI_Int in_iMode );
	/*! Returns the current position
	 * 
	 */
	SI_Long			FTK_Tell ();
	/*! Returns the next byte.
	 * 
	 */
	SI_Int			FTK_FGetc ();
	/*! Returns the next byte.
	 * 
	 */
	SI_Int			FTK_Getc ();

	//
	// Warning callback
	//
	/*! Initializes the Warning callback with a user defined function.
	 * \param in_pfncCallback	Function pointer to receive all warning messages.
	 * 
	 */
	SI_Void			SetWarningCallback ( SI_WARNING_CALLBACK_FNC in_pfncCallback );
	/*! Outputs a warning to the callback (or default if not set).
	 * \param in_szWarning			Warning message
	 * \param in_iWarningLevel		Level (SI_MESSAGE, SI_WARNING, SI_ERROR)
	 *
	 */
	SI_Void			FTK_Warning ( SI_Char *in_szWarning, SI_Int in_iWarningLevel );

	/*! Returns the last error that occurred in the parser.
	 *  \return	SI_Error	The error code indicating the last error that occurred.
	 */
	SI_Error		GetLastError() { return m_nLastError; }

	//
	// read callback
	//
	/*! Initializes the Read callback with a user defined function.
	 * \param in_pCallback		Callback function to use.
	 * 
	 */
	SI_Void					SetReadCallback(SI_READ_CALLBACK_FNC in_pCallback);
	/*! Returns the current Read Callback
	 * 
	 */
	SI_READ_CALLBACK_FNC	GetReadCallback();

protected:
	SI_Error	ParseToken();								// Parse content for next token

private:
	enum
	{
		BUFFER_SIZE = 1024,
		FILE_BUFFER_SIZE = 16384
	};

    CSIBCString m_sFilename;                        // File name
	SI_Char		*m_Buffer;							// File read buffer
	SI_Int		m_BufferSize;
	SI_Char		*m_pCurrentToken;					// Current token
    SI_Int      m_nOpenMode;                        // 0 = READ, 1 = WRITE
	SI_Error	m_nLastError;						// Holds the last error that occurred during parsing.

	SI_Bool		IsInvalidParameterValue();			// Checks the current token, to see if it might not be a valid parameter.
	SI_Error	IncrementParseSpot();
	SI_Error	GetNextByte(SI_Byte *in_pRead);		
	void		CreateFilter ();
	SI_Error	ReadDotXSIHeader();
	CXSIFilter*	CreateFilterFromID ( SI_Char* in_szID );

    // dotXSIFile support
    CdotXSITemplates   *m_dotXSITemplates;
    CdotXSITemplates   *m_dotXSISupported;
    CdotXSITemplates   *m_dotXSIUnsupported;

    SI_Long             m_dotXSIFileVersionMajor;
    SI_Long             m_dotXSIFileVersionMinor;
    SI_Int              m_dotXSIFormat;                 // TEXT | BINARY
    SI_Int              m_dotXSISystemFlags;
    
    CdotXSIDict         m_dotXSIDict;
    CdotXSITemplates    m_dotXSINativeTemplates;
	CSIAccumString		m_szAccumBuffer;
	SI_Long				m_lFilePtr;

	//
	// IO Filter
	//

	CXSIFilter			*m_pParserFilter;
	SI_Int				m_iDataExported;

	//
	// Warning reporting callback
	//
	SI_WARNING_CALLBACK_FNC	m_pfncWarningCallback;

	// read callback
	SI_READ_CALLBACK_FNC m_pReadCallback;

};


//***************************************************************************************
// Global Functions
//***************************************************************************************
/*! \addtogroup Utility Utility for the parser
*  @{
*/
/*! Function to return whether the char is a separator or not.
 * \param i_Char
 * \return True if a separator.
 *
 */
extern SI_Bool IsSeparator( SI_Char i_Char );


/* @} */

/*! \addtogroup Callbacks Callbacks for reading/writing
*  @{
*/



/*! function to read a generic template
 * \param i_pParser	Parser used
 * \param i_pCurrentTemplate	Parent Template
 * \param i_pNewTemplate		New child template
 */
SI_Error dotXSICB_ReadGeneric( CXSIParser *i_pParser, CdotXSITemplate *i_pCurrentTemplate, CdotXSITemplate *i_pNewTemplate );
/*! function to read a cluster template
 * \param i_pParser	Parser used
 * \param i_pCurrentTemplate	Parent Template
 * \param i_pNewTemplate		New child template
 */
SI_Error dotXSICB_ReadCluster( CXSIParser *i_pParser, CdotXSITemplate *i_pCurrentTemplate, CdotXSITemplate *i_pNewTemplate );
/*! function to read a constraint template
 * \param i_pParser	Parser used
 * \param i_pCurrentTemplate	Parent Template
 * \param i_pNewTemplate		New child template
 */
SI_Error dotXSICB_ReadConstraint( CXSIParser *i_pParser, CdotXSITemplate *i_pCurrentTemplate, CdotXSITemplate *i_pNewTemplate );
/*! function to read a CustomPset template
 * \param i_pParser	Parser used
 * \param i_pCurrentTemplate	Parent Template
 * \param i_pNewTemplate		New child template
 */
SI_Error dotXSICB_ReadCustomPSet( CXSIParser *i_pParser, CdotXSITemplate *i_pCurrentTemplate, CdotXSITemplate *i_pNewTemplate );
/*! function to read a Envelope template
 * \param i_pParser	Parser used
 * \param i_pCurrentTemplate	Parent Template
 * \param i_pNewTemplate		New child template
 */
SI_Error dotXSICB_ReadEnvelope( CXSIParser *i_pParser, CdotXSITemplate *i_pCurrentTemplate, CdotXSITemplate *i_pNewTemplate );
/*! function to read a FCurve template
 * \param i_pParser	Parser used
 * \param i_pCurrentTemplate	Parent Template
 * \param i_pNewTemplate		New child template
 */
SI_Error dotXSICB_ReadFCurve( CXSIParser *i_pParser, CdotXSITemplate *i_pCurrentTemplate, CdotXSITemplate *i_pNewTemplate );
/*! function to read a FileInfo template
 * \param i_pParser	Parser used
 * \param i_pCurrentTemplate	Parent Template
 * \param i_pNewTemplate		New child template
 */
SI_Error dotXSICB_ReadFileInfo( CXSIParser *i_pParser, CdotXSITemplate *i_pCurrentTemplate, CdotXSITemplate *i_pNewTemplate );
/*! function to read a IKRoot template
 * \param i_pParser	Parser used
 * \param i_pCurrentTemplate	Parent Template
 * \param i_pNewTemplate		New child template
 */
SI_Error dotXSICB_ReadIKRoot( CXSIParser *i_pParser, CdotXSITemplate *i_pCurrentTemplate, CdotXSITemplate *i_pNewTemplate );
/*! function to read a ImageClip template
 * \param i_pParser	Parser used
 * \param i_pCurrentTemplate	Parent Template
 * \param i_pNewTemplate		New child template
 */
SI_Error dotXSICB_ReadImageClip( CXSIParser *i_pParser, CdotXSITemplate *i_pCurrentTemplate, CdotXSITemplate *i_pNewTemplate );
/*! function to read a Light template
 * \param i_pParser	Parser used
 * \param i_pCurrentTemplate	Parent Template
 * \param i_pNewTemplate		New child template
 */
SI_Error dotXSICB_ReadLight( CXSIParser *i_pParser, CdotXSITemplate *i_pCurrentTemplate, CdotXSITemplate *i_pNewTemplate );
/*! function to read a NurbsSurface template
 * \param i_pParser	Parser used
 * \param i_pCurrentTemplate	Parent Template
 * \param i_pNewTemplate		New child template
 */
SI_Error dotXSICB_ReadNurbsSurface( CXSIParser *i_pParser, CdotXSITemplate *i_pCurrentTemplate, CdotXSITemplate *i_pNewTemplate );
/*! function to read a NurbsCurve template
 * \param i_pParser	Parser used
 * \param i_pCurrentTemplate	Parent Template
 * \param i_pNewTemplate		New child template
 */
SI_Error dotXSICB_ReadNurbsCurve( CXSIParser *i_pParser, CdotXSITemplate *i_pCurrentTemplate, CdotXSITemplate *i_pNewTemplate );
/*! function to read a PatchSurface template
 * \param i_pParser	Parser used
 * \param i_pCurrentTemplate	Parent Template
 * \param i_pNewTemplate		New child template
 */
SI_Error dotXSICB_ReadPatchSurface( CXSIParser *i_pParser, CdotXSITemplate *i_pCurrentTemplate, CdotXSITemplate *i_pNewTemplate );
/*! function to read a PolygonList template
 * \param i_pParser	Parser used
 * \param i_pCurrentTemplate	Parent Template
 * \param i_pNewTemplate		New child template
 */
SI_Error dotXSICB_ReadPolygonList( CXSIParser *i_pParser, CdotXSITemplate *i_pCurrentTemplate, CdotXSITemplate *i_pNewTemplate );
/*! function to read a Scene template
 * \param i_pParser	Parser used
 * \param i_pCurrentTemplate	Parent Template
 * \param i_pNewTemplate		New child template
 */
SI_Error dotXSICB_ReadScene( CXSIParser *i_pParser, CdotXSITemplate *i_pCurrentTemplate, CdotXSITemplate *i_pNewTemplate );
/*! function to read a Shape template
 * \param i_pParser	Parser used
 * \param i_pCurrentTemplate	Parent Template
 * \param i_pNewTemplate		New child template
 */
SI_Error dotXSICB_ReadShape( CXSIParser *i_pParser, CdotXSITemplate *i_pCurrentTemplate, CdotXSITemplate *i_pNewTemplate );
/*! function to read a TriangleList template
 * \param i_pParser	Parser used
 * \param i_pCurrentTemplate	Parent Template
 * \param i_pNewTemplate		New child template
 */
SI_Error dotXSICB_ReadTriangleList( CXSIParser *i_pParser, CdotXSITemplate *i_pCurrentTemplate, CdotXSITemplate *i_pNewTemplate );
/*! function to read a TriStrip template
 * \param i_pParser	Parser used
 * \param i_pCurrentTemplate	Parent Template
 * \param i_pNewTemplate		New child template
 */
SI_Error dotXSICB_ReadTriStrip( CXSIParser *i_pParser, CdotXSITemplate *i_pCurrentTemplate, CdotXSITemplate *i_pNewTemplate );
            
/*! function to write a Generic template
 * \param i_pParser	Parser used
 * \param i_pCurrentTemplate	Parent Template
 * \param i_pNewTemplate		New child template
 */
SI_Error dotXSICB_WriteGeneric( CXSIParser *i_pParser, CdotXSITemplate *i_pCurrentTemplate, CdotXSITemplate *i_pNewTemplate, SI_Int i_nLevel );
/*! function to write a Cluster template
 * \param i_pParser	Parser used
 * \param i_pCurrentTemplate	Parent Template
 * \param i_pNewTemplate		New child template
 */
SI_Error dotXSICB_WriteCluster( CXSIParser *i_pParser, CdotXSITemplate *i_pCurrentTemplate, CdotXSITemplate *i_pNewTemplate, SI_Int i_nLevel );
/*! function to write a Constraint template
 * \param i_pParser	Parser used
 * \param i_pCurrentTemplate	Parent Template
 * \param i_pNewTemplate		New child template
 */
SI_Error dotXSICB_WriteConstraint( CXSIParser *i_pParser, CdotXSITemplate *i_pCurrentTemplate, CdotXSITemplate *i_pNewTemplate, SI_Int i_nLevel );
/*! function to write a CustomPSet template
 * \param i_pParser	Parser used
 * \param i_pCurrentTemplate	Parent Template
 * \param i_pNewTemplate		New child template
 */
SI_Error dotXSICB_WriteCustomPSet( CXSIParser *i_pParser, CdotXSITemplate *i_pCurrentTemplate, CdotXSITemplate *i_pNewTemplate, SI_Int i_nLevel );
/*! function to write a Envelope template
 * \param i_pParser	Parser used
 * \param i_pCurrentTemplate	Parent Template
 * \param i_pNewTemplate		New child template
 */
SI_Error dotXSICB_WriteEnvelope( CXSIParser *i_pParser, CdotXSITemplate *i_pCurrentTemplate, CdotXSITemplate *i_pNewTemplate, SI_Int i_nLevel );
/*! function to write a FCurve template
 * \param i_pParser	Parser used
 * \param i_pCurrentTemplate	Parent Template
 * \param i_pNewTemplate		New child template
 */
SI_Error dotXSICB_WriteFCurve( CXSIParser *i_pParser, CdotXSITemplate *i_pCurrentTemplate, CdotXSITemplate *i_pNewTemplate, SI_Int i_nLevel );
/*! function to write a FileInfo template
 * \param i_pParser	Parser used
 * \param i_pCurrentTemplate	Parent Template
 * \param i_pNewTemplate		New child template
 */
SI_Error dotXSICB_WriteFileInfo( CXSIParser *i_pParser, CdotXSITemplate *i_pCurrentTemplate, CdotXSITemplate *i_pNewTemplate, SI_Int i_nLevel );
/*! function to write a IKRoot template
 * \param i_pParser	Parser used
 * \param i_pCurrentTemplate	Parent Template
 * \param i_pNewTemplate		New child template
 */
SI_Error dotXSICB_WriteIKRoot( CXSIParser *i_pParser, CdotXSITemplate *i_pCurrentTemplate, CdotXSITemplate *i_pNewTemplate, SI_Int i_nLevel );
/*! function to write a ImageClip template
 * \param i_pParser	Parser used
 * \param i_pCurrentTemplate	Parent Template
 * \param i_pNewTemplate		New child template
 */
SI_Error dotXSICB_WriteImageClip( CXSIParser *i_pParser, CdotXSITemplate *i_pCurrentTemplate, CdotXSITemplate *i_pNewTemplate, SI_Int i_nLevel );
/*! function to write a NurbsSurface template
 * \param i_pParser	Parser used
 * \param i_pCurrentTemplate	Parent Template
 * \param i_pNewTemplate		New child template
 */
SI_Error dotXSICB_WriteNurbsSurface( CXSIParser *i_pParser, CdotXSITemplate *i_pCurrentTemplate, CdotXSITemplate *i_pNewTemplate, SI_Int i_nLevel );
/*! function to write a NurbsCurve template
 * \param i_pParser	Parser used
 * \param i_pCurrentTemplate	Parent Template
 * \param i_pNewTemplate		New child template
 */
SI_Error dotXSICB_WriteNurbsCurve( CXSIParser *i_pParser, CdotXSITemplate *i_pCurrentTemplate, CdotXSITemplate *i_pNewTemplate, SI_Int i_nLevel );
/*! function to write a PatchSurface template
 * \param i_pParser	Parser used
 * \param i_pCurrentTemplate	Parent Template
 * \param i_pNewTemplate		New child template
 */
SI_Error dotXSICB_WritePatchSurface( CXSIParser *i_pParser, CdotXSITemplate *i_pCurrentTemplate, CdotXSITemplate *i_pNewTemplate, SI_Int i_nLevel );
/*! function to write a PolygonList template
 * \param i_pParser	Parser used
 * \param i_pCurrentTemplate	Parent Template
 * \param i_pNewTemplate		New child template
 */
SI_Error dotXSICB_WritePolygonList( CXSIParser *i_pParser, CdotXSITemplate *i_pCurrentTemplate, CdotXSITemplate *i_pNewTemplate, SI_Int i_nLevel );
/*! function to write a Shape template
 * \param i_pParser	Parser used
 * \param i_pCurrentTemplate	Parent Template
 * \param i_pNewTemplate		New child template
 */
SI_Error dotXSICB_WriteShape( CXSIParser *i_pParser, CdotXSITemplate *i_pCurrentTemplate, CdotXSITemplate *i_pNewTemplate, SI_Int i_nLevel );
/*! function to write a TriangleList template
 * \param i_pParser	Parser used
 * \param i_pCurrentTemplate	Parent Template
 * \param i_pNewTemplate		New child template
 */
SI_Error dotXSICB_WriteTriangleList( CXSIParser *i_pParser, CdotXSITemplate *i_pCurrentTemplate, CdotXSITemplate *i_pNewTemplate, SI_Int i_nLevel );
/*! function to write a TriStrip template
 * \param i_pParser	Parser used
 * \param i_pCurrentTemplate	Parent Template
 * \param i_pNewTemplate		New child template
 */
SI_Error dotXSICB_WriteTriStrip( CXSIParser *i_pParser, CdotXSITemplate *i_pCurrentTemplate, CdotXSITemplate *i_pNewTemplate, SI_Int i_nLevel );

/* @} */

#endif // CXSIParser

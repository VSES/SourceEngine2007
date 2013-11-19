//***************************************************************************************
//
// File supervisor: Dominic Laflamme
//
// (c) Copyright 2001 Avid Technology, Inc. . All rights reserved.
//
//
// @doc
//
// @module      CXSIFilter.h | Main header file for CXSIFilter implementation
//***************************************************************************************

/****************************************************************************************
THIS CODE IS PUBLISHED AS A SAMPLE ONLY AND IS PROVIDED "AS IS".
IN NO EVENT SHALL SOFTIMAGE, AVID TECHNOLOGY, INC. AND/OR THEIR RESPECTIVE 
SUPPLIERS BE LIABLE FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR ANY
DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN
CONNECTION WITH THE USE OR PERFORMANCE OF THIS CODE . 
 
COPYRIGHT NOTICE. Copyright © 1999-2002 Avid Technology Inc. . All rights reserved. 

SOFTIMAGE is a registered trademark of Avid Technology Inc. or its subsidiaries 
or divisions. Windows NT is a registered trademark of Microsoft Corp. All other
trademarks contained herein are the property of their respective owners. 
****************************************************************************************/
#ifndef _CXSIFILTER_H_
#define _CXSIFILTER_H_

#include <SIBCUtil.h>
#include <SIBCString.h>

#define	SI_MEMORY_FILE_NAME		"3DMEMORYBUFFER"

//! Base class for filters.
/**
* This base class defines the basis for all filters in the system. A filter will
* be used both for reading and writing to/from a file. Filters can be chained so we
* can create constructs like, BUFFERING | ASCII  or COMPRESS for example.
*/
class CXSIFilter
{
public:

	/*!
	 * Constructor
	*/
	CXSIFilter();
	/*!
	 * Destructor
	*/
	virtual ~CXSIFilter();


	/*! Sets the name of the file to be used, needed because the FTK receives the name, but
	 *  it is the last filter that will open the file, so it has to be passed down from filter 
	 * to filter.
	 * \param in_szName		Name of the filter.
	*/
	void			SetName ( const CSIBCString & in_szName );

	/*! Returns the name of the file used
	 * \return CSIBCString Name of the file.
	*/
	CSIBCString		GetName () ;

	/*! Sets the ID for the filter. This ID will appear inside the dotXSI header
	 * \param in_szID String ID ("txt" or "bin")
	*/
	void			SetID ( const CSIBCString & in_szID );
	/*! returns the current ID
	 * \return SIBCString ID
	*/
	CSIBCString&	GetID () ;

	/*! Sets the header to be used for the file ("xsi 0360txt") for example
	 * \param in_szHeader header
	*/
	virtual void			SetHeader ( CSIBCString in_szHeader );

	/*! returns the current header
	 * \return SIBCString Header
	*/
	CSIBCString		GetHeader () ;
	/*!
	 * Writes out the header, this is called when we are about to write the file, to make
	 * sure the header is the first thing written in the file.
	*/
	virtual void	WriteHeader();

	/*! Opens the file for either reading or writing
	 * \param in_szFilename	Filename
	 * \param in_Mode		mode
	*/
	virtual int	Open ( CSIBCString in_szFilename, _SI_FILE_MODE in_Mode );
	/*! Closes the file when finished
	*/
	virtual int	Close ();


	/*! Reads a number of bytes into the buffer passed in.
	 * \param out_pBuffer	output buffer
	 * \param in_lSize	number of bytes to read
	 * \return	int		number of bytes read.
	*/
	virtual int Read ( SI_Char * out_pBuffer, SI_Long in_lSize );
	/*! Writes a number of bytes to the file
	 * \param in_pBuffer	input buffer
	 * \param in_lSize	number of bytes to write
	 * \return int		number of bytes written
	*/
	virtual int Write( SI_Char * in_pBuffer,  SI_Long in_lSize );

	/*! Tests whether we have reached the end of file
	 * \return int	flag for end of file (1 - end of file, 0 - not end of file)
	*/
	virtual int Eof();

	/*! Returns the current position in the file.
	 * \returns int	position in the file (0 - beginning)
	*/
	virtual int Tell();

	/*! Sets  sub filter, used during construction.
	 * \param in_pSub	sub filter to use.
	*/
	void		SetSubFilter ( CXSIFilter *in_pSub ) { m_pSubFilter = in_pSub;} ;
	/*! Returns the current subfilter
	 * \return CXSIFilter*	subfilter
	*/
	CXSIFilter *GetSubFilter () { return m_pSubFilter ; };

	int FillBuffer ( CSIBCString in_szFilename );
protected:

	_SI_FILE	m_pStream;
	CSIBCString	m_szHeader;
	CSIBCString	m_szName;
	CSIBCString	m_szID;
	CXSIFilter	*m_pSubFilter;
	SI_Long		m_FileSize;
	SI_Char		*m_pBuffer;
	SI_Long		m_lSubFilterPos;
	SI_Bool		m_bEof;

};

#endif

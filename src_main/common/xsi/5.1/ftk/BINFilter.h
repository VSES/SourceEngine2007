//***************************************************************************************
//
// File supervisor: Dominic Laflamme
//
// (c) Copyright 2001 Avid Technology, Inc. . All rights reserved.
//
//
// @doc
//
// @module      BINFilter.h | Main header file for CBINFilter implementation
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

#ifndef _BINFILTER_H_
#define _BINFILTER_H_

#include <SIBCUtil.h>
#include <SIBCString.h>

#include "CXSIFilter.h"

#define BIN_FILTER_NAME	"Binary"
#define BIN_FILTER_ID		"bin"
#define GROW_BUFFER_MULTIPLIER		0.1f
#define GROW_BUFFER_BYTE_AMOUNT		14

//! This class supports both the legacy and the new binary format.
/*
 * This supports 3.0 compression (entire file at once) and 3.5 and up where we compress
 * one chunk at a time therefore reducing the overall memory usage
 */
class CBINFilter : public CXSIFilter
{
public:
	/*!
	 * Constructor
	 * \param in_bLegacy (in legacy mode (3.0), we have to read the ENTIRE file in before we can decompress)...
	*/
	CBINFilter( bool in_bLegacy );
	/*!
	 * Destructor
	*/
	virtual ~CBINFilter();

	/*! Opens the file for either reading or writing
	 * \param in_szFilename	Filename
	 * \param in_Mode	mode
	*/
	int	Open ( CSIBCString in_szFilename,  _SI_FILE_MODE in_Mode );
	/*! Closes the file when finished
	*/
	int	Close ();
	
	/*! Reads a number of bytes into the buffer passed in.
	 * \param out_pBuffer	output buffer
	 * \param in_lSize	number of bytes to read
	 * \return	int		number of bytes read.
	*/
	int Read ( SI_Char * out_pBuffer, SI_Long in_lSize );
	/*! Writes a number of bytes to the file
	 * \param in_pBuffer	input buffer
	 * \param in_lSize	number of bytes to write
	 * \return int		number of bytes written
	*/
	int Write( SI_Char * in_pBuffer,  SI_Long in_lSize );


	/*! Tests whether we have reached the end of file
	 * \return int	flag for end of file (1 - end of file, 0 - not end of file)
	*/
	int Eof();
	/*! Returns the current position in the file.
	 * \returns int	position in the file (0 - beginning)
	*/
	int Tell();


private:
	void ReadBlock();

	void AdjustBuffer( SI_Long in_lSize );

	SI_UByte	* m_pBytes;
	SI_UByte	* m_pCompressedBuffer;
	SI_Long		m_lCompressedBufferSize;
	SI_Long		m_lPosition, m_lSize;
	bool		m_bLegacy;
	bool		m_bEof;
};

#endif

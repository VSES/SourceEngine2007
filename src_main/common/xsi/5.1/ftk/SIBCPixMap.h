//***************************************************************************************
//
// File supervisor: Softimage 3D Games & 3D Bridge team
//
// (c) Copyright 2001-2002 Avid Technology, Inc. . All rights reserved.
//
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

//***************************************************************************************
// Defines
//***************************************************************************************
#ifndef __CSIBCPixMap_H__
#define __CSIBCPixMap_H__

//***************************************************************************************
// Includes
//***************************************************************************************
#include <SIBCUtil.h>
#include <SIBCNode.h>
#include <SIBCString.h>
#include <SIBCSearchPath.h>


class CSIBCPixMap;

//! Base class for pixmap loader drivers
class XSICOREEXPORT CSIBCPixMapDriver
{
	private:
		SI_Void			*m_pUnused;

	public:

		/*! Constructor
		* \return an instance of CSIBCPixMapDriver
		*/
		CSIBCPixMapDriver();

		/*! Destructor
		*/
		virtual ~CSIBCPixMapDriver();

		/*! tells whether the extension passed in the string is supported by this driver
		* \param in_Filename file name to check 
		* \return SI_Bool
		* \retval TRUE if this file is supported
		* \retval FALSE if this file is not supported
		*/
		virtual SI_Bool Supported( CSIBCString &in_Filename ) = 0;

		/*! Loads a bitmap file into a CSIBCPixMap object
		* \param in_filename bitmap file name
		* \param in_PixMap CSIBCPixMap object to load the file into
		* \return SI_Error
		* \retval SI_SUCCESS loading has been succesful
		* \retval SI_ERR_ERROR_MSG loading has failed
		* \retval SI_FILE_NOT_FOUND the file has not been found
		*/
		virtual SI_Error Load( CSIBCString &in_filename, CSIBCPixMap &in_PixMap ) = 0;		

		/*! Loads a memory bitmap file into a CSIBCPixMap object
		* \param in_pMemoryBlock memory block
		* \param in_lCount memory block size 
		* \param in_PixMap CSIBCPixMap object to load the file into
		* \return SI_Error
		* \retval SI_SUCCESS loading has been succesful
		* \retval SI_ERR_ERROR_MSG loading has failed
		* \retval SI_FILE_NOT_FOUND the file has not been found
		*/
		virtual SI_Error LoadFromMemory( void *in_pMemoryBlock, SI_Int in_lCount, CSIBCPixMap &in_PixMap ) { return SI_FILE_NOT_FOUND; }
};


// Pixel map class
class XSICOREEXPORT CSIBCPixMap : public CSIBCNode
{
public:

	//! Type of RGBA component
	enum
	{
		RED,		//!< Red component.
		GREEN,		//!< Green component.
		BLUE,		//!< Blue component.
		ALPHA		//!< Alpha component.
	};

	//! Type of pixmap.
	enum
	{
		//Types
		RGB,		//!< RGB
		RGBA,		//!< RGBA
		PALETIZED	//!< Paletized
	};

	//! texture ID
	enum
	{
		INVALID_TEXTURE_ID = 0xffffffff //!< invalid texture ID
	};

	//! PixMap resizing method
	enum
	{
		RESIZE_LOWER,		//!< Resize Lower 
		RESIZE_HIGHER,		//!< Resize Higher
		RESIZE_NEAREST		//!< Resize Nearest
	};

	/*! Constructor
	* \return an instance of CSIBCPixMap
	*/
	CSIBCPixMap();															

	/*! Destructor
	*/
	virtual ~CSIBCPixMap();											

	//Info Retrieval
	/*! Get the size of the Color LookUp Table (CLUT)
	* \return SI_Int the size of the color lookup table
	*/
	SI_Int GetCLUTSize(){return (1<<m_ClutWidth);};	

	/*! Get the width of the image in pixels
	* \return SI_Int the width of the image in pixels
	*/
	SI_Int GetWidth();

	/*! Get the height of the image in pixels
	* \return SI_Int the height of the image in pixels
	*/
	SI_Int GetHeight();

	/*! Gets the type of image
	* \return SI_Int the type of the image
	* \retval CSIBCPixMap::RGB RGB image
	* \retval CSIBCPixMap::RGBA RGBA image
	* \retval CSIBCPixMap::PALETIZED Paletized image
	*/
	SI_Int GetType();	

	// 
	/*! Get the depth of all components
	* \return SI_Int the depth of all components
	*/
	SI_Int GetDepth();

	/*! Get the depth of a given component
	* \param in_lComponent which component to get the depth for
	* \return SI_Int the depth of a specific component
	*/
	SI_Int GetDepth( SI_Int in_lComponent);						

	/*! Set the depth of all components
	* \param an array of 4 SI_Int containing the depth for each component
	* \return SI_Error
	* \retval SI_SUCCESS this method always returns SI_SUCCESS
	*/
	SI_Error SetDepth( SI_Int *i_pDepth);

	/*! Gets the total depth of a pixel in byte
	* \return SI_Int the total depth of a pixel in byte
	*/
    SI_Int GetTotalPixelDepthByte();	

	/*! Get the internal pixel buffer
	* \return SI_UByte* the buffer pointer
	*/
	SI_UByte *GetMap();
	
	/*! Gets a pixel at x,y
	* \param x the X coordinate to get the pixel at
	* \param y the Y coordinate to get the pixel at
	* \param pVal an array of SI_UByte to store the pixel RGBA values
	* \return SI_Error
	* \retval SI_SUCCESS this method always return SI_SUCCESS
	*/

	SI_Error GetPixel( SI_Int x, SI_Int y, SI_UByte *pVal );
	/*! Gets a pixel at x,y
	* \param x the X coordinate to get the pixel at
	* \param y the Y coordinate to get the pixel at
	* \param r a SI_Int holding the red value of the pixel
	* \param g a SI_Int holding the green value of the pixel
	* \param b a SI_Int holding the blue value of the pixel
	* \param a a SI_Int holding the alpha value of the pixel
	* \return SI_Error
	* \retval SI_SUCCESS this method always return SI_SUCCESS
	*/

	SI_Error GetPixel( SI_Int x, SI_Int y, SI_Int &r, SI_Int &g, SI_Int &b, SI_Int &a );

	/*! Sets a pixel at x,y
	* \param x the X coordinate to set the pixel at
	* \param y the Y coordinate to set the pixel at
	* \param pVal an array of SI_UByte to store the pixel RGBA values
	* \return SI_Error
	* \retval SI_SUCCESS this method always return SI_SUCCESS
	*/
	SI_Error SetPixel( SI_Int x, SI_Int y, SI_UByte *pVal );				

	/*! Sets a pixel at x,y
	* \param x the X coordinate to set the pixel at
	* \param y the Y coordinate to set the pixel at
	* \param r a SI_Int holding the red value of the pixel
	* \param g a SI_Int holding the green value of the pixel
	* \param b a SI_Int holding the blue value of the pixel
	* \param a a SI_Int holding the alpha value of the pixel
	* \return SI_Error
	* \retval SI_SUCCESS this method always return SI_SUCCESS
	*/
	SI_Error SetPixel( SI_Int x, SI_Int y, SI_Int r, SI_Int g, SI_Int b, SI_Int a );

	/*! Gets a pixel color index at x,y
	* \param x the X coordinate to get the pixel at
	* \param y the Y coordinate to get the pixel at
	* \param index a SI_Int holding the color index of the pixel
	* \return SI_Error
	* \retval SI_SUCCESS this method always return SI_SUCCESS
	*/
	SI_Error SetPixel( SI_Int x, SI_Int y, SI_Int index );

	/*! Gets a pixel color index at x,y
	* \param x the X coordinate to get the pixel at
	* \param y the Y coordinate to get the pixel at
	* \param pVal an array of SI_UByte to store the pixel color index values
	* \return SI_Error
	* \retval SI_SUCCESS this method always return SI_SUCCESS
	*/
	SI_Error GetPixelColourIndex( SI_Int x, SI_Int y, SI_UByte *pVal );

	/*! Retrieves a colour from the CLUT (PALETIZED only)
	* \param i the color index in the CLUT
	* \param out_pRGBA an array of SI_UByte to store the color index values
	* \return SI_Error
	* \retval SI_SUCCESS this method always return SI_SUCCESS
	*/
	SI_Error GetColour( SI_UByte i, SI_UByte *out_pRGBA ); 

	/*! Retrieves a colour from the CLUT (PALETIZED only)
	* \param i the color index in the CLUT
	* \param r a SI_Int* holding the red value of the color entry
	* \param g a SI_Int* holding the green value of the color entry
	* \param b a SI_Int* holding the blue value of the color entry
	* \param a a SI_Int* holding the alpha value of the color entry
	* \return SI_Error
	* \retval SI_SUCCESS this method always return SI_SUCCESS
	*/
	SI_Error GetColour( SI_UByte i, SI_Int *r, SI_Int *g, SI_Int *b, SI_Int *a );

	/*! Sets a colour in the CLUT (PALETIZED only)
	* \param i the color index in the CLUT
	* \param in_pRGBA an array of SI_UByte to store the color index values
	* \return SI_Error
	* \retval SI_SUCCESS this method always return SI_SUCCESS
	*/
	SI_Error SetColour( SI_UByte i, SI_UByte *in_pRGBA );

	/*! Sets a colour in the CLUT (PALETIZED only)
	* \param i the color index in the CLUT
	* \param r a SI_Int holding the red value of the color entry
	* \param g a SI_Int holding the green value of the color entry
	* \param b a SI_Int holding the blue value of the color entry
	* \param a a SI_Int holding the alpha value of the color entry
	* \return SI_Error
	* \retval SI_SUCCESS this method always return SI_SUCCESS
	*/	
	SI_Error SetColour( SI_UByte i, SI_Int r, SI_Int g, SI_Int b, SI_Int a = 255 );


	/*! Computes the image buffer CRC
	* \return SI_Int the CRC value of the pixmap buffer
	*/
    SI_Int ComputeCRC();

	/*! Pack a rgba pixel in a packed pixel value
	* \param r the red value of the pixel
	* \param g the green value of the pixel
	* \param b the blue value of the pixel
	* \param a the alpha value of the pixel
	* \param pVal an array containing the packed pixel value
	* \return void
	*/
	void PackPixel( SI_Int r, SI_Int g, SI_Int b, SI_Int a, SI_UByte *pVal ); // Turns RGBA into a packed pixel value

	/*! Unpacks a rgba pixel from a packed pixel value
	* \param r the red value of the pixel
	* \param g the green value of the pixel
	* \param b the blue value of the pixel
	* \param a the alpha value of the pixel
	* \param pVal an array containing the packed pixel value
	* \return void
	*/
	void UnPackPixel( SI_Int &r, SI_Int &g, SI_Int &b, SI_Int &a, SI_UByte *pVal );

	/*! Retrieves the index into the image from an XY pair
	* \param x the X coordinates
	* \param y the Y coordinates
	* \return SI_Int the index in the image buffer
	*/
	SI_Int Index( SI_Int x, SI_Int y );	

	// don't comment this, it should no longer be used
	SI_UInt &TextureID() { return m_TextureID; }	

	/*! Initialize the image buffer with a specified width, height, type and depth
	* \param i_Width the width of the image
	* \param i_Height the height of the image
	* \param i_Type the type of the image
	* \param i_pDepth an array containing the depth for each channel
	* \return SI_Error
	* \retval SI_SUCCESS this method always returns SI_SUCCESS
	*/
	SI_Error CreateNew( SI_Int i_Width, SI_Int i_Height,
						SI_Int i_Type, SI_Int *i_pDepth );


	/*! Resizes this pixmap to a width and a height that is a power of 2
	* \param method resizing method
	* \return SI_Error
	* \retval SI_SUCCESS this method always returns SI_SUCCESS
	*/
	SI_Error ResizeToPowerOfTwo( SI_Int method);	

	/*! destroy the image buffer
	* \return SI_Error
	* \retval SI_SUCCESS this method always returns SI_SUCCESS
	*/
	SI_Error DisposeData();

	/*! Converts this color indexed pixmap to RGBA
	* \param in_PixMap the CSIBCPixMap to convert to RGBA
	* \return SI_Error
	* \retval SI_SUCCESS this method always returns SI_SUCCESS
	*/	
	SI_Error ConvertToRGB( CSIBCPixMap & );	


	// Memory estimation over-riden from CSIBCNode
	/*! Gets the used memory
	* \return SI_UInt the used memory
	*/
	virtual SI_UInt UsedMemory();

	/*! Gets the allocated memory
	* \return SI_UInt the allocated memory
	*/
	virtual SI_UInt AllocatedMemory();

	/*! Gets the local size
	* \return SI_UInt the local size
	*/
	virtual SI_UInt LocalSize();

	//Static members

	/*! Get the search path for loading bitmaps
	* \return CSIBCSearchPath instance of CSIBCSearchPath containing all the search paths to load bitmaps from
	* \sa CSIBCSearchPath
	*/
	static CSIBCSearchPath *GetSearchPath();														// Search path for textures

	/*! Add pixmap loader driver
	* \param in_pDriver the new driver to add
	* \return SI_Error
	* \retval SI_SUCCESS this method always returns SI_SUCCESS
	* \sa CSIBCPixMapDriver
	*/
	static SI_Error AddDriver( CSIBCPixMapDriver * );												

	/*! Loads an image from disk into a pixmap
	* \param in_Filename the name of the file to load
	* \param in_PixMap the pixmap to load into
	* \param in_ResizeToPowerOfTwo whether the image should be resized to dimensions of powers of two
	* \return SI_Error
	* \retval SI_SUCCESS is load has succeded
	* \retval SI_ERR_ERROR_MSG is load has failed
	*/
	static SI_Error Load( CSIBCString &in_Filename, CSIBCPixMap &in_PixMap, SI_Bool in_ResizeToPowerOfTwo = TRUE  );

	/*! Loads an image from disk into a pixmap
	* \param in_Filename the name of the file to load
	* \param in_PixMap the pixmap to load into
	* \param in_ResizeToPowerOfTwo whether the image should be resized to dimensions of powers of two
	* \return SI_Error
	* \retval SI_SUCCESS is load has succeded
	* \retval SI_ERR_ERROR_MSG is load has failed
	*/
	static SI_Error Load( const SI_Char *in_Filename, CSIBCPixMap &in_PixMap, SI_Bool in_ResizeToPowerOfTwo = TRUE );

	/*! Loads an image from disk into a pixmap
	* \param in_Filename the name of the file to load
	* \param i_OutMap the pixmap to load into
	* \param pMemoryBlock memory block
	* \param nSize memory block size 
	* \return SI_Error
	* \retval SI_SUCCESS is load has succeded
	* \retval SI_ERR_ERROR_MSG is load has failed
	*/
	static SI_Error Load( CSIBCString &, void *pMemoryBlock, SI_Int nSize, CSIBCPixMap &i_OutMap);

	/*! Loads an image from disk into a pixmap
	* \param in_Filename the name of the file to load
	* \param i_OutMap the pixmap to load into
	* \param pMemoryBlock memory block
	* \param nSize memory block size 
	* \return SI_Error
	* \retval SI_SUCCESS is load has succeded
	* \retval SI_ERR_ERROR_MSG is load has failed
	*/
	static SI_Error Load( const SI_Char *, void *pMemoryBlock, SI_Int nSize, CSIBCPixMap &i_OutMap);

	/*! Cleans the list of loader drivers
	* \return SI_Error
	* \retval SI_SUCCESS this method always returns SI_SUCCESS
	* \sa CSIBCPixMapDriver
	*/
	static SI_Error Cleanup();

protected:
	

private:
	SI_Int m_Width;
	SI_Int m_Height;
	SI_Int m_Type;
	SI_Int m_Depth[4];														// Total bit depth of each component
	SI_Int m_TotalDepth;													// Total byte depth of a pixel
	SI_UInt m_TextureID;
	SI_Int m_ClutDepth;														// Total byte depth of a CLUT entry;
	SI_Int m_ClutWidth;														// bits in CLUT index (ie 2 colour = 1, 256 colour = 8);

	SI_UByte *m_pMap;														// pixel map
	SI_UByte *m_pCLUT;														// Colour Lookup Table
	CSIBCString	m_Filename;													// Filename

	//Static members
	static CSIBCSearchPath s_Path;											// the paths to search for an image
	static CSIBCArray< CSIBCPixMapDriver * > s_Drivers;						// the drivers to try when asked to load an image

	SI_Void	*m_pUnused;
};



#endif 
// CSIBCPixMap

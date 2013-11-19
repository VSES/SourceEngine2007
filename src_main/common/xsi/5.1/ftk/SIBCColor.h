//***************************************************************************************
//
// File supervisor: Softimage 3D Games & 3D Bridge team
//
// (c) Copyright 1999-2002 Avid Technology, Inc. . All rights reserved.
//
// SIBCColor.h | Main header file for SIBCColor implementation
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
#ifndef __CSIBCColor_H__
#define __CSIBCColor_H__

//***************************************************************************************
// Includes
//***************************************************************************************
#include <math.h>
#include <SIBCUtil.h>

//***************************************************************************************
// CSIBCColorf | 
//***************************************************************************************

//! Class representing RGBA colours.
/*!	Represents colours and stores RGBA values as floating point values. 
	Member functions with the "Alpha" suffix operate on alpha as well as RGB, whereas
	their counterparts operate only on RGB color component values. This class is analogous 
	to the CSIBCColorb class, which uses unsigned bytes to represent color components, instead
	of floating-point numbers.
*/
class XSICOREEXPORT CSIBCColorf
{
	public:

		/*! Default constructor.
			Creates a new color object, with uninitialized color values.
		*/			
		CSIBCColorf();								

		/*! Copy Constructor.
			Creates a new color object, based on the values in the \p i_Color object.
			\param	i_Color	Color object to copy values from for the new object.
		*/
		CSIBCColorf( const CSIBCColorf & i_Color);

		/*! Constructor.
			Creates a new color with the given color component values.
			\param	i_fR	Value to set the red component of the color to.
			\param	i_fB	Value to set the blue component of the color to.
			\param	i_fG	Value to set the green component of the color to.
			\param	i_fA	Value to set the alpha component of the color to (defaults to 1.0f).
		*/
		CSIBCColorf(	const SI_Float i_fR, 
						const SI_Float i_fB, 
						const SI_Float i_fG, 
						const SI_Float i_fA = 1.0f );

		~CSIBCColorf();

		// Comparison

		/*! Determines whether the color \p i_Color and this color are equal to each other,
			within a given limit. This is necessary because of floating-point round-off errors.
			\param	i_Color	The color to compare this color with.
			\param	i_Thresh \b Ignored. The comparison uses #PICO_EPS for the comparison threshhold.
			\return	SI_Bool TRUE if all component values of the two colors are no less than #PICO_EPS 
					different, FALSE otherwise.
			\sa CSIBCColorf::operator==
			\sa CSIBCColorf::operator!=
		*/
		SI_Bool IsAlmostEqualTo(const CSIBCColorf& i_Color, const SI_Float i_Thresh) const;

		/*!	Determines whether all component values (\b excluding alpha) of this color are zero. 
			Due to floating-point round-off error, #PICO_EPS is used for the zero comparison. 
			\return SI_Bool	TRUE if all component values are equal to zero, FALSE otherwise.
			\sa CSIBCColorf::IsNullAlpha
			\sa CSIBCColorf::SetNull
		*/
		SI_Bool IsNull() const;

		/*!	Determines whether all component values (\b including alpha) of this color are zero. 
			Due to floating-point round-off error, #PICO_EPS is used for the zero comparison. 
			\return SI_Bool	TRUE if all component values are equal to zero, FALSE otherwise.
			\sa CSIBCColorf::IsNull
			\sa CSIBCColorf::SetNull
		*/
		SI_Bool IsNullAlpha() const;

		// Computed Info

		/*! Gets the dot-product of this color with another color. This is the sum of the scalar 
			multiplication of all corresponding component values in the two colors (\b excluding
			alpha). 
			\param	i_Color	Color object to get this object's dot product with.
			\return	SI_Float	Dot product of the two colors.
			\sa CSIBCColorf::GetDotAlpha
		*/
		SI_Float GetDot(const CSIBCColorf& i_Color) const;
		
		/*! Gets the dot-product of this color with another color. This is the sum of the scalar 
			multiplication of all corresponding component values in the two colors (\b including
			alpha). 
			\param	i_Color	Color object to get this object's dot product with.
			\return	SI_Float	Dot product of the two colors.
			\sa CSIBCColorf::GetDot
		*/
		SI_Float GetDotAlpha(const CSIBCColorf& i_Color) const;

		/*! Computes the distance between the this color and \p i_Color in RGB space (i.e \b excluding
			alpha). 
			\param	i_Color	Color object to get this object's distance from.
			\return	SI_Float	Distance between the two colors in RGB space.
			\sa CSIBCColorf::GetDistanceFromAlpha
		*/
		SI_Float GetDistanceFrom(const CSIBCColorf& i_Color) const;

		/*! Computes the distance between the this color and \p i_Color in RGBA space (i.e \b including
			alpha). 
			\param	i_Color	Color object to get this object's distance from.
			\return	SI_Float	Distance between the two colors in RGBA space.
			\sa CSIBCColorf::GetDistanceFrom
		*/
		SI_Float GetDistanceFromAlpha(const CSIBCColorf& i_Color) const;

		/*! Computes the length of this object's color vector in RGB space (i.e \b excluding alpha).
			\return	SI_Float	The length of this color in RGB space.
			\sa CSIBCColorf::GetLengthAlpha
			\sa CSIBCColorf::GetSquaredLength
		*/
		SI_Float GetLength() const;

		/*! Computes the length of this object's color vector in RGBA space (i.e \b including alpha).
			\return	SI_Float	The length of this color in RGBA space.
			\sa CSIBCColorf::GetLength
			\sa CSIBCColorf::GetSquaredLengthAlpha
		*/
		SI_Float GetLengthAlpha() const;

		/*! Computes the squared length of this object's color vector in RGB space (i.e \b excluding alpha).
			\return	SI_Float	The length of this color in RGB space.
			\sa CSIBCColorf::GetLength
			\sa CSIBCColorf::GetSquaredLengthAlpha
		*/
		SI_Float GetSquaredLength() const;

		/*! Computes the squared length of this object's color vector in RGBA space (i.e \b including alpha).
			\return	SI_Float	The length of this color in RGBA space.
			\sa CSIBCColorf::GetLengthAlpha
			\sa CSIBCColorf::GetSquaredLength
		*/
		SI_Float GetSquaredLengthAlpha() const;

		/*!	Gets the value and index largest color component (\b excluding alpha).
			\param	i_pIndex	Pointer to a value set to the index of the largest component. 
					(0 = Red, 1 = Green, 2 = Blue).
			\return	SI_Float	Value of the largest color component
			\sa	CSIBCColorf::GetMaxComponentAlpha
			\sa CSIBCColorf::GetMinComponent
		*/
		SI_Float GetMaxComponent(SI_Int * i_pIndex) const;

		/*!	Gets the value and index largest color component (\b including alpha).
			\param	i_pIndex	Pointer to a value set to the index of the largest component. 
					(0 = Red, 1 = Green, 2 = Blue, 3 = Alpha).
			\return	SI_Float	Value of the largest color component
			\sa	CSIBCColorf::GetMaxComponent
			\sa CSIBCColorf::GetMinComponentAlpha
		*/
		SI_Float GetMaxComponentAlpha(SI_Int * i_pIndex) const;

		/*!	Gets the value and index smallest color component (\b excluding alpha).
			\param	i_pIndex	Pointer to a value set to the index of the smallest component. 
					(0 = Red, 1 = Green, 2 = Blue).
			\return	SI_Float	Value of the smallest color component
			\sa	CSIBCColorf::GetMinComponentAlpha
			\sa CSIBCColorf::GetMaxComponent
		*/
		SI_Float GetMinComponent(SI_Int * i_pIndex) const;

		/*!	Gets the value and index smallest color component (\b including alpha).
			\param	i_pIndex	Pointer to a value set to the index of the smallest component. 
					(0 = Red, 1 = Green, 2 = Blue, 3 = Alpha).
			\return	SI_Float	Value of the smallest color component
			\sa	CSIBCColorf::GetMinComponent
			\sa CSIBCColorf::GetMaxComponentAlpha
		*/
		SI_Float GetMinComponentAlpha(SI_Int * i_pIndex) const;
		
		// Components

		/*! Gets the value of each color component in this color object (\b including alpha).
			\param	o_pR	Pointer to a SI_Float to receive the Red component of this color.
			\param	o_pG	Pointer to a SI_Float to receive the Green component of this color.
			\param	o_pB	Pointer to a SI_Float to receive the Blue component of this color.
			\param	o_pA	Pointer to a SI_Float to receive the Alpha component of this color.
			\sa CSIBCColorf::Set
		*/
		void Get(SI_Float* o_pR, SI_Float* o_pG, SI_Float* o_pB, SI_Float* o_pA) const;

		/*! Gets the value of each color component in this color object (\b excluding alpha).
			\param	o_pR	Pointer to a SI_Float to receive the Red component of this color.
			\param	o_pG	Pointer to a SI_Float to receive the Green component of this color.
			\param	o_pB	Pointer to a SI_Float to receive the Blue component of this color.
			\sa CSIBCColorf::Set
		*/
		void Get(SI_Float* o_pR, SI_Float* o_pG, SI_Float* o_pB) const;

		/*! Sets each of the color components of this color object.
			\param	i_fR	The value to set the Red component to.
			\param	i_fG	The value to set the Green component to.
			\param	i_fB	The value to set the Blue component to.
			\param	i_fA	The value to set the Alpha component to (defaults to 1.0f).
			\return	CSIBCColorf&	Reference to this object.
			\sa CSIBCColorf::Get
		*/
		CSIBCColorf& Set(SI_Float i_fR, SI_Float i_fG, SI_Float i_fB, SI_Float i_fA = 1.0f );
		
		// Operations

		/*! Normalizes the RGB color vector of this color.
			\return	CSIBCColorf&	Reference to this object.
			\sa CSIBCColorf::NormalizeAlpha
		*/
		CSIBCColorf& Normalize();

		/*! Normalizes the RGBA color vector of this color.
			\return	CSIBCColorf&	Reference to this object.
			\sa CSIBCColorf::Normalize
		*/
		CSIBCColorf& NormalizeAlpha();

		/*! Sets the RGB components of this color to zero, and the alpha to \p alpha.
			\param	alpha	Value to set the alpha component of this color to.
			\return	CSIBCColorf&	Reference to this object
			\sa CSIBCColorf::IsNull
			\sa CSIBCColorf::IsNullAlpha
		*/
		CSIBCColorf& SetNull( SI_Float alpha = 1.0f );
		
		/*! Sets the length of the RGB vector (i.e \b excluding alpha) to \p in_dLength
			\param	in_dLength	The new length for the RGB vector. 
			\return	CSIBCColorf&	Reference to this object.
			\sa CSIBCColorf::SetLengthAlpha
			\sa CSIBCColorf::GetLength
			\sa CSIBCColorf::GetSquaredLength
		*/
		CSIBCColorf& SetLength(const SI_Float in_dLength);

		/*! Sets the length of the RGBA vector (i.e \b including alpha) to \p in_dLength
			\param	in_dLength	The new length for the RGBA vector. 
			\return	CSIBCColorf&	Reference to this object.
			\sa CSIBCColorf::SetLength
			\sa CSIBCColorf::GetLengthAlpha
			\sa CSIBCColorf::GetSquaredLengthAlpha
		*/
		CSIBCColorf& SetLengthAlpha(const SI_Float);

		/*! Negates all color component values (\b excluding alpha). 
			\return	CSIBCColorf&	Reference to this object
			\sa CSIBCColorf::NegateAlpha
		*/
		CSIBCColorf& Negate();

		/*! Negates all color component values (\b including alpha).
			\return	CSIBCColorf&	Reference to this object
			\sa CSIBCColorf::Negate
		*/
		CSIBCColorf& NegateAlpha();

		/*! Sets the color components of this object to the sum of its components with the
			components of \p i_Color (\b excluding alpha).
			\param	i_Color	Color object to add color component values from.
			\return	CSIBCColorf&	Reference to this object
			\sa CSIBCColorf::AddAlpha
		*/
		CSIBCColorf& Add(const CSIBCColorf& i_Color);

		/*! Sets the color components of this object to the sum of its components with the
			components of \p i_Color (\b including alpha).
			\param	i_Color	Color object to add color component values from.
			\return	CSIBCColorf&	Reference to this object
			\sa CSIBCColorf::Add
			\sa CSIBCColorf::operator+
		*/
		CSIBCColorf& AddAlpha(const CSIBCColorf& i_Color);

		/*! Sets the color components of this object to the difference of its components with the
			components of \p i_Color (\b excluding alpha).
			\param	i_Color	Color object to subtract color component values from.
			\return	CSIBCColorf&	Reference to this object
			\sa CSIBCColorf::SubAlpha
		*/
		CSIBCColorf& Sub(const CSIBCColorf& i_Color);

		/*! Sets the color components of this object to the difference of its components with the
			components of \p i_Color (\b including alpha).
			\param	i_Color	Color object to subtract color component values from.
			\return	CSIBCColorf&	Reference to this object
			\sa CSIBCColorf::Sub
		*/
		CSIBCColorf& SubAlpha(const CSIBCColorf& i_Color);

		/*! Scales the color components of this object by \p i_fFactor (\b excluding alpha).
			\param	i_fFactor	Scaling factor to use.
			\return	CSIBCColorf&	Reference to this object
			\sa CSIBCColorf::ScaleAlpha
		*/
		CSIBCColorf& Scale(const SI_Float i_fFactor);

		/*! Scales the color components of this object by \p i_fFactor (\b including alpha).
			\param	i_fFactor	Scaling factor to use.
			\return	CSIBCColorf&	Reference to this object
			\sa CSIBCColorf::Scale
			\sa CSIBCColorf::operator*
		*/
		CSIBCColorf& ScaleAlpha(const SI_Float i_fFactor);

		/*! Sets this object's color component values to the product of its original values,
			and the component values in \p i_Color (\b including alpha).
			\param	i_Color	The color to multiply component values with.
			\return	CSIBCColorf&	Reference to this object
			\sa CSIBCColorf::DivComp
		*/			
		CSIBCColorf& MulComp(const CSIBCColorf& i_Color);

		/*! Sets this object's color component values to the quotient of its original values,
			divided by the component values in \p i_Color (\b including alpha).
			\param	i_Color	The color to divide component values with.
			\return	CSIBCColorf&	Reference to this object
			\sa CSIBCColorf::MulComp
		*/			
		CSIBCColorf& DivComp(const CSIBCColorf& i_Color);
		
	    // overloaded operators

		/*! Equality operator. Determines whether each color component of \p i_Color
			is exactly equal to the corresponding color component in this color.
			\param	i_Color	The color to compare equality with.
			\return	SI_Bool	TRUE if the color components in the objects are identical, FALSE otherwise.
			\sa CSIBCColorf::IsAlmostEqualTo
		*/
		SI_Bool operator == (const CSIBCColorf& i_Color) const;

		/*! Inequality operator. Determines whether any color component of \p i_Color
			are different to the corresponding color component in this color.
			\param	i_Color	The color to compare inequality with.
			\return	SI_Bool	TRUE if the color components in the objects are not identical, FALSE otherwise.
			\sa CSIBCColorf::IsAlmostEqualTo
		*/
		SI_Bool operator != (const CSIBCColorf& i_Color) const;

		/*! Addition operator. Computes a color object with the color components of this objects, and
			\p i_Color summed.
			\param	i_Color	The color object to sum components with.
			\return	CSIBCColorf	Color object representing the color obtained by summing this object and
					\p i_Color's color components.
			\sa CSIBCColorf::Add
		*/
		CSIBCColorf operator+(const CSIBCColorf & i_Color);

		/*! Scaling operator. Computes a color object whose color components are this object's color
			components multiplied by \p i_fFactor.
			\param	i_fFactor	Scaling factor for color components.
			\return	CSIBCColorf	Color object representing this object's scaled color components.
			\sa CSIBCColorf::ScaleAlpha
		*/
		CSIBCColorf operator*(const SI_Float i_fFactor);

		/*! Member variable representing the value of the Red component of the color. */
		SI_Float	m_fR;

		/*! Member variable representing the value of the Green component of the color. */
		SI_Float	m_fG;
		
		/*! Member variable representing the value of the Blue component of the color. */
		SI_Float	m_fB;
		
		/*! Member variable representing the value of the Alpha component of the color. */
		SI_Float	m_fA;

protected:


private:


};

//***************************************************************************************
// CSIBCColorb |
//***************************************************************************************

//! Class representing RGBA colours.
/*!	Represents colours and stores RGBA values as unsigned byte values. 
	Member functions with the "Alpha" suffix operate on alpha as well as RGB, whereas
	their counterparts operate only on RGB color component values. This class is analogous 
	to the CSIBCColorf class, which uses floating-point numbers to represent color components, 
	instead of unsigned bytes.
*/
class XSICOREEXPORT CSIBCColorb
{
	public:

		/*! Default constructor.
			Creates a new color object, with uninitialized color values.
		*/			
		CSIBCColorb();

		/*! Copy Constructor.
			Creates a new color object, based on the values in the \p i_Color object.
			\param	i_Color	Color object to copy values from for the new object.
		*/
		CSIBCColorb( const CSIBCColorb & i_Color);

		/*! Constructor.
			Creates a new color with the given color component values.
			\param	i_bR	Value to set the red component of the color to.
			\param	i_bB	Value to set the blue component of the color to.
			\param	i_bG	Value to set the green component of the color to.
			\param	i_bA	Value to set the alpha component of the color to (defaults to 0).
		*/
		CSIBCColorb(	const SI_UByte	i_bR, 
						const SI_UByte	i_bG, 
						const SI_UByte	i_bB, 
						const SI_UByte	i_bA = 0 );

		~CSIBCColorb();								// Default Destructor

		// Comparison

		/*!	Determines whether all component values (\b excluding alpha) of this color are zero. 
			\return SI_Bool	TRUE if all component values are equal to zero, FALSE otherwise.
			\sa CSIBCColorb::IsNullAlpha
			\sa CSIBCColorb::SetNull
		*/
		SI_Bool IsNull() const;

		/*!	Determines whether all component values (\b including alpha) of this color are zero. 
			\return SI_Bool	TRUE if all component values are equal to zero, FALSE otherwise.
			\sa CSIBCColorb::IsNull
			\sa CSIBCColorb::SetNull
		*/
		SI_Bool IsNullAlpha() const;

		// Computed Info

		/*! Gets the dot-product of this color with another color. This is the sum of the scalar 
			multiplication of all corresponding component values in the two colors (\b excluding
			alpha). 
			\param	i_Color	Color object to get this object's dot product with.
			\return	SI_UByte	Dot product of the two colors.
			\sa CSIBCColorb::GetDotAlpha
		*/
		SI_UByte GetDot(const CSIBCColorb&) const;

		/*! Gets the dot-product of this color with another color. This is the sum of the scalar 
			multiplication of all corresponding component values in the two colors (\b including
			alpha). 
			\param	i_Color	Color object to get this object's dot product with.
			\return	SI_UByte	Dot product of the two colors.
			\sa CSIBCColorb::GetDot
		*/
		SI_UByte GetDotAlpha(const CSIBCColorb&) const;

		/*! Computes the distance between the this color and \p i_Color in RGB space (i.e \b excluding
			alpha). 
			\param	i_Color	Color object to get this object's distance from.
			\return	SI_Float	Distance between the two colors in RGB space.
			\sa CSIBCColorb::GetDistanceFromAlpha
		*/
		SI_Float GetDistanceFrom(const CSIBCColorb&) const;

		/*! Computes the distance between the this color and \p i_Color in RGBA space (i.e \b including
			alpha). 
			\param	i_Color	Color object to get this object's distance from.
			\return	SI_Float	Distance between the two colors in RGBA space.
			\sa CSIBCColorb::GetDistanceFrom
		*/
		SI_Float GetDistanceFromAlpha(const CSIBCColorb&) const;

		/*! Computes the length of this object's color vector in RGB space (i.e \b excluding alpha).
			\return	SI_Float	The length of this color in RGB space.
			\sa CSIBCColorb::GetLengthAlpha
			\sa CSIBCColorb::GetSquaredLength
		*/
		SI_Float GetLength() const;

		/*! Computes the length of this object's color vector in RGBA space (i.e \b including alpha).
			\return	SI_Float	The length of this color in RGBA space.
			\sa CSIBCColorb::GetLength
			\sa CSIBCColorb::GetSquaredLengthAlpha
		*/
		SI_Float GetLengthAlpha() const;

		/*! Computes the squared length of this object's color vector in RGB space (i.e \b excluding alpha).
			\return	SI_Float	The length of this color in RGB space.
			\sa CSIBCColorb::GetLength
			\sa CSIBCColorb::GetSquaredLengthAlpha
		*/
		SI_Float GetSquaredLength() const;

		/*! Computes the squared length of this object's color vector in RGBA space (i.e \b including alpha).
			\return	SI_Float	The length of this color in RGBA space.
			\sa CSIBCColorb::GetLengthAlpha
			\sa CSIBCColorb::GetSquaredLength
		*/
		SI_Float GetSquaredLengthAlpha() const;

		/*!	Gets the value and index largest color component (\b excluding alpha).
			\param	i_pIndex	Pointer to a value set to the index of the largest component. 
					(0 = Red, 1 = Green, 2 = Blue).
			\return	SI_UByte	Value of the largest color component
			\sa	CSIBCColorb::GetMaxComponentAlpha
			\sa CSIBCColorb::GetMinComponent
		*/
		SI_UByte GetMaxComponent(SI_Int * i_pIndex) const;

		/*!	Gets the value and index largest color component (\b including alpha).
			\param	i_pIndex	Pointer to a value set to the index of the largest component. 
					(0 = Red, 1 = Green, 2 = Blue, 3 = Alpha).
			\return	SI_UByte	Value of the largest color component
			\sa	CSIBCColorb::GetMaxComponent
			\sa CSIBCColorb::GetMinComponentAlpha
		*/
		SI_UByte GetMaxComponentAlpha(SI_Int * i_pIndex) const;

		/*!	Gets the value and index smallest color component (\b excluding alpha).
			\param	i_pIndex	Pointer to a value set to the index of the smallest component. 
					(0 = Red, 1 = Green, 2 = Blue).
			\return	SI_UByte	Value of the smallest color component
			\sa	CSIBCColorb::GetMinComponentAlpha
			\sa CSIBCColorb::GetMaxComponent
		*/
		SI_UByte GetMinComponent(SI_Int * i_pIndex) const;

		/*!	Gets the value and index smallest color component (\b including alpha).
			\param	i_pIndex	Pointer to a value set to the index of the smallest component. 
					(0 = Red, 1 = Green, 2 = Blue, 3 = Alpha).
			\return	SI_UByte	Value of the smallest color component
			\sa	CSIBCColorb::GetMinComponent
			\sa CSIBCColorb::GetMaxComponentAlpha
		*/
		SI_UByte GetMinComponentAlpha(SI_Int * i_pIndex) const;
		
		// Components
		/*! Gets the value of each color component in this color object (\b including alpha).
			\param	o_pR	Pointer to a SI_UByte to receive the Red component of this color.
			\param	o_pG	Pointer to a SI_UByte to receive the Green component of this color.
			\param	o_pB	Pointer to a SI_UByte to receive the Blue component of this color.
			\param	o_pA	Pointer to a SI_UByte to receive the Alpha component of this color.
			\sa CSIBCColorb::Set
		*/
		void Get(SI_UByte* o_pR, SI_UByte* o_pG, SI_UByte* o_pB, SI_UByte* o_pA) const;	


		/*! Gets the value of each color component in this color object (\b excluding alpha).
			\param	o_pR	Pointer to a SI_UByte to receive the Red component of this color.
			\param	o_pG	Pointer to a SI_UByte to receive the Green component of this color.
			\param	o_pB	Pointer to a SI_UByte to receive the Blue component of this color.
			\sa CSIBCColorb::Set
		*/
		void Get(SI_UByte* o_pR, SI_UByte* o_pG, SI_UByte* o_pB) const;

		
		/*! Sets each of the color components of this color object.
			\param	i_bR	The value to set the Red component to.
			\param	i_bG	The value to set the Green component to.
			\param	i_bB	The value to set the Blue component to.
			\param	i_bA	The value to set the Alpha component to (defaults to 255).
			\return	CSIBCColorb&	Reference to this object.
			\sa CSIBCColorb::Get
		*/
		CSIBCColorb& Set(SI_UByte i_bR, SI_UByte i_bG, SI_UByte i_bB, SI_UByte i_bA = 255 );
		
		// Operations

		/*! Normalizes the RGB color vector of this color.
			\return	CSIBCColorb&	Reference to this object.
			\sa CSIBCColorb::NormalizeAlpha
		*/
		CSIBCColorb& Normalize();

		/*! Normalizes the RGBA color vector of this color.
			\return	CSIBCColorb&	Reference to this object.
			\sa CSIBCColorb::Normalize
		*/
		CSIBCColorb& NormalizeAlpha();

		/*! Sets the RGB components of this color to zero, and the alpha to \p alpha.
			\param	alpha	Value to set the alpha component of this color to.
			\return	CSIBCColorb&	Reference to this object
			\sa CSIBCColorb::IsNull
			\sa CSIBCColorb::IsNullAlpha
		*/
		CSIBCColorb& SetNull( SI_UByte alpha = 0 );

		/*! Sets the length of the RGB vector (i.e \b excluding alpha) to \p in_dLength
			\param	in_dLength	The new length for the RGB vector. 
			\return	CSIBCColorb&	Reference to this object.
			\sa CSIBCColorb::SetLengthAlpha
			\sa CSIBCColorb::GetLength
			\sa CSIBCColorb::GetSquaredLength
		*/
		CSIBCColorb& SetLength(const SI_Float in_dLength);

		/*! Sets the length of the RGBA vector (i.e \b including alpha) to \p in_dLength
			\param	in_dLength	The new length for the RGBA vector. 
			\return	CSIBCColorb&	Reference to this object.
			\sa CSIBCColorb::SetLength
			\sa CSIBCColorb::GetLengthAlpha
			\sa CSIBCColorb::GetSquaredLengthAlpha
		*/
		CSIBCColorb& SetLengthAlpha(const SI_Float in_dLength);

		/*! Negates all color component values (\b excluding alpha). 
			\return	CSIBCColorb&	Reference to this object
			\sa CSIBCColorb::NegateAlpha
		*/
		CSIBCColorb& Negate();

		/*! Negates all color component values (\b including alpha).
			\return	CSIBCColorb&	Reference to this object
			\sa CSIBCColorb::Negate
		*/
		CSIBCColorb& NegateAlpha();

		/*! Sets the color components of this object to the sum of its components with the
			components of \p i_Color (\b excluding alpha).
			\param	i_Color	Color object to add color component values from.
			\return	CSIBCColorb&	Reference to this object
			\sa CSIBCColorb::AddAlpha
		*/
		CSIBCColorb& Add(const CSIBCColorb& i_Color);

		/*! Sets the color components of this object to the sum of its components with the
			components of \p i_Color (\b including alpha).
			\param	i_Color	Color object to add color component values from.
			\return	CSIBCColorb&	Reference to this object
			\sa CSIBCColorb::Add
			\sa CSIBCColorb::operator+
		*/
		CSIBCColorb& AddAlpha(const CSIBCColorb& i_Color);

		/*! Sets the color components of this object to the difference of its components with the
			components of \p i_Color (\b excluding alpha).
			\param	i_Color	Color object to subtract color component values from.
			\return	CSIBCColorb&	Reference to this object
			\sa CSIBCColorb::SubAlpha
		*/
		CSIBCColorb& Sub(const CSIBCColorb& i_Color);

		/*! Sets the color components of this object to the difference of its components with the
			components of \p i_Color (\b including alpha).
			\param	i_Color	Color object to subtract color component values from.
			\return	CSIBCColorb&	Reference to this object
			\sa CSIBCColorb::Sub
		*/
		CSIBCColorb& SubAlpha(const CSIBCColorb& i_Color);

		/*! Scales the color components of this object by \p i_fFactor (\b excluding alpha).
			\param	i_fFactor	Scaling factor to use.
			\return	CSIBCColorb&	Reference to this object
			\sa CSIBCColorb::ScaleAlpha
		*/
		CSIBCColorb& Scale(const SI_Float i_fFactor);

		/*! Scales the color components of this object by \p i_fFactor (\b including alpha).
			\param	i_fFactor	Scaling factor to use.
			\return	CSIBCColorb&	Reference to this object
			\sa CSIBCColorb::Scale
			\sa CSIBCColorb::operator*
		*/
		CSIBCColorb& ScaleAlpha(const SI_Float i_fFactor);

		/*! Sets this object's color component values to the product of its original values,
			and the component values in \p i_Color (\b including alpha).
			\param	i_Color	The color to multiply component values with.
			\return	CSIBCColorb&	Reference to this object
			\sa CSIBCColorb::DivComp
		*/			
		CSIBCColorb& MulComp(const CSIBCColorb& i_Color);

		/*! Sets this object's color component values to the quotient of its original values,
			divided by the component values in \p i_Color (\b including alpha).
			\param	i_Color	The color to divide component values with.
			\return	CSIBCColorb&	Reference to this object
			\sa CSIBCColorb::MulComp
		*/			
		CSIBCColorb& DivComp(const CSIBCColorb& i_Color);
		
	    // overloaded operators
		/*! Equality operator. Determines whether each color component of \p i_Color
			is equal to the corresponding color component in this color.
			\param	i_Color	The color to compare equality with.
			\return	SI_Bool	TRUE if the color components in the objects are identical, FALSE otherwise.
		*/
		SI_Bool operator == (const CSIBCColorb& i_Color) const;

		/*! Inequality operator. Determines whether any color component of \p i_Color
			are different to the corresponding color component in this color.
			\param	i_Color	The color to compare inequality with.
			\return	SI_Bool	TRUE if the color components in the objects are not identical, FALSE otherwise.
		*/
		SI_Bool operator != (const CSIBCColorb& i_Color) const;

		/*! Addition operator. Computes a color object with the color components of this objects, and
			\p i_Color summed.
			\param	i_Color	The color object to sum components with.
			\return	CSIBCColorb	Color object representing the color obtained by summing this object and
					\p i_Color's color components.
			\sa CSIBCColorb::Add
		*/
		CSIBCColorb operator+(const CSIBCColorb & i_Color);

		/*! Member variable representing the value of the Red component of the color. */
		SI_UByte	m_fR;

		/*! Member variable representing the value of the Green component of the color. */
		SI_UByte	m_fG;
		
		/*! Member variable representing the value of the Blue component of the color. */
		SI_UByte	m_fB;

		/*! Member variable representing the value of the Alpha component of the color. */
		SI_UByte	m_fA;

protected:


private:


};




#endif // CSIBCColor

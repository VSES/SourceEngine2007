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
#ifndef __DOTXSITEMPLATE_H
#define __DOTXSITEMPLATE_H

//***************************************************************************************
// Includes
//***************************************************************************************
#include "SIBCArray.h"
#include "SIBCNode.h"
#include "SIBCString.h"
#include "dotXSIParams.h"

//***************************************************************************************
// Typedefs
//***************************************************************************************
class CdotXSITemplate;
class CXSIParser;

// @type DOTXSICALLBACK | dotXSI Native Templates callback function.
typedef SI_Error ( *DOTXSIREADCALLBACK ) ( CXSIParser *in_pParser, CdotXSITemplate *i_pCurrentTemplate, CdotXSITemplate *i_pNewTemplate );
typedef SI_Error ( *DOTXSIWRITECALLBACK ) ( CXSIParser *in_pParser, CdotXSITemplate *i_pCurrentTemplate, CdotXSITemplate *i_pNewTemplate, SI_Int i_nLevel );

//***************************************************************************************
// @class CdotXSITemplates | container for an array of CdotXSITemplate
//***************************************************************************************

//! Used as a container class for CdotXSITemplate.
/**	This class supports methods for grouping CdotXSITemplate objects together, 
	including indexing of objects, finding objects, etc.
*/
class XSIEXPORT CdotXSITemplates  
{
public:

	/*! Default constructor. Creates a CdotXSITemplates object. 
		Sets the m_bDeleteOnExit flag to true by default.
	*/
	CdotXSITemplates();

	/*! Constructor. Creates a CdotXSITemplates object. 
		Sets the m_bDeleteOnExit flag to the input parameter value
	* \param	in_bDeleteOnExit	This flag has no effect.
	*/
	CdotXSITemplates( SI_Bool in_bDeleteOnExit);

	/*! Destructor.
	*/
	virtual ~CdotXSITemplates();


	/*! Get the number of elements in this particular template hierarchy level.
	* \return The number of elements in this heirarchy level.
	*/
	SI_Int GetCount();

	/*! Get the total number of elements (recursive, including child templates).
	* \return The total number of elements.
	*/
    SI_Int GetTotalCount();	

	/*! Add a CdotXSITemplate element to this container, in the specified position.
	* \param i_Template	Pointer to the object to add to this container.
	* \param i_nPosition	Position in which to add the item. -1 == Current position. Default of -1.
	*/
	SI_Void Add( CdotXSITemplate *i_Template, SI_Int i_nPosition = -1 );

	/*! Gets the specified element (indirectly). This call is equivalent to ::Item(SI_Int),
		except that it stores the return in an out parameter, rather than returning the pointer
		directly from the function itself.

	* \param	i_nPosition	Position of the item to receive.
	* \param	o_Template	Double pointer to receive the pointer to the item. 
				(NULL if \param i_nPosition is invalid).
	* \return	SI_Void
	*/
	SI_Void Item( SI_Int i_nPosition, CdotXSITemplate **o_Template );

	/*! Get the specified element (directly). This call is equivalent to ::Item(SI_Int, CdotXSITemplate **),
		except that it returns the pointer directly from the function instead of storing the return in an 
		out parameter.
	* \param	i_nPosition	Position of the item to receive.
	* \return	CdotXSITemplate	Item at position \param i_nPosition. 
				(NULL if \param i_nPosition is invalid).
	*/
    CdotXSITemplate *Item( SI_Int i_nPosition );

	/*! Remove the specified element.
	* \param	i_nPosition	The position of the element to remove. If \param i_nPosition is invalid, 
				nothing is removed.
	* \return	SI_Void
	*/
    SI_Void Remove( SI_Int i_nPosition );

	/*! Removes all elements from the container.
	* \return	SI_Void
	*/
    SI_Void Clear();

	/*! Checks whether a template of a given type name is contained within this container.
	* \param	i_sName	Type name of the template to search for.
	* \param	o_nInd	Set to the position of the first element of the type given by \param i_sName.
				If none was found, this parameter is set to -1.
	* \return SI_Bool	TRUE if a template with the given type name was found, FALSE otherwise.
	*/
    SI_Bool IsSupported( CSIBCString *i_sName, SI_Int &o_nInd );			

	/*! Searches all templates, and all children of templates contained within this container for
		a CdotXSITemplate whose type name is given by \p i_sTemplateName, and whose instance name
		is given by \p i_sInstanceName.
	* \param	i_sTemplateName	The type name of the template to find. (Required).
	* \param	i_sInstanceName	The instance name of the template to find. (Required).
	* \param	o_pTemplate		Double pointer to receive the pointer to the item. If no item is found
				this parameter is set to NULL.
	* \return	SI_Bool	TRUE if a template matching the given parameters was found, FALSE otherwise.
	*/
	SI_Bool Find( CSIBCString *i_sTemplateName, CSIBCString *i_sInstanceName, CdotXSITemplate **o_pTemplate );	// Find an element by template and instance name

	/*! Search all templates contained within this container for a CdotXSITemplate that has
		a type name of \p in_name.
	* \param in_name	Type name of the template to search for.
	* \return	CdotXSITemplate *	First template within this container whose type name matchs the 
				\p in_name parameter. (NULL if there is no match).
	*/
	CdotXSITemplate	* FindByType( CSIBCString in_name );

	/**	Creates a new container object that contains all elements within this container
		that have a type name that matches \p in_type
	* \param	in_type	Type name of the elements to put in the new container.
	* \return	CdotXSITemplates	Pointer to the new container created. If no elements match the
				type name given type \p in_type, the container is empty (but is still valid).
	*/
	CdotXSITemplates *CdotXSITemplates::ChildrenOfType( CSIBCString in_type );

private:
	SI_Int                          m_nCount;
	SI_Bool							m_bDeleteMembers;
	CSIBCArray< CdotXSITemplate * > m_Templates;
};

//***************************************************************************************
// @class CdotXSITemplate | a template
// @base public | CSIBCNode
//***************************************************************************************

//! Class representing the base class for all template types in XSI.
/**	All scene information in XSI (excluding sources, such as image or audio files) is
	stored within templates. These templates contain parameters that define what type
	the template is, and given the type, what object/property the template represents
	in terms of the scene. This is the base class representing all templates used in
	XSI.

	This class is distinct from the CSLTemplate class. That class, as its name indicates,
	is the template representation used in the Semantic Layer library, whereas this class
	represents the native XSI template.

	Also unlike the CSLTemplate class, this class is concrete, meaning that it can be 
	instantiated. This is not generally done however. Generally, CdotXSITemplate objects
	are only allocated within methods of the Semantic Layer.
*/
class XSIEXPORT CdotXSITemplate : public CSIBCNode
{
public:

	/*! Default constructor.
		Sets the default version information, and the default read and write callbacks
		for the template.
	* \return an instance of CdotXSITemplate
	*/
	CdotXSITemplate();	
	
	/*! Constructor.
		Sets the type name of the template to the name give by \p i_sName.
	* \param	i_sName	Type name of the new template.
	* \return	an instance of CdotXSITemplate
	*/
	CdotXSITemplate( CSIBCString *i_sName );

	/*! Constructor.
		Sets the type name, read and write callbacks, version information, as well as
		a variable number of CdotXSIParam parameter definitions to add to the new template.
	* \param	i_sName	Type name of the new template.

	* \param	i_ReadCallback	Pointer to the read callback function used to read this template.
	* \param	i_WriteCallback	Pointer to the write callback function used to write this template.
	* \param	i_lVersionMajor	Major version number used for this template.
	* \param	i_lVersionMinor	Minor version number used for this template.
	* \param	i_nNbParams		Number of variable CdotXSIParam arguments to follow.
	* \param	i_Param1		First parameter definition (if \p i_nNbParams > 0) to add to this template.
	* \param	...				More parameter definitions (of CdotXSIParam * type) to add to this template.
	* \return	an instance of CdotXSITemplate
	*/
	CdotXSITemplate( CSIBCString i_sName,									// Constructor
                     DOTXSIREADCALLBACK i_ReadCallback,
                     DOTXSIWRITECALLBACK i_WriteCallback,
					 SI_Int i_lVersionMajor, SI_Int i_lVersionMinor,
                     SI_Int i_nNbParams, 
                     CdotXSIParam *i_Param1, ... );

	/*! Constructor.
		Sets the type name, read and write callbacks, as well as a variable number of CdotXSIParam 
		parameter definitions to add to the new template. Note that it does not contain an entry for
		version information, which is set the the default.

	* \param	i_sName	Type name of the new template.
	* \param	i_ReadCallback	Pointer to the read callback function used to read this template.
	* \param	i_WriteCallback	Pointer to the write callback function used to write this template.
	* \param	i_lVersionMajor	Major version number used for this template.
	* \param	i_lVersionMinor	Minor version number used for this template.
	* \param	i_nNbParams		Number of variable CdotXSIParam arguments to follow.
	* \param	i_Param1		First parameter definition (if \p i_nNbParams > 0) to add to this template.
	* \param	...				More parameter definitions (of CdotXSIParam * type) to add to this template.
	* \return	an instance of CdotXSITemplate
	*/
	CdotXSITemplate( CSIBCString i_sName,
                     DOTXSIREADCALLBACK i_ReadCallback,
                     DOTXSIWRITECALLBACK i_WriteCallback,
                     SI_Int i_nNbParams, 
                     CdotXSIParam *i_Param1, ... );


	/*! Destructor
	*/
    virtual ~CdotXSITemplate();

	/*! Changes the instance name of this template to the one given by \p i_sInstanceName
	* \param	i_sInstanceName	The new instance name of this template.
	* \return	SI_Void
	*/
    SI_Void SetInstanceName( CSIBCString i_sInstanceName );

	/**	Returns the instance name of this template.
	* \return CSIBCString&	Reference to the instance name of this template.
	*/
    CSIBCString &InstanceName() { return m_sInstanceName; }

	/*! Set the user data type for this template.
	* \param	i_sUserDataType	String representing the type of user data contained
				in this template.
	* \return	SI_Void
	*/
    SI_Void SetUserDataType( CSIBCString i_sUserDataType );

	/*! Gets the string representing the user data type name.
	* \return CSIBCString&	Reference to the user data type name.
	*/
    CSIBCString &UserDataType() { return m_sUserDataType; }

	/*! Gets the CdotXSITemplates children container instance.
	* \return CdotXSITemplates& Returns the reference to the CdotXSITemplates children container instance.
	*/
    CdotXSITemplates &Children() { return m_ChildrenCol; }

	/**	Sets the parent template of this template to \p i_Parent
	* \param	i_Parent	The new parent template of this template.
	* \return	SI_Void
	*/
    SI_Void SetParent( CdotXSITemplate *i_Parent );

	/*! Gets the parent template of this template.
	* \return	CdotXSITemplate*	Pointer to the parent template of this template.
	*/
    CdotXSITemplate *Parent() { return m_Parent; }

    /*! Sets the system flags for this template. System flags have no specific meaning within
		the template. Applications using the FTK may set/query system flags as a convenient way
		to set application defined flags, per template. Note: This function does not remove any
		flags previously set.

	* \param	i_nSystemFlags	Bit-wise system flags to set for this template.
	* \return	SI_Void
	*/
	SI_Void SetSystemFlags( SI_Int i_nSystemFlags );


    /*! Gets the system flags for this template. System flags have no specific meaning within
		the template. Applications using the FTK may set/query system flags as a convenient way
		to set application defined flags, per template. 

	* \param	o_nSystemFlags	Pointer to an SI_Int to recieve system flags set for this template.
	* \return	SI_Void
	*/
    SI_Void GetSystemFlags( SI_Int *o_nSystemFlags );

    /*! Sets the user flags for this template. User flags have no specific meaning within
		the template. Applications using the FTK may set/query user flags as a convenient way
		to set application defined flags, per template. Note: This function does not remove any
		flags previously set.

	* \param	i_nUserFlags	Bit-wise user flags to set for this template.
	* \return	SI_Void
	*/
    SI_Void SetUserFlags( SI_Int i_nUserFlags );

    /*! Gets the user flags for this template. User flags have no specific meaning within
		the template. Applications using the FTK may set/query user flags as a convenient way
		to set application defined flags, per template.

	* \param	o_nUserFlags	Pointer to an SI_Int to recieve system flags set for this template.
	*/
    SI_Void GetUserFlags( SI_Int *o_nUserFlags );

	/*! Gets the CdotXSIParams params container instance.
	* \return	CdotXSIParams&	Reference to the CdotXSIParams container holding the CdotXSIParam entries
				for this template.
	*/
	CdotXSIParams &Params() { return m_ParamsCol; }

	/*! Initialize the template from a name in the native templates collection.
	* \param i_pChar	String representing the type name of the template to initialize.
	* \param in_pParser	Pointer to the parser object. 
	* \return	SI_Void
	*/
    SI_Void InitializeFromName( SI_Char *i_pChar, CXSIParser *in_pParser );

	/*! Returns the version information of this template. If either parameter is NULL, no information
		is returned.

	* \param	out_iMajor	Pointer to an SI_Int which takes the value of the major version.
	* \param	out_iMinor	Pointer to an SI_Int which takes the value of the minor version.
	* \return	SI_Void
	*/
	SI_Void	GetVersionInfo( SI_Int *out_iMajor, SI_Int *out_iMinor );

	/*! Creates a new CdotXSITemplates container, containing all child templates of this template,
		that match the type name given by \p in_type. If there are no matches, the created container
		is empty (but is still valid).

	* \param	in_type	The type name to match to the child type name.
	* \return	CdotXSITemplates*	Pointer to the created container.
	*/
	CdotXSITemplates *ChildrenOfType( CSIBCString in_type );

	/**	Returns the first child template that has type name \p in_type and instance name \p in_name.
	* \param	in_type	The type name to match to a child template.
	* \param	in_name	The instance name to match to a child template.
	* \return	CdotXSITemplate*	The first child of the template that matches both the type name
				given by \p in_type and instance name \p in_name. NULL if no children match the given
				parameters.
	*/
	CdotXSITemplate *ChildrenOfTypeNamed( CSIBCString in_type, CSIBCString in_name );

private:
	CSIBCString        m_sInstanceName;
	CSIBCString        m_sUserDataType;
	CdotXSITemplates   m_ChildrenCol;
	CdotXSITemplate   *m_Parent;
	SI_Int             m_nSystemFlags;
	SI_Int             m_nUserFlags;
	CdotXSIParams      m_ParamsCol;
	SI_Int			   m_nVersionMajor, m_nVersionMinor;

public:
    //! Callback function for reading the template.
    DOTXSIREADCALLBACK        m_ReadCallback;

    //! Callback function for writing the template.
    DOTXSIWRITECALLBACK       m_WriteCallback;
};

//***************************************************************************************
// Functions
//***************************************************************************************

/*! \addtogroup glbfunc_template Global dotXSI template functions
*  @{
*/

/*! Allocate a CdotXSITemplate
* \return CdotXSITemplate* the newly created CdotXSITemplate
*/
XSIEXPORT CdotXSITemplate* CreatedotXSITemplate();							// 

/*! Allocate a CdotXSITemplate collection
* \return CdotXSITemplates* the newly created CdotXSITemplates
*/
XSIEXPORT CdotXSITemplates* CreatedotXSITemplates();						// Allocate a CdotXSITemplates

/* @} */

//***************************************************************************************
// Function prototype to create templates....
//***************************************************************************************

typedef	CdotXSITemplate* (*TemplateCreation)();

#endif // __DOTXSITEMPLATE_H

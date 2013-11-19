/*! \file xsi_miuserdata_defs.h
    \brief XSI mental ray userdata defines.
    
    The file defines the magic numbers and structures required to query and access
	for userdata passed to a mental ray shader by XSI.
*/
#ifndef XSI_MIUSERDATA_DEFS_H
#define XSI_MIUSERDATA_DEFS_H


/* Light attributes ------------------------------------------*/

/*! \def XSILightAttributes_Magic
    \brief The magic number for querying light userdata from a mental ray shader.
   
	In XSI, you can set lights to affect different portions of the lighting calculation. 
	Normally, a lighting model, such as Phong, computes a diffuse component and a 
	specular component as part of the lighting calculation. However, users can specify 
	that a light should not contribute to the diffuse component. All four combinations 
	(diffuse light, specular light, both, neither) are acceptable. 

	To pass this information to the shaders, XSI associates a piece of user data 
	with the light. The shader can then extract this user data from the light, 
	and perform the appropriate calculations. 

	To find the user data, the shader must ask for the head of the list of user data 
	associated with the light. The shader then searches through this list for the 
	user data with the magic number XSILightAttributes_Magic. If it finds it, 
	it extracts the settings using the XSILightAttributesUD struct. If it does 
	not find the data, it implies that both specular and diffuse computations 
	should be performed. 

*/
#define XSILightAttributes_Magic		2860411

/*! \def XSILightAttributes_Diffuse
    \brief The light contributes to the diffuse component.
   
	Perform an logical AND operator with this value and the 
	value in XSILightAttributesUD's "affects" attribute to determine 
	it if the light contributes to the diffuse component. 
*/
#define XSILightAttributes_Diffuse		1

/*! \def XSILightAttributes_Specular
    \brief The light contributes to the specular component.
   
	Perform an logical AND operator with this value and the 
	value in XSILightAttributesUD's "affects" attribute to determine 
	it if the light contributes to the specular component. 
*/
#define XSILightAttributes_Specular		2

/*! \struct XSILightAttributesUD
    \brief mental ray userdata struct for Light settings.

     Perform an logical AND operator with the constant values:
	 XSILightAttributes_Diffuse and XSILightAttributes_Specular
	 with the affects attribute to determine if the light 
	 contributes to the diffuse & specular component.
 
*/
typedef struct 
{
	unsigned char affects;	 /*!< A flag determining if the diffuse and specular are applied. */
} XSILightAttributesUD;
/* -----------------------------------------------------------*/

/* RenderMap -------------------------------------------------*/
/*! \def XSIRenderMapData_Magic
    \brief The magic number for querying rendermap userdata from a mental ray shader.
   
     For future use.
*/
#define XSIRenderMapData_Magic			2890442

/* Pass Channels ---------------------------------------------*/
#define XSIPassChannels_Magic			2890443

/*! \struct XSIPassChannel
	\brief mental ray userdata for individual pass channel information.

	This struct contains information on how a named pass channel in XSI maps 
	to a framebuffer number in mental ray. 
	\seealso XSIPassChannelsUD
*/
typedef struct {
	int			fb;			/*!< The framebuffer index to use with mi_fb_put etc. */
	char		name[ 64 ];	/*!< The name of the pass channel associated with this framebuffer index */
} XSIPassChannel;

/*! \struct XSIPassChannelsUD
	\brief mental ray userdata for information on all active pass channels.
	
	Each pass channel corresponds to an active pass channel in XSI. It is 
	intended that the shader loops through each pass channel data block, in
	its _init function, to see if any of them matches the named pass channel 
	requirement for that shader. It can then store the corresponding framebuffer 
	ids in a userdata block for the shader.
	This userdata lives on the miOptions found in miState::options.userdata
*/
typedef struct {
	int				nb_passchannels;	/*!< Number of pass channels defined */
	XSIPassChannel	passchannel[ 1 ];	/*!< An array of pass channels. The number of pass channels is defined by nb_passchannels */
} XSIPassChannelsUD;

/* -----------------------------------------------------------*/

#endif /* XSI_MIUSERDATA_DEFS_H */

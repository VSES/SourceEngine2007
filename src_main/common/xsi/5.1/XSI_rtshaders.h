//***************************************************************************************
//
// File supervisor: Softimage 3D Games & 3D Bridge team
//
// (c) Copyright 1999-2003 Avid Technology, Inc. . All rights reserved.
//
//***************************************************************************************

/****************************************************************************************
THIS CODE IS PUBLISHED AS A SAMPLE ONLY AND IS PROVIDED "AS IS".
IN NO EVENT SHALL SOFTIMAGE, AVID TECHNOLOGY, INC. AND/OR THEIR RESPECTIVE
SUPPLIERS BE LIABLE FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR ANY
DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN
CONNECTION WITH THE USE OR PERFORMANCE OF THIS CODE .

COPYRIGHT NOTICE. Copyright © 1999-2003 Avid Technology Inc. . All rights reserved. 

SOFTIMAGE is a registered trademark of Avid Technology Inc. or its subsidiaries 
or divisions. Windows NT is a registered trademark of Microsoft Corp. All other
trademarks contained herein are the property of their respective owners. 
****************************************************************************************/

#ifndef XSI_RTSHADERS_H
#define XSI_RTSHADERS_H

#include <GL/gl.h>
#ifdef linux
#undef GL_VERSION_1_2
#undef GL_VERSION_1_3
#undef __glext_h_
#undef GL_ARB_multitexture
#undef GL_ATI_vertex_array_object
#endif
#include <glh/glext.h>
#include <math.h>
#include <float.h>
#include <stdlib.h>
#include <SIBCArray.h>
#include <string.h>

#ifdef linux
#define strnicmp strncasecmp
#define stricmp  strcasecmp
#endif

//***************************************************************************************
// Versioning
//***************************************************************************************

#define XSI_RTSHADER_VERSION	200		//! Realtime shader protocol version 2.00

//***************************************************************************************
// Application IDs
//***************************************************************************************

#define	APPID_XSI						0x00000000	//! XSI
#define	APPID_XSI_OGL					0x00000001	//! XSI running in OpenGL (always the case)
#define	APPID_XSIVIEWER					0x00000100	//! XSIViewer
#define	APPID_XSIVIEWER_OGL				0x00000101	//! XSIViewer running the OpenGL driver
#define	APPID_XSIVIEWER_D3D				0x00000102	//! XSIViewer running the DirectX driver
#define	APPID_XSIVIEWER_XBOX			0x00000103	//! XSIViewer running on XBOX
#define	APPID_XSIVIEWER_PS2GL			0x00000104	//! XSIViewer running on PS2 GL (not supported)

//***************************************************************************************
// Shader flags
//***************************************************************************************

#define RTS_SHADER_FLAG_NONE			0x00000000	//! no flag
#define RTS_SHADER_FLAG_PARAM_CHANGE	0x00000001	//! parameter changed
#define RTS_SHADER_FLAG_FIRST			0x00000002  //! shader is the first to be executed for this material
#define RTS_SHADER_FLAG_LAST			0x00000004	//! shader is the last to be executed for this material
#define RTS_SHADER_FLAG_TREE_CHANGE		0x00000008	//! shader tree has physically changed

//***************************************************************************************
// Object flags
//***************************************************************************************

#define RTS_OBJECT_FLAG_NONE			0x00000000	//! no flag
#define RTS_OBJECT_FLAG_TOPO			0x00000001	//! topological change occured
#define RTS_OBJECT_FLAG_DEFORM			0x00000002	//! deform occured


//***************************************************************************************
// Lights and camera enums
//***************************************************************************************

enum eXSI_RTS_LightType
{
	RTS_LIGHT_POINT,		//! A point light
	RTS_LIGHT_INFINITE,		//! An infinite (or parallel) light
	RTS_LIGHT_SPOT			//! A spot light
};

enum eXSI_RTS_CameraType
{
	RTS_ORTHOGRAPHIC,		//! Orthographic
	RTS_PERSPECTIVE			//! Perspective
};

enum eXSI_RTS_RequirementType
{
	RTS_REQUIRE_ZSORT								=	0,				//! The shader requires the object to be zsorted
	RTS_REQUIRE_RESERVED							=	1,				//! Obsolete and no longer supported
	RTS_REQUIRE_OBJECTHANDLE						=	2,				//! The shader requires the object handle 
	RTS_REQUIRE_MATERIALHANDLE						=	3,				//! The shader requires the material handle 
	RTS_REQUIRE_UNUSED								=   0xffffffff		//! Unused
};

enum eXSI_RTS_TriangleListType
{
	RTS_TL_SINGLE_INDEX_ARRAY,			//! arrays define positions, normals, UV and colors and a single index is used
	RTS_TL_BYREF,						//! positions, normals, UVs and colors are accessed by pointers, everything is in float
	RTS_TL_BYREF_POSD					//! positions, normals, UVs and colors are accessed by pointers, everything is in float, except position which is double
};

//***************************************************************************************
// Dynamic load library stuff
//***************************************************************************************

#if !defined(_GNU) 

// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the RTShader_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// RTSHADER_API functions as being imported from a DLL, wheras this DLL sees symbols
// defined with this macro as being exported.

#ifdef SHADERS_EXPORTS
	#ifndef _XBOX
		#define RTSHADER_API __declspec(dllexport)
	#else
		#define RTSHADER_API
	#endif
#else
	#ifdef XSI_STATIC_LINK
		#define RTSHADER_API
	#else
		#define RTSHADER_API __declspec(dllimport)
	#endif
#endif

#else

// Although Linux figures the shared library import and exports, it does not define any of the
// standard windows types, such as LPVOID, BOOL, etc. Since this file uses them, and files that
// use this file use them aswell, they are defined here.
#define RTSHADER_API

#endif

//***************************************************************************************
// Types
//***************************************************************************************

typedef size_t RTSContext;									//! 32/64 bits identifier for RTS context.

// All structures that are used to construct a PPG must be compiled without padding 
// between struct fields.  This is because a pointer to the PPG struct will be initialized 
// directly by type casting a void pointer as in the following:
//
//   MyPPG * my_ppg = (MyPPG*) in_pVoid;
//
// The pragma is not necessary under a 32 bit architecture since a PPG will only contain
// floats (4 bytes), longs (4 bytes) and pointers (4 bytes).  Therefore structs will 
// never contain padding.  But, in a 64 bit architecture where pointers are 8 bytes, 
// if we don't force the compiler to remove the automatic padding, the struct may contain
// padding.  This padding is not considered when initializing the void pointer (in_pVoid).
// Therefore, the direct type casting would not work.
#pragma pack(push, 1)

typedef struct tagXSI_RTS_Vector2
{
	float	m_x;											//! The X component
	float	m_y;											//! The Y component
} XSI_RTS_Vector2;


typedef struct tagXSI_RTS_Vector3d
{
	double	m_x;											//! The X component
	double	m_y;											//! The Y component
	double	m_z;											//! The Z component
} XSI_RTS_Vector3d;

typedef struct tagXSI_RTS_Vector3
{
	float	m_x;											//! The X component
	float	m_y;											//! The Y component
	float	m_z;											//! The Z component
} XSI_RTS_Vector3;


typedef struct tagXSI_RTS_Vector4
{
	float	m_x;											//! The X component
	float	m_y;											//! The Y component
	float	m_z;											//! The Z component
	float	m_w;											//! The W component
} XSI_RTS_Vector4;

typedef struct tagXSI_RTS_ColorRGB
{
	unsigned char	m_red;									//! The red component
	unsigned char	m_green;								//! The green component
	unsigned char	m_blue;									//! The blue component
} XSI_RTS_ColorRGB;


typedef struct tagXSI_RTS_ColorRGBf
{
	float			m_red;									//! The red component
	float			m_green;								//! The green component
	float			m_blue;									//! The blue component
} XSI_RTS_ColorRGBf;

typedef struct tagXSI_RTS_ColorRGBAf
{
	float			m_red;									//! The red component
	float			m_green;								//! The green component
	float			m_blue;									//! The blue component
	float			m_alpha;								//! The alpha component
} XSI_RTS_ColorRGBAf;

typedef struct tagXSI_RTS_ColorRGBA
{
	unsigned char	m_red;									//! The red component
	unsigned char	m_green;								//! The green component
	unsigned char	m_blue;									//! The blue component
	unsigned char	m_alpha;								//! The alpha component
} XSI_RTS_ColorRGBA;

typedef struct tagXSI_RTS_ColorARGB
{
	unsigned char	m_alpha;								//! The alpha component
	unsigned char	m_red;									//! The red component
	unsigned char	m_green;								//! The green component
	unsigned char	m_blue;									//! The blue component
} XSI_RTS_ColorARGB;

typedef struct tagXSI_RTS_Texture
{
	char			*m_pchPath;								//! The path of the image for this texture
	ULONG	m_lWidth;								//! The width of the image
	ULONG	m_lHeight;								//! The height of the image
	ULONG	m_lNbChannels;							//! The number of channels
	ULONG	m_lPixelType;							//! The type of pixel
	ULONG	m_lBufferSize;							//! The size of the pixel buffer
	unsigned char	*m_pBuffer;								//! The pixel buffer
	ULONG	m_lDirty;								//! The texture is dirty and should be refreshed
} XSI_RTS_Texture;

#pragma pack(pop)

//***************************************************************************************
// OpenGL extensions wrappers
//***************************************************************************************

//! XGS GL extensions structure
/*! The is the structure for keeping GL extension support status
*/
typedef struct tagXGSExtensionsGL
{
	int	 version;
	int  size;

	bool _GL_ARB_multitexture ;
	bool _GL_ARB_transpose_matrix ;
	bool _GL_ARB_multisample ;
	bool _GL_ARB_texture_env_add ;
	bool _GL_ARB_texture_cube_map ;
	bool _GL_ARB_texture_compression ;
	bool _GL_ARB_texture_border_clamp ;
	bool _GL_ARB_point_parameters ;
	bool _GL_ARB_vertex_blend ;
	bool _GL_ARB_matrix_palette ;
	bool _GL_ARB_texture_env_combine ;
	bool _GL_ARB_texture_env_crossbar ;
	bool _GL_ARB_texture_env_dot3 ;
	bool _GL_ARB_texture_mirrored_repeat ;
	bool _GL_ARB_depth_texture ;
	bool _GL_ARB_shadow ;
	bool _GL_ARB_shadow_ambient ;
	bool _GL_ARB_window_pos ;
	bool _GL_ARB_vertex_program ;
	bool _GL_ARB_fragment_program ;
	bool _GL_EXT_abgr ;
	bool _GL_EXT_blend_color ;
	bool _GL_EXT_polygon_offset ;
	bool _GL_EXT_texture ;
	bool _GL_EXT_texture3D ;
	bool _GL_SGIS_texture_filter4 ;
	bool _GL_EXT_subtexture ;
	bool _GL_EXT_copy_texture ;
	bool _GL_EXT_histogram ;
	bool _GL_EXT_convolution ;
	bool _GL_SGI_color_matrix ;
	bool _GL_SGI_color_table ;
	bool _GL_SGIS_pixel_texture ;
	bool _GL_SGIX_pixel_texture ;
	bool _GL_SGIS_texture4D ;
	bool _GL_SGI_texture_color_table ;
	bool _GL_EXT_cmyka ;
	bool _GL_EXT_texture_object ;
	bool _GL_SGIS_detail_texture ;
	bool _GL_SGIS_sharpen_texture ;
	bool _GL_EXT_packed_pixels ;
	bool _GL_SGIS_texture_lod ;
	bool _GL_SGIS_multisample ;
	bool _GL_EXT_rescale_normal ;
	bool _GL_EXT_vertex_array ;
	bool _GL_EXT_misc_attribute ;
	bool _GL_SGIS_generate_mipmap ;
	bool _GL_SGIX_clipmap ;
	bool _GL_SGIX_shadow ;
	bool _GL_SGIS_texture_edge_clamp ;
	bool _GL_SGIS_texture_border_clamp ;
	bool _GL_EXT_blend_minmax ;
	bool _GL_EXT_blend_subtract ;
	bool _GL_EXT_blend_logic_op ;
	bool _GL_SGIX_interlace ;
	bool _GL_SGIS_texture_select ;
	bool _GL_SGIX_sprite ;
	bool _GL_SGIX_texture_multi_buffer ;
	bool _GL_EXT_point_parameters ;
	bool _GL_SGIX_instruments ;
	bool _GL_SGIX_texture_scale_bias ;
	bool _GL_SGIX_framezoom ;
	bool _GL_SGIX_tag_sample_buffer ;
	bool _GL_SGIX_reference_plane ;
	bool _GL_SGIX_flush_raster ;
	bool _GL_SGIX_depth_texture ;
	bool _GL_SGIS_fog_function ;
	bool _GL_SGIX_fog_offset ;
	bool _GL_HP_image_transform ;
	bool _GL_HP_convolution_border_modes ;
	bool _GL_SGIX_texture_add_env ;
	bool _GL_EXT_color_subtable ;
	bool _GL_PGI_vertex_hints ;
	bool _GL_PGI_misc_hints ;
	bool _GL_EXT_paletted_texture ;
	bool _GL_EXT_clip_volume_hint ;
	bool _GL_SGIX_list_priority ;
	bool _GL_SGIX_ir_instrument1 ;
	bool _GL_SGIX_texture_lod_bias ;
	bool _GL_SGIX_shadow_ambient ;
	bool _GL_EXT_index_texture ;
	bool _GL_EXT_index_material ;
	bool _GL_EXT_index_func ;
	bool _GL_EXT_index_array_formats ;
	bool _GL_EXT_compiled_vertex_array ;
	bool _GL_EXT_cull_vertex ;
	bool _GL_SGIX_ycrcb ;
	bool _GL_EXT_fragment_lighting ;
	bool _GL_IBM_rasterpos_clip ;
	bool _GL_HP_texture_lighting ;
	bool _GL_EXT_draw_range_elements ;
	bool _GL_WIN_phong_shading ;
	bool _GL_WIN_specular_fog ;
	bool _GL_EXT_light_texture ;
	bool _GL_SGIX_blend_alpha_minmax ;
	bool _GL_EXT_scene_marker ;
	bool _GL_SGIX_pixel_texture_bits ;
	bool _GL_EXT_bgra ;
	bool _GL_SGIX_async ;
	bool _GL_SGIX_async_pixel ;
	bool _GL_SGIX_async_histogram ;
	bool _GL_INTEL_texture_scissor ;
	bool _GL_INTEL_parallel_arrays ;
	bool _GL_HP_occlusion_test ;
	bool _GL_EXT_pixel_transform ;
	bool _GL_EXT_pixel_transform_color_table ;
	bool _GL_EXT_shared_texture_palette ;
	bool _GL_EXT_separate_specular_color ;
	bool _GL_EXT_secondary_color ;
	bool _GL_EXT_texture_env ;
	bool _GL_EXT_texture_perturb_normal ;
	bool _GL_EXT_multi_draw_arrays ;
	bool _GL_EXT_fog_coord ;
	bool _GL_REND_screen_coordinates ;
	bool _GL_EXT_coordinate_frame ;
	bool _GL_EXT_texture_env_combine ;
	bool _GL_APPLE_specular_vector ;
	bool _GL_APPLE_transform_hint ;
	bool _GL_SUNX_constant_data ;
	bool _GL_SUN_global_alpha ;
	bool _GL_SUN_triangle_list ;
	bool _GL_SUN_vertex ;
	bool _GL_EXT_blend_func_separate ;
	bool _GL_INGR_color_clamp ;
	bool _GL_INGR_interlace_read ;
	bool _GL_EXT_stencil_wrap ;
	bool _GL_EXT_422_pixels ;
	bool _GL_NV_texgen_reflection ;
	bool _GL_SGIX_texture_range ;
	bool _GL_SUN_convolution_border_modes ;
	bool _GL_EXT_texture_env_add ;
	bool _GL_EXT_texture_lod_bias ;
	bool _GL_EXT_texture_filter_anisotropic ;
	bool _GL_EXT_vertex_weighting ;
	bool _GL_NV_light_max_exponent ;
	bool _GL_NV_vertex_array_range ;
	bool _GL_NV_register_combiners ;
	bool _GL_NV_fog_distance ;
	bool _GL_NV_texgen_emboss ;
	bool _GL_NV_blend_square ;
	bool _GL_NV_texture_env_combine4 ;
	bool _GL_MESA_resize_buffers ;
	bool _GL_MESA_window_pos ;
	bool _GL_EXT_texture_compression_s3tc ;
	bool _GL_IBM_cull_vertex ;
	bool _GL_IBM_multimode_draw_arrays ;
	bool _GL_IBM_vertex_array_lists ;
	bool _GL_3DFX_texture_compression_FXT1 ;
	bool _GL_3DFX_multisample ;
	bool _GL_3DFX_tbuffer ;
	bool _GL_SGIX_vertex_preclip ;
	bool _GL_SGIX_resample ;
	bool _GL_SGIS_texture_color_mask ;
	bool _GL_EXT_texture_env_dot3 ;
	bool _GL_ATI_texture_mirror_once ;
	bool _GL_NV_fence ;
	bool _GL_IBM_static_data ;
	bool _GL_IBM_texture_mirrored_repeat ;
	bool _GL_NV_evaluators ;
	bool _GL_NV_packed_depth_stencil ;
	bool _GL_NV_register_combiners2 ;
	bool _GL_NV_texture_compression_vtc ;
	bool _GL_NV_texture_rectangle ;
	bool _GL_NV_texture_shader ;
	bool _GL_NV_texture_shader2 ;
	bool _GL_NV_vertex_array_range2 ;
	bool _GL_NV_vertex_program ;
	bool _GL_SGIX_texture_coordinate_clamp ;
	bool _GL_OML_interlace ;
	bool _GL_OML_subsample ;
	bool _GL_OML_resample ;
	bool _GL_NV_copy_depth_to_color ;
	bool _GL_ATI_envmap_bumpmap ;
	bool _GL_ATI_fragment_shader ;
	bool _GL_ATI_pn_triangles ;
	bool _GL_ATI_vertex_array_object ;
	bool _GL_EXT_vertex_shader ;
	bool _GL_ATI_vertex_streams ;
	bool _GL_ATI_element_array ;
	bool _GL_SUN_mesh_array ;
	bool _GL_SUN_slice_accum ;
	bool _GL_NV_multisample_filter_hint ;
	bool _GL_NV_depth_clamp ;
	bool _GL_NV_occlusion_query ;
	bool _GL_NV_point_sprite ;
	bool _GL_NV_texture_shader3 ;
	bool _GL_NV_vertex_program1_1 ;
	bool _GL_EXT_shadow_funcs ;
	bool _GL_EXT_stencil_two_side ;
	bool _GL_ATI_text_fragment_shader ;
	bool _GL_APPLE_client_storage ;
	bool _GL_APPLE_element_array ;
	bool _GL_APPLE_fence ;
	bool _GL_APPLE_vertex_array_object ;
	bool _GL_APPLE_vertex_array_range ;
	bool _GL_APPLE_ycbcr_422 ;
	bool _GL_ARB_vertex_buffer_object ;
	bool _GL_S3_s3tc ;
	bool _GL_ATI_draw_buffers ;
	bool _GL_ATI_texture_env_combine3 ;
	bool _GL_ATI_texture_float ;
	bool _GL_NV_float_buffer ;
	bool _GL_NV_fragment_program ;
	bool _GL_NV_half_float ;
	bool _GL_NV_pixel_data_range ;
	bool _GL_NV_primitive_restart ;
	bool _GL_NV_texture_expand_normal ;
	bool _GL_NV_vertex_program2 ;
	bool _GL_ATI_map_object_buffer ;
	bool _GL_ATI_separate_stencil ;
	bool _GL_ATI_vertex_attrib_array_object ;

} XGSExtensionsGL;

//! XGS GL extensions functions
/*! The is the structure for keeping GL extension functions
*/
typedef struct tagXGSGLExtensionsFuncs
{	
	int	 version;
	int  size;

	PFNGLBLENDCOLORPROC													glBlendColor;
	PFNGLBLENDEQUATIONPROC												glBlendEquation;
	PFNGLDRAWRANGEELEMENTSPROC											glDrawRangeElements;
	PFNGLCOLORTABLEPROC													glColorTable;
	PFNGLCOLORTABLEPARAMETERFVPROC										glColorTableParameterfv;
	PFNGLCOLORTABLEPARAMETERIVPROC										glColorTableParameteriv;
	PFNGLCOPYCOLORTABLEPROC												glCopyColorTable;
	PFNGLGETCOLORTABLEPROC												glGetColorTable;
	PFNGLGETCOLORTABLEPARAMETERFVPROC									glGetColorTableParameterfv;
	PFNGLGETCOLORTABLEPARAMETERIVPROC									glGetColorTableParameteriv;
	PFNGLCOLORSUBTABLEPROC												glColorSubTable;
	PFNGLCOPYCOLORSUBTABLEPROC											glCopyColorSubTable;
	PFNGLCONVOLUTIONFILTER1DPROC										glConvolutionFilter1D;
	PFNGLCONVOLUTIONFILTER2DPROC										glConvolutionFilter2D;
	PFNGLCONVOLUTIONPARAMETERFPROC										glConvolutionParameterf;
	PFNGLCONVOLUTIONPARAMETERFVPROC										glConvolutionParameterfv;
	PFNGLCONVOLUTIONPARAMETERIPROC										glConvolutionParameteri;
	PFNGLCONVOLUTIONPARAMETERIVPROC										glConvolutionParameteriv;
	PFNGLCOPYCONVOLUTIONFILTER1DPROC									glCopyConvolutionFilter1D;
	PFNGLCOPYCONVOLUTIONFILTER2DPROC									glCopyConvolutionFilter2D;
	PFNGLGETCONVOLUTIONFILTERPROC										glGetConvolutionFilter;
	PFNGLGETCONVOLUTIONPARAMETERFVPROC									glGetConvolutionParameterfv;
	PFNGLGETCONVOLUTIONPARAMETERIVPROC									glGetConvolutionParameteriv;
	PFNGLGETSEPARABLEFILTERPROC											glGetSeparableFilter;
	PFNGLSEPARABLEFILTER2DPROC											glSeparableFilter2D;
	PFNGLGETHISTOGRAMPROC												glGetHistogram;
	PFNGLGETHISTOGRAMPARAMETERFVPROC									glGetHistogramParameterfv;
	PFNGLGETHISTOGRAMPARAMETERIVPROC									glGetHistogramParameteriv;
	PFNGLGETMINMAXPROC													glGetMinmax;
	PFNGLGETMINMAXPARAMETERFVPROC										glGetMinmaxParameterfv;
	PFNGLGETMINMAXPARAMETERIVPROC										glGetMinmaxParameteriv;
	PFNGLHISTOGRAMPROC													glHistogram;
	PFNGLMINMAXPROC														glMinmax;
	PFNGLRESETHISTOGRAMPROC												glResetHistogram;
	PFNGLRESETMINMAXPROC												glResetMinmax;
	PFNGLTEXIMAGE3DPROC													glTexImage3D;
	PFNGLTEXSUBIMAGE3DPROC												glTexSubImage3D;
	PFNGLCOPYTEXSUBIMAGE3DPROC											glCopyTexSubImage3D;
																		
	PFNGLACTIVETEXTUREPROC												glActiveTexture;
	PFNGLCLIENTACTIVETEXTUREPROC										glClientActiveTexture;
	PFNGLMULTITEXCOORD1DPROC											glMultiTexCoord1d;
	PFNGLMULTITEXCOORD1DVPROC											glMultiTexCoord1dv;
	PFNGLMULTITEXCOORD1FPROC											glMultiTexCoord1f;
	PFNGLMULTITEXCOORD1FVPROC											glMultiTexCoord1fv;
	PFNGLMULTITEXCOORD1IPROC											glMultiTexCoord1i;
	PFNGLMULTITEXCOORD1IVPROC											glMultiTexCoord1iv;
	PFNGLMULTITEXCOORD1SPROC											glMultiTexCoord1s;
	PFNGLMULTITEXCOORD1SVPROC											glMultiTexCoord1sv;
	PFNGLMULTITEXCOORD2DPROC											glMultiTexCoord2d;
	PFNGLMULTITEXCOORD2DVPROC											glMultiTexCoord2dv;
	PFNGLMULTITEXCOORD2FPROC											glMultiTexCoord2f;
	PFNGLMULTITEXCOORD2FVPROC											glMultiTexCoord2fv;
	PFNGLMULTITEXCOORD2IPROC											glMultiTexCoord2i;
	PFNGLMULTITEXCOORD2IVPROC											glMultiTexCoord2iv;
	PFNGLMULTITEXCOORD2SPROC											glMultiTexCoord2s;
	PFNGLMULTITEXCOORD2SVPROC											glMultiTexCoord2sv;
	PFNGLMULTITEXCOORD3DPROC											glMultiTexCoord3d;
	PFNGLMULTITEXCOORD3DVPROC											glMultiTexCoord3dv;
	PFNGLMULTITEXCOORD3FPROC											glMultiTexCoord3f;
	PFNGLMULTITEXCOORD3FVPROC											glMultiTexCoord3fv;
	PFNGLMULTITEXCOORD3IPROC											glMultiTexCoord3i;
	PFNGLMULTITEXCOORD3IVPROC											glMultiTexCoord3iv;
	PFNGLMULTITEXCOORD3SPROC											glMultiTexCoord3s;
	PFNGLMULTITEXCOORD3SVPROC											glMultiTexCoord3sv;
	PFNGLMULTITEXCOORD4DPROC											glMultiTexCoord4d;
	PFNGLMULTITEXCOORD4DVPROC											glMultiTexCoord4dv;
	PFNGLMULTITEXCOORD4FPROC											glMultiTexCoord4f;
	PFNGLMULTITEXCOORD4FVPROC											glMultiTexCoord4fv;
	PFNGLMULTITEXCOORD4IPROC											glMultiTexCoord4i;
	PFNGLMULTITEXCOORD4IVPROC											glMultiTexCoord4iv;
	PFNGLMULTITEXCOORD4SPROC											glMultiTexCoord4s;
	PFNGLMULTITEXCOORD4SVPROC											glMultiTexCoord4sv;
	PFNGLLOADTRANSPOSEMATRIXFPROC										glLoadTransposeMatrixf;
	PFNGLLOADTRANSPOSEMATRIXDPROC										glLoadTransposeMatrixd;
	PFNGLMULTTRANSPOSEMATRIXFPROC										glMultTransposeMatrixf;
	PFNGLMULTTRANSPOSEMATRIXDPROC										glMultTransposeMatrixd;
	PFNGLSAMPLECOVERAGEPROC												glSampleCoverage;
	PFNGLCOMPRESSEDTEXIMAGE3DPROC										glCompressedTexImage3D;
	PFNGLCOMPRESSEDTEXIMAGE2DPROC										glCompressedTexImage2D;
	PFNGLCOMPRESSEDTEXIMAGE1DPROC										glCompressedTexImage1D;
	PFNGLCOMPRESSEDTEXSUBIMAGE3DPROC									glCompressedTexSubImage3D;
	PFNGLCOMPRESSEDTEXSUBIMAGE2DPROC									glCompressedTexSubImage2D;
	PFNGLCOMPRESSEDTEXSUBIMAGE1DPROC									glCompressedTexSubImage1D;
	PFNGLGETCOMPRESSEDTEXIMAGEPROC										glGetCompressedTexImage;
																		
	PFNGLACTIVETEXTUREPROC												glActiveTextureARB;
	PFNGLCLIENTACTIVETEXTUREARBPROC										glClientActiveTextureARB;				
	PFNGLMULTITEXCOORD1DARBPROC											glMultiTexCoord1dARB;			
	PFNGLMULTITEXCOORD1DVARBPROC										glMultiTexCoord1dvARB;				
	PFNGLMULTITEXCOORD1FARBPROC											glMultiTexCoord1fARB;			
	PFNGLMULTITEXCOORD1FVARBPROC										glMultiTexCoord1fvARB;				
	PFNGLMULTITEXCOORD1IARBPROC											glMultiTexCoord1iARB;			
	PFNGLMULTITEXCOORD1IVARBPROC										glMultiTexCoord1ivARB;				
	PFNGLMULTITEXCOORD1SARBPROC											glMultiTexCoord1sARB;			
	PFNGLMULTITEXCOORD1SVARBPROC										glMultiTexCoord1svARB;				
	PFNGLMULTITEXCOORD2DARBPROC											glMultiTexCoord2dARB;			
	PFNGLMULTITEXCOORD2DVARBPROC										glMultiTexCoord2dvARB;				
	PFNGLMULTITEXCOORD2FARBPROC											glMultiTexCoord2fARB;			
	PFNGLMULTITEXCOORD2FVARBPROC										glMultiTexCoord2fvARB;				
	PFNGLMULTITEXCOORD2IARBPROC											glMultiTexCoord2iARB;			
	PFNGLMULTITEXCOORD2IVARBPROC										glMultiTexCoord2ivARB;				
	PFNGLMULTITEXCOORD2SARBPROC											glMultiTexCoord2sARB;			
	PFNGLMULTITEXCOORD2SVARBPROC										glMultiTexCoord2svARB;				
	PFNGLMULTITEXCOORD3DARBPROC											glMultiTexCoord3dARB;			
	PFNGLMULTITEXCOORD3DVARBPROC										glMultiTexCoord3dvARB;				
	PFNGLMULTITEXCOORD3FARBPROC											glMultiTexCoord3fARB;			
	PFNGLMULTITEXCOORD3FVARBPROC										glMultiTexCoord3fvARB;				
	PFNGLMULTITEXCOORD3IARBPROC											glMultiTexCoord3iARB;			
	PFNGLMULTITEXCOORD3IVARBPROC										glMultiTexCoord3ivARB;				
	PFNGLMULTITEXCOORD3SARBPROC											glMultiTexCoord3sARB;			
	PFNGLMULTITEXCOORD3SVARBPROC										glMultiTexCoord3svARB;				
	PFNGLMULTITEXCOORD4DARBPROC											glMultiTexCoord4dARB;			
	PFNGLMULTITEXCOORD4DVARBPROC										glMultiTexCoord4dvARB;				
	PFNGLMULTITEXCOORD4FARBPROC											glMultiTexCoord4fARB;			
	PFNGLMULTITEXCOORD4FVARBPROC										glMultiTexCoord4fvARB;				
	PFNGLMULTITEXCOORD4IARBPROC											glMultiTexCoord4iARB;			
	PFNGLMULTITEXCOORD4IVARBPROC										glMultiTexCoord4ivARB;				
	PFNGLMULTITEXCOORD4SARBPROC											glMultiTexCoord4sARB;			
	PFNGLMULTITEXCOORD4SVARBPROC										glMultiTexCoord4svARB;				
																		
	PFNGLLOADTRANSPOSEMATRIXFARBPROC									glLoadTransposeMatrixfARB;
	PFNGLLOADTRANSPOSEMATRIXDARBPROC									glLoadTransposeMatrixdARB;
	PFNGLMULTTRANSPOSEMATRIXFARBPROC									glMultTransposeMatrixfARB;
	PFNGLMULTTRANSPOSEMATRIXDARBPROC									glMultTransposeMatrixdARB;
																		
	PFNGLSAMPLECOVERAGEARBPROC											glSampleCoverageARB;
																		
	PFNGLCOMPRESSEDTEXIMAGE3DARBPROC									glCompressedTexImage3DARB;
	PFNGLCOMPRESSEDTEXIMAGE2DARBPROC									glCompressedTexImage2DARB;
	PFNGLCOMPRESSEDTEXIMAGE1DARBPROC									glCompressedTexImage1DARB;
	PFNGLCOMPRESSEDTEXSUBIMAGE3DARBPROC									glCompressedTexSubImage3DARB;
	PFNGLCOMPRESSEDTEXSUBIMAGE2DARBPROC									glCompressedTexSubImage2DARB;
	PFNGLCOMPRESSEDTEXSUBIMAGE1DARBPROC									glCompressedTexSubImage1DARB;
	PFNGLGETCOMPRESSEDTEXIMAGEARBPROC									glGetCompressedTexImageARB;
																		
	PFNGLPOINTPARAMETERFARBPROC											glPointParameterfARB;
	PFNGLPOINTPARAMETERFVARBPROC										glPointParameterfvARB;
																		
	PFNGLWEIGHTBVARBPROC												glWeightbvARB;
	PFNGLWEIGHTSVARBPROC												glWeightsvARB;
	PFNGLWEIGHTIVARBPROC												glWeightivARB;
	PFNGLWEIGHTFVARBPROC												glWeightfvARB;
	PFNGLWEIGHTDVARBPROC												glWeightdvARB;
	PFNGLWEIGHTUBVARBPROC												glWeightubvARB;
	PFNGLWEIGHTUSVARBPROC												glWeightusvARB;
	PFNGLWEIGHTUIVARBPROC												glWeightuivARB;
	PFNGLWEIGHTPOINTERARBPROC											glWeightPointerARB;	
	PFNGLVERTEXBLENDARBPROC												glVertexBlendARB;
																		
																		
	PFNGLCURRENTPALETTEMATRIXARBPROC									glCurrentPaletteMatrixARB;
	PFNGLMATRIXINDEXUBVARBPROC											glMatrixIndexubvARB;
	PFNGLMATRIXINDEXUSVARBPROC											glMatrixIndexusvARB;
	PFNGLMATRIXINDEXUIVARBPROC											glMatrixIndexuivARB;
	PFNGLMATRIXINDEXPOINTERARBPROC										glMatrixIndexPointerARB;
																		
	PFNGLWINDOWPOS2DARBPROC												glWindowPos2dARB;
	PFNGLWINDOWPOS2DVARBPROC											glWindowPos2dvARB;
	PFNGLWINDOWPOS2FARBPROC												glWindowPos2fARB;
	PFNGLWINDOWPOS2FVARBPROC											glWindowPos2fvARB;
	PFNGLWINDOWPOS2IARBPROC												glWindowPos2iARB;
	PFNGLWINDOWPOS2IVARBPROC											glWindowPos2ivARB;
	PFNGLWINDOWPOS2SARBPROC												glWindowPos2sARB;
	PFNGLWINDOWPOS2SVARBPROC											glWindowPos2svARB;
	PFNGLWINDOWPOS3DARBPROC												glWindowPos3dARB;
	PFNGLWINDOWPOS3DVARBPROC											glWindowPos3dvARB;
	PFNGLWINDOWPOS3FARBPROC												glWindowPos3fARB;
	PFNGLWINDOWPOS3FVARBPROC											glWindowPos3fvARB;
	PFNGLWINDOWPOS3IARBPROC												glWindowPos3iARB;
	PFNGLWINDOWPOS3IVARBPROC											glWindowPos3ivARB;
	PFNGLWINDOWPOS3SARBPROC												glWindowPos3sARB;
	PFNGLWINDOWPOS3SVARBPROC											glWindowPos3svARB;
																		
	PFNGLBLENDCOLOREXTPROC												glBlendColorEXT;
																		
	PFNGLPOLYGONOFFSETEXTPROC											glPolygonOffsetEXT;
																		
	PFNGLTEXIMAGE3DEXTPROC												glTexImage3DEXT;
	PFNGLTEXSUBIMAGE3DEXTPROC											glTexSubImage3DEXT;	
																		
	PFNGLGETTEXFILTERFUNCSGISPROC										glGetTexFilterFuncSGIS;		
	PFNGLTEXFILTERFUNCSGISPROC											glTexFilterFuncSGIS;	
																		
	PFNGLTEXSUBIMAGE1DEXTPROC											glTexSubImage1DEXT;	
	PFNGLTEXSUBIMAGE2DEXTPROC											glTexSubImage2DEXT;	
																		
	PFNGLCOPYTEXIMAGE1DEXTPROC											glCopyTexImage1DEXT;
	PFNGLCOPYTEXIMAGE2DEXTPROC											glCopyTexImage2DEXT;
	PFNGLCOPYTEXSUBIMAGE1DEXTPROC										glCopyTexSubImage1DEXT;	
	PFNGLCOPYTEXSUBIMAGE2DEXTPROC										glCopyTexSubImage2DEXT;	
	PFNGLCOPYTEXSUBIMAGE3DEXTPROC										glCopyTexSubImage3DEXT;	
																		
	PFNGLGETHISTOGRAMEXTPROC											glGetHistogramEXT;
	PFNGLGETHISTOGRAMPARAMETERFVEXTPROC									glGetHistogramParameterfvEXT;	
	PFNGLGETHISTOGRAMPARAMETERIVEXTPROC									glGetHistogramParameterivEXT;	
	PFNGLGETMINMAXEXTPROC												glGetMinmaxEXT;
	PFNGLGETMINMAXPARAMETERFVEXTPROC									glGetMinmaxParameterfvEXT;	
	PFNGLGETMINMAXPARAMETERIVEXTPROC									glGetMinmaxParameterivEXT;	
	PFNGLHISTOGRAMEXTPROC												glHistogramEXT;
	PFNGLMINMAXEXTPROC													glMinmaxEXT;
	PFNGLRESETHISTOGRAMEXTPROC											glResetHistogramEXT;
	PFNGLRESETMINMAXEXTPROC												glResetMinmaxEXT;
																		
	PFNGLCONVOLUTIONFILTER1DEXTPROC										glConvolutionFilter1DEXT;
	PFNGLCONVOLUTIONFILTER2DEXTPROC										glConvolutionFilter2DEXT;
	PFNGLCONVOLUTIONPARAMETERFEXTPROC									glConvolutionParameterfEXT;	
	PFNGLCONVOLUTIONPARAMETERFVEXTPROC									glConvolutionParameterfvEXT;	
	PFNGLCONVOLUTIONPARAMETERIEXTPROC									glConvolutionParameteriEXT;	
	PFNGLCONVOLUTIONPARAMETERIVEXTPROC									glConvolutionParameterivEXT;	
	PFNGLCOPYCONVOLUTIONFILTER1DEXTPROC									glCopyConvolutionFilter1DEXT;	
	PFNGLCOPYCONVOLUTIONFILTER2DEXTPROC									glCopyConvolutionFilter2DEXT;	
	PFNGLGETCONVOLUTIONFILTEREXTPROC									glGetConvolutionFilterEXT;	
	PFNGLGETCONVOLUTIONPARAMETERFVEXTPROC								glGetConvolutionParameterfvEXT;		
	PFNGLGETCONVOLUTIONPARAMETERIVEXTPROC								glGetConvolutionParameterivEXT;		
	PFNGLGETSEPARABLEFILTEREXTPROC										glGetSeparableFilterEXT;
	PFNGLSEPARABLEFILTER2DEXTPROC										glSeparableFilter2DEXT;
																		
	PFNGLCOLORTABLESGIPROC												glColorTableSGI;
	PFNGLCOLORTABLEPARAMETERFVSGIPROC									glColorTableParameterfvSGI;
	PFNGLCOLORTABLEPARAMETERIVSGIPROC									glColorTableParameterivSGI;
	PFNGLCOPYCOLORTABLESGIPROC											glCopyColorTableSGI;
	PFNGLGETCOLORTABLESGIPROC											glGetColorTableSGI;
	PFNGLGETCOLORTABLEPARAMETERFVSGIPROC								glGetColorTableParameterfvSGI;	
	PFNGLGETCOLORTABLEPARAMETERIVSGIPROC								glGetColorTableParameterivSGI;	
																		
	PFNGLPIXELTEXGENSGIXPROC											glPixelTexGenSGIX;
																		
	PFNGLPIXELTEXGENPARAMETERISGISPROC									glPixelTexGenParameteriSGIS;				
	PFNGLPIXELTEXGENPARAMETERIVSGISPROC									glPixelTexGenParameterivSGIS;				
	PFNGLPIXELTEXGENPARAMETERFSGISPROC									glPixelTexGenParameterfSGIS;				
	PFNGLPIXELTEXGENPARAMETERFVSGISPROC									glPixelTexGenParameterfvSGIS;				
	PFNGLGETPIXELTEXGENPARAMETERIVSGISPROC								glGetPixelTexGenParameterivSGIS;					
	PFNGLGETPIXELTEXGENPARAMETERFVSGISPROC								glGetPixelTexGenParameterfvSGIS;					
																		
	PFNGLTEXIMAGE4DSGISPROC												glTexImage4DSGIS;
	PFNGLTEXSUBIMAGE4DSGISPROC											glTexSubImage4DSGIS;
																		
	PFNGLARETEXTURESRESIDENTEXTPROC										glAreTexturesResidentEXT;			
	PFNGLBINDTEXTUREEXTPROC												glBindTextureEXT;	
	PFNGLDELETETEXTURESEXTPROC											glDeleteTexturesEXT;		
	PFNGLGENTEXTURESEXTPROC												glGenTexturesEXT;	
	PFNGLISTEXTUREEXTPROC												glIsTextureEXT;	
	PFNGLPRIORITIZETEXTURESEXTPROC										glPrioritizeTexturesEXT;			
																		
	PFNGLDETAILTEXFUNCSGISPROC											glDetailTexFuncSGIS;
	PFNGLGETDETAILTEXFUNCSGISPROC										glGetDetailTexFuncSGIS;
																		
	PFNGLSHARPENTEXFUNCSGISPROC											glSharpenTexFuncSGIS;
	PFNGLGETSHARPENTEXFUNCSGISPROC										glGetSharpenTexFuncSGIS;
																		
	PFNGLSAMPLEMASKSGISPROC												glSampleMaskSGIS;
	PFNGLSAMPLEPATTERNSGISPROC											glSamplePatternSGIS;
																		
	PFNGLARRAYELEMENTEXTPROC											glArrayElementEXT;		
	PFNGLCOLORPOINTEREXTPROC											glColorPointerEXT;		
	PFNGLDRAWARRAYSEXTPROC												glDrawArraysEXT;	
	PFNGLEDGEFLAGPOINTEREXTPROC											glEdgeFlagPointerEXT;		
	PFNGLGETPOINTERVEXTPROC												glGetPointervEXT;	
	PFNGLINDEXPOINTEREXTPROC											glIndexPointerEXT;		
	PFNGLNORMALPOINTEREXTPROC											glNormalPointerEXT;		
	PFNGLTEXCOORDPOINTEREXTPROC											glTexCoordPointerEXT;		
	PFNGLVERTEXPOINTEREXTPROC											glVertexPointerEXT;		
																		
	PFNGLBLENDEQUATIONEXTPROC											glBlendEquationEXT;
																		
	PFNGLSPRITEPARAMETERFSGIXPROC										glSpriteParameterfSGIX;			
	PFNGLSPRITEPARAMETERFVSGIXPROC										glSpriteParameterfvSGIX;			
	PFNGLSPRITEPARAMETERISGIXPROC										glSpriteParameteriSGIX;			
	PFNGLSPRITEPARAMETERIVSGIXPROC										glSpriteParameterivSGIX;			
																		
	PFNGLPOINTPARAMETERFEXTPROC											glPointParameterfEXT;		
	PFNGLPOINTPARAMETERFVEXTPROC										glPointParameterfvEXT;			
																		
	PFNGLPOINTPARAMETERFSGISPROC										glPointParameterfSGIS;			
	PFNGLPOINTPARAMETERFVSGISPROC										glPointParameterfvSGIS;			
																		
	PFNGLGETINSTRUMENTSSGIXPROC											glGetInstrumentsSGIX;		
	PFNGLINSTRUMENTSBUFFERSGIXPROC										glInstrumentsBufferSGIX;			
	PFNGLPOLLINSTRUMENTSSGIXPROC										glPollInstrumentsSGIX;			
	PFNGLREADINSTRUMENTSSGIXPROC										glReadInstrumentsSGIX;			
	PFNGLSTARTINSTRUMENTSSGIXPROC										glStartInstrumentsSGIX;			
	PFNGLSTOPINSTRUMENTSSGIXPROC										glStopInstrumentsSGIX;			
																		
	PFNGLFRAMEZOOMSGIXPROC												glFrameZoomSGIX;
																		
	PFNGLTAGSAMPLEBUFFERSGIXPROC										glTagSampleBufferSGIX;
																		
	PFNGLDEFORMATIONMAP3DSGIXPROC										glDeformationMap3dSGIX;			
	PFNGLDEFORMATIONMAP3FSGIXPROC										glDeformationMap3fSGIX;			
	PFNGLDEFORMSGIXPROC													glDeformSGIX;
	PFNGLLOADIDENTITYDEFORMATIONMAPSGIXPROC								glLoadIdentityDeformationMapSGIX;					
																		
	PFNGLREFERENCEPLANESGIXPROC											glReferencePlaneSGIX;
																		
	PFNGLFLUSHRASTERSGIXPROC											glFlushRasterSGIX;
																		
	PFNGLFOGFUNCSGISPROC												glFogFuncSGIS;
	PFNGLGETFOGFUNCSGISPROC												glGetFogFuncSGIS;
																		
	PFNGLIMAGETRANSFORMPARAMETERIHPPROC									glImageTransformParameteriHP;				
	PFNGLIMAGETRANSFORMPARAMETERFHPPROC									glImageTransformParameterfHP;				
	PFNGLIMAGETRANSFORMPARAMETERIVHPPROC								glImageTransformParameterivHP;					
	PFNGLIMAGETRANSFORMPARAMETERFVHPPROC								glImageTransformParameterfvHP;					
	PFNGLGETIMAGETRANSFORMPARAMETERIVHPPROC								glGetImageTransformParameterivHP;					
	PFNGLGETIMAGETRANSFORMPARAMETERFVHPPROC								glGetImageTransformParameterfvHP;					
																		
	PFNGLCOLORSUBTABLEEXTPROC											glColorSubTableEXT;
	PFNGLCOPYCOLORSUBTABLEEXTPROC										glCopyColorSubTableEXT;
																		
	PFNGLHINTPGIPROC													glHintPGI;
																		
	PFNGLCOLORTABLEEXTPROC												glColorTableEXT;	
	PFNGLGETCOLORTABLEEXTPROC											glGetColorTableEXT;		
	PFNGLGETCOLORTABLEPARAMETERIVEXTPROC								glGetColorTableParameterivEXT;					
	PFNGLGETCOLORTABLEPARAMETERFVEXTPROC								glGetColorTableParameterfvEXT;					
																		
	PFNGLGETLISTPARAMETERFVSGIXPROC										glGetListParameterfvSGIX;			
	PFNGLGETLISTPARAMETERIVSGIXPROC										glGetListParameterivSGIX;			
	PFNGLLISTPARAMETERFSGIXPROC											glListParameterfSGIX;		
	PFNGLLISTPARAMETERFVSGIXPROC										glListParameterfvSGIX;			
	PFNGLLISTPARAMETERISGIXPROC											glListParameteriSGIX;		
	PFNGLLISTPARAMETERIVSGIXPROC										glListParameterivSGIX;			
																		
	PFNGLINDEXMATERIALEXTPROC											glIndexMaterialEXT;
																		
	PFNGLINDEXFUNCEXTPROC												glIndexFuncEXT;
																		
	PFNGLLOCKARRAYSEXTPROC												glLockArraysEXT;
	PFNGLUNLOCKARRAYSEXTPROC											glUnlockArraysEXT;
																		
	PFNGLCULLPARAMETERDVEXTPROC											glCullParameterdvEXT;
	PFNGLCULLPARAMETERFVEXTPROC											glCullParameterfvEXT;
																		
	PFNGLFRAGMENTCOLORMATERIALSGIXPROC									glFragmentColorMaterialSGIX;				
	PFNGLFRAGMENTLIGHTFSGIXPROC											glFragmentLightfSGIX;		
	PFNGLFRAGMENTLIGHTFVSGIXPROC										glFragmentLightfvSGIX;			
	PFNGLFRAGMENTLIGHTISGIXPROC											glFragmentLightiSGIX;		
	PFNGLFRAGMENTLIGHTIVSGIXPROC										glFragmentLightivSGIX;			
	PFNGLFRAGMENTLIGHTMODELFSGIXPROC									glFragmentLightModelfSGIX;				
	PFNGLFRAGMENTLIGHTMODELFVSGIXPROC									glFragmentLightModelfvSGIX;				
	PFNGLFRAGMENTLIGHTMODELISGIXPROC									glFragmentLightModeliSGIX;				
	PFNGLFRAGMENTLIGHTMODELIVSGIXPROC									glFragmentLightModelivSGIX;				
	PFNGLFRAGMENTMATERIALFSGIXPROC										glFragmentMaterialfSGIX;			
	PFNGLFRAGMENTMATERIALFVSGIXPROC										glFragmentMaterialfvSGIX;			
	PFNGLFRAGMENTMATERIALISGIXPROC										glFragmentMaterialiSGIX;			
	PFNGLFRAGMENTMATERIALIVSGIXPROC										glFragmentMaterialivSGIX;			
	PFNGLGETFRAGMENTLIGHTFVSGIXPROC										glGetFragmentLightfvSGIX;			
	PFNGLGETFRAGMENTLIGHTIVSGIXPROC										glGetFragmentLightivSGIX;			
	PFNGLGETFRAGMENTMATERIALFVSGIXPROC									glGetFragmentMaterialfvSGIX;				
	PFNGLGETFRAGMENTMATERIALIVSGIXPROC									glGetFragmentMaterialivSGIX;				
	PFNGLLIGHTENVISGIXPROC												glLightEnviSGIX;	
																		
	PFNGLDRAWRANGEELEMENTSEXTPROC										glDrawRangeElementsEXT;
																		
	PFNGLAPPLYTEXTUREEXTPROC											glApplyTextureEXT;		
	PFNGLTEXTURELIGHTEXTPROC											glTextureLightEXT;		
	PFNGLTEXTUREMATERIALEXTPROC											glTextureMaterialEXT;		
																		
	PFNGLASYNCMARKERSGIXPROC											glAsyncMarkerSGIX;		
	PFNGLFINISHASYNCSGIXPROC											glFinishAsyncSGIX;		
	PFNGLPOLLASYNCSGIXPROC												glPollAsyncSGIX;	
	PFNGLGENASYNCMARKERSSGIXPROC										glGenAsyncMarkersSGIX;			
	PFNGLDELETEASYNCMARKERSSGIXPROC										glDeleteAsyncMarkersSGIX;			
	PFNGLISASYNCMARKERSGIXPROC											glIsAsyncMarkerSGIX;		
																		
	PFNGLVERTEXPOINTERVINTELPROC										glVertexPointervINTEL;			
	PFNGLNORMALPOINTERVINTELPROC										glNormalPointervINTEL;			
	PFNGLCOLORPOINTERVINTELPROC											glColorPointervINTEL;		
	PFNGLTEXCOORDPOINTERVINTELPROC										glTexCoordPointervINTEL;			
																		
	PFNGLPIXELTRANSFORMPARAMETERIEXTPROC								glPixelTransformParameteriEXT;					
	PFNGLPIXELTRANSFORMPARAMETERFEXTPROC								glPixelTransformParameterfEXT;					
	PFNGLPIXELTRANSFORMPARAMETERIVEXTPROC								glPixelTransformParameterivEXT;					
	PFNGLPIXELTRANSFORMPARAMETERFVEXTPROC								glPixelTransformParameterfvEXT;					
																		
	PFNGLSECONDARYCOLOR3BEXTPROC										glSecondaryColor3bEXT;			
	PFNGLSECONDARYCOLOR3BVEXTPROC										glSecondaryColor3bvEXT;			
	PFNGLSECONDARYCOLOR3DEXTPROC										glSecondaryColor3dEXT;			
	PFNGLSECONDARYCOLOR3DVEXTPROC										glSecondaryColor3dvEXT;			
	PFNGLSECONDARYCOLOR3FEXTPROC										glSecondaryColor3fEXT;			
	PFNGLSECONDARYCOLOR3FVEXTPROC										glSecondaryColor3fvEXT;			
	PFNGLSECONDARYCOLOR3IEXTPROC										glSecondaryColor3iEXT;			
	PFNGLSECONDARYCOLOR3IVEXTPROC										glSecondaryColor3ivEXT;			
	PFNGLSECONDARYCOLOR3SEXTPROC										glSecondaryColor3sEXT;			
	PFNGLSECONDARYCOLOR3SVEXTPROC										glSecondaryColor3svEXT;			
	PFNGLSECONDARYCOLOR3UBEXTPROC										glSecondaryColor3ubEXT;			
	PFNGLSECONDARYCOLOR3UBVEXTPROC										glSecondaryColor3ubvEXT;			
	PFNGLSECONDARYCOLOR3UIEXTPROC										glSecondaryColor3uiEXT;			
	PFNGLSECONDARYCOLOR3UIVEXTPROC										glSecondaryColor3uivEXT;			
	PFNGLSECONDARYCOLOR3USEXTPROC										glSecondaryColor3usEXT;			
	PFNGLSECONDARYCOLOR3USVEXTPROC										glSecondaryColor3usvEXT;			
	PFNGLSECONDARYCOLORPOINTEREXTPROC									glSecondaryColorPointerEXT;				
																		
	PFNGLTEXTURENORMALEXTPROC											glTextureNormalEXT;
																		
	PFNGLMULTIDRAWARRAYSEXTPROC											glMultiDrawArraysEXT;		
	PFNGLMULTIDRAWELEMENTSEXTPROC										glMultiDrawElementsEXT;			
																		
	PFNGLFOGCOORDFEXTPROC												glFogCoordfEXT;	
	PFNGLFOGCOORDFVEXTPROC												glFogCoordfvEXT;	
	PFNGLFOGCOORDDEXTPROC												glFogCoorddEXT;	
	PFNGLFOGCOORDDVEXTPROC												glFogCoorddvEXT;	
	PFNGLFOGCOORDPOINTEREXTPROC											glFogCoordPointerEXT;		
																		
	PFNGLTANGENT3BEXTPROC												glTangent3bEXT;	
	PFNGLTANGENT3BVEXTPROC												glTangent3bvEXT;	
	PFNGLTANGENT3DEXTPROC												glTangent3dEXT;	
	PFNGLTANGENT3DVEXTPROC												glTangent3dvEXT;	
	PFNGLTANGENT3FEXTPROC												glTangent3fEXT;	
	PFNGLTANGENT3FVEXTPROC												glTangent3fvEXT;	
	PFNGLTANGENT3IEXTPROC												glTangent3iEXT;	
	PFNGLTANGENT3IVEXTPROC												glTangent3ivEXT;	
	PFNGLTANGENT3SEXTPROC												glTangent3sEXT;	
	PFNGLTANGENT3SVEXTPROC												glTangent3svEXT;	
	PFNGLBINORMAL3BEXTPROC												glBinormal3bEXT;	
	PFNGLBINORMAL3BVEXTPROC												glBinormal3bvEXT;	
	PFNGLBINORMAL3DEXTPROC												glBinormal3dEXT;	
	PFNGLBINORMAL3DVEXTPROC												glBinormal3dvEXT;	
	PFNGLBINORMAL3FEXTPROC												glBinormal3fEXT;	
	PFNGLBINORMAL3FVEXTPROC												glBinormal3fvEXT;	
	PFNGLBINORMAL3IEXTPROC												glBinormal3iEXT;	
	PFNGLBINORMAL3IVEXTPROC												glBinormal3ivEXT;	
	PFNGLBINORMAL3SEXTPROC												glBinormal3sEXT;	
	PFNGLBINORMAL3SVEXTPROC												glBinormal3svEXT;	
	PFNGLTANGENTPOINTEREXTPROC											glTangentPointerEXT;		
	PFNGLBINORMALPOINTEREXTPROC											glBinormalPointerEXT;		
																		
	PFNGLFINISHTEXTURESUNXPROC											glFinishTextureSUNX;
																		
	PFNGLGLOBALALPHAFACTORBSUNPROC										glGlobalAlphaFactorbSUN;			
	PFNGLGLOBALALPHAFACTORSSUNPROC										glGlobalAlphaFactorsSUN;			
	PFNGLGLOBALALPHAFACTORISUNPROC										glGlobalAlphaFactoriSUN;			
	PFNGLGLOBALALPHAFACTORFSUNPROC										glGlobalAlphaFactorfSUN;			
	PFNGLGLOBALALPHAFACTORDSUNPROC										glGlobalAlphaFactordSUN;			
	PFNGLGLOBALALPHAFACTORUBSUNPROC										glGlobalAlphaFactorubSUN;			
	PFNGLGLOBALALPHAFACTORUSSUNPROC										glGlobalAlphaFactorusSUN;			
	PFNGLGLOBALALPHAFACTORUISUNPROC										glGlobalAlphaFactoruiSUN;			
																		
	PFNGLREPLACEMENTCODEUISUNPROC										glReplacementCodeuiSUN;			
	PFNGLREPLACEMENTCODEUSSUNPROC										glReplacementCodeusSUN;			
	PFNGLREPLACEMENTCODEUBSUNPROC										glReplacementCodeubSUN;			
	PFNGLREPLACEMENTCODEUIVSUNPROC										glReplacementCodeuivSUN;			
	PFNGLREPLACEMENTCODEUSVSUNPROC										glReplacementCodeusvSUN;			
	PFNGLREPLACEMENTCODEUBVSUNPROC										glReplacementCodeubvSUN;			
	PFNGLREPLACEMENTCODEPOINTERSUNPROC									glReplacementCodePointerSUN;				
	
	PFNGLCOLOR4UBVERTEX2FSUNPROC										glColor4ubVertex2fSUN;
	PFNGLCOLOR4UBVERTEX2FVSUNPROC										glColor4ubVertex2fvSUN;
	PFNGLCOLOR4UBVERTEX3FSUNPROC										glColor4ubVertex3fSUN;
	PFNGLCOLOR4UBVERTEX3FVSUNPROC										glColor4ubVertex3fvSUN;
	PFNGLCOLOR3FVERTEX3FSUNPROC											glColor3fVertex3fSUN;
	PFNGLCOLOR3FVERTEX3FVSUNPROC										glColor3fVertex3fvSUN;
	PFNGLNORMAL3FVERTEX3FSUNPROC										glNormal3fVertex3fSUN;
	PFNGLNORMAL3FVERTEX3FVSUNPROC										glNormal3fVertex3fvSUN;
	PFNGLCOLOR4FNORMAL3FVERTEX3FSUNPROC									glColor4fNormal3fVertex3fSUN;
	PFNGLCOLOR4FNORMAL3FVERTEX3FVSUNPROC								glColor4fNormal3fVertex3fvSUN;
	PFNGLTEXCOORD2FVERTEX3FSUNPROC										glTexCoord2fVertex3fSUN;
	PFNGLTEXCOORD2FVERTEX3FVSUNPROC										glTexCoord2fVertex3fvSUN;
	PFNGLTEXCOORD4FVERTEX4FSUNPROC										glTexCoord4fVertex4fSUN;
	PFNGLTEXCOORD4FVERTEX4FVSUNPROC										glTexCoord4fVertex4fvSUN;
	PFNGLTEXCOORD2FCOLOR4UBVERTEX3FSUNPROC								glTexCoord2fColor4ubVertex3fSUN;
	PFNGLTEXCOORD2FCOLOR4UBVERTEX3FVSUNPROC								glTexCoord2fColor4ubVertex3fvSUN;
	PFNGLTEXCOORD2FCOLOR3FVERTEX3FSUNPROC								glTexCoord2fColor3fVertex3fSUN;
	PFNGLTEXCOORD2FCOLOR3FVERTEX3FVSUNPROC								glTexCoord2fColor3fVertex3fvSUN;
	PFNGLTEXCOORD2FNORMAL3FVERTEX3FSUNPROC								glTexCoord2fNormal3fVertex3fSUN;
	PFNGLTEXCOORD2FNORMAL3FVERTEX3FVSUNPROC								glTexCoord2fNormal3fVertex3fvSUN;
	PFNGLTEXCOORD2FCOLOR4FNORMAL3FVERTEX3FSUNPROC						glTexCoord2fColor4fNormal3fVertex3fSUN;
	PFNGLTEXCOORD2FCOLOR4FNORMAL3FVERTEX3FVSUNPROC						glTexCoord2fColor4fNormal3fVertex3fvSUN;
	PFNGLTEXCOORD4FCOLOR4FNORMAL3FVERTEX4FSUNPROC						glTexCoord4fColor4fNormal3fVertex4fSUN;
	PFNGLTEXCOORD4FCOLOR4FNORMAL3FVERTEX4FVSUNPROC						glTexCoord4fColor4fNormal3fVertex4fvSUN;
	PFNGLREPLACEMENTCODEUIVERTEX3FSUNPROC								glReplacementCodeuiVertex3fSUN;
	PFNGLREPLACEMENTCODEUIVERTEX3FVSUNPROC								glReplacementCodeuiVertex3fvSUN;
	PFNGLREPLACEMENTCODEUICOLOR4UBVERTEX3FSUNPROC						glReplacementCodeuiColor4ubVertex3fSUN;
	PFNGLREPLACEMENTCODEUICOLOR4UBVERTEX3FVSUNPROC						glReplacementCodeuiColor4ubVertex3fvSUN;
	PFNGLREPLACEMENTCODEUICOLOR3FVERTEX3FSUNPROC						glReplacementCodeuiColor3fVertex3fSUN;
	PFNGLREPLACEMENTCODEUICOLOR3FVERTEX3FVSUNPROC						glReplacementCodeuiColor3fVertex3fvSUN;
	PFNGLREPLACEMENTCODEUINORMAL3FVERTEX3FSUNPROC						glReplacementCodeuiNormal3fVertex3fSUN;
	PFNGLREPLACEMENTCODEUINORMAL3FVERTEX3FVSUNPROC						glReplacementCodeuiNormal3fVertex3fvSUN;
	PFNGLREPLACEMENTCODEUICOLOR4FNORMAL3FVERTEX3FSUNPROC				glReplacementCodeuiColor4fNormal3fVertex3fSUN;		
	PFNGLREPLACEMENTCODEUICOLOR4FNORMAL3FVERTEX3FVSUNPROC				glReplacementCodeuiColor4fNormal3fVertex3fvSUN;		
	PFNGLREPLACEMENTCODEUITEXCOORD2FVERTEX3FSUNPROC						glReplacementCodeuiTexCoord2fVertex3fSUN;
	PFNGLREPLACEMENTCODEUITEXCOORD2FVERTEX3FVSUNPROC					glReplacementCodeuiTexCoord2fVertex3fvSUN;	
	PFNGLREPLACEMENTCODEUITEXCOORD2FNORMAL3FVERTEX3FSUNPROC				glReplacementCodeuiTexCoord2fNormal3fVertex3fSUN;		
	PFNGLREPLACEMENTCODEUITEXCOORD2FNORMAL3FVERTEX3FVSUNPROC			glReplacementCodeuiTexCoord2fNormal3fVertex3fvSUN;			
	PFNGLREPLACEMENTCODEUITEXCOORD2FCOLOR4FNORMAL3FVERTEX3FSUNPROC		glReplacementCodeuiTexCoord2fColor4fNormal3fVertex3fSUN;				
	PFNGLREPLACEMENTCODEUITEXCOORD2FCOLOR4FNORMAL3FVERTEX3FVSUNPROC		glReplacementCodeuiTexCoord2fColor4fNormal3fVertex3fvSUN;				

	PFNGLBLENDFUNCSEPARATEEXTPROC										glBlendFuncSeparateEXT;
	PFNGLBLENDFUNCSEPARATEINGRPROC										glBlendFuncSeparateINGR;
																		
	PFNGLVERTEXWEIGHTFEXTPROC											glVertexWeightfEXT;	
	PFNGLVERTEXWEIGHTFVEXTPROC											glVertexWeightfvEXT;	
	PFNGLVERTEXWEIGHTPOINTEREXTPROC										glVertexWeightPointerEXT;		
																		
	PFNGLFLUSHVERTEXARRAYRANGENVPROC									glFlushVertexArrayRangeNV;
	PFNGLVERTEXARRAYRANGENVPROC											glVertexArrayRangeNV;
																		
	PFNGLCOMBINERPARAMETERFVNVPROC										glCombinerParameterfvNV;		
	PFNGLCOMBINERPARAMETERFNVPROC										glCombinerParameterfNV;		
	PFNGLCOMBINERPARAMETERIVNVPROC										glCombinerParameterivNV;		
	PFNGLCOMBINERPARAMETERINVPROC										glCombinerParameteriNV;		
	PFNGLCOMBINERINPUTNVPROC											glCombinerInputNV;	
	PFNGLCOMBINEROUTPUTNVPROC											glCombinerOutputNV;	
	PFNGLFINALCOMBINERINPUTNVPROC										glFinalCombinerInputNV;		
	PFNGLGETCOMBINERINPUTPARAMETERFVNVPROC								glGetCombinerInputParameterfvNV;				
	PFNGLGETCOMBINERINPUTPARAMETERIVNVPROC								glGetCombinerInputParameterivNV;				
	PFNGLGETCOMBINEROUTPUTPARAMETERFVNVPROC								glGetCombinerOutputParameterfvNV;				
	PFNGLGETCOMBINEROUTPUTPARAMETERIVNVPROC								glGetCombinerOutputParameterivNV;				
	PFNGLGETFINALCOMBINERINPUTPARAMETERFVNVPROC							glGetFinalCombinerInputParameterfvNV;					
	PFNGLGETFINALCOMBINERINPUTPARAMETERIVNVPROC							glGetFinalCombinerInputParameterivNV;					
																		
	PFNGLRESIZEBUFFERSMESAPROC											glResizeBuffersMESA;
																		
	PFNGLWINDOWPOS2DMESAPROC											glWindowPos2dMESA;	
	PFNGLWINDOWPOS2DVMESAPROC											glWindowPos2dvMESA;	
	PFNGLWINDOWPOS2FMESAPROC											glWindowPos2fMESA;	
	PFNGLWINDOWPOS2FVMESAPROC											glWindowPos2fvMESA;	
	PFNGLWINDOWPOS2IMESAPROC											glWindowPos2iMESA;	
	PFNGLWINDOWPOS2IVMESAPROC											glWindowPos2ivMESA;	
	PFNGLWINDOWPOS2SMESAPROC											glWindowPos2sMESA;	
	PFNGLWINDOWPOS2SVMESAPROC											glWindowPos2svMESA;	
	PFNGLWINDOWPOS3DMESAPROC											glWindowPos3dMESA;	
	PFNGLWINDOWPOS3DVMESAPROC											glWindowPos3dvMESA;	
	PFNGLWINDOWPOS3FMESAPROC											glWindowPos3fMESA;	
	PFNGLWINDOWPOS3FVMESAPROC											glWindowPos3fvMESA;	
	PFNGLWINDOWPOS3IMESAPROC											glWindowPos3iMESA;	
	PFNGLWINDOWPOS3IVMESAPROC											glWindowPos3ivMESA;	
	PFNGLWINDOWPOS3SMESAPROC											glWindowPos3sMESA;	
	PFNGLWINDOWPOS3SVMESAPROC											glWindowPos3svMESA;	
	PFNGLWINDOWPOS4DMESAPROC											glWindowPos4dMESA;	
	PFNGLWINDOWPOS4DVMESAPROC											glWindowPos4dvMESA;	
	PFNGLWINDOWPOS4FMESAPROC											glWindowPos4fMESA;	
	PFNGLWINDOWPOS4FVMESAPROC											glWindowPos4fvMESA;	
	PFNGLWINDOWPOS4IMESAPROC											glWindowPos4iMESA;	
	PFNGLWINDOWPOS4IVMESAPROC											glWindowPos4ivMESA;	
	PFNGLWINDOWPOS4SMESAPROC											glWindowPos4sMESA;	
	PFNGLWINDOWPOS4SVMESAPROC											glWindowPos4svMESA;	
																		
	PFNGLMULTIMODEDRAWARRAYSIBMPROC										glMultiModeDrawArraysIBM;
	PFNGLMULTIMODEDRAWELEMENTSIBMPROC									glMultiModeDrawElementsIBM;
																		
	PFNGLCOLORPOINTERLISTIBMPROC										glColorPointerListIBM;		
	PFNGLSECONDARYCOLORPOINTERLISTIBMPROC								glSecondaryColorPointerListIBM;				
	PFNGLEDGEFLAGPOINTERLISTIBMPROC										glEdgeFlagPointerListIBM;		
	PFNGLFOGCOORDPOINTERLISTIBMPROC										glFogCoordPointerListIBM;		
	PFNGLINDEXPOINTERLISTIBMPROC										glIndexPointerListIBM;		
	PFNGLNORMALPOINTERLISTIBMPROC										glNormalPointerListIBM;		
	PFNGLTEXCOORDPOINTERLISTIBMPROC										glTexCoordPointerListIBM;		
	PFNGLVERTEXPOINTERLISTIBMPROC										glVertexPointerListIBM;		
																		
	PFNGLTBUFFERMASK3DFXPROC											glTbufferMask3DFX;
																		
	PFNGLSAMPLEMASKEXTPROC												glSampleMaskEXT;
	PFNGLSAMPLEPATTERNEXTPROC											glSamplePatternEXT;
																		
	PFNGLTEXTURECOLORMASKSGISPROC										glTextureColorMaskSGIS;
																		
	PFNGLIGLOOINTERFACESGIXPROC											glIglooInterfaceSGIX;
																		
	PFNGLDELETEFENCESNVPROC												glDeleteFencesNV;
	PFNGLGENFENCESNVPROC												glGenFencesNV;
	PFNGLISFENCENVPROC													glIsFenceNV;
	PFNGLTESTFENCENVPROC												glTestFenceNV;
	PFNGLGETFENCEIVNVPROC												glGetFenceivNV;
	PFNGLFINISHFENCENVPROC												glFinishFenceNV;
	PFNGLSETFENCENVPROC													glSetFenceNV;
																		
	PFNGLMAPCONTROLPOINTSNVPROC											glMapControlPointsNV;	
	PFNGLMAPPARAMETERIVNVPROC											glMapParameterivNV;	
	PFNGLMAPPARAMETERFVNVPROC											glMapParameterfvNV;	
	PFNGLGETMAPCONTROLPOINTSNVPROC										glGetMapControlPointsNV;		
	PFNGLGETMAPPARAMETERIVNVPROC										glGetMapParameterivNV;		
	PFNGLGETMAPPARAMETERFVNVPROC										glGetMapParameterfvNV;		
	PFNGLGETMAPATTRIBPARAMETERIVNVPROC									glGetMapAttribParameterivNV;			
	PFNGLGETMAPATTRIBPARAMETERFVNVPROC									glGetMapAttribParameterfvNV;			
	PFNGLEVALMAPSNVPROC													glEvalMapsNV;
																		
	PFNGLCOMBINERSTAGEPARAMETERFVNVPROC									glCombinerStageParameterfvNV;
	PFNGLGETCOMBINERSTAGEPARAMETERFVNVPROC								glGetCombinerStageParameterfvNV;
																		
	PFNGLAREPROGRAMSRESIDENTNVPROC										glAreProgramsResidentNV;		
	PFNGLBINDPROGRAMNVPROC												glBindProgramNV;
	PFNGLDELETEPROGRAMSNVPROC											glDeleteProgramsNV;	
	PFNGLEXECUTEPROGRAMNVPROC											glExecuteProgramNV;	
	PFNGLGENPROGRAMSNVPROC												glGenProgramsNV;
	PFNGLGETPROGRAMPARAMETERDVNVPROC									glGetProgramParameterdvNV;			
	PFNGLGETPROGRAMPARAMETERFVNVPROC									glGetProgramParameterfvNV;			
	PFNGLGETPROGRAMIVNVPROC												glGetProgramivNV;
	PFNGLGETPROGRAMSTRINGNVPROC											glGetProgramStringNV;	
	PFNGLGETTRACKMATRIXIVNVPROC											glGetTrackMatrixivNV;	
	PFNGLGETVERTEXATTRIBDVNVPROC										glGetVertexAttribdvNV;		
	PFNGLGETVERTEXATTRIBFVNVPROC										glGetVertexAttribfvNV;		
	PFNGLGETVERTEXATTRIBIVNVPROC										glGetVertexAttribivNV;		
	PFNGLGETVERTEXATTRIBPOINTERVNVPROC									glGetVertexAttribPointervNV;			
	PFNGLISPROGRAMNVPROC												glIsProgramNV;
	PFNGLLOADPROGRAMNVPROC												glLoadProgramNV;
	PFNGLPROGRAMPARAMETER4DNVPROC										glProgramParameter4dNV;		
	PFNGLPROGRAMPARAMETER4DVNVPROC										glProgramParameter4dvNV;		
	PFNGLPROGRAMPARAMETER4FNVPROC										glProgramParameter4fNV;		
	PFNGLPROGRAMPARAMETER4FVNVPROC										glProgramParameter4fvNV;		
	PFNGLPROGRAMPARAMETERS4DVNVPROC										glProgramParameters4dvNV;		
	PFNGLPROGRAMPARAMETERS4FVNVPROC										glProgramParameters4fvNV;		
	PFNGLREQUESTRESIDENTPROGRAMSNVPROC									glRequestResidentProgramsNV;			
	PFNGLTRACKMATRIXNVPROC												glTrackMatrixNV;
	PFNGLVERTEXATTRIBPOINTERNVPROC										glVertexAttribPointerNV;		
	PFNGLVERTEXATTRIB1DNVPROC											glVertexAttrib1dNV;	
	PFNGLVERTEXATTRIB1DVNVPROC											glVertexAttrib1dvNV;	
	PFNGLVERTEXATTRIB1FNVPROC											glVertexAttrib1fNV;	
	PFNGLVERTEXATTRIB1FVNVPROC											glVertexAttrib1fvNV;	
	PFNGLVERTEXATTRIB1SNVPROC											glVertexAttrib1sNV;	
	PFNGLVERTEXATTRIB1SVNVPROC											glVertexAttrib1svNV;	
	PFNGLVERTEXATTRIB2DNVPROC											glVertexAttrib2dNV;	
	PFNGLVERTEXATTRIB2DVNVPROC											glVertexAttrib2dvNV;	
	PFNGLVERTEXATTRIB2FNVPROC											glVertexAttrib2fNV;	
	PFNGLVERTEXATTRIB2FVNVPROC											glVertexAttrib2fvNV;	
	PFNGLVERTEXATTRIB2SNVPROC											glVertexAttrib2sNV;	
	PFNGLVERTEXATTRIB2SVNVPROC											glVertexAttrib2svNV;	
	PFNGLVERTEXATTRIB3DNVPROC											glVertexAttrib3dNV;	
	PFNGLVERTEXATTRIB3DVNVPROC											glVertexAttrib3dvNV;	
	PFNGLVERTEXATTRIB3FNVPROC											glVertexAttrib3fNV;	
	PFNGLVERTEXATTRIB3FVNVPROC											glVertexAttrib3fvNV;	
	PFNGLVERTEXATTRIB3SNVPROC											glVertexAttrib3sNV;	
	PFNGLVERTEXATTRIB3SVNVPROC											glVertexAttrib3svNV;	
	PFNGLVERTEXATTRIB4DNVPROC											glVertexAttrib4dNV;	
	PFNGLVERTEXATTRIB4DVNVPROC											glVertexAttrib4dvNV;	
	PFNGLVERTEXATTRIB4FNVPROC											glVertexAttrib4fNV;	
	PFNGLVERTEXATTRIB4FVNVPROC											glVertexAttrib4fvNV;	
	PFNGLVERTEXATTRIB4SNVPROC											glVertexAttrib4sNV;	
	PFNGLVERTEXATTRIB4SVNVPROC											glVertexAttrib4svNV;	
	PFNGLVERTEXATTRIB4UBNVPROC											glVertexAttrib4ubNV;	
	PFNGLVERTEXATTRIB4UBVNVPROC											glVertexAttrib4ubvNV;	
	PFNGLVERTEXATTRIBS1DVNVPROC											glVertexAttribs1dvNV;	
	PFNGLVERTEXATTRIBS1FVNVPROC											glVertexAttribs1fvNV;	
	PFNGLVERTEXATTRIBS1SVNVPROC											glVertexAttribs1svNV;	
	PFNGLVERTEXATTRIBS2DVNVPROC											glVertexAttribs2dvNV;	
	PFNGLVERTEXATTRIBS2FVNVPROC											glVertexAttribs2fvNV;	
	PFNGLVERTEXATTRIBS2SVNVPROC											glVertexAttribs2svNV;	
	PFNGLVERTEXATTRIBS3DVNVPROC											glVertexAttribs3dvNV;	
	PFNGLVERTEXATTRIBS3FVNVPROC											glVertexAttribs3fvNV;	
	PFNGLVERTEXATTRIBS3SVNVPROC											glVertexAttribs3svNV;	
	PFNGLVERTEXATTRIBS4DVNVPROC											glVertexAttribs4dvNV;	
	PFNGLVERTEXATTRIBS4FVNVPROC											glVertexAttribs4fvNV;	
	PFNGLVERTEXATTRIBS4SVNVPROC											glVertexAttribs4svNV;	
	PFNGLVERTEXATTRIBS4UBVNVPROC										glVertexAttribs4ubvNV;		
																		
	PFNGLTEXBUMPPARAMETERIVATIPROC										glTexBumpParameterivATI;		
	PFNGLTEXBUMPPARAMETERFVATIPROC										glTexBumpParameterfvATI;		
	PFNGLGETTEXBUMPPARAMETERIVATIPROC									glGetTexBumpParameterivATI;			
	PFNGLGETTEXBUMPPARAMETERFVATIPROC									glGetTexBumpParameterfvATI;			
																		
	PFNGLGENFRAGMENTSHADERSATIPROC										glGenFragmentShadersATI;		
	PFNGLBINDFRAGMENTSHADERATIPROC										glBindFragmentShaderATI;		
	PFNGLDELETEFRAGMENTSHADERATIPROC									glDeleteFragmentShaderATI;			
	PFNGLBEGINFRAGMENTSHADERATIPROC										glBeginFragmentShaderATI;		
	PFNGLENDFRAGMENTSHADERATIPROC										glEndFragmentShaderATI;		
	PFNGLPASSTEXCOORDATIPROC											glPassTexCoordATI;	
	PFNGLSAMPLEMAPATIPROC												glSampleMapATI;
	PFNGLCOLORFRAGMENTOP1ATIPROC										glColorFragmentOp1ATI;		
	PFNGLCOLORFRAGMENTOP2ATIPROC										glColorFragmentOp2ATI;		
	PFNGLCOLORFRAGMENTOP3ATIPROC										glColorFragmentOp3ATI;		
	PFNGLALPHAFRAGMENTOP1ATIPROC										glAlphaFragmentOp1ATI;		
	PFNGLALPHAFRAGMENTOP2ATIPROC										glAlphaFragmentOp2ATI;		
	PFNGLALPHAFRAGMENTOP3ATIPROC										glAlphaFragmentOp3ATI;		
	PFNGLSETFRAGMENTSHADERCONSTANTATIPROC								glSetFragmentShaderConstantATI;				
																		
	PFNGLPNTRIANGLESIATIPROC											glPNTrianglesiATI;	
	PFNGLPNTRIANGLESFATIPROC											glPNTrianglesfATI;	
																		
	PFNGLNEWOBJECTBUFFERATIPROC											glNewObjectBufferATI;	
	PFNGLISOBJECTBUFFERATIPROC											glIsObjectBufferATI;	
	PFNGLUPDATEOBJECTBUFFERATIPROC										glUpdateObjectBufferATI;		
	PFNGLGETOBJECTBUFFERFVATIPROC										glGetObjectBufferfvATI;		
	PFNGLGETOBJECTBUFFERIVATIPROC										glGetObjectBufferivATI;		
	PFNGLFREEOBJECTBUFFERATIPROC										glFreeObjectBufferATI;		
	PFNGLARRAYOBJECTATIPROC												glArrayObjectATI;
	PFNGLGETARRAYOBJECTFVATIPROC										glGetArrayObjectfvATI;		
	PFNGLGETARRAYOBJECTIVATIPROC										glGetArrayObjectivATI;		
	PFNGLVARIANTARRAYOBJECTATIPROC										glVariantArrayObjectATI;		
	PFNGLGETVARIANTARRAYOBJECTFVATIPROC									glGetVariantArrayObjectfvATI;			
	PFNGLGETVARIANTARRAYOBJECTIVATIPROC									glGetVariantArrayObjectivATI;			
																		
	PFNGLBEGINVERTEXSHADEREXTPROC										glBeginVertexShaderEXT;		
	PFNGLENDVERTEXSHADEREXTPROC											glEndVertexShaderEXT;	
	PFNGLBINDVERTEXSHADEREXTPROC										glBindVertexShaderEXT;		
	PFNGLGENVERTEXSHADERSEXTPROC										glGenVertexShadersEXT;		
	PFNGLDELETEVERTEXSHADEREXTPROC										glDeleteVertexShaderEXT;		
	PFNGLSHADEROP1EXTPROC												glShaderOp1EXT;
	PFNGLSHADEROP2EXTPROC												glShaderOp2EXT;
	PFNGLSHADEROP3EXTPROC												glShaderOp3EXT;
	PFNGLSWIZZLEEXTPROC													glSwizzleEXT;
	PFNGLWRITEMASKEXTPROC												glWriteMaskEXT;
	PFNGLINSERTCOMPONENTEXTPROC											glInsertComponentEXT;	
	PFNGLEXTRACTCOMPONENTEXTPROC										glExtractComponentEXT;		
	PFNGLGENSYMBOLSEXTPROC												glGenSymbolsEXT;
	PFNGLSETINVARIANTEXTPROC											glSetInvariantEXT;	
	PFNGLSETLOCALCONSTANTEXTPROC										glSetLocalConstantEXT;		
	PFNGLVARIANTBVEXTPROC												glVariantbvEXT;
	PFNGLVARIANTSVEXTPROC												glVariantsvEXT;
	PFNGLVARIANTIVEXTPROC												glVariantivEXT;
	PFNGLVARIANTFVEXTPROC												glVariantfvEXT;
	PFNGLVARIANTDVEXTPROC												glVariantdvEXT;
	PFNGLVARIANTUBVEXTPROC												glVariantubvEXT;
	PFNGLVARIANTUSVEXTPROC												glVariantusvEXT;
	PFNGLVARIANTUIVEXTPROC												glVariantuivEXT;
	PFNGLVARIANTPOINTEREXTPROC											glVariantPointerEXT;	
	PFNGLENABLEVARIANTCLIENTSTATEEXTPROC								glEnableVariantClientStateEXT;				
	PFNGLDISABLEVARIANTCLIENTSTATEEXTPROC								glDisableVariantClientStateEXT;				
	PFNGLBINDLIGHTPARAMETEREXTPROC										glBindLightParameterEXT;		
	PFNGLBINDMATERIALPARAMETEREXTPROC									glBindMaterialParameterEXT;			
	PFNGLBINDTEXGENPARAMETEREXTPROC										glBindTexGenParameterEXT;		
	PFNGLBINDTEXTUREUNITPARAMETEREXTPROC								glBindTextureUnitParameterEXT;				
	PFNGLBINDPARAMETEREXTPROC											glBindParameterEXT;	
	PFNGLISVARIANTENABLEDEXTPROC										glIsVariantEnabledEXT;		
	PFNGLGETVARIANTBOOLEANVEXTPROC										glGetVariantBooleanvEXT;		
	PFNGLGETVARIANTINTEGERVEXTPROC										glGetVariantIntegervEXT;		
	PFNGLGETVARIANTFLOATVEXTPROC										glGetVariantFloatvEXT;		
	PFNGLGETVARIANTPOINTERVEXTPROC										glGetVariantPointervEXT;		
	PFNGLGETINVARIANTBOOLEANVEXTPROC									glGetInvariantBooleanvEXT;			
	PFNGLGETINVARIANTINTEGERVEXTPROC									glGetInvariantIntegervEXT;			
	PFNGLGETINVARIANTFLOATVEXTPROC										glGetInvariantFloatvEXT;		
	PFNGLGETLOCALCONSTANTBOOLEANVEXTPROC								glGetLocalConstantBooleanvEXT;				
	PFNGLGETLOCALCONSTANTINTEGERVEXTPROC								glGetLocalConstantIntegervEXT;				
	PFNGLGETLOCALCONSTANTFLOATVEXTPROC									glGetLocalConstantFloatvEXT;			
																		
	PFNGLVERTEXSTREAM1SATIPROC											glVertexStream1sATI;	
	PFNGLVERTEXSTREAM1SVATIPROC											glVertexStream1svATI;	
	PFNGLVERTEXSTREAM1IATIPROC											glVertexStream1iATI;	
	PFNGLVERTEXSTREAM1IVATIPROC											glVertexStream1ivATI;	
	PFNGLVERTEXSTREAM1FATIPROC											glVertexStream1fATI;	
	PFNGLVERTEXSTREAM1FVATIPROC											glVertexStream1fvATI;	
	PFNGLVERTEXSTREAM1DATIPROC											glVertexStream1dATI;	
	PFNGLVERTEXSTREAM1DVATIPROC											glVertexStream1dvATI;	
	PFNGLVERTEXSTREAM2SATIPROC											glVertexStream2sATI;	
	PFNGLVERTEXSTREAM2SVATIPROC											glVertexStream2svATI;	
	PFNGLVERTEXSTREAM2IATIPROC											glVertexStream2iATI;	
	PFNGLVERTEXSTREAM2IVATIPROC											glVertexStream2ivATI;	
	PFNGLVERTEXSTREAM2FATIPROC											glVertexStream2fATI;	
	PFNGLVERTEXSTREAM2FVATIPROC											glVertexStream2fvATI;	
	PFNGLVERTEXSTREAM2DATIPROC											glVertexStream2dATI;	
	PFNGLVERTEXSTREAM2DVATIPROC											glVertexStream2dvATI;	
	PFNGLVERTEXSTREAM3SATIPROC											glVertexStream3sATI;	
	PFNGLVERTEXSTREAM3SVATIPROC											glVertexStream3svATI;	
	PFNGLVERTEXSTREAM3IATIPROC											glVertexStream3iATI;	
	PFNGLVERTEXSTREAM3IVATIPROC											glVertexStream3ivATI;	
	PFNGLVERTEXSTREAM3FATIPROC											glVertexStream3fATI;	
	PFNGLVERTEXSTREAM3FVATIPROC											glVertexStream3fvATI;	
	PFNGLVERTEXSTREAM3DATIPROC											glVertexStream3dATI;	
	PFNGLVERTEXSTREAM3DVATIPROC											glVertexStream3dvATI;	
	PFNGLVERTEXSTREAM4SATIPROC											glVertexStream4sATI;	
	PFNGLVERTEXSTREAM4SVATIPROC											glVertexStream4svATI;	
	PFNGLVERTEXSTREAM4IATIPROC											glVertexStream4iATI;	
	PFNGLVERTEXSTREAM4IVATIPROC											glVertexStream4ivATI;	
	PFNGLVERTEXSTREAM4FATIPROC											glVertexStream4fATI;	
	PFNGLVERTEXSTREAM4FVATIPROC											glVertexStream4fvATI;	
	PFNGLVERTEXSTREAM4DATIPROC											glVertexStream4dATI;	
	PFNGLVERTEXSTREAM4DVATIPROC											glVertexStream4dvATI;	
	PFNGLNORMALSTREAM3BATIPROC											glNormalStream3bATI;	
	PFNGLNORMALSTREAM3BVATIPROC											glNormalStream3bvATI;	
	PFNGLNORMALSTREAM3SATIPROC											glNormalStream3sATI;	
	PFNGLNORMALSTREAM3SVATIPROC											glNormalStream3svATI;	
	PFNGLNORMALSTREAM3IATIPROC											glNormalStream3iATI;	
	PFNGLNORMALSTREAM3IVATIPROC											glNormalStream3ivATI;	
	PFNGLNORMALSTREAM3FATIPROC											glNormalStream3fATI;	
	PFNGLNORMALSTREAM3FVATIPROC											glNormalStream3fvATI;	
	PFNGLNORMALSTREAM3DATIPROC											glNormalStream3dATI;	
	PFNGLNORMALSTREAM3DVATIPROC											glNormalStream3dvATI;	
	PFNGLCLIENTACTIVEVERTEXSTREAMATIPROC								glClientActiveVertexStreamATI;				
	PFNGLVERTEXBLENDENVIATIPROC											glVertexBlendEnviATI;	
	PFNGLVERTEXBLENDENVFATIPROC											glVertexBlendEnvfATI;	
																		
	PFNGLELEMENTPOINTERATIPROC											glElementPointerATI;	
	PFNGLDRAWELEMENTARRAYATIPROC										glDrawElementArrayATI;		
	PFNGLDRAWRANGEELEMENTARRAYATIPROC									glDrawRangeElementArrayATI;			
																		
	PFNGLDRAWMESHARRAYSSUNPROC											glDrawMeshArraysSUN;
																		
	PFNGLGENOCCLUSIONQUERIESNVPROC										glGenOcclusionQueriesNV;		
	PFNGLDELETEOCCLUSIONQUERIESNVPROC									glDeleteOcclusionQueriesNV;			
	PFNGLISOCCLUSIONQUERYNVPROC											glIsOcclusionQueryNV;	
	PFNGLBEGINOCCLUSIONQUERYNVPROC										glBeginOcclusionQueryNV;		
	PFNGLENDOCCLUSIONQUERYNVPROC										glEndOcclusionQueryNV;		
	PFNGLGETOCCLUSIONQUERYIVNVPROC										glGetOcclusionQueryivNV;		
	PFNGLGETOCCLUSIONQUERYUIVNVPROC										glGetOcclusionQueryuivNV;		
																		
	PFNGLPOINTPARAMETERINVPROC											glPointParameteriNV;
	PFNGLPOINTPARAMETERIVNVPROC											glPointParameterivNV;
																		
	PFNGLACTIVESTENCILFACEEXTPROC										glActiveStencilFaceEXT;

	PFNGLDRAWBUFFERSATIPROC												glDrawBuffersATI;

	PFNGLPROGRAMNAMEDPARAMETER4FNVPROC									glProgramNamedParameter4fNV;					
	PFNGLPROGRAMNAMEDPARAMETER4DNVPROC									glProgramNamedParameter4dNV;					
	PFNGLPROGRAMNAMEDPARAMETER4FVNVPROC									glProgramNamedParameter4fvNV;					
	PFNGLPROGRAMNAMEDPARAMETER4DVNVPROC									glProgramNamedParameter4dvNV;					
	PFNGLGETPROGRAMNAMEDPARAMETERFVNVPROC								glGetProgramNamedParameterfvNV;						
	PFNGLGETPROGRAMNAMEDPARAMETERDVNVPROC								glGetProgramNamedParameterdvNV;						

	PFNGLVERTEX2HNVPROC													glVertex2hNV;	
	PFNGLVERTEX2HVNVPROC												glVertex2hvNV;		
	PFNGLVERTEX3HNVPROC													glVertex3hNV;	
	PFNGLVERTEX3HVNVPROC												glVertex3hvNV;		
	PFNGLVERTEX4HNVPROC													glVertex4hNV;	
	PFNGLVERTEX4HVNVPROC												glVertex4hvNV;		
	PFNGLNORMAL3HNVPROC													glNormal3hNV;	
	PFNGLNORMAL3HVNVPROC												glNormal3hvNV;		
	PFNGLCOLOR3HNVPROC													glColor3hNV;	
	PFNGLCOLOR3HVNVPROC													glColor3hvNV;	
	PFNGLCOLOR4HNVPROC													glColor4hNV;	
	PFNGLCOLOR4HVNVPROC													glColor4hvNV;	
	PFNGLTEXCOORD1HNVPROC												glTexCoord1hNV;		
	PFNGLTEXCOORD1HVNVPROC												glTexCoord1hvNV;		
	PFNGLTEXCOORD2HNVPROC												glTexCoord2hNV;		
	PFNGLTEXCOORD2HVNVPROC												glTexCoord2hvNV;		
	PFNGLTEXCOORD3HNVPROC												glTexCoord3hNV;		
	PFNGLTEXCOORD3HVNVPROC												glTexCoord3hvNV;		
	PFNGLTEXCOORD4HNVPROC												glTexCoord4hNV;		
	PFNGLTEXCOORD4HVNVPROC												glTexCoord4hvNV;		
	PFNGLMULTITEXCOORD1HNVPROC											glMultiTexCoord1hNV;			
	PFNGLMULTITEXCOORD1HVNVPROC											glMultiTexCoord1hvNV;			
	PFNGLMULTITEXCOORD2HNVPROC											glMultiTexCoord2hNV;			
	PFNGLMULTITEXCOORD2HVNVPROC											glMultiTexCoord2hvNV;			
	PFNGLMULTITEXCOORD3HNVPROC											glMultiTexCoord3hNV;			
	PFNGLMULTITEXCOORD3HVNVPROC											glMultiTexCoord3hvNV;			
	PFNGLMULTITEXCOORD4HNVPROC											glMultiTexCoord4hNV;			
	PFNGLMULTITEXCOORD4HVNVPROC											glMultiTexCoord4hvNV;			
	PFNGLFOGCOORDHNVPROC												glFogCoordhNV;		
	PFNGLFOGCOORDHVNVPROC												glFogCoordhvNV;		
	PFNGLSECONDARYCOLOR3HNVPROC											glSecondaryColor3hNV;			
	PFNGLSECONDARYCOLOR3HVNVPROC										glSecondaryColor3hvNV;				
	PFNGLVERTEXWEIGHTHNVPROC											glVertexWeighthNV;			
	PFNGLVERTEXWEIGHTHVNVPROC											glVertexWeighthvNV;			
	PFNGLVERTEXATTRIB1HNVPROC											glVertexAttrib1hNV;			
	PFNGLVERTEXATTRIB1HVNVPROC											glVertexAttrib1hvNV;			
	PFNGLVERTEXATTRIB2HNVPROC											glVertexAttrib2hNV;			
	PFNGLVERTEXATTRIB2HVNVPROC											glVertexAttrib2hvNV;			
	PFNGLVERTEXATTRIB3HNVPROC											glVertexAttrib3hNV;			
	PFNGLVERTEXATTRIB3HVNVPROC											glVertexAttrib3hvNV;			
	PFNGLVERTEXATTRIB4HNVPROC											glVertexAttrib4hNV;			
	PFNGLVERTEXATTRIB4HVNVPROC											glVertexAttrib4hvNV;			
	PFNGLVERTEXATTRIBS1HVNVPROC											glVertexAttribs1hvNV;			
	PFNGLVERTEXATTRIBS2HVNVPROC											glVertexAttribs2hvNV;			
	PFNGLVERTEXATTRIBS3HVNVPROC											glVertexAttribs3hvNV;			
	PFNGLVERTEXATTRIBS4HVNVPROC											glVertexAttribs4hvNV;			

	PFNGLPIXELDATARANGENVPROC											glPixelDataRangeNV;			
	PFNGLFLUSHPIXELDATARANGENVPROC										glFlushPixelDataRangeNV;				

	PFNGLPRIMITIVERESTARTNVPROC											glPrimitiveRestartNV;			
	PFNGLPRIMITIVERESTARTINDEXNVPROC									glPrimitiveRestartIndexNV;					

	PFNGLMAPOBJECTBUFFERATIPROC											glMapObjectBufferATI;			
	PFNGLUNMAPOBJECTBUFFERATIPROC										glUnmapObjectBufferATI;				

	PFNGLSTENCILOPSEPARATEATIPROC										glStencilOpSeparateATI;				
	PFNGLSTENCILFUNCSEPARATEATIPROC										glStencilFuncSeparateATI;				

	PFNGLVERTEXATTRIBARRAYOBJECTATIPROC									glVertexAttribArrayObjectATI;					
	PFNGLGETVERTEXATTRIBARRAYOBJECTFVATIPROC							glGetVertexAttribArrayObjectfvATI;							
	PFNGLGETVERTEXATTRIBARRAYOBJECTIVATIPROC							glGetVertexAttribArrayObjectivATI;							

} XGSGLExtensionsFuncs;

//***************************************************************************************
// Structures
//***************************************************************************************

//! XSI_RTS_Light
/*! Structure holding information about a single light source
*/
typedef struct tagXSI_RTS_Light
{
	float					m_fRed;							//! Light red value
	float					m_fGreen;						//! Light green value
	float					m_fBlue;						//! Light blue value
	float					m_fAlpha;						//! Light alpha value
															
	float					m_fPosX;						//! Light X position
	float					m_fPosY;						//! Light Y position
	float					m_fPosZ;						//! Light Z position
	float					m_fPosW;						//! Light W position
															
	float					m_fDirX;						//! Light X direction
	float					m_fDirY;						//! Light Y direction
	float					m_fDirZ;						//! Light Z direction
															
	float					m_fConeAngle;					//! Spot light cone angle
															
	int						m_iID;							//! OpenGL light ID
	char*					m_pchID;						//! Light name
															
	eXSI_RTS_LightType		m_eType;						//! Light type

	bool					m_bCastsShadows;				//! Light casts shadows
	
	bool					m_bVisible;						//! Light object is visible

} XSI_RTS_Light;

//! XSI_RTS_Camera
/*! Structure holding information about a camera
*/
typedef struct tagXSI_RTS_Camera
{
	float					m_mMatrix[16];					//! The camera matrix
	eXSI_RTS_CameraType		m_eCameraType;					//! The type of camera (ortho, perspective)
															
	float					m_fRight;						//! Frustum right
	float					m_fLeft;						//! Frustum left
	float					m_fTop;							//! Frustum top
	float					m_fBottom;						//! Frustum bottom
															
	float					m_fNear;						//! Near plane
	float					m_fFar;							//! Far plane
															
	float					m_fFOV;							//! Field of view
															
	float					m_fAspect;						//! Aspect ratio
} XSI_RTS_Camera;


typedef struct tagXSI_RTS_SceneData
{
	int						m_iNbLights;					//! Number of lights
	XSI_RTS_Light*			m_pLights;						//! Array of XSI_RTS_Light structures
															
	XSI_RTS_Camera			m_sCamera;						//! The current camera

	double					m_fCurrentTime;					//! Current time
} XSI_RTS_SceneData;

typedef struct tagXSI_RTS_SystemParams
{
	ULONG					m_lNbParams;					//! Number of parameters
									
	ULONG					m_lAppID;						//! Application ID
	ULONG					m_lRTSVersion;					//! Realtime shader protocol version
	RTSContext				m_lContext;						//! Context
							
	void					*m_pShaderHandle;				//! Shader instance handle (CRef)
	void					*m_pObjectHandle;				//! Owner object handle (CRef)
	void					*m_pMaterialHandle;				//! Owner material handle (CRef)

	void					*m_pGS;							//! Handle to the graphic system (CRef)
	XGSExtensionsGL			*m_pGLExt;						//! XSI OpenGL extension wrapper flags
	XGSGLExtensionsFuncs	*m_pGLExtFuncs;					//! XSI OpenGL extension wrapper functions

	ULONG					m_lShaderFlags;					//! Shader flags
	ULONG					m_lObjectFlags;					//! Object flags

	float					m_m4x4ObjectPose[16];			//! Object's transformation matrix
	
	XSI_RTS_SceneData		*m_SceneData;					//! Scene Data
} XSI_RTS_SystemParams;

typedef struct tagXSI_RTS_TriangleList
{
	ULONG			m_lOffset;						//! Starting offset of that triangle list
	ULONG			m_lNbVertices;					//! Number of vertices for that triangle list
	ULONG			m_lType;						//! Type of triangle list (strip, list, fan : Mapped with OGL constants)
	ULONG			m_lMinIndex;					//! Minimum index used for this triangle list
	ULONG			m_lMaxIndex;					//! Maximum index used for this triangle list

	// The following fields are added in order for the XSI_RTS_TriangleList to
	// use the same amound of memory as a C3DStrip<TvCoord, TNormal, TColor, TTCCord> 
	// object.  This enables to cast a array of one type into a array of the other 
	// type without problems.  This is used in C3DDrawObject<TVcoord, TNormal, TColor, TTcoord>::RealtimeShaderDraw
	// for example.

	void *			m_pReserved0;					//! Reserved0
	void *			m_pReserved1;					//! Reserved1
	void *			m_pReserved2;					//! Reserved2
	ULONG			m_lReserved3;					//! Reserved3
} XSI_RTS_TriangleList;


typedef struct tagXSI_RTS_TriangulatedGeometry
{
	// Triangle lists information
	ULONG					m_Type;
	ULONG					m_lNbTriangleLists;				//! Number of triangle lists for the associated geometry
	XSI_RTS_TriangleList	*m_pTriangleLists;				//! Triangle lists
	ULONG					m_NbVertices;					//! Total number of vertices

	// Pointer streams
	XSI_RTS_Vector3d		**m_pPositionDPtrArray;			//! Array of pointers to the positions (DOUBLE)
	XSI_RTS_Vector3			**m_pPositionPtrArray;			//! Array of pointers to the positions
	XSI_RTS_Vector3			**m_pNormalPtrArray;			//! Array of pointers to the normals (interleaved by streams)
	XSI_RTS_ColorRGBA		**m_pColorPtrArray;				//! Array of pointers to the colors (interleaved by streams)
	XSI_RTS_Vector2			**m_pTexCoordPtrArray;			//! Array of pointers to the texture coordinates (interleaved by streams)

	// Vertex attribute arrays streams
	ULONG					m_NbUniqueVertices;				//! Total number of unique vertices
	XSI_RTS_Vector3			*m_pPositionArray;				//! Array of positions
	XSI_RTS_Vector3			*m_pNormalArray;				//! Array of normals (interleaved by streams)
	XSI_RTS_ColorRGBA		*m_pColorArray;					//! Array of colors (interleaved by streams)
	XSI_RTS_Vector2			*m_pTexCoordArray;				//! Array of texture coordinates (interleaved by streams)
	ULONG					*m_pIndices;					//! Array of global indices

	// Number of normals, colors and texture coordinates
	ULONG					m_lNbNormalStreams;				//! Number of normal streams
	ULONG					m_lNbColorStreams;				//! Number of color streams
	ULONG					m_lNbTexCoordStreams;			//! Number of texture coordinates streams

															
} XSI_RTS_TriangulatedGeometry;

typedef struct tagXSI_RTS_SasCamera
{
    float m_WorldToView[4][4];	   
    float m_Projection[4][4];    
    float m_NearFarClipping[2]; 
    float m_Position[3];
} XSI_RTS_SasCamera;

typedef struct tagXSI_RTS_SasAmbientLight
{
    float m_Color[3];         // default: {0, 0, 0}
} XSI_RTS_SasAmbientLight;

typedef struct tagXSI_RTS_SasDirectionalLight
{
    float m_Color[3];         // default: {0, 0, 0}
    float m_Direction[3];     // default: {0, 0, 0}
} XSI_RTS_SasDirectionalLight;

typedef struct tagXSI_RTS_SasPointLight
{
    float m_Color[3];         // default: {0, 0, 0}
    float m_Position[3];      // default: {0, 0, 0}
    float m_Range;            // default: 0
} XSI_RTS_SasPointLight;

typedef struct tagXSI_RTS_SasSpotLight
{
    float m_Color[3];         // default: {0, 0, 0}
    float m_Position[3];      // default: {0, 0, 0}
    float m_Direction[3];     // default: {0, 0, 0}
    float m_Range;            // default: 0
    float m_Theta;            // default: 0
    float m_Phi;              // default: 0
} XSI_RTS_SasSpotLight;

#define MAX_SAS_LIGHTS	256

typedef struct tagXSI_RTS_FXSemantics
{
	// the matrices
	float	m_World[4][4];
	float	m_WorldI[4][4];
	float	m_WorldT[4][4];
	float	m_WorldIT[4][4];

	float	m_WorldView[4][4];
	float	m_WorldViewI[4][4];
	float	m_WorldViewT[4][4];
	float	m_WorldViewIT[4][4];

	float	m_WorldViewProj[4][4];
	float	m_WorldViewProjI[4][4];
	float	m_WorldViewProjT[4][4];
	float	m_WorldViewProjIT[4][4];

	float	m_View[4][4];
	float	m_ViewI[4][4];
	float	m_ViewT[4][4];
	float	m_ViewIT[4][4];

	float	m_Proj[4][4];
	float	m_ProjI[4][4];
	float	m_ProjT[4][4];
	float	m_ProjIT[4][4];

	// vectors
	float	m_CameraPos[4];
	float	m_CameraNearFar[4];

	// scalars
	float	m_TimeNow[1];
	float	m_TimeLast[1];
	float	m_TimeFrameNumber[1];
	float	m_NumAmbientLights[1];
	float	m_NumDirectionalLights[1];
	float	m_NumPointLights[1];
	float	m_NumSpotLights[1];

	// structures
	XSI_RTS_SasCamera				m_SasCamera[1];
	XSI_RTS_SasAmbientLight			m_AmbientLights[MAX_SAS_LIGHTS];
	XSI_RTS_SasDirectionalLight		m_DirectionalLights[MAX_SAS_LIGHTS];
	XSI_RTS_SasPointLight			m_PointLights[MAX_SAS_LIGHTS];
	XSI_RTS_SasSpotLight			m_SpotLights[MAX_SAS_LIGHTS];

} XSI_RTS_FXSemantics;

typedef struct tagXSI_RTS_FXSemantics_Param
{
	void*	m_pSourceParam;
	void*	m_pDestParam;
} XSI_RTS_FXSemantics_Param;

typedef struct tagXSI_RTS_FXSemantics_inuse
{
	// the matrices
	bool	m_World;
	bool	m_WorldI;
	bool	m_WorldT;
	bool	m_WorldIT;

	bool	m_WorldView;
	bool	m_WorldViewI;
	bool	m_WorldViewT;
	bool	m_WorldViewIT;

	bool	m_WorldViewProj;
	bool	m_WorldViewProjI;
	bool	m_WorldViewProjT;
	bool	m_WorldViewProjIT;

	bool	m_View;
	bool	m_ViewI;
	bool	m_ViewT;
	bool	m_ViewIT;

	bool	m_Proj;
	bool	m_ProjI;
	bool	m_ProjT;
	bool	m_ProjIT;

	// vectors
	bool	m_CameraPos;
	bool	m_CameraNearFar;

	// scalars
	bool	m_TimeNow;
	bool	m_TimeLast;
	bool	m_TimeFrameNumber;
	bool	m_NumAmbientLights;
	bool	m_NumDirectionalLights;
	bool	m_NumPointLights;
	bool	m_NumSpotLights;

	// structures
	bool	m_SasCamera;
	bool	m_AmbientLights;
	bool	m_DirectionalLights;
	bool	m_PointLights;
	bool	m_SpotLights;

	// last time
	float	m_LastTime;
	
	// table of parameters to bind
	CSIBCArray<XSI_RTS_FXSemantics_Param>	m_Params;

} XSI_RTS_FXSemantics_inuse;

//***************************************************************************************
// Entry points
//***************************************************************************************

//***************************************************************************************
//	PostFix		: _Init 
//	Called		: when the shader is created
//  Mandatory	: No
//
//***************************************************************************************
typedef 	RTSHADER_API bool (*RTS_Shader2_Init)		
(
	XSI_RTS_SystemParams			*in_pSysParams,					//! The system parameters
	void							*in_pParams,					//! The property page parameters
	void							**io_pShaderInstanceData		//! The shader instance data
);

//***************************************************************************************
//	PostFix		: _Term
//	Called		: when the shader is deleted
//  Mandatory	: No
//
//***************************************************************************************
typedef 	RTSHADER_API bool (*RTS_Shader2_Term)		
(
	XSI_RTS_SystemParams			*in_pSysParams,					//! The system parameters
	void							*in_pParams,					//! The property page parameters
	void							**io_pShaderInstanceData		//! The shader instance data
);

//***************************************************************************************
//	PostFix		: _GetRequirements
//	Called		: At various places
//  Mandatory	: No
//
//***************************************************************************************
typedef 	RTSHADER_API bool (*RTS_Shader2_GetRequirements)		
(
	XSI_RTS_SystemParams			*in_pSysParams,					//! The system parameters
	void							*in_pParams,					//! The property page parameters
	XSI_RTS_TriangulatedGeometry	*in_pGeom,						//! The geometry information
	void							**io_pShaderInstanceData,		//! The shader instance data
	ULONG					in_lRequirement,				//! The requirement identifier
	ULONG					*out_pRequirementValue			//! The returned requirement value
);

//***************************************************************************************
//	PostFix		: _Execute
//	Called		: on every refresh for every object that own the shader
//  Mandatory	: Yes
//
//***************************************************************************************
typedef 	RTSHADER_API bool (*RTS_Shader2_Execute)		
(
	XSI_RTS_SystemParams			*in_pSysParams,					//! The system parameters
	void							*in_pParams,					//! The property page parameters
	XSI_RTS_TriangulatedGeometry	*in_pGeom,						//! The geometry information
	void							**io_pShaderInstanceData		//! The shader instance data
);

//***************************************************************************************
//	Called		: one per DLL
//  Mandatory	: Yes
//
//***************************************************************************************
typedef 	RTSHADER_API ULONG (*RTS_Shader2_GetVersion)();

enum XSI_RTS_PPG_TYPES
{
	XSI_RTS_SCALAR,
	XSI_RTS_STRING,
	XSI_RTS_IMAGE,
	XSI_RTS_TSPACE,
	XSI_RTS_CSPACE,
};

typedef struct
{
	char				*m_pName;
	XSI_RTS_PPG_TYPES	m_type;
} ShaderParamDefinition;

typedef struct
{
	char					*m_pProgID;
	char					*m_pModule;
	char					*m_pPrefix;
	int						m_pNbParams;
	ShaderParamDefinition	*m_ShaderParams;
} ShaderDefinition;

typedef		RTSHADER_API int (*RTS2_GetNumberOfShadersFunc)();
typedef		RTSHADER_API ShaderDefinition * (*RTS2_GetShaderDefinitionFunc)(int in_nIndex);


//***************************************************************************************
// Inlined functionality
//***************************************************************************************

#define XSI_RTS_EPS  	1.0e-3

inline void   XSI_RTS_MatrixTranspose(float in_pSrcM4x4[4][4], float in_pDstM4x4[4][4])
{
	float	tmpMatrix[4][4];

	memcpy(tmpMatrix, in_pSrcM4x4, sizeof(float) * 16);
	in_pDstM4x4[0][0] = tmpMatrix[0][0];
	in_pDstM4x4[0][1] = tmpMatrix[1][0];
	in_pDstM4x4[0][2] = tmpMatrix[2][0];
	in_pDstM4x4[0][3] = tmpMatrix[3][0];

	in_pDstM4x4[1][0] = tmpMatrix[0][1];
	in_pDstM4x4[1][1] = tmpMatrix[1][1];
	in_pDstM4x4[1][2] = tmpMatrix[2][1];
	in_pDstM4x4[1][3] = tmpMatrix[3][1];

	in_pDstM4x4[2][0] = tmpMatrix[0][2];
	in_pDstM4x4[2][1] = tmpMatrix[1][2];
	in_pDstM4x4[2][2] = tmpMatrix[2][2];
	in_pDstM4x4[2][3] = tmpMatrix[3][2];

	in_pDstM4x4[3][0] = tmpMatrix[0][3];
	in_pDstM4x4[3][1] = tmpMatrix[1][3];
	in_pDstM4x4[3][2] = tmpMatrix[2][3];
	in_pDstM4x4[3][3] = tmpMatrix[3][3];
}

inline bool   XSI_RTS_MatrixInvert(float in_pSrcM4x4[4][4], float in_pDstM4x4[4][4])
{

	float   sys[ 4 ][ 8 ] = 
	{
		1,0,0,0,1,0,0,0,
		0,1,0,0,0,1,0,0,
		0,0,1,0,0,0,1,0,
		0,0,0,1,0,0,0,1
	};

	int		i,  j,  k;
	float   temp;


	memcpy(sys[0], in_pSrcM4x4[0], sizeof(float) * 4);
	memcpy(sys[1], in_pSrcM4x4[1], sizeof(float) * 4);
	memcpy(sys[2], in_pSrcM4x4[2], sizeof(float) * 4);
	memcpy(sys[3], in_pSrcM4x4[3], sizeof(float) * 4);

	for( j = 0; j < 4; j++ )
	{
		if( fabs( sys[ j ][ j ] ) < XSI_RTS_EPS )
		{
			for( i = j + 1; ( i < 4 ) && ( fabs ( sys[ i ][ j ] ) < XSI_RTS_EPS ); i++ );

			if( i == 4 )
				return( false );
			else
				for( k = 0; k < 8; k++ )
				{
					temp = sys[ i ][ k ];
					sys[ i ][ k ] = sys[ j ][ k ];
					sys[ j ][ k ] = temp;
				}
		}

		temp = 1.0f / sys[ j ][ j ];

		for( i = 0; i < 8; i++ )
			sys[ j ][ i ] *= temp;

		for( i = 0; i < 4; i++ )
		{
			if( i != j )
			{
				temp = - sys[ i ][ j ];
				for( k = 0; k < 8; k++ )
				{
					sys[ i ][ k ] += sys[ j ][ k ] * temp;
				}
			}
		}
	}

	memcpy(in_pDstM4x4[0], &(sys[0][4]), sizeof(float) * 4);
	memcpy(in_pDstM4x4[1], &(sys[1][4]), sizeof(float) * 4);
	memcpy(in_pDstM4x4[2], &(sys[2][4]), sizeof(float) * 4);
	memcpy(in_pDstM4x4[3], &(sys[3][4]), sizeof(float) * 4);

	return true;
}

inline void	XSI_RTS_MatrixMultiply(float in_pSrc1M4x4[4][4], float in_pSrc2M4x4[4][4], float in_pDstM4x4[4][4])
{
	float *a = (float*)in_pSrc2M4x4,*b = (float*)in_pSrc1M4x4,*c = (float*)in_pDstM4x4;

	*c = b[0]*a[0] + b[1]*a[4] + b[2]*a[8] + b[3]*a[12];   c++;
	*c = b[0]*a[1] + b[1]*a[5] + b[2]*a[9] + b[3]*a[13];   c++;
	*c = b[0]*a[2] + b[1]*a[6] + b[2]*a[10] + b[3]*a[14];   c++;
	*c = b[0]*a[3] + b[1]*a[7] + b[2]*a[11] + b[3]*a[15];   c++;
	
	*c = b[4]*a[0] + b[5]*a[4] + b[6]*a[8] + b[7]*a[12];   c++;
	*c = b[4]*a[1] + b[5]*a[5] + b[6]*a[9] + b[7]*a[13];   c++;
	*c = b[4]*a[2] + b[5]*a[6] + b[6]*a[10] + b[7]*a[14];   c++;
	*c = b[4]*a[3] + b[5]*a[7] + b[6]*a[11] + b[7]*a[15];   c++;
	
	*c = b[8]*a[0] + b[9]*a[4] + b[10]*a[8] + b[11]*a[12];   c++;
	*c = b[8]*a[1] + b[9]*a[5] + b[10]*a[9] + b[11]*a[13];   c++;
	*c = b[8]*a[2] + b[9]*a[6] + b[10]*a[10] + b[11]*a[14];   c++;
	*c = b[8]*a[3] + b[9]*a[7] + b[10]*a[11] + b[11]*a[15];   c++;
	
	*c = b[12]*a[0] + b[13]*a[4] + b[14]*a[8] + b[15]*a[12];   c++;
	*c = b[12]*a[1] + b[13]*a[5] + b[14]*a[9] + b[15]*a[13];   c++;
	*c = b[12]*a[2] + b[13]*a[6] + b[14]*a[10] + b[15]*a[14];   c++;
	*c = b[12]*a[3] + b[13]*a[7] + b[14]*a[11] + b[15]*a[15];   
}

inline void XSI_RTS_InitializeSemantics_inuse(XSI_RTS_FXSemantics_inuse& in_Data)
{
	memset(&in_Data, 0, sizeof(XSI_RTS_FXSemantics_inuse) - sizeof(in_Data.m_Params));
	in_Data.m_Params.DisposeData();
}

inline void XSI_RTS_InitializeSemantics(XSI_RTS_FXSemantics& in_Data)
{
	memset(&in_Data, 0, sizeof(XSI_RTS_FXSemantics));
}


inline bool XSI_RTS_RegisterSemanticParam
(
	XSI_RTS_FXSemantics_inuse&	in_SemanticInUse,
	void*						in_pSrcParam,
	void*						in_pDstParam,
	bool&						in_pComputeFlag
)
{
	LONG index;
	
	in_SemanticInUse.m_Params.Extend(1);
	index = in_SemanticInUse.m_Params.GetUsed()-1;
	in_SemanticInUse.m_Params[index].m_pSourceParam = in_pSrcParam;
	in_SemanticInUse.m_Params[index].m_pDestParam = in_pDstParam;
	in_pComputeFlag = true;
	return true;
}

inline bool XSI_RTS_RegisterSemantics
(
	XSI_RTS_FXSemantics&		in_SemanticData, 
	XSI_RTS_FXSemantics_inuse&	in_SemanticInUse, 
	char*						in_pSemanticName,
	void*						in_pParameterHandle
)
{
	bool l_bRetValue = true;

	// big if cascade, should be easy to add stuff in there

	if(strnicmp(in_pSemanticName, "Sas.Skeleton.MeshToJointToWorld", strlen("Sas.Skeleton.MeshToJointToWorld")) == 0)
	{
		XSI_RTS_RegisterSemanticParam(in_SemanticInUse, &in_SemanticData.m_World, in_pParameterHandle, in_SemanticInUse.m_World);	
	}
	else if(stricmp(in_pSemanticName, "world") == 0)
	{
		XSI_RTS_RegisterSemanticParam(in_SemanticInUse, &in_SemanticData.m_World, in_pParameterHandle, in_SemanticInUse.m_World);	
	}
	else if(stricmp(in_pSemanticName, "worldi") == 0)
	{
		XSI_RTS_RegisterSemanticParam(in_SemanticInUse, &in_SemanticData.m_WorldI, in_pParameterHandle, in_SemanticInUse.m_WorldI);	
	}
	else if(stricmp(in_pSemanticName, "worldt") == 0)
	{
		XSI_RTS_RegisterSemanticParam(in_SemanticInUse, &in_SemanticData.m_WorldT, in_pParameterHandle, in_SemanticInUse.m_WorldT);	
	}
	else if(stricmp(in_pSemanticName, "worldit") == 0)
	{
		XSI_RTS_RegisterSemanticParam(in_SemanticInUse, &in_SemanticData.m_WorldIT, in_pParameterHandle, in_SemanticInUse.m_WorldIT);	
	}
	else if(stricmp(in_pSemanticName, "worldinverse") == 0)
	{
		XSI_RTS_RegisterSemanticParam(in_SemanticInUse, &in_SemanticData.m_WorldI, in_pParameterHandle, in_SemanticInUse.m_WorldI);	
	}
	else if(stricmp(in_pSemanticName, "worldtranspose") == 0)
	{
		XSI_RTS_RegisterSemanticParam(in_SemanticInUse, &in_SemanticData.m_WorldT, in_pParameterHandle, in_SemanticInUse.m_WorldT);	
	}
	else if(stricmp(in_pSemanticName, "worldinversetranspose") == 0)
	{
		XSI_RTS_RegisterSemanticParam(in_SemanticInUse, &in_SemanticData.m_WorldIT, in_pParameterHandle, in_SemanticInUse.m_WorldIT);	
	}
	else if(stricmp(in_pSemanticName, "model") == 0)
	{
		XSI_RTS_RegisterSemanticParam(in_SemanticInUse, &in_SemanticData.m_World, in_pParameterHandle, in_SemanticInUse.m_World);	
	}
	else if(stricmp(in_pSemanticName, "modeli") == 0)
	{
		XSI_RTS_RegisterSemanticParam(in_SemanticInUse, &in_SemanticData.m_WorldI, in_pParameterHandle, in_SemanticInUse.m_WorldI);	
	}
	else if(stricmp(in_pSemanticName, "modelt") == 0)
	{
		XSI_RTS_RegisterSemanticParam(in_SemanticInUse, &in_SemanticData.m_WorldT, in_pParameterHandle, in_SemanticInUse.m_WorldT);	
	}
	else if(stricmp(in_pSemanticName, "modelit") == 0)
	{
		XSI_RTS_RegisterSemanticParam(in_SemanticInUse, &in_SemanticData.m_WorldIT, in_pParameterHandle, in_SemanticInUse.m_WorldIT);	
	}
	else if(stricmp(in_pSemanticName, "modelinverse") == 0)
	{
		XSI_RTS_RegisterSemanticParam(in_SemanticInUse, &in_SemanticData.m_WorldI, in_pParameterHandle, in_SemanticInUse.m_WorldI);	
	}
	else if(stricmp(in_pSemanticName, "modeltranspose") == 0)
	{
		XSI_RTS_RegisterSemanticParam(in_SemanticInUse, &in_SemanticData.m_WorldT, in_pParameterHandle, in_SemanticInUse.m_WorldT);	
	}
	else if(stricmp(in_pSemanticName, "modelinversetranspose") == 0)
	{
		XSI_RTS_RegisterSemanticParam(in_SemanticInUse, &in_SemanticData.m_WorldIT, in_pParameterHandle, in_SemanticInUse.m_WorldIT);	
	}
	else if(stricmp(in_pSemanticName, "simodel") == 0)
	{
		XSI_RTS_RegisterSemanticParam(in_SemanticInUse, &in_SemanticData.m_World, in_pParameterHandle, in_SemanticInUse.m_World);	
	}
	else if(stricmp(in_pSemanticName, "simodeli") == 0)
	{
		XSI_RTS_RegisterSemanticParam(in_SemanticInUse, &in_SemanticData.m_WorldI, in_pParameterHandle, in_SemanticInUse.m_WorldI);	
	}
	else if(stricmp(in_pSemanticName, "simodelt") == 0)
	{
		XSI_RTS_RegisterSemanticParam(in_SemanticInUse, &in_SemanticData.m_WorldT, in_pParameterHandle, in_SemanticInUse.m_WorldT);	
	}
	else if(stricmp(in_pSemanticName, "simodelit") == 0)
	{
		XSI_RTS_RegisterSemanticParam(in_SemanticInUse, &in_SemanticData.m_WorldIT, in_pParameterHandle, in_SemanticInUse.m_WorldIT);	
	}
	else if(stricmp(in_pSemanticName, "view") == 0)
	{
		XSI_RTS_RegisterSemanticParam(in_SemanticInUse, &in_SemanticData.m_View, in_pParameterHandle, in_SemanticInUse.m_View);	
	}
	else if(stricmp(in_pSemanticName, "viewi") == 0)
	{
		XSI_RTS_RegisterSemanticParam(in_SemanticInUse, &in_SemanticData.m_ViewI, in_pParameterHandle, in_SemanticInUse.m_ViewI);	
	}
	else if(stricmp(in_pSemanticName, "viewt") == 0)
	{
		XSI_RTS_RegisterSemanticParam(in_SemanticInUse, &in_SemanticData.m_ViewT, in_pParameterHandle, in_SemanticInUse.m_ViewT);	
	}
	else if(stricmp(in_pSemanticName, "viewit") == 0)
	{
		XSI_RTS_RegisterSemanticParam(in_SemanticInUse, &in_SemanticData.m_ViewIT, in_pParameterHandle, in_SemanticInUse.m_ViewIT);	
	}
	else if(stricmp(in_pSemanticName, "viewinverse") == 0)
	{
		XSI_RTS_RegisterSemanticParam(in_SemanticInUse, &in_SemanticData.m_ViewI, in_pParameterHandle, in_SemanticInUse.m_ViewI);	
	}
	else if(stricmp(in_pSemanticName, "viewtranspose") == 0)
	{
		XSI_RTS_RegisterSemanticParam(in_SemanticInUse, &in_SemanticData.m_ViewT, in_pParameterHandle, in_SemanticInUse.m_ViewT);	
	}
	else if(stricmp(in_pSemanticName, "viewinversetranspose") == 0)
	{
		XSI_RTS_RegisterSemanticParam(in_SemanticInUse, &in_SemanticData.m_ViewIT, in_pParameterHandle, in_SemanticInUse.m_ViewIT);	
	}
	else if(stricmp(in_pSemanticName, "siview") == 0)
	{
		XSI_RTS_RegisterSemanticParam(in_SemanticInUse, &in_SemanticData.m_View, in_pParameterHandle, in_SemanticInUse.m_View);	
	}
	else if(stricmp(in_pSemanticName, "siviewi") == 0)
	{
		XSI_RTS_RegisterSemanticParam(in_SemanticInUse, &in_SemanticData.m_ViewI, in_pParameterHandle, in_SemanticInUse.m_ViewI);	
	}
	else if(stricmp(in_pSemanticName, "siviewt") == 0)
	{
		XSI_RTS_RegisterSemanticParam(in_SemanticInUse, &in_SemanticData.m_ViewT, in_pParameterHandle, in_SemanticInUse.m_ViewT);	
	}
	else if(stricmp(in_pSemanticName, "siviewit") == 0)
	{
		XSI_RTS_RegisterSemanticParam(in_SemanticInUse, &in_SemanticData.m_ViewIT, in_pParameterHandle, in_SemanticInUse.m_ViewIT);	
	}
	else if(stricmp(in_pSemanticName, "Sas.Camera.Projection") == 0)
	{
		XSI_RTS_RegisterSemanticParam(in_SemanticInUse, &in_SemanticData.m_Proj, in_pParameterHandle, in_SemanticInUse.m_Proj);	
	}
	else if(stricmp(in_pSemanticName, "projection") == 0)
	{
		XSI_RTS_RegisterSemanticParam(in_SemanticInUse, &in_SemanticData.m_Proj, in_pParameterHandle, in_SemanticInUse.m_Proj);	
	}
	else if(stricmp(in_pSemanticName, "projectioni") == 0)
	{
		XSI_RTS_RegisterSemanticParam(in_SemanticInUse, &in_SemanticData.m_ProjI, in_pParameterHandle, in_SemanticInUse.m_ProjI);	
	}
	else if(stricmp(in_pSemanticName, "projectiont") == 0)
	{
		XSI_RTS_RegisterSemanticParam(in_SemanticInUse, &in_SemanticData.m_ProjT, in_pParameterHandle, in_SemanticInUse.m_ProjT);	
	}
	else if(stricmp(in_pSemanticName, "projectionit") == 0)
	{
		XSI_RTS_RegisterSemanticParam(in_SemanticInUse, &in_SemanticData.m_ProjIT, in_pParameterHandle, in_SemanticInUse.m_ProjIT);	
	}
	else if(stricmp(in_pSemanticName, "projectioninverse") == 0)
	{
		XSI_RTS_RegisterSemanticParam(in_SemanticInUse, &in_SemanticData.m_ProjI, in_pParameterHandle, in_SemanticInUse.m_ProjI);	
	}
	else if(stricmp(in_pSemanticName, "projectiontranspose") == 0)
	{
		XSI_RTS_RegisterSemanticParam(in_SemanticInUse, &in_SemanticData.m_ProjT, in_pParameterHandle, in_SemanticInUse.m_ProjT);	
	}
	else if(stricmp(in_pSemanticName, "projectioninversetranspose") == 0)
	{
		XSI_RTS_RegisterSemanticParam(in_SemanticInUse, &in_SemanticData.m_ProjIT, in_pParameterHandle, in_SemanticInUse.m_ProjIT);	
	}
	else if(stricmp(in_pSemanticName, "siproj") == 0)
	{
		XSI_RTS_RegisterSemanticParam(in_SemanticInUse, &in_SemanticData.m_Proj, in_pParameterHandle, in_SemanticInUse.m_Proj);	
	}
	else if(stricmp(in_pSemanticName, "siproji") == 0)
	{
		XSI_RTS_RegisterSemanticParam(in_SemanticInUse, &in_SemanticData.m_ProjI, in_pParameterHandle, in_SemanticInUse.m_ProjI);	
	}
	else if(stricmp(in_pSemanticName, "siprojt") == 0)
	{
		XSI_RTS_RegisterSemanticParam(in_SemanticInUse, &in_SemanticData.m_ProjT, in_pParameterHandle, in_SemanticInUse.m_ProjT);	
	}
	else if(stricmp(in_pSemanticName, "siprojit") == 0)
	{
		XSI_RTS_RegisterSemanticParam(in_SemanticInUse, &in_SemanticData.m_ProjIT, in_pParameterHandle, in_SemanticInUse.m_ProjIT);	
	}
	else if(stricmp(in_pSemanticName, "Sas.Camera.WorldToView") == 0)
	{
		XSI_RTS_RegisterSemanticParam(in_SemanticInUse, &in_SemanticData.m_WorldView, in_pParameterHandle, in_SemanticInUse.m_WorldView);	
	}
	else if(stricmp(in_pSemanticName, "worldview") == 0)
	{
		XSI_RTS_RegisterSemanticParam(in_SemanticInUse, &in_SemanticData.m_WorldView, in_pParameterHandle, in_SemanticInUse.m_WorldView);	
	}
	else if(stricmp(in_pSemanticName, "worldviewi") == 0)
	{
		XSI_RTS_RegisterSemanticParam(in_SemanticInUse, &in_SemanticData.m_WorldViewI, in_pParameterHandle, in_SemanticInUse.m_WorldViewI);	
	}
	else if(stricmp(in_pSemanticName, "worldviewt") == 0)
	{
		XSI_RTS_RegisterSemanticParam(in_SemanticInUse, &in_SemanticData.m_WorldViewT, in_pParameterHandle, in_SemanticInUse.m_WorldViewT);	
	}
	else if(stricmp(in_pSemanticName, "worldviewit") == 0)
	{
		XSI_RTS_RegisterSemanticParam(in_SemanticInUse, &in_SemanticData.m_WorldViewIT, in_pParameterHandle, in_SemanticInUse.m_WorldViewIT);	
	}
	else if(stricmp(in_pSemanticName, "worldviewinverse") == 0)
	{
		XSI_RTS_RegisterSemanticParam(in_SemanticInUse, &in_SemanticData.m_WorldViewI, in_pParameterHandle, in_SemanticInUse.m_WorldViewI);	
	}
	else if(stricmp(in_pSemanticName, "worldviewtranspose") == 0)
	{
		XSI_RTS_RegisterSemanticParam(in_SemanticInUse, &in_SemanticData.m_WorldViewT, in_pParameterHandle, in_SemanticInUse.m_WorldViewT);	
	}
	else if(stricmp(in_pSemanticName, "worldviewinversetranspose") == 0)
	{
		XSI_RTS_RegisterSemanticParam(in_SemanticInUse, &in_SemanticData.m_WorldViewIT, in_pParameterHandle, in_SemanticInUse.m_WorldViewIT);	
	}
	else if(stricmp(in_pSemanticName, "modelview") == 0)
	{
		XSI_RTS_RegisterSemanticParam(in_SemanticInUse, &in_SemanticData.m_WorldView, in_pParameterHandle, in_SemanticInUse.m_WorldView);	
	}
	else if(stricmp(in_pSemanticName, "modelviewi") == 0)
	{
		XSI_RTS_RegisterSemanticParam(in_SemanticInUse, &in_SemanticData.m_WorldViewI, in_pParameterHandle, in_SemanticInUse.m_WorldViewI);	
	}
	else if(stricmp(in_pSemanticName, "modelviewt") == 0)
	{
		XSI_RTS_RegisterSemanticParam(in_SemanticInUse, &in_SemanticData.m_WorldViewT, in_pParameterHandle, in_SemanticInUse.m_WorldViewT);	
	}
	else if(stricmp(in_pSemanticName, "modelviewit") == 0)
	{
		XSI_RTS_RegisterSemanticParam(in_SemanticInUse, &in_SemanticData.m_WorldViewIT, in_pParameterHandle, in_SemanticInUse.m_WorldViewIT);	
	}
	else if(stricmp(in_pSemanticName, "modelviewinverse") == 0)
	{
		XSI_RTS_RegisterSemanticParam(in_SemanticInUse, &in_SemanticData.m_WorldViewI, in_pParameterHandle, in_SemanticInUse.m_WorldViewI);	
	}
	else if(stricmp(in_pSemanticName, "modelviewtranspose") == 0)
	{
		XSI_RTS_RegisterSemanticParam(in_SemanticInUse, &in_SemanticData.m_WorldViewT, in_pParameterHandle, in_SemanticInUse.m_WorldViewT);	
	}
	else if(stricmp(in_pSemanticName, "modelviewinversetranspose") == 0)
	{
		XSI_RTS_RegisterSemanticParam(in_SemanticInUse, &in_SemanticData.m_WorldViewIT, in_pParameterHandle, in_SemanticInUse.m_WorldViewIT);	
	}
	else if(stricmp(in_pSemanticName, "simodelview") == 0)
	{
		XSI_RTS_RegisterSemanticParam(in_SemanticInUse, &in_SemanticData.m_WorldView, in_pParameterHandle, in_SemanticInUse.m_WorldView);	
	}
	else if(stricmp(in_pSemanticName, "simodelviewi") == 0)
	{
		XSI_RTS_RegisterSemanticParam(in_SemanticInUse, &in_SemanticData.m_WorldViewI, in_pParameterHandle, in_SemanticInUse.m_WorldViewI);	
	}
	else if(stricmp(in_pSemanticName, "simodelviewt") == 0)
	{
		XSI_RTS_RegisterSemanticParam(in_SemanticInUse, &in_SemanticData.m_WorldViewT, in_pParameterHandle, in_SemanticInUse.m_WorldViewT);	
	}
	else if(stricmp(in_pSemanticName, "simodelviewit") == 0)
	{
		XSI_RTS_RegisterSemanticParam(in_SemanticInUse, &in_SemanticData.m_WorldViewIT, in_pParameterHandle, in_SemanticInUse.m_WorldViewIT);	
	}
	else if(stricmp(in_pSemanticName, "worldviewprojection") == 0)
	{
		XSI_RTS_RegisterSemanticParam(in_SemanticInUse, &in_SemanticData.m_WorldViewProj, in_pParameterHandle, in_SemanticInUse.m_WorldViewProj);	
	}
	else if(stricmp(in_pSemanticName, "worldviewprojectioni") == 0)
	{
		XSI_RTS_RegisterSemanticParam(in_SemanticInUse, &in_SemanticData.m_WorldViewProjI, in_pParameterHandle, in_SemanticInUse.m_WorldViewProjI);	
	}
	else if(stricmp(in_pSemanticName, "worldviewprojectiont") == 0)
	{
		XSI_RTS_RegisterSemanticParam(in_SemanticInUse, &in_SemanticData.m_WorldViewProjT, in_pParameterHandle, in_SemanticInUse.m_WorldViewProjT);	
	}
	else if(stricmp(in_pSemanticName, "worldviewprojectionit") == 0)
	{
		XSI_RTS_RegisterSemanticParam(in_SemanticInUse, &in_SemanticData.m_WorldViewProjIT, in_pParameterHandle, in_SemanticInUse.m_WorldViewProjIT);	
	}
	else if(stricmp(in_pSemanticName, "worldviewprojectioninverse") == 0)
	{
		XSI_RTS_RegisterSemanticParam(in_SemanticInUse, &in_SemanticData.m_WorldViewProjI, in_pParameterHandle, in_SemanticInUse.m_WorldViewProjI);	
	}
	else if(stricmp(in_pSemanticName, "worldviewprojectiontranspose") == 0)
	{
		XSI_RTS_RegisterSemanticParam(in_SemanticInUse, &in_SemanticData.m_WorldViewProjT, in_pParameterHandle, in_SemanticInUse.m_WorldViewProjT);	
	}
	else if(stricmp(in_pSemanticName, "worldviewprojectioninversetranspose") == 0)
	{
		XSI_RTS_RegisterSemanticParam(in_SemanticInUse, &in_SemanticData.m_WorldViewProjIT, in_pParameterHandle, in_SemanticInUse.m_WorldViewProjIT);	
	}
	else if(stricmp(in_pSemanticName, "modelviewprojection") == 0)
	{
		XSI_RTS_RegisterSemanticParam(in_SemanticInUse, &in_SemanticData.m_WorldViewProj, in_pParameterHandle, in_SemanticInUse.m_WorldViewProj);	
	}
	else if(stricmp(in_pSemanticName, "modelviewprojectioni") == 0)
	{
		XSI_RTS_RegisterSemanticParam(in_SemanticInUse, &in_SemanticData.m_WorldViewProjI, in_pParameterHandle, in_SemanticInUse.m_WorldViewProjI);	
	}
	else if(stricmp(in_pSemanticName, "modelviewprojectiont") == 0)
	{
		XSI_RTS_RegisterSemanticParam(in_SemanticInUse, &in_SemanticData.m_WorldViewProjT, in_pParameterHandle, in_SemanticInUse.m_WorldViewProjT);	
	}
	else if(stricmp(in_pSemanticName, "modelviewprojectionit") == 0)
	{
		XSI_RTS_RegisterSemanticParam(in_SemanticInUse, &in_SemanticData.m_WorldViewProjIT, in_pParameterHandle, in_SemanticInUse.m_WorldViewProjIT);	
	}
	else if(stricmp(in_pSemanticName, "modelviewprojectioninverse") == 0)
	{
		XSI_RTS_RegisterSemanticParam(in_SemanticInUse, &in_SemanticData.m_WorldViewProjI, in_pParameterHandle, in_SemanticInUse.m_WorldViewProjI);	
	}
	else if(stricmp(in_pSemanticName, "modelviewprojectiontranspose") == 0)
	{
		XSI_RTS_RegisterSemanticParam(in_SemanticInUse, &in_SemanticData.m_WorldViewProjT, in_pParameterHandle, in_SemanticInUse.m_WorldViewProjT);	
	}
	else if(stricmp(in_pSemanticName, "modelviewprojectioninversetranspose") == 0)
	{
		XSI_RTS_RegisterSemanticParam(in_SemanticInUse, &in_SemanticData.m_WorldViewProjIT, in_pParameterHandle, in_SemanticInUse.m_WorldViewProjIT);	
	}
	else if(stricmp(in_pSemanticName, "simodelviewproj") == 0)
	{
		XSI_RTS_RegisterSemanticParam(in_SemanticInUse, &in_SemanticData.m_WorldViewProj, in_pParameterHandle, in_SemanticInUse.m_WorldViewProj);	
	}
	else if(stricmp(in_pSemanticName, "simodelviewproji") == 0)
	{
		XSI_RTS_RegisterSemanticParam(in_SemanticInUse, &in_SemanticData.m_WorldViewProjI, in_pParameterHandle, in_SemanticInUse.m_WorldViewProjI);	
	}
	else if(stricmp(in_pSemanticName, "simodelviewprojt") == 0)
	{
		XSI_RTS_RegisterSemanticParam(in_SemanticInUse, &in_SemanticData.m_WorldViewProjT, in_pParameterHandle, in_SemanticInUse.m_WorldViewProjT);	
	}
	else if(stricmp(in_pSemanticName, "simodelviewprojit") == 0)
	{
		XSI_RTS_RegisterSemanticParam(in_SemanticInUse, &in_SemanticData.m_WorldViewProjIT, in_pParameterHandle, in_SemanticInUse.m_WorldViewProjIT);	
	}
	else if(stricmp(in_pSemanticName, "cameraposition") == 0)
	{
		XSI_RTS_RegisterSemanticParam(in_SemanticInUse, &in_SemanticData.m_CameraPos, in_pParameterHandle, in_SemanticInUse.m_CameraPos);	
	}
	else if(stricmp(in_pSemanticName, "sieyepos") == 0)
	{
		XSI_RTS_RegisterSemanticParam(in_SemanticInUse, &in_SemanticData.m_CameraPos, in_pParameterHandle, in_SemanticInUse.m_CameraPos);	
	}
	else if(stricmp(in_pSemanticName, "sas.camera.position") == 0)
	{
		XSI_RTS_RegisterSemanticParam(in_SemanticInUse, &in_SemanticData.m_CameraPos, in_pParameterHandle, in_SemanticInUse.m_CameraPos);	
	}
	else if(stricmp(in_pSemanticName, "sas.camera.nearfarclipping") == 0)
	{
		XSI_RTS_RegisterSemanticParam(in_SemanticInUse, &in_SemanticData.m_CameraNearFar, in_pParameterHandle, in_SemanticInUse.m_CameraNearFar);	
	}
	else if(stricmp(in_pSemanticName, "sas.time.now") == 0)
	{
		XSI_RTS_RegisterSemanticParam(in_SemanticInUse, &in_SemanticData.m_TimeNow, in_pParameterHandle, in_SemanticInUse.m_TimeNow);	
	}
	else if(stricmp(in_pSemanticName, "sas.time.last") == 0)
	{
		XSI_RTS_RegisterSemanticParam(in_SemanticInUse, &in_SemanticData.m_TimeLast, in_pParameterHandle, in_SemanticInUse.m_TimeLast);	
	}
	else if(stricmp(in_pSemanticName, "sas.time.framenumber") == 0)
	{
		XSI_RTS_RegisterSemanticParam(in_SemanticInUse, &in_SemanticData.m_TimeFrameNumber, in_pParameterHandle, in_SemanticInUse.m_TimeFrameNumber);	
	}
	else if(stricmp(in_pSemanticName, "sas.numambientlights") == 0)
	{
		XSI_RTS_RegisterSemanticParam(in_SemanticInUse, &in_SemanticData.m_NumAmbientLights, in_pParameterHandle, in_SemanticInUse.m_NumAmbientLights);	
	}
	else if(stricmp(in_pSemanticName, "sas.numdirectionallights") == 0)
	{
		XSI_RTS_RegisterSemanticParam(in_SemanticInUse, &in_SemanticData.m_NumDirectionalLights, in_pParameterHandle, in_SemanticInUse.m_NumDirectionalLights);	
	}
	else if(stricmp(in_pSemanticName, "sas.numpointlights") == 0)
	{
		XSI_RTS_RegisterSemanticParam(in_SemanticInUse, &in_SemanticData.m_NumPointLights, in_pParameterHandle, in_SemanticInUse.m_NumPointLights);	
	}
	else if(stricmp(in_pSemanticName, "sas.numspotlights") == 0)
	{
		XSI_RTS_RegisterSemanticParam(in_SemanticInUse, &in_SemanticData.m_NumSpotLights, in_pParameterHandle, in_SemanticInUse.m_NumSpotLights);	
	}
	else if(stricmp(in_pSemanticName, "sas.camera") == 0)
	{
		XSI_RTS_RegisterSemanticParam(in_SemanticInUse, &in_SemanticData.m_SasCamera, in_pParameterHandle, in_SemanticInUse.m_SasCamera);	
	}
	else if(stricmp(in_pSemanticName, "sas.ambientlight") == 0)
	{
		XSI_RTS_RegisterSemanticParam(in_SemanticInUse, &in_SemanticData.m_AmbientLights, in_pParameterHandle, in_SemanticInUse.m_AmbientLights);	
	}
	else if(strnicmp(in_pSemanticName, "sas.ambientlight", strlen("sas.ambientlight")) == 0)
	{
		char l_IndexText[256];
		char *l_pAddress = NULL;
		
		l_pAddress = strchr(in_pSemanticName, '[');
		if(l_pAddress)
		{
			int l_Index, l_DstIndex = 0;

			l_pAddress ++;
			while((*l_pAddress != 0) && (*l_pAddress >= '0') && (*l_pAddress <= '9'))
			{
				l_IndexText[l_DstIndex ++] = *l_pAddress;
				l_pAddress++;
			}
			l_IndexText[l_DstIndex ++] = 0;

			l_pAddress = strchr(l_pAddress, '.');
			if(l_pAddress)
				l_pAddress++;

			l_Index = atol(l_IndexText);


			if(l_pAddress == NULL)
			{
				XSI_RTS_RegisterSemanticParam(in_SemanticInUse, &in_SemanticData.m_AmbientLights, in_pParameterHandle, in_SemanticInUse.m_AmbientLights);	
			}
			else if(stricmp(l_pAddress, "color") == 0)
			{
				XSI_RTS_RegisterSemanticParam(in_SemanticInUse, &in_SemanticData.m_AmbientLights[l_Index].m_Color, in_pParameterHandle, in_SemanticInUse.m_AmbientLights);	
			}
		}
	}
	else if(stricmp(in_pSemanticName, "sas.directionallight") == 0)
	{
		XSI_RTS_RegisterSemanticParam(in_SemanticInUse, &in_SemanticData.m_DirectionalLights, in_pParameterHandle, in_SemanticInUse.m_DirectionalLights);	
	}
	else if(strnicmp(in_pSemanticName, "sas.directionallight", strlen("sas.directionallight")) == 0)
	{
		char l_IndexText[256];
		char *l_pAddress = NULL;
		
		l_pAddress = strchr(in_pSemanticName, '[');
		if(l_pAddress)
		{
			int l_Index, l_DstIndex = 0;

			l_pAddress ++;
			while((*l_pAddress != 0) && (*l_pAddress >= '0') && (*l_pAddress <= '9'))
			{
				l_IndexText[l_DstIndex ++] = *l_pAddress;
				l_pAddress++;
			}
			l_IndexText[l_DstIndex ++] = 0;

			l_pAddress = strchr(l_pAddress, '.');
			if(l_pAddress)
				l_pAddress++;

			l_Index = atol(l_IndexText);


			if(l_pAddress == NULL)
			{
				XSI_RTS_RegisterSemanticParam(in_SemanticInUse, &in_SemanticData.m_DirectionalLights[l_Index], in_pParameterHandle, in_SemanticInUse.m_DirectionalLights);	
			}
			else if(stricmp(l_pAddress, "color") == 0)
			{
				XSI_RTS_RegisterSemanticParam(in_SemanticInUse, &in_SemanticData.m_DirectionalLights[l_Index].m_Color, in_pParameterHandle, in_SemanticInUse.m_DirectionalLights);	
			}
			else if(stricmp(l_pAddress, "direction") == 0)
			{
				XSI_RTS_RegisterSemanticParam(in_SemanticInUse, &in_SemanticData.m_DirectionalLights[l_Index].m_Direction, in_pParameterHandle, in_SemanticInUse.m_DirectionalLights);	
			}
		}
	}
	else if(stricmp(in_pSemanticName, "sas.pointlight") == 0)
	{
		XSI_RTS_RegisterSemanticParam(in_SemanticInUse, &in_SemanticData.m_PointLights, in_pParameterHandle, in_SemanticInUse.m_PointLights);	
	}
	else if(strnicmp(in_pSemanticName, "sas.pointlight", strlen("sas.pointlight")) == 0)
	{
		char l_IndexText[256];
		char *l_pAddress = NULL;
		
		l_pAddress = strchr(in_pSemanticName, '[');
		if(l_pAddress)
		{
			int l_Index, l_DstIndex = 0;

			l_pAddress ++;
			while((*l_pAddress != 0) && (*l_pAddress >= '0') && (*l_pAddress <= '9'))
			{
				l_IndexText[l_DstIndex ++] = *l_pAddress;
				l_pAddress++;
			}
			l_IndexText[l_DstIndex ++] = 0;

			l_pAddress = strchr(l_pAddress, '.');
			if(l_pAddress)
				l_pAddress++;

			l_Index = atol(l_IndexText);


			if(l_pAddress == NULL)
			{
				XSI_RTS_RegisterSemanticParam(in_SemanticInUse, &in_SemanticData.m_PointLights[l_Index], in_pParameterHandle, in_SemanticInUse.m_PointLights);	
			}
			else if(stricmp(l_pAddress, "color") == 0)
			{
				XSI_RTS_RegisterSemanticParam(in_SemanticInUse, &in_SemanticData.m_PointLights[l_Index].m_Color, in_pParameterHandle, in_SemanticInUse.m_PointLights);	
			}
			else if(stricmp(l_pAddress, "position") == 0)
			{
				XSI_RTS_RegisterSemanticParam(in_SemanticInUse, &in_SemanticData.m_PointLights[l_Index].m_Position, in_pParameterHandle, in_SemanticInUse.m_PointLights);	
			}
			else if(stricmp(l_pAddress, "range") == 0)
			{
				XSI_RTS_RegisterSemanticParam(in_SemanticInUse, &in_SemanticData.m_PointLights[l_Index].m_Range, in_pParameterHandle, in_SemanticInUse.m_PointLights);	
			}
		}
	}
	else if(stricmp(in_pSemanticName, "sas.spotlight") == 0)
	{
		XSI_RTS_RegisterSemanticParam(in_SemanticInUse, &in_SemanticData.m_SpotLights, in_pParameterHandle, in_SemanticInUse.m_SpotLights);	
	}
	else if(strnicmp(in_pSemanticName, "sas.spotlight", strlen("sas.spotlight")) == 0)
	{
		char l_IndexText[256];
		char *l_pAddress = NULL;
		
		l_pAddress = strchr(in_pSemanticName, '[');
		if(l_pAddress)
		{
			int l_Index, l_DstIndex = 0;

			l_pAddress ++;
			while((*l_pAddress != 0) && (*l_pAddress >= '0') && (*l_pAddress <= '9'))
			{
				l_IndexText[l_DstIndex ++] = *l_pAddress;
				l_pAddress++;
			}
			l_IndexText[l_DstIndex ++] = 0;

			l_pAddress = strchr(l_pAddress, '.');
			if(l_pAddress)
				l_pAddress++;

			l_Index = atol(l_IndexText);


			if(l_pAddress == NULL)
			{
				XSI_RTS_RegisterSemanticParam(in_SemanticInUse, &in_SemanticData.m_SpotLights[l_Index], in_pParameterHandle, in_SemanticInUse.m_SpotLights);	
			}
			else if(stricmp(l_pAddress, "color") == 0)
			{
				XSI_RTS_RegisterSemanticParam(in_SemanticInUse, &in_SemanticData.m_SpotLights[l_Index].m_Color, in_pParameterHandle, in_SemanticInUse.m_SpotLights);	
			}
			else if(stricmp(l_pAddress, "position") == 0)
			{
				XSI_RTS_RegisterSemanticParam(in_SemanticInUse, &in_SemanticData.m_SpotLights[l_Index].m_Position, in_pParameterHandle, in_SemanticInUse.m_SpotLights);	
			}
			else if(stricmp(l_pAddress, "direction") == 0)
			{
				XSI_RTS_RegisterSemanticParam(in_SemanticInUse, &in_SemanticData.m_SpotLights[l_Index].m_Direction, in_pParameterHandle, in_SemanticInUse.m_SpotLights);	
			}
			else if(stricmp(l_pAddress, "range") == 0)
			{
				XSI_RTS_RegisterSemanticParam(in_SemanticInUse, &in_SemanticData.m_SpotLights[l_Index].m_Range, in_pParameterHandle, in_SemanticInUse.m_SpotLights);	
			}
			else if(stricmp(l_pAddress, "phi") == 0)
			{
				XSI_RTS_RegisterSemanticParam(in_SemanticInUse, &in_SemanticData.m_SpotLights[l_Index].m_Phi, in_pParameterHandle, in_SemanticInUse.m_SpotLights);	
			}
			else if(stricmp(l_pAddress, "theta") == 0)
			{
				XSI_RTS_RegisterSemanticParam(in_SemanticInUse, &in_SemanticData.m_SpotLights[l_Index].m_Theta, in_pParameterHandle, in_SemanticInUse.m_SpotLights);	
			}
		}
	}
	else if(strnicmp(in_pSemanticName, "sasglobal", strlen("sasglobal")) == 0)
	{

	}
	else
	{
		l_bRetValue = false;
	}

#ifdef _DEBUG	
	if(!l_bRetValue)
	{
		wchar_t l_SemanticW[2048];

		mbstowcs(l_SemanticW, in_pSemanticName, 2048);

#ifdef UNICODE
		OutputDebugString(l_SemanticW);
		OutputDebugString(L" is not supported \n");
#else 
		OutputDebugString(in_pSemanticName);
		OutputDebugString(" is not supported \n");
#endif
	}
#endif

	return l_bRetValue;
}

typedef 	void (*RTS_BindSemanticCallback)		
(
	void*		in_pContext,
	void*		in_pParameterHandle,
	void*		in_pValue
);

inline void XSI_RTS_BindCommonSemantics
(
	XSI_RTS_FXSemantics&		in_SemanticData, 
	XSI_RTS_FXSemantics_inuse&	in_SemanticInUse, 
	XSI_RTS_SystemParams*		in_pSysParams,
	void*						in_pContext,
	RTS_BindSemanticCallback	fnBind
)
{

	memcpy(in_SemanticData.m_ViewI, in_pSysParams->m_SceneData->m_sCamera.m_mMatrix, sizeof(in_SemanticData.m_ViewI));
	XSI_RTS_MatrixInvert(in_SemanticData.m_ViewI, in_SemanticData.m_View);

	memcpy(in_SemanticData.m_World, in_pSysParams->m_m4x4ObjectPose, sizeof(in_SemanticData.m_World));

	// compute these matrices
	XSI_RTS_MatrixMultiply(in_SemanticData.m_World, in_SemanticData.m_View, in_SemanticData.m_WorldView);
	XSI_RTS_MatrixMultiply(in_SemanticData.m_WorldView, in_SemanticData.m_Proj, in_SemanticData.m_WorldViewProj);

	if(in_SemanticInUse.m_WorldI)
	{
		XSI_RTS_MatrixInvert(in_SemanticData.m_World, in_SemanticData.m_WorldI);
	}
	
	if(in_SemanticInUse.m_WorldT)
	{
		XSI_RTS_MatrixTranspose(in_SemanticData.m_World, in_SemanticData.m_WorldT);
	}
	
	if(in_SemanticInUse.m_WorldIT)
	{
		XSI_RTS_MatrixInvert(in_SemanticData.m_World, in_SemanticData.m_WorldI);
		XSI_RTS_MatrixTranspose(in_SemanticData.m_WorldI, in_SemanticData.m_WorldIT);
	}
	
	if(in_SemanticInUse.m_ViewT)
	{
		XSI_RTS_MatrixTranspose(in_SemanticData.m_View, in_SemanticData.m_ViewT);
	}
	
	if(in_SemanticInUse.m_ViewIT)
	{
		XSI_RTS_MatrixTranspose(in_SemanticData.m_ViewI, in_SemanticData.m_ViewIT);
	}

	if(in_SemanticInUse.m_ProjI)
	{
		XSI_RTS_MatrixInvert(in_SemanticData.m_Proj, in_SemanticData.m_ProjI);
	}
	
	if(in_SemanticInUse.m_ProjT)
	{
		XSI_RTS_MatrixTranspose(in_SemanticData.m_Proj, in_SemanticData.m_ProjT);
	}
	
	if(in_SemanticInUse.m_ProjIT)
	{
		XSI_RTS_MatrixInvert(in_SemanticData.m_Proj, in_SemanticData.m_ProjI);
		XSI_RTS_MatrixTranspose(in_SemanticData.m_ProjI, in_SemanticData.m_ProjIT);
	}


	if(in_SemanticInUse.m_WorldViewI)
	{
		XSI_RTS_MatrixInvert(in_SemanticData.m_WorldView, in_SemanticData.m_WorldViewI);
	}
	
	if(in_SemanticInUse.m_WorldViewT)
	{
		XSI_RTS_MatrixTranspose(in_SemanticData.m_WorldView, in_SemanticData.m_WorldViewT);
	}
	
	if(in_SemanticInUse.m_WorldViewIT)
	{
		XSI_RTS_MatrixInvert(in_SemanticData.m_WorldView, in_SemanticData.m_WorldViewI);
		XSI_RTS_MatrixTranspose(in_SemanticData.m_WorldViewI, in_SemanticData.m_WorldViewIT);
	}

	if(in_SemanticInUse.m_WorldViewProjI)
	{
		XSI_RTS_MatrixInvert(in_SemanticData.m_WorldViewProj, in_SemanticData.m_WorldViewProjI);
	}
	
	if(in_SemanticInUse.m_WorldViewProjT)
	{
		XSI_RTS_MatrixTranspose(in_SemanticData.m_WorldViewProj, in_SemanticData.m_WorldViewProjT);
	}
	
	if(in_SemanticInUse.m_WorldViewProjIT)
	{
		XSI_RTS_MatrixInvert(in_SemanticData.m_WorldViewProj, in_SemanticData.m_WorldViewProjI);
		XSI_RTS_MatrixTranspose(in_SemanticData.m_WorldViewProjI, in_SemanticData.m_WorldViewProjIT);
	}

	if( in_SemanticInUse.m_CameraPos )
	{
		if(!in_SemanticInUse.m_ViewI)
		{
			XSI_RTS_MatrixInvert(in_SemanticData.m_View, in_SemanticData.m_ViewI);
		}

		memcpy(in_SemanticData.m_CameraPos, in_SemanticData.m_ViewI[3], sizeof(in_SemanticData.m_CameraPos));
	}

	if( in_SemanticInUse.m_CameraNearFar )
	{
		in_SemanticData.m_CameraNearFar[0] = in_pSysParams->m_SceneData->m_sCamera.m_fNear;
		in_SemanticData.m_CameraNearFar[1] = in_pSysParams->m_SceneData->m_sCamera.m_fFar;
	}

	if( in_SemanticInUse.m_TimeNow )
	{
		in_SemanticData.m_TimeNow[0] = (float) in_pSysParams->m_SceneData->m_fCurrentTime;
	}

	if( in_SemanticInUse.m_TimeLast )
	{
		in_SemanticData.m_TimeLast[0] = in_SemanticInUse.m_LastTime;
	}

	if( in_SemanticInUse.m_TimeFrameNumber )
	{
		in_SemanticData.m_TimeFrameNumber[0] = (float)in_pSysParams->m_SceneData->m_fCurrentTime * 30.0f; // hard coded for 30 fps
	}

	if( in_SemanticInUse.m_NumAmbientLights )
	{
		in_SemanticData.m_NumAmbientLights[0] = 1;
	}

	if( in_SemanticInUse.m_NumDirectionalLights || in_SemanticInUse.m_NumPointLights || in_SemanticInUse.m_NumSpotLights )
	{
		int loop;

		in_SemanticData.m_NumDirectionalLights[0] = 0;
		in_SemanticData.m_NumPointLights[0] = 0;
		in_SemanticData.m_NumSpotLights[0] = 0;

		for(loop = 0; loop < in_pSysParams->m_SceneData->m_iNbLights; loop++)
		{
			if(in_pSysParams->m_SceneData->m_pLights[loop].m_eType == RTS_LIGHT_POINT)
			{
				in_SemanticData.m_NumPointLights[0]++;
			}
			else if(in_pSysParams->m_SceneData->m_pLights[loop].m_eType == RTS_LIGHT_INFINITE)
			{
				in_SemanticData.m_NumDirectionalLights[0]++;
			}
			else if(in_pSysParams->m_SceneData->m_pLights[loop].m_eType == RTS_LIGHT_SPOT)
			{
				in_SemanticData.m_NumSpotLights[0]++;
			}
		}
	}

	if( in_SemanticInUse.m_SasCamera )
	{
		if(!in_SemanticInUse.m_ViewI)
		{
			XSI_RTS_MatrixInvert(in_SemanticData.m_View, in_SemanticData.m_ViewI);
		}

		in_SemanticData.m_SasCamera[0].m_NearFarClipping[0] = in_pSysParams->m_SceneData->m_sCamera.m_fNear;
		in_SemanticData.m_SasCamera[0].m_NearFarClipping[1] = in_pSysParams->m_SceneData->m_sCamera.m_fFar;
		memcpy(in_SemanticData.m_SasCamera[0].m_Position, in_SemanticData.m_ViewI[3], sizeof(in_SemanticData.m_SasCamera[0].m_Position));
		memcpy(in_SemanticData.m_SasCamera[0].m_Projection, in_SemanticData.m_Proj,  sizeof(in_SemanticData.m_SasCamera[0].m_Projection));
		memcpy(in_SemanticData.m_SasCamera[0].m_WorldToView, in_SemanticData.m_WorldView,  sizeof(in_SemanticData.m_SasCamera[0].m_WorldToView));
	}

	if( in_SemanticInUse.m_AmbientLights )
	{
		in_SemanticData.m_AmbientLights[0].m_Color[0] = 0.3f;
		in_SemanticData.m_AmbientLights[0].m_Color[1] = 0.3f;
		in_SemanticData.m_AmbientLights[0].m_Color[2] = 0.3f;
		in_SemanticData.m_AmbientLights[0].m_Color[3] = 1.0f;
	}

	int loop;

	int l_NumDirectionalLights = 0;
	int l_NumPointLights = 0;
	int l_NumSpotLights = 0;

	if( (in_SemanticInUse.m_PointLights) || (in_SemanticInUse.m_DirectionalLights) || (in_SemanticInUse.m_SpotLights))
	{
		for(loop = 0; loop < in_pSysParams->m_SceneData->m_iNbLights; loop++)
		{
			if(in_pSysParams->m_SceneData->m_pLights[loop].m_eType == RTS_LIGHT_POINT)
			{
				in_SemanticData.m_PointLights[l_NumPointLights].m_Color[0] = in_pSysParams->m_SceneData->m_pLights[loop].m_fRed;
				in_SemanticData.m_PointLights[l_NumPointLights].m_Color[1] = in_pSysParams->m_SceneData->m_pLights[loop].m_fGreen;
				in_SemanticData.m_PointLights[l_NumPointLights].m_Color[2] = in_pSysParams->m_SceneData->m_pLights[loop].m_fBlue;
				in_SemanticData.m_PointLights[l_NumPointLights].m_Color[3] = in_pSysParams->m_SceneData->m_pLights[loop].m_fAlpha;

				in_SemanticData.m_PointLights[l_NumPointLights].m_Position[0] = in_pSysParams->m_SceneData->m_pLights[loop].m_fPosX;
				in_SemanticData.m_PointLights[l_NumPointLights].m_Position[1] = in_pSysParams->m_SceneData->m_pLights[loop].m_fPosY;
				in_SemanticData.m_PointLights[l_NumPointLights].m_Position[2] = in_pSysParams->m_SceneData->m_pLights[loop].m_fPosZ;
				in_SemanticData.m_PointLights[l_NumPointLights].m_Position[3] = in_pSysParams->m_SceneData->m_pLights[loop].m_fPosW;

				in_SemanticData.m_PointLights[l_NumPointLights].m_Range = FLT_MAX;

				l_NumPointLights++;
			}
			else if(in_pSysParams->m_SceneData->m_pLights[loop].m_eType == RTS_LIGHT_INFINITE)
			{
				in_SemanticData.m_DirectionalLights[l_NumDirectionalLights].m_Color[0] = in_pSysParams->m_SceneData->m_pLights[loop].m_fRed;
				in_SemanticData.m_DirectionalLights[l_NumDirectionalLights].m_Color[1] = in_pSysParams->m_SceneData->m_pLights[loop].m_fGreen;
				in_SemanticData.m_DirectionalLights[l_NumDirectionalLights].m_Color[2] = in_pSysParams->m_SceneData->m_pLights[loop].m_fBlue;
				in_SemanticData.m_DirectionalLights[l_NumDirectionalLights].m_Color[3] = in_pSysParams->m_SceneData->m_pLights[loop].m_fAlpha;

				in_SemanticData.m_DirectionalLights[l_NumDirectionalLights].m_Direction[0] = -in_pSysParams->m_SceneData->m_pLights[loop].m_fPosX;
				in_SemanticData.m_DirectionalLights[l_NumDirectionalLights].m_Direction[1] = -in_pSysParams->m_SceneData->m_pLights[loop].m_fPosY;
				in_SemanticData.m_DirectionalLights[l_NumDirectionalLights].m_Direction[2] = -in_pSysParams->m_SceneData->m_pLights[loop].m_fPosZ;
				in_SemanticData.m_DirectionalLights[l_NumDirectionalLights].m_Direction[3] = in_pSysParams->m_SceneData->m_pLights[loop].m_fPosW;

				l_NumDirectionalLights++;
			}
			else if(in_pSysParams->m_SceneData->m_pLights[loop].m_eType == RTS_LIGHT_SPOT)
			{
				in_SemanticData.m_SpotLights[l_NumSpotLights].m_Color[0] = in_pSysParams->m_SceneData->m_pLights[loop].m_fRed;
				in_SemanticData.m_SpotLights[l_NumSpotLights].m_Color[1] = in_pSysParams->m_SceneData->m_pLights[loop].m_fGreen;
				in_SemanticData.m_SpotLights[l_NumSpotLights].m_Color[2] = in_pSysParams->m_SceneData->m_pLights[loop].m_fBlue;
				in_SemanticData.m_SpotLights[l_NumSpotLights].m_Color[3] = in_pSysParams->m_SceneData->m_pLights[loop].m_fAlpha;

				in_SemanticData.m_SpotLights[l_NumSpotLights].m_Position[0] = in_pSysParams->m_SceneData->m_pLights[loop].m_fPosX;
				in_SemanticData.m_SpotLights[l_NumSpotLights].m_Position[1] = in_pSysParams->m_SceneData->m_pLights[loop].m_fPosY;
				in_SemanticData.m_SpotLights[l_NumSpotLights].m_Position[2] = in_pSysParams->m_SceneData->m_pLights[loop].m_fPosZ;
				in_SemanticData.m_SpotLights[l_NumSpotLights].m_Position[3] = in_pSysParams->m_SceneData->m_pLights[loop].m_fPosW;

				in_SemanticData.m_SpotLights[l_NumSpotLights].m_Direction[0] = in_pSysParams->m_SceneData->m_pLights[loop].m_fDirX;
				in_SemanticData.m_SpotLights[l_NumSpotLights].m_Direction[1] = in_pSysParams->m_SceneData->m_pLights[loop].m_fDirX;
				in_SemanticData.m_SpotLights[l_NumSpotLights].m_Direction[2] = in_pSysParams->m_SceneData->m_pLights[loop].m_fDirX;
				in_SemanticData.m_SpotLights[l_NumSpotLights].m_Direction[3] = in_pSysParams->m_SceneData->m_pLights[loop].m_fDirX;


				in_SemanticData.m_SpotLights[l_NumSpotLights].m_Range = FLT_MAX;
				in_SemanticData.m_SpotLights[l_NumSpotLights].m_Phi = in_pSysParams->m_SceneData->m_pLights[loop].m_fConeAngle;
				in_SemanticData.m_SpotLights[l_NumSpotLights].m_Theta = in_pSysParams->m_SceneData->m_pLights[loop].m_fConeAngle;

				l_NumSpotLights++;
			}
		}
	}

	// now bind everything
	for(loop = 0; loop < in_SemanticInUse.m_Params.GetUsed(); loop++)
	{
		fnBind(in_pContext, in_SemanticInUse.m_Params[loop].m_pDestParam, in_SemanticInUse.m_Params[loop].m_pSourceParam);
	}

}

#endif // XSI_RTSHADERS_H

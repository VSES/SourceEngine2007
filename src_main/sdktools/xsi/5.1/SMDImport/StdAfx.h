// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__43A90550_3CE5_4916_B90C_08C1A52B7EE4__INCLUDED_)
#define AFX_STDAFX_H__43A90550_3CE5_4916_B90C_08C1A52B7EE4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


// Insert your headers here
#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers

#include <windows.h>
#include <Atlbase.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include <xsi_plugin.h>
#include <xsi_pluginitem.h>
#include <xsi_pluginregistrar.h>
#include <xsi_status.h>
#include <xsi_application.h>
#include <xsi_value.h>
#include <xsi_context.h>
#include <xsi_menuitem.h>
#include <xsi_menu.h>
#include <xsi_command.h>
#include <xsi_argument.h>
#include <xsi_ppglayout.h>
#include <xsi_ppgeventcontext.h>
#include <xsi_customproperty.h>
#include <xsi_model.h>
#include <xsi_null.h>
#include <xsi_chainbone.h>
#include <xsi_chainroot.h>
#include <xsi_kinematics.h>
#include <xsi_kinematicstate.h>
#include <xsi_fcurve.h>
#include <xsi_math.h>
#include <xsi_mixer.h>
#include <xsi_actionsource.h>
#include <xsi_animationsourceitem.h>
#include <xsi_uitoolkit.h>
#include <xsi_progressbar.h>
#include <xsi_longarray.h>
#include <xsi_envelope.h>
#include <xsi_geometry.h>
#include <xsi_polygonmesh.h>
#include <xsi_primitive.h>
#include <xsi_triangle.h>
#include <xsi_material.h>
#include <xsi_vertex.h>
#include <xsi_ogltexture.h>
#include <xsi_imageclip2.h>

void W2AHelper( LPSTR out_sz, LPCWSTR in_wcs, int in_cch = -1);

inline void DSA2WHelper( LPWSTR out_wcs, LPCSTR in_sz, int in_cch = -1 )
{
	if ( out_wcs != NULL && 0 != in_cch )
	{
		out_wcs[0] = L'\0' ;

		if ( in_sz != NULL )
		{
			// Right now, we can't really test the return value because mbstowcs() is
			// mapped to strcpy() on the Irix o32 build. When this build is not done
			// anymore, the != 0 test should be updated.

			size_t l_iLen = 0;
			l_iLen = ::mbstowcs( out_wcs, in_sz, ( in_cch < 0 ) ? ::strlen(in_sz) + 1 : (size_t) in_cch ) ;
			assert( (int)l_iLen != -1 );

        		if ( in_cch > 0 ) { out_wcs[ in_cch - 1 ] = L'\0'; }
		}
	}
}

//////////////////////////////////////////////////////////////////////
// Helper functions
//////////////////////////////////////////////////////////////////////
char * GetNextToken(char *in_buffer, const char *in_separators);

#define DSA2W(out_wcs, in_sz) \
	if (NULL == (LPCSTR)(in_sz)) \
		*(out_wcs) = NULL; \
	else \
	{ \
		*(out_wcs) = (LPWSTR)alloca((strlen((in_sz)) + 1) * sizeof(WCHAR)); \
		DSA2WHelper(*(out_wcs), (in_sz)); }\

// TODO: reference additional headers your program requires here

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__43A90550_3CE5_4916_B90C_08C1A52B7EE4__INCLUDED_)

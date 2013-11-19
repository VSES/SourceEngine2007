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
#if defined(_WIN32) || defined(_WIN32_WCE)
#ifndef _XBOX
#include <windows.h>
#endif
#endif 

#include <SIBCUtil.h>
#include <SIBaseApplication.h>
#include <Scene.h>

#include <RTP_Model.h>
#include <RTP_Transform.h>
#include <RTP_Camera.h>
#include <RTP_Mesh.h>
#include <RTP_Material.h>
#include <RTP_Visibility.h>
#include <RTP_FCurve.h>
#include <RTP_Shader.h>
#include <RTP_Constraint.h>
#include <RTP_Light.h>
#include <RTP_Envelope.h>
#include <RTP_Scene.h>
#include <RTP_ShapeAnimation.h>
#include <RTP_Cluster.h>
#include <RTP_Ambience.h>
#include <RTP_2D2JIK.h>
#include <RTP_Instance.h>

#ifdef XSI_STATIC_LINK
extern SI_Error	Basic_InitializePlugins ( void *in_pData );
extern SI_Error	Basic_RegisterPlugins ( CSIBaseApplication* in_pApplication, CSLScene *in_pCurrentScene );
extern SI_Error	Basic_CleanupPlugins ();
#endif
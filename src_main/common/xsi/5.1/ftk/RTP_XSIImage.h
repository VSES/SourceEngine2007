/****************************************************************************************
THIS CODE IS PUBLISHED AS A SAMPLE ONLY AND IS PROVIDED "AS IS".
IN NO EVENT SHALL SOFTIMAGE, AVID TECHNOLOGY, INC. AND/OR THEIR RESPECTIVE 
SUPPLIERS BE LIABLE FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR ANY
DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN
CONNECTION WITH THE USE OR PERFORMANCE OF THIS CODE . 
 
COPYRIGHT NOTICE. Copyright © 1999 Avid Technology Inc. . All rights reserved. 

SOFTIMAGE is a registered trademark of Avid Technology Inc. or its subsidiaries 
or divisions. Windows NT is a registered trademark of Microsoft Corp. All other
trademarks contained herein are the property of their respective owners. 
****************************************************************************************/
#ifndef _RTP_XSIImage_H_
#define _RTP_XSIImage_H_

#include <Scene.h>
#include <SIBCUtil.h>
#include <SIBaseApplication.h>
#include <RTP_BasicProcessesTypes.h>

SI_Error	RegisterXSIImage ( CSIBaseApplication* in_pApplication, CSLScene *in_pCurrentScene );
CSLTemplate	*XSI_Image_RTPCallback(CdotXSITemplate *in_pTemplate, SLContext *in_pContext);




#endif // _RTP_XSIImage_H_

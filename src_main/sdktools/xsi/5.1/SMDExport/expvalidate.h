#if (_MSC_VER > 1000) || defined(SGI_COMPILER)
#pragma once
#endif

//*****************************************************************************
//
// File supervisor: Marc Poirier/Modified: Robert Belanger
// Date: 5/20/2003
// (c) Copyright 2003 SOFTIMAGE co.
//
// @doc These functions are used to force the user to enter a registration code
// in order to use the software, the two known ones will be when the user saves
// for the first time and the other is when the user attempts to export SMD files
// from within the Half-Life 2 plugin.
//
//
// @module expvalidate.h | Declaration of the expvalidate.h.
//
//*****************************************************************************


#ifndef __EXPVALIDATE_H__
#define __EXPVALIDATE_H__

bool ValidateRegistrationCurrentKey( TCHAR * );
bool ValidateRegistrationNewKey( TCHAR *, TCHAR * web_path,TCHAR * welcome_text, TCHAR * title_text );

#endif

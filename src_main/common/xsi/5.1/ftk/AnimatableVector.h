/******************************************************************************\
*
* File:          AnimatableVector.h
* Creation date: Feb 5, 2002
* Purpose:       Declaration of AnimatableVector class
*
* Modifications:
*	Feb 5, 2002, Created - dolaflam
* Copyright 2002, XXXXX
* All rights are reserved. Reproduction in whole or part is prohibited
* without the written consent of the copyright owner.
*
\******************************************************************************/

#ifndef _ANIMATABLEVECTOR_H
#define _ANIMATABLEVECTOR_H

#include <SIBCUtil.h>

class CSLAnimatableVector
{
public:
	CSLAnimatableVector() {};
	virtual ~CSLAnimatableVector() {};

	virtual SI_Error Synchronize() = 0;

};

#endif

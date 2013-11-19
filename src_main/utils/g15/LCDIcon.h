//************************************************************************
//
// LCDIcon.h
//
// The CLCDIcon class allows for drawing icons on the lcd.
// 
// Logitech LCD SDK
//
// Copyright 2005 Logitech Inc.
//************************************************************************

#ifndef _LCDICON_H_INCLUDED_ 
#define _LCDICON_H_INCLUDED_ 

#include "LCDBase.h"

class CLCDIcon : public CLCDBase
{
public:
    CLCDIcon();
    virtual ~CLCDIcon();

    void SetIcon(HICON hIcon, int nWidth = 16, int nHeight = 16);

protected:
    virtual void OnDraw(CLCDGfx &rGfx);

private:
    HICON m_hIcon;
    int m_nIconWidth, m_nIconHeight;
};


#endif // !_LCDICON_H_INCLUDED_ 

//** end of LCDIcon.h ****************************************************

//************************************************************************
//
// LCDIcon.cpp
//
// The CLCDIcon class draws icons onto the lcd.
// 
// Logitech LCD SDK
//
// Copyright 2005 Logitech Inc.
//************************************************************************

#include "StdLCD.h"
#include "LCDIcon.h"


//************************************************************************
//
// CLCDIcon::CLCDIcon
//
//************************************************************************

CLCDIcon::CLCDIcon()
{

    m_hIcon = NULL;
    m_nIconWidth = 16;
    m_nIconHeight = 16;
}


//************************************************************************
//
// CLCDIcon::CLCDIcon
//
//************************************************************************

CLCDIcon::~CLCDIcon()
{

}


//************************************************************************
//
// CLCDIcon::SetIcon
//
//************************************************************************

void CLCDIcon::SetIcon(HICON hIcon, int nWidth, int nHeight)
{
    LOGIASSERT(NULL != hIcon);
    m_hIcon = hIcon;
    m_nIconWidth = nWidth;
    m_nIconHeight = nHeight;
}


//************************************************************************
//
// CLCDIcon::OnDraw
//
//************************************************************************

void CLCDIcon::OnDraw(CLCDGfx &rGfx)
{
    int nOldBkMode = SetBkMode(rGfx.GetHDC(), TRANSPARENT);     

    if (m_hIcon)
    {
        DrawIconEx(rGfx.GetHDC(), 0, 0, m_hIcon,
                   m_nIconWidth, m_nIconHeight, 0, NULL, DI_NORMAL);
    }

    // restores
    SetBkMode(rGfx.GetHDC(), nOldBkMode);
}


//** end of LCDIcon.cpp **************************************************

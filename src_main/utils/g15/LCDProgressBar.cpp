//************************************************************************
//
// LCDProgressBar.cpp
//
// The CLCDProgressBar class draws a progress bar onto the LCD.
// 
// Logitech LCD SDK
//
// Copyright 2005 Logitech Inc.
//************************************************************************

#include "StdLCD.h"
#include "LCDProgressBar.h"


//************************************************************************
//
// CLCDProgressBar::CLCDProgressBar
//
//************************************************************************

CLCDProgressBar::CLCDProgressBar()
{
    m_fPos = 0.0f;
    m_eStyle = STYLE_CURSOR;
    m_Range.nMin = 0;
    m_Range.nMax = 100;
    m_nCursorWidth = 5;
}


//************************************************************************
//
// CLCDProgressBar::~CLCDProgressBar
//
//************************************************************************

CLCDProgressBar::~CLCDProgressBar()
{

}


//************************************************************************
//
// CLCDProgressBar:Initialize
//
//************************************************************************

HRESULT CLCDProgressBar::Initialize()
{

    m_hBrush = (HBRUSH)GetStockObject(WHITE_BRUSH);

    return CLCDBase::Initialize();
}


//************************************************************************
//
// CLCDProgressBar::OnDraw
//
//************************************************************************

void CLCDProgressBar::OnDraw(CLCDGfx &rGfx)
{   
    rGfx.ClearScreen();

    // draw the border
    RECT r = { 0, 0, GetWidth(), GetHeight() };
    
    FrameRect(rGfx.GetHDC(), &r, m_hBrush);

    // draw the progress
    switch(m_eStyle)
    {
    case STYLE_CURSOR:
        {
            int nCursorPos = (int)Scalef((float)m_Range.nMin, (float)m_Range.nMax,
                                   (float)1, (float)(GetWidth() - m_nCursorWidth-1),
                                   m_fPos);
            r.left = nCursorPos;
            r.right = r.left + m_nCursorWidth;
            FillRect(rGfx.GetHDC(), &r, m_hBrush);
        }
        break;
    case STYLE_FILLED:
        {
            int nBarWidth = (int)Scalef((float)m_Range.nMin, (float)m_Range.nMax,
                                  0.0f, (float)GetWidth(),
                                  m_fPos);
            r.right = nBarWidth;
            FillRect(rGfx.GetHDC(), &r, m_hBrush);
        }
        break;
    default:
        break;
    }
}


//************************************************************************
//
// CLCDProgressBar::ResetUpdate
//
//************************************************************************

void CLCDProgressBar::ResetUpdate()
{

}


//************************************************************************
//
// CLCDProgressBar::SetRange
//
//************************************************************************

void CLCDProgressBar::SetRange(int nMin, int nMax)
{
    m_Range.nMin = nMin;
    m_Range.nMax = nMax;
}


//************************************************************************
//
// CLCDProgressBar::SetRange
//
//************************************************************************

void CLCDProgressBar::SetRange(RANGE& Range)
{
    m_Range = Range;
}


//************************************************************************
//
// CLCDProgressBar::GetRange
//
//************************************************************************

RANGE& CLCDProgressBar::GetRange()
{
    return m_Range;
}


//************************************************************************
//
// CLCDProgressBar::SetPos
//
//************************************************************************

float CLCDProgressBar::SetPos(float fPos)
{
    return ( m_fPos = max((float)m_Range.nMin, min(fPos, (float)m_Range.nMax)) );
}


//************************************************************************
//
// CLCDProgressBar::GetPos
//
//************************************************************************

float CLCDProgressBar::GetPos()
{
    return m_fPos;
}


//************************************************************************
//
// CLCDProgressBar::EnableCursor
//
//************************************************************************

void CLCDProgressBar::EnableCursor(BOOL bEnable)
{
    m_eStyle = bEnable ? STYLE_CURSOR : STYLE_FILLED;
}


//************************************************************************
//
// CLCDProgressBar::Scalef
//
//************************************************************************

float CLCDProgressBar::Scalef(float fFromMin, float fFromMax,
                             float fToMin, float fToMax, float fFromValue)
{

    // normalize the input
    float fFromValueN = 0;
    float fOffset = 0;
    if (fFromMin < 0)
        fOffset = (0 - fFromMin);
    fFromValueN = (fFromValue + fOffset) / (fFromMax + fOffset);

    // now scale to the output
    float fToRange = fToMax - fToMin;

    return ( fToMin + (fFromValueN * fToRange) );
}


//************************************************************************
//
// CLCDProgressBar::Scale
//
//************************************************************************

int CLCDProgressBar::Scale(int nFromMin, int nFromMax,
                           int nToMin, int nToMax, int nFromValue)
{
    return (int)Scalef(
        (float)nFromMin,
        (float)nFromMax,
        (float)nToMin,
        (float)nToMax,
        (float)nFromValue
        );
}


//** end of LCDProgressBar.cpp *******************************************

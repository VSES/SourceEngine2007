//************************************************************************
//
// LCDProgressBar.h
//
// The CLCDProgressBar class draws a progress bar onto the LCD.
// 
// Logitech LCD SDK
//
// Copyright 2005 Logitech Inc.
//************************************************************************

#ifndef _LCDPROGRESSBAR_H_INCLUDED_ 
#define _LCDPROGRESSBAR_H_INCLUDED_ 

#include "LCDBase.h"

typedef struct RANGE
{
    int nMin;
    int nMax;

}RANGE, *LPRANGE;

class CLCDProgressBar : public CLCDBase
{

    enum ePROGRESS_STYLE { STYLE_FILLED, STYLE_CURSOR };
    
public:
    CLCDProgressBar();
    virtual ~CLCDProgressBar();

    // CLCDBase
    virtual HRESULT Initialize(void);
    virtual void OnDraw(CLCDGfx &rGfx);
    virtual void ResetUpdate(void);
    
    // CLCDProgressBar
    virtual void SetRange(int nMin, int nMax);
    virtual void SetRange(RANGE& Range);
    virtual RANGE& GetRange(void);
    virtual float SetPos(float fPos);
    virtual float GetPos(void);
    virtual void EnableCursor(BOOL bEnable);

protected:
    float Scalef(float fFromMin, float fFromMax,
                 float fToMin, float fToMax, float fFromValue);
    int Scale(int nFromMin, int nFromMax,
              int nToMin, int nToMax, int nFromValue);

private:
    RANGE m_Range;
    float m_fPos;
    ePROGRESS_STYLE m_eStyle;
    HBRUSH m_hBrush;
    int m_nCursorWidth;
};


#endif // !_LCDPROGRESSBAR_H_INCLUDED_ 

//** end of LCDProgressBar.h *********************************************

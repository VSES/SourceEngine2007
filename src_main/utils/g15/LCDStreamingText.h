//************************************************************************
//
// LCDStreamingText.h
//
// The CLCDStreamingText class draws streaming text onto the LCD.
// Streaming text is a single line of text that is repeatedly streamed
// horizontally across the LCD.
// 
// Logitech LCD SDK
//
// Copyright 2005 Logitech Inc.
//************************************************************************

#ifndef _LCDSTREAMINGTEXT_H_INCLUDED_ 
#define _LCDSTREAMINGTEXT_H_INCLUDED_ 

#include "LCDBase.h"
#include "LCDCollection.h"
#include "LCDText.h"

class CLCDStreamingText: public CLCDCollection
{

public:
    CLCDStreamingText();
    virtual ~CLCDStreamingText();

    // CLCDBase
    virtual HRESULT Initialize(void);
    virtual void ResetUpdate(void);
    virtual void Show(BOOL bShow);
    virtual void SetOrigin(POINT pt);
    virtual void SetOrigin(int nX, int nY);

    void SetText(LPCTSTR szText);
    void SetGapText(LPCTSTR szGapText);
    void SetStartDelay(DWORD dwMilliseconds);
    void SetSpeed(DWORD dwSpeed);
	void SetScrollingStep(DWORD dwStepInPixels);
    void SetAlignment(int nAlignment = DT_LEFT);
    void SetFont(LOGFONT& lf);
    void SetFontFaceName(LPCTSTR szFontName);
    void SetFontPointSize(int nSize);
    void SetFontWeight(int nPointSize);
    HFONT GetFont();

    enum { DEFAULT_DPI = 96, DEFAULT_POINTSIZE = 8 };

protected:
    virtual void OnUpdate(DWORD dwTimestamp);
    virtual void OnDraw(CLCDGfx &rGfx);

private:
    int AddText(LPCTSTR szText);
    void RemoveText(int nIndex);
    void RemoveAllText();

private:
    BOOL RecalcTextBoxes(CLCDGfx &rGfx);
    void RecalcTextBoxOrigins();
    void ApplyOrigins(int nOffset);

    enum { MAX_TEXT = 256 };
    enum eSCROLL_STATES { STATE_DELAY, STATE_SCROLL};

    DWORD m_dwEllapsedTime; // ellapsed time in state
    DWORD m_dwStartDelay;   // milliseconds
    DWORD m_dwSpeed;        // pixels/second
	DWORD m_dwStepInPixels; // Number of pixels to shift
    DWORD m_dwLastUpdate;   // milliseconds
	DWORD m_dwLastTickCount;// milliseconds

    eSCROLL_STATES m_eState;
    BOOL m_bRecalcExtent;

    CLCDText* m_pQueueHead;
    TCHAR m_szText[MAX_TEXT];
    TCHAR m_szGapText[MAX_TEXT];

    HFONT m_hFont;
    int m_nTextAlignment;
    float m_fFractDistance;
};


#endif // !_LCDSTREAMINGTEXT_H_INCLUDED_ 

//** end of LCDStreamingText.h *******************************************

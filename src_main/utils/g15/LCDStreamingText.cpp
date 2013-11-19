//************************************************************************
//
// LCDStreamingText.cpp
//
// The CLCDStreamingText class draws streaming text onto the LCD.
// Streaming text is a single line of text that is repeatedly streamed
// horizontally across the LCD.
// 
// Logitech LCD SDK
//
// Copyright 2005 Logitech Inc.
//************************************************************************

#include "StdLCD.h"
#include "LCDStreamingText.h"
//#include <math.h>

//************************************************************************
//
// CLCDStreamingText::CLCDStreamingText
//
//************************************************************************

CLCDStreamingText::CLCDStreamingText()
{
    ZeroMemory(m_szText, sizeof(m_szText));
    ZeroMemory(m_szGapText, sizeof(m_szGapText));

    _tcscpy(m_szGapText, _T("   "));
    m_hFont = NULL;
    m_nTextAlignment = DT_LEFT;
	m_dwLastTickCount = 0;
}


//************************************************************************
//
// CLCDStreamingText::~CLCDStreamingText
//
//************************************************************************

CLCDStreamingText::~CLCDStreamingText()
{
    RemoveAllText();
    if (m_hFont)
    {
        DeleteObject(m_hFont);
        m_hFont = NULL;
    }
}


//************************************************************************
//
// CLCDStreamingText::Initialize
//
//************************************************************************

HRESULT CLCDStreamingText::Initialize(void)
{
    m_eState = STATE_DELAY;
    m_dwStartDelay  = 2000;
    m_dwSpeed       = 7;
	m_dwStepInPixels = 7;
    m_dwLastUpdate = 0;
    m_dwEllapsedTime = 0;
    m_dwLastUpdate = 0;
    m_bRecalcExtent = FALSE;
    m_pQueueHead = NULL;
    m_fFractDistance = 0.0f;
	m_dwLastTickCount = 0;

    m_hFont = (HFONT) GetStockObject(DEFAULT_GUI_FONT);
    if(NULL != m_hFont)
    {
        SetFontPointSize(DEFAULT_POINTSIZE);
    }

    return CLCDCollection::Initialize();
}


//************************************************************************
//
// CLCDStreamingText::ResetUpdate
//
//************************************************************************

void CLCDStreamingText::ResetUpdate(void)
{
    m_eState = STATE_DELAY;
    m_dwEllapsedTime = 0;
    m_dwLastUpdate = 0;
    m_pQueueHead = NULL;
    m_fFractDistance = 0.0f;
    
    // remove, re-add, and recalculate the text
    m_bRecalcExtent = TRUE;
    RemoveAllText();
    AddText(m_szText);
}


//************************************************************************
//
// CLCDStreamingText::Show
//
//************************************************************************

void CLCDStreamingText::Show(BOOL bShow)
{
    CLCDCollection::Show(bShow);
}


//************************************************************************
//
// CLCDStreamingText::SetText
//
//************************************************************************

void CLCDStreamingText::SetText(LPCTSTR szText)
{
    LOGIASSERT(NULL != szText);
    if(szText && _tcscmp(m_szText, szText))
    {
        _tcsncpy(m_szText, szText, MAX_TEXT);
        m_bRecalcExtent = TRUE;
        ResetUpdate();
    }
}


//************************************************************************
//
// CLCDStreamingText::SetOrigin
//
//************************************************************************

void CLCDStreamingText::SetOrigin(POINT pt)
{
	m_Origin = pt;
	SetOrigin(pt.x, pt.y);
}


//************************************************************************
//
// CLCDStreamingText::SetOrigin
//
//************************************************************************

void CLCDStreamingText::SetOrigin(int nX, int nY)
{
	m_Origin.x = nX;
	m_Origin.y = nY;

    LCD_OBJECT_LIST::iterator it = m_Objects.begin();
	if (it != m_Objects.end())
	{
        CLCDBase *pObject = *it;
		POINT ptOldOrigin = pObject->GetOrigin();
		pObject->SetOrigin(nX, nY);

		if ( (ptOldOrigin.x != nX) && (ptOldOrigin.y != nY) )
		{
			ResetUpdate();
		}
	}
}


//************************************************************************
//
// CLCDStreamingText::SetGapText
//
//************************************************************************

void CLCDStreamingText::SetGapText(LPCTSTR szGapText)
{
    LOGIASSERT(NULL != szGapText);
    if(szGapText && _tcscmp(m_szGapText, szGapText))
    {
        _tcsncpy(m_szGapText, szGapText, MAX_TEXT);
        m_bRecalcExtent = TRUE;
    }
}


//************************************************************************
//
// CLCDStreamingText::SetFont
//
//************************************************************************

void CLCDStreamingText::SetFont(LOGFONT& lf)
{
    if (m_hFont)
    {
        DeleteObject(m_hFont);
        m_hFont = NULL;
    }

    m_hFont = CreateFontIndirect(&lf);
    m_bRecalcExtent = TRUE;
}


//************************************************************************
//
// CLCDStreamingText::GetFont
//
//************************************************************************

HFONT CLCDStreamingText::GetFont()
{
    return m_hFont;
}


//************************************************************************
//
// CLCDText::SetFontFaceName
//
//************************************************************************

void CLCDStreamingText::SetFontFaceName(LPCTSTR szFontName)
{
    // if NULL, uses the default gui font
    if (NULL == szFontName)
        return;

    LOGFONT lf;
    ZeroMemory(&lf, sizeof(lf));
    GetObject(m_hFont, sizeof(LOGFONT), &lf);

    DeleteObject(m_hFont);
    m_hFont = NULL;

    _tcsncpy(lf.lfFaceName, szFontName, LF_FACESIZE);

    m_hFont = CreateFontIndirect(&lf);
}


//************************************************************************
//
// CLCDStreamingText::SetFontPointSize
//
//************************************************************************

void CLCDStreamingText::SetFontPointSize(int nPointSize)
{
    LOGFONT lf;
    ZeroMemory(&lf, sizeof(lf));
    GetObject(m_hFont, sizeof(LOGFONT), &lf);

    DeleteObject(m_hFont);
    m_hFont = NULL;

    lf.lfHeight = -MulDiv(nPointSize, DEFAULT_DPI, 72);

    m_hFont = CreateFontIndirect(&lf);
}


//************************************************************************
//
// CLCDStreamingText::SetFontWeight
//
//************************************************************************

void CLCDStreamingText::SetFontWeight(int nWeight)
{
    LOGFONT lf;
    ZeroMemory(&lf, sizeof(lf));
    GetObject(m_hFont, sizeof(LOGFONT), &lf);

    DeleteObject(m_hFont);
    m_hFont = NULL;

    lf.lfWeight = nWeight;

    m_hFont = CreateFontIndirect(&lf);
}


//************************************************************************
//
// CLCDStreamingText::SetAlignment
//
// only relevant if no streaming
//************************************************************************

void CLCDStreamingText::SetAlignment(int nAlignment)
{
    m_nTextAlignment = nAlignment;
}


//************************************************************************
//
// CLCDStreamingText::AddText
//
//************************************************************************

int CLCDStreamingText::AddText(LPCTSTR szText)
{

    CLCDText* pText = new CLCDText;
    pText->Initialize();
    pText->SetText(szText);
    pText->SetOrigin(GetOrigin().x, GetOrigin().y);
    pText->SetLogicalOrigin(GetLogicalOrigin().x, GetLogicalOrigin().y);
    pText->SetSize(GetWidth(), GetHeight());
    pText->SetBackgroundMode(OPAQUE);

    LOGFONT lf;
    GetObject(m_hFont, sizeof(LOGFONT), &lf);
    pText->SetFont(lf);

    m_bRecalcExtent = TRUE;

    m_Objects.push_back(pText);

    if (NULL == m_pQueueHead)
    {
        m_pQueueHead = pText;
    }

    // return the zero-based index
    return (int)(m_Objects.size()-1);
}


//************************************************************************
//
// CLCDStreamingText::RemoveText
//
//************************************************************************

void CLCDStreamingText::RemoveText(int nIndex)
{
    LCD_OBJECT_LIST::iterator it = m_Objects.begin();
    int i = 0;
    while(it != m_Objects.end())
    {
        CLCDBase *pObject = *it;
        LOGIASSERT(NULL != pObject);

        if (i == nIndex)
        {
            m_Objects.erase(it);
            if(NULL != pObject)
            {
                delete pObject;
            }
            break;
        }
        
        ++it;
        ++i;
    }
}


//************************************************************************
//
// CLCDStreamingText::RemoveAllText
//
//************************************************************************

void CLCDStreamingText::RemoveAllText()
{
    LCD_OBJECT_LIST::iterator it = m_Objects.begin();
    while(it != m_Objects.end())
    {
        // we need to delete all the text objects that were placed in to m_Objects
        CLCDBase *pObject = *it;
        LOGIASSERT(NULL != pObject);

        if(NULL != pObject)
        {
            delete pObject;
        }
        ++it;
    }
    m_Objects.clear();
}


//************************************************************************
//
// CLCDStreamingText::SetStartDelay
//
//************************************************************************

void CLCDStreamingText::SetStartDelay(DWORD dwMilliseconds)
{
    m_dwStartDelay = dwMilliseconds;
}


//************************************************************************
//
// CLCDStreamingText::SetSpeed
//
//************************************************************************

void CLCDStreamingText::SetSpeed(DWORD dwSpeed)
{
    m_dwSpeed = dwSpeed;
}

//************************************************************************
//
// CLCDStreamingText::SetScrollingStep: sets the number of pixels the text 
// will jump when it scrolls
//
//************************************************************************

void CLCDStreamingText::SetScrollingStep(DWORD dwStepInPixels)
{
    m_dwStepInPixels = dwStepInPixels;
}

//************************************************************************
//
// CLCDStreamingText::OnUpdate
//
//************************************************************************

void CLCDStreamingText::OnUpdate(DWORD dwTimestamp)
{
	m_dwLastUpdate = m_dwLastUpdate ? m_dwLastUpdate : dwTimestamp;
	m_dwEllapsedTime = (dwTimestamp - m_dwLastUpdate);
	m_dwLastTickCount = dwTimestamp;
}


//************************************************************************
//
// CLCDStreamingText::OnDraw
//
//************************************************************************

void CLCDStreamingText::OnDraw(CLCDGfx &rGfx)
{

    if (m_bRecalcExtent)
    {
        // this just recalculates the text extents
        RecalcTextBoxes(rGfx);
        m_bRecalcExtent = FALSE;
        return;
    }

    switch(m_eState)
    {
    case STATE_DELAY:
        if (m_dwEllapsedTime > m_dwStartDelay)
        {
            m_eState = STATE_SCROLL;
            m_dwEllapsedTime = 0;
			m_dwLastUpdate = m_dwLastTickCount;
        }
        break;
    case STATE_SCROLL:
        {
            // update the positions
            float fDistance = (float)(m_dwSpeed * m_dwEllapsedTime) / 1000.0f;
			m_dwLastUpdate = m_dwLastTickCount;
 
            if (m_Objects.size() > 1)
            {
                // extract any previous fractional remainder
                // and add it to the current distance
                float fTotDistance = (fDistance + m_fFractDistance);
                m_fFractDistance = (fTotDistance >= (float)m_dwStepInPixels) ? (float)(fTotDistance - (int)fTotDistance) : fTotDistance;

                if (fTotDistance < 0.0f)
                    fTotDistance = 0.0f;
                if (m_fFractDistance < 0.0f)
                    m_fFractDistance = 0.0f;

				if (fTotDistance >= (float)m_dwStepInPixels)
					ApplyOrigins(-1 * (int)fTotDistance); // left
            }
        }
        break;
    default:
        break;
    }

    CLCDCollection::OnDraw(rGfx);
}


//************************************************************************
//
// CLCDStreamingText::C
//
//************************************************************************

BOOL CLCDStreamingText::RecalcTextBoxes(CLCDGfx &rGfx)
{

    // check if we need to add another text box
    LCD_OBJECT_LIST::iterator it = m_Objects.begin();

    if (it == m_Objects.end())
        return FALSE;

    CLCDBase* pObject = *it;
    CLCDText* pText = (CLCDText*)pObject;
    LOGIASSERT(NULL != pObject);
    
    LOGFONT lf;
    GetObject(m_hFont, sizeof(LOGFONT), &lf);
    pText->SetFont(lf);

    // this will re-evaluate the main text object
    LOGIASSERT(m_Objects.size() == 1);
    CLCDCollection::OnDraw(rGfx);

    if (it != m_Objects.end())
    {
        if (pText->GetHExtent().cx > GetWidth())
        {

            pText->SetAlignment(DT_LEFT);

            // add a gap
            AddText(m_szGapText);
            // add another text
            AddText(m_szText);
            // add last gap
            AddText(m_szGapText);
        }
        else
        {
            pText->SetAlignment(m_nTextAlignment);
        }
    }

    // this will re-evaluate the other text objects
    CLCDCollection::OnDraw(rGfx);
    RecalcTextBoxOrigins();

    return TRUE;
}


//************************************************************************
//
// CLCDStreamingText::RecalcTextBoxOrigins
//
// Puts all the text boxes in order next to each other
//************************************************************************

void CLCDStreamingText::RecalcTextBoxOrigins()
{

    if (m_Objects.size() <= 1)
        return;

    // draw everyone to the left by the offset
    int nOrgOffset = 0;
    LCD_OBJECT_LIST::iterator it = m_Objects.begin();
    while(it != m_Objects.end())
    {
        CLCDBase* pObject = *it;
        CLCDText* pText = (CLCDText*)pObject;
        LOGIASSERT(NULL != pObject);

        pText->SetLogicalSize(pText->GetHExtent().cx, pText->GetHExtent().cy);

        // string can be empty which generates zero logical space
        //LOGIASSERT(pText->GetLogicalSize().cx);
        //LOGIASSERT(pText->GetLogicalSize().cy);

        POINT& ptOrigin = pText->GetLogicalOrigin();

		if (nOrgOffset == 0)
		{
			nOrgOffset = pText->GetLogicalOrigin().x;
		}

        pText->SetLogicalOrigin(nOrgOffset, ptOrigin.y);
        nOrgOffset += pText->GetHExtent().cx;

        ++it;
    }

}


//************************************************************************
//
// CLCDStreamingText::ApplyOrigins
//
//************************************************************************

void CLCDStreamingText::ApplyOrigins(int nOffset)
{
    
    // draw everyone to the left by the offset
    LCD_OBJECT_LIST::iterator it = m_Objects.begin();
    while(it != m_Objects.end())
    {
        CLCDBase* pObject = *it;
        CLCDText* pText = (CLCDText*)pObject;
        LOGIASSERT(NULL != pObject);

        POINT& ptOrigin = pText->GetLogicalOrigin();

        pText->SetLogicalOrigin(ptOrigin.x + nOffset, ptOrigin.y);

        ++it;
    }

    // If the active box is no longer visible, 
    // pop it off the push it to the end of the list
    if (abs(m_pQueueHead->GetLogicalOrigin().x) >= m_pQueueHead->GetHExtent().cx)
    {
        m_Objects.pop_front();
        m_Objects.push_back(m_pQueueHead);
        RecalcTextBoxOrigins();
        m_pQueueHead = (CLCDText*)*m_Objects.begin();
    }

}


//** end of LCDStreamingText.cpp *******************************************

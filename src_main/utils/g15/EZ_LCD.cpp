/****h* EZ.LCD.Wrapper/EzLcd
 * NAME
 *   EZ LCD Wrapper
 * COPYRIGHT
 *   The Logitech EZ LCD Wrapper, including all accompanying 
 *   documentation, is protected by intellectual property laws. All rights 
 *   not expressly granted by Logitech are reserved.
 * PURPOSE
 *   The EZ LCD Wrapper is aimed at games wanting to make use of the LCD 
 *   display on Logitech G-series keyboard. It comes with a very intuitive 
 *   and easy to use interface which enables to very easily display static 
 *   or scrolling strings, progress bars, and icons.
 *   See the following files to get started:
 *       - readme.txt: tells you how to get started
 *       - EZ_LCD_Wrapper.cpp: sample implementation. Just plug in a 
 *         Logitech G-series keyboard with LCD, compile, and run.
 * AUTHOR
 *   Christophe Juncker (christophe_juncker@logitech.com)
 * CREATION DATE
 *   06/13/2005
 * MODIFICATION HISTORY
 *   
 *******
 */

// Public interface
#include "interface.h"
#include "g15/ig15.h"

#include "StdLCD.h"

#include "EZ_LCD.h"
#include "LCDText.h"
#include "LCDStreamingText.h"
#include "LCDIcon.h"
#include "LCDProgressBar.h"

// text box height for various font sizes
const INT LG_SMALL_FONT_TEXT_BOX_HEIGHT = 12;
const INT LG_MEDIUM_FONT_TEXT_BOX_HEIGHT = 13;
const INT LG_BIG_FONT_TEXT_BOX_HEIGHT = 20;

// corresponding font size
const INT LG_SMALL_FONT_SIZE = 7;
const INT LG_MEDIUM_FONT_SIZE = 8;
const INT LG_BIG_FONT_SIZE = 12;

// logical origin Y value for various font sizes
const INT LG_SMALL_FONT_LOGICAL_ORIGIN_Y = -3;
const INT LG_MEDIUM_FONT_LOGICAL_ORIGIN_Y = -2;
const INT LG_BIG_FONT_LOGICAL_ORIGIN_Y = -4;

// Scrolling text parameters
const INT LG_SCROLLING_SPEED = 7;
const INT LG_SCROLLING_STEP = 7;
const INT LG_SCROLLING_DELAY_MS = 2000;
const TCHAR LG_SCROLLING_GAP_TEXT[] = _T("       ");

// Progress bar parameters
const INT LG_PROGRESS_BAR_RANGE_MIN = 0;
const INT LG_PROGRESS_BAR_RANGE_MAX = 100;
const INT LG_PROGRESS_BAR_INITIAL_HEIGHT = 5;

/****f* EZ.LCD.Wrapper/CEzLcd::CEzLcd
 * NAME
 *  CEzLcd::CEzLcd -- Constructor
 * FUNCTION
 *  Does necessary initialization.
 * INPUTS
 *  friendlyName  - friendly name of the applet/game. This name will be 
 *                  displayed in the Logitech G-series LCD Manager.
 *  width         - width in pixels of the LCD.
 *  height        - height in pixels of the LCD.
 ******
 */
CEzLcd::CEzLcd(LPCTSTR friendlyName, INT width, INT height)
{
	m_LcdWidth = width;
	m_LcdHeight = height;
	m_friendlyName = friendlyName;

	m_initNeeded = TRUE;
	m_initSucceeded = FALSE;

	for (INT ii = 0; ii < NUMBER_SOFT_BUTTONS; ii++)
	{
		m_buttonIsPressed[ii] = FALSE;
        m_buttonWasPressed[ii] = FALSE;
	}
}

CEzLcd::~CEzLcd()
{
    LCD_OBJECT_LIST::iterator it = m_Objects.begin();
    while(it != m_Objects.end())
    {
        CLCDBase *pObject = *it;
        LOGIASSERT(NULL != pObject);
        delete pObject;

        ++it;
    }

	m_output.Shutdown();
}

/****f* EZ.LCD.Wrapper/CEzLcd::IsConnected
 * NAME
 *  BOOL CEzLcd::IsConnected -- Check if a Logitech G-series LCD is connected.
 * RETURN VALUE 
 *  TRUE if an LCD is connected
 *  FALSE otherwise
 ******
 */
BOOL CEzLcd::IsConnected()
{
	return m_output.IsOpened();
}

/****f* EZ.LCD.Wrapper/CEzLcd::AddText
 * NAME
 *  HANDLE CEzLcd::AddText -- Add a text object to the LCD
 * INPUTS
 *  type            - specifies whether the text is static or 
 *                    scrolling. Possible types are: LG_SCROLLING_TEXT,
 *                    LG_STATIC_TEXT
 *  size            - size of the text. Choose between these three: 
 *                    LG_SMALL, LG_MEDIUM or LG_BIG.
 *  alignment       - alignment of the text. Values are: DT_LEFT, 
 *                    DT_CENTER, DT_RIGHT.
 *  maxLengthPixels - max length in pixels of the text. If the text is
 *                    longer and of type LG_STATIC_TEXT, it will be cut
 *                    off. If the text is longer and of type 
 *                    LG_SCROLLING_TEXT, it will scroll.
 * RETURN VALUE 
 *  Handle for this object.
 * SEE ALSO
 *  CEzLcd::AddIcon
 *  CEzLcd::AddProgressBar
 ******
 */
HANDLE CEzLcd::AddText(LGObjectType type, LGTextSize size, INT alignment, INT maxLengthPixels)
{
    LOGIASSERT(LG_SCROLLING_TEXT == type || LG_STATIC_TEXT == type);
	CLCDText* staticText;
	CLCDStreamingText* streamingText;

	INT boxHeight = LG_MEDIUM_FONT_TEXT_BOX_HEIGHT;
	INT fontSize = LG_MEDIUM_FONT_SIZE;
	INT localOriginY = LG_MEDIUM_FONT_LOGICAL_ORIGIN_Y;

	switch (type)
	{
	case LG_SCROLLING_TEXT:
		streamingText = new CLCDStreamingText();
		LOGIASSERT(NULL != streamingText);
		streamingText->Initialize();
		streamingText->SetOrigin(0, 0);
		streamingText->SetFontFaceName(_T("Microsoft Sans Serif"));
		streamingText->SetAlignment(alignment);
		streamingText->SetText(_T(" "));
		streamingText->SetGapText(LG_SCROLLING_GAP_TEXT);
		streamingText->SetSpeed(LG_SCROLLING_SPEED);
		streamingText->SetScrollingStep(LG_SCROLLING_STEP);
		streamingText->SetStartDelay(LG_SCROLLING_DELAY_MS);

		if (LG_SMALL == size)
		{
			boxHeight = LG_SMALL_FONT_TEXT_BOX_HEIGHT;
			fontSize = LG_SMALL_FONT_SIZE;
			localOriginY = LG_SMALL_FONT_LOGICAL_ORIGIN_Y;
		}
		else if (LG_MEDIUM == size)
		{
			boxHeight = LG_MEDIUM_FONT_TEXT_BOX_HEIGHT;
			fontSize = LG_MEDIUM_FONT_SIZE;
			localOriginY = LG_MEDIUM_FONT_LOGICAL_ORIGIN_Y;
		}
		else if (LG_BIG == size)
		{
			streamingText->SetFontWeight(FW_BOLD);
			boxHeight = LG_BIG_FONT_TEXT_BOX_HEIGHT;
			fontSize = LG_BIG_FONT_SIZE;
			localOriginY = LG_BIG_FONT_LOGICAL_ORIGIN_Y;
		}

		streamingText->SetSize(maxLengthPixels, boxHeight);		
		streamingText->SetFontPointSize(fontSize);
		streamingText->SetLogicalOrigin(0, localOriginY);
        streamingText->SetObjectType(LG_SCROLLING_TEXT);

		AddObject(streamingText);

		return streamingText;
		break;
	case LG_STATIC_TEXT:
		staticText = new CLCDText();
		LOGIASSERT(NULL != staticText);
		staticText->Initialize();
		staticText->SetOrigin(0, 0);
		staticText->SetFontFaceName(_T("Microsoft Sans Serif"));
		staticText->SetAlignment(alignment);
		//staticText->SetBackgroundMode(OPAQUE);
		staticText->SetBackgroundMode(TRANSPARENT);
		staticText->SetText(_T(" "));

		if (LG_SMALL == size)
		{
			boxHeight = LG_SMALL_FONT_TEXT_BOX_HEIGHT;
			fontSize = LG_SMALL_FONT_SIZE;
			localOriginY = LG_SMALL_FONT_LOGICAL_ORIGIN_Y;
		}
		else if (LG_MEDIUM == size)
		{
			boxHeight = LG_MEDIUM_FONT_TEXT_BOX_HEIGHT;
			fontSize = LG_MEDIUM_FONT_SIZE;
			localOriginY = LG_MEDIUM_FONT_LOGICAL_ORIGIN_Y;
		}
		else if (LG_BIG == size)
		{
			staticText->SetFontWeight(FW_BOLD);
			boxHeight = LG_BIG_FONT_TEXT_BOX_HEIGHT;
			fontSize = LG_BIG_FONT_SIZE;
			localOriginY = LG_BIG_FONT_LOGICAL_ORIGIN_Y;
		}

		staticText->SetSize(maxLengthPixels, boxHeight);
		staticText->SetFontPointSize(fontSize);
		staticText->SetLogicalOrigin(0, localOriginY);
        staticText->SetObjectType(LG_STATIC_TEXT);

		AddObject(staticText);

		return staticText;
		break;
	default:
		LOGITRACE(_T("ERROR: trying to add text object with undefined type\n"));
	}

	return NULL;
}

/****f* EZ.LCD.Wrapper/CEzLcd::SetText
 * NAME
 *  HRESULT CEzLcd::SetText -- Set text
 * INPUTS
 *  handle          - handle to the object.
 *  text            - text string.
 * RETURN VALUE 
 *  E_FAIL if there was an error.
 *  S_OK if no error.
 ******
 */
HRESULT CEzLcd::SetText(HANDLE handle, LPCTSTR text)
{
	CLCDBase* myObject = GetObject(handle);

	if (NULL != myObject)
	{
		LOGIASSERT(LG_STATIC_TEXT == myObject->GetObjectType() || LG_SCROLLING_TEXT == myObject->GetObjectType());
		if (LG_STATIC_TEXT == myObject->GetObjectType())
		{
			CLCDText* staticText = static_cast<CLCDText*>(myObject);
            LOGIASSERT(NULL != staticText);
			staticText->SetText(text);
			return S_OK;
		}
		else if (LG_SCROLLING_TEXT == myObject->GetObjectType())
		{
			CLCDStreamingText* streamingText = static_cast<CLCDStreamingText*>(myObject);
            LOGIASSERT(NULL != streamingText);
			streamingText->SetText(text);
			return S_OK;
		}
	}

	return E_FAIL;
}

/****f* EZ.LCD.Wrapper/CEzLcd::AddIcon
 * NAME
 *  HANDLE CEzLcd::AddIcon -- Add an icon object to the LCD
 * INPUTS
 *  icon            - icon to be displayed on the LCD. Should be 1 bpp
 *                    bitmap.
 *  sizeX           - x-axis size of the bitmap.
 *  sizeY           - y-axis size of the bitmap.
 * RETURN VALUE 
 *  Handle for this object.
 * SEE ALSO
 *  CEzLcd::AddText
 *  CEzLcd::AddProgressBar
 ******
 */
HANDLE CEzLcd::AddIcon(HICON icon, INT sizeX, INT sizeY)
{
	CLCDIcon* icon_ = new CLCDIcon();
	LOGIASSERT(NULL != icon_);
	icon_->Initialize();
	icon_->SetOrigin(0, 0);
	icon_->SetSize(sizeX, sizeY);
	icon_->SetIcon(icon, sizeX, sizeY);
    icon_->SetObjectType(LG_ICON);

	AddObject(icon_);

	return icon_;
}
/****f* EZ.LCD.Wrapper/CEzLcd::AddProgressBar
 * NAME
 *  HANDLE CEzLcd::AddProgressBar -- Add a progress bar object to the LCD.
 * INPUTS
 *  type            - type of the progress bar. Types are: LG_CURSOR, 
 *                    LG_FILLED.
 * RETURN VALUE 
 *  Handle for this object.
 * SEE ALSO
 *  CEzLcd::AddText
 *  CEzLcd::AddIcon
 ******
 */

HANDLE CEzLcd::AddProgressBar(LGProgressBarType type)
{
    LOGIASSERT(LG_FILLED == type || LG_CURSOR == type);
	CLCDProgressBar *progressBar = new CLCDProgressBar();
	LOGIASSERT(NULL != progressBar);
	progressBar->Initialize();
	progressBar->SetOrigin(0, 0);
	progressBar->SetSize(m_LcdWidth, LG_PROGRESS_BAR_INITIAL_HEIGHT);
	progressBar->SetRange(LG_PROGRESS_BAR_RANGE_MIN, LG_PROGRESS_BAR_RANGE_MAX );
	progressBar->SetPos(static_cast<FLOAT>(LG_PROGRESS_BAR_RANGE_MIN));
    progressBar->SetObjectType(LG_PROGRESS_BAR);

	if (LG_FILLED == type)
	{
		progressBar->EnableCursor(FALSE);
	}

	AddObject(progressBar);

	return progressBar;
}

/****f* EZ.LCD.Wrapper/CEzLcd::SetProgressBarPosition
 * NAME
 *  HRESULT CEzLcd::SetProgressBarPosition -- Set position of the 
 *  progress bar's cursor
 * INPUTS
 *  handle          - handle to the object.
 *  percentage      - percentage of progress (0 to 100).
 * RETURN VALUE 
 *  E_FAIL if there was an error or if handle does not correspond to a
 *  progress bar.
 *  S_OK if no error.
 ******
 */
HRESULT CEzLcd::SetProgressBarPosition(HANDLE handle, FLOAT percentage)
{
	CLCDBase* myObject = GetObject(handle);

	if (NULL != myObject)
	{
		LOGIASSERT(LG_PROGRESS_BAR == myObject->GetObjectType());
        // only allow this function for progress bars
        if (LG_PROGRESS_BAR == myObject->GetObjectType())
        {
			CLCDProgressBar *progressBar = static_cast<CLCDProgressBar*>(myObject);
            LOGIASSERT(NULL != progressBar);
			progressBar->SetPos(percentage);
			return S_OK;
        }
	}

	return E_FAIL;
}

/****f* EZ.LCD.Wrapper/CEzLcd::SetProgressBarSize
 * NAME
 *  HRESULT CEzLcd::SetProgressBarSize -- Set size of progress bar
 * INPUTS
 *  handle          - handle to the object.
 *  x               - x-axis part of the size
 *  y               - y-axis part of the size (a good default value is 5).
 * RETURN VALUE 
 *  E_FAIL if there was an error or if handle does not correspond to a
 *  progress bar.
 *  S_OK if no error.
 ******
 */
HRESULT CEzLcd::SetProgressBarSize(HANDLE handle, INT x, INT y)
{
	CLCDBase* myObject = GetObject(handle);
    LOGIASSERT(NULL != myObject);
    LOGIASSERT(NULL != myObject);

	if (NULL != myObject && NULL != myObject)
	{
		LOGIASSERT(LG_PROGRESS_BAR == myObject->GetObjectType());
        // only allow this function for progress bars
        if (LG_PROGRESS_BAR == myObject->GetObjectType())
        {
			myObject->SetSize(x, y);
			return S_OK;
        }
	}

	return E_FAIL;
}

/****f* EZ.LCD.Wrapper/CEzLcd::SetOrigin
 * NAME
 *  HRESULT CEzLcd::SetOrigin -- Set the origin of an object. The 
 *                  origin corresponds to the furthest pixel on the 
 *                  upper left corner of an object.
 * INPUTS
 *  handle          - handle to the object.
 *  x               - x-axis part of the origin.
 *  y               - y-axis part of the origin.
 * RETURN VALUE 
 *  E_FAIL if there was an error.
 *  S_OK if no error.
 ******
 */
HRESULT CEzLcd::SetOrigin(HANDLE handle, INT x, INT y)
{
	CLCDBase* myObject = GetObject(handle);
    LOGIASSERT(NULL != myObject);
    LOGIASSERT(NULL != myObject);

	if (NULL != myObject && NULL != myObject)
	{
		myObject->SetOrigin(x, y);
		return S_OK;
	}

	return E_FAIL;
}

/****f* EZ.LCD.Wrapper/CEzLcd::SetVisible
 * NAME
 *  HRESULT CEzLcd::SetVisible -- set corresponding object to be 
 *  visible or invisible.
 * INPUTS
 *  handle          - handle to the object.
 *  visible         - set to FALSE to make object invisible, TRUE to 
 *                    make it visible (default).
 * RETURN VALUE 
 *  E_FAIL if there was an error.
 *  S_OK if no error.
 ******
 */
HRESULT CEzLcd::SetVisible(HANDLE handle, BOOL visible)
{
	CLCDBase* myObject = GetObject(handle);
    LOGIASSERT(NULL != myObject);
    LOGIASSERT(NULL != myObject);

	if (NULL != myObject && NULL != myObject)
	{        
		myObject->Show(visible);
		return S_OK;
	}

	return E_FAIL;
}

/****f* EZ.LCD.Wrapper/CEzLcd::ButtonTriggered
 * NAME
 *  BOOL CEzLcd::ButtonTriggered -- Check if a button was triggered.
 * INPUTS
 *  button      - name of the button to be checked. Possible names are:
 *                LG_BUTTON_1, LG_BUTTON_2, LG_BUTTON_3, LG_BUTTON_4
 * RETURN VALUE 
 *  TRUE if the specific button was triggered
 *  FALSE otherwise
 * SEE ALSO
 *  CEzLcd::ButtonReleased
 *  CEzLcd::ButtonIsPressed
 ******
 */
BOOL CEzLcd::ButtonTriggered(INT button)
{
    if (m_buttonIsPressed[button] && !m_buttonWasPressed[button])
    {
        return TRUE;
    }

	return FALSE;
}

/****f* EZ.LCD.Wrapper/CEzLcd::ButtonReleased
 * NAME
 *  BOOL CEzLcd::ButtonReleased -- Check if a button was released.
 * INPUTS
 *  button      - name of the button to be checked. Possible names are:
 *                LG_BUTTON_1, LG_BUTTON_2, LG_BUTTON_3, LG_BUTTON_4
 * RETURN VALUE 
 *  TRUE if the specific button was released
 *  FALSE otherwise
 * SEE ALSO
 *  CEzLcd::ButtonTriggered
 *  CEzLcd::ButtonIsPressed
 ******
 */
BOOL CEzLcd::ButtonReleased(INT button)
{
    if (!m_buttonIsPressed[button] && m_buttonWasPressed[button])
    {
        return TRUE;
    }
	return FALSE;
}

/****f* EZ.LCD.Wrapper/CEzLcd::ButtonIsPressed
 * NAME
 *  BOOL CEzLcd::ButtonIsPressed -- Check if a button is being pressed.
 * INPUTS
 *  button      - name of the button to be checked. Possible names are:
 *                LG_BUTTON_1, LG_BUTTON_2, LG_BUTTON_3, LG_BUTTON_4
 * RETURN VALUE 
 *  TRUE if the specific button is being pressed
 *  FALSE otherwise
 * SEE ALSO
 *  CEzLcd::ButtonTriggered
 *  CEzLcd::ButtonReleased
 ******
 */
BOOL CEzLcd::ButtonIsPressed(INT button)
{
	return m_buttonIsPressed[button];
}

/****f* EZ.LCD.Wrapper/CEzLcd::UpdateLCD
 * NAME
 *  void CEzLcd::UpdateLCD -- Update LCD display
 * FUNCTION
 *  Updates the display. Must be called every loop.
 ******
 */
void CEzLcd::UpdateLCD(DWORD dwTimestamp)
{
	if (m_initNeeded)
	{
		SetExpiration(INFINITE);

		m_output.AddScreen(this);

		// Setup registration and configure contexts.
		lgLcdConnectContext lgdConnectContext;
		lgLcdConfigureContext lgdConfigureContext;

		// Further expansion of registration and configure contexts.
		lgdConfigureContext.configCallback = NULL;

		lgdConnectContext.appFriendlyName = m_friendlyName;
		lgdConnectContext.isPersistent = FALSE;
		lgdConnectContext.isAutostartable = FALSE;
		lgdConnectContext.onConfigure = lgdConfigureContext;

		if (FAILED(m_output.Initialize(&lgdConnectContext, FALSE)))
		{
			// This means the LCD SDK's lgLcdInit failed, and therefore
			// we will not be able to ever connect to the LCD, even if
			// a G-series keyboard is actually connected.
			LOGITRACE(_T("ERROR: LCD SDK initialization failed\n"));
			m_initSucceeded = FALSE;
		}
		else
		{
			m_output.LockScreen(this);
			m_initSucceeded = TRUE;
		}

		m_initNeeded = FALSE;
	}

	// Only do stuff if initialization was successful. Otherwise 
	// IsConnected will simply return false.
	if (m_initSucceeded)
	{
        // Save copy of button state
        for (INT ii = 0; ii < NUMBER_SOFT_BUTTONS; ii++)
	    {
		    m_buttonWasPressed[ii] = m_buttonIsPressed[ii];
	    }

		m_output.Update(dwTimestamp);
		m_output.Draw();
	}
}

CLCDBase* CEzLcd::GetObject(HANDLE handle)
{
    LCD_OBJECT_LIST::iterator it = m_Objects.begin();
    while(it != m_Objects.end())
    {
        CLCDBase *pObject = *it;
        LOGIASSERT(NULL != pObject);

        if (pObject == handle)
        {
            return pObject;
        }
        ++it;
    }

	return NULL;
}

VOID CEzLcd::OnLCDButtonDown(INT nButton)
{
	switch(nButton)
	{
	case LGLCDBUTTON_BUTTON0:
		m_buttonIsPressed[LG_BUTTON_1] = TRUE;
		break;
	case LGLCDBUTTON_BUTTON1:
		m_buttonIsPressed[LG_BUTTON_2] = TRUE;
		break;
	case LGLCDBUTTON_BUTTON2:
		m_buttonIsPressed[LG_BUTTON_3] = TRUE;
		break;
	case LGLCDBUTTON_BUTTON3:
		m_buttonIsPressed[LG_BUTTON_4] = TRUE;
		break;
	default:
		LOGITRACE(_T("ERROR: unknown button was pressed\n"));
		break;
	}
}

void CEzLcd::OnLCDButtonUp(int nButton)
{
	switch(nButton)
	{
	case LGLCDBUTTON_BUTTON0:
		m_buttonIsPressed[LG_BUTTON_1] = FALSE;
		break;
	case LGLCDBUTTON_BUTTON1:
		m_buttonIsPressed[LG_BUTTON_2] = FALSE;
		break;
	case LGLCDBUTTON_BUTTON2:
		m_buttonIsPressed[LG_BUTTON_3] = FALSE;
		break;
	case LGLCDBUTTON_BUTTON3:
		m_buttonIsPressed[LG_BUTTON_4] = FALSE;
		break;
	default:
		LOGITRACE(_T("ERROR: unknown button was pressed\n"));
		break;
	}
}

void CEzLcd::Update(DWORD dwTimestamp)
{
	CLCDManager::Update(dwTimestamp);
}
    
void CEzLcd::HideLCD(BOOL bHide)
{
	if (bHide)
	{
		m_output.SetScreenPriority(LGLCD_PRIORITY_IDLE_NO_SHOW);
	}
	else
	{
		m_output.SetScreenPriority(LGLCD_PRIORITY_NORMAL);
	}
}

void CEzLcd::RemoveAndDestroyObject( HANDLE hObject )
{
	CLCDBase* myObject = GetObject(hObject);
	LCD_OBJECT_LIST::iterator it = m_Objects.begin();
    while(it != m_Objects.end())
    {
        CLCDBase *pObject = *it;
        LOGIASSERT(NULL != pObject);

        if (pObject == myObject)
        {
			m_Objects.erase( it );
			delete myObject;
            return;
        }
        ++it;
    }
}

class CG15 : public IG15
{
public:
	CG15() : 
	  m_pLcd( NULL )
	{
	}

	virtual ~CG15()
	{
		// User of .dll should have called Shutdown() method!!!
		Assert( m_pLcd == NULL );
	}

	virtual void		GetLCDSize( int &w, int &h )
	{
		w = LGLCD_BMP_WIDTH;
		h = LGLCD_BMP_HEIGHT;
	}
	
	// w, h should match the return value from GetLCDSize!!!
	virtual bool		Init( char const *name )
	{
		if ( m_pLcd )
			return true;

		m_pLcd = new CEzLcd( name, LGLCD_BMP_WIDTH, LGLCD_BMP_HEIGHT );
		return true;
	}

	virtual void		Shutdown()
	{
		if ( m_pLcd )
		{
			m_pLcd->Shutdown();
		}

		delete m_pLcd;
		m_pLcd = NULL;
	}

	virtual bool		IsConnected()
	{
		if ( !m_pLcd )
			return false;

		if ( m_pLcd->IsConnected() )
		{
			return true;
		}
		return false;
	}

	// Add/remove
	virtual G15_HANDLE	AddText(G15ObjectType type, G15TextSize size, int alignment, int maxLengthPixels)
	{
		if ( !m_pLcd )
			return (G15_HANDLE)0;

		return (G15_HANDLE)m_pLcd->AddText( (LGObjectType)type, (LGTextSize)size, alignment, maxLengthPixels );
	}

	virtual G15_HANDLE	AddIcon( void *icon, int sizeX, int sizeY)
	{
		if ( !m_pLcd )
			return (G15_HANDLE)0;

		return (G15_HANDLE)m_pLcd->AddIcon( (HICON)icon, sizeX, sizeY );
	}

	virtual void		RemoveAndDestroyObject( G15_HANDLE hObject )
	{
		if ( !m_pLcd )
			return;
		m_pLcd->RemoveAndDestroyObject( (HANDLE)hObject );
	}

	// Change
	virtual int			SetText(G15_HANDLE handle, char const * text)
	{
		if ( !m_pLcd )
			return (int)E_FAIL;

		return m_pLcd->SetText( (HANDLE)handle, (LPCTSTR)text );
	}

	virtual int			SetOrigin(G15_HANDLE handle, int x, int y)
	{
		if ( !m_pLcd )
			return (int)E_FAIL;

		return m_pLcd->SetOrigin( (HANDLE)handle, x, y );
	}

	virtual int			SetVisible(G15_HANDLE handle, bool visible) 
	{
		if ( !m_pLcd )
			return (int)E_FAIL;

		return m_pLcd->SetVisible( (HANDLE)handle, (BOOL)visible );
	}

	virtual bool		ButtonTriggered(int button)
	{
		if ( !m_pLcd )
			return false;

		if ( m_pLcd->ButtonTriggered( button ) )
			return true;

		return false;
	}

	virtual void		UpdateLCD( unsigned int dwTimestamp )
	{
		if ( m_pLcd )
		{
			m_pLcd->UpdateLCD( (DWORD)dwTimestamp );
		}
	}

private:

	CEzLcd *m_pLcd;
};

EXPOSE_SINGLE_INTERFACE( CG15, IG15, G15_INTERFACE_VERSION );



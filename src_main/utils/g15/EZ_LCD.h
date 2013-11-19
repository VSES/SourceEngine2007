#ifndef EZLCD_H_INCLUDED_
#define EZLCD_H_INCLUDED_

#include "LCDManager.h"
#include "LCDOutput.h"

const INT NUMBER_SOFT_BUTTONS = 4;

typedef enum
{
    LG_BUTTON_1, LG_BUTTON_2, LG_BUTTON_3, LG_BUTTON_4
} LGSoftButton;

typedef enum
{
    LG_SMALL, LG_MEDIUM, LG_BIG
} LGTextSize;

typedef enum
{
    LG_CURSOR, LG_FILLED
} LGProgressBarType;

class CEzLcd: public CLCDManager
{
public:
	CEzLcd(LPCTSTR friendlyName, INT width, INT height);
	~CEzLcd();

	BOOL IsConnected();

	HANDLE AddText(LGObjectType type, LGTextSize size, INT alignment, INT maxLengthPixels);
	HRESULT SetText(HANDLE handle, LPCTSTR text);

	HANDLE AddIcon(HICON icon, INT sizeX, INT sizeY);

	HANDLE AddProgressBar(LGProgressBarType type);
	HRESULT SetProgressBarPosition(HANDLE handle, FLOAT percentage);
    HRESULT SetProgressBarSize(HANDLE handle, INT x, INT y);

	HRESULT SetOrigin(HANDLE handle, INT x, INT y);
	HRESULT SetVisible(HANDLE handle, BOOL visible);

	BOOL ButtonTriggered(INT button);
    BOOL ButtonReleased(INT button);
	BOOL ButtonIsPressed(INT button);

	BOOL IsLCDAttached();
	void HideLCD(BOOL bHide);
	void UpdateLCD(DWORD dwTimestamp);

	void	RemoveAndDestroyObject( HANDLE hObject );

private:
	virtual void Update(DWORD dwTimestamp);

	INT m_LcdWidth;
	INT m_LcdHeight;
	LPCTSTR m_friendlyName;

	CLCDOutput m_output;

	BOOL m_initNeeded;
	BOOL m_initSucceeded;

    BOOL m_buttonIsPressed[NUMBER_SOFT_BUTTONS];
	BOOL m_buttonWasPressed[NUMBER_SOFT_BUTTONS];

	CLCDBase* GetObject(HANDLE handle);

	VOID OnLCDButtonDown(INT nButton);
    VOID OnLCDButtonUp(INT nButton);
};

#endif
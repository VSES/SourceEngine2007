//************************************************************************
//
// LCDOutput.h
//
// The CLCDOutput class manages LCD hardware enumeration and screen 
// management.
// 
// Logitech LCD SDK
//
// Copyright 2005 Logitech Inc.
//************************************************************************

#ifndef _CLCDOUTPUT_H_INCLUDED_ 
#define _CLCDOUTPUT_H_INCLUDED_ 

#include "LCDManager.h"
#include "lglcd.h"
#include "vector"

using namespace std;

typedef vector <CLCDManager*> LCD_MGR_LIST;
typedef LCD_MGR_LIST::iterator LCD_MGR_LIST_ITER;

class CLCDOutput : public CLCDManager
{

public:
    CLCDOutput();
    virtual ~CLCDOutput();

    void AddScreen(CLCDManager* pScreen);

    void LockScreen(CLCDManager* pScreen);
    void UnlockScreen();
    BOOL IsLocked();

	BOOL IsOpened();
    
	void SetScreenPriority(DWORD priority);

    HRESULT Initialize(lgLcdConnectContext* pContext, BOOL bUseWindow = FALSE);

    // returns TRUE if a new display was enumerated
    BOOL HasHardwareChanged(void);

    // CLCDBase
    virtual HRESULT Initialize();
    virtual HRESULT Draw();
    virtual void Update(DWORD dwTimestamp);
    virtual void Shutdown(void);

    // CLCDManager
    lgLcdBitmap160x43x1 *GetLCDScreen(void);
    BITMAPINFO *GetBitmapInfo(void);

protected:
    void ActivateScreen(CLCDManager* pScreen);
    void ReadButtons();
    void HandleButtonState(DWORD dwButtonState, DWORD dwButton);
    void HandleErrorFromAPI(DWORD dwRes);
    void CloseAndDisconnect();

    virtual void OnLCDButtonDown(int nButton);
    virtual void OnLCDButtonUp(int nButton);

protected:
    virtual void OnScreenExpired(CLCDManager* pScreen);
    virtual void OnClosingDevice(int hDevice);
    virtual void OnDisconnecting(int hConnection);

protected:
    CLCDManager* m_pActiveScreen;

    // list 
    LCD_MGR_LIST m_LCDMgrList;

    void EnumerateDevices();
    int m_hConnection;
    int m_hDevice;
    DWORD m_nPriority;
    BOOL m_bLocked, m_bDisplayLocked;
    DWORD m_dwButtonState;
    
    lgLcdConnectContext m_lcdConnectCtx;
};

#endif // !_CLCDOUTPUT_H_INCLUDED_ 

//** end of CLCDOutput.h *************************************************

#include "trayicon.h"
#include "D3dx12jo.h"

// User defined Constants:

#define WM_TPSCREENSAVER  WM_USER + 0x05

//////////////////
// Main frame for TRAYTEST.
//
class CMainFrame : public CFrameWnd {
public:
	BOOL MonitorOffSupport();
	CMainFrame();
	BOOL IsShiftKeyDown();
	D3dx12jo* m_pD3jo;

	virtual ~CMainFrame();
protected:
	DECLARE_DYNAMIC(CMainFrame)
	CStatusBar	m_wndStatusBar;

	CTrayIcon	m_trayIcon;		// my tray icon
	//CEdit			m_wndEdit;		// to display tray notifications
	int			m_iWhichIcon;	// 0/1 which HICON to use
	BOOL			m_bShutdown;	// OK to terminate TRAYTEST
	//BOOL			m_bShowTrayNotifications;	// display info in main window

	//{{AFX_MSG(CMainFrame)
	afx_msg LRESULT OnTrayNotification(WPARAM wp, LPARAM lp);
	afx_msg int	 OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnClose();
	afx_msg void OnAppTpExit();
	afx_msg void OnTpLogOff();
	afx_msg void OnTpShutDown();
	afx_msg void OnTpRestart();
	afx_msg void OnTpStandBy();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnTpScreenSave();
	afx_msg void OnEndSession(BOOL bEnding);
	void ResetTimerTicks();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	void DoMenuDropDown();
	UINT_PTR m_nTimer;
	UINT m_BtnClicked;
	UINT m_nTicks;
	UINT m_DblClicked;
	void OnStopTimer();
	void OnStartTimer();
	void TpExitWindows(BOOL Force,int shtdn);
};

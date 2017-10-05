////////////////////////////////////////////////////////////////
// Main frame window implementation
//

#include "stdafx.h"
#include "TrayPwr.h"
#include "mainfrm.h"
#include <VersionHelpers.h>

// Message ID used for tray notifications
#define WM_MY_TRAY_NOTIFICATION WM_USER+0
#define EWX_FORCEIFHUNG      0x00000010

IMPLEMENT_DYNAMIC(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_MESSAGE(WM_MY_TRAY_NOTIFICATION, OnTrayNotification)
	ON_WM_CREATE()
	ON_WM_CLOSE()
	ON_COMMAND(ID_APPTP_EXIT, OnAppTpExit)
	ON_COMMAND(ID_TPLOGOFF, OnTpLogOff)
	ON_COMMAND(ID_TPSHUTDOWN, OnTpShutDown)
	ON_COMMAND(ID_TPRESTART, OnTpRestart)
	ON_COMMAND(ID_TPSTANDBY, OnTpStandBy)
	ON_WM_TIMER()
	ON_COMMAND(ID_TPSCREENSAVE, OnTpScreenSave)
	ON_WM_ENDSESSION()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

static UINT BASED_CODE indicators[] = {
	ID_SEPARATOR,				// status line indicator
};

CMainFrame::CMainFrame() : m_trayIcon(IDR_TRAYICON)
{
	m_bShutdown = FALSE;
	m_DblClicked = FALSE;
	m_BtnClicked = 0;
	m_nTicks = 0;
	m_nTimer = 0;
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators, 
		sizeof(indicators)/sizeof(UINT))) 
		return -1;		 // fail to create

	//assign global hwnd for d3dx12
	//JJJ SET hwnd_jo IN DXjo class = m_hWnd;
	D3dx12jo* pD3jo = new D3dx12jo();
	m_pD3jo = pD3jo;
	if (!m_pD3jo->InitD3D(m_hWnd))
	{
		//MessageBox(0, L"Failed to initialize direct3d 12", L"Error", MB_OK);
		m_pD3jo->Cleanup();
		return -1;
	}

	// Set up tray icon
	m_trayIcon.SetNotificationWnd(this, WM_MY_TRAY_NOTIFICATION);
	m_iWhichIcon = 1;
	m_trayIcon.SetIcon(IDI_MYICON);

	// start timer for dx rendering and managing messages
	OnStartTimer();

	return 0;
}

//////////////////
// Close window. Unless we are shutting down, just hide it.
//
void CMainFrame::OnClose() 
{
	if (m_bShutdown)
		CFrameWnd::OnClose();
	else
		ShowWindow(SW_HIDE);
}

//////////////////
// Handle notification from tray icon: display a message.
//
LRESULT CMainFrame::OnTrayNotification(WPARAM uID, LPARAM lEvent)
{

	switch (lEvent)
	{
	case WM_LBUTTONUP: 
		//if one click then turn monitor off:
		m_BtnClicked=WM_LBUTTONUP;
		ResetTimerTicks();
		break;
	case WM_LBUTTONDBLCLK: 
		if (m_nTimer!=0) 
		{
			m_DblClicked=TRUE;
			m_BtnClicked = 0;
			if (IsShiftKeyDown())
			{
				OnTpStandBy();
			}else{ 
				//OnTpShutDown();
			}
		}
		break;
	case WM_RBUTTONUP: 
		m_BtnClicked=WM_RBUTTONUP;
		ResetTimerTicks();
		break;
	case WM_RBUTTONDBLCLK: 
		if (m_nTimer!=0) 
		{
			m_DblClicked=TRUE;
			if (IsShiftKeyDown())
			{
				OnTpLogOff();
			}else{ 
				OnTpRestart();
			}
		}
		break;
	}
	// let tray icon do default stuff
	return 0;
}

////////////////////////////////////////////////////////////////
// Command handlers below.
//

void CMainFrame::OnAppTpExit() 
{
	m_bShutdown = TRUE;		// really exit
	//clean up dx12:
	OnStopTimer();
	m_pD3jo->CloseFenceHandle();
	m_pD3jo->Cleanup();
	//close and exit:
	SendMessage(WM_CLOSE);	
}


void CMainFrame::OnTpLogOff()
{
	TpExitWindows(FALSE,ID_TPLOGOFF);
}
void CMainFrame::OnTpShutDown()
{
	TpExitWindows(FALSE,ID_TPSHUTDOWN);
}
void CMainFrame::OnTpRestart()
{
	TpExitWindows(FALSE,ID_TPRESTART);
}
void CMainFrame::OnTpStandBy()
{
	TpExitWindows(FALSE,ID_TPSTANDBY);
}
void CMainFrame::OnTpScreenSave() 
{
	// TODO: Add your command handler code here
	m_BtnClicked=WM_TPSCREENSAVER;
	ResetTimerTicks();
}

void CMainFrame::TpExitWindows(BOOL Force,int shtdn)
{

	if(IsWindowsVersionOrGreater(4, 0, 0)) {
        // Get the access token of the current process.  Get it
        // with the privileges of querying the access token and
        // adjusting its privileges.
		HANDLE hToken=0;
		LUID tLuid;
		TOKEN_PRIVILEGES tTokenPriv;

		BOOL bRes=OpenProcessToken(GetCurrentProcess(), 
            TOKEN_ADJUST_PRIVILEGES + TOKEN_QUERY, &hToken);
		if (bRes==0L){
			return; //Failed
		}
        // Get the locally unique identifier (LUID) which
        // represents the shutdown privilege.
		if (LookupPrivilegeValue(0L, "SeShutdownPrivilege", &tLuid)==0L)
			return; //Failed

        // Populate the new TOKEN_PRIVILEGES values with the LUID
        // and allow your current process to shutdown the computer.
		tTokenPriv.PrivilegeCount=1;
		tTokenPriv.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
		tTokenPriv.Privileges[0].Luid = tLuid;

		AdjustTokenPrivileges(hToken, FALSE, &tTokenPriv, sizeof(TOKEN_PRIVILEGES),
			(PTOKEN_PRIVILEGES) NULL, (PDWORD) NULL);
	} //Now we have the correct privileges, hopefully.
	
	//Perform desired windows power-down:
	
	UINT FORCE_FLAG=(Force == TRUE ? EWX_FORCEIFHUNG : 0L);

	switch (shtdn)
	{
	case ID_TPLOGOFF:{
		ExitWindowsEx(FORCE_FLAG | EWX_LOGOFF, 0L);
		}
		break;

	case ID_TPSHUTDOWN:{
		ExitWindowsEx(FORCE_FLAG | EWX_SHUTDOWN | EWX_POWEROFF, 0L);
		}
		break;

	case ID_TPRESTART:{
		ExitWindowsEx(FORCE_FLAG | EWX_REBOOT, 0L);
		}
		break;

	case ID_TPSTANDBY:{
		SetSystemPowerState(1L, 1L);
		}
		break;
	}

}

void CMainFrame::OnTimer(UINT_PTR nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	if (m_DblClicked==FALSE) 
	{
		
		m_nTicks++;
		if (m_nTicks>2)
		{
			//Some useful debugging code:
			//CString msg;
			//msg.Format("Hello: m_nTicks=%d", m_nTicks);   
			//MessageBeep(0xFFFFFFFF);   // Beep
			//MessageBox(msg); 
			switch (m_BtnClicked)
			{
			case WM_RBUTTONUP:
				DoMenuDropDown();
				m_BtnClicked = 0;
				break;
			case WM_LBUTTONUP:
				//if one click then turn monitor off:
				if (m_nTicks>18) // but wait longer: 18x50 =900 msec.
				{
					if (MonitorOffSupport()==TRUE)
					{
						SendMessage(WM_SYSCOMMAND, SC_MONITORPOWER, 2L);
					}
					else //no monitor-off support, just kick in screen saver.
					{
						SendMessage(WM_SYSCOMMAND, SC_SCREENSAVE, 0L);
					}
					m_BtnClicked = 0;
				}
				break;
			case WM_TPSCREENSAVER:
				if (m_nTicks>10) // but wait longer
				{
					SendMessage(WM_SYSCOMMAND, SC_SCREENSAVE, 0L);
					m_BtnClicked = 0;
				}
				break;
			default:
				if (m_nTicks > 100)
				{
					//TRACE(_T("OnTimer - default, %d ticks.\n", m_nTicks));
					m_pD3jo->Render();
					ResetTimerTicks();

				}
				break;
			}
		}
	}
	else //m_DblClicked=TRUE;
	{
		m_DblClicked=FALSE;
		m_BtnClicked = 0;
		ResetTimerTicks();
	}
	CFrameWnd::OnTimer(nIDEvent);
}

void CMainFrame::ResetTimerTicks()
{
	m_nTicks = 0;
}

void CMainFrame::OnStartTimer()
{
	m_nTicks = 0;
	m_nTimer = SetTimer(1, 50, 0);
}

void CMainFrame::OnStopTimer()
{
	if (m_nTimer != 0)
	{
		KillTimer(m_nTimer);
		m_nTimer = 0;
	}
}

void CMainFrame::DoMenuDropDown()
{
	CMenu menu;
	if (!menu.LoadMenu(IDR_TRAYICON))
		return;
	CMenu* pSubMenu = menu.GetSubMenu(0);
	if (!pSubMenu) 
		return;

		// Make first menu item the default (bold font)
		//::SetMenuDefaultItem(pSubMenu->m_hMenu, 0, TRUE);

		// Display the menu at the current mouse location. There's a "bug"
		// (Microsoft calls it a feature) in Windows 95 that requires calling
		// SetForegroundWindow. To find out more, search for Q135788 in MSDN.
		//
		CPoint mouse;
		GetCursorPos(&mouse);
		::SetForegroundWindow(m_hWnd);	
		::TrackPopupMenu(pSubMenu->m_hMenu, 0, mouse.x, mouse.y, 0,
			m_hWnd, NULL);
}

BOOL CMainFrame::IsShiftKeyDown()
{
	return ((GetAsyncKeyState(VK_SHIFT) & 0x8000)!=0);
}

// Function MonitorOffSupport()
//This function determines if the operating system supports monitor off
//Returns false for NT4, true for Win95,98,2000
BOOL CMainFrame::MonitorOffSupport()
{
	if(IsWindowsVersionOrGreater(4, 0, 0)) {
		if (IsWindowsVersionOrGreater(5, 0, 0))
		{
			return TRUE; // NT 5 or more
		}
		return FALSE; // NT 4 or less
	}
	return TRUE; //Win95,98 etc...
}

void CMainFrame::OnEndSession(BOOL bEnding) 
{
	CFrameWnd::OnEndSession(bEnding);
	
	// Force app to quit:
	OnAppTpExit();
}

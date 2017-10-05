// AboutDlg.cpp : implementation file
//

#include "stdafx.h"
#include "traypwr.h"
#include "AboutDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog


BOOL CAboutDlg::m_bVisible = FALSE;
HWND CAboutDlg::m_MyhWnd = NULL;

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	m_hChildProcess = NULL;
	m_dwChildProcessId = 0;
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
    //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
	ON_WM_DESTROY()
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDSYSINFO, OnSysinfo)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CAboutDlg::OnInitDialog() 
{
	const char *mylic =
		"MIT License\r\n\r\n"
		"Copyright© 2017 J.S. Obeid\r\n\r\n"
		"Permission is hereby granted, free of charge, to any person obtaining a copy\r\n"
		"of this software and associated documentation files(the \"Software\"), to deal\r\n"
		"in the Software without restriction, including without limitation the rights\r\n"
		"to use, copy, modify, merge, publish, distribute, sublicense, and / or sell\r\n"
		"copies of the Software, and to permit persons to whom the Software is\r\n"
		"furnished to do so, subject to the following conditions : \r\n\r\n"
		"The above copyright notice and this permission notice shall be included in\r\n"
		"all copies or substantial portions of the Software.\r\n\r\n"
		"THE SOFTWARE IS PROVIDED \"AS IS\", WITHOUT WARRANTY OF ANY KIND,\r\n"
		"EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES\r\n"
		"OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND \r\n"
		"NONINFRINGEMENT.IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT\r\n"
		"HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,\r\n"
		"WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,\r\n"
		"OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER\r\n"
		"DEALINGS IN THE SOFTWARE.";
	CDialog::OnInitDialog();

	GetDlgItem(IDC_EDITLIC)->SetWindowTextA(mylic);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CAboutDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);
}

void CAboutDlg::OnDestroy() 
{
	CDialog::OnDestroy();
	m_MyhWnd=NULL;
    //CSystemTray::MinimiseToTray(this);
    m_bVisible = FALSE;
}

void CAboutDlg::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
    m_MyhWnd=m_hWnd;
    //if (bShow) CSystemTray::MaximiseFromTray(this);
}

INT_PTR CAboutDlg::DoModal()
{
    if (m_bVisible)
	{	
		BringToTop();
        return IDCANCEL;
	}
    m_bVisible = TRUE;
	return CDialog::DoModal();
}


/////////////////////////////////////////////////////////////////////////////
// CAboutDlg message handlers

void CAboutDlg::OnSysinfo() 
{
	StartSysInfo();
}

void CAboutDlg::BringToTop()
{
	if (m_MyhWnd!=0) 
	{
		::SetWindowPos(m_MyhWnd,HWND_TOP,0,0,0,0,SWP_NOMOVE | SWP_NOSIZE);
	}
}

BOOL CAboutDlg::OnCommand(WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CDialog::OnCommand(wParam, lParam);
}

//-----------------------------------------------------------
//
// StartSysInfo()
// Calls MSINFO32.EXE
//
BOOL CAboutDlg::StartSysInfo()
{
	OnInvokeMSInfo("msinfo32.exe");
	return TRUE;
}
//-----------start external exe functions:
void CAboutDlg::OnInvokeMSInfo(LPSTR SomeExeNm)
{
	PROCESS_INFORMATION pi;
	STARTUPINFO si;
	ZeroMemory(&si, sizeof(STARTUPINFO));
	si.cb = sizeof(STARTUPINFO);

	if (WaitForSingleObject(m_hChildProcess, 0) != WAIT_TIMEOUT &&
		CreateProcess(NULL, SomeExeNm,
			NULL, NULL,
			FALSE, 0,
			NULL, NULL,
			&si, &pi))
	{
		// Close the old process handle
		CloseHandle(m_hChildProcess);

		// Close the thread handle (process handle will be closed in ExitInstance)
		CloseHandle(pi.hThread);

		// store child process information
		m_hChildProcess = pi.hProcess;
		m_dwChildProcessId = pi.dwProcessId;
	}
	else
	{
		// ActivateChildProcess() activates all top-level windows whose PID matches m_dwChildProcessId.
		EnumWindows((WNDENUMPROC)CAboutDlg::ActivateChildProcess,
			(LPARAM)m_dwChildProcessId);
	}
	WaitForInputIdle(m_hChildProcess, 1000);
}

BOOL CALLBACK CAboutDlg::CloseChildProcess(HWND hWnd, LPARAM lParam)
{
	DWORD dwID;
	GetWindowThreadProcessId(hWnd, &dwID);
	if (dwID == (DWORD)lParam)
	{
		::PostMessage(hWnd, WM_CLOSE, 0, 0);
	}
	return TRUE;
}

BOOL CALLBACK CAboutDlg::ActivateChildProcess(HWND hWnd, LPARAM lParam)
{
	DWORD dwID;
	GetWindowThreadProcessId(hWnd, &dwID);
	if (dwID == (DWORD)lParam)
	{
		// Note: This block is adapted from CFrameWnd::ActivateFrame
		// and from CFrameWnd::BringToTop.
		int nCmdShow = -1;
		HWND hWndLastPop;

		// translate default nCmdShow (-1)
		if (nCmdShow == -1)
		{
			if (::IsIconic(hWnd))
				nCmdShow = SW_RESTORE;
		}
		ASSERT(nCmdShow != SW_HIDE &&
			nCmdShow != SW_MINIMIZE && nCmdShow != SW_SHOWMINNOACTIVE &&
			nCmdShow != SW_SHOWNA && nCmdShow != SW_SHOWNOACTIVATE);

		// bring to top before showing
		// if no last active popup, it will return hWnd
		hWndLastPop = ::GetLastActivePopup(hWnd);
		::BringWindowToTop(hWndLastPop);

		if (nCmdShow != -1)
		{
			// show the window as specified
			::ShowWindow(hWnd, nCmdShow);

			// and finally, bring to top after showing
			// if no last active popup, it will return hWnd
			hWndLastPop = ::GetLastActivePopup(hWnd);
			::BringWindowToTop(hWndLastPop);
		}

		::PostMessage(hWnd, WM_SETFOCUS, 0, 0);
	}
	return TRUE;
}

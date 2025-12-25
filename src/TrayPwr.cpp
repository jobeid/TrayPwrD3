////////////////////////////////////////////////////////////////
// TrayPwr Adopted from Microsoft Systems Journal's CTrayIcon by Paul DiLascia.
//
// All the activity takes place in MainFrm.cpp.

#include "stdafx.h"
#include "TrayPwr.h"
#include "mainfrm.h"
#include "AboutDlg.h"
#include <io.h>
#include <TlHelp32.h>

CMyApp theApp;

BEGIN_MESSAGE_MAP(CMyApp, CWinApp)
	//{{AFX_MSG_MAP(CMyApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

namespace {
	// Return PID of first other process that has the same executable name as this process.
	DWORD FindOtherInstancePid()
	{
		DWORD myPid = GetCurrentProcessId();

		TCHAR myPath[MAX_PATH] = { 0 };
		if (!GetModuleFileName(NULL, myPath, _countof(myPath)))
			return 0;
		// Extract executable file name only (e.g. "TrayPwrD3.exe")
		TCHAR *pName = _tcsrchr(myPath, '\\');
		LPCTSTR myExeName = pName ? pName + 1 : myPath;

		HANDLE hsnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
		if (hsnap == INVALID_HANDLE_VALUE)
			return 0;

		PROCESSENTRY32 pe = { 0 };
		pe.dwSize = sizeof(pe);
		if (Process32First(hsnap, &pe))
		{
			do
			{
				// szExeFile holds the base exe name
				if (pe.th32ProcessID != myPid &&
					_tcsicmp(pe.szExeFile, myExeName) == 0)
				{
					CloseHandle(hsnap);
					return pe.th32ProcessID;
				}
			} while (Process32Next(hsnap, &pe));
		}
		CloseHandle(hsnap);
		return 0;
	}

	// Terminates the process with the given PID. Returns TRUE on success.
	BOOL TerminateProcessByPid(DWORD pid, DWORD timeoutMs = 5000)
	{
		HANDLE hProc = OpenProcess(PROCESS_TERMINATE | SYNCHRONIZE, FALSE, pid);
		if (!hProc)
			return FALSE;
		BOOL ok = TerminateProcess(hProc, 0) != FALSE;
		if (ok)
		{
			WaitForSingleObject(hProc, timeoutMs);
		}
		CloseHandle(hProc);
		return ok;
	}
}

CMyApp::CMyApp()
{

}

BOOL CMyApp::InitInstance()
{
	// Check for another running instance of this exe
	DWORD otherPid = FindOtherInstancePid();
	if (otherPid != 0)
	{
		CString msg;
		msg.Format(_T("Another instance of %s is already running (PID %u).\n\n"
			"Do you want to terminate the running instance and continue?"),
			AfxGetAppName(), otherPid);
		int res = AfxMessageBox(msg, MB_YESNO | MB_ICONQUESTION);
		if (res == IDYES)
		{
			if (!TerminateProcessByPid(otherPid))
			{
				AfxMessageBox(_T("Failed to terminate the running instance. Startup will be cancelled."),
					MB_OK | MB_ICONERROR);
				return FALSE;
			}
			// optionally wait a moment to ensure process is gone
			Sleep(200);
		}
		else
		{
			// user chose not to terminate the other instance -> exit
			return FALSE;
		}
	}

	// Create main frame window (don't use doc/view stuff)
	m_bAboutDialogActive=FALSE;
	CMainFrame* pMainFrame = new CMainFrame;
	if (!pMainFrame->LoadFrame(IDR_MAINFRAME))
		return FALSE;
	m_pMainWnd = pMainFrame;
	return TRUE;
}

void CMyApp::OnAppAbout()
{
	CAboutDlg aboutDlg; 
	aboutDlg.DoModal();
}

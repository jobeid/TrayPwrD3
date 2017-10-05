////////////////////////////////////////////////////////////////
// TrayPwr Adopted from Microsoft Systems Journal's CTrayIcon by Paul DiLascia.
//
// All the activity takes place in MainFrm.cpp.

#include "stdafx.h"
#include "TrayPwr.h"
#include "mainfrm.h"
#include "AboutDlg.h"
#include <io.h>

CMyApp theApp;

BEGIN_MESSAGE_MAP(CMyApp, CWinApp)
	//{{AFX_MSG_MAP(CMyApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

CMyApp::CMyApp()
{

}

BOOL CMyApp::InitInstance()
{
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

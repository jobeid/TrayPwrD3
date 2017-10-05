////////////////////////////////////////////////////////////////
// TrayPwr Copyright 1996 Microsoft Systems Journal.
// See TrayPwr.CPP for description of program.
//
#include "resource.h"
#include "D3dx12jo.h"

class CMyApp : public CWinApp {
public:
	CMyApp();

	virtual BOOL InitInstance();
	//{{AFX_MSG(CMyApp)
	afx_msg void OnAppAbout();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
protected:

private:
	BOOL m_bAboutDialogActive;
};

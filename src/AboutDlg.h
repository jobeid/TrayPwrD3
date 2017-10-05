// AboutDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

	void BringToTop();
// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
    //}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	public:
	virtual INT_PTR DoModal();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
protected:
    static BOOL m_bVisible;
	static HWND m_MyhWnd;
	//stuff for calling external exe's:
	HANDLE m_hChildProcess;
	DWORD m_dwChildProcessId;
	void OnInvokeMSInfo(LPSTR SomeExeNm);
	static BOOL CALLBACK CloseChildProcess(HWND hWnd, LPARAM lParam);
	static BOOL CALLBACK ActivateChildProcess(HWND hWnd, LPARAM lParam);
	BOOL StartSysInfo();

	//{{AFX_MSG(CAboutDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnSysinfo();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

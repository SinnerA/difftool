// diffDlg.cpp : implementation file
//

#include "stdafx.h"
#include "diff.h"
#include "FilePartition.h"
#include "diffDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
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
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDiffDlg dialog

CDiffDlg::CDiffDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDiffDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDiffDlg)
	m_szFile1 = _T("");
	m_szFile2 = _T("");
	m_bCheckCase = FALSE;
	m_bCheckIndent = FALSE;
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CDiffDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDiffDlg)
	DDX_Text(pDX, IDC_EDIT1, m_szFile1);
	DDX_Text(pDX, IDC_EDIT2, m_szFile2);
	DDX_Check(pDX, IDC_CHECK_CASESENSITIVE, m_bCheckCase);
	DDX_Check(pDX, IDC_CHECK_INDENT, m_bCheckIndent);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDiffDlg, CDialog)
	//{{AFX_MSG_MAP(CDiffDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1,	OnButton1)
	ON_BN_CLICKED(IDC_BUTTON2,	OnButton2)
	ON_BN_CLICKED(IDC_DIFF,		OnDiff)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDiffDlg message handlers

BOOL CDiffDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CDiffDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CDiffDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CDiffDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CDiffDlg::OnButton1() 
{
	UpdateData(TRUE); // update the DDX variables

	// show open file dialog, and let choose one or more files

	CFileDialog my_dialog(TRUE,NULL,NULL,OFN_ALLOWMULTISELECT|OFN_FILEMUSTEXIST|OFN_EXPLORER/*flags*/,NULL);

	my_dialog.m_ofn.lpstrFile = (TCHAR*) malloc( 2*MAX_PATH*sizeof(TCHAR) );
	memset( my_dialog.m_ofn.lpstrFile, 0, 2*MAX_PATH*sizeof(TCHAR) );

	my_dialog.m_ofn.nMaxFile = 2*MAX_PATH; // number of chars, not number of bytes

	if (my_dialog.DoModal()==IDOK)
	{
		// do we have two files selected ?
		if (!ExtractFiles(my_dialog, m_szFile1, m_szFile2) )
		{
			UpdateData(TRUE); // update the DDX variables
			m_szFile1=my_dialog.GetPathName(); 
			UpdateData(FALSE); // update the dialog box
		}
		else
			UpdateData(FALSE); // update the dialog box
	}

	free( my_dialog.m_ofn.lpstrFile );
	
}

void CDiffDlg::OnButton2() 
{
	UpdateData(TRUE); // update the DDX variables

	// show open file dialog, and let choose one or more files

	CFileDialog my_dialog(TRUE,NULL,NULL,OFN_ALLOWMULTISELECT|OFN_FILEMUSTEXIST|OFN_EXPLORER/*flags*/,NULL);

	my_dialog.m_ofn.lpstrFile = (TCHAR*) malloc( 2*MAX_PATH*sizeof(TCHAR) );
	memset( my_dialog.m_ofn.lpstrFile, 0, 2*MAX_PATH*sizeof(TCHAR) );

	my_dialog.m_ofn.nMaxFile = 2*MAX_PATH; // number of chars, not number of bytes

	if (my_dialog.DoModal()==IDOK)
	{
		// do we have two files selected ?
		if (!ExtractFiles(my_dialog, m_szFile1, m_szFile2) )
		{
			UpdateData(TRUE); // update the DDX variables
			m_szFile2=my_dialog.GetPathName(); 
			UpdateData(FALSE); // update the dialog box
		}
		else
			UpdateData(FALSE); // update the dialog box
	}

	free( my_dialog.m_ofn.lpstrFile );
}


BOOL CDiffDlg::ExtractFiles(CFileDialog &fd, CString &f1, CString &f2)
{
	if ( fd.m_ofn.lpstrFile[0]=='\0' )
		return FALSE; // struct was not used

	// get directory
	CString szDir = fd.m_ofn.lpstrFile;

	// get first file
	CString szFile1 = fd.m_ofn.lpstrFile + fd.m_ofn.nFileOffset;
	szFile1 = szDir + "\\" + szFile1;

	TCHAR *p = fd.m_ofn.lpstrFile + fd.m_ofn.nFileOffset;
	if (*p==0) return FALSE;

	while ( *p )
		p++;

	p++; // do we have a double 0 ?

	if (*p==0) return FALSE; // so, only one file was selected

	CString szFile2 = p;
	szFile2 = szDir + "\\" + szFile2;

	f1 = szFile2;
	f2 = szFile1;

	return TRUE;
}


afx_msg void CDiffDlg::OnDiff()
{
	UpdateData(TRUE); // update the DDX variables

	CFileOptions o;
	if (!m_bCheckCase) o.SetOption( CString("case"), CString("no") );
	if (!m_bCheckIndent) o.SetOption( CString("indent"), CString("no") );


	((CDiffApp*)AfxGetApp())->DoDiff(FALSE, m_szFile1, m_szFile2, CString(""), o);
}

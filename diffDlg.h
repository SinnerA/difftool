// diffDlg.h : header file
//

#if !defined(AFX_DIFFDLG_H__19B750A7_BFE0_11D3_AA3D_B10D6B65B37B__INCLUDED_)
#define AFX_DIFFDLG_H__19B750A7_BFE0_11D3_AA3D_B10D6B65B37B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CDiffDlg dialog

class CDiffDlg : public CDialog
{
// Construction
public:
	CDiffDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CDiffDlg)
	enum { IDD = IDD_DIFF_DIALOG };
	CString	m_szFile1;
	CString	m_szFile2;
	BOOL	m_bCheckCase;
	BOOL	m_bCheckIndent;
	//}}AFX_DATA

	CString GetFile1() { return m_szFile1; }
	CString GetFile2() { return m_szFile2; }
	void	SetCaseOption(BOOL bCase) { m_bCheckCase = bCase; }
	BOOL    GetCaseOption() { return m_bCheckCase; }
	void	SetIndentOption(BOOL bIndent) { m_bCheckIndent = bIndent; }
	BOOL    GetIndentOption() { return m_bCheckIndent; }

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDiffDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	BOOL ExtractFiles(CFileDialog &fd, CString &f1, CString &f2);

	// Generated message map functions
	//{{AFX_MSG(CDiffDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnButton1();
	afx_msg void OnButton2();
	afx_msg void OnDiff();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIFFDLG_H__19B750A7_BFE0_11D3_AA3D_B10D6B65B37B__INCLUDED_)

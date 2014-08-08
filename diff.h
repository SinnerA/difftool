// diff.h : main header file for the DIFF application
//

#if !defined(AFX_DIFF_H__19B750A5_BFE0_11D3_AA3D_B10D6B65B37B__INCLUDED_)
#define AFX_DIFF_H__19B750A5_BFE0_11D3_AA3D_B10D6B65B37B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

class CFileOptions; // forward declaration

/////////////////////////////////////////////////////////////////////////////
// CDiffApp:
// See diff.cpp for the implementation of this class
//

class CDiffApp : public CWinApp
{
public:
	CDiffApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDiffApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation
	BOOL DoDiff(BOOL bSilentMode, CString &szFile1, CString &szFile2, CString &szOutfile, CFileOptions &o);

	//{{AFX_MSG(CDiffApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIFF_H__19B750A5_BFE0_11D3_AA3D_B10D6B65B37B__INCLUDED_)

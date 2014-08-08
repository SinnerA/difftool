// diff.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "diff.h"
#include "diffDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "FilePartition.h"
#include "DiffEngine.h"

// helper
//
// purpose:tells us whether the file exists
BOOL FileExists(CString &szFilename)
{
	BOOL bFound = FALSE;

	WIN32_FIND_DATA findFile;
	HANDLE hContext;
	hContext=::FindFirstFile(szFilename.GetBuffer(0), &findFile);
	if (  hContext != INVALID_HANDLE_VALUE )
	{
		::FindClose(hContext);
		bFound = TRUE;
	}
	return bFound;
}

BOOL IsAFolder(CString &szFilename)
{
	if (!FileExists(szFilename)) return FALSE;

	BOOL bIsADirectory = FALSE;

	WIN32_FIND_DATA findFile;
	HANDLE hContext;
	hContext=::FindFirstFile(szFilename.GetBuffer(0), &findFile);
	if (  hContext != INVALID_HANDLE_VALUE )
	{
		if (findFile.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) 
			bIsADirectory = TRUE;

		::FindClose(hContext);
	}
	return bIsADirectory;
}

/////////////////////////////////////////////////////////////////////////////
// CDiffApp

BEGIN_MESSAGE_MAP(CDiffApp, CWinApp)
	//{{AFX_MSG_MAP(CDiffApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDiffApp construction

CDiffApp::CDiffApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CDiffApp object

CDiffApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CDiffApp initialization

BOOL CDiffApp::InitInstance()
{
	AfxEnableControlContainer();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	if (__argc>1 && __argc<3)
	{
		AfxMessageBox( _T("usage: diff.exe [-c] [-i] <file1> <file2> <outfile>\r\nexample diff.exe bookshop.4.xml bookshop.5.xml diff.html") );
		return FALSE;
	}

	BOOL bSilentMode = FALSE;
	BOOL bCaseOption = TRUE;
	BOOL bIndentOption = TRUE;
	CString szFile1, szFile2, szOutfile;

	// get cmdline parameters
	int iStart = 0;

	for (int i = 1; i < __argc; i++)
	{
		bSilentMode = TRUE;
		LPCTSTR pszParam = __targv[i];
		if (stricmp(pszParam,"-c")==0)
		{
			bCaseOption = FALSE;
			iStart++;
		}
		else if (stricmp(pszParam,"-i")==0)
		{
			bIndentOption = FALSE;
			iStart++;
		}
		else
		{
			switch (i-iStart)
			{
				case 1 : szFile1 = pszParam; break;
				case 2 : szFile2 = pszParam; break;
				case 3 : szOutfile = pszParam; break;
			}
		}
	}

	if (bSilentMode) // no GUI
	{
		// process files

		CFileOptions o;
		if (!bCaseOption) o.SetOption( CString("case"), CString("no") );
		if (!bIndentOption) o.SetOption( CString("indent"), CString("no") );

		DoDiff(bSilentMode, szFile1, szFile2, szOutfile, o);

		return FALSE;
	}

	// GUI mode
	CDiffDlg dlg;

	dlg.SetCaseOption(TRUE);
	dlg.SetIndentOption(TRUE);

	dlg.DoModal();
	return FALSE;
}



BOOL CDiffApp::DoDiff(BOOL bSilentMode, CString &szFile1, CString &szFile2, CString &szOutfile, CFileOptions &o)
{
	// make sure those files exist
	if ( !FileExists(szFile1) )
	{
		TCHAR szError[MAX_PATH];
		sprintf(szError, _T("%s does not exist."), szFile1.GetBuffer(0) );
		AfxMessageBox(szError);
		return FALSE;
	}

	if ( !FileExists(szFile2) )
	{
		TCHAR szError[MAX_PATH];
		sprintf(szError, _T("%s does not exist."), szFile2.GetBuffer(0) );
		AfxMessageBox(szError);
		return FALSE;
	}

	CFilePartition f1;
	CFilePartition f2;	
	
	BOOL bFile1IsAFolder = IsAFolder(szFile1);
	BOOL bFile2IsAFolder = IsAFolder(szFile2);
	if ( bFile1IsAFolder && bFile2IsAFolder )
	{
		// process folders
		f1.PreProcessFolder( szFile1, o );
		f2.PreProcessFolder( szFile2, o );
	}
	else if ( !bFile1IsAFolder && !bFile2IsAFolder)
	{
		// process files
		f1.PreProcess( szFile1, o );
		f2.PreProcess( szFile2, o );
	}
	else
	{
		// error (folder and a file cannot match, anyway)
		TCHAR szError[MAX_PATH];
		sprintf(szError, _T("%s cannot be compared with %s."), szFile1.GetBuffer(0), szFile2.GetBuffer(0) );
		AfxMessageBox(szError);
		return FALSE;
	}

	CFilePartition f1_bis, f2_bis;
	CDiffEngine d;
	d.Diff(f1,f2,f1_bis,f2_bis);

	if ( szOutfile.IsEmpty() ) // default output file for GUI mode
		szOutfile = "out.html";

	d.ExportAsHtml(szOutfile, d.Serialize(f1_bis, f2_bis));

	// open with the default browser
	if (!bSilentMode)
		::ShellExecute(NULL, "open", szOutfile.GetBuffer(0), NULL, NULL, SW_SHOW);

	return TRUE;

}
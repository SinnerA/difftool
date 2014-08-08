// diffstdout.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Templates.h"
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

int main(int argc, char* argv[])
{

	CString szFile1, szFile2, szOutfile;

	if (argc<3)
	{
		printf( _T("usage: diffstdout.exe [-c] [-i] <file1> <file2> [<outfilename>]\r\nexample diffstdout.exe bookshop.4.xml bookshop.5.xml diff.html") );
		return 0;
	}

	BOOL bCaseOption = TRUE;
	BOOL bIndentOption = TRUE;

	// get cmdline parameters
	int iStart = 0;

	for (int i = 1; i < argc; i++)
	{
		LPTSTR pszParam = argv[i];
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

	// make sure those files exist
	if ( !FileExists(szFile1) )
	{
		printf(_T("%s does not exist."), szFile1.GetBuffer(0) );
		return FALSE;
	}

	if ( !FileExists(szFile2) )
	{
		printf(_T("%s does not exist."), szFile2.GetBuffer(0) );
		return FALSE;
	}

	// process files

	CFileOptions o;
	if (!bCaseOption) o.SetOption( CString("case"), CString("no") );
	if (!bIndentOption) o.SetOption( CString("indent"), CString("no") );

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
		printf(_T("%s cannot be compared with %s."), szFile1.GetBuffer(0), szFile2.GetBuffer(0) );
		return 0;
	}

	CFilePartition f1_bis, f2_bis;
	CDiffEngine d;
	d.Diff(f1,f2,f1_bis,f2_bis);

	if ( szOutfile.IsEmpty() ) // default output
		d.ExportAsStdout( d.Serialize(f1_bis, f2_bis) );
	else
		d.ExportAsHtml(szOutfile, d.Serialize(f1_bis, f2_bis) );

	return 0;
}


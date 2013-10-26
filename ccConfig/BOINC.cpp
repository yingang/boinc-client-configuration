#include "StdAfx.h"
#include "BOINC.h"

#include "Util.h"

void error()
{
    LPVOID lpMsgBuf;
    FormatMessage( 
        FORMAT_MESSAGE_ALLOCATE_BUFFER | 
        FORMAT_MESSAGE_FROM_SYSTEM | 
        FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL,
        GetLastError(),
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
        (LPTSTR) &lpMsgBuf,
        0,
        NULL 
        );

    // Display the string.
    MessageBox( NULL, (LPCTSTR)lpMsgBuf, _T("Error"), MB_OK | MB_ICONINFORMATION );
    // Free the buffer.
    LocalFree( lpMsgBuf );
}

BOOL BOINC::getSetupReg(const CString& strKey, CString& strValue)
{
	HKEY hKey;
	LONG lResult = ::RegOpenKeyEx(HKEY_LOCAL_MACHINE,
		_T("SOFTWARE\\Space Sciences Laboratory, U.C. Berkeley\\BOINC Setup"),
		NULL,
		KEY_READ | KEY_WOW64_64KEY,
		&hKey);

    if (lResult == 2)
    {
        lResult = ::RegOpenKeyEx(HKEY_LOCAL_MACHINE,
            _T("SOFTWARE\\Space Sciences Laboratory, U.C. Berkeley\\BOINC Setup"),
            NULL,
            KEY_READ,
            &hKey);
    }

	if (lResult != ERROR_SUCCESS)
		return FALSE;

	DWORD dwBufSize = 8192;
	TCHAR* pData = new TCHAR[dwBufSize];

	DWORD dwType = REG_SZ;
	DWORD cbData = dwBufSize;
	DWORD dwRet = ::RegQueryValueEx(hKey, strKey, NULL, &dwType, (LPBYTE)pData, &cbData);

	if (dwRet != ERROR_SUCCESS/* || dwRet == ERROR_MORE_DATA*/)
		return FALSE;

	strValue = pData;
	return TRUE;
}

CString BOINC::getDataDir(void)
{
	CString strDir;
	if (getSetupReg(_T("DATADIR"), strDir))
		return strDir;
	else
		return getInstallDir();
}

CString BOINC::getInstallDir(void)
{
	CString strDir;
	if (getSetupReg(_T("INSTALLDIR"), strDir))
		return strDir;
	else
		return _T("");
}

BOOL BOINC::getAuthPwd(const CString& strDataDir, CString& strPwd)
{
	CString strAuthFile = strDataDir + _T("gui_rpc_auth.cfg");
	if (!UTIL::doesFileExist(strAuthFile))
		return FALSE;

	CStdioFile f;
	if (!f.Open(strAuthFile, CFile::modeRead | CFile::typeText))
		return FALSE;

	BOOL bRet = f.ReadString(strPwd);
	f.Close();
	return bRet;
}

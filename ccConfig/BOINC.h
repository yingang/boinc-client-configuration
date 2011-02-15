#pragma once

namespace BOINC
{
	BOOL getSetupReg(const CString& strKey, CString& strValue);

	CString getDataDir(void);

	CString getInstallDir(void);

	BOOL getAuthPwd(const CString& strDataDir, CString& strPwd);
};

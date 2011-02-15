#include "StdAfx.h"
#include "Util.h"

std::wstring UTIL::convertToWideChar(const std::string& str)
{
	int nLen = ::MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, NULL, 0); 

	wchar_t* szWideText = new wchar_t[nLen + 1]; 

	memset(szWideText, 0, (nLen + 1) * sizeof(wchar_t)); 

	::MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, (LPWSTR)szWideText, nLen); 

	std::wstring rt = (wchar_t*)szWideText;

	delete[] szWideText;

	return rt; 
}

std::string UTIL::convertToMultiChar(const std::wstring& str)
{
	int nLen = ::WideCharToMultiByte(CP_ACP, 0, str.c_str(), -1, NULL, 0, NULL, NULL);

	char* szText = new char[nLen + 1];

	memset((void*)szText, 0, sizeof(char) * (nLen + 1));

	::WideCharToMultiByte(CP_ACP, 0, str.c_str(), -1, szText, nLen, NULL, NULL);

	std::string strText = szText;

	delete[] szText;

	return strText;
}

void UTIL::splitString(const CString& strSrc, const CString& strElem, std::vector<CString>& vStrings)
{
	int nPos = 0;
	CString sToken = strSrc.Tokenize(strElem, nPos);
	while (sToken.GetLength() > 0)
	{
		vStrings.push_back(sToken);
		sToken = strSrc.Tokenize(strElem, nPos);
	}
}

void UTIL::addTiXMLChild(TiXmlElement* pParent, const std::string& sTagName, const std::string& sTagValue)
{
	TiXmlElement* pItem = new TiXmlElement(sTagName.c_str());
	pItem->LinkEndChild(new TiXmlText(sTagValue.c_str()));
	pParent->LinkEndChild(pItem);
}

bool UTIL::tryStrToInt(const CString& s, int& n)
{
	n = _tstoi(LPCTSTR(s));

	if (0 == n)
	{
		int idx = 0, nLen = s.GetLength();
		for ( ; idx < nLen; idx++)
			if ('0' != s[idx])
				break;

		if (idx == nLen
			&& nLen != 0)
			return true;
		else
			return false;
	}
	else
		return true;
}

bool UTIL::tryStrToInt(const std::string& s, int& n)
{
	n = strtol(s.c_str(), (char **)NULL, 10);

	if (0 == n)
	{
		int idx = 0, nLen = s.length();
		for ( ; idx < nLen; idx++)
			if ('0' != s[idx])
				break;

		if (idx == nLen)
			return true;
		else
			return false;
	}
	else
		return true;
}

std::string UTIL::formatNum(const std::string& sFmt, int n)
{
	char szBuf[32];
	_snprintf(szBuf, sizeof(szBuf), sFmt.c_str(), n);
	return szBuf;
}

void UTIL::openURL(const CString& strURL)
{
	::ShellExecute(AfxGetMainWnd()->m_hWnd, _T("open"), strURL, _T(""), _T(""), SW_SHOW);
}

BOOL UTIL::doesFileExist(const CString& strFilePath)
{
	CFileFind ff;
	return ff.FindFile(strFilePath);
}
#pragma once

#include <string>
#include <vector>
#include "tinyxml.h"

namespace UTIL
{
	std::wstring convertToWideChar(const std::string& str);

	std::string convertToMultiChar(const std::wstring& str);

	CString getDataDir(void);

	void splitString(const CString& strSrc, const CString& strElem, std::vector<CString>& vStrings);

	void addTiXMLChild(TiXmlElement* pParent, const std::string& sTagName, const std::string& sTagValue);

	bool tryStrToInt(const CString& s, int& n);

	bool tryStrToInt(const std::string& s, int& n);

	std::string formatNum(const std::string& sFmt, int n);

	void openURL(const CString& strURL);
};

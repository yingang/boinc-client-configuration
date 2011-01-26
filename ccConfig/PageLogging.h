#pragma once

#include "tinyxml.h"

#include <map>
#include <string>

// CPageLogging dialog

class CPageLogging : public CDialog
{
	DECLARE_DYNAMIC(CPageLogging)

public:
	CPageLogging(CWnd* pParent = NULL);   // standard constructor
	virtual ~CPageLogging();

// Dialog Data
	enum { IDD = IDD_PAGE_LOGGING };

private:
	void initLoggingItem(void);
	BOOL isDefaultSet(int nID);

	std::map<std::string, int> m_mapLogging;

public:
	void saveToXML(TiXmlElement* pParent);
	void loadFromXML(TiXmlElement* pParent);
	void restore(void);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	virtual BOOL OnInitDialog();
};

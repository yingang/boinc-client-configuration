#pragma once

#include "tinyxml.h"

#include <map>
#include <string>

// CPageOption dialog

class CPageOption : public CDialog
{
	DECLARE_DYNAMIC(CPageOption)

public:
	CPageOption(CWnd* pParent = NULL);   // standard constructor
	virtual ~CPageOption();

// Dialog Data
	enum { IDD = IDD_PAGE_OPTION };

private:
	BOOL isDefaultSet(int nID);
	void initOption(void);

	std::map<std::string, int> m_mapOption;

public:
	void saveToXML(TiXmlElement* pParent);
	void loadFromXML(TiXmlElement* pParent);
	void restore(void);
	void enable(BOOL bEnabled);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	virtual BOOL OnInitDialog();
};

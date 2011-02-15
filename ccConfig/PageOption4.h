#pragma once

#include "tinyxml.h"

#include <string>
#include <map>

// CPageOption4 dialog

class CPageOption4 : public CDialog
{
	DECLARE_DYNAMIC(CPageOption4)

public:
	CPageOption4(CWnd* pParent = NULL);   // standard constructor
	virtual ~CPageOption4();

// Dialog Data
	enum { IDD = IDD_PAGE_OPTION4 };

private:
	void initOption(void);
	std::map<std::string, std::pair<int, int> > m_mapText;

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

#pragma once
#include "afxwin.h"

#include "tinyxml.h"

#include <string>
#include <map>

// CPageOption2 dialog

class CPageOption2 : public CDialog
{
	DECLARE_DYNAMIC(CPageOption2)

public:
	CPageOption2(CWnd* pParent = NULL);   // standard constructor
	virtual ~CPageOption2();

// Dialog Data
	enum { IDD = IDD_PAGE_OPTION2 };

private:
	void initOption(void);

	std::map<std::string, std::pair<int, int> > m_mapText;
	CComboBox m_cmbForceAuth;

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

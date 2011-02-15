// PageOption4.cpp : implementation file
//

#include "stdafx.h"
#include "ccConfig.h"
#include "PageOption4.h"

#include "Util.h"

enum PROXY_FIELD
{
	PF_PORT,
	PF_NORMAL,
};

// CPageOption4 dialog

IMPLEMENT_DYNAMIC(CPageOption4, CDialog)

CPageOption4::CPageOption4(CWnd* pParent /*=NULL*/)
	: CDialog(CPageOption4::IDD, pParent)
{

}

CPageOption4::~CPageOption4()
{
}

void CPageOption4::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CPageOption4, CDialog)
	ON_BN_CLICKED(IDOK, &CPageOption4::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CPageOption4::OnBnClickedCancel)
END_MESSAGE_MAP()

// CPageOption4 message handlers

void CPageOption4::OnBnClickedOk()
{
//	OnOK();
}

void CPageOption4::OnBnClickedCancel()
{
//	OnCancel();
}

BOOL CPageOption4::OnInitDialog()
{
	CDialog::OnInitDialog();

	initOption();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CPageOption4::saveToXML(TiXmlElement* pParent)
{
	for (std::map<std::string, std::pair<int, int> >::iterator it = m_mapText.begin();
		it != m_mapText.end(); ++it)
	{
		CString strText;
		GetDlgItem(it->second.first)->GetWindowText(strText);
		if (strText.GetLength() == 0)
			continue;

		UTIL::addTiXMLChild(pParent, it->first, UTIL::convertToMultiChar(LPCTSTR(strText)));
	}
}

void CPageOption4::loadFromXML(TiXmlElement* pParent)
{
	restore();

	TiXmlElement* pItem = pParent->FirstChildElement();
	for (pItem; pItem; pItem = pItem->NextSiblingElement())
	{
		std::map<std::string, std::pair<int, int> >::iterator it = m_mapText.find(pItem->Value());
		if (it == m_mapText.end())
			continue;

		GetDlgItem(it->second.first)->SetWindowText(UTIL::convertToWideChar(pItem->GetText()).c_str());
	}
}

void CPageOption4::restore(void)
{
	for (std::map<std::string, std::pair<int, int> >::iterator it = m_mapText.begin();
		it != m_mapText.end(); ++it)
	{
		GetDlgItem(it->second.first)->SetWindowText(_T(""));
	}
}

void CPageOption4::initOption(void)
{
	m_mapText["http_server_name"] = std::make_pair(IDC_EDIT_HTTP_SERVER_NAME, PF_NORMAL);
	m_mapText["http_server_port"] = std::make_pair(IDC_EDIT_HTTP_SERVER_PORT, PF_PORT);
	m_mapText["http_user_name"] = std::make_pair(IDC_EDIT_HTTP_USER_NAME, PF_NORMAL);
	m_mapText["http_user_passwd"] = std::make_pair(IDC_EDIT_HTTP_USER_PASSWD, PF_NORMAL);
	m_mapText["socks_version"] = std::make_pair(IDC_EDIT_SOCKS_VERSION, PF_NORMAL);
	m_mapText["socks_server_name"] = std::make_pair(IDC_EDIT_SOCKS_SERVER_NAME, PF_NORMAL);
	m_mapText["socks_server_port"] = std::make_pair(IDC_EDIT_SOCKS_SERVER_PORT, PF_PORT);
	m_mapText["socks5_user_name"] = std::make_pair(IDC_EDIT_SOCK5_USER_NAME, PF_NORMAL);
	m_mapText["socks5_user_passwd"] = std::make_pair(IDC_EDIT_SOCKS5_USER_PASSWD, PF_NORMAL);
	m_mapText["no_proxy"] = std::make_pair(IDC_EDIT_NO_PROXY, PF_NORMAL);
}

void CPageOption4::enable(BOOL bEnabled)
{
	for (std::map<std::string, std::pair<int, int> >::iterator it = m_mapText.begin();
		it != m_mapText.end(); ++it)
	{
		GetDlgItem(it->second.first)->EnableWindow(bEnabled);
	}
}

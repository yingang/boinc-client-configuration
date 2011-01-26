// PageOption2.cpp : implementation file
//

#include "stdafx.h"
#include "ccConfig.h"
#include "PageOption2.h"

#include "Util.h"

enum TEXT_TYPE
{
	TT_NORMAL,
	TT_EXES,
	TT_URL,
};

// CPageOption2 dialog

IMPLEMENT_DYNAMIC(CPageOption2, CDialog)

CPageOption2::CPageOption2(CWnd* pParent /*=NULL*/)
	: CDialog(CPageOption2::IDD, pParent)
{
}

CPageOption2::~CPageOption2()
{
}

void CPageOption2::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CMB_FORCE_AUTH, m_cmbForceAuth);
}


BEGIN_MESSAGE_MAP(CPageOption2, CDialog)
	ON_BN_CLICKED(IDOK, &CPageOption2::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CPageOption2::OnBnClickedCancel)
END_MESSAGE_MAP()


// CPageOption2 message handlers

void CPageOption2::OnBnClickedOk()
{
//	OnOK();
}

void CPageOption2::OnBnClickedCancel()
{
//	OnCancel();
}

void CPageOption2::saveToXML(TiXmlElement* pParent)
{
	for (std::map<std::string, std::pair<int, int> >::iterator it = m_mapText.begin();
		it != m_mapText.end(); ++it)
	{
		CString strFullText;
		GetDlgItem(it->second.first)->GetWindowText(strFullText);
		if (strFullText.GetLength() == 0)
			continue;

		std::vector<CString> vText;
		UTIL::splitString(strFullText, _T(",£¬"), vText);
		for (std::vector<CString>::iterator itText = vText.begin();
			itText != vText.end(); ++itText)
		{
			CString strText = *itText;

			if ((it->second.second == TT_EXES && strText.Right(4).MakeLower() != _T(".exe"))
				|| (it->second.second == TT_URL && strText.Left(7).MakeLower() != _T("http://")))
			{
				MessageBox(CString(_T("Invalid <"))
						+ UTIL::convertToWideChar(it->first).c_str()
						+ "> value: "
						+ strText,
					_T("Save Option2"), MB_OK|MB_ICONERROR);
				continue;
			}

			UTIL::addTiXMLChild(pParent, it->first, UTIL::convertToMultiChar(LPCTSTR(strText)));
		}
	}

	if (m_cmbForceAuth.GetCurSel() > 0)
	{
		CString strAuthType;
		m_cmbForceAuth.GetLBText(m_cmbForceAuth.GetCurSel(), strAuthType);
		UTIL::addTiXMLChild(pParent, "force_auth", UTIL::convertToMultiChar(LPCTSTR(strAuthType)));
	}
}

void CPageOption2::loadFromXML(TiXmlElement* pParent)
{
	restore();

	TiXmlElement* pItem = pParent->FirstChildElement();
	for (pItem; pItem; pItem = pItem->NextSiblingElement())
	{
		std::map<std::string, std::pair<int, int> >::iterator it = m_mapText.find(pItem->Value());
		if (it == m_mapText.end())
			continue;

		CWnd* pEdit = GetDlgItem(it->second.first);
		CString strText = UTIL::convertToWideChar(pItem->GetText()).c_str();

		CString strOrigText;
		pEdit->GetWindowText(strOrigText);

		if (strOrigText.GetLength() == 0)
			pEdit->SetWindowText(strText);
		else if (it->second.second == TT_EXES)
			pEdit->SetWindowText(strOrigText + _T(",") + strText);
	}

	pItem = pParent->FirstChildElement();
	for (pItem; pItem; pItem = pItem->NextSiblingElement())
	{
		if (pItem->Value() == std::string("force_auth"))
		{
			CString strAuthType = UTIL::convertToWideChar(pItem->GetText()).c_str();
			m_cmbForceAuth.SelectString(-1, strAuthType.MakeLower());
			break;
		}
	}
}

void CPageOption2::restore(void)
{
	for (std::map<std::string, std::pair<int, int> >::iterator it = m_mapText.begin();
		it != m_mapText.end(); ++it)
	{
		GetDlgItem(it->second.first)->SetWindowText(_T(""));
	}

	m_cmbForceAuth.SetCurSel(0);
}

BOOL CPageOption2::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_cmbForceAuth.InsertString(-1, _T("none"));
	m_cmbForceAuth.InsertString(-1, _T("basic"));
	m_cmbForceAuth.InsertString(-1, _T("digest"));
	m_cmbForceAuth.InsertString(-1, _T("gss-negotiate"));
	m_cmbForceAuth.InsertString(-1, _T("ntlm"));
	m_cmbForceAuth.SetCurSel(0);

	initOption();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CPageOption2::initOption(void)
{
	m_mapText["alt_platform"] = std::make_pair(IDC_EDIT_ALT_PLATFORM, TT_NORMAL);
	m_mapText["exclusive_app"] = std::make_pair(IDC_EDIT_EXCLUSIVE_APP, TT_EXES);
	m_mapText["exclusive_gpu_app"] = std::make_pair(IDC_EDIT_EXCLUSIVE_GPU_APP, TT_EXES);
	m_mapText["client_download_url"] = std::make_pair(IDC_EDIT_CLIENT_DOWNLOAD_URL, TT_URL);
	m_mapText["client_version_check_url"] = std::make_pair(IDC_EDIT_CLIENT_VERSION_CHECK_URL, TT_URL);
	m_mapText["network_test_url"] = std::make_pair(IDC_EDIT_NETWORK_TEST_URL, TT_URL);
}

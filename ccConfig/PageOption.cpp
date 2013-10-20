// PageOption.cpp : implementation file
//

#include "stdafx.h"
#include "ccConfig.h"
#include "PageOption.h"

#include "Util.h"

// CPageOption dialog

IMPLEMENT_DYNAMIC(CPageOption, CDialog)

CPageOption::CPageOption(CWnd* pParent /*=NULL*/)
	: CDialog(CPageOption::IDD, pParent)
{

}

CPageOption::~CPageOption()
{
}

void CPageOption::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CPageOption, CDialog)
	ON_BN_CLICKED(IDOK, &CPageOption::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CPageOption::OnBnClickedCancel)
END_MESSAGE_MAP()

// CPageOption message handlers

void CPageOption::OnBnClickedOk()
{
//	OnOK();
}

void CPageOption::OnBnClickedCancel()
{
//	OnCancel();
}

BOOL CPageOption::OnInitDialog()
{
	CDialog::OnInitDialog();

	initOption();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CPageOption::saveToXML(TiXmlElement* pParent)
{
	for (std::map<std::string, int>::iterator it = m_mapOption.begin();
		it != m_mapOption.end(); ++it)
	{
		BOOL bDefault = isDefaultSet(it->second);
		BOOL bChecked = ((CButton *)GetDlgItem(it->second))->GetCheck();

		if ((bDefault && bChecked)
			|| (!bDefault && !bChecked))
			continue;

		UTIL::addTiXMLChild(pParent, it->first, bChecked ? "1" : "0");
	}
}

void CPageOption::loadFromXML(TiXmlElement* pParent)
{
	restore();

	TiXmlElement* pItem = pParent->FirstChildElement();
	for (pItem; pItem; pItem = pItem->NextSiblingElement())
	{
		std::map<std::string, int>::iterator it = m_mapOption.find(pItem->Value());
		if (it == m_mapOption.end())
			continue;

		CButton* pBtn = (CButton *)GetDlgItem(it->second);
		if (pItem->GetText() == std::string("1"))
			pBtn->SetCheck(TRUE);
		else
			pBtn->SetCheck(FALSE);
	}
}

void CPageOption::restore(void)
{
	for (std::map<std::string, int>::iterator it = m_mapOption.begin();
		it != m_mapOption.end(); ++it)
	{
		CButton *pBtn = (CButton *)GetDlgItem(it->second);
		if (isDefaultSet(it->second))
			pBtn->SetCheck(TRUE);
		else
			pBtn->SetCheck(FALSE);
	}
}

void CPageOption::initOption(void)
{
	m_mapOption["abort_jobs_on_exit"] = IDC_CHECK_ABORT_JOBS_ON_EXIT;
	m_mapOption["allow_multiple_clients"] = IDC_CHECK_ALLOW_MULTIPLE_CLIENTS;
	m_mapOption["allow_remote_gui_rpc"] = IDC_CHECK_ALLOW_REMOTE_GUI_RPC;
	m_mapOption["disallow_attach"] = IDC_CHECK_DISABLE_ATTACH;
	m_mapOption["dont_check_file_sizes"] = IDC_CHECK_DONT_CHECK_FILE_SIZES;
	m_mapOption["dont_contact_ref_site"] = IDC_CHECK_DONT_CONTACT_REF_SITE;
    m_mapOption["exit_before_start"] = IDC_CHECK_EXIT_BEFORE_START;
	m_mapOption["exit_when_idle"] = IDC_CHECK_EXIT_WHEN_IDLE;
	m_mapOption["fetch_minimal_work"] = IDC_CHECK_FETCH_MINIMAL_WORK;
    m_mapOption["fetch_on_update"] = IDC_CHECK_FETCH_ON_UPDATE;
	m_mapOption["http_1_0"] = IDC_CHECK_HTTP_1_0;
	m_mapOption["no_alt_platform"] = IDC_CHECK_NO_ALT_PLATFORM;
	m_mapOption["no_gpus"] = IDC_CHECK_NO_GPUS;
	m_mapOption["no_priority_change"] = IDC_CHECK_NO_PRIORITY_CHANGE;

	m_mapOption["os_random_only"] = IDC_CHECK_OS_RANDOM_ONLY;
	m_mapOption["report_results_immediately"] = IDC_CHECK_REPORT_RESULTS_IMMEDIATELY;
	m_mapOption["run_apps_manually"] = IDC_CHECK_RUN_APPS_MANUALLY;
	m_mapOption["simple_gui_only"] = IDC_CHECK_SIMPLE_GUI_ONLY;
	m_mapOption["suppress_net_info"] = IDC_CHECK_SUPRESS_NET_INFO;
	m_mapOption["use_all_gpus"] = IDC_CHECK_USE_ALL_GPUS;
	m_mapOption["use_certs"] = IDC_CHECK_USE_CERTS;
	m_mapOption["use_certs_only"] = IDC_CHECK_USE_CERTS_ONLY;
}

BOOL CPageOption::isDefaultSet(int nID)
{
	return FALSE;
}

void CPageOption::enable(BOOL bEnabled)
{
	for (std::map<std::string, int>::iterator it = m_mapOption.begin();
		it != m_mapOption.end(); ++it)
	{
		GetDlgItem(it->second)->EnableWindow(bEnabled);
	}
}
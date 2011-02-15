// PageLogging.cpp : implementation file
//

#include "stdafx.h"
#include "ccConfig.h"
#include "PageLogging.h"

#include "Util.h"

// CPageLogging dialog

IMPLEMENT_DYNAMIC(CPageLogging, CDialog)

CPageLogging::CPageLogging(CWnd* pParent /*=NULL*/)
	: CDialog(CPageLogging::IDD, pParent)
{

}

CPageLogging::~CPageLogging()
{
}

void CPageLogging::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CPageLogging, CDialog)
	ON_BN_CLICKED(IDOK, &CPageLogging::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CPageLogging::OnBnClickedCancel)
END_MESSAGE_MAP()


// CPageLogging message handlers

void CPageLogging::OnBnClickedOk()
{
//	OnOK();
}

void CPageLogging::OnBnClickedCancel()
{
//	OnCancel();
}

BOOL CPageLogging::OnInitDialog()
{
	CDialog::OnInitDialog();

	initLoggingItem();

	((CButton *)GetDlgItem(IDC_CHECK_TASK))->SetCheck(TRUE);
	((CButton *)GetDlgItem(IDC_CHECK_FILE_XFER))->SetCheck(TRUE);
	((CButton *)GetDlgItem(IDC_CHECK_SCHED_OPS))->SetCheck(TRUE);

	m_tooltip.Create(this, TTS_NOPREFIX | TTS_BALLOON);
	m_tooltip.AddTool(GetDlgItem(IDC_CHECK_TASK), _T("The start and completion of compute jobs (should get two messages per job). "));
	m_tooltip.AddTool(GetDlgItem(IDC_CHECK_FILE_XFER), _T("The start and completion of file transfers. "));
	m_tooltip.AddTool(GetDlgItem(IDC_CHECK_SCHED_OPS), _T("Connections with scheduling servers. "));

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CPageLogging::saveToXML(TiXmlElement* pParent)
{
	for (std::map<std::string, int>::iterator it = m_mapLogging.begin();
		it != m_mapLogging.end(); ++it)
	{
		BOOL bDefault = isDefaultSet(it->second);
		BOOL bChecked = ((CButton *)GetDlgItem(it->second))->GetCheck();
		
		if ((bDefault && bChecked)
			|| (!bDefault && !bChecked))
			continue;

		UTIL::addTiXMLChild(pParent, it->first, bChecked ? "1" : "0");
	}
}

void CPageLogging::loadFromXML(TiXmlElement* pParent)
{
	restore();

	TiXmlElement* pItem = pParent->FirstChildElement();
	for (pItem; pItem; pItem = pItem->NextSiblingElement())
	{
		std::map<std::string, int>::iterator it = m_mapLogging.find(pItem->Value());
		if (it != m_mapLogging.end())
		{
			CButton* pBtn = (CButton *)GetDlgItem(it->second);
			if (pItem->GetText() == std::string("1"))
				pBtn->SetCheck(TRUE);
			else
				pBtn->SetCheck(FALSE);
		}
	}
}

void CPageLogging::restore(void)
{
	for (std::map<std::string, int>::iterator it = m_mapLogging.begin();
		it != m_mapLogging.end(); ++it)
	{
		CButton *pBtn = (CButton *)GetDlgItem(it->second);
		if (isDefaultSet(it->second))
			pBtn->SetCheck(TRUE);
		else
			pBtn->SetCheck(FALSE);
	}
}

BOOL CPageLogging::isDefaultSet(int nID)
{
	return nID == IDC_CHECK_TASK
		|| nID == IDC_CHECK_FILE_XFER
		|| nID == IDC_CHECK_SCHED_OPS;
}

void CPageLogging::initLoggingItem(void)
{
	m_mapLogging["task"] = IDC_CHECK_TASK;
	m_mapLogging["file_xfer"] = IDC_CHECK_FILE_XFER;
	m_mapLogging["sched_ops"] = IDC_CHECK_SCHED_OPS;
	m_mapLogging["app_msg_receive"] = IDC_CHECK_APP_MSG_RECEIVE;
	m_mapLogging["app_msg_send"] = IDC_CHECK_APP_MSG_SEND;
	m_mapLogging["benchmark_debug"] = IDC_CHECK_BENCHMARK_DEBUG;
	m_mapLogging["checkpoint_debug"] = IDC_CHECK_CHECKPOINT_DEBUG;
	m_mapLogging["coproc_debug"] = IDC_CHECK_COPROC_DEBUG;
	m_mapLogging["cpu_sched"] = IDC_CHECK_CPU_SCHED;
	m_mapLogging["cpu_sched_debug"] = IDC_CHECK_CPU_SCHED_DEBUG;
	m_mapLogging["dcf_debug"] = IDC_CHECK_DCF_DEBUG;
	m_mapLogging["debt_debug"] = IDC_CHECK_DEBT_DEBUG;
	m_mapLogging["file_xfer_debug"] = IDC_CHECK_FILE_XFER_DEBUG;
	m_mapLogging["gui_rpc_debug"] = IDC_CHECK_GUI_RPC_DEBUG;

	m_mapLogging["http_debug"] = IDC_CHECK_HTTP_DEBUG;
	m_mapLogging["http_xfer_debug"] = IDC_CHECK_HTTP_XFER_DEBUG;
	m_mapLogging["mem_usage_debug"] = IDC_CHECK_MEM_USAGE_DEBUG;
	m_mapLogging["network_status_debug"] = IDC_CHECK_NETWORK_STATUS_DEBUG;
	m_mapLogging["poll_debug"] = IDC_CHECK_POLL_DEBUG;
	m_mapLogging["proxy_debug"] = IDC_CHECK_PROXY_DEBUG;
	m_mapLogging["rr_simulation"] = IDC_CHECK_RR_SIMULATION;
	m_mapLogging["sched_op_debug"] = IDC_CHECK_SCHED_OP_DEBUG;
	m_mapLogging["scrsave_debug"] = IDC_CHECK_SCRSAVE_DEBUG;
	m_mapLogging["state_debug"] = IDC_CHECK_STATE_DEBUG;
	m_mapLogging["statefile_debug"] = IDC_CHECK_STATEFILE_DEBUG;
	m_mapLogging["task_debug"] = IDC_CHECK_TASK_DEBUG;
	m_mapLogging["time_debug"] = IDC_CHECK_TIME_DEBUG;
	m_mapLogging["unparsed_xml"] = IDC_CHECK_UNPARSED_XML;

	m_mapLogging["work_fetch_debug"] = IDC_CHECK_WORK_FETCH_DEBUG;
}

void CPageLogging::enable(BOOL bEnabled)
{
	for (std::map<std::string, int>::iterator it = m_mapLogging.begin();
		it != m_mapLogging.end(); ++it)
	{
		GetDlgItem(it->second)->EnableWindow(bEnabled);
	}
}
BOOL CPageLogging::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_MOUSEMOVE)
		m_tooltip.RelayEvent(pMsg);

	return CDialog::PreTranslateMessage(pMsg);
}

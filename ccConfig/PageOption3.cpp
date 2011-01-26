// PageOption3.cpp : implementation file
//

#include "stdafx.h"
#include "ccConfig.h"
#include "PageOption3.h"

#include "Util.h"

enum NUMBER_TYPE
{
	NT_POSITIVE,	// 正数
	NT_ZEROABLE,	// 非负数
};

// CPageOption3 dialog

IMPLEMENT_DYNAMIC(CPageOption3, CDialog)

CPageOption3::CPageOption3(CWnd* pParent /*=NULL*/)
	: CDialog(CPageOption3::IDD, pParent)
{
}

CPageOption3::~CPageOption3()
{
}

void CPageOption3::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CPageOption3, CDialog)
	ON_BN_CLICKED(IDOK, &CPageOption3::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CPageOption3::OnBnClickedCancel)
END_MESSAGE_MAP()

// CPageOption3 message handlers

void CPageOption3::OnBnClickedOk()
{
//	OnOK();
}

void CPageOption3::OnBnClickedCancel()
{
//	OnCancel();
}

BOOL CPageOption3::OnInitDialog()
{
	CDialog::OnInitDialog();

	initOption();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CPageOption3::saveToXML(TiXmlElement* pParent)
{
	for (std::map<std::string, std::pair<int, int> >::iterator it = m_mapOption.begin();
		it != m_mapOption.end(); ++it)
	{
		CString strNumbers;
		GetDlgItem(it->second.first)->GetWindowText(strNumbers);
		if (strNumbers.GetLength() == 0)
			continue;

		std::vector<CString> vNumber;
		UTIL::splitString(strNumbers, _T(",，"), vNumber);

		for (std::vector<CString>::iterator itNum = vNumber.begin();
			itNum != vNumber.end(); ++itNum)
		{
			CString strNumber = *itNum;

			int nNumber = -1;
			if (!UTIL::tryStrToInt(strNumber, nNumber)
				|| nNumber < 0
				|| (nNumber == 0 && it->second.second == NT_POSITIVE))
			{
				MessageBox(CString(_T("Invalid <"))
						+ UTIL::convertToWideChar(it->first).c_str()
						+ _T("> value: ")
						+ strNumber,
					_T("Save Option3"), MB_OK|MB_ICONERROR);
				continue;
			}

			UTIL::addTiXMLChild(pParent, it->first, UTIL::formatNum("%d", nNumber));
		}
	}
}

void CPageOption3::loadFromXML(TiXmlElement* pParent)
{
	restore();

	TiXmlElement* pItem = pParent->FirstChildElement();
	for (pItem; pItem; pItem = pItem->NextSiblingElement())
	{
		std::map<std::string, std::pair<int, int> >::iterator it = m_mapOption.find(pItem->Value());
		if (it == m_mapOption.end())
			continue;

		int nNumber = -1;
		std::string sNumber = pItem->GetText();
		if (!UTIL::tryStrToInt(sNumber, nNumber)
			|| nNumber < 0
			|| (nNumber == 0 && it->second.second == NT_POSITIVE))
			continue;

		CString strNumber = UTIL::convertToWideChar(pItem->GetText()).c_str();

		CString strOrigText;
		CWnd* pEdit = GetDlgItem(it->second.first);
		pEdit->GetWindowText(strOrigText);
		if (strOrigText.GetLength() > 0)
			pEdit->SetWindowText(strOrigText + _T(",") + strNumber);
		else
			pEdit->SetWindowText(strNumber);
	}
}

void CPageOption3::restore(void)
{
	for (std::map<std::string, std::pair<int, int> >::iterator it = m_mapOption.begin();
		it != m_mapOption.end(); ++it)
	{
		GetDlgItem(it->second.first)->SetWindowText(_T(""));
	}
}

void CPageOption3::initOption(void)
{
	m_mapOption["ignore_ati_dev"] = std::make_pair(IDC_EDIT_IGNORE_ATI_DEV, NT_ZEROABLE);
	m_mapOption["ignore_cuda_dev"] = std::make_pair(IDC_EDIT_IGNORE_CUDA_DEV, NT_ZEROABLE);
	m_mapOption["max_file_xfers"] = std::make_pair(IDC_EDIT_MAX_FILE_XFERS, NT_POSITIVE);
	m_mapOption["max_file_xfers_per_project"] = std::make_pair(IDC_EDIT_MAX_FILE_XFERS_PER_PROJECT, NT_POSITIVE);
	m_mapOption["max_stderr_file_size"] = std::make_pair(IDC_EDIT_MAX_STDERR_FILE_SIZE, NT_POSITIVE);
	m_mapOption["max_stdout_file_size"] = std::make_pair(IDC_EDIT_MAX_STDOUT_FILE_SIZE, NT_POSITIVE);
	m_mapOption["max_tasks_reported"] = std::make_pair(IDC_EDIT_MAX_TASKS_REPORTED, NT_POSITIVE);
	m_mapOption["ncpus"] = std::make_pair(IDC_EDIT_NCPUS, NT_ZEROABLE);
	m_mapOption["save_stats_days"] = std::make_pair(IDC_EDIT_SAVE_STATS_DAYS, NT_POSITIVE);
	m_mapOption["start_delay"] = std::make_pair(IDC_EDIT_START_DELAY, NT_POSITIVE);
}
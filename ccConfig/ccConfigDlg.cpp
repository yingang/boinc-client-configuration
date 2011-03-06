
// ccConfigDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ccConfig.h"
#include "ccConfigDlg.h"

#include "Util.h"
#include "BOINC.h"
#include "tinyxml.h"

#include <algorithm>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()

// CccConfigDlg dialog

#define ID_MYMENU				0x8000
#define ID_MYMENU_HELP_ENG		0x8001
#define ID_MYMENU_HELP_CHS		0x8002
#define ID_MYMENU_HELP_WEBSITE	0x8010
#define ID_MYMENU_HELP_EMAIL	0x8011
#define ID_MYMENU_SAVE_AS		0x8020
#define ID_MYMENU_FILE_OPEN		0x8021

#define ID_MYMENU_MRU_FIRST		0x8050
#define ID_MYMENU_MRU_END		0x8060

CccConfigDlg::CccConfigDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CccConfigDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	m_pMenuOpen = NULL;
	m_pMenuSave = NULL;
	m_pMenuHelp = NULL;
}

void CccConfigDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TABSHEET, m_tabSheet);
	DDX_Control(pDX, IDC_BTN_OPEN, m_btnOpen);
	DDX_Control(pDX, IDC_BTN_HELP, m_btnHelp);
	DDX_Control(pDX, IDC_BTN_SAVE, m_btnSave);
}

BEGIN_MESSAGE_MAP(CccConfigDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDCANCEL, &CccConfigDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDOK, &CccConfigDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BTN_SAVE, &CccConfigDlg::OnBnClickedSave)
	ON_BN_CLICKED(IDC_BTN_HELP, &CccConfigDlg::OnBnClickedHelp)
	ON_BN_CLICKED(IDC_BTN_DELETE, &CccConfigDlg::OnBnClickedDelete)
	ON_BN_CLICKED(IDC_BTN_OPEN, &CccConfigDlg::OnBnClickedOpen)
	ON_COMMAND_EX_RANGE(ID_MYMENU_MRU_FIRST, ID_MYMENU_MRU_END, CccConfigDlg::OnRecentFileOpen)
	ON_COMMAND_EX_RANGE(ID_MYMENU, ID_MYMENU + 100, CccConfigDlg::OnPopupMenuClicked)
END_MESSAGE_MAP()


// CccConfigDlg message handlers

BOOL CccConfigDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	m_tabSheet.AddPage(_T("Logging"), &m_pageLogging, IDD_PAGE_LOGGING);
	m_tabSheet.AddPage(_T("Options1"), &m_pageOption, IDD_PAGE_OPTION);
	m_tabSheet.AddPage(_T("Options2"), &m_pageOption2, IDD_PAGE_OPTION2);
	m_tabSheet.AddPage(_T("Options3"), &m_pageOption3, IDD_PAGE_OPTION3);
	m_tabSheet.AddPage(_T("Proxy"), &m_pageOption4, IDD_PAGE_OPTION4);
	m_tabSheet.Show();

	initPopupMenu();

	m_btnOpen.SetParentWnd(this)
		.SetDropDownMenu(m_pMenuOpen);
	loadRecentFileList();

	m_btnSave.SetParentWnd(this)
		.SetDropDownMenu(m_pMenuSave);

	m_btnHelp.SetParentWnd(this)
		.SetDropDownMenu(m_pMenuHelp);

	m_tooltip.Create(this, TTS_NOPREFIX | TTS_BALLOON);
	m_tooltip.AddTool(GetDlgItem(IDC_BTN_HELP), _T("Browse online documentation for all those options."));
	m_tooltip.AddTool(GetDlgItem(IDC_BTN_DELETE), _T("Delete currently loaded cc_config.xml."));
	m_tooltip.AddTool(GetDlgItem(IDC_BTN_SAVE), _T("Save and apply currently loaded cc_config.xml."));
	m_tooltip.AddTool(GetDlgItem(IDC_BTN_OPEN), _T("Open the local in-use cc_config.xml."));

	OnBnClickedOpen();

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CccConfigDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CccConfigDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CccConfigDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CccConfigDlg::OnBnClickedCancel()
{
	OnCancel();
}

void CccConfigDlg::OnBnClickedOk()
{
//	OnOK();
}

void CccConfigDlg::OnBnClickedSave()
{
	if (m_strFilePath.GetLength() == 0)
	{
		CString strFilePath;
		if (!getSaveAsFilePath(strFilePath))
			return;

		setCurrentFilePath(strFilePath);
	}

	saveConfigFile(m_strFilePath);
	applyConfigFile();
}

void CccConfigDlg::saveConfigFile(const CString& strFilePath)
{
	TiXmlDocument doc;

	TiXmlDeclaration* pDecl = new TiXmlDeclaration("1.0", "UTF-8", "");
	doc.LinkEndChild(pDecl);

	TiXmlElement* pRoot = new TiXmlElement("cc_config");

	TiXmlElement* pLogFlags = new TiXmlElement("log_flags");
	m_pageLogging.saveToXML(pLogFlags);
	pRoot->LinkEndChild(pLogFlags);

	TiXmlElement* pOptions = new TiXmlElement("options");
	m_pageOption.saveToXML(pOptions);
	m_pageOption2.saveToXML(pOptions);
	m_pageOption3.saveToXML(pOptions);
	m_pageOption4.saveToXML(pOptions);
	pRoot->LinkEndChild(pOptions);

	doc.LinkEndChild(pRoot);
	doc.SaveFile(UTIL::convertToMultiChar(LPCTSTR(m_strFilePath)).c_str());
}

void CccConfigDlg::applyConfigFile(void)
{
	CString strCmdPath = BOINC::getInstallDir() + _T("boinccmd.exe");
	if (!UTIL::doesFileExist(strCmdPath))
		return; // failed to locate the local boinccmd.exe

	UINT uDriveType = getConfigFileLocation();
	if (uDriveType == DRIVE_FIXED)
	{
		CString strDataDir = BOINC::getDataDir();
		if (strDataDir.GetLength() == 0
			|| m_strFilePath.Left(strDataDir.GetLength()) != strDataDir)
			return;	// it's not the local in-use file

		::ShellExecute(AfxGetMainWnd()->m_hWnd,
			_T("open"),
			strCmdPath,
			_T("--read_cc_config"),
			_T(""),
			SW_SHOW);
	}
	else if (uDriveType == DRIVE_REMOTE)
	{
		CString strPwd;
		if (!BOINC::getAuthPwd(m_strFilePath.Left(m_strFilePath.ReverseFind('\\') + 1), strPwd))
		{
			MessageBox(_T("Failed to access the remote gui_rpc_auth.cfg."),
				_T("Apply cc_config.xml"), MB_OK|MB_ICONERROR);
			return;
		}

		if (m_strFilePath.Left(2) != _T("\\\\"))
			return;	// can't figure out the remote hostname

		CString strHostName = m_strFilePath.Mid(2, m_strFilePath.Find(_T("\\"), 2) - 2);

		::ShellExecute(AfxGetMainWnd()->m_hWnd,
			_T("open"),
			strCmdPath,
			_T("--host ") + strHostName + _T(" --passwd ") + strPwd + _T(" --read_cc_config"),
			_T(""),
			SW_SHOW);

		MessageBox(m_strFilePath + _T(" has been modified, ")
			_T("please check the remote client log to see if the new configuation has been applied.\n\n")
			_T("If it's not, please check the remote machine:\n")
			_T("1. firewall setting (port 31416 must be unblocked)\n")
			_T("2. remote_hosts.cfg (local machine's ip/hostname must be added."),
			_T("Apply cc_config.xml"), MB_OK|MB_ICONINFORMATION);
	}
}

UINT CccConfigDlg::getConfigFileLocation(void)
{
	if (m_strFilePath.GetLength() < 3)
		return DRIVE_UNKNOWN;

	if (m_strFilePath.Left(2) == _T("\\\\"))
		return DRIVE_REMOTE;

	if (m_strFilePath.Mid(1, 2) != _T(":\\"))
		return DRIVE_UNKNOWN;

	return ::GetDriveType(m_strFilePath.Left(3));
}

BOOL CccConfigDlg::getSaveAsFilePath(CString& strFilePath)
{
	static TCHAR BASED_CODE szFilter[] = _T("BOINC Client Configuration|*.xml||");
	CFileDialog dlg(FALSE, 0, _T("cc_config.xml"), OFN_PATHMUSTEXIST | OFN_OVERWRITEPROMPT, szFilter);

	CString strInitDir = BOINC::getDataDir();
	dlg.m_ofn.lpstrInitialDir = strInitDir.GetBuffer();
	if (dlg.DoModal() == IDOK)
		strFilePath = dlg.GetPathName();
	else
		strFilePath = _T("");

	strInitDir.ReleaseBuffer();
	return strFilePath.GetLength() != 0;
}

BOOL CccConfigDlg::loadConfigFile(const CString& strFilePath)
{
	TiXmlDocument hXMLDoc(UTIL::convertToMultiChar(LPCTSTR(strFilePath)).c_str());
	if (!hXMLDoc.LoadFile())
	{
		MessageBox(_T("Failed to load cc_config.xml!"),
			_T("Load cc_config.xml"), MB_OK|MB_ICONERROR);
		return FALSE;
	}

	TiXmlElement* pElem = hXMLDoc.FirstChildElement();
	if (!pElem)
	{
		MessageBox(_T("Syntax error in cc_config.xml!"),
			_T("Load cc_config.xml"), MB_OK|MB_ICONERROR);
		return FALSE;
	}

	TiXmlHandle hXMLRoot = TiXmlHandle(pElem);

	TiXmlElement* pLogging = hXMLRoot.FirstChild("log_flags").Element();
	if (pLogging)
		m_pageLogging.loadFromXML(pLogging);

	TiXmlElement* pOptions = hXMLRoot.FirstChild("options").Element();
	if (pOptions)
	{
		m_pageOption.loadFromXML(pOptions);
		m_pageOption2.loadFromXML(pOptions);
		m_pageOption3.loadFromXML(pOptions);
		m_pageOption4.loadFromXML(pOptions);
	}

	MessageBox(strFilePath + _T(" has been loaded successfully."),
		_T("Load cc_config.xml"), MB_OK|MB_ICONINFORMATION);

	return TRUE;
}

void CccConfigDlg::OnBnClickedHelp()
{
	UTIL::openURL(_T("http://boinc.berkeley.edu/wiki/Client_configuration"));
}

void CccConfigDlg::OnBnClickedDelete()
{
	if (IDYES == MessageBox(_T("Do u want to delete the ") + m_strFilePath + _T("?"),
		_T("Delete cc_config.xml"), MB_YESNO|MB_ICONQUESTION))
	{
		removeFromRecentFileList(m_strFilePath);
		::DeleteFile(m_strFilePath);
		applyConfigFile();

		setCurrentFilePath(_T(""));

		m_pageLogging.restore();
		m_pageOption.restore();
		m_pageOption2.restore();
		m_pageOption3.restore();
		m_pageOption4.restore();
	}
}

void CccConfigDlg::OnBnClickedOpen()
{
	CString strFilePath = BOINC::getDataDir() + _T("cc_config.xml");
	if (loadConfigFile(strFilePath))
		setCurrentFilePath(strFilePath);
	else
		setCurrentFilePath(_T(""));
}

BOOL CccConfigDlg::OnRecentFileOpen(UINT nID)
{
	int nFileNo = nID - ID_MYMENU_MRU_FIRST + 1;

	std::list<CString>::iterator it= m_listRecentFile.begin();
	while (--nFileNo && it != m_listRecentFile.end())
		++it;

	if (it == m_listRecentFile.end())
		return TRUE;

	CString strFilePath = *it;
	if (loadConfigFile(strFilePath))
		setCurrentFilePath(strFilePath);
	else
		setCurrentFilePath(_T(""));

	return TRUE;
}

void CccConfigDlg::updateRecentFileMenu(void)
{
	for (int i = 1; i <= MAX_MRU_ITEM; ++i)
		m_pMenuOpen->DeleteMenu(ID_MYMENU_MRU_FIRST + i - 1, MF_BYCOMMAND);

	int i = 1;
	for (std::list<CString>::iterator it = m_listRecentFile.begin();
		it != m_listRecentFile.end() && i <= MAX_MRU_ITEM; ++it, ++i)
	{
		CString strNo;
		strNo.Format(_T("%d "), i);
		m_pMenuOpen->AppendMenu(MF_STRING, ID_MYMENU_MRU_FIRST + i - 1, strNo + *it);
	}
}

void CccConfigDlg::loadRecentFileList(void)
{
	for (int i = 1; i <= MAX_MRU_ITEM; ++i)
	{
		CString sKeyName;
		sKeyName.Format(_T("File%d"), i);

		CString sFileName = AfxGetApp()->GetProfileString(_T("Recent File List"), sKeyName, _T(""));
		if (sFileName.GetLength() == 0)
			break;
		m_listRecentFile.push_back(sFileName);
	}

	updateRecentFileMenu();
}

void CccConfigDlg::saveRecentFileList(void)
{
	int i = 1;
	CString sKeyName;
	for (std::list<CString>::iterator it = m_listRecentFile.begin();
		it != m_listRecentFile.end() && i <= MAX_MRU_ITEM; ++it, ++i)
	{
		sKeyName.Format(_T("File%d"), i);
		AfxGetApp()->WriteProfileString(_T("Recent File List"), sKeyName, *it);
	}
}

void CccConfigDlg::PostNcDestroy()
{
	saveRecentFileList();

	CDialog::PostNcDestroy();
}

void CccConfigDlg::addToRecentFileList(const CString& strFilePath)
{
	removeFromRecentFileList(strFilePath, FALSE);
	m_listRecentFile.push_front(strFilePath);

	updateRecentFileMenu();
}

void CccConfigDlg::removeFromRecentFileList(const CString& strFilePath, BOOL bRefresh)
{
	std::list<CString>::iterator it = std::find(m_listRecentFile.begin(), m_listRecentFile.end(), strFilePath);
	if (it != m_listRecentFile.end())
		m_listRecentFile.erase(it);

	if (bRefresh)
		updateRecentFileMenu();
}

void CccConfigDlg::initPopupMenu(void)
{
	m_pMenuOpen = new CMenu();
	m_pMenuOpen->CreatePopupMenu();
	m_pMenuOpen->AppendMenu(MF_STRING|MF_BYCOMMAND, ID_MYMENU_FILE_OPEN, _T("&Open..."));
	m_pMenuOpen->AppendMenu(MF_SEPARATOR, 0, _T(""));

	m_pMenuSave = new CMenu();
	m_pMenuSave->CreatePopupMenu();
	m_pMenuSave->AppendMenu(MF_STRING|MF_BYCOMMAND, ID_MYMENU_SAVE_AS, _T("Save &As..."));

	m_pMenuHelp = new CMenu();
	m_pMenuHelp->CreatePopupMenu();
	m_pMenuHelp->AppendMenu(MF_STRING|MF_BYCOMMAND, ID_MYMENU_HELP_ENG, _T("English"));
	m_pMenuHelp->AppendMenu(MF_STRING|MF_BYCOMMAND, ID_MYMENU_HELP_CHS, _T("Chinese"));
	m_pMenuHelp->AppendMenu(MF_SEPARATOR);
	m_pMenuHelp->AppendMenu(MF_STRING|MF_BYCOMMAND, ID_MYMENU_HELP_WEBSITE, _T("Project Website"));
	m_pMenuHelp->AppendMenu(MF_STRING|MF_BYCOMMAND, ID_MYMENU_HELP_EMAIL, _T("Email to Author"));
}

BOOL CccConfigDlg::OnPopupMenuClicked(UINT nID)
{
	switch (nID)
	{
	case ID_MYMENU_HELP_ENG:
		OnBnClickedHelp();
		break;
	case ID_MYMENU_HELP_CHS:
		UTIL::openURL(_T("http://www.equn.com/wiki/BOINC:%E5%AE%A2%E6%88%B7%E7%AB%AF%E9%85%8D%E7%BD%AE"));
		break;
	case ID_MYMENU_HELP_WEBSITE:
		UTIL::openURL(_T("http://code.google.com/p/boinc-client-configuration/"));
		break;
	case ID_MYMENU_HELP_EMAIL:
		UTIL::openURL(_T("mailto://zenith.yin@gmail.com"));
		break;
	case ID_MYMENU_FILE_OPEN:
		{
			static TCHAR BASED_CODE szFilter[] = _T("BOINC Client Configuration|*.xml||");
			CFileDialog dlg(TRUE, 0, 0, OFN_FILEMUSTEXIST, szFilter);
			if (dlg.DoModal() == IDOK)
			{
				CString strFilePath = dlg.GetPathName();
				if (loadConfigFile(strFilePath))
					setCurrentFilePath(strFilePath);
				else
					setCurrentFilePath(_T(""));
			}
		}
	case ID_MYMENU_SAVE_AS:
		{
			CString strFilePath;
			if (getSaveAsFilePath(strFilePath))
			{
				setCurrentFilePath(strFilePath);
				OnBnClickedSave();
			}
		}
	default:
		break;
	}

	return TRUE;
}

BOOL CccConfigDlg::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_MOUSEMOVE)
		m_tooltip.RelayEvent(pMsg);

	return CDialog::PreTranslateMessage(pMsg);
}

BOOL CccConfigDlg::setToolTipIconAndTitle(CToolTipCtrl *pToolTip, int tti, const CString& strTitle)
{
	return ::SendMessage((HWND)pToolTip->m_hWnd, (UINT)TTM_SETTITLE, (WPARAM)tti, (LPARAM)(LPCTSTR)strTitle);
}

void CccConfigDlg::setCurrentFilePath(const CString& strFilePath)
{
	m_strFilePath = strFilePath;

	if (m_strFilePath.GetLength() > 0)
	{
		GetDlgItem(IDC_EDIT_FILEPATH)->SetWindowText(m_strFilePath);
		GetDlgItem(IDC_BTN_DELETE)->EnableWindow(TRUE);

		addToRecentFileList(m_strFilePath);
	}
	else
	{
		GetDlgItem(IDC_EDIT_FILEPATH)->SetWindowText(_T("No file is loaded!"));
		GetDlgItem(IDC_BTN_DELETE)->EnableWindow(FALSE);
	}
}
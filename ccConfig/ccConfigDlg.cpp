
// ccConfigDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ccConfig.h"
#include "ccConfigDlg.h"

#include "Util.h"
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

#define ID_MYMENU			0x8000
#define ID_MYMENU_HELP_E	0x8001
#define ID_MYMENU_HELP_C	0x8002

CccConfigDlg::CccConfigDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CccConfigDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CccConfigDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TABSHEET, m_tabSheet);
	DDX_Control(pDX, IDC_BTN_OPEN, m_btnOpen);
	DDX_Control(pDX, IDC_BTN_HELP, m_btnHelp);
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
	ON_COMMAND(ID_FILE_OPEN, &CccConfigDlg::OnFileOpen)
	ON_COMMAND_EX_RANGE(ID_FILE_MRU_FILE1, ID_FILE_MRU_FILE16, CccConfigDlg::OnRecentFileOpen)
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

	m_tabSheet.AddPage(_T("Logging Flags"), &m_pageLogging, IDD_PAGE_LOGGING);
	m_tabSheet.AddPage(_T("Options1"), &m_pageOption, IDD_PAGE_OPTION);
	m_tabSheet.AddPage(_T("Options2"), &m_pageOption2, IDD_PAGE_OPTION2);
	m_tabSheet.AddPage(_T("Options3"), &m_pageOption3, IDD_PAGE_OPTION3);
	m_tabSheet.AddPage(_T("Options4"), &m_pageOption4, IDD_PAGE_OPTION4);
	m_tabSheet.Show();

	initPopupMenu();

	m_btnOpen.SetParentWnd(this)
		.SetDropDownMenu(m_pMenuOpen);
	loadRecentFileList();

	m_btnHelp.SetParentWnd(this)
		.SetDropDownMenu(m_pMenuHelp);

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
	saveConfigFileAs(m_strFileName);
}

void CccConfigDlg::saveConfigFileAs(const CString& strFileName)
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
	doc.SaveFile(UTIL::convertToMultiChar(LPCTSTR(m_strFileName)).c_str());

	MessageBox(m_strFileName + _T(" has been modified, ")
			+ _T("please re-read it in BOINC Manager (5.8.2+) or restart boinc.exe (prior 5.8.2)."),
		_T("Save cc_config.xml"), MB_OK|MB_ICONINFORMATION);
}

BOOL CccConfigDlg::loadConfigFile()
{
	TiXmlDocument hXMLDoc(UTIL::convertToMultiChar(LPCTSTR(m_strFileName)).c_str());
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

	MessageBox(m_strFileName + _T(" has been loaded successfully."),
		_T("Load cc_config.xml"), MB_OK|MB_ICONINFORMATION);

	return TRUE;
}

void CccConfigDlg::OnBnClickedHelp()
{
	UTIL::openURL(_T("http://boinc.berkeley.edu/wiki/Client_configuration"));
}

void CccConfigDlg::OnBnClickedDelete()
{
	if (IDYES == MessageBox(_T("Do u want to delete the ") + m_strFileName + _T("?"),
		_T("Delete cc_config.xml"), MB_YESNO|MB_ICONQUESTION))
	{
		::DeleteFile(m_strFileName);

		m_pageLogging.restore();
		m_pageOption.restore();
		m_pageOption2.restore();
		m_pageOption3.restore();
		m_pageOption4.restore();
	}
}

void CccConfigDlg::OnBnClickedOpen()
{
	m_strFileName = UTIL::getDataDir() + _T("cc_config.xml");
	if (loadConfigFile())
		addToRecentFileList(m_strFileName);
}

void CccConfigDlg::OnFileOpen()
{
	static TCHAR BASED_CODE szFilter[] = _T("BOINC Client Configuration|*.xml||");

	CFileDialog dlg(TRUE, 0, 0, OFN_FILEMUSTEXIST, szFilter);
	if (dlg.DoModal() == IDOK)
	{
		m_strFileName = dlg.GetPathName();
		if (loadConfigFile())
			addToRecentFileList(m_strFileName);
	}
}

BOOL CccConfigDlg::OnRecentFileOpen(UINT nID)
{
	int nFileNo = nID - ID_FILE_MRU_FILE1 + 1;

	std::list<CString>::iterator it= m_listRecentFile.begin();
	while (--nFileNo && it != m_listRecentFile.end())
		++it;

	if (it == m_listRecentFile.end())
		return TRUE;

	m_strFileName = *it;
	if (loadConfigFile())
		addToRecentFileList(m_strFileName);

	return TRUE;
}

void CccConfigDlg::updateRecentFileMenu(void)
{
	for (int i = 1; i <= MAX_MRU_ITEM; ++i)
		m_pMenuOpen->DeleteMenu(ID_FILE_MRU_FILE1 + i - 1, MF_BYCOMMAND);

	int i = 1;
	for (std::list<CString>::iterator it = m_listRecentFile.begin();
		it != m_listRecentFile.end() && i <= MAX_MRU_ITEM; ++it, ++i)
	{
		CString strNo;
		strNo.Format(_T("%d "), i);
		m_pMenuOpen->AppendMenu(MF_STRING, ID_FILE_MRU_FILE1 + i - 1, strNo + *it);

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

void CccConfigDlg::addToRecentFileList(const CString& strFileName)
{
	std::list<CString>::iterator it = std::find(m_listRecentFile.begin(), m_listRecentFile.end(), strFileName);
	if (it != m_listRecentFile.end())
		m_listRecentFile.erase(it);
	m_listRecentFile.push_front(strFileName);

	updateRecentFileMenu();
}

void CccConfigDlg::initPopupMenu(void)
{
	m_pMenuOpen = new CMenu();
	m_pMenuOpen->CreatePopupMenu();
	m_pMenuOpen->AppendMenu(MF_STRING|MF_BYCOMMAND, ID_FILE_OPEN, _T("&Open..."));
	m_pMenuOpen->AppendMenu(MF_SEPARATOR, 0, _T(""));

	m_pMenuHelp = new CMenu();
	m_pMenuHelp->CreatePopupMenu();
	m_pMenuHelp->AppendMenu(MF_STRING|MF_BYCOMMAND, ID_MYMENU_HELP_E, _T("English"));
	m_pMenuHelp->AppendMenu(MF_STRING|MF_BYCOMMAND, ID_MYMENU_HELP_C, _T("Chinese"));
}

BOOL CccConfigDlg::OnPopupMenuClicked(UINT nID)
{
	switch (nID)
	{
	case ID_MYMENU_HELP_E:
		OnBnClickedHelp();
		break;
	case ID_MYMENU_HELP_C:
		UTIL::openURL(_T("http://www.equn.com/wiki/BOINC:%E5%AE%A2%E6%88%B7%E7%AB%AF%E9%85%8D%E7%BD%AE"));
		break;
	default:
		break;
	}

	return TRUE;
}
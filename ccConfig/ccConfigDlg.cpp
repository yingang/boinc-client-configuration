
// ccConfigDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ccConfig.h"
#include "ccConfigDlg.h"

#include "Util.h"
#include "tinyxml.h"

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

CccConfigDlg::CccConfigDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CccConfigDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CccConfigDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TABSHEET, m_tabSheet);
}

BEGIN_MESSAGE_MAP(CccConfigDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDCANCEL, &CccConfigDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDOK, &CccConfigDlg::OnBnClickedOk)
	ON_BN_CLICKED(ID_SAVE, &CccConfigDlg::OnBnClickedSave)
	ON_BN_CLICKED(ID_LOAD, &CccConfigDlg::OnBnClickedLoad)
	ON_BN_CLICKED(ID_HELP_E, &CccConfigDlg::OnBnClickedHelpE)
	ON_BN_CLICKED(ID_DELETE, &CccConfigDlg::OnBnClickedDelete)
	ON_BN_CLICKED(ID_HELP_C, &CccConfigDlg::OnBnClickedHelpC)
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

	OnBnClickedLoad();

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
	doc.SaveFile((UTIL::convertToMultiChar(LPCTSTR(UTIL::getDataDir())) + "cc_config.xml").c_str());

	MessageBox(UTIL::getDataDir()
			+ _T("cc_config.xml has been modified, ")
			+ _T("please re-read it in BOINC Manager (5.8.2+) or restart boinc.exe (prior 5.8.2)."),
		_T("Save cc_config.xml"), MB_OK|MB_ICONINFORMATION);
}

void CccConfigDlg::OnBnClickedLoad()
{
	TiXmlDocument hXMLDoc((UTIL::convertToMultiChar(LPCTSTR(UTIL::getDataDir())) + "cc_config.xml").c_str());
	if (!hXMLDoc.LoadFile())
	{
		MessageBox(_T("Failed to locate cc_config.xml!"),
			_T("Load cc_config.xml"), MB_OK|MB_ICONERROR);
		return;
	}

	TiXmlElement* pElem = hXMLDoc.FirstChildElement();
	if (!pElem)
	{
		MessageBox(_T("Syntax error in cc_config.xml!"),
			_T("Load cc_config.xml"), MB_OK|MB_ICONERROR);
		return;
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

	MessageBox(UTIL::getDataDir() + _T("cc_config.xml has been loaded successfully."),
		_T("Save cc_config.xml"), MB_OK|MB_ICONINFORMATION);
}

void CccConfigDlg::OnBnClickedHelpE()
{
	UTIL::openURL(_T("http://boinc.berkeley.edu/wiki/Client_configuration"));
}

void CccConfigDlg::OnBnClickedDelete()
{
	if (IDYES == MessageBox(_T("Do u want to delete the cc_config.xml?"),
		_T("Delete cc_config.xml"), MB_YESNO|MB_ICONQUESTION))
	{
		::DeleteFile(UTIL::getDataDir() + _T("cc_config.xml"));

		m_pageLogging.restore();
		m_pageOption.restore();
		m_pageOption2.restore();
		m_pageOption3.restore();
		m_pageOption4.restore();
	}
}

void CccConfigDlg::OnBnClickedHelpC()
{
	UTIL::openURL(_T("http://www.equn.com/wiki/BOINC:%E5%AE%A2%E6%88%B7%E7%AB%AF%E9%85%8D%E7%BD%AE"));
}

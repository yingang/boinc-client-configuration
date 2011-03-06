
// ccConfigDlg.h : header file
//

#pragma once
#include "afxcmn.h"

#include "TabSheet.h"
#include "PageLogging.h"
#include "PageOption.h"
#include "PageOption2.h"
#include "PageOption3.h"
#include "PageOption4.h"
#include "..\XButtonXP\XButtonXP.h"

#include <list>

#include "afxwin.h"

#define MAX_MRU_ITEM	9

// CccConfigDlg dialog
class CccConfigDlg : public CDialog
{
// Construction
public:
	CccConfigDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_CCCONFIG_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

private:
	void loadRecentFileList(void);
	void saveRecentFileList(void);
	void addToRecentFileList(const CString& strFilePath);
	void removeFromRecentFileList(const CString& strFilePath, BOOL bRefresh = TRUE);

	void updateRecentFileMenu(void);

	BOOL loadConfigFile(const CString& strFilePath);
	void saveConfigFile(const CString& strFilePath);
	void applyConfigFile(void);

	UINT getConfigFileLocation(void);
	BOOL getSaveAsFilePath(CString& strFilePath);
	void setCurrentFilePath(const CString& strFilePath);

	void initPopupMenu(void);
	BOOL setToolTipIconAndTitle(CToolTipCtrl* pToolTip, int tti, const CString& strTitle);

	std::list<CString> m_listRecentFile;
	CString m_strFilePath;
	CMenu* m_pMenuOpen;
	CMenu* m_pMenuSave;
	CMenu* m_pMenuHelp;
	CToolTipCtrl m_tooltip;

	CPageLogging m_pageLogging;
	CPageOption m_pageOption;
	CPageOption2 m_pageOption2;
	CPageOption3 m_pageOption3;
	CPageOption4 m_pageOption4;

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	virtual void PostNcDestroy();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CTabSheet m_tabSheet;
	CXButtonXP m_btnOpen;
	CXButtonXP m_btnSave;
	CXButtonXP m_btnHelp;
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedSave();
	afx_msg void OnBnClickedHelp();
	afx_msg void OnBnClickedDelete();
	afx_msg void OnBnClickedOpen();
	afx_msg BOOL OnRecentFileOpen(UINT nID);
	afx_msg BOOL OnPopupMenuClicked(UINT nID);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};

// TabSheet.cpp : implementation file
//

#include "stdafx.h"
#include "TabSheet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTabSheet

CTabSheet::CTabSheet()
{
	m_nNumOfPages = 0;
	m_nCurrentPage = 0;

	m_nMaxWidth = 0;
	m_nMaxHeight = 0;
}

CTabSheet::~CTabSheet()
{
}

BEGIN_MESSAGE_MAP(CTabSheet, CTabCtrl)
	//{{AFX_MSG_MAP(CTabSheet)
	ON_WM_LBUTTONDOWN()
	ON_NOTIFY_REFLECT(TCN_SELCHANGE, OnTcnSelchange)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTabSheet message handlers

BOOL CTabSheet::AddPage(LPCTSTR title, CDialog *pDialog, UINT ID)
{
	if (MAXPAGE == m_nNumOfPages)
		return FALSE;

	m_IDD[m_nNumOfPages] = ID;
	m_Title[m_nNumOfPages] = title;

	pDialog->DestroyWindow();
	pDialog->Create(m_IDD[m_nNumOfPages], GetParent());

	CRect rect;
	pDialog->GetWindowRect(&rect);
	m_nMaxWidth = rect.Width() < m_nMaxWidth ? m_nMaxWidth : rect.Width();
	m_nMaxHeight = rect.Height() < m_nMaxHeight ? m_nMaxHeight : rect.Height();

	m_pPages[m_nNumOfPages] = pDialog;

	InsertItem(m_nNumOfPages, m_Title[m_nNumOfPages]);

	m_nNumOfPages++;

	return TRUE;
}

void CTabSheet::SetRect()
{
	CRect DisplayArea;

	GetWindowRect(&DisplayArea);
	GetParent()->ScreenToClient(&DisplayArea);	// to our parent's coordinates
	AdjustRect(FALSE, &DisplayArea);			// Get current display area
	DisplayArea.right = m_nMaxWidth + DisplayArea.left;
	DisplayArea.bottom = m_nMaxHeight + DisplayArea.top;
	AdjustRect(TRUE, &DisplayArea);
	MoveWindow(&DisplayArea);					// resize ourself
	AdjustRect(FALSE, &DisplayArea);

	for (int nCount = 0; nCount < m_nNumOfPages; nCount++)
		m_pPages[nCount]->MoveWindow(DisplayArea.left, DisplayArea.top, DisplayArea.Width(), DisplayArea.Height());
}

void CTabSheet::Show()
{
	SetRect();

	m_pPages[0]->ShowWindow(SW_SHOW);
	for (int i=1; i < m_nNumOfPages; i++)
		m_pPages[i]->ShowWindow(SW_HIDE);
}

void CTabSheet::OnLButtonDown(UINT nFlags, CPoint point) 
{
	CTabCtrl::OnLButtonDown(nFlags, point);

	if (m_nCurrentPage != GetCurFocus())
	{
		m_pPages[m_nCurrentPage]->ShowWindow(SW_HIDE);
		m_nCurrentPage=GetCurFocus();
		m_pPages[m_nCurrentPage]->ShowWindow(SW_SHOW);
	}
}

void CTabSheet::OnTcnSelchange(NMHDR *pNMHDR, LRESULT *pResult)
{
	if (m_nCurrentPage != GetCurFocus())
	{
		m_pPages[m_nCurrentPage]->ShowWindow(SW_HIDE);
		m_nCurrentPage=GetCurFocus();
		m_pPages[m_nCurrentPage]->ShowWindow(SW_SHOW);
	}
}

int CTabSheet::SetCurSel(int nItem)
{
	if ((nItem < 0) || (nItem >= m_nNumOfPages))
		return -1;

	int ret = m_nCurrentPage;

	if (m_nCurrentPage != nItem)
	{
		m_pPages[m_nCurrentPage]->ShowWindow(SW_HIDE);
		m_nCurrentPage = nItem;
		m_pPages[m_nCurrentPage]->ShowWindow(SW_SHOW);
		CTabCtrl::SetCurSel(nItem);
	}

	return ret;
}

int CTabSheet::GetCurSel()
{
	return CTabCtrl::GetCurSel();
}

#pragma once

#define MAXPAGE 16

class CTabSheet : public CTabCtrl
{
public:
	CTabSheet();
	virtual ~CTabSheet();

public:
	int GetCurSel();
	int SetCurSel(int nItem);
	void Show();
	BOOL AddPage(LPCTSTR title, CDialog *pDialog, UINT ID);

protected:
	void SetRect();

private:
	LPCTSTR m_Title[MAXPAGE];
	UINT m_IDD[MAXPAGE];
	CDialog* m_pPages[MAXPAGE];
	int m_nNumOfPages;
	int m_nCurrentPage;
	int m_nMaxWidth;
	int m_nMaxHeight;

protected:
	//{{AFX_MSG(CTabSheet)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnTcnSelchange(NMHDR *pNMHDR, LRESULT *pResult);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

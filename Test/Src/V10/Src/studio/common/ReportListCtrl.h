#pragma once

#include <list>
// CReportListCtrl

class CReportListCtrl : public CListCtrl
{
	DECLARE_DYNAMIC(CReportListCtrl)

public:
	CReportListCtrl();
	virtual ~CReportListCtrl();

public:
  virtual BOOL SetItemText(_In_ int nItem, _In_ int nSubItem, _In_z_ LPCTSTR lpszText);

  virtual int InsertItem(const LVITEM* pItem);
  virtual int InsertItem(int nItem, LPCTSTR lpszItem);
  virtual int InsertItem(int nItem, LPCTSTR lpszItem, int nImage);
  virtual int InsertItem(UINT nMask, int nItem, LPCTSTR lpszItem, UINT nState, UINT nStateMask, int nImage, LPARAM lParam);

  virtual int InsertColumn(int nCol, const LVCOLUMN* pColumn);
  virtual int InsertColumn(int nCol, LPCTSTR lpszColumnHeading, int nFormat = LVCFMT_LEFT, int nWidth = -1, int nSubItem = -1);

public:
  // 添加列头描述
  void AddColumnHeader(CString strHeader);

  // 自动调整列宽
  void AdjustColumnWidth();

protected:
  void Sort(int nColumnIndex = 0, bool bAsc = true);
protected:
  int m_nSortColumnIndex;
  bool m_bAscending;

protected:
  std::list<CString> m_list_column_header;

private:
  static int CALLBACK MyCompareProc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort);

protected:
	DECLARE_MESSAGE_MAP()

protected:
  virtual void PreSubclassWindow();

public:
  afx_msg void OnLvnColumnclick(NMHDR *pNMHDR, LRESULT *pResult);
};
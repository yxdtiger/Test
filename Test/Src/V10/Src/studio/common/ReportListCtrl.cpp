// MyListCtrl.cpp : implementation file
//

#include "../main_frame/StdAfx.h"
#include "ReportListCtrl.h"

using namespace std;

// CReportListCtrl

IMPLEMENT_DYNAMIC(CReportListCtrl, CListCtrl)

CReportListCtrl::CReportListCtrl()
{
  m_nSortColumnIndex = 0;
}

CReportListCtrl::~CReportListCtrl()
{
}


BEGIN_MESSAGE_MAP(CReportListCtrl, CListCtrl)
  ON_NOTIFY(HDN_ITEMCLICKW, 0, &CReportListCtrl::OnLvnColumnclick)  // Unicode
  ON_NOTIFY(HDN_ITEMCLICKA, 0, &CReportListCtrl::OnLvnColumnclick)  // ANSI
END_MESSAGE_MAP()

// CReportListCtrl message handlers
int CALLBACK CReportListCtrl::MyCompareProc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
  // lParamSort contains a pointer to the list view control.
  // The lParam of an item is just its index.
  CReportListCtrl* pListCtrl = (CReportListCtrl*)lParamSort;
  CString    strItem1 = pListCtrl->GetItemText(lParam1, pListCtrl->m_nSortColumnIndex);
  CString    strItem2 = pListCtrl->GetItemText(lParam2, pListCtrl->m_nSortColumnIndex);

  //int nVal1 = atoi(strItem1.GetBuffer(0));
  //int nVal2 = atoi(strItem2.GetBuffer(0));
  //strItem1.ReleaseBuffer();
  //strItem2.ReleaseBuffer();

  //return nVal1 < nVal2 ? -1 : 1;

  if(pListCtrl->m_bAscending)
    return strItem1 < strItem2 ? -1 : 1;
  else
    return strItem1 < strItem2 ? 1 : -1;
}

void CReportListCtrl::Sort(int nColumnIndex /* = 0 */, bool bAsc /* = true */)
{
  m_nSortColumnIndex = nColumnIndex;
  m_bAscending = bAsc;

  // 设定排序值
  int num = this->GetItemCount(); 
  while(num--)
    this->SetItemData(num, num);  

  SortItems(MyCompareProc, (LPARAM)this);
}

void CReportListCtrl::PreSubclassWindow()
{
  // TODO: Add your specialized code here and/or call the base class

  CRect rc;
  GetClientRect(&rc);

  if(!m_list_column_header.empty())
  {
    int nWidth = rc.Width() / m_list_column_header.size();
    int nColumnIndex = 0;
    for( list<CString>::iterator it = m_list_column_header.begin(); it != m_list_column_header.end(); ++it )
    {
      InsertColumn(nColumnIndex++, *it, LVCFMT_CENTER, nWidth);
    }
  }

  // 设置样式
  SetExtendedStyle(LVS_EX_FLATSB|LVS_EX_FULLROWSELECT|LVS_EX_HEADERDRAGDROP|LVS_EX_ONECLICKACTIVATE|LVS_EX_GRIDLINES);

  CListCtrl::PreSubclassWindow();
}

void CReportListCtrl::AddColumnHeader(CString strHeader)
{
  m_list_column_header.push_back(strHeader);
}

void CReportListCtrl::AdjustColumnWidth()
{
  ShowWindow(SW_HIDE);
  SetRedraw(FALSE);

  __super::InsertColumn(GetHeaderCtrl()->GetItemCount(), ""); 

  int nColumnCount = GetHeaderCtrl()->GetItemCount();
  for (int i = 0; i < nColumnCount; i++)
  {
    //SetColumnWidth(i, LVSCW_AUTOSIZE);  
    //int nColumnWidth =GetColumnWidth(i);     // 表头的宽度  
    //SetColumnWidth(i, LVSCW_AUTOSIZE_USEHEADER);  // 数据的宽度  
    //int nHeaderWidth = GetColumnWidth(i);  
    //SetColumnWidth(i, nColumnWidth>nHeaderWidth?nColumnWidth:nHeaderWidth);  // 取较大值  

    // 表头宽度调整
    SetColumnWidth(i, LVSCW_AUTOSIZE_USEHEADER);  
  }
  DeleteColumn(GetHeaderCtrl()->GetItemCount()-1); 

  SetRedraw(TRUE);
  ShowWindow(SW_SHOW);
}

BOOL CReportListCtrl::SetItemText(_In_ int nItem, _In_ int nSubItem, _In_z_ LPCTSTR lpszText)
{
  BOOL bRet = __super::SetItemText(nItem, nSubItem, lpszText);

  // 自动调整列宽
  AdjustColumnWidth();

  return bRet;
}

int CReportListCtrl::InsertItem(const LVITEM* pItem)
{
  int nRet = __super::InsertItem(pItem);
  // 自动调整列宽
  AdjustColumnWidth();
  return nRet;
}

int CReportListCtrl::InsertItem(int nItem, LPCTSTR lpszItem)
{
  int nRet = __super::InsertItem(nItem, lpszItem);
  // 自动调整列宽
  AdjustColumnWidth();
  return nRet;
}

int CReportListCtrl::InsertItem(int nItem, LPCTSTR lpszItem, int nImage)
{
  int nRet = __super::InsertItem(nItem, lpszItem, nImage);
  // 自动调整列宽
  AdjustColumnWidth();
  return nRet;
}

int CReportListCtrl::InsertItem(UINT nMask, int nItem, LPCTSTR lpszItem, UINT nState, UINT nStateMask, int nImage, LPARAM lParam)
{
  int nRet = __super::InsertItem(nMask, nItem, lpszItem, nState, nStateMask, nImage, lParam);
  // 自动调整列宽
  AdjustColumnWidth();
  return nRet;
}

int CReportListCtrl::InsertColumn(int nCol, const LVCOLUMN* pColumn)
{
  int nRet = __super::InsertColumn(nCol, pColumn);
  // 自动调整列宽
  AdjustColumnWidth();
  return nRet;
}

int CReportListCtrl::InsertColumn(int nCol, LPCTSTR lpszColumnHeading, int nFormat /* = LVCFMT_LEFT */, int nWidth /* = -1 */, int nSubItem /* = -1 */)
{
  int nRet = __super::InsertColumn(nCol, lpszColumnHeading, nFormat, nWidth, nSubItem);
  // 自动调整列宽
  AdjustColumnWidth();
  return nRet;
}

void CReportListCtrl::OnLvnColumnclick(NMHDR *pNMHDR, LRESULT *pResult)
{
  LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
  // TODO: Add your control notification handler code here
  Sort(pNMLV->iItem, m_bAscending);
  m_bAscending = !m_bAscending;

  *pResult = 0;
}

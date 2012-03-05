// MyListCtrl.cpp : implementation file
//

#include "../main_frame/StdAfx.h"
#include "MyListCtrl.h"

using namespace std;

// CReportListCtrl

IMPLEMENT_DYNAMIC(CReportListCtrl, CListCtrl)

CReportListCtrl::CReportListCtrl()
{

}

CReportListCtrl::~CReportListCtrl()
{
}


BEGIN_MESSAGE_MAP(CReportListCtrl, CListCtrl)
END_MESSAGE_MAP()



// CReportListCtrl message handlers
int CALLBACK CReportListCtrl::MyCompareProc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
  // lParamSort contains a pointer to the list view control.
  // The lParam of an item is just its index.
  CListCtrl* pListCtrl = (CListCtrl*)lParamSort;
  CString    strItem1 = pListCtrl->GetItemText(lParam1, 0);
  CString    strItem2 = pListCtrl->GetItemText(lParam2, 0);

  int nVal1 = atoi(strItem1.GetBuffer(0));
  int nVal2 = atoi(strItem2.GetBuffer(0));
  strItem1.ReleaseBuffer();
  strItem2.ReleaseBuffer();

  return nVal1 < nVal2 ? -1 : 1;
}

void CReportListCtrl::Sort()
{
  // Éè¶¨ÅÅÐòÖµ
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

  CListCtrl::PreSubclassWindow();
}

void CReportListCtrl::AddColumnHeader(CString strHeader)
{
  m_list_column_header.push_back(strHeader);
}
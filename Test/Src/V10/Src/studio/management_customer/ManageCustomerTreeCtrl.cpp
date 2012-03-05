// ManageCustomerTreeCtrl.cpp : implementation file
//

#include "../main_frame/StdAfx.h"
#include "ManageCustomerTreeCtrl.h"
#include "../main_frame/Resource.h"

// ManageCustomerTreeCtrl

IMPLEMENT_DYNAMIC(ManageCustomerTreeCtrl, CTreeCtrl)

ManageCustomerTreeCtrl::ManageCustomerTreeCtrl()
{

}

ManageCustomerTreeCtrl::~ManageCustomerTreeCtrl()
{
}


BEGIN_MESSAGE_MAP(ManageCustomerTreeCtrl, CTreeCtrl)
  ON_WM_LBUTTONDOWN()
  ON_WM_RBUTTONDOWN()
  ON_NOTIFY_REFLECT(NM_CLICK, OnStateIconClick)
  ON_NOTIFY_REFLECT(TVN_KEYDOWN, OnKeydown)
  ON_WM_KEYDOWN()
  ON_NOTIFY_REFLECT(TVN_SELCHANGED, &ManageCustomerTreeCtrl::OnTvnSelchanged)
END_MESSAGE_MAP()

// ManageCustomerTreeCtrl message handlers


void ManageCustomerTreeCtrl::OnRButtonDown(UINT nFlags, CPoint point) 
{
  // set focus to the tree control
  SetFocus();

  // map the point that is passed to the 
  // function from client coordinates
  // to screen coordinates
  ClientToScreen(&point);

  // Get the currently selected item
  HTREEITEM hCurrSel = GetSelectedItem();

  // Figure out which item was right clicked
  CPoint pt(0, 0);
  ::GetCursorPos(&pt);
  ScreenToClient (&pt);
  HTREEITEM hNewSel = HitTest(pt, &nFlags);

  // Set the selection to the item that the
  // mouse was over when the user right
  // clicked
  if (NULL == hNewSel)
  {
    SelectItem(NULL);
  }
  else if (hCurrSel != hNewSel)
  {
    SelectItem(hNewSel);
    SetFocus();
  }
}


void ManageCustomerTreeCtrl::OnTvnSelchanged(NMHDR *pNMHDR, LRESULT *pResult)
{
  LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
  // TODO: Add your control notification handler code here
  *pResult = 0;
}
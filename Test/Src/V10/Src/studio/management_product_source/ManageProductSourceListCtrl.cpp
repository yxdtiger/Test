// ManageProductSourceListCtrl.cpp : implementation file
//

#include "../main_frame/StdAfx.h"
#include "ManageProductSourceListCtrl.h"

// ManageProductSourceListCtrl

IMPLEMENT_DYNAMIC(ManageProductSourceListCtrl, CListCtrl)

ManageProductSourceListCtrl::ManageProductSourceListCtrl()
{

}

ManageProductSourceListCtrl::~ManageProductSourceListCtrl()
{
}


BEGIN_MESSAGE_MAP(ManageProductSourceListCtrl, CListCtrl)
END_MESSAGE_MAP()



// ManageProductSourceListCtrl message handlers

void ManageProductSourceListCtrl::InsertProductSource(wmdf::ProductSource* pProductSource)
{
  // fill "m_conListUser" 
  int item_index = GetItemCount();
  int sub_item_index = 0;
  CString context;

  // add user_id
  context.Format("%d", pProductSource->product_id());
  InsertItem(item_index, context);

  // add ip_address
  context = pProductSource->ip_address().c_str();
  SetItemText(item_index, ++sub_item_index, context);

  // add port
  context.Format("%d", pProductSource->port()); 
  SetItemText(item_index, ++sub_item_index, context);

  // add description
  context = pProductSource->description().c_str();
  SetItemText(item_index, ++sub_item_index, context);

  // ≈≈–Ú
  Sort();
}

void ManageProductSourceListCtrl::DeleteProductSource(int32_t product_id)
{
  for(int i = 0; i < GetItemCount(); ++i)
  {
    CString str_user_id = GetItemText(i, 0);
    int32_t temp_user_id =  static_cast<int32_t>(atoi(str_user_id.GetBuffer(0)));
    str_user_id.ReleaseBuffer();

    if(temp_user_id == product_id)
    {
      DeleteItem(i);
      break;        
    }
  }
}
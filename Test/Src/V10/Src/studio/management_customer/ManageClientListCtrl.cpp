// ManageClientListCtrl.cpp : implementation file
//

#include "../main_frame/StdAfx.h"
#include "ManageClientListCtrl.h"
#include "../common/UIDateTime.h"

using namespace wmdf;

// CManageClientListCtrl

IMPLEMENT_DYNAMIC(CManageClientListCtrl, CReportListCtrl)

CManageClientListCtrl::CManageClientListCtrl()
{

}

CManageClientListCtrl::~CManageClientListCtrl()
{
}


BEGIN_MESSAGE_MAP(CManageClientListCtrl, CReportListCtrl)
END_MESSAGE_MAP()



// CManageClientListCtrl message handlers

void CManageClientListCtrl::InsertUser(wmdf::User* pUser)
{
  // fill "m_conListUser" 
  int item_index = GetItemCount();
  int sub_item_index = 0;
  CString context;

  // add user_id_
  context.Format("%d", pUser->user_id());
  InsertItem(item_index, context);

  // add ip_address_
  context = pUser->ip_address().c_str();
  SetItemText(item_index, ++sub_item_index, context);

  // add user_name_
  context = pUser->user_name().c_str();
  SetItemText(item_index, ++sub_item_index, context);

  // add passwd_
  context = pUser->passwd().c_str();
  SetItemText(item_index, ++sub_item_index, context);

  // add company_
  context = pUser->company().c_str();
  SetItemText(item_index, ++sub_item_index, context);

  // add contacts_
  context = pUser->contacts().c_str();
  SetItemText(item_index, ++sub_item_index, context);

  // add phone_number_
  context = pUser->phone_number().c_str();
  SetItemText(item_index, ++sub_item_index, context);

  // add email_address_
  context = pUser->email_address().c_str();
  SetItemText(item_index, ++sub_item_index, context);

  if(pUser->is_privileged())
    context = "授权用户";
  else
    context = "测试用户";
  SetItemText(item_index, ++sub_item_index, context);

  context.Format("%d",pUser->authorization_deadline());
  SetItemText(item_index, ++sub_item_index, context);

  // 插入后排序
  Sort(0);  // 按id排序
}

void CManageClientListCtrl::DeleteUser(int32_t user_id)
{
   for(int i = 0; i < GetItemCount(); ++i)
   {
     CString str_user_id = GetItemText(i, 0);
     int32_t temp_user_id =  static_cast<int32_t>(atoi(str_user_id.GetBuffer(0)));
     str_user_id.ReleaseBuffer();

     if(temp_user_id == user_id)
     {
       DeleteItem(i);
       break;        
     }
   }
}
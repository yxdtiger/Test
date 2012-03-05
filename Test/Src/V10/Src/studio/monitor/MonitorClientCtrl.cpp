// D:\Projects\¸ßÆµ\WMDF\Src\ver10_2\src\studio\monitor\MonitorClientCtrl.cpp : implementation file
//

#include "../main_frame/StdAfx.h"
#include "MonitorClientCtrl.h"
#include "../../include/date_time.h"
#include "../../environment/strutils.h"
#include "../../environment/utils.h"
#include "monitor_environment.h"
using namespace wmdf;

// MonitorClientCtrl dialog

IMPLEMENT_DYNAMIC(CMonitorClientCtrl, CDialog)

CMonitorClientCtrl::CMonitorClientCtrl(CWnd* pParent /*=NULL*/)
	: CDialog(CMonitorClientCtrl::IDD, pParent)
{

}

CMonitorClientCtrl::~CMonitorClientCtrl()
{
}

void CMonitorClientCtrl::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CMonitorClientCtrl, CDialog)
  ON_BN_CLICKED(IDOK, &CMonitorClientCtrl::OnBnClickedOk)
  ON_WM_TIMER()
END_MESSAGE_MAP()


// MonitorClientCtrl message handlers

void CMonitorClientCtrl::OnBnClickedOk()
{
  // TODO: Add your control notification handler code here
  OnOK();
}
void CMonitorClientCtrl::OnTimer(UINT_PTR nIDEvent)
{
  // TODO: Add your message handler code here and/or call default
  m_client_ctrl.SetRedraw(FALSE);

  m_client_ctrl.DeleteAllItems();

  MdfRecord* record = NULL;
  ClientStatus* stat = NULL;
  int line = 0;  
  char pre_user_name[32];
  char pre_client_ip[15];
  for(int i=0;i<MonitorEnvironment::Instance()->server_manager()->server_info_count();i++)
  {
    memset(pre_user_name,0,32);
    memset(pre_client_ip,0,15);

    ConnectInfo* info = MonitorEnvironment::Instance()->server_manager()->server_infos(i);
    if(info->status()>0)
    {
      record = MonitorEnvironment::Instance()->GetClientStatusCopy(info->connect_id());
      if(record==NULL)
        continue;
      stat = (ClientStatus*)record->body;
      m_client_ctrl.ListInsertItem(line,StrUtils::formatstring(20,"%s:%d",info->ip().c_str(),info->port()).c_str());
      HTREEITEM fs = m_client_ctrl.GetItemOfRow(line++);      
      HTREEITEM client_ptr = NULL;
      HTREEITEM id_tree_item = NULL;
      for(int j = 0;j<record->record_count;j++)
      {         
        if(memcmp(pre_user_name,(char*)stat[j].user_name,32)!=0
          ||memcmp(pre_client_ip,(char*)stat[j].client_ip,15)!=0)
        {
          client_ptr = m_client_ctrl.InsertItem((char*)stat[j].user_name,2,2,fs);line++;
          m_client_ctrl.SetItemText(client_ptr,(char*)stat[j].client_ip,1);          
          memcpy(pre_user_name,(char*)stat[j].user_name,32);
          memcpy(pre_client_ip,(char*)stat[j].client_ip,15);
        }
        id_tree_item = m_client_ctrl.InsertItem(StrUtils::formatstring(10,"%d",stat[j].socket_id).c_str(),2,2,client_ptr);line++;     
        m_client_ctrl.SetItemText(id_tree_item,DateTime::time2hms(stat[j].duration,":").c_str(),2);
        m_client_ctrl.SetItemText(id_tree_item,StrUtils::formatstring(20,"%.2f",stat[j].send_data_flow).c_str(),3);
        m_client_ctrl.SetItemText(id_tree_item,StrUtils::formatstring(20,"%.2f",(double)stat[j].send_bytes/1024.0/1024.0).c_str(),4);
        m_client_ctrl.SetItemText(id_tree_item,StrUtils::formatstring(20,"%d",stat[j].msg_in_queue).c_str(),5);
        
        if(stat[j].product_count>0)
        {
          HTREEITEM item_ptr = m_client_ctrl.InsertItem("",2,2,id_tree_item);line++;          
          m_client_ctrl.SetItemText(item_ptr,"product id",1);
          m_client_ctrl.SetItemText(item_ptr,"major version",2);
          m_client_ctrl.SetItemText(item_ptr,"minor version",3);
          m_client_ctrl.SetItemText(item_ptr,"codec",4);
          for(int k = 0;k<stat[j].product_count;k++)
          {
            HTREEITEM item_ptr = m_client_ctrl.InsertItem("",2,2,id_tree_item);line++;            
            m_client_ctrl.SetItemText(item_ptr,StrUtils::formatstring(5,"%d",stat[j].product_infos[k].product_id).c_str(),1);
            m_client_ctrl.SetItemText(item_ptr,StrUtils::formatstring(5,"%d",stat[j].product_infos[k].major_version).c_str(),2);
            m_client_ctrl.SetItemText(item_ptr,StrUtils::formatstring(5,"%d",stat[j].product_infos[k].minor_version).c_str(),3);
            m_client_ctrl.SetItemText(item_ptr,StrUtils::formatstring(5,"%d",stat[j].product_infos[k].codec).c_str(),4);
          }
        }        
        m_client_ctrl.Expand(id_tree_item,TVE_EXPAND);
        m_client_ctrl.Expand(client_ptr,TVE_EXPAND);
      }
      m_client_ctrl.Expand(fs,TVE_EXPAND);
      Utils::DestroyMDFRecord(record);
    }
    
  }

  m_client_ctrl.SetRedraw(TRUE);
  m_client_ctrl.Invalidate();
  m_client_ctrl.UpdateWindow();
  CDialog::OnTimer(nIDEvent);
}

BOOL CMonitorClientCtrl::OnInitDialog()
{
  CDialog::OnInitDialog();

  CRect		sRect;
  CPoint		sPoint(0,0);
  GetDlgItem(IDC_Client_Ctrl)->GetWindowRect(&sRect);
  ClientToScreen(&sPoint);

  sRect.left	 -= sPoint.x;
  sRect.right	 -= sPoint.x;
  sRect.top	 -= sPoint.y;
  sRect.bottom -= sPoint.y;

  unsigned uStyle		= TVS_HASBUTTONS|TVS_HASBUTTONS|TVS_HASLINES|TVS_LINESATROOT|TVS_FULLROWSELECT;
  unsigned uExStyle	= TVS_EX_ITEMLINES|TVS_EX_ITEMLINES|TVS_EX_ALTERNATECOLOR|TVS_EX_SUBSELECT;
  m_client_ctrl.Create(uStyle|WS_CHILD|WS_VISIBLE|WS_BORDER,sRect,this,IDC_TREELIST);

  //m_client_list.SetImageList(&m_bmpImgList,TVSIL_NORMAL);
  m_client_ctrl.InsertColumn(0,"name",LVCFMT_CENTER,100);
  m_client_ctrl.InsertColumn(1,"ip",LVCFMT_CENTER,60);
  m_client_ctrl.InsertColumn(2,"duration",LVCFMT_CENTER,80);
  m_client_ctrl.InsertColumn(3,"flow(KB/S)",LVCFMT_CENTER,80);
  m_client_ctrl.InsertColumn(4,"send_size(MB)",LVCFMT_CENTER,80);
  m_client_ctrl.InsertColumn(5,"queued_msg",LVCFMT_CENTER,80);
  m_client_ctrl.SetExtendedStyle(uExStyle);
  m_client_ctrl.SetUserDataSize(USER_DATA_SIZE);
  SetTimer(1, 5000, NULL);
  return TRUE;
}

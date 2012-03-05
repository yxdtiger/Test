// D:\Projects\¸ßÆµ\WMDF\Src\ver10_2\src\studio\monitor\TreeTest.cpp : implementation file
//

#include "../main_frame/StdAfx.h"
#include "TreeTest.h"
#include "../../include/date_time.h"
#include "../../environment/strutils.h"
#include "../../environment/utils.h"
#include "monitor_environment.h"

// TreeTest dialog

using namespace wmdf;

IMPLEMENT_DYNAMIC(TreeTest, CDialog)

TreeTest::TreeTest(CWnd* pParent /*=NULL*/)
	: CDialog(TreeTest::IDD, pParent)
{

}

TreeTest::~TreeTest()
{
}

void TreeTest::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  DDX_Control(pDX, IDC_TREE1, m_client_ctrl);
}


BEGIN_MESSAGE_MAP(TreeTest, CDialog)
  ON_WM_TIMER()
END_MESSAGE_MAP()


// TreeTest message handlers

BOOL TreeTest::OnInitDialog()
{
  CDialog::OnInitDialog();

  CRect		sRect;
  CPoint		sPoint(0,0);
  GetDlgItem(IDC_TREE1)->GetWindowRect(&sRect);
  ClientToScreen(&sPoint);

  sRect.left	 -= sPoint.x;
  sRect.right	 -= sPoint.x;
  sRect.top	 -= sPoint.y;
  sRect.bottom -= sPoint.y;
  unsigned uStyle		= TVS_HASBUTTONS|TVS_HASBUTTONS|TVS_HASLINES|TVS_LINESATROOT|TVS_FULLROWSELECT;
  unsigned uExStyle	= TVS_EX_ITEMLINES|TVS_EX_ITEMLINES|TVS_EX_ALTERNATECOLOR|TVS_EX_SUBSELECT;
  //m_client_ctrl.Create(uStyle|WS_CHILD|WS_VISIBLE|WS_BORDER,sRect,this,IDC_TREE1);
  m_client_ctrl.SetStyle(uStyle|WS_CHILD|WS_VISIBLE|WS_BORDER);
  m_client_ctrl.InsertColumn(0,"name",LVCFMT_CENTER,100);
  m_client_ctrl.InsertColumn(1,"ip",LVCFMT_CENTER,60);
  m_client_ctrl.InsertColumn(2,"duration",LVCFMT_CENTER,80);
  m_client_ctrl.InsertColumn(3,"flow(KB/S)",LVCFMT_CENTER,80);
  m_client_ctrl.InsertColumn(4,"send_size(MB)",LVCFMT_CENTER,80);
  m_client_ctrl.InsertColumn(5,"queued_msg",LVCFMT_CENTER,80);
  m_client_ctrl.SetExtendedStyle(uExStyle);
  m_client_ctrl.SetUserDataSize(256);
  
  //SetTimer(1, 1000, NULL);

  return TRUE;  // return TRUE unless you set the focus to a control
  // EXCEPTION: OCX Property Pages should return FALSE
}

void TreeTest::OnTimer(UINT_PTR nIDEvent)
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

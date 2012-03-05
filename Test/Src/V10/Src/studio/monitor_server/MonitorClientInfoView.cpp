// MonitorClientInfoView.cpp : implementation file
//
#include "MonitorClientInfoView.h"
#include "../main_frame/StdAfx.h"
#include "../../environment/strutils.h"
#include "../../include/date_time.h"
#include "../main_frame/MainFrm.h"
#include "../monitor/monitor_environment.h"
#include "../../environment/utils.h"

// MonitorClientInfoView

IMPLEMENT_DYNCREATE(MonitorClientInfoView, CFormView)

MonitorClientInfoView::MonitorClientInfoView()
	: CFormView(MonitorClientInfoView::IDD)
{

}

MonitorClientInfoView::~MonitorClientInfoView()
{
}

void MonitorClientInfoView::DoDataExchange(CDataExchange* pDX)
{
  CFormView::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(MonitorClientInfoView, CFormView)
  ON_WM_DESTROY()
  ON_WM_TIMER()
  ON_WM_SIZE()
END_MESSAGE_MAP()



// MonitorClientInfoView diagnostics

#ifdef _DEBUG
void MonitorClientInfoView::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void MonitorClientInfoView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// MonitorClientInfoView message handlers

void MonitorClientInfoView::OnInitialUpdate()
{
  CFormView::OnInitialUpdate();

  // TODO: Add your specialized code here and/or call the base class
  GetParentFrame()->SetWindowText("客户端监控");

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

  m_client_ctrl.InsertColumn(0,"name",LVCFMT_CENTER,150);
  m_client_ctrl.InsertColumn(1,"ip",LVCFMT_CENTER,120);
  m_client_ctrl.InsertColumn(2,"duration",LVCFMT_CENTER,100);
  m_client_ctrl.InsertColumn(3,"flow(KB/S)",LVCFMT_CENTER,100);
  m_client_ctrl.InsertColumn(4,"send_size(MB)",LVCFMT_CENTER,100);
  m_client_ctrl.InsertColumn(5,"queued_msg",LVCFMT_CENTER,80);

  CFont font;
  font.CreateFont(14, 0, 0, 0, 10, FALSE, FALSE, FALSE, 0, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,   DEFAULT_QUALITY, DEFAULT_PITCH | FF_ROMAN, "System");
  //font.CreatePointFont(120,"宋体 ");
  m_client_ctrl.SetFont(&font);

  m_client_ctrl.SetExtendedStyle(uExStyle);
  m_client_ctrl.SetUserDataSize(USER_DATA_SIZE);

  
  SetTimer(1, 2000, NULL);
}

void MonitorClientInfoView::OnDestroy()
{
  CFormView::OnDestroy();

  // TODO: Add your message handler code here
  ((CMainFrame*)AfxGetMainWnd())->m_pMonitorClient=NULL; // 清空窗口指针
}


void MonitorClientInfoView::OnTimer(UINT_PTR nIDEvent)
{
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
        if(stat[j].msg_in_queue>0)
          m_client_ctrl.SetItemTextColor(id_tree_item,5,RGB(255,0,0));        
        if(stat[j].product_count>0)
        {
          HTREEITEM item_ptr = m_client_ctrl.InsertItem("product id",2,2,id_tree_item);line++;          
          
          m_client_ctrl.SetItemText(item_ptr,"major version",1);
          m_client_ctrl.SetItemText(item_ptr,"minor version",2);
          m_client_ctrl.SetItemText(item_ptr,"codec",3);
          for(int k = 0;k<stat[j].product_count;k++)
          {
            HTREEITEM item_ptr = m_client_ctrl.InsertItem(StrUtils::formatstring(5,"%d",stat[j].product_infos[k].product_id).c_str(),2,2,id_tree_item);line++;            
            m_client_ctrl.SetItemText(item_ptr,StrUtils::formatstring(5,"%d",stat[j].product_infos[k].major_version).c_str(),1);
            m_client_ctrl.SetItemText(item_ptr,StrUtils::formatstring(5,"%d",stat[j].product_infos[k].minor_version).c_str(),2);
            m_client_ctrl.SetItemText(item_ptr,StrUtils::formatstring(5,"%d",stat[j].product_infos[k].codec).c_str(),3);
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

  CFormView::OnTimer(nIDEvent);
}

void MonitorClientInfoView::OnSize(UINT nType, int cx, int cy)
{
  CFormView::OnSize(nType, cx, cy); 
  //m_client_ctrl.Invalidate(TRUE);
  /*重置LIST大小*/ 
  CWnd  *pList=GetDlgItem(IDC_TREELIST); 
  if(pList) 
  { 
    CRect  rect; 
    GetWindowRect(&rect);  //获取窗口rect，
    ScreenToClient(rect);  //从窗口尺寸转换到用户区rect
    pList->MoveWindow(&rect,true); 
  } 
  UpdateWindow();
}

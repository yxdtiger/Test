// MonitorClientInfoView.cpp : implementation file
//
#include "MonitorClientInfoView.h"
#include "../main_frame/StdAfx.h"
#include "../../environment/strutils.h"
#include "../../include/date_time.h"
#include "../main_frame/MainFrm.h"
#include "server_manager.h"
#include "model_id_factory.h"
#include "monitor_environment.h"
#include "../../environment/utils.h"
#include "../common/WordTranslation.h"

#define 	IDC_TREELIST	123

using namespace wmdf;

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

  m_client_ctrl.InsertColumn(0,"连接",LVCFMT_LEFT,200);
  m_client_ctrl.InsertColumn(1,"连接时长",LVCFMT_LEFT,120);
  m_client_ctrl.InsertColumn(2,"流量(KB/S)",LVCFMT_LEFT,100);
  m_client_ctrl.InsertColumn(3,"发送字节数(MB)",LVCFMT_LEFT,100);
  m_client_ctrl.InsertColumn(4,"拥塞包数",LVCFMT_LEFT,100);
  
  CFont font;
  font.CreateFont(20, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, GB2312_CHARSET, 
    OUT_DEFAULT_PRECIS,   CLIP_DEFAULT_PRECIS,   DEFAULT_QUALITY,   (_TCHAR)   (FF_SWISS   |   DEFAULT_PITCH   |   TMPF_TRUETYPE), "楷体_GB2312");
  m_client_ctrl.SetFont(&font);

  m_client_ctrl.SetExtendedStyle(uExStyle);
  m_client_ctrl.SetUserDataSize(256);

  UpdateClientView();
  SetTimer(1, 6000, NULL);
}

void MonitorClientInfoView::OnDestroy()
{
  CFormView::OnDestroy();
  ((CMainFrame*)AfxGetMainWnd())->m_pMonitorClient=NULL; // 清空窗口指针
}


void MonitorClientInfoView::OnTimer(UINT_PTR nIDEvent)
{
  UpdateClientView();
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

void MonitorClientInfoView::UpdateClientView()
{
  m_client_ctrl.SetRedraw(FALSE);

  int top_item_pos = m_client_ctrl.ListGetTopIndex();
  int selected_item_pos = m_client_ctrl.ListGetFirstSelected();
  int selected_column = m_client_ctrl.GetSelectionColumn();
  m_client_ctrl.DeleteAllItems();

  ClientStatus* stat = NULL;
  int line = 0;  
  char pre_user_name[32];
  char pre_client_ip[15];
  for(int i=0;i<ServerManager::Instance()->net_info_collection()->server_info_count();i++)
  {
    memset(pre_user_name,0,32);
    memset(pre_client_ip,0,15);

    ServerNetInfo* info = ServerManager::Instance()->net_info_collection()->server_infos(i);
    info->LockClientsStatus();

    m_client_ctrl.ListInsertItem(line,StrUtils::formatstring(20,"%s:%d",info->ip().c_str(),info->quote_port()).c_str());
    HTREEITEM fs = m_client_ctrl.GetItemOfRow(line++);    
    if(info->server_type()==0)
      m_client_ctrl.SetItemText(fs,"Feed Adapter",1);
    else
      m_client_ctrl.SetItemText(fs,"Feed Server",1);

    HTREEITEM client_ptr = NULL;
    HTREEITEM id_tree_item = NULL;
    for(int j = 0;j<info->client_status_count();j++)
    {     
      stat = info->client_status(j);
      if(memcmp(pre_user_name,(char*)stat->user_name,32)!=0
        ||memcmp(pre_client_ip,(char*)stat->client_ip,15)!=0)
      {
        client_ptr = m_client_ctrl.InsertItem((char*)stat->client_ip,2,2,fs);line++;
        m_client_ctrl.SetItemText(client_ptr,(char*)stat->user_name,1);          
        memcpy(pre_user_name,(char*)stat->user_name,32);
        memcpy(pre_client_ip,(char*)stat->client_ip,15);
      }
      id_tree_item = m_client_ctrl.InsertItem(StrUtils::formatstring(10,"%d",stat->socket_id).c_str(),2,2,client_ptr);line++;     
      m_client_ctrl.SetItemText(id_tree_item,DateTime::time2hms(stat->duration,":").c_str(),1);
      m_client_ctrl.SetItemText(id_tree_item,StrUtils::formatstring(20,"%.2f",stat->send_data_flow).c_str(),2);
      m_client_ctrl.SetItemText(id_tree_item,StrUtils::formatstring(20,"%.2f",(double)stat->send_bytes/1024.0/1024.0).c_str(),3);
      m_client_ctrl.SetItemText(id_tree_item,StrUtils::formatstring(20,"%d",stat->msg_in_queue).c_str(),4);
      if(stat->msg_in_queue>0)
        m_client_ctrl.SetItemTextColor(id_tree_item,4,RGB(255,0,0));    
      string key = ModelIDFactory::GetServer2ClientSocket(info->server_id(),stat->socket_id);
      SymbolModel* model = info->GetSymbolModel(key,SST_Client);
      if(NULL != model)
      {
        if(model->symbol_status&SMP_NoSend)
        {
          m_client_ctrl.SetItemTextColor(id_tree_item,3,RGB(255,0,0));
        }
      }
      if(stat->product_count>0)
      {
        HTREEITEM item_ptr = m_client_ctrl.InsertItem("product id",2,2,id_tree_item);        
        m_client_ctrl.SetItemText(item_ptr,"major version",1);
        m_client_ctrl.SetItemText(item_ptr,"minor version",2);
        m_client_ctrl.SetItemText(item_ptr,"codec",3);
        for(int k = 0;k<stat->product_count;k++)
        {
          HTREEITEM item_ptr = m_client_ctrl.InsertItem(CWordTranslation::TranslationProductID(stat->product_infos[k].product_id).c_str(),2,2,id_tree_item);
          m_client_ctrl.SetItemText(item_ptr,StrUtils::formatstring(5,"%d",stat->product_infos[k].major_version).c_str(),1);
          m_client_ctrl.SetItemText(item_ptr,StrUtils::formatstring(5,"%d",stat->product_infos[k].minor_version).c_str(),2);
          m_client_ctrl.SetItemText(item_ptr,StrUtils::formatstring(5,"%d",stat->product_infos[k].codec).c_str(),3);
        }
      }
      m_client_ctrl.Expand(client_ptr,TVE_EXPAND);
    }
    m_client_ctrl.Expand(fs,TVE_EXPAND);
    info->UnlockClientsStatus();
  }

  m_client_ctrl.SetRedraw(TRUE);
  m_client_ctrl.ListSetTopIndex(top_item_pos);
  m_client_ctrl.ListSelectItem(selected_item_pos,selected_column);
  m_client_ctrl.Invalidate();
}

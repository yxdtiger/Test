// MonitorTransferInfoView.cpp : implementation file
//
#include "MonitorTransferInfoView.h"
#include "../main_frame/StdAfx.h"
#include "../../environment/strutils.h"
#include "../../include/date_time.h"
#include "../main_frame/MainFrm.h"
#include "../monitor/monitor_environment.h"
#include "../../environment/utils.h"

// MonitorTransferInfoView

IMPLEMENT_DYNCREATE(MonitorTransferInfoView, CFormView)

MonitorTransferInfoView::MonitorTransferInfoView()
	: CFormView(MonitorTransferInfoView::IDD)
{

}

MonitorTransferInfoView::~MonitorTransferInfoView()
{
}

void MonitorTransferInfoView::DoDataExchange(CDataExchange* pDX)
{
  CFormView::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(MonitorTransferInfoView, CFormView)
  ON_WM_DESTROY()
  ON_WM_TIMER()
  ON_WM_SIZE()
END_MESSAGE_MAP()


// MonitorTransferInfoView diagnostics

#ifdef _DEBUG
void MonitorTransferInfoView::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void MonitorTransferInfoView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// MonitorTransferInfoView message handlers

void MonitorTransferInfoView::OnInitialUpdate()
{
  CFormView::OnInitialUpdate();

  // TODO: Add your specialized code here and/or call the base class
  GetParentFrame()->SetWindowText("传输监控");

  CRect		sRect;
  CPoint		sPoint(0,0);
  GetDlgItem(IDC_TRANSFER_CTRL)->GetWindowRect(&sRect);
  ClientToScreen(&sPoint);

  sRect.left	 -= sPoint.x;
  sRect.right	 -= sPoint.x;
  sRect.top	 -= sPoint.y;
  sRect.bottom -= sPoint.y;

  unsigned uStyle		= TVS_HASBUTTONS|TVS_HASBUTTONS|TVS_HASLINES|TVS_LINESATROOT|TVS_FULLROWSELECT;
  unsigned uExStyle	= TVS_EX_ITEMLINES|TVS_EX_ITEMLINES|TVS_EX_ALTERNATECOLOR|TVS_EX_SUBSELECT;
  m_transfer_ctrl.Create(uStyle|WS_CHILD|WS_VISIBLE|WS_BORDER,sRect,this,IDC_TRANSFER_TREELIST);

  
  
  //m_client_list.SetImageList(&m_bmpImgList,TVSIL_NORMAL);
  m_transfer_ctrl.InsertColumn(0,"server/product_id",LVCFMT_CENTER,150);
  /*m_transfer_ctrl.InsertColumn(1,"product_id",LVCFMT_CENTER,60);*/
  m_transfer_ctrl.InsertColumn(1,"delay(ms)",LVCFMT_CENTER,80);
  m_transfer_ctrl.InsertColumn(2,"r_flow(KB/S)",LVCFMT_CENTER,100);
  m_transfer_ctrl.InsertColumn(3,"s_flow(KB/S)",LVCFMT_CENTER,100);
  m_transfer_ctrl.InsertColumn(4,"r_size(MB)",LVCFMT_CENTER,100);
  m_transfer_ctrl.InsertColumn(5,"s_size(MB)",LVCFMT_CENTER,100);
  m_transfer_ctrl.InsertColumn(6,"compress(%)",LVCFMT_CENTER,80);
  m_transfer_ctrl.InsertColumn(7,"r_total_pack",LVCFMT_CENTER,100);
  m_transfer_ctrl.InsertColumn(8,"s_total_pack",LVCFMT_CENTER,100);

  CFont font;
  font.CreateFont(14, 0, 0, 0, 10, FALSE, FALSE, FALSE, 0, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,   DEFAULT_QUALITY, DEFAULT_PITCH | FF_ROMAN, "System");
  //font.CreatePointFont(120,"宋体 ");
  m_transfer_ctrl.SetFont(&font);
  
  m_transfer_ctrl.SetExtendedStyle(uExStyle);
  m_transfer_ctrl.SetUserDataSize(256);
  SetTimer(1, 2000, NULL);
}

void MonitorTransferInfoView::OnDestroy()
{
  CFormView::OnDestroy();

  // TODO: Add your message handler code here
  ((CMainFrame*)AfxGetMainWnd())->m_pMonitorTransfer=NULL; // 清空窗口指针
}


void MonitorTransferInfoView::OnTimer(UINT_PTR nIDEvent)
{
  m_transfer_ctrl.SetRedraw(FALSE);

  m_transfer_ctrl.DeleteAllItems();
  MdfRecord* record = NULL;
  TransferDataStat* stat = NULL; 
  int line = 0;
  for(int i=0;i<MonitorEnvironment::Instance()->server_manager()->server_info_count();i++,line++)
  {
    ConnectInfo* info = MonitorEnvironment::Instance()->server_manager()->server_infos(i);
    m_transfer_ctrl.ListInsertItem(line,StrUtils::formatstring(20,"%s:%d",info->ip().c_str(),info->port()).c_str());
    if(info->status()>0)
    {
      record = MonitorEnvironment::Instance()->GetTransferDataStatsCopy(info->connect_id());
      if(record==NULL)
        continue;
      stat = (TransferDataStat*)record->body;
      HTREEITEM fs = m_transfer_ctrl.GetItemOfRow(line);
      for(int j = 0;j<record->record_count;j++,line++)
      {
        HTREEITEM item_ptr = m_transfer_ctrl.InsertItem(StrUtils::formatstring(10,"%d",stat[j].product_id).c_str(),2,2,fs);        
        m_transfer_ctrl.SetItemText(item_ptr,StrUtils::formatstring(30,"%d(%d/%d)",stat[j].msg_avg_delay,stat[j].delayed_msg_delta,stat[j].send_packet_delta).c_str(),1);
        m_transfer_ctrl.SetItemText(item_ptr,StrUtils::formatstring(10,"%.2f",stat[j].receive_data_flow).c_str(),2);
        m_transfer_ctrl.SetItemText(item_ptr,StrUtils::formatstring(10,"%.2f",stat[j].send_data_flow).c_str(),3);
        m_transfer_ctrl.SetItemText(item_ptr,StrUtils::formatstring(10,"%.2f",stat[j].receive_data_size/(1024.0*1024.0)).c_str(),4);
        m_transfer_ctrl.SetItemText(item_ptr,StrUtils::formatstring(10,"%.2f",stat[j].encoded_size/(1024.0*1024.0)).c_str(),5);
        m_transfer_ctrl.SetItemText(item_ptr,StrUtils::formatstring(10,"%.2f",stat[j].compression_ratio).c_str(),6); 
        m_transfer_ctrl.SetItemText(item_ptr,StrUtils::formatstring(10,"%d",stat[j].receive_packet_count).c_str(),7);   
        m_transfer_ctrl.SetItemText(item_ptr,StrUtils::formatstring(10,"%d",stat[j].send_packet_count).c_str(),8);
      }      
      m_transfer_ctrl.Expand(fs,TVE_EXPAND);
      Utils::DestroyMDFRecord(record);
    }
  }


  m_transfer_ctrl.SetRedraw(TRUE);
  m_transfer_ctrl.Invalidate();
  m_transfer_ctrl.UpdateWindow();


  CFormView::OnTimer(nIDEvent);
}

void MonitorTransferInfoView::OnSize(UINT nType, int cx, int cy)
{
  CFormView::OnSize(nType, cx, cy);
  /*重置LIST大小*/ 
  CWnd  *pList=GetDlgItem(IDC_TRANSFER_TREELIST); 
  if(pList) 
  { 
    CRect  rect; 
    GetWindowRect(&rect);  //获取窗口rect，
    ScreenToClient(rect);  //从窗口尺寸转换到用户区rect
    pList->MoveWindow(&rect,true); 
  } 
}

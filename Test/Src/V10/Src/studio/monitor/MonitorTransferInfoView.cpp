// MonitorTransferInfoView.cpp : implementation file
//
#include "MonitorTransferInfoView.h"
#include "../main_frame/StdAfx.h"
#include "../../environment/strutils.h"
#include "../../environment/utils.h"
#include "../../include/date_time.h"
#include "../main_frame/MainFrm.h"
#include "server_manager.h"
#include "model_id_factory.h"
#include "monitor_environment.h"
#include "../common/WordTranslation.h"
#define 	IDC_TRANSFER_TREELIST	123
using namespace wmdf;
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
  ON_NOTIFY(NM_DBLCLK, IDC_TRANSFER_TREELIST, &MonitorTransferInfoView::OnNMDClick)
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
  m_transfer_ctrl.InsertColumn(0,"数据源",LVCFMT_CENTER,200);
  /*m_transfer_ctrl.InsertColumn(1,"product_id",LVCFMT_CENTER,60);*/
  m_transfer_ctrl.InsertColumn(1,"延迟(ms)",LVCFMT_CENTER,100);
  m_transfer_ctrl.InsertColumn(2,"接收流量(KB/S)",LVCFMT_CENTER,100);
  m_transfer_ctrl.InsertColumn(3,"发送流量(KB/S)",LVCFMT_CENTER,100);
  m_transfer_ctrl.InsertColumn(4,"接收字节数(MB)",LVCFMT_CENTER,100);
  m_transfer_ctrl.InsertColumn(5,"发送字节数(MB)",LVCFMT_CENTER,100);
  m_transfer_ctrl.InsertColumn(6,"压缩率(%)",LVCFMT_CENTER,80);
  m_transfer_ctrl.InsertColumn(7,"接收总包数",LVCFMT_CENTER,100);
  m_transfer_ctrl.InsertColumn(8,"发送总包数",LVCFMT_CENTER,100);

  CFont font;
  //font.CreateFont(20, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, GB2312_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,   DEFAULT_QUALITY, DEFAULT_PITCH | FF_ROMAN, "楷体_GB2312");
  font.CreateFont(20, 0, 0, 0, FW_BOLD ,0, 1, 0, GB2312_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH ,"楷体_GB2312");

  //font.CreatePointFont(120,"宋体 ");
  m_transfer_ctrl.SetFont(&font);
  
  m_transfer_ctrl.SetExtendedStyle(uExStyle);
  m_transfer_ctrl.SetUserDataSize(256);

  UpdateTransferView();
  SetTimer(1, 7000, NULL);
}

void MonitorTransferInfoView::OnDestroy()
{
  CFormView::OnDestroy();

  // TODO: Add your message handler code here
  ((CMainFrame*)AfxGetMainWnd())->m_pMonitorTransfer=NULL; // 清空窗口指针
}


void MonitorTransferInfoView::OnTimer(UINT_PTR nIDEvent)
{
  UpdateTransferView();
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

void MonitorTransferInfoView::OnNMDClick( NMHDR *pNMHDR, LRESULT *pResult )
{
  m_transfer_ctrl.RedrawWindow();
}

void MonitorTransferInfoView::UpdateTransferView()
{
  m_transfer_ctrl.SetRedraw(FALSE);

  int top_item_pos = m_transfer_ctrl.ListGetTopIndex();
  int selected_item_pos = m_transfer_ctrl.ListGetFirstSelected();
  int selected_column = m_transfer_ctrl.GetSelectionColumn();
  m_transfer_ctrl.DeleteAllItems();

  TransferDataStat* stat = NULL; 
  int line = 0;
  for(int i=0;i<ServerManager::Instance()->net_info_collection()->server_info_count();i++,line++)
  {
    ServerNetInfo* info = ServerManager::Instance()->net_info_collection()->server_infos(i);
    info->LockTransfersStatus();
    m_transfer_ctrl.ListInsertItem(line,StrUtils::formatstring(20,"%s:%d",info->ip().c_str(),info->quote_port()).c_str());
    HTREEITEM fs = m_transfer_ctrl.GetItemOfRow(line);
    if(info->server_type()==0)
      m_transfer_ctrl.SetItemText(fs,"Feed Adapter",1);
    else
      m_transfer_ctrl.SetItemText(fs,"Feed Server",1);


    for(int j = 0;j<info->transfer_status_count();j++,line++)
    {
      stat = info->transfer_status(j);
      string key = ModelIDFactory::GetServer2SourcePort(info->server_id(),(char*)stat->source_ip,stat->source_port);
      HTREEITEM item_ptr = m_transfer_ctrl.InsertItem(CWordTranslation::TranslationProductID(stat->product_id).c_str(),2,2,fs);        
      m_transfer_ctrl.SetItemText(item_ptr,StrUtils::formatstring(30,"%d(%d/%d)",stat->msg_avg_delay,stat->delayed_msg_delta,stat->send_packet_delta).c_str(),1);
      m_transfer_ctrl.SetItemText(item_ptr,StrUtils::formatstring(20,"%.2f",stat->receive_data_flow).c_str(),2);
      m_transfer_ctrl.SetItemText(item_ptr,StrUtils::formatstring(20,"%.2f",stat->send_data_flow).c_str(),3);
      m_transfer_ctrl.SetItemText(item_ptr,StrUtils::formatstring(20,"%.2f",stat->receive_data_size/(1024.0*1024.0)).c_str(),4);
      m_transfer_ctrl.SetItemText(item_ptr,StrUtils::formatstring(20,"%.2f",stat->encoded_size/(1024.0*1024.0)).c_str(),5);
      m_transfer_ctrl.SetItemText(item_ptr,StrUtils::formatstring(20,"%.2f",stat->compression_ratio).c_str(),6); 
      m_transfer_ctrl.SetItemText(item_ptr,StrUtils::formatstring(20,"%d",stat->receive_packet_count).c_str(),7);   
      m_transfer_ctrl.SetItemText(item_ptr,StrUtils::formatstring(20,"%d",stat->send_packet_count).c_str(),8);

      if(stat->msg_avg_delay>0)
        m_transfer_ctrl.SetItemTextColor(item_ptr,1,RGB(255,0,0));
      SymbolModel* model = info->GetSymbolModel(key,SST_Source);
      if(NULL != model)
      {         
        if(model->symbol_status&SMP_NoReceive)
        {
          m_transfer_ctrl.SetItemTextColor(item_ptr,4,RGB(255,0,0));
        }
        if(model->symbol_status&SMP_NoSend)
        {
          m_transfer_ctrl.SetItemTextColor(item_ptr,5,RGB(255,0,0));
        }          
      }
    }      
    m_transfer_ctrl.Expand(fs,TVE_EXPAND);
    info->UnlockTransfersStatus();
  }


  m_transfer_ctrl.SetRedraw(TRUE);
  m_transfer_ctrl.ListSetTopIndex(top_item_pos);
  m_transfer_ctrl.ListSelectItem(selected_item_pos,selected_column);
  m_transfer_ctrl.Invalidate();
}

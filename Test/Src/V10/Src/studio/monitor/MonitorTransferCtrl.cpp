// D:\Projects\¸ßÆµ\WMDF\Src\ver10_2\src\studio\monitor\MonitorTransferCtrl.cpp : implementation file
//

#include "../main_frame/StdAfx.h"
#include "MonitorTransferCtrl.h"
#include "monitor_environment.h"
#include "../../environment/strutils.h"
#include "../../environment/utils.h"

// CMonitorTransferCtrl dialog

IMPLEMENT_DYNAMIC(CMonitorTransferCtrl, CDialog)
using namespace wmdf;
CMonitorTransferCtrl::CMonitorTransferCtrl(CWnd* pParent /*=NULL*/)
	: CDialog(CMonitorTransferCtrl::IDD, pParent)
{
 
}

CMonitorTransferCtrl::~CMonitorTransferCtrl()
{
}

void CMonitorTransferCtrl::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CMonitorTransferCtrl, CDialog)
  ON_BN_CLICKED(IDOK, &CMonitorTransferCtrl::OnBnClickedOk)
  ON_WM_TIMER()
END_MESSAGE_MAP()



void CMonitorTransferCtrl::OnBnClickedOk()
{
  // TODO: Add your control notification handler code here
  OnOK();
}



void CMonitorTransferCtrl::OnTimer(UINT_PTR nIDEvent)
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
        HTREEITEM item_ptr = m_transfer_ctrl.InsertItem(StrUtils::formatstring(10,"%d",info->connect_id()).c_str(),2,2,fs);
        m_transfer_ctrl.SetItemText(item_ptr,StrUtils::formatstring(10,"%d",stat[j].product_id).c_str(),1);   
        m_transfer_ctrl.SetItemText(item_ptr,StrUtils::formatstring(30,"%d(%d/%d)",stat[j].msg_avg_delay,stat[j].delayed_msg_delta,stat[j].send_packet_delta).c_str(),2);
        m_transfer_ctrl.SetItemText(item_ptr,StrUtils::formatstring(10,"%.2f",stat[j].receive_data_flow).c_str(),3);   
        m_transfer_ctrl.SetItemText(item_ptr,StrUtils::formatstring(10,"%.2f",stat[j].send_data_flow).c_str(),4);   
        m_transfer_ctrl.SetItemText(item_ptr,StrUtils::formatstring(10,"%.2f",stat[j].receive_data_size/(1024.0*1024.0)).c_str(),5);
        m_transfer_ctrl.SetItemText(item_ptr,StrUtils::formatstring(10,"%.2f",stat[j].encoded_size/(1024.0*1024.0)).c_str(),6);
        m_transfer_ctrl.SetItemText(item_ptr,StrUtils::formatstring(10,"%.2f",stat[j].compression_ratio).c_str(),7);   
        m_transfer_ctrl.SetItemText(item_ptr,StrUtils::formatstring(10,"%d",stat[j].receive_packet_count).c_str(),8);   
        m_transfer_ctrl.SetItemText(item_ptr,StrUtils::formatstring(10,"%d",stat[j].send_packet_count).c_str(),9);
      }
      m_transfer_ctrl.Expand(fs,TVE_EXPAND);
      Utils::DestroyMDFRecord(record);
    }
  }


  m_transfer_ctrl.SetRedraw(TRUE);
  m_transfer_ctrl.Invalidate();
  m_transfer_ctrl.UpdateWindow();

  CDialog::OnTimer(nIDEvent);
}

BOOL CMonitorTransferCtrl::OnInitDialog()
{
  CDialog::OnInitDialog();

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
  m_transfer_ctrl.InsertColumn(0,"server:port",LVCFMT_CENTER,100);
  m_transfer_ctrl.InsertColumn(1,"product_id",LVCFMT_CENTER,60);
  m_transfer_ctrl.InsertColumn(2,"delay(ms)",LVCFMT_CENTER,60);
  m_transfer_ctrl.InsertColumn(3,"r_flow(KB/S)",LVCFMT_CENTER,80);
  m_transfer_ctrl.InsertColumn(4,"s_flow(KB/S)",LVCFMT_CENTER,80);
  m_transfer_ctrl.InsertColumn(5,"r_size(MB)",LVCFMT_CENTER,80);
  m_transfer_ctrl.InsertColumn(6,"s_size(MB)",LVCFMT_CENTER,80);
  m_transfer_ctrl.InsertColumn(7,"compress(%)",LVCFMT_CENTER,80);
  m_transfer_ctrl.InsertColumn(8,"r_total_pack",LVCFMT_CENTER,80);
  m_transfer_ctrl.InsertColumn(9,"s_total_pack",LVCFMT_CENTER,80);
  m_transfer_ctrl.SetExtendedStyle(uExStyle);
  m_transfer_ctrl.SetUserDataSize(256);
  SetTimer(1, 1000, NULL);
  return TRUE;
}

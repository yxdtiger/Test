// LogBrowserView.cpp : implementation file
//
#include "LogBrowserView.h"
#include "../main_frame/StdAfx.h"
#include "../main_frame/MainFrm.h"
#include "../../include/wmdf_structs.h"
#include "../../include/date_time.h"
#include "../../environment/strutils.h"
#include "../common/studio_utility.h"
#include "server_manager.h"

#define 	IDC_LOG_TREELIST	124
// LogBrowserView

IMPLEMENT_DYNCREATE(LogBrowserView, CFormView)

using namespace wmdf;
using namespace std;
LogBrowserView::LogBrowserView()
	: CFormView(LogBrowserView::IDD)
{

}

LogBrowserView::~LogBrowserView()
{
}

void LogBrowserView::DoDataExchange(CDataExchange* pDX)
{
  CFormView::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(LogBrowserView, CFormView)
  ON_WM_DESTROY()
  ON_WM_SIZE()
  ON_WM_TIMER()
END_MESSAGE_MAP()


// LogBrowserView diagnostics

#ifdef _DEBUG
void LogBrowserView::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void LogBrowserView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// LogBrowserView message handlers

void LogBrowserView::OnInitialUpdate()
{
  CFormView::OnInitialUpdate();

  // TODO: Add your specialized code here and/or call the base class
  GetParentFrame()->SetWindowText("日志查看器");

  CRect		sRect;
  CPoint		sPoint(0,0);
  GetDlgItem(IDC_LOG_BACKGROUND)->GetWindowRect(&sRect);
  ClientToScreen(&sPoint);

  sRect.left	 -= sPoint.x;
  sRect.right	 -= sPoint.x;
  sRect.top	 -= sPoint.y;
  sRect.bottom -= sPoint.y;

  unsigned uStyle		= TVS_HASBUTTONS|TVS_HASBUTTONS|TVS_HASLINES|TVS_LINESATROOT|TVS_FULLROWSELECT;
  unsigned uExStyle	= TVS_EX_ITEMLINES|TVS_EX_ITEMLINES|TVS_EX_ALTERNATECOLOR|TVS_EX_SUBSELECT;
  m_log_ctrl.Create(uStyle|WS_CHILD|WS_VISIBLE|WS_BORDER,sRect,this,IDC_LOG_TREELIST);

  
  
  m_log_ctrl.InsertColumn(0,"服务器/时间",LVCFMT_LEFT,250);
  m_log_ctrl.InsertColumn(1,"事件级别",LVCFMT_LEFT,100);
  m_log_ctrl.InsertColumn(2,"事件ID",LVCFMT_LEFT,100);
  m_log_ctrl.InsertColumn(3,"类别",LVCFMT_CENTER,100);
  m_log_ctrl.InsertColumn(4,"明细",LVCFMT_LEFT,300);

  CFont font;
  font.CreateFont(14, 0, 0, 0, 10, FALSE, FALSE, FALSE, 0, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,   DEFAULT_QUALITY, DEFAULT_PITCH | FF_ROMAN, "System");
  //font.CreatePointFont(120,"宋体 ");
  m_log_ctrl.SetFont(&font);
  
  m_log_ctrl.SetExtendedStyle(uExStyle);
  m_log_ctrl.SetUserDataSize(256);

  UpdateLogView();
  SetTimer(1, 5000, NULL);
}

void LogBrowserView::OnDestroy()
{
  CFormView::OnDestroy();

  // TODO: Add your message handler code here
  ((CMainFrame*)AfxGetMainWnd())->m_pMonitorLog=NULL; // 清空窗口指针
}



void LogBrowserView::OnSize(UINT nType, int cx, int cy)
{
  CFormView::OnSize(nType, cx, cy);
  /*重置LIST大小*/ 
  CWnd  *pList=GetDlgItem(IDC_LOG_TREELIST); 
  if(pList) 
  { 
    CRect  rect; 
    GetWindowRect(&rect);  //获取窗口rect，
    ScreenToClient(rect);  //从窗口尺寸转换到用户区rect
    pList->MoveWindow(&rect,true); 
  } 
}

//相应双击事件
void LogBrowserView::OnNMDClick( NMHDR *pNMHDR, LRESULT *pResult )
{
  //KillTimer(1);
  //int line = m_log_ctrl.ListGetFirstSelected();
  //if(line>=0)
  //{ 
  //  LogDetailDialog dlg;
  //  dlg.DoModal();
  //}
  //SetTimer(1,5000,NULL);
}



void LogBrowserView::OnTimer(UINT_PTR nIDEvent)
{
  UpdateLogView();
  CFormView::OnTimer(nIDEvent);
}

void LogBrowserView::UpdateLogView()
{
  m_log_ctrl.SetRedraw(FALSE);

  /*pos  =  m_log_ctrl.GetScrollPos(1);*/
  int top_item_pos = m_log_ctrl.ListGetTopIndex();
  int selected_item_pos = m_log_ctrl.ListGetFirstSelected();
  int selected_column = m_log_ctrl.GetSelectionColumn();
  m_log_ctrl.DeleteAllItems();
  MonitorEvent* monitor_event = NULL; 
  int line = 0;
  for(int i=0;i<ServerManager::Instance()->net_info_collection()->server_info_count();i++,line++)
  {
    ServerNetInfo* info = ServerManager::Instance()->net_info_collection()->server_infos(i);
    info->LockMonitorEvent();
    m_log_ctrl.ListInsertItem(line,StrUtils::formatstring(20,"%s:%d",info->ip().c_str(),info->quote_port()).c_str());
    HTREEITEM fs = m_log_ctrl.GetItemOfRow(line);
    if(info->server_type()==0)
      m_log_ctrl.SetItemText(fs,"Feed Adapter",1);
    else
      m_log_ctrl.SetItemText(fs,"Feed Server",1);

    for(int j = 0;j<info->monitor_events_count();j++,line++)
    {
      monitor_event = info->monitor_events(j);      
      HTREEITEM item_ptr = m_log_ctrl.InsertItem(DateTime::datetime2str(monitor_event->date_time).c_str(),2,2,fs);
      m_log_ctrl.SetItemText(item_ptr,wmdf::Log_Level_Str[monitor_event->level].c_str(),1);
      m_log_ctrl.SetItemText(item_ptr,StrUtils::formatstring(10,"%d",monitor_event->event_id).c_str(),2);
      m_log_ctrl.SetItemText(item_ptr,(char*)monitor_event->at_where,3);
      m_log_ctrl.SetItemText(item_ptr,(char*)monitor_event->what_happened,4);
      if(monitor_event->level<=2)
      {
        m_log_ctrl.SetItemTextColor(item_ptr,0,RGB(255,0,0));
        m_log_ctrl.SetItemTextColor(item_ptr,1,RGB(255,0,0));
        m_log_ctrl.SetItemTextColor(item_ptr,2,RGB(255,0,0));
        m_log_ctrl.SetItemTextColor(item_ptr,3,RGB(255,0,0));
        m_log_ctrl.SetItemTextColor(item_ptr,4,RGB(255,0,0));
      }
    }
    m_log_ctrl.Expand(fs,TVE_EXPAND);
    info->UnlockMonitorEvent();
  }
  m_log_ctrl.SetRedraw(TRUE);

  m_log_ctrl.ListSetTopIndex(top_item_pos);
  m_log_ctrl.ListSelectItem(selected_item_pos,selected_column);

  m_log_ctrl.Invalidate();
}



// MonitorServerView.cpp : implementation file
//
#include "MonitorServerView.h"
#include <string>
#include "../main_frame/StdAfx.h"
#include "../main_frame/WMDF_Studio.h"
#include "../../environment/strutils.h"
#include "../common/WordTranslation.h"
#include "../main_frame/MainFrm.h"
#include "../monitor/monitor_environment.h"
#include "../monitor/AddServerDialog.h"

// MonitorServerView

IMPLEMENT_DYNCREATE(MonitorServerView, CFormView)

MonitorServerView::MonitorServerView()
	: CFormView(MonitorServerView::IDD)
{
  invoker_ = new WmdfInvoke();
  monitor_client_dlg_ = NULL;
  monitor_transfer_dlg_ = NULL;
}

MonitorServerView::~MonitorServerView()
{
  if(invoker_!=NULL)
  {
    invoker_->Disconnect();
    delete invoker_;
    invoker_ = NULL;
  }  
  delete MonitorEnvironment::Instance();
}

void MonitorServerView::DoDataExchange(CDataExchange* pDX)
{
  CFormView::DoDataExchange(pDX);
  DDX_Control(pDX, IDC_Server_LIST, m_server_list);
}

BEGIN_MESSAGE_MAP(MonitorServerView, CFormView)
  ON_WM_DESTROY()
  ON_NOTIFY(NM_RCLICK, IDC_Server_LIST, &MonitorServerView::OnNMRClickServerList)
  ON_COMMAND(ID_Transfer_Monitor, &MonitorServerView::OnTransferMonitor)
  ON_COMMAND(ID_Client_Monitor, &MonitorServerView::OnClientMonitor)
  ON_BN_CLICKED(ID_BUTTON_CONNECT_ALL, &MonitorServerView::OnConnectAll)
  ON_BN_CLICKED(ID_BUTTON_ADD_SERVER, &MonitorServerView::OnAddServer)
  ON_BN_CLICKED(ID_BUTTON_DELETE_SERVER, &MonitorServerView::OnDeleteSelectedServer)
  ON_BN_CLICKED(ID_BUTTON_MONITE_TRANSFER, &MonitorServerView::OnTransferMonitor)
  ON_BN_CLICKED(ID_BUTTON_MONITE_CLIENTS, &MonitorServerView::OnClientMonitor)
  ON_WM_SIZE()
  ON_WM_SIZING()
  ON_WM_PAINT()
END_MESSAGE_MAP()


// MonitorServerView diagnostics

#ifdef _DEBUG
void MonitorServerView::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void MonitorServerView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// MonitorServerView message handlers

void MonitorServerView::OnInitialUpdate()
{
  CFormView::OnInitialUpdate();

  if( !m_editor.m_hWnd )
  {
    CRect rect;
    GetClientRect( rect );
    rect.left += 250;
    GetData()->SetClipboardHandler( &theApp.m_clip );
    m_editor.Create( WS_CHILD | WS_VISIBLE, rect, this, GetData() );

    m_editor.SetModified( FALSE );  
  }
  else
    m_editor.Clear();  

  // TODO: Add your specialized code here and/or call the base class
  GetParentFrame()->SetWindowText("服务监控");
  MonitorEnvironment::Instance()->Initialize();

  m_server_list.InsertColumn(0,"ip地址");
  m_server_list.InsertColumn(1,"端口");
  m_server_list.InsertColumn(2,"连接ID");
  m_server_list.InsertColumn(3,"连接状态");
  m_server_list.SetColumnWidth( 0 , LVSCW_AUTOSIZE_USEHEADER);
  m_server_list.SetColumnWidth( 1 , LVSCW_AUTOSIZE_USEHEADER); 
  m_server_list.SetColumnWidth( 2 , LVSCW_AUTOSIZE_USEHEADER); 
  m_server_list.SetColumnWidth( 3 , LVSCW_AUTOSIZE_USEHEADER);
  
  m_server_list.SetExtendedStyle(LVS_EX_GRIDLINES|LVS_EX_FULLROWSELECT);

  ReloadServerList();

  m_toolBarImageList.Create(16, 16, TRUE | ILC_COLOR8, 5, 0);
  HICON hIcon = NULL;

  hIcon = (HICON)::LoadImage(::AfxGetInstanceHandle(), 
    MAKEINTRESOURCE(IDI_ICON1), IMAGE_ICON, 16, 16, 0);
  m_toolBarImageList.Add(hIcon);

  hIcon = (HICON)::LoadImage(::AfxGetInstanceHandle(), 
    MAKEINTRESOURCE(IDI_ICON2), IMAGE_ICON, 16, 16, 0);
  m_toolBarImageList.Add(hIcon);

  hIcon = (HICON)::LoadImage(::AfxGetInstanceHandle(), 
    MAKEINTRESOURCE(IDI_ICON3), IMAGE_ICON, 16, 16, 0);
  m_toolBarImageList.Add(hIcon);

  hIcon = (HICON)::LoadImage(::AfxGetInstanceHandle(), 
    MAKEINTRESOURCE(IDI_ICON4), IMAGE_ICON, 16, 16, 0);
  m_toolBarImageList.Add(hIcon);

  hIcon = (HICON)::LoadImage(::AfxGetInstanceHandle(), 
    MAKEINTRESOURCE(IDI_ICON5), IMAGE_ICON, 16, 16, 0);
  m_toolBarImageList.Add(hIcon);

  //   创建工具条并调入资源   
  if(!m_wndToolBar.Create(this)   ||   !m_wndToolBar.LoadToolBar(IDR_TOOLBAR_SERVER_MONITOR))   
  {   
    TRACE0("Failed   to   Create   Dialog   Toolbar/n");   
    return;
  } 

  m_wndToolBar.GetToolBarCtrl().SetImageList(&m_toolBarImageList);  
  RepositionBars(AFX_IDW_CONTROLBAR_FIRST,AFX_IDW_CONTROLBAR_LAST,0);   
  
}

void MonitorServerView::OnDestroy()
{
  CFormView::OnDestroy();

  // TODO: Add your message handler code here
  ((CMainFrame*)AfxGetMainWnd())->m_pMonitorServer=NULL; // 清空窗口指针
}



void MonitorServerView::OnNMRClickServerList(NMHDR *pNMHDR, LRESULT *pResult)
{
  LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
  // TODO: 在此添加控件通知处理程序代码
  POINT pt;

  GetCursorPos(&pt);

  CMenu menu;
  VERIFY(menu.LoadMenu(IDR_LIST_POPUPMENU));

  CMenu* pPopup = menu.GetSubMenu(0); // 这个就是菜单在菜单资源条中的位置
  ASSERT(pPopup != NULL);

  // 控制菜单状态，单击空白处使菜单项变灰无效，当然也可以不显示菜单，看你的需要了
  if(pNMItemActivate->iItem != -1)
  {
    pPopup->EnableMenuItem(ID_Transfer_Monitor, MF_ENABLED);
    pPopup->EnableMenuItem(ID_Client_Monitor, MF_ENABLED);
    pPopup->EnableMenuItem(ID_Connect, MF_ENABLED);
    pPopup->EnableMenuItem(ID_Disconnect, MF_ENABLED);
    pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON,pt.x,pt.y,this);
  }    

  *pResult = 0;
}

void MonitorServerView::OnTransferMonitor()
{
  // TODO: Add your command handler code here
  //monitor_transfer_dlg_ = new CMonitorTransferCtrl();
  //monitor_transfer_dlg_->Create(IDD_Monitor_Transfer,this);
  //monitor_transfer_dlg_->ShowWindow(SW_SHOW);

  ((CMainFrame *)AfxGetMainWnd())->SendMessage(WM_MONITOR_TRANSFER);
}

void MonitorServerView::OnClientMonitor()
{
  /*monitor_client_dlg_ = new CMonitorClientCtrl();
  monitor_client_dlg_->Create(IDD_Monitor_Client,this);
  monitor_client_dlg_->ShowWindow(SW_SHOW);*/
  
  ((CMainFrame *)AfxGetMainWnd())->SendMessage(WM_MONITOR_CLIENT);
}

void MonitorServerView::OnConnectAll()
{
  invoker_->Connect();
  ReloadServerList();
}

void MonitorServerView::OnAddServer()
{
  AddServerDialog dlg;
  dlg.DoModal();
  ReloadServerList();
}

void MonitorServerView::OnDeleteSelectedServer()
{
  if( IDOK != MessageBox("确认从监控列表中删除该服务?", "删除", MB_OKCANCEL) )
    return;

  POSITION pos = m_server_list.GetFirstSelectedItemPosition();
  if (pos != NULL)
  {
    while (pos)
    {
      int nItem = m_server_list.GetNextSelectedItem(pos);
      string s (m_server_list.GetItemText(nItem,2));
      int conn_id = atoi(s.c_str());
      MonitorEnvironment::Instance()->server_manager()->delete_server_by_cid(conn_id);
      m_server_list.DeleteItem(nItem);
    }
  }
  //ReloadServerList();
}

void MonitorServerView::ReloadServerList()
{
  m_server_list.DeleteAllItems();
  ConnectInfo* server_info = NULL;
  for(int i =0;i<MonitorEnvironment::Instance()->server_manager()->server_info_count();i++)
  {
    server_info = MonitorEnvironment::Instance()->server_manager()->server_infos(i);
    m_server_list.InsertItem(i,server_info->ip().c_str());      
    m_server_list.SetItemText(i,1 ,StrUtils::formatstring(5,"%d",server_info->port()).c_str());
    m_server_list.SetItemText(i,2 ,StrUtils::formatstring(5,"%d",server_info->connect_id()).c_str());
    m_server_list.SetItemText(i,3 ,CWordTranslation::TranslateServerStatus(server_info->status()).c_str());
  }
  m_server_list.SetColumnWidth( 0 , LVSCW_AUTOSIZE);
  m_server_list.SetColumnWidth( 1 , LVSCW_AUTOSIZE); 
}


void MonitorServerView::OnSize(UINT nType, int cx, int cy)
{
  CFormView::OnSize(nType, cx, cy);
}

void MonitorServerView::OnSizing(UINT fwSide, LPRECT pRect)
{
  CFormView::OnSizing(fwSide, pRect);

  
}

void MonitorServerView::OnPaint()
{
  if (IsIconic())
  {
    CPaintDC dc(this); // device context for painting
  }
  else
  {
    CFormView::OnPaint();
  }
}

void MonitorServerView::OnBnClickedButton1()
{
  CNetworkSymbol*	obj = new CNetworkSymbol( IDB_BITMAP_SYMBOL_SERVER );
  obj->SetTitle("Test000");
  obj->SetName("Test000");
  m_editor.StartDrawingObject( obj );
  ::SendMessage(m_editor.m_hWnd,WM_LBUTTONDOWN,0,MAKELONG(10,10)); 
  ::SendMessage(m_editor.m_hWnd,WM_LBUTTONUP,0,MAKELONG(10,10)); 
  
  CNetworkSymbol*	obj1 = new CNetworkSymbol( IDB_BITMAP_SYMBOL_SERVER );
  obj1->SetTitle("Test001");
  obj1->SetName("Test001");
  m_objs.Add(obj1);
  obj1->SetSymbol(IDB_BITMAP_SYMBOL_CLIENT);
  obj1->SetRect(10,100,42,132);

  CNetworkLink* link = new CNetworkLink();
  link->from = "Test000";
  link->to = "Test001";
  m_objs.AddLink(link);
  RedrawWindow();
}
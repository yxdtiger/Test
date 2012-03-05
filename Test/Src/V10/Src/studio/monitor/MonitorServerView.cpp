// MonitorServerView.cpp : implementation file
//
#include <string>
#include "../main_frame/StdAfx.h"
#include "../main_frame/WMDF_Studio.h"
#include "../main_frame/MainFrm.h"

#include "../../environment/strutils.h"
#include "../../environment/utils.h"
#include "../common/WordTranslation.h"

#include "server_manager.h"
#include "monitor_environment.h"
#include "AddServerDialog.h"
#include "MonitorServerView.h"

// MonitorServerView

IMPLEMENT_DYNCREATE(MonitorServerView, CFormView)

MonitorServerView::MonitorServerView()
	: CFormView(MonitorServerView::IDD)
{
  add_server_dialog_ = NULL;
}

MonitorServerView::~MonitorServerView()
{
  if(add_server_dialog_!=NULL)
  {
    delete add_server_dialog_;
  }
}

void MonitorServerView::DoDataExchange(CDataExchange* pDX)
{
  CFormView::DoDataExchange(pDX);
  DDX_Control(pDX, IDC_Server_LIST, m_server_list);
}

BEGIN_MESSAGE_MAP(MonitorServerView, CFormView)
  ON_WM_DESTROY()
  ON_NOTIFY(NM_RCLICK, IDC_Server_LIST, &MonitorServerView::OnNMRClickServerList)
  ON_BN_CLICKED(ID_BUTTON_CONNECT_ALL, &MonitorServerView::OnConnectAll)
  ON_BN_CLICKED(ID_BUTTON_ADD_SERVER, &MonitorServerView::OnAddServer)
  ON_BN_CLICKED(ID_BUTTON_DELETE_SERVER, &MonitorServerView::OnDeleteSelectedServer)
  ON_WM_SIZE()
  ON_WM_TIMER()
  ON_BN_CLICKED(ID_BUTTON_MONITE_TRANSFER, &MonitorServerView::OnTransferMonitor)
  ON_BN_CLICKED(ID_BUTTON_MONITE_CLIENTS, &MonitorServerView::OnClientMonitor)
  ON_BN_CLICKED(ID_MONITOR_LOG, &MonitorServerView::OnLogMonitor)
  ON_COMMAND(ID_REAL_QUOTE, &MonitorServerView::OnQuoteMonitor)
  ON_COMMAND(ID_BUTTON_NETWORK, &MonitorServerView::OnMonitorNetwork)
  ON_BN_CLICKED(ID_DISABLE_SOUND, &MonitorServerView::OnBnClickedSoundSwitch)
  ON_BN_CLICKED(ID_CLEAR_WARNING, &MonitorServerView::OnClearWarnings)
  ON_NOTIFY_EX_RANGE(TTN_NEEDTEXTW, 0, 0xFFFF, &MonitorServerView::OnToolTipText)
  /*ON_NOTIFY_EX_RANGE(TTN_NEEDTEXTA, 0, 0xFFFF, &MonitorServerView::OnToolTipText)*/
  ON_COMMAND(ID_BUTTON_MODIFY_CONNECTION, &MonitorServerView::OnButtonModifyConnection)
  ON_COMMAND(ID_DISCONNECT_ALL, &MonitorServerView::OnDisconnectAll)
  ON_COMMAND(ID_DISCONNECT_THIS, &MonitorServerView::OnDisconnectThis)
  ON_COMMAND(ID_CONNECT_THIS, &MonitorServerView::OnConnectThis)
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

  VERIFY(m_Font.CreateFont( 
    20,                                                 //   nHeight 
    0,                                                   //   nWidth 
    0,                                                   //   nEscapement 
    0,                                                   //   nOrientation 
    FW_NORMAL,                                   //   nWeight 
    FALSE,                                           //   bItalic 
    FALSE,                                           //   bUnderline 
    0,                                                   //   cStrikeOut 
    GB2312_CHARSET,                             //   nCharSet 
    OUT_DEFAULT_PRECIS,                 //   nOutPrecision 
    CLIP_DEFAULT_PRECIS,               //   nClipPrecision 
    DEFAULT_QUALITY,                       //   nQuality 
    DEFAULT_PITCH   |   FF_SWISS,     //   nPitchAndFamily 
    "Arial "));                                   //   lpszFacename 


  // TODO: Add your specialized code here and/or call the base class
  GetParentFrame()->SetWindowText("服务监控");
  

  m_bmpImgList.Create(32 , 32, ILC_COLOR | ILC_MASK, 3, 2);//
  CBitmap mapRed,mapYellow,mapGreen;
  mapRed.LoadBitmap(IDB_BITMAP_RED);
  mapYellow.LoadBitmap(IDB_BITMAP_YELLOW);
  mapGreen.LoadBitmap(IDB_BITMAP_GREEN);

  m_bmpImgList.Add(&mapGreen,RGB(192,192,192));
  m_bmpImgList.Add(&mapYellow,RGB(192,192,192));
  m_bmpImgList.Add(&mapRed,RGB(192,192,192));

  m_server_list.SetImageList(&m_bmpImgList,LVSIL_SMALL);

  m_server_list.InsertColumn(0,"ip地址",LVCFMT_LEFT,200);
  m_server_list.InsertColumn(1,"端口",LVCFMT_LEFT,100);
  m_server_list.InsertColumn(2,"连接ID",LVCFMT_LEFT,100);
  m_server_list.InsertColumn(3,"服务类型",LVCFMT_LEFT,150);
  m_server_list.InsertColumn(4,"连接状态",LVCFMT_LEFT,150);
  m_server_list.SetFont(&m_Font);
  m_server_list.SetExtendedStyle(LVS_EX_GRIDLINES|LVS_EX_FULLROWSELECT);

  ReloadServerList();

  m_toolBarImageList.Create(32, 32, TRUE | ILC_COLOR8, 12, 0);
  HICON hIcon = NULL;

  hIcon = (HICON)::LoadImage(::AfxGetInstanceHandle(), 
    MAKEINTRESOURCE(IDI_ADD_SERVER), IMAGE_ICON, 32, 32, 0);
  m_toolBarImageList.Add(hIcon);

  hIcon = (HICON)::LoadImage(::AfxGetInstanceHandle(), 
    MAKEINTRESOURCE(IDI_DELETE_SERVER), IMAGE_ICON, 32, 32, 0);
  m_toolBarImageList.Add(hIcon);

  hIcon = (HICON)::LoadImage(::AfxGetInstanceHandle(), 
    MAKEINTRESOURCE(IDI_MODIFY_CONNECTION), IMAGE_ICON, 32, 32, 0);
  m_toolBarImageList.Add(hIcon);

  hIcon = (HICON)::LoadImage(::AfxGetInstanceHandle(), 
    MAKEINTRESOURCE(IDI_CONNECT_ALL), IMAGE_ICON, 32, 32, 0);
  m_toolBarImageList.Add(hIcon);

  hIcon = (HICON)::LoadImage(::AfxGetInstanceHandle(), 
    MAKEINTRESOURCE(IDI_DISCONNECT_ALL), IMAGE_ICON, 32, 32, 0);
  m_toolBarImageList.Add(hIcon);

  hIcon = (HICON)::LoadImage(::AfxGetInstanceHandle(), 
    MAKEINTRESOURCE(IDI_FLOW_MONITOR), IMAGE_ICON, 32, 32, 0);
  m_toolBarImageList.Add(hIcon);

  hIcon = (HICON)::LoadImage(::AfxGetInstanceHandle(), 
    MAKEINTRESOURCE(IDI_CLIENT_MONITOR), IMAGE_ICON, 32, 32, 0);
  m_toolBarImageList.Add(hIcon);

  hIcon = (HICON)::LoadImage(::AfxGetInstanceHandle(), 
    MAKEINTRESOURCE(IDI_ICON_LOG_VIEWER), IMAGE_ICON, 32, 32, 0);
  m_toolBarImageList.Add(hIcon);

  hIcon = (HICON)::LoadImage(::AfxGetInstanceHandle(), 
    MAKEINTRESOURCE(IDI_ICON_NETWORK), IMAGE_ICON, 32, 32, 0);
  m_toolBarImageList.Add(hIcon);

  hIcon = (HICON)::LoadImage(::AfxGetInstanceHandle(), 
    MAKEINTRESOURCE(IDI_ICON_QUOTE), IMAGE_ICON, 32, 32, 0);
  m_toolBarImageList.Add(hIcon);

  hIcon = (HICON)::LoadImage(::AfxGetInstanceHandle(), 
    MAKEINTRESOURCE(IDI_ICON_DISABLESOUND), IMAGE_ICON, 32, 32, 0);
  m_toolBarImageList.Add(hIcon);  

  hIcon = (HICON)::LoadImage(::AfxGetInstanceHandle(), 
    MAKEINTRESOURCE(IDI_DELETE_ALL_WARNING), IMAGE_ICON, 32, 32, 0);
  m_toolBarImageList.Add(hIcon);  
   
  

  //   创建工具条并调入资源   
  if(!m_wndToolBar.Create(this)   ||   !m_wndToolBar.LoadToolBar(IDR_TOOLBAR_SERVER_MONITOR))   
  {   
    TRACE0("Failed   to   Create   Dialog   Toolbar/n");   
    return;
  } 

  m_wndToolBar.GetToolBarCtrl().SetImageList(&m_toolBarImageList);  
  m_wndToolBar.EnableToolTips(TRUE);
  RepositionBars(AFX_IDW_CONTROLBAR_FIRST,AFX_IDW_CONTROLBAR_LAST,0);   

  SetTimer(1, 1000, NULL);
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


void MonitorServerView::OnLogMonitor()
{
  ((CMainFrame *)AfxGetMainWnd())->SendMessage(WM_MONITOR_LOG);
}


void MonitorServerView::OnMonitorNetwork()
{
  ((CMainFrame *)AfxGetMainWnd())->SendMessage(WM_MONITOR_NETWORK);
}

void MonitorServerView::OnQuoteMonitor()
{
  ((CMainFrame *)AfxGetMainWnd())->SendMessage(WM_MONITOR_QUOTE);
}

void MonitorServerView::OnConnectAll()
{
  ServerManager::Instance()->net_info_collection()->RestAllModels();
  MonitorEnvironment::Instance()->StartMonitor();
  ReloadServerList();
}

void MonitorServerView::OnDisconnectAll()
{
  MonitorEnvironment::Instance()->EndMonitor();
  ReloadServerList();
}

void MonitorServerView::OnDisconnectThis()
{
  POSITION pos = m_server_list.GetFirstSelectedItemPosition();
  if (pos != NULL)
  {
    while (pos)
    {
      int nItem = m_server_list.GetNextSelectedItem(pos);
      string s (m_server_list.GetItemText(nItem,2));
      int conn_id = atoi(s.c_str());
      MonitorEnvironment::Instance()->StopMonitorOne(conn_id);
    }
  }
}

void MonitorServerView::OnConnectThis()
{
  POSITION pos = m_server_list.GetFirstSelectedItemPosition();
  if (pos != NULL)
  {
    while (pos)
    {
      int nItem = m_server_list.GetNextSelectedItem(pos);
      string s (m_server_list.GetItemText(nItem,2));
      int conn_id = atoi(s.c_str());
      MonitorEnvironment::Instance()->StartMonitorOne(conn_id);
    }
  }
}


void MonitorServerView::OnAddServer()
{
  AddServerDialog dlg;
  dlg.DoModal();
  m_server_list.DeleteAllItems();
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
      MonitorEnvironment::Instance()->StopMonitorOne(conn_id);
      ServerNetInfo* server_info = ServerManager::Instance()->net_info_collection()->find_server_by_cid(conn_id);
      if(server_info!=NULL)
      {
        if(server_info->is_drawed())
        {
          ((CMainFrame *)AfxGetMainWnd())->SendMessage(WM_MONITOR_REMOVEVIEW,conn_id,0);
        }
      }
      ServerManager::Instance()->delete_server_by_cid(conn_id);
      m_server_list.DeleteItem(nItem);
    }
  }
}


void MonitorServerView::OnButtonModifyConnection()
{
  POSITION pos = m_server_list.GetFirstSelectedItemPosition();
  if (pos != NULL)
  {
    if(pos)
    {
      int nItem = m_server_list.GetNextSelectedItem(pos);
      string s (m_server_list.GetItemText(nItem,2));
      int conn_id = atoi(s.c_str());
      ServerNetInfo* server_info = ServerManager::Instance()->net_info_collection()->find_server_by_cid(conn_id);
      BYTE fields[4];
      memcpy(fields,server_info->ip().c_str(),4);
      if(add_server_dialog_==NULL)
      {
        add_server_dialog_=new AddServerDialog(this);
        add_server_dialog_->Create(IDD_DIALOG_ADD_SERVER);
        add_server_dialog_->SetWindowText("修改监控对象");
      }
      add_server_dialog_->m_server_type_combo.SetCurSel(server_info->server_type());
      add_server_dialog_->server_id = server_info->server_id();
      add_server_dialog_->m_monitor_port = server_info->monitor_port();
      add_server_dialog_->m_quote_port = server_info->quote_port();
      add_server_dialog_->m_monitor_id = server_info->monitor_user_name().c_str();
      add_server_dialog_->m_quote_id = server_info->quote_user_name().c_str();
      add_server_dialog_->m_monitor_password = server_info->monitor_keyword().c_str();
      add_server_dialog_->m_quote_password = server_info->quote_keyword().c_str();
      add_server_dialog_->m_server_ip.SetWindowText(server_info->ip().c_str());
      
      add_server_dialog_->UpdateData(FALSE);
      add_server_dialog_->ShowWindow(SW_SHOW);
    }
  }
}

void MonitorServerView::ReloadServerList()
{
  int lines = m_server_list.GetItemCount();
  if(lines>0)
  {
    LVITEM* pItem = NULL;
    for(int i =0;i<lines;i++)
    {
      string s (m_server_list.GetItemText(i,2));
      int conn_id = atoi(s.c_str());
      ServerNetInfo* server_info = ServerManager::Instance()->net_info_collection()->find_server_by_cid(conn_id);
      m_server_list.SetItemText(i,0 ,server_info->ip().c_str());
      m_server_list.SetItemText(i,1 ,StrUtils::formatstring(5,"%d",server_info->monitor_port()).c_str());
      m_server_list.SetItemText(i,2 ,StrUtils::formatstring(5,"%d",server_info->server_id()).c_str());
      if(server_info->server_type()==0)
        m_server_list.SetItemText(i,3,"Feed Adapter");
      else
        m_server_list.SetItemText(i,3,"Feed Server");
      SetListItem(i, server_info);

    }
  }
  else
  {    
    ServerNetInfo* server_info = NULL;
    for(int i =0;i<ServerManager::Instance()->net_info_collection()->server_info_count();i++)
    {
      server_info = ServerManager::Instance()->net_info_collection()->server_infos(i);
      m_server_list.InsertItem(i,server_info->ip().c_str());
      m_server_list.SetItemText(i,1 ,StrUtils::formatstring(5,"%d",server_info->monitor_port()).c_str());
      m_server_list.SetItemText(i,2 ,StrUtils::formatstring(5,"%d",server_info->server_id()).c_str());
      if(server_info->server_type()==0)
        m_server_list.SetItemText(i,3,"Feed Adapter");
      else
        m_server_list.SetItemText(i,3,"Feed Server");
      SetListItem(i, server_info);
    }
  }
}


void MonitorServerView::OnSize(UINT nType, int cx, int cy)
{
  CFormView::OnSize(nType, cx, cy);
  /*重置LIST大小*/ 
  CWnd  *pList=GetDlgItem(IDC_Server_LIST); 
  if(pList) 
  { 
    CRect  rect; 
    GetWindowRect(&rect);  //获取窗口rect，
    ScreenToClient(rect);  //从窗口尺寸转换到用户区rect
    rect.top+=54;
    pList->MoveWindow(&rect,true); 
  } 
}

void MonitorServerView::UpdateServerStatus()
{
  ReloadServerList();
}


void MonitorServerView::OnTimer(UINT_PTR nIDEvent)
{
  // TODO: Add your message handler code here and/or call default
  UpdateServerStatus();
  CFormView::OnTimer(nIDEvent);
}


void MonitorServerView::OnBnClickedSoundSwitch()
{
  if(MonitorEnvironment::Instance()->is_sound_enable())
  {
    MonitorEnvironment::Instance()->set_is_sound_enable(false);
    m_wndToolBar.GetToolBarCtrl().CheckButton(ID_DISABLE_SOUND, TRUE);
  }
  else
  {
    MonitorEnvironment::Instance()->set_is_sound_enable(true);
    m_wndToolBar.GetToolBarCtrl().CheckButton(ID_DISABLE_SOUND, FALSE);
  }
}

void MonitorServerView::SetListItem( int i, ServerNetInfo* server_info )
{
  m_server_list.SetItemText(i,4,CWordTranslation::TranslateServerStatus(server_info->status()).c_str());
  int nImage;
  if(server_info->status()>0)
  {
    nImage = 0;
  }
  else if(server_info->status() ==0)
  {
    nImage = 1;
  }
  else
    nImage = 2;
  m_server_list.SetItem(   i,   0,   LVIF_IMAGE,   NULL,   nImage,  0,   0,   0   );  
}

void MonitorServerView::OnClearWarnings()
{
  ServerManager::Instance()->net_info_collection()->ClearWarningStatus();
}

BOOL   MonitorServerView::OnToolTipText(UINT,   NMHDR*   pNMHDR,   LRESULT*   pResult)   
{   
  ASSERT(pNMHDR->code   ==   TTN_NEEDTEXTA   ||   pNMHDR->code   ==   TTN_NEEDTEXTW);  

  //   if   (GetRoutingFrame()   !=   NULL)   return   FALSE;  

  //   UNICODE消息   
  TOOLTIPTEXTA*   pTTTA   =   (TOOLTIPTEXTA*)pNMHDR;   
  TOOLTIPTEXTW*   pTTTW   =   (TOOLTIPTEXTW*)pNMHDR;   
  //TCHAR   szFullText[512];   
  CString   strTipText;   
  UINT   nID   =   pNMHDR->idFrom;  

  if   (pNMHDR->code   ==   TTN_NEEDTEXTA   &&   (pTTTA->uFlags   &   TTF_IDISHWND)   ||   
    pNMHDR->code   ==   TTN_NEEDTEXTW   &&   (pTTTW->uFlags   &   TTF_IDISHWND))   
  {   
    //   idFrom为工具条的HWND     
    nID   =   ::GetDlgCtrlID((HWND)nID);   
  }  

  if   (nID   !=   0)   //不为分隔符   
  {
    //AfxLoadString(nID,   szFullText);   
    //strTipText=szFullText;   
    //添加自己想要的提示语
    strTipText.LoadString(nID);   
    strTipText   =   strTipText.Mid(strTipText.Find('\n',0)+1);  

#ifndef   _UNICODE   
    if   (pNMHDR->code   ==   TTN_NEEDTEXTA)   
    {   
      lstrcpyn(pTTTA->szText,   strTipText,   sizeof(pTTTA->szText));   
    }   
    else   
    {   
      _mbstowcsz(pTTTW->szText,   strTipText,   sizeof(pTTTW->szText));   
    }   
#else   
    if   (pNMHDR->code   ==   TTN_NEEDTEXTA)   
    {   
      _wcstombsz(pTTTA->szText,   strTipText,sizeof(pTTTA->szText));   
    }   
    else   
    {   
      lstrcpyn(pTTTW->szText,   strTipText,   sizeof(pTTTW->szText));   
    }   
#endif  

    *pResult   =   0;  

    //   使工具条提示窗口在最上面   
    ::SetWindowPos(pNMHDR->hwndFrom,   HWND_TOP,   0,   0,   0,   0,SWP_NOACTIVATE|   
      SWP_NOSIZE|SWP_NOMOVE|SWP_NOOWNERZORDER);     
    return   TRUE;   
  }   
  return   TRUE;   
}




// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "WMDF_Studio.h"

#include "MainFrm.h"
#include "ChildFrm.h"

#include "../monitor/NetworkMapEditorDoc.h"
#include "../monitor/NetworkMapEditorView.h"

#include "../management_customer/ManageClientDialog.h"
#include "../management_system_config/ManagementSystemConfighDlg.h"
#include "../management_product/ManageProductDialog.h"
#include "../management_product_source/ManageProductSourceDialog.h"

#include "../monitor/MonitorServerView.h"
#include "../monitor/MonitorTransferInfoView.h"
#include "../monitor/MonitorClientInfoView.h"
#include "../monitor/LogBrowserView.h"
#include "../monitor/MonitorQuote.h"

#include "../monitor/server_manager.h"
#include "../monitor/monitor_environment.h"
#include "../../environment/server_environment.h"
#include "../../environment/application_directory.h"
#include "../../environment/file.h"
#include "../../environment/utils.h"
#include "../../environment/strutils.h"
#include "../../environment/wmdf_file.h"
#include "../../monitor/logger.h"
#include "../../monitor/monitor_event_collector.h"
#include <boost/thread.hpp>
#include <boost/make_shared.hpp>
#include <boost/shared_ptr.hpp>
#include "../common/studio_utility.h"
using namespace boost;

using namespace wmdf;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMainFrame
IMPLEMENT_DYNAMIC(CMainFrame, CMDIFrameWndEx)

const int  iMaxUserToolbars = 10;
const UINT uiFirstUserToolBarId = AFX_IDW_CONTROLBAR_FIRST + 40;
const UINT uiLastUserToolBarId = uiFirstUserToolBarId + iMaxUserToolbars - 1;

BEGIN_MESSAGE_MAP(CMainFrame, CMDIFrameWndEx)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
  ON_WM_TIMER()
  ON_WM_CLOSE()
  ON_COMMAND(ID_MANAGE_CLIENT, &CMainFrame::OnManageClient)
  ON_COMMAND(ID_MANAGE_PRODUCT_SOURCE, &CMainFrame::OnManageProductSource)
  ON_COMMAND(ID_MANAGE_SYSTEM_CONFIG, &CMainFrame::OnManageSystemConfig)
  ON_COMMAND(ID_MANAGE_PRODUCT, &CMainFrame::OnManageProduct)
  
  ON_COMMAND(ID_APP_EXIT, &CMainFrame::OnAppExit)
  ON_COMMAND(ID_MDI_MONITOR, &CMainFrame::OnMdiMonitor)

  ON_COMMAND(ID_BUTTON_MONITE_TRANSFER, &CMainFrame::OnMonitorTransfer)
  ON_COMMAND(ID_BUTTON_MONITE_CLIENTS, &CMainFrame::OnMonitorClient)
  ON_COMMAND(ID_MONITOR_LOG, &CMainFrame::OnLogViewer)
  ON_COMMAND(ID_REAL_QUOTE, &CMainFrame::OnMonitorQuote)
  ON_COMMAND(ID_BUTTON_NETWORK, &CMainFrame::OnMonitorNetwork)

  ON_MESSAGE(WM_MONITOR_QUOTE, &CMainFrame::OnMonitorQuote)
  ON_MESSAGE(WM_MONITOR_TRANSFER, &CMainFrame::OnMonitorTransfer)
  ON_MESSAGE(WM_MONITOR_CLIENT, &CMainFrame::OnMonitorClient)
  ON_MESSAGE(WM_MONITOR_LOG, &CMainFrame::OnLogViewer)
  ON_MESSAGE(WM_MONITOR_NETWORK, &CMainFrame::OnMonitorNetwork)
  ON_MESSAGE(WM_MONITOR_REMOVEVIEW, &CMainFrame::RemoveAView)

  ON_COMMAND(ID_Horizontal, &CMainFrame::OnHorizontal)
  ON_COMMAND(ID_Vertical, &CMainFrame::OnVertical)
  ON_COMMAND(ID_CASCADE, &CMainFrame::OnCascade)

  ON_MESSAGE(WM_MONITOR_WARNING, &CMainFrame::OnWarningEvent)  
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	// TODO: add member initialization code here
  m_pManageClient = NULL;
  m_pManageProductSource = NULL;
  m_pManageSystemConfig = NULL;
  m_pManageProduct = NULL;
  m_pMonitorServer = NULL;
  m_pMonitorClient = NULL;
  m_pMonitorTransfer = NULL;
  m_pMonitorLog = NULL;
  m_pMonitorQuote = NULL;

  m_bPlay=FALSE;
  m_wave.OpenDevice();
}

CMainFrame::~CMainFrame()
{
  if(m_pManageClient!=NULL)
    delete m_pManageClient;
  if(m_pManageProductSource!=NULL)
    delete m_pManageProductSource;
  if(m_pManageSystemConfig!=NULL)
    delete m_pManageSystemConfig;
  if(m_pManageProduct!=NULL)
    delete m_pManageProduct;
  if(m_pMonitorClient!=NULL)
    delete m_pMonitorClient;
  if(m_pMonitorServer!=NULL)
    delete m_pMonitorServer;  
  if(m_pMonitorTransfer!=NULL)
    delete m_pMonitorTransfer;
  if(m_pMonitorLog!=NULL)
    delete m_pMonitorLog;

  if( m_bPlay )
  {
    m_wave.Stop();   
  }
   m_wave.CloseDevice();
  ServerManager::Instance()->Finalize();
  MonitorEnvironment::Instance()->Finalize();
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CMDIFrameWndEx::OnCreate(lpCreateStruct) == -1)
		return -1;

  // set the visual manager and style based on persisted value
  //OnApplicationLook();

  //CMDITabInfo mdiTabParams;
  //mdiTabParams.m_style = CMFCTabCtrl::STYLE_3D_VS2005; // other styles available...
  //mdiTabParams.m_bActiveTabCloseButton = TRUE;      // set to FALSE to place close button at right of tab area
  //mdiTabParams.m_bTabIcons = TRUE;    // set to TRUE to enable document icons on MDI taba
  //mdiTabParams.m_bAutoColor = FALSE;    // set to FALSE to disable auto-coloring of MDI tabs
  //mdiTabParams.m_bDocumentMenu = TRUE; // enable the document menu at the right edge of the tab area
  //EnableMDITabbedGroups(TRUE, mdiTabParams);

  if (!m_wndMenuBar.Create(this))
  {
    TRACE0("Failed to create menubar\n");
    return -1;      // fail to create
  }

  m_wndMenuBar.SetPaneStyle(m_wndMenuBar.GetPaneStyle() | CBRS_SIZE_DYNAMIC | CBRS_TOOLTIPS | CBRS_FLYBY);

  // prevent the menu bar from taking the focus on activation
  CMFCPopupMenu::SetForceMenuFocus(FALSE);

	//if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP	| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
	//	!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	//{
	//	TRACE0("Failed to create toolbar\n");
	//	return -1;      // fail to create
	//}

	/*m_imageList.Create(16, 16, TRUE | ILC_COLOR8, 6, 0);
	HICON hIcon = NULL;

	hIcon = (HICON)::LoadImage(::AfxGetInstanceHandle(), 
		MAKEINTRESOURCE(IDI_ICON_LOGON), IMAGE_ICON, 16, 16, 0);
	m_imageList.Add(hIcon);

	hIcon = (HICON)::LoadImage(::AfxGetInstanceHandle(), 
		MAKEINTRESOURCE(IDI_ICON_LOGOUT), IMAGE_ICON, 16, 16, 0);
	m_imageList.Add(hIcon);

	hIcon = (HICON)::LoadImage(::AfxGetInstanceHandle(), 
		MAKEINTRESOURCE(IDI_ICON_OVERALL), IMAGE_ICON, 16, 16, 0);
	m_imageList.Add(hIcon);

	hIcon = (HICON)::LoadImage(::AfxGetInstanceHandle(), 
		MAKEINTRESOURCE(IDI_ICON_PRODUCT), IMAGE_ICON, 16, 16, 0);
	m_imageList.Add(hIcon);

	hIcon = (HICON)::LoadImage(::AfxGetInstanceHandle(), 
		MAKEINTRESOURCE(IDI_ICON_SECURITIES), IMAGE_ICON, 16, 16, 0);
	m_imageList.Add(hIcon);

	hIcon = (HICON)::LoadImage(::AfxGetInstanceHandle(), 
		MAKEINTRESOURCE(IDI_ICON_LOG_VIEWER), IMAGE_ICON, 16, 16, 0);
	m_imageList.Add(hIcon);

	hIcon = (HICON)::LoadImage(::AfxGetInstanceHandle(), 
		MAKEINTRESOURCE(IDI_ICON_ABOUT), IMAGE_ICON, 16, 16, 0);
	m_imageList.Add(hIcon);*/

	//m_wndToolBar.GetToolBarCtrl().SetImageList(&m_imageList);


 // Allow user-defined toolbars operations:
  InitUserToolbars(NULL, uiFirstUserToolBarId, uiLastUserToolBarId);
	m_wndMenuBar.EnableDocking(CBRS_ALIGN_ANY);
	//m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
  DockPane(&m_wndMenuBar);
//	DockPane(&m_wndToolBar);  

  // enable Visual Studio 2005 style docking window behavior
  //CDockingManager::SetDockingMode(DT_STANDARD);
  // enable Visual Studio 2005 style docking window auto-hide behavior
  EnableAutoHidePanes(CBRS_ALIGN_ANY);

  ServerManager::Instance()->Initialize();
  MonitorEnvironment::Instance()->Initialize();
  Logger::instance()->StartFileAppender();
	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CMDIFrameWndEx::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return TRUE;
}


void CMainFrame::OnApplicationLook()
{  

}
void CMainFrame::OnAppExit()
{
  // TODO: Add your command handler code here
  OnClose();
}

void CMainFrame::OnClose()
{
  // TODO: Add your message handler code here and/or call default

  CMDIFrameWndEx::OnClose();
}

// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CMDIFrameWndEx::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CMDIFrameWndEx::Dump(dc);
}

#endif //_DEBUG

// CMainFrame message handlers



void CMainFrame::OnManageClient()
{
  CManageClientDialog dlg;
  dlg.DoModal();
}


void CMainFrame::OnManageProductSource()
{

  CManageProductSourceDialog dlg;
  dlg.DoModal();
}

void CMainFrame::OnManageSystemConfig()
{
 
  CManagementSystemConfigDlg dlg;
  dlg.DoModal();
}

void CMainFrame::OnManageProduct()
{
   CManageProductDialog dlg;
  dlg.DoModal();
}


void CMainFrame::OnHorizontal()
{
  CMDIFrameWnd *pFrame = (CMDIFrameWnd*)AfxGetApp()->m_pMainWnd;
  pFrame->MDITile(ID_WINDOW_TILE_HORZ);
}

void CMainFrame::OnVertical()
{
  CMDIFrameWnd *pFrame = (CMDIFrameWnd*)AfxGetApp()->m_pMainWnd;
  pFrame->MDITile(ID_WINDOW_TILE_VERT);
}

void CMainFrame::OnCascade()
{
  /*CMDIFrameWnd *pFrame = (CMDIFrameWnd*)AfxGetApp()->m_pMainWnd;
  pFrame->MDITile(ID_WINDOW_CASCADE);*/
  ::PostMessage(AfxGetMainWnd()-> m_hWnd,WM_COMMAND,ID_WINDOW_CASCADE,0);
}


void CMainFrame::OnMdiMonitor()
{
  if(m_pMonitorServer!=NULL)
  {
    m_pMonitorServer->MDIActivate();
    return;
  }

  m_pMonitorServer=new CChildFrame();
  CCreateContext context;
  context.m_pNewViewClass=RUNTIME_CLASS(MonitorServerView);
  if(!m_pMonitorServer->LoadFrame(IDI_ICON_NETWORK, WS_SYSMENU|WS_OVERLAPPEDWINDOW, this, &context))
    return;
  m_pMonitorServer->InitialUpdateFrame(NULL,TRUE);
  m_pMonitorServer->ShowWindow(SW_SHOWNORMAL);
  RecalcLayout();
}

void CMainFrame::OnMonitorClient()
{
  OnMonitorClient(NULL,NULL);
}

LRESULT CMainFrame::OnMonitorClient(WPARAM wParam, LPARAM lParam)
{
  if(m_pMonitorClient!=NULL)
  {
    m_pMonitorClient->MDIActivate();
    return TRUE;
  }

  m_pMonitorClient=new CChildFrame();
  CCreateContext context;
  context.m_pNewViewClass=RUNTIME_CLASS(MonitorClientInfoView);
  if(!m_pMonitorClient->LoadFrame(IDI_CLIENT_MONITOR, WS_SYSMENU|WS_OVERLAPPEDWINDOW, this, &context))
    return FALSE;
  
  m_pMonitorClient->InitialUpdateFrame(NULL,TRUE);
  m_pMonitorClient->ShowWindow(SW_SHOWNORMAL);
  RecalcLayout();
  return TRUE;
}

void CMainFrame::OnMonitorTransfer()
{
  OnMonitorTransfer(NULL,NULL);
}

LRESULT CMainFrame::OnMonitorTransfer(WPARAM wParam, LPARAM lParam)
{
  if(m_pMonitorTransfer!=NULL)
  {
    m_pMonitorTransfer->MDIActivate();
    return TRUE;
  }

  m_pMonitorTransfer=new CChildFrame();
  CCreateContext context;
  context.m_pNewViewClass=RUNTIME_CLASS(MonitorTransferInfoView);
  if(!m_pMonitorTransfer->LoadFrame(IDI_FLOW_MONITOR, WS_SYSMENU|WS_MAXIMIZE|WS_OVERLAPPEDWINDOW, this, &context))
    return FALSE;  
  m_pMonitorTransfer->InitialUpdateFrame(NULL,TRUE);
  m_pMonitorTransfer->ShowWindow(SW_SHOWNORMAL);
  RecalcLayout();
  return TRUE;
}


void CMainFrame::OnMonitorQuote()
{
  OnMonitorQuote(NULL,NULL);
}

LRESULT CMainFrame::OnMonitorQuote(WPARAM wParam, LPARAM lParam)
{
  if(m_pMonitorQuote!=NULL)
  {
    m_pMonitorQuote->MDIActivate();
    return TRUE;
  }

  m_pMonitorQuote=new CChildFrame();
  CCreateContext context;
  context.m_pNewViewClass=RUNTIME_CLASS(CMonitorQuote);
  if(!m_pMonitorQuote->LoadFrame(IDI_ICON_QUOTE, (WS_OVERLAPPEDWINDOW)&~WS_MAXIMIZE, this, &context))
    return FALSE;  
  m_pMonitorQuote->ShowWindow(SW_SHOWNORMAL);
  m_pMonitorQuote->InitialUpdateFrame(NULL,TRUE);
  
  RecalcLayout();
  return TRUE;
}

void CMainFrame::OnLogViewer()
{
  OnLogViewer(NULL,NULL);
}

LRESULT CMainFrame::OnLogViewer(WPARAM wParam, LPARAM lParam)
{
  if(m_pMonitorLog!=NULL)
  {
    m_pMonitorLog->MDIActivate();
    return TRUE;
  }

  m_pMonitorLog=new CChildFrame();
  CCreateContext context;
  context.m_pNewViewClass=RUNTIME_CLASS(LogBrowserView);
  if(!m_pMonitorLog->LoadFrame(IDI_ICON_LOG_VIEWER, WS_SYSMENU|WS_MAXIMIZE|WS_OVERLAPPEDWINDOW, this, &context))
    return FALSE;  
  m_pMonitorLog->InitialUpdateFrame(NULL,TRUE);
  m_pMonitorLog->ShowWindow(SW_SHOWNORMAL);
  RecalcLayout();
  return TRUE;
}

LRESULT CMainFrame::OnWarningEvent( WPARAM wParam, LPARAM lParam )
{
  if( m_bPlay )
  {
    m_wave.Stop();
    m_bPlay=FALSE;
  }

  string filePath = ApplicationDirectory::ApplicationPath()+std::string("alarm.wav");
  if(File::Exists(filePath))
  {
    CString strFileName(filePath.c_str());
    m_bPlay=TRUE;
    m_wave.Play(this,strFileName);
  }  
  return TRUE;
}

LRESULT CMainFrame::OnMCINotify( WPARAM wParam, LPARAM lParam )
{
  m_bPlay=FALSE;
  return 0;
}

std::string CMainFrame::GetKey(std::string ip,int port)
{
  return StrUtils::formatstring(20,"%s:%d",ip.c_str(),port);
}


void CMainFrame::OnMonitorNetwork()
{
  OnMonitorNetwork(NULL,NULL);
}

LRESULT CMainFrame::OnMonitorNetwork( WPARAM wParam, LPARAM lParam )
{
  CNetworkMapEditorView* view = NULL;
  ServerNetInfo* server_info = NULL;
  for(int i =0;i<ServerManager::Instance()->net_info_collection()->server_info_count();i++)
  {
    server_info = ServerManager::Instance()->net_info_collection()->server_infos(i);
    if(server_info->is_drawed())
    {
      view = views_[server_info->server_id()];
      //view = ServerManager::Instance()->net_info_collection()->GetView(server_info->server_id());
    }
    else
    {
      string pre_title;
      if(server_info->server_type()==0)
      {
        pre_title="Feed Adapter--";
      }
      else
      {
        pre_title="Feed Server--";
      }
      string key = GetKey(server_info->ip(),server_info->quote_port());
      POSITION template_pos = AfxGetApp()->GetFirstDocTemplatePosition();
      CDocTemplate* pDocTempl=AfxGetApp()->GetNextDocTemplate(template_pos);
      CNetworkMapEditorDoc*  oDoc = (CNetworkMapEditorDoc*)pDocTempl->OpenDocumentFile(NULL);
      oDoc->SetTitle((pre_title+key).c_str());
      POSITION view_pos  = oDoc->GetFirstViewPosition();
      server_info->set_is_drawed(true);
      view = (CNetworkMapEditorView*)oDoc->GetNextView(view_pos);
      view->StartTimer(server_info->server_id());
      //ServerManager::Instance()->net_info_collection()->AddView(view);
      views_[view->conn_id()]=view;
    }
  }
  return TRUE;
}

LRESULT CMainFrame::RemoveAView( WPARAM wParam, LPARAM lParam )
{
  int conn_id = (int)wParam;
  std::map<int, CNetworkMapEditorView*>::iterator iter = views_.find(conn_id);
  if (iter != views_.end())
  {
    CNetworkMapEditorView* view = iter->second;
    CWnd* hwnd = view->GetParent();
    if(hwnd!=NULL)
    {
      hwnd->SendMessage(WM_CLOSE);
    }
    iter->second = NULL;
    views_.erase(iter);
  }
  return TRUE;
}

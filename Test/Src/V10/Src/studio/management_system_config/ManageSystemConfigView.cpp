// AuditingCertView.cpp : implementation file
//

#include "../main_frame/StdAfx.h"
#include "../main_frame/WMDF_Studio.h"
#include "ManageSystemConfigView.h"
#include "../main_frame/MainFrm.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "../../environment/file.h"
#include "../../environment/application_directory.h"
#include "../../environment/utils.h"
using namespace wmdf;

/////////////////////////////////////////////////////////////////////////////
// CManageSystemConfigView
extern CString str[35];
IMPLEMENT_DYNCREATE(CManageSystemConfigView, CFormView)

CManageSystemConfigView::CManageSystemConfigView()
	: CFormView(CManageSystemConfigView::IDD)
  , m_server_port(0)
  , m_server_thread_num(0)
  , m_max_connection_count(0)
  , m_heart_beat_interval(0)
  , m_max_cache_msg_count(0)
  , m_max_packet_size(0)
{
	//{{AFX_DATA_INIT(CManageSystemConfigView)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
  m_bModifyState = false;
}

CManageSystemConfigView::~CManageSystemConfigView()
{
}

void CManageSystemConfigView::DoDataExchange(CDataExchange* pDX)
{
  CFormView::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(CManageSystemConfigView)
  //}}AFX_DATA_MAP
  DDX_Text(pDX, IDC_EDIT_0, m_server_port);
  DDX_Text(pDX, IDC_EDIT_1, m_server_thread_num);
  DDX_Text(pDX, IDC_EDIT_2, m_max_connection_count);
  DDX_Text(pDX, IDC_EDIT_3, m_heart_beat_interval);
  DDX_Text(pDX, IDC_EDIT_4, m_max_cache_msg_count);
  DDX_Text(pDX, IDC_EDIT_5, m_max_packet_size);
}


BEGIN_MESSAGE_MAP(CManageSystemConfigView, CFormView)
	//{{AFX_MSG_MAP(CManageSystemConfigView)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
  ON_BN_CLICKED(IDC_BUTTON_OP, &CManageSystemConfigView::OnBnClickedButtonOp)
  ON_BN_CLICKED(IDC_BUTTON_CANCEL, &CManageSystemConfigView::OnBnClickedButtonCancel)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CManageSystemConfigView diagnostics

#ifdef _DEBUG
void CManageSystemConfigView::AssertValid() const
{
	CFormView::AssertValid();
}

void CManageSystemConfigView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CManageSystemConfigView message handlers

void CManageSystemConfigView::OnInitialUpdate() 
{
	CFormView::OnInitialUpdate();

  // 设置窗口标题
  GetParentFrame()->SetWindowText("系统配置管理");

  // 初始化m_system_config成员
  if (File::Exists(ApplicationDirectory::SystemDescriptionFile()))
  {
    Utils::Deserialize(ApplicationDirectory::SystemDescriptionFile(),&m_system_config);
  }

  m_server_port = m_system_config.server_port();
  m_server_thread_num = m_system_config.server_thread_num();
  m_max_connection_count = m_system_config.max_connect_count();
  m_heart_beat_interval = m_system_config.heart_beat_interval();
  m_max_cache_msg_count = m_system_config.max_cache_msg_count();
  m_max_packet_size = m_system_config.max_packet_size();

  // 修改界面状态
  GetDlgItem(IDC_EDIT_0)->EnableWindow(FALSE);
  GetDlgItem(IDC_EDIT_1)->EnableWindow(FALSE);
  GetDlgItem(IDC_EDIT_2)->EnableWindow(FALSE);
  GetDlgItem(IDC_EDIT_3)->EnableWindow(FALSE);
  GetDlgItem(IDC_EDIT_4)->EnableWindow(FALSE);
  GetDlgItem(IDC_EDIT_5)->EnableWindow(FALSE);

  GetDlgItem(IDC_BUTTON_OP)->SetWindowText("修改");
  GetDlgItem(IDC_BUTTON_CANCEL)->SetWindowText("取消");
  GetDlgItem(IDC_BUTTON_CANCEL)->ShowWindow(FALSE);

  UpdateData(FALSE);
}

void CManageSystemConfigView::OnDestroy() 
{
	CFormView::OnDestroy();
	((CMainFrame*)AfxGetMainWnd())->m_pManageSystemConfig=NULL; // 清空窗口指针

	// TODO: Add your message handler code here
	
}
void CManageSystemConfigView::OnBnClickedButtonOp()
{
  // TODO: Add your control notification handler code here
  m_bModifyState = !m_bModifyState;

  if(m_bModifyState)
  {
    GetDlgItem(IDC_BUTTON_OP)->SetWindowText("确定");
    GetDlgItem(IDC_BUTTON_CANCEL)->ShowWindow(TRUE);
  }
  else
  {
    // 保存数据
    FillSystemConfig();

    GetDlgItem(IDC_BUTTON_OP)->SetWindowText("修改");
  }

  SetControlState();
}

void CManageSystemConfigView::OnBnClickedButtonCancel()
{
  // TODO: Add your control notification handler code here
  m_bModifyState = !m_bModifyState;
  GetDlgItem(IDC_BUTTON_OP)->SetWindowText("修改");
  GetDlgItem(IDC_BUTTON_CANCEL)->ShowWindow(FALSE);

  UpdateData(FALSE);

  SetControlState();
}

void CManageSystemConfigView::SetControlState()
{
  GetDlgItem(IDC_EDIT_0)->EnableWindow(m_bModifyState);
  GetDlgItem(IDC_EDIT_1)->EnableWindow(m_bModifyState);
  GetDlgItem(IDC_EDIT_2)->EnableWindow(m_bModifyState);
  GetDlgItem(IDC_EDIT_3)->EnableWindow(m_bModifyState);
  GetDlgItem(IDC_EDIT_4)->EnableWindow(m_bModifyState);
  GetDlgItem(IDC_EDIT_5)->EnableWindow(m_bModifyState);
}

void CManageSystemConfigView::FillSystemConfig()
{
  UpdateData(TRUE);

  m_system_config.set_server_port(m_server_port);
  m_system_config.set_server_thread_num(m_server_thread_num);
  m_system_config.set_max_connect_count(m_max_connection_count);
  m_system_config.set_heart_beat_interval(m_heart_beat_interval);
  m_system_config.set_max_cache_msg_count(m_max_cache_msg_count);
  m_system_config.set_max_packet_size(m_max_packet_size);

  SaveProductSourceToXmlFile( ApplicationDirectory::SystemDescriptionFile() );
}

void CManageSystemConfigView::SaveProductSourceToXmlFile(const std::string& file_path)
{
  Utils::Serialize(file_path, reinterpret_cast<Persistable*>(&m_system_config));
};
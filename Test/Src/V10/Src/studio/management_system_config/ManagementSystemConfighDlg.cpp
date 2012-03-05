// UserSerachDlg.cpp : implementation file
//

#include "../main_frame/StdAfx.h"
#include "../main_frame/WMDF_Studio.h"
#include "ManagementSystemConfighDlg.h"
#include "../../include/wmdf_internal_api.h"
#include "../../environment/file.h"
#include "../../environment/application_directory.h"
#include "../../environment/utils.h"
#include "../../environment/strutils.h"
#include "../../environment/server_environment.h"
#include "../monitor/server_info.h"
#include "../monitor/server_manager.h"
using namespace wmdf;


// CManagementSystemConfigDlg dialog

IMPLEMENT_DYNAMIC(CManagementSystemConfigDlg, CDialog)

CManagementSystemConfigDlg::CManagementSystemConfigDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CManagementSystemConfigDlg::IDD, pParent)
  , m_server_port(0)
  , m_server_thread_num(0)
  , m_max_connection_count(0)
  , m_heart_beat_interval(0)
  , m_max_cache_msg_count(0)
  , m_max_packet_size(0)
  , m_monitor_server_port(0)
  , m_nMonitorServerState(0)
  , m_reset_hour(0)
  , m_preprocess_open(0)
  , m_monitor_interval(0)
  , m_process_close(0)
  , m_flag(false)
{
   m_system_config = new wmdf::SystemConfig();
}

CManagementSystemConfigDlg::~CManagementSystemConfigDlg()
{
  if (m_system_config!= NULL)
  {
    delete m_system_config;
    m_system_config = NULL;
  }
}

void CManagementSystemConfigDlg::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);

  DDX_Text(pDX, IDC_EDIT_0, m_server_port);
  DDX_Text(pDX, IDC_EDIT_1, m_server_thread_num);
  DDX_Text(pDX, IDC_EDIT_2, m_max_connection_count);
  DDX_Text(pDX, IDC_EDIT_3, m_heart_beat_interval);
  DDX_Text(pDX, IDC_EDIT_4, m_max_cache_msg_count);
  DDX_Text(pDX, IDC_EDIT_5, m_max_packet_size);

  DDX_Text(pDX, IDC_EDIT_SERVER_RESET, m_reset_hour);
  DDX_Text(pDX, IDC_EDIT_SERVER_PREPROCESS, m_preprocess_open);
  DDX_Text(pDX, IDC_EDIT_CloseTime, m_process_close);

  DDX_Text(pDX, IDC_EDIT_6, m_monitor_server_port);  
  DDX_Radio(pDX, IDC_RADIO1, m_nMonitorServerState);
  DDX_Text(pDX, IDC_EDIT_MONITOR_INTERVAL, m_monitor_interval);
  DDX_Control(pDX, IDOK, m_conButton_ok);
  DDX_Control(pDX, IDCANCEL, m_conButton_cancel);
  DDX_Control(pDX, DEST_SERVER_ID, m_dest_server);
}


BEGIN_MESSAGE_MAP(CManagementSystemConfigDlg, CDialog)
  ON_BN_CLICKED(IDOK, &CManagementSystemConfigDlg::OnBnClickedOk)
  ON_BN_CLICKED(IDC_GET_CONFIG_BUTTON, &CManagementSystemConfigDlg::OnBnClickedGetConfigButton)
  ON_BN_CLICKED(IDC_UPDATE_CONFIG_BUTTON, &CManagementSystemConfigDlg::OnBnClickedUpdateConfigButton)
END_MESSAGE_MAP()


// CManagementSystemConfigDlg message handlers

BOOL CManagementSystemConfigDlg::OnInitDialog()
{
  CDialog::OnInitDialog();

  // TODO:  Add extra initialization here
  // 设置窗口标题
  SetWindowText("系统配置管理");
 
  StudioUtility::InitServerList(m_dest_server);
  UpdateData(FALSE);

  return TRUE;  // return TRUE unless you set the focus to a control
  // EXCEPTION: OCX Property Pages should return FALSE
}

void CManagementSystemConfigDlg::FillSystemConfig()
{
  UpdateData(TRUE);

  m_system_config->set_server_port(m_server_port);
  m_system_config->set_server_thread_num(m_server_thread_num);
  m_system_config->set_max_connection_count(m_max_connection_count);
  m_system_config->set_heart_beat_interval(m_heart_beat_interval);
  m_system_config->set_max_cache_msg_count(m_max_cache_msg_count);
  m_system_config->set_max_packet_size(m_max_packet_size);
  m_system_config->set_monitor_server_port(m_monitor_server_port);
  m_system_config->set_monitor_server_on( (bool)m_nMonitorServerState);
  m_system_config->set_server_reset_operation_hour(m_reset_hour);
  m_system_config->set_pre_market_operation_hour(m_preprocess_open);
  m_system_config->set_after_market_operation_hour(m_process_close);
  m_system_config->set_monitor_interval(m_monitor_interval);
}

void CManagementSystemConfigDlg::OnBnClickedOk()
{
  // TODO: Add your control notification handler code here
  OnBnClickedUpdateConfigButton();
  OnOK();
}

void CManagementSystemConfigDlg::OnBnClickedGetConfigButton()
{
  int index = m_dest_server.GetCurSel();

  ConfigFileType configType = CFT_SYSTEM;
  if(StudioUtility::GetConfigInfo(index, configType,m_system_config))
  {
    m_server_port = m_system_config->server_port();
    m_server_thread_num = m_system_config->server_thread_num();
    m_max_connection_count = m_system_config->max_connection_count();
    m_heart_beat_interval = m_system_config->heart_beat_interval();
    m_max_cache_msg_count = m_system_config->max_cache_msg_count();
    m_max_packet_size = m_system_config->max_packet_size();
    m_nMonitorServerState = m_system_config->monitor_server_on();
    m_monitor_server_port = m_system_config->monitor_server_port();

    m_reset_hour = m_system_config->server_reset_operation_hour();
    m_preprocess_open = m_system_config->pre_market_operation_hour();
    m_process_close = m_system_config->after_market_operation_hour();
    m_monitor_interval = m_system_config->monitor_interval();
    m_flag = true;
  }
  else
    MessageBox("获取服务配置信息失败！请检查连接是否正常");  
  UpdateData(FALSE);
}

void CManagementSystemConfigDlg::OnBnClickedUpdateConfigButton()
{
  if(m_flag)
  {
    FillSystemConfig();
    int index = m_dest_server.GetCurSel();
    if(!StudioUtility::SetConfigInfo(index,CFT_SYSTEM,m_system_config))
    {
      MessageBox("修改系统信息失败！");
    }
    else
    {
      MessageBox("修改系统信息成功！");
    }
  }
  else
  {
    MessageBox("请先获取配置信息并修改！");
  }
  
}


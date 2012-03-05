#include "../main_frame/StdAfx.h"
#include "server_manager.h"
#include "AddServerDialog.h"
#include <string>
#include "../main_frame/resource.h"
// AddServerDialog dialog
using namespace wmdf;
IMPLEMENT_DYNAMIC(AddServerDialog, CDialog)

AddServerDialog::AddServerDialog(CWnd* pParent /*=NULL*/)
	: CDialog(AddServerDialog::IDD, pParent),m_monitor_port(0)
  , m_quote_port(0)
  , m_monitor_id(_T("monitor"))
  , m_quote_id(_T("admin"))
  , m_monitor_password(_T("monitor"))
  , m_quote_password(_T("admin"))
  , server_id(-1)
{
  
}

AddServerDialog::~AddServerDialog()
{
}

void AddServerDialog::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  DDX_Control(pDX, IDC_SERVER_IP, m_server_ip);
  DDX_Text(pDX, IDC_Monitor_port, m_monitor_port);
  DDX_Text(pDX, IDC_QUOTE_PORT, m_quote_port);
  DDX_Control(pDX, IDC_SERVER_TYPE, m_server_type_combo);
  DDX_Text(pDX, IDC_MONITOR_ID, m_monitor_id);
  DDX_Text(pDX, IDC_QUOTE_ID, m_quote_id);
  DDX_Text(pDX, IDC_MONITOR_KEYWORD, m_monitor_password);
  DDX_Text(pDX, IDC_QUOTE_PASSWORD, m_quote_password);
}


BEGIN_MESSAGE_MAP(AddServerDialog, CDialog)
  ON_BN_CLICKED(IDOK, &AddServerDialog::OnBnClickedOk)
END_MESSAGE_MAP()


// AddServerDialog message handlers

void AddServerDialog::OnBnClickedOk()
{
  UpdateData(TRUE);
  CString m_szIPAddress;
  BYTE nField0 = 0, nField1 = 0, nField2 = 0, nField3 = 0;
  if(!m_server_ip.IsBlank())
  {
    m_server_ip.GetAddress(nField0, nField1, nField2, nField3);
  }
  m_szIPAddress.Format(_T("%d.%d.%d.%d"), nField0, nField1, nField2, nField3);
  string ip =m_szIPAddress.GetBuffer(0);

  ServerNetInfo* conn_info=NULL;
  if(server_id<0)
  {
    conn_info = new ServerNetInfo();
    conn_info->set_server_id(server_id);
    ServerManager::Instance()->add_server_info(conn_info);
  }
  else
  {
    conn_info = ServerManager::Instance()->net_info_collection()->find_server_by_cid(server_id);
  }

  conn_info->set_ip(ip) ;
  conn_info->set_monitor_port(m_monitor_port);
  conn_info->set_server_type(m_server_type_combo.GetCurSel());
  conn_info->set_quote_port(m_quote_port);

  conn_info->set_monitor_user_name(m_monitor_id.GetBuffer(0));
  conn_info->set_monitor_keyword(m_monitor_password.GetBuffer(0));
  conn_info->set_quote_user_name(m_quote_id.GetBuffer(0));
  conn_info->set_quote_keyword(m_quote_password.GetBuffer(0));
  
  OnOK();
}

BOOL AddServerDialog::OnInitDialog()
{
  CDialog::OnInitDialog();

  m_server_type_combo.AddString("Feed Adapter");
  m_server_type_combo.AddString("Feed Server");
  m_server_type_combo.SetCurSel(m_server_type_combo.GetCount() - 1); 

  return TRUE;  // return TRUE unless you set the focus to a control
  // EXCEPTION: OCX Property Pages should return FALSE
}

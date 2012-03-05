// D:\SVN\MDF\Src\ver10\src\monitor\UI\ManagmentProductSourceAdd.cpp : implementation file
//

#include "../main_frame/StdAfx.h"
#include "ManagmentProductSourceAdd.h"
#include <sstream>
#include <assert.h>
#include "../../environment/utils.h"
#include "../../environment/splitter.h"

using namespace std;
using namespace wmdf;

// CManagmentProductSourceAdd dialog

IMPLEMENT_DYNAMIC(CManagmentProductSourceAdd, CDialog)

CManagmentProductSourceAdd::CManagmentProductSourceAdd(CWnd* pParent /*=NULL*/, bool bDialogType /*=true*/)
	: CDialog(CManagmentProductSourceAdd::IDD, pParent)
  , m_str_id(_T(""))
  , m_nPort(0)
  , m_str_des(_T(""))
  , m_bAddDialog(bDialogType)
{

}

CManagmentProductSourceAdd::~CManagmentProductSourceAdd()
{
}

void CManagmentProductSourceAdd::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  DDX_Text(pDX, IDC_EDIT_id, m_str_id);
  DDX_Text(pDX, IDC_EDIT_port, m_nPort);
  DDX_Text(pDX, IDC_EDIT_des, m_str_des);
  DDX_Control(pDX, IDC_IPADDRESS, m_conIPAddr);
}


BEGIN_MESSAGE_MAP(CManagmentProductSourceAdd, CDialog)
  ON_BN_CLICKED(IDOK, &CManagmentProductSourceAdd::OnBnClickedOk)
  ON_BN_CLICKED(IDC_BUTTON_SAVE_CONTINUE, &CManagmentProductSourceAdd::OnBnClickedButtonSaveContinue)
END_MESSAGE_MAP()


// CManagmentProductSourceAdd message handlers

void CManagmentProductSourceAdd::OnBnClickedOk()
{
  // TODO: Add your control notification handler code here
  AddProductSource();

  OnOK();
}

void CManagmentProductSourceAdd::OnBnClickedButtonSaveContinue()
{
  // TODO: Add your control notification handler code here
  AddProductSource();
}

void CManagmentProductSourceAdd::FillProductSource()
{
  UpdateData(TRUE);

  m_productSource.set_product_id(atoi(m_str_id.GetBuffer(0)));
  m_str_id.ReleaseBuffer();

  BYTE nField[4];
  m_conIPAddr.GetAddress(nField[0], nField[1], nField[2], nField[3]);
  stringstream ss;
  ss << static_cast<int>(nField[0]) << "." << static_cast<int>(nField[1]) << "." << static_cast<int>(nField[2]) << "." << static_cast<int>(nField[3]);
  m_productSource.set_ip_address(ss.str());

  m_productSource.set_port(m_nPort);

  m_productSource.set_description(string(m_str_des.GetBuffer(0)));

}

void CManagmentProductSourceAdd::AddProductSource()
{
  FillProductSource();

  // 发送消息至框架，通知添加了一个新用户
  m_pParentWnd->SendMessage(WM_ADD_PRODUCT_SOURCE, reinterpret_cast<WPARAM>(&m_productSource), reinterpret_cast<LPARAM>(&m_bAddDialog));
}
BOOL CManagmentProductSourceAdd::OnInitDialog()
{
  CDialog::OnInitDialog();

  // TODO:  Add extra initialization here
  m_str_id.Format("%d", m_productSource.product_id());

  if(!m_productSource.ip_address().empty())
  {
    Splitter split(m_productSource.ip_address(), string("."));
    assert(split.size() == 4);  // ip4
    m_conIPAddr.SetAddress( static_cast<BYTE>(atoi(split[0].c_str())), static_cast<BYTE>(atoi(split[1].c_str())), static_cast<BYTE>(atoi(split[2].c_str())), static_cast<BYTE>(atoi(split[3].c_str())) );
  }
  
  m_nPort = m_productSource.port();
  m_str_des = m_productSource.description().c_str();

  if(!m_bAddDialog)
    GetDlgItem(IDC_BUTTON_SAVE_CONTINUE)->ShowWindow(FALSE);

  UpdateData(FALSE);

  return TRUE;  // return TRUE unless you set the focus to a control
  // EXCEPTION: OCX Property Pages should return FALSE
}

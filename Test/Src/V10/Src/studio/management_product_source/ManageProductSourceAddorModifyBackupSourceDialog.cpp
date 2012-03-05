// ManageProductSourceAddorModifyBackupSourceDialog.cpp : implementation file
//

#include "../main_frame/StdAfx.h"
#include "ManageProductSourceAddorModifyBackupSourceDialog.h"

using namespace wmdf;

// CManageProductSourceAddorModifyBackupSourceDialog dialog

IMPLEMENT_DYNAMIC(CManageProductSourceAddorModifyBackupSourceDialog, CDialog)

CManageProductSourceAddorModifyBackupSourceDialog::CManageProductSourceAddorModifyBackupSourceDialog(CWnd* pParent /* = NULL */, bool bDialogType /* = true */)
	: CAddOrModifyDialog(CManageProductSourceAddorModifyBackupSourceDialog::IDD, pParent, bDialogType)
{
}

CManageProductSourceAddorModifyBackupSourceDialog::~CManageProductSourceAddorModifyBackupSourceDialog()
{
}

BEGIN_MESSAGE_MAP(CManageProductSourceAddorModifyBackupSourceDialog, CDialog)
  ON_BN_CLICKED(IDOK, &CManageProductSourceAddorModifyBackupSourceDialog::OnBnClickedOk)
  ON_BN_CLICKED(IDC_BUTTON_CONTINUEADD, &CManageProductSourceAddorModifyBackupSourceDialog::OnBnClickedSaveAdd)
  ON_WM_TIMER()
END_MESSAGE_MAP()


// CManageProductSourceAddorModifyBackupSourceDialog message handlers

void CManageProductSourceAddorModifyBackupSourceDialog::FillItem()
{
  if(!m_pData)
    m_pData = new BackupSource;

  CString strContent;
  GetDlgItem(IDC_EDIT_ip)->GetWindowText(strContent);
  reinterpret_cast<BackupSource*>(m_pData)->set_ip_address(strContent.GetBuffer(0));
  strContent.ReleaseBuffer();

  GetDlgItem(IDC_EDIT_port)->GetWindowText(strContent);
  reinterpret_cast<BackupSource*>(m_pData)->set_port( static_cast<int32_t>(atoi(strContent.GetBuffer(0))) );
  strContent.ReleaseBuffer();

  GetDlgItem(IDC_EDIT_USERNAME)->GetWindowText(strContent);
  reinterpret_cast<BackupSource*>(m_pData)->set_user_name(strContent.GetBuffer(0));
  strContent.ReleaseBuffer();

  GetDlgItem(IDC_EDIT_PASSWORD)->GetWindowText(strContent);
  reinterpret_cast<BackupSource*>(m_pData)->set_pass_word(strContent.GetBuffer(0));
  strContent.ReleaseBuffer();
}

BOOL CManageProductSourceAddorModifyBackupSourceDialog::OnInitDialog()
{
  CAddOrModifyDialog::OnInitDialog();

  // TODO:  Add extra initialization here
  if(!m_bAddDialog)
  {
    GetDlgItem(IDC_BUTTON_CONTINUEADD)->ShowWindow(FALSE);
  }

  m_type = DIALOG_BACKUP_SOURCE;

  return TRUE;  // return TRUE unless you set the focus to a control
  // EXCEPTION: OCX Property Pages should return FALSE
}

void CManageProductSourceAddorModifyBackupSourceDialog::TimerProcess()
{
  // ÃÓ≥‰ΩÁ√Ê
  if(m_pData)
  {
    CString strContent;

    strContent.Format("%s", reinterpret_cast<BackupSource*>(m_pData)->ip_address().c_str());
    GetDlgItem(IDC_EDIT_ip)->SetWindowText(strContent);

    strContent.Format("%d", reinterpret_cast<BackupSource*>(m_pData)->port());
    GetDlgItem(IDC_EDIT_port)->SetWindowText(strContent);

    strContent.Format("%s", reinterpret_cast<BackupSource*>(m_pData)->user_name().c_str());
    GetDlgItem(IDC_EDIT_USERNAME)->SetWindowText(strContent);

    strContent.Format("%s", reinterpret_cast<BackupSource*>(m_pData)->pass_word().c_str());
    GetDlgItem(IDC_EDIT_PASSWORD)->SetWindowText(strContent);
  }
}
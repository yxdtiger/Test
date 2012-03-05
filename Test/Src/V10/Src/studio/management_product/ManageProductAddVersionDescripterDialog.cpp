// ManageProductAddVersionDescripterDialog.cpp : implementation file
//

#include "../main_frame/StdAfx.h"
#include "ManageProductAddVersionDescripterDialog.h"

using namespace wmdf;

// CManageProductAddVersionDescripterDialog dialog

IMPLEMENT_DYNAMIC(CManageProductAddVersionDescripterDialog, CAddOrModifyDialog)

CManageProductAddVersionDescripterDialog::CManageProductAddVersionDescripterDialog(CWnd* pParent /* = NULL */, bool bDialogType /* = true */)
: CAddOrModifyDialog(CManageProductAddVersionDescripterDialog::IDD, pParent, bDialogType)
{
}

CManageProductAddVersionDescripterDialog::~CManageProductAddVersionDescripterDialog()
{
}

void CManageProductAddVersionDescripterDialog::DoDataExchange(CDataExchange* pDX)
{
  CAddOrModifyDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CManageProductAddVersionDescripterDialog, CAddOrModifyDialog)
END_MESSAGE_MAP()


// CManageProductAddVersionDescripterDialog message handlers

void CManageProductAddVersionDescripterDialog::FillItem()
{
  if(!m_pData)
    m_pData = new VersionDescripter;

  CString strText;
  // get major version
  GetDlgItem(IDC_EDIT_MAJOR_VERSION)->GetWindowText(strText);
  int major_version = atoi(strText.GetBuffer(0));
  strText.ReleaseBuffer();
  reinterpret_cast<VersionDescripter*>(m_pData)->set_major_version(static_cast<int8_t>(major_version));

  // get minor version
  GetDlgItem(IDC_EDIT_MINOR_VERSION)->GetWindowText(strText);
  int minor_version = atoi(strText.GetBuffer(0));
  strText.ReleaseBuffer();
  reinterpret_cast<VersionDescripter*>(m_pData)->set_minor_version(static_cast<int8_t>(minor_version));

  // 先清除
  reinterpret_cast<VersionDescripter*>(m_pData)->clear_codecs();

  if( reinterpret_cast<CButton*>(GetDlgItem(IDC_CHECK_FAST))->GetCheck() == BST_CHECKED )
    reinterpret_cast<VersionDescripter*>(m_pData)->add_support_codecs(CT_FAST);

  if( reinterpret_cast<CButton*>(GetDlgItem(IDC_CHECK_BINARY))->GetCheck() == BST_CHECKED )
    reinterpret_cast<VersionDescripter*>(m_pData)->add_support_codecs(CT_BINARY);

  if( reinterpret_cast<CButton*>(GetDlgItem(IDC_CHECK_RELAY))->GetCheck() == BST_CHECKED )
    reinterpret_cast<VersionDescripter*>(m_pData)->add_support_codecs(CT_RAWDATA);
}

BOOL CManageProductAddVersionDescripterDialog::OnInitDialog()
{
  CAddOrModifyDialog::OnInitDialog();

  // TODO:  Add extra initialization here

  // 如果是修改框，不能修改产品ID
  if(!m_bAddDialog)
  {
    GetDlgItem(IDC_EDIT_MAJOR_VERSION)->EnableWindow(FALSE);
    GetDlgItem(IDC_EDIT_MINOR_VERSION)->EnableWindow(FALSE);
    GetDlgItem(IDC_BUTTON_CONTINUEADD)->ShowWindow(FALSE);
  }

  m_type = DIALOG_VERSION_DESCRIPTER;

  return TRUE;  // return TRUE unless you set the focus to a control
  // EXCEPTION: OCX Property Pages should return FALSE
}

void CManageProductAddVersionDescripterDialog::TimerProcess()
{
  // 填充界面
  if(m_pData)
  {
    CString strContent;
    strContent.Format("%d", reinterpret_cast<VersionDescripter*>(m_pData)->major_version());
    GetDlgItem(IDC_EDIT_MAJOR_VERSION)->SetWindowText(strContent);

    strContent.Format("%d", reinterpret_cast<VersionDescripter*>(m_pData)->minor_version());
    GetDlgItem(IDC_EDIT_MINOR_VERSION)->SetWindowText(strContent);

    for(int32_t i = 0; i < reinterpret_cast<VersionDescripter*>(m_pData)->support_codecs_count(); ++i)
    {
      CodecType ct = reinterpret_cast<VersionDescripter*>(m_pData)->support_codecs(i);

      switch(ct)
      {
      case CT_FAST:
        reinterpret_cast<CButton*>(GetDlgItem(IDC_CHECK_FAST))->SetCheck(1);
        break;
      case CT_BINARY:
        reinterpret_cast<CButton*>(GetDlgItem(IDC_CHECK_BINARY))->SetCheck(1);
        break;
      case CT_RAWDATA:
        reinterpret_cast<CButton*>(GetDlgItem(IDC_CHECK_RELAY))->SetCheck(1);
        break;
      default:
        break;
      }
    }
  }
}

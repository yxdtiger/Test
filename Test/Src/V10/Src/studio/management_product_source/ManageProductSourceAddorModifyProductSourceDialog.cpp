// ManageProductSourceAddorModifyProductSourceDialog.cpp : implementation file
//

#include "../main_frame/StdAfx.h"
#include "ManageProductSourceAddorModifyProductSourceDialog.h"

using namespace wmdf;

// CManageProductSourceAddorModifyProductSourceDialog dialog

IMPLEMENT_DYNAMIC(CManageProductSourceAddorModifyProductSourceDialog, CDialog)

CManageProductSourceAddorModifyProductSourceDialog::CManageProductSourceAddorModifyProductSourceDialog(CWnd* pParent /* = NULL */, bool bDialogType /* = true */)
: CAddOrModifyDialog(CManageProductSourceAddorModifyProductSourceDialog::IDD, pParent, bDialogType)
{
}

CManageProductSourceAddorModifyProductSourceDialog::~CManageProductSourceAddorModifyProductSourceDialog()
{
}

BEGIN_MESSAGE_MAP(CManageProductSourceAddorModifyProductSourceDialog, CDialog)
  ON_BN_CLICKED(IDOK, &CManageProductSourceAddorModifyProductSourceDialog::OnBnClickedOk)
  ON_BN_CLICKED(IDC_BUTTON_CONTINUEADD, &CManageProductSourceAddorModifyProductSourceDialog::OnBnClickedSaveAdd)
  ON_WM_TIMER()
END_MESSAGE_MAP()


// CManageProductSourceAddorModifyProductSourceDialog message handlers

void CManageProductSourceAddorModifyProductSourceDialog::FillItem()
{
  if(!m_pData)
    m_pData = new ProductSource;

  CString strContent;
  GetDlgItem(IDC_EDIT_PRODUCT_ID)->GetWindowText(strContent);
  reinterpret_cast<ProductSource*>(m_pData)->set_product_id( static_cast<int16_t>(atoi(strContent.GetBuffer(0))) );
  strContent.ReleaseBuffer();

  GetDlgItem(IDC_EDIT_ip)->GetWindowText(strContent);
  reinterpret_cast<ProductSource*>(m_pData)->set_ip_address(strContent.GetBuffer(0));
  strContent.ReleaseBuffer();

  GetDlgItem(IDC_EDIT_port)->GetWindowText(strContent);
  reinterpret_cast<ProductSource*>(m_pData)->set_port( static_cast<int32_t>(atoi(strContent.GetBuffer(0))) );
  strContent.ReleaseBuffer();

  GetDlgItem(IDC_EDIT_CODEC)->GetWindowText(strContent);
  reinterpret_cast<ProductSource*>(m_pData)->set_codec_type( (CodecType)static_cast<int32_t>(atoi(strContent.GetBuffer(0))) );
  strContent.ReleaseBuffer();

  GetDlgItem(IDC_EDIT_USERNAME)->GetWindowText(strContent);
  reinterpret_cast<ProductSource*>(m_pData)->set_user_name(strContent.GetBuffer(0));
  strContent.ReleaseBuffer();

  GetDlgItem(IDC_EDIT_PASSWORD)->GetWindowText(strContent);
  reinterpret_cast<ProductSource*>(m_pData)->set_pass_word(strContent.GetBuffer(0));
  strContent.ReleaseBuffer();

  GetDlgItem(IDC_EDIT_DESCRIPTION)->GetWindowText(strContent);
  reinterpret_cast<ProductSource*>(m_pData)->set_description(strContent.GetBuffer(0));
  strContent.ReleaseBuffer();
}

BOOL CManageProductSourceAddorModifyProductSourceDialog::OnInitDialog()
{
  CAddOrModifyDialog::OnInitDialog();

  // TODO:  Add extra initialization here
  if(!m_bAddDialog)
  {
    GetDlgItem(IDC_EDIT_PRODUCT_ID)->EnableWindow(FALSE);
    GetDlgItem(IDC_BUTTON_CONTINUEADD)->ShowWindow(FALSE);
  }

  m_type = DIALOG_PRODUCT_SOURCE;

  return TRUE;  // return TRUE unless you set the focus to a control
  // EXCEPTION: OCX Property Pages should return FALSE
}

void CManageProductSourceAddorModifyProductSourceDialog::TimerProcess()
{
  // ÃÓ≥‰ΩÁ√Ê
  if(m_pData)
  {
    CString strContent;
    strContent.Format("%d", reinterpret_cast<ProductSource*>(m_pData)->product_id());
    GetDlgItem(IDC_EDIT_PRODUCT_ID)->SetWindowText(strContent);

    strContent.Format("%s", reinterpret_cast<ProductSource*>(m_pData)->ip_address().c_str());
    GetDlgItem(IDC_EDIT_ip)->SetWindowText(strContent);

    strContent.Format("%d", reinterpret_cast<ProductSource*>(m_pData)->port());
    GetDlgItem(IDC_EDIT_port)->SetWindowText(strContent);

    strContent.Format("%d", reinterpret_cast<ProductSource*>(m_pData)->codec_type());
    GetDlgItem(IDC_EDIT_CODEC)->SetWindowText(strContent);

    strContent.Format("%s", reinterpret_cast<ProductSource*>(m_pData)->user_name().c_str());
    GetDlgItem(IDC_EDIT_USERNAME)->SetWindowText(strContent);

    strContent.Format("%s", reinterpret_cast<ProductSource*>(m_pData)->pass_word().c_str());
    GetDlgItem(IDC_EDIT_PASSWORD)->SetWindowText(strContent);

    strContent.Format("%s", reinterpret_cast<ProductSource*>(m_pData)->description().c_str());
    GetDlgItem(IDC_EDIT_DESCRIPTION)->SetWindowText(strContent);
  }
}
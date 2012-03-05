// ManageProductSourceAddorModifySubscribedProductDialog.cpp : implementation file
//

#include "../main_frame/StdAfx.h"
#include "ManageProductSourceAddorModifySubscribedProductDialog.h"

using namespace wmdf;

// CManageProductSourceAddorModifySubscribedProductDialog dialog

IMPLEMENT_DYNAMIC(CManageProductSourceAddorModifySubscribedProductDialog, CDialog)

CManageProductSourceAddorModifySubscribedProductDialog::CManageProductSourceAddorModifySubscribedProductDialog(CWnd* pParent /* = NULL */, bool bDialogType /* = true */)
: CAddOrModifyDialog(CManageProductSourceAddorModifySubscribedProductDialog::IDD, pParent, bDialogType)
{
}

CManageProductSourceAddorModifySubscribedProductDialog::~CManageProductSourceAddorModifySubscribedProductDialog()
{
}

BEGIN_MESSAGE_MAP(CManageProductSourceAddorModifySubscribedProductDialog, CDialog)
  ON_BN_CLICKED(IDOK, &CManageProductSourceAddorModifySubscribedProductDialog::OnBnClickedOk)
  ON_BN_CLICKED(IDC_BUTTON_CONTINUEADD, &CManageProductSourceAddorModifySubscribedProductDialog::OnBnClickedSaveAdd)
  ON_WM_TIMER()
END_MESSAGE_MAP()


// CManageProductSourceAddorModifySubscribedProductDialog message handlers

void CManageProductSourceAddorModifySubscribedProductDialog::FillItem()
{
  if(!m_pData)
    m_pData = new ProtocolDescription;

  CString strContent;
  GetDlgItem(IDC_EDIT_PRODUCT_ID)->GetWindowText(strContent);
  reinterpret_cast<ProtocolDescription*>(m_pData)->set_product_id( static_cast<int16_t>(atoi(strContent.GetBuffer(0))) );
  strContent.ReleaseBuffer();

  GetDlgItem(IDC_EDIT_MAJOR_VERSION)->GetWindowText(strContent);
  reinterpret_cast<ProtocolDescription*>(m_pData)->set_major_version( static_cast<int8_t>(atoi(strContent.GetBuffer(0))) );
  strContent.ReleaseBuffer();

  GetDlgItem(IDC_EDIT_MINOR_VERSION)->GetWindowText(strContent);
  reinterpret_cast<ProtocolDescription*>(m_pData)->set_minor_version( static_cast<int8_t>(atoi(strContent.GetBuffer(0))) );
  strContent.ReleaseBuffer();

  GetDlgItem(IDC_EDIT_CODEC)->GetWindowText(strContent);
  reinterpret_cast<ProtocolDescription*>(m_pData)->set_codec_type( static_cast<CodecType>(atoi(strContent.GetBuffer(0))) );
  strContent.ReleaseBuffer();
}

BOOL CManageProductSourceAddorModifySubscribedProductDialog::OnInitDialog()
{
  CAddOrModifyDialog::OnInitDialog();

  // TODO:  Add extra initialization here
  if(!m_bAddDialog)
  {
    GetDlgItem(IDC_BUTTON_CONTINUEADD)->ShowWindow(FALSE);
  }

  m_type = DIALOG_SUBSCRIBED_PRODUCT;

  return TRUE;  // return TRUE unless you set the focus to a control
  // EXCEPTION: OCX Property Pages should return FALSE
}

void CManageProductSourceAddorModifySubscribedProductDialog::TimerProcess()
{
  // ÃÓ≥‰ΩÁ√Ê
  if(m_pData)
  {
    CString strContent;
    strContent.Format("%d", reinterpret_cast<ProtocolDescription*>(m_pData)->product_id());
    GetDlgItem(IDC_EDIT_PRODUCT_ID)->SetWindowText(strContent);

    strContent.Format("%d", reinterpret_cast<ProtocolDescription*>(m_pData)->major_version());
    GetDlgItem(IDC_EDIT_MAJOR_VERSION)->SetWindowText(strContent);

    strContent.Format("%d", reinterpret_cast<ProtocolDescription*>(m_pData)->minor_version());
    GetDlgItem(IDC_EDIT_MINOR_VERSION)->SetWindowText(strContent);

    strContent.Format("%d", reinterpret_cast<ProtocolDescription*>(m_pData)->codec_type());
    GetDlgItem(IDC_EDIT_CODEC)->SetWindowText(strContent);
  }
}
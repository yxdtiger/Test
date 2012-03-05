// ManageProductAddProductDescripterDialog.cpp : implementation file
//

#include "../main_frame/StdAfx.h"
#include "ManageProductAddProductDescripterDialog.h"

using namespace wmdf;

// CManageProductAddProductDescripterDialog dialog

IMPLEMENT_DYNAMIC(CManageProductAddProductDescripterDialog, CAddOrModifyDialog)

CManageProductAddProductDescripterDialog::CManageProductAddProductDescripterDialog(CWnd* pParent /* = NULL */, bool bDialogType /* = true */)
: CAddOrModifyDialog(CManageProductAddProductDescripterDialog::IDD, pParent, bDialogType)
{
}

CManageProductAddProductDescripterDialog::~CManageProductAddProductDescripterDialog()
{
}

void CManageProductAddProductDescripterDialog::DoDataExchange(CDataExchange* pDX)
{
  CAddOrModifyDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CManageProductAddProductDescripterDialog, CAddOrModifyDialog)
END_MESSAGE_MAP()


// CManageProductAddProductDescripterDialog message handlers

void CManageProductAddProductDescripterDialog::FillItem()
{
  if(!m_pData)
    m_pData = new ProductDescripter;

  CString strText;
  // get product id
  GetDlgItem(IDC_EDIT_PRODUCT_ID)->GetWindowText(strText);
  int nTemp = atoi(strText.GetBuffer(0));
  strText.ReleaseBuffer();
  reinterpret_cast<ProductDescripter*>(m_pData)->set_product_id(static_cast<int16_t>(nTemp));

  // get product name
  GetDlgItem(IDC_EDIT_PRODUCT_NAME)->GetWindowText(strText);
  reinterpret_cast<ProductDescripter*>(m_pData)->set_product_name(strText.GetBuffer(0));
  strText.ReleaseBuffer();

  // get product supplier
  GetDlgItem(IDC_EDIT_SUPPLIER)->GetWindowText(strText);
  reinterpret_cast<ProductDescripter*>(m_pData)->set_supplier(strText.GetBuffer(0));
  strText.ReleaseBuffer();

  // get product description
  GetDlgItem(IDC_EDIT_DESCRIPTION)->GetWindowText(strText);
  reinterpret_cast<ProductDescripter*>(m_pData)->set_description(strText.GetBuffer(0));
  strText.ReleaseBuffer();
}
BOOL CManageProductAddProductDescripterDialog::OnInitDialog()
{
  CAddOrModifyDialog::OnInitDialog();

  // TODO:  Add extra initialization here

  // 如果是修改框，不能修改产品ID
  if(!m_bAddDialog)
  {
    GetDlgItem(IDC_EDIT_PRODUCT_ID)->EnableWindow(FALSE);
    GetDlgItem(IDC_BUTTON_CONTINUEADD)->ShowWindow(FALSE);
  }

  m_type = DIALOG_PRODUCT_DESCRIPTER;

  return TRUE;  // return TRUE unless you set the focus to a control
  // EXCEPTION: OCX Property Pages should return FALSE
}

void CManageProductAddProductDescripterDialog::TimerProcess()
{
  // 填充界面
  if(m_pData)
  {
    CString strContent;
    strContent.Format("%d", reinterpret_cast<ProductDescripter*>(m_pData)->product_id());
    GetDlgItem(IDC_EDIT_PRODUCT_ID)->SetWindowText(strContent);

    strContent.Format("%s", reinterpret_cast<ProductDescripter*>(m_pData)->product_name().c_str());
    GetDlgItem(IDC_EDIT_PRODUCT_NAME)->SetWindowText(strContent);

    strContent.Format("%s", reinterpret_cast<ProductDescripter*>(m_pData)->supplier().c_str());
    GetDlgItem(IDC_EDIT_SUPPLIER)->SetWindowText(strContent);

    strContent.Format("%s", reinterpret_cast<ProductDescripter*>(m_pData)->description().c_str());
    GetDlgItem(IDC_EDIT_DESCRIPTION)->SetWindowText(strContent);
  }
}
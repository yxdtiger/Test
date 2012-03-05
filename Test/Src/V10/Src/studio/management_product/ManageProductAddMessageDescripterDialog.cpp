// ManageProductAddMessageDescripterDialog.cpp : implementation file
//

#include "../main_frame/StdAfx.h"
#include "ManageProductAddMessageDescripterDialog.h"

using namespace wmdf;

// CManageProductAddMessageDescripterDialog dialog

IMPLEMENT_DYNAMIC(CManageProductAddMessageDescripterDialog, CAddOrModifyDialog)

CManageProductAddMessageDescripterDialog::CManageProductAddMessageDescripterDialog(CWnd* pParent /* = NULL */, bool bDialogType /* = true */)
: CAddOrModifyDialog(CManageProductAddMessageDescripterDialog::IDD, pParent, bDialogType)
{

}

CManageProductAddMessageDescripterDialog::~CManageProductAddMessageDescripterDialog()
{
}

void CManageProductAddMessageDescripterDialog::DoDataExchange(CDataExchange* pDX)
{
  CAddOrModifyDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CManageProductAddMessageDescripterDialog, CAddOrModifyDialog)
END_MESSAGE_MAP()


// CManageProductAddMessageDescripterDialog message handlers

void CManageProductAddMessageDescripterDialog::FillItem()
{
  if(!m_pData)
    m_pData = new MessageDescripter;

  CString strText;
  // get product id
  GetDlgItem(IDC_EDIT_MESSAGE_ID)->GetWindowText(strText);
  int message_id = atoi(strText.GetBuffer(0));
  strText.ReleaseBuffer();
  reinterpret_cast<MessageDescripter*>(m_pData)->set_message_id( static_cast<int16_t>(message_id) );

  // get product description
  GetDlgItem(IDC_EDIT_DESCRIPTION)->GetWindowText(strText);
  reinterpret_cast<MessageDescripter*>(m_pData)->set_message_description( strText.GetBuffer(0) );
  strText.ReleaseBuffer();
}
BOOL CManageProductAddMessageDescripterDialog::OnInitDialog()
{
  CAddOrModifyDialog::OnInitDialog();

  // TODO:  Add extra initialization here

  // 如果是修改框，不能修改产品ID
  if(!m_bAddDialog)
  {
    GetDlgItem(IDC_EDIT_MESSAGE_ID)->EnableWindow(FALSE);
    GetDlgItem(IDC_BUTTON_CONTINUEADD)->ShowWindow(FALSE);
  }

  m_type = DIALOG_MESSAGE_DESCRIPTER;

  return TRUE;  // return TRUE unless you set the focus to a control
  // EXCEPTION: OCX Property Pages should return FALSE
}

void CManageProductAddMessageDescripterDialog::TimerProcess()
{
  // 填充界面
  if(m_pData)
  {
    CString strContent;
    strContent.Format("%d", reinterpret_cast<MessageDescripter*>(m_pData)->message_id());
    GetDlgItem(IDC_EDIT_MESSAGE_ID)->SetWindowText(strContent);

    strContent.Format("%s", reinterpret_cast<MessageDescripter*>(m_pData)->message_description().c_str());
    GetDlgItem(IDC_EDIT_DESCRIPTION)->SetWindowText(strContent);
  }
}
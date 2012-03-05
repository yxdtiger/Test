// ManageProductSourceSelectDialog.cpp : implementation file
//

#include "../main_frame/StdAfx.h"
#include "ManageProductSourceSelectDialog.h"

// CManageProductSourceSelectDialog dialog

IMPLEMENT_DYNAMIC(CManageProductSourceSelectDialog, CDialog)

CManageProductSourceSelectDialog::CManageProductSourceSelectDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CManageProductSourceSelectDialog::IDD, pParent)
  , m_nSelectType(0)
{

}

CManageProductSourceSelectDialog::~CManageProductSourceSelectDialog()
{
}

void CManageProductSourceSelectDialog::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  DDX_Radio(pDX, IDC_RADIO1, m_nSelectType);
  DDX_Control(pDX, IDOK, m_conButton_ok);
  DDX_Control(pDX, IDCANCEL, m_conButton_cancel);
}


BEGIN_MESSAGE_MAP(CManageProductSourceSelectDialog, CDialog)
  ON_BN_CLICKED(IDOK, &CManageProductSourceSelectDialog::OnBnClickedOk)
  ON_BN_CLICKED(IDCANCEL, &CManageProductSourceSelectDialog::OnBnClickedCancel)
END_MESSAGE_MAP()


// CManageProductSourceSelectDialog message handlers

void CManageProductSourceSelectDialog::OnBnClickedOk()
{
  // TODO: Add your control notification handler code here

  OnOK();
}

void CManageProductSourceSelectDialog::OnBnClickedCancel()
{
  // TODO: Add your control notification handler code here

  OnCancel();
}
// AddOrModifyDialog.cpp : implementation file
//

#include "../main_frame/StdAfx.h"
#include "AddOrModifyDialog.h"


// CAddOrModifyDialog dialog

IMPLEMENT_DYNAMIC(CAddOrModifyDialog, CDialog)

CAddOrModifyDialog::CAddOrModifyDialog(UINT IDD, CWnd* pParent /* = NULL */, bool bDialogType /* = true */)
	: CDialog(IDD, pParent)
  , m_bAddDialog(bDialogType)
{
  m_pData = NULL;
  m_title = "产品描述";
}

CAddOrModifyDialog::~CAddOrModifyDialog()
{
}

void CAddOrModifyDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
  DDX_Control(pDX, IDC_BUTTON_CONTINUEADD, m_conButton_saveadd);
  DDX_Control(pDX, IDOK, m_conButton_ok);
  DDX_Control(pDX, IDCANCEL, m_conButton_cancel);
}


BEGIN_MESSAGE_MAP(CAddOrModifyDialog, CDialog)
  ON_BN_CLICKED(IDOK, &CAddOrModifyDialog::OnBnClickedOk)
  ON_BN_CLICKED(IDCANCEL, &CAddOrModifyDialog::OnBnClickedCancel)
  ON_BN_CLICKED(IDC_BUTTON_CONTINUEADD, &CAddOrModifyDialog::OnBnClickedSaveAdd)
  ON_WM_TIMER()
END_MESSAGE_MAP()


// CAddOrModifyDialog message handlers

void CAddOrModifyDialog::OnBnClickedOk()
{
  // TODO: Add your control notification handler code here
  AddItem();

  __super::OnOK();
}

void CAddOrModifyDialog::OnBnClickedCancel()
{
  // TODO: Add your control notification handler code here
  __super::OnCancel();
}

void CAddOrModifyDialog::OnBnClickedSaveAdd()
{
  // TODO: Add your control notification handler code here
  AddItem();

  // 数据初始化
  Reset();
}

void CAddOrModifyDialog::Reset()
{
  m_pData = NULL;
}

void CAddOrModifyDialog::AddItem()
{
  FillItem();

  ItemParam* pIP = new ItemParam;
  pIP->pData = m_pData;
  pIP->bAddDialog = m_bAddDialog;
  pIP->type = m_type;

  m_pParentWnd->SendMessage( WM_ADD_MODIFY_ITEM, reinterpret_cast<WPARAM>(pIP), 0 );
}

void CAddOrModifyDialog::OnTimer(UINT_PTR nIDEvent)
{
  // TODO: Add your message handler code here and/or call default
  TimerProcess();

  KillTimer(1);

  CDialog::OnTimer(nIDEvent);
}

BOOL CAddOrModifyDialog::OnInitDialog()
{  
  CDialog::OnInitDialog();

  // TODO:  Add extra initialization here
  SetTimer(1, 100, NULL);
  SetWindowText(m_title);
  return TRUE;  // return TRUE unless you set the focus to a control
  // EXCEPTION: OCX Property Pages should return FALSE
}

void CAddOrModifyDialog::TimerProcess()
{

}

void CAddOrModifyDialog::WnSetWindowText( CString s )
{
  m_title = s;
}

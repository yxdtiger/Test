// ManageProductSourceAddorModifyMarketTimeDialog.cpp : implementation file
//

#include "../main_frame/StdAfx.h"
#include "ManageProductSourceAddorModifyMarketTimeDialog.h"

using namespace wmdf;

// CManageProductSourceAddorModifyMarketTimeDialog dialog

IMPLEMENT_DYNAMIC(CManageProductSourceAddorModifyMarketTimeDialog, CAddOrModifyDialog)

CManageProductSourceAddorModifyMarketTimeDialog::CManageProductSourceAddorModifyMarketTimeDialog(CWnd* pParent /* = NULL */, bool bDialogType /* = true */)
: CAddOrModifyDialog(CManageProductSourceAddorModifyMarketTimeDialog::IDD, pParent, bDialogType)
{
  m_pData = NULL;
}

CManageProductSourceAddorModifyMarketTimeDialog::~CManageProductSourceAddorModifyMarketTimeDialog()
{
}

BEGIN_MESSAGE_MAP(CManageProductSourceAddorModifyMarketTimeDialog, CDialog)
  ON_BN_CLICKED(IDOK, &CManageProductSourceAddorModifyMarketTimeDialog::OnBnClickedOk)
  ON_BN_CLICKED(IDC_BUTTON_CONTINUEADD, &CManageProductSourceAddorModifyMarketTimeDialog::OnBnClickedSaveAdd)
  ON_WM_TIMER()
END_MESSAGE_MAP()


// CManageProductSourceAddorModifyMarketTimeDialog message handlers

void CManageProductSourceAddorModifyMarketTimeDialog::FillItem()
{
  if(!m_pData)
    m_pData = new MarketTime;

  CString strContent;
  GetDlgItem(IDC_EDIT_BEGIN_TIME)->GetWindowText(strContent);
  reinterpret_cast<MarketTime*>(m_pData)->set_begin_time( static_cast<int32_t>(atoi(strContent.GetBuffer(0))) );
  strContent.ReleaseBuffer();

  GetDlgItem(IDC_EDIT_END_TIME)->GetWindowText(strContent);
  reinterpret_cast<MarketTime*>(m_pData)->set_end_time( static_cast<int32_t>(atoi(strContent.GetBuffer(0))) );
  strContent.ReleaseBuffer();
}

BOOL CManageProductSourceAddorModifyMarketTimeDialog::OnInitDialog()
{
  CAddOrModifyDialog::OnInitDialog();

  // TODO:  Add extra initialization here
  if(!m_bAddDialog)
  {
    GetDlgItem(IDC_BUTTON_CONTINUEADD)->ShowWindow(FALSE);
  }

  m_type = DIALOG_MARKET_TIME;

  return TRUE;  // return TRUE unless you set the focus to a control
  // EXCEPTION: OCX Property Pages should return FALSE
}

void CManageProductSourceAddorModifyMarketTimeDialog::TimerProcess()
{
  // ÃÓ≥‰ΩÁ√Ê
  if(m_pData)
  {
    CString strContent;
    strContent.Format("%d", reinterpret_cast<MarketTime*>(m_pData)->begin_time());
    GetDlgItem(IDC_EDIT_BEGIN_TIME)->SetWindowText(strContent);

    strContent.Format("%d", reinterpret_cast<MarketTime*>(m_pData)->end_time());
    GetDlgItem(IDC_EDIT_END_TIME)->SetWindowText(strContent);
  }
}
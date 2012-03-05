// UserSerachDlg.cpp : implementation file
//

#include "../main_frame/StdAfx.h"
#include "../main_frame/WMDF_Studio.h"
#include "UserSerachDlg.h"


// CUserSerachDlg dialog

IMPLEMENT_DYNAMIC(CUserSerachDlg, CDialog)

CUserSerachDlg::CUserSerachDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CUserSerachDlg::IDD, pParent)
{

}

CUserSerachDlg::~CUserSerachDlg()
{
}

void CUserSerachDlg::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  DDX_Control(pDX, IDC_BUTTON1, m_conButton_find_next);
  DDX_Control(pDX, IDC_BUTTON2, m_conButton_count);
  DDX_Control(pDX, IDOK, m_conButton_close);
}


BEGIN_MESSAGE_MAP(CUserSerachDlg, CDialog)
  ON_BN_CLICKED(IDOK, &CUserSerachDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CUserSerachDlg message handlers

void CUserSerachDlg::OnBnClickedOk()
{
  // TODO: Add your control notification handler code here
}

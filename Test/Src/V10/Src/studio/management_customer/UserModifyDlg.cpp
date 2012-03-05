// UserModifyDlg.cpp : implementation file
//

#include "../main_frame/StdAfx.h"
#include "../main_frame/WMDF_Studio.h"
#include "UserModifyDlg.h"


// CUserModifyDlg dialog

IMPLEMENT_DYNAMIC(CUserModifyDlg, CDialog)

CUserModifyDlg::CUserModifyDlg(CWnd* pParent /*=NULL*/)
	: CUserAddDlg(pParent)
{

}

CUserModifyDlg::~CUserModifyDlg()
{
}

void CUserModifyDlg::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  DDX_Control(pDX, IDC_TREE_PRODUCT, m_con_treectrl);
}


BEGIN_MESSAGE_MAP(CUserModifyDlg, CDialog)
END_MESSAGE_MAP()


// CUserModifyDlg message handlers

BOOL CUserModifyDlg::OnInitDialog()
{
  CUserAddDlg::OnInitDialog();

  // TODO:  Add extra initialization here
 
  return TRUE;  // return TRUE unless you set the focus to a control
  // EXCEPTION: OCX Property Pages should return FALSE
}

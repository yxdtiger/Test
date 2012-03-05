// UserSerachDlg.cpp : implementation file
//

#include "../main_frame/StdAfx.h"
#include "../main_frame/WMDF_Studio.h"
#include "MonitorBaseDlg.h"


// CMonitorBaseDlg dialog

IMPLEMENT_DYNAMIC(CMonitorBaseDlg, CDialog)

CMonitorBaseDlg::CMonitorBaseDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMonitorBaseDlg::IDD, pParent)
{

}

CMonitorBaseDlg::~CMonitorBaseDlg()
{
}

void CMonitorBaseDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CMonitorBaseDlg, CDialog)
END_MESSAGE_MAP()


// CMonitorBaseDlg message handlers

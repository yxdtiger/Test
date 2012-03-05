#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include "afxext.h"
#include "../main_frame/resource.h"
#include "../main_frame/StdAfx.h"
#include "wmdf_invoke.h"
#include "server_manager.h"
#include "MonitorClientCtrl.h"
#include "MonitorTransferCtrl.h"
// Monitor_Overview dialog
using namespace wmdf;

class CMonitorOverviewDialog : public CDialog
{
  DECLARE_EASYSIZE
	DECLARE_DYNAMIC(CMonitorOverviewDialog)

public:
	CMonitorOverviewDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~CMonitorOverviewDialog();

// Dialog Data
	enum { IDD = IDD_Monitor_OverView };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
  virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
public:
  afx_msg void OnBnClickedOk();
  afx_msg void OnAddServer(); 
  afx_msg void OnDeleteSelectedServer();   
  afx_msg void OnConnectAll(); 
  afx_msg void OnNMRClickServerList(NMHDR *pNMHDR, LRESULT *pResult);
  afx_msg void OnTransferMonitor();
  afx_msg void OnClientMonitor();  
  void ReloadServerList();

  CListCtrl m_server_list;
  CToolBar   m_wndToolBar;   
  CImageList m_toolBarImageList;
  WmdfInvoke* invoker_;
private:
  CMonitorClientCtrl* monitor_client_dlg_;
  CMonitorTransferCtrl* monitor_transfer_dlg_;
public:
  afx_msg void OnPaint();
  afx_msg void OnSize(UINT nType, int cx, int cy);
  afx_msg void OnSizing(UINT fwSide, LPRECT pRect);
  afx_msg void OnBnClickedButton1();
};

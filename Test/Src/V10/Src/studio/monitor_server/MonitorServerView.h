#pragma once



// MonitorServerView form view
#include "../main_frame/StdAfx.h"
#include "../main_frame/resource.h"
#include "../monitor/server_manager.h"
#include "../monitor/wmdf_invoke.h"
#include "../monitor/MonitorClientCtrl.h"
#include "../monitor/MonitorTransferCtrl.h"
#include "NetworkEditor/NetworkEditor.h"
#include "NetworkEditor/NetworkEntityContainer.h"

class MonitorServerView : public CFormView
{
	DECLARE_DYNCREATE(MonitorServerView)

protected:
	MonitorServerView();           // protected constructor used by dynamic creation
	virtual ~MonitorServerView();

public:
	enum { IDD = IDD_Monitor_OverView };
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
  virtual void OnInitialUpdate();
  
	DECLARE_MESSAGE_MAP()
public:
  CNetworkEntityContainer* GetData() { return &m_objs; }
  afx_msg void OnDestroy();

  afx_msg void OnAddServer(); 
  afx_msg void OnDeleteSelectedServer();   
  afx_msg void OnConnectAll(); 
  afx_msg void OnNMRClickServerList(NMHDR *pNMHDR, LRESULT *pResult);
  afx_msg void OnTransferMonitor();
  afx_msg void OnClientMonitor();  
  afx_msg void OnBnClickedButton1();
  void ReloadServerList();

  CListCtrl m_server_list;
  CToolBar   m_wndToolBar;   
  CImageList m_toolBarImageList;
  WmdfInvoke* invoker_;
private:
  CMonitorClientCtrl* monitor_client_dlg_;
  CMonitorTransferCtrl* monitor_transfer_dlg_;
  CNetworkEditor	m_editor;
public:
  afx_msg void OnSize(UINT nType, int cx, int cy);
  afx_msg void OnSizing(UINT fwSide, LPRECT pRect);
  afx_msg void OnPaint();
private:
  CNetworkEntityContainer	m_objs;
};
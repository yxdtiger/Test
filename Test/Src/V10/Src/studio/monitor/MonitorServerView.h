#pragma once



// MonitorServerView form view
#include "../main_frame/StdAfx.h"
#include "../main_frame/resource.h"
#include "server_manager.h"

using namespace wmdf;
class AddServerDialog;

class MonitorServerView : public CFormView
{
	DECLARE_DYNCREATE(MonitorServerView)

protected:
	MonitorServerView();           // protected constructor used by dynamic creation
	virtual ~MonitorServerView();

public:
	enum { IDD = IDD_MONITOR_OVERVIEW };  

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
  afx_msg void OnDestroy();

  afx_msg void OnAddServer(); 
  afx_msg void OnDeleteSelectedServer();   

  afx_msg void OnConnectAll();
  afx_msg void OnButtonModifyConnection();
  afx_msg void OnDisconnectAll();
  afx_msg void OnDisconnectThis();
  afx_msg void OnNMRClickServerList(NMHDR *pNMHDR, LRESULT *pResult);

  afx_msg void OnTransferMonitor();
  afx_msg void OnClientMonitor();
  afx_msg void OnLogMonitor();
  afx_msg void OnMonitorNetwork();
  afx_msg void OnQuoteMonitor();

  afx_msg void UpdateServerStatus();

  void ReloadServerList();

  void SetListItem( int i, ServerNetInfo* server_info ) ;
  
public:
  afx_msg void OnSize(UINT nType, int cx, int cy);
  afx_msg void OnTimer(UINT_PTR nIDEvent);
private:
  CButton m_sound_switch;
  

  CFont   m_Font; 
  CImageList    m_bmpImgList;                     //±£¥ÊBITMAP
  CListCtrl m_server_list;
  CToolBar   m_wndToolBar;   
  CImageList m_toolBarImageList;
  
  AddServerDialog* add_server_dialog_;
public:
  
  
  afx_msg void OnBnClickedSoundSwitch();
  afx_msg void OnClearWarnings();
  afx_msg BOOL OnToolTipText(UINT,NMHDR* pNMHDR,LRESULT* pResult);
  afx_msg void OnConnectThis();
};
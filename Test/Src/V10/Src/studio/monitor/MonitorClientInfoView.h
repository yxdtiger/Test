#pragma once



// MonitorClientInfoView form view
#include "../main_frame/StdAfx.h"
#include "../main_frame/resource.h"
#include "../monitor/server_manager.h"
#include "../monitor/monitor_invoke.h"
#include "../monitor/TreeList/TreeListCtrl.h"

class MonitorClientInfoView : public CFormView
{
	DECLARE_DYNCREATE(MonitorClientInfoView)

protected:
	MonitorClientInfoView();           // protected constructor used by dynamic creation
	virtual ~MonitorClientInfoView();

public:
	enum { IDD = IDD_MONITOR_CLIENT };
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
  CTreeListCtrl m_client_ctrl;
  afx_msg void OnTimer(UINT_PTR nIDEvent);

  void UpdateClientView();

  afx_msg void OnSize(UINT nType, int cx, int cy);
};

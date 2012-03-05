#pragma once



// MonitorTransferInfoView form view
#include "../main_frame/StdAfx.h"
#include "../main_frame/resource.h"
#include "../monitor/server_manager.h"
#include "../monitor/monitor_invoke.h"
#include "../monitor/TreeList/TreeListCtrl.h"

class MonitorTransferInfoView : public CFormView
{
  //DECLARE_EASYSIZE
	DECLARE_DYNCREATE(MonitorTransferInfoView)

protected:
	MonitorTransferInfoView();           // protected constructor used by dynamic creation
	virtual ~MonitorTransferInfoView();

public:
	enum { IDD = IDD_MONITOR_TRANSFER };
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
  afx_msg void OnNMDClick(NMHDR *pNMHDR, LRESULT *pResult);
  afx_msg void OnDestroy();
  afx_msg void OnTimer(UINT_PTR nIDEvent);

  void UpdateTransferView();

  afx_msg void OnSize(UINT nType, int cx, int cy);
  CTreeListCtrl m_transfer_ctrl;  
};

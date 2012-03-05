#pragma once



// LogBrowserView form view
#include "../main_frame/StdAfx.h"
#include "../main_frame/resource.h"

#include "../monitor/TreeList/TreeListCtrl.h"

class LogBrowserView : public CFormView
{
  //DECLARE_EASYSIZE
	DECLARE_DYNCREATE(LogBrowserView)

protected:
	LogBrowserView();           // protected constructor used by dynamic creation
	virtual ~LogBrowserView();

public:
	enum { IDD = IDD_MONITOR_LOG };
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
  afx_msg void OnSize(UINT nType, int cx, int cy);
  afx_msg void OnTimer(UINT_PTR nIDEvent);

  void UpdateLogView();


private:
  CTreeListCtrl m_log_ctrl;
  afx_msg LRESULT OnMyRefresh(WPARAM wParam, LPARAM lParam);
};

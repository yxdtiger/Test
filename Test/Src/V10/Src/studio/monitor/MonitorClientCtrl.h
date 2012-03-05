#pragma once
#include "TreeListCtrl.h"

#include "../main_frame/resource.h"
#include "afxcmn.h"

// MonitorClientCtrl dialog
#define 	IDC_TREELIST	123
#define 	USER_DATA_SIZE	256

class CMonitorClientCtrl : public CDialog
{
	DECLARE_DYNAMIC(CMonitorClientCtrl)

public:
	CMonitorClientCtrl(CWnd* pParent = NULL);   // standard constructor
	virtual ~CMonitorClientCtrl();

// Dialog Data
	enum { IDD = IDD_Monitor_Client };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
  virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
public:  
  afx_msg void OnBnClickedOk();
  afx_msg void OnTimer(UINT_PTR nIDEvent);
  CTreeListCtrl m_client_ctrl;
};

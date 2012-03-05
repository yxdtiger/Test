#pragma once

// AuditingCertView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CManageSystemConfigView form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

#include "../../environment/system_config.h"

class CManageSystemConfigView : public CFormView
{
protected:
	CManageSystemConfigView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CManageSystemConfigView)

// Form Data
public:
	//{{AFX_DATA(CManageSystemConfigView)
	enum { IDD = IDD_MANAGE_SYSTEM_CONFIG };
	//}}AFX_DATA

// Attributes
public:
  wmdf::SystemConfig m_system_config;

  // 是否处于修改状态
  bool m_bModifyState;

  // 控件关联变量
  int m_server_port;
  int m_server_thread_num;
  int m_max_connection_count;
  int m_heart_beat_interval;
  int m_max_cache_msg_count;
  int m_max_packet_size;

// Operations
public:

protected:
  void SetControlState();
  void FillSystemConfig();
  void SaveProductSourceToXmlFile(const std::string& file_path);


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CManageSystemConfigView)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CManageSystemConfigView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CManageSystemConfigView)
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
  afx_msg void OnBnClickedButtonOp();
  afx_msg void OnBnClickedButtonCancel();
};


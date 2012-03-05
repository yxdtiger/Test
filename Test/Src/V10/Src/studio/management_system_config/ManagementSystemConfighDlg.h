#pragma once

#include "../../environment/system_config.h"
#include "../common/BeautifulDialog.h"
#include "../common/BeautifulButton.h"
#include "afxwin.h"


// CManagementSystemConfigDlg dialog

class CManagementSystemConfigDlg : public CDialog
{
	DECLARE_DYNAMIC(CManagementSystemConfigDlg)

public:
	CManagementSystemConfigDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CManagementSystemConfigDlg();

// Dialog Data
	enum { IDD = IDD_MANAGE_SYSTEM_CONFIG };

  // Attributes
public:
  wmdf::SystemConfig* m_system_config;

  // 控件关联变量
  int m_server_port;
  int m_server_thread_num;
  int m_max_connection_count;
  int m_heart_beat_interval;
  int m_max_cache_msg_count;
  int m_max_packet_size;
  int m_monitor_server_port;
  int m_nMonitorServerState;
  int m_reset_hour;
  int m_preprocess_open;
  int m_process_close;
  int m_monitor_interval;
  bool m_flag;

protected:
  void FillSystemConfig();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
  virtual BOOL OnInitDialog();
  afx_msg void OnBnClickedOk();

  CButton m_conButton_ok;
  CButton m_conButton_cancel;
  
  
  CComboBox m_dest_server;
  afx_msg void OnBnClickedGetConfigButton();
  afx_msg void OnBnClickedUpdateConfigButton();


};

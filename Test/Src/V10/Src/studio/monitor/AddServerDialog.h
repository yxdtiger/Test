#pragma once
#include "afxcmn.h"
#include "../main_frame/resource.h"
#include "afxwin.h"

// AddServerDialog dialog

class AddServerDialog : public CDialog
{
	DECLARE_DYNAMIC(AddServerDialog)

public:
	AddServerDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~AddServerDialog();

// Dialog Data
	enum { IDD = IDD_DIALOG_ADD_SERVER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
  afx_msg void OnBnClickedOk();
  virtual BOOL OnInitDialog();
  
  CIPAddressCtrl m_server_ip;
  CComboBox m_server_type_combo;
  int m_monitor_port;
  int m_quote_port;    
  int server_id;
  CString m_monitor_id;
  CString m_quote_id;
  CString m_monitor_password;
  CString m_quote_password;
};

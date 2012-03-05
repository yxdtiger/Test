#pragma once

#include "afxcmn.h"
#include "afxwin.h"
#include "TreeListCtrl.h"
#include "../main_frame/resource.h"

// CMonitorTransferCtrl dialog
#define 	IDC_TRANSFER_TREELIST	124
class CMonitorTransferCtrl : public CDialog
{
	DECLARE_DYNAMIC(CMonitorTransferCtrl)

public:
	CMonitorTransferCtrl(CWnd* pParent = NULL);   // standard constructor
	virtual ~CMonitorTransferCtrl();

// Dialog Data
	enum { IDD = IDD_Monitor_Transfer };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
  virtual BOOL OnInitDialog();
  afx_msg void OnBnClickedOk();
  afx_msg void OnTimer(UINT_PTR nIDEvent);

  CTreeListCtrl m_transfer_ctrl;
};
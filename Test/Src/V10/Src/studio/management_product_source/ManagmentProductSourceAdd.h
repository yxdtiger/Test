#pragma once

#include "../main_frame/Resource.h"
#include "../../product/product_source.h"
#include "afxcmn.h"

// CManagmentProductSourceAdd dialog

class CManagmentProductSourceAdd : public CDialog
{
	DECLARE_DYNAMIC(CManagmentProductSourceAdd)

public:
	CManagmentProductSourceAdd(CWnd* pParent = NULL, bool bDialogType = true);   // standard constructor
	virtual ~CManagmentProductSourceAdd();

// Dialog Data
	enum { IDD = IDD_DIALOG_MANAGMENT_PRODUCT_SOURCE_ADD };

public:
  void SetProductSource(const wmdf::ProductSource& product_source) {m_productSource = product_source;}

private:
  void FillProductSource();
  void AddProductSource();
  bool m_bAddDialog;

  // 新增加的一个用户
  wmdf::ProductSource m_productSource;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
  CString m_str_id;
  CIPAddressCtrl m_conIPAddr;
  int m_nPort;
  CString m_str_des;
  afx_msg void OnBnClickedOk();
  afx_msg void OnBnClickedButtonSaveContinue();
  virtual BOOL OnInitDialog();
};

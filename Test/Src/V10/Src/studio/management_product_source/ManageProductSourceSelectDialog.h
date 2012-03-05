#pragma once

#include "../main_frame/resource.h"
#include "../common/BeautifulDialog.h"
#include "../common/BeautifulButton.h"
#include "afxwin.h"

// CManageProductSourceSelectDialog dialog

class CManageProductSourceSelectDialog : public CDialog
{
	DECLARE_DYNAMIC(CManageProductSourceSelectDialog)

public:
	CManageProductSourceSelectDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~CManageProductSourceSelectDialog();

// Dialog Data
	enum { IDD = IDD_MANAGE_PRODUCT_SOURCE_SELECT };

public:
  virtual afx_msg void OnBnClickedOk();
  virtual afx_msg void OnBnClickedCancel();

public:
  //PRODUCT_SOURCE_TYPE m_productSourceType;
  int m_nSelectType;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
  CButton m_conButton_ok;
  CButton m_conButton_cancel;
};

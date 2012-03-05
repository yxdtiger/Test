#pragma once
#include "afxcmn.h"
#include "TreeListCtrl.h"
#include "../main_frame/resource.h"
// TreeTest dialog

class TreeTest : public CDialog
{
	DECLARE_DYNAMIC(TreeTest)

public:
	TreeTest(CWnd* pParent = NULL);   // standard constructor
	virtual ~TreeTest();

// Dialog Data
	enum { IDD = IDD_DIALOG1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
  virtual BOOL OnInitDialog();
  afx_msg void OnTimer(UINT_PTR nIDEvent);
  CTreeListCtrl m_client_ctrl;
};

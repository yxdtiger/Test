#pragma once
#include "afxwin.h"

#include "../../usermanager/user_manager.h"
#include "../../usermanager/user.h"
#include "UserAddDlg.h"
#include "../common/UserSerachDlg.h"
#include "ManageClientListCtrl.h"
#include <vector>

// CManageClientDialog form view

class CManageClientDialog : public CDialog
{
	DECLARE_DYNCREATE(CManageClientDialog)

public:
	CManageClientDialog(CWnd* pParent = NULL);           // protected constructor used by dynamic creation
	virtual ~CManageClientDialog();

public:
	enum { IDD = IDD_MANAGE_CLIENT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
  CManageClientListCtrl m_conListUser;
  CReportListCtrl m_conListAuthorize;
  afx_msg void OnBnClickedButtonSearch();
  afx_msg void OnBnClickedButtonAdd();
  afx_msg void OnBnClickedButtonDelete();
  afx_msg void OnBnClickedButtonModify();
  afx_msg void OnDestroy();

  afx_msg LRESULT OnAddUser(WPARAM wParam, LPARAM lParam);
  afx_msg LRESULT OnModifyUser(WPARAM wParam, LPARAM lParam);

private:
  // 在界面上添加user
  void AddUser(const wmdf::User& user);

  // 在界面上修改user
  void ModifyUser(const wmdf::User& user);

  bool m_bInitOK;

private:
  wmdf::UserManager* m_user_manager;
  wmdf::ProductManager* m_product_manager;

public:
  afx_msg void OnLvnItemchangedListUser(NMHDR *pNMHDR, LRESULT *pResult);
  virtual BOOL OnInitDialog();

  afx_msg void OnBnClickedOk();
  afx_msg void OnBnClickedCancel();
  CButton m_conButton_add;
  CButton m_conButton_modify;
  CButton m_conButton_delete;
  CButton m_conButton_search;
  CButton m_conButton_ok;
  CButton m_conButton_cancel;
  CComboBox m_dest_server;
  afx_msg void OnBnClickedGetConfigButton();

  afx_msg void OnBnClickedUpdateConfigButton();
};
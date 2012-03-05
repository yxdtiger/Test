#pragma once
#include "afxcmn.h"
#include "afxwin.h"

#include "../../usermanager/user.h"
#include <vector>
#include "ManageCustomerTreeCtrl.h"
#include "afxdtctl.h"
#include "../common/AddOrModifyDialog.h"
#include "../../product/product_manager.h"
#include "../common/BeautifulButton.h"

// CUserAddDlg dialog

class CUserAddDlg : public CAddOrModifyDialog
{
	DECLARE_DYNAMIC(CUserAddDlg)

public:
	CUserAddDlg(CWnd* pParent = NULL, bool bDialogType = true);   // standard constructor
	virtual ~CUserAddDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_USER_ADD };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

protected:
  virtual void FillItem();
  virtual void AddItem();

  void TimerProcess();

public:
  void SetProductManager(wmdf::ProductManager* pProductManager);

private:
  wmdf::ProductManager* m_pProductManager;

public:
  ManageCustomerTreeCtrl m_con_treectrl;
  CImageList m_imgState;

  virtual BOOL OnInitDialog();
  CIPAddressCtrl con_ip_addr_;
  CString str_user_name_;
  CString str_password_;
  CString str_company_;
  CString str_contact_;
  CString str_phone_number_;
  CString str_email_;
  CButton con_customer_type_;
  BOOL m_is_ip_limited;
  // 新增加的一个用户
  wmdf::User* m_pUser;
  int customer_type_int_;

  CDateTimeCtrl m_conAuthDate;
  DWORD ip_addr_;
};
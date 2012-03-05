#pragma once

#include "../main_frame/resource.h"
#include "../common/AddOrModifyDialog.h"
#include "../../product/product_descripter.h"
#include "afxwin.h"

// CManageProductAddProductDescripterDialog dialog

class CManageProductAddProductDescripterDialog : public CAddOrModifyDialog
{
	DECLARE_DYNAMIC(CManageProductAddProductDescripterDialog)

public:
	CManageProductAddProductDescripterDialog(CWnd* pParent = NULL, bool bDialogType = true);   // standard constructor
	virtual ~CManageProductAddProductDescripterDialog();

// Dialog Data
	enum { IDD = IDD_MANAGE_PRODUCT_ADD_PRODUCT_DESCRIPTER };

protected:
  virtual void FillItem();
  virtual void TimerProcess();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
  virtual BOOL OnInitDialog();
};

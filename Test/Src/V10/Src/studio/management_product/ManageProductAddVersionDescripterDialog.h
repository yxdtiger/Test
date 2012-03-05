#pragma once

#include "../main_frame/resource.h"
#include "../common/AddOrModifyDialog.h"
#include "../../product/version_descripter.h"
#include "afxwin.h"

// CManageProductAddVersionDescripterDialog dialog



class CManageProductAddVersionDescripterDialog : public CAddOrModifyDialog
{
	DECLARE_DYNAMIC(CManageProductAddVersionDescripterDialog)

public:
	CManageProductAddVersionDescripterDialog(CWnd* pParent = NULL, bool bDialogType = true);   // standard constructor
	virtual ~CManageProductAddVersionDescripterDialog();

// Dialog Data
	enum { IDD = IDD_MANAGE_PRODUCT_ADD_VERSION_DESCRIPTER };

protected:
  virtual void FillItem();
  virtual void TimerProcess();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
  virtual BOOL OnInitDialog();
};

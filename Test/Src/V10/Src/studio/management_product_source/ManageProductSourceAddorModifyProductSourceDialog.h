#pragma once

#include "../common/AddOrModifyDialog.h"
#include "../../product/product_source.h"

// CManageProductSourceAddorModifyProductSourceDialog dialog

class CManageProductSourceAddorModifyProductSourceDialog : public CAddOrModifyDialog
{
	DECLARE_DYNAMIC(CManageProductSourceAddorModifyProductSourceDialog)

public:
	CManageProductSourceAddorModifyProductSourceDialog(CWnd* pParent = NULL, bool bDialogType = true);   // standard constructor
	virtual ~CManageProductSourceAddorModifyProductSourceDialog();

// Dialog Data
	enum { IDD = IDD_MANAGE_PRODUCT_SOURCE_ADDORMODIFY_PRODUCTSOURCE };

protected:
  virtual void FillItem();
  virtual void TimerProcess();

	DECLARE_MESSAGE_MAP()
public:
  virtual BOOL OnInitDialog();
};

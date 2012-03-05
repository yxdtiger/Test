#pragma once

#include "../common/AddOrModifyDialog.h"
#include "../../product/product_source.h"

// CManageProductSourceAddorModifyMarketTimeDialog dialog

class CManageProductSourceAddorModifyMarketTimeDialog : public CAddOrModifyDialog
{
	DECLARE_DYNAMIC(CManageProductSourceAddorModifyMarketTimeDialog)

public:
	CManageProductSourceAddorModifyMarketTimeDialog(CWnd* pParent = NULL, bool bDialogType = true);   // standard constructor
	virtual ~CManageProductSourceAddorModifyMarketTimeDialog();

// Dialog Data
	enum { IDD = IDD_MANAGE_PRODUCT_SOURCE_ADDORMODIFY_MARKETTIME };

protected:
  virtual void FillItem();
  virtual void TimerProcess();

	DECLARE_MESSAGE_MAP()
public:
  virtual BOOL OnInitDialog();
};

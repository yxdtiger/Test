#pragma once

#include "../common/AddOrModifyDialog.h"
#include "../../product/product_source.h"
#include "../../protocol/protocol_description.h"

// CManageProductSourceAddorModifySubscribedProductDialog dialog

class CManageProductSourceAddorModifySubscribedProductDialog : public CAddOrModifyDialog
{
	DECLARE_DYNAMIC(CManageProductSourceAddorModifySubscribedProductDialog)

public:
	CManageProductSourceAddorModifySubscribedProductDialog(CWnd* pParent = NULL, bool bDialogType = true);   // standard constructor
	virtual ~CManageProductSourceAddorModifySubscribedProductDialog();

// Dialog Data
	enum { IDD = IDD_MANAGE_PRODUCT_SOURCE_ADDORMODIFY_SUBSCRIBEDPRODUCT };

protected:
  virtual void FillItem();
  virtual void TimerProcess();

	DECLARE_MESSAGE_MAP()
public:
  virtual BOOL OnInitDialog();
};

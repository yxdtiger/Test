#pragma once

#include "../common/AddOrModifyDialog.h"
#include "../../product/product_source.h"

// CManageProductSourceAddorModifyBackupSourceDialog dialog

class CManageProductSourceAddorModifyBackupSourceDialog : public CAddOrModifyDialog
{
	DECLARE_DYNAMIC(CManageProductSourceAddorModifyBackupSourceDialog)

public:
	CManageProductSourceAddorModifyBackupSourceDialog(CWnd* pParent = NULL, bool bDialogType = true);   // standard constructor
	virtual ~CManageProductSourceAddorModifyBackupSourceDialog();

// Dialog Data
	enum { IDD = IDD_MANAGE_PRODUCT_SOURCE_ADDORMODIFY_BACKUPSOURCE };

protected:
  virtual void FillItem();
  virtual void TimerProcess();

	DECLARE_MESSAGE_MAP()
public:
  virtual BOOL OnInitDialog();
};

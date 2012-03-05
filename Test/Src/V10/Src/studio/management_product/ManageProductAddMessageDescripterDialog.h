#pragma once

#include "../main_frame/resource.h"
#include "../common/AddOrModifyDialog.h"
#include "../../product/message_descripter.h"
#include "afxwin.h"

// CManageProductAddMessageDescripterDialog dialog

class CManageProductAddMessageDescripterDialog : public CAddOrModifyDialog
{
	DECLARE_DYNAMIC(CManageProductAddMessageDescripterDialog)

public:
	CManageProductAddMessageDescripterDialog(CWnd* pParent = NULL, bool bDialogType = true);   // standard constructor
	virtual ~CManageProductAddMessageDescripterDialog();

// Dialog Data
	enum { IDD = IDD_MANAGE_PRODUCT_ADD_MESSAGE_DESCRIPTER };

protected:
  virtual void FillItem();
  virtual void TimerProcess();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
  virtual BOOL OnInitDialog();

};

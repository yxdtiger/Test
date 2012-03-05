#pragma once

#include "../main_frame/resource.h"
#include "BeautifulDialog.h"
#include "BeautifulButton.h"

enum ENUM_DIALOG_TYPE
{
  DIALOG_PRODUCT_DESCRIPTER = 0,
  DIALOG_VERSION_DESCRIPTER,
  DIALOG_MESSAGE_DESCRIPTER,
  DIALOG_PRODUCT_SOURCE,
  DIALOG_MARKET_TIME,
  DIALOG_BACKUP_SOURCE,
  DIALOG_SUBSCRIBED_PRODUCT
};

struct ItemParam
{
  void* pData;
  bool bAddDialog;
  ENUM_DIALOG_TYPE type;

  ItemParam()
  {
    pData = NULL;
    bAddDialog = false;
    type = DIALOG_PRODUCT_DESCRIPTER;
  }
};

// CAddOrModifyDialog dialog

class CAddOrModifyDialog : public CDialog
{
	DECLARE_DYNAMIC(CAddOrModifyDialog)

public:
	CAddOrModifyDialog(UINT IDD, CWnd* pParent = NULL, bool bDialogType = true);   // standard constructor
	virtual ~CAddOrModifyDialog();

// Dialog Data
//	enum { IDD = IDD_ADDORMODIFY_DIALOG };
public:
  virtual afx_msg void OnBnClickedOk();
  virtual afx_msg void OnBnClickedCancel();
  virtual afx_msg void OnBnClickedSaveAdd();

protected:
  virtual void FillItem() = 0;
  virtual void AddItem();
  virtual void Reset();
  virtual void TimerProcess();

public:
  void* m_pData;
  ENUM_DIALOG_TYPE m_type;

protected:
  // true: add dialog
  // false: modify dialog
  bool m_bAddDialog;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
  afx_msg void OnTimer(UINT_PTR nIDEvent);
  virtual BOOL OnInitDialog();
  void WnSetWindowText(CString s);
  CButton m_conButton_saveadd;
  CButton m_conButton_ok;
  CButton m_conButton_cancel;
  CString m_title;
};

#pragma once
#include "afxcmn.h"
#include "UserAddDlg.h"


// CUserModifyDlg dialog

class CUserModifyDlg : public CUserAddDlg
{
	DECLARE_DYNAMIC(CUserModifyDlg)

public:
	CUserModifyDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CUserModifyDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_USER_ADD };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
  virtual BOOL OnInitDialog();
};

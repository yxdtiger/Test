#pragma once
#include "../common/BeautifulDialog.h"

// CUserDeleteDlg dialog

class CUserDeleteDlg : public CDialog
{
	DECLARE_DYNAMIC(CUserDeleteDlg)

public:
	CUserDeleteDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CUserDeleteDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_USER_DELETE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};

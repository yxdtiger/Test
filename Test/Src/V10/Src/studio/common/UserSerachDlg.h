#pragma once
#include "../common/BeautifulDialog.h"
#include "../common/BeautifulButton.h"
#include "afxwin.h"

// CUserSerachDlg dialog

class CUserSerachDlg : public CDialog
{
	DECLARE_DYNAMIC(CUserSerachDlg)

public:
	CUserSerachDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CUserSerachDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_USER_SEARCH };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
  CButton m_conButton_find_next;
  CButton m_conButton_count;
  CButton m_conButton_close;
  afx_msg void OnBnClickedOk();
};

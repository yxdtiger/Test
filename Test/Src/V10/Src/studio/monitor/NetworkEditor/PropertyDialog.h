#if !defined(AFX_PROPERTYDIALOG_H__C2873C9B_A082_44A5_AE88_A0765553316E__INCLUDED_)
#define AFX_PROPERTYDIALOG_H__C2873C9B_A082_44A5_AE88_A0765553316E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PropertyDialog.h : header file
//

#include "../../main_frame/resource.h"
#include "DiagramEditor/DiagramPropertyDlg.h"

/////////////////////////////////////////////////////////////////////////////
// CPropertyDialog dialog

class CPropertyDialog : public CDiagramPropertyDlg
{
// Construction
public:
	CPropertyDialog(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CPropertyDialog)
	enum { IDD = IDD_PROPERTY_DIALOG };
	CString	m_title;
	//}}AFX_DATA

	virtual void	SetValues();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPropertyDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPropertyDialog)
	virtual void OnCancel();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROPERTYDIALOG_H__C2873C9B_A082_44A5_AE88_A0765553316E__INCLUDED_)

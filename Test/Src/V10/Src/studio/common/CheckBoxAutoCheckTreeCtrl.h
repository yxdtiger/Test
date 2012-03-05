#pragma once

#include "CheckBoxTreeCtrl.h"

/////////////////////////////////////////////////////////////////////////////
// CCheckBoxAutoCheckTreeCtrl window

class CCheckBoxAutoCheckTreeCtrl : public CCheckBoxTreeCtrl
{
// Construction
public:
	CCheckBoxAutoCheckTreeCtrl();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCheckBoxAutoCheckTreeCtrl)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual BOOL SetCheck( HTREEITEM hItem,  BOOL fCheck=TRUE);
	virtual ~CCheckBoxAutoCheckTreeCtrl();

	// Generated message map functions
protected:

	DECLARE_MESSAGE_MAP()
};
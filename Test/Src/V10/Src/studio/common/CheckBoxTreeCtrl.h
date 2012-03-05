#pragma once

#include "ExpandTreeCtrl.h"

/////////////////////////////////////////////////////////////////////////////
// CCheckBoxTreeCtrl window

class CCheckBoxTreeCtrl : public CExpandTreeCtrl
{
// Construction
public:
	CCheckBoxTreeCtrl();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCheckBoxTreeCtrl)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual BOOL SetCheck( HTREEITEM hItem,  BOOL fCheck=TRUE);
  BOOL SetItemState( HTREEITEM hItem, UINT nState, UINT nStateMask, BOOL bSearch=TRUE);
	virtual ~CCheckBoxTreeCtrl();

	// Generated message map functions
protected:
	//{{AFX_MSG(CCheckBoxTreeCtrl)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
  afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnStateIconClick(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnKeydown(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
protected:
	UINT m_uFlags;
	void TravelSiblingAndParent(HTREEITEM hItem, int nState);
	void TravelChild(HTREEITEM hItem, int nState);
};
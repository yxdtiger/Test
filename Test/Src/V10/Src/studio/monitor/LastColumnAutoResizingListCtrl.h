/*
* Copyright (c) 2011 by Julijan Sribar
* 
* This software is provided 'as-is', without any express or implied
* warranty. In no event will the authors be held liable for any damages
* arising from the use of this software.
* 
* Permission is granted to anyone to use this software for any purpose,
* including commercial applications, and to alter it and redistribute it
* freely, subject to the following restrictions:
* 
*     1. The origin of this software must not be misrepresented; you must not
*     claim that you wrote the original software. If you use this software
*     in a product, an acknowledgment in the product documentation would be
*     appreciated but is not required.
* 
*     2. Altered source versions must be plainly marked as such, and must not be
*     misrepresented as being the original software.
* 
*     3. This notice may not be removed or altered from any source
*     distribution.
*/

#pragma once

#include ".\NonExtendableHeaderCtrl.h"

// CLastColumnAutoResizingListCtrl class definition

class CLastColumnAutoResizingListCtrl : public CListCtrl
{
	DECLARE_DYNAMIC(CLastColumnAutoResizingListCtrl)

public:
	CLastColumnAutoResizingListCtrl();
	virtual ~CLastColumnAutoResizingListCtrl();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnWindowPosChanging(WINDOWPOS* lpwndpos);

	afx_msg void OnHdnBegintrack(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnHdnItemchanging(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnHdnItemchanged(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnHdnEndtrack(NMHDR *pNMHDR, LRESULT *pResult);

	afx_msg void OnHdnDividerdblclick(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);

	virtual void PreSubclassWindow();

private:
	CNonExtendableHeaderCtrl m_header;
	BOOL m_trackingHeaderDivider;
	int m_oldColumnWidth;

	int GetLastColumnIndex();
	int GetAllItemsHeight();
	int GetRowWidth();
	int GetHeaderHeight();
	bool IsScrollBarVisible(DWORD scrollBar);
};



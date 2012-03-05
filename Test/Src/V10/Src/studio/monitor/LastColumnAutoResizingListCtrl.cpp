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

// AutoResizingListCtrl.cpp : implementation file
//

#include "..\main_frame\StdAfx.h"
#include ".\LastColumnAutoResizingListCtrl.h"


// CLastColumnAutoResizingListCtrl

IMPLEMENT_DYNAMIC(CLastColumnAutoResizingListCtrl, CListCtrl)

CLastColumnAutoResizingListCtrl::CLastColumnAutoResizingListCtrl() 
	: m_trackingHeaderDivider(FALSE)
{
}

CLastColumnAutoResizingListCtrl::~CLastColumnAutoResizingListCtrl()
{
}

BEGIN_MESSAGE_MAP(CLastColumnAutoResizingListCtrl, CListCtrl)
	ON_WM_SIZE()
	ON_WM_WINDOWPOSCHANGING()
	ON_NOTIFY(HDN_BEGINTRACKA, 0, &CLastColumnAutoResizingListCtrl::OnHdnBegintrack)
	ON_NOTIFY(HDN_BEGINTRACKW, 0, &CLastColumnAutoResizingListCtrl::OnHdnBegintrack)
	ON_NOTIFY(HDN_ITEMCHANGINGA, 0, &CLastColumnAutoResizingListCtrl::OnHdnItemchanging)
	ON_NOTIFY(HDN_ITEMCHANGINGW, 0, &CLastColumnAutoResizingListCtrl::OnHdnItemchanging)
	ON_NOTIFY(HDN_ITEMCHANGEDA, 0, &CLastColumnAutoResizingListCtrl::OnHdnItemchanged)
	ON_NOTIFY(HDN_ITEMCHANGEDW, 0, &CLastColumnAutoResizingListCtrl::OnHdnItemchanged)
	ON_NOTIFY(HDN_ENDTRACKA, 0, &CLastColumnAutoResizingListCtrl::OnHdnEndtrack)
	ON_NOTIFY(HDN_ENDTRACKW, 0, &CLastColumnAutoResizingListCtrl::OnHdnEndtrack)
	ON_NOTIFY(HDN_DIVIDERDBLCLICKA, 0, &CLastColumnAutoResizingListCtrl::OnHdnDividerdblclick)
	ON_NOTIFY(HDN_DIVIDERDBLCLICKW, 0, &CLastColumnAutoResizingListCtrl::OnHdnDividerdblclick)
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()

// CLastColumnAutoResizingListCtrl message handlers

// Ensures that last column width is adjusted each time control is resized
void CLastColumnAutoResizingListCtrl::OnSize(UINT nType, int cx, int cy)
{
	CListCtrl::OnSize(nType, cx, cy);
	// apply only if control when not minimized
	if (cx > 0) {
		SetRedraw(FALSE);
		SetColumnWidth(GetLastColumnIndex(), LVSCW_AUTOSIZE_USEHEADER);
		SetRedraw(TRUE);
		Invalidate();
		UpdateWindow();
	}
}

// Prevents scrollbar flickering while last column is being narrowed and
// prevents blank rows at the top end left when control is enlarged 
void CLastColumnAutoResizingListCtrl::OnWindowPosChanging(WINDOWPOS* lpwndpos)
{
	CListCtrl::OnWindowPosChanging(lpwndpos);
	// override only if control is resized
	if ((lpwndpos->flags & SWP_NOSIZE) != 0)
		return;
	// get current size of the control
	RECT rect;
	GetWindowRect(&rect);
	// calculate control width change
	int deltaX = lpwndpos->cx - (rect.right - rect.left);
	// if control is narrowed, correct the width of the last column 
	// to prevent horizontal scroll bar to appear
	if (deltaX < 0) {
		int lastColumnIndex = GetLastColumnIndex();
		int columnWidth = GetColumnWidth(lastColumnIndex);
		SetColumnWidth(lastColumnIndex, columnWidth + deltaX);
	}
	// is vertical scroll bar visible?
	if (IsScrollBarVisible(WS_VSCROLL)) {
		// vertical scroll bar may become hidden either if height of control is increased
		// or if width is increased (in case both scrollbars are visible)
		int deltaY = lpwndpos->cy - (rect.bottom - rect.top);
		if (deltaX <= 0 && deltaY <= 0)
			return;
		// height required for all items to be visible
		int allItemsHeight = GetAllItemsHeight();
		// row width 
		int rowWidth = GetRowWidth();
		// calculate new client height and width after resize
		RECT clientRect;
		GetClientRect(&clientRect);
		int newClientHeight = clientRect.bottom - GetHeaderHeight() + deltaY;
		// is horizontal scrollbar is visible?
		if (IsScrollBarVisible(WS_HSCROLL)) {
			int newClientWidth = clientRect.right + deltaX;
			int hScrollBarHeight = GetSystemMetrics(SM_CYHSCROLL);
			int vScrollBarWidth = GetSystemMetrics(SM_CXVSCROLL);
			// if both scrollbars will be hidden then correct new height of client area
			if ((newClientHeight + hScrollBarHeight >= allItemsHeight) && (newClientWidth + vScrollBarWidth >= rowWidth))
				newClientHeight += hScrollBarHeight;
			// if vertical scrollbar is going to be hidden then correct new width of client area
			if (newClientHeight >= allItemsHeight)
				newClientWidth += vScrollBarWidth;
			// horizontal scrollbar is going to be hidden...
			if (newClientWidth >= rowWidth) {
				// ...so scroll the view to the left to avoid blank column at the left end
				SendMessage(WM_HSCROLL, LOWORD(SB_LEFT), NULL);
				// ensure that bottom item remains visible
				if (IsItemVisible(GetItemCount() - 1))
					PostMessage(WM_VSCROLL, LOWORD(SB_BOTTOM), NULL);
			}
		}
		// vertical scrollbar will be hidden so ensure the first item is moved to the top
		if (newClientHeight >= allItemsHeight)
			EnsureVisible(0, FALSE);
	}
}

// stores current column width on the beginning of divider dragging process
// and sets a flag that indicates that dragging process is in progress
void CLastColumnAutoResizingListCtrl::OnHdnBegintrack(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMHEADER phdr = reinterpret_cast<LPNMHEADER>(pNMHDR);
	if ((phdr->pitem) != 0 && (phdr->pitem->mask & HDI_WIDTH) != 0) {
		// prevent resizing the last column
		if (phdr->iItem == GetLastColumnIndex()) {
			*pResult = 1;
			return;
		}
		// save current width of the column being resized
		m_oldColumnWidth = phdr->pitem->cxy;
		m_trackingHeaderDivider = TRUE;
	}
	*pResult = 0;
}

// prevents flickering scrollbar during divider dragging and adjusts last column width
void CLastColumnAutoResizingListCtrl::OnHdnItemchanging(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMHEADER phdr = reinterpret_cast<LPNMHEADER>(pNMHDR);
	if ((phdr->pitem) != 0 && (phdr->pitem->mask & HDI_WIDTH) != 0 && m_trackingHeaderDivider) {
		int lastColumnIndex = GetLastColumnIndex();
		// if resizing any column except the last one...
		if (phdr->iItem < lastColumnIndex) {
			SetRedraw(FALSE);
			int newWidth = phdr->pitem->cxy;
			// if column is being widened, correct width of the last column
			// to avoid flickering horizontal scrollbar
			if (newWidth > m_oldColumnWidth) {
				int lastColumnWidth = GetColumnWidth(lastColumnIndex) - newWidth + m_oldColumnWidth;
				SetColumnWidth(lastColumnIndex, lastColumnWidth);
			}
			// if column is narrowed, set LVSCW_AUTOSIZE_USEHEADER for the last column
			else
				SetColumnWidth(lastColumnIndex, LVSCW_AUTOSIZE_USEHEADER);
			// store new width of the column
			m_oldColumnWidth = newWidth;
		}
		else {
			// all columns have been resized, so redraw the control
			SetRedraw(TRUE);
			Invalidate();
			UpdateWindow();
		}
	}
	*pResult = 0;
}

// readjust column widths if HDS_FULLDARG style is not set
void CLastColumnAutoResizingListCtrl::OnHdnItemchanged(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMHEADER phdr = reinterpret_cast<LPNMHEADER>(pNMHDR);
	if ((phdr->pitem) != 0 && (phdr->pitem->mask & HDI_WIDTH) != 0 && m_trackingHeaderDivider == FALSE) {
		int lastColumnIndex = GetLastColumnIndex();
		// if any column except the last one was resized
		if (phdr->iItem < lastColumnIndex) {
			SetRedraw(FALSE);
			SetColumnWidth(lastColumnIndex, LVSCW_AUTOSIZE_USEHEADER);
			SetRedraw(TRUE);
			Invalidate();
			UpdateWindow();
		}
	}
	*pResult = 0;
}

// resets flag that indicates divider is going on
void CLastColumnAutoResizingListCtrl::OnHdnEndtrack(NMHDR *pNMHDR, LRESULT *pResult) {
	m_trackingHeaderDivider = FALSE;
	*pResult = 0;
}

// adjusts last column width when user double-clicks a column divider
void CLastColumnAutoResizingListCtrl::OnHdnDividerdblclick(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMHEADER phdr = reinterpret_cast<LPNMHEADER>(pNMHDR);
	int lastColumnIndex = GetLastColumnIndex();
	// prevent double-click resizing for the last column
	if (phdr->iItem < lastColumnIndex) {
		SetRedraw(FALSE);
		SetColumnWidth(phdr->iItem, LVSCW_AUTOSIZE_USEHEADER);
		SetColumnWidth(lastColumnIndex, LVSCW_AUTOSIZE_USEHEADER);
		SetRedraw(TRUE);
		Invalidate();
		UpdateWindow();
	}
	*pResult = 0;
}

// adjusts last column width when user presses CTRL and + on numeric keypad
void CLastColumnAutoResizingListCtrl::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// handle CTRL + Add to adjust all column widths
	if (nChar == VK_ADD && ::GetKeyState(VK_CONTROL) != 0) {
		SetRedraw(FALSE);
		for (int i = 0; i <= GetLastColumnIndex(); ++i)
			SetColumnWidth(i, LVSCW_AUTOSIZE_USEHEADER);
		SetRedraw(TRUE);
		Invalidate();
		UpdateWindow();
		return;
	}
	CListCtrl::OnKeyDown(nChar, nRepCnt, nFlags);
}

// CLastColumnAutoResizingListCtrl overrides

// substitutes original header control with CNonExtendableHeaderCtrl
void CLastColumnAutoResizingListCtrl::PreSubclassWindow()
{
	m_header.SubclassDlgItem(0, this);
	CListCtrl::PreSubclassWindow();
}

// CLastColumnAutoResizingListCtrl private members

int CLastColumnAutoResizingListCtrl::GetLastColumnIndex()
{
	return GetHeaderCtrl()->GetItemCount() - 1;
}

int CLastColumnAutoResizingListCtrl::GetAllItemsHeight()
{
	RECT viewRect;
	GetViewRect(&viewRect);
	return viewRect.bottom - viewRect.top;
}

int CLastColumnAutoResizingListCtrl::GetRowWidth()
{
	if (GetItemCount() == 0)
		return 0;
	RECT rect;
	GetItemRect(0, &rect, LVIR_BOUNDS);
	return rect.right - rect.left;
}

int CLastColumnAutoResizingListCtrl::GetHeaderHeight()
{
	RECT headerRect;
	GetHeaderCtrl()->GetWindowRect(&headerRect);
	return headerRect.bottom - headerRect.top;
}

bool CLastColumnAutoResizingListCtrl::IsScrollBarVisible(DWORD scrollBar)
{
	return (GetWindowLong(m_hWnd, GWL_STYLE) & scrollBar) != 0;
}
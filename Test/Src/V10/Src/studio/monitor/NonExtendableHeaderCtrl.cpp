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

// NonExtendableHeaderCtrl.cpp : implementation file
//

#include "..\main_frame\StdAfx.h"
#include "NonExtendableHeaderCtrl.h"


// CNonExtendableHeaderCtrl

IMPLEMENT_DYNAMIC(CNonExtendableHeaderCtrl, CHeaderCtrl)

CNonExtendableHeaderCtrl::CNonExtendableHeaderCtrl()
	: m_headerResizeDisabled(FALSE)
{
}

CNonExtendableHeaderCtrl::~CNonExtendableHeaderCtrl()
{
}

BEGIN_MESSAGE_MAP(CNonExtendableHeaderCtrl, CHeaderCtrl)
	ON_WM_NCHITTEST()
	ON_WM_SETCURSOR()
END_MESSAGE_MAP()


// CNonExtendableHeaderCtrl message handlers

// checks the position of cursor and sets corresponding flag
LRESULT CNonExtendableHeaderCtrl::OnNcHitTest(CPoint point) 
{
	POINT clientPoint = point;
	ScreenToClient(&clientPoint);
	m_headerResizeDisabled = IsOnLastColumnDivider(clientPoint);
	return CHeaderCtrl::OnNcHitTest(point);
}

// prevents changing cursor to double-pointing arrow if cursor is above last header divider
BOOL CNonExtendableHeaderCtrl::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	if (m_headerResizeDisabled)
		return TRUE;
	return CHeaderCtrl::OnSetCursor(pWnd, nHitTest, message);
}

// chacks if point is above last column divider dragging area
BOOL CNonExtendableHeaderCtrl::IsOnLastColumnDivider(const CPoint& point) 
{
	// width of the area above header divider in which cursor
	// changes its shape to double-pointing east-west arrow
	int dragWidth = GetSystemMetrics(SM_CXCURSOR);
	// last column's header rectangle
	RECT rect;
	GetItemRect(GetItemCount() - 1, &rect);
	return point.x > rect.right - dragWidth / 2;
}

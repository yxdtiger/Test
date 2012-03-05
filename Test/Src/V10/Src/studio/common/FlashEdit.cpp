// WnEdit.cpp : implementation file
//

#include "../main_frame/stdafx.h"
#include "FlashEdit.h"

const COLORREF NORMAL_BK_COLOR = RGB(255, 255, 255);
const COLORREF FLASH_BK_COLOR = RGB(224, 208, 25);

// CFlashEdit

IMPLEMENT_DYNAMIC(CFlashEdit, CEdit)

// CFlashEdit message handlers
BEGIN_MESSAGE_MAP(CFlashEdit, CEdit)
	ON_WM_PAINT()
	ON_WM_TIMER()
END_MESSAGE_MAP()

CFlashEdit::CFlashEdit()
{
	// 创建白色画刷
	m_brushWhite.CreateSolidBrush(NORMAL_BK_COLOR);

	// 创建红色画刷
	m_brushRed.CreateSolidBrush(FLASH_BK_COLOR);

	m_pBrush = NULL;

	m_bDrawText = false;

	m_font.CreatePointFont(90, "Arial");
}

CFlashEdit::~CFlashEdit()
{
}

void CFlashEdit::SetWindowText(LPCTSTR lpszString)
{
	m_bDrawText = false;
	m_str = lpszString;

	m_pBrush = &m_brushRed;
	Invalidate();

	SetTimer(1, 100, NULL);
}

void CFlashEdit::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: Add your message handler code here
	// Do not call CEdit::OnPaint() for painting messages
	
	CRect rc;
	GetClientRect(&rc);
	dc.FillRect(&rc, m_pBrush);

	if(m_bDrawText)
	{
    rc.DeflateRect(0, 2);

		CFont* pOldFont = dc.SelectObject(&m_font);
		dc.DrawText(m_str, &rc, DT_LEFT);
		dc.SelectObject(pOldFont);
	}
}

void CFlashEdit::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	switch (nIDEvent)
	{
	case  1:
		m_pBrush = &m_brushWhite;
		m_bDrawText = true;
		Invalidate();
		KillTimer(1);
		break;
	default:
		break;
	}

	CEdit::OnTimer(nIDEvent);
}

#include "../main_frame/StdAfx.h"
#include "BeautifulButton.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CBeautifulButton

CBeautifulButton::CBeautifulButton()
{
	m_MouseOnButton = FALSE;
	m_bIsFlat = TRUE;
	m_bDrawBorder = TRUE; 
	m_bDrawFlatFocus = TRUE;
	m_bAlwaysTrack = TRUE;
	m_hBtnCursor = NULL;

	InitDefaultColor();
}

CBeautifulButton::~CBeautifulButton()
{
}


BEGIN_MESSAGE_MAP(CBeautifulButton, CButton)
	ON_WM_CAPTURECHANGED()
	ON_WM_MOUSEMOVE()
	ON_WM_SETCURSOR()
	ON_WM_KILLFOCUS()
END_MESSAGE_MAP()

void CBeautifulButton::InitDefaultColor()
{
	m_crInactiveBg = RGB(188,197,230);
	m_crActiveBg = RGB(43,200,213);
	m_crText = RGB(13,125,188);
	m_crTextHilight = ::GetSysColor(COLOR_HIGHLIGHTTEXT);
	m_crBorder = RGB(0,0,255);
}

void CBeautifulButton::SetFlat(BOOL bState, BOOL bRepaint)
{ 
	m_bIsFlat = bState; 
	if(bRepaint) 
		Invalidate();
}

void CBeautifulButton::SetFlatFocus(BOOL bDrawFlatFocus, BOOL bRepaint)
{
	m_bDrawFlatFocus = bDrawFlatFocus;
	// Repaint the button
	if(bRepaint) 
		Invalidate();
} // End of SetFlatFocus

void CBeautifulButton::SetInactiveBgColor(COLORREF crNew, BOOL bRepaint)
{
	m_crInactiveBg = crNew; 
	if(bRepaint) 
		Invalidate();
}

void CBeautifulButton::SetActiveBgColor(COLORREF crNew, BOOL bRepaint)
{
	m_crActiveBg = crNew; 
	if (bRepaint) 
		Invalidate();
}

void CBeautifulButton::SetTextColor(COLORREF crNew, BOOL bRepaint)
{
	m_crText = crNew; 
	if (bRepaint) 
		Invalidate();
}

void CBeautifulButton::SetTextHightColor(COLORREF crNew, BOOL bRepaint)
{
	m_crTextHilight = crNew; 
	if (bRepaint) 
		Invalidate();
}

void CBeautifulButton::SetBorderColor(COLORREF crNew, BOOL bRepaint)
{
	m_crBorder = crNew; 
	if (bRepaint) 
		Invalidate();
}

// CBeautifulButton message handlers

void CBeautifulButton::PreSubclassWindow() 
{
	UINT nBS = GetButtonStyle();
	if(nBS & BS_DEFPUSHBUTTON)
		m_bDefaultBtn = TRUE;
	else
		m_bDefaultBtn = FALSE;
  SetButtonStyle(nBS | BS_OWNERDRAW);	
	
	CButton::PreSubclassWindow();
}

void CBeautifulButton::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
  HPEN hOldPen;
  BOOL bIsPressed  = (lpDrawItemStruct->itemState & ODS_SELECTED);
  BOOL bIsFocused  = ::GetFocus() == m_hWnd;//(lpdis->itemState & ODS_FOCUS) == ODS_FOCUS;
  BOOL bIsDisabled = (lpDrawItemStruct->itemState & ODS_DISABLED);
//	BOOL bIsDefault = (lpDIS->itemState & ODS_DEFAULT);
  HDC hDC = lpDrawItemStruct->hDC;
	RECT itemRect = lpDrawItemStruct->rcItem;

	SetBkMode(hDC,TRANSPARENT);

	if(!m_bIsFlat)
	{
		if(bIsFocused || m_bDefaultBtn)
		{
			HBRUSH hbr = CreateSolidBrush(RGB(0,0,0));  
			FrameRect(hDC,&itemRect, hbr);//draw a black color thin line around button
			DeleteObject(hbr);
			InflateRect(&itemRect,-1,-1);
		}
	}

  // Prepare draw... paint button's area with background color
	COLORREF crBkgnd,crText,crFrame;
	if(bIsDisabled)
	{
		crFrame = GetSysColor(COLOR_INACTIVEBORDER);
		crText = GetSysColor(COLOR_GRAYTEXT);
		crBkgnd = m_crInactiveBg;
	}
	else
	{
		if (m_MouseOnButton || bIsPressed)
		{
			crBkgnd = m_crActiveBg;
			crText = m_crTextHilight;
		}
		else
		{
			crBkgnd = m_crInactiveBg;
			crText = m_crText;
		}
		crFrame = m_crBorder;
	}
	//draw background 
	::SetBkColor(hDC, crBkgnd);
	::ExtTextOut(hDC, 0, 0, ETO_OPAQUE, &itemRect, NULL, 0, NULL);

  // Draw pressed button
  if (bIsPressed)
  {
		HPEN penBtnHiLight = CreatePen(PS_SOLID, 0, GetSysColor(COLOR_3DHILIGHT));//COLOR_3DLIGHT)); // Bianco
		HPEN penBtnShadow = CreatePen(PS_SOLID, 0, GetSysColor(COLOR_3DSHADOW));   // Grigio scuro

		// draw darkline of left-top conner
		hOldPen = (HPEN)SelectObject(hDC,penBtnShadow);
		MoveToEx(hDC,itemRect.left, itemRect.bottom-1,NULL);
		LineTo(hDC,itemRect.left, itemRect.top);
		LineTo(hDC,itemRect.right, itemRect.top);
		// draw hilight line of right-bottom conner
		SelectObject(hDC,penBtnHiLight);
		MoveToEx(hDC,itemRect.left, itemRect.bottom-1,NULL);
		LineTo(hDC,itemRect.right-1, itemRect.bottom-1);
		LineTo(hDC,itemRect.right-1, itemRect.top-1);
		//release resource
		SelectObject(hDC,hOldPen);
		DeleteObject(penBtnShadow);
		DeleteObject(penBtnHiLight);
  }
  else // ...else draw non pressed button
  {
    if(!m_bIsFlat || (m_bIsFlat && m_MouseOnButton))
    {
			HPEN pen3DLight = CreatePen(PS_SOLID, 0, GetSysColor(COLOR_3DHILIGHT));//COLOR_3DLIGHT));       // Light gray
			HPEN penBtnShadow = CreatePen(PS_SOLID, 0, GetSysColor(COLOR_3DSHADOW));   // Dark gray
      // White line
      hOldPen = (HPEN)SelectObject(hDC,pen3DLight);
      MoveToEx(hDC,itemRect.left, itemRect.bottom-1,NULL);
      LineTo(hDC,itemRect.left, itemRect.top);
      LineTo(hDC,itemRect.right, itemRect.top);
      // Dark gray line
      SelectObject(hDC,penBtnShadow);
      MoveToEx(hDC,itemRect.left, itemRect.bottom-1,NULL);
      LineTo(hDC,itemRect.right-1, itemRect.bottom-1);
      LineTo(hDC,itemRect.right-1, itemRect.top-1);
			//
			SelectObject(hDC,hOldPen);
			DeleteObject(pen3DLight); 
			DeleteObject(penBtnShadow);
    }
		else
		{
			if(m_bDrawBorder)
			{
				HPEN penBorder = CreatePen(PS_SOLID, 1, crFrame);
				hOldPen = (HPEN)SelectObject(hDC,penBorder);
				SelectObject(hDC,GetStockObject(NULL_BRUSH));
				Rectangle(hDC,itemRect.left,itemRect.top,itemRect.right,itemRect.bottom);
				SelectObject(hDC,hOldPen);
				DeleteObject(penBorder);
			}
		}
  }

  // Read the button title
  TCHAR sTitle[64];
  GetWindowText(sTitle,64);

  RECT captionRect = lpDrawItemStruct->rcItem;

  // Write the button title (if any)
  if (lstrlen(sTitle) != 0)
  {
    if(bIsPressed)
      OffsetRect(&captionRect,1, 1);
    
		::SetTextColor(hDC,crText);
		RECT centerRect = captionRect;
		DrawText(hDC,sTitle, -1, &captionRect, DT_SINGLELINE|DT_CALCRECT);
		int xoff = ((centerRect.right - centerRect.left) - (captionRect.right - captionRect.left)) / 2;
		int yoff = ((centerRect.bottom - centerRect.top) - (captionRect.bottom - captionRect.top)) / 2;
		OffsetRect(&captionRect,xoff, yoff);
		COLORREF oc = SetBkColor(hDC,crBkgnd);
		DrawText(hDC,sTitle, -1, &captionRect, DT_SINGLELINE);
		::SetTextColor(hDC,oc);
  }

	if(!m_bIsFlat || (m_bIsFlat && m_bDrawFlatFocus))
	{
		// Draw the focus rect
		if (bIsFocused)
		{
			RECT focusRect = itemRect;
			InflateRect(&focusRect,-2, -2);
			DrawFocusRect(hDC,&focusRect);
		}
	}	
}

void CBeautifulButton::OnCaptureChanged(CWnd *pWnd) 
{
	if(m_MouseOnButton == TRUE)
	{
		ReleaseCapture();
		Invalidate();
	}

	CButton::OnCaptureChanged(pWnd);
}

void CBeautifulButton::OnMouseMove(UINT nFlags, CPoint point) 
{
	CButton::OnMouseMove(nFlags, point);

  HWND hParent; // Finestra che contiene il bottone
  
	// If the mouse enter the button with the left button pressed then do nothing
  if (nFlags & MK_LBUTTON && !m_MouseOnButton) 
		return;

  // If our button is not flat then do nothing
  if (m_bIsFlat) 
	{
		hParent = ::GetParent(m_hWnd);
		if ((::GetCapture() != m_hWnd) &&	(hParent != NULL)) 
		{
			m_MouseOnButton = TRUE;
			SetCapture();
			Invalidate();
		}
		else
		{
			RECT rc;
			GetClientRect(&rc);
			if (!PtInRect(&rc,point))
			{
				// Redraw only if mouse goes out
				if (m_MouseOnButton == TRUE)
				{
					m_MouseOnButton = FALSE;
					Invalidate();
				}
				// If user is NOT pressing left button then release capture!
				if (!(nFlags & MK_LBUTTON)) 
					ReleaseCapture();
			}
		}
	}
}

BOOL CBeautifulButton::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	if (m_hBtnCursor != NULL)
	{
		::SetCursor(m_hBtnCursor);
		return TRUE;
	}
	
	return CButton::OnSetCursor(pWnd, nHitTest, message);
}

void CBeautifulButton::OnKillFocus(CWnd* pNewWnd) 
{
	CButton::OnKillFocus(pNewWnd);
	
  if(!m_bIsFlat) 
	{
		m_MouseOnButton = FALSE;
		Invalidate();
	}
}
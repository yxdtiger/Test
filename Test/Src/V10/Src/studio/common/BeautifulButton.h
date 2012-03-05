#pragma once

// CBeautifulButton window

class CBeautifulButton : public CButton
{
// Construction
public:
	CBeautifulButton();

// Attributes
public:

// Operations
public:
	void InitDefaultColor();
	void SetFlat(BOOL bState, BOOL bRepaint = FALSE);
	BOOL GetFlat();
	void SetFlatFocus(BOOL bDrawFlatFocus, BOOL bRepaint = FALSE);
	BOOL GetFlatFocus();
	void DrawBorder(BOOL bEnable);
	void SetActiveBgColor(COLORREF crNew, BOOL bRepaint = FALSE);
	void SetInactiveBgColor(COLORREF crNew, BOOL bRepaint = FALSE);
	void SetTextColor(COLORREF crNew, BOOL bRepaint = FALSE);
	void SetTextHightColor(COLORREF crNew, BOOL bRepaint = FALSE);
	void SetBorderColor(COLORREF crNew, BOOL bRepaint = FALSE);
	void SetButtonCursor(HCURSOR hCursor);

// Overrides
	public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	protected:
	virtual void PreSubclassWindow();

// Implementation
public:
	virtual ~CBeautifulButton();

	// Generated message map functions
protected:
	afx_msg void OnCaptureChanged(CWnd *pWnd);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnKillFocus(CWnd* pNewWnd);

	DECLARE_MESSAGE_MAP()
protected:
	BOOL m_bDefaultBtn;
	BOOL m_MouseOnButton;
	BOOL m_bAlwaysTrack;
	BOOL m_bIsFlat;			// Is a flat button?
	BOOL m_bDrawBorder;		// Draw border?
	BOOL m_bDrawFlatFocus;	// Draw focus rectangle for flat button?
	HCURSOR m_hBtnCursor;
	
	COLORREF m_crInactiveBg;
	COLORREF m_crActiveBg;
	COLORREF m_crText;
	COLORREF m_crTextHilight;
	COLORREF m_crBorder;

};

inline BOOL CBeautifulButton::GetFlat()
{ return m_bIsFlat;}

inline void CBeautifulButton::DrawBorder(BOOL bEnable)
{ m_bDrawBorder = bEnable; }

inline BOOL CBeautifulButton::GetFlatFocus()
{	return m_bDrawFlatFocus; }

inline void CBeautifulButton::SetButtonCursor(HCURSOR hCursor)
{ m_hBtnCursor = hCursor;}

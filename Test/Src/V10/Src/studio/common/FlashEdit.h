#pragma once


// CFlashEdit

class CFlashEdit : public CEdit
{
	DECLARE_DYNAMIC(CFlashEdit)

public:
	CFlashEdit();
	virtual ~CFlashEdit();

protected:
	DECLARE_MESSAGE_MAP()

public:
	void SetWindowText(LPCTSTR lpszString);
	afx_msg void OnPaint();

protected:
	// 红色画刷
	CBrush m_brushRed;

	// 白色画刷
	CBrush m_brushWhite;

	// 使用的画刷
	CBrush* m_pBrush;

	// 控件的Client Rect
	CRect m_rect;

	//edit 显示的字符串
	CString m_str;

	// 控制是否重画字符串
	bool m_bDrawText;

	// 字体
	CFont m_font;	

public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};



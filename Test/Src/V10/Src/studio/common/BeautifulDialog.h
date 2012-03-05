#pragma once

//定义对话框各个部分的标记
#define LEFTBAR      1    //左边框
#define LEFTTITLE    2    //左标题
#define RIGHTTITLE   4    //右标题
#define MIDTITLE     8    //中间标题
#define RIGHTBAR     16   //右边框
#define BOTTOMBAR    32   //底边框
#define MINBUTTON    64   //最小化按钮
#define MAXBUTTON    128  //最大化按钮
#define CLOSEBUTTON  256  //关闭按钮
#define ALL          511  //所有标识


//定义按钮状态标记
enum ButtonState{BS_NONE, BS_MIN, BS_MAX, BS_RES, BS_CLOSE};

class CBeautifulDialog : public CDialog
{
  DECLARE_DYNAMIC(CBeautifulDialog)

public:
  void SetTextColor(COLORREF color) {m_clText = color;}
  void SetTitleTextColor(COLORREF color) {m_clTitle = color;}
  void SetBkgnColor(COLORREF color) {m_crBK = color;}

  void SetEnableMaxsize(bool bEnableMaxsize) {m_bEnableMaxsize = bEnableMaxsize;}

protected:
  virtual void AutoAdjustLayout();    // 自动调整对话框控件布局

//定义成员变量
private:
	int m_nTitleBarCX;	        // 标题栏宽度
	int m_nTitleBarCY;	        // 标题栏高度
	int m_nFrameLeft;	          // 对话框左边框宽度
	int m_nFrameRight;	        // 对话框右边框宽度
	int m_nBorderCX;	          // 对话框边框宽度
	int m_nBorderCY;	          // 对话框边框高度
	int m_nTitleBtnCX;	        // 标题栏按钮宽度
	int m_nTitleBtnCY;	        // 标题栏按钮高度
	int m_nRightTitleCX;        // 记录右标题栏部分的宽度

	CPoint m_MinPT;		          // 最小化按钮坐标(逻辑坐标)
	CPoint m_MaxPT;		          // 最大化按钮坐标(逻辑坐标)
	CPoint m_ClosePT;	          // 关闭按钮坐标(逻辑坐标)

	COLORREF m_crBK;	          // 对话框背景颜色
	COLORREF m_clText;	        // 对话框文本颜色
  COLORREF m_clTitle;         // 对话框标题文本颜色
	CRect m_MinRC;		          // 最下化按钮显示区域
	CRect m_MaxRC;		          // 最大化按钮显示区域
	CRect m_CloseRC;	          // 关闭按钮显示区域

	CRect m_TitleBarRC;	        // 记录右标题栏区域
	BOOL  m_bFirstDraw;	        // 首次绘制
	ButtonState m_BtnState;
	BOOL m_bMaxed;		          // 是否为最大化状态	
	
	CString m_csText;	          // 标题栏文本

private:
  bool m_bEnableMaxsize;
  bool m_bInitOK;

protected:
  CBrush m_brBkgnd;
  CBrush m_brControlBkgnd1;
  CBrush m_brControlBkgnd2;

// Construction
public:
	void DrawDialog(int nFlag = ALL);
	void DrawTitlebarText();
	void SetWindowText(CString csText);
	CBeautifulDialog(UINT ID = IDD, CWnd* pParent = NULL);	// standard constructor

	enum { IDD = 0 };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

// Implementation
protected:
	HICON m_hIcon;
	
	virtual BOOL OnInitDialog();
  afx_msg void OnPaint();
	afx_msg BOOL OnNcActivate(BOOL bActive);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnNcMouseMove(UINT nHitTest, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnNcLButtonDown(UINT nHitTest, CPoint point);
	afx_msg void OnNcLButtonUp(UINT nHitTest, CPoint point);
	afx_msg void OnNcLButtonDblClk(UINT nHitTest, CPoint point);
	afx_msg void OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS FAR* lpncsp);
	DECLARE_MESSAGE_MAP()
};

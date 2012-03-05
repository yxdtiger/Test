// MainFrm.h : interface of the CMainFrame class
//

#pragma once

#include "ChildFrm.h"
#include "Wave.h"
#include <map>
#include "../monitor/NetworkEditor/NetworkEditor.h"
#include "../monitor/NetworkEditor/NetworkEntityContainer.h"

class CNetworkMapEditorView;

class CMainFrame : public CMDIFrameWndEx
{
	DECLARE_DYNAMIC(CMainFrame)
public:
	CMainFrame();

// Attributes
public:
// Operations
public:

  // 客户管理子框架
  CChildFrame* m_pManageClient;

  // 产品源管理子框架
  CChildFrame* m_pManageProductSource;

  // 系统配置管理子框架
  CChildFrame* m_pManageSystemConfig;

  // 产品管理子框架
  CChildFrame* m_pManageProduct;

	CImageList	m_imageList;

  CChildFrame* m_pMonitorServer;

  CChildFrame* m_pMonitorClient;

  CChildFrame* m_pMonitorTransfer;

  CChildFrame* m_pMonitorLog;

  CChildFrame* m_pMonitorQuote;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainFrame)
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

public:
  void CreateWholeProductViewDialog();

// Implementation
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // control bar embedded members
	//CMFCStatusBar  m_wndStatusBar;
	//CMFCToolBar    m_wndToolBar;
  CMFCMenuBar       m_wndMenuBar;
  //CToolBar    m_wndToolBar_Replay;

// Generated message map functions
protected:
	//{{AFX_MSG(CMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
  afx_msg void OnApplicationLook();

	//}}AFX_MSG

  DECLARE_MESSAGE_MAP()
public:
  
  afx_msg void OnManageClient();
  afx_msg void OnReferenceProduct();
  afx_msg void OnReferenceApiandstruct();
  afx_msg void OnManageProductSource();
  afx_msg void OnManageSystemConfig();
  afx_msg void OnManageProduct();

  afx_msg void OnAppExit();
  afx_msg void OnClose();

  afx_msg void OnMdiMonitor();
  afx_msg void OnHorizontal();
  afx_msg void OnVertical();

  afx_msg void OnMonitorTransfer();
  afx_msg void OnMonitorClient();
  afx_msg void OnLogViewer();
  afx_msg void OnMonitorQuote();

  afx_msg LRESULT OnMonitorTransfer(WPARAM wParam, LPARAM lParam);
  afx_msg LRESULT OnMonitorClient(WPARAM wParam, LPARAM lParam);
  afx_msg LRESULT OnLogViewer(WPARAM wParam, LPARAM lParam);
  afx_msg LRESULT OnMonitorQuote(WPARAM wParam, LPARAM lParam);

  afx_msg void OnMonitorNetwork();
  afx_msg LRESULT OnMonitorNetwork(WPARAM wParam, LPARAM lParam);
  afx_msg LRESULT RemoveAView( WPARAM wParam, LPARAM lParam );

  afx_msg LRESULT OnWarningEvent(WPARAM wParam, LPARAM lParam);
  afx_msg LRESULT OnMCINotify(WPARAM wParam,  LPARAM lParam);

private:
  CWave m_wave;	//CWave类变量
  BOOL m_bPlay;	//标记当前播放状态
  std::map<int,CNetworkMapEditorView*> views_;

  std::string GetKey(std::string ip,int port);
public:
  afx_msg void OnCascade();
};
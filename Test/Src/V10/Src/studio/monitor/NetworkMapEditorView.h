// NetworkMapEditorView.h : interface of the CNetworkMapEditorView class
//
/////////////////////////////////////////////////////////////////////////////

#ifndef NETWORKMAPEDITORVIEW_H
#define NETWORKMAPEDITORVIEW_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include <string>
#include "../../include/wmdf_api.h"
#include "NetworkEditor/NetworkEditor.h"
#include "NetworkMapEditorDoc.h"
#include "server_info.h"
#include "../common/studio_utility.h"

class CNetworkMapEditorView : public CView
{
protected: // create from serialization only
	CNetworkMapEditorView();
	DECLARE_DYNCREATE(CNetworkMapEditorView)

// Attributes
public:
	CNetworkMapEditorDoc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNetworkMapEditorView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnInitialUpdate();

public:
  afx_msg void OnTimer(UINT_PTR nIDEvent);

  void StartTimer(int conn_id);
  void EndTimer();
  int conn_id(){return conn_id_;}
  std::string GetKey(std::string ip,int32_t port);  
  void UpdateSymbolStatus(std::string symbol_name,wmdf::StudioSymbolType type);
  
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CNetworkMapEditorView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

// Generated message map functions
protected:
	//{{AFX_MSG(CNetworkMapEditorView)
	
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
  CNetworkSymbol* mid_symbol_;
  wmdf::ServerNetInfo* server_info;
  int bitmap_length_,bitmap_h_gap_,bitmap_w_gap_;
  int x2_ ,x3_,x4_,y2_,y3_,y4_;
  CRect rect;

  int conn_id_;
	CNetworkEditor	m_editor;
	int				m_screenResolutionX;
  afx_msg void UpdateServerStatus();

  void UpdateMidServerStatus();
  void UpdateSourceStatus();
  void UpdateClientStatus();

  void RedrawLink( CNetworkSymbol* server_symbol, CNetworkSymbol* server_symbol2, std::string key, wmdf::StudioSymbolType type);

  void ResetSymbolSize();

  CNetworkSymbol* GetSymbol(std::string symbol_name);
  

  int GetSourceCount(wmdf::ServerNetInfo* server_info);
  int GetClientCount(wmdf::ServerNetInfo* server_info);

};

#ifndef _DEBUG  // debug version in NetworkMapEditorView.cpp
inline CNetworkMapEditorDoc* CNetworkMapEditorView::GetDocument()
   { return (CNetworkMapEditorDoc*)m_pDocument; }
#endif

#endif 

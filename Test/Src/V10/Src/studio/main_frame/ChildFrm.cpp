// RaChildFrame.cpp : implementation file
//

#include "stdafx.h"
#include "WMDF_Studio.h"
#include "ChildFrm.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CChildFrame
IMPLEMENT_DYNCREATE(CChildFrame, CMDIChildWndEx)

BEGIN_MESSAGE_MAP(CChildFrame, CMDIChildWndEx)
END_MESSAGE_MAP()

CChildFrame::CChildFrame()
{
}


CChildFrame::~CChildFrame()
{
}

BOOL CChildFrame::PreCreateWindow(CREATESTRUCT& cs)
{
  // TODO: Modify the Window class or styles here by modifying the CREATESTRUCT cs
  if( !CMDIChildWndEx::PreCreateWindow(cs) )
    return FALSE;

  return TRUE;
}

// CChildFrame diagnostics

#ifdef _DEBUG
void CChildFrame::AssertValid() const
{
  CMDIChildWndEx::AssertValid();
}

void CChildFrame::Dump(CDumpContext& dc) const
{
  CMDIChildWndEx::Dump(dc);
}
#endif //_DEBUG

// CChildFrame message handlers
//
//// Overloaded to fix a MFC bug.
//void CChildFrame::AdjustDockingLayout(HDWP hdwp)
//{
//  ClipStyleReset reset(this);
//  CMDIChildWndEx::AdjustDockingLayout(hdwp);
//}
//
//// Overloaded to fix a MFC bug.
//void CChildFrame::RecalcLayout(BOOL bNotify)
//{
//  ClipStyleReset reset(this);
//  CMDIChildWndEx::RecalcLayout(bNotify);
//}
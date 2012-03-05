// WMDF_Studio.h : main header file for the RACLIENT application
//

#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols
#include "../monitor/NetworkEditor/NetworkEditorClipboardHandler.h"

// CWMDFMonitorStudioApp:
// See WMDF_Studio.cpp for the implementation of this class
//

class CWMDFMonitorStudioApp : public CWinAppEx
{
public:
	CWMDFMonitorStudioApp();
  CNetworkEditorClipboardHandler	m_clip;
// Overrides
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation
  UINT  m_nAppLook;
  BOOL  m_bHiColorIcons;

	HMENU m_hMDIMenu;
	HMENU m_hMenu;
	HACCEL m_hMDIAccel;


public:
	afx_msg void OnAppAbout();
	afx_msg void OnSysDisconnect();

	DECLARE_MESSAGE_MAP()
};

extern CWMDFMonitorStudioApp theApp;
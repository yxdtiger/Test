// WMDF_Studio.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "WMDF_Studio.h"

#include "MainFrm.h"
#include "ChildFrm.h"

#include "time.h"
#include "../common/BeautifulDialog.h"

#include "../monitor/NetworkMapEditorDoc.h"
#include "../monitor/NetworkMapEditorView.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()

// App command to run the dialog
void CWMDFMonitorStudioApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

/////////////////////////////////////////////////////////////////////////////
// CWMDFMonitorStudioApp

BEGIN_MESSAGE_MAP(CWMDFMonitorStudioApp, CWinAppEx)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
	ON_COMMAND(ID_SYS_DISCONNECT, OnSysDisconnect)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWMDFMonitorStudioApp construction

CWMDFMonitorStudioApp::CWMDFMonitorStudioApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance

  m_bHiColorIcons = TRUE;

}

// The one and only CWMDFMonitorStudioApp object
CWMDFMonitorStudioApp theApp;

// CWMDFMonitorStudioApp initialization

BOOL CWMDFMonitorStudioApp::InitInstance()
{
  // InitCommonControlsEx() is required on Windows XP if an application
  // manifest specifies use of ComCtl32.dll version 6 or later to enable
  // visual styles.  Otherwise, any window creation will fail.
  INITCOMMONCONTROLSEX InitCtrls;
  InitCtrls.dwSize = sizeof(InitCtrls);
  // Set this to include all the common control classes you want to use
  // in your application.
  InitCtrls.dwICC = ICC_WIN95_CLASSES;
  InitCommonControlsEx(&InitCtrls);

  CWinAppEx::InitInstance();

  // Initialize OLE libraries
  if (!AfxOleInit())
  {
    AfxMessageBox("OLE initialization failed.  Make sure that the OLE libraries are the correct version.");
    return FALSE;
  }
  AfxEnableControlContainer();


#ifdef _AFXDLL
  Enable3dControls();			// Call this when using MFC in a shared DLL
#else
  Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

  // Standard initialization
  // If you are not using these features and wish to reduce the size
  // of your final executable, you should remove from the following
  // the specific initialization routines you do not need
  // Change the registry key under which our settings are stored
  // TODO: You should modify this string to be something appropriate
  // such as the name of your company or organization
	SetRegistryKey(_T("Local AppWizard-Generated Applications"));
  LoadStdProfileSettings(4);  // Load standard INI file options (including MRU)


  CMultiDocTemplate* pDocTemplate;
  pDocTemplate = new CMultiDocTemplate(
    IDI_ICON_NETWORK,
    RUNTIME_CLASS(CNetworkMapEditorDoc),
    RUNTIME_CLASS(CChildFrame), // custom MDI child frame
    RUNTIME_CLASS(CNetworkMapEditorView));
  AddDocTemplate(pDocTemplate);

  

  InitContextMenuManager();

  InitKeyboardManager();

  InitTooltipManager();
  CMFCToolTipInfo ttParams;
  ttParams.m_bVislManagerTheme = TRUE;
  theApp.GetTooltipManager()->SetTooltipParams(AFX_TOOLTIP_TYPE_ALL,
    RUNTIME_CLASS(CMFCToolTipCtrl), &ttParams);

  // create main MDI Frame window
	CMDIFrameWnd* pMainFrame = new CMainFrame;
  if (!pMainFrame || !pMainFrame->LoadFrame(IDR_MAINFRAME))
  {
    delete pMainFrame;
    return FALSE;
  }
	m_pMainWnd = pMainFrame;
  // call DragAcceptFiles only if there's a suffix
  //  In an MDI app, this should occur immediately after setting m_pMainWnd


  // Parse command line for standard shell commands, DDE, file open
  CCommandLineInfo cmdInfo;
  cmdInfo.m_nShellCommand = CCommandLineInfo::FileNothing;
  ParseCommandLine(cmdInfo);



  // Dispatch commands specified on the command line.  Will return FALSE if
  // app was launched with /RegServer, /Register, /Unregserver or /Unregister.
  if (!ProcessShellCommand(cmdInfo))
    return FALSE;

	// try to load shared MDI menus and accelerator table
	//TODO: add additional member variables and load calls for
	//	additional menu types your application may need. 

	HINSTANCE hInst = AfxGetResourceHandle();
	//m_hMDIMenu  = ::LoadMenu(hInst, MAKEINTRESOURCE(IDR_RACLIETYPE));
	m_hMenu=::LoadMenu(hInst, MAKEINTRESOURCE(IDR_MAINFRAME));
	//m_hMDIAccel = ::LoadAccelerators(hInst, MAKEINTRESOURCE(IDR_RACLIETYPE));



	// The main window has been initialized, so show and update it.
  // pFrame->ShowWindow(m_nCmdShow);
	pMainFrame->ShowWindow(SW_SHOWMAXIMIZED);
  pMainFrame->UpdateWindow();
	pMainFrame->SetWindowText("wmdf studio");
  
  SetMenu(AfxGetMainWnd()->m_hWnd,m_hMDIMenu);

  
  //int cx=GetSystemMetrics(SM_CXMENUCHECK);
  //int cy=GetSystemMetrics(SM_CYMENUCHECK);
  //CMenu *pMenu=AfxGetMainWnd()->GetMenu();

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CWMDFMonitorStudioApp message handlers

int CWMDFMonitorStudioApp::ExitInstance() 
{
	//TODO: handle additional resources you may have added
	if (m_hMDIMenu != NULL)
		FreeResource(m_hMDIMenu);
	if (m_hMDIAccel != NULL)
		FreeResource(m_hMDIAccel);
	return CWinAppEx::ExitInstance();
}

void CWMDFMonitorStudioApp::OnSysDisconnect() 
{
  if(((CMainFrame *)AfxGetMainWnd())->m_pManageClient!=NULL)
    ((CMainFrame *)AfxGetMainWnd())->m_pManageClient->SendMessage(WM_CLOSE);

  if(((CMainFrame *)AfxGetMainWnd())->m_pManageProductSource!=NULL)
    ((CMainFrame *)AfxGetMainWnd())->m_pManageProductSource->SendMessage(WM_CLOSE);	

  if(((CMainFrame *)AfxGetMainWnd())->m_pManageSystemConfig!=NULL)
    ((CMainFrame *)AfxGetMainWnd())->m_pManageSystemConfig->SendMessage(WM_CLOSE);

  if(((CMainFrame *)AfxGetMainWnd())->m_pManageProduct!=NULL)
    ((CMainFrame *)AfxGetMainWnd())->m_pManageProduct->SendMessage(WM_CLOSE);

	SetMenu (AfxGetMainWnd()->m_hWnd,m_hMenu);
	
	m_pMainWnd->SetWindowText("wmdf studio");

}




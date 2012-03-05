#pragma once


// CMonitorBaseDlg dialog

class CMonitorBaseDlg : public CDialog
{
	DECLARE_DYNAMIC(CMonitorBaseDlg)

public:
	CMonitorBaseDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CMonitorBaseDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_BASE_MONITOR };

public:
  virtual void Update() = 0;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};
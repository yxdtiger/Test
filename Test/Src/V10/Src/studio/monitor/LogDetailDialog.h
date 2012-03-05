#pragma once
#include "../main_frame/resource.h"

// LogDetailDialog dialog

class LogDetailDialog : public CDialog
{
	DECLARE_DYNAMIC(LogDetailDialog)

public:
	LogDetailDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~LogDetailDialog();

// Dialog Data
	enum { IDD = IDD_MONITOR_LOG_DETAIL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};

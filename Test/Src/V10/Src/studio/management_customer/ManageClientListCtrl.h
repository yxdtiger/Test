#pragma once

#include "../../usermanager/user.h"
#include "../common/ReportListCtrl.h"

// CManageClientListCtrl

class CManageClientListCtrl : public CReportListCtrl
{
	DECLARE_DYNAMIC(CManageClientListCtrl)

public:
	CManageClientListCtrl();
	virtual ~CManageClientListCtrl();

public:
  void InsertUser(wmdf::User* pUser);
  void DeleteUser(int32_t user_id);

protected:
	DECLARE_MESSAGE_MAP()
};



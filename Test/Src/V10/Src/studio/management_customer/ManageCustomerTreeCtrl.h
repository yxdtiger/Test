#pragma once

#include "../common/CheckBoxAutoCheckTreeCtrl.h"

// ManageCustomerTreeCtrl
class ManageCustomerTreeCtrl : public CCheckBoxAutoCheckTreeCtrl
{
	DECLARE_DYNAMIC(ManageCustomerTreeCtrl)

public:
	ManageCustomerTreeCtrl();
	virtual ~ManageCustomerTreeCtrl();

protected:
  afx_msg void OnRButtonDown(UINT nFlags, CPoint point);

	DECLARE_MESSAGE_MAP()
public:
  afx_msg void OnTvnSelchanged(NMHDR *pNMHDR, LRESULT *pResult);
};
#pragma once

#include <list>
// CReportListCtrl

class CReportListCtrl : public CListCtrl
{
	DECLARE_DYNAMIC(CReportListCtrl)

public:
	CReportListCtrl();
	virtual ~CReportListCtrl();

public:
  void AddColumnHeader(CString strHeader);

protected:
  void Sort();

protected:
  std::list<CString> m_list_column_header;

private:
  static int CALLBACK MyCompareProc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort);

protected:
	DECLARE_MESSAGE_MAP()

protected:
  virtual void PreSubclassWindow();
};



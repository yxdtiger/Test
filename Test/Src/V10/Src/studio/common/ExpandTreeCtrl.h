#pragma once


// CExpandTreeCtrl

class CExpandTreeCtrl : public CTreeCtrl
{
	DECLARE_DYNAMIC(CExpandTreeCtrl)

public:
	CExpandTreeCtrl();
	virtual ~CExpandTreeCtrl();

public:
  void ExpandAll();

protected:
	DECLARE_MESSAGE_MAP()
public:

};



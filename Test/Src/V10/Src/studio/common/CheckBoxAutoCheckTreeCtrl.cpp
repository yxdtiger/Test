// TWTreeCtrl.cpp : implementation file
//

#include "../main_frame/StdAfx.h"
#include "CheckBoxAutoCheckTreeCtrl.h"
#include "../main_frame/Resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



/////////////////////////////////////////////////////////////////////////////
// CCheckBoxAutoCheckTreeCtrl

CCheckBoxAutoCheckTreeCtrl::CCheckBoxAutoCheckTreeCtrl()
{
}

CCheckBoxAutoCheckTreeCtrl::~CCheckBoxAutoCheckTreeCtrl()
{
}


BEGIN_MESSAGE_MAP(CCheckBoxAutoCheckTreeCtrl, CCheckBoxTreeCtrl)
	//{{AFX_MSG_MAP(CCheckBoxAutoCheckTreeCtrl)
	//ON_WM_LBUTTONDOWN()
 // ON_WM_RBUTTONDOWN()
	//ON_NOTIFY_REFLECT(NM_CLICK, OnStateIconClick)
	//ON_NOTIFY_REFLECT(TVN_KEYDOWN, OnKeydown)
	//ON_WM_KEYDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCheckBoxAutoCheckTreeCtrl message handlers

BOOL CCheckBoxAutoCheckTreeCtrl::SetCheck(HTREEITEM hItem, BOOL fCheck)
{
  int nState;
  if(fCheck)
    nState = 3;
  else
    nState = 1;

  BOOL bReturn = CCheckBoxTreeCtrl::SetItemState( hItem, INDEXTOSTATEIMAGEMASK(nState), TVIS_STATEIMAGEMASK );

	TravelChild(hItem, nState);
	TravelSiblingAndParent(hItem, nState);
	return bReturn;
}
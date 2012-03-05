// ExpandTreeCtrl.cpp : implementation file
//

#include "../main_frame/StdAfx.h"
#include "ExpandTreeCtrl.h"


// CExpandTreeCtrl

IMPLEMENT_DYNAMIC(CExpandTreeCtrl, CTreeCtrl)

CExpandTreeCtrl::CExpandTreeCtrl()
{

}

CExpandTreeCtrl::~CExpandTreeCtrl()
{
}


BEGIN_MESSAGE_MAP(CExpandTreeCtrl, CTreeCtrl)
END_MESSAGE_MAP()



// CExpandTreeCtrl message handlers

void CExpandTreeCtrl::ExpandAll()
{
  HTREEITEM hItem;
  hItem= GetFirstVisibleItem();
  while (hItem != NULL)
  {
    Expand(hItem,TVE_EXPAND);
    hItem= GetNextItem(hItem, TVGN_NEXTVISIBLE);
  }
}
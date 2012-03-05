#ifndef _NETWORKEDITORMENU_H_
#define _NETWORKEDITORMENU_H_

#include "DiagramEditor/DiagramMenu.h"

class CNetworkEditorMenu : public CDiagramMenu
{
public:
	CNetworkEditorMenu();
	virtual ~CNetworkEditorMenu();

	virtual CMenu* GetPopupMenu( CDiagramEditor* editor );

};

#endif //_NETWORKEDITORMENU_H_

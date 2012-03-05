#ifndef _NETWORKEDITORCLIPBOARDHANDLER_H_
#define _NETWORKEDITORCLIPBOARDHANDLER_H_

#include "DiagramEditor/DiagramClipboardHandler.h"

class CNetworkEditorClipboardHandler : public CDiagramClipboardHandler
{
public:
	CNetworkEditorClipboardHandler();
	~CNetworkEditorClipboardHandler();

// Overrides
	virtual void	Copy( CDiagramEntity* obj );
	virtual void	Paste( CDiagramEntityContainer* container );
	virtual void	CopyAllSelected( CDiagramEntityContainer* container );
	virtual void	ClearPaste();

private:
// Private data
	CObArray		m_pasteLinks;


};

#endif //_NETWORKEDITORCLIPBOARDHANDLER_H_

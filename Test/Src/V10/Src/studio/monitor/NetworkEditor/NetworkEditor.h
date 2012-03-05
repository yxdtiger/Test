#ifndef _NETWORKEDITOR_H_
#define _NETWORKEDITOR_H_

#include "DiagramEditor/DiagramEditor.h"
#include "NetworkSymbol.h"

class CNetworkEditor : public CDiagramEditor
{
public:
	CNetworkEditor();
	virtual ~CNetworkEditor();

	BOOL CanLink();
	BOOL IsLinked();

	afx_msg void OnLink();
	afx_msg void OnUnlink();
  CNetworkSymbol*	GetNamedObject( const CString& name ) const;

	DECLARE_MESSAGE_MAP()
// Overrides
protected:
	virtual void DrawObjects( CDC* dc, double zoom ) const;
  void DrawAText(double flow,CDC* dc, CRect rect) const;
};

#endif //_NETWORKEDITOR_H_
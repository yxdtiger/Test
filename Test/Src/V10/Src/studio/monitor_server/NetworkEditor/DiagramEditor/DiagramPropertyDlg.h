#ifndef _DIAGRAMPROPERTYDLG_H_
#define _DIAGRAMPROPERTYDLG_H_

#include "DiagramEntity.h"

class CDiagramEntity;
class CDiagramPropertyDlg : public CDialog
{

public:
	CDiagramPropertyDlg( UINT res, CWnd* parent );
	BOOL Create( UINT nIDTemplate, CWnd* pParentWnd );

	void			SetEntity( CDiagramEntity* entity );
	CDiagramEntity*	GetEntity() const;

	virtual CWnd*	GetRedrawWnd();
	virtual void	SetRedrawWnd( CWnd* redrawWnd );
	virtual void	Redraw();

	virtual void	SetValues() = 0;

private:
	CDiagramEntity*	m_entity;
	CWnd*			m_redrawWnd;

};

#endif // _DIAGRAMPROPERTYDLG_H_
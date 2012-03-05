#ifndef _NETWORKSYMBOL_H_
#define _NETWORKSYMBOL_H_

#include "DiagramEditor/DiagramEntity.h"
#include "PropertyDialog.h"

class CNetworkSymbol : public CDiagramEntity
{
public:
	CNetworkSymbol( UINT resid );
	~CNetworkSymbol();

  bool server_status(){return server_status_;}
  void set_server_status(bool status){server_status_=status;}
	virtual CDiagramEntity* Clone();
	static	CDiagramEntity* CreateFromString( const CString& str );

// Overrides
	virtual int		GetHitCode( CPoint point ) const;
	virtual CPoint	GetLinkPosition();
	virtual void	Draw( CDC* dc, CRect rect );
	virtual void	Copy( CDiagramEntity* obj );
	virtual void	DrawSelectionMarkers( CDC* dc, CRect rect ) const;
	virtual BOOL	FromString( const CString& str );
	virtual CString	GetString() const;

	void			SetSymbol( UINT resid );
	UINT			GetSymbol() const;

private:
	UINT	m_symbol;
  bool server_status_;

	CPropertyDialog	m_dlg;

};

#endif // _NETWORKSYMBOL_H_

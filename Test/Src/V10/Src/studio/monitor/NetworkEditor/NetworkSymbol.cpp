/* ==========================================================================
	CNetworkSymbol

	Author :		Johan Rosengren, Abstrakt Mekanik AB

	Date :			2004-05-04

	Purpose :		CNetworkSymbol derives from CDiagramEntity, and is an 
					object drawing a DIB from the application resources.

	Description :	The class adds a member for the resource id of the DIB 
					to load, in addition to the normal overrides for 
					creating CDiagramEntity-instances.

	Usage :			Use as a CDiagramEntity.

   ========================================================================*/
#include "../../main_frame/stdafx.h"
#include "NetworkSymbol.h"
#include "LinkFactory.h"
#include "DiagramEditor/Tokenizer.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


CNetworkSymbol::CNetworkSymbol( UINT resid )
/* ============================================================
	Function :		CNetworkSymbol::CNetworkSymbol
	Description :	constructor
					
	Return :		void
	Parameters :	UINT resid	-	Resource id of icon to show.
					
	Usage :			

   ============================================================*/
{

	m_symbol = resid;
	SetConstraints( CSize( 32, 44 ), CSize( -1, -1 ) );
	SetType( _T( "network_symbol" ) );
	SetTitle( _T( "title" ) );
	SetName( CLinkFactory::GetID() );
	
	SetMarkerSize( CSize( 6, 6 ) );
	SetPropertyDialog( &m_dlg, CPropertyDialog::IDD );
  server_status_ = true;
}

CNetworkSymbol::~CNetworkSymbol()
/* ============================================================
	Function :		CNetworkSymbol::~CNetworkSymbol
	Description :	destructor
					
	Return :		void
	Parameters :	none

	Usage :			

   ============================================================*/
{

	if( m_dlg.m_hWnd )
		m_dlg.DestroyWindow();

}

void CNetworkSymbol::SetSymbol( UINT resid )
/* ============================================================
	Function :		CNetworkSymbol::SetSymbol
	Description :	Sets the reource id for the symbol
					
	Return :		void
	Parameters :	UINT resid	-	Resource id of icon
					
	Usage :			

   ============================================================*/
{

	m_symbol = resid;

}

CDiagramEntity* CNetworkSymbol::Clone()
/* ============================================================
	Function :		CNetworkSymbol::Clone
	Description :	Clone this object to a new object.
					
	Return :		CDiagramEntity*	-	The new object.
	Parameters :	none

	Usage :			Call to create a clone of the object. The 
					caller will have to delete the object.

   ============================================================*/
{

	CNetworkSymbol* obj = new CNetworkSymbol( m_symbol );
	obj->Copy( this );
	return obj;

}

CDiagramEntity* CNetworkSymbol::CreateFromString( const CString& str )
/* ============================================================
	Function :		CNetworkSymbol::CreateFromString
	Description :	Static factory function that creates and 
					returns an instance of this class if str 
					is a valid representation.
					
	Return :		CDiagramEntity*		-	The object, or NULL 
											if str is not a 
											representation of 
											this type.
	Parameters :	const CString& str	-	The string to create 
											from.
					
	Usage :			Can be used as a factory for text file loads. 
					Each object type should have its own 
					version - the default one is a model 
					implementation.

   ============================================================*/
{

	CNetworkSymbol* obj = new CNetworkSymbol( 0 );
	if(!obj->FromString( str ) )
	{
		delete obj;
		obj = NULL;
	}

	return obj;

}

int CNetworkSymbol::GetHitCode( CPoint point ) const
/* ============================================================
	Function :		CNetworkSymbol::GetHitCode
	Description :	Returns the hit point constant for point.
					
	Return :		int				-	The hit point, 
										DEHT_NONE if none.
	Parameters :	CPoint point	-	The point to check
					
	Usage :			Call to see in what part of the object point 
					lies.

   ============================================================*/
{
	return CDiagramEntity::GetHitCode( point );
}


CPoint CNetworkSymbol::GetLinkPosition()
/* ============================================================
	Function :		CNetworkSymbol::GetLinkPosition
	Description :	Returns the position of a link.
					
	Return :		CPoint		-	The position of the link,
	Parameters :	none
					
	Usage :			Called while drawing links

   ============================================================*/
{

	CPoint point( -1, -1 );
	CRect rect = GetRect();
	point.x = rect.left + rect.Width() / 2;
	point.y = rect.top + rect.Height() / 2;
	return point;

}

void CNetworkSymbol::Draw( CDC* dc, CRect rect )
/* ============================================================
	Function :		CNetworkSymbol::Draw
	Description :	Draws the object.

	Return :		void
	Parameters :	CDC* dc		-	The CDC to draw to. 
					CRect rect	-	The real rectangle of the 
									object.

	Usage :			The function should clean up all selected 
					objects. Note that the CDC is a memory CDC, 
					so creating a memory CDC in this function 
					will probably not speed up the function.

   ============================================================*/
{
	ASSERT( m_symbol );

	dc->SelectStockObject( BLACK_PEN );
	dc->SelectStockObject( WHITE_BRUSH );

	CDC memDC;
	memDC.CreateCompatibleDC( dc );
    HBITMAP hbitmap = (HBITMAP)::LoadImage(AfxGetResourceHandle(), 
                                        MAKEINTRESOURCE( m_symbol ),
                                        IMAGE_BITMAP, 
                                        0,0, 
                                        LR_CREATEDIBSECTION
                                        );

	CBitmap* bitmap = CBitmap::FromHandle( hbitmap );
  
	CBitmap* oldbitmap = memDC.SelectObject( bitmap );
  dc->SetStretchBltMode(COLORONCOLOR);
	dc->StretchBlt( rect.left, rect.top, rect.Width(), rect.Height() - round( 12 * GetZoom() ), &memDC, 0, 0, 64, 64, SRCCOPY );
	memDC.SelectObject( oldbitmap );
	bitmap->DeleteObject();

	CRect r( rect );
	r.top = r.bottom - round( 12.0 * GetZoom() );
  r.left = r.left - round( 40 * GetZoom() );
  r.right = r.right+round( 40 * GetZoom() );
	CFont font;
	font.CreateFont( -round( 10.0 * GetZoom() ), 0,0,0,FW_NORMAL,0,0,0,0,0,0,0,0, _T( "Arial" ) );
	dc->SelectObject( &font );

	COLORREF oldback = dc->GetBkColor();
	COLORREF oldtext = dc->GetTextColor();
	if( IsSelected() )
	{
		dc->SetTextColor( RGB( 255, 255, 255 ) );
		dc->SetBkColor( RGB( 128, 128, 128 ) );
	}

	dc->DrawText( GetTitle(), r, DT_NOPREFIX | DT_WORDBREAK | DT_CENTER );
	dc->SelectStockObject( DEFAULT_GUI_FONT );
	dc->SetTextColor( oldtext );
	dc->SetBkColor( oldback );

}

void CNetworkSymbol::Copy( CDiagramEntity* obj )
/* ============================================================
	Function :		CNetworkSymbol::Copy
	Description :	Copy the information in obj to this object.
					
	Return :		void
	Parameters :	CDiagramEntity* obj	-	The object to copy 
											from.
					
	Usage :			Copies basic information. from obj to this.
					GetType can be used to check for the correct 
					object type in overridden versions.
   ============================================================*/
{

	CDiagramEntity::Copy( obj );
	m_symbol = static_cast< CNetworkSymbol* >( obj )->m_symbol;

}

void CNetworkSymbol::DrawSelectionMarkers( CDC* dc, CRect rect ) const
/* ============================================================
	Function :		CNetworkSymbol::DrawSelectionMarkers
	Description :	
					
	Return :		void
	Parameters :	CDC*	-	CDC to draw to
					CRect	-	True rectangle of symbol
					
	Usage :			Overridden to get rid of selection markers. 
					We draw the selected state by darkening 
					the title instead.

   ============================================================*/
{
	// Leave empty

	CDiagramEntity::DrawSelectionMarkers( dc, rect );

}

BOOL CNetworkSymbol::FromString( const CString& str )
/* ============================================================
	Function :		CNetworkSymbol::FromString
	Description :	Sets the values for an object from str. 
					
	Return :		BOOL				-	TRUE if str 
											represents an 
											object of this 
											type.
	Parameters :	const CString& str	-	Possible text 
											format 
											representation.
					
	Usage :			Can be called to fill an existing object 
					with information from a string created with 
					GetString.

   ============================================================*/
{
	BOOL result = FALSE;
	CString data( str );

	if( LoadFromString( data ) )
	{
		CTokenizer tok( data );

		int resid;
		tok.GetAt( 0, resid );

		SetSymbol( resid );
		result = TRUE;

	}

	return result;

}

CString CNetworkSymbol::GetString() const
/* ============================================================
	Function :		CNetworkSymbol::GetString
	Description :	Creates a string representing the object.
					
	Return :		CString	-	The resulting string
	Parameters :	none

	Usage :			Used to save this object to a text file.

   ============================================================*/
{

	CString str;

	str.Format( _T( ",%i;" ), 
			GetSymbol()
		);

	str = GetDefaultGetString() + str;
	return str;

}

UINT CNetworkSymbol::GetSymbol() const
/* ============================================================
	Function :		CNetworkSymbol::GetSymbol
	Description :	Gets the current resource id for the symbol.
	Access :		

	Return :		UINT	-	Current resource id
	Parameters :	none

	Usage :			Call to get the current resource id.

   ============================================================*/
{

	return m_symbol;

}
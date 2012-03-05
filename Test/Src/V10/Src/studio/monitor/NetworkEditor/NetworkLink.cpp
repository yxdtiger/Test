/* ==========================================================================
	CNetworkLink

	Author :		Johan Rosengren, Abstrakt Mekanik AB

	Date :			2004-05-04

	Purpose :		CNetworkLink represents a link between two objects.	

	Description :	CNetworkLink works as a struct with functions helping 
					loading and saving modellen on those of CDiagramEntity.

	Usage :			Instances of this class are managed by 
					CNetworkEntityContainer.

   ========================================================================*/
#include "../../main_frame/stdafx.h"
#include "NetworkLink.h"
#include "DiagramEditor/Tokenizer.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


CNetworkLink::CNetworkLink()
/* ============================================================
	Function :		CNetworkLink::CNetworkLink
	Description :	constructor
					
	Return :		void
	Parameters :	none

	Usage :			

   ============================================================*/
{
  network_stat = -1;
  flow = 0;
}

CNetworkLink::~CNetworkLink()
/* ============================================================
	Function :		CNetworkLink::~CNetworkLink
	Description :	destructor
					
	Return :		void
	Parameters :	none

	Usage :			

   ============================================================*/
{
}

CNetworkLink* CNetworkLink::Clone() const
/* ============================================================
	Function :		CNetworkLink::Clone
	Description :	Clone this object to a new object.
					
	Return :		CDiagramEntity*	-	The new object.
	Parameters :	none

	Usage :			Call to create a clone of the object. The 
					caller will have to delete the object.

   ============================================================*/
{

	CNetworkLink* obj = new CNetworkLink;
	obj->from = from;
	obj->to = to;
  obj->network_stat = network_stat;
	return obj;

}

CString CNetworkLink::GetString() const 
/* ============================================================
	Function :		CNetworkLink::GetString
	Description :	Returns this object as a string
					
	Return :		CString	-	String representation of this 
								link.
	Parameters :	none

	Usage :			Call to save the object to a file.

   ============================================================*/
{

	CString str;
	str.Format( _T( "network_link:%s,%s;" ), from, to );
	return str;

}

BOOL CNetworkLink::FromString( const CString& str )
/* ============================================================
	Function :		CNetworkLink::FromString
	Description :	Creates a CNetworkLink-object from a 
					string.
					
	Return :		BOOL				-	TRUE if str is a 
											representation of 
											a link
	Parameters :	const CString& str	-	String representation 
											of a link.
					
	Usage :			Call to load data from a text stream.

   ============================================================*/
{

	BOOL result = FALSE;
	CTokenizer main( str, _T( ":" ) );
	CString header;
	CString data;
	if( main.GetSize() == 2 )
	{
		main.GetAt( 0, header );
		main.GetAt( 1, data );
		header.TrimLeft();
		header.TrimRight();
		data.TrimLeft();
		data.TrimRight();
		if( header == _T( "network_link" ) )
		{
			CTokenizer tok( data.Left( data.GetLength() - 1 ) );
			int size = tok.GetSize();
			if( size == 2 )
			{
				CString readfrom;
				CString readto;

				tok.GetAt(0, readfrom );
				tok.GetAt(1, readto );

				from = readfrom;
				to = readto;

				result = TRUE;
			}
		}
	}

	return result;

}


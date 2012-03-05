#include "../../main_frame/stdafx.h"
#include "NetworkControlFactory.h"
#include "NetworkSymbol.h"

CDiagramEntity* CNetworkControlFactory::CreateFromString( const CString& str )
/* ============================================================
	Function :		CNetworkControlFactory::CreateFromString
	Description :	The function returns an object from the 
					parameter str.
					
	Return :		CDiagramEntity*		-	The new object, or 
											NULL is str is not a 
											valid representation.
	Parameters :	const CString& str	-	The string to create 
											a new object from
					
	Usage :			Call this static function while reading 
					string representations of objects from a 
					text file. Note that the caller is 
					responsible for the allocated memory.

   ============================================================*/
{
	CDiagramEntity* obj;

	obj = CNetworkSymbol::CreateFromString( str );

	return obj;
}
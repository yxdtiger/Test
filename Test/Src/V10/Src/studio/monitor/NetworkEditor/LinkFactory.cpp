/* ==========================================================================
	CLinkFactory

	Author :		Johan Rosengren, Abstrakt Mekanik AB

	Date :			2004-04-29

	Purpose :		CLinkFactory is a simple class with one single static 
					function, returning and updating a single static int 
					value to use as a unique id for drawing objects.	

	Description :	See below

	Usage :			Call CLinkFactory::GetID to get a unique id during this 
					session.

   ========================================================================*/
#include "../../main_frame/stdafx.h""
#include "LinkFactory.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

int CLinkFactory::s_currentID = 0;

CString CLinkFactory::GetID()
/* ============================================================
	Function :		CLinkFactory::GetID
	Description :	Returns a unique int every time it is 
					called.
					
	Return :		CString	-	CString with the unique id.
	Parameters :	none

	Usage :			Call to get a unique id. Used to get unique 
					ids for the object name attributes.

   ============================================================*/
{
	CString str;

	str.Format( _T( "%i" ), CLinkFactory::s_currentID );
	CLinkFactory::s_currentID++;

	return str;

}
/* ==========================================================================
	CNetworkEditorClipboardHandler

	Author :		Johan Rosengren, Abstrakt Mekanik AB

	Date :			2004-05-04

	Purpose :		CNetworkEditorClipboardHandler is a clipboard handler 
					for CNetworkEditor.

	Description :	The class, in addition to the default handling, will 
					take care of assigning new ids to copied objects. It 
					will also copy and paste links.

	Usage :			Add to the editor container using 
					CDiagramEntityContainer::SetClipboardHandler

   ========================================================================*/
#include "../../main_frame/stdafx.h"
#include "NetworkEditorClipboardHandler.h"
#include "NetworkEntityContainer.h"
#include "NetworkLink.h"
#include "LinkFactory.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CNetworkEditorClipboardHandler::CNetworkEditorClipboardHandler()
/* ============================================================
	Function :		CNetworkEditorClipboardHandler::CNetworkEditorClipboardHandler
	Description :	constructor
					
	Return :		void
	Parameters :	none

	Usage :			

   ============================================================*/
{
}

CNetworkEditorClipboardHandler::~CNetworkEditorClipboardHandler()
/* ============================================================
	Function :		CNetworkEditorClipboardHandler::~CNetworkEditorClipboardHandler
	Description :	destructor
					
	Return :		void
	Parameters :	none

	Usage :			

   ============================================================*/
{

	ClearPaste();

}

void CNetworkEditorClipboardHandler::Copy( CDiagramEntity* obj )
/* ============================================================
	Function :		CNetworkEditorClipboardHandler::Copy
	Description :	Copies obj to the paste array
					
	Return :		void
	Parameters :	CDiagramEntity* obj	-	Object to copy.
					
	Usage :			Overridden to assign a new id to newobj.

   ============================================================*/
{

	if( obj )
	{
		ClearPaste();
		CDiagramEntity* newobj = obj->Clone();
		newobj->Select( TRUE );
		newobj->MoveRect( 10, 10 );
		newobj->SetName( CLinkFactory::GetID() );
		GetData()->Add( newobj );
	}

}

void CNetworkEditorClipboardHandler::Paste( CDiagramEntityContainer* container )
/* ============================================================
	Function :		CNetworkEditorClipboardHandler::Paste
	Description :	Pastes the contents of the paste array to 
					the data array.
					
	Return :		void
	Parameters :	none

	Usage :			Overridden to paste links as well.

   ============================================================*/
{

	CDiagramClipboardHandler::Paste( container );
	int max = m_pasteLinks.GetSize();
	CNetworkEntityContainer* flow = static_cast< CNetworkEntityContainer* >( container );
	for( int t = 0 ; t < max ; t++ )
		flow->AddLink( ( static_cast< CNetworkLink* >( m_pasteLinks[ t ] ) )->Clone() );

}

void CNetworkEditorClipboardHandler::CopyAllSelected( CDiagramEntityContainer* container )
/* ============================================================
	Function :		CNetworkEditorClipboardHandler::CopyAllSelected
	Description :	Copies all the selected items to the paste 
					array.
					
	Return :		void
	Parameters :	none

	Usage :			Overridden to add links as well. New ids 
					are assigned to the copied objects, and the 
					copied links are updated.

   ============================================================*/
{

	CDiagramClipboardHandler::CopyAllSelected( container );
	CNetworkEntityContainer* flow = static_cast< CNetworkEntityContainer* >( container );
	CObArray* links = flow->GetLinkArray();

	int max = links->GetSize();
	for( int t = 0; t < max ; t++ )
	{
		CNetworkLink* link = static_cast< CNetworkLink* >( links->GetAt(t ) );
		m_pasteLinks.Add( link->Clone() );
	}

	CObArray* paste = GetData();
	max = paste->GetSize();

	for(int t = 0; t < max ; t++ )
	{
		CDiagramEntity* obj = static_cast< CDiagramEntity* >( paste->GetAt( t ) );
		CString newID = CLinkFactory::GetID();

		int maxlinks = m_pasteLinks.GetSize();
		for( int i = 0 ; i < maxlinks ; i++ )
		{
			CNetworkLink* link = static_cast< CNetworkLink* >( m_pasteLinks[ i ] );
			if( link->from == obj->GetName() )
				link->from = newID;
			if( link->to == obj->GetName() )
				link->to = newID;
		}

		obj->SetName( newID );

	}

}

void CNetworkEditorClipboardHandler::ClearPaste()
/* ============================================================
	Function :		CNetworkEditorClipboardHandler::ClearPaste
	Description :	Clears the paste array.
					
	Return :		void
	Parameters :	none

	Usage :			Overridden to clear the paste link array as 
					well.

   ============================================================*/
{

	CDiagramClipboardHandler::ClearPaste();

	int max = m_pasteLinks.GetSize();
	for( int t = max - 1 ; t >= 0 ; t-- )
		delete m_pasteLinks[t];

	m_pasteLinks.RemoveAll();

}


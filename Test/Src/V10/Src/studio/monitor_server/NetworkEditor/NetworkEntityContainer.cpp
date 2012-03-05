/* ==========================================================================
	CNetworkEntityContainer

	Author :		Johan Rosengren, Abstrakt Mekanik AB

	Date :			2004-05-04

	Purpose :		CNetworkEntityContainer is derived from 
					CDiagramEntityContainer, and acts as the data repository 
					for CNetworkEditor.

	Description :	In addition to default handling, CNetworkEntityContainer 
					handle the link objects, as well as enhance the undo-
					functionality to take link operations into consideration.

	Usage :			As CDiagramEntityContainer.

   ========================================================================*/
#include "../../main_frame/stdafx.h"
#include "NetworkEntityContainer.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CNetworkEntityContainer::CNetworkEntityContainer()
/* ============================================================
	Function :		CNetworkEntityContainer::CNetworkEntityContainer
	Description :	constructor
					
	Return :		void
	Parameters :	none

	Usage :			

   ============================================================*/
{
}

CNetworkEntityContainer::~CNetworkEntityContainer()
/* ============================================================
	Function :		CNetworkEntityContainer::~CNetworkEntityContainer
	Description :	destructor
					
	Return :		void
	Parameters :	none

	Usage :			

   ============================================================*/
{

	ClearLinks();
	ClearUndo();

}

void CNetworkEntityContainer::RemoveAt( int index )
/* ============================================================
	Function :		CNetworkEntityContainer::RemoveAt
	Description :	Removes the object at index. Will also 
					remove all links refering to this object.
					
	Return :		void
	Parameters :	int index	-	Index for object to remove.
					
	Usage :			Overridden to remove links as well.

   ============================================================*/
{

	CDiagramEntity* obj = GetAt( index );
	if( obj )
	{
		CString name = obj->GetName();
		CDiagramEntityContainer::RemoveAt( index );

		int max = m_links.GetUpperBound();
		for( int t = max; t >= 0 ; t-- )
		{
			CNetworkLink* link = static_cast< CNetworkLink* >( m_links.GetAt( t ) );
			if( link->to == name || link->from == name )
			{
				delete link;
				m_links.RemoveAt( t );
			}
		}
	}

}

/////////////////////////////////////////////////////////////////////////////
// Undo operations

void CNetworkEntityContainer::Undo()
/* ============================================================
	Function :		CNetworkEntityContainer::Undo
	Description :	Undo the latest operation
					
	Return :		void
	Parameters :	none

	Usage :			Overridden to also undo link operations.

   ============================================================*/
{

	CDiagramEntityContainer::Undo();

	if( m_undoLinks.GetSize() )
	{
		CObArray* undo = static_cast< CObArray* >( m_undoLinks.GetAt( m_undoLinks.GetUpperBound() ) );
		int count = undo->GetSize();
		for( int t = 0 ; t < count ; t++ )
		{

			CNetworkLink* obj = static_cast< CNetworkLink* >( undo->GetAt( t ) );
			AddLink( obj->Clone() );

		}

		int max = undo->GetSize();
		for(int t = max - 1 ; t >= 0 ; t-- )
			delete undo->GetAt( t );
		undo->RemoveAll();
		delete undo;

		m_undoLinks.RemoveAt( m_undoLinks.GetUpperBound() );

	}

}

void CNetworkEntityContainer::Snapshot()
/* ============================================================
	Function :		CNetworkEntityContainer::Snapshot
	Description :	Creates a snapshot of the current data 
					state for the undo-functionality.
					
	Return :		void
	Parameters :	none

	Usage :			Overridden to save the link state as well.

   ============================================================*/
{

	CDiagramEntityContainer::Snapshot();
	if( GetUndoStackSize() > 0 && m_undoLinks.GetSize() == GetUndoStackSize() )
	{
		delete m_undoLinks.GetAt( 0 );
		m_undoLinks.RemoveAt( 0 );
	}

	CObArray* undo = new CObArray;

	while( !undo && m_undoLinks.GetSize() )
	{

		delete m_undoLinks.GetAt( 0 );
		m_undoLinks.RemoveAt( 0 );
		undo = new CObArray;

	}

	if( undo )
	{

		// Save all objects
		int count = m_links.GetSize();
		for( int t = 0 ; t < count ; t++ )
			undo->Add( GetLinkAt( t )->Clone() );

		// Add to undo stack
		m_undoLinks.Add( undo );

	}

}

void CNetworkEntityContainer::ClearUndo()
/* ============================================================
	Function :		CNetworkEntityContainer::ClearUndo
	Description :	Clears the undo-array
					
	Return :		void
	Parameters :	none

	Usage :			Overridden to also clear the link undo 
					states.

   ============================================================*/
{

	CDiagramEntityContainer::ClearUndo();
	int count = m_undoLinks.GetSize() - 1;
	for( int t = count ; t >= 0 ; t-- )
	{
		CObArray* undo = static_cast< CObArray* >( m_undoLinks.GetAt(t) );

		// Remove all objects in the stack entry
		int max = undo->GetSize();
		for( int i = 0; i < max ; i++ )
			delete undo->GetAt( i );
		undo->RemoveAll();

		// Remove the stack entry itself.
		delete undo;
	}

	m_undoLinks.RemoveAll();

}

/////////////////////////////////////////////////////////////////////////////
// Link operations

int CNetworkEntityContainer::GetLinks() const
/* ============================================================
	Function :		CNetworkEntityContainer::GetLinkCount
	Description :	
					
	Return :		int	-	
	Parameters :	none

	Usage :			

   ============================================================*/
{

	return m_links.GetSize();

}

CObArray* CNetworkEntityContainer::GetLinkArray()
/* ============================================================
	Function :		CNetworkEntityContainer::GetLinkArray
	Description :	
					
	Return :		CObArray*	-	
	Parameters :	none

	Usage :			

   ============================================================*/
{

	return &m_links;

}

CNetworkSymbol* CNetworkEntityContainer::GetSecondaryLink()
/* ============================================================
	Function :		CNetworkEntityContainer::GetSecondaryLink
	Description :	Returns the secondary object of the two 
					currently selected and linked.
					
	Return :		CNetworkSymbol*	-	Secondary object or 
										NULL if none.
	Parameters :	none

	Usage :			Returns NULL if not exactly two objects are 
					selected, or they are not linked.

   ============================================================*/
{

	CNetworkSymbol* result = NULL;

	if( GetSelectCount() == 2 )
	{
		int max = GetSize();
		CNetworkSymbol* primary = NULL;
		CNetworkSymbol* secondary = NULL;

		for( int t = 0 ; t < max ; t++ )
		{
			CNetworkSymbol* obj = dynamic_cast< CNetworkSymbol* >( GetAt( t ) );
			if( obj && obj->IsSelected() )
			{
				if( primary == NULL )
					primary = obj;
				else
					secondary = obj;
			}
		}

		if( primary && secondary )
		{
			CNetworkLink* link = FindLink( primary, secondary );
			if( link )
			{
				if( primary->GetName() == link->from )
					result = secondary;
				else
					result = primary;
			}
		}
	}

	return result;

}

BOOL CNetworkEntityContainer::CanLink()
/* ============================================================
	Function :		CNetworkEntityContainer::CanLink
	Description :	Returns if the currently selected objects 
					can be linked.
					
	Return :		BOOL	-	TRUE if currently exactly two 
								objects are selected and they 
								can be linked.
	Parameters :	none
					
	Usage :			Intended as a command enabler for a Link 
					command.

   ============================================================*/
{

	BOOL result = TRUE;

	if( GetSelectCount() == 2 )
	{
		CNetworkSymbol* primary = GetPrimarySelected();
		CNetworkSymbol* secondary = GetSecondarySelected();

		if( primary && secondary )
		{
			// Loop all links, trying to find one having both primary and secondary as nodes
			if( FindLink( primary, secondary ) )
				result = FALSE;
		}
	}

	return result;

}

BOOL CNetworkEntityContainer::IsLinked()
/* ============================================================
	Function :		CNetworkEntityContainer::IsLinked
	Description :	Returns the current link-state of the data.
					
	Return :		BOOL	-	TRUE if two exactly objects are 
								selected and linked.
	Parameters :	none
					
	Usage :			Intended as a command enabler for an Unlink
					command.

   ============================================================*/
{

	BOOL result = FALSE;

	if( GetSelectCount() == 2 )
	{
		CNetworkSymbol* primary = GetPrimaryLink();
		CNetworkSymbol* secondary = GetSecondaryLink();

		result = ( primary && secondary );
	}

	return result;

}

CNetworkLink* CNetworkEntityContainer::GetLinkAt( int index )
/* ============================================================
	Function :		CNetworkEntityContainer::GetLinkAt
	Description :	Returns the link object at position index 
					in the link array.
					
	Return :		CNetworkLink*	-	Pointer to the link, or
										NULL if out of bounds.
	Parameters :	int index		-	Index to get link from.
					
	Usage :			Call to get the link at a specific index in 
					the array.

   ============================================================*/
{

	CNetworkLink* link = NULL;
	if( index < GetLinks() )
		link = dynamic_cast< CNetworkLink* >( m_links.GetAt( index ) );
	return link;

}

CNetworkLink* CNetworkEntityContainer::FindLink( CDiagramEntity* obj1, CDiagramEntity* obj2 )
/* ============================================================
	Function :		CNetworkEntityContainer::FindLink
	Description :	Finds a link between obj1 and obj2.
					
	Return :		CNetworkLink*			-	Link between obj1 
												and obj2, or NULL 
												if they are not 
												linked.
	Parameters :	CDiagramEntity* obj1	-	First object to 
												test.
					CDiagramEntity* obj2	-	Second object 
												to test.
					
	Usage :			Call to get the link between obj1 and obj2, 
					or to check if obj1 and obj2 are linked.

   ============================================================*/
{

	CNetworkLink* result = NULL;

	if( obj1 && obj2 )
	{

		int max = m_links.GetUpperBound();
		for( int t = max; t >= 0 ; t-- )
		{
			CNetworkLink* link = static_cast< CNetworkLink* >( m_links.GetAt( t ) );
			if( ( link->from == obj1->GetName() && link->to == obj2->GetName() ) || ( link->from == obj2->GetName() && link->to == obj1->GetName() ) )
				result = link;
		}
	}

	return result;

}

void CNetworkEntityContainer::ClearLinks()
/* ============================================================
	Function :		CNetworkEntityContainer::ClearLinks
	Description :	Clears the link array.
					
	Return :		void
	Parameters :	none

	Usage :			Call to remove all links.

   ============================================================*/
{

	int max = m_links.GetSize();
	for( int t = 0 ; t < max ; t++ )
		delete m_links[ t ];

	m_links.RemoveAll();

}

CNetworkSymbol* CNetworkEntityContainer::GetPrimaryLink()
/* ============================================================
	Function :		CNetworkEntityContainer::GetPrimaryLink
	Description :	Returns the primary object of the two 
					currently selected and linked.
					
	Return :		CNetworkSymbol*	-	Primary object or 
										NULL if none.
	Parameters :	none

	Usage :			Returns NULL if not exactly two objects are 
					selected, or they are not linked.

   ============================================================*/
{

	CNetworkSymbol* result = NULL;

	if( GetSelectCount() == 2 )
	{
		int max = GetSize();
		CNetworkSymbol* primary = NULL;
		CNetworkSymbol* secondary = NULL;

		for( int t = 0 ; t < max ; t++ )
		{
			CNetworkSymbol* obj = dynamic_cast< CNetworkSymbol* >( GetAt( t ) );
			if( obj && obj->IsSelected() )
			{
				if( primary == NULL )
					primary = obj;
				else
					secondary = obj;
			}
		}

		if( primary && secondary )
		{
			CNetworkLink* link = FindLink( primary, secondary );
			if( link )
			{
				if( primary->GetName() == link->from )
					result = primary;
				else
					result = secondary;
			}
		}
	}

	return result;

}

CNetworkSymbol* CNetworkEntityContainer::GetPrimarySelected()
/* ============================================================
	Function :		CNetworkEntityContainer::GetPrimarySelected
	Description :	Returns the primary object of the two 
					currently selected.
					
	Return :		CNetworkSymbol*	-	Primary object or 
										NULL if none.
	Parameters :	none

	Usage :			Returns NULL if not exactly two objects are 
					selected.

   ============================================================*/
{

	CNetworkSymbol* result = NULL;

	if( GetSelectCount() == 2 )
	{
		int max = GetSize();

		for( int t = 0 ; t < max ; t++ )
		{
			CNetworkSymbol* obj = dynamic_cast< CNetworkSymbol* >( GetAt( t ) );
			if( obj && obj->IsSelected() )
			{
				if( result == NULL )
					result = obj;
			}
		}
	}

	return result;

}

CNetworkSymbol* CNetworkEntityContainer::GetSecondarySelected()
/* ============================================================
	Function :		CNetworkEntityContainer::GetSecondarySelected
	Description :	Returns the secondary object of the two 
					currently selected.
					
	Return :		CNetworkSymbol*	-	secondary object or 
										NULL if none.
	Parameters :	none

	Usage :			Returns NULL if not exactly two objects are 
					selected.

   ============================================================*/
{

	CNetworkSymbol* result = NULL;

	if( GetSelectCount() == 2 )
	{
		int max = GetSize();

		for( int t = 0 ; t < max ; t++ )
		{
			CNetworkSymbol* obj = dynamic_cast< CNetworkSymbol* >( GetAt( t ) );
			if( obj && obj->IsSelected() )
				result = obj;
		}
	}

	return result;

}

int	CNetworkEntityContainer::GetSelectCount()
/* ============================================================
	Function :		int	CNetworkEntityContainer::GetSelectCount
	Description :	Returns the number of currently selected 
					objects.
					
	Return :		int		-	The number of selected objects.
	Parameters :	none

	Usage :			Call to see how many objects are selected.

   ============================================================*/
{

	int count = 0;
	int max = GetSize();

	for( int t = 0 ; t < max ; t++ )
	{
		CNetworkSymbol* obj = dynamic_cast< CNetworkSymbol* >( GetAt( t ) );

		if( obj && obj->IsSelected() )
			count++;
	}

	return count;

}

void CNetworkEntityContainer::AddLink( CNetworkLink* link )
/* ============================================================
	Function :		CNetworkEntityContainer::AddLink
	Description :	Adds a links to the link array.
					
	Return :		void
	Parameters :	CNetworkLink* link	-	Link to add
					
	Usage :			Call to add a link to the link array.

   ============================================================*/
{

	m_links.Add( link );

}

BOOL CNetworkEntityContainer::CreateLink( CNetworkSymbol* from, CNetworkSymbol* to )
/* ============================================================
	Function :		CNetworkEntityContainer::CreateLink
	Description :	Creates a link between from and to and 
					puts it into the link-array.
					
	Return :		BOOL					-	TRUE if ok.
	Parameters :	CNetworkSymbol* from	-	From-object.
					CNetworkSymbol* to		-	To-object.
					
	Usage :			Call to create a link between from and to.

   ============================================================*/
{

	BOOL result = FALSE;

	if( CanLink() )
	{

		CNetworkLink* link = new CNetworkLink;

		link->from = from->GetName();
		link->to = to->GetName();

		m_links.Add( link );
		result = TRUE;
	}

	return result;

}

void CNetworkEntityContainer::DeleteLink( CNetworkLink* inlink )
/* ============================================================
	Function :		CNetworkEntityContainer::DeleteLink
	Description :	Finds and deletes inlink from the internal 
					link array.
					
	Return :		void
	Parameters :	CNetworkLink* inlink	-	Link to delete
					
	Usage :			Call to delete a link from the link array.

   ============================================================*/
{

	int max = m_links.GetUpperBound();
	for( int t = max; t >= 0 ; t-- )
	{
		CNetworkLink* link = static_cast< CNetworkLink* >( m_links.GetAt( t ) );
		if( link == inlink )
		{
			delete link;
			m_links.RemoveAt( t );
		}
	}

}


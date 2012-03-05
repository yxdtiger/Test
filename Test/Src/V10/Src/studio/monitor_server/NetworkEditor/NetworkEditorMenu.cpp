#include "../../main_frame/stdafx.h"
#include "NetworkEditorMenu.h"
#include "NetworkEditor.h"

#include "../../main_frame/resource.h"

CNetworkEditorMenu::CNetworkEditorMenu()
{
}

CNetworkEditorMenu::~CNetworkEditorMenu()
{
}

CMenu* CNetworkEditorMenu::GetPopupMenu( CDiagramEditor* editor )
{

	if( m_menu.m_hMenu == NULL )
		m_menu.LoadMenu( IDR_MENU_EDITOR_POPUP );

	UINT cut = MF_GRAYED;
	UINT copy = MF_GRAYED;
	UINT paste = MF_GRAYED;
	UINT link = MF_GRAYED;
	UINT linked = MF_GRAYED;

	// Copy/paste
	if( editor->IsAnyObjectSelected() )
	{
		cut = 0;
		copy = 0;
	}

	if( editor->GetDiagramEntityContainer() && editor->GetDiagramEntityContainer()->ObjectsInPaste() )
		paste = 0;

	// Link-related
	CNetworkEditor* ed = dynamic_cast< CNetworkEditor* >( editor );
	if( ed )
	{
		if( ed->CanLink() )
			link = 0;
		if( ed->IsLinked() )
			linked = 0;
	}

	m_menu.EnableMenuItem( ID_EDIT_CUT, MF_BYCOMMAND | cut );
	m_menu.EnableMenuItem( ID_EDIT_COPY, MF_BYCOMMAND | copy );
	m_menu.EnableMenuItem( ID_EDIT_PASTE, MF_BYCOMMAND | paste );
	m_menu.EnableMenuItem( IDM_NETWORKEDITOR_LINK, MF_BYCOMMAND | link );
	m_menu.EnableMenuItem( IDM_NETWORKEDITOR_UNLINK, MF_BYCOMMAND | linked );

	return m_menu.GetSubMenu( 0 );
}

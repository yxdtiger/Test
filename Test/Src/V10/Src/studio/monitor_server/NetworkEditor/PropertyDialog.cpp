/* ==========================================================================
	Class :			CPropertyDialog

	Author :		Johan Rosengren, Abstrakt Mekanik AB

	Date :			2004-05-04

	Purpose :		CPropertyDialog is a property dialog for CNetworkSymbol-
					objects.	

	Description :	Derived from ClassWizard, the base class is changed to 
					CDiagramPropertyDlg afterwards.

	Usage :			Managed by CNetworkEditor. CNetworkSymbol contains a 
					member of this class that is attached as a property 
					dialog in the ctor.

   ========================================================================*/
#include "../../main_frame/stdafx.h"
#include "PropertyDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPropertyDialog dialog


CPropertyDialog::CPropertyDialog(CWnd* pParent /*=NULL*/)
	: CDiagramPropertyDlg(CPropertyDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPropertyDialog)
	m_title = _T("");
	//}}AFX_DATA_INIT
}

void CPropertyDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPropertyDialog)
	DDX_Text(pDX, IDC_EDIT_TITLE, m_title);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CPropertyDialog, CDialog)
	//{{AFX_MSG_MAP(CPropertyDialog)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPropertyDialog message handlers

void CPropertyDialog::OnCancel() 
/* ============================================================
	Function :		CPropertyDialog::OnCancel
	Description :	Sets the focus to the editor
					
	Return :		void
	Parameters :	none

	Usage :			Called from MFC

   ============================================================*/
{
	CDialog::OnCancel();
	GetRedrawWnd()->SetFocus();
}

void CPropertyDialog::OnOK() 
/* ============================================================
	Function :		CPropertyDialog::OnOK
	Description :	Updates the object from the dialog box, 
					redrawing the editor window.
					
	Return :		void
	Parameters :	none

	Usage :			Called from MFC

   ============================================================*/
{
	UpdateData( TRUE );

	GetEntity()->SetTitle( m_title );
	Redraw();

	CDialog::OnOK();
	GetRedrawWnd()->SetFocus();
}

void CPropertyDialog::SetValues()
/* ============================================================
	Function :		CPropertyDialog::SetValues
	Description :	Sets the values of the dialog box fields 
					from the attached object.
					
	Return :		void
	Parameters :	none

	Usage :			Called from CDiagramEditor.

   ============================================================*/
{

	m_title = GetEntity()->GetTitle();

	if( m_hWnd )
		UpdateData( FALSE );

}


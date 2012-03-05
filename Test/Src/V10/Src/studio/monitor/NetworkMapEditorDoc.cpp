// NetworkMapEditorDoc.cpp : implementation of the CNetworkMapEditorDoc class
//

#include "../main_frame/StdAfx.h"
#include "../main_frame/WMDF_Studio.h"

#include "NetworkMapEditorDoc.h"

#include "NetworkEditor/NetworkLink.h"
#include "NetworkEditor/NetworkControlFactory.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#pragma warning( disable : 4706 )

/////////////////////////////////////////////////////////////////////////////
// CNetworkMapEditorDoc

IMPLEMENT_DYNCREATE(CNetworkMapEditorDoc, CDocument)

BEGIN_MESSAGE_MAP(CNetworkMapEditorDoc, CDocument)
	//{{AFX_MSG_MAP(CNetworkMapEditorDoc)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNetworkMapEditorDoc construction/destruction

CNetworkMapEditorDoc::CNetworkMapEditorDoc()
{
}

CNetworkMapEditorDoc::~CNetworkMapEditorDoc()
{
}

BOOL CNetworkMapEditorDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	m_objs.Clear();

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CNetworkMapEditorDoc serialization

void CNetworkMapEditorDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		ar.WriteString( m_objs.GetString() + _T( "\r\n" ) );
		int count = 0;
		CDiagramEntity* obj;
		while( ( obj = m_objs.GetAt( count++ ) ) )
			ar.WriteString( obj->GetString() + _T( "\r\n" ) );

		int max = m_objs.GetLinks();
		for( int t = 0 ; t < max ; t++ )
		{
			CNetworkLink* link = m_objs.GetLinkAt( t );
			if( link )
				ar.WriteString( link->GetString() + _T( "\r\n" ) );
		}

		m_objs.SetModified( FALSE );
	}
	else
	{

		m_objs.Clear();
		CString str;
		while(ar.ReadString( str ) )
		{

			if( !m_objs.FromString( str ) )
			{
				CDiagramEntity* obj = CNetworkControlFactory::CreateFromString( str );
				if( obj )
					m_objs.Add( obj );
				else
				{
					CNetworkLink* link = new CNetworkLink;
					if( link->FromString( str ) )
						m_objs.AddLink( link );
					else
						delete link;
				}
			}
		}

		m_objs.SetModified( FALSE );

	}
}

/////////////////////////////////////////////////////////////////////////////
// CNetworkMapEditorDoc diagnostics

#ifdef _DEBUG
void CNetworkMapEditorDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CNetworkMapEditorDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CNetworkMapEditorDoc commands

BOOL CNetworkMapEditorDoc::SaveModified() 
{
  return TRUE;
	/*SetModifiedFlag( m_objs.IsModified() );
	return CDocument::SaveModified();*/
}



#pragma warning( default : 4706 )

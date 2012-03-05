// NetworkMapEditorView.cpp : implementation of the CNetworkMapEditorView class
//

#include "../main_frame/StdAfx.h"

#include "../main_frame/WMDF_Studio.h"
#include "../../include/wmdf_structs.h"
#include "../../environment/strutils.h"

#include "server_manager.h"
#include "monitor_environment.h"
#include "model_id_factory.h"

#include "NetworkMapEditorDoc.h"
#include "NetworkMapEditorView.h"
#include "NetworkEditor/NetworkSymbol.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using namespace wmdf;
/////////////////////////////////////////////////////////////////////////////
// CNetworkMapEditorView

IMPLEMENT_DYNCREATE(CNetworkMapEditorView, CView)

BEGIN_MESSAGE_MAP(CNetworkMapEditorView, CView)
	//{{AFX_MSG_MAP(CNetworkMapEditorView)

	ON_WM_SIZE()
	//}}AFX_MSG_MAP
	
  ON_WM_TIMER()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNetworkMapEditorView construction/destruction

CNetworkMapEditorView::CNetworkMapEditorView()
{
	m_screenResolutionX = 0;
  mid_symbol_=NULL;
}

CNetworkMapEditorView::~CNetworkMapEditorView()
{
  ServerNetInfo* server_info = ServerManager::Instance()->net_info_collection()->find_server_by_cid(conn_id_);
  if(NULL!=server_info)
    server_info->set_is_drawed(false);
}

BOOL CNetworkMapEditorView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CNetworkMapEditorView drawing

void CNetworkMapEditorView::OnDraw(CDC* pDC)
{
	CNetworkMapEditorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	if( pDC->IsPrinting() )
	{

		COLORREF col = m_editor.GetBackgroundColor();
		// Print zoom is the difference between screen- 
		// and printer resolution.
		double zoom = pDC->GetDeviceCaps( LOGPIXELSX ) / m_screenResolutionX;

		CRect rect( 0,0, 
			( int )( ( double ) m_editor.GetVirtualSize().cx * zoom ), 
			( int )( ( double ) m_editor.GetVirtualSize().cy * zoom ) );
		m_editor.SetRedraw( FALSE );
		m_editor.SetBackgroundColor( RGB( 255, 255, 255 ) );
		m_editor.Print( pDC, rect, zoom );
		m_editor.SetBackgroundColor( col );
		m_editor.SetRedraw( TRUE );

	}
}



/////////////////////////////////////////////////////////////////////////////
// CNetworkMapEditorView diagnostics

#ifdef _DEBUG
void CNetworkMapEditorView::AssertValid() const
{
	CView::AssertValid();
}

void CNetworkMapEditorView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CNetworkMapEditorDoc* CNetworkMapEditorView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CNetworkMapEditorDoc)));
	return (CNetworkMapEditorDoc*)m_pDocument;
}
#endif //_DEBUG


void CNetworkMapEditorView::OnInitialUpdate() 
{
  CView::OnInitialUpdate();

  if( !m_editor.m_hWnd )
  {
    // Creating the editor window
    CNetworkMapEditorDoc* pDoc = GetDocument();

    CRect rect;
    GetClientRect( rect );

    pDoc->GetData()->SetClipboardHandler( &theApp.m_clip );
    m_editor.Create( WS_CHILD | WS_VISIBLE, rect, this, pDoc->GetData() );

    m_editor.SetModified( FALSE );

    CClientDC dc( this );
    m_screenResolutionX = dc.GetDeviceCaps( LOGPIXELSX );
  }
  else
    m_editor.Clear();
}

void CNetworkMapEditorView::OnSize(UINT nType, int cx, int cy) 
{
	CView::OnSize(nType, cx, cy);

	if( m_editor.m_hWnd )
		m_editor.MoveWindow(0,0,cx,cy);
	
}

void CNetworkMapEditorView::OnTimer(UINT_PTR nIDEvent)
{
  // TODO: Add your message handler code here and/or call default
  UpdateServerStatus();
  CView::OnTimer(nIDEvent);
}


void CNetworkMapEditorView::UpdateMidServerStatus()
{
  int socket_id = -1;
  std::string key;
  std::string title;

  //画监控服务点
  if(server_info!=NULL)
  {
    key = ModelIDFactory::GetServer2ClientSocket(conn_id_,socket_id);
    title =GetKey(server_info->ip(),server_info->quote_port());
    mid_symbol_ = m_editor.GetNamedObject(key.c_str());

    if(NULL==mid_symbol_)
    {
      mid_symbol_ = new CNetworkSymbol(IDB_BITMAP_ENABLE );
      mid_symbol_->SetTitle(title.c_str());
      mid_symbol_->SetName(key.c_str());
      m_editor.AddObject(mid_symbol_);
    }
    mid_symbol_->SetRect(x3_-bitmap_length_/2,y3_-bitmap_length_/2,x3_+bitmap_length_/2,y3_+bitmap_length_/2);
  }

  if(mid_symbol_!=NULL)
  {
    UpdateSymbolStatus(key.c_str(),SST_Server);
    mid_symbol_->SetRect(x3_-bitmap_length_/2,y3_-bitmap_length_/2,x3_+bitmap_length_/2,y3_+bitmap_length_/2);
  }
}


void CNetworkMapEditorView::UpdateSourceStatus()
{
  std::string key;
  std::string title;
  CNetworkSymbol*	server_symbol = NULL;
  TransferDataStat* transfer_status =NULL;
  int source_count = GetSourceCount(server_info);
  //画上游
  server_info->LockTransfersStatus();
  if (server_info->transfer_status_count() > 0)
  {
    int length = 64;
    if(source_count!=0)
    {
      length = (rect.Height()-((source_count-1)*10+80)) / source_count ;
    }    
    bitmap_length_ = length > 64 ? 64 :  length;
    bitmap_h_gap_ = 10;
    bitmap_w_gap_ = (rect.Width()-3*bitmap_length_) / 3;
  }

  for(int i=0, j=0;j<server_info->transfer_status_count();j++)
  {  
    transfer_status = server_info->transfer_status(j);
    if(transfer_status->source_port==0)//有可能有下游客户端 但没有专属的源（CFFE L1）或者源被禁用
    {
      continue;
    }

    key = ModelIDFactory::GetServer2SourcePort(conn_id_,(char*)transfer_status->source_ip,transfer_status->source_port);

    server_symbol = m_editor.GetNamedObject(key.c_str());
    if(NULL==server_symbol)
    {
      server_symbol = new CNetworkSymbol(IDB_BITMAP_ENABLE);
      server_symbol->SetTitle(key.c_str());
      server_symbol->SetName(key.c_str());
      m_editor.AddObject(server_symbol);
    }
    x2_ = x3_ - bitmap_length_-bitmap_w_gap_;
    x4_ = x3_ + bitmap_w_gap_;
    if (i % 2 == 0)
    {
      y2_ = y3_-bitmap_length_/2 - (i+1)/2*(bitmap_h_gap_+bitmap_length_);
    }
    else
    {
      y2_ = y3_-bitmap_length_/2 + (i+1)/2*(bitmap_h_gap_+bitmap_length_);
    }
    i++;
    UpdateSymbolStatus(key.c_str(),SST_Source);
    server_symbol->SetRect(x2_,y2_,x2_+bitmap_length_,y2_+bitmap_length_);

    RedrawLink(server_symbol, mid_symbol_, key, SST_Source);
  }
  server_info->UnlockTransfersStatus();
}


void CNetworkMapEditorView::UpdateClientStatus()
{
  std::string title;
  std::string key;
  CNetworkSymbol*	server_symbol = NULL;
  ClientStatus* client_status =NULL;
  server_info->LockClientsStatus();
  if (server_info->client_status_count() > 0)
  {
    int length = (rect.Height()-((GetClientCount(server_info)-1)*10)) / GetClientCount(server_info);
    bitmap_length_ = length > 64 ? 64 : length;
    bitmap_h_gap_ = 10;
    bitmap_w_gap_ = (rect.Width()-3*bitmap_length_) / 3;
  }
  for(int i=0, j=0;j<server_info->client_status_count();j++)
  {
    title = ModelIDFactory::GetClientTitle(conn_id_,(char*)server_info->client_status(j)->client_ip,server_info->client_status(j)->socket_id);
    key = ModelIDFactory::GetServer2ClientSocket(conn_id_,server_info->client_status(j)->socket_id);
    server_symbol = m_editor.GetNamedObject(key.c_str());
    if(NULL==server_symbol)
    {
      server_symbol = new CNetworkSymbol(IDB_BITMAP_ENABLE);
      server_symbol->SetTitle(title.c_str());
      server_symbol->SetName(key.c_str());
      m_editor.AddObject(server_symbol);
    }

    if (i % 2 == 0)
    {
      y4_ = y3_-bitmap_length_/2 - (i+1)/2*(bitmap_length_ + bitmap_h_gap_);
    }
    else
    {
      y4_ = y3_-bitmap_length_/2 + (i+1)/2*(bitmap_length_ + bitmap_h_gap_);
    }
    i++;
    UpdateSymbolStatus(key.c_str(),SST_Client);
    server_symbol->SetRect(x4_,y4_,x4_+bitmap_length_,y4_+bitmap_length_);
    RedrawLink(mid_symbol_, server_symbol, key, SST_Client);
  }
  server_info->UnlockClientsStatus();
}

void CNetworkMapEditorView::UpdateServerStatus()
{
  ((CNetworkEntityContainer*)m_editor.GetDiagramEntityContainer())->ClearLinks();
  ((CNetworkEntityContainer*)m_editor.GetDiagramEntityContainer())->RemoveAll();
  ResetSymbolSize();
  
  server_info = ServerManager::Instance()->net_info_collection()->find_server_by_cid(conn_id_);
  if(server_info!=NULL)
  {
    //画监控服务点
    UpdateMidServerStatus();

    //画上游
    UpdateSourceStatus();

    //画下游
    UpdateClientStatus();
  }
  m_editor.RedrawWindow();
}


void CNetworkMapEditorView::RedrawLink( CNetworkSymbol* server_symbol, CNetworkSymbol* server_symbol2, std::string key, StudioSymbolType type )
{
  SymbolModel* model = server_info->GetSymbolModel(key,type);
  CNetworkLink* link = ((CNetworkEntityContainer*)m_editor.GetDiagramEntityContainer())->FindLink(server_symbol,server_symbol2);
  if(link==NULL)
  {
    link = new CNetworkLink();
    link->from = server_symbol->GetName();
    link->to = server_symbol2->GetName();
    ((CNetworkEntityContainer*)m_editor.GetDiagramEntityContainer())->AddLink(link);
  }
  link->flow += model->flow;
  link->network_stat =model->symbol_status;
}

std::string CNetworkMapEditorView::GetKey(std::string ip,int32_t port)
{
  return StrUtils::formatstring(20,"%s:%d",ip.c_str(),port);
}

void CNetworkMapEditorView::StartTimer( int conn_id )
{
  conn_id_ = conn_id;
  UpdateServerStatus();
  SetTimer(1, 1000, NULL);
}

int CNetworkMapEditorView::GetSourceCount( ServerNetInfo* server_info )
{
  int count=0;
  int tmp_port =0;
  for( int j=0;j<server_info->transfer_status_count();j++)
  {  
    if(server_info->transfer_status(j)->source_port==0)//有可能有下游客户端 但没有专属的源（CFFE L1）或者源被禁用
    {
      continue;
    }
    else if(server_info->transfer_status(j)->source_port!=tmp_port)
    {
      count++;
      tmp_port = server_info->transfer_status(j)->source_port;
    }
  }
  return count;
}

int CNetworkMapEditorView::GetClientCount( ServerNetInfo* server_info )
{
  return server_info->client_status_count();
}

void CNetworkMapEditorView::UpdateSymbolStatus( std::string symbol_name,StudioSymbolType type)
{
  CNetworkSymbol* symbol = m_editor.GetNamedObject(symbol_name.c_str());
  SymbolModel* model = server_info->GetSymbolModel(symbol_name,type);
  if(symbol!=NULL)
  {
    symbol->set_server_status(model->symbol_status);
    if(model->symbol_status&SMP_Disconnect)
    {
      symbol->SetSymbol(IDB_BITMAP_DESABLE);
    }
    else
    {
      symbol->SetSymbol(IDB_BITMAP_ENABLE);
    }
  } 
}

CNetworkSymbol* CNetworkMapEditorView::GetSymbol( std::string symbol_name )
{
  return m_editor.GetNamedObject(symbol_name.c_str());
}

void CNetworkMapEditorView::ResetSymbolSize()
{
  GetClientRect( rect );
  x3_ = rect.Width()/2;
  y3_ = rect.Height()/2;
  bitmap_length_ =  64;
  bitmap_h_gap_ = 100;
  bitmap_w_gap_ = 100;
}

void CNetworkMapEditorView::EndTimer()
{
  KillTimer(1);
}



// D:\Projects\高频\WMDF\Src\ver10_2\src\studio\monitor\MonitorQuote.cpp : implementation file
//

#include "../main_frame/StdAfx.h"
#include "../main_frame/resource.h"
#include "MonitorQuote.h"
#include "../../environment/strutils.h"
#include "../../include/date_time.h"
#include "../main_frame/MainFrm.h"
#include "../common/studio_utility.h"
#include <WinUser.h>
// CMonitorQuote

IMPLEMENT_DYNCREATE(CMonitorQuote, CFormView)

CMonitorQuote::CMonitorQuote()
	: CFormView(CMonitorQuote::IDD)
  , m_trade_time(-99999)
  , m_new_price(-99999)
  , m_windcode(_T("600000.SH"))
  , m_trade_date(-99999)
  , m_pre_close(-99999)
  , m_high(-99999)
  , m_open(-99999)
  , m_low(-99999)
  , m_deal_count(-99999)
  , m_bid_price1(-99999)
  , m_bid_price2(-99999)
  , m_bid_price3(-99999)
  , m_bid_price4(-99999)
  , m_bid_price5(-99999)
  , m_bid_price6(-99999)
  , m_bid_price7(-99999)
  , m_bid_price8(-99999)
  , m_bid_price9(-99999)
  , m_bid_price10(-99999)
  , m_ask_price1(-99999)
  , m_ask_price2(-99999)
  , m_ask_price3(-99999)
  , m_ask_price4(-99999)
  , m_ask_price5(-99999)
  , m_ask_price6(-99999)
  , m_ask_price7(-99999)
  , m_ask_price8(-99999)
  , m_ask_price9(-99999)
  , m_ask_price10(-99999)
  , m_avg_bid_price(-99999)
  , m_peratio1(-99999)
  , m_peratio2(-99999)
  , m_total_bid_size(-99999)
  , m_total_ask_size(-99999)
  , m_avg_ask_price(-99999)
  , m_bid_size1(-99999)
  , m_bid_size2(-99999)
  , m_bid_size3(-99999)
  , m_bid_size4(-99999)
  , m_bid_size5(-99999)
  , m_bid_size6(-99999)
  , m_bid_size7(-99999)
  , m_bid_size8(-99999)
  , m_bid_size9(-99999)
  , m_bid_size10(-99999)
  , m_ask_size1(-99999)
  , m_ask_size2(-99999)
  , m_ask_size3(-99999)
  , m_ask_size4(-99999)
  , m_ask_size5(-99999)
  , m_ask_size6(-99999)
  , m_ask_size7(-99999)
  , m_ask_size8(-99999)
  , m_ask_size9(-99999)
  , m_ask_size10(-99999)
  , m_trade_time2(-99999)
  , m_trade_price2(-99999)
  , m_trade_volume(-99999)
  , m_trade_amount(-99999)
  , m_set_no(-99999)
  , m_rec_no(-99999)
  , m_buy_order_no(-99999)
  , m_sell_order_no(-99999)
  , m_order_kind(-99999)
  , m_function_code(-99999)
  , m_total_amount(-99999)
  , m_total_volume(-99999)
  , m_pre_settle(-99999)
  , m_settle(-99999)
  , m_pre_position(-99999)
  , m_position(-99999)
  , m_high_limit(-99999)
  , m_low_limit(-99999)
  , m_avgprice(-99999)
  , m_trade_status(-99999)
{
  invoker_ = NULL;
  
  //m_colorEditText = RGB(255,255,255);
  m_colorEditBK = RGB(255,255,255);
  m_pEditBkBrush = new CBrush(RGB(255,255,255));
}

CMonitorQuote::~CMonitorQuote()
{
  if(NULL != invoker_)
  {
    invoker_->Disconnect();
    invoker_ = NULL;
  }
}

void CMonitorQuote::DoDataExchange(CDataExchange* pDX)
{
  CFormView::DoDataExchange(pDX);
  DDX_Control(pDX, IDC_SERVERID, m_server_list);
  DDX_Text(pDX, IDC_TRADETIME, m_trade_time);
  DDX_Text(pDX, IDC_NEW, m_new_price);
  DDX_Text(pDX, IDC_WINDCODE, m_windcode);
  DDX_Text(pDX, IDC_TRADEDATE, m_trade_date);
  DDX_Text(pDX, IDC_PRECLOSE, m_pre_close);
  DDX_Text(pDX, IDC_HIGH, m_high);
  DDX_Text(pDX, IDC_OPEN, m_open);
  DDX_Text(pDX, IDC_LOW, m_low);
  DDX_Text(pDX, IDC_DEALCOUNT, m_deal_count);

  DDX_Text(pDX, IDC_AVGBIDPRICE, m_avg_bid_price);
  DDX_Text(pDX, IDC_PERATIO1, m_peratio1);
  DDX_Text(pDX, IDC_PERATIO2, m_peratio2);
  DDX_Text(pDX, IDC_TOTALBIDVOLUME, m_total_bid_size);
  DDX_Text(pDX, IDC_TOTALASKVOLUME, m_total_ask_size);
  DDX_Text(pDX, IDC_AVGASKPRICE, m_avg_ask_price);

  DDX_Text(pDX, IDC_BIDPRICE1, m_bid_price1);
  DDX_Text(pDX, IDC_BIDPRICE2, m_bid_price2);
  DDX_Text(pDX, IDC_BIDPRICE3, m_bid_price3);
  DDX_Text(pDX, IDC_BIDPRICE4, m_bid_price4);
  DDX_Text(pDX, IDC_BIDPRICE5, m_bid_price5);
  DDX_Text(pDX, IDC_BIDPRICE6, m_bid_price6);
  DDX_Text(pDX, IDC_BIDPRICE7, m_bid_price7);
  DDX_Text(pDX, IDC_BIDPRICE8, m_bid_price8);
  DDX_Text(pDX, IDC_BIDPRICE9, m_bid_price9);
  DDX_Text(pDX, IDC_BIDPRICE10, m_bid_price10);
  DDX_Text(pDX, IDC_BIDVOLUME1, m_bid_size1);
  DDX_Text(pDX, IDC_BIDVOLUME2, m_bid_size2);
  DDX_Text(pDX, IDC_BIDVOLUME3, m_bid_size3);
  DDX_Text(pDX, IDC_BIDVOLUME4, m_bid_size4);
  DDX_Text(pDX, IDC_BIDVOLUME5, m_bid_size5);
  DDX_Text(pDX, IDC_BIDVOLUME6, m_bid_size6);
  DDX_Text(pDX, IDC_BIDVOLUME7, m_bid_size7);
  DDX_Text(pDX, IDC_BIDVOLUME8, m_bid_size8);
  DDX_Text(pDX, IDC_BIDVOLUME9, m_bid_size9);
  DDX_Text(pDX, IDC_BIDVOLUME10, m_bid_size10);  
  DDX_Text(pDX, IDC_ASKPRICE1, m_ask_price1);
  DDX_Text(pDX, IDC_ASKPRICE2, m_ask_price2);
  DDX_Text(pDX, IDC_ASKPRICE3, m_ask_price3);
  DDX_Text(pDX, IDC_ASKPRICE4, m_ask_price4);
  DDX_Text(pDX, IDC_ASKPRICE5, m_ask_price5);
  DDX_Text(pDX, IDC_ASKPRICE6, m_ask_price6);
  DDX_Text(pDX, IDC_ASKPRICE7, m_ask_price7);
  DDX_Text(pDX, IDC_ASKPRICE8, m_ask_price8);
  DDX_Text(pDX, IDC_ASKPRICE9, m_ask_price9);
  DDX_Text(pDX, IDC_ASKPRICE10, m_ask_price10);
  DDX_Text(pDX, IDC_ASKVOLUME1, m_ask_size1);
  DDX_Text(pDX, IDC_ASKVOLUME2, m_ask_size2);
  DDX_Text(pDX, IDC_ASKVOLUME3, m_ask_size3);
  DDX_Text(pDX, IDC_ASKVOLUME4, m_ask_size4);
  DDX_Text(pDX, IDC_ASKVOLUME5, m_ask_size5);
  DDX_Text(pDX, IDC_ASKVOLUME6, m_ask_size6);
  DDX_Text(pDX, IDC_ASKVOLUME7, m_ask_size7);
  DDX_Text(pDX, IDC_ASKVOLUME8, m_ask_size8);
  DDX_Text(pDX, IDC_ASKVOLUME9, m_ask_size9);
  DDX_Text(pDX, IDC_ASKVOLUME10, m_ask_size10);
  DDX_Text(pDX, IDC_TRADETIME2, m_trade_time2);
  DDX_Text(pDX, IDC_TRADEPRICE, m_trade_price2);
  DDX_Text(pDX, IDC_TRADEVOLUME, m_trade_volume);
  DDX_Text(pDX, IDC_TRADEAMOUNT, m_trade_amount);
  DDX_Text(pDX, IDC_SETNO, m_set_no);
  DDX_Text(pDX, IDC_RECNO, m_rec_no);
  DDX_Text(pDX, IDC_BUYORDERNO, m_buy_order_no);
  DDX_Text(pDX, IDC_SELLORDERNO, m_sell_order_no);
  DDX_Text(pDX, IDC_ORDERKIND, m_order_kind);
  DDX_Text(pDX, IDC_FUNCTIONCODE, m_function_code);
  DDX_Text(pDX, IDC_TOTALAMOUNT, m_total_amount);
  DDX_Text(pDX, IDC_TOTALVOLUME, m_total_volume);
  DDX_Text(pDX, IDC_PRE_SETTLE, m_pre_settle);
  DDX_Text(pDX, IDC_SETTLE, m_settle);
  DDX_Text(pDX, IDC_PRE_POSITION, m_pre_position);
  DDX_Text(pDX, IDC_POSITION, m_position);
  DDX_Text(pDX, IDC_HIGHLIMIT, m_high_limit);
  DDX_Text(pDX, IDC_LOW_LIMIT, m_low_limit);
  DDX_Text(pDX, IDC_AVGPRICE, m_avgprice);
  DDX_Text(pDX, IDC_TRADE_STATUS, m_trade_status);
}

BEGIN_MESSAGE_MAP(CMonitorQuote, CFormView)
  ON_BN_CLICKED(IDC_SETWINDCODE, &CMonitorQuote::OnBnClickedSetwindcode)
  ON_BN_CLICKED(IDC_START, &CMonitorQuote::OnBnClickedStart)
  ON_BN_CLICKED(IDC_STOP, &CMonitorQuote::OnBnClickedStop)
  ON_MESSAGE(WM_L1_MKT_RECORD, &CMonitorQuote::OnReceiveL1MktRecord)
  ON_MESSAGE(WM_L2_MKT_RECORD, &CMonitorQuote::OnReceiveL2MktRecord)
  ON_MESSAGE(WM_L2_TRANS_RECORD, &CMonitorQuote::OnReceiveL2TransRecord)
  ON_MESSAGE(WM_L1_Future_MKT_RECORD, &CMonitorQuote::OnReceiveFutureMktRecord)
  ON_MESSAGE(WM_L2_CFFEL2_MKT_RECORD, &CMonitorQuote::OnReceiveCFFEL2MktRecord)
  ON_WM_DESTROY()
  ON_WM_CTLCOLOR()
  ON_WM_SIZE()
END_MESSAGE_MAP()

//BEGIN_EASYSIZE_MAP(CMonitorQuote)
//  EASYSIZE(IDC_OPERATION_FRAME,ES_BORDER,ES_BORDER,ES_BORDER,ES_KEEPSIZE,0)  
//  EASYSIZE(IDC_TRANSACTION_FRAME,ES_BORDER,IDC_OPERATION_FRAME,ES_BORDER,ES_KEEPSIZE,0)
//  EASYSIZE(IDC_MARKETDATA_FRAME,ES_BORDER,IDC_TRANSACTION_FRAME,ES_BORDER,ES_KEEPSIZE,0)
//END_EASYSIZE_MAP

// CMonitorQuote diagnostics

#ifdef _DEBUG
void CMonitorQuote::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CMonitorQuote::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CMonitorQuote message handlers

void CMonitorQuote::OnBnClickedSetwindcode()
{
  //TODO: Add your control notification handler code here
  if(NULL!=invoker_)
  {  
    UpdateData(TRUE);
    ResetAll();
    invoker_->SetFilter(m_windcode.GetBuffer(0));
  }
}

void CMonitorQuote::OnBnClickedStart()
{
  if(NULL!=invoker_)
    return;

  UpdateData(true);
  HWND handle = this->GetSafeHwnd();
  int index = m_server_list.GetCurSel();
  if(index<0)
  {
    MessageBox("请选择服务ID！");
    return;
  }
  if(strcmp(m_windcode.GetBuffer(0),"")==0)
  {
    MessageBox("请重新设置WindCode！");
    return;
  }
  
  ServerNetInfo* server_info = ServerManager::Instance()->net_info_collection()->server_infos(index);
  invoker_ = new QuoteInvoke(server_info->server_id(),handle);  
  invoker_->SetFilter(m_windcode.GetBuffer(0));
  invoker_->Connect();
  GetDlgItem(IDC_START)->EnableWindow(FALSE);
  GetDlgItem(IDC_STOP)->EnableWindow();
}

void CMonitorQuote::OnBnClickedStop()
{
  // TODO: Add your control notification handler code here
  if(NULL != invoker_)
  {
    invoker_->Disconnect();
    invoker_ = NULL;
  }
  ResetAll();
  GetDlgItem(IDC_STOP)->EnableWindow(FALSE);
  GetDlgItem(IDC_START)->EnableWindow();
}

void CMonitorQuote::ResetAll()
{  
  m_trade_time = -99999 ;
  m_new_price = -99999 ;
  m_trade_date = -99999 ;
  m_pre_close = -99999 ;
  m_high = -99999 ;
  m_open = -99999 ;
  m_low = -99999 ;
  m_deal_count = -99999 ;
  m_bid_price1 = -99999 ;
  m_bid_price2 = -99999 ;
  m_bid_price3 = -99999 ;
  m_bid_price4 = -99999 ;
  m_bid_price5 = -99999 ;
  m_bid_price6 = -99999 ;
  m_bid_price7 = -99999 ;
  m_bid_price8 = -99999 ;
  m_bid_price9 = -99999 ;
  m_bid_price10 = -99999 ;
  m_ask_price1 = -99999 ;
  m_ask_price2 = -99999 ;
  m_ask_price3 = -99999 ;
  m_ask_price4 = -99999 ;
  m_ask_price5 = -99999 ;
  m_ask_price6 = -99999 ;
  m_ask_price7 = -99999 ;
  m_ask_price8 = -99999 ;
  m_ask_price9 = -99999 ;
  m_ask_price10 = -99999 ;
  m_avg_bid_price = -99999 ;
  m_peratio1 = -99999 ;
  m_peratio2 = -99999 ;
  m_total_bid_size = -99999 ;
  m_total_ask_size = -99999 ;
  m_avg_ask_price = -99999 ;
  m_bid_size1 = -99999 ;
  m_bid_size2 = -99999 ;
  m_bid_size3 = -99999 ;
  m_bid_size4 = -99999 ;
  m_bid_size5 = -99999 ;
  m_bid_size6 = -99999 ;
  m_bid_size7 = -99999 ;
  m_bid_size8 = -99999 ;
  m_bid_size9 = -99999 ;
  m_bid_size10 = -99999 ;
  m_ask_size1 = -99999 ;
  m_ask_size2 = -99999 ;
  m_ask_size3 = -99999 ;
  m_ask_size4 = -99999 ;
  m_ask_size5 = -99999 ;
  m_ask_size6 = -99999 ;
  m_ask_size7 = -99999 ;
  m_ask_size8 = -99999 ;
  m_ask_size9 = -99999 ;
  m_ask_size10 = -99999 ;
  m_trade_time2 = -99999 ;
  m_trade_price2 = -99999 ;
  m_trade_volume = -99999 ;
  m_trade_amount = -99999 ;
  m_set_no = -99999 ;
  m_rec_no = -99999 ;
  m_buy_order_no = -99999 ;
  m_sell_order_no = -99999 ;
  m_order_kind = -99999 ;
  m_function_code = -99999 ;
  m_total_amount = -99999 ;
  m_total_volume = -99999 ;
  m_pre_settle = -99999 ;
  m_settle = -99999 ;
  m_pre_position = -99999 ;
  m_position = -99999 ;
  m_high_limit = -99999 ;
  m_low_limit = -99999 ;
  m_avgprice = -99999 ;
  UpdateData(FALSE);
}


void CMonitorQuote::OnInitialUpdate()
{
  CFormView::OnInitialUpdate();
  GetParentFrame()->SetWindowText("行情监控");
  // TODO: Add your specialized code here and/or call the base class
  ServerNetInfo* server_info = NULL;
  for(int i =0;i<ServerManager::Instance()->net_info_collection()->server_info_count();i++)
  {
    server_info = ServerManager::Instance()->net_info_collection()->server_infos(i);
    m_server_list.InsertString(i,StrUtils::formatstring(30,"%d-%s:%d",server_info->server_id(),server_info->ip().c_str(),server_info->quote_port()).c_str());
  }
  m_server_list.SetCurSel(0);

  GetDlgItem(IDC_START)->EnableWindow();
  GetDlgItem(IDC_STOP)->EnableWindow(FALSE);
  //string s = "600000.SH";
  //m_windcode=s.c_str();
  ////this->GetDlgItem(IDC_WINDCODE)->SetWindowText(s.c_str());
  //UpdateData(FALSE);
  //INIT_EASYSIZE;
}

void CMonitorQuote::OnDestroy()
{
  CFormView::OnDestroy();
  // TODO: Add your message handler code here
  ((CMainFrame*)AfxGetMainWnd())->m_pMonitorQuote=NULL; // 清空窗口指针  
  m_pEditBkBrush->DeleteObject();
}

HBRUSH CMonitorQuote::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
  //HBRUSH hbr = CFormView::OnCtlColor(pDC, pWnd, nCtlColor);

  // TODO:  Change any attributes of the DC here
  if(nCtlColor== CTLCOLOR_EDIT)
  {
    //pDC->SetBkMode(TRANSPARENT);
    //pDC->SetTextColor(m_colorEditText);
    pDC->SetBkColor(m_colorEditBK);
    return (HBRUSH)(m_pEditBkBrush->GetSafeHandle());
  }
  else
    return CFormView::OnCtlColor(pDC, pWnd, nCtlColor);
}

LRESULT CMonitorQuote::OnReceiveL1MktRecord( WPARAM wParam, LPARAM lParam )
{
  L1MarketSnapshot* item = (L1MarketSnapshot* )wParam;  
  RedrawSingleL1MarketDataEdit(IDC_TRADETIME,item);
  RedrawSingleL1MarketDataEdit( IDC_PRECLOSE,item ); 
  RedrawSingleL1MarketDataEdit( IDC_OPEN,item );
  RedrawSingleL1MarketDataEdit( IDC_HIGH,item );
  RedrawSingleL1MarketDataEdit( IDC_LOW,item );   
  RedrawSingleL1MarketDataEdit( IDC_NEW,item );  
  RedrawSingleL1MarketDataEdit(IDC_TOTALVOLUME,item);
  RedrawSingleL1MarketDataEdit( IDC_TOTALAMOUNT,item );
  
  RedrawSingleL1MarketDataEdit(IDC_ASKVOLUME1,item);
  RedrawSingleL1MarketDataEdit(IDC_ASKVOLUME2,item);
  RedrawSingleL1MarketDataEdit(IDC_ASKVOLUME3,item);
  RedrawSingleL1MarketDataEdit(IDC_ASKVOLUME4,item);
  RedrawSingleL1MarketDataEdit(IDC_ASKVOLUME5,item);      
  RedrawSingleL1MarketDataEdit( IDC_ASKPRICE1,item );
  RedrawSingleL1MarketDataEdit( IDC_ASKPRICE2,item );
  RedrawSingleL1MarketDataEdit( IDC_ASKPRICE3,item );
  RedrawSingleL1MarketDataEdit( IDC_ASKPRICE4,item );
  RedrawSingleL1MarketDataEdit( IDC_ASKPRICE5,item );
  RedrawSingleL1MarketDataEdit(IDC_BIDVOLUME1,item);
  RedrawSingleL1MarketDataEdit(IDC_BIDVOLUME2,item);
  RedrawSingleL1MarketDataEdit(IDC_BIDVOLUME3,item);
  RedrawSingleL1MarketDataEdit(IDC_BIDVOLUME4,item);
  RedrawSingleL1MarketDataEdit(IDC_BIDVOLUME5,item);
  RedrawSingleL1MarketDataEdit( IDC_BIDPRICE1,item );
  RedrawSingleL1MarketDataEdit( IDC_BIDPRICE2,item );
  RedrawSingleL1MarketDataEdit( IDC_BIDPRICE3,item );
  RedrawSingleL1MarketDataEdit( IDC_BIDPRICE4,item );
  RedrawSingleL1MarketDataEdit( IDC_BIDPRICE5,item );
  delete item;
  return TRUE;
}


LRESULT CMonitorQuote::OnReceiveL2MktRecord( WPARAM wParam, LPARAM lParam )
{
  L2MarketSnapshot* item = (L2MarketSnapshot* )wParam;  
  RedrawSingleL2MarketDataEdit(IDC_TRADETIME,item);
  RedrawSingleL2MarketDataEdit( IDC_PRECLOSE,item );  
  RedrawSingleL2MarketDataEdit( IDC_OPEN,item );
  RedrawSingleL2MarketDataEdit( IDC_HIGH,item );
  RedrawSingleL2MarketDataEdit( IDC_LOW,item );   
  RedrawSingleL2MarketDataEdit( IDC_NEW,item );

  RedrawSingleL2MarketDataEdit(IDC_TOTALVOLUME,item);
  RedrawSingleL2MarketDataEdit( IDC_TOTALAMOUNT,item );
  RedrawSingleL2MarketDataEdit(IDC_DEALCOUNT,item);
  RedrawSingleL2MarketDataEdit(IDC_TOTALASKVOLUME,item);
  RedrawSingleL2MarketDataEdit(IDC_TOTALBIDVOLUME,item);
  

  RedrawSingleL2MarketDataEdit(IDC_BIDVOLUME1,item);
  RedrawSingleL2MarketDataEdit(IDC_BIDVOLUME2,item);
  RedrawSingleL2MarketDataEdit(IDC_BIDVOLUME3,item);
  RedrawSingleL2MarketDataEdit(IDC_BIDVOLUME4,item);
  RedrawSingleL2MarketDataEdit(IDC_BIDVOLUME5,item);
  RedrawSingleL2MarketDataEdit(IDC_BIDVOLUME6,item);
  RedrawSingleL2MarketDataEdit(IDC_BIDVOLUME7,item);
  RedrawSingleL2MarketDataEdit(IDC_BIDVOLUME8,item);
  RedrawSingleL2MarketDataEdit(IDC_BIDVOLUME9,item);
  RedrawSingleL2MarketDataEdit(IDC_BIDVOLUME10,item);
  RedrawSingleL2MarketDataEdit(IDC_BIDPRICE1,item );
  RedrawSingleL2MarketDataEdit(IDC_BIDPRICE2,item );
  RedrawSingleL2MarketDataEdit(IDC_BIDPRICE3,item );
  RedrawSingleL2MarketDataEdit(IDC_BIDPRICE4,item );
  RedrawSingleL2MarketDataEdit(IDC_BIDPRICE5,item );
  RedrawSingleL2MarketDataEdit(IDC_BIDPRICE6,item );
  RedrawSingleL2MarketDataEdit(IDC_BIDPRICE7,item );
  RedrawSingleL2MarketDataEdit(IDC_BIDPRICE8,item );
  RedrawSingleL2MarketDataEdit(IDC_BIDPRICE9,item );
  RedrawSingleL2MarketDataEdit(IDC_BIDPRICE10,item );

  RedrawSingleL2MarketDataEdit(IDC_ASKVOLUME1,item);
  RedrawSingleL2MarketDataEdit(IDC_ASKVOLUME2,item);
  RedrawSingleL2MarketDataEdit(IDC_ASKVOLUME3,item);
  RedrawSingleL2MarketDataEdit(IDC_ASKVOLUME4,item);
  RedrawSingleL2MarketDataEdit(IDC_ASKVOLUME5,item);
  RedrawSingleL2MarketDataEdit(IDC_ASKVOLUME6,item);
  RedrawSingleL2MarketDataEdit(IDC_ASKVOLUME7,item);
  RedrawSingleL2MarketDataEdit(IDC_ASKVOLUME8,item);
  RedrawSingleL2MarketDataEdit(IDC_ASKVOLUME9,item);
  RedrawSingleL2MarketDataEdit(IDC_ASKVOLUME10,item);
  RedrawSingleL2MarketDataEdit( IDC_ASKPRICE1,item );
  RedrawSingleL2MarketDataEdit( IDC_ASKPRICE2,item );
  RedrawSingleL2MarketDataEdit( IDC_ASKPRICE3,item );
  RedrawSingleL2MarketDataEdit( IDC_ASKPRICE4,item );
  RedrawSingleL2MarketDataEdit( IDC_ASKPRICE5,item );
  RedrawSingleL2MarketDataEdit( IDC_ASKPRICE6,item );
  RedrawSingleL2MarketDataEdit( IDC_ASKPRICE7,item );
  RedrawSingleL2MarketDataEdit( IDC_ASKPRICE8,item );
  RedrawSingleL2MarketDataEdit( IDC_ASKPRICE9,item );
  RedrawSingleL2MarketDataEdit( IDC_ASKPRICE10,item );

  RedrawSingleL2MarketDataEdit( IDC_AVGASKPRICE,item );    
  RedrawSingleL2MarketDataEdit( IDC_AVGBIDPRICE,item );    
  RedrawSingleL2MarketDataEdit( IDC_PERATIO1,item );
  RedrawSingleL2MarketDataEdit( IDC_PERATIO2,item );
  RedrawSingleL2MarketDataEdit( IDC_TRADE_STATUS,item );
  delete item;
  return TRUE;
}

LRESULT CMonitorQuote::OnReceiveL2TransRecord( WPARAM wParam, LPARAM lParam )
{
  L2Transaction* item = (L2Transaction*)wParam;
  RedrawSingleL2TransactionEdit(IDC_TRADETIME2,item);
  RedrawSingleL2TransactionEdit(IDC_TRADEPRICE,item);
  RedrawSingleL2TransactionEdit(IDC_TRADEVOLUME,item);
  RedrawSingleL2TransactionEdit(IDC_TRADEAMOUNT,item);
  RedrawSingleL2TransactionEdit(IDC_SETNO,item);
  RedrawSingleL2TransactionEdit(IDC_RECNO,item);
  RedrawSingleL2TransactionEdit(IDC_BUYORDERNO,item);
  RedrawSingleL2TransactionEdit(IDC_SELLORDERNO,item);
  RedrawSingleL2TransactionEdit(IDC_ORDERKIND,item);
  RedrawSingleL2TransactionEdit(IDC_FUNCTIONCODE,item);
  delete item;
  return TRUE;
}

LRESULT CMonitorQuote::OnReceiveFutureMktRecord( WPARAM wParam, LPARAM lParam )
{
  FuturesMarketSnapshot* item = (FuturesMarketSnapshot* )wParam;  
  RedrawSingleFutureMarketDataEdit(IDC_TRADEDATE,item);
  RedrawSingleFutureMarketDataEdit(IDC_TRADETIME,item);  
  RedrawSingleFutureMarketDataEdit( IDC_OPEN,item );
  RedrawSingleFutureMarketDataEdit( IDC_HIGH,item );
  RedrawSingleFutureMarketDataEdit( IDC_LOW,item );   
  RedrawSingleFutureMarketDataEdit( IDC_NEW,item );   
  RedrawSingleFutureMarketDataEdit(IDC_TOTALVOLUME,item);
  RedrawSingleFutureMarketDataEdit( IDC_TOTALAMOUNT,item );
  RedrawSingleFutureMarketDataEdit( IDC_PRECLOSE,item );   
  RedrawSingleFutureMarketDataEdit( IDC_ASKPRICE1,item );
  RedrawSingleFutureMarketDataEdit( IDC_BIDPRICE1,item );
  RedrawSingleFutureMarketDataEdit(IDC_BIDVOLUME1,item);
  RedrawSingleFutureMarketDataEdit(IDC_ASKVOLUME1,item);
  delete item;
  return TRUE;
}



LRESULT CMonitorQuote::OnReceiveCFFEL2MktRecord( WPARAM wParam, LPARAM lParam )
{
  CFEL2MarketSnapshot* item = (CFEL2MarketSnapshot* )wParam;  
  RedrawSingleCFFEL2MarketDataEdit(IDC_TRADEDATE,item);
  RedrawSingleCFFEL2MarketDataEdit(IDC_TRADETIME,item);
  RedrawSingleCFFEL2MarketDataEdit(IDC_PRECLOSE,item ); 
  RedrawSingleCFFEL2MarketDataEdit(IDC_OPEN,item );
  RedrawSingleCFFEL2MarketDataEdit(IDC_HIGH,item );
  RedrawSingleCFFEL2MarketDataEdit(IDC_LOW,item );   
  RedrawSingleCFFEL2MarketDataEdit(IDC_NEW,item );  
  RedrawSingleCFFEL2MarketDataEdit(IDC_TOTALVOLUME,item);
  RedrawSingleCFFEL2MarketDataEdit(IDC_TOTALAMOUNT,item );
  RedrawSingleCFFEL2MarketDataEdit(IDC_ASKVOLUME1,item);
  RedrawSingleCFFEL2MarketDataEdit(IDC_ASKVOLUME2,item);
  RedrawSingleCFFEL2MarketDataEdit(IDC_ASKVOLUME3,item);
  RedrawSingleCFFEL2MarketDataEdit(IDC_ASKVOLUME4,item);
  RedrawSingleCFFEL2MarketDataEdit(IDC_ASKVOLUME5,item);      
  RedrawSingleCFFEL2MarketDataEdit(IDC_ASKPRICE1,item );
  RedrawSingleCFFEL2MarketDataEdit(IDC_ASKPRICE2,item );
  RedrawSingleCFFEL2MarketDataEdit(IDC_ASKPRICE3,item );
  RedrawSingleCFFEL2MarketDataEdit(IDC_ASKPRICE4,item );
  RedrawSingleCFFEL2MarketDataEdit(IDC_ASKPRICE5,item );
  RedrawSingleCFFEL2MarketDataEdit(IDC_BIDVOLUME1,item);
  RedrawSingleCFFEL2MarketDataEdit(IDC_BIDVOLUME2,item);
  RedrawSingleCFFEL2MarketDataEdit(IDC_BIDVOLUME3,item);
  RedrawSingleCFFEL2MarketDataEdit(IDC_BIDVOLUME4,item);
  RedrawSingleCFFEL2MarketDataEdit(IDC_BIDVOLUME5,item);
  RedrawSingleCFFEL2MarketDataEdit(IDC_BIDPRICE1,item );
  RedrawSingleCFFEL2MarketDataEdit(IDC_BIDPRICE2,item );
  RedrawSingleCFFEL2MarketDataEdit(IDC_BIDPRICE3,item );
  RedrawSingleCFFEL2MarketDataEdit(IDC_BIDPRICE4,item );
  RedrawSingleCFFEL2MarketDataEdit(IDC_BIDPRICE5,item );
  delete item;
  return TRUE;
}
void CMonitorQuote::RedrawSingleL1MarketDataEdit( int nID, L1MarketSnapshot* item )
{
  bool equal_flag;
  string text("");
  switch(nID)
  {
  case IDC_TRADETIME:
    equal_flag = StudioUtility::CompareAndReset(wmdf::DateTime::time2hms(item->data_time_stamp) , m_trade_time);
    text = StrUtils::formatstring(15,"%d",m_trade_time);
    break;
  case IDC_BIDVOLUME1:
    equal_flag = StudioUtility::CompareAndReset(item->bid_size1 , m_bid_size1);
    text = StrUtils::formatstring(15,"%d",m_bid_size1);
    break;
  case IDC_BIDVOLUME2:
    equal_flag = StudioUtility::CompareAndReset(item->bid_size2 , m_bid_size2);
    text = StrUtils::formatstring(15,"%d",m_bid_size2);
    break;
  case IDC_BIDVOLUME3:
    equal_flag = StudioUtility::CompareAndReset(item->bid_size3 , m_bid_size3);
    text = StrUtils::formatstring(15,"%d",m_bid_size3);
    break;
  case IDC_BIDVOLUME4:
    equal_flag = StudioUtility::CompareAndReset(item->bid_size4 , m_bid_size4);
    text = StrUtils::formatstring(15,"%d",m_bid_size4);
    break;
  case IDC_BIDVOLUME5:
    equal_flag = StudioUtility::CompareAndReset(item->bid_size5 , m_bid_size5);
    text = StrUtils::formatstring(15,"%d",m_bid_size5);
    break;
  case IDC_ASKVOLUME1:
    equal_flag = StudioUtility::CompareAndReset(item->ask_size1 , m_ask_size1);
    text = StrUtils::formatstring(15,"%d",m_ask_size1);
    break;
  case IDC_ASKVOLUME2:
    equal_flag = StudioUtility::CompareAndReset(item->ask_size2 , m_ask_size2);
    text = StrUtils::formatstring(15,"%d",m_ask_size2);
    break;
  case IDC_ASKVOLUME3:
    equal_flag = StudioUtility::CompareAndReset(item->ask_size3 , m_ask_size3);
    text = StrUtils::formatstring(15,"%d",m_ask_size3);
    break;
  case IDC_ASKVOLUME4:
    equal_flag = StudioUtility::CompareAndReset(item->ask_size4 , m_ask_size4);
    text = StrUtils::formatstring(15,"%d",m_ask_size4);
    break;
  case IDC_ASKVOLUME5:
    equal_flag = StudioUtility::CompareAndReset(item->ask_size5 , m_ask_size5);
    text = StrUtils::formatstring(15,"%d",m_ask_size5);
    break;
  case IDC_TOTALVOLUME:
    equal_flag = StudioUtility::CompareAndReset(item->total_volume , m_total_volume);
    text = StrUtils::formatstring(15,"%I64d",m_total_volume);
    break;
  case IDC_TOTALAMOUNT:
    equal_flag = StudioUtility::CompareAndReset(item->total_amount , m_total_amount);
    text = StrUtils::formatstring(20,"%.2f",m_total_amount);
    break;
  case IDC_OPEN:
    equal_flag = StudioUtility::CompareAndReset(item->open_price , m_open);
    text = StrUtils::formatstring(15,"%.2f",m_open);
    break;
  case IDC_HIGH:
    equal_flag = StudioUtility::CompareAndReset(item->high_price , m_high);
    text = StrUtils::formatstring(15,"%.2f",m_high);
    break;
  case IDC_LOW:
    equal_flag = StudioUtility::CompareAndReset(item->low_price , m_low);
    text = StrUtils::formatstring(15,"%.2f",m_low);
    break;
  case IDC_NEW:
    equal_flag = StudioUtility::CompareAndReset(item->new_price , m_new_price);
    text = StrUtils::formatstring(15,"%.2f",m_new_price);
    break;
  case IDC_PRECLOSE:
    equal_flag = StudioUtility::CompareAndReset(item->pre_close , m_pre_close);
    text = StrUtils::formatstring(15,"%.2f",m_pre_close);
    break;
  case IDC_ASKPRICE1:
    equal_flag = StudioUtility::CompareAndReset(item->ask_price1 , m_ask_price1);
    text = StrUtils::formatstring(15,"%.2f",m_ask_price1);
    break;
  case IDC_ASKPRICE2:
    equal_flag = StudioUtility::CompareAndReset(item->ask_price2 , m_ask_price2);
    text = StrUtils::formatstring(15,"%.2f",m_ask_price2);
    break;
  case IDC_ASKPRICE3:
    equal_flag = StudioUtility::CompareAndReset(item->ask_price3 , m_ask_price3);
    text = StrUtils::formatstring(15,"%.2f",m_ask_price3);
    break;
  case IDC_ASKPRICE4:
    equal_flag = StudioUtility::CompareAndReset(item->ask_price4 , m_ask_price4);
    text = StrUtils::formatstring(15,"%.2f",m_ask_price4);
    break;
  case IDC_ASKPRICE5:
    equal_flag = StudioUtility::CompareAndReset(item->ask_price5 , m_ask_price5);
    text = StrUtils::formatstring(15,"%.2f",m_ask_price5);
    break;
  case IDC_BIDPRICE1:
    equal_flag = StudioUtility::CompareAndReset(item->bid_price1 , m_bid_price1);
    text = StrUtils::formatstring(15,"%.2f",m_bid_price1);
    break;
  case IDC_BIDPRICE2:
    equal_flag = StudioUtility::CompareAndReset(item->bid_price2 , m_bid_price2);
    text = StrUtils::formatstring(15,"%.2f",m_bid_price2);
    break;
  case IDC_BIDPRICE3:
    equal_flag = StudioUtility::CompareAndReset(item->bid_price3 , m_bid_price3);
    text = StrUtils::formatstring(15,"%.2f",m_bid_price3);
    break;
  case IDC_BIDPRICE4:
    equal_flag = StudioUtility::CompareAndReset(item->bid_price4 , m_bid_price4);
    text = StrUtils::formatstring(15,"%.2f",m_bid_price4);
    break;
  case IDC_BIDPRICE5:
    equal_flag = StudioUtility::CompareAndReset(item->bid_price5 , m_bid_price5);
    text = StrUtils::formatstring(15,"%.2f",m_bid_price5);
    break;  
  default:
    equal_flag = true;
    break;
  }
  RedrawText(equal_flag, nID,text);
}


void CMonitorQuote::RedrawSingleL2MarketDataEdit( int nID, L2MarketSnapshot* item )
{
  bool equal_flag;
  string text("");
  switch(nID)
  {
  case IDC_TRADETIME:
    equal_flag = StudioUtility::CompareAndReset(wmdf::DateTime::time2hms(item->data_time_stamp) , m_trade_time);
    text = StrUtils::formatstring(15,"%d",m_trade_time);
    break;
  case IDC_BIDVOLUME1:
    equal_flag = StudioUtility::CompareAndReset(item->bid_size1 , m_bid_size1);
    text = StrUtils::formatstring(15,"%d",m_bid_size1);
    break;
  case IDC_BIDVOLUME2:
    equal_flag = StudioUtility::CompareAndReset(item->bid_size2 , m_bid_size2);
    text = StrUtils::formatstring(15,"%d",m_bid_size2);
    break;
  case IDC_BIDVOLUME3:
    equal_flag = StudioUtility::CompareAndReset(item->bid_size3 , m_bid_size3);
    text = StrUtils::formatstring(15,"%d",m_bid_size3);
    break;
  case IDC_BIDVOLUME4:
    equal_flag = StudioUtility::CompareAndReset(item->bid_size4 , m_bid_size4);
    text = StrUtils::formatstring(15,"%d",m_bid_size4);
    break;
  case IDC_BIDVOLUME5:
    equal_flag = StudioUtility::CompareAndReset(item->bid_size5 , m_bid_size5);
    text = StrUtils::formatstring(15,"%d",m_bid_size5);
    break;
  case IDC_BIDVOLUME6:
    equal_flag = StudioUtility::CompareAndReset(item->bid_size6 , m_bid_size6);
    text = StrUtils::formatstring(15,"%d",m_bid_size6);
    break;
  case IDC_BIDVOLUME7:
    equal_flag = StudioUtility::CompareAndReset(item->bid_size7 , m_bid_size7);
    text = StrUtils::formatstring(15,"%d",m_bid_size7);
    break;
  case IDC_BIDVOLUME8:
    equal_flag = StudioUtility::CompareAndReset(item->bid_size8 , m_bid_size8);
    text = StrUtils::formatstring(15,"%d",m_bid_size8);
    break;
  case IDC_BIDVOLUME9:
    equal_flag = StudioUtility::CompareAndReset(item->bid_size9 , m_bid_size9);
    text = StrUtils::formatstring(15,"%d",m_bid_size9);
    break;
  case IDC_BIDVOLUME10:
    equal_flag = StudioUtility::CompareAndReset(item->bid_size10 , m_bid_size10);
    text = StrUtils::formatstring(15,"%d",m_bid_size10);
    break;
  case IDC_ASKVOLUME1:
    equal_flag = StudioUtility::CompareAndReset(item->ask_size1 , m_ask_size1);
    text = StrUtils::formatstring(15,"%d",m_ask_size1);
    break;
  case IDC_ASKVOLUME2:
    equal_flag = StudioUtility::CompareAndReset(item->ask_size2 , m_ask_size2);
    text = StrUtils::formatstring(15,"%d",m_ask_size2);
    break;
  case IDC_ASKVOLUME3:
    equal_flag = StudioUtility::CompareAndReset(item->ask_size3 , m_ask_size3);
    text = StrUtils::formatstring(15,"%d",m_ask_size3);
    break;
  case IDC_ASKVOLUME4:
    equal_flag = StudioUtility::CompareAndReset(item->ask_size4 , m_ask_size4);
    text = StrUtils::formatstring(15,"%d",m_ask_size4);
    break;
  case IDC_ASKVOLUME5:
    equal_flag = StudioUtility::CompareAndReset(item->ask_size5 , m_ask_size5);
    text = StrUtils::formatstring(15,"%d",m_ask_size5);
    break;
  case IDC_ASKVOLUME6:
    equal_flag = StudioUtility::CompareAndReset(item->ask_size6 , m_ask_size6);
    text = StrUtils::formatstring(15,"%d",m_ask_size6);
    break;
  case IDC_ASKVOLUME7:
    equal_flag = StudioUtility::CompareAndReset(item->ask_size7 , m_ask_size7);
    text = StrUtils::formatstring(15,"%d",m_ask_size7);
    break;
  case IDC_ASKVOLUME8:
    equal_flag = StudioUtility::CompareAndReset(item->ask_size8 , m_ask_size8);
    text = StrUtils::formatstring(15,"%d",m_ask_size8);
    break;
  case IDC_ASKVOLUME9:
    equal_flag = StudioUtility::CompareAndReset(item->ask_size9 , m_ask_size9);
    text = StrUtils::formatstring(15,"%d",m_ask_size9);
    break;
  case IDC_ASKVOLUME10:
    equal_flag = StudioUtility::CompareAndReset(item->ask_size10 , m_ask_size10);
    text = StrUtils::formatstring(15,"%d",m_ask_size10);
    break;
  case IDC_TOTALVOLUME:
    equal_flag = StudioUtility::CompareAndReset(item->total_volume , m_total_volume);
    text = StrUtils::formatstring(15,"%I64d",m_total_volume);
    break;
  case IDC_TOTALASKVOLUME:
    equal_flag = StudioUtility::CompareAndReset(item->total_ask_size,m_total_ask_size);
    text = StrUtils::formatstring(15,"%I64d",m_total_ask_size);
    break;
  case IDC_TOTALBIDVOLUME:
    equal_flag = StudioUtility::CompareAndReset(item->total_bid_size , m_total_bid_size);
    text = StrUtils::formatstring(15,"%I64d",m_total_bid_size);
    break;
  case IDC_DEALCOUNT:
    equal_flag = StudioUtility::CompareAndReset(item->transactions_count , m_deal_count);
    text = StrUtils::formatstring(15,"%I64d",m_deal_count);
    break;
  case IDC_TOTALAMOUNT:
    equal_flag = StudioUtility::CompareAndReset(item->total_amount , m_total_amount);
    text = StrUtils::formatstring(20,"%.2f",m_total_amount);
    break;
  case IDC_OPEN:
    equal_flag = StudioUtility::CompareAndReset(item->open_price , m_open);
    text = StrUtils::formatstring(15,"%.2f",m_open);
    break;
  case IDC_HIGH:
    equal_flag = StudioUtility::CompareAndReset(item->high_price , m_high);
    text = StrUtils::formatstring(15,"%.2f",m_high);
    break;
  case IDC_LOW:
    equal_flag = StudioUtility::CompareAndReset(item->low_price , m_low);
    text = StrUtils::formatstring(15,"%.2f",m_low);
    break;
  case IDC_NEW:
    equal_flag = StudioUtility::CompareAndReset(item->new_price , m_new_price);
    text = StrUtils::formatstring(15,"%.2f",m_new_price);
    break;
  case IDC_PRECLOSE:
    equal_flag = StudioUtility::CompareAndReset(item->pre_close , m_pre_close);
    text = StrUtils::formatstring(15,"%.2f",m_pre_close);
    break;
  case IDC_AVGASKPRICE:
    equal_flag = StudioUtility::CompareAndReset(item->weighted_average_ask_price , m_avg_ask_price);
    text = StrUtils::formatstring(15,"%.2f",m_avg_ask_price);
    break;
  case IDC_AVGBIDPRICE:
    equal_flag = StudioUtility::CompareAndReset(item->weighted_average_bid_price , m_avg_bid_price);
    text = StrUtils::formatstring(15,"%.2f",m_avg_bid_price);
    break;
  case IDC_ASKPRICE1:
    equal_flag = StudioUtility::CompareAndReset(item->ask_price1 , m_ask_price1);
    text = StrUtils::formatstring(15,"%.2f",m_ask_price1);
    break;
  case IDC_ASKPRICE2:
    equal_flag = StudioUtility::CompareAndReset(item->ask_price2 , m_ask_price2);
    text = StrUtils::formatstring(15,"%.2f",m_ask_price2);
    break;
  case IDC_ASKPRICE3:
    equal_flag = StudioUtility::CompareAndReset(item->ask_price3 , m_ask_price3);
    text = StrUtils::formatstring(15,"%.2f",m_ask_price3);
    break;
  case IDC_ASKPRICE4:
    equal_flag = StudioUtility::CompareAndReset(item->ask_price4 , m_ask_price4);
    text = StrUtils::formatstring(15,"%.2f",m_ask_price4);
    break;
  case IDC_ASKPRICE5:
    equal_flag = StudioUtility::CompareAndReset(item->ask_price5 , m_ask_price5);
    text = StrUtils::formatstring(15,"%.2f",m_ask_price5);
    break;
  case IDC_ASKPRICE6:
    equal_flag = StudioUtility::CompareAndReset(item->ask_price6 , m_ask_price6);
    text = StrUtils::formatstring(15,"%.2f",m_ask_price6);
    break;
  case IDC_ASKPRICE7:
    equal_flag = StudioUtility::CompareAndReset(item->ask_price7 , m_ask_price7);
    text = StrUtils::formatstring(15,"%.2f",m_ask_price7);
    break;
  case IDC_ASKPRICE8:
    equal_flag = StudioUtility::CompareAndReset(item->ask_price8 , m_ask_price8);
    text = StrUtils::formatstring(15,"%.2f",m_ask_price8);
    break;
  case IDC_ASKPRICE9:
    equal_flag = StudioUtility::CompareAndReset(item->ask_price9 , m_ask_price9);
    text = StrUtils::formatstring(15,"%.2f",m_ask_price9);
    break;
  case IDC_ASKPRICE10:
    equal_flag = StudioUtility::CompareAndReset(item->ask_price10 , m_ask_price10);
    text = StrUtils::formatstring(15,"%.2f",m_ask_price10);
    break;
  case IDC_BIDPRICE1:
    equal_flag = StudioUtility::CompareAndReset(item->bid_price1 , m_bid_price1);
    text = StrUtils::formatstring(15,"%.2f",m_bid_price1);
    break;
  case IDC_BIDPRICE2:
    equal_flag = StudioUtility::CompareAndReset(item->bid_price2 , m_bid_price2);
    text = StrUtils::formatstring(15,"%.2f",m_bid_price2);
    break;
  case IDC_BIDPRICE3:
    equal_flag = StudioUtility::CompareAndReset(item->bid_price3 , m_bid_price3);
    text = StrUtils::formatstring(15,"%.2f",m_bid_price3);
    break;
  case IDC_BIDPRICE4:
    equal_flag = StudioUtility::CompareAndReset(item->bid_price4 , m_bid_price4);
    text = StrUtils::formatstring(15,"%.2f",m_bid_price4);
    break;
  case IDC_BIDPRICE5:
    equal_flag = StudioUtility::CompareAndReset(item->bid_price5 , m_bid_price5);
    text = StrUtils::formatstring(15,"%.2f",m_bid_price5);
    break;
  case IDC_BIDPRICE6:
    equal_flag = StudioUtility::CompareAndReset(item->bid_price6 , m_bid_price6);
    text = StrUtils::formatstring(15,"%.2f",m_bid_price6);
    break;
  case IDC_BIDPRICE7:
    equal_flag = StudioUtility::CompareAndReset(item->bid_price7 , m_bid_price7);
    text = StrUtils::formatstring(15,"%.2f",m_bid_price7);
    break;
  case IDC_BIDPRICE8:
    equal_flag = StudioUtility::CompareAndReset(item->bid_price8 , m_bid_price8);
    text = StrUtils::formatstring(15,"%.2f",m_bid_price8);
    break;
  case IDC_BIDPRICE9:
    equal_flag = StudioUtility::CompareAndReset(item->bid_price9 , m_bid_price9);
    text = StrUtils::formatstring(15,"%.2f",m_bid_price9);
    break;
  case IDC_BIDPRICE10:
    equal_flag = StudioUtility::CompareAndReset(item->bid_price10 , m_bid_price10);
    text = StrUtils::formatstring(15,"%.2f",m_bid_price10);
    break;
  case IDC_PERATIO1:
    equal_flag = StudioUtility::CompareAndReset(item->peratio1 , m_peratio1);
    text = StrUtils::formatstring(15,"%.2f",m_peratio1);
    break;
  case IDC_PERATIO2:
    equal_flag = StudioUtility::CompareAndReset(item->peratio2 , m_peratio2);
    text = StrUtils::formatstring(15,"%.2f",m_peratio2);
    break;
  case IDC_TRADE_STATUS:
    equal_flag = StudioUtility::CompareAndReset(item->trade_status , m_trade_status);
    text = StrUtils::formatstring(10,"%d",m_peratio2);
  default:
    equal_flag = true;
    break;
  }
  RedrawText(equal_flag, nID,text);
}

void CMonitorQuote::RedrawSingleL2TransactionEdit( int nID, L2Transaction* item )
{
  bool equal_flag;
  string text("");
  switch(nID)
  {
  case IDC_TRADETIME2:
    equal_flag = StudioUtility::CompareAndReset(wmdf::DateTime::time2hms(item->trade_time) , m_trade_time2);
    text = StrUtils::formatstring(15,"%d",m_trade_time2);
    break;
  case IDC_TRADEPRICE:
    equal_flag = StudioUtility::CompareAndReset(item->price , m_trade_price2);
    text = StrUtils::formatstring(15,"%.2f",m_trade_price2);
    break;
  case IDC_TRADEVOLUME:
    equal_flag = StudioUtility::CompareAndReset(item->volume , m_trade_volume);
    text = StrUtils::formatstring(15,"%d",m_trade_volume);
    break;
  case IDC_TRADEAMOUNT:
    equal_flag = StudioUtility::CompareAndReset(item->amount , m_trade_amount);
    text = StrUtils::formatstring(15,"%I64d",m_trade_amount);
    break;
  case IDC_SETNO:
    equal_flag = StudioUtility::CompareAndReset(item->set_no , m_set_no);
    text = StrUtils::formatstring(15,"%d",m_set_no);
    break;
  case IDC_RECNO:
    equal_flag = StudioUtility::CompareAndReset(item->rec_no , m_rec_no);
    text = StrUtils::formatstring(15,"%d",m_rec_no);
    break;
  case IDC_BUYORDERNO:
    equal_flag = StudioUtility::CompareAndReset(item->buyOrderRecNo , m_buy_order_no);
    text = StrUtils::formatstring(15,"%d",m_buy_order_no);
    break;
  case IDC_SELLORDERNO:
    equal_flag = StudioUtility::CompareAndReset(item->sellOrderRecNo , m_sell_order_no);
    text = StrUtils::formatstring(15,"%d",m_sell_order_no);
    break;
  case IDC_ORDERKIND:
    equal_flag = StudioUtility::CompareAndReset(item->order_kind , m_order_kind);
    text = StrUtils::formatstring(15,"%d",m_order_kind);
    break;
  case IDC_FUNCTIONCODE:
    equal_flag = StudioUtility::CompareAndReset(item->function_code , m_function_code);
    text = StrUtils::formatstring(15,"%d",m_function_code);
    break;
  default:
    equal_flag = true;
    break;;
  }  
  RedrawText(equal_flag, nID,text);
}

void CMonitorQuote::RedrawSingleFutureMarketDataEdit( int nID, FuturesMarketSnapshot* item )
{
  bool equal_flag;
  string text("");
  switch(nID)
  {
  case IDC_TRADEDATE:
    equal_flag = StudioUtility::CompareAndReset(item->trade_date , m_trade_date);
    text = StrUtils::formatstring(15,"%d",m_trade_date);
    break;
  case IDC_TRADETIME:
    equal_flag = StudioUtility::CompareAndReset(wmdf::DateTime::time2hms(item->data_time_stamp) , m_trade_time);
    text = StrUtils::formatstring(15,"%d",m_trade_time);
    break;
  case IDC_BIDVOLUME1:
    equal_flag = StudioUtility::CompareAndReset(item->bid_size1 , m_bid_size1);
    text = StrUtils::formatstring(15,"%d",m_bid_size1);
    break;  
  case IDC_ASKVOLUME1:
    equal_flag = StudioUtility::CompareAndReset(item->ask_size1 , m_ask_size1);
    text = StrUtils::formatstring(15,"%d",m_ask_size1);
    break;
  case IDC_TOTALVOLUME:
    equal_flag = StudioUtility::CompareAndReset(item->total_volume , m_total_volume);
    text = StrUtils::formatstring(15,"%I64d",m_total_volume);
    break;
  case IDC_TOTALAMOUNT:
    equal_flag = StudioUtility::CompareAndReset(item->total_amount , m_total_amount);
    text = StrUtils::formatstring(20,"%.2f",m_total_amount);
    break;
  case IDC_OPEN:
    equal_flag = StudioUtility::CompareAndReset(item->open_price , m_open);
    text = StrUtils::formatstring(15,"%.2f",m_open);
    break;
  case IDC_HIGH:
    equal_flag = StudioUtility::CompareAndReset(item->high_price , m_high);
    text = StrUtils::formatstring(15,"%.2f",m_high);
    break;
  case IDC_LOW:
    equal_flag = StudioUtility::CompareAndReset(item->low_price , m_low);
    text = StrUtils::formatstring(15,"%.2f",m_low);
    break;
  case IDC_NEW:
    equal_flag = StudioUtility::CompareAndReset(item->new_price , m_new_price);
    text = StrUtils::formatstring(15,"%.2f",m_new_price);
    break;
  case IDC_PRECLOSE:
    equal_flag = StudioUtility::CompareAndReset(item->pre_close , m_pre_close);
    text = StrUtils::formatstring(15,"%.2f",m_pre_close);
    break;
  case IDC_ASKPRICE1:
    equal_flag = StudioUtility::CompareAndReset(item->ask_price1 , m_ask_price1);
    text = StrUtils::formatstring(15,"%.2f",m_ask_price1);
    break; 
  case IDC_BIDPRICE1:
    equal_flag = StudioUtility::CompareAndReset(item->bid_price1 , m_bid_price1);
    text = StrUtils::formatstring(15,"%.2f",m_bid_price1);
    break;  
  case IDC_PRE_SETTLE:
    equal_flag = StudioUtility::CompareAndReset(item->pre_settle , m_pre_settle);
    text = StrUtils::formatstring(15,"%.2f",m_pre_settle);
    break;  
  case IDC_SETTLE:
    equal_flag = StudioUtility::CompareAndReset(item->settle , m_settle);
    text = StrUtils::formatstring(15,"%.2f",m_settle);
    break;  
  case IDC_PRE_POSITION:
    equal_flag = StudioUtility::CompareAndReset(item->pre_position , m_pre_position);
    text = StrUtils::formatstring(15,"%I64d",m_pre_position);
    break;  
  case IDC_POSITION:
    equal_flag = StudioUtility::CompareAndReset(item->position , m_position);
    text = StrUtils::formatstring(15,"%I64d",m_position);
    break;  
  case IDC_HIGHLIMIT:
    equal_flag = StudioUtility::CompareAndReset(item->high_limit , m_high_limit);
    text = StrUtils::formatstring(15,"%.2f",m_high_limit);
    break;  
  case IDC_LOW_LIMIT:
    equal_flag = StudioUtility::CompareAndReset(item->low_limit , m_low_limit);
    text = StrUtils::formatstring(15,"%.2f",m_low_limit);
    break;  
  case IDC_AVGPRICE:
    equal_flag = StudioUtility::CompareAndReset(item->average_price, m_avgprice);
    text = StrUtils::formatstring(15,"%.2f",m_avgprice);
    break;  
  default:
    equal_flag = true;
    break;
  }
  RedrawText(equal_flag, nID,text);
}

void CMonitorQuote::RedrawSingleCFFEL2MarketDataEdit( int nID, CFEL2MarketSnapshot* item )
{
  bool equal_flag;
  string text("");
  switch(nID)
  {
  case IDC_TRADEDATE:
    equal_flag = StudioUtility::CompareAndReset(item->trade_date , m_trade_date);
    text = StrUtils::formatstring(15,"%d",m_trade_date);
    break;
  case IDC_TRADETIME:
    equal_flag = StudioUtility::CompareAndReset(wmdf::DateTime::time2hms(item->data_time_stamp) , m_trade_time);
    text = StrUtils::formatstring(15,"%d",m_trade_time);
    break;
  case IDC_BIDVOLUME1:
    equal_flag = StudioUtility::CompareAndReset(item->bid_size1 , m_bid_size1);
    text = StrUtils::formatstring(15,"%d",m_bid_size1);
    break;
  case IDC_BIDVOLUME2:
    equal_flag = StudioUtility::CompareAndReset(item->bid_size2 , m_bid_size2);
    text = StrUtils::formatstring(15,"%d",m_bid_size2);
    break;
  case IDC_BIDVOLUME3:
    equal_flag = StudioUtility::CompareAndReset(item->bid_size3 , m_bid_size3);
    text = StrUtils::formatstring(15,"%d",m_bid_size3);
    break;
  case IDC_BIDVOLUME4:
    equal_flag = StudioUtility::CompareAndReset(item->bid_size4 , m_bid_size4);
    text = StrUtils::formatstring(15,"%d",m_bid_size4);
    break;
  case IDC_BIDVOLUME5:
    equal_flag = StudioUtility::CompareAndReset(item->bid_size5 , m_bid_size5);
    text = StrUtils::formatstring(15,"%d",m_bid_size5);
    break;
  case IDC_ASKVOLUME1:
    equal_flag = StudioUtility::CompareAndReset(item->ask_size1 , m_ask_size1);
    text = StrUtils::formatstring(15,"%d",m_ask_size1);
    break;
  case IDC_ASKVOLUME2:
    equal_flag = StudioUtility::CompareAndReset(item->ask_size2 , m_ask_size2);
    text = StrUtils::formatstring(15,"%d",m_ask_size2);
    break;
  case IDC_ASKVOLUME3:
    equal_flag = StudioUtility::CompareAndReset(item->ask_size3 , m_ask_size3);
    text = StrUtils::formatstring(15,"%d",m_ask_size3);
    break;
  case IDC_ASKVOLUME4:
    equal_flag = StudioUtility::CompareAndReset(item->ask_size4 , m_ask_size4);
    text = StrUtils::formatstring(15,"%d",m_ask_size4);
    break;
  case IDC_ASKVOLUME5:
    equal_flag = StudioUtility::CompareAndReset(item->ask_size5 , m_ask_size5);
    text = StrUtils::formatstring(15,"%d",m_ask_size5);
    break;
  case IDC_TOTALVOLUME:
    equal_flag = StudioUtility::CompareAndReset(item->total_volume , m_total_volume);
    text = StrUtils::formatstring(15,"%I64d",m_total_volume);
    break;
  case IDC_TOTALAMOUNT:
    equal_flag = StudioUtility::CompareAndReset(item->total_amount , m_total_amount);
    text = StrUtils::formatstring(20,"%.2f",m_total_amount);
    break;
  case IDC_OPEN:
    equal_flag = StudioUtility::CompareAndReset(item->open_price , m_open);
    text = StrUtils::formatstring(15,"%.2f",m_open);
    break;
  case IDC_HIGH:
    equal_flag = StudioUtility::CompareAndReset(item->high_price , m_high);
    text = StrUtils::formatstring(15,"%.2f",m_high);
    break;
  case IDC_LOW:
    equal_flag = StudioUtility::CompareAndReset(item->low_price , m_low);
    text = StrUtils::formatstring(15,"%.2f",m_low);
    break;
  case IDC_NEW:
    equal_flag = StudioUtility::CompareAndReset(item->new_price , m_new_price);
    text = StrUtils::formatstring(15,"%.2f",m_new_price);
    break;
  case IDC_PRECLOSE:
    equal_flag = StudioUtility::CompareAndReset(item->pre_close , m_pre_close);
    text = StrUtils::formatstring(15,"%.2f",m_pre_close);
    break;
  case IDC_ASKPRICE1:
    equal_flag = StudioUtility::CompareAndReset(item->ask_price1 , m_ask_price1);
    text = StrUtils::formatstring(15,"%.2f",m_ask_price1);
    break;
  case IDC_ASKPRICE2:
    equal_flag = StudioUtility::CompareAndReset(item->ask_price2 , m_ask_price2);
    text = StrUtils::formatstring(15,"%.2f",m_ask_price2);
    break;
  case IDC_ASKPRICE3:
    equal_flag = StudioUtility::CompareAndReset(item->ask_price3 , m_ask_price3);
    text = StrUtils::formatstring(15,"%.2f",m_ask_price3);
    break;
  case IDC_ASKPRICE4:
    equal_flag = StudioUtility::CompareAndReset(item->ask_price4 , m_ask_price4);
    text = StrUtils::formatstring(15,"%.2f",m_ask_price4);
    break;
  case IDC_ASKPRICE5:
    equal_flag = StudioUtility::CompareAndReset(item->ask_price5 , m_ask_price5);
    text = StrUtils::formatstring(15,"%.2f",m_ask_price5);
    break;
  case IDC_BIDPRICE1:
    equal_flag = StudioUtility::CompareAndReset(item->bid_price1 , m_bid_price1);
    text = StrUtils::formatstring(15,"%.2f",m_bid_price1);
    break;
  case IDC_BIDPRICE2:
    equal_flag = StudioUtility::CompareAndReset(item->bid_price2 , m_bid_price2);
    text = StrUtils::formatstring(15,"%.2f",m_bid_price2);
    break;
  case IDC_BIDPRICE3:
    equal_flag = StudioUtility::CompareAndReset(item->bid_price3 , m_bid_price3);
    text = StrUtils::formatstring(15,"%.2f",m_bid_price3);
    break;
  case IDC_BIDPRICE4:
    equal_flag = StudioUtility::CompareAndReset(item->bid_price4 , m_bid_price4);
    text = StrUtils::formatstring(15,"%.2f",m_bid_price4);
    break;
  case IDC_BIDPRICE5:
    equal_flag = StudioUtility::CompareAndReset(item->bid_price5 , m_bid_price5);
    text = StrUtils::formatstring(15,"%.2f",m_bid_price5);
    break;  
  case IDC_PRE_SETTLE:
    equal_flag = StudioUtility::CompareAndReset(item->pre_settle , m_pre_settle);
    text = StrUtils::formatstring(15,"%.2f",m_pre_settle);
    break;
  case IDC_PRE_POSITION:
    equal_flag = StudioUtility::CompareAndReset(item->pre_position , m_pre_position);
    text = StrUtils::formatstring(15,"%I64d",m_pre_position);
    break;  
  case IDC_POSITION:
    equal_flag = StudioUtility::CompareAndReset(item->position , m_position);
    text = StrUtils::formatstring(15,"%I64d",m_position);
    break;  
  case IDC_HIGHLIMIT:
    equal_flag = StudioUtility::CompareAndReset(item->high_limit , m_high_limit);
    text = StrUtils::formatstring(15,"%.2f",m_high_limit);
    break;  
  case IDC_LOW_LIMIT:
    equal_flag = StudioUtility::CompareAndReset(item->low_limit , m_low_limit);
    text = StrUtils::formatstring(15,"%.2f",m_low_limit);
    break;    
  default:
    equal_flag = true;
    break;
  }
  RedrawText(equal_flag, nID,text);
}

void CMonitorQuote::RedrawText( bool equal_flag, int nID,std::string text )
{
  if(equal_flag)
  {
    DrawDefaultText(nID);
  }
  else
  {    
    DrawColorText(nID,text);
  }
}

void CMonitorQuote::DrawDefaultText(int nID)
{
  m_colorEditBK = RGB(255,255,255);
  // redraw edit control
  ((CEdit *)GetDlgItem(nID))->SetRedraw(TRUE);
  // invalidate the entire control, force painting
  ((CEdit *)GetDlgItem(nID))->Invalidate();
  ((CEdit *)GetDlgItem(nID))->UpdateWindow();
}

void CMonitorQuote::DrawColorText(int nID,std::string text)
{
  m_colorEditBK = RGB(255,255,0);
  ((CEdit *)GetDlgItem(nID))->SetWindowText(text.c_str());
  ((CEdit *)GetDlgItem(nID))->SetRedraw(TRUE);
  ((CEdit *)GetDlgItem(nID))->Invalidate();  
  ((CEdit *)GetDlgItem(nID))->UpdateWindow();
}









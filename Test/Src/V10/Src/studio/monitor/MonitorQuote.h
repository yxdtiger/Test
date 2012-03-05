#pragma once

#include "../monitor/quote_invoke.h"
//#include "../main_frame/EasySize.h"
#include "afxwin.h"
#include <string>
// CMonitorQuote form view
using namespace wmdf;

class CMonitorQuote : public CFormView
{  
	DECLARE_DYNCREATE(CMonitorQuote)
  //DECLARE_EASYSIZE

protected:
	CMonitorQuote();           // protected constructor used by dynamic creation
	virtual ~CMonitorQuote();

public:
	enum { IDD = IDD_MONITOR_QUOTE };
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
  virtual void OnInitialUpdate();

  afx_msg LRESULT OnReceiveL1MktRecord(WPARAM wParam, LPARAM lParam);
  afx_msg LRESULT OnReceiveL2MktRecord(WPARAM wParam, LPARAM lParam);  
  afx_msg LRESULT OnReceiveL2TransRecord(WPARAM wParam, LPARAM lParam);
  afx_msg LRESULT OnReceiveFutureMktRecord(WPARAM wParam, LPARAM lParam);  
  afx_msg LRESULT OnReceiveCFFEL2MktRecord(WPARAM wParam, LPARAM lParam); 
  afx_msg void OnBnClickedSetwindcode();
  afx_msg void OnBnClickedStart();  
  afx_msg void OnBnClickedStop();
  afx_msg void OnDestroy();
  afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
private:
  void ResetAll();
  void RedrawSingleL1MarketDataEdit(int nID, L1MarketSnapshot* item);  
  void RedrawSingleL2MarketDataEdit(int nID, L2MarketSnapshot* item);   
  void RedrawSingleL2TransactionEdit(int nID, L2Transaction* item);
  void RedrawSingleFutureMarketDataEdit(int nID, FuturesMarketSnapshot* item);  
  void RedrawSingleCFFEL2MarketDataEdit(int nID, CFEL2MarketSnapshot* item);  

  void RedrawText( bool equal_flag, int nID ,std::string text);
  void DrawDefaultText(int nID);
  void DrawColorText(int nID,std::string text);
  QuoteInvoke* invoker_;
  
  //COLORREF m_colorEditText; // edit控件的字体颜色
  COLORREF m_colorEditBK;  // edit控件的背景颜色
  CBrush* m_pEditBkBrush;
public:
  
  CComboBox m_server_list;
  int m_trade_time;
  double m_new_price;
  
  
  CString m_windcode;
  int m_trade_date;
  double m_pre_close;
  double m_high;
  double m_open;
  double m_low;  
  double m_avg_bid_price;
  double m_peratio1;
  double m_peratio2;
  LONGLONG m_total_bid_size;
  LONGLONG m_total_ask_size;
  double m_avg_ask_price;
  LONGLONG m_deal_count;
  
  int m_bid_size1;
  int m_bid_size2;
  int m_bid_size3;
  int m_bid_size4;
  int m_bid_size5;
  int m_bid_size6;
  int m_bid_size7;
  int m_bid_size8;
  int m_bid_size9;
  int m_bid_size10;
  int m_ask_size1;
  int m_ask_size2;
  int m_ask_size3;
  int m_ask_size4;
  int m_ask_size5;
  int m_ask_size6;
  int m_ask_size7;
  int m_ask_size8;
  int m_ask_size9;
  int m_ask_size10;

  double m_bid_price1;
  double m_bid_price2;
  double m_bid_price3;
  double m_bid_price4;
  double m_bid_price5;
  double m_bid_price6;
  double m_bid_price7;
  double m_bid_price8;
  double m_bid_price9;
  double m_bid_price10;
  double m_ask_price1;
  double m_ask_price2;
  double m_ask_price3;
  double m_ask_price4;
  double m_ask_price5;
  double m_ask_price6;
  double m_ask_price7;
  double m_ask_price8;
  double m_ask_price9;
  double m_ask_price10;
  
  
  int m_trade_time2;
  double m_trade_price2;
  int m_trade_volume;
  LONGLONG m_trade_amount;
  int m_set_no;
  int m_rec_no;
  int m_buy_order_no;
  int m_sell_order_no;
  int m_order_kind;
  int m_function_code;
  double m_total_amount;
  LONGLONG m_total_volume;
  double m_pre_settle;
  double m_settle;
  LONGLONG m_pre_position;
  LONGLONG m_position;
  double m_high_limit;
  double m_low_limit;
  double m_avgprice;
  int m_trade_status;
};



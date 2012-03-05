#ifndef TEST_CODEC_H
#define TEST_CODEC_H

#include "shl2_market_data_codec_test.h"
#include "szl2_market_data_codec_test.h"
#include "shszl1_market_data_codec_test.h"
#include "shl2_order_queue_codec_test.h"
#include "shl2_trans_codec_test.h"
#include "cffel1_market_data_codec_test.h"
#include "szl2_orders_codec_test.h"
#include "szl2_trans_codec_test.h"

#include "binary_codec_login_request_test.h"
#include "binary_codec_login_response_test.h"
#include "binary_codec_logout_test.h"
#include "binary_codec_shl2_order_queue_test.h"
#include "binary_codec_shl2_trans_test.h"
#include "binary_codec_shszl1_market_data_test.h"
#include "binary_codec_shl2_market_data_test.h"
#include "binary_codec_cffel1_market_data_test.h"

namespace wmdf{
class TestCodec
{
public:
  static void RunTest();
};

void TestCodec::RunTest()
{
  TestLoginRequestBinaryCodec();
  TestLoginResponseBinaryCodec();
  TestLogoutBinaryCodec();

  TestSHL2TransactionCodec();
  TestSHL2OrderQueueCodec();
  TestSHL2MarketDataCodec();
  TestSZL2MarketDataCodec();
  TestSZL2OrdersCodec();
  TestSZL2TransactionCodec();
  TestSHSZL1MarketDataCodec();
  TestCFFEL1MarketDataCodec();

  TestSHSZL1MarketDataBinaryCodec();
  TestSHL2MarketDataBinaryCodec();
  TestSSEL2TransBinaryCodec();
  TestSHL2OrderQueueBinaryCodec();   
  TestCFFEL1MarketDataBianryCodec();

  
}

}
#endif
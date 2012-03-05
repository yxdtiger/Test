#pragma once

#include <string>
#include "../../include/wmdf_api.h"

// 对studio中出现的id转义成字符
class CWordTranslation
{
public:
  CWordTranslation(void);
  ~CWordTranslation(void);

public:
  // 将product id 转译成product name
  static std::string TranslationProductID(const int16_t product_id);

  // 将message id 转译成message name
  static std::string TranslationMessageID(const int16_t product_id, const int16_t message_id);

  // 将委托队列的side转译成name
  static std::string TranslationOrderQueueL2Side(const uint8_t side);

  // 将委托队列的image_status转译成name
  static std::string TranslationOrderQueueL2ImageStatus(const int32_t image_status);

  // 将委托队列的price_level_operator转译成name
  static std::string TranslationOrderQueueL2PriceLevelOperator(const int32_t price_level_operator);

  // 将委托队列的order_queue_operator转译成name
  static std::string TranslationOrderQueueL2OrderQueueOperator(const int32_t order_queue_operator);

  // 将逐笔委托的order_kind转译成name
  static std::string TranslationOrdersL2order_kind(const char order_kind);

  static std::string TranslateServerStatus(const int32_t server_stat);
};

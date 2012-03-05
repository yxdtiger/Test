#ifndef SZSE_STEP_MARKETDATA_TEMPLATE_H
#define SZSE_STEP_MARKETDATA_TEMPLATE_H

#include "../common/step_template.h"
#include <map>
#include <string>

namespace wmdf
{
  struct MarketData;

	class SZSEStepMarketDataTemplate : public StepTemplate
	{
	public:
		SZSEStepMarketDataTemplate();
		~SZSEStepMarketDataTemplate();
		virtual WindStepMessage* DecodeRaw(uint8_t* message_start,uint8_t* message_end);
    WindStepMessage* bid_order_queue_msg() {return bid_order_queue_msg_;}
    WindStepMessage* ask_order_queue_msg() {return ask_order_queue_msg_;}
  private:
    void DecodeOrderQueue(MarketData* market_data,int32_t orders_total,int32_t orders_count,int32_t side);
  private:
		std::map<std::string,CompareStruct*> compare_map_;
    WindStepMessage* bid_order_queue_msg_;
    WindStepMessage* ask_order_queue_msg_;
	};

}

#endif

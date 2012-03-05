#ifndef SSE_FAST_TEMPLATE_DECODER_H
#define SSE_FAST_TEMPLATE_DECODER_H

#include <vector>
#include "../common/fast_template_decoder.h"


namespace wmdf
{
  class FastTemplate;

  class SSEFastTemplateDecoder: public FastTemplateDecoder
	{
	public:
		SSEFastTemplateDecoder(int32_t trade_date);
		~SSEFastTemplateDecoder();
  protected:
    virtual void Decode96Field(uint8_t* tid,uint8_t*& field96_start,uint8_t* field96_end);
 private:
		FastTemplate* sse_marketdata_3102_template_;
		FastTemplate* sse_orderqueue_3111_template_;
		FastTemplate* sse_indexdata_3113_template_;
		FastTemplate* sse_virtualauctionprice_3107_template_;
		FastTemplate* sse_transaction_3101_template_;
		FastTemplate* sse_indexdata_1113_template_;
		FastTemplate* sse_virtualauctionprice_1107_template_;
		FastTemplate* sse_orderqueue_1111_template_;
		FastTemplate* sse_transaction_1101_template_;
		FastTemplate* sse_marketdata_1102_template_;
    FastTemplate* sse_marketdata_3202_template_;
    FastTemplate* sse_transaction_3201_template_;

	};
}


#endif

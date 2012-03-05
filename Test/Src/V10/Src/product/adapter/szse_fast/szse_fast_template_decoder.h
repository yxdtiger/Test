#ifndef SZSE_FAST_TEMPLATE_DECODER_H
#define SZSE_FAST_TEMPLATE_DECODER_H

#include <vector>
#include "../common/fast_template_decoder.h"

namespace wmdf
{
	class FastSzseIndexTemplate;
  class FastSzseOrdersTemplate;
  class FastSzseSnapshotTemplate;
  class FastSzseStockInfoTemplate;
  class FastSzseTradeTemplate;

  class SZSEFastTemplateDecoder:public FastTemplateDecoder
	{
  public:
    SZSEFastTemplateDecoder();
    virtual ~SZSEFastTemplateDecoder();
  protected:
    virtual void Decode96Field(uint8_t* tid,uint8_t*& field96_start,uint8_t* field96_end);
  private:
    FastSzseIndexTemplate* fast_szse_index_template_;
    FastSzseOrdersTemplate* fast_szse_orders_template_;
    FastSzseSnapshotTemplate* fast_szse_snapshot_template_;
    FastSzseStockInfoTemplate* fast_szse_stockinfo_template_;
    FastSzseTradeTemplate* fast_szse_trade_template_;
  };
}


#endif

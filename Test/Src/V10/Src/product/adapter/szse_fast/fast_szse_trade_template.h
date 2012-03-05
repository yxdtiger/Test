#ifndef FAST_SE_TRADE_TEMPLATE
#define FAST_SE_TRADE_TEMPLATE


#include "../common/fast_template.h"

namespace wmdf
{
  class FastSzseTradeTemplate:public FastTemplate
  {
  public:
    FastSzseTradeTemplate(void);
    ~FastSzseTradeTemplate(void);
    virtual WindFastMessage* DecodeRaw(uint8_t* raw_data,uint32_t len);
  private:
    static const int item_count_ = 11;//模板的一级子节点个数，其中不包含constant元素
  };
}

#endif

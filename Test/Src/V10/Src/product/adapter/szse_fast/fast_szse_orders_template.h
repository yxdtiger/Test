#ifndef FAST_SZSE_ORDERS_TEMPLATE
#define FAST_SZSE_ORDERS_TEMPLATE

#include "../common/fast_template.h"

namespace wmdf
{
  class FastSzseOrdersTemplate:public FastTemplate
  {
  public:
    FastSzseOrdersTemplate(void);
    ~FastSzseOrdersTemplate(void);
    virtual WindFastMessage* DecodeRaw(uint8_t* raw_data,uint32_t len);
  private:
    static const int item_count_ = 9;//模板的一级子节点个数，其中不包含constant元素
  };
}

#endif

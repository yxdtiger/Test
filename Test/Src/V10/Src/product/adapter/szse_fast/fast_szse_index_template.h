#ifndef FAST_SZSE_INDEX_TEMPLATE
#define FAST_SZSE_INDEX_TEMPLATE

#include "../common/fast_template.h"
#include <map>


namespace wmdf
{
  class FastSzseIndexTemplate:public FastTemplate
  {
  public:
    FastSzseIndexTemplate(void);
    ~FastSzseIndexTemplate(void);
    virtual WindFastMessage* DecodeRaw(uint8_t* raw_data,uint32_t len);
  private:
    static const int item_count_ = 12;//模板的一级子节点个数，其中不包含constant元素
    std::map<std::string,int32_t> time_map_;
  };
}
#endif

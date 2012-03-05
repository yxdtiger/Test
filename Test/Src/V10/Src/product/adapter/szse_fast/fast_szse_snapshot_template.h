#ifndef FAST_SE_SNAPSHOT_TEMPLATE
#define FAST_SE_SNAPSHOT_TEMPLATE

#include "../common/fast_template.h"
#include <map>

namespace wmdf
{
	struct CompareStruct;

  class FastSzseSnapshotTemplate:public FastTemplate
  {
  public:
    FastSzseSnapshotTemplate(void);
    ~FastSzseSnapshotTemplate(void);
    virtual WindFastMessage* DecodeRaw(uint8_t* raw_data,uint32_t len);
  private:
		std::map<string,CompareStruct*> compare_map_;
    static const int item_count_ = 21;//模板的一级子节点个数，其中不包含constant元素
  };
}
#endif

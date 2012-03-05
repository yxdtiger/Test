#ifndef FTSE_ADAPTER_H
#define FTSE_ADAPTER_H
#include "dbf_adapter.h"

namespace wmdf
{
  #define FTSE_WINDCODE_INDEX 0
  #define FTSE_PRECLOSE_INDEX 2
  #define FTSE_OPEN_INDEX 3
  #define FTSE_HIGH_INDEX 5
  #define FTSE_LOW_INDEX 6
  #define FTSE_NEW_INDEX 7
  #define FTSE_FIELD_NUMBER 22
  class FTSEAdapter:public DBFAdapter
  {
  public:
    FTSEAdapter(ProductSource* product_source);
    virtual ~FTSEAdapter(void);

    virtual wmdf::MdfMessage* CreateMDFMessage(int index,int count);
    virtual void* FetchARecord();
  private:
		static const char *ftse_suffix_;
  };
}

#endif

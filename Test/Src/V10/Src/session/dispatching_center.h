#ifndef DISPATCHING_CENTER_H
#define DISPATCHING_CENTER_H

#include <vector>
#include "../include/wmdf_api.h"

namespace wmdf
{
  class IProductAdapter;

  class DispatchingCenter
  {
  public:
    DispatchingCenter(void);
    virtual ~DispatchingCenter(void);
    void AddAdapter(IProductAdapter* adapter);
    void RemoveAdapter(IProductAdapter* adapter);
    void StartAllAdapters();
    void StopAllAdapters();
    void StartAllLoggers();
    void StopAllLoggers();

    IProductAdapter* adapters(int index);
    int32_t adapters_count();
  private:
    std::vector<IProductAdapter*> adapters_;
  };
}

#endif

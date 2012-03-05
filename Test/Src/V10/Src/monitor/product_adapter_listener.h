#ifndef PRODUCT_ADAPTER_LISTENER_H
#define PRODUCT_ADAPTER_LISTENER_H

#include "event_listener.h"

namespace wmdf
{
  class ProductAdapterListener:public EventListener
  {
  public:
    ProductAdapterListener(void);
    virtual ~ProductAdapterListener(void);
  };
}

#endif

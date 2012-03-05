#ifndef PRODUCT_SOURCE_MANAGER_H
#define PRODUCT_SOURCE_MANAGER_H

#include <vector>
#include "product_source.h"
#include "persistable.h"
#include "memento.h"
#include "../include/wmdf_api.h"

namespace wmdf
{
  class ProductSourceManager: public Persistable
  {
  public:
    ProductSourceManager(void);
    virtual ~ProductSourceManager(void);
    virtual bool LoadState(Memento* m);
    virtual bool SaveState(Memento* m);
    ProductSource* product_sources(int32_t index){return product_sources_[index];}
    void add_product_source(ProductSource* product_source){product_sources_.push_back(product_source);}
    int32_t product_source_count(){return (int32_t)product_sources_.size();}
    void delete_product_source(int32_t product_source_id);
    void delete_product_source(ProductSource* ptr_product_source);
    ProductSource* FindProductSource(int32_t product_source_id);
  private:
    std::vector<ProductSource*> product_sources_;
  };
}

#endif

#ifndef PRODUCT_MANAGER_H
#define PRODUCT_MANAGER_H

#include <string>
#include <vector>
#include "persistable.h"
#include "memento.h"
#include "product_descripter.h"
#include "../include/wmdf_api.h"

namespace wmdf
{
  class ProductManager : public Persistable
  {
  public:
    static ProductManager* instance();
    virtual bool SaveState(Memento* m);
    virtual bool LoadState(Memento* m);
    ProductDescripter* products(int32_t index){return products_[index];}
    void add_products(ProductDescripter* products){products_.push_back(products);}
    int32_t products_count(){return (int32_t)products_.size();}
    ProductDescripter* FindProductDescripter(int16_t product_id);
    ProductDescripter* FindProductDescripter(std::string product_name);
    void delete_products(int16_t product_id);
    void delete_products(ProductDescripter* ptr_product);
  private:
    ProductManager();
    ~ProductManager();
    std::vector<ProductDescripter*> products_;
    static ProductManager* instance_;
  };
}

#endif

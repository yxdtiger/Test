#ifndef PRODUCT_DESCRIPTER_H
#define PRODUCT_DESCRIPTER_H

#include <string>
#include <vector>
#include "persistable.h"
#include "memento.h"
#include "message_descripter.h"
#include "version_descripter.h"
#include "../include/wmdf_api.h"

namespace wmdf
{
  class ProductDescripter : public Persistable
  {
  public:
    ProductDescripter();
    ~ProductDescripter();
    virtual bool SaveState(Memento* m);
    virtual bool LoadState(Memento* m);
    int16_t product_id() const {return product_id_;}
    void set_product_id(int16_t product_id){product_id_=product_id;}
    std::string product_name() const {return product_name_;}
    void set_product_name(std::string product_name){product_name_=product_name;}
    std::string supplier() const {return supplier_;}
    void set_supplier(std::string supplier){supplier_=supplier;}
    std::string description() const {return description_;}
    void set_description(std::string description){description_=description;}
    VersionDescripter* support_versions(int32_t index) const {return support_versions_[index];}
    void add_support_versions(VersionDescripter* support_versions){support_versions_.push_back(support_versions);}
    int32_t support_versions_count() const {return (int32_t)support_versions_.size();}
    VersionDescripter* FindVersionDescripter(int8_t major_version, int8_t minor_version);
    void delete_version(int8_t major_version, int8_t minor_version);
    void delete_version(VersionDescripter* ptr_version);
  public:
    ProductDescripter& operator= (const ProductDescripter& rhs);

  private:
    int16_t product_id_;
    std::string product_name_;
    std::string supplier_;
    std::string description_;
    std::vector<VersionDescripter*> support_versions_;
  };
}

#endif

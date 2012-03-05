#ifndef PRODUCT_TARGET_H
#define PRODUCT_TARGET_H

#include "dest_product.h"

namespace wmdf
{
  class ProductTarget:public Persistable
  {
  public:
    ProductTarget(void);
    virtual ~ProductTarget(void);

    virtual bool SaveState( Memento* m );
    virtual bool LoadState( Memento* m );

    std::string dest_ip() const {return dest_ip_;}
    void set_dest_ip(std::string dest_ip){dest_ip_=dest_ip;}
    int32_t dest_port() const {return dest_port_;}
    void set_dest_port(int32_t dest_port){dest_port_=dest_port;}
    
    DestProduct* products(int32_t index){return products_[index];}
    int32_t products_count(){return (int32_t)products_.size();}
  private:
    std::string dest_ip_;
    int32_t dest_port_;
    std::vector<DestProduct*> products_;
  };
}

#endif

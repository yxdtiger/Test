#ifndef DEST_PRODUCT_H
#define DEST_PRODUCT_H

#include "persistable.h"
#include "memento.h"
#include "../../include/wmdf_api.h"

namespace wmdf
{
  class DestProduct:public Persistable
  {
  public:
    DestProduct(void);
    virtual ~DestProduct(void);

    virtual bool SaveState( Memento* m );
    virtual bool LoadState( Memento* m );

    int16_t product_id() const{return product_id_;}
    void set_product_id(int16_t product_id){product_id_=product_id;}

  private:
    int16_t product_id_;
  };
}

#endif

#ifndef PRODUCT_TARGET_H
#define PRODUCT_TARGET_H

#include "persistable.h"
#include "memento.h"
#include "../../include/wmdf_api.h"

namespace wmdf
{
  class ProductTarget:public Persistable
  {
  public:
    ProductTarget(void);
    virtual ~ProductTarget(void);

    virtual bool SaveState( Memento* m );
    virtual bool LoadState( Memento* m );

    int16_t product_id() const{return product_id_;}
    void set_product_id(int16_t id){product_id_=id;}

    std::string dest_dir() const {return dest_dir_;}
    void set_dest_dir(std::string dir){dest_dir_=dir;}

    int32_t time_span() const {return time_span_;}
    void set_time_span(int32_t span){time_span_ = span;}

    int32_t start_time() const {return start_time_;}
    void set_start_time(int32_t time){start_time_ = time;}

    int32_t end_time() const {return end_time_;}
    void set_end_time(int32_t time){end_time_ = time;}
    
    
  private:
    int16_t product_id_;
    std::string dest_dir_;
    int32_t time_span_;
    int32_t start_time_;
    int32_t end_time_;
    
  };
}

#endif

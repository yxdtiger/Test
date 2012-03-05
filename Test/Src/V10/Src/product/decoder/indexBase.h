#ifndef INDEX_BASE_H_
#define INDEX_BASE_H_

#include "../../include/wmdf_api.h"

namespace wmdf
{
  enum IndexType
  {
	SIMPLE_INDEX=1,
	COMPLEX_INDEX=2,
	SPECIAL_INDEX=3
  };

  class IndexBase
  {
  public:
    IndexBase();
    virtual ~IndexBase();
    uint16_t get_index(){return index_;}
    void set_index(uint16_t index_temp){index_ = index_temp;}
	  IndexType indextype(){return index_type_;}
	  void set_indextype(IndexType index_type){index_type_=index_type;}
  protected:
	  IndexType index_type_;
  private:
    uint16_t index_;
  };
}

#endif
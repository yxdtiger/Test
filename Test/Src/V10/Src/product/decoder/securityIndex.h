#ifndef SIMPLE_INDEX_H
#define SIMPLE_INDEX_H

#include <string>
#include "indexBase.h"

namespace wmdf
{
  class SimpleIndex : public IndexBase
  {
  public:
    SimpleIndex();
    virtual ~SimpleIndex();
    int64_t value_;
  };
  
  class ComplexIndex : public IndexBase
  {
  public:
    ComplexIndex();
    virtual ~ComplexIndex();
    void* struct_;
	int32_t unit_count;
  };

  class SpecialIndex : public IndexBase
  {
  public:
    SpecialIndex();
    virtual ~SpecialIndex();
    std::string name_;
  };
}

#endif
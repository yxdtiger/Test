#include "securityIndex.h"


wmdf::SimpleIndex::SimpleIndex()
{
  index_type_=SIMPLE_INDEX;
}



wmdf::SimpleIndex::~SimpleIndex()
{
}

wmdf::ComplexIndex::ComplexIndex()
{
  index_type_=COMPLEX_INDEX;
  unit_count=0;
}

wmdf::ComplexIndex::~ComplexIndex()
{
  delete[] struct_;
  struct_=NULL;
}

wmdf::SpecialIndex::SpecialIndex()
{
  index_type_=SPECIAL_INDEX;
  name_="";
}


wmdf::SpecialIndex::~SpecialIndex()
{

}

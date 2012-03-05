#include "rawdata_adapter_factory.h"
#include "../sse_step/sse_step_rawdata_adapter.h"
#include "../szse_step/szse_step_rawdata_adapter.h"
#include "../sse_fast/sse_fast_rawdata_adapter.h"
#include "../szse_fast/szse_fast_rawdata_adapter.h"

#include "../../../product/product_source.h"
#include "../../../product/adapter/iproduct_adapter.h"
#include "../../../include/date_time.h"



wmdf::RawdataAdapterFactory::RawdataAdapterFactory()
{

}

wmdf::RawdataAdapterFactory::~RawdataAdapterFactory()
{

}



wmdf::IProductAdapter* wmdf::RawdataAdapterFactory::CreateSSERawdataAdapter( ProductSource* product_source,date_t date )
{
  int32_t ymd = DateTime::date2ymd(date);

  if(ymd >=20050101 && ymd <= 20081231)
    return new SSEStepRawDataAdapter(product_source);
  else if(ymd >= 20090101 && ymd <= 20111211)
    return new SSEFASTRawDataAdapter(product_source,20111211);
  else if(ymd >= 20111212 )
    return new SSEFASTRawDataAdapter(product_source,20111212);
  else 
		return NULL;
}

wmdf::IProductAdapter* wmdf::RawdataAdapterFactory::CreateSZSERawdataAdapter( ProductSource* product_source, date_t date )
{
  int32_t ymd = DateTime::date2ymd(date);

  if(ymd >=20050101 && ymd <= 20110914)
    return new SZSEStepRawDataAdapter(product_source);
  else if(ymd>= 20110915)
    return new SZSEFASTRawDataAdapter(product_source);
  else 
    return NULL;
}

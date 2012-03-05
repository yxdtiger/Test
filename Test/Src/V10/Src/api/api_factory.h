#ifndef API_FACTORY_H
#define API_FACTORY_H

#include "../include/wmdf_api.h"

namespace wmdf
{
  class ProductSource;
  class ProtocolDescription;


  class ApiFactory
  {
  public:
    ApiFactory(void);
    virtual ~ApiFactory(void);
    static ProductSource* CreateProductSource(OpenArgs* args);
    static ProductSource* CreateProductSource(OpenExArgs* args);
  private:
    static ProtocolDescription* CreateSubscribedProduct(int16_t product_id,int8_t major_version,int8_t minor_version, CodecType codec_type);
    static void CreateSubscribedProducts(ProductSource* ps,CodecType codec_type);
  };
}

#endif

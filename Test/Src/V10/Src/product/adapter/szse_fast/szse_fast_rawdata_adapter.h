#ifndef SZSE_FAST_RAWDATA_ADAPTER_H
#define SZSE_FAST_RAWDATA_ADAPTER_H

#include "../product_adapter.h"


namespace wmdf
{
  struct WindFastMessage;
  class SZSEFastTemplateDecoder;
  class SZSEFastRawDataConverter;

  class SZSEFASTRawDataAdapter : public ProductAdapter
  {
  public:
    SZSEFASTRawDataAdapter( ProductSource *product_source);
    ~SZSEFASTRawDataAdapter();
    virtual void ResetContext();
  protected:
    virtual uint32_t ParseMessage(const uint32_t length);
  private:
    void ReleaseFastMessage(WindFastMessage*& curr_msg_);
  private:
    SZSEFastTemplateDecoder* data_source_decoder_;
    SZSEFastRawDataConverter* szse_fast_converter_;
  };
}

#endif

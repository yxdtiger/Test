#ifndef SSE_FAST_RAWDATA_ADAPTER_H
#define SSE_FAST_RAWDATA_ADAPTER_H

#include "../product_adapter.h"


namespace wmdf
{
  struct WindFastMessage;
  class SSEFastTemplateDecoder;
  class SSEFastRawDataConverter;

  class SSEFASTRawDataAdapter : public ProductAdapter
  {
  public:
    SSEFASTRawDataAdapter( ProductSource *product_source,int32_t tradedate);
    ~SSEFASTRawDataAdapter();
  protected:
    virtual uint32_t ParseMessage(const uint32_t length);
    virtual void ResetContext();
  private:
    void ReleaseFastMessage(WindFastMessage*& curr_msg_);
  private:
    SSEFastTemplateDecoder* data_source_decoder_;
    SSEFastRawDataConverter* sse_fast_converter_;
  };
}

#endif

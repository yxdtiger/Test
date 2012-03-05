#ifndef SSE_STEP_RAWDATA_ADAPTER_H
#define SSE_STEP_RAWDATA_ADAPTER_H

#include "../product_adapter.h"

namespace wmdf
{
	class SSEStepRawDataConverter;
  struct WindStepMessage;
  class SSEStepTemplateDecoder;

	class SSEStepRawDataAdapter : public ProductAdapter
	{
	public:
		SSEStepRawDataAdapter(ProductSource *product_source);
		virtual ~SSEStepRawDataAdapter();
    virtual void ResetContext();
  protected:
    virtual uint32_t ParseMessage(const uint32_t length);
	private:
		virtual void ReleaseMessages(std::vector<WindStepMessage*>& msgs);

		void LaunchL2MarketSnapshot();
		void LaunchL2Transactions();
		void LaunchL2OrderQueue();
		void LaunchL2IndexData();

	private:
		SSEStepRawDataConverter *rawdata_converter_;
    SSEStepTemplateDecoder* data_source_decoder_;
	};
}



#endif

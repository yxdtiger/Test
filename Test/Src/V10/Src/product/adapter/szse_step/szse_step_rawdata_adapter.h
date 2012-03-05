#ifndef SZSE_STEP_RAWDATA_ADAPTER_H
#define SZSE_STEP_RAWDATA_ADAPTER_H

#include "../product_adapter.h"

namespace wmdf
{
	class SZSEStepRawDataConverter;
  struct WindStepMessage;
  class StepTemplateDecoder;

	class SZSEStepRawDataAdapter : public ProductAdapter
	{
	public:
		SZSEStepRawDataAdapter(ProductSource* product_source);
		virtual ~SZSEStepRawDataAdapter();
    virtual void ResetContext();
  protected:
    virtual uint32_t ParseMessage(const uint32_t length);
		virtual void ReleaseRawData(std::vector<WindStepMessage*>& msgs);

		virtual void LaunchL2MarketSnapshot();
		virtual void LaunchL2Transactions();
		virtual void LaunchL2OrderQueue();
		void LaunchL2IndexData();
		void LaunchL2Orders();
  private:

	private:
		SZSEStepRawDataConverter *rawdata_converter_;
    StepTemplateDecoder* data_source_decoder_;
	};
}


#endif

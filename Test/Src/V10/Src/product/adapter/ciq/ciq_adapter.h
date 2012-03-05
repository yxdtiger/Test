#ifndef CIQ_ADAPTER_H
#define CIQ_ADAPTER_H

#include "../iproduct_adapter.h"
#include "../../../include/wmdf_structs.h"
#include <vector>
#include <boost/thread.hpp>

namespace wmdf
{
	class MdfMessage;
	class CIQFile;
	struct CIQIndex;

	class CIQAdapter : public IProductAdapter
	{
	public:
		CIQAdapter(ProductSource *product_source);
		virtual ~CIQAdapter();
		virtual void Start();
		virtual void Stop();
	private:
		void Dispatch();
		void FetchRecords();
		void PackAndSend();
		IndexSnapshot *FetchARecord(CIQIndex *data);
		MdfMessage* CreateMDFMessage(int32_t index,int32_t count);
		void DestroyRecords();
		virtual bool is_connected();
	private:
		CIQFile *ciq_file_;
		boost::thread *work_thread_;
		std::vector<IndexSnapshot*> index_data_;
		const static int DEFAULT_RECORDS_PER_MESSAGE_ = 100;
		const static char *suffix_;
	};
}



#endif

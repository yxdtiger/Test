#ifndef WINDINDEX_ADAPTER_H
#define WINDINDEX_ADAPTER_H

#include "../iproduct_adapter.h"
#include "../../../include/wmdf_structs.h"
#include <vector>
#include <boost/thread.hpp>

namespace wmdf
{
	class WindIndexFile;
	class MdfMessage;
	struct WindIndex;

	class WindIndexAdapter : public IProductAdapter
	{
	public:
		WindIndexAdapter(ProductSource *product_source);
		virtual ~WindIndexAdapter();
		virtual void Start();
		virtual void Stop();
	private:
		void Dispatch();
		void FetchRecords();
		void PackAndSend();
		IndexSnapshot* FetchARecord(WindIndex *data);
		MdfMessage* CreateMDFMessage(int32_t index,int32_t count);
		void DestroyRecords();
		virtual bool is_connected();
	private:
		WindIndexFile *windindex_file_;
		boost::thread *work_thread_;
		std::vector<IndexSnapshot*> index_data_;
		const static int32_t DEFAULT_RECORDS_PER_MESSAGE_ = 100;
	};
}

#endif
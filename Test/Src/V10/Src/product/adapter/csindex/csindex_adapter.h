#ifndef CSINDEX_ADAPTER_H
#define CSINDEX_ADAPTER_H

#include "../iproduct_adapter.h"
#include <boost/thread.hpp>
#include <vector>
#include "../../../include/wmdf_structs.h"

namespace wmdf
{
	class CSIndexFile;
	class MdfMessage;
	class Splitter;

	class CSIndexAdapter : public IProductAdapter
	{
	public:
		CSIndexAdapter(ProductSource *product_source);
		virtual ~CSIndexAdapter();
		virtual void Start();
		virtual void Stop();
	private:
		void Dispatch();
		virtual bool is_connected();
		void FetchRecords();
		void PackAndSend();
		MdfMessage* CreateMDFMessage(int32_t index,int count);
		void DestroyRecords();
		IndexSnapshot* ProcessLine();
	private:
		CSIndexFile *csindex_file_;
		boost::thread *work_thread_;
		std::vector<IndexSnapshot*> index_data_;
		Splitter *splitter_;
		const static int DEFAULT_RECORDS_PER_MESSAGE = 100;
		const static char *suffix;
	};

}



#endif

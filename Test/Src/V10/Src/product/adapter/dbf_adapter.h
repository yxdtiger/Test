#ifndef DBF_ADAPTER_H
#define DBF_ADAPTER_H

#include <boost/thread.hpp>
#include <string>
#include "../../include/wmdf_api.h"
#include "../../protocol/mdf_message.h"
#include "iproduct_adapter.h"
#include "dbf.h"

namespace wmdf
{
  class DBFAdapter:public IProductAdapter
  {
  public:
    DBFAdapter(ProductSource* product_source);
    virtual ~DBFAdapter(void);    

    virtual bool CheckDbfFile();
    virtual void* FetchARecord() = 0;
    virtual wmdf::MdfMessage* CreateMDFMessage(int index,int count) = 0;

    void Start();
    void Stop();
    void Dispatch();
    void FetchRecords();
    void PackAndSend();
    void DestroyRecords();
    virtual bool is_connected();    
  protected:
    bool Open();
    bool Close();

  protected:
    std::string adapter_name_;
    dbhead_t* dbh_;
    int max_fields_num_;
    int records_per_message_;
    std::vector<void*> records_;

    int time_span_;
    boost::thread* work_thread_;
	private:
		static const int DEFAULT_RECORDS_PER_MESSAGE = 100; 
  };
}

#endif

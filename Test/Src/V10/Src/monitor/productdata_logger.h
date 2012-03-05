#ifndef PRODUCTDATA_LOGGER_H
#define PRODUCTDATA_LOGGER_H

#include <boost/thread/mutex.hpp>
#include <boost/thread.hpp>

#include <vector>
#include <map>
#include "../include/wmdf_api.h"


namespace wmdf
{
  class WMDFFile;
  class ProtocolDescription;

  class ProductRawData
  {
  public:
    ProductRawData(int16_t product_id,uint8_t* rawdata, int32_t length);
    ~ProductRawData();
    int16_t product_id(){return product_id_;}
    int32_t length(){return length_;}
    uint8_t* rawdata(){return rawdata_;}
  private:
    int16_t product_id_;
    int32_t length_;
    uint8_t* rawdata_;
  };

  class ProductDataLogger
  {
  public:
    ProductDataLogger(void);
    virtual ~ProductDataLogger(void);
    void Start();
    void Stop();
    void RegisterProduct(ProtocolDescription* protocol_description);
    void AddToBuffer(ProductRawData* raw_data);    
  private:
    void WriteBuffer();
    WMDFFile* FetchProductFile(int16_t product_id);
    void CloseAllFile();
  private:
    WMDFFile* mdf_file_;
    std::vector<ProductRawData*> buffer_;
    std::vector<ProductRawData*> tempbuff_;
    std::map<int16_t,WMDFFile*> product_files_;
    boost::mutex mutex_;
    boost::thread* work_thread_;
    bool working_;
    date_t date_;
    std::map<int16_t,ProtocolDescription*> protocol_descriptions_;
  };
}

#endif

#ifndef IPRODUCT_ADAPTER_H
#define IPRODUCT_ADAPTER_H

#include "../product_source.h"
#include "../../include/wmdf_api.h"
#include "../../session/feed_client.h"

namespace wmdf
{
  class ProductDataLogger;
  class MdfMessage;
  class RelayMdfMessage;
  class IProductAdapter:public IClientCallback
	{
	public:
		IProductAdapter(ProductSource* product_source);
		virtual ~IProductAdapter();
		virtual void Start() = 0;
		virtual void Stop() = 0;
    virtual void ResetContext();
    virtual void OnRecvMsg(Message* msg){};
    virtual void OnCloseCallback(EventMsg* args){};
    virtual void OnEventCallback(EventMsg* args){};
    virtual void PublishMdfMessage(MdfMessage* mdf_message);
    virtual void PublishMdfMessage(RelayMdfMessage* mdf_message);
    //每天开盘时清理
    virtual void Clear();      
    void StartLogger();
    void StopLogger();
    ProductSource* product_source(){return product_source_;}
		int16_t get_product_id(){return product_source_->product_id();}
    bool is_market_time(int32_t curr_time);
    virtual bool is_connected();
    uint64_t receive_size() {return receive_size_;}
    uint64_t pre_receive_size() {return pre_receive_size_;}
    void set_pre_receive_size(int64_t size) {pre_receive_size_ = size;}
    uint32_t receive_packet_count() {return receive_packet_count_;}
    time_stamp_t last_heart_beat_time() {return last_heart_beat_time_;}
    void set_last_heart_beat_time(time_stamp_t time){last_heart_beat_time_ = time;}
	protected:
    ProductDataLogger* logger_;
		bool is_working_;
		ProductSource* product_source_;
		uint32_t packet_id_;
    volatile uint64_t receive_size_;    //把收到的数据大小记录在此无需加锁
    volatile uint64_t pre_receive_size_;
    volatile uint32_t receive_packet_count_;
    volatile time_stamp_t last_heart_beat_time_;
    
	};
}

#endif

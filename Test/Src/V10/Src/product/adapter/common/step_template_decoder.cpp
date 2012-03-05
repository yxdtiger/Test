#include "step_template_decoder.h"
#include "../common/rawdata_object.h"
#include "../../../environment/strutils.h"
#include "../../../monitor/logger.h"

using namespace std;

wmdf::StepTemplateDecoder::StepTemplateDecoder()
{
  step_packet_count_ = 0;    //step 包的个数
  max_packet_size_ = 0;      //最大的包大小
  total_packet_size_ = 0;    //总的包大小
}


wmdf::StepTemplateDecoder::~StepTemplateDecoder()
{

}

int32_t wmdf::StepTemplateDecoder::DecodeRawData(uint8_t* data,int32_t len)
{
  msg_items_.clear();
  head_ = data;
  tail_ = data+len;
  buff_len_ = len;
  int32_t offset = 0;

  while (offset+exchange_msg_min_len_ < buff_len_  )
  {
    uint8_t* temp=StrUtils::Str3Find(head_+offset,(uint8_t*)"8=S",tail_);//head start tag
    if (NULL == temp)
    {
      return offset;
    }
    temp+=exchange_msg_head_len_;//指向“9=”之后
    char* body_len_ptr;  //指向“9=81 ”的空格
    int32_t body_len = strtol((char*)temp,&body_len_ptr,10);

    //判断一个完整的包长是否小于剩下的字节数
    int32_t full_packet_len = ((uint8_t*)body_len_ptr-(head_+offset)+1)+ body_len+ exchange_mag_tail_len_;
    if(full_packet_len >buff_len_-offset)
      return offset;

    //判断包结束标记 10=
    uint8_t* pack_end=StrUtils::Str3Find((uint8_t*)body_len_ptr+1+body_len,(uint8_t*)"10=",tail_);
    if (pack_end != (uint8_t*)body_len_ptr+1+body_len)
    {
      offset += 12; //跳过“8=STEP.1.0.0”
      continue;
    }

    offset += full_packet_len;
    //获取msgtype
    temp=(uint8_t*)body_len_ptr+4; //指向“ 35=”的3
    uint8_t* current_tid=temp;
    while(*temp!=1)temp++;//将ASCII ”1“转为0 为后面的字符串函数做准备
    *temp=0;


    //跳过 49=VDE56=VDR34=052=20061213-09:21:35347=UTF-8
    for(int count=0;count<5;temp++)
    {
      if(*temp==1)
        count++;
    }
    DecodeMessage(current_tid,temp,head_+offset);

    step_packet_count_++; 
    max_packet_size_ = (max_packet_size_ > full_packet_len) ? max_packet_size_ : full_packet_len; 
    total_packet_size_ += full_packet_len;      
  }
  
  return offset;


}

void wmdf::StepTemplateDecoder::ResetContext()
{
  Logger::instance()->WriteFormatLog(LL_INFO,
    "decoder summary: step_packet_count: %I64d,  max_packet_size: %I64d, total_size: %I64d \n",
    step_packet_count_,max_packet_size_,total_packet_size_);  
  step_packet_count_ = 0;    //step 包的个数
  max_packet_size_ = 0;      //最大的包大小
  total_packet_size_ = 0;    //总的包大小
}

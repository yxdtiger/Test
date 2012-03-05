#include "fast_template_decoder.h"
#include "../../../monitor/logger.h"

wmdf::FastTemplateDecoder::FastTemplateDecoder()
{
  exchange_msg_min_len_ = 0;  //市场最小包大小
  exchange_msg_head_len_ = 0;//消息头大小
  exchange_mag_tail_len_ = 0;//消息尾字节数

  step_packet_count_ = 0;    //step 包的个数
  fast_content_count_ = 0;   //包含fast包的个数
  max_packet_size_ = 0;      //最大的包大小
  total_packet_size_ = 0;    //总的包大小
}

wmdf::FastTemplateDecoder::~FastTemplateDecoder()
{

}


int32_t wmdf::FastTemplateDecoder::DecodeRawData( uint8_t* data,int32_t len )
{
  msg_items_.clear();
  head_ = data;
  tail_ = data+len;
  buff_len_ = len;
  int32_t offset_ = 0;

  while (offset_+exchange_msg_min_len_ < buff_len_  )
  {
    uint8_t* temp=StrUtils::Str3Find(head_+offset_,(uint8_t*)"8=S",tail_);//head start tag
    if (NULL == temp)
    {
      return offset_;
    }

    //做个保护防止找到96字段中的“8=STEP”
    if(temp!=head_+offset_)
    {
      uint8_t* check_str=temp+strlen("8=STEP.1.0.0 "); 
      uint8_t* tmp_check_str=StrUtils::Str2Find(check_str,(uint8_t*)"9=",tail_);
      if(check_str==tmp_check_str)//进一步确认
      {
        while(*check_str!=1)check_str++;//到下一key的位置
        check_str++;
        tmp_check_str=StrUtils::Str3Find(check_str,(uint8_t*)"35=",tail_);
        if(tmp_check_str!=check_str)
        {
          offset_ = temp - head_ + 12;
          continue;
        }
      }
      else
      {
        offset_ = temp - head_ + 12;
        continue;
      }
    }
    temp+=exchange_msg_head_len_;//指向“9=”之后
    char* body_len_ptr;  //指向“9=81 ”的空格
    int32_t body_len = strtol((char*)temp,&body_len_ptr,10);

    //判断一个完整的包长是否小于剩下的字节数
    int32_t full_packet_len = ((uint8_t*)body_len_ptr-(head_+offset_)+1)+ body_len+ exchange_mag_tail_len_;
    if(full_packet_len >buff_len_-offset_)
      return offset_;

    //判断包结束标记 10=
    uint8_t* pack_end=StrUtils::Str3Find((uint8_t*)body_len_ptr+1+body_len,(uint8_t*)"10=",tail_);
    if (pack_end != (uint8_t*)body_len_ptr+1+body_len)
    {
      offset_ += 12; //跳过“8=STEP.1.0.0”
      continue;
    }

    offset_ += full_packet_len;
    //获取msgtype
    temp=(uint8_t*)body_len_ptr+4; //指向“ 35=”的3
    uint8_t* current_tid=temp;
    while(*temp!=1)temp++;//将ASCII ”1“转为0 为后面的字符串函数做准备
    *temp=0;
    Decode96Field(current_tid,temp,head_+offset_);

    //统计包数及包大小    
    step_packet_count_++; 
    max_packet_size_ = (max_packet_size_ > full_packet_len) ? max_packet_size_ : full_packet_len; 
    total_packet_size_ += full_packet_len;      
  }
  return offset_;	
}

void wmdf::FastTemplateDecoder::ResetContext()
{
  Logger::instance()->WriteFormatLog(LL_INFO,
    "decoder summary: step_packet_count: %I64d, 96_field_count: %I64d, max_packet_size: %I64d, total_size: %I64d \n",
    step_packet_count_,fast_content_count_,max_packet_size_,total_packet_size_);  
  step_packet_count_ = 0;    //step 包的个数
  fast_content_count_ = 0;   //包含fast包的个数
  max_packet_size_ = 0;      //最大的包大小
  total_packet_size_ = 0;    //总的包大小
}

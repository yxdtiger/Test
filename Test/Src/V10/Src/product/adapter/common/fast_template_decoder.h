#ifndef FAST_TEMPLATE_DECODER_H
#define FAST_TEMPLATE_DECODER_H

#include "../../../include/wmdf_api.h"
#include "../../../environment/strutils.h"
#include <vector>
#include <cstring>
namespace wmdf
{
	struct WindFastMessage;

	class FastTemplateDecoder
	{
	public:
    FastTemplateDecoder();
    virtual ~FastTemplateDecoder();
    void ResetContext();
    int32_t DecodeRawData(uint8_t* data,int32_t len);
    std::vector<WindFastMessage*>& msg_items(){return msg_items_;}
  protected:
    virtual void Decode96Field(uint8_t* tid,uint8_t*& field96_start,uint8_t* field96_end) = 0;

    bool FindContext(uint8_t*& contex_start,uint8_t* context_end,int& field96_len)
    {
      //连续跳过4个标准头字段 49\56\34\52
      int count = 0;
      while(count < 4)
      {
        if(*contex_start==1)
          count++;
        contex_start++;
      }
      //10142/ 10172
      //进入95=msg_len
      contex_start=StrUtils::Str3Find(contex_start,(uint8_t*)"95=",context_end);
      if (NULL == contex_start)
      {
        field96_len = 0;
        return false;
      }
      //获得96内容的开始
      contex_start+=strlen("95=");//跳过95=
      char* field96_ptr;
      field96_len = strtol((char*)contex_start,&field96_ptr,10);
      contex_start=(uint8_t*)field96_ptr+1;
      contex_start+=strlen("96=");
      return true;
    }

	protected:
    uint8_t* head_;
    uint8_t* tail_;
    int32_t buff_len_;
    std::vector<WindFastMessage*> msg_items_;
    int32_t exchange_msg_min_len_;  //市场最小包大小
    int32_t exchange_msg_head_len_;//消息头大小
    int32_t exchange_mag_tail_len_;//消息尾字节数

    int64_t step_packet_count_;    //step 包的个数
    int64_t fast_content_count_;   //包含fast包的个数
    int64_t max_packet_size_;      //最大的包大小
    int64_t total_packet_size_;    //总的包大小
	};
}

#endif

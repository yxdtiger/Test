#include "sseszse_adapter.h"
#include "net_module.h"
#include "../../../include/wmdf_structs.h"
#include "../../../include/date_time.h"
#include "../../../protocol/mdf_message.h"
#include "../../../include/date_time.h"
#include "zlib.h"
#include "../../decoder/dataDecoder.h"
#include "../../decoder/securityContainer.h"
#include "../../decoder/windCodeCodec.h"
#include "../../../monitor/logger.h"

const unsigned int DEFAULT_SS_PROCESS_BUF_SIZE     = 1024*512;

wmdf::SSESZSEAdapter::SSESZSEAdapter( ProductSource* product_source):
ProductAdapter(product_source),data_decoder_(new DataDecoder())
{
  WindCodeCodec::InitWindCodeCodecLookUpTable();
  memset(buf_unzip,0,512*1024);
}

wmdf::SSESZSEAdapter::~SSESZSEAdapter()
{
  if (data_decoder_)
  {
    delete data_decoder_;
    data_decoder_=NULL;
  }
  
}

uint32_t wmdf::SSESZSEAdapter::ParseMessage( const uint32_t length )
{
  uint32_t pos = 0;
  uint32_t data_len = 0; //消息包中数据段的长度，不包含包头包尾
  uint32_t packet_len =0; //整个消息包的长度
  uint32_t end_pos=0;
  do 
  {
    //包头长5 suffix+datalen，如果数据长度小于包头则退出
    if (length < pos+5 ) 
    {
      break;
    }
    if (message_buffer_[pos]==PREFIX)
    {
      memcpy(&data_len, &message_buffer_[pos+1], sizeof(data_len));
      packet_len = data_len+6;
      if (pos+packet_len <= length) 
      {
        if (message_buffer_[pos+packet_len-1]==SUFFIX)
        {
          unsigned long buf_len = DEFAULT_SS_PROCESS_BUF_SIZE;
          if(!uncompress(buf_unzip,&buf_len,(uint8_t*)(message_buffer_+pos+5),data_len))
          {
            data_decoder_->DecodeOnePacket(buf_len,buf_unzip);   
          }
          else
          {
            Logger::instance()->WriteFormatLog(LL_ERROR,"failed to decompress buffer. data length: %d, decompressed length: %d", data_len,buf_len);
          }
          pos=pos+packet_len;
          end_pos = pos;
        }
        else
        {
          pos++;
        }
      }
      else
      {
        break;
      }
    }
    else
    {
      pos++;
    }
  } while (pos<length);
  
  PublishMessages();
  return end_pos; 
}

void wmdf::SSESZSEAdapter::PublishMessages()
{
  SecurityContainerManager* scm=data_decoder_->get_SecurityContainerManager();

  if(scm->l2marketsnapshot_count()!=0)
  {
    LaunchMarketSnapshotL2(scm);
  }

  if(scm->l2transaction_count()!=0)
  {
    LaunchL2Transactions(scm);
  }

  if(scm->l2orderqueue_count()!=0)
  {
    LaunchOrderQueueL2(scm);
  }

  if(scm->l2orders_count()!=0)
  {
    LaunchSZSEL2Orders(scm);
  }

  scm->reset();
}

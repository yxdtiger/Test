#include "wmdf_stream.h"
#include "wmdf_file.h"
#include "net_module.h"
#include <assert.h>
#include <stdlib.h>

wmdf::WMDFStream::WMDFStream():mdf_file_(new WMDFFile())
{
}

wmdf::WMDFStream::~WMDFStream()
{
  Close();
}

bool wmdf::WMDFStream::Open(const char* path,const char* mode )
{
  return mdf_file_->Open(path, mode);
}

void wmdf::WMDFStream::Close()
{
  mdf_file_->Close();
}

int wmdf::WMDFStream::Write(Message* msg )
{
  int32_t len=0;
  len+=mdf_file_->Write(msg->msg_len);
  len+=mdf_file_->Write(msg->STX);  
  len+=mdf_file_->Write(msg->local_time);
  len+=mdf_file_->Write(msg->product_id);
  len+=mdf_file_->Write(msg->message_id);
  len+=mdf_file_->Write(msg->placeholder,2);
  len+=mdf_file_->Write(msg->check_sum);
  len+=mdf_file_->Write(msg->data,msg->msg_len-MSG_HEADER_LEN);
  return len;
}









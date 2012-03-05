#include "mdf_message.h"
#include "../environment/utils.h"

wmdf::MdfMessage::MdfMessage()
{
  mdf_record_ = new MdfRecord();
  mdf_record_->product_id = PRODUCT_INVALID_ID;
  mdf_record_->message_id = MESSAGE_INVALID_ID;
  mdf_record_->major_version = VERSION_INVALID_ID;
  mdf_record_->minor_version = VERSION_INVALID_ID;
  mdf_record_->header = NULL;   
  mdf_record_->record_count = 0;
  mdf_record_->body = NULL;      
}

wmdf::MdfMessage::MdfMessage( MdfRecord* mdf_record )
{
  mdf_record_ = mdf_record;
}

wmdf::MdfMessage::~MdfMessage()
{
  if (NULL != mdf_record_) 
  {
    Utils::DestroyMDFRecord(mdf_record_);
  }
}


wmdf::RelayMdfMessage::RelayMdfMessage()
{

}

wmdf::RelayMdfMessage::~RelayMdfMessage()
{
  if (NULL != data)
  {
    delete[] data;
    data = NULL;
  }
}
#include "protocol_description.h"
#include "memento.h"
#include "../environment/utils.h"

wmdf::ProtocolDescription::ProtocolDescription( void )
{
  product_id_ = PRODUCT_INVALID_ID;
  major_version_ = VERSION_INVALID_ID;
  minor_version_ = VERSION_INVALID_ID;
  codec_type_ = CT_UNKNOW;
}


wmdf::ProtocolDescription::ProtocolDescription( int16_t productId,int8_t majorVersion,int8_t minorVersion,CodecType codecType )
{
  product_id_ = productId;
  major_version_ = majorVersion;
  minor_version_ = minorVersion;
  codec_type_ = codecType;
}

wmdf::ProtocolDescription::ProtocolDescription(ProtocolDescription* src)
{
  product_id_ = src->product_id();
  major_version_ = src->major_version();
  minor_version_ = src->minor_version();
  codec_type_ = src->codec_type();
}

wmdf::ProtocolDescription::~ProtocolDescription()
{

}

bool wmdf::ProtocolDescription::LoadState( Memento* m )
{
  product_id_=m->GetInteger("ProductId");
  major_version_=Utils::GetMajorVersion(product_id_);
  minor_version_=Utils::GetMinorVersion(product_id_);
  codec_type_ = (CodecType)m->GetInteger("Codec");
  return true;
}


bool wmdf::ProtocolDescription::SaveState( Memento* m )
{
  m->SetName("ProtocolDescription");
  m->SetInteger("ProductId", product_id_);
  m->SetInteger("Major", major_version_);
  m->SetInteger("Minor", minor_version_);
  m->SetInteger("Codec", (int32_t)codec_type_);
  return true;
}

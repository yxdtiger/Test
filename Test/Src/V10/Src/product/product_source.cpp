#include "product_source.h"
#include "memento.h"
#include "../protocol/protocol_description.h"

using namespace wmdf;

wmdf::MarketTime::MarketTime()
{

}

wmdf::MarketTime::~MarketTime()
{

}

bool wmdf::MarketTime::LoadState( Memento* m )
{
  begin_time_=m->GetInteger("BeginTime");
  end_time_=m->GetInteger("EndTime");
  return true;
}

bool wmdf::MarketTime::SaveState( Memento* m )
{
  m->SetInteger("BeginTime",begin_time_);
  m->SetInteger("EndTime",end_time_);
  return true;
}

wmdf::BackupSource::BackupSource()
{

}

wmdf::BackupSource::~BackupSource()
{

}

bool wmdf::BackupSource::LoadState( Memento* m )
{
  ip_address_=m->GetString("IPAddr");
  port_=m->GetInteger("Port");
  user_name_=m->GetString("UserName");
  pass_word_=m->GetString("Password");
  return true;
}

bool wmdf::BackupSource::SaveState( Memento* m )
{
  m->SetString("IPAddr",ip_address_);
  m->SetInteger("Port",port_);
  m->SetString("UserName",user_name_);
  m->SetString("Password",pass_word_);
  return true;
}

wmdf::ProductSource::ProductSource(void)
{
  product_id_ = 0;
  port_ = 0;
  retry_times_ = 3;
  timeout_ = 3000;
  which_is_in_use_ = -1;
}

wmdf::ProductSource::~ProductSource(void)
{
  for(uint32_t i=0;i<subscribed_products_.size();i++)
  {
    delete subscribed_products_[i];
  }
  subscribed_products_.clear();
  for(uint32_t i=0;i<market_times_.size();i++)
  {
    delete market_times_[i];
  }
  market_times_.clear();
  for(uint32_t i=0;i<backup_sources_.size();i++)
  {
    delete backup_sources_[i];
  }
  backup_sources_.clear();
}

bool wmdf::ProductSource::LoadState( Memento* m )
{
  enabled_ = m->GetBoolean("Enabled");
  product_id_=m->GetInteger("ProductId");
  ip_address_=m->GetString("IPAddr");
  port_=m->GetInteger("Port");
  user_name_=m->GetString("UserName");
  pass_word_=m->GetString("Password");
  if (m->HasAttributes("RetryTimes"))
  {
    retry_times_ = m->GetInteger("RetryTimes");
  }
  if (m->HasAttributes("Timeout"))
  {
    timeout_ = m->GetInteger("Timeout");
  }
  codec_type_ = (CodecType)m->GetInteger("CodecType");
  description_=m->GetString("Description");
  for(int32_t i=0;i<m->GetChildCount();i++)
  {
    Memento* child_memento=m->GetChild(i);
    if (child_memento->GetName() == "ProtocolDescription")
    {
      ProtocolDescription* product=new ProtocolDescription();
      product->LoadState(child_memento);
      subscribed_products_.push_back(product);
    }
    else if (child_memento->GetName() == "MarketTime")
    {
      MarketTime* market_time = new MarketTime();
      market_time->LoadState(child_memento);
      market_times_.push_back(market_time);
    }
    else if (child_memento->GetName() == "Backup")
    {
      BackupSource* source = new BackupSource();
      source->LoadState(child_memento);
      backup_sources_.push_back(source);
    }
  }
  return true;
}

bool wmdf::ProductSource::SaveState( Memento* m )
{
  m->SetName("ProductSource");
  m->SetBoolean("Enabled",enabled_);
  m->SetInteger("ProductId",product_id_);
  m->SetString("IPAddr",ip_address_);
  m->SetInteger("Port",port_);
  m->SetString("UserName",user_name_);
  m->SetString("Password",pass_word_);
  m->SetInteger("CodecType", (int32_t)codec_type_);
  m->SetInteger("RetryTimes",retry_times_);
  m->SetInteger("Timeout",timeout_);
  m->SetString("Description",description_);
  for(uint32_t i=0;i<subscribed_products_.size();i++)
  {
    Memento* child_memento=m->CreateChild("SubscribedProduct");
    subscribed_products_[i]->SaveState(child_memento);
  }
  for(uint32_t i=0;i<market_times_.size();i++)
  {
    Memento* child_memento=m->CreateChild("MarketTime");
    market_times_[i]->SaveState(child_memento);
  }
  for(uint32_t i=0;i<backup_sources_.size();i++)
  {
    Memento* child_memento=m->CreateChild("Backup");
    backup_sources_[i]->SaveState(child_memento);
  }

  return true;
}

wmdf::ProductSource& wmdf::ProductSource::operator= (const wmdf::ProductSource& rhs)
{
  product_id_ = rhs.product_id();
  ip_address_ = rhs.ip_address();
  port_ = rhs.port();
  description_ = rhs.description();
  return *this;
}

ProtocolDescription* wmdf::ProductSource::FindSubscribedProduct( int16_t product_id )
{
  std::vector<ProtocolDescription*>::iterator iter = subscribed_products_.begin();
  while (iter != subscribed_products_.end())
  {
    if ((*iter)->product_id() == product_id)
    {
      return *iter;
    }
    iter++;
  }
  return NULL;
}

bool wmdf::ProductSource::IsSubscribedProduct( int16_t product_id )
{
  return (FindSubscribedProduct(product_id) != NULL);
}

std::string wmdf::ProductSource::in_use_ip_address()
{
  if(which_is_in_use_==0)
    return ip_address_;
  else if(which_is_in_use_>0)
    return backup_sources(which_is_in_use_-1)->ip_address();
  else return "<null ip>";
}

int32_t wmdf::ProductSource::in_use_port()
{
  if(which_is_in_use_==0)
    return port_;
  else if(which_is_in_use_>0)
    return backup_sources(which_is_in_use_-1)->port();
  else return WMDF_CONSTS_32BIT_NIL_VALUE;
}

std::string wmdf::ProductSource::in_use_user_name()
{
  if(which_is_in_use_==0)
    return user_name_;
  else if(which_is_in_use_>0)
    return backup_sources(which_is_in_use_-1)->user_name();
  else return "<null user name>";
}

std::string wmdf::ProductSource::in_use_pass_word()
{
  if(which_is_in_use_==0)
    return pass_word_;
  else if(which_is_in_use_>0)
    return backup_sources(which_is_in_use_-1)->pass_word();
  else return "<null password>";
}

void ProductSource::delete_market_time(int32_t begin_time, int32_t end_time)
{
  for(vector<MarketTime*>::iterator it = market_times_.begin(); it != market_times_.end(); ++it)
  {
    if( (*it)->begin_time() == begin_time && (*it)->end_time() == end_time )
    {
      delete (*it);
      market_times_.erase(it);
      break;
    }
  }
}

void ProductSource::delete_market_time(MarketTime* ptr_market_time)
{
  for(vector<MarketTime*>::iterator it = market_times_.begin(); it != market_times_.end(); ++it)
  {
    if( *it == ptr_market_time )
    {
      delete (*it);
      market_times_.erase(it);
      break;
    }
  }
}

void ProductSource::delete_backup_source(std::string ip_addr, int32_t port)
{
  for(vector<BackupSource*>::iterator it = backup_sources_.begin(); it != backup_sources_.end(); ++it)
  {
    if( (*it)->ip_address() == ip_addr && (*it)->port() == port )
    {
      delete (*it);
      backup_sources_.erase(it);
      break;
    }
  }
}

void ProductSource::delete_backup_source(BackupSource* ptr_backup_source)
{
  for(vector<BackupSource*>::iterator it = backup_sources_.begin(); it != backup_sources_.end(); ++it)
  {
    if( *it == ptr_backup_source )
    {
      delete (*it);
      backup_sources_.erase(it);
      break;
    }
  }
}

void ProductSource::delete_subscribed_product(int32_t product_id, int8_t major_version, int8_t minor_version, CodecType codec)
{
  for(vector<ProtocolDescription*>::iterator it = subscribed_products_.begin(); it != subscribed_products_.end(); ++it)
  {
    if( (*it)->product_id() == product_id && (*it)->major_version() == major_version
      && (*it)->minor_version() == minor_version && (*it)->codec_type() == codec)
    {
      delete (*it);
      subscribed_products_.erase(it);
      break;
    }
  }
}

void ProductSource::delete_subscribed_product(ProtocolDescription* ptr_subscribed_product)
{
  for(vector<ProtocolDescription*>::iterator it = subscribed_products_.begin(); it != subscribed_products_.end(); ++it)
  {
    if( *it == ptr_subscribed_product )
    {
      delete (*it);
      subscribed_products_.erase(it);
      break;
    }
  }
}

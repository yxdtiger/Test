#include "api_factory.h"
#include "../product/product_source.h"
#include "../protocol/protocol_description.h"
#include <string>

wmdf::ApiFactory::ApiFactory(void)
{
}

wmdf::ApiFactory::~ApiFactory(void)
{
}

wmdf::ProductSource* wmdf::ApiFactory::CreateProductSource( OpenArgs* args )
{
  ProductSource* ps = new ProductSource();
  ps->set_product_id(PRODUCT_RELAY_BEGIN_ID);
  std::string addr((char*)args->server_addr);
  ps->set_ip_address(addr);
  ps->set_port(args->server_port);
  std::string username((char*)args->user_name);
  ps->set_user_name(username);
  std::string password((char*)args->password);
  ps->set_pass_word(password);
  MarketTime* mt = new MarketTime();
  mt->set_begin_time(args->marktet_open_time);
  mt->set_end_time(args->markt_close_time);
  ps->add_market_time(mt);
  CreateSubscribedProducts(ps,args->codec_type);
  return ps;
}

wmdf::ProductSource* wmdf::ApiFactory::CreateProductSource( OpenExArgs* args )
{
  ProductSource* ps = new ProductSource();
  ps->set_product_id(PRODUCT_RELAY_BEGIN_ID);
  std::string addr((char*)args->server_addr);
  ps->set_ip_address(addr);
  ps->set_port(args->server_port);
  std::string username((char*)args->user_name);
  ps->set_user_name(username);
  std::string password((char*)args->password);
  ps->set_pass_word(password);
  MarketTime* mt = new MarketTime();
  mt->set_begin_time(args->marktet_open_time);
  mt->set_end_time(args->markt_close_time);
  ps->add_market_time(mt);
  ps->set_retry_times(args->retry_times);
  ps->set_timeout(args->timeout);
  for (int32_t index=0;index<args->backup_source_count;index++)
  {
    BackupSource* bs = new BackupSource();
    std::string server_addr((char*)args->backup_sources[index].server_addr);
    bs->set_ip_address(server_addr);
    bs->set_port(args->backup_sources[index].server_port);
    std::string user_name((char*)args->backup_sources[index].user_name);
    bs->set_user_name(user_name);
    std::string pass_word((char*)args->backup_sources[index].password);
    bs->set_pass_word(pass_word);
    ps->add_backup_source(bs);
  }
  CreateSubscribedProducts(ps,args->codec_type);
  return ps;
}
wmdf::ProtocolDescription* wmdf::ApiFactory::CreateSubscribedProduct( int16_t product_id,int8_t major_version,int8_t minor_version, CodecType codec_type )
{
  ProtocolDescription* sp = new ProtocolDescription();
  sp->set_product_id(product_id);
  sp->set_major_version(major_version);
  sp->set_minor_version(minor_version);
  sp->set_codec_type(codec_type);
  return sp;
}

void wmdf::ApiFactory::CreateSubscribedProducts( ProductSource* ps,CodecType codec_type )
{
  ProtocolDescription* sp = CreateSubscribedProduct(PRODEUCT_SYSTEM_ID,MAJOR_VERSION_SYSTEM_ID,MINOR_VERSION_SYSTEM_ID,CT_BINARY);
  ps->add_subscribed_product(sp);
  sp = CreateSubscribedProduct(PRODEUCT_MONITOR_ID,MAJOR_VERSION_MONITOR_ID,MINOR_VERSION_MONITOR_ID,CT_BINARY);
  ps->add_subscribed_product(sp);
  sp = CreateSubscribedProduct(PRODUCT_SSE_LEVEL1_ID,MAJOR_VERSION_SSE_LEVEL1_ID,MINOR_VERSION_SSE_LEVEL1_ID,codec_type);
  ps->add_subscribed_product(sp);
  sp = CreateSubscribedProduct(PRODUCT_SZSE_LEVEL1_ID,MAJOR_VERSION_SZSE_LEVEL1_ID,MINOR_VERSION_SZSE_LEVEL1_ID,codec_type);
  ps->add_subscribed_product(sp);
  sp = CreateSubscribedProduct(PRODUCT_SSE_LEVEL2_ID,MAJOR_VERSION_SSE_LEVEL2_ID,MINOR_VERSION_SSE_LEVEL2_ID,codec_type);
  ps->add_subscribed_product(sp);
  sp = CreateSubscribedProduct(PRODUCT_SZSE_LEVEL2_ID,MAJOR_VERSION_SZSE_LEVEL2_ID,MINOR_VERSION_SZSE_LEVEL2_ID,codec_type);
  ps->add_subscribed_product(sp);
  sp = CreateSubscribedProduct(PRODUCT_CFFE_LEVEL1_ID,MAJOR_VERSION_CFFE_LEVEL1_ID,MINOR_VERSION_CFFE_LEVEL1_ID,codec_type);
  ps->add_subscribed_product(sp);

  sp = CreateSubscribedProduct(PRODUCT_CFFE_LEVEL2_ID,MAJOR_VERSION_CFFE_LEVEL2_ID,MINOR_VERSION_CFFE_LEVEL2_ID,codec_type);
  ps->add_subscribed_product(sp);
  sp = CreateSubscribedProduct(PRODUCT_SHFE_LEVEL1_ID,MAJOR_VERSION_SHFE_LEVEL1_ID,MINOR_VERSION_SHFE_LEVEL1_ID,codec_type);
  ps->add_subscribed_product(sp);
  sp = CreateSubscribedProduct(PRODUCT_DCE_LEVEL1_ID,MAJOR_VERSION_DCE_LEVEL1_ID,MINOR_VERSION_DCE_LEVEL1_ID,codec_type);
  ps->add_subscribed_product(sp);
  sp = CreateSubscribedProduct(PRODUCT_CZCE_LEVEL1_ID,MAJOR_VERSION_CZCE_LEVEL1_ID,MINOR_VERSION_CZCE_LEVEL1_ID,codec_type);
  ps->add_subscribed_product(sp);
  sp = CreateSubscribedProduct(PRODUCT_FTSE_LEVEL1_ID,MAJOR_VERSION_FTSE_ID,MINOR_VERSION_FTSE_ID,codec_type);
  ps->add_subscribed_product(sp);
	sp = CreateSubscribedProduct(PRODUCT_CIQINDEX_LEVEL1_ID,MAJOR_VERSION_CIQINDEX_ID,MINOR_VERSION_CIQINDEX_ID,codec_type);
	ps->add_subscribed_product(sp);
	sp = CreateSubscribedProduct(PRODUCT_CSINDEX_LEVEL1_ID,MAJOR_VERSION_CSINDEX_ID,MINOR_VERSION_CSINDEX_ID,codec_type);
	ps->add_subscribed_product(sp);
	sp = CreateSubscribedProduct(PRODUCT_WINDINDEX_LEVEL1_ID,MAJOR_VERSION_WINDINDEX_ID,MINOR_VERSION_WINDINDEX_ID,codec_type);
	ps->add_subscribed_product(sp);
}
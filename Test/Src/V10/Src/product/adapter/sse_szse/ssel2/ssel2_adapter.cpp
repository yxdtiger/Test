#include "ssel2_adapter.h"
#include <map>
#include "zlib.h"
#include "net_module.h"
#include "../../../../include/wmdf_structs.h"
#include "../../../../include/date_time.h"
#include "../../../../protocol/mdf_message.h"
#include "../../../decoder/dataDecoder.h"
#include "../../../decoder/securityContainer.h"
#include "../../../decoder/windCodeCodec.h"
#include "../../../../environment/utils.h"
//#include "market_snapshot_container.h"


//std::ofstream sse_os("test_ssel2");

using namespace wmdf;
using namespace std;

SSEL2Adapter::SSEL2Adapter(ProductSource* product_source)
:SSESZSEAdapter(product_source)
{
//  market_snapshot_container_ = new MarketSnapshotContainer();
}

SSEL2Adapter::~SSEL2Adapter()
{
//  if (NULL != market_snapshot_container_)
//  {
//    delete market_snapshot_container_;
//    market_snapshot_container_ = NULL;
//  }
}

void wmdf::SSEL2Adapter::LaunchMarketSnapshotL2( SecurityContainerManager* scm )
{
  MdfMessage* mdf_msg = new MdfMessage();
  mdf_msg->set_local_time(DateTime::get_current_time());
  mdf_msg->set_socket_id(0);
  mdf_msg->set_product_id(PRODUCT_SSE_LEVEL2_ID);
  mdf_msg->set_message_id(SSEL2_MARKETDATA_ID);
  mdf_msg->set_major_version(MAJOR_VERSION_SSE_LEVEL2_ID);
  mdf_msg->set_minor_version(MINOR_VERSION_SSE_LEVEL2_ID);
  mdf_msg->set_record_count(scm->l2marketsnapshot_count());

  MarketMessageHeader* header_of_snapshot = new MarketMessageHeader();
  header_of_snapshot->place_holder = 0;
	mdf_msg->set_packet_num(++packet_id_);
  mdf_msg->set_header(header_of_snapshot);
  L2MarketSnapshot* msg_body = scm->TranseferToSSEL2MarketSnapshot();
  mdf_msg->set_body(msg_body);
  
//  for (int32_t index=0;index<scm->l2marketsnapshot_count();index++)
//  {
//    market_snapshot_container_->MergeMarketSnapshot(&msg_body[index]);
//  }

  //Utils::PrintMDFMessageData(mdf_msg,sse_os);	
  PublishMdfMessage(mdf_msg);
}

void wmdf::SSEL2Adapter::LaunchL2Transactions( SecurityContainerManager* scm )
{
  MdfMessage* mdf_msg = new MdfMessage();
  mdf_msg->set_local_time(DateTime::get_current_time());
  mdf_msg->set_socket_id(0);
  mdf_msg->set_product_id(PRODUCT_SSE_LEVEL2_ID);
  mdf_msg->set_message_id(SSEL2_TRANSACTION_ID);
  mdf_msg->set_major_version(MAJOR_VERSION_SSE_LEVEL2_ID);
  mdf_msg->set_minor_version(MINOR_VERSION_SSE_LEVEL2_ID);
  mdf_msg->set_record_count(scm->l2transaction_count());

  int32_t size_of_transaction=sizeof(L2Transaction);

  MarketMessageHeader* header_of_transaction = new MarketMessageHeader();
  header_of_transaction->place_holder = 0;
	mdf_msg->set_packet_num(++packet_id_);
  mdf_msg->set_header(header_of_transaction);
  mdf_msg->set_body(scm->TransferToSSEL2Transactions());

	//Utils::PrintMDFMessageData(mdf_msg,sse_os);
  PublishMdfMessage(mdf_msg);
}

void wmdf::SSEL2Adapter::LaunchOrderQueueL2( SecurityContainerManager* scm)
{
  MdfMessage* mdf_msg = new MdfMessage();
  mdf_msg->set_local_time(DateTime::get_current_time());
  mdf_msg->set_socket_id(0);
  mdf_msg->set_product_id(PRODUCT_SSE_LEVEL2_ID);
  mdf_msg->set_message_id(SSEL2_ORDERQUEUE_ID);
  mdf_msg->set_major_version(MAJOR_VERSION_SSE_LEVEL2_ID);
  mdf_msg->set_minor_version(MINOR_VERSION_SSE_LEVEL2_ID);
  mdf_msg->set_record_count(scm->l2orderqueue_count());

  int32_t size_of_orderqueue=sizeof(L2OrderQueue);

  MarketMessageHeader* header_of_orderqueue = new MarketMessageHeader();
  header_of_orderqueue->place_holder = 0;
	mdf_msg->set_packet_num(++packet_id_);
  mdf_msg->set_header(header_of_orderqueue);
  mdf_msg->set_body(scm->TransferToSSEL2OrderQueue());

	//Utils::PrintMDFMessageData(mdf_msg,sse_os);
  PublishMdfMessage(mdf_msg);
}

void wmdf::SSEL2Adapter::LaunchSZSEL2Orders( SecurityContainerManager* scm)
{
  MdfMessage* mdf_msg = new MdfMessage();
  mdf_msg->set_local_time(DateTime::get_current_time());
  mdf_msg->set_socket_id(0);
  mdf_msg->set_product_id(PRODUCT_SZSE_LEVEL2_ID);
  mdf_msg->set_message_id(SZSEL2_SINGLEORDER_ID);
  mdf_msg->set_major_version(MAJOR_VERSION_SZSE_LEVEL2_ID);
  mdf_msg->set_minor_version(MINOR_VERSION_SZSE_LEVEL2_ID);
  mdf_msg->set_record_count(scm->l2orders_count());

  MarketMessageHeader* header_of_orders = new MarketMessageHeader();
  header_of_orders->place_holder = 0;
	mdf_msg->set_packet_num(++packet_id_);
  mdf_msg->set_header(header_of_orders);
  mdf_msg->set_body(scm->TransferToSZSEL2Orders());

	//Utils::PrintMDFMessageData(mdf_msg,sse_os);
  PublishMdfMessage(mdf_msg);
}
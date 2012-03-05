#include "szsel2_adapter.h"
#include "net_module.h"
#include "../../../../include/wmdf_structs.h"
#include "../../../../include/date_time.h"
#include "../../../../protocol/mdf_message.h"
#include "../../../../include/date_time.h"
#include "zlib.h"
#include "../../../decoder/dataDecoder.h"
#include "../../../decoder/securityContainer.h"
#include "../../../decoder/windCodeCodec.h"
#include "../../../../environment/utils.h"
//#include <fstream>
#include <map>


using namespace wmdf;
using namespace std;

SZSEL2Adapter::SZSEL2Adapter(ProductSource* product_source)
:SSESZSEAdapter(product_source)
{
}


SZSEL2Adapter::~SZSEL2Adapter()
{
}




void wmdf::SZSEL2Adapter::LaunchMarketSnapshotL2( SecurityContainerManager* scm )
{
  MdfMessage* mdf_msg = new MdfMessage();
  mdf_msg->set_local_time(DateTime::get_current_time());
  mdf_msg->set_socket_id(0);
  mdf_msg->set_product_id(PRODUCT_SZSE_LEVEL2_ID);
  mdf_msg->set_message_id(SZSEL2_MARKETDATA_ID);
  mdf_msg->set_major_version(MAJOR_VERSION_SZSE_LEVEL2_ID);
  mdf_msg->set_minor_version(MINOR_VERSION_SZSE_LEVEL2_ID);
  mdf_msg->set_record_count(scm->l2marketsnapshot_count());


  MarketMessageHeader* header_of_snapshot = new MarketMessageHeader();
  header_of_snapshot->place_holder = 0;
	mdf_msg->set_packet_num(++packet_id_);
  mdf_msg->set_header(header_of_snapshot);
  mdf_msg->set_body(scm->TransferToSZSEL2MarketSnapshot());

  //print
	//static std::ofstream os_szsel2("test_szsel2.txt");
  //Utils::PrintMDFMessageData(mdf_msg,os_szsel2);

  PublishMdfMessage(mdf_msg);
}

void wmdf::SZSEL2Adapter::LaunchL2Transactions( SecurityContainerManager* scm )
{
  MdfMessage* mdf_msg = new MdfMessage();
  mdf_msg->set_local_time(DateTime::get_current_time());
  mdf_msg->set_socket_id(0);
  mdf_msg->set_product_id(PRODUCT_SZSE_LEVEL2_ID);
  mdf_msg->set_message_id(SZSEL2_TRANSACTION_ID);
  mdf_msg->set_major_version(MAJOR_VERSION_SZSE_LEVEL2_ID);
  mdf_msg->set_minor_version(MINOR_VERSION_SZSE_LEVEL2_ID);
  mdf_msg->set_record_count(scm->l2transaction_count());

  MarketMessageHeader* header_of_transaction = new MarketMessageHeader();
  header_of_transaction->place_holder = 0;
	mdf_msg->set_packet_num(++packet_id_);
  mdf_msg->set_header(header_of_transaction);
  mdf_msg->set_body(scm->TransferToSZSEL2Transactions());

  //print
  //Utils::PrintMDFMessageData(mdf_msg,os_szsel2);

  PublishMdfMessage(mdf_msg);
}

void wmdf::SZSEL2Adapter::LaunchOrderQueueL2( SecurityContainerManager* scm)
{
  
  MdfMessage* mdf_msg = new MdfMessage();
  mdf_msg->set_local_time(DateTime::get_current_time());
  mdf_msg->set_socket_id(0);
  mdf_msg->set_product_id(PRODUCT_SZSE_LEVEL2_ID);
  mdf_msg->set_message_id(SZSEL2_ORDERQUEUE_ID);
  mdf_msg->set_major_version(MAJOR_VERSION_SZSE_LEVEL2_ID);
  mdf_msg->set_minor_version(MINOR_VERSION_SZSE_LEVEL2_ID);
  mdf_msg->set_record_count(scm->l2orderqueue_count());

  MarketMessageHeader* header_of_orderqueue = new MarketMessageHeader();
  header_of_orderqueue->place_holder = 0;
	mdf_msg->set_packet_num(++packet_id_);
  mdf_msg->set_header(header_of_orderqueue);
  mdf_msg->set_body(scm->TransferToSZSEL2OrderQueue());
  
  //print
  //Utils::PrintMDFMessageData(mdf_msg,os_szsel2);
  PublishMdfMessage(mdf_msg);

}

void wmdf::SZSEL2Adapter::LaunchSZSEL2Orders( SecurityContainerManager* scm)
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

  //print
  //Utils::PrintMDFMessageData(mdf_msg,os_szsel2);

  PublishMdfMessage(mdf_msg);
}



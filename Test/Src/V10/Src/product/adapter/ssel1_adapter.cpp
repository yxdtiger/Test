#include "ssel1_adapter.h"
#include "net_module.h"
#include "../../include/wmdf_structs.h"
#include "../../include/date_time.h"
#include "../../protocol/mdf_message.h"
#include "zlib.h"
#include "zconf.h"
#include "../decoder/dataDecoder.h"
#include "../decoder/securityContainer.h"
#include "../decoder/windCodeCodec.h"
#include <map>
#include "../../environment/utils.h"
#include <fstream>

//ofstream os_ssel1("ssel1_after_transfer.txt");

using namespace wmdf;
using namespace std;


Ssel1Adapter::Ssel1Adapter(ProductSource* product_source)
:SseSzseAdapter(product_source)
{
}


Ssel1Adapter::~Ssel1Adapter()
{
}


void wmdf::Ssel1Adapter::LaunchMarketSnapshotL2( SecurityContainerManager* scm )
{
  MdfMessage* mdf_msg = new MdfMessage();
  mdf_msg->set_local_time(DateTime::get_current_time());
  mdf_msg->set_socket_id(0);
  mdf_msg->set_product_id(PRODUCT_SSE_LEVEL1_ID);
  mdf_msg->set_message_id(SSEL1_MARKETDATA_ID);
  mdf_msg->set_major_version(MAJOR_VERSION_SSE_LEVEL1_ID);
  mdf_msg->set_minor_version(MINOR_VERSION_SSE_LEVEL1_ID);
  mdf_msg->set_record_count(scm->l2marketsnapshot_count());


  MarketMessageHeader* header_of_snapshot = new MarketMessageHeader();
  header_of_snapshot->place_holder = 0;
	mdf_msg->set_packet_num(++packet_id_);
  mdf_msg->set_header(header_of_snapshot);
  mdf_msg->set_body(scm->TranseferToSZSEL1MarketSnapshot());

	//Utils::PrintMDFMessageData(mdf_msg,os_ssel1);
  PublishMdfMessage(mdf_msg);
}



void wmdf::Ssel1Adapter::LaunchL2Transactions( SecurityContainerManager* scm )
{
  //no transaction
}

void wmdf::Ssel1Adapter::LaunchOrderQueueL2( SecurityContainerManager* scm)
{
// no orderqueue
}

void wmdf::Ssel1Adapter::LaunchSZSEL2Orders( SecurityContainerManager* scm)
{
// no orders;
}



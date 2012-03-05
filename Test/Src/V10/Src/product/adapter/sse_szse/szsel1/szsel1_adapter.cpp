#include "szsel1_adapter.h"
#include "net_module.h"
#include "../../../../include/wmdf_structs.h"
#include "../../../../include/date_time.h"
#include "../../../../protocol/mdf_message.h"
#include "zlib.h"
#include "zconf.h"
#include "../../../decoder/dataDecoder.h"
#include "../../../decoder/securityContainer.h"
#include "../../../decoder/windCodeCodec.h"
#include <map>

using namespace wmdf;
using namespace std;


SZSEL1Adapter::SZSEL1Adapter(ProductSource* product_source)
:SSESZSEAdapter(product_source)
{
}


SZSEL1Adapter::~SZSEL1Adapter()
{
}



void wmdf::SZSEL1Adapter::LaunchMarketSnapshotL2( SecurityContainerManager* scm )
{
  MdfMessage* mdf_msg = new MdfMessage();
  mdf_msg->set_local_time(DateTime::get_current_time());
  mdf_msg->set_socket_id(0);
  mdf_msg->set_product_id(PRODUCT_SZSE_LEVEL1_ID);
  mdf_msg->set_message_id(SZSEL1_MARKETDATA_ID);
  mdf_msg->set_major_version(MAJOR_VERSION_SZSE_LEVEL1_ID);
  mdf_msg->set_minor_version(MINOR_VERSION_SZSE_LEVEL1_ID);
  mdf_msg->set_record_count(scm->l2marketsnapshot_count());


  MarketMessageHeader* header_of_snapshot = new MarketMessageHeader();
  header_of_snapshot->place_holder = 0;
	mdf_msg->set_packet_num(++packet_id_);
  mdf_msg->set_header(header_of_snapshot);
  mdf_msg->set_body(scm->TransferToSZSEL1MarketSnapshot());

  PublishMdfMessage(mdf_msg);
}


void wmdf::SZSEL1Adapter::LaunchL2Transactions( SecurityContainerManager* scm )
{
//no transaction;
}

void wmdf::SZSEL1Adapter::LaunchOrderQueueL2( SecurityContainerManager* scm)
{
//no orderqueue
}

void wmdf::SZSEL1Adapter::LaunchSZSEL2Orders( SecurityContainerManager* scm)
{
// no orders
}

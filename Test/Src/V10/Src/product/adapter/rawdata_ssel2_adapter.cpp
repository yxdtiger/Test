#include "rawdata_ssel2_adapter.h"
#include "rawdata_manager.h"
#include "../../protocol/mdf_message.h"
#include "../../include/wmdf_structs.h"
#include "../../include/date_time.h"
#include <iostream>
#include <iomanip>
#include <fstream>
#include "../../monitor/logger.h"


wmdf::SSEL2RawDataAdapter::SSEL2RawDataAdapter(ProductSource* product_source)
:RawDataAdapter(product_source)
{
  Init(false);
}

wmdf::SSEL2RawDataAdapter::~SSEL2RawDataAdapter()
{
  Close();
}

bool wmdf::SSEL2RawDataAdapter::Init( bool bIsFilterHB)
{
	int rtn;
	rtn = SSEInit(bIsFilterHB);
	switch (rtn)
	{
	case E_SSE_OK:
    Logger::instance()->WriteLog(LL_INFO,"sse decoder: init successfully.");
		break;
	case E_SSE_CONFIGFILE_NOTEXIST:
	case E_SSE_CONFIGFILE_EMPTYVALUE:
	case E_SSE_CONFIGFILE_NOXMLPATHITEM:
	case E_SSE_CONFIGFILE_OPENFAILED:
		Logger::instance()->WriteLog(LL_INFO,"sse decoder: - Failed to read config files.");
		break;
	case E_SSE_PARSE_FAIL:
		Logger::instance()->WriteLog(LL_INFO,"sse decoder: failed to parse XML file.");
		break;

	case E_SSE_XMLFILENOTEXIST:
		Logger::instance()->WriteLog(LL_INFO,"sse decoder: xml file does not exist.");
		break;
	default:
		Logger::instance()->WriteLog(LL_INFO,"sse decoder: unknown exception!");
		break;
	}
	if ( rtn != E_SSE_OK)
	{
		Logger::instance()->WriteLog(LL_INFO,"sse decoder: failed to init!");
		return false;
	}
	else
		return true;
}

// time_stamp_t time_span=0;
// time_stamp_t last_time =0;
void wmdf::SSEL2RawDataAdapter::ParseMessage( uint8_t* pRawData,int32_t RawDataLength)
{
//  last_time = DateTime::get_current_time();
	SSEDecodeRawData(pRawData,RawDataLength,FH_data_manager_->FHDataContainer());
//    time_stamp_t onetime_span = DateTime::get_current_time() - last_time;
//    time_span = onetime_span;
//    printf("%s\n",DateTime::time2hmsms(time_span,":").c_str());

  FH_data_manager_->TransferToStruct();
 
	if(FH_data_manager_->orderqueue_count()!=0)
	{
		LaunchOrderQueueL2();
	}

	if(FH_data_manager_->transaction_count()!=0)
	{
		LaunchL2Transactions();
	}

	if(FH_data_manager_->marketsnapshot_count()!=0)
	{
		LaunchMarketSnapshotL2();
	}

	if(FH_data_manager_->marketoverviews_count()!=0)
	{
		LaunchMarketOverView();
	}
	if(FH_data_manager_->virtualauctionprice_count()!=0)
	{
		LaunchVirtualAuctionPrice();
	}
}

void wmdf::SSEL2RawDataAdapter::ReleaseFHData()
{
	SSEReleaseBuffer(*(FH_data_manager_->FHDataContainer()));
	FH_data_manager_->reset();
}

void wmdf::SSEL2RawDataAdapter::Close()
{
	SSEUninit();
}

void wmdf::SSEL2RawDataAdapter::LaunchMarketSnapshotL2()
{
	MdfMessage* mdf_msg = new MdfMessage();
	mdf_msg->set_local_time(DateTime::get_current_time());
	mdf_msg->set_socket_id(0);
	mdf_msg->set_product_id(PRODUCT_SSE_LEVEL2_ID);
	mdf_msg->set_message_id(SSEL2_MARKETDATA_ID);
	mdf_msg->set_major_version(MAJOR_VERSION_SSE_LEVEL2_ID);
	mdf_msg->set_minor_version(MINOR_VERSION_SSE_LEVEL2_ID);
	mdf_msg->set_record_count(FH_data_manager_->marketsnapshot_count());


	MarketMessageHeader* message_header = new MarketMessageHeader();
	message_header->place_holder = 0;
	mdf_msg->set_packet_num(++packet_id_);
	mdf_msg->set_header(message_header);
	mdf_msg->set_body(FH_data_manager_->Get_MarketSnapshot());

	PublishMdfMessage(mdf_msg);
}

void wmdf::SSEL2RawDataAdapter::LaunchL2Transactions()
{
	MdfMessage* mdf_msg = new MdfMessage();
	mdf_msg->set_local_time(DateTime::get_current_time());
	mdf_msg->set_socket_id(0);
	mdf_msg->set_product_id(PRODUCT_SSE_LEVEL2_ID);
	mdf_msg->set_message_id(SSEL2_TRANSACTION_ID);
	mdf_msg->set_major_version(MAJOR_VERSION_SSE_LEVEL2_ID);
	mdf_msg->set_minor_version(MINOR_VERSION_SSE_LEVEL2_ID);
	mdf_msg->set_record_count(FH_data_manager_->transaction_count());


	MarketMessageHeader* message_header = new MarketMessageHeader();
	message_header->place_holder = 0;
	mdf_msg->set_packet_num(++packet_id_);
	mdf_msg->set_header(message_header);
	mdf_msg->set_body(FH_data_manager_->Get_Transaction());

	PublishMdfMessage(mdf_msg);
}

void wmdf::SSEL2RawDataAdapter::LaunchOrderQueueL2()
{
	MdfMessage* mdf_msg = new MdfMessage();
	mdf_msg->set_local_time(DateTime::get_current_time());
	mdf_msg->set_socket_id(0);
	mdf_msg->set_product_id(PRODUCT_SSE_LEVEL2_ID);
	mdf_msg->set_message_id(SSEL2_ORDERQUEUE_ID);
	mdf_msg->set_major_version(MAJOR_VERSION_SSE_LEVEL2_ID);
	mdf_msg->set_minor_version(MINOR_VERSION_SSE_LEVEL2_ID);
	mdf_msg->set_record_count(FH_data_manager_->orderqueue_count());

	MarketMessageHeader* message_header = new MarketMessageHeader();
	message_header->place_holder = 0;
	mdf_msg->set_packet_num(++packet_id_);
	mdf_msg->set_header(message_header);
	mdf_msg->set_body(FH_data_manager_->Get_OrderQueue());

	PublishMdfMessage(mdf_msg);
}

void wmdf::SSEL2RawDataAdapter::LaunchMarketOverView()
{
	MdfMessage* mdf_msg = new MdfMessage();
	mdf_msg->set_local_time(DateTime::get_current_time());
	mdf_msg->set_socket_id(0);
	mdf_msg->set_product_id(PRODUCT_SSE_LEVEL2_ID);
	mdf_msg->set_message_id(SSEL2_MARKETOVERVIEW_ID);
	mdf_msg->set_major_version(MAJOR_VERSION_SSE_LEVEL2_ID);
	mdf_msg->set_minor_version(MINOR_VERSION_SSE_LEVEL2_ID);
	mdf_msg->set_record_count(FH_data_manager_->marketoverviews_count());


	MarketMessageHeader* message_header = new MarketMessageHeader();
	message_header->place_holder = 0;
	mdf_msg->set_packet_num(++packet_id_);
	mdf_msg->set_header(message_header);
	mdf_msg->set_body(FH_data_manager_->Get_MarketOverView());

	PublishMdfMessage(mdf_msg);
}

void wmdf::SSEL2RawDataAdapter::LaunchVirtualAuctionPrice()
{
	MdfMessage* mdf_msg = new MdfMessage();
	mdf_msg->set_local_time(DateTime::get_current_time());
	mdf_msg->set_socket_id(0);
	mdf_msg->set_product_id(PRODUCT_SSE_LEVEL2_ID);
	mdf_msg->set_message_id(SSEL2_VIRTUALAUCTIONPRICE_ID);
	mdf_msg->set_major_version(MAJOR_VERSION_SSE_LEVEL2_ID);
	mdf_msg->set_minor_version(MINOR_VERSION_SSE_LEVEL2_ID);
	mdf_msg->set_record_count(FH_data_manager_->virtualauctionprice_count());


	MarketMessageHeader* header_of_orderqueue = new MarketMessageHeader();
	header_of_orderqueue->place_holder = 0;
	mdf_msg->set_packet_num(++packet_id_);
	mdf_msg->set_header(header_of_orderqueue);
	mdf_msg->set_body(FH_data_manager_->Get_VirtualAuctionPrice());

	PublishMdfMessage(mdf_msg);
}


void wmdf::SSEL2RawDataAdapter::AnalyseDecodedData()
{
	static std::ofstream os_test("test.txt");
	vecFeedHandlerData* pvecFHData = *(FH_data_manager_->FHDataContainer());
	int size_of_fhdata = pvecFHData->size();
	os_test<<"证券代码个数: "<<size_of_fhdata<<std::endl;
	if(size_of_fhdata==0)
		return;
	for(int i=0;i<size_of_fhdata;i++)
	{
		int size_of_fieldvec = (*pvecFHData)[i]->vecField.size();
		os_test<<(*pvecFHData)[i]->szWindCode<<"   指标个数: "<<size_of_fieldvec<<std::endl;
		for(int j=0;j<size_of_fieldvec;j++)
		{
			ptrWnField pField = (*pvecFHData)[i]->vecField[j];
			os_test<<"------ "<<std::setw(3)<<std::setiosflags(std::ios::right)<<pField->usFieldId<<"   "<<(int)pField->ucValueType<<"   ";
			switch(pField->ucValueType)
			{
			case WN_VAL_TYPE_STRUCT:
				{
					int struct_type=(int)pField->ptrValue->ptrStruct->ucStructType;
					os_test<<"struct type: "<<struct_type<<std::endl;
					switch(struct_type)
					{
					case SEQUENCE_BidPriceLevel_3102:
						{
							os_test<<"------------------ It's BidPriceLevel of NGTSMarketData:"<<std::endl;
							ptrBidPriceLevel_3102 temp=(ptrBidPriceLevel_3102)pField->ptrValue->ptrStruct->pStructData;
							while(temp!=NULL)
							{
								os_test<<"------------------ id_134: "<<temp->BidSize<<"    id_132: "<<temp->BidPx<<std::endl;
								temp=temp->pNext;
							}	
						}
						break;
					case SEQUENCE_OfferPriceLevel_3102:
						{
							os_test<<"------------------ It's OfferPriceLevel of NGTSMarketData:"<<std::endl;
							ptrOfferPriceLevel_3102 temp=(ptrOfferPriceLevel_3102)pField->ptrValue->ptrStruct->pStructData;
							while(temp!=NULL)
							{
								os_test<<"------------------ id_135: "<<temp->OfferSize<<"    id_133: "<<temp->OfferPx<<std::endl;
								temp=temp->pNext;
							}
						}
						break;
					case SEQUENCE_Rankings_3103:
						{
							os_test<<"------------------ It's Rankings of NGTSOrderRanking:"<<std::endl;
							ptrRankings_3103 temp=(ptrRankings_3103)pField->ptrValue->ptrStruct->pStructData;
							while(temp!=NULL)
							{
								os_test<<"------------------ id_10046: "<<temp->Ranking<<"    id_48: "<<temp->SecurityID<<"    id_10047: "<<temp->TotalOrderQty<<"    id_387: "<<temp->TotalVolumeTraded<<"    id_8504: "<<temp->TotalValueTraded<<std::endl;
								temp=temp->pNext;
							}
						}
						break;
					case SEQUENCE_Rankings_3104:
						{
							os_test<<"------------------ It's Rankings of NGTSTransactionRankingByIndustry:"<<std::endl;
							ptrRankings_3104 temp=(ptrRankings_3104)pField->ptrValue->ptrStruct->pStructData;
							while(temp!=NULL)
							{
								os_test<<"------------------ id_10050: "<<temp->Ranking<<"   id_10049: "<<temp->IndustryCode<<"   id_10053: "<<temp->IndustryTotalValueTraded<<std::endl;
								ptrTotalValueTradedRankings_3104 temp2=(ptrTotalValueTradedRankings_3104)temp->pTotalValueTradedRankings;
								while(temp2!=NULL)
								{
									os_test<<"------------------------ id_10046: "<<temp2->Ranking<<"   id_48: "<<temp2->SecurityID<<"   id_8504: "<<temp2->TotalValueTraded<<std::endl;
									temp2=temp2->pNext;
								}
								temp=temp->pNext;
							}
						}
						break;
					case SEQUENCE_Rankings_3105:
						{
							os_test<<"------------------ It's Rankings of NGTSSingleOrderCancellationRanking"<<std::endl;
							ptrRankings_3105 temp=(ptrRankings_3105)pField->ptrValue->ptrStruct->pStructData;
							while(temp!=NULL)
							{
								os_test<<"------------------ id_10046: "<<temp->Ranking<<"   id_48: "<<temp->SecurityID<<"   id_8500: "<<temp->OrderEntryTime<<"   id_53: "<<temp->Quantity<<"   id_44: "<<temp->Price<<std::endl;
								temp=temp->pNext;
							}
						}
						break;
					case SEQUENCE_Rankings_3106:
						{
							os_test<<"------------------ It's Rankings of NGTSConsolidatedOrderCancellationRanking"<<std::endl;
							ptrRankings_3106 temp=(ptrRankings_3106)pField->ptrValue->ptrStruct->pStructData;
							while(temp!=NULL)
							{
								os_test<<"------------------ id_10046: "<<temp->Ranking<<"   id_48: "<<temp->SecurityID<<"   id_10054: "<<temp->TotalWithdrawnQty<<std::endl;
								temp=temp->pNext;
							}
						}
						break;
					case SEQUENCE_PriceLevel_3111:
						{
							os_test<<"------------------ It's PriceLevel of NGTSOrderQueue"<<std::endl;
							ptrPriceLevel_3111 temp = (ptrPriceLevel_3111)pField->ptrValue->ptrStruct->pStructData;
							while(temp!=NULL)
							{
								os_test<<"------------------ id_10147: "<<temp->PriceLevelOperator<<"   id_44: "<<temp->Price<<"   id_10067: "<<temp->NumOrders<<std::endl;
								ptrNoOrders_3111 temp2 = temp->pNoOrders;
								while(temp2!=NULL)
								{
									os_test<<"----------------------- id_10148: "<<temp2->OrderQueueOperator<<"   id_10149: "<<temp2->OrderQueueOperatorEntryID<<"   id_38: "<<temp2->OrderQty<<std::endl;
									temp2=temp2->pNext;
								}
								temp=temp->pNext;	
							}
						}
						break;
					case SEQUENCE_BidLevels_3112:
						{
							os_test<<"------------------ It's BidLevels of NGTSMarketOrderSummary"<<std::endl;
							ptrBidLevels_3112 temp = (ptrBidLevels_3112)pField->ptrValue->ptrStruct->pStructData;

							while(temp!=NULL)
							{
								os_test<<"------------------ id_10145: "<<temp->Operator<<"   id_44 "<<temp->Price<<"   id_38 "<<temp->OrderQty<<"   id_10067 "<<temp->NumOrders;
								temp=temp->pNext;
							}

						}
						break;
					case SEQUENCE_OfferLevels_3112:
						{
							os_test<<"------------------ It's offerLevels of NGTSMarketOrderSummary"<<std::endl;
							ptrOfferLevels_3112 temp = (ptrOfferLevels_3112)pField->ptrValue->ptrStruct->pStructData;

							while(temp!=NULL)
							{
								os_test<<"------------------ id_10145: "<<temp->Operator<<"   id_44 "<<temp->Price<<"   id_38 "<<temp->OrderQty<<"   id_10067 "<<temp->NumOrders<<std::endl;
								temp=temp->pNext;
							}

						}
						break;
					case SEQUENCE_BidPriceLevel_5102:
						{
							os_test<<"------------------ It's BidPriceLevel of Level1Data"<<std::endl;
							ptrBidPriceLevel_5102 temp = (ptrBidPriceLevel_5102)pField->ptrValue->ptrStruct->pStructData;

							while(temp!=NULL)
							{
								os_test<<"------------------ id_134: "<<temp->BidSize<<"   id_132: "<<temp->BidPx<<std::endl;
								temp=temp->pNext;
							}

						}
						break;
					case SEQUENCE_OfferPriceLevel_5102:
						{
							os_test<<"------------------ It's OfferPiceLevel of Level1Data"<<std::endl;
							ptrOfferPriceLevel_5102 temp = (ptrOfferPriceLevel_5102)pField->ptrValue->ptrStruct->pStructData;

							while( temp!=NULL)
							{
								os_test<<"------------------ id_135: "<<temp->OfferSize<<"   id_133: "<<temp->OfferPx<<std::endl;
								temp=temp->pNext;
							}

						}
						break;
					case SEQUENCE_BidOfferLevels_3202:
						{
							os_test<<"------------------ It's BidOfferPiceLevel of Level1Data"<<std::endl;
							ptrBidOfferLevels_3202 temp = (ptrBidOfferLevels_3202)pField->ptrValue->ptrStruct->pStructData;

							while(temp!=NULL)
							{
								os_test<<"------------------ id_10147: "<<temp->PriceLevelOperator<<"   id_44: "<<temp->Price<<"   id_39: "<<temp->OrderQty<<"   id_10067: "<<temp->NumOrders<<std::endl;
								ptrNoOrders_3111 temp2 = temp->pNoOrders;

								while(temp2!=NULL)
								{
									os_test<<"------------------ id_10148: "<<temp2->OrderQueueOperator<<"   id_10149: "<<temp2->OrderQueueOperatorEntryID<<"   id_38:"<<temp2->OrderQty<<std::endl;
									temp2=temp2->pNext;
								}

								temp=temp->pNext;
							}

						}
						break;
					default:
						os_test<<"------------------ It's void struct"<<std::endl;
						break;
					}
				}
				break;
			case WN_VAL_TYPE_UINT32:
				os_test<<pField->ptrValue->ui32<<std::endl;
				break;
			case WN_VAL_TYPE_INT32:
				if(pField->usFieldId==999)
					os_test<<"template id: ";
				os_test<<pField->ptrValue->i32<<std::endl;
				break;
			case WN_VAL_TYPE_UINT64:
				os_test<<pField->ptrValue->ui64<<std::endl;
				break;
			case WN_VAL_TYPE_INT64:
				os_test<<pField->ptrValue->i64<<std::endl;
				break;
			case WN_VAL_TYPE_DOUBLE:
				os_test<<pField->ptrValue->d<<std::endl;
				break;
			case WN_VAL_TYPE_STRING:
				{
					os_test<<"pstring     size: "<<pField->ptrValue->ptrString->unStringLen<<std::endl;
				}
				break;
			case WN_VAL_TYPE_NULL:
				os_test<<"null"<<std::endl;
				break;
			}
		}
		os_test<<std::endl;
	}
	os_test<<std::endl;
	
}

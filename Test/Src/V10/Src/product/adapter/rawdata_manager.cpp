#include "rawdata_manager.h"
#include "../../include/wmdf_api.h"
#include "../../include/date_time.h"
#include "iostream"
#include "fstream"



wmdf::FHDataManager::FHDataManager()
{
	reset();
}

wmdf::FHDataManager::~FHDataManager()
{
}



void wmdf::FHDataManager::reset()
{
	pvecFHdata_=NULL;
	orderqueues_=NULL;
	transactions_=NULL;
	marketsnapshots_=NULL;
	marketoverviews_=NULL;
	virtualauctionprices_=NULL;

	num_of_orderqueue_=0;
	num_of_transaction_=0;
	num_of_marketoverview_=0;
	num_of_marketsnapshot_=0;
	num_of_virtualauctionprice_=0;
}


void wmdf::FHDataManager::TransferToStruct()
{
	Calculate();
	AllocateMemory();
	Transfer();
}

void wmdf::FHDataManager::Calculate()
{
	uint32_t size_of_fhdata = pvecFHdata_->size();
	if(size_of_fhdata==0)
		return;
	for(uint32_t i=0;i<size_of_fhdata;i++)
	{
		if((*pvecFHdata_)[i]->vecField.size()==0)
			continue;
		ptrWnField pField = (*pvecFHdata_)[i]->vecField[0];
		int32_t template_id = pField->ptrValue->i32;
		switch(template_id)
		{
			//…œΩªÀ˘L2 template_id: 3202 5102 3115 3201 3113 1115 2102
			/*
		case 3202:
			break;
		case 3115:
			++num_of_marketoverview_;
			break;
		case 5102:
			++num_of_level1data_;
			break;
		case 3201:
			++num_of_transaction_;
			break;
		case 3113:
			++num_of_marketsnapshot_;
			break;
		case 1115:
			++num_of_marketoverview_;
			break;
		case 2102:
			++num_of_AH_marketdata_;
			break;
		default:
			break;*/
			
		case 3111:
			num_of_orderqueue_++;
			break;
		case 3101:
			num_of_transaction_++;
			break;
		case 3102:
		case 3113:
				num_of_marketsnapshot_++;
			break;
		case 3115:
			num_of_marketoverview_++;
			break;
		case 3107:
			num_of_virtualauctionprice_++;
			break;
		default:
			break;
		}
	}
}

void wmdf::FHDataManager::AllocateMemory()
{
	if(num_of_orderqueue_!=0)
	{
		orderqueues_ = new L2OrderQueue[num_of_orderqueue_];
	}

	if(num_of_transaction_!=0)
	{
		transactions_ = new L2Transaction[num_of_transaction_];
		for(size_t i = 0; i != num_of_transaction_; ++i)
		{
			for(int32_t j=0;j<16;j++)
			{
				transactions_[i].wind_code[j]=0;
			}
			transactions_[i].trade_time=WMDF_CONSTS_32BIT_NIL_VALUE;
			transactions_[i].set_no=WMDF_CONSTS_32BIT_NIL_VALUE;
			transactions_[i].rec_no=WMDF_CONSTS_32BIT_NIL_VALUE;
			transactions_[i].buyOrderRecNo=WMDF_CONSTS_32BIT_NIL_VALUE;
			transactions_[i].sellOrderRecNo=WMDF_CONSTS_32BIT_NIL_VALUE;
			transactions_[i].price=WMDF_CONSTS_DOUBLE_NIL_VALUE;
			transactions_[i].trade_qty=WMDF_CONSTS_32BIT_NIL_VALUE;
			transactions_[i].order_kind=WMDF_CONSTS_8BIT_NIL_VALUE;
			transactions_[i].function_code=WMDF_CONSTS_8BIT_NIL_VALUE;
			transactions_[i].trade_channel=WMDF_CONSTS_32BIT_NIL_VALUE;
			transactions_[i].trade_money=WMDF_CONSTS_64BIT_NIL_VALUE;	
		}
	}

	if(num_of_marketsnapshot_!=0)
	{
		marketsnapshots_ = new L2MarketSnapshot[num_of_marketsnapshot_];
		for(size_t i = 0; i != num_of_marketsnapshot_; ++i)
		{
			for(int32_t j=0;j<16;j++)
			{
				marketsnapshots_[i].wind_code[j]=0;
			}
			marketsnapshots_[i].trade_date=WMDF_CONSTS_32BIT_NIL_VALUE;
			marketsnapshots_[i].trade_time=WMDF_CONSTS_32BIT_NIL_VALUE;
			marketsnapshots_[i].prev_close=WMDF_CONSTS_DOUBLE_NIL_VALUE;
			marketsnapshots_[i].today_open=WMDF_CONSTS_DOUBLE_NIL_VALUE;
			marketsnapshots_[i].today_high=WMDF_CONSTS_DOUBLE_NIL_VALUE;
			marketsnapshots_[i].today_low=WMDF_CONSTS_DOUBLE_NIL_VALUE;
			marketsnapshots_[i].new_price=WMDF_CONSTS_DOUBLE_NIL_VALUE;
			marketsnapshots_[i].volume=WMDF_CONSTS_64BIT_NIL_VALUE;
			marketsnapshots_[i].total_amount=WMDF_CONSTS_DOUBLE_NIL_VALUE;
			marketsnapshots_[i].bid_price1=WMDF_CONSTS_DOUBLE_NIL_VALUE;
			marketsnapshots_[i].bid_price2=WMDF_CONSTS_DOUBLE_NIL_VALUE;
			marketsnapshots_[i].bid_price3=WMDF_CONSTS_DOUBLE_NIL_VALUE;
			marketsnapshots_[i].bid_price4=WMDF_CONSTS_DOUBLE_NIL_VALUE;
			marketsnapshots_[i].bid_price5=WMDF_CONSTS_DOUBLE_NIL_VALUE;
			marketsnapshots_[i].bid_price6=WMDF_CONSTS_DOUBLE_NIL_VALUE;
			marketsnapshots_[i].bid_price7=WMDF_CONSTS_DOUBLE_NIL_VALUE;
			marketsnapshots_[i].bid_price8=WMDF_CONSTS_DOUBLE_NIL_VALUE;
			marketsnapshots_[i].bid_price9=WMDF_CONSTS_DOUBLE_NIL_VALUE;
			marketsnapshots_[i].bid_price10=WMDF_CONSTS_DOUBLE_NIL_VALUE;
			marketsnapshots_[i].ask_price1=WMDF_CONSTS_DOUBLE_NIL_VALUE;
			marketsnapshots_[i].ask_price2=WMDF_CONSTS_DOUBLE_NIL_VALUE;
			marketsnapshots_[i].ask_price3=WMDF_CONSTS_DOUBLE_NIL_VALUE;
			marketsnapshots_[i].ask_price4=WMDF_CONSTS_DOUBLE_NIL_VALUE;
			marketsnapshots_[i].ask_price5=WMDF_CONSTS_DOUBLE_NIL_VALUE;
			marketsnapshots_[i].ask_price6=WMDF_CONSTS_DOUBLE_NIL_VALUE;
			marketsnapshots_[i].ask_price7=WMDF_CONSTS_DOUBLE_NIL_VALUE;
			marketsnapshots_[i].ask_price8=WMDF_CONSTS_DOUBLE_NIL_VALUE;
			marketsnapshots_[i].ask_price9=WMDF_CONSTS_DOUBLE_NIL_VALUE;
			marketsnapshots_[i].ask_price10=WMDF_CONSTS_DOUBLE_NIL_VALUE;
			marketsnapshots_[i].ask_volume1=WMDF_CONSTS_32BIT_NIL_VALUE;
			marketsnapshots_[i].ask_volume2=WMDF_CONSTS_32BIT_NIL_VALUE;
			marketsnapshots_[i].ask_volume3=WMDF_CONSTS_32BIT_NIL_VALUE;
			marketsnapshots_[i].ask_volume4=WMDF_CONSTS_32BIT_NIL_VALUE;
			marketsnapshots_[i].ask_volume5=WMDF_CONSTS_32BIT_NIL_VALUE;
			marketsnapshots_[i].ask_volume6=WMDF_CONSTS_32BIT_NIL_VALUE;
			marketsnapshots_[i].ask_volume7=WMDF_CONSTS_32BIT_NIL_VALUE;
			marketsnapshots_[i].ask_volume8=WMDF_CONSTS_32BIT_NIL_VALUE;
			marketsnapshots_[i].ask_volume9=WMDF_CONSTS_32BIT_NIL_VALUE;
			marketsnapshots_[i].ask_volume10=WMDF_CONSTS_32BIT_NIL_VALUE;
			marketsnapshots_[i].bid_volume1=WMDF_CONSTS_32BIT_NIL_VALUE;
			marketsnapshots_[i].bid_volume2=WMDF_CONSTS_32BIT_NIL_VALUE;
			marketsnapshots_[i].bid_volume3=WMDF_CONSTS_32BIT_NIL_VALUE;
			marketsnapshots_[i].bid_volume4=WMDF_CONSTS_32BIT_NIL_VALUE;
			marketsnapshots_[i].bid_volume5=WMDF_CONSTS_32BIT_NIL_VALUE;
			marketsnapshots_[i].bid_volume6=WMDF_CONSTS_32BIT_NIL_VALUE;
			marketsnapshots_[i].bid_volume7=WMDF_CONSTS_32BIT_NIL_VALUE;
			marketsnapshots_[i].bid_volume8=WMDF_CONSTS_32BIT_NIL_VALUE;
			marketsnapshots_[i].bid_volume9=WMDF_CONSTS_32BIT_NIL_VALUE;
			marketsnapshots_[i].bid_volume10=WMDF_CONSTS_32BIT_NIL_VALUE;
			marketsnapshots_[i].transactions_count=WMDF_CONSTS_64BIT_NIL_VALUE;
			marketsnapshots_[i].weighted_average_best_bid=WMDF_CONSTS_DOUBLE_NIL_VALUE;
			marketsnapshots_[i].weighted_average_best_ask=WMDF_CONSTS_DOUBLE_NIL_VALUE;
			//marketsnapshots_[i].trading_phase_code=WMDF_CONSTS_64BIT_NIL_VALUE;
			/*for(int32_t m=0;m<4;m++)
			{
				marketsnapshots_[i].security_pre_name[m]=0;
			}*/
// 			marketsnapshots_[i].iopv=WMDF_CONSTS_DOUBLE_NIL_VALUE;
// 			marketsnapshots_[i].ytm=WMDF_CONSTS_DOUBLE_NIL_VALUE;
			marketsnapshots_[i].l2_total_bid_qty=WMDF_CONSTS_64BIT_NIL_VALUE;
			marketsnapshots_[i].l2_total_offer_qty=WMDF_CONSTS_64BIT_NIL_VALUE;
		}
	}

	if(num_of_marketoverview_!=0)
	{
		marketoverviews_ = new MarketOverView[num_of_marketoverview_];
		for(size_t i = 0; i != num_of_marketoverview_; ++i)
		{
			for(int32_t j=0;j<16;j++)
			{
				marketoverviews_[i].wind_code[j]=0;
			}
			marketoverviews_[i].date_time_stamp=WMDF_CONSTS_32BIT_NIL_VALUE;
			marketoverviews_[i].origtime=WMDF_CONSTS_32BIT_NIL_VALUE;
			marketoverviews_[i].origdate=WMDF_CONSTS_32BIT_NIL_VALUE;
		}
	}

	if(num_of_virtualauctionprice_!=0)
	{
		virtualauctionprices_ = new VirtualAuctionPrice[num_of_virtualauctionprice_];
		for(size_t i=0;i!= num_of_virtualauctionprice_;++i)
		{
			for(int32_t j=0;j<16;j++)
			{
				virtualauctionprices_[i].wind_code[j]=0;
			}
			virtualauctionprices_[i].date_time_stamp=WMDF_CONSTS_32BIT_NIL_VALUE;
			virtualauctionprices_[i].price=WMDF_CONSTS_DOUBLE_NIL_VALUE;
			virtualauctionprices_[i].virtual_auction_qty=WMDF_CONSTS_32BIT_NIL_VALUE;
			virtualauctionprices_[i].leave_qty=WMDF_CONSTS_32BIT_NIL_VALUE;
			virtualauctionprices_[i].side=WMDF_CONSTS_8BIT_NIL_VALUE;
		}
	}
}




void wmdf::FHDataManager::Transfer()
{
	uint32_t size_of_fhdata = pvecFHdata_->size();
	uint32_t pos_of_orderqueue=0, pos_of_transaction=0, pos_of_marketsnapshot=0, pos_of_marketoverview=0, pos_of_virtualauctionprice=0;

	for(uint32_t i=0;i<size_of_fhdata;i++)
	{
		uint32_t size_of_vecField = (*pvecFHdata_)[i]->vecField.size();
		if(size_of_vecField == 0)
			continue;
		ptrWnField pField = (*pvecFHdata_)[i]->vecField[0];

		int32_t template_id = pField->ptrValue->i32;
		switch(template_id)
		{
		// order queue	
		case 3111:
			{
				memcpy(orderqueues_[pos_of_orderqueue].wind_code,(*pvecFHdata_)[i]->szWindCode,16);	
				for(uint32_t j=1;j<size_of_vecField;j++)
				{
					ptrWnField temp_field = (*pvecFHdata_)[i]->vecField[j];			
					TransferOrderQueueField(pos_of_orderqueue,temp_field);
				}
				pos_of_orderqueue++;
			}
			break;
		//transaction
		case 3101:
			{
				memcpy(transactions_[pos_of_transaction].wind_code,(*pvecFHdata_)[i]->szWindCode,16);
				for(uint32_t j=1;j<size_of_vecField;j++)
				{
					ptrWnField temp_field = (*pvecFHdata_)[i]->vecField[j];
					TransferTransactionField(pos_of_transaction,temp_field);
				}
				pos_of_transaction++;
			}
			break;
		// market data (index)
		case 3102:
		case 3113:
			{
				memcpy(marketsnapshots_[pos_of_marketsnapshot].wind_code,(*pvecFHdata_)[i]->szWindCode,16);
				for(uint32_t j=1;j<size_of_vecField;j++)
				{
					ptrWnField temp_field = (*pvecFHdata_)[i]->vecField[j];
					TransferMarketSnapshotField(pos_of_marketsnapshot,temp_field);
				}
				pos_of_marketsnapshot++;
			}
			break;
		//MarketOverview
		case 3115:
			{
				memcpy(marketoverviews_[pos_of_marketoverview].wind_code,(*pvecFHdata_)[i]->szWindCode,16);
				for(uint32_t j=1;j<size_of_vecField;j++)
				{
					ptrWnField temp_field = (*pvecFHdata_)[i]->vecField[j];
					TransferMarketOverViewField(pos_of_marketoverview,temp_field);
				}
				pos_of_marketoverview++;
			}
			break;
		//virtualauctionprice
		case 3107:
			{
				memcpy(virtualauctionprices_[pos_of_virtualauctionprice].wind_code,(*pvecFHdata_)[i]->szWindCode,16);
				for(uint32_t j=1;j<size_of_vecField;j++)
				{
					ptrWnField temp_field = (*pvecFHdata_)[i]->vecField[j];
					TransferVirtualAuctionPriceField(pos_of_virtualauctionprice,temp_field);
				}
				pos_of_virtualauctionprice++;
			}
			break;
		default:
			break;
		}
	}
}


void wmdf::FHDataManager::TransferOrderQueueField( uint32_t pos_of_orderqueues,ptrWnField pField )
{
	switch(pField->usFieldId)
	{
	case 752:
		{
			orderqueues_[pos_of_orderqueues].date_time_stamp =DateTime::hms2time( pField->ptrValue->i32)*1000;
		}
		break;
	case 759:
		{
			orderqueues_[pos_of_orderqueues].side = pField->ptrValue->ptrString->pString[0]-48;
		}
		break;
	case 765:
		{
			
			orderqueues_[pos_of_orderqueues].image_status = pField->ptrValue->i32;
		}
		break;
	case 606:
		{
			uint32_t num_of_pricelevel = 0;
			ptrPriceLevel_3111 point_to_pricelevel = (ptrPriceLevel_3111)pField->ptrValue->ptrStruct->pStructData;
			while(point_to_pricelevel!=NULL)
			{
				num_of_pricelevel++;
				point_to_pricelevel = point_to_pricelevel->pNext;
			}
			orderqueues_[pos_of_orderqueues].price_level_count = num_of_pricelevel;
			if (num_of_pricelevel == 0)
			{
				orderqueues_[pos_of_orderqueues].order_queue_levels = NULL;
			}
			else
			{
				orderqueues_[pos_of_orderqueues].order_queue_levels = new L2OrderQueuePriceLevel[num_of_pricelevel];
				uint32_t pos_of_pricelevel = 0;
				point_to_pricelevel = (ptrPriceLevel_3111)pField->ptrValue->ptrStruct->pStructData;
				while(pos_of_pricelevel < num_of_pricelevel)
				{
					orderqueues_[pos_of_orderqueues].order_queue_levels[pos_of_pricelevel].price_level_operator = point_to_pricelevel->PriceLevelOperator;
					orderqueues_[pos_of_orderqueues].order_queue_levels[pos_of_pricelevel].price = point_to_pricelevel->Price;
					orderqueues_[pos_of_orderqueues].order_queue_levels[pos_of_pricelevel].orders_total = point_to_pricelevel->NumOrders;
					uint32_t num_of_orders = 0;
					ptrNoOrders_3111 point_to_orders = point_to_pricelevel->pNoOrders;
					while(point_to_orders!=NULL)
					{
						num_of_orders++;
						point_to_orders = point_to_orders->pNext;
					}
					orderqueues_[pos_of_orderqueues].order_queue_levels[pos_of_pricelevel].orders_count = num_of_orders;
					if (num_of_orders == 0)
					{
						orderqueues_[pos_of_orderqueues].order_queue_levels[pos_of_pricelevel].order_queue_items=NULL;
					}
					else
					{
						orderqueues_[pos_of_orderqueues].order_queue_levels[pos_of_pricelevel].order_queue_items = new L2OrderQueueItem[num_of_orders];
						uint32_t pos_of_orders = 0;
						point_to_orders = point_to_pricelevel->pNoOrders;
						while(pos_of_orders < num_of_orders)
						{
							orderqueues_[pos_of_orderqueues].order_queue_levels[pos_of_pricelevel].order_queue_items[pos_of_orders].order_qty = (int32_t)point_to_orders->OrderQty;
							orderqueues_[pos_of_orderqueues].order_queue_levels[pos_of_pricelevel].order_queue_items[pos_of_orders].order_queue_operator = point_to_orders->OrderQueueOperator;
							orderqueues_[pos_of_orderqueues].order_queue_levels[pos_of_pricelevel].order_queue_items[pos_of_orders].order_queue_operator_entry_id = point_to_orders->OrderQueueOperatorEntryID;
							pos_of_orders++;
							point_to_orders=point_to_orders->pNext;
						}					
					}
					pos_of_pricelevel++;
					point_to_pricelevel=point_to_pricelevel->pNext;
				}
			}

		}
		break;
	default:
		break;
	}
}


void wmdf::FHDataManager::TransferTransactionField( uint32_t pos_of_transactions, ptrWnField pField )
{
	switch(pField->usFieldId)
	{
	case 60:
		{
			transactions_[pos_of_transactions].rec_no = pField->ptrValue->i32;
		}
		break;
	case 751:
		{
			transactions_[pos_of_transactions].trade_channel = pField->ptrValue->i32;
		}
		break;
	case 2:
		{
			transactions_[pos_of_transactions].trade_time =DateTime::hms2time( pField->ptrValue->i32)*1000;
		}
		break;
	case 3:
		{
			transactions_[pos_of_transactions].price = pField->ptrValue->d;
		}
		break;
	case 8:
		{
			transactions_[pos_of_transactions].trade_qty = (int32_t)pField->ptrValue->d;
		}
		break;
	case 59:
		{
			transactions_[pos_of_transactions].trade_money = (int64_t)pField->ptrValue->d;
		}
		break;
	default:
		break;
	}

}

void wmdf::FHDataManager::TransferMarketSnapshotField( uint32_t pos_of_marketsnapshot, ptrWnField pField )
{
	switch(pField->usFieldId)
	{
	case 752:
		marketsnapshots_[pos_of_marketsnapshot].trade_time =DateTime::hms2time( pField->ptrValue->i32)*1000;
		break;
	case 3:
		marketsnapshots_[pos_of_marketsnapshot].new_price = pField->ptrValue->d;
		break;
	case 4:
		marketsnapshots_[pos_of_marketsnapshot].prev_close = pField->ptrValue->d;
		break;
	case 5:
		marketsnapshots_[pos_of_marketsnapshot].today_open = pField->ptrValue->d;
		break;
	case 6:
		marketsnapshots_[pos_of_marketsnapshot].today_high = pField->ptrValue->d;
		break;
	case 7:
		marketsnapshots_[pos_of_marketsnapshot].today_low = pField->ptrValue->d;
		break;
	case 8:
		marketsnapshots_[pos_of_marketsnapshot].volume = (int64_t)pField->ptrValue->d;
		break;
	case 59:
		marketsnapshots_[pos_of_marketsnapshot].total_amount = pField->ptrValue->d;
		break;
	case 71:
		marketsnapshots_[pos_of_marketsnapshot].transactions_count = pField->ptrValue->i32;
		break;
	case 86:
		marketsnapshots_[pos_of_marketsnapshot].weighted_average_best_bid = pField->ptrValue->d;
		break;
	case 87:
		marketsnapshots_[pos_of_marketsnapshot].weighted_average_best_ask = pField->ptrValue->d;
		break;
// 	case 253:
// 		marketsnapshots_[pos_of_marketsnapshot].iopv = pField->ptrValue->d;
// 		break;
// 	case 261:
// 		marketsnapshots_[pos_of_marketsnapshot].ytm = pField->ptrValue->d;
// 		break;
	case 340:
		marketsnapshots_[pos_of_marketsnapshot].l2_total_bid_qty = (int64_t)pField->ptrValue->d;
		break;
	case 341:
		marketsnapshots_[pos_of_marketsnapshot].l2_total_offer_qty = (int64_t)pField->ptrValue->d;
		break;
	case 600:
		{
			ptrBidPriceLevel_3102 point_to_bidpricelevel = (ptrBidPriceLevel_3102)pField->ptrValue->ptrStruct->pStructData;
			uint8_t* price_addr = (uint8_t*)&(marketsnapshots_[pos_of_marketsnapshot].bid_price1);
			uint32_t offset_of_price_addr = 0;
			uint8_t* volume_addr = (uint8_t*)&(marketsnapshots_[pos_of_marketsnapshot].bid_volume1);
			uint32_t offset_of_volume_addr = 0;
			while(point_to_bidpricelevel != NULL)
			{
				memcpy(price_addr+offset_of_price_addr,&(point_to_bidpricelevel->BidPx),sizeof(double));
				offset_of_price_addr+=sizeof(double);
				int64_t temp = (int64_t)point_to_bidpricelevel->BidSize;
				memcpy(volume_addr+offset_of_volume_addr,&temp,sizeof(int64_t));
				offset_of_volume_addr+=sizeof(int64_t);
				point_to_bidpricelevel = point_to_bidpricelevel->pNext;
			}
		}
		break;
	case 601:
		{
			ptrOfferPriceLevel_3102 point_to_offerpricelevel = (ptrOfferPriceLevel_3102)pField->ptrValue->ptrStruct->pStructData;
			uint8_t* price_addr = (uint8_t*)&(marketsnapshots_[pos_of_marketsnapshot].ask_price1);
			uint32_t offset_of_price_addr = 0;
			uint8_t* volume_addr = (uint8_t*)&(marketsnapshots_[pos_of_marketsnapshot].ask_volume1);
			uint32_t offset_of_volume_addr = 0;
			while(point_to_offerpricelevel != NULL)
			{
				memcpy(price_addr+offset_of_price_addr,&(point_to_offerpricelevel->OfferPx),sizeof(double));
				offset_of_price_addr+=sizeof(double);
				int64_t temp = (int64_t)point_to_offerpricelevel->OfferSize;
				memcpy(volume_addr+offset_of_volume_addr,&temp,sizeof(int64_t));
				offset_of_volume_addr+=sizeof(int64_t);
				point_to_offerpricelevel = point_to_offerpricelevel->pNext;
			}
		}
		break;
	default:
		break;
	}
}

void wmdf::FHDataManager::TransferMarketOverViewField( uint32_t pos_of_marketoverview, ptrWnField pField )
{
	switch(pField->usFieldId)
	{
	case 752:
		{
			marketoverviews_[pos_of_marketoverview].date_time_stamp = DateTime::hms2time(pField->ptrValue->i32)*1000;
		}
		break;
	case 763:
		{
			marketoverviews_[pos_of_marketoverview].origtime = pField->ptrValue->i32;
		}
		break;
	case 764:
		{
			marketoverviews_[pos_of_marketoverview].origdate = pField->ptrValue->i32;
		}
		break;
	default:
		break;
	}
}

void wmdf::FHDataManager::TransferVirtualAuctionPriceField( uint32_t pos_of_virtualauctionprice, ptrWnField pField )
{
	switch(pField->usFieldId)
	{
	case 752:
		{
			virtualauctionprices_[pos_of_virtualauctionprice].date_time_stamp = DateTime::hms2time(pField->ptrValue->i32)*1000;
		}
		break;
	case 770:
		{
			virtualauctionprices_[pos_of_virtualauctionprice].price = pField->ptrValue->d;
		}
		break;
	case 771:
		{
			virtualauctionprices_[pos_of_virtualauctionprice].virtual_auction_qty = (int32_t)pField->ptrValue->d;
		}
		break;
	case 772:
		{
			virtualauctionprices_[pos_of_virtualauctionprice].leave_qty =(int32_t)pField->ptrValue->d;
		}
		break;
	case 759:
		{
			virtualauctionprices_[pos_of_virtualauctionprice].side = pField->ptrValue->ptrString->pString[0]-48;
		}
		break;
	default:
		break;
	}
}



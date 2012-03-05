#include <assert.h>
#include "securityContainer.h"
#include "../../include/date_time.h"
#include "securityIndex.h"
#include "indicatorDefs.h"
#include "old_struct.h"
#ifdef __linux__
#include <cstring>
#endif
using namespace std;


wmdf::SecurityContainer::SecurityContainer()
{

}

wmdf::SecurityContainer::~SecurityContainer()
{
  for(uint32_t k=0;k<indexes_.size();k++)
  {
    switch(indexes_[k]->get_index())
    {
    case IND_L2_ORDER_QUEUE_OFFER:
    case IND_L2_ORDER_QUEUE:
      {
        ComplexIndex* ci=(ComplexIndex*)indexes_[k];
        L2_OrderQueue* l2_o = (L2_OrderQueue*)(ci->struct_);
        uint32_t num_of_pricelevel = l2_o->NoPriceLevel;
        for(uint32_t i=0;i<num_of_pricelevel;i++)
        {
          L2_OrderQueue_PriceLevel* l2_oqpl = &(l2_o->PriceLevel[i]);
          delete[] l2_oqpl->Orders;
          l2_oqpl->Orders=NULL;
        }
        delete[] l2_o->PriceLevel;
        l2_o->PriceLevel=NULL;
      }
      break;
    default:
      break;
    }

    delete indexes_[k];
    indexes_[k]=NULL;
  }


  vector<IndexBase*> tmp;
  indexes_.swap(tmp);
}



int32_t wmdf::SecurityContainer::indexes_count()
{
  return indexes_.size();
}

void wmdf::SecurityContainer::add_index( IndexBase* index )
{
  indexes_.push_back(index);
}



wmdf::SecurityContainerManager::SecurityContainerManager()
{
  num_of_l2marketsnapshot_=0;
  num_of_l2orderqueue_=0;
  num_of_l2orders_=0;
  num_of_l2transaction_=0;
}

wmdf::SecurityContainerManager::~SecurityContainerManager()
{
  reset();
}


void wmdf::SecurityContainerManager::reset()
{
  for(uint32_t i=0;i<security_containers_.size();i++)
  {
	  delete security_containers_[i];
    security_containers_[i]=NULL;
  }

  vector<SecurityContainer*> tmp;
  security_containers_.swap(tmp);

  num_of_l2marketsnapshot_=0;
  num_of_l2orderqueue_=0;
  num_of_l2orders_=0;
  num_of_l2transaction_=0;
}

void wmdf::SecurityContainerManager::l2marketsnapshot_plus()
{
  num_of_l2marketsnapshot_++;
}

void wmdf::SecurityContainerManager::l2orderqueue_plus()
{
  num_of_l2orderqueue_++;
}

void wmdf::SecurityContainerManager::l2orders_plus()
{
  num_of_l2orders_++;
}

void wmdf::SecurityContainerManager::l2transaction_plus()
{
  num_of_l2transaction_++;
}

int32_t wmdf::SecurityContainerManager::l2marketsnapshot_count()
{
  return num_of_l2marketsnapshot_;
}

int32_t wmdf::SecurityContainerManager::l2orderqueue_count()
{
  return num_of_l2orderqueue_;
}

int32_t wmdf::SecurityContainerManager::l2orders_count()
{
  return num_of_l2orders_;
}

int32_t wmdf::SecurityContainerManager::l2transaction_count()
{
  return num_of_l2transaction_;
}

void wmdf::SecurityContainerManager::add_securitycontainer( SecurityContainer* securitycontainer )
{
  security_containers_.push_back(securitycontainer);
}



L2OrderQueue* wmdf::SecurityContainerManager::TransferToSZSEL2OrderQueue()
{
  L2OrderQueue* oql2=new L2OrderQueue[num_of_l2orderqueue_];

  memset(oql2,0,sizeof(L2OrderQueue)*num_of_l2orderqueue_);

  int32_t k=0;
  for(uint32_t i=0;i<security_containers_.size();i++)
  {
	  SecurityContainer* sc = security_containers_[i];
	  string windcode = sc->windcode();
	  for(int32_t j=0;j<sc->indexes_count();j++)
	  {
	    IndexBase* index_temp=sc->index(j);
	    if(index_temp->get_index()!=IND_L2_ORDER_QUEUE_OFFER
		  &&index_temp->get_index()!=IND_L2_ORDER_QUEUE)
		    continue;
	    else
	    {
		    L2_OrderQueue* old_l2_oq =(L2_OrderQueue*)((ComplexIndex*)index_temp)->struct_;
		    memcpy(oql2[k].wind_code,windcode.c_str(),windcode.size());
		    uint32_t hms = old_l2_oq->DateTimeStamp;
		    oql2[k].data_time_stamp = DateTime::hms2time((int32_t)hms)*1000;
		    oql2[k].order_side = old_l2_oq->Side;
		    oql2[k].image_status = (uint32_t)old_l2_oq->ImageStatus;
		    oql2[k].price_level_count = 1;
		    L2OrderQueuePriceLevel* l2_oql = new L2OrderQueuePriceLevel[1];
		    l2_oql[0].operator_type = old_l2_oq->PriceLevel[0].PriceLevelOperator;
		    l2_oql[0].order_price = ((double)old_l2_oq->PriceLevel[0].Price)/1000U;
		    l2_oql[0].total_number = old_l2_oq->PriceLevel[0].NumOfOrders;
		    l2_oql[0].orders_number = old_l2_oq->PriceLevel[0].NoOrders;

		    L2OrderQueueItem* l2_oqlq = new L2OrderQueueItem[l2_oql[0].orders_number];
		    for(int32_t n=0;n<l2_oql[0].orders_number;n++)
		    {
			    l2_oqlq[n].operator_type = old_l2_oq->PriceLevel[0].Orders[n].PriceLevelOperator;
			    l2_oqlq[n].size = old_l2_oq->PriceLevel[0].Orders[n].OrderQty;
			    l2_oqlq[n].entry_id = old_l2_oq->PriceLevel[0].Orders[n].OrderQueueOperatorEntryID;
		    }
		    l2_oql[0].order_items = l2_oqlq;

		    oql2[k].price_levels = l2_oql;
		    k++;
        if(k>=num_of_l2orderqueue_)
          break;
	    }
	  }
  }
  return oql2;
}


L2Transaction* wmdf::SecurityContainerManager::TransferToSZSEL2Transactions()
{
  L2Transaction* new_12_t = new L2Transaction[num_of_l2transaction_];
  for(int32_t i=0;i<num_of_l2transaction_;i++)
  {
		memset(new_12_t[i].wind_code,0,sizeof(new_12_t[i].wind_code));
    new_12_t[i].trade_time=WMDF_CONSTS_32BIT_NIL_VALUE;
    new_12_t[i].set_no=WMDF_CONSTS_32BIT_NIL_VALUE;
    new_12_t[i].rec_no=WMDF_CONSTS_32BIT_NIL_VALUE;
    new_12_t[i].buyOrderRecNo=WMDF_CONSTS_32BIT_NIL_VALUE;
    new_12_t[i].sellOrderRecNo=WMDF_CONSTS_32BIT_NIL_VALUE;
    new_12_t[i].price=WMDF_CONSTS_DOUBLE_NIL_VALUE;
    new_12_t[i].volume=WMDF_CONSTS_32BIT_NIL_VALUE;
    new_12_t[i].order_kind=WMDF_CONSTS_8BIT_NIL_VALUE;
    new_12_t[i].function_code=WMDF_CONSTS_8BIT_NIL_VALUE;
    new_12_t[i].amount=WMDF_CONSTS_DOUBLE_NIL_VALUE;
  }

  int32_t k=0;
  for(uint32_t i=0;i<security_containers_.size();i++)
  {
	  SecurityContainer* sc=security_containers_[i];
	  string windcode = sc->windcode();
  	  for(int32_t j=0;j<sc->indexes_count();j++)
	  {
		  IndexBase* index_temp=sc->index(j);
	    if(index_temp->get_index()!=IND_L2_TRANSACTIONS)
	    	continue;
	    else
	   {
		   ComplexIndex* ci=(ComplexIndex*)index_temp;
       L2_TransactionItem* old_l2_t = (L2_TransactionItem*)(ci->struct_);
       int32_t temp = k;

       for(int32_t m=0;m<ci->unit_count;m++,k++)
       {
         memcpy(new_12_t[k].wind_code,windcode.c_str(),windcode.size());
         uint32_t hmsms = (uint32_t)old_l2_t[m].TradeTime;
         new_12_t[k].trade_time = DateTime::hmsms2time(hmsms*10);
         new_12_t[k].rec_no = (int32_t)old_l2_t[m].TradeIndex;
         new_12_t[k].price = old_l2_t[m].TradePrice;
         new_12_t[k].volume = (int32_t)old_l2_t[m].TradeQty;
       }
       IndexBase* index_of_STREAMPACKET=NULL;
       if(j+1<sc->indexes_count() && sc->index(j+1)->get_index()==IND_L2_STREAMPACKET)
       {
		     k=temp;
         index_of_STREAMPACKET=sc->index(j+1);
         ci=(ComplexIndex*)index_of_STREAMPACKET;
         int32_t* tradetype_arr=(int32_t*)(ci->struct_);
         int32_t tradetype_num=ci->unit_count;
         for(int32_t m=0;m<tradetype_num;m++,k++)
         {
           uint32_t temp = tradetype_arr[m];
           int8_t order_kind=(uint8_t)(temp/1000);
           int8_t function_code=(uint8_t)(temp-order_kind*1000);
           new_12_t[k].order_kind=order_kind;
           new_12_t[k].function_code=function_code;
         }
		     j++;
       }
      }
    }
  }
  return new_12_t;
}



SZSEL2Order* wmdf::SecurityContainerManager::TransferToSZSEL2Orders()
{
  SZSEL2Order* l2_o=new SZSEL2Order[num_of_l2orders_];
  for(int32_t i=0;i<num_of_l2orders_;i++)
  {
		memset(l2_o[i].wind_code,0,sizeof(l2_o[i].wind_code));
    l2_o[i].set_no=WMDF_CONSTS_32BIT_NIL_VALUE;
    l2_o[i].rec_no=WMDF_CONSTS_32BIT_NIL_VALUE;
    l2_o[i].price=WMDF_CONSTS_DOUBLE_NIL_VALUE;
    l2_o[i].size=WMDF_CONSTS_32BIT_NIL_VALUE;
    l2_o[i].order_time=WMDF_CONSTS_32BIT_NIL_VALUE;
    l2_o[i].order_kind=WMDF_CONSTS_8BIT_NIL_VALUE;
    l2_o[i].function_code=WMDF_CONSTS_8BIT_NIL_VALUE;
  }
  int32_t k=0;
  for(uint32_t i=0;i<security_containers_.size();i++)
  {
    SecurityContainer* sc=security_containers_[i];
    string windcode=sc->windcode();
    for(int32_t j=0;j<sc->indexes_count();j++)
    {
      IndexBase* index_temp=sc->index(j);
      if(index_temp->get_index()!=IND_L2_ORDERS)
        continue;
      else
      {
		    ComplexIndex* ci=(ComplexIndex*)index_temp;
        L2_OrderItem* old_l2_o = (L2_OrderItem*)(ci->struct_);
        int32_t temp=k;
        for(int32_t m=0;m<ci->unit_count;m++,k++)
        {
          memcpy(l2_o[k].wind_code,windcode.c_str(),windcode.size());
          uint32_t hms = (uint32_t)old_l2_o[m].OrderTime;
          l2_o[k].order_time = DateTime::hms2time(hms)*10;
          l2_o[k].rec_no = (int32_t)old_l2_o[m].RecNo;
          l2_o[k].price = old_l2_o[m].OrderPrice;
          l2_o[k].size = (int32_t)old_l2_o[m].OrderQty;
          l2_o[k].order_kind=(char)old_l2_o[m].OrderType;
        }
      }
    }
  }
  return l2_o;
}



L2MarketSnapshot* wmdf::SecurityContainerManager::TransferToSZSEL2MarketSnapshot()
{
  L2MarketSnapshot* marketsnapshot = new L2MarketSnapshot[num_of_l2marketsnapshot_];

  for(int32_t i=0;i<num_of_l2marketsnapshot_;i++)
  {
  	memset(marketsnapshot[i].wind_code,0,sizeof(marketsnapshot[i].wind_code));
    marketsnapshot[i].data_time_stamp=WMDF_CONSTS_32BIT_NIL_VALUE;
    marketsnapshot[i].pre_close=WMDF_CONSTS_DOUBLE_NIL_VALUE;
    marketsnapshot[i].open_price=WMDF_CONSTS_DOUBLE_NIL_VALUE;
    marketsnapshot[i].high_price=WMDF_CONSTS_DOUBLE_NIL_VALUE;
    marketsnapshot[i].low_price=WMDF_CONSTS_DOUBLE_NIL_VALUE;
    marketsnapshot[i].new_price=WMDF_CONSTS_DOUBLE_NIL_VALUE;
    marketsnapshot[i].total_volume=WMDF_CONSTS_64BIT_NIL_VALUE;
    marketsnapshot[i].total_amount=WMDF_CONSTS_DOUBLE_NIL_VALUE;
    marketsnapshot[i].bid_price1=WMDF_CONSTS_DOUBLE_NIL_VALUE;
    marketsnapshot[i].bid_price2=WMDF_CONSTS_DOUBLE_NIL_VALUE;
    marketsnapshot[i].bid_price3=WMDF_CONSTS_DOUBLE_NIL_VALUE;
    marketsnapshot[i].bid_price4=WMDF_CONSTS_DOUBLE_NIL_VALUE;
    marketsnapshot[i].bid_price5=WMDF_CONSTS_DOUBLE_NIL_VALUE;
    marketsnapshot[i].bid_price6=WMDF_CONSTS_DOUBLE_NIL_VALUE;
    marketsnapshot[i].bid_price7=WMDF_CONSTS_DOUBLE_NIL_VALUE;
    marketsnapshot[i].bid_price8=WMDF_CONSTS_DOUBLE_NIL_VALUE;
    marketsnapshot[i].bid_price9=WMDF_CONSTS_DOUBLE_NIL_VALUE;
    marketsnapshot[i].bid_price10=WMDF_CONSTS_DOUBLE_NIL_VALUE;
    marketsnapshot[i].ask_price1=WMDF_CONSTS_DOUBLE_NIL_VALUE;
    marketsnapshot[i].ask_price2=WMDF_CONSTS_DOUBLE_NIL_VALUE;
    marketsnapshot[i].ask_price3=WMDF_CONSTS_DOUBLE_NIL_VALUE;
    marketsnapshot[i].ask_price4=WMDF_CONSTS_DOUBLE_NIL_VALUE;
    marketsnapshot[i].ask_price5=WMDF_CONSTS_DOUBLE_NIL_VALUE;
    marketsnapshot[i].ask_price6=WMDF_CONSTS_DOUBLE_NIL_VALUE;
    marketsnapshot[i].ask_price7=WMDF_CONSTS_DOUBLE_NIL_VALUE;
    marketsnapshot[i].ask_price8=WMDF_CONSTS_DOUBLE_NIL_VALUE;
    marketsnapshot[i].ask_price9=WMDF_CONSTS_DOUBLE_NIL_VALUE;
    marketsnapshot[i].ask_price10=WMDF_CONSTS_DOUBLE_NIL_VALUE;
    marketsnapshot[i].ask_size1=WMDF_CONSTS_32BIT_NIL_VALUE;
    marketsnapshot[i].ask_size2=WMDF_CONSTS_32BIT_NIL_VALUE;
    marketsnapshot[i].ask_size3=WMDF_CONSTS_32BIT_NIL_VALUE;
    marketsnapshot[i].ask_size4=WMDF_CONSTS_32BIT_NIL_VALUE;
    marketsnapshot[i].ask_size5=WMDF_CONSTS_32BIT_NIL_VALUE;
    marketsnapshot[i].ask_size6=WMDF_CONSTS_32BIT_NIL_VALUE;
    marketsnapshot[i].ask_size7=WMDF_CONSTS_32BIT_NIL_VALUE;
    marketsnapshot[i].ask_size8=WMDF_CONSTS_32BIT_NIL_VALUE;
    marketsnapshot[i].ask_size9=WMDF_CONSTS_32BIT_NIL_VALUE;
    marketsnapshot[i].ask_size10=WMDF_CONSTS_32BIT_NIL_VALUE;
    marketsnapshot[i].bid_size1=WMDF_CONSTS_32BIT_NIL_VALUE;
    marketsnapshot[i].bid_size2=WMDF_CONSTS_32BIT_NIL_VALUE;
    marketsnapshot[i].bid_size3=WMDF_CONSTS_32BIT_NIL_VALUE;
    marketsnapshot[i].bid_size4=WMDF_CONSTS_32BIT_NIL_VALUE;
    marketsnapshot[i].bid_size5=WMDF_CONSTS_32BIT_NIL_VALUE;
    marketsnapshot[i].bid_size6=WMDF_CONSTS_32BIT_NIL_VALUE;
    marketsnapshot[i].bid_size7=WMDF_CONSTS_32BIT_NIL_VALUE;
    marketsnapshot[i].bid_size8=WMDF_CONSTS_32BIT_NIL_VALUE;
    marketsnapshot[i].bid_size9=WMDF_CONSTS_32BIT_NIL_VALUE;
    marketsnapshot[i].bid_size10=WMDF_CONSTS_32BIT_NIL_VALUE;
    marketsnapshot[i].transactions_count=WMDF_CONSTS_64BIT_NIL_VALUE;
    marketsnapshot[i].weighted_average_bid_price=WMDF_CONSTS_DOUBLE_NIL_VALUE;
    marketsnapshot[i].weighted_average_ask_price=WMDF_CONSTS_DOUBLE_NIL_VALUE;
    marketsnapshot[i].total_bid_size=WMDF_CONSTS_64BIT_NIL_VALUE;
    marketsnapshot[i].total_ask_size=WMDF_CONSTS_64BIT_NIL_VALUE;
		marketsnapshot[i].peratio1 = WMDF_CONSTS_DOUBLE_NIL_VALUE;
		marketsnapshot[i].peratio2 = WMDF_CONSTS_DOUBLE_NIL_VALUE;
  }

  int k=0;

  for(uint32_t i=0;i<security_containers_.size();i++)
  {
    //bool hasSimpleIndex=false;
    bool isValidIndex=false;
	  SecurityContainer* sc=security_containers_[i];
	  string windcode = sc->windcode();
    memcpy(marketsnapshot[k].wind_code,windcode.c_str(),windcode.size());

    assert(strcmp(marketsnapshot[k].wind_code,"")!=0);

	  for(int32_t j=0;j<sc->indexes_count();j++)
	  {
	    IndexBase* index_temp=sc->index(j);
	    if(index_temp->indextype()!=SIMPLE_INDEX)
		    continue;
	    else
	    {
	  	  //hasSimpleIndex=true;
	  	  uint16_t ii=((SimpleIndex*)index_temp)->get_index();
	  	  int64_t vv=((SimpleIndex*)index_temp)->value_;
	  	  switch(ii)
		    {
		    case IND_TRADEDATE:
		      //marketsnapshot[k].trade_date = DateTime::ymd2date((int32_t)vv);
          isValidIndex=true;
	  	    break;
		    case IND_TRADETIME:
		      marketsnapshot[k].data_time_stamp = DateTime::hms2time((int32_t)vv)*1000;
          isValidIndex=true;
		      break;
	  	  case IND_NEWVALUE:
		      marketsnapshot[k].new_price = (double)vv/1000U;
          isValidIndex=true;
		      break;
	  	  case IND_PREVCLOSE:
	  	    marketsnapshot[k].pre_close = (double)vv/1000U;
          isValidIndex=true;
		      break;
	  	  case IND_TODAYOPEN:
		      marketsnapshot[k].open_price = (double)vv/1000U;
          isValidIndex=true;
		      break;
		    case IND_TODAYHIGH:
		      marketsnapshot[k].high_price = (double)vv/1000U;
          isValidIndex=true;
		      break;
	  	  case IND_TODAYLOW:
		      marketsnapshot[k].low_price = (double)vv/1000U;
          isValidIndex=true;
		      break;
		    case IND_TOTALVOLUME:
		      marketsnapshot[k].total_volume = vv;
          isValidIndex=true;
		      break;
		    case IND_BIDPRICE1:
		      marketsnapshot[k].bid_price1 = (double)vv/1000U;
          isValidIndex=true;
		      break;
		    case IND_BIDPRICE2:
		      marketsnapshot[k].bid_price2 = (double)vv/1000U;
          isValidIndex=true;
		      break;
	  	  case IND_BIDPRICE3:
          marketsnapshot[k].bid_price3 = (double)vv/1000U;
          isValidIndex=true;
		      break;
		    case IND_BIDPRICE4:
          marketsnapshot[k].bid_price4 = (double)vv/1000U;
          isValidIndex=true;
		      break;
  		  case IND_BIDPRICE5:
		      marketsnapshot[k].bid_price5 = (double)vv/1000U;
          isValidIndex=true;
		      break;
		    case IND_BIDPRICE6:
		      marketsnapshot[k].bid_price6 = (double)vv/1000U;
          isValidIndex=true;
		      break;
		    case IND_BIDPRICE7:
		      marketsnapshot[k].bid_price7 = (double)vv/1000U;
          isValidIndex=true;
		      break;
		    case IND_BIDPRICE8:
		      marketsnapshot[k].bid_price8 = (double)vv/1000U;
          isValidIndex=true;
		      break;
		    case IND_BIDPRICE9:
		      marketsnapshot[k].bid_price9 = (double)vv/1000U;
          isValidIndex=true;
		      break;
		    case IND_BIDPRICE10:
		      marketsnapshot[k].bid_price10 = (double)vv/1000U;
          isValidIndex=true;
		      break;
	  	  case IND_ASKPRICE1:
		      marketsnapshot[k].ask_price1 = (double)vv/1000U;
          isValidIndex=true;
		      break;
		    case IND_ASKPRICE2:
		      marketsnapshot[k].ask_price2 = (double)vv/1000U;
          isValidIndex=true;
		      break;
		    case IND_ASKPRICE3:
		      marketsnapshot[k].ask_price3 = (double)vv/1000U;
          isValidIndex=true;
		      break;
		    case IND_ASKPRICE4:
		      marketsnapshot[k].ask_price4 = (double)vv/1000U;
          isValidIndex=true;
		      break;
	  	  case IND_ASKPRICE5:
		      marketsnapshot[k].ask_price5 = (double)vv/1000U;
          isValidIndex=true;
		      break;
		    case IND_ASKPRICE6:
		      marketsnapshot[k].ask_price6 = (double)vv/1000U;
          isValidIndex=true;
		      break;
		    case IND_ASKPRICE7:
		      marketsnapshot[k].ask_price7 = (double)vv/1000U;
          isValidIndex=true;
		      break;
		    case IND_ASKPRICE8:
		      marketsnapshot[k].ask_price8 = (double)vv/1000U;
          isValidIndex=true;
		      break;
		    case IND_ASKPRICE9:
		      marketsnapshot[k].ask_price9 = (double)vv/1000U;
          isValidIndex=true;
		      break;
		    case IND_ASKPRICE10:
		      marketsnapshot[k].ask_price10 = (double)vv/1000U;
          isValidIndex=true;
		      break;
		    case IND_BIDVOLUME1:
		      marketsnapshot[k].bid_size1 = (int32_t)vv;
          isValidIndex=true;
		      break;
		    case IND_BIDVOLUME2:
		      marketsnapshot[k].bid_size2 = (int32_t)vv;
          isValidIndex=true;
		      break;
		    case IND_BIDVOLUME3:
		      marketsnapshot[k].bid_size3 = (int32_t)vv;
          isValidIndex=true;
		      break;
		    case IND_BIDVOLUME4:
		      marketsnapshot[k].bid_size4 =(int32_t) vv;
          isValidIndex=true;
		      break;
		    case IND_BIDVOLUME5:
		      marketsnapshot[k].bid_size5 =(int32_t) vv;
          isValidIndex=true;
		      break;
		    case IND_BIDVOLUME6:
		      marketsnapshot[k].bid_size6 = (int32_t)vv;
          isValidIndex=true;
		      break;
		    case IND_BIDVOLUME7:
		      marketsnapshot[k].bid_size7 = (int32_t)vv;
          isValidIndex=true;
		      break;
		    case IND_BIDVOLUME8:
		      marketsnapshot[k].bid_size8 = (int32_t)vv;
          isValidIndex=true;
		      break;
		    case IND_BIDVOLUME9:
		      marketsnapshot[k].bid_size9 = (int32_t)vv;
          isValidIndex=true;
		      break;
		    case IND_BIDVOLUME10:
		      marketsnapshot[k].bid_size10 = (int32_t)vv;
          isValidIndex=true;
		      break;
		    case IND_ASKVOLUME1:
		      marketsnapshot[k].ask_size1 = (int32_t)vv;
          isValidIndex=true;
		      break;
		    case IND_ASKVOLUME2:
		      marketsnapshot[k].ask_size2 = (int32_t)vv;
          isValidIndex=true;
		      break;
		    case IND_ASKVOLUME3:
		      marketsnapshot[k].ask_size3 = (int32_t)vv;
          isValidIndex=true;
		      break;
		    case IND_ASKVOLUME4:
		      marketsnapshot[k].ask_size4 = (int32_t)vv;
          isValidIndex=true;
		      break;
		    case IND_ASKVOLUME5:
		      marketsnapshot[k].ask_size5 = (int32_t)vv;
          isValidIndex=true;
		      break;
		    case IND_ASKVOLUME6:
		      marketsnapshot[k].ask_size6 = (int32_t)vv;
          isValidIndex=true;
		      break;
		    case IND_ASKVOLUME7:
		      marketsnapshot[k].ask_size7 = (int32_t)vv;
          isValidIndex=true;
		      break;
		    case IND_ASKVOLUME8:
		      marketsnapshot[k].ask_size8 = (int32_t)vv;
          isValidIndex=true;
		      break;
		    case IND_ASKVOLUME9:
		      marketsnapshot[k].ask_size9 = (int32_t)vv;
		      isValidIndex=true;
          break;
		    case IND_ASKVOLUME10:
		      marketsnapshot[k].ask_size10 = (int32_t)vv;
          isValidIndex=true;
		      break;
		    case IND_TOTALAMOUNT:
		      marketsnapshot[k].total_amount = (double)vv;
          isValidIndex=true;
		      break;
		    case IND_TRANSACTIONS_COUNT:
		      marketsnapshot[k].transactions_count = vv;
          isValidIndex=true;
		      break;
				case IND_IOPV:
					marketsnapshot[k].peratio2 = (double)vv/1000;
					isValidIndex=true;
					break;
				case IND_L2_TOTALBIDQTY:
					marketsnapshot[k].total_bid_size = vv;
					isValidIndex = true;
					break;
				case IND_L2_TOTALOFFERQTY:
					marketsnapshot[k].total_ask_size = vv;
					isValidIndex = true;
					break;
				case IND_WEIGHTED_AVERAGE_BEST_BID:
					marketsnapshot[k].weighted_average_bid_price = (double)vv;
					isValidIndex = true;
					break;
				case IND_WEIGHTED_AVERAGE_BEST_ASK:
					marketsnapshot[k].weighted_average_ask_price = (double)vv;
					isValidIndex = true;
					break;
		    default:
		      break;
		    }
      }
	  }
	  if( isValidIndex)
	    k++;
    if(k>=num_of_l2marketsnapshot_)
      break;
  }
  return marketsnapshot;
}

L2MarketSnapshot* wmdf::SecurityContainerManager::TranseferToSSEL2MarketSnapshot()
{
  L2MarketSnapshot* marketsnapshot = new L2MarketSnapshot[num_of_l2marketsnapshot_];

  for(int32_t i=0;i<num_of_l2marketsnapshot_;i++)
  {
		memset(marketsnapshot[i].wind_code,0,sizeof(marketsnapshot[i].wind_code));
    marketsnapshot[i].data_time_stamp=WMDF_CONSTS_32BIT_NIL_VALUE;
    marketsnapshot[i].pre_close=WMDF_CONSTS_DOUBLE_NIL_VALUE;
    marketsnapshot[i].open_price=WMDF_CONSTS_DOUBLE_NIL_VALUE;
    marketsnapshot[i].high_price=WMDF_CONSTS_DOUBLE_NIL_VALUE;
    marketsnapshot[i].low_price=WMDF_CONSTS_DOUBLE_NIL_VALUE;
    marketsnapshot[i].new_price=WMDF_CONSTS_DOUBLE_NIL_VALUE;
    marketsnapshot[i].total_volume=WMDF_CONSTS_64BIT_NIL_VALUE;
    marketsnapshot[i].total_amount=WMDF_CONSTS_DOUBLE_NIL_VALUE;
    marketsnapshot[i].bid_price1=WMDF_CONSTS_DOUBLE_NIL_VALUE;
    marketsnapshot[i].bid_price2=WMDF_CONSTS_DOUBLE_NIL_VALUE;
    marketsnapshot[i].bid_price3=WMDF_CONSTS_DOUBLE_NIL_VALUE;
    marketsnapshot[i].bid_price4=WMDF_CONSTS_DOUBLE_NIL_VALUE;
    marketsnapshot[i].bid_price5=WMDF_CONSTS_DOUBLE_NIL_VALUE;
    marketsnapshot[i].bid_price6=WMDF_CONSTS_DOUBLE_NIL_VALUE;
    marketsnapshot[i].bid_price7=WMDF_CONSTS_DOUBLE_NIL_VALUE;
    marketsnapshot[i].bid_price8=WMDF_CONSTS_DOUBLE_NIL_VALUE;
    marketsnapshot[i].bid_price9=WMDF_CONSTS_DOUBLE_NIL_VALUE;
    marketsnapshot[i].bid_price10=WMDF_CONSTS_DOUBLE_NIL_VALUE;
    marketsnapshot[i].ask_price1=WMDF_CONSTS_DOUBLE_NIL_VALUE;
    marketsnapshot[i].ask_price2=WMDF_CONSTS_DOUBLE_NIL_VALUE;
    marketsnapshot[i].ask_price3=WMDF_CONSTS_DOUBLE_NIL_VALUE;
    marketsnapshot[i].ask_price4=WMDF_CONSTS_DOUBLE_NIL_VALUE;
    marketsnapshot[i].ask_price5=WMDF_CONSTS_DOUBLE_NIL_VALUE;
    marketsnapshot[i].ask_price6=WMDF_CONSTS_DOUBLE_NIL_VALUE;
    marketsnapshot[i].ask_price7=WMDF_CONSTS_DOUBLE_NIL_VALUE;
    marketsnapshot[i].ask_price8=WMDF_CONSTS_DOUBLE_NIL_VALUE;
    marketsnapshot[i].ask_price9=WMDF_CONSTS_DOUBLE_NIL_VALUE;
    marketsnapshot[i].ask_price10=WMDF_CONSTS_DOUBLE_NIL_VALUE;
    marketsnapshot[i].ask_size1=WMDF_CONSTS_32BIT_NIL_VALUE;
    marketsnapshot[i].ask_size2=WMDF_CONSTS_32BIT_NIL_VALUE;
    marketsnapshot[i].ask_size3=WMDF_CONSTS_32BIT_NIL_VALUE;
    marketsnapshot[i].ask_size4=WMDF_CONSTS_32BIT_NIL_VALUE;
    marketsnapshot[i].ask_size5=WMDF_CONSTS_32BIT_NIL_VALUE;
    marketsnapshot[i].ask_size6=WMDF_CONSTS_32BIT_NIL_VALUE;
    marketsnapshot[i].ask_size7=WMDF_CONSTS_32BIT_NIL_VALUE;
    marketsnapshot[i].ask_size8=WMDF_CONSTS_32BIT_NIL_VALUE;
    marketsnapshot[i].ask_size9=WMDF_CONSTS_32BIT_NIL_VALUE;
    marketsnapshot[i].ask_size10=WMDF_CONSTS_32BIT_NIL_VALUE;
    marketsnapshot[i].bid_size1=WMDF_CONSTS_32BIT_NIL_VALUE;
    marketsnapshot[i].bid_size2=WMDF_CONSTS_32BIT_NIL_VALUE;
    marketsnapshot[i].bid_size3=WMDF_CONSTS_32BIT_NIL_VALUE;
    marketsnapshot[i].bid_size4=WMDF_CONSTS_32BIT_NIL_VALUE;
    marketsnapshot[i].bid_size5=WMDF_CONSTS_32BIT_NIL_VALUE;
    marketsnapshot[i].bid_size6=WMDF_CONSTS_32BIT_NIL_VALUE;
    marketsnapshot[i].bid_size7=WMDF_CONSTS_32BIT_NIL_VALUE;
    marketsnapshot[i].bid_size8=WMDF_CONSTS_32BIT_NIL_VALUE;
    marketsnapshot[i].bid_size9=WMDF_CONSTS_32BIT_NIL_VALUE;
    marketsnapshot[i].bid_size10=WMDF_CONSTS_32BIT_NIL_VALUE;
    marketsnapshot[i].transactions_count=WMDF_CONSTS_64BIT_NIL_VALUE;
    marketsnapshot[i].weighted_average_bid_price=WMDF_CONSTS_DOUBLE_NIL_VALUE;
    marketsnapshot[i].weighted_average_ask_price=WMDF_CONSTS_DOUBLE_NIL_VALUE;
    marketsnapshot[i].total_bid_size=WMDF_CONSTS_64BIT_NIL_VALUE;
    marketsnapshot[i].total_ask_size=WMDF_CONSTS_64BIT_NIL_VALUE;
		marketsnapshot[i].peratio1 = WMDF_CONSTS_DOUBLE_NIL_VALUE;
		marketsnapshot[i].peratio2 = WMDF_CONSTS_DOUBLE_NIL_VALUE;
  }

  int k=0;

  for(uint32_t i=0;i<security_containers_.size();i++)
  {
    bool isValidIndex=false;
    SecurityContainer* sc=security_containers_[i];
    string windcode = sc->windcode();
    memcpy(marketsnapshot[k].wind_code,windcode.c_str(),windcode.size());
    for(int32_t j=0;j<sc->indexes_count();j++)
    {
      IndexBase* index_temp=sc->index(j);
      if(index_temp->indextype()!=SIMPLE_INDEX)
        continue;
      else
      {
        uint16_t ii=((SimpleIndex*)index_temp)->get_index();
        int64_t vv=((SimpleIndex*)index_temp)->value_;
        switch(ii)
        {
        case IND_TRADEDATE:
          //marketsnapshot[k].trade_date = DateTime::ymd2date((int32_t)vv);
          isValidIndex=true;
          break;
        case IND_TRADETIME:
          marketsnapshot[k].data_time_stamp = DateTime::hms2time((int32_t)vv)*1000;
          isValidIndex=true;
          break;
        case IND_NEWVALUE:
          marketsnapshot[k].new_price = (double)vv/1000U;
          isValidIndex=true;
          break;
        case IND_PREVCLOSE:
          marketsnapshot[k].pre_close = (double)vv/1000U;
          isValidIndex=true;
          break;
        case IND_TODAYOPEN:
          marketsnapshot[k].open_price = (double)vv/1000U;
          isValidIndex=true;
          break;
        case IND_TODAYHIGH:
          marketsnapshot[k].high_price = (double)vv/1000U;
          isValidIndex=true;
          break;
        case IND_TODAYLOW:
          marketsnapshot[k].low_price = (double)vv/1000U;
          isValidIndex=true;
          break;
        case IND_TOTALVOLUME:
          marketsnapshot[k].total_volume = vv;
          isValidIndex=true;
          break;
        case IND_BIDPRICE1:
          marketsnapshot[k].bid_price1 = (double)vv/1000U;
          isValidIndex=true;
          break;
        case IND_BIDPRICE2:
          marketsnapshot[k].bid_price2 = (double)vv/1000U;
          isValidIndex=true;
          break;
        case IND_BIDPRICE3:
          marketsnapshot[k].bid_price3 = (double)vv/1000U;
          isValidIndex=true;
          break;
        case IND_BIDPRICE4:
          marketsnapshot[k].bid_price4 = (double)vv/1000U;
          isValidIndex=true;
          break;
        case IND_BIDPRICE5:
          marketsnapshot[k].bid_price5 = (double)vv/1000U;
          isValidIndex=true;
          break;
        case IND_BIDPRICE6:
          marketsnapshot[k].bid_price6 = (double)vv/1000U;
          isValidIndex=true;
          break;
        case IND_BIDPRICE7:
          marketsnapshot[k].bid_price7 = (double)vv/1000U;
          isValidIndex=true;
          break;
        case IND_BIDPRICE8:
          marketsnapshot[k].bid_price8 = (double)vv/1000U;
          isValidIndex=true;
          break;
        case IND_BIDPRICE9:
          marketsnapshot[k].bid_price9 = (double)vv/1000U;
          isValidIndex=true;
          break;
        case IND_BIDPRICE10:
          marketsnapshot[k].bid_price10 = (double)vv/1000U;
          isValidIndex=true;
          break;
        case IND_ASKPRICE1:
          marketsnapshot[k].ask_price1 = (double)vv/1000U;
          isValidIndex=true;
          break;
        case IND_ASKPRICE2:
          marketsnapshot[k].ask_price2 = (double)vv/1000U;
          isValidIndex=true;
          break;
        case IND_ASKPRICE3:
          marketsnapshot[k].ask_price3 = (double)vv/1000U;
          isValidIndex=true;
          break;
        case IND_ASKPRICE4:
          marketsnapshot[k].ask_price4 = (double)vv/1000U;
          isValidIndex=true;
          break;
        case IND_ASKPRICE5:
          marketsnapshot[k].ask_price5 = (double)vv/1000U;
          isValidIndex=true;
          break;
        case IND_ASKPRICE6:
          marketsnapshot[k].ask_price6 = (double)vv/1000U;
          isValidIndex=true;
          break;
        case IND_ASKPRICE7:
          marketsnapshot[k].ask_price7 = (double)vv/1000U;
          isValidIndex=true;
          break;
        case IND_ASKPRICE8:
          marketsnapshot[k].ask_price8 = (double)vv/1000U;
          isValidIndex=true;
          break;
        case IND_ASKPRICE9:
          marketsnapshot[k].ask_price9 = (double)vv/1000U;
          isValidIndex=true;
          break;
        case IND_ASKPRICE10:
          marketsnapshot[k].ask_price10 = (double)vv/1000U;
          isValidIndex=true;
          break;
        case IND_BIDVOLUME1:
          marketsnapshot[k].bid_size1 = (int32_t)vv;
          isValidIndex=true;
          break;
        case IND_BIDVOLUME2:
          marketsnapshot[k].bid_size2 = (int32_t)vv;
          isValidIndex=true;
          break;
        case IND_BIDVOLUME3:
          marketsnapshot[k].bid_size3 = (int32_t)vv;
          isValidIndex=true;
          break;
        case IND_BIDVOLUME4:
          marketsnapshot[k].bid_size4 = (int32_t)vv;
          isValidIndex=true;
          break;
        case IND_BIDVOLUME5:
          marketsnapshot[k].bid_size5 = (int32_t)vv;
          isValidIndex=true;
          break;
        case IND_BIDVOLUME6:
          marketsnapshot[k].bid_size6 = (int32_t)vv;
          isValidIndex=true;
          break;
        case IND_BIDVOLUME7:
          marketsnapshot[k].bid_size7 =(int32_t) vv;
          isValidIndex=true;
          break;
        case IND_BIDVOLUME8:
          marketsnapshot[k].bid_size8 =(int32_t) vv;
          isValidIndex=true;
          break;
        case IND_BIDVOLUME9:
          marketsnapshot[k].bid_size9 = (int32_t)vv;
          isValidIndex=true;
          break;
        case IND_BIDVOLUME10:
          marketsnapshot[k].bid_size10 = (int32_t)vv;
          isValidIndex=true;
          break;
        case IND_ASKVOLUME1:
          marketsnapshot[k].ask_size1 = (int32_t)vv;
          isValidIndex=true;
          break;
        case IND_ASKVOLUME2:
          marketsnapshot[k].ask_size2 =(int32_t) vv;
          isValidIndex=true;
          break;
        case IND_ASKVOLUME3:
          marketsnapshot[k].ask_size3 = (int32_t)vv;
          isValidIndex=true;
          break;
        case IND_ASKVOLUME4:
          marketsnapshot[k].ask_size4 = (int32_t)vv;
          isValidIndex=true;
          break;
        case IND_ASKVOLUME5:
          marketsnapshot[k].ask_size5 = (int32_t)vv;
          isValidIndex=true;
          break;
        case IND_ASKVOLUME6:
          marketsnapshot[k].ask_size6 = (int32_t)vv;
          isValidIndex=true;
          break;
        case IND_ASKVOLUME7:
          marketsnapshot[k].ask_size7 = (int32_t)vv;
          isValidIndex=true;
          break;
        case IND_ASKVOLUME8:
          marketsnapshot[k].ask_size8 = (int32_t)vv;
          isValidIndex=true;
          break;
        case IND_ASKVOLUME9:
          marketsnapshot[k].ask_size9 =(int32_t) vv;
          isValidIndex=true;
          break;
        case IND_ASKVOLUME10:
          marketsnapshot[k].ask_size10 = (int32_t)vv;
          isValidIndex=true;
          break;
        case IND_TOTALAMOUNT:
          marketsnapshot[k].total_amount = (double)vv;
          isValidIndex=true;
          break;
        case IND_WEIGHTED_AVERAGE_BEST_BID:
          marketsnapshot[k].weighted_average_bid_price = (double)vv;
          isValidIndex=true;
          break;
        case IND_WEIGHTED_AVERAGE_BEST_ASK:
          marketsnapshot[k].weighted_average_ask_price = (double)vv;
          isValidIndex=true;
          break;
        case IND_L2_TOTALBIDQTY:
          marketsnapshot[k].total_bid_size = vv;
          isValidIndex=true;
          break;
        case IND_L2_TOTALOFFERQTY:
          marketsnapshot[k].total_ask_size =vv;
          isValidIndex=true;
          break;
				case IND_IOPV:
					marketsnapshot[k].peratio2 = (double)vv;
					isValidIndex=true;
					break;
				case IND_YTM:
					marketsnapshot[k].peratio2 = (double)vv;
					isValidIndex=true;
					break;
        default:
          break;
        }
      }
    }
    if( isValidIndex)
      k++;
    if(k>=num_of_l2marketsnapshot_)
      break;
  }
  return marketsnapshot;
}

L2Transaction* wmdf::SecurityContainerManager::TransferToSSEL2Transactions()
{
  L2Transaction* new_12_t = new L2Transaction[num_of_l2transaction_];
  for(int32_t i=0;i<num_of_l2transaction_;i++)
  {
		memset(new_12_t[i].wind_code,0,sizeof(new_12_t[i].wind_code));
    new_12_t[i].trade_time=WMDF_CONSTS_32BIT_NIL_VALUE;
    new_12_t[i].set_no=WMDF_CONSTS_32BIT_NIL_VALUE;
    new_12_t[i].rec_no=WMDF_CONSTS_32BIT_NIL_VALUE;
    new_12_t[i].buyOrderRecNo=WMDF_CONSTS_32BIT_NIL_VALUE;
    new_12_t[i].sellOrderRecNo=WMDF_CONSTS_32BIT_NIL_VALUE;
    new_12_t[i].price=WMDF_CONSTS_DOUBLE_NIL_VALUE;
    new_12_t[i].volume=WMDF_CONSTS_32BIT_NIL_VALUE;
    new_12_t[i].order_kind=WMDF_CONSTS_8BIT_NIL_VALUE;
    new_12_t[i].function_code=WMDF_CONSTS_8BIT_NIL_VALUE;
    new_12_t[i].amount = WMDF_CONSTS_DOUBLE_NIL_VALUE;
  }

  int32_t k=0;
  for(uint32_t i=0;i<security_containers_.size();i++)
  {
    SecurityContainer* sc=security_containers_[i];
    string windcode = sc->windcode();
    for(int32_t j=0;j<sc->indexes_count();j++)
    {
      IndexBase* index_temp=sc->index(j);
      if(index_temp->get_index()!=IND_L2_TRANSACTIONS)
        continue;
      else
      {
		    ComplexIndex* ci = (ComplexIndex*)index_temp;
        L2_TransactionItem* old_l2_t = (L2_TransactionItem*)(ci->struct_);
        int32_t temp = k;
        for(int32_t m=0;m<ci->unit_count;m++,k++)
        {
          memcpy(new_12_t[k].wind_code,windcode.c_str(),windcode.size());
          uint32_t hmsms = (uint32_t)old_l2_t[m].TradeTime;
          new_12_t[k].trade_time = DateTime::hmsms2time(hmsms*10);
          new_12_t[k].rec_no = (int32_t)old_l2_t[m].TradeIndex;
          new_12_t[k].price = old_l2_t[m].TradePrice;
          new_12_t[k].volume = (int32_t)old_l2_t[m].TradeQty;
        }
      }
    }
  }
  return new_12_t;
}

L2OrderQueue* wmdf::SecurityContainerManager::TransferToSSEL2OrderQueue()
{
  L2OrderQueue* oql2=new L2OrderQueue[num_of_l2orderqueue_];
  memset(oql2,0,sizeof(L2OrderQueue)*num_of_l2orderqueue_);
  int32_t k=0;
  for(uint32_t i=0;i<security_containers_.size();i++)
  {
    SecurityContainer* sc = security_containers_[i];
    string windcode = sc->windcode();
    for(int32_t j=0;j<sc->indexes_count();j++)
    {
      IndexBase* index_temp=sc->index(j);
      if(index_temp->get_index()!=IND_L2_ORDER_QUEUE_OFFER
        &&index_temp->get_index()!=IND_L2_ORDER_QUEUE)
        continue;
      else
      {
        L2_OrderQueue* old_l2_oq =(L2_OrderQueue*)((ComplexIndex*)index_temp)->struct_;
        memcpy(oql2[k].wind_code,windcode.c_str(),windcode.size());
        uint32_t hms = old_l2_oq->DateTimeStamp;
        oql2[k].data_time_stamp = DateTime::hms2time((int32_t)hms)*1000;
        oql2[k].order_side = old_l2_oq->Side;
        oql2[k].image_status = (uint32_t)old_l2_oq->ImageStatus;
        oql2[k].price_level_count = old_l2_oq->NoPriceLevel;
        L2OrderQueuePriceLevel* l2_oql = new L2OrderQueuePriceLevel[oql2[k].price_level_count];
        for(int32_t m=0;m<oql2[k].price_level_count;m++)
        {
          l2_oql[m].operator_type = old_l2_oq->PriceLevel[m].PriceLevelOperator;
          l2_oql[m].order_price = ((double)old_l2_oq->PriceLevel[m].Price)/1000U;
          l2_oql[m].total_number = old_l2_oq->PriceLevel[m].NumOfOrders;
          l2_oql[m].orders_number = old_l2_oq->PriceLevel[m].NoOrders;

          L2OrderQueueItem* l2_oqlq = new L2OrderQueueItem[l2_oql[m].orders_number];
          for(int32_t n=0;n<l2_oql[m].orders_number;n++)
          {
            l2_oqlq[n].operator_type = old_l2_oq->PriceLevel[m].Orders[n].PriceLevelOperator;
            l2_oqlq[n].size = old_l2_oq->PriceLevel[m].Orders[n].OrderQty;
            l2_oqlq[n].entry_id = old_l2_oq->PriceLevel[m].Orders[n].OrderQueueOperatorEntryID;
          }
          l2_oql[m].order_items = l2_oqlq;
        }
        oql2[k].price_levels = l2_oql;
        k++;
        if(k>=num_of_l2orderqueue_)
          break;
      }
    }
  }
  return oql2;
}

L1MarketSnapshot* wmdf::SecurityContainerManager::TransferToSZSEL1MarketSnapshot()
{
  L1MarketSnapshot* marketsnapshot = new L1MarketSnapshot[num_of_l2marketsnapshot_];

  for(int32_t i=0;i<num_of_l2marketsnapshot_;i++)
  {
		memset(marketsnapshot[i].wind_code,0,sizeof(marketsnapshot[i].wind_code));
    marketsnapshot[i].data_time_stamp=WMDF_CONSTS_32BIT_NIL_VALUE;
    marketsnapshot[i].pre_close=WMDF_CONSTS_DOUBLE_NIL_VALUE;
    marketsnapshot[i].open_price=WMDF_CONSTS_DOUBLE_NIL_VALUE;
    marketsnapshot[i].high_price=WMDF_CONSTS_DOUBLE_NIL_VALUE;
    marketsnapshot[i].low_price=WMDF_CONSTS_DOUBLE_NIL_VALUE;
    marketsnapshot[i].new_price=WMDF_CONSTS_DOUBLE_NIL_VALUE;
    marketsnapshot[i].total_volume=WMDF_CONSTS_64BIT_NIL_VALUE;
    marketsnapshot[i].total_amount=WMDF_CONSTS_DOUBLE_NIL_VALUE;
    marketsnapshot[i].bid_price1=WMDF_CONSTS_DOUBLE_NIL_VALUE;
    marketsnapshot[i].bid_price2=WMDF_CONSTS_DOUBLE_NIL_VALUE;
    marketsnapshot[i].bid_price3=WMDF_CONSTS_DOUBLE_NIL_VALUE;
    marketsnapshot[i].bid_price4=WMDF_CONSTS_DOUBLE_NIL_VALUE;
    marketsnapshot[i].bid_price5=WMDF_CONSTS_DOUBLE_NIL_VALUE;
    marketsnapshot[i].ask_price1=WMDF_CONSTS_DOUBLE_NIL_VALUE;
    marketsnapshot[i].ask_price2=WMDF_CONSTS_DOUBLE_NIL_VALUE;
    marketsnapshot[i].ask_price3=WMDF_CONSTS_DOUBLE_NIL_VALUE;
    marketsnapshot[i].ask_price4=WMDF_CONSTS_DOUBLE_NIL_VALUE;
    marketsnapshot[i].ask_price5=WMDF_CONSTS_DOUBLE_NIL_VALUE;
    marketsnapshot[i].ask_size1=WMDF_CONSTS_32BIT_NIL_VALUE;
    marketsnapshot[i].ask_size2=WMDF_CONSTS_32BIT_NIL_VALUE;
    marketsnapshot[i].ask_size3=WMDF_CONSTS_32BIT_NIL_VALUE;
    marketsnapshot[i].ask_size4=WMDF_CONSTS_32BIT_NIL_VALUE;
    marketsnapshot[i].ask_size5=WMDF_CONSTS_32BIT_NIL_VALUE;
    marketsnapshot[i].bid_size1=WMDF_CONSTS_32BIT_NIL_VALUE;
    marketsnapshot[i].bid_size2=WMDF_CONSTS_32BIT_NIL_VALUE;
    marketsnapshot[i].bid_size3=WMDF_CONSTS_32BIT_NIL_VALUE;
    marketsnapshot[i].bid_size4=WMDF_CONSTS_32BIT_NIL_VALUE;
    marketsnapshot[i].bid_size5=WMDF_CONSTS_32BIT_NIL_VALUE;
  }

  int k=0;

  for(uint32_t i=0;i<security_containers_.size();i++)
  {
    bool isValidIndex=false;
    SecurityContainer* sc=security_containers_[i];
    string windcode = sc->windcode();
    memcpy(marketsnapshot[k].wind_code,windcode.c_str(),windcode.size());
    for(int32_t j=0;j<sc->indexes_count();j++)
    {
      IndexBase* index_temp=sc->index(j);
      if(index_temp->indextype()!=SIMPLE_INDEX)
        continue;
      else
      {
        uint16_t ii=((SimpleIndex*)index_temp)->get_index();
        int64_t vv=((SimpleIndex*)index_temp)->value_;
        switch(ii)
        {
        case IND_TRADEDATE:
          //marketsnapshot[k].trade_date = DateTime::ymd2date((int32_t)vv);
          isValidIndex=true;
          break;
        case IND_TRADETIME:
          marketsnapshot[k].data_time_stamp = DateTime::hms2time((int32_t)vv)*1000;
          isValidIndex=true;
          break;
        case IND_NEWVALUE:
          marketsnapshot[k].new_price = (double)vv/1000U;
          isValidIndex=true;
          break;
        case IND_PREVCLOSE:
          marketsnapshot[k].pre_close = (double)vv/1000U;
          isValidIndex=true;
          break;
        case IND_TODAYOPEN:
          marketsnapshot[k].open_price = (double)vv/1000U;
          isValidIndex=true;
          break;
        case IND_TODAYHIGH:
          marketsnapshot[k].high_price = (double)vv/1000U;
          isValidIndex=true;
          break;
        case IND_TODAYLOW:
          marketsnapshot[k].low_price = (double)vv/1000U;
          isValidIndex=true;
          break;
        case IND_TOTALVOLUME:
          marketsnapshot[k].total_volume = vv ;
          isValidIndex=true;
          break;
        case IND_BIDPRICE1:
          marketsnapshot[k].bid_price1 = (double)vv/1000U;
          isValidIndex=true;
          break;
        case IND_BIDPRICE2:
          marketsnapshot[k].bid_price2 = (double)vv/1000U;
          isValidIndex=true;
          break;
        case IND_BIDPRICE3:
          marketsnapshot[k].bid_price3 = (double)vv/1000U;
          isValidIndex=true;
          break;
        case IND_BIDPRICE4:
          marketsnapshot[k].bid_price4 = (double)vv/1000U;
          isValidIndex=true;
          break;
        case IND_BIDPRICE5:
          marketsnapshot[k].bid_price5 = (double)vv/1000U;
          isValidIndex=true;
          break;
        case IND_ASKPRICE1:
          marketsnapshot[k].ask_price1 = (double)vv/1000U;
          isValidIndex=true;
          break;
        case IND_ASKPRICE2:
          marketsnapshot[k].ask_price2 = (double)vv/1000U;
          isValidIndex=true;
          break;
        case IND_ASKPRICE3:
          marketsnapshot[k].ask_price3 = (double)vv/1000U;
          isValidIndex=true;
          break;
        case IND_ASKPRICE4:
          marketsnapshot[k].ask_price4 = (double)vv/1000U;
          isValidIndex=true;
          break;
        case IND_ASKPRICE5:
          marketsnapshot[k].ask_price5 = (double)vv/1000U;
          isValidIndex=true;
          break;
        case IND_BIDVOLUME1:
          marketsnapshot[k].bid_size1 = (int32_t)vv;
          isValidIndex=true;
          break;
        case IND_BIDVOLUME2:
          marketsnapshot[k].bid_size2 = (int32_t)vv;
          isValidIndex=true;
          break;
        case IND_BIDVOLUME3:
          marketsnapshot[k].bid_size3 = (int32_t)vv;
          isValidIndex=true;
          break;
        case IND_BIDVOLUME4:
          marketsnapshot[k].bid_size4 = (int32_t)vv;
          isValidIndex=true;
          break;
        case IND_BIDVOLUME5:
          marketsnapshot[k].bid_size5 = (int32_t)vv;
          isValidIndex=true;
          break;
        case IND_ASKVOLUME1:
          marketsnapshot[k].ask_size1 =(int32_t) vv;
          isValidIndex=true;
          break;
        case IND_ASKVOLUME2:
          marketsnapshot[k].ask_size2 =(int32_t) vv;
          isValidIndex=true;
          break;
        case IND_ASKVOLUME3:
          marketsnapshot[k].ask_size3 = (int32_t)vv;
          isValidIndex=true;
          break;
        case IND_ASKVOLUME4:
          marketsnapshot[k].ask_size4 = (int32_t)vv;
          isValidIndex=true;
          break;
        case IND_ASKVOLUME5:
          marketsnapshot[k].ask_size5 = (int32_t)vv;
          isValidIndex=true;
          break;
        case IND_TOTALAMOUNT:
					marketsnapshot[k].total_amount = (double)vv;
					isValidIndex=true;
          break;
        case IND_DELTAAMOUNT:
          break;
        case IND_DELTAVOLUME:
          break;
        default:
          break;
        }
      }
    }
    if( isValidIndex)
      k++;
    if(k>=num_of_l2marketsnapshot_)
      break;
  }
  return marketsnapshot;
}


L1MarketSnapshot* wmdf::SecurityContainerManager::TransferToSSEL1MarketSnapshot()
{
	L1MarketSnapshot* marketsnapshot = new L1MarketSnapshot[num_of_l2marketsnapshot_];

	for(int32_t i=0;i<num_of_l2marketsnapshot_;i++)
	{
		memset(marketsnapshot[i].wind_code,0,sizeof(marketsnapshot[i].wind_code));
		marketsnapshot[i].data_time_stamp=WMDF_CONSTS_32BIT_NIL_VALUE;
		marketsnapshot[i].pre_close=WMDF_CONSTS_DOUBLE_NIL_VALUE;
		marketsnapshot[i].open_price=WMDF_CONSTS_DOUBLE_NIL_VALUE;
		marketsnapshot[i].high_price=WMDF_CONSTS_DOUBLE_NIL_VALUE;
		marketsnapshot[i].low_price=WMDF_CONSTS_DOUBLE_NIL_VALUE;
		marketsnapshot[i].new_price=WMDF_CONSTS_DOUBLE_NIL_VALUE;
		marketsnapshot[i].total_volume=WMDF_CONSTS_64BIT_NIL_VALUE;
		marketsnapshot[i].total_amount=WMDF_CONSTS_DOUBLE_NIL_VALUE;
		marketsnapshot[i].bid_price1=WMDF_CONSTS_DOUBLE_NIL_VALUE;
		marketsnapshot[i].bid_price2=WMDF_CONSTS_DOUBLE_NIL_VALUE;
		marketsnapshot[i].bid_price3=WMDF_CONSTS_DOUBLE_NIL_VALUE;
		marketsnapshot[i].bid_price4=WMDF_CONSTS_DOUBLE_NIL_VALUE;
		marketsnapshot[i].bid_price5=WMDF_CONSTS_DOUBLE_NIL_VALUE;
		marketsnapshot[i].ask_price1=WMDF_CONSTS_DOUBLE_NIL_VALUE;
		marketsnapshot[i].ask_price2=WMDF_CONSTS_DOUBLE_NIL_VALUE;
		marketsnapshot[i].ask_price3=WMDF_CONSTS_DOUBLE_NIL_VALUE;
		marketsnapshot[i].ask_price4=WMDF_CONSTS_DOUBLE_NIL_VALUE;
		marketsnapshot[i].ask_price5=WMDF_CONSTS_DOUBLE_NIL_VALUE;
		marketsnapshot[i].ask_size1=WMDF_CONSTS_32BIT_NIL_VALUE;
		marketsnapshot[i].ask_size2=WMDF_CONSTS_32BIT_NIL_VALUE;
		marketsnapshot[i].ask_size3=WMDF_CONSTS_32BIT_NIL_VALUE;
		marketsnapshot[i].ask_size4=WMDF_CONSTS_32BIT_NIL_VALUE;
		marketsnapshot[i].ask_size5=WMDF_CONSTS_32BIT_NIL_VALUE;
		marketsnapshot[i].bid_size1=WMDF_CONSTS_32BIT_NIL_VALUE;
		marketsnapshot[i].bid_size2=WMDF_CONSTS_32BIT_NIL_VALUE;
		marketsnapshot[i].bid_size3=WMDF_CONSTS_32BIT_NIL_VALUE;
		marketsnapshot[i].bid_size4=WMDF_CONSTS_32BIT_NIL_VALUE;
		marketsnapshot[i].bid_size5=WMDF_CONSTS_32BIT_NIL_VALUE;
	}

	int k=0;

	for(uint32_t i=0;i<security_containers_.size();i++)
	{
		bool isValidIndex=false;
		SecurityContainer* sc=security_containers_[i];
		string windcode = sc->windcode();
		memcpy(marketsnapshot[k].wind_code,windcode.c_str(),windcode.size());
		for(int32_t j=0;j<sc->indexes_count();j++)
		{
			IndexBase* index_temp=sc->index(j);
			if(index_temp->indextype()!=SIMPLE_INDEX)
				continue;
			else
			{
				uint16_t ii=((SimpleIndex*)index_temp)->get_index();
				int64_t vv=((SimpleIndex*)index_temp)->value_;
				switch(ii)
				{
				case IND_TRADEDATE:
					//marketsnapshot[k].trade_date = DateTime::ymd2date((int32_t)vv);
					isValidIndex=true;
					break;
				case IND_TRADETIME:
					marketsnapshot[k].data_time_stamp = DateTime::hms2time((int32_t)vv)*1000;
					isValidIndex=true;
					break;
				case IND_NEWVALUE:
					marketsnapshot[k].new_price = (double)vv/1000U;
					isValidIndex=true;
					break;
				case IND_PREVCLOSE:
					marketsnapshot[k].pre_close = (double)vv/1000U;
					isValidIndex=true;
					break;
				case IND_TODAYOPEN:
					marketsnapshot[k].open_price = (double)vv/1000U;
					isValidIndex=true;
					break;
				case IND_TODAYHIGH:
					marketsnapshot[k].high_price = (double)vv/1000U;
					isValidIndex=true;
					break;
				case IND_TODAYLOW:
					marketsnapshot[k].low_price = (double)vv/1000U;
					isValidIndex=true;
					break;
				case IND_TOTALVOLUME:
					marketsnapshot[k].total_volume = vv ;
					isValidIndex=true;
					break;
				case IND_BIDPRICE1:
					marketsnapshot[k].bid_price1 = (double)vv/1000U;
					isValidIndex=true;
					break;
				case IND_BIDPRICE2:
					marketsnapshot[k].bid_price2 = (double)vv/1000U;
					isValidIndex=true;
					break;
				case IND_BIDPRICE3:
					marketsnapshot[k].bid_price3 = (double)vv/1000U;
					isValidIndex=true;
					break;
				case IND_BIDPRICE4:
					marketsnapshot[k].bid_price4 = (double)vv/1000U;
					isValidIndex=true;
					break;
				case IND_BIDPRICE5:
					marketsnapshot[k].bid_price5 = (double)vv/1000U;
					isValidIndex=true;
					break;
				case IND_ASKPRICE1:
					marketsnapshot[k].ask_price1 = (double)vv/1000U;
					isValidIndex=true;
					break;
				case IND_ASKPRICE2:
					marketsnapshot[k].ask_price2 = (double)vv/1000U;
					isValidIndex=true;
					break;
				case IND_ASKPRICE3:
					marketsnapshot[k].ask_price3 = (double)vv/1000U;
					isValidIndex=true;
					break;
				case IND_ASKPRICE4:
					marketsnapshot[k].ask_price4 = (double)vv/1000U;
					isValidIndex=true;
					break;
				case IND_ASKPRICE5:
					marketsnapshot[k].ask_price5 = (double)vv/1000U;
					isValidIndex=true;
					break;
				case IND_BIDVOLUME1:
					marketsnapshot[k].bid_size1 = (int32_t)vv;
					isValidIndex=true;
					break;
				case IND_BIDVOLUME2:
					marketsnapshot[k].bid_size2 = (int32_t)vv;
					isValidIndex=true;
					break;
				case IND_BIDVOLUME3:
					marketsnapshot[k].bid_size3 = (int32_t)vv;
					isValidIndex=true;
					break;
				case IND_BIDVOLUME4:
					marketsnapshot[k].bid_size4 = (int32_t)vv;
					isValidIndex=true;
					break;
				case IND_BIDVOLUME5:
					marketsnapshot[k].bid_size5 = (int32_t)vv;
					isValidIndex=true;
					break;
				case IND_ASKVOLUME1:
					marketsnapshot[k].ask_size1 =(int32_t) vv;
					isValidIndex=true;
					break;
				case IND_ASKVOLUME2:
					marketsnapshot[k].ask_size2 =(int32_t) vv;
					isValidIndex=true;
					break;
				case IND_ASKVOLUME3:
					marketsnapshot[k].ask_size3 = (int32_t)vv;
					isValidIndex=true;
					break;
				case IND_ASKVOLUME4:
					marketsnapshot[k].ask_size4 = (int32_t)vv;
					isValidIndex=true;
					break;
				case IND_ASKVOLUME5:
					marketsnapshot[k].ask_size5 = (int32_t)vv;
					isValidIndex=true;
					break;
				case IND_TOTALAMOUNT:
					marketsnapshot[k].total_amount = (double)vv;
					isValidIndex=true;
					break;
				case IND_DELTAAMOUNT:
					break;
				case IND_DELTAVOLUME:
					break;
				default:
					break;
				}
			}
		}
					
		if( isValidIndex)
			k++;
		if(k>=num_of_l2marketsnapshot_)
			break;
	}
	return marketsnapshot;
}


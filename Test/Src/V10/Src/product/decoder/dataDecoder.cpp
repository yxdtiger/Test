#include "dataDecoder.h"
#include "indicatorDefs.h"
#include "packetDecoder.h"
#include <string>
#include "securityIndex.h"
#include "../../include/date_time.h"
#include "../../protocol/mdf_message.h"
#include "securityContainer.h"
#include "packetDecoder.h"
#include "old_struct.h"
//#include <fstream>



using namespace std;

//ofstream fout("data_receive.txt");

wmdf::DataDecoder::DataDecoder():scm_(new SecurityContainerManager())
{
}

wmdf::DataDecoder::~DataDecoder()
{
   delete scm_;
   scm_=NULL;
}
//int32_t last_pack_id =-1;
bool wmdf::DataDecoder::DecodeOnePacket( const unsigned long& buflen, uint8_t* buf )
{
  PacketDecoder* pd=new PacketDecoder(buflen,buf);
  uint32_t pack_size = pd->ReadUInt32();  //PackSize
  uint32_t pack_id = pd->ReadUInt32();  //Packid
//   if (last_pack_id > 0)
//   {
//     //assert(pack_id == last_pack_id+1);
//   }
//   last_pack_id = pack_id;
  pd->ReadUInt32();  //SendType
  
  uint32_t code_total = pd->ReadUInt32();  //windcode count;
  for(uint32_t i=0;i<code_total;i++)
  {
    if(!DecodeOneWindCode(pd))
      return false;
  }
  delete pd;
  pd=NULL;
  return true;
}

bool wmdf::DataDecoder::DecodeOneWindCode(PacketDecoder* pd)
{
  string windcode = pd->ReadWindCode();

	//print
	//fout<<windcode<<"   ";

  if(windcode.empty())
    return false;

	//过滤心跳包
	if(!windcode.compare("HB.00"))
	{
		pd->ReadUInt32();
		pd->ReadUInt32();
		pd->ReadInt64();
		return true;
	}


  SecurityContainer* security_container = new SecurityContainer();
  security_container->set_windcode(windcode);

  uint32_t index_count = pd->ReadUInt32();
	//print
	//fout<<index_count<<endl;

  bool hasSimpleIndex=false;
	bool is_bond = IsBond(security_container->windcode().c_str());

  for(uint32_t i=0;i<index_count;i++)
  {
     uint16_t ii = pd->ReadUInt32();
		 //print
		 //fout<<ii<<"   ";

     IndexBase* index_base = NULL;
     switch(ii)
     {
     case IND_MODIFYTIME:
       ReadTime(pd);
       break;
     case IND_L2_ORDER_QUEUE_OFFER:
     case IND_L2_ORDER_QUEUE:
       { 
         index_base = DecodeL2_OrderQueue(pd,is_bond);
       }
       break;
     case IND_L2_TRANSACTIONS:
       {
          index_base = DecodeL2_Transactions(pd,is_bond);
       }
       break;
     case IND_L2_ORDERS:
       {
          index_base = DecodeL2_Orders(pd,is_bond);
       }
       break;
     case IND_L2_STREAMPACKET:
       {
         index_base = DecodeStreamPacket(pd);
       }
       break;
     case IND_STREAM:
       {
         int nLen = pd->ReadUInt32();
         pd->SkipBytes(nLen);
       }
       break;
     default:
			 {
				 index_base = Decode_Int64(ii,pd);
				 if(index_base!=NULL)
					 hasSimpleIndex=true;
			 }
       break;
     }

     if (index_base!=NULL)
     {
       index_base->set_index(ii);
       security_container->add_index(index_base);
       //以下代码调试用
//        if ((ii == IND_TRADETIME))
//        {
//          if (((SimpleIndex*)(index_base))->value_ == 100459)
//          {
//            if (security_container->windcode() == "002550.SZ")
//            {
//              printf("time");
//            }
//            
//          }
//        }

     }
  }
	//fout<<endl;

  //滤掉债券
  if (is_bond)
  {
    delete security_container;
  }
  //其他品种
  else
  {
		if((hasSimpleIndex) && (security_container->indexes_count()>1))
			scm_->l2marketsnapshot_plus();

		scm_->add_securitycontainer(security_container);
  }
  return true;
}

wmdf::IndexBase* wmdf::DataDecoder::DecodeL2_OrderQueue( PacketDecoder* pd,bool is_bond)
{
  L2_OrderQueue* l2_oq=new L2_OrderQueue[1];
  l2_oq[0].DateTimeStamp=WMDF_CONSTS_32BIT_NIL_VALUE;
  l2_oq[0].ImageStatus=WMDF_CONSTS_8BIT_NIL_VALUE;
  l2_oq[0].NoPriceLevel=WMDF_CONSTS_32BIT_NIL_VALUE;
  l2_oq[0].Side=WMDF_CONSTS_8BIT_NIL_VALUE;
  l2_oq[0].PriceLevel=NULL;

  ComplexIndex* complex_index = new ComplexIndex();
  complex_index->struct_=l2_oq;
  complex_index->unit_count=1;

  l2_oq[0].DateTimeStamp = pd->ReadUInt32();
  l2_oq[0].Side = pd->ReadUInt32();
  l2_oq[0].ImageStatus = pd->ReadUInt32();
  l2_oq[0].NoPriceLevel = pd->ReadUInt32();

  ////print l2 orderqueue
  //fout<<"orderqueue\n"<<"-------- "<<l2_oq[0].DateTimeStamp<<"   "<<(int32_t)l2_oq[0].Side<<"   "<<(int32_t)l2_oq[0].ImageStatus<<"   "<<l2_oq[0].NoPriceLevel<<endl;

  L2_OrderQueue_PriceLevel* pricelevel = new L2_OrderQueue_PriceLevel[l2_oq[0].NoPriceLevel];

  for ( unsigned int i = 0; i < l2_oq[0].NoPriceLevel; i++)
  {
    pricelevel[i].PriceLevelOperator = pd->ReadUInt32();
    pricelevel[i].Price = pd->ReadUInt32();
    pricelevel[i].NumOfOrders = pd->ReadUInt32();
    pricelevel[i].NoOrders = pd->ReadUInt32();

    ////print l2 orderqueue's pricelevel
    //fout<<"---------------- "<<(int32_t)(pricelevel[i].PriceLevelOperator)<<"   "<<pricelevel[i].Price<<"   "<<pricelevel[i].NumOfOrders<<"   "<<pricelevel[i].NoOrders<<endl;

    L2_OrderQueueItem* items=new L2_OrderQueueItem[pricelevel[i].NoOrders];
    for (unsigned int j = 0; j < pricelevel[i].NoOrders; j++)
    {
      items[j].PriceLevelOperator = pd->ReadUInt32();
      items[j].OrderQueueOperatorEntryID = pd->ReadUInt32();
      items[j].OrderQty = pd->ReadUInt32(); 

      ////print l2 orderqueue orders
      //fout<<"---------------------------- "<<(int32_t)(items[j].PriceLevelOperator)<<"   "<<items[j].OrderQueueOperatorEntryID<<"   "<<items[j].OrderQty<<endl;

    }
    pricelevel[i].Orders = items;
  }

  l2_oq[0].PriceLevel = pricelevel;
	if (!is_bond)
	{
		scm_->l2orderqueue_plus();
	}
  return complex_index;
}

wmdf::IndexBase* wmdf::DataDecoder::DecodeL2_Transactions( PacketDecoder* pd,bool is_bond )
{ 
  unsigned int n = pd->ReadUInt32();
  if (n == 0)
    return NULL;
  if (pd->m_error_outofrange)
    return NULL;

  ComplexIndex* complex_index = new ComplexIndex();
  complex_index->unit_count = n;
 
  //print transaction count
  //fout<<"transaction count: "<<n<<endl;

  int64_t TmpTradePrice = 0;

  L2_TransactionItem* l2_t = new L2_TransactionItem[n];

  for ( unsigned int i = 0; i < n; i++)
  {
    if(i == 0)
    {
      l2_t[i].TradeNumber = pd->ReadInt64();
      l2_t[i].TradeIndex = pd->ReadInt64();
      l2_t[i].TradeTime = pd->ReadInt64();
      TmpTradePrice = pd->ReadInt64();
      l2_t[i].TradePrice = TmpTradePrice * 0.001;
      l2_t[i].TradeQty = pd->ReadInt64();
      l2_t[i].TradeMoney = pd->ReadInt64();
    }
    else
    {
      l2_t[i].TradeNumber = l2_t[0].TradeNumber + pd->ReadInt64();
      l2_t[i].TradeIndex = l2_t[0].TradeIndex + pd->ReadInt64();
      l2_t[i].TradeTime = l2_t[0].TradeTime + pd->ReadInt64();
      l2_t[i].TradePrice = (TmpTradePrice + pd->ReadInt64())*0.001;
      l2_t[i].TradeQty = l2_t[0].TradeQty + pd->ReadInt64();
      l2_t[i].TradeMoney = l2_t[0].TradeMoney+pd->ReadInt64();
    }
    //print l2 transactions
    //fout<<"------------------- "<<l2_t[i].TradeNumber<<"   "<<l2_t[i].TradeIndex<<"   "<<l2_t[i].TradeTime<<"   "
     //  <<l2_t[i].TradePrice<<"   "<<l2_t[i].TradeQty<<"   "<<l2_t[i].TradeMoney<<endl;
	 if (!is_bond)
	 {
			scm_->l2transaction_plus();
	 }
  }
  complex_index->struct_=l2_t;
  return complex_index;
}



wmdf::IndexBase* wmdf::DataDecoder::DecodeL2_Orders( PacketDecoder* pd ,bool is_bond)
{
  unsigned int n = pd->ReadUInt32();
  if (n == 0)
    return NULL;
  if (pd->m_error_outofrange)
    return NULL;

  ComplexIndex* complex_index = new ComplexIndex();
  complex_index->unit_count = n;

  //print orders count
  //fout<<"orders count: "<<n<<endl;

  int64_t TmpTradePrice = 0;

  L2_OrderItem* l2_q = new L2_OrderItem[n];

  for ( unsigned int i = 0; i < n; i++)
  {
    if(i==0)
    {
      l2_q[i].OrderNumber = pd->ReadInt64();
      l2_q[i].RecNo = pd->ReadInt64();
      TmpTradePrice = pd->ReadUInt64();
      l2_q[i].OrderPrice = TmpTradePrice * 0.001;
      l2_q[i].OrderQty = pd->ReadInt64();
      l2_q[i].OrderTime = pd->ReadInt64();
      l2_q[i].OrderType = pd->ReadInt64();
    }
    else
    {
      l2_q[i].OrderNumber = l2_q[0].OrderNumber + pd->ReadInt64();
      l2_q[i].RecNo = l2_q[0].RecNo + pd->ReadInt64();
      l2_q[i].OrderPrice = (TmpTradePrice + pd->ReadInt64()) * 0.001;
      l2_q[i].OrderQty = l2_q[0].OrderQty + pd->ReadInt64();
      l2_q[i].OrderTime = l2_q[0].OrderTime + pd->ReadInt64();
      l2_q[i].OrderType = l2_q[0].OrderType + pd->ReadInt64();
    }
    //print l2 orders
    //fout<<"------------------- "<<l2_q[i].OrderNumber<<"   "<<l2_q[i].RecNo<<"   "<<l2_q[i].OrderPrice<<"   "<<l2_q[i].OrderQty<<"   "<<l2_q[i].OrderTime<<"   "<<l2_q[i].OrderType<<endl;
		if (!is_bond)
		{
			scm_->l2orders_plus();
		}
    
  }
  complex_index->struct_=l2_q;
  return complex_index;
}

wmdf::IndexBase* wmdf::DataDecoder::DecodeStreamPacket( PacketDecoder* pd )
{
  unsigned int packet_size = pd->ReadUInt32();
  uint8_t* buf = new uint8_t[packet_size];
  pd->ReadBytes(packet_size, buf);

  PacketDecoder* _pd = new PacketDecoder(packet_size, buf);
  delete[] buf;
  buf=NULL;

  uint32_t field_index = _pd->ReadUInt32();

  //print field_index
  //fout<<"field_index= "<<field_index<<endl;

  switch(field_index)
  {
  case 2202: 
    {
      int32_t count=(int32_t)_pd->ReadUInt32();    
      int32_t* tradetype_arr = new int32_t[count];
      for(int32_t i=0;i<count;i++)
      {
        uint32_t tradetype=(uint32_t)_pd->ReadInt64();

        //print tradetype
        //fout<<"------------------"<<tradetype<<endl;

        tradetype_arr[i] = tradetype;
      }
      delete _pd;
      _pd=NULL;
      ComplexIndex* complex_index=new ComplexIndex();
	    complex_index->unit_count=count;
      complex_index->struct_=tradetype_arr;
      return complex_index;
    }
    break;
  case IND_SHORTNAME_CHS:
    {
      string temp=_pd->ReadString();
	  char* shortname=new char[temp.size()+1];
	  memcpy(shortname,temp.c_str(),temp.size()+1);

      //print shortname
      //fout<<"-------------------"<<shortname<<endl;

      delete _pd;
      _pd=NULL;
	  ComplexIndex* complex_index = new ComplexIndex();
	  complex_index->unit_count=1;
	  complex_index->struct_=shortname;
	  return complex_index;
    }
    break;
  default:
    {
      delete _pd;
      _pd=NULL;
      return NULL;
    }
    break;
  }
}

wmdf::IndexBase* wmdf::DataDecoder::Decode_Int64(uint16_t index, PacketDecoder* pd )
{
  int64_t temp = pd->ReadInt64();
	//print value
	//fout<<temp<<endl;

  //深交所开盘前会发初始的无效值
	if(index == IND_TODAYLOW && (temp == 100000000 || temp==999999999))
		return NULL;
  //证券竞价状态和简称前缀
	if(index == 147 || index ==148)
		return NULL;

  if ((index == IND_TRADETIME) &&(temp == 0))
  {
    return NULL;
  }

  //以下代码调试用
  //if ((index == IND_TRADETIME))
  //{
  //  if (temp == 100459)
  //  {
  //    printf("time d%",temp);
  //  }
  //}


  SimpleIndex* simple_index = new SimpleIndex();
  simple_index->value_=temp;

  return simple_index;
}


wmdf::IndexBase* wmdf::DataDecoder::ReadTime( PacketDecoder* pd )
{
  uint32_t temp=pd->ReadChinaMarketTime();
	//print value
	//fout<<"MarketTime:"<<temp<<endl;
  return NULL;


//   SimpleIndex* simple_index = new SimpleIndex();
//   simple_index->value_=temp;
//   return simple_index;
}







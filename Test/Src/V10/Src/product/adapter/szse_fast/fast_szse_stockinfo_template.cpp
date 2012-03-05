#include "fast_szse_stockinfo_template.h"
#include "fast_szse_template_object.h"
#include "../common/rawdata_object.h"

wmdf::FastSzseStockInfoTemplate::FastSzseStockInfoTemplate( void )
:FastTemplate(item_count_)
{
  
}

wmdf::FastSzseStockInfoTemplate::~FastSzseStockInfoTemplate( void )
{
}

wmdf::WindFastMessage* wmdf::FastSzseStockInfoTemplate::DecodeRaw(uint8_t* raw_data,uint32_t len)
{
  int data_status=0;
  int tid=0;
  fast_decoder_->ReSet((uint8_t*)raw_data,len);
  int capacity=4;
  int size=0;
  uint8_t noMiscFeesMap[56];
  SZSEL2_StockInfo_101* data_array = new SZSEL2_StockInfo_101[capacity];//做整体分配的方法	
	for(int32_t i=0;i!=capacity;++i)
	{
		data_array[i].SecurityTradingStatus = WMDF_CONSTS_32BIT_NIL_VALUE;
	}
  while(!fast_decoder_->Empty())
  {
    fast_decoder_->BeginNewMessage();
    tid = fast_decoder_->DecodeUInt32(0,DEFAULT);
    assert(tid==101);
    // constant 变量不需要
    if(size>=capacity)
    {
      SZSEL2_StockInfo_101* temp = new SZSEL2_StockInfo_101[capacity*2];
      memcpy(temp,data_array,sizeof(SZSEL2_StockInfo_101)*capacity);
      capacity=capacity*2;
      delete[] data_array;
      data_array = temp;
			for(int32_t i=size;i!=capacity;++i)
			{
				data_array[i].SecurityTradingStatus = WMDF_CONSTS_32BIT_NIL_VALUE;
			}
    }
    //读取数据
    SZSEL2_StockInfo_101& current_stockinfo=data_array[size++];
    current_stockinfo.RecNo = fast_decoder_->DecodeUInt32(1,INCREASE);
    fast_decoder_->DecodeString(2,NONE,current_stockinfo.SecurityID,sizeof(current_stockinfo.SecurityID));
    fast_decoder_->DecodeUnicodeString(3,NONE,current_stockinfo.Symbol,sizeof(current_stockinfo.Symbol));
    fast_decoder_->DecodeString(4,NONE,current_stockinfo.SecurityDesc,sizeof(current_stockinfo.SecurityDesc));
    fast_decoder_->DecodeString(5,NONE,current_stockinfo.UnderlyingSecurityID,sizeof(current_stockinfo.UnderlyingSecurityID));
    fast_decoder_->DecodeString(2,DEFAULT,current_stockinfo.SecurityIDSource,sizeof(current_stockinfo.SecurityIDSource),"102");//6
    current_stockinfo.nSecurityAltIDsNum = fast_decoder_->DecodeInt32(7,NONE);
    if(current_stockinfo.nSecurityAltIDsNum>0)
    {
      current_stockinfo.pSecurityAltIDs = new stSecurityAltIDs_101[current_stockinfo.nSecurityAltIDsNum];
      for(int i=0;i<current_stockinfo.nSecurityAltIDsNum;++i)
      {
        fast_decoder_->DecodeString(1,NONE,current_stockinfo.pSecurityAltIDs[i].SecurityAltID,sizeof(current_stockinfo.pSecurityAltIDs[i].SecurityAltID));
        fast_decoder_->DecodeString(2,NONE,current_stockinfo.pSecurityAltIDs[i].SecurityAltIDSource,sizeof(current_stockinfo.pSecurityAltIDs[i].SecurityAltIDSource));
      }
    }
    fast_decoder_->DecodeString(8,NONE,current_stockinfo.IndustryClassification,sizeof(current_stockinfo.IndustryClassification));//8
    fast_decoder_->DecodeString(3,DEFAULT,current_stockinfo.Currency,sizeof(current_stockinfo.Currency),"CNY");//9
    current_stockinfo.ShareFaceValue = Utils::Divide(fast_decoder_->DecodeUInt32(4,COPY),WMDF_CONSTS_ONE_THOUSAND);//10
    current_stockinfo.OutstandingShare = fast_decoder_->DecodeUInt64(11,NONE);
    current_stockinfo.PublicFloatShareQuantity = fast_decoder_->DecodeUInt64(12,NONE);
    current_stockinfo.PreviousYearProfitPerShare = Utils::Divide(fast_decoder_->DecodeInt32(13,NONE),WMDF_CONSTS_TEN_THOUSAND);
    current_stockinfo.CurrentYearProfitPerShare = Utils::Divide(fast_decoder_->DecodeInt32(14,NONE),WMDF_CONSTS_TEN_THOUSAND);
    current_stockinfo.NAV = Utils::Divide(fast_decoder_->DecodeUInt32(15,NONE),WMDF_CONSTS_TEN_THOUSAND);
    current_stockinfo.nMiscFeesNum = fast_decoder_->DecodeInt32(16,NONE);
    if(current_stockinfo.nMiscFeesNum>0)
    {
      current_stockinfo.pMiscFees = new stMiscFees_101[current_stockinfo.nMiscFeesNum];
      for (int i=0;i<current_stockinfo.nMiscFeesNum;++i)
      {
        fast_decoder_->BeginSqueuce(noMiscFeesMap,sizeof(noMiscFeesMap));
        fast_decoder_->DecodeString(0,NONE,current_stockinfo.pMiscFees[i].MiscFeeType,sizeof(current_stockinfo.pMiscFees[i].MiscFeeType));
        current_stockinfo.pMiscFees[i].MiscFeeBasis = fast_decoder_->DecodeUInt32(1,COPY);
        current_stockinfo.pMiscFees[i].MiscFeeAmt = Utils::Divide(fast_decoder_->DecodeUInt32(2,NONE),WMDF_CONSTS_ONE_MILLION);
        fast_decoder_->EndSqueuce();
      }
    }
    current_stockinfo.IssueDate = fast_decoder_->DecodeUInt32(17,NONE);
    fast_decoder_->DecodeString(6,COPY,current_stockinfo.BondType,sizeof(current_stockinfo.BondType));//18
    current_stockinfo.CouponRate = Utils::Divide(fast_decoder_->DecodeUInt32(19,NONE),WMDF_CONSTS_ONE_MILLION);
    current_stockinfo.ConversionPrice = fast_decoder_->DecodeUInt32(20,NONE);
    fast_decoder_->DecodeString(21,NONE,current_stockinfo.AmericanOrEuropean,sizeof(current_stockinfo.AmericanOrEuropean));
    fast_decoder_->DecodeString(22,NONE,current_stockinfo.CallOrPut,sizeof(current_stockinfo.CallOrPut));
    fast_decoder_->DecodeString(7,COPY,current_stockinfo.WarrantClearingType,sizeof(current_stockinfo.WarrantClearingType));//23
    current_stockinfo.CVRatio = Utils::Divide(fast_decoder_->DecodeUInt32(24,NONE),WMDF_CONSTS_TEN_THOUSAND);
    current_stockinfo.ConversionBeginDate = fast_decoder_->DecodeUInt32(25,NONE);
    current_stockinfo.ConversionEndDate = fast_decoder_->DecodeUInt32(26,NONE);
    current_stockinfo.InterestAccrualDate = fast_decoder_->DecodeUInt32(27,NONE);
    current_stockinfo.MaturityDate = fast_decoder_->DecodeUInt32(28,NONE);
    current_stockinfo.RoundLot = fast_decoder_->DecodeUInt32(8,COPY);//29
    current_stockinfo.BidLotSize = fast_decoder_->DecodeInt32(9,COPY);//30
    current_stockinfo.AskLotSize = fast_decoder_->DecodeUInt32(10,COPY);//31
    current_stockinfo.MaxFloor = fast_decoder_->DecodeInt32(11,COPY);//32
    fast_decoder_->DecodeString(12,COPY,current_stockinfo.TradeMethod,sizeof(current_stockinfo.TradeMethod));//33
    current_stockinfo.PriceTickSize = fast_decoder_->DecodeUInt32(13,COPY);//34 
    fast_decoder_->DecodeString(14,COPY,current_stockinfo.PriceLimitType,sizeof(current_stockinfo.PriceLimitType));//35
    current_stockinfo.AuctionPriceLimit= Utils::Divide(fast_decoder_->DecodeUInt32(15,COPY),WMDF_CONSTS_ONE_THOUSAND);//36
    current_stockinfo.ContinuousTradePriceLimit= Utils::Divide(fast_decoder_->DecodeUInt32(16,COPY),WMDF_CONSTS_ONE_THOUSAND);//37
    current_stockinfo.DailyPriceUpLimit= Utils::Divide(fast_decoder_->DecodeUInt32(38,NONE),WMDF_CONSTS_ONE_THOUSAND);

    current_stockinfo.DailyPriceLowLimit= Utils::Divide(fast_decoder_->DecodeUInt32(39,NONE),WMDF_CONSTS_ONE_THOUSAND);
    current_stockinfo.DailyPriceUpLimit2= Utils::Divide(fast_decoder_->DecodeUInt32(40,NONE),WMDF_CONSTS_ONE_THOUSAND);
    current_stockinfo.DailyPriceLowLimit2= Utils::Divide(fast_decoder_->DecodeUInt32(41,NONE),WMDF_CONSTS_ONE_THOUSAND);

    
    current_stockinfo.ContractMultiplier= Utils::Divide(fast_decoder_->DecodeUInt32(42,NONE),WMDF_CONSTS_ONE_HUNDRED);
    current_stockinfo.GageRate= Utils::Divide(fast_decoder_->DecodeUInt32(43,NONE),WMDF_CONSTS_ONE_HUNDRED);
    fast_decoder_->DecodeString(17,COPY,current_stockinfo.CrdBuyUnderlying,sizeof(current_stockinfo.CrdBuyUnderlying));//44
    fast_decoder_->DecodeString(18,COPY,current_stockinfo.CrdSellUnderlying,sizeof(current_stockinfo.CrdSellUnderlying));//45
    current_stockinfo.nIndicesParticipatedNum = fast_decoder_->DecodeInt32(46,NONE);
    if(current_stockinfo.nIndicesParticipatedNum>0)
    {
      current_stockinfo.pIndicesParticipated = new stIndicesParticipated_101[current_stockinfo.nIndicesParticipatedNum];
      for(int i=0;i<current_stockinfo.nIndicesParticipatedNum;++i)
        fast_decoder_->DecodeString(0,NONE,current_stockinfo.pIndicesParticipated[i].participatingIndexID ,sizeof(current_stockinfo.pIndicesParticipated[i].participatingIndexID ));
    }
    fast_decoder_->DecodeString(19,COPY,current_stockinfo.MarketMakerFlag,sizeof(current_stockinfo.MarketMakerFlag));//47
    fast_decoder_->DecodeString(20,DEFAULT,current_stockinfo.SecurityExchange,sizeof(current_stockinfo.SecurityExchange),"XSHE");
    fast_decoder_->DecodeString(21,COPY,current_stockinfo.CFICode,sizeof(current_stockinfo.CFICode));
    fast_decoder_->DecodeString(22,COPY,current_stockinfo.SecuritySubType,sizeof(current_stockinfo.SecuritySubType));
    fast_decoder_->DecodeString(23,COPY,current_stockinfo.SecurityProperties,sizeof(current_stockinfo.SecurityProperties),"N");
    current_stockinfo.SecurityTradingStatus= fast_decoder_->DecodeUInt32(24,DEFAULT);
    fast_decoder_->DecodeString(25,DEFAULT,current_stockinfo.CorporateAction,sizeof(current_stockinfo.CorporateAction),"N");
    fast_decoder_->DecodeString(26,DEFAULT,current_stockinfo.TradingMechanism,sizeof(current_stockinfo.TradingMechanism),"G");
    fast_decoder_->DecodeString(27,DEFAULT,current_stockinfo.CrdPriceCheckType,sizeof(current_stockinfo.CrdPriceCheckType),"1");
    fast_decoder_->DecodeString(28,DEFAULT,current_stockinfo.NetVotingFlag,sizeof(current_stockinfo.NetVotingFlag),"N");
    fast_decoder_->DecodeString(29,DEFAULT,current_stockinfo.ShrstrurefmFlag,sizeof(current_stockinfo.ShrstrurefmFlag),"Y");
    fast_decoder_->DecodeString(30,DEFAULT,current_stockinfo.OfferingFlag,sizeof(current_stockinfo.OfferingFlag),"N");

  }
  WindFastMessage* msg = new WindFastMessage();
  msg->MsgType = SZSE_STOCKINFO_TYPE;
  msg->Size=size;
  msg->Data = data_array;
  return msg;
}

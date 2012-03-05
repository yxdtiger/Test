#include <iostream>
#include <string>
#include <time.h>
#include "../fast_codec_shl2_market_data.h"
#include "testUtils.h"
#include "../../environment/utils.h"

using namespace std;

namespace wmdf{
void TestSHL2MarketDataCodec()
	{
    cout<<"------SSE L2 MarketData------"<<endl;
    clock_t start, end;
    int times = 1,ilength=1000,bufferSize=0;
    L2MarketSnapshot* marketDatas = new L2MarketSnapshot[ilength];
    memset(marketDatas,0,ilength*sizeof(L2MarketSnapshot));
		for(int i=0;i<ilength;i++)
		{
			string windCode = "000001.SH";
			memcpy(marketDatas[i].wind_code,windCode.c_str(),10);
			marketDatas[i].trade_date=20110201;
			marketDatas[i].trade_time = 103001+i%100;

			marketDatas[i].prev_close=15.6+i;
			marketDatas[i].today_open= 15.9+i;
			marketDatas[i].today_high= 15.9+i;
			marketDatas[i].today_low= 15.5+i;
			marketDatas[i].new_price = 15.8+i/3;
			marketDatas[i].volume =		1000000;
			marketDatas[i].total_amount =15800000;

			marketDatas[i].bid_price1=15.8+i/3;
			marketDatas[i].bid_price2=15.79+i/3;
			marketDatas[i].bid_price3=15.78+i/3;
			marketDatas[i].bid_price4=15.75+i/3;
			marketDatas[i].bid_price5=15.73+i/3;
      marketDatas[i].bid_price6=15.8+i/3;
      marketDatas[i].bid_price7=15.79+i/3;
      marketDatas[i].bid_price8=15.78+i/3;
      marketDatas[i].bid_price9=15.75+i/3;
      marketDatas[i].bid_price10=15.73+i/3;

			marketDatas[i].ask_price1=15.81+i/3;
			marketDatas[i].ask_price2=15.82+i/3;
			marketDatas[i].ask_price3=15.85+i/3;
			marketDatas[i].ask_price4=15.88+i/3;
			marketDatas[i].ask_price5=15.89+i/3;
      marketDatas[i].ask_price6=15.81+i/3;
      marketDatas[i].ask_price7=15.82+i/3;
      marketDatas[i].ask_price8=15.85+i/3;
      marketDatas[i].ask_price9=15.88+i/3;
      marketDatas[i].ask_price10=15.89+i/3;

			marketDatas[i].ask_volume1=2600+i/3;
			marketDatas[i].ask_volume2=2000+i/3;
			marketDatas[i].ask_volume3=1900+i/3;
			marketDatas[i].ask_volume4=2100+i/3;
			marketDatas[i].ask_volume5=2200+i/3;
      marketDatas[i].ask_volume6=2600+i/3;
      marketDatas[i].ask_volume7=2000+i/3;
      marketDatas[i].ask_volume8=1900+i/3;
      marketDatas[i].ask_volume9=2100+i/3;
      marketDatas[i].ask_volume10=2200+i/3;

			marketDatas[i].bid_volume1=2300+i/3;
			marketDatas[i].bid_volume2=2400+i/3;
			marketDatas[i].bid_volume3=2000+i/3;
			marketDatas[i].bid_volume4=1500+i/3;
			marketDatas[i].bid_volume5=1300+i/3;
      marketDatas[i].bid_volume6=2300+i/3;
      marketDatas[i].bid_volume7=2400+i/3;
      marketDatas[i].bid_volume8=2000+i/3;
      marketDatas[i].bid_volume9=1500+i/3;
      marketDatas[i].bid_volume10=1300+i/3;


      marketDatas[i].weighted_average_best_bid = 178;
      marketDatas[i].weighted_average_best_ask = 179;
      marketDatas[i].l2_total_bid_qty=1300*(i+1);
      marketDatas[i].l2_total_offer_qty=1700*(i+1);



		}

		FastCodecSHL2MarketData* codec = new FastCodecSHL2MarketData();

    int count=0;
   /* while(true)
    {*/
		  start = clock();
		  for(int j=0;j<times;j++)
		  {
			  bufferSize = codec->EncodeData(marketDatas,ilength);
		  }

		  end = clock();
		  cout<<"编码码耗时: "<<(double)(end - start)/1000<<"S"<<endl;
		  cout<<"每秒编码数: "<<(double)times*100/((end - start))<<"（万)"<<endl;

      bufferSize = codec->EncodeData(marketDatas,ilength);
      std::cout<<"BufferSize:"<<bufferSize<<std::endl;

		  if(bufferSize>0)
		  {
			  uint8_t* buffer = (uint8_t*)malloc(bufferSize);
			  codec->DequeueBuffer(buffer);

			  start = clock();
			  for(int p=0;p<times;p++)
			  {
				 L2MarketSnapshot* items = (L2MarketSnapshot*)codec->DecodeData(buffer,bufferSize,count);

				  std::cout<<p<<std::endl;
          for(int i = 0;i<count;i++)
          {
            if(!TestUtils::CompareMarketDataL2(marketDatas[i],items[i]))
            {
              std::cout<<"FastCodecSHL2MarketData：["<<i<<"] error!"<<std::endl;
            }
            /*else
            {
              std::cout<<"index ["<<i<<"] ok!"<<std::endl;
            }*/
          }
          delete[] items;
          items=NULL;
			  }
        free(buffer);
			  end = clock();
			  cout<<"解码码耗时: "<<(double)(end - start)/1000<<"S"<<endl;
			  cout<<"每秒解码数: "<<(double)times*100/((end - start))<<"（万)"<<endl;
		  }
      Utils::SleepTime(10);
      delete codec;
    //}
	}
}

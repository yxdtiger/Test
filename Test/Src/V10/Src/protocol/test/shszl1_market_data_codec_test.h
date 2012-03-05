#include <iostream>
#include <string>
#include "../../environment/utils.h"
#include "../../include/date_time.h"
#include "../fast_codec_shszl1_market_data.h"
#include "testUtils.h"

using namespace std;

namespace wmdf{
void TestSHSZL1MarketDataCodec()
	{
    cout<<"------SSE L1 MarketData------"<<endl;
    double start, end;
    int times = 1,ilength=1000,bufferSize=0;
    L1MarketSnapshot* marketDatas = new L1MarketSnapshot[ilength];
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
			marketDatas[i].total_amount =15800000	;
			marketDatas[i].bid_price1=15.8+i/3;
			marketDatas[i].bid_price2=15.79+i/3;
			marketDatas[i].bid_price3=15.78+i/3;
			marketDatas[i].bid_price4=15.75+i/3;
			marketDatas[i].bid_price5=15.73+i/3;

			marketDatas[i].ask_price1=15.81+i/3;
			marketDatas[i].ask_price2=15.82+i/3;
			marketDatas[i].ask_price3=15.85+i/3;
			marketDatas[i].ask_price4=15.88+i/3;
			marketDatas[i].ask_price5=15.89+i/3;
			marketDatas[i].ask_volume1=2600+i/3;
			marketDatas[i].ask_volume2=2000+i/3;
			marketDatas[i].ask_volume3=1900+i/3;
			marketDatas[i].ask_volume4=2100+i/3;
			marketDatas[i].ask_volume5=2200+i/3;
			marketDatas[i].bid_volume1=2300+i/3;
			marketDatas[i].bid_volume2=2400+i/3;
			marketDatas[i].bid_volume3=2000+i/3;
			marketDatas[i].bid_volume4=1500+i/3;
			marketDatas[i].bid_volume5=1300+i/3;
		}

		FastCodecSHSZL1MarketData* codec = new FastCodecSHSZL1MarketData();


    start = DateTime::now();
		for(int j=0;j<times;j++)
		{
			bufferSize = codec->EncodeData(marketDatas,ilength);
		}

		end = DateTime::now();
		cout<<"编码码耗时: "<<(double)(end - start)/1000<<"S"<<endl;
		cout<<"每秒编码数: "<<(double)times*100/((end - start))<<"（万)"<<endl;

    bufferSize = codec->EncodeData(marketDatas,ilength);
    std::cout<<"BufferSize:"<<bufferSize<<std::endl;
		if(bufferSize>0)
		{
			uint8_t* buffer = (uint8_t*)malloc(bufferSize);
			codec->DequeueBuffer(buffer);
			int count=0;
			start = DateTime::now();
			for(int p=0;p<times;p++)
			{
				L1MarketSnapshot* items = (L1MarketSnapshot*)codec->DecodeData(buffer,bufferSize,count);

				//std::cout<<p<<std::endl;
        for(int i = 0;i<count;i++)
        {
          if(!TestUtils::CompareMarketData(marketDatas[i],items[i]))
          {
            std::cout<<"FastCodecSHSZL1MarketData ["<<i<<"] error!"<<std::endl;
          }
        /*  else
          {
            std::cout<<"index ["<<i<<"] ok!"<<std::endl;
          }*/
        }
        delete[] items;
        items=NULL;
			}

			end = DateTime::now();
			cout<<"解码码耗时: "<<(end - start)/1000<<"S"<<endl;
			cout<<"每秒解码数: "<<times*100/((end - start))<<"（万)"<<endl;
		}
    delete codec;
	}
}

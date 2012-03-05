#include <iostream>
#include <string>
//#include <windows.h>
#include "../fast_codec_cffel1_market_data.h"
using std::string;
namespace wmdf{


void TestCFFEL1MarketDataCodec()
	{
    cout<<"------MarketData CFFE L1------"<<endl;
		FuturesMarketSnapshot* markets = new FuturesMarketSnapshot[1000];
		int ilength=1000,bufferSize=0,times=1;
		for(int i=0;i<ilength;i++)
		{
		  string windCode = "000001.SH";
		  memcpy(markets[i].wind_code,windCode.c_str(),10);
		  markets[i].trade_date=20110201;
		  markets[i].trade_time = 103001+i;

		  markets[i].prev_close=15.6+i;
		  markets[i].today_open= 15.9+i;
		  markets[i].today_high= 15.9+i;
		  markets[i].today_low= 15.5+i;
		  markets[i].new_price = 15.8+i/3;
		  markets[i].volume =		1000000;
		  markets[i].amount =15800000	;

		  markets[i].bid_price1=15.8+i/3;
		  markets[i].ask_price1=15.81+i/3;
		  markets[i].ask_volume1=2600+i/3;
		  markets[i].bid_volume1=2300+i/3;
      markets[i].prev_settle=15.1+i/3;
      markets[i].position=2100+i/3;
      markets[i].prev_position=1900+i/3;
      markets[i].high_limit=(i+100)*100/3;
      markets[i].low_limit=(i+50)*100/3;
		}

    FastCodecCFFEL1MarketData* codec = new FastCodecCFFEL1MarketData();

    clock_t start, end;uint8_t* buffer=NULL;
    start = clock();
    for(int i=0;i<times;i++)
    {
      bufferSize = codec->EncodeData(markets,ilength);
    }
    end = clock();
    cout<<"编码码耗时: "<<(double)(end - start)/1000<<"S"<<endl;
    cout<<"每秒编码数: "<<(double)times*100/(end - start)<<"（万）"<<endl;

    bufferSize = codec->EncodeData(markets,ilength);
    std::cout<<"BufferSize:"<<bufferSize<<std::endl;

    if(bufferSize>0)
    {
      buffer = (uint8_t*)malloc(bufferSize);
      codec->DequeueBuffer(buffer);
      int count=0;
      start = clock();
      for(int p=0;p<times;p++)
      {
        FuturesMarketSnapshot* items = (FuturesMarketSnapshot*)codec->DecodeData(buffer,bufferSize,count);
        for(int i = 0;i<count;i++)
        {
          if(!TestUtils::CompareCFFEL1MarketData(markets[i],items[i]))
          {
            std::cout<<"FastCodecCFFEL1MarketData ["<<i<<"] error!"<<std::endl;
          }
          /*else
          {
            std::cout<<"index ["<<i<<"] ok."<<std::endl;
          }*/
        }
        delete items;
      }
      end = clock();
      cout<<"解码耗时: "<<(double)(end - start)/1000<<"S"<<endl;
      cout<<"每秒解码数: "<<(double)times*100/(end - start)<<"（万）"<<endl;

      free(buffer);
      buffer=NULL;
    }
    delete codec;
	}
}

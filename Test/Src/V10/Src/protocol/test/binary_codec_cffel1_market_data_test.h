#include <iostream>
#include <string>
#include <time.h>
#include "testUtils.h"
#include "../binary_codec_cffel1_market_data.h"
using std::string;
namespace wmdf{
  

void TestCFFEL1MarketDataBianryCodec()
	{	
    cout<<"------MarketData CFFE L1 BianryCodec------"<<endl;
		FuturesMarketSnapshot* tr = new FuturesMarketSnapshot[1000];
		int ilength=1000,bufferSize=0,times=1;
		for(int i=0;i<ilength;i++)
		{
		  string windCode = "000001.SH";
		  memcpy(tr[i].wind_code,windCode.c_str(),10);
		  tr[i].trade_date=20110201;
		  tr[i].trade_time = 103001+i;
  		
		  tr[i].prev_close=15.6+i;
		  tr[i].today_open= 15.9+i;
		  tr[i].today_high= 15.9+i;
		  tr[i].today_low= 15.5+i;
		  tr[i].new_price = 15.8+i/3;
		  tr[i].volume =		1000000;
		  tr[i].amount =15800000	;
		  
		  tr[i].bid_price1=15.8+i/3;
		  tr[i].ask_price1=15.81+i/3;			
		  tr[i].ask_volume1=2600+i/3;			
		  tr[i].bid_volume1=2300+i/3;
      tr[i].prev_settle=15.1+i/3;
      tr[i].position=2100+i/3;
      tr[i].prev_position=1900+i/3;
      tr[i].high_limit=(i+100)*100/3;
      tr[i].low_limit=(i+50)*100/3;
		}	

     BinaryCodecCFFEL1MarketData* codec = new BinaryCodecCFFEL1MarketData();		
		    
    clock_t start, end;uint8_t* buffer=NULL;
   

    start = clock();     
    for(int i=0;i<times;i++)
    {
      bufferSize = codec->EncodeData(tr,ilength);      
    }		
    end = clock();
    cout<<"编码码耗时: "<<(double)(end - start)/1000<<"S"<<endl;
    cout<<"每秒编码数: "<<(double)times*100/(end - start)<<"（万）"<<endl;

    bufferSize = codec->EncodeData(tr,ilength);
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
          if(!TestUtils::CompareCFFEL1MarketData(tr[i],items[i]))
          {
            std::cout<<"BinaryCodecCFFEL1MarketData ["<<i<<"] error!"<<std::endl;
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
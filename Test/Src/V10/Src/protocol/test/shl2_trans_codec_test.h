#include <iostream>
#include <string>
#include <time.h>
#include "../fast_codec_shl2_trans.h"
#include "testUtils.h"
using std::string;
namespace wmdf{


void TestSHL2TransactionCodec()
{	
  cout<<"------SSE L2 Transaction------"<<endl;
  clock_t start, end;
  int times = 1,ilength=1000,bufferSize=0;

  L2Transaction* trans = new L2Transaction[ilength];
  for(int i=0;i<ilength;i++)
  {
	  string windCode = "000001.SH";
	  memcpy(trans[i].wind_code,windCode.c_str(),10);
		trans[i].trade_channel = 10;
		trans[i].rec_no=i;
		trans[i].trade_money = 119*100000;
		trans[i].price=119;
		trans[i].trade_qty=100000;
	  trans[i].trade_time = 103001+i%100;			
  }
  
  FastCodecSHL2Trans* codec = new FastCodecSHL2Trans();

  start = clock();
  for(int j=0;j<times;j++)		
  {
	  bufferSize = codec->EncodeData(trans,ilength);
  }      
  end = clock();
  cout<<"编码码耗时: "<<(double)(end - start)/1000<<"S"<<endl;
  cout<<"每秒编码数: "<<(double)times*100/((end - start))<<"（万)"<<endl;

  bufferSize = codec->EncodeData(trans,ilength);
  std::cout<<"BufferSize:"<<bufferSize<<std::endl;  				
  if(bufferSize>0)
  {
	  uint8_t* buffer = (uint8_t*)malloc(bufferSize);
	  codec->DequeueBuffer(buffer);
	  int count=0;
	  start = clock();
	  for(int p=0;p<times;p++)
	  {  				
		  L2Transaction* items = (L2Transaction*)codec->DecodeData(buffer,bufferSize,count);
		  
		  std::cout<<p<<std::endl;
		  for(int i = 0;i<count;i++)
		  {
		    if(!TestUtils::CompareTransaction(trans[i],items[i]))
		    {
		      std::cout<<"FastCodecSHL2Trans： ["<<i<<"] error!"<<std::endl;
		    }
        /*else
        {
          std::cout<<"index ["<<i<<"] ok!"<<std::endl;
        }*/
		  }
      delete[] items;
      items=NULL;
	  }
        
	  end = clock();
	  cout<<"解码码耗时: "<<(double)(end - start)/1000<<"S"<<endl;
	  cout<<"每秒解码数: "<<(double)times*100/((end - start))<<"（万)"<<endl;
	  }
  delete codec;
  }
}
#include <iostream>
#include <string>
#include <time.h>
#include "../fast_codec_szl2_trans.h"
#include "testUtils.h"
using std::string;
namespace wmdf{


void TestSZL2TransactionCodec()
{	
  cout<<"------SZSE L2 Transaction------"<<endl;
  clock_t start, end;
  int times = 1,ilength=1000,bufferSize=0;

  L2Transaction* trans = new L2Transaction[ilength];
  for(int i=0;i<ilength;i++)
  {
	  string windCode = "000001.SH";
	  memcpy(trans[i].wind_code,windCode.c_str(),10);
		trans[i].rec_no=i+10;
    trans[i].set_no=i+100;
    trans[i].buyOrderRecNo = i+25;
    trans[i].sellOrderRecNo = i+50;
    trans[i].order_kind = 58;
    trans[i].function_code = 89;
		trans[i].price=119;
		trans[i].trade_qty=100000;
	  trans[i].trade_time = 103001+i%100;			
  }
  
  FastCodecSZL2Trans* encoder = new FastCodecSZL2Trans();
  FastCodecSZL2Trans* decoder = new FastCodecSZL2Trans();
  start = clock();
  for(int j=0;j<times;j++)		
  {
	  bufferSize = encoder->EncodeData(trans,ilength);
  }      
  end = clock();
  cout<<"编码码耗时: "<<(double)(end - start)/1000<<"S"<<endl;
  cout<<"每秒编码数: "<<(double)times*100/((end - start))<<"（万)"<<endl;

  //bufferSize = encoder->EncodeData(trans,ilength);
  std::cout<<"BufferSize:"<<bufferSize<<std::endl;  				
  if(bufferSize>0)
  {
	  uint8_t* buffer = (uint8_t*)malloc(bufferSize);
	  encoder->DequeueBuffer(buffer);
	  int count=0;
	  start = clock();
	  for(int p=0;p<times;p++)
	  {  				
		  L2Transaction* items = (L2Transaction*)decoder->DecodeData(buffer,bufferSize,count);
		  
		  std::cout<<p<<std::endl;
		  for(int i = 0;i<count;i++)
		  {
		    if(!TestUtils::CompareTransaction(trans[i],items[i]))
		    {
		      std::cout<<"FastCodecSZL2Trans ["<<i<<"] error!"<<std::endl;
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
  delete encoder;
  delete decoder;
  }
}
#include <iostream>
#include <string>
#include <time.h>
#include "testUtils.h"
#include "../../environment/utils.h"
#include "../binary_codec_login_request.h"
using std::string;
namespace wmdf{

	void TestLoginRequestBinaryCodec()
	{	
    cout<<"------System Login Request BinaryCodec------"<<endl;
    clock_t start, end;
    int times = 1,ilength=1,bufferSize=0;

		
   
    LoginRequest* request = new LoginRequest();    
    string name = "helloworld";
    memcpy(request->user_name,name.c_str(),32);
    memcpy(request->password,name.c_str(),16);
    request->product_count=1;
    request->product = new ProductInfo();
    request->product->codec = 1;
    request->product->product_id=10;
    request->product->major_version=1;
    request->product->major_version=0;
    BinaryCodecLoginRequest* codec=new BinaryCodecLoginRequest();

    start = clock();
    for(int j=0;j<times;j++)		
    {
      bufferSize = codec->EncodeData(request,ilength);
    }      
    end = clock();
    cout<<"编码码耗时: "<<(double)(end - start)/1000<<"S"<<endl;
    cout<<"每秒编码数: "<<(double)times*1000/((end - start))<<"（万)"<<endl;

    //bufferSize = codec->EncodeData(request,ilength);
    std::cout<<"BufferSize:"<<bufferSize<<std::endl;  				
    if(bufferSize>0)
    {
      uint8_t* buffer = (uint8_t*)malloc(bufferSize);
      codec->DequeueBuffer(buffer);
      int count=0;
      start = clock();
      for(int p=0;p<times;p++)
      {  
        LoginRequest* items = (LoginRequest*)codec->DecodeData(buffer,bufferSize,count);

        for(int i = 0;i<count;i++)
        {
          if(!TestUtils::CompareLoginRequest(request[0],items[i]))
          {
            std::cout<<"BinaryCodecLoginRequest ["<<i<<"] error!"<<std::endl;
          }
          /*else
          {
            std::cout<<"index ["<<i<<"] ok!"<<std::endl;
          }*/
        }
        Utils::DestroyLoginRequest(items);
        items=NULL;
      }
      end = clock();
      cout<<"解码码耗时: "<<(double)(end - start)/1000<<"S"<<endl;
      cout<<"每秒解码数: "<<(double)times*1000/((end - start))<<"（万)"<<endl;
    }
    delete codec;
    Utils::DestroyLoginRequest(request);
	}
}
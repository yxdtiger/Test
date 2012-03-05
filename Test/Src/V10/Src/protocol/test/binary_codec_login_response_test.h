#include <iostream>
#include <string>
#include <time.h>
#include "testUtils.h"
#include "../../environment/utils.h"
#include "../binary_codec_login_response.h"
using std::string;
namespace wmdf{

	void TestLoginResponseBinaryCodec()
	{	
    cout<<"------System Login Response BinaryCodec------"<<endl;
    clock_t start, end;
    int times = 1,ilength=1,bufferSize=0;

		
   
    LoginResponse* response = new LoginResponse();    
    string address = "127.0.0.1";
    response->user_id=12345;
    response->ack_type = 1;
    response->server_count=2;    
    response->serverinfo = new ServerInfo[response->server_count];
    for(int i=0;i<response->server_count;i++)
    {
      memcpy(response->serverinfo[i].server_addr,address.c_str(),15);
      response->serverinfo[i].server_port = 6588+i;
    }
    response->product_count=3;
    response->product_status = new ProductStatus[3];
    for(int i=0;i<response->product_count;i++)
    {
      response->product_status[i].codec=i;
      response->product_status[i].product_status = 1;
      response->product_status[i].major_version=i%2+1;
      response->product_status[i].minor_version=i%2;
      response->product_status[i].product_id = i*10;
      response->product_status[i].message_count = 2;
      response->product_status[i].message_info = new MessageInfo[2];
      for(int j=0;j<response->product_status[i].message_count;j++)
      {
        response->product_status[i].message_info[j].message_id=j;
        response->product_status[i].message_info[j].message_status=j+10;
      }
    }
    
    BinaryCodecLoginResponse* codec = new BinaryCodecLoginResponse();

    start = clock();
    for(int j=0;j<times;j++)		
    {
      bufferSize = codec->EncodeData(response,ilength);
    }      
    end = clock();
    cout<<"编码码耗时: "<<(double)(end - start)/1000<<"S"<<endl;
    cout<<"每秒编码数: "<<(double)times*1000/((end - start))<<"（万)"<<endl;

    bufferSize = codec->EncodeData(response,ilength);
    std::cout<<"BufferSize:"<<bufferSize<<std::endl;  				
    if(bufferSize>0)
    {
      uint8_t* buffer = (uint8_t*)malloc(bufferSize);
      codec->DequeueBuffer(buffer);
      int count=0;
      start = clock();
      for(int p=0;p<times;p++)
      {  				
        LoginResponse* items = (LoginResponse*)codec->DecodeData(buffer,bufferSize,count);

        for(int i = 0;i<count;i++)
        {
          if(!TestUtils::CompareLoginResponse(response,&items[i]))
          {
            std::cout<<"BinaryCodecLoginResponse ["<<i<<"] error!"<<std::endl;
          }
           /*else
          {
            std::cout<<"index ["<<i<<"] ok!"<<std::endl;
          }*/
        }
        Utils::DestroyLoginResponse(items);
        items=NULL;
      }
      end = clock();
      cout<<"解码码耗时: "<<(double)(end - start)/1000<<"S"<<endl;
      cout<<"每秒解码数: "<<(double)times*1000/((end - start))<<"（万)"<<endl;
    }
    delete codec;
    Utils::DestroyLoginResponse(response);
	}
}
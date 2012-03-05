#include <iostream>
#include <string>
#include <time.h>
#include "testUtils.h"
#include "../binary_codec_logout.h"
using std::string;
namespace wmdf{

	void TestLogoutBinaryCodec()
	{	
    cout<<"------System Logout BinaryCodec------"<<endl;
    clock_t start, end;
    int times = 1,ilength=1,bufferSize=0;

		
   
    Logout* logout = new Logout();    
    logout->logout_type=2;
    BinaryCodecLogout* codec= new BinaryCodecLogout();

    start = clock();
    for(int j=0;j<times;j++)		
    {
      bufferSize = codec->EncodeData(logout,ilength);
    }      
    end = clock();
    cout<<"编码码耗时: "<<(double)(end - start)/1000<<"S"<<endl;
    cout<<"每秒编码数: "<<(double)times/(10*(end - start))<<"（万)"<<endl;

    bufferSize = codec->EncodeData(logout,ilength);
    std::cout<<"BufferSize:"<<bufferSize<<std::endl;  				
    if(bufferSize>0)
    {
      uint8_t* buffer = (uint8_t*)malloc(bufferSize);
      codec->DequeueBuffer(buffer);
      int count=0;
      start = clock();
      for(int p=0;p<times;p++)
      {  
        Logout* items = (Logout*)codec->DecodeData(buffer,bufferSize,count);

        for(int i = 0;i<count;i++)
        {
          if(!TestUtils::CompareLogout(&logout[0],&items[i]))
          {
            std::cout<<"BinaryCodecLogout ["<<i<<"] error!"<<std::endl;
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
      cout<<"每秒解码数: "<<(double)times/(10*(end - start))<<"（万)"<<endl;
    }
    delete codec;
	}
}
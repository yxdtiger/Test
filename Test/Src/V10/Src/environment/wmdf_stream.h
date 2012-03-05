#ifndef WMDF_STREAM_H
#define WMDF_STREAM_H
#include <cstdio>

struct Message;
namespace wmdf
{  
  class WMDFFile;
  class WMDFStream
  {
  public:
    WMDFStream();
    virtual ~WMDFStream();
    bool Open(const char* path,const char* mode);
    void Close();
    int Write(Message* msg);    
  private:
    WMDFFile* mdf_file_;    
  };
}

#endif

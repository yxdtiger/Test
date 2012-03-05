#include "binary_codec_monitor_event.h"
#include <assert.h>
#include "../include/wmdf_structs.h"

using namespace wmdf;
wmdf::BinaryCodecMonitorEvent::BinaryCodecMonitorEvent(void)
{
}


wmdf::BinaryCodecMonitorEvent::~BinaryCodecMonitorEvent( void )
{

}


int BinaryCodecMonitorEvent::EncodeData(void* items,int count)
{
  binaryCodec->InitEncoder();
  binaryCodec->EncodeInt32(count);
  MonitorEvent* item = (MonitorEvent* )items;
  int length = sizeof(MonitorEvent) * count;
  binaryCodec->EncodeBytes((uint8_t*)item,length);	
  return binaryCodec->buffer.Get_Buffer_Used();
}

void* BinaryCodecMonitorEvent::DecodeData(uint8_t* bytes,int length,int& count)
{	
  binaryCodec->InitDecoder(bytes,length);
  count = binaryCodec->DecodeInt32();	
  MonitorEvent* itemArray=new MonitorEvent[count];
  int tLength = sizeof(MonitorEvent)*count;
  assert((tLength+sizeof(count))==length);
  binaryCodec->DecodeBytes((uint8_t*)itemArray,tLength);	
  return itemArray;
}

int32_t wmdf::BinaryCodecMonitorEvent::SizeOfRecords( void* items,int count )
{
  return sizeof(MonitorEvent)*count;
}

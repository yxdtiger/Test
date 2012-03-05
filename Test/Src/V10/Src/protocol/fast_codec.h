#ifndef FAST_CODEC_H
#define FAST_CODEC_H
#include <string>
#include "math.h"
#include "codec_buffer.h"
#ifdef __linux__
#include <cstring>
#include <cstdlib>
#endif


namespace wmdf{

typedef enum fast_op_t
{
	FAST_OP_NONE = 0,
	FAST_OP_COPY,
	FAST_OP_INCR,
	FAST_OP_DELTA,
	FAST_OP_DEFAULT,
	FAST_OP_CONST
}
fast_op_t;


typedef struct Fast_PMap_T
{
	uint8_t* pos;//ÓÃÓÚ¼ÇÂ¼±àÂëÊ±Ð´ÈëpmapµÄÎ»ÖÃ£¨ÒòÎªpmapÐèÒªµÈ±àÍê»ØÐ´£©
	int size;
	int max_pos;
	uint8_t bits[1];
}
Fast_PMap_T;

static uint32_t bit_mask [8] = { 64, 32, 16, 8, 4, 2, 1, 0 };

typedef struct tap_TValue {
	int32_t id;
	union
	{
		int32_t int32_value;
		int64_t int64_value;
		uint8_t str_value[10];
	};

} TValue ;

class FastCodec
{
public:
	FastCodec(void)
  {

  }
	virtual ~FastCodec(void)
  {
    currentPMap=NULL;
    defaultValue=NULL;
  }

	inline void  InitEncoder();
	inline bool  EncodeNewMsg(Fast_PMap_T* pMap,TValue* defaultValue);
	inline int   EncodeInt32 (int op, int pos, int value);
	inline int   EncodeInt64 (int op, int pos, int64_t value);
  inline int   EncodeDouble (int op, int pos, double value,int decimalPlaces);
	inline int   EncodeStr (int op,int pos,uint8_t* data, int size);
	inline void  EncodeEndMsg(Fast_PMap_T* parentPmap=NULL,TValue* parentDefaultValue=NULL);

	inline void  InitDecoder(uint8_t* bytes,int length);
	inline int   DecodeNewMsg (Fast_PMap_T* pmap,TValue* defaultValue);
	inline int   DecodeStr (int op,int pos,uint8_t* data, int size);
	inline int   DecodeInt32 (int op,int pos, int32_t& value);
	inline int   DecodeInt64 (int op,int pos, int64_t& value);
  inline int   DecodeDouble (int op, int pos, double& value,int decimalPlaces);
	inline int   DecodeEndMsg (Fast_PMap_T* parentPmap=NULL,TValue* parentDefaultValue=NULL);

  inline double Round(double number);

inline int Check_Msg_Avail ();
	CodecBuffer buffer;

private:
	int m_PMap_Bytes;
	Fast_PMap_T* currentPMap;
	TValue* defaultValue;

	inline void Emit_Byte(uint8_t data);
	inline int Emit_PMap(uint8_t* buffer);
	inline int Emit_Int32 (int data);
	inline int EmitInt64 (int64_t data);
	inline int Emit_UInt32 (uint32_t data);
	inline int EmitStr (uint8_t* data, int size);

	inline int Parse_Bytes (uint8_t* data, int size);
	inline int Parse_PMap (int bytesNum);
	inline int Parse_64 (uint64_t& data, int sign_mask);
	inline int ParseInt64 (int64_t& data);

	inline int Parse32 (uint32_t& data, int sign_mask);
	inline int Parse_UInt32 (uint32_t& data);
	inline int ParseInt32 (int32_t& data);
	inline int ParseStr (uint8_t* data, int size);

	inline void SetPMap (int value = 1);
  inline void SetStrValue(int pos ,const void* value,int size);
	inline void SetIntValue(int pos,int32_t& value);
	inline void SetInt64Value(int pos,int64_t& value);

	inline int GetPMap ();


	inline int UInt32_To_Size (uint32_t data);
	inline int Int32_to_Size (int data);
	inline int Int64_to_Size (int64_t data);

	uint32_t align (uint32_t x, uint32_t size)
	{
		return (x + size - 1) / size;
	}

	uint32_t align7 (uint32_t x) { return align (x, 7); }
};

void FastCodec::InitEncoder()
{
  buffer.MallocBuffer();
}

void FastCodec::InitDecoder(uint8_t* bytes,int length)
{
  buffer.ResetBuffer(bytes,length);
}

bool FastCodec::EncodeNewMsg(Fast_PMap_T* pMap,TValue* value)
{
	currentPMap=pMap;
	currentPMap->pos=buffer.tail;
	defaultValue=value;
	m_PMap_Bytes = align7 (currentPMap->size); // ½«PMapÐ´ËÀ
	memset (buffer.tail, 0, m_PMap_Bytes);
  pMap->max_pos=0;
	buffer.tail += m_PMap_Bytes;
	return true;
}


void FastCodec::EncodeEndMsg(Fast_PMap_T* parentPmap,TValue* parentDefaultValue)
{
	Emit_PMap (currentPMap->pos);
  currentPMap=parentPmap;
  defaultValue = parentDefaultValue;
}

int FastCodec::EncodeInt32 (int op, int pos, int value)
{
	switch (op)
	{
	case FAST_OP_NONE:
    return Emit_Int32 (value); // emit the value
	case FAST_OP_DEFAULT:
		if (WMDF_CONSTS_32BIT_NIL_VALUE==value)
    {
      SetPMap(0);
      return 0;
    }
		break;
	case FAST_OP_COPY:
		if (defaultValue[pos].int32_value==value)
    {
      SetPMap(0);
      return 0;
    }
		break;
	case FAST_OP_INCR:
		if (defaultValue[pos].int32_value+1==value)
		{
      SetPMap(0);
      SetIntValue(pos,value);
			return 0;
		}
		break;
	case FAST_OP_DELTA:
		{
			int delta =  value - defaultValue[pos].int32_value;
      SetIntValue(pos,value);
		  return Emit_Int32 (delta);
		}
	default:
		break;
	}
  SetPMap();
  SetIntValue(pos,value);
	return Emit_Int32 (value); // emit the value
}

int FastCodec::EncodeInt64 (int op, int pos, int64_t value)
{
	int64_t next = value;

	switch (op)
	{
	case FAST_OP_NONE:
    return EmitInt64 (value);
	case FAST_OP_DEFAULT:
		if (WMDF_CONSTS_64BIT_NIL_VALUE==value)
    {
      SetPMap(0);
      return 0;
    }
		break;
	case FAST_OP_COPY:
		if (defaultValue[pos].int64_value==value)
    {
      SetPMap(0);
      return 0;
    }
		break;
	case FAST_OP_INCR:
		if (defaultValue[pos].int64_value+1==value)
		{
      SetPMap(0);
      SetInt64Value(pos,next);
			return 0;
		}
		break;
	case FAST_OP_DELTA:
		{
			int64_t delta =  value - defaultValue[pos].int64_value;
			SetInt64Value(pos,next);
			return EmitInt64 (delta);
		}
	default:
		break;
	}
  SetPMap();
  SetInt64Value(pos,next);
	return EmitInt64 (value); // emit the value
}


int FastCodec::EncodeDouble( int op, int pos, double value,int decimalPlaces)
{
  if(value==value)
  {
    return EncodeInt64(op,pos,int64_t(Round(value*pow(10.0,decimalPlaces))));
  }
  else
  {
    return EncodeInt64(op,pos,WMDF_CONSTS_64BIT_NIL_VALUE);
  }
}

double FastCodec::Round(double number)
{
  return number < 0.0 ? ceil(number - 0.5) : floor(number + 0.5);
}


//! @brief Encode an ASCII string

int FastCodec::EncodeStr (int op,int pos,uint8_t* value, int size)
{
	// Note: We don't prune the string in the codec layer
	// Pruning should be done by the app codec
	switch (op)
	{
	case FAST_OP_NONE:
    return EmitStr (value, size);
	case FAST_OP_DEFAULT:
    if (strcmp((char*)WMDF_CONSTS_STRING_NIL_VALUE,(char*)value)==0)
    {
      SetPMap(0);
      return 0;
    }
    SetPMap();
    return EmitStr (value, size);
	case FAST_OP_COPY:
		if (memcmp (defaultValue[pos].str_value, value, size) == 0)
    {
      SetPMap(0);
      return 0;
    }
		break;
	default:
		return 0;
	}
	SetPMap();
  SetStrValue(pos,value,size);
	return EmitStr (value, size);
}


int FastCodec::DecodeNewMsg (Fast_PMap_T* pmap,TValue* value)
{
  if(buffer.Get_Buffer_Left()<=0)
    return -1;
	currentPMap=pmap;
	defaultValue = value;
	memset(currentPMap->bits,0,pmap->size);
  currentPMap->max_pos = 0;
	uint32_t size = align7 (pmap->size);
	return Parse_PMap(size);
}

int FastCodec::DecodeEndMsg (Fast_PMap_T* parentPmap,TValue* parentDefaultValue)
{
  currentPMap=parentPmap;
  defaultValue = parentDefaultValue;
	return 0;
}

//! @brief Decode an ASCII string

int FastCodec::DecodeStr (int op,int pos,uint8_t* data, int size)
{
	int bytes;
	switch (op)
	{
  case FAST_OP_NONE:
    return ParseStr(data, size);
  case FAST_OP_DEFAULT:
    if(GetPMap()==1)
    {
      return ParseStr(data, size);
    }
    data=WMDF_CONSTS_STRING_NIL_VALUE;
    return 0;
	case FAST_OP_COPY:
    if(GetPMap()==0)
    {
      memcpy (data, defaultValue[pos].str_value, size);
      return 0;
    }
    bytes = ParseStr (data, size);
    if (bytes > 0)
    {
      SetStrValue(pos,data, size);
      return size;
    }
    return -1;
	default:
		return -1;
	}
}

int FastCodec::DecodeInt32 (int op,int pos, int32_t& value)
{
	int32_t temp = 0;
	int bytes;

	switch (op)
	{
	case FAST_OP_NONE:
		return ParseInt32 (value);
	case FAST_OP_DEFAULT:
    if(GetPMap()==0)
    {
      value =WMDF_CONSTS_32BIT_NIL_VALUE;
      return 0;
    }
		break;
	case FAST_OP_COPY:
    if(GetPMap()==0)
    {
       value =defaultValue[pos].int32_value;
       return 0;
    }
		break;
	case FAST_OP_INCR:
    if(GetPMap()==0)
    {
      value = defaultValue[pos].int32_value+1;
      SetIntValue(pos,value);
      return 0;
    }
		break;
	case FAST_OP_DELTA:
		int delta;
		bytes = ParseInt32(delta);
		if (bytes <= 0)
			return -1;
    else
		{
			temp = defaultValue[pos].int32_value + delta;
      value = temp;
      SetIntValue(pos,value);
      return bytes;
		}
		break;
	default:
		return -1;
	}

  bytes = ParseInt32 (value);
  if (bytes <= 0)
    return -1;
  SetIntValue(pos,value);
	return bytes;
}

int FastCodec::DecodeInt64 (int op,int pos, int64_t& value)
{

	int64_t temp = 0;
	int bytes;

	switch (op)
	{
  case FAST_OP_NONE:
    return ParseInt64 (value);
	case FAST_OP_DEFAULT:
    if(GetPMap()==0)
    {
      value =WMDF_CONSTS_64BIT_NIL_VALUE;
      return 0;
    }
		break;
	case FAST_OP_COPY:
    if(GetPMap()==0)
    {
      value =defaultValue[pos].int64_value;
      return 0;
    }
		break;
	case FAST_OP_INCR:
    if(GetPMap()==0)
    {
      value = defaultValue[pos].int64_value+1;
      SetInt64Value(pos,value);
      return 0;
    }
		break;
	case FAST_OP_DELTA:
		int64_t delta;
		bytes = ParseInt64 (delta);
		if (bytes <= 0) // error state already set
			return -1;
    else
		{
			temp = defaultValue[pos].int64_value + delta;
      value = temp;
			SetInt64Value(pos,temp);
		}
		break;
	default:
		return -1;
	}
  bytes = ParseInt64 (value);
  if (bytes <= 0)
    return -1;
  SetInt64Value(pos,value);
  return bytes;
}


int FastCodec::DecodeDouble( int op, int pos, double& value,int decimalPlaces )
{
  int64_t temp ;
  DecodeInt64(op,pos,temp);
  if(temp==WMDF_CONSTS_64BIT_NIL_VALUE)
  {
    value = WMDF_CONSTS_DOUBLE_NIL_VALUE;
  }
  else
  {
    value = (double)temp/(pow(10.0,decimalPlaces));

  }
  return 0;
}



void FastCodec::SetIntValue(int pos,int32_t& value)
{
	defaultValue[pos].int32_value=value;
}
void FastCodec::SetInt64Value(int pos,int64_t& value)
{
	defaultValue[pos].int64_value=value;
}
void FastCodec::SetStrValue(int pos ,const void* value,int size)
{
  memcpy (defaultValue[pos].str_value, value, size);
}

//! @brief Append a byte to the message buffer

void FastCodec::Emit_Byte(uint8_t data)
{
	//if(*buffer.tail>=*buffer.end)
	//{
	//	buffer.RiseBuffer();
	//}
	*buffer.tail = data;
  buffer.tail ++;
}

//! Serialize the current presence map

int FastCodec::Emit_PMap (uint8_t* data)
{
	int bytes = align7 (currentPMap->size); // ½«PMapÐ´ËÀ
	int offset;
	int p1;

	// pack slots from map->bits (seven at a time) into
	// data to form a STOP bit coded field.

	for (p1 = 0, offset = 0 ; p1 < bytes ; p1 ++, offset += 7)
	{
		uint8_t* bits = & currentPMap->bits [offset];
		uint32_t temp = 0;
		int p2;

		for (p2 = 0 ; p2 < 7 ; p2 ++) // pack 7 slot into temp
		{
			if (bits [p2] > 0)
				temp |= bit_mask [p2];
		}

		data [p1] = temp;
	}

	data [bytes - 1] |= 0x80; // Set the STOP bit
  //std::cout<<"±àÂëbyteÊý£º"<<bytes<<std::endl;
	return bytes;
}

//! @brief Serialize a 32-bit signed integer field

int FastCodec::Emit_Int32 (int data)
{
	int size = Int32_to_Size (data);
	switch (size)
	{
	case 5: Emit_Byte ((data >> 28) & 0x7f);
	case 4: Emit_Byte ((data >> 21) & 0x7f);
	case 3: Emit_Byte ((data >> 14) & 0x7f);
	case 2: Emit_Byte ((data >>  7) & 0x7f);
	case 1: Emit_Byte ((data & 0x7f) | 0x80);
     return 0;
	default:
		return -1;
	}
}

int FastCodec::EmitInt64 (int64_t data)
{
	int size = Int64_to_Size (data);

	switch (size)
	{
	case 10:Emit_Byte ((data >> 63) & 0x7f);
	case 9:	Emit_Byte ((data >> 56) & 0x7f);
	case 8:	Emit_Byte ((data >> 49) & 0x7f);
	case 7:	Emit_Byte ((data >> 42) & 0x7f);
	case 6:	Emit_Byte ((data >> 35) & 0x7f);
	case 5: Emit_Byte ((data >> 28) & 0x7f);
	case 4: Emit_Byte ((data >> 21) & 0x7f);
	case 3: Emit_Byte ((data >> 14) & 0x7f);
	case 2: Emit_Byte ((data >>  7) & 0x7f);
	case 1: Emit_Byte ((data & 0x7f) | 0x80);
    return 0;
	default:
		return -1;
	}
}

int FastCodec::Emit_UInt32 (uint32_t data)
{
	int size = UInt32_To_Size (data);

	switch (size)
	{
	case 5: Emit_Byte (data >> 28);
	case 4: Emit_Byte ((data >> 21) & 0x7f);
	case 3: Emit_Byte ((data >> 14) & 0x7f);
	case 2: Emit_Byte ((data >>  7) & 0x7f);
	case 1: Emit_Byte ((data & 0x7f) | 0x80);
    return 0;
	default:
		return -1;
	}
}


//! @brief Serialize an ASCII string field

int FastCodec::EmitStr (uint8_t* data, int size)
{
	if (size <= 0)
		return -1;
  int p1;
	for (p1 = 0 ; p1 < size - 1 ; p1 ++)
		Emit_Byte (data [p1]&0x7F);

	Emit_Byte (data [p1] | 0x80);
  return 0;
}


int FastCodec::Parse_Bytes (uint8_t* data, int size)
{
	int p1;
	uint8_t temp;
	memset (data, 0, size);
	for (p1 = 0 ; p1 < size ; p1 ++)
	{
		temp = *buffer.tail ++;

		data [p1] = temp;

		if (temp >= 0x80)
		{
			data [p1] &= 0x7f;
			return p1 + 1;
		}
	}

	return -1;
}

int FastCodec::Parse_PMap (int size)
{
	uint8_t* data= new uint8_t[size];

	int bytes = Parse_Bytes (data, size);
	int offset;
	int p1;

	if (bytes < 0)
		return bytes;

	for (p1 = 0, offset = 0 ; p1 < bytes ; p1 ++, offset += 7)
	{
		uint32_t temp = data [p1];
		int p2;
		for (p2 = 0 ; p2 < 7 ; p2 ++)
		{
			currentPMap->bits [offset + p2] = (temp & bit_mask [p2]) != 0;
		}
	}
	currentPMap->size = 7 * bytes;
	delete[] data;
	return bytes;
}

#define SIGN_MASK_I32 0x40
#define SIGN_MASK_U32 0x00

int FastCodec::Parse32 (uint32_t& value, int sign_mask)
{
	uint8_t  buffer [5];
	int bytes;
	int p1;
	int temp;

	bytes = Parse_Bytes (buffer, sizeof (buffer));

	if (bytes < 0)
		return bytes;

	temp = 0 - ((buffer [0] & sign_mask) != 0);

	for (p1 = 0 ; p1 < bytes ; p1 ++)
		temp = (temp << 7) | buffer [p1];

	value = temp;
  //std::cout<<"½âÂëbyteÊý£º"<<bytes<<std::endl;
	return bytes;
}

int FastCodec::Parse_UInt32 (uint32_t& value)
{
	return Parse32 (value, SIGN_MASK_U32);
}

int FastCodec::ParseInt32 (int32_t& data)
{
	return Parse32 ((uint32_t&) data, SIGN_MASK_I32);
}

int FastCodec::Parse_64 (uint64_t& value, int sign_mask)
{
	uint8_t  buffer [10];
	int bytes;
	int p1;
	int64_t temp;

	bytes = Parse_Bytes (buffer, sizeof (buffer));

	if (bytes < 0)
		return bytes;

	temp = 0 - ((buffer [0] & sign_mask) != 0);

	for (p1 = 0 ; p1 < bytes ; p1 ++)
		temp = (temp << 7) | buffer [p1];

	value = temp;
  //std::cout<<"½âÂëbyteÊý£º"<<bytes<<std::endl;
	return bytes;
}

int FastCodec::ParseInt64 (int64_t& data)
{
	return Parse_64 ((uint64_t&) data, SIGN_MASK_I32);
}

int FastCodec::ParseStr (uint8_t* data, int size)
{
  int count=Parse_Bytes (data, size);
  //std::cout<<"½âÂëbyteÊý£º"<<count<<std::endl;
	return count;
}


void FastCodec::SetPMap (int value)
{
  int pos = currentPMap->max_pos++;
	currentPMap->bits [pos] = value;
}

int FastCodec::UInt32_To_Size (uint32_t data)
{
	if (data < 0x00000080) return 1; // 128
	if (data < 0x00004000) return 2; // 16384
	if (data < 0x00200000) return 3; // 2097152
	if (data < 0x10000000) return 4; // 268435456

	return 5;
}

int FastCodec::Int32_to_Size (int data)
{
	if (data < 0)
	{
		if (data >= 0xffffffc0) return 1; // -64
		if (data >= 0xffffe000) return 2; // -8192
		if (data >= 0xfff00000) return 3; // -1048576
		if (data >= 0xf8000000) return 4; // -124317728
	}
	else
	{
		if (data <  0x00000040) return 1; // 64
		if (data <  0x00002000) return 2; // 8192
		if (data <  0x00100000) return 3; // 1048576
		if (data <  0x08000000) return 4; // 134217728
	}
	return 5;
}

int FastCodec::Int64_to_Size (int64_t data)
{
	if (data < 0)
	{
		if (data >= 0xffffffffffffffc0) return 1; // -64
		if (data >= 0xffffffffffffe000) return 2; // -8192
		if (data >= 0xfffffffffff00000) return 3; // -1048576
		if (data >= 0xfffffffff8000000) return 4; // -124317728
		if (data >= 0xFFFFFFFC00000000) return 5; //
		if (data >= 0xFFFFFE0000000000) return 6; //
		if (data >= 0xFFFF000000000000) return 7; //
		if (data >= 0xFF80000000000000) return 8; //
		if (data >= 0xC000000000000000) return 9; //
	}
	else
	{
		if (data <  0x0000000000000040) return 1; // 2^6
		if (data <  0x0000000000002000) return 2; // 2^13
		if (data <  0x0000000000100000) return 3; // 2^20
		if (data <  0x0000000008000000) return 4; // 2^27
		if (data <  0x0000000400000000) return 5; // 2^34
		if (data <  0x0000020000000000) return 6; // 2^41
		if (data <  0x0001000000000000) return 7; // 2^48
		if (data <  0x0080000000000000) return 8; // 2^55
		if (data <  0x4000000000000000) return 9; //2^62

	}
	return 10;
}


int FastCodec::GetPMap ()
{
  int pos = currentPMap->max_pos++;
	if (pos >= currentPMap->size)
		return 0;
	return currentPMap->bits [pos];
}

int FastCodec::Check_Msg_Avail ()
{
	if (buffer.Get_Buffer_Left () < 102400)
		buffer.RiseBuffer();
	return 0;
}
}
#endif

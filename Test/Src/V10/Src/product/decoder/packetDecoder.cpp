#include "packetDecoder.h"

using namespace wmdf;
using namespace std;

string PacketDecoder::ReadString()
{
	unsigned int strlen = ReadUInt32();
	if ( (strlen == 0) || m_error_outofrange)
		return "";

	if ( (strlen > m_size - m_pos ) || (strlen > 4096) )  // max string len: 4095
	{
		m_error_outofrange = true;
		return "";
	}

	char* str = new char[strlen + 1];
	memcpy(str, &(m_decodeBuf[m_pos]), strlen);
	str[strlen] = 0x00;
	m_pos += strlen;
	string result(str);
	delete[] str;

	return result;
}





uint32_t PacketDecoder::ReadChinaMarketTime()
{
	// encoded time value
	if (m_pos > m_size - 2)
	{
		m_error_outofrange = true;
		return 0L;
	}

	uint32_t etv = (m_decodeBuf[m_pos] << 8) | m_decodeBuf[m_pos+1];
	m_pos += 2;

	unsigned int result = ((etv >> 12) + 8) * 10000 + ((etv >> 6) & 0x3F) * 100 + (etv & 0x3F);
	if ( (result < 80000) || (result > 220000) )
	{
		m_error_outofrange = true;
		return 0L;
	}

	return result;
}

uint32_t PacketDecoder::ReadUInt32()
{
	uint32_t result = 0;
	int encoded_size = 0;
	if (m_pos > m_size - 1)
	{
		m_error_outofrange = true;
		return 0;
	}

	while (m_decodeBuf[m_pos + encoded_size] < 0x80)
	{
		encoded_size++;
		if (encoded_size > MAX_ENCODED_INT32_SIZE)
		{
			m_error_outofrange = true;
			return 0;
		}
		if (encoded_size > (int)(m_size - m_pos - 1))   // out of range error
		{
			m_error_outofrange = true;
			return 0;
		}
	}

	encoded_size++;
	for ( unsigned int i = m_pos; i < m_pos + encoded_size - 1; i++)
		result = (result << 7) | m_decodeBuf[i];

	result = (result << 7) | (m_decodeBuf[m_pos + encoded_size - 1] & 0x7F);
	m_pos += encoded_size;
	return result;
}

//Added by Yuan.z.j on 2010-08-13
uint64_t PacketDecoder::ReadUInt64()
{
	uint64_t result = 0;
	int encoded_size = 0;
	if (m_pos > m_size - 1 )
	{
		m_error_outofrange = true;
		return 0;
	}

	while (m_decodeBuf[m_pos + encoded_size] < 0x80)
	{
		encoded_size++;
		if (encoded_size > MAX_ENCODED_INT32_SIZE)
		{
			m_error_outofrange = true;
			return 0;
		}
		if (encoded_size > (int)(m_size - m_pos - 1))   // out of range error
		{
			m_error_outofrange = true;
			return 0;
		}
	}

	encoded_size++;
	for ( unsigned int i = m_pos; i < m_pos + encoded_size - 1; i++)
		result = (result << 7) | m_decodeBuf[i];

	result = (result << 7) | (m_decodeBuf[m_pos + encoded_size - 1] & 0x7F);
	m_pos += encoded_size;
	return result;
}

int64_t PacketDecoder::ReadInt64()
{
	if (m_pos > m_size - 1)
	{
		m_error_outofrange = true;
		return 0;
	}

	int64_t result = 0;
	if ((m_decodeBuf[m_pos] & 0x40) != 0)	//·ûºÅÎ»
		result = -1;

	int encoded_size = 0;
	while (m_decodeBuf[m_pos + encoded_size] < 0x80)    // warning: might be out of range
	{
		encoded_size++;
		if (encoded_size > MAX_ENCODED_INT64_SIZE)
		{
			m_error_outofrange = true;
			return 0;
		}
		if (encoded_size > (int)(m_size - m_pos - 1) )  // out of range error
		{
			m_error_outofrange = true;
			return 0;
		}
	}

	encoded_size++;
	for ( unsigned int i = m_pos; i < m_pos + encoded_size - 1; i++)
		result = (result << 7) | m_decodeBuf[i];

	result = (result << 7) | (m_decodeBuf[m_pos + encoded_size - 1] & 0x7F);
	m_pos += encoded_size;
	return result;
}




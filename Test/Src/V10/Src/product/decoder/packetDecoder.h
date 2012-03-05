#ifndef _PACKETDECODER_H
#define _PACKETDECODER_H

#include "windCodeCodec.h"
#include "utilsString.h"
#include "../../include/wmdf_api.h"

namespace wmdf
{

  const uint32_t MAX_ENCODED_INT32_SIZE = 6;
  const uint32_t MAX_ENCODED_INT64_SIZE = 9;
  const unsigned int MAX_DATA_REQUEST_SIZE_ALLOWED   =  2097152;
  const unsigned int MAX_DATA_PACKET_SIZE_ALLOWED   =   4194304;


  class PacketDecoder
  {
  private:
    unsigned char *m_decodeBuf;
    unsigned int m_size;
    unsigned int m_pos;
  public:
    bool m_error_outofrange;

    PacketDecoder(const unsigned int& packetLen, unsigned char* packetBuf)
      :m_size(packetLen), m_pos(0) 
    {
      m_decodeBuf = new unsigned char[packetLen];
      memcpy(m_decodeBuf, packetBuf, packetLen);

      m_error_outofrange = false;
    }

    ~PacketDecoder() 
    { 
      delete[] m_decodeBuf; 
    }

    uint32_t ReadUInt32();
    uint64_t ReadUInt64();
    int64_t ReadInt64();

	inline std::string ReadWindCode()
    {
	  std::string result = WindCodeCodec::DecodeWindCode(&(m_decodeBuf[m_pos]), &m_pos, m_size - m_pos);
      UpperCaseString(result);
      return result;
    }


    inline unsigned int SkipBytes(unsigned int buflen)
    {
      if (m_pos + buflen > m_size)
      {
        m_error_outofrange = true;
        return 0;
      }
      m_pos += buflen;
      return buflen;
    }

    inline unsigned int ReadBytes(unsigned int buflen, unsigned char *buf)
    {
      if (m_pos + buflen > m_size)
      {
        m_error_outofrange = true;
        return 0;
      }

      memcpy(buf, &(m_decodeBuf[m_pos]), buflen);
      m_pos += buflen;
      return buflen;
    }

    uint32_t ReadChinaMarketTime();
    std::string ReadString();

  };

}

#endif // _PACKETDECODER_H

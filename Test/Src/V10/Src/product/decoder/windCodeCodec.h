#ifndef _WINDCODECODEC_H
#define _WINDCODECODEC_H
#include <boost/thread/mutex.hpp>

#include <string>
#include <vector>
#ifdef __linux__
#include <ext/hash_map>
using namespace __gnu_cxx;
#else
#include <hash_map>
using namespace stdext;
#endif

namespace wmdf
{

struct WindCodeCodecLookUpTableItem
{
	unsigned char IdCode;
	unsigned int CodePrefix;
	unsigned int CodeMax;
	unsigned int CodeLen;
	unsigned int PriceUnit;
	char* Suffix;
};

class WindCodeCodec
{
private:
	static char* FillZeroFormatString[20];
	static WindCodeCodecLookUpTableItem static_lookup_table[128];
	static bool LookUpTableInited;

  static std::vector<std::string> abbr_suffix_table;

	static boost::mutex mutex_hashed_encoded_windcodes;
  static hash_map<int, std::string> hashtable_encoded_windcodes;

	static void InitAbbrSuffixTable();

	static void InitWindCodeCodecLookUpTableItem(WindCodeCodecLookUpTableItem* item,
		unsigned char idCode, unsigned int codePrefix,
		unsigned int codeMax, unsigned int codeLen, unsigned int priceUnit, const std::string& suffix);


	static std::string DecodeNoneEncodedWindCode(unsigned char* encoded_buf,
		unsigned int* buf_pos, const unsigned int remaining_buf_size);
	static std::string DecodeSuffixedWindCode(unsigned char* encoded_buf,
		unsigned int* buf_pos, const unsigned int remaining_buf_size);
	static std::string DecodeNVOFWindCode(unsigned char* encoded_buf,
		unsigned int* buf_pos, const unsigned int remaining_buf_size);
public:
	static void InitWindCodeCodecLookUpTable();

	static std::string DecodeWindCode(unsigned char *encoded_buf,
		unsigned int *buf_pos, const unsigned int remaining_buf_size);
};

}

#endif // _WINDCODECODEC_H

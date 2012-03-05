#include <boost/thread/mutex.hpp>
#include "windCodeCodec.h"
//#include "stopBitCodec.h"
#include "packetDecoder.h"
#ifdef __linux__
#include <cstdio>
#endif

using namespace wmdf;
using namespace std;


bool WindCodeCodec::LookUpTableInited = false;

WindCodeCodecLookUpTableItem WindCodeCodec::static_lookup_table[128];
hash_map<int, string> WindCodeCodec::hashtable_encoded_windcodes;
vector<string> WindCodeCodec::abbr_suffix_table;
boost::mutex WindCodeCodec::mutex_hashed_encoded_windcodes;

char* WindCodeCodec::FillZeroFormatString[20] =
{
	"%u", "%01u", "%02u", "%03u", "%04u", "%05u",
	"%06u", "%07u", "%08u", "%09u", "%10u", "%011u",
	"%012u", "%013u", "%014u", "%015u", "%016u", "%017u",
	"%018u", "%019u"
};

// pending: optimization
void WindCodeCodec::InitAbbrSuffixTable()
{
	abbr_suffix_table.push_back("SH");
	abbr_suffix_table.push_back("SZ");
	abbr_suffix_table.push_back("HK");
	abbr_suffix_table.push_back("IB");
	abbr_suffix_table.push_back("DQ");
	abbr_suffix_table.push_back("OF");
	abbr_suffix_table.push_back("WI");
	abbr_suffix_table.push_back("MI");
	abbr_suffix_table.push_back("SI");
	abbr_suffix_table.push_back("XI");
	abbr_suffix_table.push_back("CI");
	abbr_suffix_table.push_back("IL");
	abbr_suffix_table.push_back("HI");
	abbr_suffix_table.push_back("GI");
	abbr_suffix_table.push_back("NM");
	abbr_suffix_table.push_back("US");
	abbr_suffix_table.push_back("SG");
	abbr_suffix_table.push_back("OV");
	abbr_suffix_table.push_back("ZC");
	abbr_suffix_table.push_back("CRB");
	abbr_suffix_table.push_back("IR");
	abbr_suffix_table.push_back("FX");
	abbr_suffix_table.push_back("CZC");
	abbr_suffix_table.push_back("DCE");
	abbr_suffix_table.push_back("SHF");
	abbr_suffix_table.push_back("IPE");
	abbr_suffix_table.push_back("NYM");
	abbr_suffix_table.push_back("ME");
	abbr_suffix_table.push_back("CFF");
}

void WindCodeCodec::InitWindCodeCodecLookUpTableItem(WindCodeCodecLookUpTableItem* item,
											 unsigned char idCode, unsigned int codePrefix,
											 unsigned int codeMax, unsigned int codeLen, unsigned int priceUnit, const string& suffix)
{
	if ( (idCode == 0) || suffix.empty() )
	{
		item->IdCode = 0;
		item->CodeMax = 0;
		item->CodeLen = 0;
		item->Suffix = NULL;
	}
	else
	{
		item->IdCode = idCode;
		item->CodePrefix = codePrefix;
		item->CodeMax = codeMax;
		item->CodeLen = codeLen;
		item->PriceUnit = priceUnit;
		unsigned int suffixLen = suffix.length() + 1;
		item->Suffix = new char[suffixLen];
		strcpy(item->Suffix, suffix.c_str());
	}
}

void WindCodeCodec::InitWindCodeCodecLookUpTable()
{
	if (LookUpTableInited == true)
		return;

	for (int i = 0; i < 128; i++)
		InitWindCodeCodecLookUpTableItem(&(static_lookup_table[i]), 0, 0, 0, 0, 0, "");

	InitWindCodeCodecLookUpTableItem(&(static_lookup_table[3]), 3, 0, 999, 6, 100, "SH");
	InitWindCodeCodecLookUpTableItem(&(static_lookup_table[4]), 4, 399000, 399999, 6, 100, "SZ");

	for (int i = 60; i <= 69; i++)
		InitWindCodeCodecLookUpTableItem(&(static_lookup_table[i]), i, 600000+(i-60)*100, 600000+(i-59)*100-1, 6, 100, "SH");

	for (int i = 40; i <= 49; i++)
		InitWindCodeCodecLookUpTableItem(&(static_lookup_table[i]), i, 601000+(i-40)*100, 601000+(i-39)*100-1, 6, 100, "SH");

	for (int i = 30; i <= 39; i++)
		InitWindCodeCodecLookUpTableItem(&(static_lookup_table[i]), i, 0+(i-30)*100, 0+(i-29)*100-1, 6, 100, "SZ");

	InitWindCodeCodecLookUpTableItem(&(static_lookup_table[2]), 2, 2000, 2999, 6, 100, "SZ");

	InitWindCodeCodecLookUpTableItem(&(static_lookup_table[50]), 50, 500000, 500999, 6, 1000, "SH");
	InitWindCodeCodecLookUpTableItem(&(static_lookup_table[18]), 18, 184000, 184999, 6, 1000, "SZ");

	InitWindCodeCodecLookUpTableItem(&(static_lookup_table[58]), 58, 580000, 580999, 6, 1000, "SH");
	InitWindCodeCodecLookUpTableItem(&(static_lookup_table[59]), 59, 580900, 580999, 6, 1000, "SH");

	InitWindCodeCodecLookUpTableItem(&(static_lookup_table[5]), 5, 30000, 31999, 6, 1000, "SZ");
	InitWindCodeCodecLookUpTableItem(&(static_lookup_table[6]), 6, 38000,  39999, 6, 1000, "SZ");

	InitWindCodeCodecLookUpTableItem(&(static_lookup_table[7]), 7, 900900, 900999, 6, 1000, "SH");
	InitWindCodeCodecLookUpTableItem(&(static_lookup_table[20]), 20, 200000, 209999, 6, 1000, "SZ");

	InitWindCodeCodecLookUpTableItem(&(static_lookup_table[16]), 16, 160000, 169999, 6, 1000, "SZ");
	InitWindCodeCodecLookUpTableItem(&(static_lookup_table[19]), 19, 150000, 159999, 6, 1000, "SZ");
	InitWindCodeCodecLookUpTableItem(&(static_lookup_table[51]), 51, 510000, 510999, 6, 1000, "SH");

	// --------------------- Modified by huang.z.d ---------------------
	// 港股由四位升级为五位
	for (int i = 70; i <= 79; i++)
		InitWindCodeCodecLookUpTableItem(&(static_lookup_table[i]), i, 0+(i-70)*100, 0+(i-69)*100-1, 5, 1000, "HK");
	for (int i = 80; i <= 89; i++)
		InitWindCodeCodecLookUpTableItem(&(static_lookup_table[i]), i, 1000+(i-80)*100, 1000+(i-79)*100-1, 5, 1000, "HK");
	for (int i = 90; i <= 99; i++)
		InitWindCodeCodecLookUpTableItem(&(static_lookup_table[i]), i, 3000+(i-90)*100, 3000+(i-89)*100-1, 5, 1000, "HK");

	InitWindCodeCodecLookUpTableItem(&(static_lookup_table[100]), 100, 0, 4999, 5, 1000, "HK");       // 其它港股，5位
	// --------------------- End Modify ---------------------

	// 指数编码

	InitWindCodeCodecLookUpTableItem(&(static_lookup_table[111]), 111, 800000, 899999, 6, 100, "SI");
	InitWindCodeCodecLookUpTableItem(&(static_lookup_table[112]), 112, 800000, 899999, 6, 100, "CI");
	InitWindCodeCodecLookUpTableItem(&(static_lookup_table[113]), 113, 800000, 899999, 6, 100, "WI");
	InitWindCodeCodecLookUpTableItem(&(static_lookup_table[114]), 114, 800000, 899999, 6, 100, "XI");
	InitWindCodeCodecLookUpTableItem(&(static_lookup_table[115]), 115, 130000, 999999, 6, 100, "MI");

	InitWindCodeCodecLookUpTableItem(&(static_lookup_table[121]), 121, 0, 999999, 6, 100, "SZ");
	InitWindCodeCodecLookUpTableItem(&(static_lookup_table[122]), 122, 0, 999999, 6, 100, "SH");

	// OF
	InitWindCodeCodecLookUpTableItem(&(static_lookup_table[29]), 29, 0, 999999, 6, 10000, "OF");

	InitAbbrSuffixTable();

	LookUpTableInited = true;
}


std::string WindCodeCodec::DecodeSuffixedWindCode(unsigned char *encoded_buf, unsigned int *buf_pos, const unsigned int remaining_buf_size)
{
	unsigned char suffix_code = encoded_buf[1];
	if (suffix_code < abbr_suffix_table.size())
	{
		string suffix = abbr_suffix_table[suffix_code];

		char* strcode = new char[16];
		unsigned int j = 0;
		for (unsigned int i = 2; i < 16; i++)
		{
			if (i > remaining_buf_size - 1)
			{
				delete strcode;
				return "";
			}

			if (encoded_buf[i] < 0x80)
				strcode[j++] = (char)encoded_buf[i];
			else
			{
				char last_char = (char)(encoded_buf[i] & 0x7F);
				strcode[j++] = last_char;
				strcode[j] = 0x00;
				break;
			}
		}

		string result(strcode);
		delete[] strcode;
		*buf_pos = (*buf_pos) + j + 2;
		return (result + "." + suffix);
	}
	else
		return DecodeNoneEncodedWindCode(encoded_buf, buf_pos, remaining_buf_size);
}

string WindCodeCodec::DecodeNoneEncodedWindCode(unsigned char *encoded_buf, unsigned int *buf_pos, const unsigned int remaining_buf_size)
{
	for (unsigned int i = 1; i < 24; i++)   // maximum windcode length, 16 bytes
	{
		if (i > remaining_buf_size - 1)
			return "";

		if (encoded_buf[i] > 0x80)
		{
			unsigned int encoded_size = i;
			char *codes = new char[encoded_size + 1];
			if (encoded_size > 1)
				memcpy(codes, &(encoded_buf[1]), encoded_size - 1);
			codes[encoded_size - 1] = encoded_buf[i] & 0x7F;
			codes[encoded_size] = 0x00;

			string result(codes);
			*buf_pos = (*buf_pos) + encoded_size + 1;
			delete[] codes;
			return result;
		}
	}

	return "";
}

std::string WindCodeCodec::DecodeNVOFWindCode(unsigned char* encoded_buf, unsigned int* buf_pos, const unsigned int remaining_buf_size)
{
	unsigned int encoded_size = 1;   // from 1
	unsigned int code_num = 0;
	while (encoded_buf[encoded_size] < 0x80)
	{
		code_num = (code_num << 7) | encoded_buf[encoded_size];
		encoded_size++;
		if (encoded_size > MAX_ENCODED_INT32_SIZE)
			return "";
		if (encoded_size > remaining_buf_size - 1)   // out of range error
			return "";
	}

	code_num = (code_num << 7) | (encoded_buf[encoded_size] & 0x7F);
	if (code_num > 9999999)
		return "";

	unsigned int strcode_size = 16;   // default 15 for maximum windcode
	char* str_code = new char[strcode_size];
	sprintf(str_code, "%06u", code_num);
	strcat(str_code,  "NV.OF");
	string result(str_code);
	delete[] str_code;

	*buf_pos = (*buf_pos) + 1 + encoded_size;

	return result;
}

string WindCodeCodec::DecodeWindCode(unsigned char *encoded_buf, unsigned int *buf_pos, const unsigned int remaining_buf_size)
{
	if (encoded_buf == NULL)
		return "";

	unsigned char idcode = encoded_buf[0];
	if (idcode == 0x80)
		return DecodeNoneEncodedWindCode(encoded_buf, buf_pos, remaining_buf_size);
	else if (idcode == 0xFF)
		return DecodeSuffixedWindCode(encoded_buf, buf_pos, remaining_buf_size);
	else if (idcode == 0xFB)  // 123
		return DecodeNVOFWindCode(encoded_buf, buf_pos, remaining_buf_size);

	idcode = idcode & 0x7F;
	int encoded_hash_key = idcode;

	WindCodeCodecLookUpTableItem* item = &(static_lookup_table[idcode]);
	if (item->IdCode == 0)
		return "";

	unsigned int encoded_size = 1;   // from 1
	while (encoded_buf[encoded_size] < 0x80)
	{
		encoded_hash_key = (encoded_hash_key << 8) | encoded_buf[encoded_size];
		encoded_size++;
		if (encoded_size > MAX_ENCODED_INT32_SIZE)
			return "";
		if (encoded_size > remaining_buf_size - 1)   // out of range error
			return "";
	}

	encoded_hash_key = (encoded_hash_key << 8) | (encoded_buf[encoded_size] & 0x7F);

	WindCodeCodec::mutex_hashed_encoded_windcodes.lock();
	string hashed_windcode = WindCodeCodec::hashtable_encoded_windcodes[encoded_hash_key];
	WindCodeCodec::mutex_hashed_encoded_windcodes.unlock();

	if (!hashed_windcode.empty())
	{
		*buf_pos = (*buf_pos) + 1 + encoded_size;
		return hashed_windcode;
	}

	unsigned int encoded_value = 0;
	for (unsigned int i = 0; i < encoded_size - 1; i++)
		encoded_value = (encoded_value << 7) | encoded_buf[i + 1];

	encoded_value = (encoded_value << 7) | (encoded_buf[encoded_size] & 0x7F);
	encoded_value += item->CodePrefix;

	// check for valid wind_code
	if (encoded_value > 99999999)
		return "";
	if ( (item->Suffix == NULL) || (item->CodeLen < 2) )
		return 0;

	unsigned int strcode_size = 25;   // default 24 for maximum windcode
	char* str_code = new char[strcode_size];

	sprintf(str_code,  FillZeroFormatString[item->CodeLen], encoded_value);
	strcat(str_code,  ".");
	strcat(str_code,  item->Suffix);
	string result(str_code);
	delete[] str_code;

	*buf_pos = (*buf_pos) + 1 + encoded_size;

	WindCodeCodec::mutex_hashed_encoded_windcodes.lock();
	WindCodeCodec::hashtable_encoded_windcodes[encoded_hash_key] = result;
	WindCodeCodec::mutex_hashed_encoded_windcodes.unlock();

	return result;
}

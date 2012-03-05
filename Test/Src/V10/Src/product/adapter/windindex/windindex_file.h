#ifndef WINDINDEX_FILE_H
#define WINDINDEX_FILE_H

#include <cstdio>
#include "../../../include/wmdf_api.h"
#include <string>

namespace wmdf
{
#pragma pack(push) 
#pragma pack(1)  
	typedef struct WindIndex
	{
		char windcode[36];
		char indexname[36];
		unsigned int tradedate;
		float preclose;
		float openprice;
		float highprice;
		float lowprice;
		float newprice;
		float amount;
		float volume;
	}WindIndex;
#pragma pack(pop)

	class WindIndexFile
	{
	public:
		WindIndexFile(const std::string &filename);
		virtual ~WindIndexFile();
		bool Open();
		void Close();
		bool IsExist();
		bool IsEOF();
		int64_t FileSize();
		bool IsModified();
		int64_t RemainBytes();
		WindIndex* ReadWindIndex(int32_t &);
	private:
		WindIndexFile();
		int64_t GetPos();
		bool SetPos(int64_t);
	private:
		FILE *file_;
		int64_t file_size_;
		std::string filepath_;
		time_t last_modified_time_;
		const static int RECORD_SIZE_ = 104;
	};
}


#endif
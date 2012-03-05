#ifndef CIQ_FILE_H
#define CIQ_FILE_H

#include <cstdio>
#include "../../../include/wmdf_api.h"
#include <string>

namespace wmdf
{
#pragma pack(push) 
#pragma pack(1)  
	typedef struct CIQIndex
	{
		char del;
		char windcode[6];
		char name[8];
		char preclose[8];
		char openprice[8];
		char amount[12];
		char highprice[8];
		char lowprice[8];
		char newprice[8];
		char skip[16];
		char volume[10];
		char tail[100];
	}CIQIndex;
#pragma pack(pop)

	class CIQFile
	{
	public:
		CIQFile(const std::string &filename);
		virtual ~CIQFile();
		bool Open();
		void Close();
		bool IsExist();
		bool IsEOF();
		bool IsModified();
		CIQIndex* ReadCIQIndex(int32_t &);
	private:
		int64_t GetPos();
		bool SetPos(int64_t);
		CIQFile();
	private:
		FILE *file_;
		std::string filepath_;
		int64_t file_size_;
		time_t last_modified_time_;
		const static int SIZE_OF_RECORD_ = 193;
	};
}

#endif

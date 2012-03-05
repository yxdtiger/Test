#ifndef CSINDEX_FILE_H
#define CSINDEX_FILE_H

#include "../../../include/wmdf_api.h"
#include <string>
#include <cstdio>

namespace wmdf
{

	class CSIndexFile
	{
	public:
		CSIndexFile(const std::string &filename);
		virtual ~CSIndexFile();
		bool Open();
		void Close();
		bool IsExist();
		bool IsEOF();
		int64_t FileSize();
		bool IsModified();
		void ReadLine();
		void GetLine(std::string &line);
	private:
		CSIndexFile();
		int64_t GetPos();
		bool SetPos(int64_t);
	private:
		FILE *file_;
		std::string filepath_;
		int64_t file_size_;
		time_t last_modified_time_;
		const static int line_length_ = 512;
		std::string curr_line_;
	};
}


#endif
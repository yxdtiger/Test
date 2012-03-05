#ifndef DAILY_RAWDATA_FILE_H
#define DAILY_RAWDATA_FILE_H

#include <string>
#include "../include/wmdf_api.h"

namespace wmdf
{
	class DailyRawdataFile
	{
	public:
		DailyRawdataFile(int32_t date);
		~DailyRawdataFile();
		bool IsWholeDay();
		void InsertAMFile(std::string);
		void InsertPMFile(std::string);
		void InsertWholeDayFile(std::string);
		std::string GetAMFile(){return filename_[0];}
		std::string GetPMFile(){return filename_[1];}
		std::string GetWholeDayFile(){return filename_[2];}
		std::string GetNextFile();
    bool IsLastFile();
    bool IsCompressed(const std::string& filename);
    void Uncompress(const std::string& filename);
    std::string GetUncompressedFile(const std::string& filename);
    int32_t date(){return date_;}
	private:
		std::string filename_[3];
    int32_t current_index_;
    int32_t date_;


	};
}



#endif

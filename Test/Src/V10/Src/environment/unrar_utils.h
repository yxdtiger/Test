#ifndef UNRAR_UTILS_H
#define UNRAR_UTILS_H

#ifdef WIN32
#include <windows.h>
#endif

#include <stdio.h>
#include <ctype.h>
#include "unrar.h"

namespace wmdf
{
	class UnrarUtils
	{
		enum { EXTRACT, TEST, PRINT, LIST };
	public:
		UnrarUtils();
		~UnrarUtils();
		static void ExtractArchive(char *filename,char* output);
	private:
		static void ExtractArchive(char *ArcName,int Mode,char* output);
		static void ListArchive(char *ArcName);
		static void ShowComment(char *CmtBuf);
		static void OutHelp(void);
		static void OutOpenArchiveError(int Error,char *ArcName);
		static void ShowArcInfo(unsigned int Flags,char *ArcName);
		static void OutProcessFileError(int Error);
		static int CALLBACK CallbackProc(UINT msg,LPARAM UserData,LPARAM P1,LPARAM P2);
	};
}


#endif

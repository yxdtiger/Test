#include "unrar_utils.h"
#include "../include/wmdf_api.h"
#include "string"
#ifdef __linux__
#include <cstring>
#define MAX_PATH 260
#endif

void wmdf::UnrarUtils::ExtractArchive(char *filename,char *output)
{
	ExtractArchive(filename,EXTRACT,output);
}

void wmdf::UnrarUtils::ExtractArchive( char *ArcName,int Mode,char *output )
{
	HANDLE hArcData;
	int RHCode,PFCode;
	char CmtBuf[16384];
	RARHeaderData headerdata;
	RAROpenArchiveDataEx openarchivedata;

	memset(&openarchivedata,0,sizeof(openarchivedata));
	openarchivedata.ArcName=ArcName;
	openarchivedata.CmtBuf=CmtBuf;
	openarchivedata.CmtBufSize=sizeof(CmtBuf);
	openarchivedata.OpenMode=RAR_OM_EXTRACT;
	openarchivedata.Callback=CallbackProc;
	openarchivedata.UserData=Mode;
	hArcData=RAROpenArchiveEx(&openarchivedata);

	if (openarchivedata.OpenResult!=0)
	{
		OutOpenArchiveError(openarchivedata.OpenResult,ArcName);
		return;
	}

	//ShowArcInfo(openarchivedata.Flags,ArcName);

	if (openarchivedata.CmtState==1)
		ShowComment(CmtBuf);

	// Obsolete, use RAROpenArchiveDataEx callback fields above.
	// RARSetCallback(hArcData,CallbackProc,(LPARAM)&Mode);

	headerdata.CmtBuf=NULL;
	memset(&openarchivedata.Reserved,0,sizeof(openarchivedata.Reserved));

	while ((RHCode=RARReadHeader(hArcData,&headerdata))==0)
	{
		switch(Mode)
		{
		case EXTRACT:
			printf("\nExtracting %-45s",headerdata.FileName);
			break;
		case TEST:
			printf("\nTesting %-45s",headerdata.FileName);
			break;
		case PRINT:
			printf("\nPrinting %-45s\n",headerdata.FileName);
			break;
		}

// 		std::string filename = ArcName;
// 		size_t pos = filename.find_last_of("\\");
// 		std::string file_path = filename.substr(0,pos);

		PFCode=RARProcessFile(hArcData,(Mode==EXTRACT) ? RAR_EXTRACT:RAR_TEST,
			output,NULL);
		if (PFCode==0)
			printf(" OK\n");
		else
		{
			OutProcessFileError(PFCode);
			break;
		}
	}

	if (RHCode==ERAR_BAD_DATA)
		printf("\nFile header broken");

	RARCloseArchive(hArcData);
}

void wmdf::UnrarUtils::ListArchive( char *ArcName )
{
	HANDLE hArcData;
	int RHCode,PFCode;
	char CmtBuf[16384];
	struct RARHeaderDataEx HeaderData;
	struct RAROpenArchiveDataEx OpenArchiveData;

	memset(&OpenArchiveData,0,sizeof(OpenArchiveData));
	OpenArchiveData.ArcName=ArcName;
	OpenArchiveData.CmtBuf=CmtBuf;
	OpenArchiveData.CmtBufSize=sizeof(CmtBuf);
	OpenArchiveData.OpenMode=RAR_OM_LIST;
	OpenArchiveData.Callback=CallbackProc;
	OpenArchiveData.UserData=LIST;
	hArcData=RAROpenArchiveEx(&OpenArchiveData);

	if (OpenArchiveData.OpenResult!=0)
	{
		OutOpenArchiveError(OpenArchiveData.OpenResult,ArcName);
		return;
	}

	ShowArcInfo(OpenArchiveData.Flags,ArcName);

	if (OpenArchiveData.CmtState==1)
		ShowComment(CmtBuf);

	// Obsolete, use RAROpenArchiveDataEx callback fields above.
	// RARSetCallback(hArcData,CallbackProc,0);

	HeaderData.CmtBuf=NULL;
	memset(&OpenArchiveData.Reserved,0,sizeof(OpenArchiveData.Reserved));

	printf("\nFile                       Size");
	printf("\n-------------------------------");
	while ((RHCode=RARReadHeaderEx(hArcData,&HeaderData))==0)
	{
		int64_t UnpSize=HeaderData.UnpSize+(((int64_t)HeaderData.UnpSizeHigh)<<32);
		printf("\n%-20s %10Ld ",HeaderData.FileName,UnpSize);
		if ((PFCode=RARProcessFile(hArcData,RAR_SKIP,NULL,NULL))!=0)
		{
			OutProcessFileError(PFCode);
			break;
		}
	}

	if (RHCode==ERAR_BAD_DATA)
		printf("\nFile header broken");

	RARCloseArchive(hArcData);
}

void wmdf::UnrarUtils::ShowComment( char *CmtBuf )
{
  printf("\nComment:\n%s\n",CmtBuf);
}

void wmdf::UnrarUtils::OutHelp( void )
{
	printf("\nUNRDLL.   This is a simple example of UNRAR.DLL usage\n");
	printf("\nSyntax:\n");
	printf("\nUNRDLL X <Archive>     extract archive contents");
	printf("\nUNRDLL T <Archive>     test archive contents");
	printf("\nUNRDLL P <Archive>     print archive contents to stdout");
	printf("\nUNRDLL L <Archive>     view archive contents\n");
}

void wmdf::UnrarUtils::OutOpenArchiveError( int Error,char *ArcName )
{
	switch(Error)
	{
	case ERAR_NO_MEMORY:
		printf("\nNot enough memory");
		break;
	case ERAR_EOPEN:
		printf("\nCannot open %s",ArcName);
		break;
	case ERAR_BAD_ARCHIVE:
		printf("\n%s is not RAR archive",ArcName);
		break;
	case ERAR_BAD_DATA:
		printf("\n%s: archive header broken",ArcName);
		break;
	case ERAR_UNKNOWN:
		printf("Unknown error");
		break;
	}
}

void wmdf::UnrarUtils::ShowArcInfo( unsigned int Flags,char *ArcName )
{
	printf("\nArchive %s\n",ArcName);
	printf("\nVolume:\t\t%s",(Flags & 1) ? "yes":"no");
	printf("\nComment:\t%s",(Flags & 2) ? "yes":"no");
	printf("\nLocked:\t\t%s",(Flags & 4) ? "yes":"no");
	printf("\nSolid:\t\t%s",(Flags & 8) ? "yes":"no");
	printf("\nNew naming:\t%s",(Flags & 16) ? "yes":"no");
	printf("\nAuthenticity:\t%s",(Flags & 32) ? "yes":"no");
	printf("\nRecovery:\t%s",(Flags & 64) ? "yes":"no");
	printf("\nEncr.headers:\t%s",(Flags & 128) ? "yes":"no");
	printf("\nFirst volume:\t%s",(Flags & 256) ? "yes":"no or older than 3.0");
	printf("\n---------------------------\n");
}

void wmdf::UnrarUtils::OutProcessFileError( int Error )
{
	switch(Error)
	{
	case ERAR_UNKNOWN_FORMAT:
		printf("Unknown archive format");
		break;
	case ERAR_BAD_ARCHIVE:
		printf("Bad volume");
		break;
	case ERAR_ECREATE:
		printf("File create error");
		break;
	case ERAR_EOPEN:
		printf("Volume open error");
		break;
	case ERAR_ECLOSE:
		printf("File close error");
		break;
	case ERAR_EREAD:
		printf("Read error");
		break;
	case ERAR_EWRITE:
		printf("Write error");
		break;
	case ERAR_BAD_DATA:
		printf("CRC error");
		break;
	case ERAR_UNKNOWN:
		printf("Unknown error");
		break;
	case ERAR_MISSING_PASSWORD:
		printf("Password for encrypted file is not specified");
		break;
	}
}

int CALLBACK wmdf::UnrarUtils::CallbackProc( UINT msg,LPARAM UserData,LPARAM P1,LPARAM P2 )
{
	switch(msg)
	{
	case UCM_CHANGEVOLUME:
		if (P2==RAR_VOL_ASK)
		{
			printf("\n\nVolume %s is required\nPossible options:\n",(char *)P1);
			printf("\nEnter - try again");
			printf("\n'R'   - specify a new volume name");
			printf("\n'Q'   - quit");
			printf("\nEnter your choice: ");
			switch(toupper(getchar()))
			{
			case 'Q':
				return(-1);
			case 'R':
				{
					char *eol;
					printf("\nEnter new name: ");
					fflush(stdin);
					fgets((char *)P1,MAX_PATH,stdin);
					eol=strpbrk((char *)P1,"\r\n");
					if (eol!=NULL)
						*eol=0;
				}
				return(0);
			default:
				return(0);
			}
		}
		if (P2==RAR_VOL_NOTIFY)
			printf("\n  ... volume %s\n",(char *)P1);
		return(0);
	case UCM_PROCESSDATA:
		if (UserData==PRINT)
		{
			fflush(stdout);
			fwrite((char *)P1,1,P2,stdout);
			fflush(stdout);
		}
		return(0);
	case UCM_NEEDPASSWORD:
		printf("\nPassword required: ");
		gets((char *)P1);
		return(0);
	}
	return(0);
}

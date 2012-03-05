// Wave.cpp : implementation file
//

#include "stdafx.h"
#include "Wave.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CWave

CWave::CWave() 
{ 
	m_nDeviceID=0; 
	m_nElementID=0;
} 

CWave::~CWave() 
{ 
	if(m_nElementID)
		Stop(); 
	if(m_nDeviceID) 
		CloseDevice(); 
} 

DWORD CWave::OpenDevice() 
{ 
	DWORD dwResult=0; 
	if ( !m_nDeviceID )
	{
		MCI_OPEN_PARMS mciOpenParms; 
		mciOpenParms.lpstrDeviceType=(LPSTR)MCI_DEVTYPE_WAVEFORM_AUDIO; 

		dwResult=mciSendCommand(NULL,MCI_OPEN, 
			MCI_OPEN_TYPE|MCI_OPEN_TYPE_ID|MCI_WAIT, 
			(DWORD)(LPVOID)&mciOpenParms); 

		m_nDeviceID=mciOpenParms.wDeviceID; 

		if(dwResult) 
			DisplayErrorMsg(dwResult); 
	} 
	return dwResult; 
} 

DWORD CWave::CloseDevice() 
{ 
	DWORD dwResult=0; 

	if(m_nDeviceID) 
	{
		dwResult=mciSendCommand(m_nDeviceID,MCI_CLOSE,NULL,NULL); 

		if(dwResult) 
			DisplayErrorMsg(dwResult);
		else
			m_nDeviceID=0; 
	} 
	return dwResult; 
} 

DWORD CWave::Play(CWnd* pWnd,LPCSTR pFileName) 
{ 
	MCI_OPEN_PARMS mciOpenParms; 
	memset(&mciOpenParms,0,sizeof(MCI_OPEN_PARMS)); 

	mciOpenParms.lpstrElementName=pFileName; 

	DWORD dwResult=mciSendCommand(m_nDeviceID,MCI_OPEN, 
		MCI_OPEN_ELEMENT,(DWORD)(LPVOID)&mciOpenParms); 

	if(dwResult) 
		DisplayErrorMsg(dwResult);
	else
	{
		m_nElementID=mciOpenParms.wDeviceID; 
		MCI_PLAY_PARMS mciPlayParms; 
		mciPlayParms.dwCallback=(DWORD)pWnd->GetSafeHwnd();
		dwResult=mciSendCommand(m_nElementID,MCI_PLAY,MCI_NOTIFY,(DWORD)(LPVOID)&mciPlayParms); 

		if(dwResult) 
		{ 
			DisplayErrorMsg(dwResult); 
			Stop();
		} 
	}
	return dwResult; 
} 

DWORD CWave::Stop() 
{
	DWORD dwResult=0;

	if(m_nElementID) 
	{ 
		dwResult=mciSendCommand(m_nElementID,MCI_CLOSE,NULL,NULL); 

		if(dwResult) 
			DisplayErrorMsg(dwResult);  
		else 
			m_nElementID=0; 
	} 
	return dwResult; 
} 

void CWave::DisplayErrorMsg(DWORD dwError) 
{ 
	if(dwError) 
	{
		char szErrorMsg[MAXERRORLENGTH]; 

		if(!mciGetErrorString(dwError,szErrorMsg,sizeof(szErrorMsg))) 
			strcpy(szErrorMsg,"Unknown Error"); 
		AfxMessageBox(szErrorMsg); 
	} 
} 

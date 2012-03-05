#if !defined(AFX_WAVE_H__10430C0F_918E_11D4_8B3A_0050BAD195A9__INCLUDED_)
#define AFX_WAVE_H__10430C0F_918E_11D4_8B3A_0050BAD195A9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Wave.h : header file
//

#include <mmsystem.h>

/////////////////////////////////////////////////////////////////////////////
// CWave window

class CWave:public CObject 
{ 
//Construction 
public: 
	CWave(); 
	virtual ~CWave(); 

//Operations 
public: 
	DWORD OpenDevice(); 
	DWORD CloseDevice(); 
	DWORD Play(CWnd *pParentWnd,LPCSTR pFileName); 
	DWORD Stop(); 

//Implementation 
protected: 
	void DisplayErrorMsg(DWORD dwError); 

//Members 
protected: 
	MCIDEVICEID m_nDeviceID; 
	MCIDEVICEID m_nElementID; 
}; 

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WAVE_H__10430C0F_918E_11D4_8B3A_0050BAD195A9__INCLUDED_)


// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently,
// but are changed infrequently

#pragma once

#ifndef _SECURE_ATL
#define _SECURE_ATL 1
#endif

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // Exclude rarely-used stuff from Windows headers
#endif

#include "targetver.h"

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // some CString constructors will be explicit

// turns off MFC's hiding of some common and often safely ignored warning messages
#define _AFX_ALL_WARNINGS

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions


#include <afxdisp.h>        // MFC Automation classes



#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // MFC support for Internet Explorer 4 Common Controls
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>             // MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxcontrolbars.h>     // MFC support for ribbons and control bars

#include "easysize.h"

//#include "ClipStyleReset.h"
// define message id

// 添加一个用户
const UINT WM_ADD_CUSTOMER = WM_USER + 100;

// 添加一个item
const UINT WM_ADD_MODIFY_ITEM = WM_USER + 101;


//// 添加一个产品描述
//const UINT WM_ADD_PRODUCT_DESCRIPTER = WM_USER + 102;
//
//// 增加一个版本描述
//const UINT WM_ADD_VERSION_DESCRIPTER = WM_USER + 103;
//
//// 增加一个消息模块描述
//const UINT WM_ADD_MESSAGE_DESCRIPTER = WM_USER + 104;
//
//// 添加一个产品源
//const UINT WM_ADD_PRODUCT_SOURCE = WM_USER + 105;
//
//// 添加一个产品源交易时间
//const UINT WM_ADD_PRODUCT_SOURCE_MARKET_TIME = WM_USER + 106;
//
//// 添加一个产品源备源
//const UINT WM_ADD_PRODUCT_SOURCE_BACKUP_SOURCE = WM_USER + 107;
//
//// 添加一个产品源订阅产品
//const UINT WM_ADD_PRODUCT_SOURCE_SUBSCRIBED_PRODUCT = WM_USER + 108;

const UINT WM_VIEW_PRODUCT_RECEIVER_MDFMESSAGE = WM_USER + 110;

const UINT WM_MONITOR_DATA_INIT_OK = WM_USER + 111;

const UINT WM_MONITOR_CLIENT = WM_USER + 112;
const UINT WM_MONITOR_TRANSFER = WM_USER + 113;
const UINT WM_MONITOR_WARNING = WM_USER + 114;
const UINT WM_MONITOR_LOG = WM_USER + 115;
const UINT WM_MONITOR_QUOTE =WM_USER + 116;
const UINT WM_L1_MKT_RECORD =WM_USER + 117;
const UINT WM_L2_MKT_RECORD =WM_USER + 118;
const UINT WM_L2_TRANS_RECORD =WM_USER + 119;
const UINT WM_L1_Future_MKT_RECORD =WM_USER + 120;
const UINT WM_L2_CFFEL2_MKT_RECORD =WM_USER + 121;
const UINT WM_MONITOR_NETWORK =WM_USER + 122;
const UINT WM_MONITOR_REMOVEVIEW =WM_USER + 123;
const UINT WM_MYREFRESH = WM_USER+100;

#ifdef _UNICODE
#if defined _M_IX86
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_IA64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='ia64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif
#endif



#pragma once
#include "SSEL2_StructDefine.h"

#define VALUE_ABSENT	-1000

enum E_SSE_ERROR_CODE
{
	E_SSE_OK						= 0,
	// 配置文件不存在;
	E_SSE_CONFIGFILE_NOTEXIST		= -1,
	// 配置文件打开失败;
	E_SSE_CONFIGFILE_OPENFAILED		= -2,
	// 模板路径为空;
	E_SSE_CONFIGFILE_NOXMLPATHITEM	= -3,
	// 配置文件中有空值;
	E_SSE_CONFIGFILE_EMPTYVALUE		= -4,
	// 解析XML模板失败;
	E_SSE_PARSE_FAIL				= -5,
	// 初始化未完成;
	E_SSE_INIT_UNCOMPLETE			= -6,
	// 传入的数据中存在心跳包;
	E_SSE_EXIST_HEARTBEAT			= -7,
	// XML文件不存在;
	E_SSE_XMLFILENOTEXIST			= -8
};

extern "C"
{
	/**
	*初始化函数;
	*@param:  bIsFilterHB  当等于true的时候过滤心跳包，反之不过滤;
	*@retval: 初始化信息;
	*/
	__declspec(dllexport) int SSEInit(bool bIsFilterHB);
	/**
	*解析SSE_L2行情数据;
	*@param:  pucRawData  需要解析的数据;
	*@param:  unRawDataLength  需要解析的数据长度;
	*@param:  ppVecFHData  指向存放解析后数据的vector指针的指针;
	*@retval: 1. E_SSE_OK	解析成功 2. E_SSE_INIT_UNCOMPLETE 初始化未完成3. E_SSE_EXIST_HEARTBEAT  存在心跳包;
	*/
	__declspec(dllexport) int SSEDecodeRawData(const unsigned char *pucRawData,unsigned int unRawDataLength,vecFeedHandlerData** ppVecFHData);
	/**
	*释放资源;
	*@param:  资源指针;
	*@retval: 无;
	*/
	__declspec(dllexport) void SSEReleaseBuffer(vecFeedHandlerData *ptrVecFHData);
	/**
	*卸载解码模块函数;
	*@param:  无;
	*@retval: E_SSE_OK;
	*/
	__declspec(dllexport) int SSEUninit();
}
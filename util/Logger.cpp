#include "../stdafx.h"
#include "Logger.h"
#include "Date.h"
#include <cstdio>
#include <cstdarg>

using String = std::string;

void Logger::Info(const char* szFormat, ...)
{
	va_list pArgList;
	va_start(pArgList, szFormat);
	char szBuffer[1024] = { 0x00 };
	size_t nszBufferSize = sizeof(szBuffer) / sizeof(char);
	Date date;
	sprintf_s(szBuffer, nszBufferSize,
		"[ INFO ] [ %04d-%02d-%02d %02d:%02d:%02d ] [ %4d ] ",
		date.GetYear(), date.GetMonth(), date.GetDay(), 
		date.GetHour(), date.GetMinute(), date.GetSecond(), GetCurrentThreadId());
	String strContent;
	strContent.append(szBuffer);
	memset(szBuffer, 0x00, nszBufferSize);
	vsnprintf_s(szBuffer, nszBufferSize, szFormat, pArgList);
	va_end(pArgList);
	strContent.append(szBuffer);
	fprintf_s(stdout, "%s\n", strContent.c_str());
}
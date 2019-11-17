#ifndef __STRING_UTIL_H__
#define __STRING_UTIL_H__

#include<string>
#include<tchar.h>

using String = std::string;
using WString = std::wstring;

class StringUtil
{
public:
	StringUtil() = delete;
	~StringUtil() = delete;

public:
	static int SundaySearch(const String& strText, const String& strSubText);
	static String GBK2UTF8(const char* strGBK);
	static String UTF82GBK(const char* strUTF8);
	static String UTF16_2GBK(const wchar_t* strUTF8);
	static WString String2WString(const String & str);
	static String WString2String(const WString & wstr);
	
};

#endif __STRING_UTIL_H__



#include "../stdafx.h"
#include "StringUtil.h"
#include <Windows.h>
#include <tchar.h>

int StringUtil::SundaySearch(const String& strText, const String& strSubText)
{
	int nTextLen = strText.length();
	int nSubTextLen = strSubText.length();
	if (nTextLen == 0 || nSubTextLen == 0 || nTextLen < nSubTextLen)
	{
		return -1;
	}
	int j = 0;
	for (int i = 0; i <= nTextLen - nSubTextLen; i++)
	{
		while (j < nSubTextLen && strText.at(i + j) == strSubText.at(j))
		{
			j++;
		}
		if (j == nSubTextLen)
		{
			return i;
		}
		int k = nSubTextLen - 1; // ×îÓÒÆ¥ÅäÔ­Ôò
		while (k >= 0)
		{
			if (strText.at(i + nSubTextLen) == strSubText.at(k))
			{
				break;
			}
			k--;
		}
		{
			k = k < 0 ? 0 : k;
			j = k;
			i += nSubTextLen - j - 1;
		}
	}
		return -1;
}

String StringUtil::GBK2UTF8(const char* strGBK)
{
	LPCSTR pszSrc = strGBK;
	int nLen = MultiByteToWideChar(CP_ACP, 0, pszSrc, -1, NULL, 0);
	if (!nLen) {
		return String("");
	}
	wchar_t* pwszSrc = new wchar_t[nLen] { 0x00 };
	if (!pwszSrc) {
		return String("");
	}
	nLen = MultiByteToWideChar(CP_ACP, 0, pszSrc, -1, pwszSrc, nLen);
	if (nLen <= 0)
	{
		delete[] pwszSrc;
		pwszSrc = NULL;
		return String("");
	}

	nLen = WideCharToMultiByte(CP_UTF8, 0, pwszSrc, -1, NULL, 0, NULL, NULL);
	if (!nLen) {
		delete[] pwszSrc;
		pwszSrc = NULL;
		return String("");
	}

	char* pszDst = new char[nLen] { 0x00 };
	if (!pszDst) {
		delete[] pwszSrc;
		pwszSrc = NULL;
		return String("");
	}
	nLen = WideCharToMultiByte(CP_UTF8, 0, pwszSrc, -1, pszDst, nLen, NULL, NULL);
	if (nLen > 0)
	{
		delete[] pwszSrc;
		pwszSrc = NULL;
		String str(pszDst);
		WString wstr = StringUtil::String2WString(str);
		delete[] pszDst;
		pszDst = NULL;
		return str;
	}

	delete[] pwszSrc;
	pwszSrc = NULL;
	delete[] pszDst;
	pszDst = NULL;
	return String("");
}

String StringUtil::UTF82GBK(const char* strUTF8)
{
	LPCSTR pszSrc = strUTF8;
	int nLen = MultiByteToWideChar(CP_UTF8, 0, pszSrc, -1, NULL, 0);
	if (!nLen) {
		return String("");
	}
	wchar_t* pwszSrc = new wchar_t[nLen] { 0x00 };
	if (!pwszSrc) {
		return String("");
	}
	nLen = MultiByteToWideChar(CP_UTF8, 0, pszSrc, -1, pwszSrc, nLen);
	if (nLen > 0)
	{
		nLen = WideCharToMultiByte(CP_ACP, 0, pwszSrc, -1, NULL, 0, NULL, NULL);
		char* pszDst = new char[nLen] { 0x00 };
		if (!pszDst) {
			delete[] pwszSrc;
			pwszSrc = NULL;
			return String("");
		}
		nLen = WideCharToMultiByte(CP_ACP, 0, pwszSrc, -1, pszDst, nLen, NULL, NULL);
		if (nLen > 0)
		{
			delete[] pwszSrc;
			pwszSrc = NULL;
			String str(pszDst);
			delete[] pszDst;
			pszDst = NULL;
			return str;
		}
	}
	delete[] pwszSrc;
	pwszSrc = NULL;
	return String("");
}

String StringUtil::UTF16_2GBK(const wchar_t* strUTF8)
{
	LPCWSTR pwszSrc = strUTF8;
	int nLen = WideCharToMultiByte(CP_ACP, 0, pwszSrc, -1, NULL, 0, NULL, NULL);
	if (nLen == 0) {
		return String("");
	}

	char* pszDst = new char[nLen] { 0x00 };
	if (!pszDst) {
		return String("");
	}

	nLen = WideCharToMultiByte(CP_ACP, 0, pwszSrc, -1, pszDst, nLen, NULL, NULL);
	if (nLen > 0)
	{
		String str(pszDst);
		delete[] pszDst;
		pszDst = NULL;
		return str;
	}

	delete[] pszDst;
	return String("");
}


WString StringUtil::String2WString(const String & str)
{
	LPCSTR pszSrc = str.c_str();
	int nLen = MultiByteToWideChar(CP_ACP, 0, pszSrc, -1, NULL, 0);
	if (!nLen) {
		return WString(L"");
	}
	wchar_t* pwszDst = new wchar_t[nLen]{ 0x00 };
	if (!pwszDst) {
		return WString(L"");
	}

	nLen = MultiByteToWideChar(CP_ACP, 0, pszSrc, -1, pwszDst, nLen);
	if (nLen > 0)
	{
		WString wstr(pwszDst);
		delete[] pwszDst;
		pwszDst = NULL;
		return wstr;
	}
	delete[] pwszDst;
	return WString(L"");
}

String StringUtil::WString2String(const WString & wstr)
{
	LPCWSTR pwszSrc = wstr.c_str();
	int nLen = WideCharToMultiByte(CP_UTF8, 0, pwszSrc, -1, NULL, 0, NULL, NULL);
	if (nLen == 0) {
		return String("");
	}

	char* pszDst = new char[nLen]{ 0x00 };
	if (!pszDst) {
		return String("");
	}

	nLen = WideCharToMultiByte(CP_UTF8, 0, pwszSrc, -1, pszDst, nLen, NULL, NULL);
	if (nLen > 0)
	{
		String str(pszDst);
		delete[] pszDst;
		pszDst = NULL;
		return str;
	}
	
	delete[] pszDst;
	return String("");
}

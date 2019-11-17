#include "../stdafx.h"
#include "File.h"


File::File(const String& strFileName, bool bCreate)
{
	if (strFileName.empty())
	{
		throw "file name not empty";
	}
	this->m_wstrFileName = StringUtil::String2WString(strFileName);
	DWORD dwCreationDisposition = 0;
	dwCreationDisposition = bCreate ? CREATE_ALWAYS : OPEN_ALWAYS;// 存在打开，不存在创建
	this->m_hFile = ::CreateFileW(
		m_wstrFileName.c_str(),
		GENERIC_READ |GENERIC_WRITE, // 写文件
		FILE_SHARE_READ,
		NULL,
		dwCreationDisposition,
		FILE_ATTRIBUTE_NORMAL, // 常规文件属性
		NULL // 无模板
		);
	if (INVALID_HANDLE_VALUE == this->m_hFile)
	{
		throw "open or create file failed";
	}
}

File::~File()
{
	if (this->m_hFile != INVALID_HANDLE_VALUE)
	{
		::CloseHandle(this->m_hFile);
	}
}

bool File::Read(LPVOID lpBuffer, DWORD nNumberOfBytesToRead, LPDWORD lpNumberOfBytesRead)
{
	BOOL bRet = ::ReadFile(this->m_hFile, lpBuffer, nNumberOfBytesToRead, lpNumberOfBytesRead, NULL);
	if (bRet)
	{
		return true;
	}
	return false;
}

bool File::Write(LPCVOID lpBuffer, DWORD nNumberOfBytesToWrite, LPDWORD lpNumberOfBytesWritten)
{
	if (lpBuffer) {
		return ::WriteFile(this->m_hFile, lpBuffer, nNumberOfBytesToWrite, lpNumberOfBytesWritten, NULL);
	}
	return false;
}

bool File::Delete()
{
	::CloseHandle(this->m_hFile);
	BOOL iRet = ::DeleteFileW(this->m_wstrFileName.c_str());
	if (iRet)
	{
		this->m_hFile = INVALID_HANDLE_VALUE;
		return true;
	}
	return false;
}

bool File::Copy(const std::string& strNewFileName, bool bCover)
{
	BOOL bFailIfExists = bCover ? 0 : 1;
	BOOL iRet = CopyFileW(this->m_wstrFileName.c_str(),
		StringUtil::String2WString(strNewFileName).c_str(), bFailIfExists);
	if (iRet)
	{
		return true;
	}
	return false;
}

long long File::GetSize()
{
	LARGE_INTEGER liFileSize;
	BOOL bRet = ::GetFileSizeEx(this->m_hFile, &liFileSize);
	if (bRet) {
		return liFileSize.QuadPart;
	}
	throw "get file size failed";
}

DWORD File::SetPointer(LONG lDistanceToMove, PLONG lpDistanceToMoveHigh, DWORD dwMoveMethod)
{
	return ::SetFilePointer(m_hFile, lDistanceToMove, lpDistanceToMoveHigh, dwMoveMethod);
}

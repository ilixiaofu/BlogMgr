#ifndef _FILE_H_
#define _FILE_H_

#include <windows.h>
#include <string>

class File
{
private:
	using String = std::string;
	using WString = std::wstring;
public:
	File(const String& strFileName, bool bCreate = false);
	virtual ~File();
	bool Read(LPVOID lpBuffer, DWORD nNumberOfBytesToRead, LPDWORD lpNumberOfBytesRead);
	bool Write(LPCVOID lpBuffer, DWORD nNumberOfBytesToWrite, LPDWORD lpNumberOfBytesWritten);
	bool Delete(); // É¾³ýÎÄ¼þ
	bool Copy(const std::string& strNewFileName, bool bCover = false);
	long long GetSize();
	DWORD SetPointer(LONG lDistanceToMove, PLONG lpDistanceToMoveHigh, DWORD dwMoveMethod);
	HANDLE GetHandle() {
		return m_hFile;
	}

private:
	HANDLE m_hFile;
	String m_strFileName;
	WString m_wstrFileName;
};

#endif // !_FILE_H_
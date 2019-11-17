#include "../stdafx.h"
#include "Directory.h"
#include "Logger.h"

using String = std::string;

bool Directory::Createdirectory(const String& strPathName)
{
	BOOL bRet = ::CreateDirectoryA(strPathName.c_str(), NULL);
	if (bRet)
	{
		return true;
	}
	return false;
}

String Directory::GetCurrentdirectory()
{
	char lpBuffer[260] = {0x00};
	DWORD dwSize = ::GetCurrentDirectoryA(260, lpBuffer);
	String strPathName(lpBuffer);
	return strPathName;
}

bool Directory::SetCurrentdirectory(const String& strPathName)
{
	BOOL bRet = ::SetCurrentDirectoryA(strPathName.c_str());
	if (bRet)
	{
		return true;
	}
	return false;
}

void Directory::ScanningDirectory(const String& strPath, const String& strFilter, const String& strIgnore, std::vector<std::string>& result)
{
	Logger::Info("[ %s ]: %s", __FUNCTION__, strPath.c_str());

	WIN32_FIND_DATAA FindFileData;
	HANDLE hFind;
	String strFilePath = strPath;
	strFilePath.append("\\*");
	
	hFind = FindFirstFileA(strFilePath.c_str(), &FindFileData);
	if (hFind == INVALID_HANDLE_VALUE)
	{
		Logger::Info("[ %s ]: %s is not a file or directory", __FUNCTION__, strFilePath.c_str());
		return;
	}

	do
	{
		String strFullPath = FindFileData.cFileName;
		if (strFullPath == "." || strFullPath == "..")
		{
			continue;
		}
		strFullPath.assign(strPath);
		strFullPath.append("\\");
		strFullPath.append(FindFileData.cFileName);

		if (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			if (strFullPath.find(".") == String::npos)
			{
				ScanningDirectory(strFullPath, strFilter, strIgnore, result);
			}
		}
		else
		{
			if (strFullPath.find(strFilter) != std::string::npos && strFullPath.find(strIgnore) == std::string::npos)
			{
				result.push_back(strFullPath);
			}
		}
	} while (FindNextFileA(hFind, &FindFileData));
}
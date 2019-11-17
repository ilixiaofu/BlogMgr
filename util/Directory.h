#ifndef _DIRECTORY_H_
#define _DIRECTORY_H_

#include <windows.h>
#include <vector>


class Directory
{
private:
	using String = std::string;
public:
	static bool Createdirectory(const String& strPathName);
	static String GetCurrentdirectory();
	static bool SetCurrentdirectory(const String& strPathName);
	static void ScanningDirectory(const String& strPath, const String& strFilter, const String& strIgnore, std::vector<std::string>& result);

private:
	Directory() = delete;
	~Directory() = delete;

};

#endif //!_DIRECTORY_H_

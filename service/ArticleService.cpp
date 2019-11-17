#include "../stdafx.h"
#include "ArticleService.h"
#include "../util/Logger.h"
#include "../util/Date.h"
#include "../util/Directory.h"
#include "../util/File.h"


std::string ArticleService::CreateMultiDirectory(const String& strPathName)
{
	Logger::Info("[ %s ]: create directory %s", __FUNCTION__, strPathName.c_str());

	std::string strPath;

	Date date;
	String strFormat;
	strFormat.assign(std::to_string(date.GetYear()));
	Directory::Createdirectory(strFormat);
	Directory::SetCurrentdirectory(strFormat);
	strPath.append(strFormat);
	strPath.append("\\");

	char format[5] = { 0x00 };

	sprintf_s(format, 4, "%02d", date.GetMonth());
	strFormat.assign(format);
	Directory::Createdirectory(strFormat);
	Directory::SetCurrentdirectory(strFormat);
	strPath.append(strFormat);
	strPath.append("\\");

	sprintf_s(format, "%02d", date.GetDay());
	strFormat.assign(format);
	Directory::Createdirectory(strFormat);
	Directory::SetCurrentdirectory(strFormat);
	strPath.append(strFormat);
	strPath.append("\\");

	Directory::Createdirectory(strPathName);
	strPath.append(strPathName);
	Directory::SetCurrentdirectory(".\\..\\..\\..\\");
	Logger::Info("[ %s ]: create directory %s success", __FUNCTION__, strPathName.c_str());
	return strPath;
}

bool ArticleService::GenerateArticleTemplates(std::string strTitle)
{
	Logger::Info("[%s]: start generate article %s", __FUNCTION__, strTitle.c_str());

	String strPath = Directory::GetCurrentdirectory();
	strPath.append("\\");
	strPath.append(CreateMultiDirectory(strTitle));
	strPath.append("\\");
	strPath.append("README.md");
	try {
		Logger::Info("[ %s ]: %s", __FUNCTION__, "create file");
		File file(strPath, true);
		file.SetPointer(0, NULL, FILE_BEGIN);
		DWORD dwWrittenSize = 0;
		DWORD dwTotalSize = dwWrittenSize;

		String content;
		content.assign("<!--\r\n");
		content.append("{\r\n");
		content.append("\t\"author\": \"lxf\",");
		content.append("\r\n");
		content = StringUtil::GBK2UTF8(content.c_str());
		file.Write(content.c_str(), content.length(), &dwWrittenSize);
		dwTotalSize += dwWrittenSize;

		content.assign("\t\"title\": \"");
		content.append(strTitle);
		content.append("\",");
		content.append("\r\n");
		content = StringUtil::GBK2UTF8(content.c_str());
		file.Write(content.c_str(), content.length(), &dwWrittenSize);
		dwTotalSize += dwWrittenSize;

		char szDate[25] = { 0x00 };
		Date date;
		sprintf_s(szDate, 24, "%04d-%02d-%02d %02d:%02d:%02d",
			date.GetYear(), date.GetMonth(), date.GetDay(), date.GetHour(), date.GetMinute(), date.GetSecond());
		content.assign("\t\"date\": \"");
		content.append(szDate);
		content.append("\",");
		content.append("\r\n");
		content = StringUtil::GBK2UTF8(content.c_str());
		file.Write(content.c_str(), content.length(), &dwWrittenSize);
		dwTotalSize += dwWrittenSize;

		content.assign("\t\"category\": \"default\"\r\n");
		content.append("}\r\n");
		content.append("-->\r\n\r\n");
		content = StringUtil::GBK2UTF8(content.c_str());
		file.Write(content.c_str(), content.length(), &dwWrittenSize);
		dwTotalSize += dwWrittenSize;

		content.assign(strTitle);
		content = StringUtil::GBK2UTF8(content.c_str());
		file.Write(content.c_str(), content.length(), &dwWrittenSize);
		dwTotalSize += dwWrittenSize;

		content.assign(STRING_LINE);
		content.append("\r\n");
		content.append(szDate);
		content.append(" &emsp;   \r\n\r\n");
		content = StringUtil::GBK2UTF8(content.c_str());
		file.Write(content.c_str(), content.length(), &dwWrittenSize);
		dwTotalSize += dwWrittenSize;

		content.assign(STRING_LINE);
		content = StringUtil::GBK2UTF8(content.c_str());
		file.Write(content.c_str(), content.length(), &dwWrittenSize);
		dwTotalSize += dwWrittenSize;

		content.assign(BACK_LAST_HOME);
		content.append(BACK_HOME);
		content = StringUtil::GBK2UTF8(content.c_str());
		file.Write(content.c_str(), content.length(), &dwWrittenSize);
		dwTotalSize += dwWrittenSize;
		Logger::Info("[ %s ]: %s:%d", __FUNCTION__, "write content over written total size", dwTotalSize);
		return true;
	}
	catch (const char *ex) {
		Logger::Info("[ %s ]: %s", __FUNCTION__, ex);
	}
	return false;
}
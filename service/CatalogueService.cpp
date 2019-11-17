#include "../stdafx.h"
#include "CatalogueService.h"
#include "../util/Directory.h"
#include "../util/Logger.h"
#include "../util/File.h"
#include "../util/Language.h"

void CatalogueService::GenerateCatalogueFile()
{
	std::string path = Directory::GetCurrentdirectory();
	std::vector<std::string> vecFilePaths;
	Directory::ScanningDirectory(path, "README.md", "categories", vecFilePaths);
	for (auto iter = vecFilePaths.begin(); iter!= vecFilePaths.end(); ++iter)
	{
		ParserFile(*iter);
	}
	vecFilePaths.clear();
	CreateCategoryFile();
}

void CatalogueService::ParserFile(std::string& strFilePath)
{
	Logger::Info("[ %s ]: %s", __FUNCTION__, strFilePath.c_str());
	File file(strFilePath);
	DWORD dwSize = file.GetSize();
	char *pszBuffer = new char[dwSize + 1]{ 0 };
	DWORD dwRead = 0;
	file.Read(pszBuffer, dwSize, &dwRead);
	std::string strFileContent = StringUtil::UTF82GBK(pszBuffer);
	delete[] pszBuffer;
	pszBuffer = const_cast<char*>(strFileContent.c_str());
	std::string strJson;
	char *pBuffer = NULL;
	for (size_t i = 0; i < dwRead; i++)
	{
		if (pszBuffer[i] == '<')
		{
			std::string strStart;
			strStart.append(pszBuffer + i, 7);
			if (strStart == "<!--\r\n{")
			{
				pBuffer = pszBuffer + i + 6;
				i += 7;
			}
		}
		if (pBuffer)
		{
			if (pszBuffer[i] == '}')
			{
				std::string strEnd;
				strEnd.append(pszBuffer + i, 6);
				if (strEnd == "}\r\n-->")
				{
					strJson.append(pBuffer, pszBuffer + i + 1 - pBuffer);
					Logger::Info("[ %s ]: %s", __FUNCTION__, strJson.c_str());
					if (!strJson.empty())
					{
						Json::Value root;
						JSONCPP_STRING err;
						//Json::Reader reader;
						//reader.parse(strJson, root);
						Json::CharReaderBuilder builder;
						const std::unique_ptr<Json::CharReader> reader(builder.newCharReader());
						bool bRet = reader->parse(strJson.c_str(),
							strJson.c_str() + strJson.length(),
							&root,
							&err);
						if (!bRet)
						{
							Logger::Info("[ %s ]: parer %s error", __FUNCTION__, strJson.c_str());
							break;
						}
						std::string date = root["date"].asString();
						std::string title = root["title"].asString();
						std::string category = root["category"].asString();
						std::string url = title;
						url.insert(0, "[**");
						url.append("**]");

						const std::string strCurDirectory = Directory::GetCurrentdirectory();
						std::string physics_path = strFilePath;
						physics_path.replace(physics_path.find(strCurDirectory), strCurDirectory.length() + 1, "");
						std::string link_path = physics_path;
						int pos = std::string::npos;
						do
						{
							pos = link_path.rfind("\\");
							if (pos != std::string::npos)
							{
								link_path = link_path.replace(pos, 1, "/");
							}
						} while (pos != std::string::npos);
						url.append("(./../../");
						url.append(link_path);
						url.append(")");

						std::string tableName;
						Language::ChineseConvertPinyin(category, tableName);
						tableName.insert(0, "tb_category_");
						tableName = StringUtil::GBK2UTF8(tableName.c_str());
						category = StringUtil::GBK2UTF8(category.c_str());
						bRet = mCategoryTableDao->AddCategoryTable(category, tableName);
						if (!bRet)
						{
							mCategoryTableDao->UpdateCategoryTable(category, tableName);
						}

						title = StringUtil::GBK2UTF8(title.c_str());
						date = StringUtil::GBK2UTF8(date.c_str());
						link_path = StringUtil::GBK2UTF8(link_path.c_str());
						url = StringUtil::GBK2UTF8(url.c_str());

						TiXmlElement* item = new TiXmlElement("menu_item");
						item->SetAttribute("title", title.c_str());
						item->SetAttribute("date", date.c_str());
						item->SetAttribute("category", category.c_str());
						item->SetAttribute("link_path", link_path.c_str());
						item->SetAttribute("url", url.c_str());
						bRet = mCategoryItemDao->AddCategoryItem(tableName, item);
						if (!bRet)
						{
							mCategoryItemDao->UpdateCategoryItem(tableName, item);
						}
					}
					break;
				}
			}
		}
	}
}

void CatalogueService::CreateCategoryFile()
{
	std::map<std::string, std::string> CategoryTableMap;
	mCategoryTableDao->GetAllCategoryTable(CategoryTableMap);
	if (CategoryTableMap.size() == 0)
	{
		return;
	}
	std::string footer("\r\n[返回上级目录](./../../README.md)");
	footer = StringUtil::GBK2UTF8(footer.c_str());

	Directory::Createdirectory("categories");
	for (auto iter = CategoryTableMap.begin(); iter != CategoryTableMap.end(); ++iter)
	{
		Directory::SetCurrentdirectory("categories");
		std::string strCategory = iter->first;
		Directory::Createdirectory(strCategory);
		Directory::SetCurrentdirectory(strCategory);
		File *pFile = NULL;
		try 
		{
			pFile = new File("README.md", false);
			if (pFile->GetSize() > 0)
			{
				pFile->Delete();
				delete pFile;
				pFile = NULL;
				pFile = new File("README.md", false);
			}
			if (pFile)
			{
				Directory::SetCurrentdirectory(".\\..\\..");
				std::string strTable = iter->second;
				std::string strDocument = mCategoryItemDao->GetCategoryItemByTbale(strTable);
				TiXmlDocument document;
				document.Parse(strDocument.c_str());
				TiXmlElement *category_menus = document.RootElement();
				TiXmlElement *menu_item = category_menus->FirstChildElement("menu_item");
				int seq = 0;
				while (menu_item)
				{
					TiXmlPrinter printer;
					menu_item->Accept(&printer);
					printf_s("[ %s ]: \n %s\n", __FUNCTION__, StringUtil::UTF82GBK(printer.CStr()).c_str());
					std::string strUrl = menu_item->Attribute("url");
					++seq;
					char buffer[256] = { 0x00 };
					sprintf_s(buffer, 256, "%d. %s  \r\n", seq, StringUtil::UTF82GBK(strUrl.c_str()).c_str());
					std::string write = StringUtil::GBK2UTF8(buffer);
					pFile->Write(write.c_str(), write.length(), NULL);
					menu_item = menu_item->NextSiblingElement();
				}
				pFile->Write(footer.c_str(), footer.length(), NULL);
			}
			delete pFile;
			pFile = NULL;
		}
		catch (std::string &ex)
		{
			Logger::Info("[ %s ]: %s", __FUNCTION__, ex.c_str());
		}
	}
}

void CatalogueService::GenerateHomePageFile()
{
	std::map<std::string, std::string> CategoryTableMap;
	mCategoryTableDao->GetAllCategoryTable(CategoryTableMap);
	if (CategoryTableMap.size() == 0)
	{
		return;
	}
	std::string header("### 分类  \r\n\r\n");
	header = StringUtil::GBK2UTF8(header.c_str());

	File *pFile = NULL;
	try
	{
		pFile = new File("README.md", false);
		if (pFile->GetSize() > 0)
		{
			pFile->Delete();
			delete pFile;
			pFile = NULL;
			pFile = new File("README.md", true);
		}
		if (pFile)
		{
			pFile->Write(header.c_str(), header.length(), NULL);
			for (auto iter = CategoryTableMap.begin(); iter != CategoryTableMap.end(); ++iter)
			{
				int count = mCategoryItemDao->GetCategoryItemCountByTbale(iter->second);
				std::string strCategory = iter->first;
				char buffer[512] = { 0x00 };
				sprintf_s(buffer, 512, "   - [**%s ( %d )**](categories/%s/README.md)  \r\n",
					strCategory.c_str(), count, strCategory.c_str());
				std::string item = buffer;
				std::string write = StringUtil::GBK2UTF8(buffer);
				pFile->Write(write.c_str(), write.length(), NULL);
			}
		}
	}
	catch (std::string &ex)
	{
		Logger::Info("[ %s ]: %s", __FUNCTION__, ex.c_str());
	}
	delete pFile;
}
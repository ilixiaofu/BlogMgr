#pragma once

#include "../util/SqlFactory.h"


class CategoryItemDao
{
public:
	CategoryItemDao(std::string strConfigFile);

	~CategoryItemDao()
	{
		delete pSqlFactory;
	}

public:
	bool AddCategoryItem(std::string& strTableName, const TiXmlElement* XmlData);
	bool UpdateCategoryItem(std::string& strTableName, const TiXmlElement* XmlData);
	std::string GetCategoryItemByTbale(std::string& strTableName);
	int GetCategoryItemCountByTbale(std::string& strTableName);
private:
	bool OpenDataBase();
	bool CreateTable(std::string& strTableName);
private:
	SQLiteWrapper msqlite;
	SqlFactory *pSqlFactory;
	std::string m_databasePath;
	std::string m_schema;
};

#pragma once

#include "../util/SqlFactory.h"

class CategoryTableDao
{
public:
	CategoryTableDao(std::string strConfigFile);

	~CategoryTableDao()
	{
		delete pSqlFactory;
	}

public:
	bool AddCategoryTable(std::string& strCategoryName, std::string& strTableName);
	bool UpdateCategoryTable(std::string& strCategoryName, std::string& strTableName);
	void GetAllCategoryTable(std::map<std::string, std::string>& CategoryTableMap);
private:
	bool OpenDataBase();
	bool CreateTable();
private:
	SQLiteWrapper msqlite;
	SqlFactory *pSqlFactory;
	std::string m_databasePath;
	std::string m_schema;
	std::string m_table;
};

#include "../stdafx.h"
#include "CategoryTableDao.h"
#include "../util/Logger.h"

CategoryTableDao::CategoryTableDao(std::string strConfigFile)
{
	pSqlFactory = NULL;
	pSqlFactory = new SqlFactory(strConfigFile);
	m_schema = "data.db";
	m_table = "tb_category_table";
	m_databasePath = strConfigFile.substr(0, strConfigFile.find_last_of("/") + 1) + m_schema;
}

bool CategoryTableDao::OpenDataBase()
{
	return msqlite.Open(m_databasePath);
}

bool CategoryTableDao::CreateTable()
{
	std::string sqlOperatorType = "create";
	std::string sql = pSqlFactory->GetSql(m_schema, m_table, sqlOperatorType);
	if (sql.empty())
	{
		return false;
	}
	return msqlite.DirectStatement(sql);
}

bool CategoryTableDao::AddCategoryTable(std::string& strCategoryName, std::string& strTableName)
{
	std::string sqlOperatorType = "insert";
	std::string sql = pSqlFactory->GetSql(m_schema, m_table, sqlOperatorType);
	if (!sql.empty())
	{
		if (OpenDataBase())
		{
			Logger::Info("[ %s ]: open %s success", __FUNCTION__, m_databasePath.c_str());
			SQLiteStatement* stmtInsert = msqlite.Statement(sql);
			if (!stmtInsert)
			{
				Logger::Info("[ %s ]: executing [%s] statement occur error:%d ", __FUNCTION__, sql.c_str(), msqlite.LastError());
				bool bRet = CreateTable();
				if (!bRet)
				{
					return false;
				}
				stmtInsert = msqlite.Statement(sql);
			}
			stmtInsert->Bind(0, strCategoryName);
			stmtInsert->Bind(1, strTableName);
			if (stmtInsert->Execute()) {
				Logger::Info("[ %s ]: statement [%s] executed", __FUNCTION__, sql.c_str());
				return true;
			}
			else
			{
				Logger::Info("[ %s ]: executing [%s] statement occur error:%d ", __FUNCTION__, sql.c_str(), msqlite.LastError());
				return false;
			}
		}
		else
		{
			Logger::Info("[ %s ]: couldn't open %s !", __FUNCTION__, m_databasePath.c_str());
			return false;
		}
	}
	return false;
}

bool CategoryTableDao::UpdateCategoryTable(std::string& strCategoryName, std::string& strTableName)
{
	std::string sqlOperatorType = "update";
	std::string sql = pSqlFactory->GetSql(m_schema, m_table, sqlOperatorType);
	if (!sql.empty())
	{
		if (OpenDataBase())
		{
			Logger::Info("[ %s ]: open %s success", __FUNCTION__, m_databasePath.c_str());
			SQLiteStatement* stmtUpdate = msqlite.Statement(sql);
			if (stmtUpdate)
			{
				stmtUpdate->Bind(0, strTableName);
				stmtUpdate->Bind(1, strCategoryName);
				if (stmtUpdate->Execute()) {
					Logger::Info("[ %s ]: statement [%s] executed", __FUNCTION__, sql.c_str());
					return true;
				}
				else
				{
					Logger::Info("[ %s ]: executing [%s] statement occur error:%d ", __FUNCTION__, sql.c_str(), msqlite.LastError());
					return false;
				}
			}
			else
			{
				Logger::Info("[ %s ]: executing [%s] statement occur error:%d ", __FUNCTION__, sql.c_str(), msqlite.LastError());
				return false;
			}
		}
		else
		{
			Logger::Info("[ %s ]: couldn't open %s !", __FUNCTION__, m_databasePath.c_str());
			return false;
		}
	}
	return false;
}

void CategoryTableDao::GetAllCategoryTable(std::map<std::string, std::string>& CategoryTableMap)
{
	std::string sqlOperatorType = "select";
	std::string sql = pSqlFactory->GetSql(m_schema, m_table, sqlOperatorType);
	if (!sql.empty())
	{
		if (OpenDataBase())
		{
			Logger::Info("[ %s ]: open %s success", __FUNCTION__, m_databasePath.c_str());
			SQLiteStatement* stmtQuery = msqlite.Statement(sql);
			if (stmtQuery)
			{
				while (stmtQuery->NextRow())
				{
					std::string category = stmtQuery->ValueString(0);
					std::string tableName = stmtQuery->ValueString(1);
					Logger::Info("[ %s ]: %s , %s", __FUNCTION__,
						StringUtil::UTF82GBK(category.c_str()).c_str(),
						StringUtil::UTF82GBK(tableName.c_str()).c_str());
					category = StringUtil::UTF82GBK(category.c_str());
					tableName = StringUtil::UTF82GBK(tableName.c_str());
					CategoryTableMap.insert(std::pair<std::string, std::string>(category, tableName));
					//CategoryTableMap.insert(std::map<std::string, std::string>::value_type(category, tableName));
					
				}
			}
			else
			{
				Logger::Info("[ %s ]: executing [%s] statement occur error:%d ", __FUNCTION__, sql.c_str(), msqlite.LastError());
			}
		}
		else
		{
			Logger::Info("[ %s ]: couldn't open %s !", __FUNCTION__, m_databasePath.c_str());
		}
	}
}
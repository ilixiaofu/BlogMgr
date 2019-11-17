#include "../stdafx.h"
#include "CategoryItemDao.h"

#include "../util/StringUtil.h"
#include "../util/Logger.h"
#include "../util/Language.h"

CategoryItemDao::CategoryItemDao(std::string strConfigFile)
{
	pSqlFactory = NULL;
	pSqlFactory = new SqlFactory(strConfigFile);
	m_schema = "data.db";
	m_databasePath = strConfigFile.substr(0, strConfigFile.find_last_of("/") + 1) + m_schema;
}

bool CategoryItemDao::OpenDataBase()
{
	return msqlite.Open(m_databasePath);
}

bool CategoryItemDao::CreateTable(std::string& strTableName)
{
	std::string sqlOperatorType = "create";
	std::string table = "tb_category_";
	std::string sql = pSqlFactory->GetSql(m_schema, table, sqlOperatorType);
	std::string::size_type pos =  sql.find(table);
	if (pos != std::string::npos)
	{
		sql.insert(pos + table.length(), strTableName);
		sql.replace(sql.find_first_of(table), table.length(), "");
	}
	if (sql.empty())
	{
		return false;
	}
	return msqlite.DirectStatement(sql);
}

bool CategoryItemDao::AddCategoryItem(std::string& strTableName, const TiXmlElement* XmlData)
{
	std::string sqlOperatorType = "insert";
	std::string table = "tb_category_";
	std::string sql = pSqlFactory->GetSql(m_schema, table, sqlOperatorType);
	std::string::size_type pos = sql.find(table);
	if (pos != std::string::npos)
	{
		sql.insert(pos + table.length(), strTableName);
		sql.replace(sql.find_first_of(table), table.length(), "");
	}
	if (!sql.empty())
	{
		if (OpenDataBase())
		{
			Logger::Info("[ %s ]: open %s success", __FUNCTION__, m_databasePath.c_str());
			SQLiteStatement* stmtInsert = msqlite.Statement(sql);
			if (!stmtInsert)
			{
				Logger::Info("[ %s ]: executing [%s] statement occur error:%d ", __FUNCTION__, sql.c_str(), msqlite.LastError());
				bool bRet = CreateTable(strTableName);
				if (!bRet)
				{
					return false;
				}
				stmtInsert = msqlite.Statement(sql);
			}
			stmtInsert->Bind(0, XmlData->Attribute("title"));
			stmtInsert->Bind(1, XmlData->Attribute("date"));
			stmtInsert->Bind(2, XmlData->Attribute("category"));
			stmtInsert->Bind(3, XmlData->Attribute("link_path"));
			stmtInsert->Bind(4, XmlData->Attribute("url"));
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

bool CategoryItemDao::UpdateCategoryItem(std::string& strTableName, const TiXmlElement* XmlData)
{
	std::string sqlOperatorType = "update";
	std::string table = "tb_category_";
	std::string sql = pSqlFactory->GetSql(m_schema, table, sqlOperatorType);
	std::string::size_type pos = sql.find(table);
	if (pos != std::string::npos)
	{
		sql.insert(pos + table.length(), strTableName);
		sql.replace(sql.find_first_of(table), table.length(), "");
	}
	if (!sql.empty())
	{
		if (OpenDataBase())
		{
			Logger::Info("[ %s ]: open %s success", __FUNCTION__, m_databasePath.c_str());
			SQLiteStatement* stmtUpdate = msqlite.Statement(sql);
			if (stmtUpdate)
			{
				stmtUpdate->Bind(0, XmlData->Attribute("date"));
				stmtUpdate->Bind(1, XmlData->Attribute("category"));
				stmtUpdate->Bind(2, XmlData->Attribute("link_path"));
				stmtUpdate->Bind(3, XmlData->Attribute("url"));
				stmtUpdate->Bind(4, XmlData->Attribute("title"));
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

std::string CategoryItemDao::GetCategoryItemByTbale(std::string& strTableName)
{
	std::string strResulet;
	TiXmlDocument *document = new TiXmlDocument();
	TiXmlDeclaration* declaration = new TiXmlDeclaration("1.0", "UTF-8", "yes");
	document->LinkEndChild(declaration);
	TiXmlElement *rootElement = new TiXmlElement("category_menus");
	document->LinkEndChild(rootElement);

	std::string sqlOperatorType = "select";
	std::string table = "tb_category_";
	std::string sql = pSqlFactory->GetSql(m_schema, table, sqlOperatorType);
	std::string::size_type pos = sql.find(table);
	if (pos != std::string::npos)
	{
		sql.insert(pos + table.length(), strTableName);
		sql.replace(sql.find_first_of(table), table.length(), "");
	}
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
					TiXmlElement* menu_item = new TiXmlElement("menu_item");
					menu_item->SetAttribute("title", stmtQuery->ValueString(0).c_str());
					menu_item->SetAttribute("date", stmtQuery->ValueString(1).c_str());
					menu_item->SetAttribute("category", stmtQuery->ValueString(2).c_str());
					menu_item->SetAttribute("link_path", stmtQuery->ValueString(3).c_str());
					menu_item->SetAttribute("url", stmtQuery->ValueString(4).c_str());
					rootElement->LinkEndChild(menu_item);

					TiXmlPrinter printer;
					menu_item->Accept(&printer);
					Logger::Info("[ %s ]: \n %s", __FUNCTION__, StringUtil::UTF82GBK(printer.CStr()).c_str());
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
	TiXmlPrinter printer;
	document->Accept(&printer);
	strResulet = printer.CStr();
	return strResulet;
}

int CategoryItemDao::GetCategoryItemCountByTbale(std::string& strTableName)
{
	int count = -1;
	std::string sqlOperatorType = "select_count";
	std::string table = "tb_category_";
	std::string sql = pSqlFactory->GetSql(m_schema, table, sqlOperatorType);
	std::string::size_type pos = sql.find(table);
	if (pos != std::string::npos)
	{
		sql.replace(pos, table.length(), "");
		sql.replace(pos, strTableName.length(), strTableName);
		sql.append("];");
	}
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
					count = stmtQuery->ValueInt(0);
					Logger::Info("[ %s ]: table:%s item %d", __FUNCTION__, strTableName.c_str(), count);
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
	return count;
}

#include "../stdafx.h"
#include "SqlFactory.h"

SqlFactory::SqlFactory(std::string strConfigFile)
{
	if (strConfigFile.empty())
	{
		throw "ConfigFile is empty";
	}
	TiXmlDocument doc(strConfigFile.c_str());
	bool loadOkay = doc.LoadFile();
	if (!loadOkay)
	{
		throw "couldn't open file " + strConfigFile;
	}
	m_ConfigFile = strConfigFile;
}

std::string SqlFactory::GetSql(std::string& schema, std::string& tableName, std::string& sqlOperator)
{
	std::string sql;
	TiXmlDocument doc(m_ConfigFile.c_str());
	bool loadOkay = doc.LoadFile();
	if (loadOkay)
	{
		TiXmlElement *config = doc.RootElement();
		TiXmlElement *database = config->FirstChildElement("database");
		while (database)
		{
			std::string mschema = database->Attribute("schema");
			if (mschema == schema)
			{
				TiXmlElement *table = database->FirstChildElement("table");
				while (database)
				{
					std::string strTableName = table->Attribute("name");
					if (strTableName == tableName)
					{
						TiXmlElement *sql_operator = table->FirstChildElement();
						while (sql_operator)
						{
							std::string strSqlOperator = sql_operator->Attribute("operator");
							if (strSqlOperator == sqlOperator)
							{
								sql = sql_operator->GetText();
								break;
							}
							sql_operator = sql_operator->NextSiblingElement();
						}
						break;
					}
					table = table->NextSiblingElement();
				}
				break;
			}
			database = database->NextSiblingElement();
		}
	}

	return sql;
}
#pragma once

class SqlFactory
{
public:
	SqlFactory(std::string strConfigFile);

	~SqlFactory()
	{
	}

	std::string GetSql(std::string& schema, std::string& tableName, std::string& sqlOperator);

private:
	std::string m_ConfigFile;
};

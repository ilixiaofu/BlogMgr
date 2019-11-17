#pragma once

#include "../dao/CategoryItemDao.h"
#include "../dao/CategoryTableDao.h"

class CatalogueService
{
public:
	CatalogueService()
	{
		mCategoryTableDao = new CategoryTableDao("config/config.xml");
		mCategoryItemDao = new CategoryItemDao("config/config.xml");
	}

	~CatalogueService()
	{
		delete mCategoryTableDao;
		delete mCategoryItemDao;
	}

public:
	void GenerateCatalogueFile();
	void GenerateHomePageFile();

private:
	void ParserFile(std::string& strFilePath);
	void CreateCategoryFile();

private:
	CategoryTableDao *mCategoryTableDao;
	CategoryItemDao *mCategoryItemDao;
};


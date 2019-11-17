#pragma once

class ArticleService
{
public:
	ArticleService()
	{
	}

	~ArticleService()
	{
	}

public:
	bool GenerateArticleTemplates(std::string strTitle);

private:
	std::string CreateMultiDirectory(const String& strPathName);
};


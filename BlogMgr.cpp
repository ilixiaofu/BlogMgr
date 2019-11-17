#include "stdafx.h"
#include "service/ArticleService.h"
#include "service/CatalogueService.h"
#include "util/Logger.h"

void command_help()
{
	Logger::Info("[ %s ]: %s", __FUNCTION__, "init <ÎÄ¼þÃû>");
	Logger::Info("[ %s ]: %s", __FUNCTION__, "b: build");
	Logger::Info("[ %s ]: %s", __FUNCTION__, "g: generate");
}

int main(int argc, char* argv[])
{
	setlocale(LC_ALL, "");
	if (argc == 1)
	{
		command_help();
	}
	else
	{
		for (int i = 1; i < argc; i++)
		{
			if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0)
			{
				command_help();
				return 0;
			}

			if (strcmp(argv[i], "init") == 0)
			{
				if (i + 1 != argc)
				{
					ArticleService articleService;
					articleService.GenerateArticleTemplates(argv[i + 1]);
				}
				else
				{
					Logger::Info("[ %s ]: %s error", __FUNCTION__, "command: ", argv[1]);
				}
				return 0;
			}

			if (strcmp(argv[i], "b") == 0 || strcmp(argv[i], "B") == 0)
			{
				if (i + 1 == argc)
				{
					CatalogueService catalogueService;
					catalogueService.GenerateCatalogueFile();
					catalogueService.GenerateHomePageFile();
				}
				else
				{
					Logger::Info("[ %s ]: %s error", __FUNCTION__, "command: ", argv[1]);
				}
				return 0;
			}
		}
		Logger::Info("[ %s ]: %s %s", __FUNCTION__, "can not find command: ", argv[1]);
		command_help();
	}
	return 0;
}


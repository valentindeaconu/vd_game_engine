#include "ConfigurationFile.hpp"

namespace vd
{
	namespace config
	{
		ConfigurationFile::ConfigurationFile(const std::string& filePath)
			: filePath(filePath)
		{
		}

		ConfigurationFile::~ConfigurationFile()
		{
		}

		void ConfigurationFile::parse()
		{
			std::ifstream fin(filePath);

			if (!fin.is_open())
			{
				Logger::terminate("could not load terrain config from " + filePath, 1);
			}

			std::string line;
			while (std::getline(fin, line))
			{
				std::vector<std::string> tokens;
				std::stringstream ss(line);
				std::string token;

				bool isFirst = true;
				std::string command;
				while (std::getline(ss, token, ' '))
				{
					if (token.compare("") == 0)
					{
						continue;
					}

					if (isFirst)
					{
						command = token;
						isFirst = false;
					}
					else
					{
						tokens.push_back(token);
					}
				}

				// empty line
				if (isFirst)
				{
					continue;
				}

				// comment line
				if (command.compare(0, 1, "#") == 0)
				{
					continue;
				}
				
				onTokenReceived(command, tokens);
			}

			fin.close();

			onParseFinish();
		}

		const std::string& ConfigurationFile::getFilePath() const
		{
			return filePath;
		}
	}
}
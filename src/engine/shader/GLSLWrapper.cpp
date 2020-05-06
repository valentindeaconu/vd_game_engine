#include "GLSLWrapper.hpp"

namespace vd
{
	namespace shader
	{
		GLSLWrapper::GLSLWrapper()
		{
		}

		GLSLWrapper::~GLSLWrapper()
		{
		}

		std::string GLSLWrapper::parseGLSL(const std::string& fileName)
		{
			std::string content;
			parseGLSL(fileName, content);
			return content;
		}

		void GLSLWrapper::parseGLSL(const std::string& fileName, std::string& content)
		{
			readFile(fileName, content);
		}

		void GLSLWrapper::readFile(const std::string& fileName, std::string& buffer)
		{
			std::ifstream fin(fileName);

			std::string filePath;

			const size_t lastSlashIdx = fileName.find_last_of("\\/");
			if (lastSlashIdx != std::string::npos)
			{
				filePath = fileName.substr(0, lastSlashIdx + 1);
			}

			buffer.clear();

			if (fin.is_open())
			{
				std::stringstream bufferBuilder;

				const std::string includeStmt = "#include";
				const int szIncludeStmt = includeStmt.size();

				std::string line;
				while (std::getline(fin, line))
				{
					if (line.rfind(includeStmt, 0) == 0)
					{
						// line should be smth like <#include "filename">

						// filename length should be size(line) - size(includeStmt) - 3 (1 space and 2 '"')
						int szFilename = line.size() - szIncludeStmt - 3;
						std::string includeFileName = filePath + line.substr(szIncludeStmt + 2, szFilename);

						std::string includeFileContent;
						readFile(includeFileName, includeFileContent);

						bufferBuilder << includeFileContent << '\n';
					} 
					else
					{
						bufferBuilder << line << '\n';
					}
				}

				buffer = bufferBuilder.str();
			}
			else
			{
				logger::Logger::warn("Trying to read an invalid file... attaching an empty string");
			}

			fin.close();
		}
	}
}
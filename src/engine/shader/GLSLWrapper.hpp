#ifndef __GLSL_WRAPPER_HPP_
#define __GLSL_WRAPPER_HPP_

#include <string>
#include <fstream>
#include <sstream>

#include <engine/logger/Logger.hpp>

namespace vd
{
	namespace shader
	{
		class GLSLWrapper
		{
		public:
			GLSLWrapper();
			~GLSLWrapper();

			std::string parseGLSL(const std::string& fileName);
			void parseGLSL(const std::string& fileName, std::string& content);

		private:
			void readFile(const std::string& fileName, std::string& buffer);

		};
	}
}

#endif // !__GLSL_WRAPPER_HPP_

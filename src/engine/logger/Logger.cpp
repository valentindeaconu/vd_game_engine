#include "Logger.hpp"

namespace vd
{
	namespace logger
	{
		void Logger::log(const std::string& message)
		{
			std::cout << "[Message] " << message << "\n";
		}

		void Logger::warn(const std::string& message)
		{
			std::cout << "[Warning] " << message << "\n";
		}

		void Logger::terminate(const std::string& message, uint8_t code)
		{
			std::cout << "[Error] " << message << "\n";
			exit(code);
		}
	}
}
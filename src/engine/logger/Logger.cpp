#include "Logger.hpp"

namespace vd::logger
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
        std::cerr << "[Error] " << message << "\n";
        exit(code);
    }
}
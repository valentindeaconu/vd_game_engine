#ifndef __LOGGER_HPP_
#define __LOGGER_HPP_

#include <iostream>
#include <string>
#include <sstream>

namespace vd::logger
{
    class Logger
    {
    public:
        static void log(const std::string &message);

        static void warn(const std::string &message);

        static void terminate(const std::string &message, uint8_t code);
    };
}

namespace vd
{
	typedef logger::Logger Logger;
}

#endif // !__LOGGER_HPP_

//
// Created by Vali on 11/10/2020.
//

#ifndef VD_GAME_ENGINE_LOGGER_HPP
#define VD_GAME_ENGINE_LOGGER_HPP

#include <iostream>
#include <string>

namespace vd {
    class Logger {
    public:
        static void log(const std::string& message);
        static void warn(const std::string& message);
        static void terminate(const std::string& message, uint8_t code);
    };

}

#endif //VD_GAME_ENGINE_LOGGER_HPP

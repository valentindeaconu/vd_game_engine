//
// Created by Vali on 11/14/2020.
//

#ifndef VD_GAME_ENGINE_EXCEPTIONS_HPP
#define VD_GAME_ENGINE_EXCEPTIONS_HPP

#include <stdexcept>
#include <string>

namespace vd {
    class CustomException : public std::exception {
    public:
        explicit CustomException(const std::string& prefix, const std::string& message);

        [[nodiscard]] const char* what() const noexcept override { return m_Message.c_str(); }
    private:
        std::string m_Message;
    };

    struct NoSuchFileOrDirectory: public CustomException {
        explicit NoSuchFileOrDirectory(const std::string& path);
    };

    struct MissingDependency : public CustomException {
        explicit MissingDependency(const std::string& name);
    };

    struct SyntaxError : public CustomException {
        explicit SyntaxError(const std::string& message, uint64_t line);
    };
}


#endif //VD_GAME_ENGINE_EXCEPTIONS_HPP

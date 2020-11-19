//
// Created by Vali on 11/14/2020.
//

#include "Exceptions.hpp"

namespace vd {
    CustomException::CustomException(const std::string& prefix, const std::string& message) {
        m_Message = prefix + " " + message;
    }

    NoSuchFileOrDirectory::NoSuchFileOrDirectory(const std::string& path)
        : CustomException("NoSuchFileOrDirectory", path)
    {
    }

    MissingDependency::MissingDependency(const std::string& name)
        : CustomException("MissingDependency", name)
    {
    }

    RuntimeError::RuntimeError(const std::string& message)
        : CustomException("RuntimeError", message)
    {

    }

    SyntaxError::SyntaxError(const std::string& message, uint64_t line)
        : CustomException("SyntaxError", "[" + std::to_string(line) + "]" + message)
    {
    }

}
//
// Created by Vali on 3/17/2021.
//

#include "Context.hpp"

namespace vd::gl {

    Context::Context()
        : m_Renderer("Unknown")
        , m_Version("Unknown")
    {
    }

    void Context::OnCreate() {
        glewExperimental = GL_TRUE;

        if (GLEW_OK != glewInit()) {
            throw RuntimeError("Failed to initialize GLEW");
        }

        std::stringstream r;
        r << glGetString(GL_RENDERER);
        m_Renderer = r.str();

        std::stringstream v;
        v << glGetString(GL_VERSION);
        m_Version = v.str();

        Summary();    
    }

    void Context::OnCleanUp() {
        
    }

    GLenum Context::CheckError(const char* file, int line) {
        GLenum errorCode;
        while ((errorCode = glGetError()) != GL_NO_ERROR) {
            std::string error;
            switch (errorCode) {
                case GL_INVALID_ENUM:                  error = "Invalid Enum"; break;
                case GL_INVALID_VALUE:                 error = "Invalid Value"; break;
                case GL_INVALID_OPERATION:             error = "Invalid Operation"; break;
                case GL_STACK_OVERFLOW:                error = "Stack Overflow"; break;
                case GL_STACK_UNDERFLOW:               error = "Stack Underflow"; break;
                case GL_OUT_OF_MEMORY:                 error = "Out of memory"; break;
                case GL_INVALID_FRAMEBUFFER_OPERATION: error = "Invalid Framebuffer Operation"; break;
                default:                               continue;
            }

            std::stringstream merr;
            merr << error << " | " << file << " (" << line << ")";           
            Logger::warn(merr.str());
        }
        return errorCode;
    }

    void Context::Summary() {
        Logger::log("Renderer: " + m_Renderer);
        Logger::log("OpenGL Version: " + m_Version);
    }

}
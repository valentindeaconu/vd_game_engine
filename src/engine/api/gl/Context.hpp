//
// Created by Vali on 3/17/2021.
//

#ifndef VDGE_GL_CONTEXT_HPP
#define VDGE_GL_CONTEXT_HPP

#include <string>
#include <sstream>
#include <memory>

#define GLEW_STATIC
#include <GL/glew.h>

#include <engine/logger/Logger.hpp>

#include "Component.hpp"

namespace vd::gl {
    class Context : public Component {
    public:
        Context();

        void OnCreate() override;
        void OnCleanUp() override;

        static GLenum CheckError(const char* file, int line);
    private:
        void Summary();

        std::string m_Renderer;
        std::string m_Version;
    };
    typedef std::shared_ptr<Context>    ContextPtr;
}

#define glCheckError() vd::gl::Context::CheckError(__FILE__, __LINE__)

#endif //VDGE_GL_CONTEXT_HPP

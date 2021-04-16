//
// Created by vali on 4/16/21.
//

#include "Factory.hpp"

#include <engine/gapi/gl/BufferGLImpl.hpp>
#include <engine/gapi/gl/ProgramGLImpl.hpp>

using BufferImpl = vd::gl::BufferGLImpl;
using ProgramImpl = vd::gl::ProgramGLImpl;

namespace vd::gapi {
    BufferPtr vd::gapi::Factory::CreateBuffer() {
        return std::dynamic_pointer_cast<Buffer>(std::make_shared<BufferImpl>());
    }

    ProgramPtr Factory::CreateProgram() {
        return std::dynamic_pointer_cast<Program>(std::make_shared<ProgramImpl>());
    }
}

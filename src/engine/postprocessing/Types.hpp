//
// Created by Vali on 2/24/2021.
//

#ifndef VDGE_POSTPROCESSING_TYPES_HPP
#define VDGE_POSTPROCESSING_TYPES_HPP

#include <engine/api/gl/FrameBuffer.hpp>

namespace vd::postprocessing {
    typedef std::function<vd::gl::FrameBufferPtr()>     FrameBufferResolver;
    typedef std::vector<FrameBufferResolver>            FrameBufferResolvers;
    typedef std::function<vd::gl::FrameBufferPtrVec()>  FrameBuffersResolver;
    typedef FrameBufferResolver                         FrameBufferGenerator;
    typedef FrameBufferResolvers                        FrameBufferGenerators;

    typedef std::function<void(vd::gl::FrameBufferPtr&)>    FrameBufferConfigurator;
}

#endif //VDGE_POSTPROCESSING_TYPES_HPP

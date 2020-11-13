//
// Created by Vali on 11/11/2020.
//

#ifndef VD_GAME_ENGINE_GL_BUFFER_HPP
#define VD_GAME_ENGINE_GL_BUFFER_HPP

#include <engine/model/Mesh.hpp>

namespace vd::gl {
    class Buffer {
    public:
        virtual void Allocate(const vd::model::MeshPtr& meshPtr) = 0;
        virtual void Render() = 0;
        virtual void CleanUp() = 0;
    };
    typedef std::shared_ptr<Buffer>	BufferPtr;
    typedef std::vector<BufferPtr>	BufferPtrVec;
}

#endif //VD_GAME_ENGINE_GL_BUFFER_HPP
